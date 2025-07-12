#include "RouteAPI.h"

#include <iomanip>
#include <iostream>

#include <cpr/cpr.h>

std::string RouteAPI::GetYandexAPIKey() {
    const char* api_key = std::getenv("YANDEX_API_KEY");
    if (!api_key) {
        std::cerr << "Environment variable YANDEX_API_KEY is not set.\n";
        return "";
    }
    return api_key;
}

json RouteAPI::FetchQuery(const std::string &url) {
    cpr::Response r = cpr::Get(cpr::Url{url});
    if (r.status_code != 200) {
        std::cerr << "Bad status code: " << r.status_code << "\n";
        return {};
    }
    auto result = json::parse(r.text, nullptr, false);
    if (result.is_discarded()) {
        std::cerr << "Invalid JSON was fetched.\n";
        return {};
    }
    return result;
}

json RouteAPI::FetchRouteJSON(const std::string& departure_yandex_code, const std::string& destination_yandex_code,
                              const std::chrono::year_month_day& date) {
     std::ostringstream oss;
     oss << static_cast<int>(date.year()) << "-"
         << std::setw(2) << std::setfill('0') << static_cast<unsigned>(date.month()) << "-"
         << std::setw(2) << std::setfill('0') << static_cast<unsigned>(date.day());
     const std::string date_str = oss.str();

     const std::string url = "https://api.rasp.yandex.net/v3.0/search/"
                             "?apikey=" + GetYandexAPIKey() +
                             "&from=" + departure_yandex_code +
                             "&to=" + destination_yandex_code +
                             "&format=json" +
                             "&date=" + date_str +
                             "&transfers=true";
     return FetchQuery(url);
 }

json RouteAPI::FetchStationsList() {
    const std::string url = "https://api.rasp.yandex.net/v3.0/stations_list/"
                             "?apikey=" + GetYandexAPIKey() +
                             "&lang=ru_RU" +
                             "&format=json";
    return FetchQuery(url);
}
