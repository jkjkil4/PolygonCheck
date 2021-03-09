#pragma once

#include <QWidget>
#include <QPushButton>

#include "Widget/CheckPointWidget/checkpointwidget.h"
#include "Widget/CheckLineWidget/checklinewidget.h"
#include "Widget/CheckCircleWidget/checkcirclewidget.h"

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

    template<typename T>void newWnd();

private:
    QGroupBox *mGroupBox = new QGroupBox("检测类型");
    QPushButton *mBtnCheckPointWidget = new QPushButton("检测 点");
    QPushButton *mBtnCheckLineWidget = new QPushButton("检测 线");
    QPushButton *mBtnCheckCircleWidget = new QPushButton("检测 圆");
};
