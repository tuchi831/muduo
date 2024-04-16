#ifndef __STDC_LIMIT_MACROS
#define __STDC_LIMIT_MACROS
#endif

#include "muduo/net/TimerQueue.h"
#include "muduo/base/Logging.h"
#include "muduo/net/EventLoop.h"
#include "muduo/net/Timer.h"
#include "muduo/net/TimerId.h"


#include <sys/timerfd.h>
#include <unistd.h>
#include <iostream>

namespace muduo
{
  namespace net
  {
    namespace detail
    {
        int createTimerfd()
        {
          // 创建一个文件描述符 以非阻塞的形式 使用exec的话就关闭这个文件秒护肤
          int timerfd = ::timerfd_create(CLOCK_MONOTONIC,
                                        TFD_NONBLOCK | TFD_CLOEXEC);
          if(timerfd < 0)
          {
            LOG_SYSFATAL << "Failed in timerfd_create";
          }
          return timerfd;
        }

        struct timespec howMuchTimeFromNow(Timestamp when)
        {
          int64_t microseconds = when.microSecondsSinceEpoch()
                                - Timestamp::now().microSecondsSinceEpoch();
          if (microseconds < 100)
          {
            microseconds = 100;
          }
          struct timespec ts;
          ts.tv_sec = static_cast<time_t>(
              microseconds / Timestamp::kMicroSecondsPerSecond);
          ts.tv_nsec = static_cast<long>(
              (microseconds % Timestamp::kMicroSecondsPerSecond) * 1000);
          return ts;
        }

        void readTimerfd(int timerfd,Timestamp now)
        {
          uint64_t howmany;
          ssize_t n = ::read( timerfd, &howmany ,sizeof howmany);
          LOG_TRACE << "TimerQueue::handleRead() " << howmany << " at " << now.toString();
          if (n != sizeof howmany)
          {
            LOG_ERROR << "TimerQueue::handleRead() reads " << n << " bytes instead of 8";
          }
        }

        void resetTimerfd(int timerfd, Timestamp expiration)
        {
          // 记录超时时间和间隔
          struct itimerspec newValue ;
          struct itimerspec oldValue ;
          memZero(&newValue,sizeof newValue);
          memZero(&oldValue,sizeof oldValue);

          newValue.it_value = howMuchTimeFromNow(expiration);

          //标志位为0 表示使用的是相对时间 oldValue用来记录上一个状态
          int ret = ::timerfd_settime(timerfd, 0, &newValue, &oldValue);
          if (ret)
          {
            LOG_SYSERR << "timerfd_settime()";
          }

        }
    }
  }
}

using namespace muduo;
using namespace muduo::net;
using namespace muduo::net::detail;

TimerQueue::TimerQueue(EventLoop* loop)
  : loop_(loop),
    timerfd_(createTimerfd()),
    timerfdChannel_(loop,timerfd_),
    timers_(),
    callingExpiredTimers_(false)
{
  timerfdChannel_.setReadCallback(
      std::bind(&TimerQueue::handleRead,this)
  );
  timerfdChannel_.enableReading();
}

TimerQueue::~TimerQueue()
{
  timerfdChannel_.disableAll();
  timerfdChannel_.remove();
  ::close(timerfd_);

  for(const Entry& timer : timers_)
  {
    delete timer.second;
  }
}

TimerId TimerQueue::addTimer(TimerCallback cb,Timestamp when, double interval)
{
  Timer * timer = new Timer(std::move(cb),when,interval);
  loop_->runInLoop(
    std::bind(&TimerQueue::addTimerInLoop,this,timer)
  );
  return TimerId(timer,timer->sequence());
}

void TimerQueue::cancel(TimerId timerId)
{
  loop_->runInLoop(
    std::bind(&TimerQueue::cancelInLoop,this,timerId)
  );
}

void TimerQueue::addTimerInLoop(Timer * timer)
{
  loop_->assertInLoopThread();
  bool earliestChanged = insert(timer);

  if(earliestChanged)
  {
    resetTimerfd(timerfd_,timer->expiration());
  }
}

void TimerQueue::cancelInLoop(TimerId timerId)
{
  loop_->assertInLoopThread();
  assert(timers_.size() == activeTimers_.size());
  ActiveTimer timer(timerId.timer_,timerId.sequence_);

  ActiveTimerSet::iterator it = activeTimers_.find(timer);

  if(it != activeTimers_.end())
  {
    size_t n = timers_.erase(Entry(it->first->expiration(),it->first));
    assert( n == 1); (void) n;
    delete it->first;
    activeTimers_.erase(it);
  }
  else if(callingExpiredTimers_)
  {
    cancelingTimers_.insert(timer);
  }
  assert(timers_.size() == activeTimers_.size());
}

void TimerQueue::handleRead()
{ 
  loop_->assertInLoopThread();

  Timestamp now(Timestamp::now());

  readTimerfd(timerfd_,now);

  std::vector<Entry> expired = getExpired(now);

  callingExpiredTimers_ = true;
  cancelingTimers_.clear();

  for(const Entry& it : expired)
  {
    it.second->run();
  }
  callingExpiredTimers_ = false;

  reset(expired,now);
}

std::vector<TimerQueue::Entry>TimerQueue::getExpired(Timestamp now)
{
  assert(timers_.size() == activeTimers_.size());

  std::vector<Entry>expired;
  Entry sentry(now, reinterpret_cast<Timer*>(UINTPTR_MAX));

  TimerList::iterator end = timers_.lower_bound(sentry);

  assert(end == timers_.end() || now < end->first);

  std::copy(timers_.begin(), end , back_inserter(expired));

  timers_.erase(timers_.begin(),end);

  for(const Entry& it : expired)
  {
    ActiveTimer timer(it.second,it.second->sequence());
    size_t n = activeTimers_.erase(timer);
    assert(n == 1); (void)n;
  }
  assert(timers_.size() == activeTimers_.size());
  return expired;
}

void TimerQueue::reset(const std::vector<Entry>&expired,Timestamp now)
{
  Timestamp nextExpire;

  for( const Entry & it : expired)
  {
    ActiveTimer timer(it.second,it.second->sequence());

    if(it.second->repeat() 
       && cancelingTimers_.find(timer) == cancelingTimers_.end())
      {
        it.second->restart(now);
        insert(it.second);
      }
    else
    {
      delete it.second;
    }
  }

  if(!timers_.empty())
  {
    nextExpire = timers_.begin()->second->expiration();
  }
  if(nextExpire.valid())
  {
    resetTimerfd(timerfd_,nextExpire);
  }
}

bool TimerQueue::insert(Timer * timer)
{
  loop_->assertInLoopThread();
  assert(timer_.size() == activeTimers_.size());

  bool earliestChanged = false;
  Timestamp when = timer->expiration();

  TimerList::iterator it = timers_.begin();

  if(it == timers_.end() || when < it->first)
  {
    earliestChanged = true;
  }
  
  {
    std::pair<TimerList::iterator, bool> result
      = timers_.insert(Entry(when, timer));
    assert(result.second); (void)result;
  }

  {
    std::pair<ActiveTimerSet::iterator,bool>result = activeTimers_.insert(ActiveTimer(timer,timer->sequence()));
    assert(result.second); (void)result;
  }

  assert(timers_.size() ==  activeTimers_.size());

  return earliestChanged;
}

