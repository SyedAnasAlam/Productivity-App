#include <QDebug>
#include <QtGui>
#include "widget.h"
#include "ui_widget.h"
#include "todolist.h"

Widget::Widget(QWidget *parent) : QWidget(parent), ui(new Ui::Widget)
{
    ui->setupUi(this);
    __todolist.openDatabase();
    ui->todoListWidget->addItems(__todolist.getTodoList());
}

void Widget::on_todoAddBtn_clicked()
{
    QString newTaskDescription = ui->newTaskLineEdit->text();
    __todolist.addTask(newTaskDescription);
    ui->todoListWidget->addItem(newTaskDescription);
}


void Widget::on_todoListWidget_itemDoubleClicked()
{
    int taskIndex = ui->todoListWidget->currentRow();
    __todolist.completeTask(taskIndex);
    ui->todoListWidget->takeItem(taskIndex);
}

Widget::~Widget()
{
    delete ui;
}


/* #include<QGraphicsEllipseItem>
 * QGraphicsScene * scene = new QGraphicsScene(ui->graphicsView);
 QGraphicsEllipseItem * ellipse = new QGraphicsEllipseItem(0, 0, 200, 200);
 ellipse->setStartAngle(0);
 ellipse->setSpanAngle(90*16);
 scene->addItem(ellipse);
 ui->graphicsView->show();
 ui->graphicsView->setScene(scene);*/
