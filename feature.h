#ifndef FEATURE_H
#define FEATURE_H

#include <QString>
#include <QJsonArray>
#include <QTabWidget>

class Feature
{


public:
    Feature(const QString & databaseName);

    bool m_openDatabase();
    bool m_saveDatabase();
    virtual void v_display(QTabWidget * parentWidget) = 0;

    static QString __databasePath;

protected:
    QJsonArray __database;

private:
    QString __filePath;


};

#endif // FEATURE_H
