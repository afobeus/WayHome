#include "RouteCLIController.h"

std::string Help() {
    return "Note the usage:\n"
              "--departure \"city\" --arrival \"city\" --date \"DD.MM.YYYY\"\n";
}

bool IsValidDate(const int day, const int month, const int year) {
    if (year < 0 || month < 1 || month > 12 || day < 1)
        return false;
    const int days_in_month[] = {31, 28, 31, 30, 31, 30,
                                 31, 31, 30, 31, 30, 31};
    bool leap = (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
    if (leap && month == 2)
        return day <= 29;

    return day <= days_in_month[month - 1];
}

std::optional<std::chrono::year_month_day> ParseDate(const std::string& date_str) {
    int d, m, y;
    char sep1, sep2;
    std::istringstream iss(date_str);
    iss >> d >> sep1 >> m >> sep2 >> y;

    if (sep1 != '.' || sep2 != '.' || !IsValidDate(d, m, y)) {
        std::cerr << "Invalid date was provided: " << date_str << '\n' + Help();
        return std::nullopt;
    }

    return std::chrono::year{y} / std::chrono::month{static_cast<unsigned>(m)} / std::chrono::day{static_cast<unsigned>(d)};
}

std::optional<TripInfo> ParseArguments(const int argc, char* argv[]) {
    TripInfo info;
    bool departure_found = false, arrival_found = false, date_found = false;

    for (int i = 1; i < argc - 1; ++i) {
        std::string arg = argv[i];

        if (arg == "--departure") {
            info.departure_city = argv[++i];
            if (info.departure_city.empty()) {
                std::cerr << "Departure city cannot be empty.\n" + Help();
                return std::nullopt;
            }
            departure_found = true;
        } else if (arg == "--arrival") {
            info.arrival_city = argv[++i];
            if (info.arrival_city.empty()) {
                std::cerr << "Arrival city cannot be empty.\n" + Help();
                return std::nullopt;
            }
            arrival_found = true;
        } else if (arg == "--date") {
            auto parsed_date = ParseDate(argv[++i]);
            if (parsed_date.has_value()) {
                info.date = parsed_date.value();
            } else {
                return std::nullopt;
            }
            date_found = true;
        }
    }

    if (!departure_found || !arrival_found || !date_found) {
        std::cerr << "Missing one or more required arguments.\n" + Help();
        return std::nullopt;
    }

    return info;
}