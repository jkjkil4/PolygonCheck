#pragma once

#include <QWidget>

#include "Widget/sidebar.h"

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private:
    SideBar *sideBar = new SideBar;
};
