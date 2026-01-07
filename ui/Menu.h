#pragma once
#include "../user/UserManager.h"
#include "../wallet/WalletManager.h"
#include "../trading/TradeSimulator.h"
#include "../market/MarketDataLoader.h"
#include "../market/Candlestick.h"
#include <string>
#include <memory>
#include <vector>
#include <iomanip>

// personal implementation for this project
class Menu {
public:
    // constructor: store references to the main managers, no copies
    Menu(UserManager& userManager,
         WalletManager& walletManager,
         MarketDataLoader& marketData);

    // main loop: keeps showing menus until exit
    void run();

private:
    UserManager& userManager;
    WalletManager& walletManager;
    MarketDataLoader& marketData;

    User currentUser; // current logged-in user
    bool loggedIn = false; // login state

    // menus
    void showWelcomeMenu(); // login/register
    void showMainMenu(); // after login

    // actions 
    void handleRegister(); // register new user
    void handleLogin(); // login existing user
    void handleDeposit(); // deposit funds
    void handleWithdraw(); // withdraw funds
    void handleViewWallet(); // view wallet balances
    void handleSimulateTrades(); // run trade simulation
    void handleViewTransactions(); // last few transactions
    void handlePrintStatistics(); // user stats
    void handleViewCandlesticks(); // view candlestick chart for a product

    // helpers
    std::string promptString(const std::string& msg); // read line from stdin
    double promptDouble(const std::string& msg); // read double from stdin
};
