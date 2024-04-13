/**
 * Very simple unbalanced binary tree implementation.
 *
 * Sometimes faster than std::set and sometimes not. Why? :)
 */

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
template <class dtype>
class binary_tree {
   private:
    class node;
    node* root = nullptr;

   public:
    /**
     * Empty constructor to binary tree.
     *
     * Required since the explicit deletion of copy/move operations also
     * implicitly deletes the default constructor
     */
    binary_tree(){};

    /**
     * This destructor is not strictly needed since program termination will
     * deallocate the memory, but if the data structure didn't live for the
     * duration of execution, not explicitly deleting any children would be a
     * massive memory leak.
     *
     * The default destructor would simply deallocate the node itself and
     * leave any children orphaned in memory.
     */
    ~binary_tree() {
        if (root != nullptr) delete (root);
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
    binary_tree(const binary_tree&) = delete;
    binary_tree& operator=(const binary_tree&) = delete;
    binary_tree(binary_tree&&) = delete;
    binary_tree& operator=(binary_tree&&) = delete;

    /**
     * Insert a value into the binary tree. Since the tree is logically a set
     * a duplicate will not be created if value is already present.
     *
     * @param value Element to be inserted
     */
    void insert(dtype value) {
        /* The root of the tree will be "nullptr" only as long as there has been
         * no element inserted into the tree So the affirmative branch will be
         * executed at most one time while the "else" branch can be executed
         * very many times (until the system is out of memory and crashes). The
         * [[unlikely]] annotation tells the compiler to optimize accordingly.
         * This can have a surpisingly big effect on performance due to the
         * aggressinve pipelining and instruction caching in modern
         * michroprosessors. The same could be accomplished by flipping the
         * condition:
         *
         * if (root != nullptr) {
         *     root->insert(value);
         * } else {
         *     root = new node(value);
         * }
         *
         * However an [[(un)likely]] annotation often leads to more readable
         * code. (And better conveys the intention of the programmer.)
         */
        if (root == nullptr) [[unlikely]] {
            root = new node(value);
        } else {
            root->insert(value);
        }
    }

    /**
     * In c++17, the set structures do not support simple membership queries and
     * count needs to be used instead. Due to this, since the new data
     * structures need to work with the same interface, the membership query
     * here will be called count as well.
     *
     * In C++20 membership queries do exist. But legacy code. Who can bother
     * updating...
     *
     * @param value The value to count the occurrences of.
     * @return The number of occurrences of value.
     */
    int count(dtype value) const {
        return root != nullptr ? root->query(value) : false;
    }
};

/**
 * The node class is an internal class of the binary tree.
 *
 * Defined here in a private section of the binary_tree class to keep it
 * from being visible to code referencing the "binary_tree.hpp" header.
 *
 * @tparam dtype Type of integers to store.
 */
template <class dtype>
class binary_tree<dtype>::node {
   private:
    dtype val;
    node* left = nullptr;
    node* right = nullptr;

   public:
    /**
     * Constructor for node objects. Logically equivalent to
     *
     * node (dtype value) {
     *    val = value;
     * }
     *
     * this style is often easier for the compiler to optimize, and meay
     * lead to faster code.
     *
     * For more info see the Effective c++ book (probably mentioned on the
     * course).
     *
     * @param value The value attached to the created node.
     */
    node(dtype value) : val(value) {}

    /*
     * Ensuring that copy/move semantics don't apply to an internal class is
     * less important but doesn't cost anything.
     */
    node(const node&) = delete;
    node& operator=(const node&) = delete;
    node(node&&) = delete;
    node& operator=(node&&) = delete;

    /**
     * Again, manual destructor to guard against memory leaks.
     */
    ~node() {
        if (left != nullptr) delete (left);
        if (right != nullptr) delete (right);
    }

    /**
     * Ensures that value is present in the subtree rooted at this node. If
     * the value already exists nothing is done (since this is logically a
     * set).
     *
     * @param value Value to insert.
     */
    void insert(dtype value) {
        /* For any given node, it is unlikely that (val == value), this we
         * mark this branch unlikely. Here flipping the statement would
         * again work but would be more unclear (IMO):
         *
         * if (val != value) {
         *     <rest of the method>
         * }
         */
        if (val == value) [[unlikely]] {
            return;
        }
        if (value > val) {
            if (right == nullptr) {
                right = new node(value);
            } else {
                right->insert(value);
            }
        } else {
            if (left == nullptr) {
                left = new node(value);
            } else {
                left->insert(value);
            }
        }
    }

    /**
     * Looks for value in the subtree rooted at this node.
     *
     *  @param value Value to look for.
     *  @return true iff value is present in subtree.
     */
    bool query(dtype value) const {
        if (value == val) [[unlikely]] {
            return true;
        }
        // Ternary expressions are often very easy for the compiler to
        // optimize, especially in tail recursion situations like this. The
        // first (affirmative) branch should be the more likely one.
        if (value > val) {
            return right != nullptr ? right->query(value) : false;
        }
        return left != nullptr ? left->query(value) : false;
    }
};

}  // namespace pfp