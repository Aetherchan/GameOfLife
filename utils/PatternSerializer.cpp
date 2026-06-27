/**
 * @file PatternSerializer.cpp
 * @brief 图案序列化工具类实现
 * @author 1
 */

#include "PatternSerializer.h"
#include <QJsonDocument>
#include <QJsonArray>
#include <QFile>

QJsonObject PatternSerializer::toJson(const Pattern& pattern) {
    QJsonObject obj;
    obj["name"] = QString::fromStdString(pattern.name);
    obj["description"] = QString::fromStdString(pattern.description);
    obj["category"] = QString::fromStdString(pattern.category);
    obj["rule"] = QString::fromStdString(pattern.rule);
    
    QJsonArray cellsArray;
    for (const auto& cell : pattern.cells) {
        QJsonArray cellArray;
        cellArray.append(cell.first);
        cellArray.append(cell.second);
        cellsArray.append(cellArray);
    }
    obj["cells"] = cellsArray;
    
    return obj;
}

Pattern PatternSerializer::fromJson(const QJsonObject& json) {
    Pattern pattern;
    pattern.name = json["name"].toString().toStdString();
    pattern.description = json["description"].toString().toStdString();
    pattern.category = json["category"].toString().toStdString();
    pattern.rule = json["rule"].toString().toStdString();
    
    QJsonArray cellsArray = json["cells"].toArray();
    for (const auto& cellValue : cellsArray) {
        QJsonArray cellArray = cellValue.toArray();
        pattern.cells.push_back({cellArray[0].toInt(), cellArray[1].toInt()});
    }
    
    return pattern;
}

bool PatternSerializer::saveToFile(const QString& filePath, const Pattern& pattern) {
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly)) return false;
    
    QJsonDocument doc(toJson(pattern));
    file.write(doc.toJson());
    return true;
}

bool PatternSerializer::loadFromFile(const QString& filePath, Pattern& pattern) {
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) return false;
    
    QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
    pattern = fromJson(doc.object());
    return true;
}

bool PatternSerializer::savePatternLibrary(const QString& filePath, const std::vector<Pattern>& patterns) {
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly)) return false;
    
    QJsonArray patternsArray;
    for (const auto& pattern : patterns) {
        patternsArray.append(toJson(pattern));
    }
    
    QJsonDocument doc(patternsArray);
    file.write(doc.toJson());
    return true;
}

bool PatternSerializer::loadPatternLibrary(const QString& filePath, std::vector<Pattern>& patterns) {
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) return false;
    
    QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
    QJsonArray patternsArray = doc.array();
    
    patterns.clear();
    for (const auto& patternValue : patternsArray) {
        patterns.push_back(fromJson(patternValue.toObject()));
    }
    
    return true;
}
