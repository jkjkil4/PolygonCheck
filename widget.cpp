#include "widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    QHBoxLayout *layMain = new QHBoxLayout;
    layMain->setMargin(0);
    layMain->addWidget(sideBar);
    layMain->addStretch();
    setLayout(layMain);

    resize(600, 420);
}

Widget::~Widget()
{

}
