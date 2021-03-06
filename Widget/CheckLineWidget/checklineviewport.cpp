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

    //绘制判断线
    if(mVecIntersections.size() > 1) {
        bool isIn = true;
        double firstX = mVecIntersections[0];
        double rotatedY = mCheckPos1Rotated.y();
        QPointF prev = Rotate(QPointF(firstX, rotatedY), mRotatedRadius) + mOffset;
        for(int i = 1; i < mVecIntersections.size(); i++) {
            double curX = mVecIntersections[i];
            QPointF cur = Rotate(QPointF(curX, rotatedY), mRotatedRadius) + mOffset;
            p.setPen(QPen(isIn ? Qt::green : Qt::red, 1));
            p.drawLine(prev, cur);
            prev = cur;
            isIn = !isIn;
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
        if(mIsVertexPosVisible) {
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

    //绘制端点坐标
    if(mIsLinePosVisible) {
        p.setPen(Qt::magenta);
        QString text = "(" + QString::number(mCheckPos1.x()) + ", " + QString::number(mCheckPos1.y()) + ")";
        j::DrawText(&p, mCheckPos1 + mOffset + QPoint(2, 2), Qt::AlignLeft | Qt::AlignTop, text);
        text = "(" + QString::number(mCheckPos2.x()) + ", " + QString::number(mCheckPos2.y()) + ")";
        j::DrawText(&p, mCheckPos2 + mOffset + QPoint(2, 2), Qt::AlignLeft | Qt::AlignTop, text);
    }

    //绘制鼠标位置
    if(mMouseState == MouseState::AddPoint || mMouseState == MouseState::SetPos) {
        QPoint pos = mapFromGlobal(cursor().pos());
        if(QRect(0, 0, width(), height()).contains(pos)) {
            QString text = "(" + QString::number(pos.x() - mOffset.x()) + ", " + QString::number(pos.y() - mOffset.y()) + ")";
            if(mMouseState == MouseState::SetPos) {
                p.setPen(Qt::black);
                j::DrawText(&p, pos + QPoint(2, -2), Qt::AlignLeft | Qt::AlignBottom, "左键设置端点1\n右键设置端点2");
            }
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
    mRotatedRadius = qAtan2(mCheckPos2.y() - mCheckPos1.y(), mCheckPos2.x() - mCheckPos1.x());
    double sin = qSin(-mRotatedRadius), cos = qCos(-mRotatedRadius);
    mCheckPos1Rotated = Rotate(mCheckPos1, sin, cos);
    mCheckPos2Rotated = Rotate(mCheckPos2, sin, cos);

    mVecPointsRotated.clear();
    for(const QPointF &pos : mVecPoints)
        mVecPointsRotated << Rotate(pos, sin, cos);
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
    mIsLinePosVisible = visible;
    StartTimer(mTimerLimitUpdate, 10);
}

void CheckLineViewport::onVertexPosVisibleChanged(bool visible) {
    mIsVertexPosVisible = visible;
    StartTimer(mTimerLimitUpdate, 10);
}

void CheckLineViewport::onClearVertex() {
    mVecPoints.clear();
    mVecIntersections.clear();
    StartTimer(mTimerLimitUpdate, 10);
}

void CheckLineViewport::onGetResult() {
    getRotated();
    mVecIntersections = GetIntersections(mVecPointsRotated, mCheckPos1Rotated.y());
}
