#pragma once

#include <cstddef>
#include <sstream>

struct token {
    using value_type = std::size_t;

    token() {}
    token(const token& other) = default;
    token& operator=(const token& other) = default;
    token(token&& other) = default;
    token& operator=(token&& other) = default;

    operator bool() { return _value != 0; }
    bool operator<(token const& rhs) const { return _value < rhs._value; };
    bool operator==(token const& rhs) const { return _value == rhs._value; };

    static token unique() { return token{++_count}; }

    friend std::ostream& operator<<(std::ostream& os, const token& k);

   private:
    token(value_type value) : _value(value) {}

    value_type _value = 0;
    static value_type _count;
};

std::ostream& operator<<(std::ostream& os, const token& k);