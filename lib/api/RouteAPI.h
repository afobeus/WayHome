#pragma once

#include "IRouteAPI.h"

#include <string>
#include <chrono>

class RouteAPI final : public IRouteAPI {
public:
    static std::string GetYandexAPIKey();

    json FetchQuery(const std::string &url) override;

    json FetchRouteJSON(const std::string& departure_yandex_code, const std::string& destination_yandex_code,
                          const std::chrono::year_month_day& date) override;

    json FetchStationsList();
};
