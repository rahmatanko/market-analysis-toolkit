#pragma once

#include <string>
#include <map>
#include "../market/OrderBookEntry.h"
#include <iostream>

// reused code
class Wallet 
{
public:
    Wallet();
    void insertCurrency(std::string type, double amount);
    bool removeCurrency(std::string type, double amount);
    bool containsCurrency(std::string type, double amount);
    bool canFulfillOrder(OrderBookEntry order);
    void processSale(OrderBookEntry& sale);
    std::string toString();
    friend std::ostream& operator<<(std::ostream& os, Wallet& wallet);

    // personal implementation: get actual balance
    double getBalance(const std::string& type) const;

private:
    std::map<std::string,double> currencies;
};
