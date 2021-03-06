#pragma once

#include <QDebug>
#include <QTimer>

enum class MouseState { Cursor, AddPoint, Move, SetPos };

inline void StartTimer(QTimer *pTimer, int msec) {
    if(!pTimer->isActive())
        pTimer->start(msec);
}
