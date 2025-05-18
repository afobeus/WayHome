#include "Route.h"

BaseRoute::BaseRoute(const std::string& from_city_title, const std::string& to_city_title, const std::tm& departure_datetime,
                     const std::tm& arrival_datetime)
                         : from_city_title(from_city_title), to_city_title(to_city_title),
                           departure_datetime(departure_datetime), arrival_datetime(arrival_datetime) {}

DirectRoute::DirectRoute(const std::string& from_city_title, const std::string& to_city_title, const std::tm& departure_datetime,
                         const std::tm& arrival_datetime, const std::string& from_station_title, const std::string& to_station_title,
                         void* transport_type, const std::optional<RoutePrice>& price)
                             : BaseRoute(from_city_title, to_city_title, departure_datetime, arrival_datetime),
                               from_station_title(from_station_title), to_station_title(to_station_title),
                               transport_type(transport_type), price(price) {}

TransferRoute::TransferRoute(const std::string& from_city_title, const std::string& to_city_title, const std::tm& departure_datetime,
                             const std::tm& arrival_datetime, const std::vector<DirectRoute>& transfers)
                                 : BaseRoute(from_city_title, to_city_title, departure_datetime, arrival_datetime),
                                   transfers(transfers) {}

std::string DirectRoute::GetInfo() {
    return "This is a direct route";
}

std::string TransferRoute::GetInfo() {
    return "This is a transfer route";
}