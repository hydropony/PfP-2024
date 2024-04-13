/**
 * You can use this file for programmign task 1 of set 1.
*/

#include <vector>
#include <algorithm>
#pragma once


namespace pfp {

template <class dtype>
class vs {
  private:
    std::vector<dtype> insertions;
    bool wasqueried = false;
  
  public:
    void insert(dtype val) {
      insertions.push_back(val);
    }

    int count(dtype val) {
      if (!wasqueried) {
        sort(insertions.begin(), insertions.end());
        wasqueried = true;
      }
        return std::binary_search(insertions.begin(), insertions.end(), val);
    }
};

}  // namespace pfp
