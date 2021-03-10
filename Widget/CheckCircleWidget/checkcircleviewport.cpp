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
        check();
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

    //绘制判断线
    if(!mHasCollision && mVecIntersections.size() > 1) {
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
        if(isVertexPosVisible) {
            p.setPen(Qt::black);
            for(QPointF &pos : mVecPoints) {
                QString text = "(" + QString::number(pos.x()) + ", " + QString::number(pos.y()) + ")";
                j::DrawText(&p, (int)pos.x() + mOffset.x() + 1, (int)pos.y() + mOffset.y() + 1, Qt::AlignLeft | Qt::AlignTop, text);
            }
        }
    }

    //绘制检测圆
    QColor col = isCheckCircleInside() ? QColor(0, 190, 0) : QColor(190, 0, 0);
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
    mHasCollision = false;
    if(mVecPoints.size() < 3) {
        mHasCollision = true;
        return;
    }

    int circleLeft = mCheckPos.x() - mRad;
    int circleRight = mCheckPos.x() + mRad;
    int circleTop = mCheckPos.y() - mRad;
    int circleBottom = mCheckPos.y() + mRad;

    //判断圆和多边形是否有交点
    QPointF prev = *mVecPoints.rbegin();
    for(const QPointF &cur : mVecPoints) {
        //如果圆形的矩形区域和线段的矩形区域重叠，则进行进一步判定
        if(circleRight >= qMin(prev.x(), cur.x()) && circleLeft <= qMax(prev.x(), cur.x())
                && circleBottom >= qMin(prev.y(), cur.y()) && circleTop <= qMax(prev.y(), cur.y())) {

            // 向量mCheckPos->prev 与 向量cur->prev 的 点积
            double dotProductPrev = (prev.x() - mCheckPos.x()) * (prev.x() - cur.x()) + (prev.y() - mCheckPos.y()) * (prev.y() - cur.y());
            if(dotProductPrev < 0) {   //该角为钝角
                double disPrevPow = PointDistancePow<double, QPointF>(mCheckPos, prev);   //圆心和prev之间的距离的平方
                if(disPrevPow < mRad * mRad) {
                    mHasCollision = true;
                    return;
                }
            }

            // 向量mCheckPos->cur 与 向量prev->cur 的点积
            double dotProductCur = (cur.x() - mCheckPos.x()) * (cur.x() - prev.x()) + (cur.y() - mCheckPos.y()) * (cur.y() - prev.y());
            if(dotProductCur < 0) {     //该角为钝角
                double disCurPow = PointDistancePow<double, QPointF>(mCheckPos, cur);     //圆心和cur之间的距离的平方
                if(disCurPow < mRad * mRad) {
                    mHasCollision = true;
                    return;
                }
            }

            if(dotProductPrev >= 0 && dotProductCur >= 0) {
                // 计算垂直距离并判断是否相交
                double disBetween = qSqrt(qPow(cur.x() - prev.x(), 2) + qPow(cur.y() - prev.y(), 2));     //prev和cur之间的距离
                double disPrev = qSqrt(qPow(mCheckPos.x() - prev.x(), 2) + qPow(mCheckPos.y() - prev.y(), 2));    //圆心和prev之间的距离
                double disCur = qSqrt(qPow(mCheckPos.x() - cur.x(), 2) + qPow(mCheckPos.y() - cur.y(), 2));       //圆心和cur之间的距离
                double p = (disBetween + disPrev + disCur) / 2;    //某公式相关
                double disVer = 2 * qSqrt(p * (p - disBetween) * (p - disPrev) * (p - disCur)) / disBetween;     //垂直距离
                if(disVer < qAbs(mRad)) {     //圆与线段有交点
                    mHasCollision = true;
                    return;
                }
            }
        }

        prev = cur;
    }

    //得到圆心所在水平线与多边形的交点
    mVecIntersections = GetIntersections(mVecPoints, mCheckPos.y());
}

bool CheckCircleViewport::isCheckCircleInside() {
    if(mHasCollision || mVecIntersections.isEmpty())
        return false;
    double checkX = mCheckPos.x();
    bool isInside = false;
    for(double x : mVecIntersections) {
        if(x > checkX)
            return isInside;
        isInside = !isInside;
    }
    return false;
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
    mHasCollision = true;
    mVecIntersections.clear();
    StartTimer(mTimerLimitUpdate, 10);
}
