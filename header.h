#pragma once

#include <QDebug>
#include <QTimer>
#include <QPointF>
#include <qmath.h>

enum class MouseState { Cursor, AddPoint, Move, SetPos };

inline void StartTimer(QTimer *pTimer, int msec) {
    if(!pTimer->isActive())
        pTimer->start(msec);
}

inline QPointF Rotate(const QPointF &pos, double radius) {
    double sin = qSin(radius), cos = qCos(radius);
    double x = pos.x() * cos - pos.y() * sin;
    double y = pos.x() * sin + pos.y() * cos;
    return QPointF(x, y);
}
