#pragma once
#include <vector>
#include <string>
#include <random>

#include "../market/MarketDataLoader.h"
#include "../wallet/WalletManager.h"
#include "../user/User.h"

// ANSI colors
#define RESET   "\033[0m"
#define BOLD    "\033[1m"
#define CYAN    "\033[36m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define RED     "\033[31m"
#define MAGENTA "\033[35m"
#define GRAY    "\033[90m"


// personal implementation
// responsible for simulating user trading activity (task 4)
// generates ask and bid orders and updates wallet state accordingly
class TradeSimulator
{
public:
    // constructor receives shared market data and wallet manager
    // no ownership here, just references
    TradeSimulator(MarketDataLoader& marketData,
                   WalletManager& walletManager);

    // simulate trades for all products for a given logged-in user
    void simulateTrades(const User& user);

private:
    // access to market prices (read-only)
    MarketDataLoader& marketData;

    // handles wallet updates and csv persistence
    WalletManager& walletManager;

    // helper: get current system timestamp as string
    std::string getCurrentTimestamp() const;

    // helper: generate a realistic price based on historical price
    double generatePrice(double basePrice);
};
