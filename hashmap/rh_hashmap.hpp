#include <vector>
#include <iostream>
#include <optional>
#include <cmath>

#define likely(expr) __builtin_expect(expr, 1)
#define unlikely(expr) __builtin_expect(expr, 0)

namespace qnd {

template <typename Key, typename T, typename Hash = std::hash<Key>>
class rh_hashmap {
public:
    using Buckets = std::vector<std::optional<std::pair<Key, T>>>;
    
    rh_hashmap() {
        buckets.resize(size_b);
    }

    rh_hashmap(std::size_t size) : size_b{size*4} {
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

    T& operator[](Key&& elem) {
        if (unlikely(count >= size_b / 4)) {
            resize();
        }
        std::size_t idx = hash(elem);
        std::size_t pd = 0; // current probe distance
        std::size_t ret = -1;
        while (1) {
            if (!buckets[idx].has_value()) {
                buckets[idx] = std::pair<Key, T>(elem, T{});
                ++count;
                return buckets[idx].value().second;
            } else if (buckets[idx].value().first == elem) {
                return buckets[idx].value().second;
            }
            // Swap element and curr value if probe_dist > pd
            std::size_t tmp_hash = hash(buckets[idx].value().first);
            std::size_t tmp_dist = probe_dist(idx, tmp_hash);
            if (pd > tmp_dist) {
                swap()
            }

            ++pd;
            ++idx;
            if (unlikely(idx == size_b)) idx = 0;
        }
        return ret;
    }

private:
    inline std::size_t probe_dist(std::size_t idx, std::size_t curr) {
        std::size_t diff = std::abs(curr - idx);
        return std::min(diff, size_b - diff);
    }

    std::size_t size_b = 1024;
    std::size_t count = 0;
    Buckets buckets;
};


}