#include "user/UserManager.h"
#include <iostream>

int main() {
    // path to CSV file for testing
    std::string userCSV = "data/users_test.csv";

    // create the user manager, loads existing users if CSV exists
    UserManager userManager(userCSV);

    // create a variable to hold the user object returned after registration/login
    User testUser;

    // 1. check if the user already exists in CSV
    std::vector<User> allUsers = userManager.getAllUsers();
    bool userFound = false;
    for (const auto& u : allUsers) {
        if (u.getFullName() == "Alice Johnson" && u.getEmail() == "alice@example.com") {
            testUser = u; // assign existing user
            userFound = true;
            break;
        }
    }

    // 2. register the user if not found
    if (!userFound) {
        std::cout << "Registering new user...\n";
        bool regSuccess = userManager.registerUser("Alice Johnson", "alice@example.com", "password123", testUser);
        if (regSuccess) {
            std::cout << "Registration successful!\n";
            std::cout << "UserID: " << testUser.getUserID() << "\n";
            std::cout << "WalletID: " << testUser.getWalletID() << "\n";
        } else {
            std::cout << "Registration failed (user may already exist)\n";
        }
    } else {
        std::cout << "User already exists, using existing account.\n";
        std::cout << "UserID: " << testUser.getUserID() << "\n";
        std::cout << "WalletID: " << testUser.getWalletID() << "\n";
    }

    // 3. attempt login with correct credentials
    std::cout << "\nLogging in with correct credentials...\n";
    User loginUser;
    bool loginSuccess = userManager.loginUser(testUser.getUserID(), "password123", loginUser);
    if (loginSuccess) {
        std::cout << "Login successful! Welcome " << loginUser.getFullName() << "\n";
    } else {
        std::cout << "Login failed with correct credentials (should not happen)\n";
    }

    // 4. attempt login with wrong password
    std::cout << "\nLogging in with wrong password...\n";
    User wrongLogin;
    bool wrongPass = userManager.loginUser(testUser.getUserID(), "wrongpassword", wrongLogin);
    if (!wrongPass) {
        std::cout << "Login correctly failed with wrong password.\n";
    }

    // 5. display all users currently loaded
    std::cout << "\nAll users in CSV:\n";
    allUsers = userManager.getAllUsers(); // reload in case we just added a new user
    for (const auto& u : allUsers) {
        std::cout << "UserID: " << u.getUserID()
                  << ", Name: " << u.getFullName()
                  << ", Email: " << u.getEmail()
                  << ", WalletID: " << u.getWalletID() << "\n";
    }

    return 0;
}
