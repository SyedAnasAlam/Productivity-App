#ifndef WEEKLYSCHEDULE_H
#define WEEKLYSCHEDULE_H

#include <QWidget>
#include <QTableWidget>
#include <QTabWidget>
#include <QVBoxLayout>
#include <QComboBox>
#include <QLineEdit>
#include <QSpinBox>
#include <QPushButton>
#include <QDateTime>
#include "feature.h"

struct activity
{
    QString description;
    int day;
    int hour;
};

class WeeklySchedule : public Feature, public QWidget
{

public:
    WeeklySchedule(QWidget * parent);
    void v_display(QTabWidget * tabWidget);

private:
    QVector<activity> __activities;

    QTableWidget * __tableWidget;
    const int DAY_START_HOUR = 5;
    const int DAY_END_HOUR = 22;

    QDateTime * __dateTime;

    void updateSchedule();

private slots:
    void newActivityPushButton_clicked();
    void newActivityTableWidget_changed(int row, int column);

};

#endif // WEEKLYSCHEDULE_H
