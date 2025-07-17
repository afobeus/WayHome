#pragma once

#include <vector>
#include <string>

#include "../core/Route.h"

std::string NormalizeString(const std::string& input);

CityCode FindClosest(const std::vector<CityCode>& candidate_strings,
    const std::string& target_string);
