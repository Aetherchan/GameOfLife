/**
 * @file RulePanel.h
 * @brief 规则设置面板控件
 * @author 2
 */

#ifndef RULEPANEL_H
#define RULEPANEL_H

#include <QWidget>
#include <QComboBox>
#include <QPushButton>
#include "Rule.h"

/**
 * @brief 规则设置面板，提供预设规则选择和自定义规则编辑
 */
class RulePanel : public QWidget {
    Q_OBJECT
    
public:
    explicit RulePanel(QWidget* parent = nullptr);
    
    /// @brief 获取当前选中的规则
    Rule currentRule() const;
    
signals:
    /// @brief 规则变化信号
    void ruleChanged(const Rule& rule);
    
private slots:
    /// @brief 响应预设规则下拉框选择变化
    void onPresetChanged(int index);
    
    /// @brief 响应自定义规则编辑按钮点击
    void onEditRule();
    
private:
    /// @brief 初始化预设规则列表
    void setupPresets();
    
    QComboBox* m_presetCombo;   ///< 预设规则下拉选择框
    QPushButton* m_editBtn;     ///< 自定义规则编辑按钮
    Rule m_currentRule;         ///< 当前选中的规则对象
};

#endif // RULEPANEL_H
