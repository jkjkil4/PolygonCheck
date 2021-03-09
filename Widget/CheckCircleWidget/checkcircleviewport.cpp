#include "checkcircleviewport.h"

CheckCircleViewport::CheckCircleViewport(QWidget *parent) : QWidget(parent)
{
    setMouseTracking(true);

    QPalette pal = palette();
    pal.setColor(QPalette::Window, QColor(255, 255, 255));
    setPalette(pal);
    setAutoFillBackground(true);
}


void CheckCircleViewport::mousePressEvent(QMouseEvent *ev) {

}

void CheckCircleViewport::mouseMoveEvent(QMouseEvent *ev) {

}

void CheckCircleViewport::leaveEvent(QEvent *) {

}

void CheckCircleViewport::paintEvent(QPaintEvent *) {

}

void CheckCircleViewport::closeEvent(QCloseEvent *) {
    mTimerLimitUpdate->stop();
}


void CheckCircleViewport::check() {

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
//    if(x != mCheckPos.x()) {
//        mCheckPos.setX(x);
//        StartTimer(mTimerLimitUpdate, 10);
//    }
}

void CheckCircleViewport::onYChanged(int y) {
//    if(y != mCheckPos.y()) {
//        mCheckPos.setY(y);
//        mVecIntersections = getIntersections(y);
//        StartTimer(mTimerLimitUpdate, 10);
//    }
}

void CheckCircleViewport::onRadChanged(int r) {

}

void CheckCircleViewport::onVertexPosVisibleChanged(bool visible) {

}

void CheckCircleViewport::onClearVertex() {

}
