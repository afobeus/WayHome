#pragma once

#include <iostream>
#include <string>
#include <chrono>
#include <sstream>
#include <iomanip>
#include <stdexcept>
#include <vector>

struct TripInfo {
    std::string departure_city;
    std::string arrival_city;
    std::chrono::year_month_day date;
};

std::optional<TripInfo> ParseArguments(int argc, char* argv[]);
