#pragma once

#include <chrono>
#include <string>

void PrintRoutes(const std::string& departure_yandex_code, const std::string& destination_yandex_code, std::chrono::year_month_day date);

std::string GetCityCode(std::string& city_title);