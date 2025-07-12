#include "RouteService.h"

#include <iostream>

#include "../api/RouteAPI.h"
#include "JSONParser.h"
#include "../util/StringComparison.h"

void PrintRoutes(const std::string& departure_yandex_code, const std::string& destination_yandex_code,
                 const std::chrono::year_month_day date) {

    RouteAPI api_object;
    // std::cout << api_object.FetchRouteJSON(departure_yandex_code, destination_yandex_code, date) << std::endl; // TODO delete
    const auto api_response_json = api_object.FetchRouteJSON(departure_yandex_code, destination_yandex_code, date);
    const auto routes = MakeRoutes(api_response_json);
    if (routes.empty()) {
        std::cout << "Не было найдено ни одного маршрута!";
        return;
    }
    std::cout << "Найдено " << routes.size() << " маршрутов\n";
    for (int i = 0; i < routes.size(); ++i) {
        std::cout << i + 1 << ')' << routes[i]->GetInfo() << '\n';
    }
}

std::pair<std::string, std::string> GetCityCodes(const std::string& departure_city_title, const std::string& arrival_city_title) {
    RouteAPI api_object;
    const auto api_request_json = api_object.FetchStationsList();
    const auto cities_codes = GetCitiesCodes(api_request_json);
    std::string departure_city_code = FindClosest(cities_codes, NormalizeString(departure_city_title)).second;
    std::string arrival_city_code = FindClosest(cities_codes, NormalizeString(arrival_city_title)).second;
    return {departure_city_code, arrival_city_code};
}
