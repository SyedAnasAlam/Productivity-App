#ifndef IMPOSSIBLELIST_H
#define IMPOSSIBLELIST_H

#include <QWidget>
#include <QListWidget>
#include <QTabWidget>
#include <QDialog>
#include "database.h"

struct impossibleSubGoal
{
    QString description;
    bool completed;
    bool focused;
};

struct impossibleGoal
{
    QString description;
    bool completed;
    bool allSubGoalsDone;
    bool focused;
    QVector<impossibleSubGoal> subGoals;
};

class ImpossibleList : public Database, public QWidget
{

public:
    ImpossibleList(QWidget * parent);
    void display(QTabWidget * tabWidget);

private:
    QString displayDialog();
    QVector<impossibleGoal> __goals;
    QVector<impossibleGoal> __completedGoals;

    QListWidget * __goalsListWidget;
    QListWidget * __completedGoalsListWidget;
    QLineEdit * __newGoalLineEdit;
    QDialog * completedGoalDialog;



private slots:
    void focusButton_clicked();
    void completeButton_clicked();
    void subGoalsButton_clicked();
    void newGoalButton_clicked();

};


#endif // IMPOSSIBLELIST_H
