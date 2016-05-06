#include <fstream>
#include <iostream>

int main()
{
    std::ifstream read("int.txt");
    int id;
    read >> id;
    std::cout << "ID: " << id << "\n";
}
