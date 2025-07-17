#include "RouteService.h"

#include <iostream>

#include "../api/RouteAPI.h" // TODO
#include "JSONParser.h"// TODO
#include "CacheManager.h"
#include "../util/StringComparison.h"

void PrintRoutes(const std::string& departure_city_user_title, const std::string& destination_city_user_title,
                 const std::chrono::year_month_day date) {

    RouteAPI api_object;
    const std::pair<std::string, std::string> cities_codes = GetCitiesCodes(departure_city_user_title, destination_city_user_title);
    std::cout << cities_codes.first << " " << cities_codes.second << std::endl;
    const auto routes = LoadRoutes(cities_codes.first, cities_codes.second, date);
    if (routes.empty()) {
        std::cout << "Не было найдено ни одного маршрута!";
        return;
    }
    //std::string departure_city_title = dynamic_cast<TransferRoute*>(routes[0]); // TODO add real city titles that were found
    std::cout << "Найдено " << routes.size() << " маршрутов " << departure_city_user_title << " - " << destination_city_user_title << ":\n";
    for (int i = 0; i < routes.size(); ++i) {
        std::cout << i + 1 << ')' << routes[i]->GetInfo() << '\n';
    }
}

std::pair<std::string, std::string> GetCitiesCodes(const std::string& departure_city_title, const std::string& arrival_city_title) {
    const auto cities_codes = LoadCitiesCodes();
    std::string departure_city_code = FindClosest(cities_codes, NormalizeString(departure_city_title)).code;
    std::string arrival_city_code = FindClosest(cities_codes, NormalizeString(arrival_city_title)).code;
    return {departure_city_code, arrival_city_code};
}

