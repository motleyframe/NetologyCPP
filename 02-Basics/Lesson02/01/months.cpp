#include <iostream>
#include <string>
using namespace std::string_literals;

enum class Months {
    January = 1,
    February,
    March,
    April,
    May,
    June,
    July,
    August,
    September,
    October,
    November,
    December
};

std::string ShowMonth(const Months& m) {
    std::string month;

    switch(static_cast<unsigned short>(m)) {
        case 1: month = "January"s;
            break;
        case 2: month = "February"s;
            break;
        case 3: month = "March"s;
            break;
        case 4: month = "April"s;
            break;
        case 5: month = "May"s;
            break;
        case 6: month = "June"s;
            break;
        case 7: month = "July"s;
            break;
        case 8: month = "August"s;
            break;
        case 9: month = "September"s;
            break;
        case 10: month = "October"s;
            break;
        case 11: month = "November"s;
            break;
        case 12: month = "December"s;
            break;
        default: month = "No such month!"s;
            break;
    }
    return month;
}

std::istream& operator>>(std::istream& is, Months& m) {    
    unsigned short num;
    is>>num;
    m = static_cast<Months>(num);
    return is;
}

inline bool operator==(const Months& m,const unsigned short& value) {
    return static_cast<unsigned short>(m) == value; 
}

int main() {
    Months m;
    
    while(static_cast<unsigned short>(m)) {
        std::cout<<"Input month: "s;
        std::cin>>m;
        std::cout<<(m==0 ? "Terminating...\n"s : ShowMonth(m))<<std::endl;
    }

    return 0;
}