#include <QDebug>
#include <QtGui>
#include <QTabWidget>
#include <QVBoxLayout>
#include <QLayout>
#include "widget.h"
#include "ui_widget.h"
#include "todolist.h"


Widget::Widget(QWidget * parent) : QWidget(parent)
{
    //Drawing
    QVBoxLayout * layout = new QVBoxLayout();
    this->setLayout(layout);
    QTabWidget * tabWidget = new QTabWidget(this);
    layout->addWidget(tabWidget);

    __todolist->display(tabWidget);
    __habitTracker->display(tabWidget);
    __weeklySchedule->display(tabWidget);
    __impossibleList->display(tabWidget);
}


Widget::~Widget()
{

}

