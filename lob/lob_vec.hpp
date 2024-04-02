/*
    Requirements:
        add buy/sell orders
            need to check if match - O(1) top of book
        cancel current orders
            need to be able to access and remove orders in O(1)
                -> linked list, or vector + tombstone
        market depth: volume at a price
            unordered_map O(1)
    Structures:
        std::map to prices->LL to pair of price, quantity or a vector of shared ptrs to price, quantity
        unordered_map to price->volume, need bookkeeping in every function
        unordered_map of id? to LL iterator, or shared ptr of optionals.

    Extension:
        unordered_map of price to list iterator -> fast insertion at price point
    
    Notes: 
    cancelorder should get rid of price level
    make sure to break on quantity == 0, and remove price level BEFORE that
    when cancelling, do not invalidate references to object!!!
*/

#pragma once
#include <cstdint>
#include <vector>
#include <map>
#include <unordered_map>
#include <functional>
#include <iterator>
#include <iostream>
#include <memory>

class LOB {
private:
    using OrderID = uint64_t;
    using Price = int32_t;
    using Quantity = uint64_t;
    struct Order {
        Order(Price p, Quantity q, int s) : price{p}, quantity{q}, side{s} {}
        Price price;
        Quantity quantity;
        int side;
    };
    using OrderPtr = std::unique_ptr<Order>;
    using OrderList = std::vector<OrderPtr>;
public:

    LOB() {

    }

    void printBook() {

    }

    void addOrder(OrderID id, int32_t price, uint64_t quantity, int side) {
        if (side == 0) {
            // It's a buy - match with the lowest ask
            while (m_asks.size()) {
                auto it = m_asks.begin();
                Price top_price = it->first;
                if (!(price >= top_price))
                    break;
                for (OrderPtr &o : it->second) {
                    if (o->quantity == 0) continue;
                    Quantity rem = std::min(quantity, o->quantity);
                    quantity -= rem;
                    o->quantity -= rem;
                    m_vol[top_price] -= rem;
                    if (quantity == 0) break;
                }
                if (m_vol[top_price] == 0) {
                    m_asks.erase(it);
                }
                if (quantity == 0) break;
            }
            // If quantity remaining, add to book
            if (quantity == 0) return;
            auto &orders = m_bids[price];
            orders.push_back(std::make_unique<Order>( price, quantity, side ));
            m_order[id] = orders.back().get();
            m_vol[price] += quantity;
        } else {
            while (m_bids.size()) {
                auto it = m_bids.begin();
                Price top_price = it->first;
                if (!(price <= top_price))
                    break;
                for (OrderPtr &o : it->second) {
                    if (o->quantity == 0) continue;
                    Quantity rem = std::min(quantity, o->quantity);
                    quantity -= rem;
                    o->quantity -= rem;
                    m_vol[top_price] -= rem;
                    if (quantity == 0) break;
                }
                if (m_vol[top_price] == 0) {
                    m_bids.erase(it);
                }
                if (quantity == 0) break;
            }
            // If quantity remaining, add to book
            if (quantity == 0) return;
            auto &orders = m_asks[price];
            orders.push_back(std::make_unique<Order>( price, quantity, side ));
            m_order[id] = orders.back().get();
            m_vol[price] += quantity;
        }
    }

    void cancelOrder(OrderID id) {
        Order* o = m_order[id];
        m_vol[o->price] -= o->quantity;
        o->quantity = 0;
        if (m_vol[o->price] == 0) {
            if (o->side == 0) m_bids.erase(o->price);
            else m_asks.erase(o->price);
        }
        m_order.erase(id);
    }

    uint64_t getDepth(int32_t price) {
        return m_vol[price];
    }
private:
    std::map<Price, OrderList, std::greater<Price>> m_bids;
    std::map<Price, OrderList, std::less<Price>> m_asks;
    std::unordered_map<OrderID, Order*> m_order;
    std::unordered_map<Price, Quantity> m_vol;
};