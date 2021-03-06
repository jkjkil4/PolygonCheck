#pragma once

#include <QWidget>
#include <QGroupBox>
#include <QRadioButton>
#include <QCheckBox>
#include <QPushButton>
#include <QLabel>

#include <QHBoxLayout>
#include <QVBoxLayout>

#include "../digitedit.h"

#include <Lib/header.h>
#include "header.h"

class CheckLineSideBar : public QWidget
{
    Q_OBJECT
public:
    explicit CheckLineSideBar(QWidget *parent = nullptr);

signals:
    void mouseStateChanged(MouseState ms);
    void x1Changed(int x);
    void y1Changed(int y);
    void x2Changed(int x);
    void y2Changed(int y);
    void linePosVisibleChanged(bool visible);
    void vertexPosVisibleChanged(bool visible);
    void clearVertex();

public slots:
    void onX1Changed(int x);
    void onY1Changed(int y);
    void onX2Changed(int x);
    void onY2Changed(int y);

private:
    QGroupBox *mRbtGroup = new QGroupBox("鼠标选项");
    QRadioButton *mRbtCursor = new QRadioButton("鼠标");
    QRadioButton *mRbtAddPoint = new QRadioButton("添加顶点");
    QRadioButton *mRbtMove = new QRadioButton("移动");
    QRadioButton *mRbtSetPos = new QRadioButton("设置检测线");

    QGroupBox *mEditGroup = new QGroupBox("检测线端点");
    DigitEdit *mEditX1 = new DigitEdit("0");
    DigitEdit *mEditY1 = new DigitEdit("0");
    DigitEdit *mEditX2 = new DigitEdit("0");
    DigitEdit *mEditY2 = new DigitEdit("0");
    QCheckBox *mCbbLinePosVisible = new QCheckBox("显示端点坐标");

    QCheckBox *mCbbVertexPosVisible = new QCheckBox("显示顶点坐标");

    QPushButton *mBtnClearVertex = new QPushButton("清除顶点");
};

