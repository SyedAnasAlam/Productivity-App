#include <QJsonObject>
#include <QPushButton>
#include <QProgressBar>
#include <QLabel>
#include <QVBoxLayout>
#include <QDate>
#include <QLineEdit>
#include <QComboBox>
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

    QComboBox * completedHabitsComboBox = new QComboBox(this);

    for(int i = 0; i < __habits.size(); i++)
    {
        if(__habits[i].completed)
        {
            completedHabitsComboBox->addItem(__habits[i].description);
            continue;
        }

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

    QLabel * comboBoxLabel = new QLabel("Completed habits", this);

    QLineEdit * newHabitLineEdit = new QLineEdit(this);
    newHabitLineEdit->setPlaceholderText("New habit description");

    QPushButton * newHabitPushButton = new QPushButton("Add new habit", this);
    connect(newHabitPushButton, &QPushButton::clicked, this, [=]{ newHabitButton_clicked(newHabitLineEdit->text()); });

    layout->addWidget(newHabitPushButton);
    layout->addWidget(newHabitLineEdit);
    layout->addWidget(comboBoxLabel);
    layout->addWidget(completedHabitsComboBox);

    this->setLayout(layout);
}

void HabitTracker::display(QTabWidget * tabWidget)
{
    redraw();
    tabWidget->addTab(this, "Habit tracker");
}


void HabitTracker::streakButton_clicked(int habitIndex)
{
    int newStreak = __habits[habitIndex].streak + 1;

    if(__habits[habitIndex].startDay + newStreak < __calender->dayOfYear())
    {
        __habits.removeAt(habitIndex);
        __database.removeAt(habitIndex);
    }
    else
    {
        // Update value in vector
        __habits[habitIndex].streak = newStreak;

        // Update value in databse, and save
        QJsonObject jsonHabit = __database.at(habitIndex).toObject();
        jsonHabit["streak"] = QJsonValue::fromVariant(QVariant(newStreak));
        __database.replace(habitIndex, jsonHabit);

        if(newStreak == __MAX_STREAK)
        {
            __habits[habitIndex].completed = true;
            jsonHabit["completed"] = QJsonValue::fromVariant(true);
            __database.replace(habitIndex, jsonHabit);
        }
    }

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

