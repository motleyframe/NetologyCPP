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

std::ostream& operator<<(std::ostream& os, const Address& a) {
    os << "City: "s << a.city <<'\n';
    os << "Street: "s << a.street <<'\n';
    os << "House #: "s << a.house_number <<'\n';
    os << "Apartment #: "s << a.apt_number <<'\n';
    os << "Index: "s << a.index <<'\n';

    return os;
}

int main() {
    Address addr = {"Barrie"s,"Big Bay Point rd"s,555,1,"L4N 3Z7"s};
    std::cout<<addr<<std::endl;

    return 0;
}
