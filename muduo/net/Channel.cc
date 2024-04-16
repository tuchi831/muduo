#include "muduo/base/Logging.h"
#include "muduo/net/Channel.h"
#include "muduo/net/EventLoop.h"

#include <sstream>
#include <iostream>
#include <poll.h>

using namespace muduo;
using namespace muduo::net;

const int Channel::kNoneEvent = 0;
const int Channel::kReadEvent = POLLIN | POLLPRI;
const int Channel::kWriteEvent = POLLOUT;

Channel::Channel(EventLoop * loop, int fd__)
  : loop_(loop),
    fd_(fd__),
    events_(0),
    revents_(0),
    index_(-1),
    logHup_(true),
    tied_(false),
    eventHandling_(false),
    addedToLoop_(false)
    {
     // events_|= kWriteEvent;
    }
  
Channel::~Channel()
{
  assert(!eventHandling_);
  assert(!addedToLoop_);
  if(loop_->isInLoopThread())
  {
    assert(!loop_->hasChannel(this));
  }
}

void Channel::tie(const std::shared_ptr<void>& obj)
{
  tie_ = obj;
  tied_ = true;
}

void Channel::update()
{
  //std::cout<<"update happened"<<std::endl;
  addedToLoop_ = true;
  
 //  this->enableWriting();
 //this->events_
  loop_->updateChannel(this);
}

void Channel::remove()
{
  assert(isNoneEvent());
  addedToLoop_ = false;
  loop_->removeChannel(this);
}

void Channel::handleEvent(Timestamp receiveTime)
{
  std::shared_ptr<void>guard;
  if(tied_)
  {
    guard = tie_.lock();
    if(guard)
    {
      handleEventWithGuard(receiveTime);
    }
  }
  else
  {
    handleEventWithGuard(receiveTime);
  }
}

void Channel::handleEventWithGuard(Timestamp receiveTime)
{
  eventHandling_ = true;
  LOG_TRACE << reventsToString();

  if((revents_ & POLLHUP) && !(revents_ & POLLIN))
  {
    // 发生挂起(即对端关闭连接 或者 对端关闭写入段)且没有可读事件
    if (logHup_)
    {
      LOG_WARN << "fd = " << fd_ << " Channel::handle_event() POLLHUP";
    }
    if (closeCallback_) closeCallback_();
  }

  if(revents_ & POLLNVAL)
  {
    // 访问到非法或者无效的文件描述符
    LOG_WARN << "fd = "<< fd_ <<" Channel::handle_event() POLLNVAL";
  }

  if(revents_ & (POLLERR | POLLNVAL))
  {
    if(errorCallback_) errorCallback_();
  }

  if(revents_ & (POLLIN | POLLPRI | POLLRDHUP))
  {
    // 有可读事件 高优先级的可读事件 对端关闭写入端
    if(readCallback_) readCallback_(receiveTime);
  }

  if(revents_ & POLLOUT)
  {
    if(writeCallback_)writeCallback_();
  }
  eventHandling_ = false;
}

string Channel::reventsToString()const
{
  return eventsToString(fd_,revents_);
}
string Channel::eventsToString()const
{
  return eventsToString(fd_,events_);
}

string Channel::eventsToString(int fd,int ev)
{
  std::stringstream oss;
  oss << fd << ": ";
  if( ev & POLLIN)
    oss << "IN ";
  if( ev & POLLPRI)
    oss << "PRI ";
  if( ev & POLLOUT)
    oss << "OUT ";
  if( ev & POLLHUP)
    oss << "HUP ";
  if( ev & POLLERR)
    oss << "ERR ";
  if( ev & POLLNVAL)
    oss << "NVAL ";

  return oss.str();
}
