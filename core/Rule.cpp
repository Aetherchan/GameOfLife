/**
 * @file Rule.cpp
 * @brief B/S 规则类实现
 * @author 3
 */

#include "Rule.h"
#include <sstream>
#include <cctype>
#include <algorithm>

Rule::Rule() : m_neighborhood(NeighborhoodType::Moore) {
    m_birthCounts = {3};
    m_surviveCounts = {2, 3};
}

void Rule::setBirthRule(const std::set<int>& counts) {
    m_birthCounts = counts;
}

void Rule::setSurviveRule(const std::set<int>& counts) {
    m_surviveCounts = counts;
}

void Rule::setNeighborhoodType(NeighborhoodType type) {
    m_neighborhood = type;
}

bool Rule::shouldBirth(int neighborCount) const {
    return m_birthCounts.count(neighborCount) > 0;
}

bool Rule::shouldSurvive(int neighborCount) const {
    return m_surviveCounts.count(neighborCount) > 0;
}

std::string Rule::toString() const {
    std::string result = "B";
    for (int n : m_birthCounts) {
        result += std::to_string(n);
    }
    result += "/S";
    for (int n : m_surviveCounts) {
        result += std::to_string(n);
    }
    return result;
}

Rule Rule::fromString(const std::string& str) {
    Rule rule;
    size_t slashPos = str.find('/');
    if (slashPos == std::string::npos) return rule;
    
    std::string birthPart = str.substr(0, slashPos);
    std::string survivePart = str.substr(slashPos + 1);
    
    std::set<int> birthCounts;
    std::set<int> surviveCounts;
    
    for (char c : birthPart) {
        if (std::isdigit(c) && c != 'B') {
            birthCounts.insert(c - '0');
        }
    }
    for (char c : survivePart) {
        if (std::isdigit(c) && c != 'S') {
            surviveCounts.insert(c - '0');
        }
    }
    
    rule.setBirthRule(birthCounts);
    rule.setSurviveRule(surviveCounts);
    return rule;
}

Rule::NeighborhoodType Rule::neighborhoodType() const {
    return m_neighborhood;
}
