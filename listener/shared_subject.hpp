#ifndef SHARED_SUBJECT_HPP_
#define SHARED_SUBJECT_HPP_

#include <vector>
#include <algorithm>
#include <functional>
#include <memory>

namespace shared2 {

template <typename... Args>
using Observer = std::unique_ptr<internal::Subject<Args...>::Observer>;

template <typename... Args>
using Subject = std::shared_ptr<internal::Subject<Args...>>;

namespace internal {

template <typename T, typename... Args>
std::unique_ptr<T> make_unique(Args&&... args) {
    return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
}

template <typename... Args>
class Subject : std::enable_shared_from_this<Subject> {
   public:
    //
    typedef std::function<void(Args...)> Callback;
    //
    class Observer {
       public:
        Observer(const std::shared_ptr<Subject>& subject, Callback&& callback)
            : _subject(subject), _callback(callback) {}
        ~Observer() {
            auto s = _subject.lock();
            if (s != nullptr) {
                _subject->Detach(*this);
            }
        }
        void Notify(Args... args) {
            if (_callback) {
                _callback(args...);
            }
        };

       private:
        Callback _callback;
        std::weak_ptr<Subject> _subject;
    };

    std::unique_ptr<Observer> Attach(Callback&& callback) {
        auto observer = make_unique<Observer>(shared_from_this(), callback);
        _observers.emplace_back(*observer);
        return observer;
    }
    void Detach(const Observer& observer) { _observers.erase(observer); }

    void Notify(Args... args) {
        for (auto observer : _observers) {
            observer.Notify(args...);
        }
    }

   private:
    std::unordered_set<std::reference_wrapper<Observer>> _observers;
};

}  // namespace internal
}  // namespace shared

#endif  // SHARED_SUBJECT_HPP_
