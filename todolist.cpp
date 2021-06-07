#include <QJsonArray>
#include <QVBoxLayout>
#include <QTabWidget>
#include <QListWidgetItem>
#include "Constants.h"
#include "todolist.h"

//---Back-End---//
TodoList::TodoList() : Database("TodoList")
{}

QStringList TodoList::getTodoList()
{
    QStringList stringTodoList;
    for(QVariant & v : __database.toVariantList())
        stringTodoList.append(v.toString());

    return stringTodoList;
}

bool TodoList::addTask(const QString & taskDescription)
{
    __database.append(taskDescription);
    bool ret = saveDatabase();
    return ret;
}

bool TodoList::completeTask(int taskIndex)
{
    __database.removeAt(taskIndex);
    bool ret = saveDatabase();
    return ret;
}
//--------------//

//---Front-End---//
void TodoList::draw(QTabWidget *parent)
{
    QWidget * window = this;
    QVBoxLayout * layout = new QVBoxLayout();
    window->setLayout(layout);

    __listWidget = new QListWidget(window);
    layout->addWidget(__listWidget);
    __listWidget->addItems(getTodoList());

    __addTaskButton = new QPushButton("Add new task", window);
    layout->addWidget(__addTaskButton);

    __lineEdit = new QLineEdit(window);
    __lineEdit->setPlaceholderText("New task description");
    layout->addWidget(__lineEdit);

    window->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    parent->addTab(window, "Todo List");

    connect(__addTaskButton, &QPushButton::clicked, this, &TodoList::addTaskButton_clicked);
    connect(__listWidget, &QListWidget::itemDoubleClicked, this, &TodoList::tabWidgetItem_double_clicked);

}

void TodoList::addTaskButton_clicked()
{
    QString newTaskDescription = __lineEdit->text();
    addTask(newTaskDescription);
    __listWidget->addItem(newTaskDescription);
    __lineEdit->setText("");
}

void TodoList::tabWidgetItem_double_clicked()
{
    int taskIndex = __listWidget->currentRow();
    completeTask(taskIndex);
    __listWidget->takeItem(taskIndex);
}
//--------------//

