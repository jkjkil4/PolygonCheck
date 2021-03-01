#pragma once

#include <QWidget>

#include <QTimer>
#include <QPainter>
#include <QMouseEvent>

#include "header.h"

class Viewport : public QWidget
{
    Q_OBJECT
protected:
    void mousePressEvent(QMouseEvent *ev) override;
    void mouseMoveEvent(QMouseEvent *ev) override;
    void leaveEvent(QEvent *) override;
    void paintEvent(QPaintEvent *) override;

public:
    explicit Viewport();

    void startTimer(QTimer *pTimer, int msec);

public slots:
    void onMouseStateChanged(MouseState ms);

private:
    MouseState mMouseState = MouseState::Cursor;
    QVector<QPointF> vecPoints;

    QPoint mPrevPos;
    QPoint mOffset;

    QTimer *mTimerLimitUpdate = new QTimer(this);
};

