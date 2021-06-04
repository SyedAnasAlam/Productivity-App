#include <QApplication>
#include <QJsonArray>
#include <QFile>
#include <QJsonDocument>
#include <QDebug>
#include "database.h"


Database::Database(QString databaseName) : __databasePath(QApplication::applicationDirPath() + "/Database/" + databaseName)
{}

bool Database::openDatabase()
{
    QFile database(__databasePath);
    bool ret = database.open(QIODevice::ReadOnly | QIODevice::Text);
    QString dataFileContent = database.readAll();
    database.close();

    QJsonDocument jsonDataDoc = QJsonDocument::fromJson(dataFileContent.toUtf8());
    __database = jsonDataDoc.array();
    qDebug() << "open" << ret;
    return ret;
}

bool Database::saveDatabase()
{
    QJsonDocument jsonDataDoc(__database);

    QFile file(__databasePath);
    bool ret1 = file.open(QIODevice::WriteOnly | QIODevice::Text);
    qint64 ret2 = file.write(jsonDataDoc.toJson());
    file.close();

    return ret1 && (ret2 != -1);
}


