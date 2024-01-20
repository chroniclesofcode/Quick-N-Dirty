# Hashmap

- Linear chaining
- Load factor of 0.25
- Default size 1024
- Key erase functionality
- Custom Hash function allowed, default std::hash

# Implementation
- Optionals for bucket array : `std::vector<std::optional<std::pair<Key, T>>>`
- __builtin_expect for modulo when probing
