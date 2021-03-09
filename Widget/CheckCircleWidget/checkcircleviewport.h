#pragma once

#include <QWidget>

#include "header.h"

class CheckCircleViewport : public QWidget
{
    Q_OBJECT
protected:
    void mousePressEvent(QMouseEvent *ev) override;
    void mouseMoveEvent(QMouseEvent *ev) override;
    void leaveEvent(QEvent *) override;
    void paintEvent(QPaintEvent *) override;
    void closeEvent(QCloseEvent *) override;

public:
    explicit CheckCircleViewport(QWidget *parent = nullptr);

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
    //MouseState mMouseState = MouseState::Cursor;

};

