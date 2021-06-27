#ifndef HABITTRACKER_H
#define HABITTRACKER_H

#include <QTabWidget>
#include <QProgressBar>
#include <QDate>
#include "database.h"

struct habit
{
    QString description;
    int startDay;
    int streak;
    bool completed;
};

class HabitTracker : public Database, public QWidget
{

public:
    HabitTracker(QWidget * parent);
    void display(QTabWidget * tabWidget);
    void redraw();

private:
    bool addHabit(const habit & habit);
    QVector<habit> __habits;
    QDate * __calender;
    const int __MAX_STREAK = 66;


private slots:
    void streakButton_clicked(int habitIndex);
    void newHabitButton_clicked(QString newHabitDescription);

};

#endif // HABITTRACKER_H
