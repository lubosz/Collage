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

    float getScore(QWebPage *webpage);
    Level* generate();

private:
    QWebPage *webpage;

signals:

public slots:

};

#endif // LEVELGENERATOR_H
