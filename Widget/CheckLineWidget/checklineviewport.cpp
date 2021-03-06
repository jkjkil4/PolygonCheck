#include "checklineviewport.h"

CheckLineViewport::CheckLineViewport(QWidget *parent) : QWidget(parent)
{

    QPalette pal = palette();
    pal.setColor(QPalette::Window, QColor(255, 255, 255));
    setPalette(pal);
    setAutoFillBackground(true);
}

void CheckLineViewport::onMouseStateChanged(MouseState ms) {
    Q_UNUSED(ms)
}

void CheckLineViewport::onX1Changed(int x) {
    Q_UNUSED(x)
}

void CheckLineViewport::onY1Changed(int y) {
    Q_UNUSED(y)
}

void CheckLineViewport::onX2Changed(int x) {
    Q_UNUSED(x)
}

void CheckLineViewport::onY2Changed(int y) {
    Q_UNUSED(y)
}

void CheckLineViewport::onLinePosVisibleChanged(bool visible) {
    Q_UNUSED(visible)
}

void CheckLineViewport::onVertexPosVisibleChanged(bool visible) {
    Q_UNUSED(visible)
}

void CheckLineViewport::onClearVertex() {

}
