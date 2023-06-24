
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
    QPen titlePen = QPen(qRgb(200, 210, 210));
    QPen contentPen = QPen(qRgb(140, 150, 150));
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
