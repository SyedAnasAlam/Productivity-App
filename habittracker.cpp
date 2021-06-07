#include <QJsonObject>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QGraphicsEllipseItem>
#include <QGraphicsRectItem>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QBrush>
#include <QLabel>
#include "habittracker.h"


HabitTracker::HabitTracker() : Database("HabitTracker")
{}

QVector<habit> HabitTracker::getHabits()
{
    QVector<habit> habits;

    for(QJsonValue && v : __database)
    {
        habit h;
        h.description = v["description"].toString();
        h.completed = v["completed"].toBool();
        h.startDay = v["startDay"].toInt();
        h.streak = v["streak"].toInt();
        habits.append(h);
    }

    return habits;
}

bool HabitTracker::addHabit(const habit & habit)
{
    QJsonObject jsonHabit;
    jsonHabit["description"] = habit.description;
    jsonHabit["completed"] = habit.completed;
    jsonHabit["startDay"] = habit.startDay;
    jsonHabit["streak"] = habit.streak;
    __database.append(jsonHabit);
    bool ret = saveDatabase();

    return ret;
}

void HabitTracker::drawGraphics()
{
    QVector<habit> habits = getHabits();

    QBrush brush;
    brush.setColor(Qt::green);
    brush.setStyle(Qt::BrushStyle::SolidPattern);

    QBrush brush2;
    brush2.setColor(Qt::white);
    brush2.setStyle(Qt::BrushStyle::SolidPattern);

    for(int i = 0; i < habits.size(); i++)
    {
        QGraphicsView * graphicsView = new QGraphicsView(this);
        graphicsView->setFrameShape(QFrame::NoFrame);
        QGraphicsScene * scene = new QGraphicsScene(graphicsView);
        QGraphicsEllipseItem * ellipse1 = new QGraphicsEllipseItem(0, 0, __radius, __radius);
        QGraphicsEllipseItem * ellipse2 = new QGraphicsEllipseItem(0, 0, __radius, __radius);
        QVBoxLayout * subLayout = new QVBoxLayout();

        QGraphicsRectItem * rect = new QGraphicsRectItem(0, 0, scene->width(), scene->height());
        rect->setBrush(brush2);

        scene->addItem(rect);

        ellipse2->setBrush(brush);
        ellipse2->setStartAngle(0);
        int angle = (habits[i].streak/66.0f) * 360;
        ellipse2->setSpanAngle(angle*16);

        graphicsView->setScene(scene);
        scene->addItem(ellipse1);
        scene->addItem(ellipse2);
        subLayout->addWidget(graphicsView);

        QLabel * habitDescriptionLabel = new QLabel(habits[i].description, this);
        habitDescriptionLabel->setAlignment(Qt::AlignCenter);
        subLayout->addWidget(habitDescriptionLabel);

        QLabel * streakLabel = new QLabel(QString::number(habits[i].streak), this);
        streakLabel->setAlignment(Qt::AlignCenter);
        subLayout->addWidget(streakLabel);

        QPushButton * streakButton = new QPushButton("+", this);
        subLayout->addWidget(streakButton);
        connect(streakButton, &QPushButton::clicked, this, [=](){
            streakButton_clicked(i);

        });

        __layout->addLayout(subLayout, i, 0);
    }
}

void HabitTracker::draw(QTabWidget * parent)
{
    QWidget * window = this;
    window->setLayout(__layout);
    drawGraphics();
    parent->addTab(window, "Habit tracker");

}

void HabitTracker::streakButton_clicked(int habitIndex)
{
    QJsonObject jsonHabit = __database.at(habitIndex).toObject();
    jsonHabit["streak"] = QJsonValue::fromVariant(QVariant( jsonHabit["streak"].toInt() + 1 ));
    __database.replace(habitIndex, jsonHabit);
    saveDatabase();
    QLayoutItem* item;
    while ( ( item = this->layout()->takeAt( 0 ) ) != NULL )
    {
        delete item->widget();
        delete item;
    }
    drawGraphics();
}

