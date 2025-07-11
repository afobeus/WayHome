#pragma once

#include <vector>
#include <string>

size_t LCS(const std::string& str1, const std::string& str2);

size_t LevenshteinDistance(const std::string& str1, const std::string& str2);

std::string NormalizeString(const std::string& input);

std::vector<std::string> GetTop10ByLCS(const std::vector<std::string>& candidate_strings, const std::string& target_string);

std::string FindClosest(const std::vector<std::string>& candidate_strings, const std::string& target_string);
