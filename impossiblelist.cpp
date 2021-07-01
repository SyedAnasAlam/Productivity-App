#include <QVBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QJsonObject>
#include <QDialog>
#include <QLabel>
#include "impossiblelist.h"

ImpossibleList::ImpossibleList(QWidget * parent) : Database("ImpossibleList"), QWidget(parent)
{
    openDatabase();

    for(int i = 0; i < __database.size(); i++)
    {
        impossibleGoal goal;
        goal.description = __database.at(i)["description"].toString();
        goal.completed = __database.at(i)["completed"].toBool();
        goal.allSubGoalsDone = __database.at(i)["subGoalsDone"].toBool();
        goal.focused = __database.at(i)["focused"].toBool();
        QJsonArray jsonSubGoals = __database.at(i)["subGoals"].toArray();
        for(int j = 0; j < jsonSubGoals.size(); j++)
        {
            impossibleSubGoal subGoal;
            subGoal.description = jsonSubGoals.at(j)["description"].toString();
            subGoal.completed = jsonSubGoals.at(j)["completed"].toBool();
            subGoal.focused = jsonSubGoals.at(j)["focused"].toBool();
            goal.subGoals.append(subGoal);
        }
        if(!goal.allSubGoalsDone)
            __goals.append(goal);
        else
            __completedGoals.append(goal);
    }
}

void ImpossibleList::display(QTabWidget * tabWidget)
{
    QVBoxLayout * layout = new QVBoxLayout(this);

    __goalsListWidget = new QListWidget(this);
    __completedGoalsListWidget = new QListWidget(this);

    for(int i = 0; i < __goals.size(); i++)
    {
        if(__goals.at(i).completed)
        {
            for(int j = 0; j < __goals.at(i).subGoals.size(); j++)
            {
                if(!__goals.at(i).subGoals.at(j).completed)
                {
                    __goalsListWidget->addItem(__goals.at(i).subGoals.at(j).description);
                    break;
                }
            }
        }
        else
        {
            __goalsListWidget->addItem(__goals.at(i).description);
        }
    }
    for(impossibleGoal completedGoal : __completedGoals)
    {
        __completedGoalsListWidget->addItem(completedGoal.description);
    }

    QHBoxLayout * buttonsLayout = new QHBoxLayout(this);
    QPushButton * focusButton = new QPushButton("Mark as focused", this);
    connect(focusButton, &QPushButton::clicked, this, &ImpossibleList::focusButton_clicked);
    QPushButton * completeButton = new QPushButton("Mark as completed", this);
    connect(completeButton, &QPushButton::clicked, this, &ImpossibleList::completeButton_clicked);
    QPushButton * subGoalsButton = new QPushButton("Show history", this);
    connect(subGoalsButton, &QPushButton::clicked, this, &ImpossibleList::subGoalsButton_clicked);
    buttonsLayout->addWidget(completeButton);
    buttonsLayout->addWidget(focusButton);
    buttonsLayout->addWidget(subGoalsButton);

    QHBoxLayout * newGoalLayout = new QHBoxLayout(this);
    __newGoalLineEdit = new QLineEdit(this);
    QPushButton * newGoalButton = new QPushButton("Add new goal", this);
    connect(newGoalButton, &QPushButton::clicked, this, &ImpossibleList::newGoalButton_clicked);
    newGoalLayout->addWidget(__newGoalLineEdit);
    newGoalLayout->addWidget(newGoalButton);

    layout->addWidget(__goalsListWidget);
    layout->addWidget(__completedGoalsListWidget);
    layout->addLayout(buttonsLayout);
    layout->addLayout(newGoalLayout);

    this->setLayout(layout);
    tabWidget->addTab(this, "Impossible List");
}

QString ImpossibleList::displayDialog()
{
    completedGoalDialog = new QDialog(this);

    QVBoxLayout * dialogLayout = new QVBoxLayout(this);
    QLabel * dialogLabel = new QLabel("Dialog description", this);
    QLineEdit * newSubGoalLineEdit = new QLineEdit(this);
    dialogLayout->addWidget(dialogLabel);
    dialogLayout->addWidget(newSubGoalLineEdit);

    QHBoxLayout * buttonsLayout = new QHBoxLayout(this);
    QPushButton * acceptButton = new QPushButton("Accept", this);
    QPushButton * rejectButton =new QPushButton("Reject", this);
    buttonsLayout->addWidget(acceptButton);
    buttonsLayout->addWidget(rejectButton);

    dialogLayout->addLayout(buttonsLayout);
    completedGoalDialog->setLayout(dialogLayout);

    connect(acceptButton, &QPushButton::clicked, completedGoalDialog, &QDialog::accept);
    connect(rejectButton, &QPushButton::clicked, completedGoalDialog, &QDialog::reject);

    int ret = completedGoalDialog->exec();

    return ret == QDialog::Accepted ? newSubGoalLineEdit->text() : "";
}

void ImpossibleList::completeButton_clicked()
{
    QString newSubGoalDescription = displayDialog();

    int index = __goalsListWidget->currentRow();
    impossibleGoal selectedGoal = __goals.at(index);
    if(selectedGoal.completed)
    {
        for(impossibleSubGoal & sg : selectedGoal.subGoals)
        {
            if(!sg.completed)
            {
                sg.completed = true;
                break;
            }
        }
    }
    else
    {
        selectedGoal.completed = true;
    }

    if(newSubGoalDescription != "")
    {
        impossibleSubGoal newSubGoal;
        newSubGoal.completed = false;
        newSubGoal.focused = false;
        newSubGoal.description = newSubGoalDescription;
        selectedGoal.subGoals.append(newSubGoal);
    }
    else
    {
        selectedGoal.allSubGoalsDone = true;
    }

    __goals.replace(index, selectedGoal);

    QJsonObject jsonGoal =  __database.at(index).toObject();
    QJsonArray a = jsonGoal["subGoals"].toArray();
    QJsonObject jsonSubGoal;

    if(__database.at(index)["completed"].toBool())
    {
        int subGoalIndex = 0;
        for(QJsonValueRef o : a)
        {
            if(!o.toObject()["completed"].toBool())
            {
                jsonSubGoal = o.toObject();
                jsonSubGoal["completed"] = true;
                a.replace(subGoalIndex, jsonSubGoal);
                break;
            }
            subGoalIndex++;
        }
    }
    else
    {
        jsonGoal["completed"] = QJsonValue::fromVariant(true);
    }

    if(newSubGoalDescription != "")
    {
        QJsonObject jsonNewSubGoal;
        jsonNewSubGoal["completed"] = false;
        jsonNewSubGoal["focused"] = false;
        jsonNewSubGoal["description"] = newSubGoalDescription;
        a.append(jsonNewSubGoal);
    }
    else
    {
        jsonGoal["subGoalsDone"] = true;
    }

    jsonGoal["subGoals"] = a;

    __database.replace(index, jsonGoal);

    saveDatabase();

    __goalsListWidget->takeItem(index);
    if(newSubGoalDescription != "")
    {
        __goalsListWidget->insertItem(index, newSubGoalDescription);
    }
    else
    {
        __completedGoalsListWidget->addItem(selectedGoal.description);
    }
}

void ImpossibleList::focusButton_clicked()
{

}

void ImpossibleList::subGoalsButton_clicked()
{

}

void ImpossibleList::newGoalButton_clicked()
{
    impossibleGoal newGoal;
    newGoal.description = __newGoalLineEdit->text();
    newGoal.completed = false;
    newGoal.focused = false;
    newGoal.allSubGoalsDone = false;
    QVector<impossibleSubGoal> subGoals = {};
    newGoal.subGoals = subGoals;
    __goals.append(newGoal);

    QJsonObject newJsonGoal;
    newJsonGoal["description"] = QJsonValue::fromVariant(newGoal.description);
    newJsonGoal["completed"] = QJsonValue::fromVariant(false);
    newJsonGoal["focused"] = QJsonValue::fromVariant(false);
    newJsonGoal["subGoalsDone"] = false;
    newJsonGoal["subGoals"] = QJsonValue();

    __database.append(newJsonGoal);

    saveDatabase();

    __goalsListWidget->addItem(newGoal.description);
}


