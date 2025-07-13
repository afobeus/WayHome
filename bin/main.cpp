#include "lib/cli/RouteCLIController.h"
#include "lib/core/RouteService.h"



int main(const int argc, char* argv[]) {

    std::optional<TripInfo> trip_info = ParseArguments(argc, argv);
    if (trip_info.has_value()) {
        PrintRoutes(trip_info->departure_city, trip_info->arrival_city, trip_info->date);
    }

    return 0;
}
