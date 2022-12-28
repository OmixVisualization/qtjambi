#ifndef QFUTUREINTERFACE_SHELL_H
#define QFUTUREINTERFACE_SHELL_H

#include <QtCore/QtGlobal>
#include <qconfig.h>
#if QT_CONFIG(future)
#include <qfutureinterface.h>
#endif //QT_CONFIG(future)
#include <qtjambi/qtjambi_core.h>
#include <QtCore/QList>
#include <QtCore/QVariant>
#include <QtCore/QVector>
#if QT_CONFIG(future)
#include <qfuture.h>
#endif //QT_CONFIG(future)
#include <qmutex.h>
#include <qrunnable.h>
#if QT_CONFIG(thread)
#include <qthreadpool.h>
#endif //QT_CONFIG(thread)
#include <qtjambi/qtjambi_jobjectwrapper.h>
#include <qtjambi/qtjambi_registry.h>


#if QT_CONFIG(future)

class QFutureInterface_shell : public QFutureInterface<JObjectWrapper>, public QtJambiShellInterface
{
public:
    QFutureInterface_shell(QFutureInterfaceBase::State initialState0 = QFutureInterfaceBase::NoState);
    QFutureInterface_shell(const QFutureInterface<JObjectWrapper>& other0);
    ~QFutureInterface_shell() override;

    static void operator delete(void * ptr) noexcept;
private:
    QtJambiShell* __shell() const override final;
    jmethodID __shell_javaMethod(int pos) const;
};

#endif // QT_CONFIG(future)
#endif // QFUTUREINTERFACE_SHELL_H
