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

#define FIX_INTERSECTION_AT_VERTEX
inline QVector<double> GetIntersections(const QVector<QPointF> &vecPoints, double y) {
#ifdef FIX_INTERSECTION_AT_VERTEX
    QVector<double> vecIntersections;
    if(vecPoints.size() < 2)
        return vecIntersections;

    //检测是否全部都为水平线
    double tmpY = (*vecPoints.begin()).y();
    bool isAllHor = true;
    for(int i = 1; i < vecPoints.size(); i++) {
        if(vecPoints[i].y() != tmpY) {
            isAllHor = false;
            break;
        }
    }
    if(isAllHor)
        return vecIntersections;

    //从末尾开始遍历得到第一个不为水平的线的 走向 和 交点情况
    bool prevTrend = false;
    bool prevHasIntersection = false;
    QPointF prev = *vecPoints.rbegin();
    for(auto iter = vecPoints.rbegin() + 1; iter != vecPoints.rend(); ++iter) {
        const QPointF &cur = *iter;
        if(prev.y() != cur.y()) {
            prevTrend = cur.y() < prev.y(); //因为是反向遍历，所以是小于（和后面的大于相反）
            prevHasIntersection = (y >= qMin(prev.y(), cur.y()) && y <= qMax(prev.y(), cur.y()));
            break;
        }
        prev = cur;
    }

    //计算交点
    prev = *vecPoints.rbegin();
    for(const QPointF &cur : vecPoints) {
        bool hasIntersection = false;
        if(prev.y() != cur.y()) {
            bool trend = cur.y() > prev.y();
            if(trend != prevTrend || !prevHasIntersection) {
                if(y >= qMin(prev.y(), cur.y()) && y <= qMax(prev.y(), cur.y())) {
                    hasIntersection = true;
                    vecIntersections << prev.x() + (cur.x() - prev.x()) * (y - prev.y()) / (cur.y() - prev.y());
                }
            }
            if(trend != prevTrend)
                prevTrend = trend;
        }
        prevHasIntersection = hasIntersection;
        prev = cur;
    }

    std::sort(vecIntersections.begin(), vecIntersections.end());

    return vecIntersections;
#else
    QVector<double> vecIntersections;
    if(vecPoints.size() < 2)
        return vecIntersections;

    //计算交点
    QPointF prev = *vecPoints.rbegin();
    for(const QPointF &cur : vecPoints) {
        if(prev.y() != cur.y() && y >= qMin(prev.y(), cur.y()) && y <= qMax(prev.y(), cur.y())) {
            vecIntersections << prev.x() + (cur.x() - prev.x()) * (y - prev.y()) / (cur.y() - prev.y());
        }
        prev = cur;
    }

    std::sort(vecIntersections.begin(), vecIntersections.end());

    return vecIntersections;
#endif
}

template<typename Dis, typename Point>
Dis PointDistancePow(const Point &pos1, const Point &pos2) {
    Dis x = pos2.x() - pos1.x();
    Dis y = pos2.y() - pos1.y();
    return x * x + y * y;
}
