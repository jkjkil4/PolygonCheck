#pragma once

#include <QWidget>

#include <QMouseEvent>
#include <QTimer>

#include "header.h"
#include <Lib/paint.h>

class CheckPolygonViewport : public QWidget
{
    Q_OBJECT
protected:
    void mousePressEvent(QMouseEvent *ev) override;
    void mouseMoveEvent(QMouseEvent *ev) override;
    void leaveEvent(QEvent *) override;
    void paintEvent(QPaintEvent *) override;
    void closeEvent(QCloseEvent *) override;

public:
    explicit CheckPolygonViewport(QWidget *parent = nullptr);

    void setPosByMouse(QPoint pos);

signals:
    void xChanged(int x);
    void yChanged(int y);

public slots:
    void onCheck();

    void onMouseStateChanged(MouseState ms);
    void onXChanged(int x);
    void onYChanged(int y);
    void onVertexPosVisibleChanged(bool visible);
    void onClearVertex();
    void onPolygonChanged(const QVector<QPointF> &poly);

private:
    MouseState mMouseState = MouseState::Cursor;
    
    QVector<QPointF> mVecPoints;
    QVector<QPointF> mPoly;
    bool mIsInside = false;
    
    QPoint mPrevPos;
    QPoint mOffset;
    QPoint mCheckPos;
    bool mIsVertexPosVisible = false;

    QTimer *mTimerLimitUpdate = new QTimer(this);
    QTimer *mTimerLimitCheck = new QTimer(this);
};

