#pragma once
#include <string>
#include <vector>

// personal implementation: this whole file is mine

// type of transaction
enum class TransactionType
{
    DEPOSIT,
    WITHDRAW,
    ASK,
    BID
};

// stores a single transaction
class Transaction
{
public:
    std::string userId;      // who did it
    TransactionType type;    // type of tx
    std::string product;     // currency or product
    double amount;           // how much involved
    double balanceAfter;     // wallet balance after tx
    std::string timestamp;   // when it happened

    // constructor
    Transaction(std::string userId,
                TransactionType type,
                std::string product,
                double amount,
                double balanceAfter,
                std::string timestamp);

    // convert to csv row (for saving)
    std::vector<std::string> toCSVRow() const;

    // rebuild transaction from csv row
    static Transaction fromCSVRow(const std::vector<std::string>& row);

    // helpers to convert type enum <-> string
    static std::string typeToString(TransactionType type);
    static TransactionType stringToType(const std::string& str);
};
