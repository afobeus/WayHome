#pragma once

#include <chrono>
#include <string>

void PrintRoutes(const std::string& departure_city_user_title, const std::string& destination_city_user_title, std::chrono::year_month_day date);

std::pair<std::string, std::string> GetCitiesCodes(const std::string& departure_city_title, const std::string& arrival_city_title);