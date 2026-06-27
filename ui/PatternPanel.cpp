/**
 * @file PatternPanel.cpp
 * @brief 图案面板控件实现
 * @author 2
 */

#include "PatternPanel.h"
#include "PatternSerializer.h"
#include <QVBoxLayout>
#include <QGroupBox>
#include <QLabel>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QStandardPaths>
#include <QDir>

PatternPanel::PatternPanel(QWidget* parent)
    : QWidget(parent)
    , m_patternList(new QListWidget(this))
    , m_rotateBtn(new QPushButton(tr("旋转90°"), this))
    , m_flipBtn(new QPushButton(tr("翻转"), this))
    , m_deleteBtn(new QPushButton(tr("删除"), this))
    , m_selectedIndex(-1)
{
    m_userPatternsPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/user_patterns.json";
    
    QVBoxLayout* layout = new QVBoxLayout(this);
    
    QGroupBox* group = new QGroupBox(tr("图案库"), this);
    QVBoxLayout* groupLayout = new QVBoxLayout(group);
    
    groupLayout->addWidget(m_patternList);
    
    QHBoxLayout* btnLayout = new QHBoxLayout;
    btnLayout->addWidget(m_rotateBtn);
    btnLayout->addWidget(m_flipBtn);
    btnLayout->addWidget(m_deleteBtn);
    groupLayout->addLayout(btnLayout);
    
    layout->addWidget(group);
    layout->addStretch();
    
    m_deleteBtn->setEnabled(false);
    
    connect(m_patternList, &QListWidget::itemClicked, this, &PatternPanel::onPatternClicked);
    connect(m_rotateBtn, &QPushButton::clicked, this, &PatternPanel::onRotatePattern);
    connect(m_flipBtn, &QPushButton::clicked, this, &PatternPanel::onFlipPattern);
    connect(m_deleteBtn, &QPushButton::clicked, this, &PatternPanel::onDeletePattern);
}

bool PatternPanel::loadPatterns(const QString& filePath) {
    m_patterns.clear();
    m_patternList->clear();
    
    if (PatternSerializer::loadPatternLibrary(filePath, m_patterns)) {
        for (const auto& pattern : m_patterns) {
            m_patternList->addItem(QString::fromStdString(pattern.name));
        }
        return true;
    }
    return false;
}

void PatternPanel::addPattern(const Pattern& pattern) {
    m_patterns.push_back(pattern);
    m_patternList->addItem(QString::fromStdString(pattern.name));
}

void PatternPanel::removePattern(int index) {
    if (index >= 0 && index < static_cast<int>(m_patterns.size())) {
        m_patterns.erase(m_patterns.begin() + index);
        delete m_patternList->takeItem(index);
        m_selectedIndex = -1;
        m_deleteBtn->setEnabled(false);
    }
}

int PatternPanel::selectedIndex() const {
    return m_selectedIndex;
}

void PatternPanel::onPatternClicked(QListWidgetItem* item) {
    int index = m_patternList->row(item);
    
    if (index >= 0 && index < static_cast<int>(m_patterns.size())) {
        if (m_selectedIndex == index) {
            m_patternList->clearSelection();
            m_selectedIndex = -1;
            m_deleteBtn->setEnabled(false);
            emit patternDeselected();
        } else {
            m_selectedIndex = index;
            m_currentPattern = m_patterns[index];
            m_deleteBtn->setEnabled(true);
            emit patternSelected(m_currentPattern);
        }
    }
}

void PatternPanel::onRotatePattern() {
    if (m_selectedIndex >= 0) {
        m_currentPattern = m_currentPattern.rotated(90);
        emit patternSelected(m_currentPattern);
    }
}

void PatternPanel::onFlipPattern() {
    if (m_selectedIndex >= 0) {
        m_currentPattern = m_currentPattern.flipped(Qt::Horizontal);
        emit patternSelected(m_currentPattern);
    }
}

void PatternPanel::onDeletePattern() {
    if (m_selectedIndex < 0) return;
    
    QString patternName = QString::fromStdString(m_patterns[m_selectedIndex].name);
    
    QMessageBox::StandardButton reply = QMessageBox::question(this, tr("删除图案"),
        tr("确定要删除图案 '%1' 吗？").arg(patternName),
        QMessageBox::Yes | QMessageBox::No);
    
    if (reply == QMessageBox::Yes) {
        removePattern(m_selectedIndex);
        emit patternDeselected();
        PatternSerializer::savePatternLibrary(m_userPatternsPath, m_patterns);
    }
}
