#pragma once

#include "IRouteAPI.h"

#include <string>
#include <chrono>

class RouteAPI final : public IRouteAPI {
public:
    std::string FetchRouteJSON(const std::string& departure_yandex_code, const std::string& destination_yandex_code,
                          const std::chrono::year_month_day& date) override;
};
