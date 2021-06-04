#ifndef TODOLIST_H
#define TODOLIST_H

#include <QVariant>
#include "database.h"

class TodoList : public Database
{

public:
    TodoList();
    QStringList getTodoList();
    bool addTask(QString taskDescription);
    bool completeTask(int taskIndex);

};

#endif // TODOLIST_H
