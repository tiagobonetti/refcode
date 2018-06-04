#pragma once

#include "subject.hpp"

#include <cstddef>
#include <sstream>

namespace observable {

template <typename T>
class value {
   public:
    using subject_type = subject<T>;
    using handler_type = typename subject_type::handler_type;

    value(T&& v) : _value(std::forward<T>(v)) {}

    T const& get_value() {
        return _value;
    }

    value<T>& set_value(T&& v) {
        decltype(_value) new_value = std::forward<T>(v);

        if (_value != new_value) {
            _value = new_value;
            notify();
        }

        return *this;
    }

    token observe(handler_type cb) {
        if (!cb) {
            return {};
        }

        cb(_value);
        return _subject.observe(cb);
    }

    bool remove_observer(token t) { return _subject.remove_observer(t); }
    void notify() const { _subject.notify(_value); }

   private:
    T _value;
    subject_type _subject;
};

}  // namespace observable
