#include <QtGui>

int main(int argv, char **args)
{
//! [0]
    QImageWriter writer;
    writer.setFormat("png");
    if (writer.supportsOption(QImageIOHandler::Description))
        qDebug() << "Png supports embedded text";
//! [0]
    return 0;
}
