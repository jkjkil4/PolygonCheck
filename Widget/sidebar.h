#pragma once

#include <QWidget>
#include <QRadioButton>
#include <QButtonGroup>

#include <QHBoxLayout>
#include <QVBoxLayout>

#include <Lib/header.h>
#include "header.h"

class SideBar : public QWidget
{
    Q_OBJECT
public:
    explicit SideBar(QWidget *parent = nullptr);

signals:
    void mouseStateChanged(MouseState ms);

private:
    QRadioButton *rbtCursor = new QRadioButton("鼠标");
    QRadioButton *rbtAddPoint = new QRadioButton("添加顶点");
};

