#pragma once

#include <QWidget>
#include <QGroupBox>
#include <QRadioButton>
#include <QPushButton>
#include <QCheckBox>
#include <QLabel>

#include <QHBoxLayout>
#include <QVBoxLayout>

#include "../digitedit.h"

#include "header.h"
#include <Lib/header.h>

class CheckPolygonSideBar : public QWidget
{
    Q_OBJECT
public:
    explicit CheckPolygonSideBar(QWidget *parent = nullptr);

signals:
    void mouseStateChanged(MouseState ms);
    void xChanged(int x);
    void yChanged(int y);
    void vertexPosVisibleChanged(bool visible);
    void clearVertex();

public slots:
    void onXChanged(int x);
    void onYChanged(int y);

private:
    QGroupBox *mRbtGroup = new QGroupBox("鼠标选项");
    QRadioButton *mRbtCursor = new QRadioButton("鼠标");
    QRadioButton *mRbtAddPoint = new QRadioButton("添加顶点");
    QRadioButton *mRbtMove = new QRadioButton("移动");
    QRadioButton *mRbtSetPos = new QRadioButton("设置多边形原点");

    QGroupBox *mEditGroup = new QGroupBox("多边形原点");
    DigitEdit *mEditX = new DigitEdit("0");
    DigitEdit *mEditY = new DigitEdit("0");

    QCheckBox *mCbbVertexPosVisible = new QCheckBox("显示顶点坐标");

    QPushButton *mBtnClearVertex = new QPushButton("清除顶点");
};
