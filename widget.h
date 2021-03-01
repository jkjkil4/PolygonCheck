#pragma once

#include <QWidget>

#include "Widget/sidebar.h"
#include "Widget/viewport.h"

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private:
    SideBar *mSideBar = new SideBar;
    Viewport *mViewport = new Viewport;
};
