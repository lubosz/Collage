#ifndef LEVEL_H
#define LEVEL_H

#include <QObject>

#include "Simulation.h"

class Level : public QObject
{
    Q_OBJECT
public:
    explicit Level(QObject *parent = 0);
    Level(Simulation *simulation);

private:
    Simulation *simulation;

signals:

public slots:

};

#endif // LEVEL_H
