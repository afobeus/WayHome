#pragma once

#include <string>
#include <ctime>
#include <map>
#include <optional>
#include <vector>
#include <memory>

enum class Currency {
    RUB,
    Unknown
};

struct RoutePrice {
    unsigned int whole;
    unsigned int cents;
    Currency currency;
};

enum class TransportType {
    Plane,
    Train,
    Suburban,
    Bus,
    Water,
    Helicopter
};

const std::map<std::string, TransportType> string_to_transport_type {
        {"plane",      TransportType::Plane},
        {"train",      TransportType::Train},
        {"suburban",   TransportType::Suburban},
        {"bus",        TransportType::Bus},
        {"water",      TransportType::Water},
        {"helicopter", TransportType::Helicopter},
};

const std::map<TransportType, std::string> transport_type_to_string {
        {TransportType::Plane,      "Полёт на самолёте"},
        {TransportType::Train,      "Поездка на поезде"},
        {TransportType::Suburban,   "Поездка на электричке"},
        {TransportType::Bus,        "Поездка на автобусе"},
        {TransportType::Water,      "Поездка на морском транспорте"},
        {TransportType::Helicopter, "Полёт на вертолёте"}
};

struct IRoute {
    virtual ~IRoute() = default;
    virtual std::string GetInfo() const = 0;
};

struct BaseRoute {
    BaseRoute() = default;
    BaseRoute(const std::string& from_city_title, const std::string& to_city_title, std::time_t departure_datetime,
              std::time_t arrival_datetime);

    std::string from_city_title;
    std::string to_city_title;
    std::time_t departure_datetime{};
    std::time_t arrival_datetime{};
};

struct DirectRoute final : IRoute, BaseRoute {
    DirectRoute(const std::string& from_city_title, const std::string& to_city_title, std::time_t departure_datetime,
                std::time_t arrival_datetime, const std::string& from_station_title, const std::string& to_station_title,
                TransportType transport_type, const std::optional<RoutePrice>& price=std::nullopt);
    std::string GetInfo() const override;

    std::string from_station_title;
    std::string to_station_title;
    TransportType transport_type;
    std::optional<RoutePrice> price;
};

struct SegmentRoute final : BaseRoute {
    SegmentRoute() = default;
    SegmentRoute(const std::string& from_city_title, const std::string& to_city_title, std::time_t departure_datetime,
                 std::time_t arrival_datetime, TransportType transport_type);

    TransportType transport_type;
};

struct Transfer {
    std::string transfer_from_title;
    std::string transfer_to_title;
};

struct Segment {
    SegmentRoute segment_route;
    std::optional<Transfer> transfer_after;
};

struct TransferRoute final : IRoute, BaseRoute {
    std::string GetInfo() const override;
    TransferRoute(const std::string& from_city_title, const std::string& to_city_title, std::time_t departure_datetime,
                  std::time_t arrival_datetime);
    void AddSegment(const Segment& segment);

    std::vector<Segment> segments;
};