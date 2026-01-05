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

    // get balance in usdt
    double getBalance(const std::string& type) const;
    // get all balances as a map, needed for WalletManager saveWallet
    const std::map<std::string, double> getAllBalances() const
    {
        return currencies; // just return a copy
    }

private:
    std::map<std::string,double> currencies;
};
