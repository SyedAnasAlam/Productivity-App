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
#include "database.h"

struct activity
{
    QString description;
    int day;
    int startHour;
    int endHour;
};

class WeeklySchedule : public Database, public QWidget
{

public:
    WeeklySchedule(QWidget * parent);
    void display(QTabWidget * tabWidget);

private:
    QVector<activity> __activities;

    QTableWidget * __tableWidget;
    QHBoxLayout * __newActivityLayout;
    QLineEdit * __newActivityLineEdit;
    QComboBox * __dayComboBox;
    QSpinBox * __startHourSpinBox;
    QSpinBox * __endHourSpinBox;
    QPushButton * __addActivityPushButton;

    const int DAY_START_HOUR = 8;
    const int DAY_END_HOUR = 20;

private slots:
    void newActivityPushButton_clicked();

};

#endif // WEEKLYSCHEDULE_H
