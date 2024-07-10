#include <algorithm>
#include <fstream>
#include <ios>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "boost/lexical_cast.hpp"

using namespace std::string_literals;
const int STRUCT_LEN = 4;

struct Address_struct {
    std::string city;
    std::string street;
    int house = 0;
    int apt = 0;
};

class Addr {
private:
    int num_ = 0;
    std::vector<std::string> buffer_;

    void get_blocks_number(std::ifstream& ifs) {
            std::string line;
            std::getline(ifs,line);

        try {
            num_ = boost::lexical_cast<int>(line);
        } catch(boost::bad_lexical_cast& e) {
            std::cerr<<"Exception: "s<<e.what()<<std::endl;
        }
    }

    void get_block(std::ifstream& ifs) {
        std::string line;
        for(int i=0; i<STRUCT_LEN; ++i) {
            std::getline(ifs,line);
            buffer_.push_back(line);
        }
    }

    Address_struct parse_block() {
        Address_struct addr;

        addr.city=buffer_[0];
        addr.street=buffer_[1];
        addr.house=boost::lexical_cast<int>(buffer_[2]);
        addr.apt=boost::lexical_cast<int>(buffer_[3]);
        buffer_.resize(0);

        return addr;
    }

public:
    Addr(std::ifstream& ifs) {
        if(!ifs.is_open()) {
            throw std::runtime_error("Open error\n"s);
        } else {
            get_blocks_number(ifs);
        }
    }

    void fetch_data(std::ifstream& ifs, std::vector<Address_struct>& target) {
        std::string line;

        for(int i=0; i<num_; ++i) {
            Address_struct addr;
            get_block(ifs);
            addr = parse_block();
            target.push_back(addr);
        }
    }
};

void write_struct_to_file(std::ostream& os, const Address_struct& addr) {
    os  << addr.city <<", "s
        << addr.street << ", "s
        << addr.house << ", "s
        << addr.apt<<'\n';
}

int main() {
    std::ifstream ifs;
    std::string input_path = "./in.txt"s;
    ifs.open(input_path);

    if(!ifs.is_open()) {
        throw std::runtime_error("Open error\n"s);
    } else {
        Addr a(ifs);
        // сюда запишем результат разбора файла
        std::vector<Address_struct> address_list;
        //разберём содержимое в вектор
        a.fetch_data(ifs,address_list);
        ifs.close();

        //отсортируем по городам алгоритмом sort с лямбда-функцией
        std::sort(address_list.begin(),address_list.end(),[]
            (const auto& rhs, const auto& lhs) {
                return rhs.city < lhs.city;
            } );

        std::ofstream ofs;
        std::string output_path = "./out.txt"s;
        // режим APPEND: создать, если не существует
        // если существует, то дописать в конец
        ofs.open(output_path, std::ios::app);

        ofs<<address_list.size()<<'\n';
        for(const auto& e : address_list) {
            write_struct_to_file(ofs,e);
        }
        ofs.close();
    }

    return 0;
}
