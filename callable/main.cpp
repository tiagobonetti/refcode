#include <iostream>

struct my_callback {
    int count = 0;
    void method() {
        std::cout << "my_callback::method: count=" << count << '\n';
        count++;
    }
};

template <typename T>
struct Delegate {
    using callback_type = void (T::*)();
    using member_type = T*;

    void set_stuff_callback(callback_type cb, member_type data) {
        _callback = cb;
        _data = data;
    }

    void invoke() {
        (_data->*_callback)();
    }

   private:
    callback_type _callback;
    member_type _data;
};

int main() {
    my_callback cb;

    Delegate<my_callback> s;
    s.set_stuff_callback(&my_callback::method, &cb);
    s.invoke();
    s.invoke();
    s.invoke();
    s.invoke();
    s.invoke();
    s.invoke();
}
