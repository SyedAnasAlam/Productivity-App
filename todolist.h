#ifndef TODOLIST_H
#define TODOLIST_H

#include <QListWidget>
#include <QPushButton>
#include <QLineEdit>
#include "feature.h"

class TodoList : public Feature, public QWidget
{

public:
    TodoList(QWidget * parent);
    void v_display(QTabWidget * tabWidget);

private:
    QStringList __todoList;

    bool m_addTask(const QString & taskDescription);
    bool m_completeTask(int taskIndex);

    QListWidget * __listWidget;
    QPushButton * __addTaskButton;
    QLineEdit * __lineEdit;

private slots:
    void addTaskButton_clicked();
    void tabWidgetItem_double_clicked();


};

#endif // TODOLIST_H
