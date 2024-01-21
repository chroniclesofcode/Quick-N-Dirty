# Hashmap

- Linear chaining
- Load factor of 0.25
- Default size 1024
- Key erase functionality
- Custom Hash function allowed, default std::hash

# Implementation
- Optionals for bucket array : `std::vector<std::optional<std::pair<Key, T>>>`
- __builtin_expect for modulo when probing

# Benchmark

Can see in stats folder.
Generated 1e6 random numbers from 1 to 1e9 and added them into the hashmap.
Then, extracted them from the hashmap and added them to a sum.
Timed 100 times. 
Note: set load factors as 0.25.

Average qnd::hashmap - 41ms.
Average std::unordered_map - 155ms.

More details like variance included in the stats folder.