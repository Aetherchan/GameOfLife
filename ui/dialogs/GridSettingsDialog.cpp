/**
 * @file GridSettingsDialog.cpp
 * @brief 网格设置对话框实现
 * @author 1
 */

#include "GridSettingsDialog.h"
#include <QVBoxLayout>
#include <QFormLayout>
#include <QDialogButtonBox>

GridSettingsDialog::GridSettingsDialog(QWidget* parent)
    : QDialog(parent)
    , m_widthSpin(new QSpinBox(this))
    , m_heightSpin(new QSpinBox(this))
    , m_boundaryCombo(new QComboBox(this))
    , m_modeCombo(new QComboBox(this))
{
    setWindowTitle(tr("网格设置"));
    
    m_widthSpin->setRange(10, 1000);
    m_widthSpin->setValue(100);
    
    m_heightSpin->setRange(10, 1000);
    m_heightSpin->setValue(100);
    
    m_boundaryCombo->addItem(tr("死边界"), static_cast<int>(Grid::BoundaryType::Dead));
    m_boundaryCombo->addItem(tr("环形边界"), static_cast<int>(Grid::BoundaryType::Toroidal));
    
    m_modeCombo->addItem(tr("固定网格"), static_cast<int>(Grid::Mode::Fixed));
    m_modeCombo->addItem(tr("无限网格"), static_cast<int>(Grid::Mode::Infinite));
    
    QFormLayout* formLayout = new QFormLayout;
    formLayout->addRow(tr("宽度:"), m_widthSpin);
    formLayout->addRow(tr("高度:"), m_heightSpin);
    formLayout->addRow(tr("边界类型:"), m_boundaryCombo);
    formLayout->addRow(tr("网格模式:"), m_modeCombo);
    
    QDialogButtonBox* buttons = new QDialogButtonBox(
        QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    
    connect(buttons, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(buttons, &QDialogButtonBox::rejected, this, &QDialog::reject);
    
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->addLayout(formLayout);
    mainLayout->addWidget(buttons);
}

int GridSettingsDialog::gridWidth() const {
    return m_widthSpin->value();
}

int GridSettingsDialog::gridHeight() const {
    return m_heightSpin->value();
}

Grid::BoundaryType GridSettingsDialog::boundaryType() const {
    return static_cast<Grid::BoundaryType>(m_boundaryCombo->currentData().toInt());
}

Grid::Mode GridSettingsDialog::gridMode() const {
    return static_cast<Grid::Mode>(m_modeCombo->currentData().toInt());
}

void GridSettingsDialog::setGridWidth(int width) {
    m_widthSpin->setValue(width);
}

void GridSettingsDialog::setGridHeight(int height) {
    m_heightSpin->setValue(height);
}

void GridSettingsDialog::setBoundaryType(Grid::BoundaryType type) {
    int index = m_boundaryCombo->findData(static_cast<int>(type));
    if (index >= 0) m_boundaryCombo->setCurrentIndex(index);
}

void GridSettingsDialog::setGridMode(Grid::Mode mode) {
    int index = m_modeCombo->findData(static_cast<int>(mode));
    if (index >= 0) m_modeCombo->setCurrentIndex(index);
}
