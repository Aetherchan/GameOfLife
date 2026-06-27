/**
 * @file PatternSaveDialog.h
 * @brief 图案保存对话框
 * @author 1
 */

#ifndef PATTERNSAVEDIALOG_H
#define PATTERNSAVEDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QTextEdit>
#include <QComboBox>
#include <QLabel>
#include "Pattern.h"

class PatternSaveDialog : public QDialog {
    Q_OBJECT
    
public:
    explicit PatternSaveDialog(QWidget* parent = nullptr);
    
    void setPattern(const Pattern& pattern);          ///< 设置要编辑的图案
    void setPreviewCells(const std::vector<std::pair<int, int>>& cells); ///< 设置预览细胞坐标
    Pattern getPattern() const;                       ///< 获取用户设置的图案
    
private slots:
    void validateInput();                             ///< 验证输入有效性
    
private:
    void updatePreview();                             ///< 更新预览图像
    
    QLineEdit* m_nameEdit;                            ///< 图案名称输入框
    QTextEdit* m_descEdit;                            ///< 图案描述文本框
    QComboBox* m_categoryCombo;                       ///< 图案分类下拉框
    QLabel* m_previewLabel;                           ///< 预览图像标签
    QLabel* m_cellCountLabel;                         ///< 细胞数量标签
    Pattern m_pattern;                                ///< 当前图案数据
    std::vector<std::pair<int, int>> m_cells;         ///< 预览细胞坐标
};

#endif // PATTERNSAVEDIALOG_H
