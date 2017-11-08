#include <iostream>
#include <vector>
#include <string>
#include <utility>
#include <ostream>
#include <functional>
#include <algorithm>

enum class ExampleEnumeration { A,
                                B,
                                C };

std::ostream& operator<<(std::ostream& os, const ExampleEnumeration& e) {
    switch (e) {
        case ExampleEnumeration::A:
            return os << "A";
        case ExampleEnumeration::B:
            return os << "B";
        case ExampleEnumeration::C:
            return os << "C";
    }
}

template <typename Enum>
class Combo {
    using Option = std::pair<Enum, std::string>;

   public:
    Combo(const std::vector<Option> v)
        : options_(v), current_(options_.begin()), selected_(options_.begin()) {
        std::cout << Current().first << "," << Current().second << '\n';
        Prev();
        std::cout << Current().first << "," << Current().second << '\n';
        Next();
        std::cout << Current().first << "," << Current().second << '\n';
    }

    void SetCurrent(Enum e) {
        current_ = std::find(
            options_.begin(), options_.end(),
            [e](const Option*& option) { return (option->first == e); });
    }
    void SetSelected(Enum e) {
        selected_ = std::find(
            options_.begin(), options_.end(),
            [e](const Option*& option) { return (option->first == e); });
    }

    void Select() {
        // show
    }
    void Next() {
        current_ = std::next(current_);
        if (current_ == options_.end()) {
            current_ = options_.begin();
        }
        // show
    }
    void Prev() {
        if (current_ == options_.begin()) {
            current_ = options_.end();
        }
        current_ = std::prev(current_);
        // show
    }

   private:
    const Option& Current() const { return *current_; }
    void UpdateDisplay() const {}

    const std::vector<Option> options_;
    typename std::vector<Option>::const_iterator current_;
    typename std::vector<Option>::const_iterator selected_;
};

int main() {
    Combo<ExampleEnumeration> combo({{ExampleEnumeration::A, "a"},  //
                                     {ExampleEnumeration::B, "b"},  //
                                     {ExampleEnumeration::C, "c"}});
}
