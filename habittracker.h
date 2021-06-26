#ifndef HABITTRACKER_H
#define HABITTRACKER_H

#include <QTabWidget>
#include <QColor>
#include <QResizeEvent>
#include <qevent.h>
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
    void draw(QTabWidget * parent);

private:
    bool addHabit(const habit & habit);
    void drawGraphics();
    QVector<habit> __habits;

    //Constants
    const int __RADIUS_MIN = 200;
    const int __MARGIN = 200;
    const QColor * __PIE_COLOR = new QColor(55, 31, 99);
    const float __MAX_STREAK = 66.0f;

    void resizeEvent(QResizeEvent * e);

private slots:
    void streakButton_clicked(int habitIndex);
};

#endif // HABITTRACKER_H
