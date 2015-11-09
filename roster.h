#ifndef ROSTER_H
#define ROSTER_H

#include <QListWidget>

class Roster
{
public:
    Roster(QWidget * parent);
    ~Roster();

private:
    QListWidget *roster;
};

#endif // ROSTER_H
