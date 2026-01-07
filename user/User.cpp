#include "User.h"
#include <vector>

// personal implementation

// constructor just assigns all fields
User::User(const std::string& fullName,
           const std::string& email,
           const std::string& hashedPassword,
           const std::string& userID)
    : fullName(fullName),
      email(email),
      hashedPassword(hashedPassword),
      userID(userID)
{
}

// simple getters
std::string User::getUserID() const { return userID; }
std::string User::getFullName() const { return fullName; }
std::string User::getEmail() const { return email; }
std::string User::getHashedPassword() const { return hashedPassword; }

// convert user fields to csv row
std::vector<std::string> User::toCSVRow() const {
    return {userID, fullName, email, hashedPassword};
}
