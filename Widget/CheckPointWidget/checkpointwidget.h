#pragma once

#include <QWidget>

#include "checkpointsidebar.h"
#include "checkpointviewport.h"

class CheckPointWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CheckPointWidget(QWidget *parent = nullptr);

private:
    CheckPointSideBar *mSideBar = new CheckPointSideBar;
    CheckPointViewport *mViewport = new CheckPointViewport;
};

