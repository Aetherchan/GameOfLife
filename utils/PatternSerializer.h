/**
 * @file PatternSerializer.h
 * @brief 图案序列化工具类
 * @author 1
 */

#ifndef PATTERNSERIALIZER_H
#define PATTERNSERIALIZER_H

#include "Pattern.h"
#include <QString>
#include <QJsonObject>

class PatternSerializer {
public:
    static QJsonObject toJson(const Pattern& pattern);                                   ///< 图案转 JSON
    static Pattern fromJson(const QJsonObject& json);                                    ///< JSON 解析为图案
    static bool saveToFile(const QString& filePath, const Pattern& pattern);             ///< 保存单个图案到文件
    static bool loadFromFile(const QString& filePath, Pattern& pattern);                 ///< 从文件加载单个图案
    static bool savePatternLibrary(const QString& filePath, const std::vector<Pattern>& patterns); ///< 保存图案库
    static bool loadPatternLibrary(const QString& filePath, std::vector<Pattern>& patterns);       ///< 加载图案库
};

#endif
