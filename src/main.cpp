#include <QApplication>
#include <QWidget>
#include <QPainter>
#include <QPainterPath>
#include <QMouseEvent>
#include <cmath>
#include <algorithm>
#include "MainWindow.hpp"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    MainWindow w;
    w.resize(600, 400);
    w.show();
    return app.exec();
}
