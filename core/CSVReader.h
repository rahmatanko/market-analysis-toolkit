#pragma once
#include "../market/OrderBookEntry.h"
#include <vector>
#include <string>

class CSVReader {
public:
    // constructor, nothing special here
    CSVReader() {}  // trivial constructor

    // read the market csv into a vector of OrderBookEntry objects
    // starter code
    static std::vector<OrderBookEntry> readMarketCSV(const std::string& filename);

    // split a line into tokens based on separator, e.g. ','
    // starter code
    static std::vector<std::string> tokenise(const std::string& line, char separator);

    // convert a vector of strings into an OrderBookEntry
    // starter code
    static OrderBookEntry stringsToOBE(const std::vector<std::string>& tokens);

    // convert individual strings into an OrderBookEntry
    // starter code
    OrderBookEntry stringsToOBE(std::string priceString, 
                                       std::string amountString, 
                                       std::string timestamp, 
                                       std::string product, 
                                       OrderBookType orderType);

    // write any csv data to a file
    // personal implementation
    static void writeCSV(const std::string& filename,
                         const std::vector<std::vector<std::string>>& data,
                         bool append = true);
};
