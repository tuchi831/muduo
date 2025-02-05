#ifndef MUDUO_NET_POLLER_EPOLLPOLLER_H
#define MUDUO_NET_POLLER_EPOLLPOLLER_H

#include "muduo/net/Poller.h"

#include <vector>

struct epoll_event;

namespace muduo
{
  namespace net
  {
    class EPollPoller : public Poller
    {
        public:
          EPollPoller(EventLoop* loop);
          ~EPollPoller() override;

          Timestamp poll(int timeoutMs, ChannelList* activeChannels) override;
          void updateChannel(Channel* channel) override;
          void removeChannel(Channel* channel) override;

          int epollfd_;
        private:
        
        static const int kInitEventListSize = 16;

        static const char* operationToString(int op);

        void fillActiveChannels(int numEvents,
                                ChannelList* activeChannels)const;

        void update(int operation,Channel* channel);

        typedef std::vector<struct epoll_event> EventList;

        EventList events_;
    };
  }
}

#endif