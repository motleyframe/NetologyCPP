#include <iostream>

using namespace std::string_literals;

class Counter {
private:
    int value_ = 0;

public:
    void initialize() {
        value_ = 1;
    }

    void initialize(int& val) {
        value_ = val;
    }

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
    int init_value=0;

    std::cout<<"Do you want to initialize counter's value? (y/n) "s;
    std::cin>>ans;

    Counter counter;

    if(ans=='y') {
        std::cout<<"Input initial value : "s;
        std::cin>>init_value;
        counter.initialize(init_value);
    } else counter.initialize();

    bool state = true;

    while(state) {
        char cmd;

        std::cout<<"Choose command: '+', '-', '=' or 'x': "s;
        std::cin>>cmd;

        switch(cmd) {
            case '+':
                counter.inc();
                break;

            case '-':
                counter.dec();
                break;

            case '=':
                counter.print();
                break;

            case 'x':
                std::cout<<"Goodbye!\n"s;
                state = false;
                break;

            default:
                break;
        }
    }

    return 0;
}
