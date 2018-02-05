#include <iostream>
#include <string>

#include "shared_observer.hpp"
#include "token_observer.hpp"
#include "unique_observer.hpp"

#include "timeval.hpp"

void test_unique(std::size_t subjects_count, std::size_t observers_count) {
    Timeval start, build, call, end;
    std::size_t dcall;

    gettimeofday(&start, nullptr);
    {
        std::vector<unique::Subject<>> subjects(subjects_count);
        std::vector<std::unique_ptr<const unique::Observer<>>> observers;
        for (auto& subject : subjects) {
            for (std::size_t i = 0; i < observers_count; i++) {
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
    std::cout << " Unique Observer\n"                                                                 //
              << "  Build   - " << (build - start) << '\n'                                            //
              << "  Call    - " << (call - build)                                                     //
              << "  total(us/calls)= " << dcall << '/' << (subjects_count * observers_count) << '\n'  //
              << "  Destroy - " << (end - call) << '\n';
}

void test_shared(std::size_t subjects_count, std::size_t observers_count) {
    Timeval start, build, call, end;
    std::size_t dcall;

    gettimeofday(&start, nullptr);
    {
        std::vector<shared::Subject<>> subjects(subjects_count);
        std::vector<shared::Subject<>::Observer> observers;
        for (auto& subject : subjects) {
            for (std::size_t i = 0; i < observers_count; i++) {
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

    std::cout << " Shared Observer\n"
              << "  Build   - " << (build - start) << '\n'                                            //
              << "  Call    - " << (call - build)                                                     //
              << "  total(us/calls)= " << dcall << '/' << (subjects_count * observers_count) << '\n'  //
              << "  Destroy - " << (end - call) << '\n';
}

void test_token(std::size_t subjects_count, std::size_t observers_count) {
    Timeval start, build, call, end;
    std::size_t dcall;

    gettimeofday(&start, nullptr);
    {
        std::vector<tokenobserver::subject<>> subjects(subjects_count);

        using token_vector = std::vector<tokenobserver::token>;
        std::vector<token_vector> all_tokens;

        for (auto& subject : subjects) {
            token_vector v;
            for (std::size_t i = 0; i < observers_count; i++) {
                v.emplace_back(subject.observe([]() {}));
            }
            all_tokens.push_back(std::move(v));
        }
        gettimeofday(&build, nullptr);

        for (auto& subject : subjects) {
            subject.notify();
        }
        gettimeofday(&call, nullptr);

        auto tokens_it = all_tokens.cbegin();
        for (auto& subject : subjects) {
            auto& tokens = *tokens_it++;
            for (auto& t : tokens) {
                subject.remove_observer(t);
            }
        }
    }
    gettimeofday(&end, nullptr);

    dcall = ((call - build).tv_sec * 1000 * 1000) + (call - build).tv_usec;

    std::cout << " Token Observer\n"
              << "  Build   - " << (build - start) << '\n'                                            //
              << "  Call    - " << (call - build)                                                     //
              << "  total(us/calls)= " << dcall << '/' << (subjects_count * observers_count) << '\n'  //
              << "  Destroy - " << (end - call) << '\n';
}

void test(std::size_t subjects_count, std::size_t observers_count) {
    std::cout << "Test parameters: " << subjects_count << " subjects, " << observers_count << " observers\n";
    test_unique(subjects_count, observers_count);
    test_shared(subjects_count, observers_count);
    test_token(subjects_count, observers_count);
    std::cout << '\n';
}

int main(int argc, char* argv[]) {
    test(1000000, 1);
    test(1, 1000000);
    test(1000, 1000);
}
