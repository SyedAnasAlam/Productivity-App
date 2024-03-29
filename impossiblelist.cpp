#include <QVBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QJsonObject>
#include <QDialog>
#include <QLabel>
#include <QIcon>
#include "impossiblelist.h"

ImpossibleList::ImpossibleList(QWidget * parent) : Feature("ImpossibleList"), QWidget(parent)
{
    m_openDatabase();

    for(int i = 0; i < __database.size(); i++)
    {
        goal g;
        g.description = __database.at(i)["description"].toString();
        g.completed = __database.at(i)["completed"].toBool();
        g.allSubGoalsDone = __database.at(i)["subGoalsDone"].toBool();
        g.focused = __database.at(i)["focused"].toBool();
        g.isParent = __database.at(i)["isParent"].toBool();
        QJsonArray jsonSubGoals = __database.at(i)["subGoals"].toArray();
        for(int j = 0; j < jsonSubGoals.size(); j++)
        {
            goal sg;
            sg.description = jsonSubGoals.at(j)["description"].toString();
            sg.completed = jsonSubGoals.at(j)["completed"].toBool();
            sg.focused = jsonSubGoals.at(j)["focused"].toBool();
            sg.isParent = jsonSubGoals.at(j)["isParent"].toBool();
            sg.allSubGoalsDone = jsonSubGoals.at(j)["subGoalsDone"].toBool();
            sg.subGoals = QVector<goal>();
            g.subGoals.append(sg);
        }

        __goals.append(g);
    }
}

void ImpossibleList::v_display(QTabWidget * tabWidget)
{
    QVBoxLayout * layout = new QVBoxLayout(this);

    __goalsListWidget = new QListWidget(this);
    __goalsListWidget->setObjectName("goalsListWidget");

    QIcon completedIcon(":/resources/checkbox-circle-fill.png");
    QIcon focusedIcon(":/resources/arrow-right-circle-fill.png");

    for(goal & g : __goals)
    {
        QListWidgetItem * listWidgetItem = new QListWidgetItem();

        if(g.allSubGoalsDone)
        {
            listWidgetItem->setText(g.subGoals.length() > 0 ? g.subGoals.last().description : g.description);
            listWidgetItem->setIcon(completedIcon);
        }
        else if(g.completed)
        {
            int i;
            for(i = 0; g.subGoals.at(i).completed; i++);

            listWidgetItem->setText(g.subGoals.at(i).description);

            if(g.subGoals.at(i).focused)
                listWidgetItem->setIcon(focusedIcon);
        }
        else
        {
           listWidgetItem->setText(g.description);

           if(g.focused)
             listWidgetItem->setIcon(focusedIcon);
        }

        __goalsListWidget->addItem(listWidgetItem);
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
    __newGoalLineEdit->setPlaceholderText("New goal description");
    QPushButton * newGoalButton = new QPushButton("Add new goal", this);
    connect(newGoalButton, &QPushButton::clicked, this, &ImpossibleList::newGoalButton_clicked);
    newGoalLayout->addWidget(__newGoalLineEdit);
    newGoalLayout->addWidget(newGoalButton);

    layout->addWidget(__goalsListWidget);
    layout->addLayout(buttonsLayout);
    layout->addLayout(newGoalLayout);

    this->setLayout(layout);
    tabWidget->addTab(this, "Impossible List");
}

QString ImpossibleList::v_displayNewGoalDialog()
{
    __completedGoalDialog = new QDialog(this);

    QVBoxLayout * dialogLayout = new QVBoxLayout(this);
    QLineEdit * newSubGoalLineEdit = new QLineEdit(this);
    newSubGoalLineEdit->setPlaceholderText("New subgoal description");
    dialogLayout->addWidget(newSubGoalLineEdit);

    QHBoxLayout * buttonsLayout = new QHBoxLayout(this);
    QPushButton * acceptButton = new QPushButton("Add new subgoal", this);
    QPushButton * rejectButton =new QPushButton("No not add new subgoal", this);
    buttonsLayout->addWidget(acceptButton);
    buttonsLayout->addWidget(rejectButton);

    dialogLayout->addLayout(buttonsLayout);
    __completedGoalDialog->setLayout(dialogLayout);

    connect(acceptButton, &QPushButton::clicked, __completedGoalDialog, &QDialog::accept);
    connect(rejectButton, &QPushButton::clicked, __completedGoalDialog, &QDialog::reject);

    int ret = __completedGoalDialog->exec();

    return ret == QDialog::Accepted ? newSubGoalLineEdit->text() : "";
}

void ImpossibleList::completeButton_clicked()
{
    int index = __goalsListWidget->currentRow();
    if(index == -1)
        return;

    goal selectedGoal = __goals.at(index);
    if(selectedGoal.allSubGoalsDone)
        return;

    QString newSubGoalDescription = v_displayNewGoalDialog();

    if(selectedGoal.completed)
    {
        goal sg = selectedGoal.subGoals.at(0);
        int i;
        for(i = 0; selectedGoal.subGoals.at(i).completed; i++);

        sg = selectedGoal.subGoals.at(i);
        sg.completed = true;
        sg.focused = false;
        selectedGoal.subGoals.replace(i, sg);
        __goals.replace(index, selectedGoal);

    }
    else
    {
        selectedGoal.completed = true;
        selectedGoal.focused = false;
    }

    if(newSubGoalDescription != "")
    {
        goal newSubGoal;
        newSubGoal.completed = false;
        newSubGoal.focused = false;
        newSubGoal.description = newSubGoalDescription;
        newSubGoal.allSubGoalsDone = false;
        newSubGoal.isParent = false;
        newSubGoal.subGoals = QVector<goal>();
        selectedGoal.subGoals.append(newSubGoal);
    }
    else
    {
        selectedGoal.allSubGoalsDone = true;
    }

    __goals.replace(index, selectedGoal);

    QJsonObject jsonGoal =  __database.at(index).toObject();
    QJsonArray jsonSubGoals = jsonGoal["subGoals"].toArray();
    QJsonObject jsonSubGoal;

    if(__database.at(index)["completed"].toBool())
    {
        int sgIndex;
        for(sgIndex = 0; selectedGoal.subGoals.at(sgIndex).completed; sgIndex++);
        sgIndex--;

        jsonSubGoal = jsonSubGoals.at(sgIndex).toObject();
        jsonSubGoal["completed"] = true;
        jsonSubGoal["focused"] = false;
        jsonSubGoals.replace(sgIndex, jsonSubGoal);
    }
    else
    {
        jsonGoal["completed"] = true;
        jsonGoal["focused"] = false;
    }

    if(newSubGoalDescription != "")
    {
        QJsonObject jsonNewSubGoal;
        jsonNewSubGoal["completed"] = false;
        jsonNewSubGoal["focused"] = false;
        jsonNewSubGoal["description"] = newSubGoalDescription;
        jsonNewSubGoal["isParent"] = false;
        jsonNewSubGoal["subGoalsDone"] = false;
        jsonNewSubGoal["subGoals"] = QJsonValue();
        jsonSubGoals.append(jsonNewSubGoal);
    }
    else
    {
        jsonGoal["subGoalsDone"] = true;

        if(jsonSubGoal.length() > 0)
        {
            int sgIndex;
            for(sgIndex = 0; selectedGoal.subGoals.at(sgIndex).completed; sgIndex++);

            jsonSubGoal = jsonSubGoals.at(sgIndex).toObject();
            jsonSubGoal["completed"] = true;
            jsonSubGoals.replace(sgIndex, jsonSubGoal);
        }

    }

    jsonGoal["subGoals"] = jsonSubGoals;
    __database.replace(index, jsonGoal);
    m_saveDatabase();

    if(newSubGoalDescription != "")
    {
        __goalsListWidget->currentItem()->setText(newSubGoalDescription);
        __goalsListWidget->currentItem()->setIcon(QIcon());
    }
    else
    {
        __goalsListWidget->currentItem()->setIcon(QIcon(":/resources/checkbox-circle-fill.png"));
    }
}

void ImpossibleList::focusButton_clicked()
{
    int parentIndex = __goalsListWidget->currentRow();
    if(parentIndex == -1)
        return;

    if(__goals.at(parentIndex).allSubGoalsDone)
        return;

    goal parentGoal = __goals.at(parentIndex);
    int childIndex = -1;
    bool newVal;
    if(parentGoal.completed)
    {
        int i;
        for(i = 0; parentGoal.subGoals.at(i).completed; i++);
        childIndex = i;
        goal childGoal = parentGoal.subGoals.at(childIndex);
        newVal = !childGoal.focused;
        childGoal.focused = newVal;
        parentGoal.subGoals.replace(childIndex, childGoal);
    }
    else
    {
        newVal = !parentGoal.focused;
        parentGoal.focused = newVal;
    }
    __goals.replace(parentIndex, parentGoal);

    __goalsListWidget->currentItem()->setIcon(newVal ? QIcon(":/resources/arrow-right-circle-fill.png") : QIcon());

    QJsonObject jsonGoal =  __database.at(parentIndex).toObject();
    QJsonArray jsonSubGoals = jsonGoal["subGoals"].toArray();
    QJsonObject jsonSubGoal;

    if(jsonGoal["completed"].toBool())
    {
        if(childIndex == -1) return;
        jsonSubGoal = jsonSubGoals.at(childIndex).toObject();
        jsonSubGoal["focused"] = newVal;
        jsonSubGoals.replace(childIndex, jsonSubGoal);
    }
    else
    {
        jsonGoal["focused"] = newVal;
    }

    jsonGoal["subGoals"] = jsonSubGoals;
    __database.replace(parentIndex, jsonGoal);
    m_saveDatabase();
}

void ImpossibleList::subGoalsButton_clicked()
{
    int index = __goalsListWidget->currentRow();
    if(index == -1)
        return;

    __historyDialog = new QDialog(this);
    QVBoxLayout * layout = new QVBoxLayout(this);
    QListWidget * list = new QListWidget(this);
    list->setObjectName("completedGoalsList");
    layout->addWidget(list);
    __historyDialog->setLayout(layout);

    goal selectedGoal = __goals.at(index);
    list->addItem(selectedGoal.description);

    for(goal & subgoal : selectedGoal.subGoals)
    {
        if(!subgoal.completed)
            break;
        list->addItem(subgoal.description);
    }

    __historyDialog->exec();

}

void ImpossibleList::newGoalButton_clicked()
{
    goal newGoal;
    newGoal.description = __newGoalLineEdit->text();

    if(newGoal.description.length() <= 0)
        return;

    newGoal.completed = false;
    newGoal.focused = false;
    newGoal.allSubGoalsDone = false;
    newGoal.isParent = true;
    QVector<goal> subGoals = {};
    newGoal.subGoals = subGoals;
    __goals.append(newGoal);

    QJsonObject newJsonGoal;
    newJsonGoal["description"] = newGoal.description;
    newJsonGoal["completed"] = false;
    newJsonGoal["focused"] = false;
    newJsonGoal["subGoalsDone"] = false;
    newJsonGoal["isParent"] = true,
    newJsonGoal["subGoals"] = QJsonValue();

    __database.append(newJsonGoal);
    m_saveDatabase();

    __goalsListWidget->addItem(newGoal.description);

    __newGoalLineEdit->setText("");
}


