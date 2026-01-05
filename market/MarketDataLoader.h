#pragma once
#include <vector>
#include <string>
#include <map>
#include <set>

#include "OrderBookEntry.h"
#include "Candlestick.h"

// time grouping options for task 1
enum class Timeframe {
    Daily,
    Monthly,
    Yearly,
    Hourly,    
    Minutely,  
    Secondly  
};

// personal implementation
// handles loading market data and computing candlestick summaries
class MarketDataLoader
{
public:
    // loads all market orders from the given csv file
    MarketDataLoader(const std::string& marketCSV);

    // filters by product and order type (ask / bid),
    // groups by timeframe, then computes candlesticks
    std::vector<Candlestick> computeCandlesticks(
        const std::string& product,
        OrderBookType type,
        Timeframe timeframe
    );

    // returns a list of all unique products in the dataset
    std::vector<std::string> getAllProducts() const
    {
        std::set<std::string> uniqueProducts;
        for (const auto& order : orders)
            uniqueProducts.insert(order.product);
        return std::vector<std::string>(uniqueProducts.begin(), uniqueProducts.end());
    }

private:
    // holds all market orders loaded from the dataset
    std::vector<OrderBookEntry> orders;

    // converts a timestamp into the correct bucket string
    // depending on daily / monthly / yearly mode
    std::string getTimeBucket(const std::string& timestamp,
                              Timeframe timeframe);
};
