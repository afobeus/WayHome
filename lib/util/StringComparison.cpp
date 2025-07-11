#include "StringComparison.h"

#include <string>
#include <vector>
#include <algorithm>
#include <cctype>

size_t LCS(const std::string& str1, const std::string& str2) {
    size_t i = 0, j = 0, count = 0;
    while (i < str1.size() && j < str2.size()) {
        if (str1[i] == str2[j]) {
            ++count;
            ++i;
        }
        ++j;
    }
    return count;
}

size_t LevenshteinDistance(const std::string& str1, const std::string& str2) {
    const size_t m = str1.size();
    const size_t n = str2.size();
    std::vector<std::vector<size_t>> dp(m + 1, std::vector<size_t>(n+1, 0));

    for (size_t i = 0; i <= m; ++i) {
        dp[i][0] = i;
    }
    for (size_t j = 0; j <= n; ++j) {
        dp[0][j] = j;
    }

    for (size_t i = 1; i <= m; ++i) {
        for (size_t j = 1; j <= n; ++j) {
            if (str1[i - 1] == str2[j - 1]) {
                dp[i][j] = dp[i - 1][j - 1];
            }
            else {
                dp[i][j] = std::min({ dp[i - 1][j], dp[i][j - 1], dp[i - 1][j - 1]}) + 1;
            }
        }
    }
    return dp[m][n];
}

std::string NormalizeString(const std::string& input) {
    std::string result;
    result.reserve(input.size());
    for (const char symbol : input) {
        char lower_ch = static_cast<char>(std::tolower(static_cast<unsigned char>(symbol)));
        if (!std::isspace(static_cast<unsigned char>(lower_ch))) {
            result += lower_ch;
        }
    }
    return result;
}

std::vector<std::string> GetTop10ByLCS(const std::vector<std::string>& candidate_strings, const std::string& target_string) {
    std::vector<std::pair<std::string, size_t>> scored;

    for (const std::string& s : candidate_strings) {
        scored.emplace_back(s, LCS(target_string, s));
    }

    std::sort(scored.begin(), scored.end(),
              [](const auto& str1, const auto& str2) {
                  return str1.second > str2.second;
              });

    std::vector<std::string> result;
    for (size_t i = 0; i < std::min(10UZ, scored.size()); ++i)
        result.push_back(scored[i].first);

    return result;
}

std::string FindClosest(const std::vector<std::string>& candidate_strings, const std::string& target_string) {
    const std::vector<std::string> top10 = GetTop10ByLCS(candidate_strings, target_string);

    std::string best_match;
    size_t least_difference;
    for (const std::string& str : top10) {
        const size_t difference = LevenshteinDistance(target_string, str);
        if (best_match.empty() or difference < least_difference) {
            least_difference = difference;
            best_match = str;
        }
    }
    return best_match;
}
