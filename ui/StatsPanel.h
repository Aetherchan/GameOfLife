/**
 * @file StatsPanel.h
 * @brief 统计信息面板控件
 * @author 2
 */

#ifndef STATSPANEL_H
#define STATSPANEL_H

#include <QWidget>
#include <QLabel>

/**
 * @brief 统计信息面板，显示代数、活细胞数量、运行时间和当前规则
 */
class StatsPanel : public QWidget {
    Q_OBJECT
    
public:
    explicit StatsPanel(QWidget* parent = nullptr);
    
public slots:
    /// @brief 设置当前代数显示
    void setGeneration(int gen);
    
    /// @brief 设置活细胞数量显示
    void setAliveCount(int count);
    
    /// @brief 设置运行时间显示（秒）
    void setElapsedTime(int seconds);
    
    /// @brief 设置当前规则显示
    void setRule(const QString& rule);
    
private:
    QLabel* m_genLabel;     ///< 代数标签
    QLabel* m_aliveLabel;   ///< 活细胞数量标签
    QLabel* m_timeLabel;    ///< 运行时间标签
    QLabel* m_ruleLabel;    ///< 当前规则标签
};

#endif // STATSPANEL_H
