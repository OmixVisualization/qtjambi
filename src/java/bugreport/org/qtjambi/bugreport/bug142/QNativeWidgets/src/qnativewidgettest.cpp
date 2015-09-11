#include "qnativewidgettest.h"
#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QWidget>

QNativeWidgetTest::QNativeWidgetTest(QWidget *parent) :
    QWidget(parent)
{
        if (objectName().isEmpty())
           setObjectName(QString::fromUtf8("QNativeWidgetTest"));
        resize(400, 300);
        horizontalLayout = new QHBoxLayout(this);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        label = new QLabel(this);
        label->setObjectName(QString::fromUtf8("label"));
        horizontalLayout->addWidget(label);


        this->setWindowTitle(QApplication::translate("QNativeWidgetTest", "Form", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("QNativeWidgetTest", "TextLabel", 0, QApplication::UnicodeUTF8));

        QMetaObject::connectSlotsByName(this);
}

QNativeWidgetTest::~QNativeWidgetTest()
{
}

void QNativeWidgetTest::setLabel(const QString &text) {
    label->setText(text);
}
