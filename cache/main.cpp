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
*/
using Key = int;
using Value = std::string;
/*
constexpr int L1_SZ = 5;
constexpr int L2_SZ = 10;
constexpr int L3_SZ = 20;
*/
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
    
    void put(const Key& k, Value v) {
        strat->use(k);
        auto it = cache.find(k);
        if (it != cache.end()) {
            it->second = v;
        } else {
            if (cache.size() > sz - 1) {
                cache.erase(strat->evict());
            }
            cache[k] = v;
        }
    }
    virtual std::optional<Value> get(Key k) {
        auto it = cache.find(k);
        if (it == cache.end()) {
            return std::nullopt;
        }
        strat->use(k);
        return it->second;
    }
private:
    int sz;
    std::unique_ptr<IStrategy> strat;
    std::unordered_map<Key, Value> cache;
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
    std::cout << "TESTS PASSED\n";
    return 0;
}