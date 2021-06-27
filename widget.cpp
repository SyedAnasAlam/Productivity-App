#include <QDebug>
#include <QtGui>
#include <QTabWidget>
#include <QVBoxLayout>
#include <QLayout>
#include "widget.h"
#include "ui_widget.h"
#include "todolist.h"


Widget::Widget(QWidget *parent) : QWidget(parent)
{
    //Init
    __todolist.openDatabase();

    //Drawing
    QVBoxLayout * layout = new QVBoxLayout();
    this->setLayout(layout);
    QTabWidget * tabWidget = new QTabWidget(this);
    layout->addWidget(tabWidget);

    __todolist.draw(tabWidget);
    __habitTracker->display(tabWidget);

}


Widget::~Widget()
{

}

