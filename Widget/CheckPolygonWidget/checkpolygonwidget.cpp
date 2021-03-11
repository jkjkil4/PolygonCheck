#include "checkpolygonwidget.h"

CheckPolygonWidget::CheckPolygonWidget(QWidget *parent) : QWidget(parent)
{
    connect(mBtnClearEditPoly, SIGNAL(clicked()), mEditPoly, SLOT(onClearVertex()));


    QVBoxLayout *layEdit = new QVBoxLayout;
    layEdit->addWidget(new QLabel("左键添加顶点\n右键移动"), 0, Qt::AlignLeft);
    layEdit->addWidget(mEditPoly, 1);
    layEdit->addWidget(mBtnClearEditPoly, 0, Qt::AlignRight);

    QGroupBox *groupEdit = new QGroupBox("编辑多边形");
    groupEdit->setLayout(layEdit);
    QHBoxLayout *layEdit2 = new QHBoxLayout;
    layEdit2->addWidget(groupEdit);
    QWidget *widgetEdit = new QWidget;
    widgetEdit->setLayout(layEdit2);

    QHBoxLayout *layMain = new QHBoxLayout;
    layMain->setMargin(0);
    layMain->setSpacing(0);
    layMain->addWidget(mSideBar);
    layMain->addWidget(mViewport);
    layMain->addWidget(widgetEdit);
    setLayout(layMain);

    resize(600, 420);
}
