#pragma once

#include <cstddef>
#include <functional>
#include <memory>
#include <sstream>
#include <tuple>
#include <unordered_map>

namespace autotoken {

class removable {
   public:
    using key_type = std::size_t;
    virtual bool remove_observer(key_type) = 0;
};

class token {
   public:
    using key_type = removable::key_type;

    token();
    token(removable& r, key_type k);

    token(const token&) = delete;
    token& operator=(const token&) = delete;

    token(token&&);
    token& operator=(token&&);

    ~token();

    static key_type unique_key();

   private:
    removable* _removable = nullptr;
    key_type _key = 0;

    static key_type _count;
};

template <typename... Args>
class subject : public removable {
   public:
    using handler_type = std::function<void(Args...)>;

    template <typename Handler>
    token observe(Handler&& handler);

    bool remove_observer(std::size_t key) final { return _observers.erase(key) > 0; }
    void notify(Args... args) const;

   private:
    std::unordered_map<token::key_type, handler_type> _observers;
};

template <typename... Args>
template <typename Handler>
token subject<Args...>::observe(Handler&& h) {
    handler_type handler{std::forward<Handler>(h)};
    if (!handler) {
        return {};
    }

    auto key = token::unique_key();

    typename decltype(_observers)::const_iterator it;
    bool inserted;
    std::tie(it, inserted) = _observers.emplace(key, std::move(handler));

    if (!inserted) {
        return {};
    }

    return token{*this, key};
}

template <typename... Args>
void subject<Args...>::notify(Args... args) const {
    for (auto const& entry : _observers) {
        entry.second(args...);
    }
}

}  // namespace autotoken
