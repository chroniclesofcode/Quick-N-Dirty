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

void test2() {
    // testing the resize feature, to see if it resizes properly
    qnd::hashmap<int, int> mp(10);
    int ct = 100;
    for (int i = 0; i < 100; i++) {
        //std::cout << i << ": inserting " << i*2 << " -> " << ct << '\n';
        mp[i*2] = ct;
        ct--;
    }
    ct = 1;
    for (int i = 99; i >= 0; i--) {
        //std::cout << "extracting " << i*2 << " as " << mp[i*2] << '\n';
        assert(mp[i*2] = ct++);
    }
}

void test3() {
    // testing the custom hash function + collisions
    struct CustomH {
        std::size_t operator() (const int key) {
            return key % 5;
        }
    };
    qnd::hashmap<int, int, CustomH> mp;
    // 5->6, 10->11, 15->16, 20->21, 25->26.
    mp[6] = 10;
    mp[11] = 9;
    mp[16] = 8;
    mp[21] = 7;
    mp[26] = 6;
    assert(mp[6] == 10);
    assert(mp[11] == 9);
    assert(mp[16] == 8);
    assert(mp[21] == 7);
    assert(mp[26] == 6);
    mp[16] = 500;
    assert(mp[16] == 500);
    assert(mp.erase(21) == 1);
    // 30->31 should collision, but should get inserted in 21's spot.
    mp[31] = 1000;
    assert(mp[31] == 1000);
    assert(mp.erase(21) == 0);
}

int main(void) {
    //test1();
    //test2();
    test3();
    return 0;
}