/**
 * @file Grid.cpp
 * @brief 网格类实现
 * @author 3
 */

#include "Grid.h"

Grid::Grid(int width, int height)
    : m_mode(Mode::Fixed)
    , m_boundary(BoundaryType::Dead)
    , m_width(width)
    , m_height(height)
{
    m_fixedGrid.resize(m_height, std::vector<Cell>(m_width));
}

void Grid::setMode(Mode mode) {
    m_mode = mode;
    if (mode == Mode::Infinite) {
        m_infiniteGrid.clear();
        for (int y = 0; y < m_height; ++y) {
            for (int x = 0; x < m_width; ++x) {
                if (m_fixedGrid[y][x].isAlive()) {
                    m_infiniteGrid.insert({x, y});
                }
            }
        }
    }
}

void Grid::setBoundaryType(BoundaryType type) {
    m_boundary = type;
}

void Grid::resize(int width, int height) {
    m_width = width;
    m_height = height;
    m_fixedGrid.clear();
    m_fixedGrid.resize(m_height, std::vector<Cell>(m_width));
}

Cell& Grid::at(int x, int y) {
    static Cell deadCell;
    
    if (m_mode == Mode::Infinite) {
        if (m_infiniteGrid.count({x, y})) {
            static Cell aliveCell;
            aliveCell.birth();
            return aliveCell;
        }
        return deadCell;
    }
    
    if (x < 0 || x >= m_width || y < 0 || y >= m_height) {
        if (m_boundary == BoundaryType::Toroidal) {
            x = wrapCoord(x, m_width);
            y = wrapCoord(y, m_height);
        } else {
            return deadCell;
        }
    }
    
    return m_fixedGrid[y][x];
}

const Cell& Grid::at(int x, int y) const {
    static const Cell deadCell;
    
    if (m_mode == Mode::Infinite) {
        if (m_infiniteGrid.count({x, y})) {
            static Cell aliveCell;
            aliveCell.birth();
            return aliveCell;
        }
        return deadCell;
    }
    
    if (x < 0 || x >= m_width || y < 0 || y >= m_height) {
        if (m_boundary == BoundaryType::Toroidal) {
            x = wrapCoord(x, m_width);
            y = wrapCoord(y, m_height);
        } else {
            return deadCell;
        }
    }
    
    return m_fixedGrid[y][x];
}

int Grid::width() const {
    return m_width;
}

int Grid::height() const {
    return m_height;
}

int Grid::getAliveCount() const {
    if (m_mode == Mode::Infinite) {
        return static_cast<int>(m_infiniteGrid.size());
    }
    
    int count = 0;
    for (const auto& row : m_fixedGrid) {
        for (const auto& cell : row) {
            if (cell.isAlive()) count++;
        }
    }
    return count;
}

void Grid::clear() {
    if (m_mode == Mode::Infinite) {
        m_infiniteGrid.clear();
    } else {
        for (auto& row : m_fixedGrid) {
            for (auto& cell : row) {
                cell.kill();
            }
        }
    }
}

void Grid::setCell(int x, int y, bool alive) {
    if (m_mode == Mode::Infinite) {
        if (alive) {
            m_infiniteGrid.insert({x, y});
        } else {
            m_infiniteGrid.erase({x, y});
        }
        return;
    }
    
    if (x >= 0 && x < m_width && y >= 0 && y < m_height) {
        if (alive) {
            m_fixedGrid[y][x].birth();
        } else {
            m_fixedGrid[y][x].kill();
        }
    }
}

bool Grid::isAlive(int x, int y) const {
    if (m_mode == Mode::Infinite) {
        return m_infiniteGrid.count({x, y}) > 0;
    }
    
    if (x < 0 || x >= m_width || y < 0 || y >= m_height) {
        if (m_boundary == BoundaryType::Toroidal) {
            x = wrapCoord(x, m_width);
            y = wrapCoord(y, m_height);
        } else {
            return false;
        }
    }
    
    return m_fixedGrid[y][x].isAlive();
}

void Grid::tickCell(int x, int y) {
    if (m_mode == Mode::Infinite) return;
    
    if (x >= 0 && x < m_width && y >= 0 && y < m_height) {
        m_fixedGrid[y][x].tick();
    }
}

Grid::Mode Grid::mode() const {
    return m_mode;
}

Grid::BoundaryType Grid::boundaryType() const {
    return m_boundary;
}

const std::unordered_set<std::pair<int, int>, PairHash>& Grid::getAliveCells() const {
    return m_infiniteGrid;
}

int Grid::wrapCoord(int value, int max) const {
    if (max == 0) return 0;
    return ((value % max) + max) % max;
}
