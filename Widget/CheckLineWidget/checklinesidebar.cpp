#include "checklinesidebar.h"

CheckLineSideBar::CheckLineSideBar(QWidget *parent) : QWidget(parent)
{
    mRbtCursor->setChecked(true);

    connect(mRbtCursor, &QRadioButton::clicked, [this](bool checked) { if(checked) emit mouseStateChanged(MouseState::Cursor); });
    connect(mRbtAddPoint, &QRadioButton::clicked, [this](bool checked) { if(checked) emit mouseStateChanged(MouseState::AddPoint); });
    connect(mRbtMove, &QRadioButton::clicked, [this](bool checked){ if(checked) emit mouseStateChanged(MouseState::Move); });
    connect(mRbtSetPos, &QRadioButton::clicked, [this](bool checked){ if(checked) emit mouseStateChanged(MouseState::SetPos); });
    connect(mEditX1, &QLineEdit::textChanged, [this](const QString &text){ emit x1Changed(text.toInt()); });
    connect(mEditY1, &QLineEdit::textChanged, [this](const QString &text){ emit y1Changed(text.toInt()); });
    connect(mEditX2, &QLineEdit::textChanged, [this](const QString &text){ emit x2Changed(text.toInt()); });
    connect(mEditY2, &QLineEdit::textChanged, [this](const QString &text){ emit y2Changed(text.toInt()); });
    connect(mCbbLinePosVisible, &QCheckBox::stateChanged, [this](int state){ emit linePosVisibleChanged(state); });
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
        QHBoxLayout *layEditX1 = new QHBoxLayout;
        layEditX1->addWidget(new QLabel("x1:"));
        layEditX1->addWidget(mEditX1);

        QHBoxLayout *layEditY1 = new QHBoxLayout;
        layEditY1->addWidget(new QLabel("y1:"));
        layEditY1->addWidget(mEditY1);

        QHBoxLayout *layEditX2 = new QHBoxLayout;
        layEditX2->addWidget(new QLabel("x2:"));
        layEditX2->addWidget(mEditX2);

        QHBoxLayout *layEditY2 = new QHBoxLayout;
        layEditY2->addWidget(new QLabel("y2:"));
        layEditY2->addWidget(mEditY2);

        QVBoxLayout *lay = new QVBoxLayout;
        lay->addLayout(layEditX1);
        lay->addLayout(layEditY1);
        lay->addLayout(layEditX2);
        lay->addLayout(layEditY2);
        lay->addWidget(mCbbLinePosVisible);
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

void CheckLineSideBar::onX1Changed(int x) {
    mEditX1->setText(QString::number(x));
}
void CheckLineSideBar::onY1Changed(int y) {
    mEditY1->setText(QString::number(y));
}
void CheckLineSideBar::onX2Changed(int x) {
    mEditX2->setText(QString::number(x));
}
void CheckLineSideBar::onY2Changed(int y) {
    mEditY2->setText(QString::number(y));
}

