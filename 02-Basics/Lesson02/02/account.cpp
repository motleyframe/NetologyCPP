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
    std::cout<<"Account #: "s << my_acct.acct_number << '\n'
             <<"Name: "s <<my_acct.client_name << '\n'
             <<"Balance: "s <<my_acct.balance << '\n';

    return 0;
}