#pragma once

#include <iomanip>
#include <string>

std::time_t ParseISO8601(const std::string& str);

std::string GetDate(std::time_t date_time);

std::string GetTime(std::time_t time_time);

std::string GetDateTime(std::time_t time_time);
