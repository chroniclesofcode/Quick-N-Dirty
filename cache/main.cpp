#include <bits/stdc++.h>

/*
    top level class interface: ICache
    l1, l2, l3 inherit from ICache
    Different eviction strategies? LRU, LFU, FIFO
    -> strategy pattern -> each cache will have an eviction strategy
       that will get passed in
    things ejected from higher level caches will find themselves in lower
    level caches

    - Identify interfaces, and then implement the functions
    - strategy will maintain housekeeping -> data structures of keys
    - whilst the cache will manage the storing of the value itself

    Multiple l1, l2, l3 implementations can give you tailored behaviour
    towards each cache itself (in a real world scenario). However in this
    situation, it's better to create a single cache class and l1, l2, l3
    will just be instantiations of this class, but with different strategies
    and sizes passed in.

    The actual cache manager will manage the three levels of cache. If the
    request does not exist in any caches, it will return 0, i.e a cache miss

    The problem with L2, L3 caches are every time you pull from it, you actually
    want to put it in the L1 cache. You would think you need to delete from the
    L2, L3 caches but my question is: why not leave them there? When the time
    comes, they will get evicted based on the nature of the technique. So the
    only thing you need to do when L2, L3 have a hit, is also add it to the
    L1 cache.
*/
using Key = int;
using Value = std::string;

constexpr int L1_SZ = 3;
constexpr int L2_SZ = 6;
constexpr int L3_SZ = 10;

class IStrategy {
public:
    virtual ~IStrategy() {}
    virtual void use(const Key& k) = 0;
    virtual Key evict() = 0;
};

class LRU : public IStrategy {
public:
    void use(const Key& k) override {
        auto it = m.find(k);
        if (it == m.end()) {
            // not found, have to insert new to the back
            l.push_back(k);
            m[k] = std::prev(l.end());
        } else {
            if (l.back() == k)
                return;
            auto lit = it->second; 
            l.erase(lit);
            l.push_back(k);
            m[k] = std::prev(l.end());
        }
    }

    Key evict() override {
        Key rm = l.front();
        l.erase(l.begin());
        m.erase(rm);
        return rm;
    }
private:
    // In this list, we eject the l.front() and insert to the back
    std::list<Key> l;
    std::unordered_map<Key, std::list<Key>::iterator> m;
};

class Cache {
public:
    Cache(int n, std::unique_ptr<IStrategy> &&s) : sz{n}, strat{std::move(s)} {}
    
    std::optional<std::pair<Key, Value>> put(const Key& k, Value v) {
        strat->use(k);
        auto it = cache.find(k);
        if (it != cache.end()) {
            it->second = v;
        } else {
            if (cache.size() > sz - 1) {
                Key rm = strat->evict();
                auto it = cache.find(rm);
                std::pair<Key, Value> ret = { rm, it->second };
                cache.erase(it);
                cache[k] = v;
                return ret;
            }
            cache[k] = v; 
        }
        return std::nullopt;
    }

    std::optional<Value> get(Key k) {
        auto it = cache.find(k);
        if (it == cache.end()) {
            return std::nullopt;
        }
        strat->use(k);
        return it->second;
    }

    void print() {
        for (auto &[k, v] : cache) {
            std::cout << k << ":" << v << " | ";
        }
        std::cout << '\n';
    }

private:
    int sz;
    std::unique_ptr<IStrategy> strat; // Can't use IStrategy since it's abstract class, must be ptr
    std::unordered_map<Key, Value> cache;
};

class CacheManager {
public:

    CacheManager(int s1, int s2, int s3) : L1(s1, std::make_unique<LRU>()), L2(s2, std::make_unique<LRU>()),
                                            L3(s3, std::make_unique<LRU>()) {}

    void insert(Key k, Value v) {
        std::optional<std::pair<Key, Value>> ev1 = L1.put(k, v);
        if (ev1 != std::nullopt) {
            std::optional<std::pair<Key, Value>> ev2 = L2.put(ev1->first, ev1->second);
            if (ev2 != std::nullopt) {
                L3.put(ev2->first, ev2->second);
            }
        }
    }

    std::optional<Value> retrieve(Key k) {
        std::optional<Value> v = L1.get(k);
        if (v == std::nullopt) {
            v = L2.get(k);
            if (v == std::nullopt) {
                v = L3.get(k);
            }
        }
        if (v != std::nullopt) {
            insert(k, *v);
        }
        return v;
    }

    void print() {
        L1.print();
        L2.print();
        L3.print();
    }

private:
    Cache L1, L2, L3;
};

int main(void) {
    // Small tests
    Cache c(3, std::make_unique<LRU>());
    c.put(1, "one");
    c.put(2, "two");
    c.put(3, "three");
    c.put(4, "four");
    assert(c.get(1) == std::nullopt);
    assert(c.get(2) == "two"); // Note optional overrides ==
    c.put(1, "one");
    assert(c.get(3) == std::nullopt);
    assert(c.get(10) == std::nullopt);

    CacheManager cm(L1_SZ, L2_SZ, L3_SZ);

    for (int i = 1; i <= 19; i++) {
        cm.insert(i, std::to_string(i));
    }

    assert(cm.retrieve(20) == std::nullopt);
    cm.print();
    cm.retrieve(7);
    cm.retrieve(14);
    cm.insert(7, "notseven");
    cm.insert(20, "twenty");
    cm.print();

    std::cout << "TESTS PASSED\n";
    return 0;
}