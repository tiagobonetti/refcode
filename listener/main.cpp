#include <iostream>
#include <string>
#include "unique_observer.hpp"
#include "shared_observer.hpp"
#include "timeval.hpp"

int main(int argc, char* argv[]) {
    constexpr std::size_t scount = 1000000;
    constexpr std::size_t ocount = 1;

    Timeval start, build, call, end;
    std::size_t dcall;

    gettimeofday(&start, nullptr);
    {
        std::vector<unique::Subject<>> subjects(scount);
        std::vector<std::unique_ptr<const unique::Observer<>>> observers;
        for (auto& subject : subjects) {
            for (std::size_t i = 0; i < ocount; i++) {
                auto observer = subject.Attach([]() { /* noop */ });
                observers.push_back(std::move(observer));
            }
        }
        gettimeofday(&build, nullptr);

        for (auto& subject : subjects) {
            subject.Notify();
        }
        gettimeofday(&call, nullptr);
    }
    gettimeofday(&end, nullptr);

    dcall = ((call - build).tv_sec * 1000 * 1000) + (call - build).tv_usec;
    std::cout << "Unique Observer\n"                                    //
              << " Build   - " << (build - start) << '\n'               //
              << " Call    - " << (call - build) << '\n'                //
              << "  us= " << dcall << '/' << (scount * ocount) << '\n'  //
              << " Destroy - " << (end - call) << '\n';

    gettimeofday(&start, nullptr);
    {
        std::vector<shared::Subject<>> subjects(scount);
        std::vector<shared::Subject<>::Observer> observers;
        for (auto& subject : subjects) {
            for (std::size_t i = 0; i < ocount; i++) {
                observers.emplace_back(subject.Observe([]() {}));
            }
        }
        gettimeofday(&build, nullptr);

        for (auto& subject : subjects) {
            subject.Notify();
        }
        gettimeofday(&call, nullptr);
    }
    gettimeofday(&end, nullptr);
    dcall = ((call - build).tv_sec * 1000 * 1000) + (call - build).tv_usec;

    std::cout << "Shared Observer\n"
              << " Build   - " << (build - start) << '\n'               //
              << " Call    - " << (call - build) << '\n'                //
              << "  us= " << dcall << '/' << (scount * ocount) << '\n'  //
              << " Destroy - " << (end - call) << '\n';
}
