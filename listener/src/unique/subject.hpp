#pragma once

#include <functional>
#include <memory>
#include <unordered_set>

namespace unique {

template <typename... Args>
class subject;

template <typename... Args>
class observer {
   public:
    observer(subject<Args...>* subject, const std::function<void(Args...)> callback)
        : _subject(subject), _callback(callback) {}

    ~observer() {
        if (_subject) {
            _subject->detach(*this);
        }
    }

    void update(Args... args) { _callback(args...); };
    void detached() { _subject = nullptr; }
    bool is_attached() const { return (_subject != nullptr); }

   private:
    subject<Args...>* _subject;
    const std::function<void(Args...)> _callback;
};

template <typename... Args>
class subject {
   public:
    using handler_type = std::function<void(Args...)>;
    using unique_observer = std::unique_ptr<const observer<Args...>>;

    ~subject() {
        for (auto& observer : _observers) {
            observer->detached();
        }
    }

    unique_observer attach(handler_type h) {
        auto* observer_ = new observer<Args...>(this, h);
        _observers.insert(observer_);
        return std::unique_ptr<const observer<Args...>>(observer_);
    }

    void detach(observer<Args...>& observer) { _observers.erase(&observer); }

    void notify(Args... args) const {
        for (auto& observer : _observers) {
            observer->update(args...);
        }
    }

   private:
    std::unordered_set<observer<Args...>*> _observers;
};

}  // namespace unique
