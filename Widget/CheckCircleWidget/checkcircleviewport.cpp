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

}


void CheckCircleViewport::onMouseStateChanged(MouseState ms) {

}

void CheckCircleViewport::onXChanged(int x) {

}

void CheckCircleViewport::onYChanged(int y) {

}

void CheckCircleViewport::onRadChanged(int r) {

}

void CheckCircleViewport::onVertexPosVisibleChanged(bool visible) {

}

void CheckCircleViewport::onClearVertex() {

}
