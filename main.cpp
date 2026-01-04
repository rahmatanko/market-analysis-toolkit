#include "core/CSVReader.h"
#include "core/DateUtils.h"
#include <iostream>

int main() {
    std::cout << "[TEST] DateUtils with date only\n";
    std::string dateOnly = "2025-01-04";
    std::tm t = DateUtils::stringToTm(dateOnly);
    std::cout << "Year: " << DateUtils::getYear(t)
              << ", Month: " << DateUtils::getMonth(t)
              << ", Day: " << DateUtils::getDay(t) << "\n\n";

    std::cout << "[TEST] OrderBookEntry with invalid order type\n";
    OrderBookEntry obe(100.0, 2.0, "2025-01-04 12:00:00", "ETH/BTC",
                       OrderBookEntry::stringToOrderBookType("foo"));
    std::cout << "Order type enum value: " 
              << static_cast<int>(obe.orderType) << " (0=bid,1=ask,2=unknown)\n";

    return 0;
}
