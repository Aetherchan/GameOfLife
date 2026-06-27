/**
 * @file mainwindow.h
 * @brief 应用程序主窗口
 * @author 1
 */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QElapsedTimer>
#include "Grid.h"
#include "Simulator.h"
#include "GridView.h"
#include "SideBar.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

private slots:
    void onStartStop();                  ///< 开始/暂停演化
    void onStep();                       ///< 单步演化
    void onClear();                      ///< 清空网格
    void onSpeedChanged(int value);      ///< 速度滑块改变
    void onGridSettings();               ///< 打开网格设置对话框
    void onRuleChanged(const Rule& rule);///< 规则改变
    void onGenerationChanged(int gen);   ///< 代数改变
    void onStatsUpdated(int aliveCount); ///< 统计更新
    void updateElapsedTime();            ///< 更新运行时间
    void setDrawMode();                  ///< 切换到绘制模式
    void setPanMode();                   ///< 切换到拖动模式
    void setSelectMode();                ///< 切换到选择模式
    void savePattern();                  ///< 保存选区为图案
    void onSelectionChanged(const QRect& selection); ///< 选区改变

private:
    void setupUI();           ///< 初始化 UI 布局
    void createMenus();       ///< 创建菜单栏
    void createConnections(); ///< 建立信号槽连接
    void updateStats();       ///< 更新统计信息
    void updateModeButtons(); ///< 更新模式按钮状态
    
    Ui::MainWindow* ui;             ///< UI 表单
    Grid* m_grid;                   ///< 网格数据
    Simulator* m_simulator;         ///< 演化引擎
    GridView* m_gridView;           ///< 网格视图控件
    SideBar* m_sideBar;             ///< 侧边栏
    QWidget* m_centralWidget;       ///< 中央部件
    
    QAction* m_startAction;         ///< 开始/暂停按钮
    QAction* m_stepAction;          ///< 单步按钮
    QAction* m_clearAction;         ///< 清空按钮
    QAction* m_showGridAction;      ///< 显示网格线选项
    QAction* m_showSidebarAction;   ///< 显示侧边栏选项
    QAction* m_drawModeAction;      ///< 绘制模式按钮
    QAction* m_panModeAction;       ///< 拖动模式按钮
    QAction* m_selectModeAction;    ///< 选择模式按钮
    QAction* m_savePatternAction;   ///< 保存图案按钮
    
    QElapsedTimer m_elapsedTimer;   ///< 运行时间计时器
    bool m_timerRunning;            ///< 计时器是否运行中
};

#endif // MAINWINDOW_H
