/**
 * @file RuleEditDialog.h
 * @brief 规则编辑对话框
 * @author 1
 */

#ifndef RULEEDITDIALOG_H
#define RULEEDITDIALOG_H

#include <QDialog>
#include <QCheckBox>
#include <QLabel>
#include "Rule.h"

class RuleEditDialog : public QDialog {
    Q_OBJECT
    
public:
    explicit RuleEditDialog(QWidget* parent = nullptr);
    
    Rule getRule() const;           ///< 获取用户设置的规则
    void setRule(const Rule& rule); ///< 设置要编辑的规则
    
private slots:
    void updateRuleString(Qt::CheckState state = Qt::Unchecked); ///< 更新规则字符串显示
    
private:
    QCheckBox* m_birthChecks[9];   ///< 出生条件复选框
    QCheckBox* m_surviveChecks[9]; ///< 存活条件复选框
    QLabel* m_ruleLabel;           ///< 规则字符串标签
    Rule m_rule;                   ///< 当前规则对象
};

#endif // RULEEDITDIALOG_H
