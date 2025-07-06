#include "RouteFactory.h"

#include "../util/ChronoConversion.h"

bool IsValidJSON(const json& routes_json) {
    if (!routes_json.contains("segments") || !routes_json["segments"].is_array()) {
        return false;
    }
    // # TODO: make more conditions after specified what I need
    return true;
}

std::unique_ptr<BaseRoute> CreateDirectRoute(const std::string& from_city_title, const std::string& to_city_title,
                                             const json& route_json) {
    std::time_t departure_datetime = ParseISO8601(route_json["departure"]);
    std::time_t arrival_datetime = ParseISO8601(route_json["arrival"]);
    std::string from_station_title = route_json["from"]["popular_title"].is_null()
                                        ? route_json["from"]["title"]
                                        : route_json["from"]["popular_title"];
    std::string to_station_title = route_json["to"]["popular_title"].is_null()
                                        ? route_json["to"]["title"]
                                        : route_json["to"]["popular_title"];
    auto result = std::make_unique<DirectRoute>(from_city_title,
                                                to_city_title,
                                                departure_datetime,
                                                arrival_datetime,
                                                from_station_title,
                                                to_station_title,
                                                nullptr,
                                                RoutePrice());
    return result;
}

std::vector<std::unique_ptr<BaseRoute>> MakeRoutes(const json& routes_json) {
    std::vector<std::unique_ptr<BaseRoute>> routes;
    const std::string from_city_title = routes_json["search"]["from"]["popular_title"].is_null()
                                           ? routes_json["search"]["from"]["title"]
                                           : routes_json["search"]["from"]["popular_title"];
    const std::string to_city_title = routes_json["search"]["to"]["popular_title"].is_null()
                                         ? routes_json["search"]["to"]["title"]
                                         : routes_json["search"]["to"]["popular_title"];

    for (auto element : routes_json["segments"]) {
        if (element["has_transfers"]) {
            "pops";
        } else {
            routes.push_back(CreateDirectRoute(from_city_title, to_city_title, element));
        }
    }

    return routes;
}
