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
        vecIntersections = getIntersections(mCheckPos.y());
        startTimer(mTimerLimitUpdate, 16);
        break;
    case MouseState::Move:
        if(ev->button() == Qt::LeftButton)
            mPrevPos = ev->pos();
        break;
    case MouseState::SetPos: {
        QPoint pos = ev->pos() - mOffset;
        bool isXChanged = pos.x() != mCheckPos.x();
        bool isYChanged = pos.y() != mCheckPos.y();
        if(isXChanged || isYChanged) {
            mCheckPos = pos;
            if(isXChanged) emit xChanged(pos.x());
            if(isYChanged) {
                vecIntersections = getIntersections(pos.y());
                emit yChanged(pos.y());
            }
            startTimer(mTimerLimitUpdate, 16);
        }
        break;
    }
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

    //绘制检测线
    if(vecIntersections.size() > 1) {
        bool isIn = true;
        double prev = vecIntersections[0];
        for(int i = 1; i < vecIntersections.size(); i++) {
            double cur = vecIntersections[i];

            p.fillRect((int)prev + mOffset.x(), mCheckPos.y() + mOffset.y(), (int)cur - (int)prev, 2, isIn ? Qt::green : Qt::red);

            isIn = !isIn;
            prev = cur;
        }
    }

    if(!vecPoints.isEmpty()) {
        //绘制多边形
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

        //绘制顶点坐标
        if(isVertexPosVisible) {
            p.setPen(Qt::black);
            for(QPointF &pos : vecPoints) {
                QString text = "(" + QString::number(pos.x()) + ", " + QString::number(pos.y()) + ")";
                j::DrawText(&p, (int)pos.x() + mOffset.x() + 3, (int)pos.y() + mOffset.y() + 3, Qt::AlignLeft | Qt::AlignTop, text);
            }
        }
    }
}

void Viewport::startTimer(QTimer *pTimer, int msec) {
    if(!pTimer->isActive())
        pTimer->start(msec);
}

//#define FIX_INTERSECTION_AT_VERTEX
QVector<double> Viewport::getIntersections(double y) {
#ifdef FIX_INTERSECTION_AT_VERTEX

#else
    QVector<double> vecIntersections;
    if(vecPoints.size() < 2)
        return vecIntersections;

    QPointF prev = *vecPoints.rbegin();
    for(const QPointF &cur : vecPoints) {
        if(prev.y() != cur.y() && y >= qMin(prev.y(), cur.y()) && y <= qMax(prev.y(), cur.y())) {
            vecIntersections << prev.x() + (cur.x() - prev.x()) * (y - prev.y()) / (cur.y() - prev.y());
        }
        prev = cur;
    }

    std::sort(vecIntersections.begin(), vecIntersections.end());

    return vecIntersections;
#endif
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
    case MouseState::SetPos:
        setCursor(Qt::CrossCursor);
        break;
    }
    startTimer(mTimerLimitUpdate, 16);
}

void Viewport::onXChanged(int x) {
    if(x != mCheckPos.x()) {
        mCheckPos.setX(x);
        startTimer(mTimerLimitUpdate, 16);
    }
}

void Viewport::onYChanged(int y) {
    if(y != mCheckPos.y()) {
        mCheckPos.setY(y);
        vecIntersections = getIntersections(y);
        startTimer(mTimerLimitUpdate, 16);
    }
}

void Viewport::onVertexPosVisibleChanged(bool visible) {
    isVertexPosVisible = visible;
    startTimer(mTimerLimitUpdate, 16);
}
