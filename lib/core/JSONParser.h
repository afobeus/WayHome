#pragma once

#include "Route.h"

#include <memory>
#include <vector>

#include <nlohmann/json.hpp>

using json = nlohmann::json;

bool IsValidJSON(const json& routes_json);

std::vector<std::unique_ptr<IRoute>> MakeRoutes(const json& routes_json);

std::vector<std::pair<std::string, std::string>> GetCitiesCodes(const json& routes_json);