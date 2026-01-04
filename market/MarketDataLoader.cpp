#include "MarketDataLoader.h"
#include "../core/CSVReader.h"
#include "../core/DateUtils.h"

// reuses the provided CSVReader to load market data
MarketDataLoader::MarketDataLoader(const std::string& marketCSV)
{
    // market csv is read once and cached in memory
    orders = CSVReader::readMarketCSV(marketCSV);
}

std::vector<Candlestick> MarketDataLoader::computeCandlesticks(
    const std::string& product,
    OrderBookType type,
    Timeframe timeframe)
{
    // map used to group prices by date bucket
    // key   → date string (e.g. 2023-06-01)
    // value → list of prices in chronological order
    std::map<std::string, std::vector<double>> buckets;

    // filter orders by product and order type
    for (const OrderBookEntry& e : orders)
    {
        if (e.product == product && e.orderType == type)
        {
            // determine which bucket this order belongs to
            std::string bucket = getTimeBucket(e.timestamp, timeframe);

            // add price to the corresponding bucket
            buckets[bucket].push_back(e.price);
        }
    }

    std::vector<Candlestick> result;

    // compute OHLC values for each bucket
    for (const auto& pair : buckets)
    {
        result.push_back(
            Candlestick::compute(pair.first, pair.second)
        );
    }

    return result;
}

// personal implementation
// converts a timestamp string into a date bucket string
std::string MarketDataLoader::getTimeBucket(const std::string& timestamp,
                                            Timeframe timeframe)
{
    // parse timestamp into tm struct using shared utility
    std::tm time = DateUtils::stringToTm(timestamp);

    int year  = DateUtils::getYear(time);  // extract year
    int month = DateUtils::getMonth(time); // extract month
    int day   = DateUtils::getDay(time);   // extract day
    int hour  = time.tm_hour;              // extract hour
    int min   = time.tm_min;               // extract minute
    int sec   = time.tm_sec;               // extract second

    // helper for two-digit formatting
    auto twoDigits = [](int x){ return (x < 10 ? "0" : "") + std::to_string(x); };

    if (timeframe == Timeframe::Yearly)
    {
        // yearly bucket: YYYY
        return std::to_string(year);
    }

    if (timeframe == Timeframe::Monthly)
    {
        // monthly bucket: YYYY-MM
        return std::to_string(year) + "-" + twoDigits(month);
    }

    if (timeframe == Timeframe::Daily)
    {
        // daily bucket: YYYY-MM-DD
        return std::to_string(year) + "-" + twoDigits(month) + "-" + twoDigits(day);
    }

    if (timeframe == Timeframe::Hourly)
    {
        // hourly bucket: YYYY-MM-DD HH
        return std::to_string(year) + "-" + twoDigits(month) + "-" + twoDigits(day)
             + " " + twoDigits(hour);
    }

    if (timeframe == Timeframe::Minutely)
    {
        // minutely bucket: YYYY-MM-DD HH:MM
        return std::to_string(year) + "-" + twoDigits(month) + "-" + twoDigits(day)
             + " " + twoDigits(hour) + ":" + twoDigits(min);
    }

    if (timeframe == Timeframe::Secondly)
    {
        // secondly bucket: YYYY-MM-DD HH:MM:SS
        return std::to_string(year) + "-" + twoDigits(month) + "-" + twoDigits(day)
             + " " + twoDigits(hour) + ":" + twoDigits(min) + ":" + twoDigits(sec);
    }

    // fallback in case of unknown timeframe
    return timestamp;
}

