#ifndef IMAGEDIALOG_H
#define IMAGEDIALOG_H

#include "ui_imagedialog.h"

//! [0]
class ImageDialog : public QDialog, private Ui::ImageDialog
{
    Q_OBJECT

public:
    ImageDialog(QWidget *parent = 0);

private slots:
    void checkValues();
};
//! [0]

#endif
