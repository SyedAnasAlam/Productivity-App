#include <QJsonArray>
#include <QVBoxLayout>
#include <QTabWidget>
#include <QListWidgetItem>
#include "Constants.h"
#include "todolist.h"

TodoList::TodoList(QWidget * parent) : Feature("TodoList"), QWidget(parent)
{
    m_openDatabase();
    for(QVariant & v : __database.toVariantList())
        __todoList.append(v.toString());
}

bool TodoList::m_addTask(const QString & taskDescription)
{
    __database.append(taskDescription);
    bool ret = m_saveDatabase();
    return ret;
}

bool TodoList::m_completeTask(int taskIndex)
{
    __database.removeAt(taskIndex);
    bool ret = m_saveDatabase();
    return ret;
}

void TodoList::v_display(QTabWidget * tabWidget)
{
    QVBoxLayout * layout = new QVBoxLayout();

    __listWidget = new QListWidget(this);
    __listWidget->setObjectName("todoListWidget");

    layout->addWidget(__listWidget);
    __listWidget->addItems(__todoList);

    __addTaskButton = new QPushButton("Add new task", this);
    layout->addWidget(__addTaskButton);

    __lineEdit = new QLineEdit(this);
    __lineEdit->setPlaceholderText("New task description");
    layout->addWidget(__lineEdit);

    tabWidget->addTab(this, "Todo List");

    connect(__addTaskButton, &QPushButton::clicked, this, &TodoList::addTaskButton_clicked);
    connect(__listWidget, &QListWidget::itemDoubleClicked, this, &TodoList::tabWidgetItem_double_clicked);

    this->setLayout(layout);

}

void TodoList::addTaskButton_clicked()
{
    QString newTaskDescription = __lineEdit->text();

    if(newTaskDescription.length() <= 0)
        return;

    m_addTask(newTaskDescription);
    __listWidget->addItem(newTaskDescription);
    __lineEdit->setText("");
}

void TodoList::tabWidgetItem_double_clicked()
{
    int taskIndex = __listWidget->currentRow();
    m_completeTask(taskIndex);
    __listWidget->takeItem(taskIndex);
}


