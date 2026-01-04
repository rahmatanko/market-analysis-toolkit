#pragma once
#include <string>

// starter code – represents one market order
enum class OrderBookType { bid, ask, unknown, asksale, bidsale };

class OrderBookEntry {
public:
    // constructor – creates a trade/order object
    OrderBookEntry(double _price,
                   double _amount,
                   std::string _timestamp,
                   std::string _product,
                   OrderBookType _orderType,
                   std::string username = "dataset");

    // helper: convert string to enum
    static OrderBookType stringToOrderBookType(std::string s);

    // sorting helpers
    static bool compareByTimestamp(OrderBookEntry& e1, OrderBookEntry& e2) {
        // orders sorted earliest → latest
        return e1.timestamp < e2.timestamp;
    }

    static bool compareByPriceAsc(OrderBookEntry& e1, OrderBookEntry& e2) {
        // low → high price
        return e1.price < e2.price;
    }

    static bool compareByPriceDesc(OrderBookEntry& e1, OrderBookEntry& e2) {
        // high → low price
        return e1.price > e2.price;
    }

    // core order properties
    double price;            // order price
    double amount;           // order amount
    std::string timestamp;   // string timestamp e.g. "2020-03-17 12:00:00"
    std::string product;     // product pair e.g. "ETH/USDT"
    OrderBookType orderType; // bid / ask / etc.
    std::string username;    // source of the order
};
