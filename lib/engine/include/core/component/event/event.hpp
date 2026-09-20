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
        Handle handle = m_next_handle++;
        um_callbacks.insert({handle, cb});
        return handle;
    }
    void Unsubscribe(Handle event_handle) {
      um_callbacks.erase(event_handle);
    }
    void Fire(Args... args) {
        for (const auto& [handle,cb] : um_callbacks) {
            if (cb) {
                cb(args...);
            }
        }
    }

private:
    std::unordered_map<Handle, Callback> um_callbacks;
    Handle m_next_handle = 0;
};
