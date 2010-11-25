#include "levelconstructor.h"

#include <iostream>
#include <QtGui>

int main(int argc, char * argv[])
{
    QUrl url = QUrl::fromUserInput(QString::fromLatin1(argv[1]));

    QApplication a(argc, argv);
    LevelConstructor constructor;
    QObject::connect(&constructor, SIGNAL(finished()), QApplication::instance(), SLOT(quit()));
    constructor.construct(url);

    return a.exec();
}

