#pragma once

#include <QWidget>

#include "checklinesidebar.h"
#include "checklineviewport.h"

class CheckLineWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CheckLineWidget(QWidget *parent = nullptr);

private:
    CheckLineSideBar *mSideBar = new CheckLineSideBar;
    CheckLineViewport *mViewport = new CheckLineViewport;
};

