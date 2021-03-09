#include "checkcirclewidget.h"

CheckCircleWidget::CheckCircleWidget(QWidget *parent) : QWidget(parent)
{
    connect(mSideBar, SIGNAL(mouseStateChanged(MouseState)), mViewport, SLOT(onMouseStateChanged(MouseState)));
    connect(mSideBar, SIGNAL(xChanged(int)), mViewport, SLOT(onXChanged(int)));
    connect(mSideBar, SIGNAL(yChanged(int)), mViewport, SLOT(onYChanged(int)));
    connect(mSideBar, SIGNAL(radChanged(int)), mViewport, SLOT(onRadChanged(int)));
    connect(mViewport, SIGNAL(xChanged(int)), mSideBar, SLOT(onXChanged(int)));
    connect(mViewport, SIGNAL(yChanged(int)), mSideBar, SLOT(onYChanged(int)));
    connect(mViewport, SIGNAL(radChanged(int)), mSideBar, SLOT(onRadChanged(int)));
    connect(mSideBar, SIGNAL(vertexPosVisibleChanged(bool)), mViewport, SLOT(onVertexPosVisibleChanged(bool)));
    connect(mSideBar, SIGNAL(clearVertex()), mViewport, SLOT(onClearVertex()));

    QHBoxLayout *layMain = new QHBoxLayout;
    layMain->setMargin(0);
    layMain->setSpacing(0);
    layMain->addWidget(mSideBar);
    layMain->addWidget(mViewport, 1);
    setLayout(layMain);

    resize(600, 420);
}
