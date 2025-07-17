#include "CacheManager.h"

#include <fstream>

#include <nlohmann/json.hpp>

#include "JSONParser.h"
#include "../api/RouteAPI.h"
#include "../util/DateTimeTools.h"

using json = nlohmann::json;

constexpr auto kCitiesCodesFilename = "CitiesCodes.json";
constexpr auto kRoutesFilename = "Routes.json";

std::unique_ptr<IRoute> LoadDirectRoute(const json& route_json) {
    return std::make_unique<DirectRoute>(route_json["from_city_title"],
                                         route_json["to_city_title"],
                                         ParseISO8601(route_json["departure_datetime"]),
                                         ParseISO8601(route_json["arrival_datetime"]),
                                         route_json["from_station_title"],
                                         route_json["to_station_title"],
                                         string_to_transport_type.at(route_json["transport_type"]),
                                         RoutePrice()); // TODO add price
}

std::unique_ptr<IRoute> LoadTransferRoute(const json& route_json) {
    auto result = std::make_unique<TransferRoute>(route_json["from_city_title"],
                                                  route_json["to_city_title"],
                                                  ParseISO8601(route_json["departure_datetime"]),
                                                  ParseISO8601(route_json["arrival_datetime"]));
    for (const auto& segment_json : route_json["segments"]) {
        const SegmentRoute segment_route(segment_json["segment_route"]["from_city_title"],
                                   segment_json["segment_route"]["to_city_title"],
                                   ParseISO8601(segment_json["segment_route"]["departure_datetime"]),
                                   ParseISO8601(segment_json["segment_route"]["arrival_datetime"]),
                                   string_to_transport_type.at(segment_json["segment_route"]["transport_type"]));
        const Transfer transfer(segment_json["transfer"]["transfer_from_title"],
                          segment_json["transfer"]["transfer_to_title"]);
        const Segment segment(segment_route, transfer);
        result->AddSegment(segment);
    }
    return result;
}

std::vector<std::unique_ptr<IRoute>> LoadRoutesFromJSON(const json& routes_json) {
    std::vector<std::unique_ptr<IRoute>> routes;
    for (const auto& element : routes_json["routes"]) {
        if (element["has_transfers"]) {
            routes.push_back(LoadTransferRoute(element));
        } else {
            routes.push_back(LoadDirectRoute(element));
        }
    }
    return routes;
}

void CacheRoutes(const std::vector<std::unique_ptr<IRoute>>& routes, const std::string& route_header) {
    json routes_json;
    std::ifstream input_file(kRoutesFilename);
    if (input_file.is_open()) {
        input_file >> routes_json;
    }
    input_file.close();
    json current_route_json;
    current_route_json["request_datetime"] = CurrentTimeToString();
    current_route_json["routes"] = json::array();
    for (const std::unique_ptr<IRoute>& route : routes) {
        route->Serialize(current_route_json["routes"]);
    }
    routes_json[route_header] = current_route_json;
    std::ofstream output_file(kRoutesFilename);
    output_file << routes_json;
}

std::vector<std::unique_ptr<IRoute>> LoadRoutes(const std::string& departure_city_code, const std::string& arrival_city_code,
                                                const std::chrono::year_month_day date) {
    std::vector<std::unique_ptr<IRoute>> result;
    std::ifstream input_file(kRoutesFilename);
    const std::string route_header = departure_city_code + '-' + arrival_city_code + std::format("{:%Y-%m-%d}", date);
    if (input_file.is_open()) { // TODO make sure file is valid
        json cities_codes_json;
        input_file >> cities_codes_json;
        // TODO check if cache needs to be freshen
        result = LoadRoutesFromJSON(cities_codes_json[route_header]);
    }
    input_file.close();
    if (result.empty()) {
        RouteAPI api_object;
        const auto api_response_json = api_object.FetchRouteJSON(departure_city_code, arrival_city_code, date);
        result = MakeRoutes(api_response_json);
        CacheRoutes(result, route_header);
    }
    return result;
}

std::vector<CityCode> LoadCitiesCodesFromJSON(const json& cities_codes_json) {
    std::vector<CityCode> cities_codes;
    for (const auto& city_code : cities_codes_json["cities_codes"]) {
        cities_codes.emplace_back(city_code["title"], city_code["yandex_code"]);
    }
    return cities_codes;
}

void CacheCitiesCodes(const std::vector<CityCode>& cities_codes) {
    json cities_codes_json;
    cities_codes_json["request_datetime"] = CurrentTimeToString();
    cities_codes_json["cities_codes"] = json::array();
    for (const CityCode& city_code : cities_codes) {
        json current_city_code = {
            {"title", city_code.city_title},
            {"yandex_code", city_code.code},
        };
        cities_codes_json["cities_codes"].emplace_back(current_city_code);
    }
    std::ofstream output_file(kCitiesCodesFilename);
    output_file << cities_codes_json;
}

std::vector<CityCode> LoadCitiesCodes() {
    std::vector<CityCode> result;
    std::ifstream input_file(kCitiesCodesFilename);
    if (input_file.is_open()) { // TODO make sure file is valid
        json cities_codes_json;
        input_file >> cities_codes_json;
        // TODO check if cache needs to be freshen
        result = LoadCitiesCodesFromJSON(cities_codes_json);
    }
    input_file.close();
    if (result.empty()) {
        RouteAPI api_object;
        const auto api_request_json = api_object.FetchStationsList();
        result = MakeCitiesCodes(api_request_json);
        CacheCitiesCodes(result);
    }
    return result;
}