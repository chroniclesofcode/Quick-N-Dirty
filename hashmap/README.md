# Hashmap

- Linear chaining
- Load factor of 0.25
- Default size 1024
- Custom Hash function allowed, default std::hash
- TO:DO Make separate Robin-Hood hashmap and benchmark

# Implementation
- Optionals for bucket array : `std::vector<std::optional<std::pair<Key, T>>>`
- __builtin_expect for modulo when probing

# Notes

Despite there being an erase functionality in hashmap.cpp - do NOT use it for
hashmap.hpp. It uses optionals so it cannot distinguish between tombstone and
empty element. However, the rh_hashmap (Robin Hood Hashing) is able to delete,
as well as having a much more optimal hashing. You probably want to use the
rh_hashmap for most purposes.

# Benchmark

Can see in stats folder.
Generated 1e6 random numbers from 1 to 1e9 and added them into the hashmap.
Then, extracted them from the hashmap and added them to a sum.
Timed 100 times. 
Note: set load factors as 0.25.

Average qnd::hashmap - 41ms.
Average std::unordered_map - 155ms.

More details like variance included in the stats folder.