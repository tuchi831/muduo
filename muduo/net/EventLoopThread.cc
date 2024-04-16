#include "muduo/net/EventLoopThread.h"
#include "muduo/net/EventLoop.h"

#include <iostream>

using namespace muduo;
using namespace muduo::net;

EventLoopThread::EventLoopThread(const ThreadInitCallback & cb,
                                 const string& name)
    : loop_(NULL),
      exiting_(false),
      thread_(std::bind(&EventLoopThread::threadFunc,this),name),
      mutex_(),
      cond_(mutex_),
      callback_(cb)
      {
      }

EventLoopThread::~EventLoopThread()
{
  exiting_ = true;
  if(loop_ != NULL)
  {
    // ��������quit һ�㶼�ǳ������������ʱ��quit ���ʱ���������еĳ���Ӧ�����Ѿ�������
    loop_->quit();
    thread_.join();
  }
}

EventLoop* EventLoopThread::startLoop()
{
  assert(!thread_.started());
  thread_.start();

  EventLoop* loop = NULL;
  {
    MutexLockGuard lock(mutex_);
    while(loop_ == NULL )
    {
      //ʹ��cond_.wait֮������߳̽���ȴ� Ȼ��lock����mutex_��
      cond_.wait();
    }
    loop = loop_;
  }
  return loop;
}

void EventLoopThread::threadFunc()
{
  EventLoop loop;

  if(callback_)
  {
    callback_(&loop);
  }

  {
    MutexLockGuard lock(mutex_);
    loop_ = &loop;
    cond_.notify();
  }

  loop.loop();

  MutexLockGuard lock(mutex_);
  loop_ = NULL;

}