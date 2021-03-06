#pragma once

#include <QWidget>

#include "header.h"

class CheckLineViewport : public QWidget
{
    Q_OBJECT
public:
    explicit CheckLineViewport(QWidget *parent = nullptr);

    QVector<double> getIntersections(QPointF pos1, QPointF pos2);

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
};

