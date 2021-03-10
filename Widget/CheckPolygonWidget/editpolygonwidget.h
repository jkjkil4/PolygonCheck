#ifndef EDITPOLYGONWIDGET_H
#define EDITPOLYGONWIDGET_H

#include <QWidget>

#include <QMouseEvent>
#include <QTimer>
#include <QPainter>

#include "header.h"

class EditPolygonWidget : public QWidget
{
    Q_OBJECT
protected:
    void mousePressEvent(QMouseEvent *ev) override;
    void mouseMoveEvent(QMouseEvent *ev) override;
    void mouseReleaseEvent(QMouseEvent *ev) override;
    void leaveEvent(QEvent *) override;
    void paintEvent(QPaintEvent *) override;

public:
    explicit EditPolygonWidget(QWidget *parent = nullptr);

    QPoint trueOffset();

signals:
    void polygonChanged(const QVector<QPointF> &mVecVertex);

public slots:
    void onClearVertex();

private:
    QVector<QPointF> mVecVertex;

    QPoint mPrevPos;
    QPoint mOffset;

    QTimer *mTimerLimitUpdate = new QTimer(this);
};

#endif // EDITPOLYGONWIDGET_H
