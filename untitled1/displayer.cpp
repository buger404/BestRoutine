#include "hw03.h"
#include "displayer.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"

AttractionDisplayer::AttractionDisplayer(QWidget *parent)
    : QWidget{parent}
{
    setMouseTracking(true);
}

void AttractionDisplayer::calculateDisplayPosition(Attraction& a, double& x, double &y){
    x = a.getX() * 30 * scaleFactor + xPitch;
    y = a.getY() * 30 * scaleFactor + yPitch;
}

void AttractionDisplayer::paintEvent(QPaintEvent *event)
{
    QPainter p(this);

    QRect rect(0,0,width(),height());
    p.setViewport(rect);

    p.setRenderHint(QPainter::TextAntialiasing);
    p.setRenderHint(QPainter::Antialiasing);
    p.setRenderHint(QPainter::SmoothPixmapTransform);

    p.setBrush(QBrush(qRgb(64, 64, 64)));
    p.drawRect(rect);

    QFont font = this->font();
    font.setPointSize(12.0 * scaleFactor);
    p.setFont(font);

    QFontMetrics textSize(font);

    Routine best = Routine::CurrentResult;

    routinePen.setStyle(Qt::PenStyle::SolidLine);
    routinePen.setCapStyle(Qt::RoundCap);
    routinePen.setWidth(2);

    if (!best.Data.empty() && best.Data.size() > 0){
        p.setPen(routinePen);
        for(int i = 0;i < best.Data.size() - 1;i++){
            double x1, y1, x2, y2;
            calculateDisplayPosition(Routine::Attractions[best.Data[i]], x1, y1);
            calculateDisplayPosition(Routine::Attractions[best.Data[i + 1]], x2, y2);
            p.drawLine(QPoint(x1, y1), QPoint(x2, y2));
        }
        /**cout << "(" << best.QualityCoefficient() << "、" << best.TotalScore() << "、" <<
            best.MaxMoney - best.RemainingMoney << "、" << best.MaxEnergy - best.RemainingEnergy << ")" << endl;**/
    }

    for(auto &a:Routine::Attractions){
        double x, y;
        calculateDisplayPosition(a, x, y);
        p.drawImage(QRect(x - 16 * scaleFactor, y - 16 * scaleFactor, 32 * scaleFactor, 32 * scaleFactor), icon);
        QString str = QString::fromStdString(a.getName());
        QRect r = textSize.boundingRect(str);
        p.setPen(titlePen);
        p.drawText(x - r.width() / 2, y - r.height() / 2 + 40 * scaleFactor, str);
        QString str2 = QString::asprintf("(%g,%g)", a.getX(), a.getY());
        QRect r2 = textSize.boundingRect(str2);
        p.setPen(contentPen);
        p.drawText(x - r2.width() / 2, y - r2.height() / 2 + 57 * scaleFactor, str2);
    }
}

double orPX, orPY;
QPoint startPos;
bool moving = false;

void AttractionDisplayer::mousePressEvent(QMouseEvent *event){
    startPos = event->pos();
    orPX = xPitch; orPY = yPitch;
    moving = true;
}

void AttractionDisplayer::mouseReleaseEvent(QMouseEvent *event){
    AttractionDisplayer::mouseMoveEvent(event);
    moving = false;
}

void AttractionDisplayer::mouseMoveEvent(QMouseEvent *event){
    if (!moving)
        return;
    QPoint pos = event->pos();
    xPitch = orPX + (pos.x() - startPos.x());
    yPitch = orPY + (pos.y() - startPos.y());
    repaint();
}

void AttractionDisplayer::wheelEvent(QWheelEvent *event){
    double factor = scaleFactor;
    double delta = event->angleDelta().y() > 0 ? 1.1 : 0.9;
    factor *= delta;
    if (factor < 0.5)
        factor = 0.5;
    xPitch -= (factor - scaleFactor) * (event->position().x() * 1.0 - xPitch) / scaleFactor;
    yPitch -= (factor - scaleFactor) * (event->position().y() * 1.0 - yPitch) / scaleFactor;
    scaleFactor = factor;
    repaint();
    MainWindow::UI->scaleTip->setText(QString::asprintf("缩放比例:  %.2f%%", scaleFactor * 100));
}
