#ifndef IMPOSSIBLELIST_H
#define IMPOSSIBLELIST_H

#include <QWidget>
#include <QListWidget>
#include <QTabWidget>
#include <QDialog>
#include <QColor>
#include "database.h"

struct goal
{
    QString description;
    bool completed;
    bool allSubGoalsDone;
    bool focused;
    bool isParent;
    QVector<goal> subGoals;
};

class ImpossibleList : public Database, public QWidget
{

public:
    ImpossibleList(QWidget * parent);
    void display(QTabWidget * tabWidget);

private:
    QString displayDialog();
    QVector<goal> __goals;

    QListWidget * __goalsListWidget;
    QLineEdit * __newGoalLineEdit;
    QDialog * completedGoalDialog;
    QDialog * __historyDialog;

    QColor * __focusedColor = new QColor(0, 0, 255);
    QColor * __completedColor = new QColor(0, 255, 0);


private slots:
    void focusButton_clicked();
    void completeButton_clicked();
    void subGoalsButton_clicked();
    void newGoalButton_clicked();

};


#endif // IMPOSSIBLELIST_H
