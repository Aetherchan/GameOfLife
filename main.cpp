/**
 * @file main.cpp
 * @brief 应用程序入口
 * @author 1(1为史云天,2为赵思涵,3为万泽奇)
 */

#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
