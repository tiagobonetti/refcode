#include <dbg.h>

int main() {
    constexpr bool yes = true;
    constexpr bool no = false;
    constexpr int one = 1;
    constexpr int two = 2;
    return dbg(one < two && yes && !no) ? 0 : -1;
}
