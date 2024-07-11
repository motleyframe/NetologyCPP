#include <algorithm>
#include <exception>
#include <fstream>
#include <ios>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include "boost/lexical_cast.hpp"

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
    std::ifstream ifs_;
    std::ofstream ofs_;
    std::vector<std::string> buffer_;
    bool error_state_ = false;

    void get_blocks_number() {
            std::string line;
            std::getline(ifs_,line);

        try {
            num_ = boost::lexical_cast<int>(line);
        } catch(boost::bad_lexical_cast& e) {
            std::cerr<<"Exception: "s<<e.what()<<std::endl;
        }
    }

    void get_block() {
        std::string line;
        for(int i=0; i<STRUCT_LEN; ++i) {
            std::getline(ifs_,line);
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
    Addr(const std::string& input_path) {
        ifs_.open(input_path);
        if(!ifs_.is_open()) {
            error_state_ = true;
            throw std::runtime_error("Open error\n"s);
        } else {
            error_state_ = false;
            get_blocks_number();
        }
    }

    inline bool get_error_state() {
        return error_state_;
    }

    void fetch_data(std::vector<Address_struct>& target) {
        std::string line;

        for(int i=0; i<num_; ++i) {
            Address_struct addr;
            get_block();
            addr = parse_block();
            target.push_back(addr);
        }
    }

    void open_output_file(const std::string& output_path,const std::ios_base::openmode& openmode) {
        ofs_.open(output_path,openmode);
        if(!ofs_.is_open()) {
            throw std::runtime_error("Open error\n"s);
            error_state_ = true;
        }
        error_state_ = false;
        return ;
    }

    void process_data(std::vector<Address_struct>& vault) {
        std::sort(vault.begin(),vault.end(),[]
            (const auto& rhs, const auto& lhs) {
                return rhs.city < lhs.city;
            } );

        ofs_ << vault.size() << '\n';
        for(const auto& addr : vault) {
            ofs_  << addr.city <<", "s
                  << addr.street << ", "s
                  << addr.house << ", "s
                  << addr.apt<<'\n';
        }
        return ;
    }

    ~Addr() {
        try {
            ifs_.close();
            ofs_.close();
        } catch (...) { }
    }
};

int main() {
    std::string input_path = "./in.txt"s;
    std::vector<Address_struct>* addr_ptr = nullptr;

    Addr a(input_path);

    if(!a.get_error_state()) {
        addr_ptr = new std::vector<Address_struct>();       // сюда запишем результат разбора файла
        a.fetch_data(*addr_ptr);                            // разберём содержимое в вектор
    }

    std::string output_path = "./out.txt"s;                 // режим APPEND: создать, если не существует
    a.open_output_file(output_path,std::ios::app);          // если существует, то дописать в конец

    if(!a.get_error_state()) {
        a.process_data(*addr_ptr);
        delete addr_ptr;
    }

    return 0;
}
