#include "Application.h"

using namespace QtOgre;
#include "FrameCapture.h"

int main(int argc, char *argv[])
{
    QUrl url = QUrl("http://en.wikipedia.org/wiki/Cola");
    QString fileName = "wallbase.png";

	Application app(argc, argv);

    //QApplication a(argc, argv);
    FrameCapture capture;
    //QObject::connect(&capture, SIGNAL(finished()), QApplication::instance(), SLOT(quit()));
    capture.load(url, fileName);
	return app.exec();
}
