#ifndef HABITTRACKER_H
#define HABITTRACKER_H

#include <QTabWidget>
#include <QGridLayout>
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
    HabitTracker();
    void draw(QTabWidget * parent);

private:
    QVector<habit> getHabits();
    bool addHabit(const habit & habit);
    void drawGraphics();
    const int __radius = 200;
    QGridLayout * __layout = new QGridLayout();

private slots:
    void streakButton_clicked(int habitIndex);
};

#endif // HABITTRACKER_H
