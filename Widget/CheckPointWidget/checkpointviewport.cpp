#include "checkpointviewport.h"

CheckPointViewport::CheckPointViewport(QWidget *parent) : QWidget(parent)
{
    setMouseTracking(true);

    mTimerLimitUpdate->setSingleShot(true);
    connect(mTimerLimitUpdate, SIGNAL(timeout()), this, SLOT(update()));

    QPalette pal = palette();
    pal.setColor(QPalette::Window, QColor(255, 255, 255));
    setPalette(pal);
    setAutoFillBackground(true);
}

void CheckPointViewport::mousePressEvent(QMouseEvent *ev) {
    if(ev->button() == Qt::LeftButton) {
        switch(mMouseState) {
        case MouseState::AddPoint:
            mVecPoints << ev->pos() - mOffset;
            mVecIntersections = GetIntersections(mVecPoints, mCheckPos.y());
            StartTimer(mTimerLimitUpdate, 10);
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

void CheckPointViewport::mouseMoveEvent(QMouseEvent *ev) {
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
        if(ev->buttons() & Qt::LeftButton)
            setPosByMouse(ev->pos());
        StartTimer(mTimerLimitUpdate, 10);
        break;
    default:;
    }
}

void CheckPointViewport::leaveEvent(QEvent *) {
    if(mMouseState == MouseState::AddPoint || mMouseState == MouseState::SetPos)
        StartTimer(mTimerLimitUpdate, 10);
}

void CheckPointViewport::paintEvent(QPaintEvent *) {
    QPainter p(this);
    p.setRenderHint(QPainter::RenderHint::Antialiasing);

    //绘制判断线
    if(mVecIntersections.size() > 1) {
        bool isIn = true;
        double prev = mVecIntersections[0];
        for(int i = 1; i < mVecIntersections.size(); i++) {
            double cur = mVecIntersections[i];

            p.fillRect(QRectF(prev + mOffset.x(), mCheckPos.y() + mOffset.y(), cur - prev, 2), isIn ? Qt::green : Qt::red);

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
        if(mIsVertexPosVisible) {
            p.setPen(Qt::black);
            for(QPointF &pos : mVecPoints) {
                QString text = "(" + QString::number(pos.x()) + ", " + QString::number(pos.y()) + ")";
                j::DrawText(&p, (int)pos.x() + mOffset.x() + 1, (int)pos.y() + mOffset.y() + 1, Qt::AlignLeft | Qt::AlignTop, text);
            }
        }
    }

    //绘制检测点
    p.setPen(QPen(isCheckPosInside() ? QColor(0, 190, 0) : QColor(190, 0, 0), 4));
    QPoint offsetedCheckPos = mCheckPos + mOffset;
    p.drawLine(offsetedCheckPos + QPoint(-4, -4), offsetedCheckPos + QPoint(4, 4));
    p.drawLine(offsetedCheckPos + QPoint(4, -4), offsetedCheckPos + QPoint(-4, 4));

    //绘制鼠标位置
    if(mMouseState == MouseState::AddPoint || mMouseState == MouseState::SetPos) {
        QPoint pos = mapFromGlobal(cursor().pos());
        if(QRect(0, 0, width(), height()).contains(pos)) {
            QString text = "(" + QString::number(pos.x() - mOffset.x()) + ", " + QString::number(pos.y() - mOffset.y()) + ")";
            p.setPen(Qt::blue);
            j::DrawText(&p, pos + QPoint(2, 2), Qt::AlignLeft | Qt::AlignTop, text);
        }
    }
}

void CheckPointViewport::closeEvent(QCloseEvent *) {
    mTimerLimitUpdate->stop();
}

void CheckPointViewport::setPosByMouse(QPoint pos) {
    pos -= mOffset;
    bool isXChanged = pos.x() != mCheckPos.x();
    bool isYChanged = pos.y() != mCheckPos.y();
    if(isXChanged || isYChanged) {
        mCheckPos = pos;
        if(isXChanged) emit xChanged(pos.x());
        if(isYChanged) {
            mVecIntersections = GetIntersections(mVecPoints, pos.y());
            emit yChanged(pos.y());
        }
        StartTimer(mTimerLimitUpdate, 10);
    }
}

bool CheckPointViewport::isCheckPosInside() {
    if(mVecPoints.isEmpty())
        return false;

    int checkX = mCheckPos.x();
    bool isInside = false;
    for(double x : mVecIntersections) {
        if(x > checkX)
            return isInside;
        isInside = !isInside;
    }
    return false;
}

void CheckPointViewport::onMouseStateChanged(MouseState ms) {
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

void CheckPointViewport::onXChanged(int x) {
    if(x != mCheckPos.x()) {
        mCheckPos.setX(x);
        StartTimer(mTimerLimitUpdate, 10);
    }
}

void CheckPointViewport::onYChanged(int y) {
    if(y != mCheckPos.y()) {
        mCheckPos.setY(y);
        mVecIntersections = GetIntersections(mVecPoints, y);
        StartTimer(mTimerLimitUpdate, 10);
    }
}

void CheckPointViewport::onVertexPosVisibleChanged(bool visible) {
    mIsVertexPosVisible = visible;
    StartTimer(mTimerLimitUpdate, 10);
}

void CheckPointViewport::onClearVertex() {
    mVecPoints.clear();
    mVecIntersections.clear();
    StartTimer(mTimerLimitUpdate, 10);
}
