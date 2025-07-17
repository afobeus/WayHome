#include "DateTimeTools.h"

#include <chrono>
#include <iostream>

std::time_t ParseISO8601(const std::string& str) {
    std::tm tm = {};
    std::istringstream ss(str.substr(0, 19));
    ss >> std::get_time(&tm, "%Y-%m-%dT%H:%M:%S");
    return std::mktime(&tm);
}

std::string TimeToISOString(const std::time_t date_time) {
    const std::tm tm_ptr = *std::localtime(&date_time);
    std::ostringstream oss;
    oss << std::put_time(&tm_ptr, "%Y-%m-%dT%H:%M:%S");
    return oss.str();
}

std::string GetDate(const std::time_t date_time) {
    std::ostringstream oss;
    const std::tm tm = *std::localtime(&date_time);
    oss << std::put_time(&tm, "%Y-%m-%d");
    return oss.str();
}

std::string GetTime(const std::time_t date_time) {
    std::ostringstream oss;
    const std::tm tm = *std::localtime(&date_time);
    if (tm.tm_sec == 0)
        oss << std::put_time(&tm, "%H:%M");
    else
        oss << std::put_time(&tm, "%H:%M:%S");
    return oss.str();
}

std::string GetDateTime(const std::time_t date_time) {
    return GetDate(date_time) + ' ' + GetTime(date_time);
}

std::string CurrentTimeToString() {
    const auto now = std::chrono::system_clock::now();
    const std::time_t time = std::chrono::system_clock::to_time_t(now);
    std::stringstream ss;
    ss << std::put_time(std::gmtime(&time), "%Y-%m-%dT%H:%M:%SZ");
    return ss.str();
}

bool IsExpired(const std::string& saved_time_str, const int hours_threshold) {
    const auto saved_time = std::chrono::system_clock::from_time_t(ParseISO8601(saved_time_str));
    const auto now = std::chrono::system_clock::now();
    const auto duration = std::chrono::duration_cast<std::chrono::hours>(now - saved_time);
    return duration.count() > hours_threshold;
}