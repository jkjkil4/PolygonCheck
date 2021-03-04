#pragma once

#include <QWidget>
#include <QRadioButton>
#include <QLabel>
#include <QCheckBox>

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QRegularExpressionValidator>

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

public slots:
    void onXChanged(int x);
    void onYChanged(int y);

private:
    QRadioButton *mRbtCursor = new QRadioButton("鼠标");
    QRadioButton *mRbtAddPoint = new QRadioButton("添加顶点");
    QRadioButton *mRbtMove = new QRadioButton("移动");
    QRadioButton *mRbtSetPos = new QRadioButton("设置检测位置");

    QLineEdit *mEditX = new QLineEdit("0");
    QLineEdit *mEditY = new QLineEdit("0");
    QRegularExpression mRegExp = QRegularExpression("[0-9]*");
    QRegularExpressionValidator *mRegExpVal = new QRegularExpressionValidator(mRegExp, this);

    QCheckBox *mCbbVertexPosVisible = new QCheckBox("显示顶点坐标");
};

