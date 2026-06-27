/**
 * @file GridView.h
 * @brief 生命游戏网格视图控件
 * @author 2
 */

#ifndef GRIDVIEW_H
#define GRIDVIEW_H

#include <QWidget>
#include "Grid.h"
#include "Pattern.h"

/**
 * @brief 生命游戏网格视图控件，负责细胞渲染和用户交互
 */
class GridView : public QWidget {
    Q_OBJECT
    
public:
    enum class InteractionMode { 
        Draw,   ///< 绘制模式，点击绘制/擦除细胞
        Pan,    ///< 平移模式，拖拽移动视图
        Select  ///< 选择模式，拖拽框选区域
    };
    
    explicit GridView(QWidget* parent = nullptr);
    
    /// @brief 设置关联的网格数据
    void setGrid(Grid* grid);
    
    /// @brief 设置单个细胞的显示尺寸（像素）
    void setCellSize(int size);
    
    /// @brief 获取当前细胞尺寸
    int cellSize() const;
    
    /// @brief 设置是否显示网格线
    void setShowGrid(bool show);
    
    /// @brief 获取网格线显示状态
    bool showGrid() const;
    
    /// @brief 切换当前的交互模式
    void setInteractionMode(InteractionMode mode);
    
    /// @brief 获取当前交互模式
    InteractionMode interactionMode() const;
    
    /// @brief 设置待放置的图案用于预览
    void setPattern(const Pattern& pattern);
    
    /// @brief 清除待放置的图案
    void clearPattern();
    
    /// @brief 在指定位置放置当前图案
    void placePattern(int x, int y);
    
    /// @brief 获取当前选择的矩形区域（网格坐标）
    QRect getSelection() const;
    
    /// @brief 清除当前选择
    void clearSelection();
    
    /// @brief 获取选择区域内的活细胞相对坐标列表
    std::vector<std::pair<int, int>> getSelectionCells() const;
    
    /// @brief 检查是否有有效选择
    bool hasSelection() const;
    
    /// @brief 填充选择区域为指定状态
    void fillSelection(bool alive);
    
    /// @brief 将选择区域转换为图案对象
    Pattern getSelectionAsPattern() const;
    
signals:
    /// @brief 细胞被绘制时发出
    void cellClicked(int x, int y, bool alive);
    
    /// @brief 图案被放置时发出
    void patternPlaced();
    
    /// @brief 选择区域改变时发出
    void selectionChanged(const QRect& selection);
    
    /// @brief 请求保存图案时发出
    void savePatternRequested();
    
    /// @brief 网格数据更新时发出
    void gridUpdated();
    
protected:
    void paintEvent(QPaintEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void wheelEvent(QWheelEvent* event) override;
    void keyPressEvent(QKeyEvent* event) override;
    void contextMenuEvent(QContextMenuEvent* event) override;
    
private:
    void drawGrid(QPainter& painter);
    void drawCells(QPainter& painter);
    void drawPatternPreview(QPainter& painter);
    void drawSelection(QPainter& painter);
    
    /// @brief 将屏幕坐标转换为网格世界坐标
    QPointF screenToGrid(const QPointF& screenPos) const;
    
    /// @brief 将屏幕坐标转换为细胞行列索引
    QPoint gridToCell(const QPointF& gridPos) const;
    
    void updateTransform();
    
    Grid* m_grid;                   ///< 关联的网格数据指针
    int m_cellSize;                 ///< 细胞显示尺寸（像素）
    bool m_showGrid;                ///< 是否显示网格线
    bool m_drawing;                 ///< 是否正在绘制中
    bool m_drawState;               ///< 当前绘制状态（true=活，false=死）
    
    InteractionMode m_interactionMode;  ///< 当前交互模式
    QPointF m_offset;               ///< 视图偏移量
    double m_scale;                 ///< 视图缩放比例
    bool m_panning;                 ///< 是否正在平移中
    QPoint m_lastPanPos;            ///< 上次平移鼠标位置
    
    Pattern m_currentPattern;       ///< 当前待放置的图案
    bool m_hasPattern;              ///< 是否有待放置的图案
    QPoint m_patternPos;            ///< 图案预览位置（网格坐标）
    
    bool m_selecting;               ///< 是否正在框选中
    QPoint m_selectionStart;        ///< 框选起始点（网格坐标）
    QPoint m_selectionEnd;          ///< 框选终点（网格坐标）
    QRect m_selection;              ///< 已确定的选择区域
    
    Pattern m_clipboard;            ///< 剪贴板中的图案
    bool m_hasClipboard;            ///< 剪贴板是否有内容
};

#endif // GRIDVIEW_H
