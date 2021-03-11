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

inline double RotateX(const QPointF &pos, double sin, double cos) {
    return pos.x() * cos - pos.y() * sin;
}

inline double RotateY(const QPointF &pos, double sin, double cos) {
    return pos.x() * sin + pos.y() * cos;
}

inline QPointF Rotate(const QPointF &pos, double sin, double cos) {
    return QPointF(RotateX(pos, sin, cos), RotateY(pos, sin, cos));
}

inline QPointF Rotate(const QPointF &pos, double radius) {
    return Rotate(pos, qSin(radius), qCos(radius));
}

#define FIX_INTERSECTION_AT_VERTEX
inline QVector<double> GetIntersections(const QVector<QPointF> &vecPoints, double y) {
#ifdef FIX_INTERSECTION_AT_VERTEX
    if(vecPoints.size() < 2)
        return QVector<double>();

    //从末尾开始遍历得到第一个不为水平的线的 走向 和 交点情况
    bool isAllHor = true;
    bool prevTrend = false;
    bool prevHasIntersection = false;
    QPointF prev = *vecPoints.rbegin();
    for(auto iter = vecPoints.rbegin() + 1; iter != vecPoints.rend(); ++iter) {
        const QPointF &cur = *iter;
        if(prev.y() != cur.y()) {
            isAllHor = false;
            prevTrend = cur.y() < prev.y(); //因为是反向遍历，所以是小于（和后面的大于相反）
            prevHasIntersection = (y >= qMin(prev.y(), cur.y()) && y <= qMax(prev.y(), cur.y()));
            break;
        }
        prev = cur;
    }
    if(isAllHor)
        return QVector<double>();

    //计算交点
    QVector<double> vecIntersections;
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

inline bool IsContains(const QVector<double> &vecIntersections, double checkX) {
    bool isInside = false;
    for(double x : vecIntersections) {
        if(x > checkX)
            return isInside;
        isInside = !isInside;
    }
    return false;
}

inline bool IsContains(const QVector<QPointF> &vecPoints, const QPointF &pos1, const QPointF &pos2) {
    if(vecPoints.size() < 2)
        return false;
    
    double radius = qAtan2(pos2.y() - pos1.y(), pos2.x() - pos1.x());
    double sin = qSin(-radius), cos = qCos(-radius);
    QPointF pos1Rotated = Rotate(pos1, sin, cos);
    QPointF pos2Rotated = Rotate(pos2, sin, cos);
    double y = pos1Rotated.y(); // = pos2Rotated.y()

    //从末尾开始遍历得到第一个不为水平的线的 走向 和 交点情况
    bool isAllHor = true;
    bool prevTrend = false;
    bool prevHasIntersection = false;
    double prevY = RotateY(*vecPoints.rbegin(), sin, cos);
    for(auto iter = vecPoints.rbegin() + 1; iter != vecPoints.rend(); ++iter) {
        double curY = RotateY(*iter, sin, cos);
        if(prevY != curY) {
            isAllHor = false;
            prevTrend = curY < prevY; //因为是反向遍历，所以是小于（和后面的大于相反）
            prevHasIntersection = (y >= qMin(prevY, curY) && y <= qMax(prevY, curY));
            break;
        }
    }
    if(isAllHor)
        return false;

    //计算交点
    QVector<double> vecIntersections;
    QPointF prevRotated = Rotate(*vecPoints.rbegin(), sin, cos);
    for(const QPointF &cur : vecPoints) {
        QPointF curRotated = Rotate(cur, sin, cos);
        bool hasIntersection = false;
        if(prevRotated.y() != curRotated.y()) {
            bool trend = curRotated.y() > prevRotated.y();
            if(trend != prevTrend || !prevHasIntersection) {
                if(y >= qMin(prevRotated.y(), curRotated.y()) && y <= qMax(prevRotated.y(), curRotated.y())) {
                    hasIntersection = true;
                    vecIntersections << prevRotated.x() + 
                        (curRotated.x() - prevRotated.x()) * (y - prevRotated.y()) / (curRotated.y() - prevRotated.y());
                }
            }
            if(trend != prevTrend)
                prevTrend = trend;
        }
        prevHasIntersection = hasIntersection;
        prevRotated = curRotated;
    }

    std::sort(vecIntersections.begin(), vecIntersections.end());    //排序
    
    //检测线是否在内部
    double checkX = pos1Rotated.x();
    bool isInside = false;
    for(double x : vecIntersections) {
        if(x > checkX)
            return isInside && pos2Rotated.x() < x;
        isInside = !isInside;
    }
    return false;
}

template<typename Dis, typename Point>
Dis PointDistancePow(const Point &pos1, const Point &pos2) {
    Dis x = pos2.x() - pos1.x();
    Dis y = pos2.y() - pos1.y();
    return x * x + y * y;
}
