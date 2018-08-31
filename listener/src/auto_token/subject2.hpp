#pragma once

#include "auto_token/token.hpp"

#include <functional>
#include <memory>
#include <sstream>
#include <tuple>
//#include <unordered_map>
#include <algorithm>
#include <vector>

namespace autotoken {

template <typename... Args>
class subject2 : public removable {
   public:
    using handler_type = std::function<void(Args...)>;

    template <typename Handler>
    token observe(Handler&& handler);

    void remove_observer(std::size_t key) final;
    void notify(Args... args) const;

   private:
    //std::unordered_map<token::key_type, handler_type> _observers;

    struct entry {
        token::key_type key;
        handler_type handler;
    };

    std::vector<entry> _observers;
};

template <typename... Args>
template <typename Handler>
token subject2<Args...>::observe(Handler&& h) {
    auto key = token::unique_key();
    _observers.emplace_back(entry{key, std::forward<Handler>(h)});

    if (!(_observers.back().handler)) {
        _observers.pop_back();
        return {};
    }

    return token{*this, key};
}

template <typename... Args>
void subject2<Args...>::remove_observer(std::size_t key) {
    auto it = std::find_if(_observers.cbegin(), _observers.cend(),
                           [key](entry const& e) { return e.key == key; });

    if (it != _observers.cend()) {
        _observers.erase(it);
    };
}

template <typename... Args>
void subject2<Args...>::notify(Args... args) const {
    for (auto const& entry : _observers) {
        entry.handler(args...);
    }
}

}  // namespace autotoken
