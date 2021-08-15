#include <QJsonObject>
#include <QPushButton>
#include <QProgressBar>
#include <QLabel>
#include <QVBoxLayout>
#include <QDate>
#include <QLineEdit>
#include <QComboBox>
#include <QDialog>
#include <QListWidget>
#include "habittracker.h"

HabitTracker::HabitTracker(QWidget *parent) : Feature("HabitTracker"), QWidget(parent)
{
    m_openDatabase();
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

    m_saveDatabase();
}

bool HabitTracker::m_addHabit(const habit & habit)
{
    QJsonObject jsonHabit;
    jsonHabit["description"] = habit.description;
    jsonHabit["completed"] = habit.completed;
    jsonHabit["startDay"] = habit.startDay;
    jsonHabit["streak"] = habit.streak;
    __database.append(jsonHabit);
    bool ret = m_saveDatabase();

    return ret;
}


void HabitTracker::v_redraw()
{
    qDeleteAll(this->children());

    QVBoxLayout * layout = new QVBoxLayout(this);

    for(int i = 0; i < __habits.size(); i++)
    {
        if(__habits.at(i).completed)
            continue;

        QHBoxLayout * sublayout = new QHBoxLayout();

        QString descriptionString = QString("%1   |   day %2   |   %3 days left").arg(__habits[i].description, QString::number(__habits[i].streak), QString::number(__MAX_STREAK - __habits[i].streak));
        QProgressBar * progressBar = new QProgressBar(this);
        progressBar->setRange(0, __MAX_STREAK);
        progressBar->setValue(__habits[i].streak);
        progressBar->setFormat(descriptionString);
        progressBar->setAlignment(Qt::AlignCenter);
        progressBar->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

        QPushButton * streakButton = new QPushButton("+", this);
        streakButton->setObjectName("streakButton");
        connect(streakButton, &QPushButton::clicked, this, [=](){ streakButton_clicked(i); });

        sublayout->addWidget(progressBar);
        sublayout->addWidget(streakButton);

        layout->addLayout(sublayout);
    }

    QHBoxLayout * newHabitLayout = new QHBoxLayout(this);

    QLineEdit * newHabitLineEdit = new QLineEdit(this);
    newHabitLineEdit->setPlaceholderText("New habit description");

    QPushButton * newHabitPushButton = new QPushButton("Add new habit", this);
    connect(newHabitPushButton, &QPushButton::clicked, this, [=]{ newHabitButton_clicked(newHabitLineEdit->text()); });

    newHabitLayout->addWidget(newHabitLineEdit);
    newHabitLayout->addWidget(newHabitPushButton);

    QPushButton * completedHabitsButton = new QPushButton("Show completed habits", this);
    connect(completedHabitsButton, &QPushButton::clicked, this, &HabitTracker::completedHabitButton_clicked);

    layout->addLayout(newHabitLayout);
    layout->addWidget(completedHabitsButton);

    this->setLayout(layout);
}

void HabitTracker::v_display(QTabWidget * tabWidget)
{
    v_redraw();
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

    m_saveDatabase();
    v_redraw();
}


void HabitTracker::newHabitButton_clicked(QString newHabitDescription)
{
    habit newHabit;
    newHabit.description = newHabitDescription;
    newHabit.completed = false;
    newHabit.startDay = __calender->dayOfYear();
    newHabit.streak = 0;

    if(newHabitDescription.length() <= 0)
        return;

    m_addHabit(newHabit);
    __habits.append(newHabit);

    v_redraw();
}

void HabitTracker::completedHabitButton_clicked()
{
    QDialog * completedHabitsDialog = new QDialog(this);
    QVBoxLayout * layout = new QVBoxLayout(this);
    QListWidget * list = new QListWidget(this);
    list->setObjectName("completedHabitsList");
    layout->addWidget(list);
    completedHabitsDialog->setLayout(layout);

    for(habit & h : __habits)
    {
        if(h.completed)
            list->addItem(h.description);
    }

    completedHabitsDialog->exec();
}

