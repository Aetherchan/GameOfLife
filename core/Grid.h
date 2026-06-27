#ifndef GRID_H
#define GRID_H

#include "Cell.h"
#include <vector>
#include <unordered_set>
#include <utility>

/**
 * @file Grid.h
 * @brief 网格类定义
 * @author 3
 */

struct PairHash {
    std::size_t operator()(const std::pair<int, int>& p) const {
        return std::hash<int>()(p.first) ^ (std::hash<int>()(p.second) << 1);
    }
};

class Grid {
public:
    enum class BoundaryType { 
        Dead,     ///< 死边界
        Toroidal  ///< 环形边界
    };
    
    enum class Mode { 
        Fixed,    ///< 固定网格
        Infinite  ///< 无限网格
    };
    
    Grid(int width = 100, int height = 100);

    void setMode(Mode mode);

    void setBoundaryType(BoundaryType type);

    void resize(int width, int height);

    Cell& at(int x, int y);

    const Cell& at(int x, int y) const;

    int width() const;

    int height() const;

    int getAliveCount() const;

    void clear();

    void setCell(int x, int y, bool alive);

    bool isAlive(int x, int y) const;

    void tickCell(int x, int y);

    Mode mode() const;

    BoundaryType boundaryType() const;

    const std::unordered_set<std::pair<int, int>, PairHash>& getAliveCells() const;
    
private:
    Mode m_mode;                    ///< 当前网格模式
    BoundaryType m_boundary;        ///< 边界类型
    
    std::vector<std::vector<Cell>> m_fixedGrid;  ///< 固定网格数据
    int m_width;                    ///< 固定网格宽度
    int m_height;                   ///< 固定网格高度
    
    std::unordered_set<std::pair<int, int>, PairHash> m_infiniteGrid;  ///< 无限网格活细胞坐标
    
    int wrapCoord(int value, int max) const;
};

#endif // GRID_H
