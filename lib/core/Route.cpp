#include "Route.h"

#include <iomanip>

#include "../util/ChronoConversion.h"

BaseRoute::BaseRoute(const std::string& from_city_title, const std::string& to_city_title, const std::time_t departure_datetime,
                     const std::time_t arrival_datetime)
                         : from_city_title(from_city_title), to_city_title(to_city_title),
                           departure_datetime(departure_datetime), arrival_datetime(arrival_datetime) {}

DirectRoute::DirectRoute(const std::string& from_city_title, const std::string& to_city_title, const std::time_t departure_datetime,
                         const std::time_t arrival_datetime, const std::string& from_station_title, const std::string& to_station_title,
                         void* transport_type, const std::optional<RoutePrice>& price)
                             : BaseRoute(from_city_title, to_city_title, departure_datetime, arrival_datetime),
                               from_station_title(from_station_title), to_station_title(to_station_title),
                               transport_type(transport_type), price(price) {}

TransferRoute::TransferRoute(const std::string& from_city_title, const std::string& to_city_title, const std::time_t departure_datetime,
                             const std::time_t arrival_datetime, const std::vector<DirectRoute>& transfers)
                                 : BaseRoute(from_city_title, to_city_title, departure_datetime, arrival_datetime),
                                   transfers(transfers) {}

std::string DirectRoute::GetInfo() {
    const std::string str_departure_datetime = GetDateTime(departure_datetime);
    const std::string str_arrival_datetime =  GetDateTime(arrival_datetime);

    return "A route " + from_city_title + " - " + to_city_title + '\n' +
           "Departure: " + str_departure_datetime + " from " + from_station_title + '\n' +
           "Arrival: " + str_arrival_datetime + " to " + to_station_title + '\n';
}

std::string TransferRoute::GetInfo() {
    return "This is a transfer route";
}