#pragma once

#include <algorithm>
#include <functional>
#include <memory>
#include <vector>

namespace shared2 {

template <typename... Args>
using observer = std::unique_ptr<internal::subject<Args...>::observer>;

template <typename... Args>
using subject = std::shared_ptr<internal::subject<Args...>>;

namespace internal {

template <typename T, typename... Args>
std::unique_ptr<T> make_unique(Args&&... args) {
    return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
}

template <typename... Args>
class subject : std::enable_shared_from_this<subject> {
   public:
    //
    typedef std::function<void(Args...)> callback;
    //
    class observer {
       public:
        observer(const std::shared_ptr<subject>& subject, callback&& callback)
            : _subject(subject), _callback(callback) {}
        ~observer() {
            auto s = _subject.lock();
            if (s != nullptr) {
                _subject->detach(*this);
            }
        }
        void notify(Args... args) {
            if (_callback) {
                _callback(args...);
            }
        };

       private:
        callback _callback;
        std::weak_ptr<subject> _subject;
    };

    std::unique_ptr<observer> attach(callback&& callback) {
        auto observer = make_unique<observer>(shared_from_this(), callback);
        _observers.emplace_back(*observer);
        return observer;
    }
    void detach(const observer& observer) { _observers.erase(observer); }

    void notify(Args... args) {
        for (auto observer : _observers) {
            observer.notify(args...);
        }
    }

   private:
    std::unordered_set<std::reference_wrapper<observer>> _observers;
};

}  // namespace internal
}  // namespace shared2
