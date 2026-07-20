#include <QApplication>
#include <QFontMetrics>
#include <QDoubleValidator>
#include <QMessageBox>

#include "MainWindow.hpp"
#include "Parameter.hpp"

MainWindow::MainWindow(QWidget *parent) 
: QMainWindow(parent) {
        this->setWindowTitle("Derivative Visualizer");
        this->setMinimumSize(800, 600);

        centralWidget_ = new QWidget(this);
        mainLayout_   = new QVBoxLayout(centralWidget_);
        setUpLayout_  = new QHBoxLayout();
        lineEquation_ = new QLineEdit();

        xmin_ = -5.0;
        xmax_ = 5.0;
        ymin_ = -5.0;
        ymax_ = 5.0;

        xminLineEdit_ = new QLineEdit("-5");
        xmaxLineEdit_ = new QLineEdit("5");
        yminLineEdit_ = new QLineEdit("-5");
        ymaxLineEdit_ = new QLineEdit("5");

        QDoubleValidator *validator = new QDoubleValidator(-1e4, 1e4, 2, this);
        xminLineEdit_->setValidator(validator);
        xmaxLineEdit_->setValidator(validator);
        yminLineEdit_->setValidator(validator);
        ymaxLineEdit_->setValidator(validator);
        QFontMetrics fmName(xminLineEdit_->font());
        const int rangeWidthName = fmName.horizontalAdvance(QString("M").repeated(6)) + 4;
        xminLineEdit_->setFixedWidth(rangeWidthName);
        xmaxLineEdit_->setFixedWidth(rangeWidthName);
        yminLineEdit_->setFixedWidth(rangeWidthName);
        ymaxLineEdit_->setFixedWidth(rangeWidthName);

        rangeLayout_ = new QHBoxLayout();
        rangeLayout_->addWidget(new QLabel("x: "));
        rangeLayout_->addWidget(xminLineEdit_);
        rangeLayout_->addWidget(new QLabel(" to "));
        rangeLayout_->addWidget(xmaxLineEdit_);
        rangeLayout_->addWidget(new QLabel("y: "));
        rangeLayout_->addWidget(yminLineEdit_);
        rangeLayout_->addWidget(new QLabel(" to "));
        rangeLayout_->addWidget(ymaxLineEdit_);
        rangeLayout_->addStretch();

        mainLayout_->addLayout(rangeLayout_);

        parametersLayout_ = new QVBoxLayout();
        addParameterButton_ = new QPushButton("Add Parameter");
        label_error_ = new QLabel();
        label_error_->setStyleSheet("QLabel { color : red; }");
        label_error_->setFixedHeight(30);
        parametersButtonLayout_ = new QHBoxLayout();
        parametersButtonLayout_->addWidget(addParameterButton_);
        parametersButtonLayout_->addWidget(label_error_);
        addParameterButton_->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        addParameterButton_->setFixedSize(120, 30);
        parametersLayout_->addLayout(parametersButtonLayout_);

        visualizerLayout_ = new QHBoxLayout();
        visualizer_ = new DerivativeVisualizer(parametersValue_, 
                                               xmin_, xmax_, ymin_, ymax_, this);
        visualizerLayout_->addWidget(visualizer_);

        label_fx_ = new QLabel("f(x) = ");
        setButton_ = new QPushButton("Set");

        setUpLayout_->addWidget(label_fx_);
        setUpLayout_->addWidget(lineEquation_);
        setUpLayout_->addWidget(setButton_);

        mainLayout_->addLayout(setUpLayout_);
        mainLayout_->addLayout(parametersLayout_);
        mainLayout_->addLayout(visualizerLayout_);

        licenseLayout_ = new QHBoxLayout();
        licenseButton_ = new QPushButton("License");
        licenseButton_->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        licenseButton_->setFixedSize(120, 30);
        licenseLayout_->addWidget(licenseButton_);
        licenseLayout_->addStretch();
        mainLayout_->addLayout(licenseLayout_);

        licenseTextBrowser_ = new QTextBrowser();
        licenseTextBrowser_->setHtml("<h3>使用ライセンス情報</h3>"
                                      "<p><b>Derivative Visualizer</b></p>"
                                      "<p>本アプリケーションでは以下のサードパーティ製ライブラリを使用しています。</p>"
                                      "<hr>"
                                      "<p><b>muParser</b><br>"
                                      "Copyright (c) 2004-2020 Ingo Berg<br>"
                                      "Licensed under the MIT License.</p>"
                                      "<hr>"
                                      "<p><b>Qt Toolkit</b><br>"
                                      "Licensed under LGPLv3.</p>");
        mainLayout_->addWidget(licenseTextBrowser_);

        licenseTextBrowser_->setVisible(false);
        connect(licenseButton_, &QPushButton::clicked, this, [=]() {
            licenseTextBrowser_->setVisible(!licenseTextBrowser_->isVisible());
        });

        centralWidget_->setLayout(mainLayout_);
        this->setCentralWidget(centralWidget_);

        connect(setButton_, &QPushButton::clicked, this, &MainWindow::setEquation);
        connect(addParameterButton_, &QPushButton::clicked, this, &MainWindow::addParameter);

        connect(xminLineEdit_, &QLineEdit::textChanged, this, [=](const QString &text) {
            xmin_ = text.toDouble();
            this->update();
        });
        connect(xmaxLineEdit_, &QLineEdit::textChanged, this, [=](const QString &text) {
            xmax_ = text.toDouble();
            this->update();
        });
        connect(yminLineEdit_, &QLineEdit::textChanged, this, [=](const QString &text) {
            ymin_ = text.toDouble();
            this->update();
        });
        connect(ymaxLineEdit_, &QLineEdit::textChanged, this, [=](const QString &text) {
            ymax_ = text.toDouble();
            this->update();
        });

        visualizer_->setEquation("x^2 - 2");
        lineEquation_->setText("x^2 - 2");
    }

void MainWindow::setEquation() {
    QString equation = lineEquation_->text();
    visualizer_->setEquation(equation);
    if(visualizer_->isValidEquation()) {
        label_error_->setText("");
        this->update();
    } else {
        QString errorMsg = visualizer_->getCommentInvalidEquation();
        std::cerr << "Error: " << errorMsg.toStdString() << std::endl;
        label_error_->setText("Error: " + errorMsg);
        return;
    }
    
}

void MainWindow::addParameter() {
    const QString paramName = QString("");
    Parameter *newParam = new Parameter(paramName, -10.0, 10.0);
    parameters_.append(newParam);
    QLineEdit *paramNameLineEdit = new QLineEdit(paramName);
    paramNameLineEdit->setMaxLength(1);
    QFontMetrics fmName(paramNameLineEdit->font());
    int textWidthName = fmName.horizontalAdvance(QString("M").repeated(1)) + 4;
    paramNameLineEdit->setFixedWidth(textWidthName);

    constexpr int minmaxLength = 4;
    QLineEdit *paramMinLineEdit  = new QLineEdit(QString::number(newParam->get_min()));
    paramMinLineEdit->setMaxLength(minmaxLength);
    QLineEdit *paramMaxLineEdit  = new QLineEdit(QString::number(newParam->get_max()));
    paramMaxLineEdit->setMaxLength(minmaxLength);

    QFontMetrics fm(paramMinLineEdit->font());
    int textWidth = fm.horizontalAdvance(QString("M").repeated(minmaxLength)) + 4;
    paramMinLineEdit->setFixedWidth(textWidth);
    paramMaxLineEdit->setFixedWidth(textWidth);

    QSlider *paramSlider = new QSlider(Qt::Horizontal);
    QPushButton *removeButton = new QPushButton("Remove");

    parametersName.append(paramNameLineEdit);
    parametersMin_.append(paramMinLineEdit);
    parametersMax_.append(paramMaxLineEdit);
    parametersSlider_.append(paramSlider);

    QHBoxLayout *paramLayout = new QHBoxLayout();
    paramLayout->addWidget(paramNameLineEdit);
    paramLayout->addWidget(paramMinLineEdit);
    paramLayout->addWidget(paramMaxLineEdit);
    paramLayout->addWidget(paramSlider);
    paramLayout->addWidget(removeButton);

    parametersLayout_->addLayout(paramLayout);

    connect(removeButton, &QPushButton::clicked, this, [=]() {
        parameters_.removeOne(newParam);
        delete newParam;
        delete paramNameLineEdit;
        delete paramMinLineEdit;
        delete paramMaxLineEdit;
        delete paramSlider;
        delete removeButton;
        parametersLayout_->removeItem(paramLayout);
        delete paramLayout;
        this->update();
    });

    connect(paramNameLineEdit, &QLineEdit::textChanged, this, [=](const QString &text) {
        newParam->set_name(text);
        this->changeParameters();
        //qDebug() << "Parameter name changed to: " << text << "," << newParam->get_name();
        this->update();
    });

    connect(paramSlider, &QSlider::valueChanged, this, &MainWindow::changeParameters);
}

void MainWindow::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event);
    visualizer_->update();
}

void MainWindow::changeParameters() {
    size_t numParameters = parameters_.size();
    parametersValue_.resize(numParameters);
    for(size_t i=0;i<numParameters;i++) {
        double minVal = parameters_[i]->get_min();
        double maxVal = parameters_[i]->get_max();
        int sliderValue = parametersSlider_[i]->value();
        double paramValue = minVal + (maxVal - minVal) * (sliderValue / 100.0);
        parametersValue_[i] = qMakePair(parameters_[i]->get_name(), paramValue);
        //qDebug() << "Parameter: " << parameters_[i]->get_name() << ", Value: " << paramValue;
    }
}