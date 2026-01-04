#include "CSVReader.h"
#include <iostream>
#include <fstream>
#include <sstream>

// read the market CSV file and convert each line to OrderBookEntry
// starter code
std::vector<OrderBookEntry> CSVReader::readMarketCSV(const std::string& filename)
{
    std::vector<OrderBookEntry> entries; // this will hold all the entries from the CSV

    std::ifstream csvFile{filename}; // open the file
    std::string line;
    int lineNum = 0; // keep track of the line number
    if (csvFile.is_open())
    {
        while(std::getline(csvFile, line)) // read line by line
        {
            ++lineNum;
            std::vector<std::string> tokens = tokenise(line, ',');
            
            // skip malformed lines
            if (tokens.size() != 5) {
                std::cout << "CSVReader::readMarketCSV bad data (wrong token count) at line " << lineNum << std::endl;
                continue; // skip this line
            }

            try {
                // convert line to tokens, then tokens to OrderBookEntry
                OrderBookEntry obe = stringsToOBE(tokens);
                entries.push_back(obe); // add to our vector
            } catch(const std::exception& e)
            {
                // if the line has invalid floats, just print a message and skip it
                std::cout << "CSVReader::readMarketCSV bad data (invalid float) at line " << lineNum << std::endl;
                continue; // skip this line
            }
        }
    }    

    // print how many entries we successfully read
    std::cout << "CSVReader::readMarketCSV read " << entries.size() << " entries"  << std::endl;
    return entries; 
}


// split a line into tokens based on separator, e.g. ','
// starter code
std::vector<std::string> CSVReader::tokenise(const std::string& line, char separator)
{
    std::vector<std::string> tokens; // output vector
    signed int start, end;
    std::string token;

    start = line.find_first_not_of(separator, 0); // skip any leading separators
    do{
        end = line.find_first_of(separator, start); // find next separator
        if (start == line.length() || start == end) break; // nothing left to parse
        if (end >= 0) token = line.substr(start, end - start); // extract token
        else token = line.substr(start, line.length() - start); // last token
        tokens.push_back(token); // add to vector
        start = end + 1; // move past the separator
    }while(end > 0);

    return tokens; 
}

// convert a vector of strings into OrderBookEntry
// starter code
OrderBookEntry CSVReader::stringsToOBE(const std::vector<std::string>& tokens)
{
    double price, amount;

    if (tokens.size() != 5) // not enough or too many tokens
    {
        std::cout << "Bad line " << std::endl;
        throw std::exception{};
    }

    // parse price and amount, might throw
    try {
        price = std::stod(tokens[3]);
        amount = std::stod(tokens[4]);
    }catch(const std::exception& e){
        std::cout << "CSVReader::stringsToOBE Bad float! " << tokens[3]<< std::endl;
        std::cout << "CSVReader::stringsToOBE Bad float! " << tokens[4]<< std::endl; 
        throw;        
    }

    // create the OrderBookEntry object
    OrderBookEntry obe{price, 
                       amount, 
                       tokens[0], // timestamp
                       tokens[1], // product
                       OrderBookEntry::stringToOrderBookType(tokens[2])}; // order type

    return obe; 
}

// convert individual strings to OrderBookEntry
// starter code
OrderBookEntry CSVReader::stringsToOBE(std::string priceString, 
                                       std::string amountString, 
                                       std::string timestamp, 
                                       std::string product, 
                                       OrderBookType orderType)
{
    double price, amount;

    try {
        price = std::stod(priceString); // convert price to double
        amount = std::stod(amountString); // convert amount to double
    }catch(const std::exception& e){
        std::cout << "CSVReader::stringsToOBE Bad float! " << priceString<< std::endl;
        std::cout << "CSVReader::stringsToOBE Bad float! " << amountString<< std::endl; 
        throw;        
    }

    // create the OrderBookEntry object
    OrderBookEntry obe{price, 
                       amount, 
                       timestamp,
                       product, 
                       orderType};
                
    return obe;
}

// write any csv data to a file, append or overwrite
// personal implementation
void CSVReader::writeCSV(const std::string& filename,
                         const std::vector<std::vector<std::string>>& data,
                         bool append)
{
    // open the file in append or truncate mode
    std::ofstream file(filename, append ? std::ios::app : std::ios::trunc);
    if (!file.is_open()) {
        std::cerr << "Failed to open CSV file: " << filename << std::endl;
        return; // can't continue
    }

    // write each row
    for (const auto& row : data) {
        for (size_t i = 0; i < row.size(); ++i) {
            file << row[i]; // write the cell
            if (i < row.size() - 1) file << ","; // add comma between columns
        }
        file << "\n"; // end of row
    }
}
