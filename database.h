#ifndef DATABASE_H
#define DATABASE_H

#include <QString>
#include <QJsonArray>

class Database
{
public:
    Database(const QString & databaseName);
    bool openDatabase();
    bool saveDatabase();

protected:
    QJsonArray __database;

private:
    QString __databasePath;

};

#endif // DATABASE_H
