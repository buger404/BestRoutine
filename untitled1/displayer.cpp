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
    x = a.getX() * 45 * scaleFactor + xPitch;
    y = a.getY() * 45 * scaleFactor + yPitch;
}

void AttractionDisplayer::paintEvent(QPaintEvent *event)
{
    QPainter p(this);

    QRect rect(0,0,width(),height());
    p.setViewport(rect);

    p.setRenderHint(QPainter::TextAntialiasing);
    p.setRenderHint(QPainter::Antialiasing);
    p.setRenderHint(QPainter::SmoothPixmapTransform);

    p.drawImage(rect, back);

    QFont font = this->font();
    font.setPointSize(10.0);
    p.setFont(font);

    QFontMetrics textSize(font);

    Routine best = Routine::CurrentResult;

    routinePen.setStyle(Qt::PenStyle::SolidLine);
    routinePen.setCapStyle(Qt::RoundCap);
    routinePen.setWidth(2);

    p.setPen(linePen);
    int xMin = floor(-xPitch / scaleFactor / 45);
    int xMax = ceil((rect.width() - xPitch) / scaleFactor / 45);
    int yMin = floor(-yPitch / scaleFactor / 45);
    int yMax = ceil((rect.height() - yPitch) / scaleFactor / 45);

    for(int i = xMin;i <= xMax;i++){
        double x = i * 45 * scaleFactor + xPitch;
        p.drawLine(QPoint(x + 1, 0), QPoint(x + 1, rect.height()));
    }
    for(int j = yMin;j <= yMax;j++){
        double y = j * 45 * scaleFactor + yPitch;
        p.drawLine(QPoint(0, y), QPoint(rect.width(), y));
    }

    if (!best.Data.empty() && best.Data.size() > 0){
        p.setPen(routinePen);
        for(int i = 0;i < best.Data.size() - 1;i++){
            double x1, y1, x2, y2;
            calculateDisplayPosition(Routine::Attractions[best.Data[i]], x1, y1);
            calculateDisplayPosition(Routine::Attractions[best.Data[i + 1]], x2, y2);
            p.drawLine(QPoint(x1, y1), QPoint(x2, y2));
        }
    }

    QString mark[] = {"起点", "终点"};

    for(auto &a:Routine::Attractions){
        int status = 0;
        if (Attraction::Start == &a)
            status = 1;
        else if (Attraction::End == &a)
            status = 2;
        bool light = status > 0 || Attraction::Selected == &a;
        double x, y;
        calculateDisplayPosition(a, x, y);
        bool inRoutine = false;
        if (!best.Data.empty() && best.Data.size() > 0){
            for(int i = 0;i < best.Data.size();i++){
                if (&Routine::Attractions[best.Data[i]] == &a){
                    inRoutine = true;
                    break;
                }
            }
        }
        p.setPen(light || inRoutine ? routinePen : titlePen);
        p.setBrush(light ? lightDot : normalDot);
        p.drawEllipse(QRect(x - 8, y - 8, 16, 16));
        if (Attraction::Hover == &a || light)
            p.drawImage(QRect(x - 16, y - 32 - 15, 32, 32), light ? icon : icon2);
    }

    for(auto &a:Routine::Attractions){
        int status = 0;
        if (Attraction::Start == &a)
            status = 1;
        else if (Attraction::End == &a)
            status = 2;
        bool light = status > 0 || Attraction::Selected == &a;
        double x, y;
        calculateDisplayPosition(a, x, y);
        bool inRoutine = false;
        if (!best.Data.empty() && best.Data.size() > 0){
            for(int i = 0;i < best.Data.size();i++){
                if (&Routine::Attractions[best.Data[i]] == &a){
                    inRoutine = true;
                    break;
                }
            }
        }
        QString str = QString::fromStdString(a.getName());
        QRect r = textSize.boundingRect(str);
        p.setPen(light ? routinePen : titlePen);
        p.drawText(x - r.width() / 2, y - r.height() / 2 + 30, str);

        if (Attraction::Hover == &a || inRoutine){
            QString str2 = QString::asprintf("%02d:%02d~%02d:%02d",
                                             a.getOpenTime().getHour(), a.getOpenTime().getMinute(),
                                             a.getCloseTime().getHour(), a.getCloseTime().getMinute());
            QRect r3 = textSize.boundingRect(str2);
            p.setPen(status == 0 ? titlePen : routinePen);
            p.drawText(x - r3.width() / 2, y - r3.height() / 2 + 30 + 17, str2);
            continue;
        }

        if (status == 0)
            continue;
        QRect r2 = textSize.boundingRect(mark[status - 1]);
        p.setPen(routinePen);
        p.drawText(x - r2.width() / 2, y - r2.height() / 2 + 30 + 17, mark[status - 1]);
    }


    if (!best.Data.empty() && best.Data.size() > 0){
        for(int i = 0;i < best.Data.size();i++){
            auto &a = Routine::Attractions[best.Data[i]];
            double x, y;
            calculateDisplayPosition(a, x, y);
            QString str = QString::asprintf("%d", i + 1);
            QRect r = textSize.boundingRect(str);
            p.setPen(numPen);
            p.drawText(x - r.width() / 2 - 1, y - r.height() / 2 + 13, str);
        }
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
    auto pos = event->pos();
    if (pos.x() != startPos.x() || pos.y() != startPos.y())
        return;
    for(auto &a:Routine::Attractions){
        double x, y;
        calculateDisplayPosition(a, x, y);
        if (abs(pos.x() - x) <= 16 && abs(pos.y() - y) <= 16){
            Attraction::Selected = &a;
            break;
        }
    }
    this->repaint();
}

void AttractionDisplayer::mouseMoveEvent(QMouseEvent *event){
    QPoint pos = event->pos();
    Attraction* h = nullptr;
    for(auto &a:Routine::Attractions){
        double x, y;
        calculateDisplayPosition(a, x, y);
        if (abs(pos.x() - x) <= 16 && abs(pos.y() - y) <= 16){
            h = &a;
            break;
        }
    }
    if (Attraction::Hover != h){
        Attraction::Hover = h;
        repaint();
    }
    if (!moving)
        return;
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
