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
    struct Elem {
        Elem(Key&& k, T&& v, std::size_t hsh) : key{std::move(k)}, v{std::move(v)}, hash{hsh} {}
        Key key;
        T val;
        std::size_t hash;
    }

    using Buckets = std::vector<std::optional<Elem>>;
    
    rh_hashmap() {
        buckets.resize(size_b);
    }

    rh_hashmap(std::size_t size) : size_b{(size)*load_factor + 1} {
        buckets.resize(size_b);
    }

    inline std::size_t calc_hash(const Key& key) {
        return (Hash{}(key) % (size_b-1)) + 1;
    }

    void resize() {
        size_b *= load_factor;
        count = 0;
        Buckets tmp(size_b, std::nullopt);
        swap(buckets, tmp);
        for (auto e : tmp) {
            if (e.has_value()) {
                (*this)[e.value().key] = e.value().val;
            }
        }
    }

    std::size_t erase(const Key& key) {
        /*
        std::size_t idx = hash(key);
        while (buckets[idx].has_value() && buckets[idx].value().first != key) {
            ++idx;
            if (unlikely(idx == size_b)) idx = 0;
        }
        if (buckets[idx].has_value()) {
            buckets[idx].reset();
            --count;
            return 1;
        }
        return 0;
        */
    }

    T& operator[](Key&& key) {
        if (unlikely(count >= size_b / load_factor)) {
            resize();
        }
        std::size_t chash = calc_hash(key);
        std::size_t idx = chash;
        std::size_t pd = 0; // current probe distance
        T tmp_val = T{};
        while (1) {
            if (!buckets[idx].has_value()) {
                buckets[idx] = Elem(key, T{}, chash);
                ++count;
                return buckets[idx].value().val;
            } else if (buckets[idx].value().key == key) {
                return buckets[idx].value().val;
            }
            std::size_t tmp_dist = probe_dist(idx, buckets[idx].value().hash);
            if (pd > tmp_dist) {
                swap()
            }

            ++pd;
            ++idx;
            if (unlikely(idx == size_b)) idx = 1;
        }
        return ret;
    }

private:
    inline std::size_t probe_dist(std::size_t idx, std::size_t curr) {
        std::size_t diff = std::abs(curr - idx);
        return std::min(diff, size_b-1 - diff);
    }

    std::size_t size_b = 1024;
    std::size_t count = 0;
    std::size_t load_factor = 2;
    Buckets buckets;
};


}