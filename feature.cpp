#include <QApplication>
#include <QJsonArray>
#include <QFile>
#include <QJsonDocument>
#include <QDebug>
#include "feature.h"


Feature::Feature(const QString & databaseName) : __filePath(QString("%1%2.json").arg(__databasePath).arg(databaseName))
{}

bool Feature::m_openDatabase()
{
    QFile database(__filePath);
    bool ret = database.open(QIODevice::ReadOnly | QIODevice::Text);
    QString dataFileContent = database.readAll();
    database.close();

    QJsonDocument jsonDataDoc = QJsonDocument::fromJson(dataFileContent.toUtf8());
    __database = jsonDataDoc.array();

    return ret;
}

bool Feature::m_saveDatabase()
{
    QJsonDocument jsonDataDoc(__database);

    QFile file(__filePath);
    bool ret1 = file.open(QIODevice::WriteOnly | QIODevice::Text);
    qint64 ret2 = file.write(jsonDataDoc.toJson());
    file.close();

    return ret1 && (ret2 != -1);
}


