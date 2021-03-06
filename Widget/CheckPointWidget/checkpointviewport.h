#pragma once

#include <QWidget>

#include <QTimer>
#include <QPainter>
#include <QMouseEvent>

#include "header.h"
#include <Lib/paint.h>

class CheckPointViewport : public QWidget
{
    Q_OBJECT
protected:
    void mousePressEvent(QMouseEvent *ev) override;
    void mouseMoveEvent(QMouseEvent *ev) override;
    void leaveEvent(QEvent *) override;
    void paintEvent(QPaintEvent *) override;
    void closeEvent(QCloseEvent *) override;

public:
    explicit CheckPointViewport();

    void setPosByMouse(QPoint pos);
    QVector<double> getIntersections(double y);

    bool isCheckPosInside();

signals:
    void xChanged(int x);
    void yChanged(int y);

public slots:
    void onMouseStateChanged(MouseState ms);
    void onXChanged(int x);
    void onYChanged(int y);
    void onVertexPosVisibleChanged(bool visible);
    void onClearVertex();

private:
    MouseState mMouseState = MouseState::Cursor;
    QVector<QPointF> mVecPoints;

    QPoint mPrevPos;
    QPoint mOffset;
    QPoint mCheckPos = QPoint(0, 0);
    QVector<double> mVecIntersections;

    bool isVertexPosVisible = false;

    QTimer *mTimerLimitUpdate = new QTimer(this);
};

