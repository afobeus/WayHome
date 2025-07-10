#include "RouteFactory.h"

#include "../util/ChronoConversion.h"

inline std::string GetTitle(const json& route_json) { // TODO: study something about inline and also deside whether it is necessary here or not
    return route_json["popular_title"].is_null() or route_json["popular_title"] == "" ? route_json["title"] : route_json["popular_title"];
}

bool IsValidJSON(const json& routes_json) {
    if (!routes_json.contains("segments") || !routes_json["segments"].is_array()) {
        return false;
    }
    // TODO: make more conditions after specified what I need
    return true;
}

std::unique_ptr<IRoute> CreateDirectRoute(const std::string& from_city_title, const std::string& to_city_title,
                                             const json& route_json) {
    std::time_t departure_datetime = ParseISO8601(route_json["departure"]);
    std::time_t arrival_datetime = ParseISO8601(route_json["arrival"]);
    TransportType transport_type = string_to_transport_type.at(route_json["thread"]["transport_type"]);
    return std::make_unique<DirectRoute>(from_city_title,
                                         to_city_title,
                                         departure_datetime,
                                         arrival_datetime,
                                         GetTitle(route_json["from"]),
                                         GetTitle(route_json["to"]),
                                         transport_type,
                                         RoutePrice()); // TODO add price
}

SegmentRoute CreateSegmentRoute(const json& segment_json) {
    std::time_t departure_datetime = ParseISO8601(segment_json["departure"]);
    std::time_t arrival_datetime = ParseISO8601(segment_json["arrival"]);
    const std::string from_city_title = GetTitle(segment_json["from"]);
    const std::string to_city_title = GetTitle(segment_json["to"]);
    return {from_city_title, to_city_title, departure_datetime, arrival_datetime,
        string_to_transport_type.at(segment_json["thread"]["transport_type"])};
}

std::unique_ptr<IRoute> CreateTransferRoute(const std::string& from_city_title, const std::string& to_city_title,
                                             const json& route_json) {

    std::time_t departure_datetime = ParseISO8601(route_json["departure"]);
    std::time_t arrival_datetime = ParseISO8601(route_json["arrival"]);
    auto result = std::make_unique<TransferRoute>(from_city_title,
                                                  to_city_title,
                                                  departure_datetime,
                                                  arrival_datetime);
    Segment segment;
    for (const auto &element: route_json["details"]) {
        if (element.contains("is_transfer")) {
            auto a = GetTitle(element["transfer_from"]);
            segment.transfer_after = {
                GetTitle(element["transfer_from"]),
                GetTitle(element["transfer_to"])};
            result->AddSegment(segment);
        } else {
            segment.segment_route = CreateSegmentRoute(element);
        }
    }
    result->AddSegment(segment);

    return result;
}

std::vector<std::unique_ptr<IRoute>> MakeRoutes(const json& routes_json) {
    std::vector<std::unique_ptr<IRoute>> routes;
    const std::string from_city_title = GetTitle(routes_json["search"]["from"]);
    const std::string to_city_title = GetTitle(routes_json["search"]["to"]);

    for (const auto& element : routes_json["segments"]) {
        if (element["has_transfers"]) {
            routes.push_back(CreateTransferRoute(from_city_title, to_city_title, element));
        } else {
            routes.push_back(CreateDirectRoute(from_city_title, to_city_title, element));
        }
    }

    return routes;
}
