
#ifndef ATTRACTIONDISPLAYER_H
#define ATTRACTIONDISPLAYER_H

#include <QWidget>
#include <QPainter>
#include <QMouseEvent>
#include <QWheelEvent>
#include "hw03.h"

class AttractionDisplayer : public QWidget
{
    Q_OBJECT
private:
    QImage back = QImage("background.jpg");
    QImage icon = QImage("pos.png");
    QImage icon2 = QImage("pos2.png");
    QPen numPen = QPen(qRgb(255, 255, 255));
    QPen titlePen = QPen(qRgb(200, 200, 200));
    QPen contentPen = QPen(qRgb(140, 140, 140));
    QPen routinePen = QPen(qRgb(130, 243, 169));
    QPen linePen = QPen(qRgb(90, 90, 90));
    QBrush normalDot = QBrush(qRgb(90, 90, 90));
    QBrush lightDot = QBrush(qRgb(130, 243, 169));
    double scaleFactor = 1;
    double xPitch = 0, yPitch = 0;
    void calculateDisplayPosition(Attraction& a, double& x, double &y);
public:
    explicit AttractionDisplayer(QWidget *parent = nullptr);
protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;
signals:

};

#endif // ATTRACTIONDISPLAYER_H
