#ifndef MODEL_H
#define MODEL_H

#include <QJsonArray>
#include "Constants.h"

class Model
{
private:
    QJsonArray jsonDataFile;
    void saveDataFile();

public:
    Model();
    QStringList getTodoList();
    void addTaskTodoList(QString description);
    void completeTaskTodoList(int taskIdx);


};

#endif // MODEL_H
