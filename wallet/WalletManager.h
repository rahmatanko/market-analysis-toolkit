#pragma once
#include "Wallet.h"
#include "Transaction.h"
#include "../core/CSVReader.h"
#include "../core/DateUtils.h"
#include <vector>

// personal implementation: manages wallets + transaction history
class WalletManager
{
public:
    WalletManager(const std::string& walletFile,
                  const std::string& transactionFile);

    // load a wallet from csv
    Wallet loadWallet(const std::string& userId);

    // save current wallet state
    void saveWallet(const std::string& userId, Wallet& wallet);

    // deposit and log (multi-asset; default = USDT)
    bool deposit(const std::string& userId, 
                 Wallet& wallet, 
                 double amount,
                 const std::string& asset = "USDT");

    // withdraw and log (multi-asset; default = USDT)
    bool withdraw(const std::string& userId, 
                  Wallet& wallet, 
                  double amount,
                  const std::string& asset = "USDT");

    // get last n transactions
    std::vector<Transaction> getRecentTransactions(
        const std::string& userId,
        int count = 5
    );

    // compute simple stats for a user
    void printStatistics(const std::string& userId,
                         const std::string& productFilter = "");
    
    // helper: append transaction to csv
    void logTransaction(const Transaction& tx);

private:
    std::string walletsCSV;        // where wallets are saved
    std::string transactionsCSV;   // transaction log

    // load all transactions
    std::vector<Transaction> loadAllTransactions();
};
