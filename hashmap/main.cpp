#include <iostream>
#include <cassert>
#include "hashmap.hpp"

void test1() {
    qnd::hashmap<int, int> mp;
    // testing modification
    mp[35] = 200;
    assert(50 + mp[35] == 250);
    // testing changing same value
    mp[35] = 90;
    assert(mp[35] == 90);
    // testing large int
    mp[987654] = 123456;
    assert(mp[987654] == 123456);
    int testint = 25 + mp[987654];
    assert(testint == 123481);
    // testing ++, as well as map storing multiple values
    mp[35]++;
    assert(mp[35] == 91);
}

int main(void) {
    test1();
    
    return 0;
}