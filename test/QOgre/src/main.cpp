#include "FrameCapture.h"

#include <iostream>
#include <QtGui>

int main(int argc, char * argv[])
{
    QUrl url = QUrl("http://192.168.1.46/bOo2V.jpg");
    QString fileName = "wallbase.png";

    QApplication a(argc, argv);
    FrameCapture capture;
    QObject::connect(&capture, SIGNAL(finished()), QApplication::instance(), SLOT(quit()));
    capture.load(url, fileName);

    return a.exec();
}

