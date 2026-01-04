#pragma once
#include "User.h"
#include "../core/CSVReader.h"
#include <string>
#include <vector>
#include <unordered_map>

// personal implementation
class UserManager {
public:
    UserManager(const std::string& userCSV); // load users from CSV on init

    // register a new user, returns false if duplicate
    bool registerUser(const std::string& fullName,
                      const std::string& email,
                      const std::string& password,
                      User& outUser);

    // login existing user by ID + password
    bool loginUser(const std::string& userID,
                   const std::string& password,
                   User& outUser);

    // return all loaded users
    std::vector<User> getAllUsers() const;

private:
    std::string userCSVFile;     // csv file storing users
    std::vector<User> users;     // in-memory cache

    // helpers
    void loadUsers();                               // load from csv
    void saveUser(const User& user);               // append user to csv
    bool userExists(const std::string& fullName, const std::string& email) const; // check duplicates
    std::string generateUniqueUserID() const;      // generate unique 10-digit ID
    std::string hashPassword(const std::string& password) const; // hash password
};
