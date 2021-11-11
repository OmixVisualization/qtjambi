#include <QtCore/QThreadStorage>
#include <QtWidgets/qwidget.h>
#include <QtTest/qtesttouch.h>
#include <QtTest/QSignalSpy>
#include <qtjambi/qtjambi_core.h>
#include <qtjambi/qtjambi_registry.h>
#include <qtjambi/qtjambi_cast.h>

#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
#include <QtGui/QTouchDevice>

extern "C" Q_DECL_EXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_test_QTest_touchEvent__Lio_qt_widgets_QWidget_2Lio_qt_gui_QTouchDevice_2Z)
(JNIEnv *env,
 jclass,
 jobject window0,
 jobject device1,
 jboolean autoCommit2,
 jboolean isWidget){
    struct QTouchEventSequence{
        QTouchEventSequence(QWidget *widget, QTouchDevice *aDevice, bool autoCommit)
            : sequence(QTest::touchEvent(widget, aDevice, autoCommit))
        {
        }
        QTest::QTouchEventSequence sequence;
    };

    try{
        QWidget *widget = qtjambi_to_QObject<QWidget>(env, window0);
        QTouchDevice *device = qtjambi_to_interface<QTouchDevice>(env, device1);
        QTest::QTouchEventSequence* sequence = &(new QTouchEventSequence(widget, device, autoCommit2))->sequence;
        return qtjambi_cast<jobject>(env, sequence);
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
    return nullptr;
}

extern "C" Q_DECL_EXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_test_QTest_touchEvent__Lio_qt_gui_QWindow_2Lio_qt_gui_QTouchDevice_2Z)
(JNIEnv *env,
 jclass,
 jobject window0,
 jobject device1,
 jboolean autoCommit2,
 jboolean isWidget){
    struct QTouchEventSequence{
        QTouchEventSequence(QWindow *window, QTouchDevice *aDevice, bool autoCommit)
            : sequence(QTest::touchEvent(window, aDevice, autoCommit))
        {
        }
        QTest::QTouchEventSequence sequence;
    };

    try{
        QWindow *widget = qtjambi_to_QObject<QWindow>(env, window0);
        QTouchDevice *device = qtjambi_to_interface<QTouchDevice>(env, device1);
        QTest::QTouchEventSequence* sequence = &(new QTouchEventSequence(widget, device, autoCommit2))->sequence;
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
 jobject window0,
 jobject device1,
 jboolean autoCommit2){
    struct QTouchEventWidgetSequence{
        QTouchEventWidgetSequence(QWidget *widget, QPointingDevice *aDevice, bool autoCommit)
            : sequence(QTest::touchEvent(widget, aDevice, autoCommit))
        {
        }
        QTest::QTouchEventWidgetSequence sequence;
    };

    try{
        QWidget *widget = qtjambi_to_QObject<QWidget>(env, window0);
        QPointingDevice *device = qtjambi_to_interface<QPointingDevice>(env, device1);
        QTest::QTouchEventWidgetSequence* result = &(new QTouchEventWidgetSequence(widget, device, autoCommit2))->sequence;
        return qtjambi_cast<jobject>(env, result);
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
    return nullptr;
}
#endif
