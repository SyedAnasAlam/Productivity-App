#include <QJsonArray>
#include "Constants.h"
#include "todolist.h"

TodoList::TodoList(const Model & model) : __model(model) {}

QStringList TodoList::getTodoList()
{
    return __model.getDataFile()[todoListIdx].toVariant().toStringList();
}
