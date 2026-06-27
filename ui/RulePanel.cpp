/**
 * @file RulePanel.cpp
 * @brief 规则面板控件实现
 * @author 2
 */

#include "RulePanel.h"
#include "RuleEditDialog.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QGroupBox>

struct RulePreset {
    QString name;
    QString ruleString;
};

static const RulePreset s_presets[] = {
    {QT_TR_NOOP("康威生命游戏"), "B3/S23"},
    {QT_TR_NOOP("HighLife"), "B36/S23"},
    {QT_TR_NOOP("Day & Night"), "B3678/S34678"},
    {QT_TR_NOOP("Seeds"), "B2/S"},
    {QT_TR_NOOP("Replicator"), "B1357/S1357"},
};

RulePanel::RulePanel(QWidget* parent)
    : QWidget(parent)
    , m_presetCombo(new QComboBox(this))
    , m_editBtn(new QPushButton(tr("自定义规则"), this))
{
    setupPresets();
    
    QVBoxLayout* layout = new QVBoxLayout(this);
    
    QGroupBox* group = new QGroupBox(tr("规则设置"), this);
    QVBoxLayout* groupLayout = new QVBoxLayout(group);
    
    groupLayout->addWidget(new QLabel(tr("预设规则:"), this));
    groupLayout->addWidget(m_presetCombo);
    groupLayout->addWidget(m_editBtn);
    
    layout->addWidget(group);
    layout->addStretch();
    
    connect(m_presetCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &RulePanel::onPresetChanged);
    connect(m_editBtn, &QPushButton::clicked, this, &RulePanel::onEditRule);
}

void RulePanel::setupPresets() {
    for (const auto& preset : s_presets) {
        m_presetCombo->addItem(tr(preset.name.toUtf8().constData()));
    }
    m_currentRule = Rule::fromString(s_presets[0].ruleString.toStdString());
}

Rule RulePanel::currentRule() const {
    return m_currentRule;
}

void RulePanel::onPresetChanged(int index) {
    if (index >= 0 && index < 5) {
        m_currentRule = Rule::fromString(s_presets[index].ruleString.toStdString());
        emit ruleChanged(m_currentRule);
    }
}

void RulePanel::onEditRule() {
    RuleEditDialog dialog(this);
    dialog.setRule(m_currentRule);
    
    if (dialog.exec() == QDialog::Accepted) {
        m_currentRule = dialog.getRule();
        emit ruleChanged(m_currentRule);
    }
}
