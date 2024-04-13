/**
 * You can use this for programming task 2 of set 1 
*/

#pragma once
#include <vector>
#include <unordered_set>

namespace pfp {

template <class dtype>
class bv {
  public:
    std::vector<bool> values;

    bv(dtype limit) {
      values.resize(limit, false);
      // for (bool x : values) {
      //   std::cout << x << std::endl;
      // }
    }

    void insert(dtype value) { 
      values[value] = true;
      // for(auto w:values) std::cout << (w) << " ";
      // std:: cout << std::endl;
    }

    int count(dtype value) const {
        return values[value];
    }
};

} // namespace pfp