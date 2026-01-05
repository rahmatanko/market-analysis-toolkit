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
    // load the user's wallet from csv
    Wallet wallet = walletManager.loadWallet(user.getWalletID());

    // list of known products to simulate trades for
    std::vector<std::string> knownProducts = marketData.getAllProducts();

    // loop over each product
    for (const std::string& product : knownProducts)
    {
        // compute yearly candlesticks to get a stable historical price
        auto candles = marketData.computeCandlesticks(
            product,
            OrderBookType::ask,
            Timeframe::Yearly
        );

        // if no historical data exists, skip this product
        if (candles.empty())
            continue;

        // use the last known close price as reference
        double basePrice = candles.back().close;

        // extract base currency from product string (e.g., "BTC" from "BTC/USDT")
        std::string baseCurrency = product.substr(0, product.find('/'));

        // create 5 bids and 5 asks for this product
        for (int i = 0; i < 5; ++i)
        {
            std::string timestamp = getCurrentTimestamp();
            double price = generatePrice(basePrice);

            // calculate amount for bid (buying)
            double maxAffordable = wallet.getBalance("USDT") / price; // max amount user can buy
            double bidAmount = std::min(1.0, maxAffordable);          // buy 1 unit or as much as possible
            if (bidAmount <= 0) continue;                             // skip if nothing can be bought

            // create a bid order (user buys the asset)
            OrderBookEntry bid(
                price,
                bidAmount,
                timestamp,
                product,
                OrderBookType::bidsale,   // bidsale so processSale works
                user.getUserID()
            );

            // check if wallet can afford the bid
            if (wallet.canFulfillOrder(bid))
            {
                // update wallet balances
                wallet.processSale(bid);

                // log bid transaction via public logTransaction
                Transaction tx(
                    user.getUserID(),
                    TransactionType::BID,
                    product,
                    price * bidAmount,
                    wallet.getBalance("USDT"),
                    timestamp
                );

                // persist transaction
                walletManager.logTransaction(tx);
            }

            // calculate amount for ask (selling)
            double assetBalance = wallet.getBalance(baseCurrency);    // e.g., BTC or ETH
            double askAmount = std::min(1.0, assetBalance);           // sell 1 unit or as much as possible
            if (askAmount <= 0) continue;                             // skip if none to sell

            // create an ask order (user sells the asset)
            OrderBookEntry ask(
                price,
                askAmount,
                timestamp,
                product,
                OrderBookType::asksale,  // asksale so processSale works
                user.getUserID()
            );

            // check if wallet can fulfill the ask
            if (wallet.canFulfillOrder(ask))
            {
                // update wallet balances
                wallet.processSale(ask);

                // log ask transaction via public logTransaction
                Transaction tx(
                    user.getUserID(),
                    TransactionType::ASK,
                    product,
                    price * askAmount,
                    wallet.getBalance("USDT"),
                    timestamp
                );

                // persist transaction
                walletManager.logTransaction(tx);
            }
        }
    }
    // remove tiny leftover fractions for cleanliness
    for (auto& [currency, amount] : wallet.getAllBalances())
    {
        if (amount < 1e-6) // treat as zero
            wallet.removeCurrency(currency, amount);
    }

    // final save to ensure wallet state is stored
    walletManager.saveWallet(user.getUserID(), wallet);
}


