#include <iostream>

class Interface {
   public:
    virtual void f() = 0;
};

class Base {
   public:
    virtual void f()
    {
        std::cout << " Base::f()\n";
    }
    virtual void g()
    {
        std::cout << " Base::g()\n";
    }
    void h()
    {
        std::cout << " Base::h()\n";
    }
};

class Derived : public Base, public Interface {
   public:
    void f() override
    {
        std::cout << " Derived::f() --> ";
        Base::f();
    }
    void g() override
    {
        std::cout << " Derived::g()\n";
    }
    void h()
    {
        std::cout << " Base::h()\n";
    }
};

int main()
{
    Base b;
    Derived d;

    std::cout << "Direct calls\n";
    b.f();
    d.f();
    b.g();
    d.g();
    b.h();
    d.h();

    std::cout << "\n";

    std::cout << "Base calls\n";
    Base& bb = b;
    Base& bd = d;
    bb.f();
    bd.f();
    bb.g();
    bd.g();
    bb.h();
    bd.h();

    std::cout << "\n";

    std::cout << "Interface calls\n";
    Interface& id = d;
    id.f();
}
