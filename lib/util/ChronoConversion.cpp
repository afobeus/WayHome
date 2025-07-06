#include "ChronoConversion.h"


std::time_t ParseISO8601(const std::string& str) {
    std::tm tm = {};
    std::istringstream ss(str.substr(0, 19));
    ss >> std::get_time(&tm, "%Y-%m-%dT%H:%M:%S");
    return std::mktime(&tm);
}

std::string GetDate(std::time_t date_time) {
    std::ostringstream oss;
    std::tm tm = *std::localtime(&date_time);
    oss << std::put_time(&tm, "%Y-%m-%d");
    return oss.str();
}

std::string GetTime(std::time_t date_time) {
    std::ostringstream oss;
    std::tm tm = *std::localtime(&date_time);
    if (tm.tm_sec == 0)
        oss << std::put_time(&tm, "%H:%M");
    else
        oss << std::put_time(&tm, "%H:%M:%S");
    return oss.str();
}

std::string GetDateTime(std::time_t date_time) {
    return GetDate(date_time) + ' ' + GetTime(date_time);
}
