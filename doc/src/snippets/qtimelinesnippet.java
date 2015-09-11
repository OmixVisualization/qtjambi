#include <QtGui>


public class qtimelinesnippet
{

    int main(int argv, char args)
    {
        QApplication app(argv, args);

        QProgressBar progressBar = new QProgressBar;

        QTimeLine timeLine = new QTimeLine(5000);
        timeLine.setFrameRange(0, 100);

        timeLine.frame

        QObject.connect(timeLine, SIGNAL(frameChanged(int)),
             progressBar, SLOT(setValue(int)));

        progressBar.show();

        timeLine.start();

        return app.exec();
    }
}
