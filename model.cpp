#include <QFile>
#include <QTextStream>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonValue>
#include <QVariant>

#include "model.h"

Model::Model()
{
    QFile dataFile(dataFilePath);
    dataFile.open(QIODevice::ReadOnly | QIODevice::Text);
    QString dataFileContent = dataFile.readAll();
    dataFile.close();

    QJsonDocument jsonDataDoc = QJsonDocument::fromJson(dataFileContent.toUtf8());
    jsonDataFile = jsonDataDoc.array();
}

QStringList Model::getTodoList()
{
    return jsonDataFile[todoListIdx].toVariant().toStringList();
}

void Model::addTaskTodoList(QString description)
{
    QJsonArray jsonTodoList = jsonDataFile[todoListIdx].toArray();
    jsonTodoList.append(description);
    jsonDataFile[todoListIdx] = jsonTodoList;

    saveDataFile();
}


void Model::completeTaskTodoList(int taskIdx)
{
    QJsonArray jsonTodoList = jsonDataFile[todoListIdx].toArray();
    jsonTodoList.removeAt(taskIdx);
    jsonDataFile[todoListIdx] = jsonTodoList;

    saveDataFile();
}

void Model::saveDataFile()
{
    QJsonDocument jsonDataDoc(jsonDataFile);

    QFile dataFile(dataFilePath);
    dataFile.open(QIODevice::WriteOnly | QIODevice::Text);
    dataFile.write(jsonDataDoc.toJson());
    dataFile.close();
}
