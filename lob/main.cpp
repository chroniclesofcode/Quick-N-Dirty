#include <iostream>
#include "lob.hpp"
#include <cassert>

LOB book;
uint64_t id_ct = 1;

void addBuy(int price, uint64_t quantity) {
    book.addOrder(id_ct++, price, quantity, 0);
}

void addSell(int price, uint64_t quantity) {
    book.addOrder(id_ct++, price, quantity, 1);
}

void cancelOrder(uint64_t id) {
    book.cancelOrder(id);
}

uint64_t getDepth(int price) {
    return book.getDepth(price);
}

int main(void) {
    addBuy(50, 2); // 1
    assert(getDepth(50) == 2);
    cancelOrder(1);
    assert(getDepth(50) == 0);
    addBuy(10, 3); // 2
    addBuy(9, 5); // 3
    addBuy(10, 2); // 4
    addBuy(9, 1); // 5
    addBuy(9, 4); // 6
    addBuy(8, 4); // 7
    assert(getDepth(10) == 5); 
    assert(getDepth(9) == 10); 
    assert(getDepth(8) == 4);

    addSell(11, 3); // 8
    addSell(11, 5); // 9
    addSell(12, 10); // 10
    addSell(13, 2); // 11
    addSell(12, 2); // 12
    assert(getDepth(11) == 8);
    assert(getDepth(12) == 12);
    assert(getDepth(13) == 2);

    addBuy(11, 2); // 13
    assert(getDepth(11) == 6);
    addBuy(11, 1); // 14
    assert(getDepth(11) == 5);
    addSell(10, 3); // 15
    assert(getDepth(10) == 2);
    addBuy(13, 10); // 16
    assert(getDepth(11) == 0);
    assert(getDepth(12) == 7);
    assert(getDepth(13) == 2);
    addSell(9, 20); // 17
    assert(getDepth(10) == 0);
    assert(getDepth(9) == 8);
    addSell(6, 15); // 18
    assert(getDepth(6) == 11);
    assert(getDepth(7) == 0);
    assert(getDepth(8) == 0);
    addBuy(6, 11); // 19
    assert(getDepth(6) == 0);

    addBuy(6, 1); // 20
    addBuy(6, 2); // 21
    addBuy(6, 3); // 22
    addBuy(6, 4); // 23
    assert(getDepth(6) == 10);
    cancelOrder(21);
    assert(getDepth(6) == 8);
    addSell(5, 2); // 24
    assert(getDepth(6) == 6);
    addSell(5, 5); // 25
    assert(getDepth(6) == 1);
    addSell(2, 2); // 26
    assert(getDepth(2) == 1);
    cancelOrder(26);
    assert(getDepth(2) == 0);

    assert(getDepth(10) == 0);
    assert(getDepth(9) == 8);
    addBuy(10, 9); // 27
    assert(getDepth(10) == 1);
    assert(getDepth(9) == 0);
    addBuy(10, 2); // 28
    assert(getDepth(10) == 3);
    addBuy(10, 3); // 29
    assert(getDepth(10) == 6);
    addBuy(10, 4); // 30
    assert(getDepth(10) == 10);
    cancelOrder(28);
    cancelOrder(30);
    assert(getDepth(10) == 4);
    addSell(9, 5); // 31
    assert(getDepth(9) == 1);
    assert(getDepth(10) == 0);
    addBuy(12, 7); // 32
    assert(getDepth(12) == 1);
    return 0;
}