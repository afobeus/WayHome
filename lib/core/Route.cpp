#include "Route.h"

#include <iomanip>

#include "../util/ChronoConversion.h"

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

    return transport_type_to_string.at(transport_type) + '\n' +
           "\tОтправление: " + str_departure_datetime + " из " + from_station_title + '\n' +
           "\tПрибытие: " + str_arrival_datetime + " в " + to_station_title + '\n';
}

std::string TransferRoute::GetInfo() const {
    std::string result = "Маршрут с " + std::to_string(segments.size() - 1) + " пересадками" + '\n';
    for (int i = 0; i < segments.size(); ++i) {
        result += "\t- " + transport_type_to_string.at(segments[i].segment_route.transport_type) + ' ' + segments[i].segment_route.from_city_title + " - " + segments[i].segment_route.to_city_title + '\n'; // TODO replace pops with a phrase for every kind of transport(e.g plane - "A flight on a plane")
        if (i + 1 < segments.size())
            result += "\t- Пересадка из " + segments[i].transfer_after->transfer_from_title + " в " +
                  segments[i].transfer_after->transfer_to_title + '\n';
    }
    return result;
}