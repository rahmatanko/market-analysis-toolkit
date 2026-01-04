#pragma once
#include <string>
#include <ctime>

// personal implementation
// utility functions for date parsing, extraction, and comparison
class DateUtils {
public:
    DateUtils() {}

    // convert a timestamp string "YYYY-MM-DD" → tm struct
    static std::tm stringToTm(const std::string& dateStr);

    // convert a tm struct back to string "YYYY-MM-DD"
    static std::string tmToString(const std::tm& timeStruct);

    // extract year from tm
    static int getYear(const std::tm& timeStruct);

    // extract month from tm
    static int getMonth(const std::tm& timeStruct);

    // extract day from tm
    static int getDay(const std::tm& timeStruct);

    // compare two dates, returns true if date1 <= date2
    static bool isBeforeOrEqual(const std::tm& date1, const std::tm& date2);

    // compare two dates, returns true if date1 >= date2
    static bool isAfterOrEqual(const std::tm& date1, const std::tm& date2);
};
