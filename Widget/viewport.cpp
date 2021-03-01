#include "viewport.h"

Viewport::Viewport()
{
    setMouseTracking(true);

    mTimerLimitUpdate->setSingleShot(true);
    connect(mTimerLimitUpdate, SIGNAL(timeout()), this, SLOT(update()));
}

void Viewport::mousePressEvent(QMouseEvent *ev) {
    switch(mMouseState) {
    case MouseState::AddPoint:
        vecPoints << ev->pos() - mOffset;
        startTimer(mTimerLimitUpdate, 16);
        break;
    case MouseState::Move:
        if(ev->button() == Qt::LeftButton)
            mPrevPos = ev->pos();
        break;
    default:;
    }
}

void Viewport::mouseMoveEvent(QMouseEvent *ev) {
    switch(mMouseState) {
    case MouseState::AddPoint:
        startTimer(mTimerLimitUpdate, 16);
        break;
    case MouseState::Move:
        if(ev->buttons() & Qt::LeftButton) {
            mOffset.rx() += ev->pos().x() - mPrevPos.x();
            mOffset.ry() += ev->pos().y() - mPrevPos.y();
            mPrevPos = ev->pos();
            startTimer(mTimerLimitUpdate, 16);
        }
        break;
    default:;
    }
}

void Viewport::leaveEvent(QEvent *) {
    if(mMouseState == MouseState::AddPoint)
        startTimer(mTimerLimitUpdate, 16);
}

void Viewport::paintEvent(QPaintEvent *) {
    QPainter p(this);
    p.setRenderHint(QPainter::RenderHint::Antialiasing);

    if(!vecPoints.isEmpty()) {
        p.setPen(QPen(Qt::black, 2));
        if(vecPoints.size() == 1)
            p.drawPoint(vecPoints[0] + mOffset);
        else {
            QPointF prev = vecPoints[0];
            for(int i = 1; i < vecPoints.size(); i++) {
                QPointF cur = vecPoints[i];
                p.drawLine(prev + mOffset, cur + mOffset);
                prev = cur;
            }

            if(mMouseState == MouseState::AddPoint)
                p.setPen(QPen(QColor(160, 160, 250), 2));
            p.drawLine(*vecPoints.begin() + mOffset, *vecPoints.rbegin() + mOffset);
        }
    }
}

void Viewport::startTimer(QTimer *pTimer, int msec) {
    if(!pTimer->isActive())
        pTimer->start(msec);
}

void Viewport::onMouseStateChanged(MouseState ms) {
    mMouseState = ms;
    switch(mMouseState) {
    case MouseState::Cursor:
        setCursor(Qt::ArrowCursor);
        break;
    case MouseState::AddPoint:
        setCursor(Qt::CrossCursor);
        break;
    case MouseState::Move:
        setCursor(Qt::SizeAllCursor);
        break;
    }
    update();
}
