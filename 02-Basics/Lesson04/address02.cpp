#include <algorithm>
#include <fstream>
#include <ios>
#include <iostream>
#include <string>
#include <vector>

using namespace std::string_literals;

const int STRUCT_LEN = 4;

struct Address_struct {
    std::string city;
    std::string street;
    int house = 0;
    int apt = 0;
};

enum class OperationResult {
    OPEN_SUCCESS,
    OPEN_FAILED
};

class Addr {
private:
    int num_ = 0;
    std::ifstream ifs_;
    std::ofstream ofs_;
    std::vector<std::string> buffer_;
    OperationResult result_;

    void get_blocks_number() {
            std::string line;
            std::getline(ifs_,line);
            num_ = std::stoi(line);
            return ;
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
        addr.house=std::stoi(buffer_[2]);
        addr.apt=std::stoi(buffer_[3]);
        buffer_.resize(0);

        return addr;
    }

public:
    Addr(const std::string& input_path) {
        ifs_.open(input_path);
        if(!ifs_.is_open()) {
            result_ = OperationResult::OPEN_FAILED;
            throw std::runtime_error("Open error\n"s);
        } else {
            result_ = OperationResult::OPEN_SUCCESS;
            get_blocks_number();
        }
    }

    inline OperationResult get_error_state() {
        return result_;
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

    OperationResult open_output_file(const std::string& output_path,const std::ios_base::openmode& openmode) {
        ofs_.open(output_path,openmode);
        if(!ofs_.is_open()) {
            throw std::runtime_error("Open error\n"s);
            result_ = OperationResult::OPEN_FAILED;
            return OperationResult::OPEN_FAILED;
        }
        result_ = OperationResult::OPEN_SUCCESS;
        return OperationResult::OPEN_SUCCESS;
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

    if(a.get_error_state() == OperationResult::OPEN_SUCCESS) {
        if(addr_ptr == nullptr)
            addr_ptr = new std::vector<Address_struct>();   // сюда запишем результат разбора файла
        a.fetch_data(*addr_ptr);                            // разберём содержимое в вектор

        std::string output_path = "./out.txt"s

        // режим APPEND: создать, если не существует. Иначе дописать в конец
        if(a.open_output_file(output_path,std::ios::app) == OperationResult::OPEN_SUCCESS) {
            a.process_data(*addr_ptr);
            delete addr_ptr;
        }
    }

    return 0;
}
