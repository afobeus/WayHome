#pragma once

#include <string>
#include <chrono>

#include <nlohmann/json.hpp>

using json = nlohmann::json;

class IRouteAPI {
public:
    virtual ~IRouteAPI() = default;

    virtual json FetchRouteJSON(const std::string& departure_code, const std::string& destination_code,
                                  const std::chrono::year_month_day& date) = 0;
};
