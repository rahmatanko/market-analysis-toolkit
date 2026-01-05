#include <iostream>
#include <string>
#include "wallet/WalletManager.h"
#include "wallet/Wallet.h"
#include "wallet/Transaction.h"

int main() {
    // fake CSV files for testing
    std::string walletsCSV = "data/test_wallets.csv";
    std::string transactionsCSV = "data/test_transactions.csv";

    // create WalletManager
    WalletManager wm(walletsCSV, transactionsCSV);

    // test user
    std::string userId = "1234567890";

    // load wallet (should be empty at first)
    Wallet wallet = wm.loadWallet(userId);
    std::cout << "initial wallet:\n" << wallet << std::endl;

    // deposit funds
    std::cout << "depositing 1000 USDT...\n";
    wm.deposit(userId, wallet, 1000);
    std::cout << "wallet after deposit:\n" << wallet << std::endl;

    // withdraw some funds
    std::cout << "withdrawing 250 USDT...\n";
    wm.withdraw(userId, wallet, 250);
    std::cout << "wallet after withdrawal:\n" << wallet << std::endl;

    // simulate ask transaction (personal implementation)
    Transaction askTx(userId, TransactionType::ASK, "BTC/USDT", 0.5, wallet.containsCurrency("USDT",0) ?  wallet.containsCurrency("USDT",0) : 0, "2026-01-04 16:00:00");
    wm.deposit(userId, wallet, 0); // just to trigger logging example
    wm.deposit(userId, wallet, 0); // dummy deposits to log ask/bid

    // simulate bid transaction (personal implementation)
    Transaction bidTx(userId, TransactionType::BID, "ETH/USDT", 2, wallet.containsCurrency("USDT",0) ?  wallet.containsCurrency("USDT",0) : 0, "2026-01-04 16:05:00");
    wm.deposit(userId, wallet, 0); // dummy

    // print recent transactions
    std::cout << "\nrecent transactions:\n";
    auto recent = wm.getRecentTransactions(userId, 5);
    for (auto& tx : recent) {
        std::cout << tx.userId << " | " 
                  << Transaction::typeToString(tx.type) << " | "
                  << tx.product << " | "
                  << tx.amount << " | "
                  << tx.balanceAfter << " | "
                  << tx.timestamp;
        std::cout << "\n";
    }

    // print summary statistics
    std::cout << "\nstatistics for user " << userId << ":\n";
    wm.printStatistics(userId);

    return 0;
}
