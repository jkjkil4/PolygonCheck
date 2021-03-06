#pragma once

#include <QLineEdit>
#include <QRegularExpressionValidator>

#include <QWheelEvent>

class DigitEdit : public QLineEdit
{
protected:
    void wheelEvent(QWheelEvent *ev) override;

public:
    explicit DigitEdit(QWidget *parent = nullptr);
    explicit DigitEdit(const QString &text, QWidget *parent = nullptr);

    static QRegularExpression regExp;
    QRegularExpressionValidator *mRegExpVal = new QRegularExpressionValidator(regExp, this);
};
