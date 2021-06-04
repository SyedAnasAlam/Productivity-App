#ifndef MODEL_H
#define MODEL_H

#include <QJsonArray>
#include "Constants.h"

class Model
{
private:
    QJsonArray __dataFile;
    QString __dataFilePath;


public:
    Model();
    void saveDataFile(QJsonArray dataFile);
    QJsonArray getDataFile();

    QStringList getTodoList();
    void addTaskTodoList(QString description);
    void completeTaskTodoList(int taskIdx);


};

#endif // MODEL_H
