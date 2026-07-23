#ifndef DERIVATIVEVISUALIZER_HPP
#define DERIVATIVEVISUALIZER_HPP

#include <QWidget>
#include <QPainter>
#include <QMouseEvent>
#include <QPainterPath>
#include "muParser.h"

class DerivativeVisualizer : public QWidget {
    Q_OBJECT // Qtの機能を使うためのマクロ

public:
    DerivativeVisualizer(const QVector<QPair<QString,double>>& parametersValue,
                        const double& xmin,
                        const double& xmax,
                        const double& ymin,
                        const double& ymax,
                        QWidget *parent = nullptr) 
        : QWidget(parent), parametersValue_(parametersValue), xmin_(xmin), xmax_(xmax), ymin_(ymin), ymax_(ymax) {
        setMouseTracking(true);
        isValidEquation_ = true;
        x1 = -1.5;
        x2 = 2.0;
        xscale_ = width() / (xmax_ - xmin_);
        yscale_ = height() / (ymax_ - ymin_);
    }

    void setEquation(const QString &equation) {
        parser_.SetExpr(equation.toStdString());
    }

    bool isValidEquation() {
        this->f(0.0); 
        return isValidEquation_;
    }

    QString getCommentInvalidEquation() const {
        return comment_inValidEquation_;
    }

protected:
    double f(double x) {
        try{
            isValidEquation_ = true;
            parser_.DefineVar("x", &x);

            for (const auto& param : parametersValue_) {
                double p = param.second;
                //qDebug() << "Parameter: " << param.first << ", Value: " << p;
                parser_.DefineVar(param.first.toStdString(), &p);

            }

            const double result = parser_.Eval();
            return result;
        }
        catch(mu::ParserError &e)
        {
            isValidEquation_ = false;
            comment_inValidEquation_ = QString::fromStdString(e.GetMsg());
            return 0.0;
        }
        return 0.0;
    }

    QPointF toPixel(double x, double y) const {
        double px = (x - xmin_) * xscale_;
        double py = (ymax_ - y) * yscale_;
        return QPointF(px, py);
    }

    double toMathX(double px) const {
        //return (px - width() / 2.0) / xscale_ + xmin_;
        return px / xscale_ + xmin_;
    }

    double toMathY(double py) const {
        //return (height() / 2.0 - py) / yscale_ + ymin_;
        return py / yscale_ + ymin_;
    }

    void paintEvent(QPaintEvent *event) override {
        Q_UNUSED(event);

        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing);

        painter.fillRect(rect(), Qt::white);

        if(!isValidEquation_) return;

        xscale_ = width() / (xmax_ - xmin_);
        yscale_ = height() / (ymax_ - ymin_);

        painter.setPen(QPen(Qt::lightGray, 1));
        painter.drawLine(0, -ymin_ * yscale_, width(), -ymin_ * yscale_);
        painter.drawLine(-xmin_ * xscale_, 0, -xmin_ * xscale_, height());
        //painter.drawLine(0, height() / 2, width(), height() / 2);
        //painter.drawLine(width() / 2, 0, width() / 2, height());

        painter.setPen(QPen(Qt::blue, 2));
        QPainterPath funcPath;
        bool first = true;
        for (int px = 0; px < width(); ++px) {
            double mx = toMathX(px);
            double my = f(mx);
            QPointF pt = toPixel(mx, my);
            
            if (first) {
                funcPath.moveTo(pt);
                first = false;
            } else {
                funcPath.lineTo(pt);
            }
        }
        painter.drawPath(funcPath);

        double y1 = f(x1);
        double y2 = f(x2);

        painter.setPen(QPen(Qt::darkGreen, 1, Qt::DashLine));
        if (std::abs(x1 - x2) > 1e-5) {
            double m = (y2 - y1) / (x2 - x1);
            double c = y1 - m * x1;

            double xLeft = toMathX(0);
            double xRight = toMathX(width());
            painter.drawLine(toPixel(xLeft, m * xLeft + c), toPixel(xRight, m * xRight + c));
        }

        QPointF p1 = toPixel(x1, y1);
        QPointF p2 = toPixel(x2, y2);

        painter.setBrush(Qt::red);
        painter.setPen(Qt::NoPen);
        painter.drawEllipse(p1, 6, 6);
        painter.drawEllipse(p2, 6, 6);

        painter.setPen(Qt::black);
        painter.drawText(p1 + QPointF(10, -10), QString("A (x,y) = (%1, %2)").arg(x1, 0, 'f', 2).arg(y1, 0, 'f', 2));
        painter.drawText(p2 + QPointF(10, -10), QString("B (x,y) = (%1, %2)").arg(x2, 0, 'f', 2).arg(y2, 0, 'f', 2));
    }

    void mousePressEvent(QMouseEvent *event) override {
        if (event->button() == Qt::LeftButton) {
            QPointF pos = event->position();
            
            double dist1 = QLineF(pos, toPixel(x1, f(x1))).length();
            double dist2 = QLineF(pos, toPixel(x2, f(x2))).length();

            if (dist1 < 15.0 && dist1 < dist2) {
                activePoint = 1;
            } else if (dist2 < 15.0) {
                activePoint = 2;
            } else {
                activePoint = 0;
            }
        }
    }

    void mouseMoveEvent(QMouseEvent *event) override {
        if (activePoint == 1) {
            x1 = toMathX(event->position().x());
            update();
        } else if (activePoint == 2) {
            x2 = toMathX(event->position().x());
            update();
        }
    }

    void mouseReleaseEvent(QMouseEvent *event) override {
        if (event->button() == Qt::LeftButton) {
            activePoint = 0;
        }
    }





private:
    double x1;
    double x2;
    int activePoint = 0;
    double xscale_;
    double yscale_;
    mu::Parser parser_;
    double x_val_;
    QString comment_inValidEquation_;
    bool isValidEquation_;
    const QVector<QPair<QString,double>>& parametersValue_;
    const double& xmin_;
    const double& xmax_;
    const double& ymin_;
    const double& ymax_;
};

#endif // DERIVATIVEVISUALIZER_HPP