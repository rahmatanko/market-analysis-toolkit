#include "Candlestick.h"
#include <algorithm>

Candlestick::Candlestick(std::string date,
                         double open,
                         double high,
                         double low,
                         double close)
    : date(date),
      open(open),
      high(high),
      low(low),
      close(close)
{
    // nothing else to do here
}

// personal implementation
// prices are assumed to be ordered from earliest → latest
Candlestick Candlestick::compute(const std::string& date,
                                 const std::vector<double>& prices)
{
    // open is the first trade price in this period
    double open = prices.front();

    // close is the last trade price in this period
    double close = prices.back();

    // highest traded price in this period
    double high = *std::max_element(prices.begin(), prices.end());

    // lowest traded price in this period
    double low = *std::min_element(prices.begin(), prices.end());

    // return a fully constructed candlestick object
    return Candlestick(date, open, high, low, close);
}
