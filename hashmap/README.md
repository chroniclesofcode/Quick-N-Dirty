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

# Robin Hood Hashing

A very strong hashing technique (nowadays slightly outdated) that determines the current probing distance and the probing distances of each element in each bucket. Basically, if you have probed longer than the element you are probing over, then you do a swap. Note: this even applies to deleted elements - so even if you probe into a deleted element, you have to have probed further than it to truly replace it - this stops the case of deleting an element, then instantly inserting an element in it's place -> invalidating the ones after it since you will force a swap. When you insert directly on a deleted element, you will still have probe -> stopping a lot of issues.

# Benchmark

Can see in stats folder.
Generated 1e6 random numbers from 1 to 1e9 and added them into the hashmap.
Then, extracted them from the hashmap and added them to a sum.
Timed 100 times. 
Note: set load factors as 0.5.

Average qnd::hashmap - 49ms.
Average std::unordered_map - 200ms.
Average std::rh_hashmap - 92ms.

More details like variance included in the stats folder. Definitely can make further optimizations - will do if I have time... these implementations are meant to be quick n dirty after all.

# Weaknesses

Since the struct I use for each key + value pair is fairly large (there is a boolean in there, and a 32 bit hash) - if your key is fairly large then you
won't be able to get many probes in before needing to fetch the next cache 
line. So performance may deteriorate in certain scenarios if your key is large enough - but for the integer types there shouldn't be a problem.