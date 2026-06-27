/**
 * @file SideBar.h
 * @brief 侧边栏容器控件
 * @author 2
 */

#ifndef SIDEBAR_H
#define SIDEBAR_H

#include <QWidget>
#include "RulePanel.h"
#include "StatsPanel.h"
#include "PatternPanel.h"

/**
 * @brief 侧边栏容器，容纳图案面板、规则面板和统计面板
 */
class SideBar : public QWidget {
    Q_OBJECT
    
public:
    explicit SideBar(QWidget* parent = nullptr);
    
    /// @brief 获取规则面板
    RulePanel* rulePanel() const;
    
    /// @brief 获取统计面板
    StatsPanel* statsPanel() const;
    
    /// @brief 获取图案面板
    PatternPanel* patternPanel() const;
    
private:
    RulePanel* m_rulePanel;          ///< 规则面板
    StatsPanel* m_statsPanel;        ///< 统计面板
    PatternPanel* m_patternPanel;    ///< 图案面板
};

#endif // SIDEBAR_H
