#pragma once

#include <algorithm>
#include <functional>
#include <memory>
#include <vector>

namespace shared {

template <typename... Args>
class subject {
   public:
    using callback = std::function<void(Args...)>;
    using observer = std::shared_ptr<callback>;

    observer observe(callback&& callback_) {
        auto observer = std::make_shared<callback>(callback_);
        _observers.push_back(std::weak_ptr<callback>(observer));
        return observer;
    }
    void notify(Args... args) {
        for (auto& weak : _observers) {
            auto observer = weak.lock();
            if (observer != nullptr && *observer != nullptr) {
                (*observer)(args...);
            }
        }
        _observers.erase(std::remove_if(
            _observers.begin(), _observers.end(),
            [](std::weak_ptr<callback> weak) { return weak.expired(); }));
    }

   private:
    std::vector<std::weak_ptr<callback>> _observers;
};

}  // namespace shared
