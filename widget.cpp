#include "widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    connect(mBtnCheckPointWidget, &QPushButton::clicked, [this]{ newWnd<CheckPointWidget>(); });
    connect(mBtnCheckLineWidget, &QPushButton::clicked, [this]{ newWnd<CheckLineWidget>(); });

    QVBoxLayout *layGroup = new QVBoxLayout;
    layGroup->setMargin(20);
    layGroup->addWidget(mBtnCheckPointWidget);
    layGroup->addWidget(mBtnCheckLineWidget);
    mGroupBox->setLayout(layGroup);

    QHBoxLayout *layMain = new QHBoxLayout;
    layMain->addWidget(mGroupBox);
    setLayout(layMain);

    adjustSize();
    j::LimitSize(this, qMax(260, width()), height());
}

Widget::~Widget()
{

}

template<typename T>void Widget::newWnd() {
    QWidget* w = new T;
    w->setAttribute(Qt::WA_DeleteOnClose);
    w->show();
}
