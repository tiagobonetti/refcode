#pragma once

#include "observable/value.hpp"

namespace observable {

struct auto_token {
    removable* _removable = nullptr;
    token _token;

    ~auto_token() {
        if (_removable) {
            _removable->remove_observer(_token);
        }
    }
};

}  // namespace observable
