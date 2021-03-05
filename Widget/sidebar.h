#pragma once

#include <QWidget>
#include <QRadioButton>
#include <QLabel>
#include <QCheckBox>
#include <QPushButton>
#include <QGroupBox>

#include <QHBoxLayout>
#include <QVBoxLayout>

#include "digitedit.h"

#include <Lib/header.h>
#include "header.h"

class SideBar : public QWidget
{
    Q_OBJECT
public:
    explicit SideBar(QWidget *parent = nullptr);

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
    QRadioButton *mRbtSetPos = new QRadioButton("设置检测位置");

    QGroupBox *mEditGroup = new QGroupBox("检测位置");
    DigitEdit *mEditX = new DigitEdit("0");
    DigitEdit *mEditY = new DigitEdit("0");

    QCheckBox *mCbbVertexPosVisible = new QCheckBox("显示顶点坐标");

    QPushButton *mBtnClearVertex = new QPushButton("清除顶点");
};

