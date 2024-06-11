#include <iostream>
#include <string>
using namespace std::string_literals;

struct Address {
    std::string city;
    std::string street;
    unsigned int house_number;
    unsigned int apt_number;
    std::string index;
};

void PrintAddr(const Address& a) {
    std::cout << "City: "s << a.city <<'\n';
    std::cout << "Street: "s << a.street <<'\n';
    std::cout << "House #: "s << a.house_number <<'\n';
    std::cout << "Apartment #: "s << a.apt_number <<'\n';
    std::cout << "Index: "s << a.index <<'\n';
}

int main() {
    Address addr = {"Barrie"s,"Big Bay Point rd"s,555,1,"L4N 3Z7"s};
    PrintAddr(addr);
    return 0;
}