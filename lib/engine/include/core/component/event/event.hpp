#pragma once
#include <functional>
#include <cstdint>
#include <unordered_map>

template <typename...Args>
class EventDelegate {
public:
    using Callback = std::function<void(Args...)>;
    using Handle = uint32_t;

    Handle Subscribe( Callback cb) {
        Handle handle = next_handle++;
        callbacks.insert({handle, cb});
        return handle;
    }
    void Unsubscribe(Handle event_handle) {
      callbacks.erase(event_handle);
    }
    void Fire(Args... args) {
        for (const auto& [handle,cb] : callbacks) {
            if (cb) {
                cb(args...);
            }
        }
    }

private:
    std::unordered_map<Handle, Callback> callbacks;
    Handle next_handle = 0;
};
