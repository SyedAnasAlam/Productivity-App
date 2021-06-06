#ifndef TODOLIST_H
#define TODOLIST_H

#include <QListWidget>
#include <QPushButton>
#include <QLineEdit>
#include "database.h"

class TodoList : public Database, public QWidget
{

public:
    TodoList();
    void draw(QTabWidget * parent);

private:
    QStringList getTodoList();
    bool addTask(const QString & taskDescription);
    bool completeTask(int taskIndex);

    QListWidget * __listWidget;
    QPushButton * __addTaskButton;
    QLineEdit * __lineEdit;

private slots:
    void addTaskButton_clicked();
    void tabWidgetItem_double_clicked();


};

#endif // TODOLIST_H
