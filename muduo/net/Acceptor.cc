#include "muduo/net/Acceptor.h"
#include "muduo/base/Logging.h"
#include "muduo/net/EventLoop.h"
#include "muduo/net/SocketsOps.h"
#include "muduo/net/InetAddress.h"

#include <error.h>
#include <fcntl.h>
#include <unistd.h>
#include <iostream>

using namespace muduo;
using namespace muduo::net;

Acceptor::Acceptor(EventLoop * loop, const InetAddress & listenAddr ,bool reuseport)
  : loop_(loop),
    acceptSocket_(sockets::createNonblockingOrDie(listenAddr.family())),
    acceptChannel_(loop,acceptSocket_.fd()),
    listening_(false),
    idleFd_(::open("/dev/null",O_RDONLY | O_CLOEXEC))
{
    assert(idleFd_ >= 0);
   // std::cout<<reuseport<<std::endl;
    reuseport = 1;
    acceptSocket_.setReuseAddr(true);
    acceptSocket_.setReusePort(reuseport);
    acceptSocket_.bindAddress(listenAddr);
    acceptChannel_.setReadCallback(
      std::bind(&Acceptor::handleRead,this)
    );
}

Acceptor::~Acceptor()
{
  acceptChannel_.disableAll();
  acceptChannel_.remove();
  ::close(idleFd_);
}

void Acceptor::listen()
{
  loop_->assertInLoopThread();
  listening_ = true;
  acceptSocket_.listen();
  acceptChannel_.enableReading();
}

void Acceptor::handleRead()
{
  loop_->assertInLoopThread();
  InetAddress peerAddr;

  int connfd = acceptSocket_.accept(&peerAddr);

  if(connfd >= 0)
  {
    if(newConnectionCallback_)
    {
      newConnectionCallback_(connfd,peerAddr);
    }
    else
    {
      sockets::close(connfd);
    }

  }
  
  else
  {
    LOG_SYSERR << "in Acceptor::handleRead";

    if (errno == EMFILE)
    {
      ::close(idleFd_);
      idleFd_ = ::accept(acceptSocket_.fd(), NULL, NULL);
      ::close(idleFd_);
      idleFd_ = ::open("/dev/null", O_RDONLY | O_CLOEXEC);
    }
  }

}