#include "redis-asio/asyncredis.hpp"

namespace redis_asio {

AsyncRedis::AsyncRedis(boost::asio::io_context& io): AsyncRedisIf(io) {}

AsyncRedis::~AsyncRedis() {}

uint32_t AsyncRedis::genId() {
  while (++id_ == 0) {}

  return id_;
}

void AsyncRedis::Command(CommandCallback cb, const char* format, ...) {
  if (!IsConnected()) return;

  uint32_t id = genId();
  commands_[id] = std::move(cb);

  auto data = reinterpret_cast<void*>(id);
  va_list ap;
  va_start(ap, format);
  AsyncCommand(data, format, ap);
  va_end(ap);
}

void AsyncRedis::Command(const char* format, ...) {
  if (!IsConnected()) return;

  va_list ap;
  va_start(ap, format);
  AsyncVCommand(nullptr, format, ap);
  va_end(ap);
}

void AsyncRedis::AsyncCommandCallback(redisReply* reply, void* data) {
  if (data == nullptr) return;

  auto id = static_cast<uint32_t>(reinterpret_cast<std::ptrdiff_t>(data));

  auto it = commands_.find(id);
  if (it != commands_.end()) {
    if (it->second) it->second(reply);

    commands_.erase(it);
  }
}

void AsyncRedis::Publish(boost::string_view channel, boost::string_view message) {
  AsyncCommand(nullptr, "PUBLISH %b %b", channel.data(), channel.size(), message.data(), message.size());
}

}