#include <QWidget>
#include <QVBoxLayout>
#include <QTabWidget>
#include <QApplication>
#include <QFile>
#include <QFontDatabase>

#include "todolist.h"
#include "habittracker.h"
#include "weeklyschedule.h"
#include "impossiblelist.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QWidget container;
    container.setObjectName("container");

    QVBoxLayout * layout = new QVBoxLayout();
    container.setLayout(layout);
    container.setMinimumWidth(1000);
    container.setMinimumHeight(610);
    QFontDatabase::addApplicationFont(":/resources/Roboto-Regular.ttf");

    QTabWidget * parentWidget = new QTabWidget(&container);
    layout->addWidget(parentWidget);

    QFile stylesheetSource(":/resources/stylesheet.qss");
    stylesheetSource.open(QFile::ReadOnly);
    QString stylesheet = QLatin1String(stylesheetSource.readAll());
    a.setStyleSheet(stylesheet);


    TodoList * __todolist = new TodoList(&container);
    HabitTracker * __habitTracker = new HabitTracker(&container);
    WeeklySchedule * __weeklySchedule = new WeeklySchedule(&container);
    ImpossibleList * __impossibleList = new ImpossibleList(&container);

    __todolist->v_display(parentWidget);
    __habitTracker->v_display(parentWidget);
    __weeklySchedule->v_display(parentWidget);
    __impossibleList->v_display(parentWidget);

    container.show();
    return a.exec();
}
