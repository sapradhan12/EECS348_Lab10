#ifndef MATRIX_HPP
#define MATRIX_HPP

#include <string>
#include <cctype>

bool isValidDouble(const std::string &s) {
    if (s == "")
        return false;
    std::string::size_type i = 0;
    if (s[i] == '+' || s[i] == '-')
        i++;
    if (i >= s.size())
        return false;
    bool hasDigitsBefore = false;
    while (i < s.size() && std::isdigit(s[i])) {
        hasDigitsBefore = true;
        i++;
    }

    if (!hasDigitsBefore)
        return false;
    if (i < s.size() && s[i] == '.') {
        i++;
        bool hasDigitsAfter = false;
        while (i < s.size() && std::isdigit(s[i])) {
            hasDigitsAfter = true;
            i++;
        }
        if (!hasDigitsAfter)
            return false;
    }
    return (i == s.size());
}

std::string removeLeadingZeros(const std::string &s) {
    std::string::size_type i = 0;
    while (i < s.size() && s[i] == '0')
        i++;
    if (i == s.size())
        return "0";
    return s.substr(i);
}

std::string reverseString(const std::string &s) {
    std::string rev = "";
    for (int i = static_cast<int>(s.size()); i > 0; ) {
        i--;
        rev.push_back(s[i]);
    }
    return rev;
}

struct ParsedNumber {
    int sign;
    std::string integer;
    std::string fraction;
};

ParsedNumber parseNumber(const std::string &s) {
    ParsedNumber num;
    std::string::size_type i = 0;
    if (s[i] == '+') {
        num.sign = 1;
        i++;
    } else if (s[i] == '-') {
        num.sign = -1;
        i++;
    } else {
        num.sign = 1;
    }
    std::string intPart = "";
    while (i < s.size() && std::isdigit(s[i])) {
        intPart.push_back(s[i]);
        i++;
    }
    if (intPart == "")
        intPart = "0";
    num.integer = removeLeadingZeros(intPart);
    std::string fracPart = "";
    if (i < s.size() && s[i] == '.') {
        i++;
        while (i < s.size() && std::isdigit(s[i])) {
            fracPart.push_back(s[i]);
            i++;
        }
    }
    num.fraction = fracPart;
    return num;
}

std::string addNonNegatives(const std::string &a, const std::string &b) {
    int i = static_cast<int>(a.size()) - 1;
    int j = static_cast<int>(b.size()) - 1;
    int carry = 0;
    std::string result = "";
    while (i >= 0 || j >= 0 || carry) {
        int x = (i >= 0) ? a[i] - '0' : 0;
        int y = (j >= 0) ? b[j] - '0' : 0;
        int sum = x + y + carry;
        carry = sum / 10;
        result.push_back('0' + (sum % 10));
        i--;
        j--;
    }
    return reverseString(result);
}

int compareNonNegatives(const std::string &a, const std::string &b) {
    std::string A = removeLeadingZeros(a);
    std::string B = removeLeadingZeros(b);
    if (A.size() > B.size())
        return 1;
    if (A.size() < B.size())
        return -1;
    if (A == B)
        return 0;
    return (A > B) ? 1 : -1;
}

std::string subtractNonNegatives(const std::string &a, const std::string &b) {
    int i = static_cast<int>(a.size()) - 1;
    int j = static_cast<int>(b.size()) - 1;
    int borrow = 0;
    std::string result = "";
    while (i >= 0) {
        int x = a[i] - '0';
        int y = (j >= 0) ? b[j] - '0' : 0;
        int diff = x - y - borrow;
        if (diff < 0) {
            diff += 10;
            borrow = 1;
        } else {
            borrow = 0;
        }
        result.push_back('0' + diff);
        i--;
        j--;
    }
    return removeLeadingZeros(reverseString(result));
}

std::string addDoubleStrings(const std::string &s1, const std::string &s2) {
    ParsedNumber num1 = parseNumber(s1);
    ParsedNumber num2 = parseNumber(s2);
    int len1 = static_cast<int>(num1.fraction.size());
    int len2 = static_cast<int>(num2.fraction.size());
    int maxFrac = (len1 > len2) ? len1 : len2;
    while (static_cast<int>(num1.fraction.size()) < maxFrac)
        num1.fraction.push_back('0');
    while (static_cast<int>(num2.fraction.size()) < maxFrac)
        num2.fraction.push_back('0');
    std::string resultFraction, resultInteger;
    int finalSign = 1;
    if (num1.sign == num2.sign) {
        finalSign = num1.sign;
        int carry = 0;
        resultFraction = "";
        for (int i = maxFrac - 1; i >= 0; i--) {
            int d1 = num1.fraction[i] - '0';
            int d2 = num2.fraction[i] - '0';
            int sum = d1 + d2 + carry;
            carry = sum / 10;
            char digit = '0' + (sum % 10);
            resultFraction.insert(resultFraction.begin(), digit);
        }
        resultInteger = addNonNegatives(num1.integer, num2.integer);
        if (carry > 0)
            resultInteger = addNonNegatives(resultInteger, std::to_string(carry));
    } else {
        int cmp = compareNonNegatives(num1.integer, num2.integer);
        if (cmp == 0)
            cmp = compareNonNegatives(num1.fraction, num2.fraction);
        ParsedNumber larger, smaller;
        if (cmp >= 0) {
            larger = num1;
            smaller = num2;
            finalSign = num1.sign;
        } else {
            larger = num2;
            smaller = num1;
            finalSign = num2.sign;
        }
        int borrow = 0;
        resultFraction = "";
        for (int i = maxFrac - 1; i >= 0; i--) {
            int d1 = larger.fraction[i] - '0';
            int d2 = smaller.fraction[i] - '0';
            int diff = d1 - d2 - borrow;
            if (diff < 0) {
                diff += 10;
                borrow = 1;
            } else {
                borrow = 0;
            }
            char digit = '0' + diff;
            resultFraction.insert(resultFraction.begin(), digit);
        }
        resultInteger = subtractNonNegatives(larger.integer, smaller.integer);
        if (borrow > 0) {
            resultInteger = subtractNonNegatives(resultInteger, "1");
        }
    }
    std::string result = "";
    if (finalSign < 0 && (resultInteger != "0" || resultFraction.find_first_not_of('0') != std::string::npos))
        result.push_back('-');
    result += resultInteger;
    if (maxFrac > 0) {
        result.push_back('.');
        result += resultFraction;
    }
    return result;
}

#endif