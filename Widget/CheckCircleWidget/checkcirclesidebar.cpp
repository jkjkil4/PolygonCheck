#include "checkcirclesidebar.h"

CheckCircleSideBar::CheckCircleSideBar(QWidget *parent) : QWidget(parent)
{
    mRbtCursor->setChecked(true);

    connect(mRbtCursor, &QRadioButton::clicked, [this](bool checked) { if(checked) emit mouseStateChanged(MouseState::Cursor); });
    connect(mRbtAddPoint, &QRadioButton::clicked, [this](bool checked) { if(checked) emit mouseStateChanged(MouseState::AddPoint); });
    connect(mRbtMove, &QRadioButton::clicked, [this](bool checked){ if(checked) emit mouseStateChanged(MouseState::Move); });
    connect(mRbtSetPos, &QRadioButton::clicked, [this](bool checked){ if(checked) emit mouseStateChanged(MouseState::SetPos); });
    connect(mEditX, &DigitEdit::textChanged, [this](const QString &text){ emit xChanged(text.toInt()); });
    connect(mEditY, &DigitEdit::textChanged, [this](const QString &text){ emit yChanged(text.toInt()); });
    connect(mEditRad, &DigitEdit::textChanged, [this](const QString &text){ emit radChanged(text.toInt()); });
    connect(mCbbVertexPosVisible, &QCheckBox::stateChanged, [this](int state){ emit vertexPosVisibleChanged(state); });
    connect(mBtnClearVertex, &QPushButton::clicked, [this](){ emit clearVertex(); });

    {
        QVBoxLayout *layRbt = new QVBoxLayout;
        layRbt->addWidget(mRbtCursor);
        layRbt->addWidget(mRbtAddPoint);
        layRbt->addWidget(mRbtMove);
        layRbt->addWidget(mRbtSetPos);
        mRbtGroup->setLayout(layRbt);
    }

    {
        QHBoxLayout *layEditX = new QHBoxLayout;
        layEditX->addWidget(new QLabel("x:"));
        layEditX->addWidget(mEditX);

        QHBoxLayout *layEditY = new QHBoxLayout;
        layEditY->addWidget(new QLabel("y:"));
        layEditY->addWidget(mEditY);

        QHBoxLayout *layEditRad = new QHBoxLayout;
        layEditRad->addWidget(new QLabel("半径:"));
        layEditRad->addWidget(mEditRad);

        QVBoxLayout *lay = new QVBoxLayout;
        lay->addLayout(layEditX);
        lay->addLayout(layEditY);
        lay->addLayout(layEditRad);
        mEditGroup->setLayout(lay);
    }

    QVBoxLayout *layMain = new QVBoxLayout;
    layMain->addWidget(mRbtGroup);
    layMain->addWidget(mEditGroup);
    layMain->addWidget(mCbbVertexPosVisible);
    layMain->addWidget(mBtnClearVertex);
    layMain->addStretch();

    setLayout(layMain);
    adjustSize();
    j::LimitWidth(this, width());

    QPalette pal = palette();
    pal.setColor(QPalette::Window, QColor(240, 240, 240));
    setPalette(pal);
    setAutoFillBackground(true);
}

void CheckCircleSideBar::onXChanged(int x) {
    mEditX->setText(QString::number(x));
}

void CheckCircleSideBar::onYChanged(int y) {
    mEditY->setText(QString::number(y));
}

void CheckCircleSideBar::onRadChanged(int r) {
    mEditRad->setText(QString::number(r));
}
