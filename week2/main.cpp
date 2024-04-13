#include <fstream>
#include <iostream>
#include <set>
#include <string>
#include <unordered_set>
#include <vector>
#include <chrono>

#include "include/bit_array.hpp"
#include "include/packed_array.hpp"

template <class arraytype, bool time = false, bool sum = false, bool location = false>
void run_ops(arraytype& array, std::istream& in, int n) {
    // std::cout << "runops start" << '\n';
    uint64_t val;
    
    
    if (time) {
        auto start = std::chrono::high_resolution_clock::now();

        for (int i = 0; i < n; i++) { // set the array
            in.read((char *)&val, sizeof(val));
            array.set(val);
        }

        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
        std::cerr << "Set time: " << duration.count() << " nanoseconds" << std::endl;
        // std::cerr << duration.count();

        start = std::chrono::high_resolution_clock::now();
        
        if (sum) {
            // std::cout << "sum\n";
            array.sum_precompute();
            for (int i = 0; i < n; i++) { // sum queries
                in.read((char *)&val, sizeof(val));
                std::cout << array.sum(val) << "\n";
            }
        } else if (location) {
            array.sum_precompute();
            for (int i = 0; i < n; i++) { // location queries
                in.read((char *)&val, sizeof(val));
                std::cout << array.location(val) << "\n";
            }
        } 
        else {
            for (int i = 0; i < n; i++) { // get queries
                in.read((char *)&val, sizeof(val));
                std::cout << array.get(val) << "\n";
            }
        }

        end = std::chrono::high_resolution_clock::now();
        duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
        std::cerr << "Query time: " << duration.count() << " nanoseconds" << std::endl;
        // std::cerr << "Query time: " << duration.count() << " nanoseconds" << std::endl;
        // std::cerr << "Query time: " << duration.count() << " nanoseconds" << std::endl;
        // std::cerr << "Query time: " << duration.count() << " nanoseconds" << std::endl;
        // std::cerr << duration.count() << std::endl;
    } else {
        for (int i = 0; i < n; i++) { // set the array
            in.read((char *)&val, sizeof(val));
            array.set(val);
        }

        if (sum) {
            array.sum_precompute();
            for (int i = 0; i < n; i++) { // sum queries
                in.read((char *)&val, sizeof(val));
                std::cout << array.sum(val) << "\n";
            }
        } else if (location) {
            array.sum_precompute();
            for (int i = 0; i < n; i++) { // location queries
                in.read((char *)&val, sizeof(val));
                std::cout << array.location(val) << "\n";
            }
        } else {
            for (int i = 0; i < n; i++) { // get queries
                in.read((char *)&val, sizeof(val));
                std::cout << array.get(val) << "\n";
            }
        }
    }
    
    
}

template <bool time = false>
void run_ops_packed(pfp::packed_array& array, std::istream& in, int n) {
    uint64_t val;
    
    if (time) {

    } else {
        for (int i = 0; i < n; i++) { // set the array
            in.read((char *)&val, sizeof(val));
            array.encode(val);
        }
    }
}

template <bool time = false, bool sum = false, bool location = false>
void select_qs(int type, std::istream& in) {
    // uint64_t[64] n, m;
    uint64_t n, m;
    in.read((char *)&n, sizeof(n));
    in.read((char *)&m, sizeof(m));
    // std::cout << n << "\n";
    // std::cout << m << "\n";
    
    if (type == 0) {
        
        // std::cout << "awfwafwaj" << '\n';
        pfp::bit_array array(m);
        // std::cout << "death" << '\n';
        run_ops<pfp::bit_array, time, sum, location>(array, in, n);
    } else if (type == 1) {
        pfp::packed_array array(n, m);
        run_ops_packed<time>(array, in, n);
    }

}


int main(int argc, char const* argv[]) {
    // Change the line below to override default behaviour at CSES.
    // CSES does not support command line parameters (yet).
    // This mean a workaround is required to test custom data structures.
    // For example to submit with the unbalanced binary tree "-t 3" by default
    // you should change the line below to "int type = 3;".
    
    int type = 0;
    uint64_t limit = (uint32_t(1) << 31) - 1;
    
    bool separate_queries = false;
    int input_file = 0;
    int verify = false;
    int i = 1;
    bool debug = false;
    bool time = false;
    bool sum = false;
    bool location = false;
    while (i < argc) {
        std::string s(argv[i++]);
        // std::cout << "wdad" << '\n';
        if (s.compare("-b") == 0) {
            // std::cout << "adwfa" << "\n";
            type = 0;
        } else if (s.compare("-t") == 0) {
            time = true;
        } else if (s.compare("-s") == 0) {
            sum = true;
        } else if (s.compare("-l") == 0) {
            location = true;
        } else if (s.compare("-i") == 0) {
            type = 1;
        } else {
            input_file = i - 1;
        } 
    }
    if (debug)
        std::cerr << "type = " << type << ", limit = " << limit
                  << ", separate queries = " << separate_queries << std::endl;

    // Both std::cin (console input) and std::ifstream (input file stream)
    // inherit std::istream (input stream).
    if (input_file > 0) {
        // std::cout << "inp>0" << '\n';
        if (time) {
            // auto start = std::chrono::high_resolution_clock::now();
            std::ifstream in(argv[input_file], std::ios::binary|std::ios::in);
            if (sum) {
                select_qs<true, true, false>(type, in);
            } else if (location) {
                select_qs<true, false, true>(type, in);
            } else {
                select_qs<true, false, false>(type, in);
            }
            // auto end = std::chrono::high_resolution_clock::now();
            // auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
            // std::cout << "Time taken: " << duration.count() << " microseconds" << std::endl;
        } else {
            // std::cout << "1" << "\n";
            std::ifstream in(argv[input_file], std::ios::binary|std::ios::in);
            if (sum) {
                select_qs<false, true, false>(type, in);
            } else if (location) {
                select_qs<false, false, true>(type, in);
            } else {
                select_qs<false, false, false>(type, in);
            }
        }
        
    return 0;
    } else {
        // std::cout << "manual input" << std::endl;
        if (time) {
            // auto start = std::chrono::high_resolution_clock::now();
            if (sum) {
                select_qs<true, true, false>(type, std::cin);
            } else if (location) {
                select_qs<true, false, true>(type, std::cin);
            } else {
                select_qs<true, false, false>(type, std::cin);
            }
            // auto end = std::chrono::high_resolution_clock::now();
            // auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
            // std::cout << "Time taken: " << duration.count() << " microseconds" << std::endl;
        } else {
            // std::cout << "1" << "\n";
            if (sum) {
                select_qs<false, true, false>(type, std::cin);
            } else if (location) {
                select_qs<false, false, true>(type, std::cin);
            } else {
                select_qs<false, false, false>(type, std::cin);
            }
        }
        
    return 0;
    }
}