#pragma once

#include <algorithm>
#include <functional>
#include <memory>
#include <vector>

namespace shared {

template <typename... Args>
class subject {
   public:
    using handler_type = std::function<void(Args...)>;
    using observer = std::shared_ptr<handler_type>;

    observer observe(handler_type&& h) {
        if (!h) {
            return nullptr;
        }

        auto observer = std::make_shared<handler_type>(h);
        _observers.push_back(std::weak_ptr<handler_type>(observer));
        return observer;
    }

    void notify(Args... args) {
        bool missed = false;

        for (auto& weak : _observers) {
            auto observer = weak.lock();
            if (observer) {
                (*observer)(args...);
            } else {
                missed = true;
            }
        }

        if (missed) {
            erase_expired();
        }
    }

   private:
    void erase_expired() {
        _observers.erase(
            std::remove_if(_observers.begin(), _observers.end(),
                           [](std::weak_ptr<handler_type> weak) { return weak.expired(); }));
    }

    std::vector<std::weak_ptr<handler_type>> _observers;
};

}  // namespace shared
