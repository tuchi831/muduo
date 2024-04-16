#ifndef MUDUO_NET_EVENTLOOP_H 
#define MUDUO_NET_EVENTLOOP_H

#include <atomic>
#include <functional>
#include <vector>

#include <boost/any.hpp>
#include <iostream>

#include "muduo/base/Mutex.h"
#include "muduo/base/CurrentThread.h"
#include "muduo/base/Timestamp.h"
#include "muduo/net/Callbacks.h"
#include "muduo/net/TimerId.h"

namespace muduo
{
  namespace net
  {
    class Channel;
    class Poller;
    class TimerQueue;

    class EventLoop : noncopyable
    {
      public:
        typedef std::function<void()> Functor;

        EventLoop();
        ~EventLoop();

        void loop();
        void quit();

        Timestamp pollReturnTime() const{ return pollReturnTime_; }
        int64_t iteration() const { return iteration_; }

        // 如果是跨线程runInLoop的话 就会放到对应的线程的queue中 等待那个线程自己执行 否则自身就直接执行
        // runInLoop是一个主线程可以对其他线程的操作 但是真正执行操作的还是对应的那个线程

        void runInLoop(Functor cb);
        void queueInLoop(Functor cb);

        size_t queueSize()const;

        TimerId runAt(Timestamp time,TimerCallback cb);

        TimerId runAfter(double delay,TimerCallback cb);

        TimerId runEvery(double interval,TimerCallback cb);

        void cancel(TimerId timerId);

        void wakeup();
        void updateChannel(Channel * channel);
        void removeChannel(Channel * channel);

        bool hasChannel(Channel * channel);

        pid_t threadId()const { return threadId_ ; }

        void assertInLoopThread()
        {
          if(!isInLoopThread())
          {
            abortNotInLoopThread();
          }
        }

        bool isInLoopThread()const{
            return threadId_ == CurrentThread::tid();
        }

        bool eventHandling()const{
            return eventHandling_;
        }

        void setContext(const boost::any & context)
        {
          context_ = context;
        }
        const boost::any& getContext()const
        {
          return context_;
        }
        boost::any* getMutableContext()
        {
          return &context_;
        }
        static EventLoop* getEventLoopOfCurrentThread();

      private:
        void abortNotInLoopThread();
        
        void handleRead(); // 用来进行wake up操作的
        
        void doPendingFunctors();  

        void printActiveChannels()const;

        typedef std::vector<Channel*>ChannelList;

        bool looping_;
        std::atomic<bool>quit_;
        bool eventHandling_;
        bool callingPendingFunctors_;
        int64_t iteration_;
        const pid_t threadId_;
        Timestamp pollReturnTime_;
        std::unique_ptr<Poller> poller_;
        std::unique_ptr<TimerQueue> timerQueue_;

        int wakeupFd_;

        std::unique_ptr<Channel>wakeupChannel_;
        boost::any context_;

        ChannelList activeChannels_;
        Channel* currentActiveChannel_;

        mutable MutexLock mutex_;
        std::vector<Functor>pendingFunctors_ GUARDED_BY(mutex_);

    };
  }
}

#endif 