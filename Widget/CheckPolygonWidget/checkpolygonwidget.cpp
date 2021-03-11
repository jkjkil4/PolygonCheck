#include "checkpolygonwidget.h"

CheckPolygonWidget::CheckPolygonWidget(QWidget *parent) : QWidget(parent)
{
    connect(mSideBar, SIGNAL(mouseStateChanged(MouseState)), mViewport, SLOT(onMouseStateChanged(MouseState)));
    connect(mSideBar, SIGNAL(xChanged(int)), mViewport, SLOT(onXChanged(int)));
    connect(mSideBar, SIGNAL(yChanged(int)), mViewport, SLOT(onYChanged(int)));
    connect(mViewport, SIGNAL(xChanged(int)), mSideBar, SLOT(onXChanged(int)));
    connect(mViewport, SIGNAL(yChanged(int)), mSideBar, SLOT(onYChanged(int)));
    connect(mSideBar, SIGNAL(vertexPosVisibleChanged(bool)), mViewport, SLOT(onVertexPosVisibleChanged(bool)));
    connect(mSideBar, SIGNAL(clearVertex()), mViewport, SLOT(onClearVertex()));

    connect(mEditPoly, SIGNAL(polygonChanged(const QVector<QPointF>&)), mViewport, SLOT(onPolygonChanged(const QVector<QPointF>&)));
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

    resize(800, 420);
}
