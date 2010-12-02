#ifndef LEVELCONSTRUCTOR_H
#define LEVELCONSTRUCTOR_H

#include <QtWebKit>
#include "level.h"

class LevelConstructor : public QObject
{
    Q_OBJECT

public:
    LevelConstructor();
    // Value between 0.0 and 1.0 indicating the suitability of this
    // constructor for a given webpage
    //double constructionConfidence(QWebFrame *frame);
    // Do the actual construction and return the constructed Level
    //Level construct(QWebFrame *frame);
};

#endif // LEVELCONSTRUCTOR_H
