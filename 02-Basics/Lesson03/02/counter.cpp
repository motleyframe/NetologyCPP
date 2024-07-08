#include <iostream>

using namespace std::string_literals;

class Counter {
private:
    int value_ = 0;

    void initialize() {
        value_ = 1;
    }

    void initialize(int& val) {
        value_ = val;
    }

public:
    Counter() {
        this->initialize();
    }

    Counter(int new_value) {
        this->initialize(new_value);
    }

    void inc() {
        ++value_;
    }

    void dec() {
        --value_;
    }

    void print() {
        std::cout<<value_<<std::endl;
    }
};

int main() {
    char ans;
    Counter* counter = nullptr;

    std::cout<<"Do you want to initialize counter's value? (y/n) "s;
    std::cin>>ans;

    int init_value=0;

    if(ans=='y') {
        std::cout<<"Input initial value : "s;
        std::cin>>init_value;
        counter = new Counter(init_value);
    } else {
        counter = new Counter();
    }

    bool state = true;

    while(state) {
        char cmd;

        std::cout<<"Choose command: '+', '-', '=' or 'x': "s;
        std::cin>>cmd;

        switch(cmd) {
            case '+':
                counter->inc();
                break;

            case '-':
                counter->dec();
                break;

            case '=':
                counter->print();
                break;

            case 'x':
                std::cout<<"Goodbye!\n"s;
                state = false;
                delete counter;
                break;

            default:
                break;
        }
    }

    return 0;
}
