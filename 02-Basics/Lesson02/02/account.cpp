#include <iostream>
#include <string>
using namespace std::string_literals;

struct Account {
    unsigned int acct_number;
    std::string client_name;
    double balance;
};

void ChangeBalance(Account& acct,const double& new_balance) {
    acct.balance = new_balance;
}

std::ostream& operator<<(std::ostream& os, const Account& a) {
    os <<"Account #: "s << a.acct_number << '\n'
       <<"Name: "s <<a.client_name << '\n'
       <<"Balance: "s <<a.balance << '\n';
    return os;
}

int main() {
    Account my_acct;
    std::cout<<"Input account number: "s;
    std::cin>>my_acct.acct_number;
    std::cout<<"Input client name: "s;
    std::cin>>my_acct.client_name;
    std::cout<<"Input balance: "s;
    std::cin>>my_acct.balance;
    
    double new_balance;
    std::cout<<"Input new balance: "s;
    std::cin>>new_balance;
    my_acct.balance = new_balance;
    std::cout<<"-------------------\n"s;

    std::cout<<my_acct;

    return 0;
}
