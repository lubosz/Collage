#include "levelconstructormanager.h"

#include <iostream>
#include <QtGui>

int main(int argc, char * argv[])
{
    QUrl url = QUrl::fromUserInput(QString::fromLatin1(argv[1]));

    QApplication a(argc, argv);
    LevelConstructorManager manager;
    QObject::connect(&manager, SIGNAL(finished()), QApplication::instance(), SLOT(quit()));
    manager.findConstructor(url);

    return a.exec();
}

