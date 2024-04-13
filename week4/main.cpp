#include <fstream>
#include <iostream>
#include <set>
#include <string>
#include <unordered_set>
#include <vector>
#include <chrono>

#include "include/vbyte.hpp"

template <bool differences = false, bool generalized = false, bool time = false>
void run_ops(std::istream& in) {
    // std::cout << "runops start" << '\n';
    if (differences) {
        pfp::vbyte_vector vector;
        uint64_t n, currentreading, prevreading;
    
        in.read((char *)&n, sizeof(n));
        in.read((char *)&currentreading, sizeof(currentreading));
        vector.encode(currentreading);
        prevreading = currentreading;

        for (uint64_t i = 0; i < n - 1; i++) {
            in.read((char *)&currentreading, sizeof(currentreading));
            std::cout << currentreading << std::endl;
            vector.encode_sorted(currentreading, prevreading);
            prevreading = currentreading;
        }
    
        vector.decode_sorted();
    
    
        std::cout << "n: " << n << std::endl;
        std::cout << vector.size() << std::endl;
    } else {
        pfp::vbyte_vector vector;
        uint64_t n, currentreading;

        in.read((char *)&n, sizeof(n));

        for (uint64_t i = 0; i < n; i++) {
            in.read((char *)&currentreading, sizeof(currentreading));
            std::cout << currentreading << std::endl;
            vector.encode(currentreading);
        }

        vector.decode();


        std::cout << "n: " << n << std::endl;
        std::cout << vector.size() << std::endl;
    }
}


int main(int argc, char const* argv[]) {
    // Change the line below to override default behaviour at CSES.
    // CSES does not support command line parameters (yet).
    // This mean a workaround is required to test custom data structures.
    // For example to submit with the unbalanced binary tree "-t 3" by default
    // you should change the line below to "int type = 3;".
    
    uint64_t limit = (uint32_t(1) << 31) - 1;
    
    int input_file = 0;
    int i = 1;
    bool differences = false;
    bool generalized = false;
    uint64_t bitblocks = 7;
    bool time = false;
    while (i < argc) {
        std::string s(argv[i++]);
        // std::cout << "wdad" << '\n';
        if (s.compare("-s") == 0) {
            // std::cout << "adwfa" << "\n";
            differences = true;
        } else if (s.compare("-k") == 0) {
            generalized = true;
            bitblocks = (uint64_t)argv[i + 1];
        } else if (s.compare("-t") == 0) {
            time = true;
        } else {
            input_file = i - 1;
        } 
    }
    
    // Both std::cin (console input) and std::ifstream (input file stream)
    // inherit std::istream (input stream).
    if (input_file > 0) {
        // std::cout << "inp>0" << '\n';
        if (time) {
            // auto start = std::chrono::high_resolution_clock::now();
            std::ifstream in(argv[input_file], std::ios::binary|std::ios::in);
            if (differences) {
                run_ops<true, false, true>(in);
            } else {
                run_ops<false, false, true>(in);
            }
            // auto end = std::chrono::high_resolution_clock::now();
            // auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
            // std::cout << "Time taken: " << duration.count() << " microseconds" << std::endl;
        } else {
            // std::cout << "1" << "\n";
            std::ifstream in(argv[input_file], std::ios::binary|std::ios::in);
            
            if (differences) {
                std::cout << "ebal mamu" << std::endl;
                run_ops<true, false, false>(in);
            } else {
                std::cout << "kakaxi" << std::endl;
                run_ops<false, false, false>(in);
            }
        }
        
    return 0;
    } else {
        // std::cout << "manual input" << std::endl;
        if (time) {
            // auto start = std::chrono::high_resolution_clock::now();
            if (differences) {
                run_ops<true, false, true>(std::cin);
            } else {
                run_ops<false, false, true>(std::cin);
            }
            // auto end = std::chrono::high_resolution_clock::now();
            // auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
            // std::cout << "Time taken: " << duration.count() << " microseconds" << std::endl;
        } else {
            // std::cout << "1" << "\n";
            if (differences) {
                run_ops<true, false, false>(std::cin);
            } else {
                run_ops<false, false, false>(std::cin);
            }
        }
        
    return 0;
    }
}