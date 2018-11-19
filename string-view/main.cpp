#include <iostream>
#include <iterator>
#include <string>
#include <string_view>

int main() {
    constexpr auto word = std::string_view{"string"};
    std::cout << "word : \"" << word
              << "\" .size: " << word.size() << '\n';

    auto text = std::string("this is a string with text");
    std::cout << "text : \"" << text
              << "\" .size: " << text.size() << '\n';

    auto pos = text.find(word);
    std::cout << "pos  : " << pos << '\n';

    {
        auto view = std::string_view(&text[pos], std::size(word));
        std::cout << "view : \"" << view
                  << "\" .size: " << view.size() << '\n';

        auto equal = view.compare(word) == 0;
        std::cout << "equal: " << std::boolalpha << equal << '\n';
    }
    {
        auto str = std::string(&text[pos], std::size(word));
        std::cout << "str  : \"" << str
                  << "\" .size: " << str.size() << '\n';

        auto equal = str.compare(word) == 0;
        std::cout << "equal: " << std::boolalpha << equal << '\n';
    }
}
