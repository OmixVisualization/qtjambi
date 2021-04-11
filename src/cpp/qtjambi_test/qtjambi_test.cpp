#include <QtCore/QThreadStorage>
#include <QtWidgets/qwidget.h>
#include <QtTest/qtesttouch.h>
#include <QtTest/QSignalSpy>
#include <qtjambi/qtjambi_core.h>
#include <qtjambi/qtjambi_registry.h>
#include <qtjambi/qtjambi_cast.h>

extern "C" Q_DECL_EXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_test_QSignalSpy_list)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId)
{
    QtJambiScope scope(__this_nativeId);
    try{
        QSignalSpy *__qt_this = qtjambi_object_from_nativeId<QSignalSpy>(__this_nativeId);
        qtjambi_check_resource(__jni_env, __qt_this);
        QList<QList<QVariant> >* list = __qt_this;
        return qtjambi_cast<jobject>(__jni_env, scope, list);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
    return nullptr;
}

#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
#include <QtGui/QTouchDevice>

extern "C" Q_DECL_EXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_test_QTest_touchEvent)
(JNIEnv *env,
 jclass,
 QtJambiNativeID window0,
 QtJambiNativeID device1,
 jboolean autoCommit2,
 jboolean isWidget){
    struct QTouchEventSequence{
        QTouchEventSequence(QWidget *widget, QTouchDevice *aDevice, bool autoCommit)
            : sequence(QTest::touchEvent(widget, aDevice, autoCommit))
        {
        }
        QTouchEventSequence(QWindow *window, QTouchDevice *aDevice, bool autoCommit)
            : sequence(QTest::touchEvent(window, aDevice, autoCommit))
        {
        }

        QTest::QTouchEventSequence sequence;
    };

    try{
        QTest::QTouchEventSequence* sequence;
        if(isWidget){
            QWidget *widget = qtjambi_object_from_nativeId<QWidget>(window0);
            QTouchDevice *device = qtjambi_interface_from_nativeId<QTouchDevice>(device1);
            sequence = &(new QTouchEventSequence(widget, device, autoCommit2))->sequence;
        }else{
            QWindow *widget = qtjambi_object_from_nativeId<QWindow>(window0);
            QTouchDevice *device = qtjambi_interface_from_nativeId<QTouchDevice>(device1);
            sequence = &(new QTouchEventSequence(widget, device, autoCommit2))->sequence;
        }
        return qtjambi_cast<jobject>(env, sequence);
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
    return nullptr;
}
#else
#include <QtGui/QPointingDevice>

extern "C" Q_DECL_EXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_test_QTest_touchEvent)
(JNIEnv *env,
 jclass,
 QtJambiNativeID window0,
 QtJambiNativeID device1,
 jboolean autoCommit2){
    struct QTouchEventWidgetSequence{
        QTouchEventWidgetSequence(QWidget *widget, QPointingDevice *aDevice, bool autoCommit)
            : sequence(QTest::touchEvent(widget, aDevice, autoCommit))
        {
        }
        QTest::QTouchEventWidgetSequence sequence;
    };

    try{
        QWidget *widget = qtjambi_object_from_nativeId<QWidget>(window0);
        QPointingDevice *device = qtjambi_interface_from_nativeId<QPointingDevice>(device1);
        QTest::QTouchEventWidgetSequence* result = &(new QTouchEventWidgetSequence(widget, device, autoCommit2))->sequence;
        return qtjambi_cast<jobject>(env, result);
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
    return nullptr;
}
#endif
