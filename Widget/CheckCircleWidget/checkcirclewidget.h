#pragma once

#include <QWidget>

#include "checkcirclesidebar.h"
#include "checkcircleviewport.h"

class CheckCircleWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CheckCircleWidget(QWidget *parent = nullptr);

private:
    CheckCircleSideBar *mSideBar = new CheckCircleSideBar;
    CheckCircleViewport *mViewport = new CheckCircleViewport;
};
