#include "viewport.h"

Viewport::Viewport()
{
    setMouseTracking(true);

    mTimerLimitUpdate->setSingleShot(true);
    connect(mTimerLimitUpdate, SIGNAL(timeout()), this, SLOT(update()));

    QPalette pal = palette();
    pal.setColor(QPalette::Window, QColor(255, 255, 255));
    setPalette(pal);
    setAutoFillBackground(true);
}

void Viewport::mousePressEvent(QMouseEvent *ev) {
    if(ev->button() == Qt::LeftButton) {
        switch(mMouseState) {
        case MouseState::AddPoint:
            mVecPoints << ev->pos() - mOffset;
            mVecIntersections = getIntersections(mCheckPos.y());
            startTimer(mTimerLimitUpdate, 16);
            break;
        case MouseState::Move:
            mPrevPos = ev->pos();
            break;
        case MouseState::SetPos:
            setPosByMouse(ev->pos());
            break;
        default:;
        }
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
    case MouseState::SetPos:
        if(ev->buttons() & Qt::LeftButton)
            setPosByMouse(ev->pos());
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
    if(mVecIntersections.size() > 1) {
        bool isIn = true;
        double prev = mVecIntersections[0];
        for(int i = 1; i < mVecIntersections.size(); i++) {
            double cur = mVecIntersections[i];

            p.fillRect((int)prev + mOffset.x(), mCheckPos.y() + mOffset.y(), (int)cur - (int)prev, 2, isIn ? Qt::green : Qt::red);

            isIn = !isIn;
            prev = cur;
        }
    }

    if(!mVecPoints.isEmpty()) {
        //绘制多边形
        p.setPen(QPen(Qt::black, 2));
        if(mVecPoints.size() == 1)
            p.drawPoint(mVecPoints[0] + mOffset);
        else {
            QPointF prev = mVecPoints[0];
            for(int i = 1; i < mVecPoints.size(); i++) {
                QPointF cur = mVecPoints[i];
                p.drawLine(prev + mOffset, cur + mOffset);
                prev = cur;
            }

            if(mMouseState == MouseState::AddPoint)
                p.setPen(QPen(QColor(160, 160, 250), 2));
            p.drawLine(*mVecPoints.begin() + mOffset, *mVecPoints.rbegin() + mOffset);
        }

        //绘制顶点坐标
        if(isVertexPosVisible) {
            p.setPen(Qt::black);
            for(QPointF &pos : mVecPoints) {
                QString text = "(" + QString::number(pos.x()) + ", " + QString::number(pos.y()) + ")";
                j::DrawText(&p, (int)pos.x() + mOffset.x() + 1, (int)pos.y() + mOffset.y() + 1, Qt::AlignLeft | Qt::AlignTop, text);
            }
        }
    }
}

void Viewport::startTimer(QTimer *pTimer, int msec) {
    if(!pTimer->isActive())
        pTimer->start(msec);
}

void Viewport::setPosByMouse(QPoint pos) {
    pos -= mOffset;
    bool isXChanged = pos.x() != mCheckPos.x();
    bool isYChanged = pos.y() != mCheckPos.y();
    if(isXChanged || isYChanged) {
        mCheckPos = pos;
        if(isXChanged) emit xChanged(pos.x());
        if(isYChanged) {
            mVecIntersections = getIntersections(pos.y());
            emit yChanged(pos.y());
        }
        startTimer(mTimerLimitUpdate, 16);
    }
}

#define FIX_INTERSECTION_AT_VERTEX
QVector<double> Viewport::getIntersections(double y) {
#ifdef FIX_INTERSECTION_AT_VERTEX
    QVector<double> vecIntersections;
    if(mVecPoints.size() < 2)
        return vecIntersections;

    //检测是否全部都为水平线
    double tmpY = (*mVecPoints.begin()).y();
    bool isAllHor = true;
    for(int i = 1; i < mVecPoints.size(); i++) {
        if(mVecPoints[i].y() != tmpY) {
            isAllHor = false;
            break;
        }
    }
    if(isAllHor)
        return vecIntersections;

    //从末尾开始遍历得到第一个不为水平的线的 走向 和 交点情况
    bool prevTrend = false;
    bool prevHasIntersection = false;
    QPointF prev = *mVecPoints.rbegin();
    for(auto iter = mVecPoints.rbegin() + 1; iter != mVecPoints.rend(); ++iter) {
        const QPointF &cur = *iter;
        if(prev.y() != cur.y()) {
            prevTrend = cur.y() < prev.y(); //因为是反向遍历，所以是小于（和后面的大于相反）
            prevHasIntersection = (y >= qMin(prev.y(), cur.y()) && y <= qMax(prev.y(), cur.y()));
            break;
        }
        prev = cur;
    }

    //计算交点
    prev = *mVecPoints.rbegin();
    for(const QPointF &cur : mVecPoints) {
        bool hasIntersection = false;
        if(prev.y() != cur.y()) {
            bool trend = cur.y() > prev.y();
            if(trend != prevTrend || !prevHasIntersection) {
                if(y >= qMin(prev.y(), cur.y()) && y <= qMax(prev.y(), cur.y())) {
                    hasIntersection = true;
                    vecIntersections << prev.x() + (cur.x() - prev.x()) * (y - prev.y()) / (cur.y() - prev.y());
                }
            }
            if(trend != prevTrend)
                prevTrend = trend;
        }
        prevHasIntersection = hasIntersection;
        prev = cur;
    }

    std::sort(vecIntersections.begin(), vecIntersections.end());

    return vecIntersections;
#else
    QVector<double> vecIntersections;
    if(mVecPoints.size() < 2)
        return vecIntersections;

    //计算交点
    QPointF prev = *mVecPoints.rbegin();
    for(const QPointF &cur : mVecPoints) {
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
        mVecIntersections = getIntersections(y);
        startTimer(mTimerLimitUpdate, 16);
    }
}

void Viewport::onVertexPosVisibleChanged(bool visible) {
    isVertexPosVisible = visible;
    startTimer(mTimerLimitUpdate, 16);
}

void Viewport::onClearVertex() {
    mVecPoints.clear();
    mVecIntersections.clear();
    startTimer(mTimerLimitUpdate, 16);
}
