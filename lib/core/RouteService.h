#pragma once

#include <chrono>
#include <string>

void PrintRoutes(const std::string& departure_yandex_code, const std::string& destination_yandex_code, std::chrono::year_month_day date);

std::pair<std::string, std::string> GetCityCodes(const std::string& departure_city_title, const std::string& arrival_city_title);