#include "sidebar.h"

SideBar::SideBar(QWidget *parent) : QWidget(parent)
{
    mRbtCursor->setChecked(true);

    connect(mRbtCursor, &QRadioButton::clicked, [this](bool checked) { if(checked) emit mouseStateChanged(MouseState::Cursor); });
    connect(mRbtAddPoint, &QRadioButton::clicked, [this](bool checked) { if(checked) emit mouseStateChanged(MouseState::AddPoint); });
    connect(mRbtMove, &QRadioButton::clicked, [this](bool checked){ if(checked) emit mouseStateChanged(MouseState::Move); });

    QVBoxLayout *layRbt = new QVBoxLayout;
    layRbt->addWidget(mRbtCursor);
    layRbt->addWidget(mRbtAddPoint);
    layRbt->addWidget(mRbtMove);

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

