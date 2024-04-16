#ifndef MUDUO_NET_ACCEPTOR_H
#define MUDUO_NET_ACCEPTOR_H

#include <functional>

#include "muduo/net/Channel.h"
#include "muduo/net/Socket.h"

namespace muduo
{
  namespace net
  {
    class EventLoop;
    class InetAddress;

    class Acceptor : noncopyable
    {
      public:
        typedef std::function<void(int sockfd, const InetAddress&)> NewConnectionCallback;

        Acceptor(EventLoop* loop,const InetAddress & listenAddr,bool reuseport );
        ~Acceptor();

        void setNewConnectionCallback(const NewConnectionCallback& cb)
        {
          newConnectionCallback_ = cb;
        }
        void listen();
        bool listening()const
        {
          return listening_;
        }

        private:
          void handleRead();
          EventLoop * loop_;
          Socket acceptSocket_;
          Channel acceptChannel_;
          NewConnectionCallback newConnectionCallback_;
          bool listening_;
          int idleFd_;
    };
  }
}

#endif