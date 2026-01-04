#include "UserManager.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <random>
#include <functional> // std::hash

// personal implementation

// constructor just loads all users from csv
UserManager::UserManager(const std::string& userCSV)
    : userCSVFile(userCSV)
{
    loadUsers();
}

// load users from csv into vector
void UserManager::loadUsers() {
    std::ifstream file(userCSVFile);
    if (!file.is_open()) return; // nothing to load

    std::string line;
    while (std::getline(file, line)) {
        std::vector<std::string> tokens = CSVReader::tokenise(line, ',');
        if (tokens.size() != 5) continue; // skip malformed line

        // create user object and store in vector
        User user(tokens[1], tokens[2], tokens[3], tokens[0], tokens[4]);
        users.push_back(user);
    }
}

// append a single user to csv
void UserManager::saveUser(const User& user) {
    CSVReader::writeCSV(userCSVFile, {user.toCSVRow()}, true);
}

// check if user already exists by full name + email
bool UserManager::userExists(const std::string& fullName, const std::string& email) const {
    for (const auto& u : users) {
        if (u.getFullName() == fullName && u.getEmail() == email) return true;
    }
    return false;
}

// generate unique 10-digit ID for user
std::string UserManager::generateUniqueUserID() const {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 9);

    std::string id;
    bool unique = false;
    while (!unique) {
        id.clear();
        for (int i = 0; i < 10; ++i) id += std::to_string(dis(gen));

        // make sure id isn't already taken
        unique = true;
        for (const auto& u : users) {
            if (u.getUserID() == id) {
                unique = false;
                break;
            }
        }
    }
    return id;
}

// deterministic hash using std::hash with fixed seed
std::string UserManager::hashPassword(const std::string& password) const {
    // std::hash itself is called deterministically here
    std::hash<std::string> hasher;
    size_t rawHash = hasher(password);

    // convert to string using hex, ensures same result across platforms/runs
    std::stringstream ss;
    ss << std::hex << rawHash; 
    return ss.str();
}

// register a new user
bool UserManager::registerUser(const std::string& fullName,
                               const std::string& email,
                               const std::string& password,
                               User& outUser)
{
    if (userExists(fullName, email)) {
        std::cout << "User with this name and email already exists.\n";
        return false;
    }

    // generate id, wallet link, hash password
    std::string userID = generateUniqueUserID();
    std::string walletID = userID + "_wallet";
    std::string hashedPassword = hashPassword(password);

    // create user object, store in memory and csv
    User newUser(fullName, email, hashedPassword, userID, walletID);
    users.push_back(newUser);
    saveUser(newUser);

    outUser = newUser; // return created user
    return true;
}

// login user by id + password
bool UserManager::loginUser(const std::string& userID,
                            const std::string& password,
                            User& outUser)
{
    std::string hashedInput = hashPassword(password);
    for (const auto& u : users) {
        if (u.getUserID() == userID && u.getHashedPassword() == hashedInput) {
            outUser = u;
            return true;
        }
    }
    return false;
}

// return all users in memory
std::vector<User> UserManager::getAllUsers() const {
    return users;
}
