#include "sidebar.h"

SideBar::SideBar(QWidget *parent) : QWidget(parent)
{
    rbtCursor->setChecked(true);

    QVBoxLayout *layRbt = new QVBoxLayout;
    layRbt->addWidget(rbtCursor);
    layRbt->addWidget(rbtAddPoint);

    QVBoxLayout *layMain = new QVBoxLayout;
    layMain->addLayout(layRbt);
    layMain->addStretch();

    setLayout(layMain);
    adjustSize();
    j::LimitWidth(this, width());

    QPalette pal = palette();
    pal.setColor(QPalette::Window, QColor(220, 220, 220));
    setPalette(pal);
    setAutoFillBackground(true);
}

