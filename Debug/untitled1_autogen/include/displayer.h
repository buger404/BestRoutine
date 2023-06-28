
#ifndef ATTRACTIONDISPLAYER_H
#define ATTRACTIONDISPLAYER_H

#include <QWidget>
#include <QPainter>
#include <QMouseEvent>
#include <QWheelEvent>

class AttractionDisplayer : public QWidget
{
    Q_OBJECT
private:
    QImage icon = QImage("pos.png");
    QImage icon2 = QImage("pos2.png");
    QPen titlePen = QPen(qRgb(200, 200, 200));
    QPen contentPen = QPen(qRgb(140, 140, 140));
    QPen routinePen = QPen(qRgb(130, 243, 169));
    double scaleFactor = 1;
    double xPitch = 0, yPitch = 0;
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
