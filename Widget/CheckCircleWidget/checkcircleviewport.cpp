#include "checkcircleviewport.h"

CheckCircleViewport::CheckCircleViewport(QWidget *parent) : QWidget(parent)
{
    setMouseTracking(true);

    mTimerLimitUpdate->setSingleShot(true);
    connect(mTimerLimitUpdate, SIGNAL(timeout()), this, SLOT(update()));

    QPalette pal = palette();
    pal.setColor(QPalette::Window, QColor(255, 255, 255));
    setPalette(pal);
    setAutoFillBackground(true);
}


void CheckCircleViewport::mousePressEvent(QMouseEvent *ev) {
    if(ev->button() == Qt::LeftButton) {
        switch(mMouseState) {
        case MouseState::AddPoint:
            mVecPoints << ev->pos() - mOffset;
            check();
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

void CheckCircleViewport::mouseMoveEvent(QMouseEvent *ev) {
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

void CheckCircleViewport::wheelEvent(QWheelEvent *ev) {
    if(mMouseState == MouseState::SetPos) {
        mRad += ev->delta() / 120;
        emit radChanged(mRad);
        StartTimer(mTimerLimitUpdate, 10);
    }
}

void CheckCircleViewport::leaveEvent(QEvent *) {
    if(mMouseState == MouseState::AddPoint || mMouseState == MouseState::SetPos)
        StartTimer(mTimerLimitUpdate, 10);
}

void CheckCircleViewport::paintEvent(QPaintEvent *) {
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

    //绘制检测圆
    QColor col = QColor(0, 190, 0);
    p.setPen(col.darker());
    p.setBrush(col);
    p.drawEllipse(mCheckPos, mRad, mRad);

    //绘制鼠标位置
    if(mMouseState == MouseState::AddPoint || mMouseState == MouseState::SetPos) {
        QPoint pos = mapFromGlobal(cursor().pos());
        if(QRect(0, 0, width(), height()).contains(pos)) {
            QString text = "(" + QString::number(pos.x() - mOffset.x()) + ", " + QString::number(pos.y() - mOffset.y()) + ")";
            if(mMouseState == MouseState::SetPos) {
                p.setPen(Qt::black);
                j::DrawText(&p, pos + QPoint(2, -2), Qt::AlignLeft | Qt::AlignBottom, "左键设置位置\n滚轮调整半径");
            }
            p.setPen(Qt::blue);
            j::DrawText(&p, pos + QPoint(2, 2), Qt::AlignLeft | Qt::AlignTop, text);
        }
    }
}

void CheckCircleViewport::closeEvent(QCloseEvent *) {
    mTimerLimitUpdate->stop();
}

void CheckCircleViewport::setPosByMouse(QPoint pos) {
    pos -= mOffset;
    bool isXChanged = pos.x() != mCheckPos.x();
    bool isYChanged = pos.y() != mCheckPos.y();
    if(isXChanged || isYChanged) {
        mCheckPos = pos;
        check();
        if(isXChanged) emit xChanged(pos.x());
        if(isYChanged) emit yChanged(pos.y());
        StartTimer(mTimerLimitUpdate, 10);
    }
}

void CheckCircleViewport::check() {
    //TODO: ....
}


void CheckCircleViewport::onMouseStateChanged(MouseState ms) {
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

void CheckCircleViewport::onXChanged(int x) {
    if(x != mCheckPos.x()) {
        mCheckPos.setX(x);
        check();
        StartTimer(mTimerLimitUpdate, 10);
    }
}

void CheckCircleViewport::onYChanged(int y) {
    if(y != mCheckPos.y()) {
        mCheckPos.setY(y);
        check();
        StartTimer(mTimerLimitUpdate, 10);
    }
}

void CheckCircleViewport::onRadChanged(int r) {
    if(r != mRad) {
        mRad = r;
        check();
        StartTimer(mTimerLimitUpdate, 10);
    }
}

void CheckCircleViewport::onVertexPosVisibleChanged(bool visible) {
    isVertexPosVisible = visible;
    StartTimer(mTimerLimitUpdate, 10);
}

void CheckCircleViewport::onClearVertex() {
    mVecPoints.clear();
    mHasCollision = false;
    mVecIntersections.clear();
    StartTimer(mTimerLimitUpdate, 10);
}
