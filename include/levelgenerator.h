#ifndef LEVELGENERATOR_H
#define LEVELGENERATOR_H

#include <QObject>
#include <QWebFrame>
#include "level.h"

class LevelGenerator : public QObject
{
    Q_OBJECT
public:
    explicit LevelGenerator(QObject *parent = 0);

    float getScore(QWebFrame *webframe);
    Level* generate();

private:
    QWebFrame *webframe;

signals:

public slots:

};

#endif // LEVELGENERATOR_H
