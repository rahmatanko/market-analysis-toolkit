#include "Menu.h"
#include <iostream>
#include <limits>

// personal implementation

// colors for terminal
#define BLUE "\033[1;34m"
#define GREEN "\033[1;32m"
#define RED "\033[1;31m"
#define YELLOW "\033[1;33m"
#define RESET "\033[0m"

// constructor stores references
Menu::Menu(UserManager &userMgr,
           WalletManager &walletMgr,
           MarketDataLoader &market)
    : userManager(userMgr),
      walletManager(walletMgr),
      marketData(market)
{
}

// main loop
void Menu::run()
{
    while (true)
    {
        if (!loggedIn)
            showWelcomeMenu(); // ask user to login/register
        else
            showMainMenu(); // main functionality menu
    }
}

// welcome menu
void Menu::showWelcomeMenu()
{
    std::cout << BLUE << "\n╔════════════════════╗\n"
              << "║      WELCOME       ║\n"
              << "╚════════════════════╝" << RESET << "\n";

    // options
    std::cout << "1. Register\n";
    std::cout << "2. Login\n";
    std::cout << "3. Exit\n";
    std::cout << YELLOW << "Select option: " << RESET;

    int choice;
    std::cin >> choice;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    // route choice
    switch (choice)
    {
    case 1:
        handleRegister();
        break;
    case 2:
        handleLogin();
        break;
    case 3:
        exit(0); // exit program
    default:
        std::cout << RED << "Invalid option.\n"
                  << RESET;
    }
}

// main menu
void Menu::showMainMenu()
{
    const int width = 50; // total width of the box
    std::string title = "MAIN MENU (" + currentUser.getFullName() + ")";
    int padding = (width - 2 - title.size()) / 2; // subtract 2 for box edges

    // top border
    std::cout << GREEN << "\n╔";
    for (int i = 0; i < width - 2; ++i)
        std::cout << "═";
    std::cout << "╗\n";

    // title line
    std::cout << "║";
    for (int i = 0; i < padding; ++i)
        std::cout << " ";
    std::cout << title;
    for (int i = 0; i < width - 2 - padding - title.size(); ++i)
        std::cout << " ";
    std::cout << "║\n";

    // bottom border
    std::cout << "╚";
    for (int i = 0; i < width - 2; ++i)
        std::cout << "═";
    std::cout << "╝" << RESET << "\n";

    // menu options
    std::cout << " 1. Deposit\n"
              << " 2. Withdraw\n"
              << " 3. View Wallet\n"
              << " 4. Simulate Trades\n"
              << " 5. Recent Transactions\n"
              << " 6. User Statistics\n"
              << " 7. View Candlesticks\n"
              << " 8. Logout\n";
    std::cout << YELLOW << "Select option: " << RESET;

    int choice;
    std::cin >> choice;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    // route choice
    switch (choice)
    {
    case 1:
        handleDeposit();
        break;
    case 2:
        handleWithdraw();
        break;
    case 3:
        handleViewWallet();
        break;
    case 4:
        handleSimulateTrades();
        break;
    case 5:
        handleViewTransactions();
        break;
    case 6:
        handlePrintStatistics();
        break;
    case 7:
        handleViewCandlesticks();
        break;
    case 8:
        loggedIn = false; // logout
        std::cout << BLUE << "Logged out.\n"
                  << RESET;
        break;
    default:
        std::cout << RED << "Invalid option.\n"
                  << RESET;
    }
}

// register action
void Menu::handleRegister()
{
    std::string name = promptString("Full Name: ");
    std::string email = promptString("Email: ");
    std::string password = promptString("Password: ");

    User newUser;
    if (userManager.registerUser(name, email, password, newUser))
    {
        std::cout << GREEN << "✔ User registered successfully! You can now login.\n"
                  << RESET;
    }
    else
    {
        std::cout << RED << "✖ Registration failed: user may already exist.\n"
                  << RESET;
    }
}

// login action
void Menu::handleLogin()
{
    // ask for email instead of internal user ID
    std::string email = promptString("Email: ");
    std::string password = promptString("Password: ");

    // loginUser should accept email now instead of userID
    if (userManager.loginUserByEmail(email, password, currentUser))
    {
        std::cout << GREEN << "✔ Login successful!\n"
                  << RESET;
        loggedIn = true;
    }
    else
    {
        std::cout << RED << "✖ Login failed: invalid credentials.\n"
                  << RESET;
    }
}

// deposit action
void Menu::handleDeposit()
{
    double amount = promptDouble("Amount to deposit: ");
    std::string asset = promptString("Asset (default USDT): ");
    if (asset.empty())
        asset = "USDT"; // default asset

    Wallet wallet = walletManager.loadWallet(currentUser.getUserID());
    if (walletManager.deposit(currentUser.getUserID(), wallet, amount, asset))
    {
        std::cout << GREEN << "✔ Deposit successful.\n"
                  << RESET;
    }
    else
    {
        std::cout << RED << "✖ Deposit failed.\n"
                  << RESET;
    }
}

// withdraw action
void Menu::handleWithdraw()
{
    double amount = promptDouble("Amount to withdraw: ");
    std::string asset = promptString("Asset (default USDT): ");
    if (asset.empty())
        asset = "USDT";

    Wallet wallet = walletManager.loadWallet(currentUser.getUserID());
    if (walletManager.withdraw(currentUser.getUserID(), wallet, amount, asset))
    {
        std::cout << GREEN << "✔ Withdrawal successful.\n"
                  << RESET;
    }
    else
    {
        std::cout << RED << "✖ Withdrawal failed: insufficient funds.\n"
                  << RESET;
    }
}

// view wallet
void Menu::handleViewWallet()
{
    Wallet wallet = walletManager.loadWallet(currentUser.getUserID());
    std::cout << BLUE << "\n--- Wallet Balances ---\n"
              << RESET;
    for (const auto &[currency, amount] : wallet.getAllBalances())
    {
        std::cout << "  • " << currency << ": " << amount << "\n";
    }
}

// simulate trades
void Menu::handleSimulateTrades()
{
    Wallet wallet = walletManager.loadWallet(currentUser.getUserID());
    TradeSimulator simulator(marketData, walletManager);
    simulator.simulateTrades(currentUser); // run simulation
    std::cout << GREEN << "✔ Simulated trades completed.\n"
              << RESET;
}

// view recent transactions
void Menu::handleViewTransactions()
{
    auto txs = walletManager.getRecentTransactions(currentUser.getUserID(), 5);
    std::cout << BLUE << "\n--- Last " << txs.size() << " Transactions ---\n"
              << RESET;

    // column widths 
    constexpr int textw = 9;
    constexpr int numw = 14;

    for (const auto &tx : txs)
    {
        std::cout << "  • "
                  << std::left << std::setw(3) << Transaction::typeToString(tx.type) << " | "
                  << std::left << std::setw(textw) << tx.product << " | "
                  << std::right << std::setw(numw) << std::fixed << std::setprecision(4)
                  << tx.amount << " | "
                  << std::right << std::setw(numw) << std::fixed << std::setprecision(4)
                  << tx.balanceAfter << " | "
                  << std::left << tx.timestamp
                  << std::endl;
    }
}

// print statistics
void Menu::handlePrintStatistics()
{
    std::cout << BLUE << "\n--- User Statistics ---\n"
              << RESET;
    walletManager.printStatistics(currentUser.getUserID());
}

// view candlesticks
void Menu::handleViewCandlesticks()
{
    std::vector<std::string> products = marketData.getAllProducts();
    if (products.empty())
    {
        std::cout << RED << "No products available.\n"
                  << RESET;
        return;
    }

    std::cout << BLUE << "Available products:\n"
              << RESET;
    for (size_t i = 0; i < products.size(); ++i)
        std::cout << "  " << i + 1 << ". " << products[i] << "\n";

    int choice;
    std::cout << YELLOW << "Select product: " << RESET;
    std::cin >> choice;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    if (choice < 1 || choice > products.size())
    {
        std::cout << RED << "Invalid selection.\n"
                  << RESET;
        return;
    }

    std::string product = products[choice - 1];

    // select candlestick resolution (granularity)
    std::cout << BLUE << "\nSelect timeframe:\n"
              << RESET;
    std::cout << "  1. Yearly\n";
    std::cout << "  2. Daily\n";
    std::cout << "  3. Hourly\n";
    std::cout << "  4. Minutely\n";
    std::cout << "  5. Secondly\n";

    int tfChoice;
    std::cout << YELLOW << "Timeframe: " << RESET;
    std::cin >> tfChoice;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    Timeframe timeframe;
    switch (tfChoice)
    {
        case 1: timeframe = Timeframe::Yearly;   break;
        case 2: timeframe = Timeframe::Daily;    break;
        case 3: timeframe = Timeframe::Hourly;   break;
        case 4: timeframe = Timeframe::Minutely; break;
        case 5: timeframe = Timeframe::Secondly; break;
        default:
            std::cout << RED << "Invalid timeframe.\n"
                      << RESET;
            return;
    }

    // compute candlesticks for chosen product + timeframe
    auto candles = marketData.computeCandlesticks(
        product,
        OrderBookType::ask,
        timeframe
    );

    if (candles.empty())
    {
        std::cout << RED << "No candlestick data available.\n"
                  << RESET;
        return;
    }

    // select how many results to display (prevent terminal spam)
    std::cout << BLUE << "\nDisplay how many candles:\n"
              << RESET;
    std::cout << "  1. Last 5\n";
    std::cout << "  2. Last 10\n";
    std::cout << "  3. Last 20\n";
    std::cout << "  4. Last 50\n";
    std::cout << "  5. All\n";

    int limitChoice;
    std::cout << YELLOW << "Selection: " << RESET;
    std::cin >> limitChoice;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    size_t limit;
    switch (limitChoice)
    {
        case 1: limit = 5;  break;
        case 2: limit = 10; break;
        case 3: limit = 20; break;
        case 4: limit = 50; break;
        case 5: limit = candles.size(); break;
        default:
            std::cout << RED << "Invalid selection.\n"
                      << RESET;
            return;
    }

    // determine starting index so we only print the most recent candles
    size_t start = (candles.size() > limit) ? candles.size() - limit : 0;

    std::cout << BLUE << "\n--- Candlesticks for " << product << " ---\n"
              << RESET;

    for (size_t i = start; i < candles.size(); ++i)
    {
        const auto &c = candles[i];
        std::cout << "  • " << CYAN << c.date << RESET
                  << " | O: " << GREEN << c.open << RESET
                  << " H: " << GREEN << c.high << RESET
                  << " L: " << RED   << c.low  << RESET
                  << " C: " << YELLOW << c.close << RESET
                  << std::endl;
    }
}


// prompt helpers
std::string Menu::promptString(const std::string &msg)
{
    std::cout << msg;
    std::string s;
    std::getline(std::cin, s); // read full line
    return s;
}

double Menu::promptDouble(const std::string &msg)
{
    double val;
    while (true)
    {
        std::cout << msg;
        std::cin >> val;
        if (!std::cin.fail())
        {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return val; // valid input
        }
        // input failed, clear and retry
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << RED << "Invalid number, try again.\n"
                  << RESET;
    }
}
