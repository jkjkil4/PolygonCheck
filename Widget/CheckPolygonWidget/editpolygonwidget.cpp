#include "editpolygonwidget.h"

EditPolygonWidget::EditPolygonWidget(QWidget *parent) : QWidget(parent)
{
    setMouseTracking(true);

    mTimerLimitUpdate->setSingleShot(true);
    connect(mTimerLimitUpdate, SIGNAL(timeout()), this, SLOT(update()));

    QPalette pal = palette();
    pal.setColor(QPalette::Window, QColor(255, 255, 255));
    setPalette(pal);
    setAutoFillBackground(true);

    setMinimumSize(120, 120);
}

void EditPolygonWidget::mousePressEvent(QMouseEvent *ev) {
    if(ev->button() == Qt::LeftButton) {
        mVecVertex << ev->pos() - trueOffset();
        emit polygonChanged(mVecVertex);
        StartTimer(mTimerLimitUpdate, 10);
    } else if(ev->button() == Qt::RightButton) {
        mPrevPos = ev->pos();
        setCursor(Qt::SizeAllCursor);
    }
}

void EditPolygonWidget::mouseMoveEvent(QMouseEvent *ev) {
    if(ev->buttons() & Qt::RightButton) {
        mOffset.rx() += ev->x() - mPrevPos.x();
        mOffset.ry() += ev->y() - mPrevPos.y();
        mPrevPos = ev->pos();
        StartTimer(mTimerLimitUpdate, 10);
    }
}

void EditPolygonWidget::mouseReleaseEvent(QMouseEvent *ev) {
    if(ev->button() == Qt::RightButton) {
        setCursor(Qt::ArrowCursor);
    }
}

void EditPolygonWidget::leaveEvent(QEvent *) {
    StartTimer(mTimerLimitUpdate, 10);
}

void EditPolygonWidget::paintEvent(QPaintEvent *) {
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);
    QPoint offset = trueOffset();

    if(!mVecVertex.isEmpty()) {
        //绘制多边形
        p.setPen(QPen(Qt::black, 2));
        if(mVecVertex.size() == 1)
            p.drawPoint(mVecVertex[0] + offset);
        else {
            QPointF prev = mVecVertex[0];
            for(int i = 1; i < mVecVertex.size(); i++) {
                QPointF cur = mVecVertex[i];
                p.drawLine(prev + offset, cur + offset);
                prev = cur;
            }

            p.setPen(QPen(QColor(160, 160, 250), 2));
            p.drawLine(*mVecVertex.begin() + offset, *mVecVertex.rbegin() + offset);
        }
    }

    //绘制原点
    p.fillRect(QRect(offset - QPoint(10, 0), QSize(21, 1)), Qt::magenta);
    p.fillRect(QRect(offset - QPoint(0, 10), QSize(1, 21)), Qt::magenta);
}

QPoint EditPolygonWidget::trueOffset() {
    return mOffset + QPoint(width() / 2, height() / 2);
}

void EditPolygonWidget::onClearVertex() {
    mVecVertex.clear();
    emit polygonChanged(mVecVertex);
    StartTimer(mTimerLimitUpdate, 10);
}
