#include "roster.h"

Roster::Roster(QWidget *parent)
{
    roster = new QListWidget(parent);
    roster->setGeometry(0, 0, 80, parent->height());
    roster->addItem("such");
    roster->addItem("top");
    roster->show();
}

Roster::~Roster() {
    delete roster;
}

