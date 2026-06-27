/**
 * @file mainwindow.cpp
 * @brief 应用程序主窗口实现
 * @author 1(1为史云天,2为赵思涵,3为万泽奇)
 */

#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "GridSettingsDialog.h"
#include "PatternSaveDialog.h"
#include "PatternSerializer.h"
#include <QToolBar>
#include <QStatusBar>
#include <QSlider>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMenuBar>
#include <QMenu>
#include <QMessageBox>
#include <QTimer>
#include <QCoreApplication>
#include <QDir>
#include <QDebug>
#include <QActionGroup>
#include <QStandardPaths>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_grid(new Grid(100, 100))
    , m_simulator(new Simulator(this))
    , m_gridView(new GridView(this))
    , m_sideBar(new SideBar(this))
    , m_centralWidget(new QWidget(this))
    , m_timerRunning(false)
{
    ui->setupUi(this);
    
    setupUI();
    createMenus();
    createConnections();
    
    m_simulator->setGrid(m_grid);
    m_gridView->setGrid(m_grid);
    
    QStringList searchPaths;
    searchPaths << QDir::currentPath() + "/data/patterns.json";
    searchPaths << QCoreApplication::applicationDirPath() + "/data/patterns.json";
    searchPaths << QDir::currentPath() + "/../data/patterns.json";
    searchPaths << QDir::currentPath() + "/../../data/patterns.json";
    
    bool loaded = false;
    QString loadedPath;
    for (const QString& path : searchPaths) {
        qDebug() << "Checking path:" << path << "exists:" << QFile::exists(path);
        if (QFile::exists(path)) {
            if (m_sideBar->patternPanel()->loadPatterns(path)) {
                loaded = true;
                loadedPath = path;
                qDebug() << "Successfully loaded patterns from:" << path;
                break;
            } else {
                qDebug() << "Failed to parse patterns from:" << path;
            }
        }
    }
    
    if (!loaded) {
        statusBar()->showMessage(tr("警告: 无法加载图案库，请检查 data/patterns.json"), 5000);
    } else {
        statusBar()->showMessage(tr("已加载图案库: %1").arg(loadedPath), 3000);
    }
    
    updateStats();
    updateModeButtons();
    
    QTimer* timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindow::updateElapsedTime);
    timer->start(1000);
}

MainWindow::~MainWindow() {
    delete ui;
    delete m_grid;
}

void MainWindow::setupUI() {
    setWindowTitle(tr("生命游戏 - Conway's Game of Life"));
    resize(1000, 700);
    
    QToolBar* toolbar = addToolBar(tr("控制"));
    
    m_startAction = toolbar->addAction(tr("开始"));
    m_stepAction = toolbar->addAction(tr("单步"));
    m_clearAction = toolbar->addAction(tr("清空"));
    
    toolbar->addSeparator();
    
    QActionGroup* modeGroup = new QActionGroup(this);
    modeGroup->setExclusive(true);
    
    m_drawModeAction = toolbar->addAction(tr("绘制"));
    m_drawModeAction->setCheckable(true);
    m_drawModeAction->setChecked(true);
    modeGroup->addAction(m_drawModeAction);
    
    m_panModeAction = toolbar->addAction(tr("拖动"));
    m_panModeAction->setCheckable(true);
    modeGroup->addAction(m_panModeAction);
    
    m_selectModeAction = toolbar->addAction(tr("选择"));
    m_selectModeAction->setCheckable(true);
    modeGroup->addAction(m_selectModeAction);
    
    m_savePatternAction = toolbar->addAction(tr("保存图案"));
    m_savePatternAction->setEnabled(false);
    
    toolbar->addSeparator();
    
    QLabel* speedLabel = new QLabel(tr("速度: "), this);
    toolbar->addWidget(speedLabel);
    
    QSlider* speedSlider = new QSlider(Qt::Horizontal, this);
    speedSlider->setObjectName("speedSlider");
    speedSlider->setRange(10, 500);
    speedSlider->setValue(100);
    speedSlider->setMaximumWidth(150);
    toolbar->addWidget(speedSlider);
    
    m_simulator->setInterval(410);
    
    QHBoxLayout* mainLayout = new QHBoxLayout(m_centralWidget);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->addWidget(m_gridView, 1);
    mainLayout->addWidget(m_sideBar);
    
    setCentralWidget(m_centralWidget);
    
    statusBar()->showMessage(tr("就绪"));
}

void MainWindow::createMenus() {
    QMenu* fileMenu = menuBar()->addMenu(tr("文件"));
    
    QAction* newAction = fileMenu->addAction(tr("新建"));
    newAction->setShortcut(QKeySequence::New);
    connect(newAction, &QAction::triggered, this, [this]() {
        m_grid->clear();
        m_simulator->reset();
        m_gridView->update();
        updateStats();
    });
    
    fileMenu->addSeparator();
    
    QAction* quitAction = fileMenu->addAction(tr("退出"));
    quitAction->setShortcut(QKeySequence::Quit);
    connect(quitAction, &QAction::triggered, this, &QWidget::close);
    
    QMenu* editMenu = menuBar()->addMenu(tr("编辑"));
    
    QAction* clearAction = editMenu->addAction(tr("清空"));
    clearAction->setShortcut(QKeySequence::Delete);
    connect(clearAction, &QAction::triggered, this, &MainWindow::onClear);
    
    editMenu->addAction(tr("网格设置"), this, &MainWindow::onGridSettings);
    
    QMenu* viewMenu = menuBar()->addMenu(tr("视图"));
    
    m_showGridAction = viewMenu->addAction(tr("显示网格线"), this, [this](bool checked) {
        m_gridView->setShowGrid(checked);
    });
    m_showGridAction->setCheckable(true);
    m_showGridAction->setChecked(true);
    
    m_showSidebarAction = viewMenu->addAction(tr("显示侧边栏"), this, [this](bool checked) {
        m_sideBar->setVisible(checked);
    });
    m_showSidebarAction->setCheckable(true);
    m_showSidebarAction->setChecked(true);
    
    QMenu* helpMenu = menuBar()->addMenu(tr("帮助"));
    
    helpMenu->addAction(tr("关于"), this, [this]() {
        QMessageBox::about(this, tr("关于"),
            tr("<h3>生命游戏</h3>"
               "<p>基于 Qt 开发的 Conway's Game of Life GUI 程序</p>"
               "<p>支持 B/S 规则系统、图案库、多种网格模式</p>"));
    });
}

void MainWindow::createConnections() {
    connect(m_startAction, &QAction::triggered, this, &MainWindow::onStartStop);
    connect(m_stepAction, &QAction::triggered, this, &MainWindow::onStep);
    connect(m_clearAction, &QAction::triggered, this, &MainWindow::onClear);
    
    connect(m_drawModeAction, &QAction::triggered, this, &MainWindow::setDrawMode);
    connect(m_panModeAction, &QAction::triggered, this, &MainWindow::setPanMode);
    connect(m_selectModeAction, &QAction::triggered, this, &MainWindow::setSelectMode);
    connect(m_savePatternAction, &QAction::triggered, this, &MainWindow::savePattern);
    
    connect(m_simulator, &Simulator::generationChanged, this, &MainWindow::onGenerationChanged);
    connect(m_simulator, &Simulator::statsUpdated, this, &MainWindow::onStatsUpdated);
    connect(m_simulator, &Simulator::stepped, m_gridView, QOverload<>::of(&QWidget::update));
    
    connect(m_sideBar->rulePanel(), &RulePanel::ruleChanged, this, &MainWindow::onRuleChanged);
    
    connect(m_sideBar->patternPanel(), &PatternPanel::patternSelected, this, [this](const Pattern& pattern) {
        m_gridView->setPattern(pattern);
    });
    connect(m_sideBar->patternPanel(), &PatternPanel::patternDeselected, m_gridView, &GridView::clearPattern);
    
    connect(m_gridView, &GridView::patternPlaced, this, [this]() {
        m_gridView->clearPattern();
        updateStats();
    });
    connect(m_gridView, &GridView::selectionChanged, this, &MainWindow::onSelectionChanged);
    connect(m_gridView, &GridView::savePatternRequested, this, &MainWindow::savePattern);
    connect(m_gridView, &GridView::gridUpdated, this, &MainWindow::updateStats);
    
    QSlider* speedSlider = findChild<QSlider*>("speedSlider");
    if (speedSlider) {
        connect(speedSlider, &QSlider::valueChanged, this, &MainWindow::onSpeedChanged);
    }
}

void MainWindow::onStartStop() {
    if (m_simulator->isRunning()) {
        m_simulator->stop();
        m_startAction->setText(tr("开始"));
        m_timerRunning = false;
    } else {
        m_simulator->start(m_simulator->interval());
        m_startAction->setText(tr("暂停"));
        if (!m_timerRunning) {
            m_elapsedTimer.start();
            m_timerRunning = true;
        }
    }
}

void MainWindow::onStep() {
    m_simulator->step();
    m_gridView->update();
}

void MainWindow::onClear() {
    if (m_simulator->isRunning()) {
        m_simulator->stop();
        m_startAction->setText(tr("开始"));
    }
    m_grid->clear();
    m_simulator->reset();
    m_gridView->update();
    m_timerRunning = false;
    updateStats();
    m_sideBar->statsPanel()->setElapsedTime(0);
}

void MainWindow::onSpeedChanged(int value) {
    int interval = 510 - value;
    m_simulator->setInterval(interval);
}

void MainWindow::onGridSettings() {
    GridSettingsDialog dialog(this);
    
    dialog.setGridWidth(m_grid->width());
    dialog.setGridHeight(m_grid->height());
    dialog.setBoundaryType(m_grid->boundaryType());
    dialog.setGridMode(m_grid->mode());
    
    if (dialog.exec() == QDialog::Accepted) {
        m_grid->resize(dialog.gridWidth(), dialog.gridHeight());
        m_grid->setBoundaryType(dialog.boundaryType());
        m_grid->setMode(dialog.gridMode());
        m_gridView->update();
        updateStats();
    }
}

void MainWindow::onRuleChanged(const Rule& rule) {
    m_simulator->setRule(rule);
    m_sideBar->statsPanel()->setRule(QString::fromStdString(rule.toString()));
}

void MainWindow::onGenerationChanged(int gen) {
    m_sideBar->statsPanel()->setGeneration(gen);
    statusBar()->showMessage(tr("代数: %1").arg(gen));
}

void MainWindow::onStatsUpdated(int aliveCount) {
    m_sideBar->statsPanel()->setAliveCount(aliveCount);
}

void MainWindow::updateElapsedTime() {
    if (m_timerRunning) {
        m_sideBar->statsPanel()->setElapsedTime(m_elapsedTimer.elapsed() / 1000);
    }
}

void MainWindow::updateStats() {
    m_sideBar->statsPanel()->setGeneration(m_simulator->generation());
    m_sideBar->statsPanel()->setAliveCount(m_grid->getAliveCount());
    m_sideBar->statsPanel()->setRule(QString::fromStdString(m_simulator->currentRule().toString()));
    statusBar()->showMessage(tr("活细胞: %1 | 代数: %2")
        .arg(m_grid->getAliveCount())
        .arg(m_simulator->generation()));
}

void MainWindow::setDrawMode() {
    m_gridView->setInteractionMode(GridView::InteractionMode::Draw);
    updateModeButtons();
}

void MainWindow::setPanMode() {
    m_gridView->setInteractionMode(GridView::InteractionMode::Pan);
    updateModeButtons();
}

void MainWindow::setSelectMode() {
    m_gridView->setInteractionMode(GridView::InteractionMode::Select);
    updateModeButtons();
}

void MainWindow::updateModeButtons() {
    m_drawModeAction->setChecked(m_gridView->interactionMode() == GridView::InteractionMode::Draw);
    m_panModeAction->setChecked(m_gridView->interactionMode() == GridView::InteractionMode::Pan);
    m_selectModeAction->setChecked(m_gridView->interactionMode() == GridView::InteractionMode::Select);
}

void MainWindow::onSelectionChanged(const QRect& selection) {
    m_savePatternAction->setEnabled(selection.isValid() && !m_gridView->getSelectionCells().empty());
}

void MainWindow::savePattern() {
    auto cells = m_gridView->getSelectionCells();
    
    if (cells.empty()) {
        QMessageBox::warning(this, tr("保存图案"), tr("选区内没有活细胞，无法保存图案。"));
        return;
    }
    
    PatternSaveDialog dialog(this);
    dialog.setPreviewCells(cells);
    
    if (dialog.exec() == QDialog::Accepted) {
        Pattern pattern = dialog.getPattern();
        
        QString dataPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
        QDir dir(dataPath);
        if (!dir.exists()) {
            dir.mkpath(".");
        }
        
        QString filePath = dataPath + "/user_patterns.json";
        
        std::vector<Pattern> existingPatterns;
        if (QFile::exists(filePath)) {
            PatternSerializer::loadPatternLibrary(filePath, existingPatterns);
        }
        
        existingPatterns.push_back(pattern);
        
        if (PatternSerializer::savePatternLibrary(filePath, existingPatterns)) {
            m_sideBar->patternPanel()->addPattern(pattern);
            statusBar()->showMessage(tr("图案 '%1' 已保存").arg(QString::fromStdString(pattern.name)), 3000);
            m_gridView->clearSelection();
            m_savePatternAction->setEnabled(false);
        } else {
            QMessageBox::warning(this, tr("保存图案"), tr("无法保存图案，请检查文件权限。"));
        }
    }
}
