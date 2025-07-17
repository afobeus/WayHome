#pragma once

#include <iomanip>
#include <string>

// Commonly used tools

std::time_t ParseISO8601(const std::string& str);

std::string TimeToISOString(std::time_t date_time);

// Tools for std::time_t (used in Route class)

std::string GetDate(std::time_t date_time);

std::string GetTime(std::time_t time_time);

std::string GetDateTime(std::time_t time_time);

// Tools for dating requests in cache

std::string CurrentTimeToString();



bool IsExpired(const std::string& saved_time_str, int hours_threshold);