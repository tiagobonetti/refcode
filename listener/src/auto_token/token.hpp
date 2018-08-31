#pragma once

#include <cstddef>

namespace autotoken {

class removable {
   public:
    using key_type = std::size_t;
    virtual void remove_observer(key_type) = 0;
};

class token {
   public:
    using key_type = removable::key_type;

    token();
    token(removable& r, key_type k);

    token(const token&) = delete;
    token& operator=(const token&) = delete;

    token(token&&);
    token& operator=(token&&);

    ~token();

    static key_type unique_key();

   private:
    removable* _removable = nullptr;
    key_type _key = 0;

    static key_type _count;
};

}  // namespace autotoken
