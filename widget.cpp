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
    __habitTracker.openDatabase();

    //Drawing
    QVBoxLayout * layout = new QVBoxLayout();
    this->setLayout(layout);
    QTabWidget * tabWidget = new QTabWidget(this);
    layout->addWidget(tabWidget);

    __todolist.draw(tabWidget);
}


Widget::~Widget()
{

}


/* #include<QGraphicsEllipseItem>
 * QGraphicsScene * scene = new QGraphicsScene(ui->graphicsView);
 QGraphicsEllipseItem * ellipse = new QGraphicsEllipseItem(0, 0, 200, 200);
 ellipse->setStartAngle(0);
 ellipse->setSpanAngle(90*16);
 scene->addItem(ellipse);
 ui->graphicsView->show();
 ui->graphicsView->setScene(scene);*/
