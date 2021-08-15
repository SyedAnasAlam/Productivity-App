#ifndef HABITTRACKER_H
#define HABITTRACKER_H

#include <QTabWidget>
#include <QProgressBar>
#include <QDate>
#include "feature.h"

struct habit
{
    QString description;
    int startDay;
    int streak;
    bool completed;
};

class HabitTracker : public Feature, public QWidget
{

public:
    HabitTracker(QWidget * parent);
    void v_display(QTabWidget * tabWidget);
    void v_redraw();

private:
    bool m_addHabit(const habit & habit);
    QVector<habit> __habits;
    QDate * __calender;
    const int __MAX_STREAK = 66;


private slots:
    void streakButton_clicked(int habitIndex);
    void newHabitButton_clicked(QString newHabitDescription);
    void completedHabitButton_clicked();

};

#endif // HABITTRACKER_H
