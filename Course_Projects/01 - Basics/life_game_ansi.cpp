#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iterator>
#include <sstream>
#include <queue>
#include <unistd.h>
using namespace std::string_literals;

typedef std::vector<std::vector<unsigned char>> GRID;
// ESC-sequence for
// console coloring in ANSI-compatible terminals (i.e. bash or
// old windows with ansi.sys)
#define RESET       "\033[0m"       // restore defaults
#define RED         "\033[31m"

template <typename T, int max_len, typename Container=std::deque<T>>
class Queue : public std::queue<T, Container> {
public:
    void push(const T& value) {
        if (this->size() == max_len) {
           this->c.pop_front();
        }
        std::queue<T, Container>::push(value);
    }
};

  // string to be split by elements
    template<typename T>
    std::vector<T> split(const std::string& line) {
        std::istringstream is(line);
        return std::vector<T>(std::istream_iterator<T>(is),
                          std::istream_iterator<T>());
    }

    void clear_grid(GRID& grid) {
        GRID::iterator it;

        for(it=grid.begin(); it != grid.end(); ++it)
            std::fill(it->begin(),it->end(),0);
    }

    unsigned int crc32(const GRID& grid) {
        unsigned int byte, mask;
        unsigned int crc = 0xFFFFFFFF;

        for(const auto& g : grid) {
            for(size_t i=0; g[i] != 0; ++i) {
                byte = g[i];
                crc ^= byte;
                for (unsigned char j = 0; j<7; ++j) {
                    mask = -(crc & 1);
                    crc = (crc >> 1) ^ (0xEDB88320 & mask);
                }
            }
        }
        return ~crc;
    }

class Universe {
private:
    std::string path;
    std::ifstream ifs;
    GRID current_grid;
    GRID previous_grid;
    Queue<unsigned int,2> uni_queue;
    unsigned int checksum;

    struct {
        size_t x;
        size_t y;
    } dims;

    bool collapsed;
    unsigned int generation_counter;
    unsigned int cells_alive;

    // get size of grid from the 1st line of config file
    void set_grid_dimensions() {
        std::string line;
        std::getline(ifs,line);
        const std::vector<size_t> fetch_line = split<size_t>(line.c_str());
        dims.x=fetch_line[0];
        dims.y=fetch_line[1];
    }

    void resize_grid() {
        current_grid.resize(dims.x,std::vector<unsigned char>(dims.y));
    }

    // fetch each line with live cells coordinates
    void fetch_grid() {
        std::string line;

        while(std::getline(ifs,line)) {
                //split coordinates from string to numeric vector
                const std::vector<size_t> fetch_line = split<size_t>(line.c_str());
                current_grid[fetch_line[0]]
                            [fetch_line[1]] = 0x1;            // mark cell as alive
        }
    }

    //check if coordinates are in bounds of grid size
    inline bool inBounds(const size_t& row,const size_t& col) {
        return (row>=0 && row<dims.x && col>=0 && col<dims.y);
    }

    //count live neighbors && set this info to bits# 1,2,3,4
    void set_cell_neighbors(const size_t& row, const size_t& col) {
        unsigned char live_cells_counter=0;

        for(char i=-1; i<=1; ++i)
            for(char j=-1; j<=1; ++j)
                if(!(i==0 && j==0) && inBounds(row+i,col+j)
                          && isAlive(current_grid[row+i][col+j])) {
                    ++live_cells_counter;
                }
        // write live_cells parameter to bits 1,2,3,4
        current_grid[row][col] |= live_cells_counter<<1;
    }

     // set info about all neighbors of each cell
    void set_grid_attr() {
        for(size_t r=0; r<dims.x; ++r)
                for(size_t c=0; c<dims.y; ++c)
                    set_cell_neighbors(r,c);
    }

    void set_alive_cells_counter() {
        unsigned int ctr=0;
        for(const auto& elem : current_grid)
            for(const char c : elem)
                if(c & 0x1) ++ctr;
        cells_alive=ctr;
    }

    bool not_stagnated() {
        if( (generation_counter == 1 && cells_alive) ||
             uni_queue.front() != checksum ||
             uni_queue.back()  != checksum )
            return true;

        collapsed = true;
        return false;
    }

public:
    Universe(const std::string& path_to_file) {
        path = path_to_file;
        ifs.open(path);
	if(!ifs.is_open()) {
	    throw std::runtime_error("Open error\n"s);
	} else {
            generation_counter = 1;              // 1st generation
            cells_alive = 0;
            set_grid_dimensions();               // grid size

            if(dims.x || dims.y) {               // only if grid size is not 0x0
                resize_grid();                       // resize 2d-vector
                fetch_grid();                        // mark cells as alive or dead
                ifs.close();
                set_alive_cells_counter();            // count live cells, set counter

                if(cells_alive) {                      // if someone is alive
                    set_grid_attr();                     // add info about live neighbors

                    previous_grid=current_grid;          // save current grid
                    collapsed = false;
                    checksum = 0x0;
                    uni_queue.push(crc32(current_grid));
                } else collapsed = true;               // all cells are dead
            } else collapsed = true;
        }
    }

    void next_generation() {
        clear_grid(current_grid);
        for(size_t r=0; r<dims.x; ++r) {
            for(size_t c=0; c<dims.y; ++c) {
                auto *prev = &previous_grid[r][c];
                auto *curr = &current_grid[r][c];

                if(!*prev) continue;                        // cell is not alive + no neighbors. Ignore.

                if((!isAlive(*prev) && ((*prev>>1)==0x3)) ||     // dead cell with 3 neighbors comes to life
                    (isAlive(*prev) && ((*prev>>1)==0x2 || ((*prev>>1)==0x3))))   // alive cell with 2 or 3 neighbors...
                    *curr = 0x1;                                  // ...doesn't die

                // live cell with >3 or <2 neighbors ...
                    if(isAlive(*prev) && ((*prev>>1) > 0x3 || (*prev>>1) < 0x2))
                        *curr = 0x0;             // ...dies
                }
            }
        // bypass new grid, count neighbors of each cell,set bits
            set_grid_attr();
            checksum = crc32(current_grid);
            uni_queue.push(checksum);

            if(not_stagnated()) {
                previous_grid = current_grid;    // save state otherwise
            }
            set_alive_cells_counter();       // set new counter
            ++generation_counter;
    }

    void show() const {
        for(const auto& x : current_grid) {
            for(const char y : x)
                std::cout<< RED << (y&1 ? " * "s : "\033[35m - "s)<< RESET
                         << std::flush;
            std::cout << std::endl;
        }
    }

    ~Universe() { }

    inline unsigned int get_generation_info() const {
        return generation_counter;
    }

    inline bool is_collapsed() const {
        return collapsed;
    }

    inline bool dims_0x0() const {
        return !dims.x && !dims.y;
    }

    inline bool isAlive(const char& cell) const {
        return cell & 0x1;
    }

    inline unsigned int count_live_cells () const {
        return cells_alive;
    }
};

void clear_screen() {
#ifdef WIN32
    std::system("cls");
#else
    std::system("clear");
#endif
}

   int run(Universe& U) {
        if(U.dims_0x0()) {
            std::cerr<<"Grid size 0x0. Terminating.\n"s;
            return EXIT_FAILURE;
        }

        bool flag;
        while( !U.is_collapsed() || (U.get_generation_info()==1) ) {
            clear_screen();

            std::cout<<"Generation: "s<< U.get_generation_info()
                     <<". Alive cells: "s<<U.count_live_cells()<<'\n'<<std::flush;
            U.show();

            flag=U.count_live_cells();

            U.next_generation();
            sleep(1);
        }

        if(flag) {
            clear_screen();
            std::cout<<"Generation: "s<< U.get_generation_info()
                     <<". Alive cells: "s<<U.count_live_cells() <<'\n'<<std::flush;
            U.show();
        }

        if(!U.count_live_cells()) {
            std::cout<<"All cells are dead. Game over\n"s;
        } else {
            std::cout<<"Universe stagnated. Game over\n"s;
        }

        return EXIT_SUCCESS;
    }

int main() {
    Universe Uni("./cell_data.in"s);
    run(Uni);
    return 0;
}
