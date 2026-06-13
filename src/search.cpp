#include "search.h"

#include <algorithm>
#include <vector>

namespace {

constexpr int ALPHABET = 256;

void buildBadChar(const std::string& p, std::vector<int>& bad) {
    bad.assign(ALPHABET, -1);
    for (int i = 0; i < static_cast<int>(p.size()); ++i)
        bad[static_cast<unsigned char>(p[i])] = i;
}

void buildGoodSuffix(const std::string& p, std::vector<int>& shift) {
    const int m = static_cast<int>(p.size());
    shift.assign(m + 1, 0);
    std::vector<int> border(m + 1, 0);

    int i = m, j = m + 1;
    border[i] = j;
    while (i > 0) {
        while (j <= m && p[i - 1] != p[j - 1]) {
            if (shift[j] == 0)
                shift[j] = j - i;
            j = border[j];
        }
        --i;
        --j;
        border[i] = j;
    }

    j = border[0];
    for (i = 0; i <= m; ++i) {
        if (shift[i] == 0)
            shift[i] = j;
        if (i == j)
            j = border[j];
    }
}

}  // namespace

bool boyerMooreSearch(const std::string& text, const std::string& pattern) {
    const int n = static_cast<int>(text.size());
    const int m = static_cast<int>(pattern.size());

    if (m == 0)
        return true;
    if (m > n)
        return false;

    std::vector<int> bad;
    std::vector<int> goodSuffix;
    buildBadChar(pattern, bad);
    buildGoodSuffix(pattern, goodSuffix);

    int s = 0;
    while (s <= n - m) {
        int j = m - 1;
        while (j >= 0 && pattern[j] == text[s + j])
            --j;

        if (j < 0)
            return true;

        int badShift = j - bad[static_cast<unsigned char>(text[s + j])];
        if (badShift < 1)
            badShift = 1;
        int goodShift = goodSuffix[j + 1];
        s += std::max(badShift, goodShift);
    }
    return false;
}
