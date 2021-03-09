#pragma once

#include <QWidget>

#include <QTimer>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QPainter>

#include "header.h"
#include <Lib/paint.h>

class CheckCircleViewport : public QWidget
{
    Q_OBJECT
protected:
    void mousePressEvent(QMouseEvent *ev) override;
    void mouseMoveEvent(QMouseEvent *ev) override;
    void wheelEvent(QWheelEvent *ev) override;
    void leaveEvent(QEvent *) override;
    void paintEvent(QPaintEvent *) override;
    void closeEvent(QCloseEvent *) override;

public:
    explicit CheckCircleViewport(QWidget *parent = nullptr);

    void setPosByMouse(QPoint pos);
    void check();
    bool isCheckCircleInside();

signals:
    void xChanged(int x);
    void yChanged(int y);
    void radChanged(int r);

public slots:
    void onMouseStateChanged(MouseState ms);
    void onXChanged(int x);
    void onYChanged(int y);
    void onRadChanged(int r);
    void onVertexPosVisibleChanged(bool visible);
    void onClearVertex();

private:
    MouseState mMouseState = MouseState::Cursor;

    QVector<QPointF> mVecPoints;
    QPoint mCheckPos = QPoint(0, 0);
    int mRad = 5;

    bool mHasCollision = true;
    QVector<double> mVecIntersections;

    QPoint mPrevPos;
    QPoint mOffset;
    bool isVertexPosVisible = false;

    QTimer *mTimerLimitUpdate = new QTimer(this);

};

