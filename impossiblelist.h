#ifndef IMPOSSIBLELIST_H
#define IMPOSSIBLELIST_H

#include <QWidget>
#include <QListWidget>
#include <QTabWidget>
#include <QDialog>
#include <QColor>
#include <QVBoxLayout>
#include "feature.h"

struct goal
{
    QString description;
    bool completed;
    bool allSubGoalsDone;
    bool focused;
    bool isParent;
    QVector<goal> subGoals;
};

class ImpossibleList : public Feature, public QWidget
{

public:
    ImpossibleList(QWidget * parent);
    void v_display(QTabWidget * tabWidget);

private:
    QDialog * __completedGoalDialog;
    QDialog * __historyDialog;

    QString v_displayNewGoalDialog();

    QVector<goal> __goals;

    QListWidget * __goalsListWidget;
    QLineEdit * __newGoalLineEdit;

private slots:
    void focusButton_clicked();
    void completeButton_clicked();
    void subGoalsButton_clicked();
    void newGoalButton_clicked();

};


#endif // IMPOSSIBLELIST_H
