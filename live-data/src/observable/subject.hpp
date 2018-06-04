#pragma once

#include "token.hpp"

#include <functional>
#include <memory>
#include <tuple>
#include <map>

namespace observable {

template <typename... Args>
class subject {
   public:
    using handler_type = std::function<void(Args...)>;

    token observe(handler_type cb) {
        typename decltype(_observers)::const_iterator it;
        bool inserted;
        std::tie(it, inserted) = _observers.insert(std::make_pair(token::unique(), cb));
        return inserted ? it->first : token{};
    }

    bool remove_observer(token t) { return _observers.erase(t) > 0; }

    void notify(Args... args) const {
        for (auto& entry : _observers) {
            entry.second(args...);
        }
    }

   private:
    std::map<token, handler_type> _observers;
};

}  // namespace observable
