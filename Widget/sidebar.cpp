#include "sidebar.h"

SideBar::SideBar(QWidget *parent) : QWidget(parent)
{
    mRbtCursor->setChecked(true);
    mEditY->setValidator(mRegExpVal);

    connect(mRbtCursor, &QRadioButton::clicked, [this](bool checked) { if(checked) emit mouseStateChanged(MouseState::Cursor); });
    connect(mRbtAddPoint, &QRadioButton::clicked, [this](bool checked) { if(checked) emit mouseStateChanged(MouseState::AddPoint); });
    connect(mRbtMove, &QRadioButton::clicked, [this](bool checked){ if(checked) emit mouseStateChanged(MouseState::Move); });
    connect(mRbtSetPos, &QRadioButton::clicked, [this](bool checked){ if(checked) emit mouseStateChanged(MouseState::SetPos); });
    connect(mEditX, &QLineEdit::textChanged, [this](const QString &text){ emit xChanged(text.toInt()); });
    connect(mEditY, &QLineEdit::textChanged, [this](const QString &text){ emit yChanged(text.toInt()); });
    connect(mCbbVertexPosVisible, &QCheckBox::stateChanged, [this](int state){ emit vertexPosVisibleChanged(state); });
    connect(mBtnClearVertex, &QPushButton::clicked, [this](){ emit clearVertex(); });

    QVBoxLayout *layRbt = new QVBoxLayout;
    layRbt->addWidget(mRbtCursor);
    layRbt->addWidget(mRbtAddPoint);
    layRbt->addWidget(mRbtMove);
    layRbt->addWidget(mRbtSetPos);

    QHBoxLayout *layEditX = new QHBoxLayout;
    layEditX->addWidget(new QLabel("x:"));
    layEditX->addWidget(mEditX);

    QHBoxLayout *layEditY = new QHBoxLayout;
    layEditY->addWidget(new QLabel("y:"));
    layEditY->addWidget(mEditY);

    QVBoxLayout *layCbb = new QVBoxLayout;
    layCbb->addWidget(mCbbVertexPosVisible);

    QVBoxLayout *layBtn = new QVBoxLayout;
    layBtn->addWidget(mBtnClearVertex);

    QVBoxLayout *layMain = new QVBoxLayout;
    layMain->addLayout(layRbt);
    layMain->addLayout(layEditX);
    layMain->addLayout(layEditY);
    layMain->addLayout(layCbb);
    layMain->addLayout(layBtn);
    layMain->addStretch();

    setLayout(layMain);
    adjustSize();
    j::LimitWidth(this, width());

    QPalette pal = palette();
    pal.setColor(QPalette::Window, QColor(220, 220, 220));
    setPalette(pal);
    setAutoFillBackground(true);
}

void SideBar::onXChanged(int x) {
    mEditX->setText(QString::number(x));
}

void SideBar::onYChanged(int y) {
    mEditY->setText(QString::number(y));
}

