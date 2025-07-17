#include "StringComparison.h"

#include <string>
#include <vector>
#include <algorithm>
#include <cctype>

#include "../core/Route.h"

std::vector<std::string> utf8_chars(const std::string& str) {
    std::vector<std::string> chars;
    for (size_t i = 0; i < str.size(); ) {
        const unsigned char c = str[i];
        size_t char_len = 1;
        if ((c & 0xF0) == 0xF0)
            char_len = 4;
        else if ((c & 0xE0) == 0xE0)
            char_len = 3;
        else if ((c & 0xC0) == 0xC0)
            char_len = 2;
        chars.push_back(str.substr(i, char_len));
        i += char_len;
    }
    return chars;
}

size_t LCS(const std::string& utf_str1, const std::string& utf_str2) {
    std::vector<std::string> str1 = utf8_chars(utf_str1);
    std::vector<std::string> str2 = utf8_chars(utf_str2);
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

size_t LevenshteinDistance(const std::string& utf_str1, const std::string& utf_str2) {
    const std::vector<std::string> str1 = utf8_chars(utf_str1);
    const std::vector<std::string> str2 = utf8_chars(utf_str2);
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

char32_t toLowerChar(const char32_t letter) {
    if (letter >= U'A' && letter <= U'Z')
        return letter + (U'a' - U'A');
    if (letter >= U'А' && letter <= U'Я')
        return letter + (U'а' - U'А');
    if (letter == U'Ё') return U'ё';
    return letter;
}


bool isLetter(const char32_t letter) {
    if ((letter >= U'a' && letter <= U'z') || (letter >= U'A' && letter <= U'Z'))
        return true;
    if ((letter >= U'а' && letter <= U'я') || (letter >= U'А' && letter <= U'Я') || letter == U'Ё' || letter == U'ё')
        return true;
    return false;
}


bool decodeUTF8(const std::string& str, size_t& i, char32_t& outChar) {
    if (i >= str.size()) return false;
    unsigned char letter = str[i];
    if (letter < 0x80) {
        outChar = letter;
        ++i;
        return true;
    }
    if ((letter >> 5) == 0x6) {
        if (i + 1 >= str.size()) return false;
        outChar = ((letter & 0x1F) << 6) | (str[i + 1] & 0x3F);
        i += 2;
        return true;
    }
    if ((letter >> 4) == 0xE) {
        if (i + 2 >= str.size()) return false;
        outChar = ((letter & 0x0F) << 12) |
                  ((str[i + 1] & 0x3F) << 6) |
                  (str[i + 2] & 0x3F);
        i += 3;
        return true;
    }
    ++i;
    return false;
}

std::string encodeUTF8(const char32_t letter) {
    std::string result;
    if (letter < 0x80) {
        result += static_cast<char>(letter);
    } else if (letter < 0x800) {
        result += static_cast<char>((letter >> 6) | 0xC0);
        result += static_cast<char>((letter & 0x3F) | 0x80);
    } else {
        result += static_cast<char>((letter >> 12) | 0xE0);
        result += static_cast<char>(((letter >> 6) & 0x3F) | 0x80);
        result += static_cast<char>((letter & 0x3F) | 0x80);
    }
    return result;
}

std::string NormalizeString(const std::string& input) {
    std::string output;
    size_t index = 0;
    while (index < input.size()) {
        char32_t letter;
        if (!decodeUTF8(input, index, letter))
            continue;
        if (isLetter(letter)) {
            letter = toLowerChar(letter);
            output += encodeUTF8(letter);
        }
    }
    return output;
}

std::vector<CityCode> GetTop10ByLCS(const std::vector<CityCode>& candidate_strings, const std::string& target_string) {
    std::vector<std::pair<CityCode, size_t>> scored;

    for (const CityCode& s : candidate_strings) {
        scored.emplace_back(s, LCS(target_string, s.city_title));
    }

    std::sort(scored.begin(), scored.end(),
              [](const auto& str1, const auto& str2) {
                  return str1.second > str2.second;
              });

    std::vector<CityCode> result;
    for (size_t i = 0; i < std::min(10UZ, scored.size()); ++i)
        result.push_back(scored[i].first);

    return result;
}

CityCode FindClosest(const std::vector<CityCode>& candidate_strings,
    const std::string& target_string) {
    // TODO maybe upgrade with normalized Levenshtein Distance and 1.5 for insertion and delete
    const std::vector<CityCode> top10 = GetTop10ByLCS(candidate_strings, target_string);
    CityCode best_match;
    size_t least_difference;
    for (const CityCode& str : top10) {
        const size_t difference = LevenshteinDistance(target_string, str.city_title);
        if (best_match.city_title.empty() or difference < least_difference) {
            least_difference = difference;
            best_match = str;
        }
    }
    return best_match;
}
