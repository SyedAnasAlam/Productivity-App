#include <QJsonArray>
#include "Constants.h"
#include "todolist.h"

TodoList::TodoList() : Database("TodoList")
{}

QStringList TodoList::getTodoList()
{
    QStringList stringTodoList;
    for(QVariant & v : __database.toVariantList())
        stringTodoList.append(v.toString());

    return stringTodoList;
}

bool TodoList::addTask(const QString & taskDescription)
{
    __database.append(taskDescription);
    bool ret = saveDatabase();
    return ret;
}

bool TodoList::completeTask(int taskIndex)
{
    __database.removeAt(taskIndex);
    bool ret = saveDatabase();
    return ret;
}


