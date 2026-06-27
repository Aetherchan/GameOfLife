/**
 * @file PatternPanel.h
 * @brief 图案面板控件
 * @author 2
 */

#ifndef PATTERNPANEL_H
#define PATTERNPANEL_H

#include <QWidget>
#include <QListWidget>
#include <QPushButton>
#include "Pattern.h"

/**
 * @brief 图案面板控件，用于显示和管理生命游戏图案库
 */
class PatternPanel : public QWidget {
    Q_OBJECT
    
public:
    explicit PatternPanel(QWidget* parent = nullptr);
    
    /// @brief 从 JSON 文件加载图案库
    bool loadPatterns(const QString& filePath);
    
    /// @brief 添加新图案到列表
    void addPattern(const Pattern& pattern);
    
    /// @brief 移除指定索引的图案
    void removePattern(int index);
    
    /// @brief 获取当前选中图案的索引
    int selectedIndex() const;
    
signals:
    /// @brief 图案被选中时发出
    void patternSelected(const Pattern& pattern);
    
    /// @brief 图案取消选中时发出
    void patternDeselected();
    
private slots:
    /// @brief 响应图案列表项点击
    void onPatternClicked(QListWidgetItem* item);
    
    /// @brief 将选中图案顺时针旋转 90 度
    void onRotatePattern();
    
    /// @brief 将选中图案水平翻转
    void onFlipPattern();
    
    /// @brief 删除当前选中的图案
    void onDeletePattern();
    
private:
    QListWidget* m_patternList;      ///< 图案列表控件
    QPushButton* m_rotateBtn;        ///< 旋转按钮（90°）
    QPushButton* m_flipBtn;          ///< 翻转按钮（水平）
    QPushButton* m_deleteBtn;        ///< 删除按钮
    std::vector<Pattern> m_patterns; ///< 图案数据容器
    int m_selectedIndex;             ///< 当前选中图案索引，-1 表示未选中
    Pattern m_currentPattern;        ///< 当前操作的图案副本
    QString m_userPatternsPath;      ///< 用户自定义图案保存路径
};

#endif // PATTERNPANEL_H
