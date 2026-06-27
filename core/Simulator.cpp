/**
 * @file Simulator.cpp
 * @brief 演化引擎类实现
 * @author 3
 */

#include "Simulator.h"

Simulator::Simulator(QObject* parent)
    : QObject(parent)
    , m_grid(nullptr)
    , m_timer(new QTimer(this))
    , m_generation(0)
{
    connect(m_timer, &QTimer::timeout, this, &Simulator::onTimerTick);
}

void Simulator::setGrid(Grid* grid) {
    m_grid = grid;
}

void Simulator::setRule(const Rule& rule) {
    m_rule = rule;
}

void Simulator::step() {
    if (m_grid) {
        computeNextGeneration();
        m_generation++;
        emit generationChanged(m_generation);
        emit statsUpdated(m_grid->getAliveCount());
        emit stepped();
    }
}

void Simulator::start(int intervalMs) {
    m_timer->start(intervalMs);
}

void Simulator::stop() {
    m_timer->stop();
}

void Simulator::reset() {
    m_generation = 0;
    emit generationChanged(m_generation);
    if (m_grid) {
        emit statsUpdated(m_grid->getAliveCount());
    }
}

int Simulator::generation() const {
    return m_generation;
}

bool Simulator::isRunning() const {
    return m_timer->isActive();
}

int Simulator::interval() const {
    return m_timer->interval();
}

void Simulator::setInterval(int ms) {
    m_timer->setInterval(ms);
}

Rule Simulator::currentRule() const {
    return m_rule;
}

void Simulator::onTimerTick() {
    step();
}

void Simulator::computeNextGeneration() {
    if (!m_grid) return;
    
    std::vector<std::pair<int, int>> toBirth;
    std::vector<std::pair<int, int>> toKill;
    
    if (m_grid->mode() == Grid::Mode::Infinite) {
        std::unordered_set<std::pair<int, int>, PairHash> checked;
        
        for (const auto& cell : m_grid->getAliveCells()) {
            int x = cell.first;
            int y = cell.second;
            
            for (int dy = -1; dy <= 1; ++dy) {
                for (int dx = -1; dx <= 1; ++dx) {
                    int nx = x + dx;
                    int ny = y + dy;
                    
                    if (checked.count({nx, ny})) continue;
                    checked.insert({nx, ny});
                    
                    int neighbors = countNeighbors(nx, ny);
                    bool alive = m_grid->isAlive(nx, ny);
                    
                    if (alive) {
                        if (!m_rule.shouldSurvive(neighbors)) {
                            toKill.push_back({nx, ny});
                        }
                    } else {
                        if (m_rule.shouldBirth(neighbors)) {
                            toBirth.push_back({nx, ny});
                        }
                    }
                }
            }
        }
    } else {
        int w = m_grid->width();
        int h = m_grid->height();
        
        for (int y = 0; y < h; ++y) {
            for (int x = 0; x < w; ++x) {
                int neighbors = countNeighbors(x, y);
                bool alive = m_grid->isAlive(x, y);
                
                if (alive) {
                    if (!m_rule.shouldSurvive(neighbors)) {
                        toKill.push_back({x, y});
                    }
                } else {
                    if (m_rule.shouldBirth(neighbors)) {
                        toBirth.push_back({x, y});
                    }
                }
            }
        }
    }
    
    for (const auto& pos : toBirth) {
        m_grid->setCell(pos.first, pos.second, true);
    }
    for (const auto& pos : toKill) {
        m_grid->setCell(pos.first, pos.second, false);
    }
    
    if (m_grid->mode() != Grid::Mode::Infinite) {
        int w = m_grid->width();
        int h = m_grid->height();
        for (int y = 0; y < h; ++y) {
            for (int x = 0; x < w; ++x) {
                if (m_grid->isAlive(x, y)) {
                    m_grid->tickCell(x, y);
                }
            }
        }
    }
}

int Simulator::countNeighbors(int x, int y) {
    int count = 0;
    
    if (m_rule.neighborhoodType() == Rule::NeighborhoodType::Moore) {
        for (int dy = -1; dy <= 1; ++dy) {
            for (int dx = -1; dx <= 1; ++dx) {
                if (dx == 0 && dy == 0) continue;
                if (m_grid->isAlive(x + dx, y + dy)) {
                    count++;
                }
            }
        }
    } else {
        static const int dx[] = {0, 0, -1, 1};
        static const int dy[] = {-1, 1, 0, 0};
        for (int i = 0; i < 4; ++i) {
            if (m_grid->isAlive(x + dx[i], y + dy[i])) {
                count++;
            }
        }
    }
    
    return count;
}
