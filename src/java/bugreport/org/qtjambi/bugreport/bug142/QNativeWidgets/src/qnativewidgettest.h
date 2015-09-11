#ifndef QNATIVEWIDGETTEST_H
#define QNATIVEWIDGETTEST_H

#include <QtGui/QWidget>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>

namespace Ui {
    class QNativeWidgetTest;
}

class QNativeWidgetTest : public QWidget
{
    Q_OBJECT

public:
    explicit QNativeWidgetTest(QWidget *parent = 0);
    ~QNativeWidgetTest();
    void setLabel(const QString& text);
  
protected:
	QHBoxLayout *horizontalLayout;
    QLabel *label;

};

#endif // QNATIVEWIDGETTEST_H
