#include <QFile>
#include <QTextStream>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonValue>
#include <QVariant>
#include <QApplication>
#include "model.h"

Model::Model()
{
    __dataFilePath = QApplication::applicationDirPath() + "/Data.json";
    QFile dataFile(__dataFilePath);
    dataFile.open(QIODevice::ReadOnly | QIODevice::Text);
    QString dataFileContent = dataFile.readAll();
    dataFile.close();

    QJsonDocument jsonDataDoc = QJsonDocument::fromJson(dataFileContent.toUtf8());
    __dataFile = jsonDataDoc.array();
}

void Model::saveDataFile(QJsonArray dataFile)
{
    __dataFile = dataFile;
    QJsonDocument jsonDataDoc(dataFile);

    QFile file(__dataFilePath);
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    file.write(jsonDataDoc.toJson());
    file.close();
}

QJsonArray Model::getDataFile()
{
    return __dataFile;
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


