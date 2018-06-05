#pragma once

#include <functional>
#include <memory>
#include <tuple>
#include <unordered_map>

namespace valuetoken {

struct token {
    using value_type = std::size_t;

    static token unique() { return token{++_count}; }
    static token null() { return token{0}; }

    bool operator<(token const& rhs) const { return value < rhs.value; };
    bool operator==(token const& rhs) const { return value == rhs.value; };

    value_type value;

   private:
    static value_type _count;
};

template <typename... Args>
class subject {
   public:
    using observer_type = std::function<void(Args...)>;

    token observe(observer_type cb) {
        typename decltype(_observers)::iterator it;
        bool inserted;
        std::tie(it, inserted) = _observers.insert(std::make_pair(token::unique(), cb));
        if (inserted) {
            return it->first;
        } else {
            return token::null();
        }
    }

    bool remove_observer(token t) { return _observers.erase(t) > 0; }

    void notify(Args... args) const {
        for (auto const& entry : _observers) {
            entry.second(args...);
        }
    }

   private:
    std::unordered_map<token, observer_type> _observers;
};

}  // namespace valuetoken

namespace std {
template <>
struct hash<valuetoken::token> {
    constexpr std::size_t operator()(valuetoken::token const& t) const { return t.value; }
};
}  // namespace std
