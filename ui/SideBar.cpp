/**
 * @file SideBar.cpp
 * @brief 侧边栏容器控件实现
 * @author 2
 */

#include "SideBar.h"
#include <QVBoxLayout>
#include <QScrollArea>

SideBar::SideBar(QWidget* parent)
    : QWidget(parent)
    , m_rulePanel(new RulePanel(this))
    , m_statsPanel(new StatsPanel(this))
    , m_patternPanel(new PatternPanel(this))
{
    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->setContentsMargins(5, 5, 5, 5);
    
    layout->addWidget(m_patternPanel);
    layout->addWidget(m_rulePanel);
    layout->addWidget(m_statsPanel);
    
    setMinimumWidth(200);
    setMaximumWidth(300);
}

RulePanel* SideBar::rulePanel() const {
    return m_rulePanel;
}

StatsPanel* SideBar::statsPanel() const {
    return m_statsPanel;
}

PatternPanel* SideBar::patternPanel() const {
    return m_patternPanel;
}
