#include "DateUtils.h"
#include <sstream>
#include <iomanip>
#include <iostream>
#include <algorithm> 

// convert "YYYY-MM-DD" or "YYYY/MM/DD HH:MM:SS(.micro)" string to tm struct
// personal implementation
std::tm DateUtils::stringToTm(const std::string& dateStr)
{
    std::tm timeStruct = {};
    std::string cleanedStr = dateStr;

    // strip fractional seconds if present (after dot)
    auto dotPos = cleanedStr.find('.');
    if (dotPos != std::string::npos) {
        cleanedStr = cleanedStr.substr(0, dotPos); // remove microseconds
    }

    // replace '/' with '-' to handle both formats
    std::replace(cleanedStr.begin(), cleanedStr.end(), '/', '-');

    // parse using standard YYYY-MM-DD HH:MM:SS format
    std::istringstream ss(cleanedStr);
    ss >> std::get_time(&timeStruct, "%Y-%m-%d %H:%M:%S");
    if (ss.fail()) {
        // parsing failed: maybe only date without time
        ss.clear();
        ss.str(cleanedStr);
        ss >> std::get_time(&timeStruct, "%Y-%m-%d");
        if (ss.fail()) {
            std::cerr << "DateUtils::stringToTm failed to parse: " << dateStr << std::endl;
        }
    }

    // tm_mon is 0-indexed, tm_mday is fine
    return timeStruct;
}

// convert tm struct back to "YYYY-MM-DD HH:MM:SS"
std::string DateUtils::tmToString(const std::tm& timeStruct)
{
    std::ostringstream ss;
    ss << std::put_time(&timeStruct, "%Y-%m-%d %H:%M:%S"); // simple formatting
    return ss.str();
}

// get year from tm
int DateUtils::getYear(const std::tm& timeStruct)
{
    return timeStruct.tm_year + 1900; // tm_year = years since 1900
}

// get month from tm
int DateUtils::getMonth(const std::tm& timeStruct)
{
    return timeStruct.tm_mon + 1; // tm_mon = 0..11
}

// get day from tm
int DateUtils::getDay(const std::tm& timeStruct)
{
    return timeStruct.tm_mday; // tm_mday = 1..31
}

// returns true if date1 <= date2
bool DateUtils::isBeforeOrEqual(const std::tm& date1, const std::tm& date2)
{
    if (getYear(date1) != getYear(date2)) return getYear(date1) < getYear(date2);
    if (getMonth(date1) != getMonth(date2)) return getMonth(date1) < getMonth(date2);
    return getDay(date1) <= getDay(date2);
}

// returns true if date1 >= date2
bool DateUtils::isAfterOrEqual(const std::tm& date1, const std::tm& date2)
{
    if (getYear(date1) != getYear(date2)) return getYear(date1) > getYear(date2);
    if (getMonth(date1) != getMonth(date2)) return getMonth(date1) > getMonth(date2);
    return getDay(date1) >= getDay(date2);
}
