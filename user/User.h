#pragma once
#include <string>
#include <vector>

// full file is personal implementation
class User {
public:
    User() = default;

    // constructor for creating a new user object
    User(const std::string& fullName,
         const std::string& email,
         const std::string& hashedPassword,
         const std::string& userID,
         const std::string& walletID);

    // getters
    std::string getUserID() const;         // return unique 10-digit ID
    std::string getFullName() const;       // return full name
    std::string getEmail() const;          // return email
    std::string getHashedPassword() const; // return hashed password
    std::string getWalletID() const;       // return linked wallet ID

    // convert user to csv row for saving
    std::vector<std::string> toCSVRow() const;

private:
    std::string userID;        // unique 10-digit ID
    std::string fullName;      // user's full name
    std::string email;         // user's email
    std::string hashedPassword; // hashed password using std::hash<string>
    std::string walletID;      // link to wallet CSV
};
