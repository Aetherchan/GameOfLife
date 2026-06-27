#ifndef SIMULATOR_H
#define SIMULATOR_H

#include <QObject>
#include <QTimer>
#include "Grid.h"
#include "Rule.h"

/**
 * @file Simulator.h
 * @brief 演化引擎类定义
 * @author 3
 */

class Simulator : public QObject {
    Q_OBJECT
    
public:
    explicit Simulator(QObject* parent = nullptr);

    void setGrid(Grid* grid);

    void setRule(const Rule& rule);

    void step();

    void start(int intervalMs = 100);

    void stop();

    void reset();

    int generation() const;

    bool isRunning() const;

    int interval() const;

    void setInterval(int ms);

    Rule currentRule() const;
    
signals:
    void generationChanged(int gen);

    void statsUpdated(int aliveCount);

    void stepped();
    
private slots:
    void onTimerTick();
    
private:
    void computeNextGeneration();

    int countNeighbors(int x, int y);
    
    Grid* m_grid;           ///< 网格指针
    Rule m_rule;            ///< 演化规则
    QTimer* m_timer;        ///< 定时器
    int m_generation;       ///< 当前代数
};

#endif // SIMULATOR_H
