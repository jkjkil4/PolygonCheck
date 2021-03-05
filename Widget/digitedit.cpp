#include "digitedit.h"

QRegularExpression DigitEdit::regExp("-{0,1}[0-9]*");

DigitEdit::DigitEdit(QWidget *parent) : QLineEdit(parent)
{
    setValidator(mRegExpVal);
}

DigitEdit::DigitEdit(const QString &text, QWidget *parent) : QLineEdit(text, parent)
{
    setValidator(mRegExpVal);
}

void DigitEdit::wheelEvent(QWheelEvent *ev) {
    int value = text().toInt() + ev->delta() / 120;
    setText(QString::number(value));
}
