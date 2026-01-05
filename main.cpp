#include <iostream>
#include <vector>
#include "../market/MarketDataLoader.h"
#include "../wallet/WalletManager.h"
#include "../user/User.h"
#include "../trading/TradeSimulator.h"

int main() 
{
    // load market data (starter dataset, task 1)
    std::string marketCSV = "data/small_market_data.csv";
    MarketDataLoader marketData(marketCSV);

    // initialize wallet manager with wallets + transaction csv
    std::string walletsCSV = "data/test_wallets.csv"; // test wallet already has some data
    std::string transactionsCSV = "data/test_transactions.csv";
    WalletManager walletManager(walletsCSV, transactionsCSV);

    // create a sample user manually (normally would load from user csv)
    std::string fullName = "Alice Example";
    std::string email = "alice@example.com";
    std::string hashedPassword = "dummyhashed"; // just for testing
    std::string userID = "1234567890";
    std::string walletID = userID; // 1:1 mapping for simplicity
    User user(fullName, email, hashedPassword, userID, walletID);

    // print info to show we loaded user
    std::cout << "user: " << fullName << " | id: " << user.getUserID() << "\n";

    // initialize trade simulator
    TradeSimulator simulator(marketData, walletManager);

    // run simulation for the test user
    std::cout << "simulating trades...\n";
    simulator.simulateTrades(user);

    // reload wallet to show updated balances
    std::cout << "loading wallet after trades...\n";
    Wallet wallet = walletManager.loadWallet(user.getWalletID());
    std::cout << wallet << "\n"; // 

    // print recent transactions to confirm trades
    std::cout << "recent transactions (last 5):\n";
    std::vector<Transaction> recentTxs = walletManager.getRecentTransactions(user.getUserID(), 5);
    for (const auto& tx : recentTxs)
    {
        std::cout << "[" << tx.timestamp << "] "
                  << Transaction::typeToString(tx.type) << " "
                  << tx.product << " "
                  << tx.amount << " | balance after: " 
                  << tx.balanceAfter << "\n";
    }

    // done
    std::cout << "task 4 simulation test complete\n";
    return 0;
}
