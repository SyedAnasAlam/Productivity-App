#ifndef TODOLIST_H
#define TODOLIST_H

#include <QVariant>
#include "model.h"

class TodoList
{

private:
    Model __model;

public:
    TodoList(const Model & model);
    QStringList getTodoList();
};

#endif // TODOLIST_H
