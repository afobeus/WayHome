#include "Route.h"

#include <iomanip>

#include "../util/DateTimeTools.h"

BaseRoute::BaseRoute(const std::string& from_city_title, const std::string& to_city_title, const std::time_t departure_datetime,
                     const std::time_t arrival_datetime)
                         : from_city_title(from_city_title), to_city_title(to_city_title),
                           departure_datetime(departure_datetime), arrival_datetime(arrival_datetime) {}

DirectRoute::DirectRoute(const std::string& from_city_title, const std::string& to_city_title, const std::time_t departure_datetime,
                         const std::time_t arrival_datetime, const std::string& from_station_title, const std::string& to_station_title,
                         const TransportType transport_type, const std::optional<RoutePrice>& price)
                             : BaseRoute(from_city_title, to_city_title, departure_datetime, arrival_datetime),
                               from_station_title(from_station_title), to_station_title(to_station_title),
                               transport_type(transport_type), price(price) {}

SegmentRoute::SegmentRoute(const std::string& from_city_title, const std::string& to_city_title, const std::time_t departure_datetime,
                           const std::time_t arrival_datetime, const TransportType transport_type)
                               : BaseRoute(from_city_title, to_city_title, departure_datetime, arrival_datetime),
                                 transport_type(transport_type) {}

TransferRoute::TransferRoute(const std::string& from_city_title, const std::string& to_city_title, const std::time_t departure_datetime,
                             const std::time_t arrival_datetime)
                                 : BaseRoute(from_city_title, to_city_title, departure_datetime, arrival_datetime) {}

void TransferRoute::AddSegment(const Segment &segment) {
    segments.push_back(segment);
}

std::string DirectRoute::GetInfo() const { // TODO rewrite both functions on russian and with nicer code
    const std::string str_departure_datetime = GetDateTime(departure_datetime);
    const std::string str_arrival_datetime =  GetDateTime(arrival_datetime);

    return transport_type_to_phrase.at(transport_type) + '\n' +
           "\tОтправление: " + str_departure_datetime + " из " + from_station_title + '\n' +
           "\tПрибытие: " + str_arrival_datetime + " в " + to_station_title + '\n';
}

void DirectRoute::Serialize(json& routes_array) const {
    const json route_info = {
        {"has_transfers", false},
        {"from_city_title", from_city_title},
        {"to_city_title", to_city_title},
        {"departure_datetime", TimeToISOString(departure_datetime)},
        {"arrival_datetime", TimeToISOString(arrival_datetime)},
        {"from_station_title", from_station_title},
        {"to_station_title", to_station_title},
        {"transport_type", transport_type_to_string.at(transport_type)},
        {"price", "pops"}, // TODO
    };
    routes_array.push_back(route_info);
}

std::string TransferRoute::GetInfo() const {
    std::string result = "Маршрут с " + std::to_string(segments.size() - 1) + " пересадками" + '\n';
    for (int i = 0; i < segments.size(); ++i) {
        result += "\t- " + transport_type_to_phrase.at(segments[i].segment_route.transport_type) + ' ' + segments[i].segment_route.from_city_title + " - " + segments[i].segment_route.to_city_title + '\n'; // TODO replace pops with a phrase for every kind of transport(e.g plane - "A flight on a plane")
        if (i + 1 < segments.size())
            result += "\t- Пересадка из " + segments[i].transfer_after->transfer_from_title + " в " +
                  segments[i].transfer_after->transfer_to_title + '\n';
    }
    return result;
}

void TransferRoute::Serialize(json &routes_array) const {
    json segments_json = json::array();
    for (const Segment& segment : segments) {
        const json segment_info = {
            {
                "segment_route", {
                    {"from_city_title", segment.segment_route.from_city_title},
                    {"to_city_title", segment.segment_route.to_city_title},
                    {"departure_datetime", TimeToISOString(segment.segment_route.departure_datetime)},
                    {"arrival_datetime", TimeToISOString(segment.segment_route.arrival_datetime)},
                    {"transport_type", transport_type_to_string.at(segment.segment_route.transport_type)},
                    },
            },
            {
                "transfer", {
                {"transfer_from_title", segment.transfer_after.has_value()
                                              ? segment.transfer_after.value().transfer_from_title
                                              : ""},
                {"transfer_to_title", segment.transfer_after.has_value()
                                              ? segment.transfer_after.value().transfer_to_title
                                              : ""}
                }
            }
        };
        segments_json.push_back(segment_info);
    }
    const json route_info = {
        {"has_transfers", true},
        {"from_city_title", from_city_title},
        {"to_city_title", to_city_title},
        {"departure_datetime", TimeToISOString(departure_datetime)},
        {"arrival_datetime", TimeToISOString(arrival_datetime)},
        {"segments", segments_json},
    };
    routes_array.push_back(route_info);
}
