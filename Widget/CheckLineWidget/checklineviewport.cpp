#include "checklineviewport.h"

//double CheckLineViewport::pi = qAcos(-1);

CheckLineViewport::CheckLineViewport(QWidget *parent) : QWidget(parent)
{
    setMouseTracking(true);

    mTimerLimitGetResult->setSingleShot(true);
    connect(mTimerLimitGetResult, SIGNAL(timeout()), this, SLOT(onGetResult()));
    mTimerLimitUpdate->setSingleShot(true);
    connect(mTimerLimitUpdate, SIGNAL(timeout()), this, SLOT(update()));

    QPalette pal = palette();
    pal.setColor(QPalette::Window, QColor(255, 255, 255));
    setPalette(pal);
    setAutoFillBackground(true);
}

void CheckLineViewport::mousePressEvent(QMouseEvent *ev) {
    switch(mMouseState) {
    case MouseState::AddPoint:
        if(ev->button() == Qt::LeftButton) {
            mVecPoints << ev->pos() - mOffset;
            StartTimer(mTimerLimitGetResult, 8);
            StartTimer(mTimerLimitUpdate, 10);
        }
        break;
    case MouseState::Move:
        if(ev->button() == Qt::LeftButton) {
            mPrevPos = ev->pos();
        }
        break;
    case MouseState::SetPos:
        if(ev->button() == Qt::LeftButton || ev->button() == Qt::RightButton) {
            if(ev->button() == Qt::LeftButton) {
                setPosByMouse(mCheckPos1, ev->pos(), this, &CheckLineViewport::x1Changed, &CheckLineViewport::y1Changed);
            } else if(ev->button() == Qt::RightButton) {
                setPosByMouse(mCheckPos2, ev->pos(), this, &CheckLineViewport::x2Changed, &CheckLineViewport::y2Changed);
            }
            StartTimer(mTimerLimitGetResult, 8);
            StartTimer(mTimerLimitUpdate, 10);
        }
        break;
    default:;
    }
}

void CheckLineViewport::mouseMoveEvent(QMouseEvent *ev) {
    switch(mMouseState) {
    case MouseState::AddPoint:
        StartTimer(mTimerLimitUpdate, 10);
        break;
    case MouseState::Move:
        if(ev->buttons() & Qt::LeftButton) {
            mOffset.rx() += ev->pos().x() - mPrevPos.x();
            mOffset.ry() += ev->pos().y() - mPrevPos.y();
            mPrevPos = ev->pos();
            StartTimer(mTimerLimitUpdate, 10);
        }
        break;
    case MouseState::SetPos:
        if(ev->buttons() & Qt::LeftButton || ev->buttons() & Qt::RightButton) {
            if(ev->buttons() & Qt::LeftButton)
                setPosByMouse(mCheckPos1, ev->pos(), this, &CheckLineViewport::x1Changed, &CheckLineViewport::y1Changed);
            if(ev->buttons() & Qt::RightButton)
                setPosByMouse(mCheckPos2, ev->pos(), this, &CheckLineViewport::x2Changed, &CheckLineViewport::y2Changed);
            StartTimer(mTimerLimitGetResult, 8);
        }
        StartTimer(mTimerLimitUpdate, 10);
        break;
    default:;
    }
}

void CheckLineViewport::leaveEvent(QEvent *) {
    if(mMouseState == MouseState::AddPoint || mMouseState == MouseState::SetPos)
        StartTimer(mTimerLimitUpdate, 10);
}

void CheckLineViewport::paintEvent(QPaintEvent *) {
    QPainter p(this);
    p.setRenderHint(QPainter::RenderHint::Antialiasing);

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

    //绘制检测线
    p.setPen(QPen(isCheckLineInside() ? QColor(0, 190, 0) : QColor(190, 0, 0), 4));
    p.drawLine(mCheckPos1 + mOffset, mCheckPos2 + mOffset);

    //绘制鼠标位置
    if(mMouseState == MouseState::AddPoint || mMouseState == MouseState::SetPos) {
        QPoint pos = mapFromGlobal(cursor().pos());
        if(QRect(0, 0, width(), height()).contains(pos)) {
            QString text = "(" + QString::number(pos.x() - mOffset.x()) + ", " + QString::number(pos.y() - mOffset.y()) + ")";
            p.setPen(Qt::black);
            j::DrawText(&p, pos + QPoint(2, -2), Qt::AlignLeft | Qt::AlignBottom, "左键设置端点1\n右键设置端点2");
            p.setPen(Qt::blue);
            j::DrawText(&p, pos + QPoint(2, 2), Qt::AlignLeft | Qt::AlignTop, text);
        }
    }
}

void CheckLineViewport::closeEvent(QCloseEvent *) {
    mTimerLimitGetResult->stop();
    mTimerLimitUpdate->stop();
}

void CheckLineViewport::setPosByMouse(QPoint &rPoint, QPoint pos, CheckLineViewport *obj, SignalFn xSignal, SignalFn ySignal) {
    pos -= mOffset;
    bool isXChanged = pos.x() != rPoint.x();
    bool isYChanged = pos.y() != rPoint.y();
    if(isXChanged || isYChanged) {
        rPoint = pos;
        if(isXChanged) emit (obj->*xSignal)(pos.x());
        if(isYChanged) emit (obj->*ySignal)(pos.y());
        StartTimer(mTimerLimitGetResult, 8);
        StartTimer(mTimerLimitUpdate, 10);
    }
}

void CheckLineViewport::getRotated() {
    rotatedRadius = qAtan2(mCheckPos2.y() - mCheckPos1.y(), mCheckPos2.x() - mCheckPos1.x());
    double sin = qSin(-rotatedRadius);
    double cos = qCos(-rotatedRadius);
    mCheckPos1Rotated.setX(mCheckPos1.x() * cos - mCheckPos1.y() * sin);
    mCheckPos1Rotated.setY(mCheckPos1.x() * sin + mCheckPos1.y() * cos);
    mCheckPos2Rotated.setX(mCheckPos2.x() * cos - mCheckPos2.y() * sin);
    mCheckPos2Rotated.setY(mCheckPos2.x() * sin + mCheckPos2.y() * cos);

    mVecPointsRotated.clear();
    for(const QPointF &pos : mVecPoints) {
        double x = pos.x() * cos - pos.y() * sin;
        double y = pos.x() * sin + pos.y() * cos;
        mVecPointsRotated << QPointF(x, y);
    }
}

void CheckLineViewport::getIntersections() {
    mVecIntersections.clear();
    if(mVecPoints.size() < 2)
        return;

    //检测是否全部都为水平线
    double tmpY = (*mVecPointsRotated.begin()).y();
    bool isAllHor = true;
    for(int i = 1; i < mVecPointsRotated.size(); i++) {
        if(mVecPointsRotated[i].y() != tmpY) {
            isAllHor = false;
            break;
        }
    }
    if(isAllHor)
        return;

    double y = mCheckPos1Rotated.y();

    //从末尾开始遍历得到第一个不为水平的线的 走向 和 交点情况
    bool prevTrend = false;
    bool prevHasIntersection = false;
    QPointF prev = *mVecPointsRotated.rbegin();
    for(auto iter = mVecPointsRotated.rbegin() + 1; iter != mVecPointsRotated.rend(); ++iter) {
        const QPointF &cur = *iter;
        if(prev.y() != cur.y()) {
            prevTrend = cur.y() < prev.y(); //因为是反向遍历，所以是小于（和后面的大于相反）
            prevHasIntersection = (y >= qMin(prev.y(), cur.y()) && y <= qMax(prev.y(), cur.y()));
            break;
        }
        prev = cur;
    }

    //计算交点
    prev = *mVecPointsRotated.rbegin();
    for(const QPointF &cur : mVecPointsRotated) {
        bool hasIntersection = false;
        if(prev.y() != cur.y()) {
            bool trend = cur.y() > prev.y();
            if(trend != prevTrend || !prevHasIntersection) {
                if(y >= qMin(prev.y(), cur.y()) && y <= qMax(prev.y(), cur.y())) {
                    hasIntersection = true;
                    mVecIntersections << prev.x() + (cur.x() - prev.x()) * (y - prev.y()) / (cur.y() - prev.y());
                }
            }
            if(trend != prevTrend)
                prevTrend = trend;
        }
        prevHasIntersection = hasIntersection;
        prev = cur;
    }

    std::sort(mVecIntersections.begin(), mVecIntersections.end());
}

bool CheckLineViewport::isCheckLineInside() {
    if(mVecPoints.isEmpty())
        return false;
    double checkX = mCheckPos1Rotated.x();
    bool isInside = false;
    for(double x : mVecIntersections) {
        if(x > checkX)
            return isInside && mCheckPos2Rotated.x() < x;
        isInside = !isInside;
    }
    return false;
}

void CheckLineViewport::onMouseStateChanged(MouseState ms) {
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
        setCursor(Qt::ArrowCursor);
        break;
    }
    StartTimer(mTimerLimitUpdate, 10);
}

void CheckLineViewport::onX1Changed(int x) {
    if(x != mCheckPos1.x()) {
        mCheckPos1.setX(x);
        StartTimer(mTimerLimitGetResult, 8);
        StartTimer(mTimerLimitUpdate, 10);
    }
}

void CheckLineViewport::onY1Changed(int y) {
    if(y != mCheckPos1.y()) {
        mCheckPos1.setY(y);
        StartTimer(mTimerLimitGetResult, 8);
        StartTimer(mTimerLimitUpdate, 10);
    }
}

void CheckLineViewport::onX2Changed(int x) {
    if(x != mCheckPos2.x()) {
        mCheckPos2.setX(x);
        StartTimer(mTimerLimitGetResult, 8);
        StartTimer(mTimerLimitUpdate, 10);
    }
}

void CheckLineViewport::onY2Changed(int y) {
    if(y != mCheckPos2.y()) {
        mCheckPos2.setY(y);
        StartTimer(mTimerLimitGetResult, 8);
        StartTimer(mTimerLimitUpdate, 10);
    }
}

void CheckLineViewport::onLinePosVisibleChanged(bool visible) {
    isLinePosVisible = visible;
    StartTimer(mTimerLimitUpdate, 10);
}

void CheckLineViewport::onVertexPosVisibleChanged(bool visible) {
    isVertexPosVisible = visible;
    StartTimer(mTimerLimitUpdate, 10);
}

void CheckLineViewport::onClearVertex() {
    mVecPoints.clear();
    mVecIntersections.clear();
    StartTimer(mTimerLimitUpdate, 10);
}

void CheckLineViewport::onGetResult() {
    getRotated();
    getIntersections();
}
