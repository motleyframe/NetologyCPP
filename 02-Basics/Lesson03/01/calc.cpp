#include <iostream>
using namespace std::string_literals;

class Calculator {
private:
    double num1_=0.0;
    double num2_=0.0;
public:
    inline double add() {
        return num1_+num2_;
    }

    inline double multiply() {
        return num1_*num2_;
    }

    inline double subtract_1_2() {
        return num1_-num2_;
    }

    inline double subtract_2_1() {
        return num2_-num1_;
    }
    inline double divide_1_2() {
        return num1_/num2_;
    }
    inline double divide_2_1() {
        return num2_/num1_;
    }
    inline bool set_num1(double& val) {
        if(val) {
            num1_ = val;
            return true;
        }
        return false;
    }

    inline bool set_num2(double& val) {
        if(val) {
            num2_ = val;
            return true;
        }
        return false;
    }
};

int main() {
    Calculator calc;

    for(;;) {
        double num1,num2;

        for(;;) {
            std::cout<<"Input num1: "s;
            std::cin>>num1;

            if(!calc.set_num1(num1)) {
                std::cout<<"Incorrect input\n"s;
            } else break;
        }

         for(;;) {
            std::cout<<"Input num2: "s;
            std::cin>>num2;

            if(!calc.set_num2(num2)) {
                std::cout<<"Incorrect input\n"s;
            } else break;
        }

        std::cout<<"num1 + num2 = "s << calc.add() << std::endl;
        std::cout<<"num1 - num2 = "s << calc.subtract_1_2() << std::endl;
        std::cout<<"num2 - num1 = "s << calc.subtract_2_1() << std::endl;
        std::cout<<"num1 * num2 = "s << calc.multiply() << std::endl;
        std::cout<<"num1 / num2 = "s << calc.divide_1_2() << std::endl;
        std::cout<<"num2 / num1 = "s << calc.divide_2_1() << std::endl;
    }

    return 0;
}
