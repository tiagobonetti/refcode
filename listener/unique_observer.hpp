#ifndef UNIQUE_OBSERVER_HPP_
#define UNIQUE_OBSERVER_HPP_

#include <unordered_set>
#include <functional>
#include <memory>

namespace unique {

template <typename... Args>
class Subject;

template <typename... Args>
class Observer {
   public:
    Observer(Subject<Args...>* subject,
             const std::function<void(Args...)> callback)
        : _subject(subject), _callback(callback) {}
    ~Observer() {
        if (_subject) {
            _subject->Detach(*this);
        }
    }
    void Update(Args... args) { _callback(args...); };
    void Detached() { _subject = nullptr; }
    bool IsAttached() const { return (_subject != nullptr); }

   private:
    Subject<Args...>* _subject;
    const std::function<void(Args...)> _callback;
};

template <typename... Args>
class Subject {
   public:
    ~Subject() {
        for (auto& observer : _observers) {
            observer->Detached();
        }
    }
    std::unique_ptr<const Observer<Args...>> Attach(
        const std::function<void(Args...)>& callback) {
        auto* observer = new Observer<Args...>(this, callback);
        _observers.insert(observer);
        return std::unique_ptr<const Observer<Args...>>(observer);
    }
    void Detach(Observer<Args...>& observer) { _observers.erase(&observer); }
    void Notify(Args... args) const {
        for (auto& observer : _observers) {
            observer->Update(args...);
        }
    }

   private:
    std::unordered_set<Observer<Args...>*> _observers;
};

}  // namespace unique

#endif  // UNIQUE_OBSERVER_HPP_
