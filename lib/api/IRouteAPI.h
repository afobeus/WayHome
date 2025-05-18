#pragma once

#include <string>
#include <chrono>

class IRouteAPI {
public:
    virtual ~IRouteAPI() = default;

    virtual std::string FetchRouteJSON(const std::string& departure_code, const std::string& destination_code,
                                  const std::chrono::year_month_day& date) = 0;
};
