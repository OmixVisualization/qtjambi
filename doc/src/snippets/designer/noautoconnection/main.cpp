#include "imagedialog.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    ImageDialog *dialog = new ImageDialog;
    dialog->show();
    return app.exec();
}
