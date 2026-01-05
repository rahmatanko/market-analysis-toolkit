#pragma once
#include <string>
#include "../market/OrderBookEntry.h"

// personal implementation
// simple wrapper used for simulated user trades in task 4
// we reuse OrderBookEntry directly so the rest of the system
// can treat simulated trades like normal market orders
class Trade
{
public:
    // the underlying order data (price, amount, timestamp, etc.)
    OrderBookEntry order;

    // constructor just stores the given order
    Trade(const OrderBookEntry& order)
        : order(order) {}
};
