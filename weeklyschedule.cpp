#include <QVBoxLayout>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QComboBox>
#include <QLineEdit>
#include <QSpinBox>
#include <QPushButton>
#include <QJsonObject>
#include "weeklyschedule.h"

WeeklySchedule::WeeklySchedule(QWidget * parent) : Database("WeeklySchedule"), QWidget(parent)
{
    openDatabase();
    for(int i = 0; i < __database.size(); i++)
    {
        activity a;
        a.description = __database.at(i)["description"].toString();
        a.day = __database.at(i)["day"].toInt();
        a.hour = __database.at(i)["hour"].toInt();

        __activities.append(a);
    }
}

void WeeklySchedule::display(QTabWidget * tabWidget)
{
    QVBoxLayout * layout = new QVBoxLayout(this);

    __tableWidget = new QTableWidget(DAY_END_HOUR - DAY_START_HOUR, 7, this);
    __tableWidget->setHorizontalHeaderLabels( {"Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday", "Sunday"} );

    QStringList verticalHeaderLabels;
    for(int i = DAY_START_HOUR; i <= DAY_END_HOUR; i++)
        verticalHeaderLabels.append( QString("%1:00").arg(QString::number(i)) );
    __tableWidget->setVerticalHeaderLabels(verticalHeaderLabels);

    for(activity & a : __activities)
    {
        QTableWidgetItem * item = new QTableWidgetItem(a.description);
        item->setTextAlignment(Qt::AlignCenter);
        __tableWidget->setItem(a.hour - DAY_START_HOUR, a.day, item);
    }

    connect(__tableWidget, &QTableWidget::cellChanged, this, &WeeklySchedule::newActivityTableWidget_changed);

    layout->addWidget(__tableWidget);

    this->setLayout(layout);
    this->setMinimumHeight(550);
    tabWidget->addTab(this, "Weekly Schedule");
}

void WeeklySchedule::newActivityTableWidget_changed(int row, int column)
{
    activity newActivity;
    newActivity.description = __tableWidget->item(row, column)->text();
    newActivity.day = column;
    newActivity.hour = row + DAY_START_HOUR;

    if(newActivity.description == "") return;

    QJsonObject jsonNewActivity;
    jsonNewActivity["description"] = newActivity.description;
    jsonNewActivity["day"] = newActivity.day;
    jsonNewActivity["hour"]  = newActivity.hour;

    bool isOverlapping = false;
    for(int i = 0; i < __activities.size(); i++)
    {
        if(__activities[i].day == column && __activities[i].hour == row)
        {
            isOverlapping = true;
            __activities[i] = newActivity;
            __database.replace(i, jsonNewActivity);
        }
    }
    if(!isOverlapping)
    {
        __activities.append(newActivity);
        __database.append(jsonNewActivity);
    }

    saveDatabase();
}
