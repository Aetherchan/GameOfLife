#ifndef PATTERN_H
#define PATTERN_H

#include <string>
#include <vector>
#include <utility>
#include <QRect>

/**
 * @file Pattern.h
 * @brief 图案数据结构定义
 * @author 3
 */

struct Pattern {
    std::string name;           ///< 图案名称
    std::string description;    ///< 图案描述
    std::string category;       ///< 分类
    std::vector<std::pair<int, int>> cells;  ///< 活细胞坐标列表
    std::string rule;           ///< 适用规则

    QRect boundingRect() const;

    Pattern rotated(int degrees) const;

    Pattern flipped(Qt::Orientation orientation) const;
};

#endif // PATTERN_H
