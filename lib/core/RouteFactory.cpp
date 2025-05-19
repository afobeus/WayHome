#include "RouteFactory.h"

bool ValidateJSON(const json& routes_json) {
    if (!routes_json.contains("segments") || !routes_json["segments"].is_array()) {
        return false;
    }
}

std::vector<std::unique_ptr<BaseRoute>> MakeRoutes(const json& routes_json) {
    std::vector<std::unique_ptr<BaseRoute>> routes;

    return routes;
}
