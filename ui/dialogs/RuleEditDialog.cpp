/**
 * @file RuleEditDialog.cpp
 * @brief 规则编辑对话框实现
 * @author 1
 */

#include "RuleEditDialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QGroupBox>
#include <QDialogButtonBox>

RuleEditDialog::RuleEditDialog(QWidget* parent)
    : QDialog(parent)
    , m_ruleLabel(new QLabel(tr("规则: B3/S23"), this))
{
    setWindowTitle(tr("自定义规则"));
    resize(400, 300);
    
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    
    QGroupBox* birthGroup = new QGroupBox(tr("出生条件 (B)"), this);
    QHBoxLayout* birthLayout = new QHBoxLayout(birthGroup);
    
    for (int i = 0; i < 9; ++i) {
        m_birthChecks[i] = new QCheckBox(QString::number(i), this);
        birthLayout->addWidget(m_birthChecks[i]);
        connect(m_birthChecks[i], &QCheckBox::checkStateChanged, this, &RuleEditDialog::updateRuleString);
    }
    
    QGroupBox* surviveGroup = new QGroupBox(tr("存活条件 (S)"), this);
    QHBoxLayout* surviveLayout = new QHBoxLayout(surviveGroup);
    
    for (int i = 0; i < 9; ++i) {
        m_surviveChecks[i] = new QCheckBox(QString::number(i), this);
        surviveLayout->addWidget(m_surviveChecks[i]);
        connect(m_surviveChecks[i], &QCheckBox::checkStateChanged, this, &RuleEditDialog::updateRuleString);
    }
    
    m_birthChecks[3]->setChecked(true);
    m_surviveChecks[2]->setChecked(true);
    m_surviveChecks[3]->setChecked(true);
    
    mainLayout->addWidget(birthGroup);
    mainLayout->addWidget(surviveGroup);
    mainLayout->addWidget(m_ruleLabel);
    
    QDialogButtonBox* buttons = new QDialogButtonBox(
        QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    
    connect(buttons, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(buttons, &QDialogButtonBox::rejected, this, &QDialog::reject);
    
    mainLayout->addWidget(buttons);
    
    updateRuleString();
}

Rule RuleEditDialog::getRule() const {
    return m_rule;
}

void RuleEditDialog::setRule(const Rule& rule) {
    m_rule = rule;
    
    for (int i = 0; i < 9; ++i) {
        m_birthChecks[i]->setChecked(false);
        m_surviveChecks[i]->setChecked(false);
    }
    
    Rule testRule;
    std::set<int> birthCounts;
    std::set<int> surviveCounts;
    
    for (int i = 0; i < 9; ++i) {
        if (rule.shouldBirth(i)) {
            birthCounts.insert(i);
            m_birthChecks[i]->setChecked(true);
        }
        if (rule.shouldSurvive(i)) {
            surviveCounts.insert(i);
            m_surviveChecks[i]->setChecked(true);
        }
    }
    
    updateRuleString();
}

void RuleEditDialog::updateRuleString(Qt::CheckState) {
    std::set<int> birthCounts;
    std::set<int> surviveCounts;
    
    for (int i = 0; i < 9; ++i) {
        if (m_birthChecks[i]->isChecked()) {
            birthCounts.insert(i);
        }
        if (m_surviveChecks[i]->isChecked()) {
            surviveCounts.insert(i);
        }
    }
    
    m_rule.setBirthRule(birthCounts);
    m_rule.setSurviveRule(surviveCounts);
    
    m_ruleLabel->setText(QString::fromStdString(m_rule.toString()));
}
