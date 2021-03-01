#include "widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    connect(mSideBar, SIGNAL(mouseStateChanged(MouseState)), mViewport, SLOT(onMouseStateChanged(MouseState)));


    QHBoxLayout *layMain = new QHBoxLayout;
    layMain->setMargin(0);
    layMain->setSpacing(0);
    layMain->addWidget(mSideBar);
    layMain->addWidget(mViewport, 1);
    setLayout(layMain);

    resize(600, 420);
}

Widget::~Widget()
{

}
