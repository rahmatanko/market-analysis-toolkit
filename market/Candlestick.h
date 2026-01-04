#pragma once
#include <string>
#include <vector>

// represents one OHLC summary for a single time bucket (day / month / year)
// used in task 1 for candlestick computation
class Candlestick
{
public:
    // date string depends on timeframe:
    // YYYY (yearly), YYYY-MM (monthly), YYYY-MM-DD (daily)
    std::string date;

    // open: first price in the time bucket
    // high: max price in the time bucket
    // low: min price in the time bucket
    // close: last price in the time bucket
    double open;
    double high;
    double low;
    double close;

    // simple constructor, just assigns values
    Candlestick(std::string date,
                double open,
                double high,
                double low,
                double close);

    // personal implementation
    // computes OHLC values from a list of prices already in chronological order
    static Candlestick compute(const std::string& date,
                               const std::vector<double>& prices);
};
