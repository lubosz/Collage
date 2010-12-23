#ifndef GENERALLEVELGENERATOR_H
#define GENERALLEVELGENERATOR_H

#include "LevelGenerator.h"

class GeneralLevelGenerator : public LevelGenerator
{
    Q_OBJECT
public:
    explicit GeneralLevelGenerator(QObject *parent = 0);

    float getScore(QWebPage *webpage);
    Level* generate();

signals:

public slots:

};

#endif // GENERALLEVELGENERATOR_H
