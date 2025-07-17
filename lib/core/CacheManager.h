#pragma once

#include <vector>
#include <chrono>

#include "../core/Route.h"


std::vector<std::unique_ptr<IRoute>> LoadRoutes(const std::string& departure_city_code, const std::string& arrival_city_code,
                                                std::chrono::year_month_day date);

std::vector<CityCode> LoadCitiesCodes();