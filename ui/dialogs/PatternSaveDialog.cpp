/**
 * @file PatternSaveDialog.cpp
 * @brief 图案保存对话框实现
 * @author 1
 */

#include "PatternSaveDialog.h"
#include <QVBoxLayout>
#include <QFormLayout>
#include <QDialogButtonBox>
#include <QPushButton>
#include <QPainter>

PatternSaveDialog::PatternSaveDialog(QWidget* parent)
    : QDialog(parent)
    , m_nameEdit(new QLineEdit(this))
    , m_descEdit(new QTextEdit(this))
    , m_categoryCombo(new QComboBox(this))
    , m_previewLabel(new QLabel(this))
    , m_cellCountLabel(new QLabel(tr("细胞数: 0"), this))
{
    setWindowTitle(tr("保存图案"));
    resize(400, 450);
    
    m_categoryCombo->addItem(tr("自定义"));
    m_categoryCombo->addItem(tr("振荡器"));
    m_categoryCombo->addItem(tr("飞船"));
    m_categoryCombo->addItem(tr("枪"));
    m_categoryCombo->addItem(tr("其他"));
    
    m_descEdit->setMaximumHeight(80);
    
    m_previewLabel->setMinimumSize(200, 200);
    m_previewLabel->setAlignment(Qt::AlignCenter);
    m_previewLabel->setFrameStyle(QFrame::StyledPanel | QFrame::Sunken);
    m_previewLabel->setText(tr("无预览"));
    
    QFormLayout* formLayout = new QFormLayout;
    formLayout->addRow(tr("名称:"), m_nameEdit);
    formLayout->addRow(tr("描述:"), m_descEdit);
    formLayout->addRow(tr("分类:"), m_categoryCombo);
    formLayout->addRow(m_cellCountLabel);
    
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->addLayout(formLayout);
    mainLayout->addWidget(new QLabel(tr("预览:"), this));
    mainLayout->addWidget(m_previewLabel, 1);
    
    QDialogButtonBox* buttons = new QDialogButtonBox(
        QDialogButtonBox::Save | QDialogButtonBox::Cancel, this);
    buttons->button(QDialogButtonBox::Save)->setEnabled(false);
    
    connect(buttons, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(buttons, &QDialogButtonBox::rejected, this, &QDialog::reject);
    connect(m_nameEdit, &QLineEdit::textChanged, this, &PatternSaveDialog::validateInput);
    
    mainLayout->addWidget(buttons);
}

void PatternSaveDialog::setPattern(const Pattern& pattern) {
    m_pattern = pattern;
    m_nameEdit->setText(QString::fromStdString(pattern.name));
    m_descEdit->setText(QString::fromStdString(pattern.description));
    m_cells = pattern.cells;
    updatePreview();
}

void PatternSaveDialog::setPreviewCells(const std::vector<std::pair<int, int>>& cells) {
    m_cells = cells;
    updatePreview();
}

Pattern PatternSaveDialog::getPattern() const {
    Pattern pattern;
    pattern.name = m_nameEdit->text().toStdString();
    pattern.description = m_descEdit->toPlainText().toStdString();
    pattern.category = m_categoryCombo->currentText().toStdString();
    pattern.cells = m_cells;
    pattern.rule = "B3/S23";
    return pattern;
}

void PatternSaveDialog::validateInput() {
    QPushButton* saveBtn = findChild<QDialogButtonBox*>()->button(QDialogButtonBox::Save);
    saveBtn->setEnabled(!m_nameEdit->text().trimmed().isEmpty() && !m_cells.empty());
}

void PatternSaveDialog::updatePreview() {
    if (m_cells.empty()) {
        m_previewLabel->setText(tr("无细胞"));
        m_cellCountLabel->setText(tr("细胞数: 0"));
        return;
    }
    
    m_cellCountLabel->setText(tr("细胞数: %1").arg(m_cells.size()));
    
    int minX = m_cells[0].first;
    int maxX = m_cells[0].first;
    int minY = m_cells[0].second;
    int maxY = m_cells[0].second;
    
    for (const auto& cell : m_cells) {
        minX = std::min(minX, cell.first);
        maxX = std::max(maxX, cell.first);
        minY = std::min(minY, cell.second);
        maxY = std::max(maxY, cell.second);
    }
    
    int width = maxX - minX + 1;
    int height = maxY - minY + 1;
    
    int cellSize = qMin(15, qMin(180 / width, 180 / height));
    cellSize = qMax(3, cellSize);
    
    int imgWidth = width * cellSize;
    int imgHeight = height * cellSize;
    
    QImage image(imgWidth, imgHeight, QImage::Format_RGB32);
    image.fill(Qt::white);
    
    QPainter painter(&image);
    painter.setBrush(QBrush(QColor(30, 100, 200)));
    painter.setPen(Qt::NoPen);
    
    for (const auto& cell : m_cells) {
        int x = (cell.first - minX) * cellSize;
        int y = (cell.second - minY) * cellSize;
        painter.drawRect(x, y, cellSize, cellSize);
    }
    
    m_previewLabel->setPixmap(QPixmap::fromImage(image));
    m_previewLabel->setText("");
    
    validateInput();
}
