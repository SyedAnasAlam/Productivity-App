#include <QJsonObject>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QGraphicsEllipseItem>
#include <QGraphicsRectItem>
#include <QGraphicsTextItem>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QBrush>
#include <QLabel>
#include <QtMath>
#include <QResizeEvent>
#include <qevent.h>
#include "habittracker.h"


HabitTracker::HabitTracker(QWidget *parent) : Database("HabitTracker"), QWidget(parent)
{
    openDatabase();
    for(QJsonValue && v : __database)
    {
        habit h;
        h.description = v["description"].toString();
        h.completed = v["completed"].toBool();
        h.startDay = v["startDay"].toInt();
        h.streak = v["streak"].toInt();
        __habits.append(h);
    }
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
    QGridLayout * layout = new QGridLayout();

    QBrush brush;
    brush.setColor(*__PIE_COLOR);
    brush.setStyle(Qt::BrushStyle::SolidPattern);

    int areaScreen = this->width() * this->height();
    int count = __habits.size();

    for(int i = 0; i < __habits.size(); i++)
    {
        QLabel * descriptionLabel = new QLabel(__habits[i].description + " - " + QString::number(__habits[i].streak) + " / " + QString::number(66), this);
        descriptionLabel->setAlignment(Qt::AlignCenter);
        //QLabel * streakLabel = new QLabel(QString::number(__habits[i].streak), this);
        //streakLabel->setAlignment(Qt::AlignCenter);

        QPushButton * streakButton = new QPushButton("+", this);
        connect(streakButton, &QPushButton::clicked, this, [=](){ streakButton_clicked(i); });

        QVBoxLayout * subLayout = new QVBoxLayout();
        QGraphicsView * graphicsView = new QGraphicsView(this);
        graphicsView->setFrameShape(QFrame::NoFrame);
        QGraphicsScene * scene = new QGraphicsScene(graphicsView);

        int width = this->width();
        int height = (this->height() - descriptionLabel->height() - streakButton->height())/__habits.size();
        int radius = width < height ? width/2 : height/2;

        QGraphicsRectItem * r = new QGraphicsRectItem(0, 0, width, height);
        QGraphicsEllipseItem * contour = new QGraphicsEllipseItem(0, 0, radius, radius);
        QGraphicsEllipseItem * pie = new QGraphicsEllipseItem(0, 0, radius, radius);
        pie->setStartAngle(0);
        int angle = (__habits[i].streak/__MAX_STREAK) * 360;
        pie->setSpanAngle(angle*16);
        pie->setBrush(brush);
        r->setBrush(brush);

        //scene->addItem(contour);
        //scene->addItem(pie);
        scene->addItem(r);

        graphicsView->setScene(scene);
        subLayout->addWidget(graphicsView);
        subLayout->addWidget(descriptionLabel);
        //subLayout->addWidget(streakLabel);
        subLayout->addWidget(streakButton);
        layout->addLayout(subLayout, i, 0);
    }

    this->setLayout(layout);
}

void HabitTracker::draw(QTabWidget * parent)
{
    drawGraphics();
    parent->addTab(this, "Habit tracker");
}

void HabitTracker::resizeEvent(QResizeEvent * e)
{
    qDeleteAll(this->children());
    drawGraphics();
}

void HabitTracker::streakButton_clicked(int habitIndex)
{
    QJsonObject jsonHabit = __database.at(habitIndex).toObject();
    jsonHabit["streak"] = QJsonValue::fromVariant(QVariant(jsonHabit["streak"].toInt() + 1));
    __database.replace(habitIndex, jsonHabit);
    __habits[habitIndex].streak++;
    saveDatabase();

    qDeleteAll(this->children());
    drawGraphics();
}

