#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QMainWindow>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QSlider>
#include <QTextBrowser>

#include "DerivativeVisualizer.hpp"
#include "Parameter.hpp"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);

    void paintEvent(QPaintEvent *event) override;

private:
    void setEquation();
    void addParameter();
    void changeParameters();
    void showLicenseDialog();


    QWidget *centralWidget_;
    QVBoxLayout *mainLayout_;
    QHBoxLayout *setUpLayout_;
    QVBoxLayout *parametersLayout_;
    QHBoxLayout *parametersButtonLayout_;
    QHBoxLayout *visualizerLayout_;
    QHBoxLayout *rangeLayout_;
    QHBoxLayout *licenseLayout_;
    QLineEdit   *lineEquation_;
    QLabel      *label_fx_;
    QPushButton *setButton_;
    QPushButton *addParameterButton_;
    QLabel      *label_error_;
    QLineEdit   *xminLineEdit_;
    QLineEdit   *xmaxLineEdit_;
    QLineEdit   *yminLineEdit_;
    QLineEdit   *ymaxLineEdit_;
    QPushButton *licenseButton_;
    QTextBrowser *licenseTextBrowser_;

    DerivativeVisualizer *visualizer_;
    QVector<Parameter*> parameters_;
    QVector<QLineEdit*> parametersName;
    QVector<QLineEdit*> parametersMin_;
    QVector<QLineEdit*> parametersMax_;
    QVector<QSlider*> parametersSlider_;
    QVector<QPushButton*> parametersRemoveButton_;
    QVector<QPair<QString,double>> parametersValue_;
    double xmin_;
    double xmax_;
    double ymin_;
    double ymax_;


};

#endif // MAINWINDOW_HPP