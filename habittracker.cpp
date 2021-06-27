#include <QJsonObject>
#include <QPushButton>
#include <QProgressBar>
#include <QLabel>
#include <QVBoxLayout>
#include <QDate>
#include <QLineEdit>
#include "habittracker.h"


HabitTracker::HabitTracker(QWidget *parent) : Database("HabitTracker"), QWidget(parent)
{
    openDatabase();

    __calender = new QDate(QDate::currentDate());

    for(int i = 0; i < __database.size(); i++)
    {
        habit h;
        h.description = __database.at(i)["description"].toString();
        h.completed = __database.at(i)["completed"].toBool();
        h.startDay = __database.at(i)["startDay"].toInt();
        h.streak = __database.at(i)["streak"].toInt();
        if(h.startDay + h.streak < __calender->dayOfYear())
            __database.removeAt(i);
        else
            __habits.append(h);
    }
    saveDatabase();
}

bool HabitTracker::addHabit(const habit & habit)
{
    QJsonObject jsonHabit;
    jsonHabit["description"] = habit.description;
    jsonHabit["completed"] = habit.completed;
    jsonHabit["startDay"] = habit.startDay;
    jsonHabit["streak"] = habit.streak;
    __database.append(jsonHabit);
    bool ret = saveDatabase();

    return ret;
}


void HabitTracker::redraw()
{
    qDeleteAll(this->children());

    QVBoxLayout * layout = new QVBoxLayout(this);

    for(int i = 0; i < __habits.size(); i++)
    {
        QHBoxLayout * sublayout = new QHBoxLayout();

        QString descriptionString = QString("%1 - %2 / %3").arg(__habits[i].description, QString::number(__habits[i].streak), QString::number(__MAX_STREAK));
        QProgressBar * progressBar = new QProgressBar(this);
        progressBar->setRange(0, __MAX_STREAK);
        progressBar->setValue(__habits[i].streak);
        progressBar->setFormat(descriptionString);
        progressBar->setAlignment(Qt::AlignCenter);
        progressBar->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

        QPushButton * streakButton = new QPushButton("+", this);
        streakButton->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Expanding);
        connect(streakButton, &QPushButton::clicked, this, [=](){ streakButton_clicked(i); });

        sublayout->addWidget(progressBar);
        sublayout->addWidget(streakButton);

        layout->addLayout(sublayout);
    }
    QLineEdit * newHabitLineEdit = new QLineEdit(this);
    newHabitLineEdit->setPlaceholderText("New habit description");

    QPushButton * newHabitPushButton = new QPushButton("Add new habit", this);
    connect(newHabitPushButton, &QPushButton::clicked, this, [=]{ newHabitButton_clicked(newHabitLineEdit->text()); });

    layout->addWidget(newHabitPushButton);
    layout->addWidget(newHabitLineEdit);

    this->setLayout(layout);
}

void HabitTracker::display(QTabWidget * parent)
{
    redraw();
    parent->addTab(this, "Habit tracker");
}


void HabitTracker::streakButton_clicked(int habitIndex)
{
    int newStreak = __habits[habitIndex].streak + 1;

    // Update value in vector
    __habits[habitIndex].streak = newStreak;

    // Update value in databse, and save
    QJsonObject jsonHabit = __database.at(habitIndex).toObject();
    jsonHabit["streak"] = QJsonValue::fromVariant(QVariant(newStreak));
    __database.replace(habitIndex, jsonHabit);
    saveDatabase();

    redraw();
}


void HabitTracker::newHabitButton_clicked(QString newHabitDescription)
{
    habit newHabit;
    newHabit.description = newHabitDescription;
    newHabit.completed = false;
    newHabit.startDay = __calender->dayOfYear();
    newHabit.streak = 0;
    addHabit(newHabit);
    __habits.append(newHabit);

    redraw();
}

