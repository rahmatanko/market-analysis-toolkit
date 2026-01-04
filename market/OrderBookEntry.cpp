#include "OrderBookEntry.h"

// starter code – initializes a market order entry
OrderBookEntry::OrderBookEntry(double _price,
                               double _amount,
                               std::string _timestamp,
                               std::string _product,
                               OrderBookType _orderType,
                               std::string _username)
    : price(_price),
      amount(_amount),
      timestamp(_timestamp),
      product(_product),
      orderType(_orderType),
      username(_username)
{
    // nothing else to do here
}

// starter code – convert string from CSV → enum type
OrderBookType OrderBookEntry::stringToOrderBookType(std::string s) {
    if (s == "ask") {
        return OrderBookType::ask;
    }
    if (s == "bid") {
        return OrderBookType::bid;
    }
    // fallback for unknown/unsupported types
    return OrderBookType::unknown;
}
