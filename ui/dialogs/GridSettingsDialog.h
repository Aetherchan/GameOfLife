/**
 * @file GridSettingsDialog.h
 * @brief 网格设置对话框
 * @author 1
 */

#ifndef GRIDSETTINGSDIALOG_H
#define GRIDSETTINGSDIALOG_H

#include <QDialog>
#include <QSpinBox>
#include <QComboBox>
#include "Grid.h"

class GridSettingsDialog : public QDialog {
    Q_OBJECT
    
public:
    explicit GridSettingsDialog(QWidget* parent = nullptr);
    
    int gridWidth() const;                          ///< 获取网格宽度
    int gridHeight() const;                         ///< 获取网格高度
    Grid::BoundaryType boundaryType() const;        ///< 获取边界类型
    Grid::Mode gridMode() const;                    ///< 获取网格模式
    
    void setGridWidth(int width);                   ///< 设置网格宽度
    void setGridHeight(int height);                 ///< 设置网格高度
    void setBoundaryType(Grid::BoundaryType type);  ///< 设置边界类型
    void setGridMode(Grid::Mode mode);              ///< 设置网格模式
    
private:
    QSpinBox* m_widthSpin;      ///< 宽度输入框
    QSpinBox* m_heightSpin;     ///< 高度输入框
    QComboBox* m_boundaryCombo; ///< 边界类型下拉框
    QComboBox* m_modeCombo;     ///< 网格模式下拉框
};

#endif // GRIDSETTINGSDIALOG_H
