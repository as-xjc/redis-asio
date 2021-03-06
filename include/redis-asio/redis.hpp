#pragma once

#include <memory>
#include <hiredis/hiredis.h>
#include <boost/utility/string_view.hpp>

namespace redis_asio {

/**
 * hiredis 的封装类
 */
class Redis final {
 public:
  Redis();
  virtual ~Redis();

  /// 连接redis
  bool Connect(const char* address, uint16_t port);

  /// 执行命令，等同于 redisCommand
  std::shared_ptr<redisReply> Command(const char *format, ...);

  /// ping redis，如果连接断开，会尝试重连，失败返回false
  bool Ping();

  explicit operator bool() const;

  /// 授权认证
  bool Auth(boost::string_view view);

 private:
  std::unique_ptr<redisContext, decltype(&redisFree)> redis_;
};

}