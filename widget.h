#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include "todolist.h"
#include "habittracker.h"

#include <QResizeEvent>

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private:
    TodoList __todolist;
    HabitTracker* __habitTracker = new HabitTracker(this);


};

#endif // WIDGET_H
