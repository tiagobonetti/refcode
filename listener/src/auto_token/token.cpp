#include "auto_token/subject.hpp"

namespace autotoken {

using key_type = token::key_type;

key_type token::_count = 0;

token::token() {}

token::token(removable& r, key_type k) : _removable(&r), _key(k) {}

token::token(token&& other) : _removable(other._removable), _key(other._key) {
    other._removable = nullptr;
    // XXX: invalidating only the pointer here
}

token& token::operator=(token&& other) {
    std::swap(_removable, other._removable);
    std::swap(_key, other._key);
    return *this;
}

token::~token() {
    if (_removable) {
        _removable->remove_observer(_key);
    }
}

key_type token::unique_key() { return ++_count; }

}  // namespace autotoken
