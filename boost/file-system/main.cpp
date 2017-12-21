#include <boost/filesystem.hpp>
#include <iostream>

namespace fs = boost::filesystem;

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cout << "Usage: boost-fs path\n";
        return 1;
    }

    fs::path p(argv[1]);  // p reads clearer than argv[1] in the following code

    // does p actually exist?
    if (fs::exists(p)) {
        // is p a regular file?
        if (fs::is_regular_file(p)) {
            std::cout << p << " size is " << fs::file_size(p) << '\n';

            // is p a directory?
        } else if (fs::is_directory(p)) {
            std::cout << p << " is a directory\n";
        } else {
            std::cout << p << " exists, but is neither a regular file nor a directory\n";
        }
    } else {
        std::cout << p << " does not exist\n";
    }

    return 0;
}
