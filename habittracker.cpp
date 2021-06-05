#include <QJsonObject>
#include "habittracker.h"

HabitTracker::HabitTracker() : Database("HabitTracker")
{}

QVector<habit> HabitTracker::getHabits()
{
    QVector<habit> habits;

    for(QJsonValue v : __database)
    {
        habit h;
        h.description = v["description"].toString();
        h.completed = v["completed"].toBool();
        h.startDay = v["startDay"].toInt();
        h.streak = v["streak"].toInt();
        habits.append(h);
    }

    return habits;
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
