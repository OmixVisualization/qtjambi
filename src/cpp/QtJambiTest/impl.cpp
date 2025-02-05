/****************************************************************************
**
** Copyright (C) 2009-2025 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
**
** This file is part of Qt Jambi.
**
** $BEGIN_LICENSE$
** GNU Lesser General Public License Usage
** This file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
** $END_LICENSE$
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

#include <QtCore/QThreadStorage>
#include <QtWidgets/qwidget.h>
#include <QtTest/qtesttouch.h>
#include <QtTest/QSignalSpy>
#include <QtJambi/QtJambiAPI>
#include <QtJambi/RegistryAPI>
#include <QtJambi/qtjambi_cast.h>

namespace QTest{
void addColumn(const char* name, int id){
    QTest::addColumnInternal(id, name);
}
}

#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
#include <QtGui/QTouchDevice>

extern "C" JNIEXPORT jobject JNICALL Java_io_qt_test_QTest_touchEvent__Lio_qt_widgets_QWidget_2Lio_qt_gui_QTouchDevice_2Z
(JNIEnv *env,
 jclass,
 jobject window0,
 jobject device1,
 jboolean autoCommit2,
 jboolean isWidget){
    Q_UNUSED(isWidget)
    struct QTouchEventSequence{
        QTouchEventSequence(QWidget *widget, QTouchDevice *aDevice, bool autoCommit)
            : sequence(QTest::touchEvent(widget, aDevice, autoCommit))
        {
        }
        QTest::QTouchEventSequence sequence;
    };

    jobject _result{nullptr};
    QTJAMBI_TRY{
        QWidget *widget = QtJambiAPI::convertJavaObjectToQObject<QWidget>(env, window0);
        QTouchDevice *device = QtJambiAPI::convertJavaInterfaceToNative<QTouchDevice>(env, device1);
        QTest::QTouchEventSequence* sequence = &(new QTouchEventSequence(widget, device, autoCommit2))->sequence;
        _result = qtjambi_cast<jobject>(env, sequence);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(env);
    }QTJAMBI_TRY_END
    return _result;
}

extern "C" JNIEXPORT jobject JNICALL Java_io_qt_test_QTest_touchEvent__Lio_qt_gui_QWindow_2Lio_qt_gui_QTouchDevice_2Z
(JNIEnv *env,
 jclass,
 jobject window0,
 jobject device1,
 jboolean autoCommit2,
 jboolean isWidget){
    Q_UNUSED(isWidget)
    struct QTouchEventSequence{
        QTouchEventSequence(QWindow *window, QTouchDevice *aDevice, bool autoCommit)
            : sequence(QTest::touchEvent(window, aDevice, autoCommit))
        {
        }
        QTest::QTouchEventSequence sequence;
    };

    jobject _result{nullptr};
    QTJAMBI_TRY{
        QWindow *widget = QtJambiAPI::convertJavaObjectToQObject<QWindow>(env, window0);
        QTouchDevice *device = QtJambiAPI::convertJavaInterfaceToNative<QTouchDevice>(env, device1);
        QTest::QTouchEventSequence* sequence = &(new QTouchEventSequence(widget, device, autoCommit2))->sequence;
        _result = qtjambi_cast<jobject>(env, sequence);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(env);
    }QTJAMBI_TRY_END
    return _result;
}
#else
#include <QtGui/QPointingDevice>

extern "C" JNIEXPORT jobject JNICALL Java_io_qt_test_QTest_touchEvent
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

    jobject _result{nullptr};
    QTJAMBI_TRY{
        QWidget *widget = QtJambiAPI::convertJavaObjectToQObject<QWidget>(env, window0);
        QPointingDevice *device = QtJambiAPI::convertJavaInterfaceToNative<QPointingDevice>(env, device1);
        QTest::QTouchEventWidgetSequence* result = &(new QTouchEventWidgetSequence(widget, device, autoCommit2))->sequence;
        _result = qtjambi_cast<jobject>(env, result);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(env);
    }QTJAMBI_TRY_END
    return _result;
}
#endif
