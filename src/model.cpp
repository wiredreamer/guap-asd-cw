#include "model.h"

#include <cctype>

namespace {
bool allDigits(const std::string& s, int from, int to) {
    for (int i = from; i < to; ++i)
        if (!std::isdigit(static_cast<unsigned char>(s[i])))
            return false;
    return true;
}

bool isLatinLetter(char c) {
    return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z');
}
}  // namespace

bool isValidPassport(const std::string& s) {
    if (s.size() != 11)
        return false;
    if (!allDigits(s, 0, 4))
        return false;
    if (s[4] != '-')
        return false;
    return allDigits(s, 5, 11);
}

bool isValidFlightNumber(const std::string& s) {
    if (s.size() != 7)
        return false;
    for (int i = 0; i < 3; ++i)
        if (!isLatinLetter(s[i]))
            return false;
    if (s[3] != '-')
        return false;
    return allDigits(s, 4, 7);
}

bool isValidTicketNumber(const std::string& s) {
    return s.size() == 9 && allDigits(s, 0, 9);
}
