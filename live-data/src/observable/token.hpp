#pragma once

#include "observable/removable.hpp"

#include <cstddef>
#include <sstream>

namespace observable {

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

}  // namespace observable
