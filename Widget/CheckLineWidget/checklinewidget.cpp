#include "checklinewidget.h"

CheckLineWidget::CheckLineWidget(QWidget *parent) : QWidget(parent)
{
    connect(mSideBar, SIGNAL(mouseStateChanged(MouseState)), mViewport, SLOT(onMouseStateChanged(MouseState)));
    connect(mSideBar, SIGNAL(x1Changed(int)), mViewport, SLOT(onX1Changed(int)));
    connect(mSideBar, SIGNAL(y1Changed(int)), mViewport, SLOT(onY1Changed(int)));
    connect(mViewport, SIGNAL(x1Changed(int)), mSideBar, SLOT(onX1Changed(int)));
    connect(mViewport, SIGNAL(y1Changed(int)), mSideBar, SLOT(onY1Changed(int)));
    connect(mSideBar, SIGNAL(x2Changed(int)), mViewport, SLOT(onX2Changed(int)));
    connect(mSideBar, SIGNAL(y2Changed(int)), mViewport, SLOT(onY2Changed(int)));
    connect(mViewport, SIGNAL(x2Changed(int)), mSideBar, SLOT(onX2Changed(int)));
    connect(mViewport, SIGNAL(y2Changed(int)), mSideBar, SLOT(onY2Changed(int)));
    connect(mSideBar, SIGNAL(linePosVisibleChanged(bool)), mViewport, SLOT(onLinePosVisibleChanged(bool)));
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
