#pragma once

#include <QWidget>

#include <QTimer>
#include <QMouseEvent>
#include <QPainter>

#include "header.h"
#include <Lib/paint.h>

class CheckLineViewport : public QWidget
{
    Q_OBJECT
protected:
    void mousePressEvent(QMouseEvent *ev) override;
    void mouseMoveEvent(QMouseEvent *ev) override;
    void leaveEvent(QEvent *) override;
    void paintEvent(QPaintEvent *) override;
    void closeEvent(QCloseEvent *) override;

public:
    typedef void(CheckLineViewport::*SignalFn)(int);

    explicit CheckLineViewport(QWidget *parent = nullptr);

    void setPosByMouse(QPoint &rPoint, QPoint pos, CheckLineViewport *obj, SignalFn xSignal, SignalFn ySignal);
    void getRotated();

    bool isCheckLineInside();

signals:
    void x1Changed(int x);
    void y1Changed(int y);
    void x2Changed(int x);
    void y2Changed(int y);

public slots:
    void onMouseStateChanged(MouseState ms);
    void onX1Changed(int x);
    void onY1Changed(int y);
    void onX2Changed(int x);
    void onY2Changed(int y);
    void onLinePosVisibleChanged(bool visible);
    void onVertexPosVisibleChanged(bool visible);
    void onClearVertex();

    void onGetResult();

private:
    //static double pi;

    MouseState mMouseState = MouseState::Cursor;

    QVector<QPointF> mVecPoints;
    QPoint mCheckPos1 = QPoint(0, 0);
    QPoint mCheckPos2 = QPoint(0, 0);

    double mRotatedRadius = 0;
    QVector<QPointF> mVecPointsRotated;
    QPointF mCheckPos1Rotated;
    QPointF mCheckPos2Rotated;

    QVector<double> mVecIntersections;

    QPoint mPrevPos;
    QPoint mOffset;
    bool mIsLinePosVisible = false;
    bool mIsVertexPosVisible = false;

    QTimer *mTimerLimitGetResult = new QTimer(this);
    QTimer *mTimerLimitUpdate = new QTimer(this);
};

