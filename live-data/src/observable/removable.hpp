#pragma once

#include <cstddef>

namespace observable {

class removable {
   public:
    using key_type = std::size_t;
    virtual bool remove_observer(key_type) = 0;
};

}  // namespace observable
