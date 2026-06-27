/**
 * @file Pattern.cpp
 * @brief 图案数据结构实现
 * @author 3
 */

#include "Pattern.h"
#include <algorithm>

QRect Pattern::boundingRect() const {
    if (cells.empty()) return QRect();
    
    int minX = cells[0].first;
    int maxX = cells[0].first;
    int minY = cells[0].second;
    int maxY = cells[0].second;
    
    for (const auto& cell : cells) {
        minX = std::min(minX, cell.first);
        maxX = std::max(maxX, cell.first);
        minY = std::min(minY, cell.second);
        maxY = std::max(maxY, cell.second);
    }
    
    return QRect(minX, minY, maxX - minX + 1, maxY - minY + 1);
}

Pattern Pattern::rotated(int degrees) const {
    Pattern result;
    result.name = name;
    result.description = description;
    result.category = category;
    result.rule = rule;
    
    int rotations = (degrees / 90) % 4;
    
    for (const auto& cell : cells) {
        int x = cell.first;
        int y = cell.second;
        
        for (int i = 0; i < rotations; ++i) {
            int temp = x;
            x = -y;
            y = temp;
        }
        
        result.cells.push_back({x, y});
    }
    
    return result;
}

Pattern Pattern::flipped(Qt::Orientation orientation) const {
    Pattern result;
    result.name = name;
    result.description = description;
    result.category = category;
    result.rule = rule;
    
    for (const auto& cell : cells) {
        int x = cell.first;
        int y = cell.second;
        
        if (orientation == Qt::Horizontal) {
            x = -x;
        } else {
            y = -y;
        }
        
        result.cells.push_back({x, y});
    }
    
    return result;
}
