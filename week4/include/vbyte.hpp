/**
 * VByte encoding/decoding structure
 */
#include <fstream>
#include <iostream>
#include <set>
#include <string>
#include <unordered_set>
#include <cmath>
#include <algorithm>
#include <vector>
#include <cmath>

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
class vbyte_vector {
   private:
    std::vector<uint8_t> bytes;
    std::vector<uint64_t> decoded;

   public:
    /**
     * Constructor to bit array.
     *
     * Required since the explicit deletion of copy/move operations also
     * implicitly deletes the default constructor
     */
    vbyte_vector() {

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
    ~vbyte_vector() {
        bytes.clear();
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
    void encode(uint64_t input) {
        std::cout << "start encode" << std::endl;
        uint64_t b;
        while (true) {
            std::cout << input << " ";
            b = input % 128;
            if (input < 128) {
                bytes.push_back(b + 128);
                std::cout << std::endl;
                break;
            }
            bytes.push_back(b);
            input = input / 128;
        }
        std::cout << std::endl;
        for (uint64_t num : bytes) {
            std::cout << num << " ";
        }
        std::cout << std::endl;
    }

    void encode_sorted(uint64_t input, uint64_t previous) {
        uint64_t diff = input - previous;
        std::cout << "start encode" << std::endl;
        uint64_t b;
        while (true) {
            std::cout << diff << " ";
            b = diff % 128;
            if (diff < 128) {
                bytes.push_back(b + 128);
                std::cout << std::endl;
                break;
            }
            bytes.push_back(b);
            diff = diff / 128;
        }
        std::cout << std::endl;
        for (uint64_t num : bytes) {
            std::cout << num << " ";
        }
        std::cout << std::endl;
    }

    void decode() {
        uint64_t tmp = 0;
        uint64_t powerof2 = 0;
        for (uint8_t element : bytes) {
            if (element > 128) {
                tmp += (element - 128) * std::pow(2, powerof2);
                decoded.push_back(tmp);
                tmp = 0;
                powerof2 = 0;
                continue;
            }
            tmp += element * std::pow(2, powerof2); 
            powerof2 += 7;
        }
        std::cout << "decoded size" << decoded.size() << std::endl;
        for (uint64_t num : decoded) {
            std::cout << num << " ";
        }
        std::cout << std::endl;
    }

    void decode_sorted() {
        uint64_t tmp = 0;
        uint64_t powerof2 = 0;
        for (uint8_t element : bytes) {
            if (element > 128) {
                tmp += (element - 128) * std::pow(2, powerof2);
                decoded.push_back(tmp);
                // tmp = 0;
                powerof2 = 0;
                continue;
            }
            tmp += element * std::pow(2, powerof2); 
            powerof2 += 7;
        }
        std::cout << "decoded size" << decoded.size() << std::endl;
        for (uint64_t num : decoded) {
            std::cout << num << " ";
        }
        std::cout << std::endl;
    }

    uint64_t get(uint64_t &index) {
        return decoded.at(index);
    }

    uint64_t size() {
        return bytes.size();
    }
};

}  // namespace pfp