#include <vector>
#include <iostream>
#include <optional>

#define likely(expr) __builtin_expect(expr, 1)
#define unlikely(expr) __builtin_expect(expr, 0)

namespace qnd {

template <typename Key, typename T, typename Hash = std::hash<Key>>
class hashmap {
public:
    using Buckets = std::vector<std::optional<std::pair<Key, T>>>;
    
    hashmap() {
        buckets.resize(size_b);
    }

    hashmap(std::size_t size) : size_b{size*4} {
        buckets.resize(size_b);
    }

    inline std::size_t hash(const Key& elem) {
        return Hash{}(elem) % size_b;
    }

    void resize() {
        size_b *= 2;
        count = 0;
        Buckets tmp(size_b, std::nullopt);
        swap(buckets, tmp);
        for (auto e : tmp) {
            if (e.has_value()) {
                (*this)[e.value().first] = e.value().second;
            }
        }
    }

    std::size_t erase(const Key& elem) {
        std::size_t idx = hash(elem);
        while (buckets[idx].has_value() && buckets[idx].value().first != elem) {
            ++idx;
            if (unlikely(idx == size_b)) idx = 0;
        }
        if (buckets[idx].has_value()) {
            buckets[idx].reset();
            --count;
            return 1;
        }
        return 0;
    }

    T& operator[](const Key& elem) {
        if (unlikely(count >= size_b / 4)) {
            resize();
        }
        std::size_t idx = hash(elem);
        while (buckets[idx].has_value() && buckets[idx].value().first != elem) {
            ++idx;
            if (unlikely(idx == size_b)) idx = 0;
        }
        if (!buckets[idx].has_value()) {
            buckets[idx] = std::pair<Key, T>(elem, T{});
            ++count;
        } 
        return buckets[idx].value().second;
    }

private:
    std::size_t size_b = 1024;
    std::size_t count = 0;
    Buckets buckets;
};


}