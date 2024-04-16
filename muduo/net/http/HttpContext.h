#ifndef MUDUO_NET_HTTP_HTTPCONTEXT_H
#define MUDUO_NET_HTTP_HTTPCONTEXT_H

#include "muduo/base/copyable.h"
#include "muduo/net/http/HttpRequest.h"

namespace muduo
{
  namespace net
  {
    class Buffer;

    class HttpContext : public muduo:: copyable
    {
      public:
        enum HttpRequestParseState
        {
          kExpectRequestLine,
          kExpectHeaders,
          kExpectBody,
          kGotAll,
        };

        HttpContext(): state_(kExpectRequestLine)
        {
        }

        bool parseRequest(Buffer* buf, Timestamp receiveTime);

        bool gotAll() const
        { return state_ == kGotAll; }

        const HttpRequest& request() const
        { return request_; }

        HttpRequest& request()
        { return request_; }

        void reset()
        {
          state_ = kExpectRequestLine;
          HttpRequest dummy;
          request_.swap(dummy);
        }

        private:
          bool processRequestLine(const char* begin,const char* end);

          HttpRequestParseState state_;
          HttpRequest request_;
    };
  }
}

#endif