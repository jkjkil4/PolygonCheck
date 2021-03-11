#include "checkpolygonviewport.h"

CheckPolygonViewport::CheckPolygonViewport(QWidget *parent) : QWidget(parent)
{
    setMouseTracking(true);

    mTimerLimitUpdate->setSingleShot(true);
    connect(mTimerLimitUpdate, SIGNAL(timeout()), this, SLOT(update()));

    mTimerLimitCheck->setSingleShot(true);
    connect(mTimerLimitCheck, SIGNAL(timeout()), this, SLOT(onCheck()));

    QPalette pal = palette();
    pal.setColor(QPalette::Window, QColor(255, 255, 255));
    setPalette(pal);
    setAutoFillBackground(true);
}


void CheckPolygonViewport::mousePressEvent(QMouseEvent *ev) {
    if(ev->button() == Qt::LeftButton) {
        switch(mMouseState) {
        case MouseState::AddPoint:
            mVecPoints << ev->pos() - mOffset;
            StartTimer(mTimerLimitCheck, 8);
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
void CheckPolygonViewport::mouseMoveEvent(QMouseEvent *ev) {
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
void CheckPolygonViewport::leaveEvent(QEvent *) {
    if(mMouseState == MouseState::AddPoint || mMouseState == MouseState::SetPos)
        StartTimer(mTimerLimitUpdate, 10);
}
void CheckPolygonViewport::paintEvent(QPaintEvent *) {
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);

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

    QPoint pos = mCheckPos + mOffset;
    //绘制检测多边形
    if(!mPoly.isEmpty()) {
        p.setPen(QPen(mIsInside ? Qt::green : Qt::red, 2));
        if(mPoly.size() == 1)
            p.drawPoint(mPoly[0] + pos);
        else {
            QPointF prev = *mPoly.rbegin();
            for(const QPointF &cur : mPoly) {
                p.drawLine(prev + pos, cur + pos);
                prev = cur;
            }
        }
    }
    //绘制检测多边形的原点
    p.fillRect(QRect(pos - QPoint(9, 0), QSize(19, 1)), Qt::magenta);
    p.fillRect(QRect(pos - QPoint(0, 9), QSize(1, 19)), Qt::magenta);
}
void CheckPolygonViewport::closeEvent(QCloseEvent *) {
    mTimerLimitUpdate->stop();
}


void CheckPolygonViewport::setPosByMouse(QPoint pos) {
    pos -= mOffset;
    bool isXChanged = pos.x() != mCheckPos.x();
    bool isYChanged = pos.y() != mCheckPos.y();
    if(isXChanged || isYChanged) {
        mCheckPos = pos;
        if(isXChanged) emit xChanged(pos.x());
        if(isYChanged) emit yChanged(pos.y());
        StartTimer(mTimerLimitCheck, 8);
        StartTimer(mTimerLimitUpdate, 10);
    }
}


void CheckPolygonViewport::onCheck() {
    mIsInside = true;

    //如果为空，则判断不在内部
    if(mPoly.isEmpty()) {
        mIsInside = false;
        return;
    }

    //如果只有一个顶点，则当成点来判断
    if(mPoly.size() == 1) {
        QPointF pos = mPoly[0] + mCheckPos;
        if(!IsContains(GetIntersections(mVecPoints, pos.y()), pos.x()))
            mIsInside = false;
        return;
    }

    //如果只有两个顶点，则当成线来判断
    if(mPoly.size() == 2) {
        if(!IsContains(mVecPoints, mPoly[0] + mCheckPos, mPoly[1] + mCheckPos))
            mIsInside = false;
        return;
    }

    QPointF prevOffseted = *mPoly.rbegin() + mCheckPos;
    for(const QPointF &cur : mPoly) {
        QPointF curOffseted = cur + mCheckPos;
        if(!IsContains(mVecPoints, prevOffseted, curOffseted)) {
            mIsInside = false;
            return;
        }
        prevOffseted = curOffseted;
    }
}

void CheckPolygonViewport::onMouseStateChanged(MouseState ms) {
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
void CheckPolygonViewport::onXChanged(int x) {
    if(x != mCheckPos.x()) {
        mCheckPos.setX(x);
        StartTimer(mTimerLimitCheck, 8);
        StartTimer(mTimerLimitUpdate, 10);
    }
}
void CheckPolygonViewport::onYChanged(int y) {
    if(y != mCheckPos.y()) {
        mCheckPos.setY(y);
        StartTimer(mTimerLimitCheck, 8);
        StartTimer(mTimerLimitUpdate, 10);
    }
}
void CheckPolygonViewport::onVertexPosVisibleChanged(bool visible) {
    mIsVertexPosVisible = visible;
    StartTimer(mTimerLimitUpdate, 10);
}
void CheckPolygonViewport::onClearVertex() {
    mVecPoints.clear();
    StartTimer(mTimerLimitCheck, 8);
    StartTimer(mTimerLimitUpdate, 10);
}
void CheckPolygonViewport::onPolygonChanged(const QVector<QPointF> &poly) {
    mPoly = poly;
    StartTimer(mTimerLimitCheck, 8);
    StartTimer(mTimerLimitUpdate, 10);
}
