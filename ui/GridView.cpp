/**
 * @file GridView.cpp
 * @brief 生命游戏网格视图控件实现
 * @author 2
 */

#include "GridView.h"
#include <QPainter>
#include <QMouseEvent>
#include <QPen>
#include <QBrush>
#include <QWheelEvent>
#include <QKeyEvent>
#include <QContextMenuEvent>
#include <QMenu>
#include <QAction>

GridView::GridView(QWidget* parent)
    : QWidget(parent)
    , m_grid(nullptr)
    , m_cellSize(10)
    , m_showGrid(true)
    , m_drawing(false)
    , m_drawState(true)
    , m_interactionMode(InteractionMode::Draw)
    , m_offset(0, 0)
    , m_scale(1.0)
    , m_panning(false)
    , m_hasPattern(false)
    , m_selecting(false)
    , m_hasClipboard(false)
{
    setMinimumSize(400, 400);
    setMouseTracking(true);
    setFocusPolicy(Qt::StrongFocus);
}

void GridView::setGrid(Grid* grid) {
    m_grid = grid;
    update();
}

void GridView::setCellSize(int size) {
    m_cellSize = qMax(1, size);
    update();
}

int GridView::cellSize() const {
    return m_cellSize;
}

void GridView::setShowGrid(bool show) {
    m_showGrid = show;
    update();
}

bool GridView::showGrid() const {
    return m_showGrid;
}

void GridView::setInteractionMode(InteractionMode mode) {
    if (m_interactionMode == InteractionMode::Select && mode != InteractionMode::Select) {
        clearSelection();
    }
    m_interactionMode = mode;
    if (mode == InteractionMode::Pan) {
        setCursor(Qt::OpenHandCursor);
    } else if (mode == InteractionMode::Select) {
        setCursor(Qt::CrossCursor);
    } else {
        setCursor(Qt::ArrowCursor);
    }
}

GridView::InteractionMode GridView::interactionMode() const {
    return m_interactionMode;
}

void GridView::setPattern(const Pattern& pattern) {
    m_currentPattern = pattern;
    m_hasPattern = true;
    update();
}

void GridView::clearPattern() {
    m_hasPattern = false;
    update();
}

void GridView::placePattern(int x, int y) {
    if (!m_grid || !m_hasPattern) return;
    
    for (const auto& cell : m_currentPattern.cells) {
        m_grid->setCell(x + cell.first, y + cell.second, true);
    }
    
    update();
    emit patternPlaced();
}

QRect GridView::getSelection() const {
    return m_selection;
}

void GridView::clearSelection() {
    m_selection = QRect();
    update();
}

std::vector<std::pair<int, int>> GridView::getSelectionCells() const {
    std::vector<std::pair<int, int>> cells;
    
    if (!m_grid || !m_selection.isValid()) {
        return cells;
    }
    
    for (int y = m_selection.top(); y <= m_selection.bottom(); ++y) {
        for (int x = m_selection.left(); x <= m_selection.right(); ++x) {
            if (m_grid->isAlive(x, y)) {
                cells.push_back({x - m_selection.left(), y - m_selection.top()});
            }
        }
    }
    
    return cells;
}

bool GridView::hasSelection() const {
    return m_selection.isValid();
}

void GridView::paintEvent(QPaintEvent* event) {
    Q_UNUSED(event);
    
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, false);
    
    painter.fillRect(rect(), Qt::white);
    
    painter.translate(m_offset);
    painter.scale(m_scale, m_scale);
    
    if (m_grid) {
        drawCells(painter);
        if (m_showGrid && m_scale >= 0.5) {
            drawGrid(painter);
        }
    }
    
    if (m_hasPattern) {
        drawPatternPreview(painter);
    }
    
    if (m_selecting || m_selection.isValid()) {
        drawSelection(painter);
    }
}

void GridView::drawGrid(QPainter& painter) {
    painter.setPen(QPen(QColor(200, 200, 200), 1));
    
    int w = m_grid->width();
    int h = m_grid->height();
    
    for (int x = 0; x <= w; ++x) {
        painter.drawLine(x * m_cellSize, 0, x * m_cellSize, h * m_cellSize);
    }
    for (int y = 0; y <= h; ++y) {
        painter.drawLine(0, y * m_cellSize, w * m_cellSize, y * m_cellSize);
    }
}

void GridView::drawCells(QPainter& painter) {
    painter.setPen(Qt::NoPen);
    
    int w = m_grid->width();
    int h = m_grid->height();
    
    for (int y = 0; y < h; ++y) {
        for (int x = 0; x < w; ++x) {
            const Cell& cell = m_grid->at(x, y);
            if (cell.isAlive()) {
                int age = cell.age();
                int maxAge = 50;
                float ratio = qMin(static_cast<float>(age) / maxAge, 1.0f);
                
                int r = static_cast<int>(30 + ratio * 180);
                int g = static_cast<int>(100 - ratio * 50);
                int b = static_cast<int>(200 - ratio * 150);
                
                painter.setBrush(QBrush(QColor(r, g, b)));
                painter.drawRect(x * m_cellSize, y * m_cellSize, m_cellSize, m_cellSize);
            }
        }
    }
}

void GridView::drawPatternPreview(QPainter& painter) {
    painter.setBrush(QBrush(QColor(100, 100, 255, 128)));
    painter.setPen(QPen(QColor(50, 50, 200), 1));
    
    for (const auto& cell : m_currentPattern.cells) {
        int x = m_patternPos.x() + cell.first;
        int y = m_patternPos.y() + cell.second;
        painter.drawRect(x * m_cellSize, y * m_cellSize, m_cellSize, m_cellSize);
    }
}

void GridView::drawSelection(QPainter& painter) {
    painter.setPen(QPen(QColor(255, 100, 100), 2));
    painter.setBrush(QBrush(QColor(255, 100, 100, 50)));
    
    if (m_selecting) {
        int x1 = qMin(m_selectionStart.x(), m_selectionEnd.x());
        int y1 = qMin(m_selectionStart.y(), m_selectionEnd.y());
        int x2 = qMax(m_selectionStart.x(), m_selectionEnd.x());
        int y2 = qMax(m_selectionStart.y(), m_selectionEnd.y());
        
        painter.drawRect(x1 * m_cellSize, y1 * m_cellSize,
                        (x2 - x1 + 1) * m_cellSize, (y2 - y1 + 1) * m_cellSize);
    } else if (m_selection.isValid()) {
        painter.drawRect(m_selection.left() * m_cellSize, m_selection.top() * m_cellSize,
                        m_selection.width() * m_cellSize, m_selection.height() * m_cellSize);
    }
}

void GridView::mousePressEvent(QMouseEvent* event) {
    if (!m_grid) return;
    
    if (event->button() == Qt::MiddleButton) {
        m_panning = true;
        m_lastPanPos = event->pos();
        setCursor(Qt::ClosedHandCursor);
        return;
    }
    
    if (event->button() == Qt::LeftButton && m_hasPattern) {
        placePattern(m_patternPos.x(), m_patternPos.y());
        return;
    }
    
    if (event->button() == Qt::LeftButton) {
        if (m_interactionMode == InteractionMode::Pan) {
            m_panning = true;
            m_lastPanPos = event->pos();
            setCursor(Qt::ClosedHandCursor);
        } else if (m_interactionMode == InteractionMode::Select) {
            m_selecting = true;
            m_selectionStart = gridToCell(event->pos());
            m_selectionEnd = m_selectionStart;
        } else {
            QPoint cell = gridToCell(event->pos());
            
            if (cell.x() >= 0 && cell.x() < m_grid->width() &&
                cell.y() >= 0 && cell.y() < m_grid->height()) {
                
                m_drawing = true;
                m_drawState = !m_grid->isAlive(cell.x(), cell.y());
                m_grid->setCell(cell.x(), cell.y(), m_drawState);
                emit cellClicked(cell.x(), cell.y(), m_drawState);
                update();
            }
        }
    }
}

void GridView::mouseMoveEvent(QMouseEvent* event) {
    if (m_panning) {
        QPointF delta = event->pos() - m_lastPanPos;
        m_offset += delta;
        m_lastPanPos = event->pos();
        update();
        return;
    }
    
    if (m_selecting) {
        m_selectionEnd = gridToCell(event->pos());
        update();
        return;
    }
    
    if (m_hasPattern) {
        m_patternPos = gridToCell(event->pos());
        update();
        return;
    }
    
    if (!m_grid || !m_drawing) return;
    
    QPoint cell = gridToCell(event->pos());
    
    if (cell.x() >= 0 && cell.x() < m_grid->width() &&
        cell.y() >= 0 && cell.y() < m_grid->height()) {
        
        if (m_grid->isAlive(cell.x(), cell.y()) != m_drawState) {
            m_grid->setCell(cell.x(), cell.y(), m_drawState);
            emit cellClicked(cell.x(), cell.y(), m_drawState);
            update();
        }
    }
}

void GridView::mouseReleaseEvent(QMouseEvent* event) {
    if (event->button() == Qt::MiddleButton) {
        m_panning = false;
        if (m_interactionMode == InteractionMode::Pan) {
            setCursor(Qt::OpenHandCursor);
        } else {
            setCursor(Qt::ArrowCursor);
        }
        return;
    }
    
    if (event->button() == Qt::LeftButton && m_panning) {
        m_panning = false;
        if (m_interactionMode == InteractionMode::Pan) {
            setCursor(Qt::OpenHandCursor);
        } else {
            setCursor(Qt::ArrowCursor);
        }
        return;
    }
    
    if (event->button() == Qt::LeftButton && m_selecting) {
        m_selecting = false;
        int x1 = qMin(m_selectionStart.x(), m_selectionEnd.x());
        int y1 = qMin(m_selectionStart.y(), m_selectionEnd.y());
        int x2 = qMax(m_selectionStart.x(), m_selectionEnd.x());
        int y2 = qMax(m_selectionStart.y(), m_selectionEnd.y());
        
        m_selection = QRect(x1, y1, x2 - x1 + 1, y2 - y1 + 1);
        emit selectionChanged(m_selection);
        update();
        return;
    }
    
    m_drawing = false;
}

void GridView::wheelEvent(QWheelEvent* event) {
    double factor = event->angleDelta().y() > 0 ? 1.1 : 0.9;
    double newScale = m_scale * factor;
    
    if (newScale >= 0.1 && newScale <= 10.0) {
        QPointF oldPos = event->position();
        QPointF oldWorld = (oldPos - m_offset) / m_scale;
        
        m_scale = newScale;
        
        QPointF newWorld = (oldPos - m_offset) / m_scale;
        m_offset += (newWorld - oldWorld) * m_scale;
        
        update();
    }
}

void GridView::keyPressEvent(QKeyEvent* event) {
    if (event->key() == Qt::Key_Escape) {
        clearSelection();
        clearPattern();
    }
    QWidget::keyPressEvent(event);
}

QPointF GridView::screenToGrid(const QPointF& screenPos) const {
    return (screenPos - m_offset) / m_scale;
}

QPoint GridView::gridToCell(const QPointF& gridPos) const {
    QPointF worldPos = screenToGrid(gridPos);
    return QPoint(static_cast<int>(worldPos.x() / m_cellSize),
                  static_cast<int>(worldPos.y() / m_cellSize));
}

void GridView::updateTransform() {
    update();
}

void GridView::contextMenuEvent(QContextMenuEvent* event) {
    if (!m_selection.isValid() || m_interactionMode != InteractionMode::Select) {
        return;
    }
    
    QPoint cell = gridToCell(event->pos());
    if (!m_selection.contains(cell)) {
        return;
    }
    
    QMenu menu(this);
    QAction* copyAction = menu.addAction(tr("复制图案"));
    QAction* saveAction = menu.addAction(tr("保存至图案库"));
    menu.addSeparator();
    QAction* clearAction = menu.addAction(tr("清空区域"));
    QAction* fillAction = menu.addAction(tr("填满区域"));
    
    QAction* selected = menu.exec(event->globalPos());
    
    if (selected == copyAction) {
        m_clipboard = getSelectionAsPattern();
        m_hasClipboard = true;
        m_currentPattern = m_clipboard;
        m_hasPattern = true;
        m_patternPos = gridToCell(event->pos());
        clearSelection();
        update();
    } else if (selected == saveAction) {
        emit savePatternRequested();
    } else if (selected == clearAction) {
        fillSelection(false);
        emit gridUpdated();
    } else if (selected == fillAction) {
        fillSelection(true);
        emit gridUpdated();
    }
}

void GridView::fillSelection(bool alive) {
    if (!m_grid || !m_selection.isValid()) return;
    
    for (int y = m_selection.top(); y <= m_selection.bottom(); ++y) {
        for (int x = m_selection.left(); x <= m_selection.right(); ++x) {
            if (x >= 0 && x < m_grid->width() && y >= 0 && y < m_grid->height()) {
                m_grid->setCell(x, y, alive);
            }
        }
    }
    
    update();
}

Pattern GridView::getSelectionAsPattern() const {
    Pattern pattern;
    pattern.name = "Selection";
    pattern.cells = getSelectionCells();
    return pattern;
}
