#pragma once

#include <vector>
#include <string>

std::string NormalizeString(const std::string& input);

std::pair<std::string, std::string> FindClosest(const std::vector<std::pair<std::string, std::string>>& candidate_strings,
    const std::string& target_string);
