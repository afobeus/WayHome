#pragma once

#include <string>
#include <ctime>
#include <optional>
#include <vector>

enum class Currency {
    RUB,
    Unknown
};

struct RoutePrice {
    unsigned int whole;
    unsigned int cents;
    Currency currency;
};

struct IRoute {
    virtual ~IRoute() = default;
    virtual std::string GetInfo() = 0;
};

struct BaseRoute : IRoute {
    BaseRoute(const std::string& from_city_title, const std::string& to_city_title, std::time_t departure_datetime,
              std::time_t arrival_datetime);

    std::string from_city_title;
    std::string to_city_title;
    std::time_t departure_datetime;
    std::time_t arrival_datetime;
};

struct DirectRoute final : BaseRoute {
    std::string GetInfo() override;
    DirectRoute(const std::string& from_city_title, const std::string& to_city_title, std::time_t departure_datetime,
                std::time_t arrival_datetime, const std::string& from_station_title, const std::string& to_station_title,
                void* transport_type, const std::optional<RoutePrice>& price=std::nullopt);

    std::string from_station_title;
    std::string to_station_title;
    void* transport_type; // void* is a dummy until I realize what to put instead of it
    std::optional<RoutePrice> price;
};

struct TransferRoute final : BaseRoute {
    std::string GetInfo() override;
    TransferRoute(const std::string& from_city_title, const std::string& to_city_title, std::time_t departure_datetime,
                  std::time_t arrival_datetime, const std::vector<DirectRoute>& transfers);

    std::vector<DirectRoute> transfers;
};