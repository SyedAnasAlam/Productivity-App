#include "widget.h"
#include "ui_widget.h"
#include "model.h"

#include<QDebug>

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
