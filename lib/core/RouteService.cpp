#include "RouteService.h"

#include <iostream>

#include "../api/RouteAPI.h"
#include "RouteFactory.h"

void PrintRoutes(const std::string& departure_yandex_code, const std::string& destination_yandex_code,
    const std::chrono::year_month_day date) {

    RouteAPI api_object;
    // std::cout << api_object.FetchRouteJSON(departure_yandex_code, destination_yandex_code, date) << std::endl;
    const auto api_response_json = api_object.FetchRouteJSON(departure_yandex_code, destination_yandex_code, date);
    const auto routes = MakeRoutes(api_response_json);
    for (const auto& route : routes) {
        std::cout << route->GetInfo() << std::endl;
    }
}
