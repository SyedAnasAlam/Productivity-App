#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include "todolist.h"
#include "habittracker.h"
#include "weeklyschedule.h"
#include "impossiblelist.h"

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private:
    TodoList * __todolist = new TodoList(this);
    HabitTracker * __habitTracker = new HabitTracker(this);
    WeeklySchedule * __weeklySchedule = new WeeklySchedule(this);
    ImpossibleList * __impossibleList = new ImpossibleList(this);

};

#endif // WIDGET_H
