#include "widget.h"
#include "ui_widget.h"
#include "model.h"

#include<QDebug>
#include<QtGui>
#include<QGraphicsEllipseItem>

Widget::Widget(QWidget *parent) : QWidget(parent), ui(new Ui::Widget)
{
    ui->setupUi(this);
    ui->todoListWidget->addItems(model.getTodoList());
}

Widget::~Widget()
{
    delete ui;
}


void Widget::on_todoAddBtn_clicked()
{
    QString newTaskDescription = ui->newTaskLineEdit->text();
    model.addTaskTodoList(newTaskDescription);
    ui->todoListWidget->addItem(newTaskDescription);
}


void Widget::on_todoListWidget_itemDoubleClicked(QListWidgetItem *item)
{
    int taskIdx = ui->todoListWidget->currentRow();
    model.completeTaskTodoList(taskIdx);
    ui->todoListWidget->takeItem(taskIdx);
}

/* QGraphicsScene * scene = new QGraphicsScene(ui->graphicsView);
 QGraphicsEllipseItem * ellipse = new QGraphicsEllipseItem(0, 0, 200, 200);
 ellipse->setStartAngle(0);
 ellipse->setSpanAngle(90*16);
 scene->addItem(ellipse);
 ui->graphicsView->show();
 ui->graphicsView->setScene(scene);*/
