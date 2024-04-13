/**
 * Packed array
 */
#include <fstream>
#include <iostream>
#include <set>
#include <string>
#include <unordered_set>
#include <cmath>
#include <algorithm>

/*
 * This is an include guard. Know it, love it. If you forget, you may get some
 * very strange compilation errors, especially when projects get larger.
 *
 * The "#pragma once" compiler directive is the modern way to ensure that
 * the same header is only included once, even if there are multiple references
 * to the header.
 *
 * The "old", still widely used, way to do it is to wrap the entire body of this
 * file in an "#ifndef" directive:
 *
 * #ifndef PFP_BINARY_TREE_HPP
 * #define PFP_BINARY_TREE_HPP
 * <all file contents go here
 * #endif
 *
 * The "old" approach is more error prone since you need to make sure the "name"
 * used for the #define is unique in the project, and that you don't typo one of
 * the names, completely breaking the include guard.
 */
#pragma once

/**
 * In small projects like this encapsulating things in a separate namespace is
 * not really necessary but a good practice in general.
 *
 * For example if some other file is included that also contains a node class,
 * these can be differentiated as pfp::node and someting_else::node.
 *
 * If you dislike explicitly stating the namespace see "c++ using namespace" on
 * you facourite search engine. I genrally don't "using" namespaces since I like
 * the clarity of "pfp::node" compared to a using somewhere and then just
 * "node" locally.
 */
namespace pfp {

/**
 * The binary_tree class is the class meant to be used as a binary tree
 * structure.
 *
 * Here the template parameter "dtype" allows the tree to be (re)used with any
 * types that support comparison with ">" and "==". The compiler will check what
 * types the node class is used with and compile separate machine code for each
 * of them.
 *
 * For this project the only type used is "int" so the templating is redundant
 * but good practice for code reusability.
 *
 * @tparam dtype Type of integer this tree stores.
 */
class packed_array {
   private:
    bool *array;
    // uint8_t t = 2;
    uint64_t *precomp_sumarray;
    uint64_t chunksize = 10;
    uint64_t arraysize;
    uint64_t bitnum;
    uint64_t currentposition = 0;

   public:
    /**
     * Constructor to bit array.
     *
     * Required since the explicit deletion of copy/move operations also
     * implicitly deletes the default constructor
     */
    packed_array( uint64_t n, uint64_t k ) {
        // std::cout << "nice";
        if (n && k) {
            bitnum = k;
            arraysize = n * k;
            array = new bool[n];
        }
        // std::cout << "12312";
    };

    /**
     * This destructor is not strictly needed since program termination will
     * deallocate the memory, but if the data structure didn't live for the
     * duration of execution, not explicitly deleting any children would be a
     * massive memory leak.
     *
     * The default destructor would simply deallocate the node itself and
     * leave any children orphaned in memory.
     */
    ~packed_array() {
        if (array != nullptr) delete(array);
    }

    /*
     * Ensure that no copy/move operations can be performed on this data
     * structure. C++ tends to provide default copy/move operations for classes.
     * This is often convenient but may lead to significant performance issues,
     * and for reference-based data structures like this binary tree, may also
     * lead to undefined behaviour.
     *
     * To copy a binary tree (the first to deleted methods), the entire
     * structure would need to be recurively copied. The default copy
     * constructor may or may not work correctly, but in performance critical
     * applications we probably don't want to copy complex structures unless
     * absolutely necessary, if there is code that would invoke the default copy
     * constructor somewhere, the deletion of them will generate a compilarion
     * error and the user can figure out if the copy is absolutely necessary,
     * and possibly create a user-defined copy constructor.
     *
     * Move constructors (the last two deleted methods), aren't as much of a
     * performance concern, however, for complex data structures it may be
     * better to code them manually, and deleting the defaults delays needing to
     * write them as long as possible. (why implement something that's not
     * needed?)
     *
     * Here a move constructor could simpy be:
     *
     * binary_tree&(binary_tree&& lhs) {
     *     root = std::exchange(lhs.root, nullprt);
     * }
     *
     */
    // binary_tree(const binary_tree&) = delete;
    // binary_tree& operator=(const binary_tree&) = delete;
    // binary_tree(binary_tree&&) = delete;
    // binary_tree& operator=(binary_tree&&) = delete;

    /**
     * Insert a value into the binary tree. Since the tree is logically a set
     * a duplicate will not be created if value is already present.
     *
     * @param value Index to be set
     */

    void set(uint64_t &index) {
        array[index] = 1;
    }

    void encode(uint64_t &index) {
        uint64_t slider = 1;
        for (uint64_t i = currentposition; i < currentposition + bitnum; i++) {
            if (slider & index) {
                set(i);
            }
            slider = slider << 1;
        }
        currentposition += bitnum;
        for (uint64_t i = 0; i < currentposition; i++) {
            std::cout << array[i] << " ";
        }
        std::cout << std::endl;
    }

    void unset(uint64_t &index) {
        array[index] = 0;
    }

    uint64_t get(uint64_t &index) {
        return array[index];
    }

    uint64_t unoptimized_sum(const uint64_t i) const {
        // std::cout << i << " ";
        uint64_t s = 0;
        for(uint64_t j = 0; j < i; j++){
            s += (uint64_t)array[j];
        }
        return s;
    }

    void sum_precompute(void) {
        uint64_t chunkcount = arraysize / chunksize;
        precomp_sumarray = new uint64_t[chunkcount];
        // std::cout << arraysize << std::endl;
        // std::cout << chunkcount << std::endl;
        
        for (uint64_t i = 1; i <= chunkcount; i++) {
            precomp_sumarray[i - 1] = unoptimized_sum(i * chunksize);
            // std::cerr << "presum" << i - 1 << " " << precomp_sumarray[i - 1] << std::endl;
        }
    }

    uint64_t sum(const uint64_t i) {
        // sum_precompute();
        uint64_t mysum = 0;
        if (i >= chunksize) {
            mysum = precomp_sumarray[i / chunksize - 1];
            // std::cout << "i: " << i << "presum: " << mysum << "index: " << i / chunksize - 1 << std::endl; 
        } else {
            // std::cout << "i: " << i << "presum: " << mysum << "index: " << -1 << std::endl;
        }

        for (uint64_t j = i / chunksize * chunksize; j < i; j++) {
            // std::cout << mysum;
            mysum += (uint64_t)array[j];
        }
        return mysum;
    }

    uint64_t search(uint64_t *array, uint64_t start_idx, uint64_t end_idx, uint64_t search_val) {
        if( start_idx == end_idx )
           return array[start_idx] <= search_val ? start_idx : -1;

        int mid_idx = start_idx + (end_idx - start_idx) / 2;

        if( search_val < array[mid_idx] )
           return search( array, start_idx, mid_idx, search_val );

        int ret = search( array, mid_idx+1, end_idx, search_val );
        return ret == -1 ? mid_idx : ret;
    }

    uint64_t location(uint64_t i) {
        // i = 9507;
        uint64_t index = 0;
        // uint64_t left = 0;
        // uint64_t right = arraysize / chunksize - 1;

        // while (left <= right) {
        //     // std::cout << "left " << left << " right " << right << std::endl;
        //     uint64_t mid = left + (right - left) / 2;

        //     if (precomp_sumarray[mid] >= i)
        //         right = mid - 1;
        //         if (mid == 0) {
        //             index = -1;
        //             break;
        //         }
        //     // If arr[mid] is less than or equal to target, update result and move right
        //     else {
        //         index = mid;
        //         left = mid + 1;
        //     }
        // }
        index = search(precomp_sumarray, (uint64_t)0, (uint64_t)(arraysize / chunksize - 1), i);

        // if (index == 0) {
        //     index = right;
        // }
        // std::cout << "sum 8 " << sum(16) << std::endl; 
        // std::cout << "i: " << i << "preindex: " << (index + 1) * chunksize << std::endl;
        uint64_t checksum = sum((index + 1) * chunksize);
        // std::cout << "check sum" << checksum << "index: " << (index + 1) * chunksize << std::endl;
        
        index = (index + 1) * chunksize;

        if (checksum == i) {
            while (array[index - 1] == 0) {
                index--;
            }
        }
        
        while (checksum < i) {
            checksum += array[index];
            index += 1;
            // std::cout << "da";
        }

        return index;
    }
};

}  // namespace pfp