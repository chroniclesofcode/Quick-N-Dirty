#include <vector>

namespace qnd {

template <typename T, typename U>
class hashmap {
public:
    
    hashmap() {
        buckets = std::vector<T>(size);
    }

    std::size_t hash(const T& elem) {

    }




private:
    std::size_t size = 1024;
    std::vector<T> buckets;
};


}