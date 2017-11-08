#ifndef SHARED_OBSERVER_HPP_
#define SHARED_OBSERVER_HPP_

#include <vector>
#include <algorithm>
#include <functional>
#include <memory>

namespace shared {

template <typename... Args>
class Subject {
   public:
    typedef std::function<void(Args...)> Callback;
    typedef std::shared_ptr<Callback> Observer;

    Observer Observe(Callback&& callback) {
        auto observer = std::make_shared<Callback>(callback);
        _observers.push_back(std::weak_ptr<Callback>(observer));
        return observer;
    }
    void Notify(Args... args) {
        for (auto& weak : _observers) {
            auto observer = weak.lock();
            if (observer != nullptr && *observer != nullptr) {
                (*observer)(args...);
            }
        }
        _observers.erase(std::remove_if(
            _observers.begin(), _observers.end(),
            [](std::weak_ptr<Callback> weak) { return weak.expired(); }));
    }

   private:
    std::vector<std::weak_ptr<Callback>> _observers;
};

}  // namespace shared

#endif  // SHARED_OBSERVER_HPP_
