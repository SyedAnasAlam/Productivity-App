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
        a.startHour = __database.at(i)["startHour"].toInt();
        a.endHour = __database.at(i)["endHour"].toInt();

        __activities.append(a);
    }
}

void WeeklySchedule::display(QTabWidget * tabWidget)
{
    QVBoxLayout * layout = new QVBoxLayout(this);

    __tableWidget = new QTableWidget(DAY_END_HOUR - DAY_START_HOUR, 7, this);
    __tableWidget->setHorizontalHeaderLabels( {"Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday", "Sunday"} );
    __tableWidget->setVerticalHeaderLabels( {"8", "9", "10", "11", "12", "13", "14", "15", "16", "17", "18", "19", "20"} );

    __newActivityLayout = new QHBoxLayout(this);
    __newActivityLineEdit = new QLineEdit(this);
    __dayComboBox = new QComboBox(this);
    __startHourSpinBox = new QSpinBox(this);
    __endHourSpinBox = new QSpinBox(this);
    __addActivityPushButton = new QPushButton("Add", this);
    connect(__addActivityPushButton, &QPushButton::clicked, this, &WeeklySchedule::newActivityPushButton_clicked);

    __newActivityLayout->addWidget(__newActivityLineEdit);
    __newActivityLayout->addWidget(__dayComboBox);
    __dayComboBox->addItem("Monday");
    __dayComboBox->addItem("Tuesday");
    __dayComboBox->addItem("Wednesday");
    __dayComboBox->addItem("Thursday");
    __dayComboBox->addItem("Friday");
    __dayComboBox->addItem("Saturday");
    __dayComboBox->addItem("Sunday");
    __newActivityLayout->addWidget(__startHourSpinBox);
    __newActivityLayout->addWidget(__endHourSpinBox);
    __newActivityLayout->addWidget(__addActivityPushButton);

    for(activity & a : __activities)
    {
        QTableWidgetItem * item = new QTableWidgetItem(a.description);
        for(int i = a.startHour; i <= a.endHour; i++)
            __tableWidget->setItem(i - DAY_START_HOUR, a.day, item->clone());
    }

    layout->addWidget(__tableWidget);
    layout->addLayout(__newActivityLayout);

    this->setLayout(layout);
    tabWidget->addTab(this, "Weekly Schedule");
}

void WeeklySchedule::newActivityPushButton_clicked()
{
    activity newActivity;
    newActivity.description = __newActivityLineEdit->text();
    newActivity.day = __dayComboBox->currentIndex();
    newActivity.startHour = __startHourSpinBox->value();
    newActivity.endHour = __endHourSpinBox->value();

    QJsonObject jsonNewActivity;
    jsonNewActivity["description"] = newActivity.description;
    jsonNewActivity["day"] = newActivity.day;
    jsonNewActivity["startHour"]  = newActivity.startHour;
    jsonNewActivity["endHour"] = newActivity.endHour;

    bool isOverlapping = false;

    for(int i = 0; i < __activities.size(); i++)
    {

        isOverlapping =(newActivity.day == __activities[i].day) && (
                            (newActivity.startHour >= __activities[i].startHour && newActivity.endHour <= __activities[i].endHour) ||
                            (newActivity.startHour <= __activities[i].startHour && newActivity.endHour <= __activities[i].endHour) ||
                            (newActivity.startHour >= __activities[i].startHour && newActivity.endHour >= __activities[i].endHour));
        if(isOverlapping)
        {
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

    QTableWidgetItem * item = new QTableWidgetItem(newActivity.description);
    for(int i = newActivity.startHour; i <= newActivity.endHour; i++)
        __tableWidget->setItem(i - DAY_START_HOUR, newActivity.day, item->clone());
}
