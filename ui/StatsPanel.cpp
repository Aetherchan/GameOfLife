/**
 * @file StatsPanel.cpp
 * @brief 统计信息面板实现
 * @author 2
 */

#include "StatsPanel.h"
#include <QVBoxLayout>
#include <QGroupBox>

StatsPanel::StatsPanel(QWidget* parent)
    : QWidget(parent)
    , m_genLabel(new QLabel(tr("代数: 0"), this))
    , m_aliveLabel(new QLabel(tr("活细胞: 0"), this))
    , m_timeLabel(new QLabel(tr("运行时间: 0秒"), this))
    , m_ruleLabel(new QLabel(tr("规则: B3/S23"), this))
{
    QVBoxLayout* layout = new QVBoxLayout(this);
    
    QGroupBox* group = new QGroupBox(tr("统计信息"), this);
    QVBoxLayout* groupLayout = new QVBoxLayout(group);
    
    groupLayout->addWidget(m_genLabel);
    groupLayout->addWidget(m_aliveLabel);
    groupLayout->addWidget(m_timeLabel);
    groupLayout->addWidget(m_ruleLabel);
    
    layout->addWidget(group);
    layout->addStretch();
}

void StatsPanel::setGeneration(int gen) {
    m_genLabel->setText(tr("代数: %1").arg(gen));
}

void StatsPanel::setAliveCount(int count) {
    m_aliveLabel->setText(tr("活细胞: %1").arg(count));
}

void StatsPanel::setElapsedTime(int seconds) {
    int minutes = seconds / 60;
    int secs = seconds % 60;
    
    if (minutes > 0) {
        m_timeLabel->setText(tr("运行时间: %1分%2秒").arg(minutes).arg(secs));
    } else {
        m_timeLabel->setText(tr("运行时间: %1秒").arg(secs));
    }
}

void StatsPanel::setRule(const QString& rule) {
    m_ruleLabel->setText(tr("规则: %1").arg(rule));
}
