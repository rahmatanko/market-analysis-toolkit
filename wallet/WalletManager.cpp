#include "WalletManager.h"
#include <iostream>
#include <ctime>
#include <fstream>   // for ifstream
#include <string>    // for std::string

// personal implementation
// wallet balances are tracked in base currency (USDT)


// just assign filenames
WalletManager::WalletManager(const std::string& walletFile,
                             const std::string& transactionFile)
    : walletsCSV(walletFile),
      transactionsCSV(transactionFile)
{}

// load wallet for user
Wallet WalletManager::loadWallet(const std::string& userId)
{
    Wallet wallet;
    std::ifstream file(walletsCSV);
    std::string line;

    while (std::getline(file, line))
    {
        auto tokens = CSVReader::tokenise(line, ',');
        if (tokens[0] == userId)
        {
            wallet.insertCurrency(tokens[1], std::stod(tokens[2]));
        }
    }
    return wallet;
}

// save wallet
void WalletManager::saveWallet(const std::string& userId, Wallet& wallet)
{
    std::ifstream in(walletsCSV);
    std::vector<std::vector<std::string>> rows;
    std::string line;

    // keep all other users
    while (std::getline(in, line))
    {
        auto tokens = CSVReader::tokenise(line, ',');
        if (tokens[0] != userId)
            rows.push_back(tokens);
    }

    // write updated balance
    rows.push_back({
        userId,
        "USDT",
        std::to_string(wallet.getBalance("USDT"))
    });

    CSVReader::writeCSV(walletsCSV, rows, false); // overwrite
}


// deposit funds into wallet, log tx
bool WalletManager::deposit(const std::string& userId,
                            Wallet& wallet,
                            double amount)
{
    if (amount <= 0) return false; // reject bad amounts

    wallet.insertCurrency("USDT", amount); // add to wallet
    saveWallet(userId, wallet);

    auto now = std::time(nullptr);

    // get actual balance after deposit once
    double balanceAfter = wallet.getBalance("USDT"); 

    Transaction tx(userId, TransactionType::DEPOSIT,
                   "USDT",
                   amount,
                   balanceAfter,
                   std::ctime(&now));

    // append to history
    logTransaction(tx); 
    return true;
}


// withdraw funds from wallet, log tx
bool WalletManager::withdraw(const std::string& userId,
                             Wallet& wallet,
                             double amount)
{
    if (!wallet.removeCurrency("USDT", amount)) return false; // not enough
    saveWallet(userId, wallet);

    auto now = std::time(nullptr);

    double balanceAfter = wallet.getBalance("USDT"); // correct balance

    Transaction tx(userId,
                   TransactionType::WITHDRAW,
                   "USDT",
                   amount,
                   balanceAfter,
                   std::ctime(&now));

    logTransaction(tx);
    return true;
}

// append transaction to csv
void WalletManager::logTransaction(const Transaction& tx)
{
    CSVReader::writeCSV(
        transactionsCSV,
        {tx.toCSVRow()},
        true
    );
}

// load all transactions from file
std::vector<Transaction> WalletManager::loadAllTransactions()
{
    std::ifstream file(transactionsCSV);
    std::vector<Transaction> result;
    std::string line;

    while (std::getline(file, line))
    {
        auto tokens = CSVReader::tokenise(line, ',');
        if (tokens.size() == 6)
            result.push_back(Transaction::fromCSVRow(tokens));
    }
    return result;
}

// get last n transactions for a user
std::vector<Transaction> WalletManager::getRecentTransactions(
    const std::string& userId,
    int count)
{
    auto all = loadAllTransactions();
    std::vector<Transaction> filtered;

    // filter for user
    for (auto& tx : all)
        if (tx.userId == userId)
            filtered.push_back(tx);

    // return last count
    if (filtered.size() <= count) return filtered;

    return std::vector<Transaction>(
        filtered.end() - count,
        filtered.end()
    );
}

// compute some stats for a user
void WalletManager::printStatistics(const std::string& userId,
                                    const std::string& productFilter)
{
    auto all = loadAllTransactions();

    int asks = 0, bids = 0;
    double totalSpent = 0.0;

    for (const auto& tx : all)
    {
        if (tx.userId != userId) continue;            // skip others
        if (!productFilter.empty() && tx.product != productFilter) continue; // filter

        if (tx.type == TransactionType::ASK) asks++;
        if (tx.type == TransactionType::BID) bids++;

        // only count outgoing money (withdrawals, asks, bids), not deposits
        if (tx.type == TransactionType::WITHDRAW || tx.type == TransactionType::ASK || tx.type == TransactionType::BID)
            totalSpent += tx.amount;
    }

    // print stats
    std::cout << "ASKS: " << asks << "\n";
    std::cout << "BIDS: " << bids << "\n";
    std::cout << "TOTAL SPENT: " << totalSpent << "\n";
}