#ifndef RULE_H
#define RULE_H

#include <set>
#include <string>

/**
 * @file Rule.h
 * @brief B/S 规则类定义
 * @author 3
 */

class Rule {
public:
    enum class NeighborhoodType { 
        Moore,       ///< 摩尔邻域：周围8个格子
        VonNeumann   ///< 冯诺依曼邻域：周围4个格子
    };
    
    Rule();

    void setBirthRule(const std::set<int>& counts);

    void setSurviveRule(const std::set<int>& counts);

    void setNeighborhoodType(NeighborhoodType type);

    bool shouldBirth(int neighborCount) const;

    bool shouldSurvive(int neighborCount) const;

    std::string toString() const;

    static Rule fromString(const std::string& str);

    NeighborhoodType neighborhoodType() const;
    
private:
    std::set<int> m_birthCounts;    ///< 出生条件：邻居数量集合
    std::set<int> m_surviveCounts;  ///< 存活条件：邻居数量集合
    NeighborhoodType m_neighborhood; ///< 邻居类型
};

#endif // RULE_H
