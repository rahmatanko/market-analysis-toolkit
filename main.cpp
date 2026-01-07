#include "ui/Menu.h"
#include "user/UserManager.h"
#include "wallet/WalletManager.h"
#include "market/MarketDataLoader.h"
#include <iostream>

int main() {

    // file paths for CSVs 
    const std::string usersCSV       = "data/users.csv";
    const std::string walletsCSV     = "data/wallets.csv";
    const std::string transactionsCSV = "data/transactions.csv";
    const std::string marketCSV      = "data/big_market.csv";

    // initialize core managers with CSV paths
    UserManager userManager(usersCSV);                          
    WalletManager walletManager(walletsCSV, transactionsCSV);  
    MarketDataLoader marketData(marketCSV);                    

    // create menu with references to managers
    Menu menu(userManager, walletManager, marketData);

    // start main loop
    menu.run();

    return 0; // normally never reached
}
