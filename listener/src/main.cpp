#include <iostream>
#include <string>

#include "auto_token/subject.hpp"
#include "shared/observer.hpp"
#include "token/subject.hpp"
#include "unique/subject.hpp"

#include "timeval.hpp"

void test_unique(std::size_t subjects_count, std::size_t observers_count) {
    Timeval start, build, call, end;
    std::size_t dcall;

    gettimeofday(&start, nullptr);
    {
        std::vector<unique::subject<>> subjects(subjects_count);
        std::vector<std::unique_ptr<const unique::observer<>>> observers;
        for (auto& subject : subjects) {
            for (std::size_t i = 0; i < observers_count; i++) {
                auto observer = subject.attach([]() { /* noop */ });
                observers.push_back(std::move(observer));
            }
        }
        gettimeofday(&build, nullptr);

        for (auto& subject : subjects) {
            subject.notify();
        }
        gettimeofday(&call, nullptr);
    }
    gettimeofday(&end, nullptr);

    dcall = ((call - build).tv_sec * 1000 * 1000) + (call - build).tv_usec;
    std::cout << " Unique observer\n"                                                                 //
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
        std::vector<shared::subject<>> subjects(subjects_count);
        std::vector<shared::subject<>::observer> observers;
        for (auto& subject : subjects) {
            for (std::size_t i = 0; i < observers_count; i++) {
                observers.emplace_back(subject.observe([]() {}));
            }
        }
        gettimeofday(&build, nullptr);

        for (auto& subject : subjects) {
            subject.notify();
        }
        gettimeofday(&call, nullptr);
    }
    gettimeofday(&end, nullptr);
    dcall = ((call - build).tv_sec * 1000 * 1000) + (call - build).tv_usec;

    std::cout << " Shared observer\n"
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

    std::cout << " Token observer\n"
              << "  Build   - " << (build - start) << '\n'                                            //
              << "  Call    - " << (call - build)                                                     //
              << "  total(us/calls)= " << dcall << '/' << (subjects_count * observers_count) << '\n'  //
              << "  Destroy - " << (end - call) << '\n';
}

void test_autotoken(std::size_t subjects_count, std::size_t observers_count) {
    Timeval start, build, call, end;
    std::size_t dcall;

    gettimeofday(&start, nullptr);
    {
        std::vector<autotokenobserver::subject<>> subjects(subjects_count);

        {
            std::vector<autotokenobserver::token> tokens;

            for (auto& subject : subjects) {
                for (std::size_t i = 0; i < observers_count; i++) {
                    tokens.emplace_back(subject.observe([]() {}));
                }
            }
            gettimeofday(&build, nullptr);

            for (auto& subject : subjects) {
                subject.notify();
            }

            gettimeofday(&call, nullptr);
            // destroy tokens
        }
        // destroy subjects
    }
    gettimeofday(&end, nullptr);

    dcall = ((call - build).tv_sec * 1000 * 1000) + (call - build).tv_usec;

    std::cout << " auto token observer observer\n"
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
    test_autotoken(subjects_count, observers_count);
    std::cout << '\n';
}

int main(int argc, char* argv[]) {
    test(1000000, 1);
    test(1, 1000000);
    test(1000, 1000);
}
