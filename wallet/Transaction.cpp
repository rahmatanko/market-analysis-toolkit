#include "Transaction.h"
#include <stdexcept> // for std::runtime_error


// personal implementation

// simple constructor, just assign everything
Transaction::Transaction(std::string userId,
                         TransactionType type,
                         std::string product,
                         double amount,
                         double balanceAfter,
                         std::string timestamp)
    : userId(userId),
      type(type),
      product(product),
      amount(amount),
      balanceAfter(balanceAfter),
      timestamp(timestamp)
{}

// convert transaction to a vector of strings for csv
std::vector<std::string> Transaction::toCSVRow() const
{
    return {
        userId,
        typeToString(type),       // convert enum to string
        product,
        std::to_string(amount),   // store amount
        std::to_string(balanceAfter),
        timestamp
    };
}

// rebuild a transaction from a csv row
Transaction Transaction::fromCSVRow(const std::vector<std::string>& row)
{
    return Transaction(
        row[0],                       // user id
        stringToType(row[1]),         // type
        row[2],                       // product
        std::stod(row[3]),            // amount
        std::stod(row[4]),            // balance after
        row[5]                        // timestamp
    );
}

// convert enum to string
std::string Transaction::typeToString(TransactionType type)
{
    switch (type)
    {
        case TransactionType::DEPOSIT:  return "DEPOSIT";
        case TransactionType::WITHDRAW: return "WITHDRAW";
        case TransactionType::ASK:      return "ASK";
        case TransactionType::BID:      return "BID";
    }
    return ""; // fallback
}

// convert string back to enum
TransactionType Transaction::stringToType(const std::string& str)
{
    if (str == "DEPOSIT")  return TransactionType::DEPOSIT;
    if (str == "WITHDRAW") return TransactionType::WITHDRAW;
    if (str == "ASK")      return TransactionType::ASK;
    if (str == "BID")      return TransactionType::BID;
    throw std::runtime_error("Invalid transaction type");
}
