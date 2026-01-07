#include "TradeSimulator.h"
#include <chrono>
#include <iomanip>
#include <sstream>
#include <set>

// personal implementation
// all logic in this file is written for task 4

TradeSimulator::TradeSimulator(MarketDataLoader& marketData,
                               WalletManager& walletManager)
    : marketData(marketData),
      walletManager(walletManager)
{
    // nothing else to initialize
}

// returns current system time as "YYYY-MM-DD HH:MM:SS"
// used to simulate trades occurring in 2025/2026
std::string TradeSimulator::getCurrentTimestamp() const
{
    auto now = std::chrono::system_clock::now();
    std::time_t time = std::chrono::system_clock::to_time_t(now);
    std::tm* tm = std::localtime(&time);

    std::ostringstream oss;
    oss << std::put_time(tm, "%Y-%m-%d %H:%M:%S");
    return oss.str();
}

// generate a price with small random variation around a base price
// this avoids unrealistic jumps while still being random
double TradeSimulator::generatePrice(double basePrice)
{
    static std::mt19937 rng(std::random_device{}());
    std::uniform_real_distribution<double> dist(0.95, 1.05);
    return basePrice * dist(rng);
}

void TradeSimulator::simulateTrades(const User& user) 
{
    std::cout << BOLD << CYAN
              << "\n[ SIMULATION ] Starting trade simulation for user "
              << user.getFullName()
              << RESET << "\n";

    // load the user's wallet from csv
    Wallet wallet = walletManager.loadWallet(user.getUserID());

    std::cout << GRAY << "[SIM] Initial wallet state loaded\n" << RESET;

    // list of known products to simulate trades for
    std::vector<std::string> knownProducts = marketData.getAllProducts();

    for (const std::string& product : knownProducts)
    {
        auto candles = marketData.computeCandlesticks(
            product,
            OrderBookType::ask,
            Timeframe::Yearly
        );

        if (candles.empty())
            continue;

        double basePrice = candles.back().close;
        std::string baseCurrency = product.substr(0, product.find('/'));

        std::cout << BOLD << MAGENTA
                  << "\n[PRODUCT] " << product
                  << " | Reference price: " << basePrice
                  << RESET << "\n";

        for (int i = 0; i < 5; ++i)
        {
            std::string timestamp = getCurrentTimestamp();
            double price = generatePrice(basePrice);

            // bid/buy
            double maxAffordable = wallet.getBalance("USDT") / price;
            double bidAmount = std::min(1.0, maxAffordable);
            if (bidAmount > 0)
            {
                OrderBookEntry bid(
                    price,
                    bidAmount,
                    timestamp,
                    product,
                    OrderBookType::bidsale,
                    user.getUserID()
                );

                std::cout << CYAN
                          << "[SIM-BID] Buy " << bidAmount << " "
                          << baseCurrency << " @ " << price
                          << RESET << "\n";

                if (wallet.canFulfillOrder(bid))
                {
                    wallet.processSale(bid);

                    Transaction tx(
                        user.getUserID(),
                        TransactionType::BID,
                        product,
                        price * bidAmount,
                        wallet.getBalance("USDT"),
                        timestamp
                    );

                    walletManager.logTransaction(tx);

                    std::cout << GREEN
                              << "  ✔ Feasible | USDT → "
                              << wallet.getBalance("USDT")
                              << RESET << "\n";
                }
                else
                {
                    std::cout << RED
                              << "  ✘ Rejected (insufficient USDT)"
                              << RESET << "\n";
                }
            }

            // ask/sell
            double assetBalance = wallet.getBalance(baseCurrency);
            double askAmount = std::min(1.0, assetBalance);
            if (askAmount > 0)
            {
                OrderBookEntry ask(
                    price,
                    askAmount,
                    timestamp,
                    product,
                    OrderBookType::asksale,
                    user.getUserID()
                );

                std::cout << YELLOW
                          << "[SIM-ASK] Sell " << askAmount << " "
                          << baseCurrency << " @ " << price
                          << RESET << "\n";

                if (wallet.canFulfillOrder(ask))
                {
                    wallet.processSale(ask);

                    Transaction tx(
                        user.getUserID(),
                        TransactionType::ASK,
                        product,
                        price * askAmount,
                        wallet.getBalance("USDT"),
                        timestamp
                    );

                    walletManager.logTransaction(tx);

                    std::cout << GREEN
                              << "  ✔ Feasible | USDT → "
                              << wallet.getBalance("USDT")
                              << RESET << "\n";
                }
                else
                {
                    std::cout << RED
                              << "  ✘ Rejected (insufficient asset)"
                              << RESET << "\n";
                }
            }
        }
    }

    // cleanup tiny dust
    for (auto& [currency, amount] : wallet.getAllBalances())
    {
        if (amount < 1e-6)
            wallet.removeCurrency(currency, amount);
    }

    walletManager.saveWallet(user.getUserID(), wallet);

    std::cout << BOLD << CYAN
              << "\n[ SIMULATION COMPLETE ]"
              << RESET << "\n"
              << GRAY
              << "• Trades evaluated using historical prices\n"
              << "• Transactions logged\n"
              << "• Wallet state finalized\n"
              << RESET;
}



