#pragma once

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QGroupBox>

#include <QHBoxLayout>
#include <QVBoxLayout>

#include "checkpolygonsidebar.h"
#include "checkpolygonviewport.h"
#include "editpolygonwidget.h"

class CheckPolygonWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CheckPolygonWidget(QWidget *parent = nullptr);

private:
    CheckPolygonSideBar *mSideBar = new CheckPolygonSideBar;
    CheckPolygonViewport *mViewport = new CheckPolygonViewport;

    EditPolygonWidget *mEditPoly = new EditPolygonWidget;
    QPushButton *mBtnClearEditPoly = new QPushButton("清除顶点");
};
