#ifndef HABITTRACKER_H
#define HABITTRACKER_H

#include "database.h"

struct habit
{
    QString description;
    int startDay;
    int streak;
    bool completed;
};

class HabitTracker : public Database
{

public:
    HabitTracker();
    QVector<habit> getHabits();
    bool addHabit(const habit & habit);
};

#endif // HABITTRACKER_H
