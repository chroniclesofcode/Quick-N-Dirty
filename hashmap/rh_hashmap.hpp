#include <vector>
#include <iostream>
#include <optional>
#include <cmath>
#include <cstdlib>
#include <utility>

#define likely(expr) __builtin_expect(expr, 1)
#define unlikely(expr) __builtin_expect(expr, 0)

namespace qnd {

template <typename Key, typename T, typename Hash = std::hash<Key>>
class rh_hashmap {
public:
    struct Elem {
        Elem(Key&& k, T&& v, std::size_t hsh) : key{std::move(k)}, val{std::move(v)}, hash{hsh} {}
        Key key;
        T val;
        std::size_t hash;
        bool is_del = false;
    };

    using Buckets = std::vector<std::optional<Elem>>;
    
    rh_hashmap() {
        buckets.resize(size_b);
    }

    rh_hashmap(std::size_t size) {
        size_b = size * load_factor;
        buckets.resize(size_b);
    }

    inline std::size_t calc_hash(const Key& key) {
        return Hash{}(key);
    }

    inline std::size_t calc_pos(const std::size_t hash) {
        return hash % size_b;
    }

    void resize() {
        size_b *= load_factor;
        count = 0;
        Buckets tmp(size_b, std::nullopt);
        std::swap(buckets, tmp);
        for (auto& opt : tmp) {
            if (opt.has_value() && !opt.value().is_del) {
                (*this)[std::move(opt.value().key)] = std::move(opt.value().val);
            }
        }
    }

    std::size_t erase(const Key& key) {
        std::size_t chash = calc_hash(key);
        std::size_t idx = calc_pos(chash);
        std::size_t pd = 0;

        while (1) {
            if (!buckets[idx].has_value()) {
                break;
            } else if (pd > probe_dist(idx, buckets[idx].value().hash)) {
                break;
            } else if (buckets[idx].value().hash == chash && buckets[idx].value().key == key) {
                buckets[idx].value().is_del = true;
                // --count;
                return 1;
            }

            ++idx;
            ++pd;
            if (unlikely(idx == size_b)) idx = 0;
        }
        return 0;
    }

    T& operator[](Key key) {
        if (unlikely(count >= size_b / load_factor)) {
            resize();
        }
        std::size_t chash = calc_hash(key);
        std::size_t idx = calc_pos(chash);
        T tmp_val = T{};

        std::size_t pd = 0; // current probe distance
        int32_t ret = -1;
        while (1) {
            if (!buckets[idx].has_value()) {
                buckets[idx] = Elem(std::move(key), std::move(tmp_val), chash);
                ++count;
                if (ret == -1) ret = idx;
                return buckets[ret].value().val;
            } else if (ret == -1 && buckets[idx].value().key == key) {
                return buckets[idx].value().val;
            }
            Elem& curr = buckets[idx].value();
            std::size_t tmp_dist = probe_dist(idx, curr.hash);
            if (pd > tmp_dist) {
                if (curr.is_del) {
                    curr = Elem(std::move(key), std::move(tmp_val), chash);
                    return curr.val;
                }
                std::swap(chash, curr.hash);
                std::swap(key, curr.key);
                std::swap(tmp_val, curr.val);
                pd = tmp_dist;
                if (ret == -1) ret = idx;
            }

            ++pd;
            ++idx;
            if (unlikely(idx == size_b)) idx = 0;
        }
        return buckets[ret].value().val;
    }

private:
    inline std::size_t probe_dist(std::size_t idx, std::size_t currh) {
        currh = calc_pos(currh);
        std::size_t diff = abs((int)currh - (int)idx);
        return std::min(diff, size_b - diff);
    }

    std::size_t size_b = 1024;
    std::size_t count = 0;
    std::size_t load_factor = 2;
    Buckets buckets;
};


}