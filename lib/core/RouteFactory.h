#pragma once

#include "Route.h"

#include <memory>
#include <vector>

#include <nlohmann/json.hpp>

using json = nlohmann::json;

bool ValidateJSON(const json& routes_json);

std::vector<std::unique_ptr<BaseRoute>> MakeRoutes(const json& routes_json);
