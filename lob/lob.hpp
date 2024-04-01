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
#include <list>
#include <map>
#include <unordered_map>
#include <functional>
#include <iterator>
#include <iostream>

class LOB {
private:
    using OrderID = uint64_t;
    using Price = int32_t;
    using Quantity = uint64_t;
    struct Order {
        Price price;
        Quantity quantity;
        int side;
    };
    using OrderList = std::list<Order>;
public:

    LOB() {

    }

    void printBook() {
        std::cout << "buys: \n";
        for (auto &e : m_bids) {
            std::cout << "price: " << e.first << ": ";
            for (auto &l : e.second) {
                std::cout << l.quantity << ' ';
            }
            std::cout << '\n';
        }
        std::cout << "\nsells: \n";
        for (auto &e : m_asks) {
            std::cout << "price: " << e.first << ": ";
            for (auto &l : e.second) {
                std::cout << l.quantity << ' ';
            }
            std::cout << '\n';
        }
    }

    void addOrder(OrderID id, int32_t price, uint64_t quantity, int side) {
        // Buy side
        if (side == 0) {
            // Check if matches with a sell order
            while (m_asks.size()) {
                OrderList &orders = m_asks.begin()->second;
                if (!(price >= m_asks.begin()->first)) 
                    break;
                while (!orders.empty()) {
                    Order &curr = *orders.begin();
                    Quantity rem = std::min(quantity, curr.quantity);
                    quantity -= rem;
                    curr.quantity -= rem;
                    m_vol[curr.price] -= rem;
                    if (curr.quantity == 0) orders.pop_front();
                    if (quantity == 0) break;
                }
                if (orders.empty())
                    m_asks.erase(m_asks.begin());
                if (quantity == 0)
                    break;
            }
            // If there is order remaining, add to m_bids
            if (quantity == 0) 
                return;
            m_bids[price].push_back({ price, quantity, side });
            m_order[id] = std::prev(m_bids[price].end());
            m_vol[price] += quantity;
        } else {
            // Check if matches with a sell order
            while (m_bids.size()) {
                OrderList &orders = m_bids.begin()->second;
                if (!(price <= m_bids.begin()->first)) 
                    break;
                while (!orders.empty()) {
                    Order &curr = *orders.begin();
                    Quantity rem = std::min(quantity, curr.quantity);
                    quantity -= rem;
                    curr.quantity -= rem;
                    m_vol[curr.price] -= rem;
                    if (curr.quantity == 0) orders.pop_front();
                    if (quantity == 0) break;
                }
                if (orders.empty())
                    m_bids.erase(m_bids.begin());
                if (quantity == 0)
                    break;
            }
            // If there is order remaining, add to m_bids
            if (quantity == 0) 
                return;
            m_asks[price].push_back({ price, quantity, side });
            m_order[id] = std::prev(m_asks[price].end());
            m_vol[price] += quantity;
        }
    }

    void cancelOrder(OrderID id) {
        auto it = m_order[id];
        Order &o = *it;
        Price tmp_price = o.price;
        m_vol[o.price] -= o.quantity;
        if (o.side == 0) {
            auto &orders = m_bids[o.price];
            orders.erase(it);
            if (orders.empty())
                m_bids.erase(tmp_price);
        } else {
            auto &orders = m_asks[o.price];
            orders.erase(it);
            if (orders.empty())
                m_asks.erase(tmp_price);
        }
        m_order.erase(id);
    }

    uint64_t getDepth(int32_t price) {
        return m_vol[price];
    }
private:
    std::map<Price, OrderList, std::greater<Price>> m_bids;
    std::map<Price, OrderList, std::less<Price>> m_asks;
    std::unordered_map<OrderID, OrderList::iterator> m_order;
    std::unordered_map<Price, Quantity> m_vol;
};