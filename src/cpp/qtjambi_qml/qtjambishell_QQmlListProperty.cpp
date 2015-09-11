/****************************************************************************
**
** Copyright (C) 2009-2015 Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
**
** This file is part of Qt Jambi.
**
** ** $BEGIN_LICENSE$
**
** GNU Lesser General Public License Usage
** This file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Nokia gives you certain
** additional rights. These rights are described in the Nokia Qt LGPL
** Exception version 1.0, included in the file LGPL_EXCEPTION.txt in this
** package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
**
** $END_LICENSE$
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

#include <QtCore/QVariant>
#include <qlist.h>
#include <qobject.h>

#include "qtjambishell_QQmlListProperty.h"
#include <qqmllistproperty.h>
#include <qtjambi/qtjambi_core.h>
#include <qtjambi/qtjambifunctiontable.h>
#include <qtjambi/qtjambilink.h>
static const char *qtjambi_method_names[] = {
/*   0 */ "append",
/*   1 */ "at",
/*   2 */ "canAppend",
/*   3 */ "canAt",
/*   4 */ "canClear",
/*   5 */ "canCount",
/*   6 */ "clear",
/*   7 */ "count",
/*   8 */ "object"
};

static const char *qtjambi_method_signatures[] = {
/*   0 */ "(Lorg/qtjambi/qt/core/QObject;)V",
/*   1 */ "(I)Lorg/qtjambi/qt/core/QObject;",
/*   2 */ "()Z",
/*   3 */ "()Z",
/*   4 */ "()Z",
/*   5 */ "()Z",
/*   6 */ "()V",
/*   7 */ "()I",
/*   8 */ "()Lorg/qtjambi/qt/core/QObject;"
};
static const int qtjambi_method_count = 9;

// Default constructed values used throughout final functions...
Q_GLOBAL_STATIC(QtjambiQmlListProperty, default_QtjambiQmlListProperty);

// emitting (writeShellConstructor)
QtJambiShell_QQmlListProperty::QtJambiShell_QQmlListProperty()
    : QtjambiQmlListProperty(),
      m_vtable(0),
      m_link(0)
{
    QTJAMBI_DEBUG_TRACE("(shell) entering: QtJambiShell_QQmlListProperty::QtJambiShell_QQmlListProperty::QtJambiShell_QQmlListProperty()");
    QTJAMBI_DEBUG_TRACE("(shell) leaving: QtJambiShell_QQmlListProperty::QtJambiShell_QQmlListProperty::QtJambiShell_QQmlListProperty()");
}

QtJambiShell_QQmlListProperty::QtJambiShell_QQmlListProperty(QObject*  o0, QList<QObject*>&  list1)
    : QtjambiQmlListProperty(o0, list1),
      m_vtable(0),
      m_link(0)
{
    QTJAMBI_DEBUG_TRACE("(shell) entering: QtJambiShell_QQmlListProperty::QtJambiShell_QQmlListProperty::QtJambiShell_QQmlListProperty(QObject*  o0, QList<jobject >&  list1)");
    QTJAMBI_DEBUG_TRACE("(shell) leaving: QtJambiShell_QQmlListProperty::QtJambiShell_QQmlListProperty::QtJambiShell_QQmlListProperty(QObject*  o0, QList<jobject >&  list1)");
}

// emitting (writeShellDestructor)
QtJambiShell_QQmlListProperty::~QtJambiShell_QQmlListProperty()
{
    QTJAMBI_DEBUG_TRACE("(shell) entering: QtJambiShell_QQmlListProperty::~QtJambiShell_QQmlListProperty()");
#ifdef QT_DEBUG
    if (m_vtable)
        m_vtable->deref();
#endif
    if (m_link) {
        JNIEnv *__jni_env = qtjambi_current_environment();
        if (__jni_env != 0) m_link->nativeShellObjectDestroyed(__jni_env);
    }
    QTJAMBI_DEBUG_TRACE("(shell) leaving: QtJambiShell_QQmlListProperty::~QtJambiShell_QQmlListProperty()");
}

// emitting Virtual overrides (virtualFunctions)
void QtJambiShell_QQmlListProperty::append(QObject*  object0)
{
    QTJAMBI_DEBUG_TRACE("(shell) entering: QQmlListProperty::append(jobject object)");
    jmethodID method_id = m_vtable->method(0);
    if (method_id) {
        JNIEnv *__jni_env = qtjambi_current_environment();
        if (!__jni_env) {
            QtjambiQmlListProperty::append(object0);
            return;
        }
        __jni_env->PushLocalFrame(100);
        jobject __java_return_value_object = m_link->javaObject(__jni_env);
        if(__jni_env->IsSameObject(__java_return_value_object, 0) == JNI_FALSE) {
            QTJAMBI_EXCEPTION_CHECK(__jni_env);
            jobject jobject0 = qtjambi_from_QObject(__jni_env, object0);
            QTJAMBI_EXCEPTION_CHECK(__jni_env);
            __jni_env->CallVoidMethod(__java_return_value_object, method_id, jobject0);
            qtjambi_exception_check(__jni_env);
        } else {
            qtjambishell_throw_nullpointerexception(__jni_env, "QQmlListProperty::append(jobject object)");
        }
        qtjambi_exception_check(__jni_env);
        __jni_env->PopLocalFrame(0);
        QTJAMBI_DEBUG_TRACE("(shell) -> leaving: QQmlListProperty::append(jobject object)");
    } else {
        QTJAMBI_DEBUG_TRACE("(shell) -> super() and leaving: QQmlListProperty::append(jobject object)");
        QtjambiQmlListProperty::append(object0);
    }
}

QObject*  QtJambiShell_QQmlListProperty::at(int  index0)
{
    QTJAMBI_DEBUG_TRACE("(shell) entering: QQmlListProperty::at(int index)");
    jmethodID method_id = m_vtable->method(1);
    if (method_id) {
        JNIEnv *__jni_env = qtjambi_current_environment();
        if (!__jni_env) {
            return QtjambiQmlListProperty::at((int )index0);
        }
        __jni_env->PushLocalFrame(100);
        QObject*  __qt_return_value;
        jobject __java_return_value_object = m_link->javaObject(__jni_env);
        if(__jni_env->IsSameObject(__java_return_value_object, 0) == JNI_FALSE) {
            jobject jobjectReturn = __jni_env->CallObjectMethod(__java_return_value_object, method_id, index0);
            QTJAMBI_EXCEPTION_CHECK(__jni_env);
            __qt_return_value = qtjambi_to_qobject(__jni_env, jobjectReturn);
        } else {
            qtjambishell_throw_nullpointerexception(__jni_env, "QQmlListProperty::at(int index)");
            __qt_return_value =  Q_NULLPTR;
        }
        qtjambi_exception_check(__jni_env);
        __jni_env->PopLocalFrame(0);
        QTJAMBI_DEBUG_TRACE("(shell) -> leaving: QQmlListProperty::at(int index)");
        return __qt_return_value;
    } else {
        QTJAMBI_DEBUG_TRACE("(shell) -> super() and leaving: QQmlListProperty::at(int index)");
        return QtjambiQmlListProperty::at((int )index0);
    }
}

bool  QtJambiShell_QQmlListProperty::canAppend()
{
    QTJAMBI_DEBUG_TRACE("(shell) entering: QQmlListProperty::canAppend()");
    jmethodID method_id = m_vtable->method(2);
    if (method_id) {
        JNIEnv *__jni_env = qtjambi_current_environment();
        if (!__jni_env) {
            return QtjambiQmlListProperty::canAppend();
        }
        __jni_env->PushLocalFrame(100);
        bool  __qt_return_value;
        jobject __java_return_value_object = m_link->javaObject(__jni_env);
        if(__jni_env->IsSameObject(__java_return_value_object, 0) == JNI_FALSE) {
            jboolean __java_return_value = __jni_env->CallBooleanMethod(__java_return_value_object, method_id);
            qtjambi_exception_check(__jni_env);
            bool  __qt_return_value_tmp = (bool ) __java_return_value;
            QTJAMBI_EXCEPTION_CHECK(__jni_env);
            __qt_return_value = __qt_return_value_tmp;
        } else {
            qtjambishell_throw_nullpointerexception(__jni_env, "QQmlListProperty::canAppend()");
            __qt_return_value =  false;
        }
        qtjambi_exception_check(__jni_env);
        __jni_env->PopLocalFrame(0);
        QTJAMBI_DEBUG_TRACE("(shell) -> leaving: QQmlListProperty::canAppend()");
        return __qt_return_value;
    } else {
        QTJAMBI_DEBUG_TRACE("(shell) -> super() and leaving: QQmlListProperty::canAppend()");
        return QtjambiQmlListProperty::canAppend();
    }
}

bool  QtJambiShell_QQmlListProperty::canAt()
{
    QTJAMBI_DEBUG_TRACE("(shell) entering: QQmlListProperty::canAt()");
    jmethodID method_id = m_vtable->method(3);
    if (method_id) {
        JNIEnv *__jni_env = qtjambi_current_environment();
        if (!__jni_env) {
            return QtjambiQmlListProperty::canAt();
        }
        __jni_env->PushLocalFrame(100);
        bool  __qt_return_value;
        jobject __java_return_value_object = m_link->javaObject(__jni_env);
        if(__jni_env->IsSameObject(__java_return_value_object, 0) == JNI_FALSE) {
            jboolean __java_return_value = __jni_env->CallBooleanMethod(__java_return_value_object, method_id);
            qtjambi_exception_check(__jni_env);
            bool  __qt_return_value_tmp = (bool ) __java_return_value;
            QTJAMBI_EXCEPTION_CHECK(__jni_env);
            __qt_return_value = __qt_return_value_tmp;
        } else {
            qtjambishell_throw_nullpointerexception(__jni_env, "QQmlListProperty::canAt()");
            __qt_return_value =  false;
        }
        qtjambi_exception_check(__jni_env);
        __jni_env->PopLocalFrame(0);
        QTJAMBI_DEBUG_TRACE("(shell) -> leaving: QQmlListProperty::canAt()");
        return __qt_return_value;
    } else {
        QTJAMBI_DEBUG_TRACE("(shell) -> super() and leaving: QQmlListProperty::canAt()");
        return QtjambiQmlListProperty::canAt();
    }
}

bool  QtJambiShell_QQmlListProperty::canClear()
{
    QTJAMBI_DEBUG_TRACE("(shell) entering: QQmlListProperty::canClear()");
    jmethodID method_id = m_vtable->method(4);
    if (method_id) {
        JNIEnv *__jni_env = qtjambi_current_environment();
        if (!__jni_env) {
            return QtjambiQmlListProperty::canClear();
        }
        __jni_env->PushLocalFrame(100);
        bool  __qt_return_value;
        jobject __java_return_value_object = m_link->javaObject(__jni_env);
        if(__jni_env->IsSameObject(__java_return_value_object, 0) == JNI_FALSE) {
            jboolean __java_return_value = __jni_env->CallBooleanMethod(__java_return_value_object, method_id);
            qtjambi_exception_check(__jni_env);
            bool  __qt_return_value_tmp = (bool ) __java_return_value;
            QTJAMBI_EXCEPTION_CHECK(__jni_env);
            __qt_return_value = __qt_return_value_tmp;
        } else {
            qtjambishell_throw_nullpointerexception(__jni_env, "QQmlListProperty::canClear()");
            __qt_return_value =  false;
        }
        qtjambi_exception_check(__jni_env);
        __jni_env->PopLocalFrame(0);
        QTJAMBI_DEBUG_TRACE("(shell) -> leaving: QQmlListProperty::canClear()");
        return __qt_return_value;
    } else {
        QTJAMBI_DEBUG_TRACE("(shell) -> super() and leaving: QQmlListProperty::canClear()");
        return QtjambiQmlListProperty::canClear();
    }
}

bool  QtJambiShell_QQmlListProperty::canCount()
{
    QTJAMBI_DEBUG_TRACE("(shell) entering: QQmlListProperty::canCount()");
    jmethodID method_id = m_vtable->method(5);
    if (method_id) {
        JNIEnv *__jni_env = qtjambi_current_environment();
        if (!__jni_env) {
            return QtjambiQmlListProperty::canCount();
        }
        __jni_env->PushLocalFrame(100);
        bool  __qt_return_value;
        jobject __java_return_value_object = m_link->javaObject(__jni_env);
        if(__jni_env->IsSameObject(__java_return_value_object, 0) == JNI_FALSE) {
            jboolean __java_return_value = __jni_env->CallBooleanMethod(__java_return_value_object, method_id);
            qtjambi_exception_check(__jni_env);
            bool  __qt_return_value_tmp = (bool ) __java_return_value;
            QTJAMBI_EXCEPTION_CHECK(__jni_env);
            __qt_return_value = __qt_return_value_tmp;
        } else {
            qtjambishell_throw_nullpointerexception(__jni_env, "QQmlListProperty::canCount()");
            __qt_return_value =  false;
        }
        qtjambi_exception_check(__jni_env);
        __jni_env->PopLocalFrame(0);
        QTJAMBI_DEBUG_TRACE("(shell) -> leaving: QQmlListProperty::canCount()");
        return __qt_return_value;
    } else {
        QTJAMBI_DEBUG_TRACE("(shell) -> super() and leaving: QQmlListProperty::canCount()");
        return QtjambiQmlListProperty::canCount();
    }
}

void QtJambiShell_QQmlListProperty::clear()
{
    QTJAMBI_DEBUG_TRACE("(shell) entering: QQmlListProperty::clear()");
    jmethodID method_id = m_vtable->method(6);
    if (method_id) {
        JNIEnv *__jni_env = qtjambi_current_environment();
        if (!__jni_env) {
            QtjambiQmlListProperty::clear();
            return;
        }
        __jni_env->PushLocalFrame(100);
        jobject __java_return_value_object = m_link->javaObject(__jni_env);
        if(__jni_env->IsSameObject(__java_return_value_object, 0) == JNI_FALSE) {
            __jni_env->CallVoidMethod(__java_return_value_object, method_id);
            qtjambi_exception_check(__jni_env);
        } else {
            qtjambishell_throw_nullpointerexception(__jni_env, "QQmlListProperty::clear()");
        }
        qtjambi_exception_check(__jni_env);
        __jni_env->PopLocalFrame(0);
        QTJAMBI_DEBUG_TRACE("(shell) -> leaving: QQmlListProperty::clear()");
    } else {
        QTJAMBI_DEBUG_TRACE("(shell) -> super() and leaving: QQmlListProperty::clear()");
        QtjambiQmlListProperty::clear();
    }
}

int  QtJambiShell_QQmlListProperty::count()
{
    QTJAMBI_DEBUG_TRACE("(shell) entering: QQmlListProperty::count()");
    jmethodID method_id = m_vtable->method(7);
    if (method_id) {
        JNIEnv *__jni_env = qtjambi_current_environment();
        if (!__jni_env) {
            return QtjambiQmlListProperty::count();
        }
        __jni_env->PushLocalFrame(100);
        int  __qt_return_value;
        jobject __java_return_value_object = m_link->javaObject(__jni_env);
        if(__jni_env->IsSameObject(__java_return_value_object, 0) == JNI_FALSE) {
            jint __java_return_value = __jni_env->CallIntMethod(__java_return_value_object, method_id);
            qtjambi_exception_check(__jni_env);
            int  __qt_return_value_tmp = (int ) __java_return_value;
            QTJAMBI_EXCEPTION_CHECK(__jni_env);
            __qt_return_value = __qt_return_value_tmp;
        } else {
            qtjambishell_throw_nullpointerexception(__jni_env, "QQmlListProperty::count()");
            __qt_return_value =  0;
        }
        qtjambi_exception_check(__jni_env);
        __jni_env->PopLocalFrame(0);
        QTJAMBI_DEBUG_TRACE("(shell) -> leaving: QQmlListProperty::count()");
        return __qt_return_value;
    } else {
        QTJAMBI_DEBUG_TRACE("(shell) -> super() and leaving: QQmlListProperty::count()");
        return QtjambiQmlListProperty::count();
    }
}

QObject*  QtJambiShell_QQmlListProperty::object()
{
    QTJAMBI_DEBUG_TRACE("(shell) entering: QQmlListProperty::object()");
    jmethodID method_id = m_vtable->method(8);
    if (method_id) {
        JNIEnv *__jni_env = qtjambi_current_environment();
        if (!__jni_env) {
            return QtjambiQmlListProperty::object();
        }
        __jni_env->PushLocalFrame(100);
        QObject*  __qt_return_value;
        jobject __java_return_value_object = m_link->javaObject(__jni_env);
        if(__jni_env->IsSameObject(__java_return_value_object, 0) == JNI_FALSE) {
            jobject __java_return_value = __jni_env->CallObjectMethod(__java_return_value_object, method_id);
            qtjambi_exception_check(__jni_env);
            QObject*  __qt_return_value_tmp = (QObject* ) qtjambi_to_qobject(__jni_env, __java_return_value);
            QTJAMBI_EXCEPTION_CHECK(__jni_env);
            __qt_return_value = __qt_return_value_tmp;
        } else {
            qtjambishell_throw_nullpointerexception(__jni_env, "QQmlListProperty::object()");
            __qt_return_value =  0;
        }
        qtjambi_exception_check(__jni_env);
        __jni_env->PopLocalFrame(0);
        QTJAMBI_DEBUG_TRACE("(shell) -> leaving: QQmlListProperty::object()");
        return __qt_return_value;
    } else {
        QTJAMBI_DEBUG_TRACE("(shell) -> super() and leaving: QQmlListProperty::object()");
        return QtjambiQmlListProperty::object();
    }
}

// emitting Functions in shell class (nonVirtualShellFunctions)
// emitting Public Override Functions (publicOverrideFunctions)
// emitting Virtual Override Functions (virtualOverrideFunctions)
void QtJambiShell_QQmlListProperty::__override_append(QObject*  object0, bool static_call)
{
    if (static_call) {
        QtjambiQmlListProperty::append(object0);
    } else {
        append(object0);
    }
}

QObject*  QtJambiShell_QQmlListProperty::__override_at(int  index0, bool static_call)
{
    if (static_call) {
        return QtjambiQmlListProperty::at((int )index0);
    } else {
        return at((int )index0);
    }
}

bool  QtJambiShell_QQmlListProperty::__override_canAppend(bool static_call)
{
    if (static_call) {
        return QtjambiQmlListProperty::canAppend();
    } else {
        return canAppend();
    }
}

bool  QtJambiShell_QQmlListProperty::__override_canAt(bool static_call)
{
    if (static_call) {
        return QtjambiQmlListProperty::canAt();
    } else {
        return canAt();
    }
}

bool  QtJambiShell_QQmlListProperty::__override_canClear(bool static_call)
{
    if (static_call) {
        return QtjambiQmlListProperty::canClear();
    } else {
        return canClear();
    }
}

bool  QtJambiShell_QQmlListProperty::__override_canCount(bool static_call)
{
    if (static_call) {
        return QtjambiQmlListProperty::canCount();
    } else {
        return canCount();
    }
}

void QtJambiShell_QQmlListProperty::__override_clear(bool static_call)
{
    if (static_call) {
        QtjambiQmlListProperty::clear();
    } else {
        clear();
    }
}

int  QtJambiShell_QQmlListProperty::__override_count(bool static_call)
{
    if (static_call) {
        return QtjambiQmlListProperty::count();
    } else {
        return count();
    }
}

QObject*  QtJambiShell_QQmlListProperty::__override_object(bool static_call)
{
    if (static_call) {
        return QtjambiQmlListProperty::object();
    } else {
        return object();
    }
}

// emitting (writeExtraFunctions)
// emitting (writeToStringFunction)
// emitting (writeCloneFunction)

extern "C" Q_DECL_EXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_org_qtjambi_qt_qml_QQmlListProperty__1_1qt_1clone)
(JNIEnv *__jni_env, jobject, jlong __this_nativeId)
{
    QtjambiQmlListProperty *__qt_this = (QtjambiQmlListProperty *) qtjambi_from_jlong(__this_nativeId);
    QTJAMBI_EXCEPTION_CHECK(__jni_env);
    Q_ASSERT(__qt_this);
    QtjambiQmlListProperty *res = __qt_this;
    return qtjambi_from_object(__jni_env, res, "QQmlListProperty", "org/qtjambi/qt/qml/", true);
}

// emitting (writeSignalFunction)
// emitting  (functionsInTargetLang writeFinalFunction)
// QQmlListProperty::QtjambiQmlListProperty()
extern "C" Q_DECL_EXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_org_qtjambi_qt_qml_QQmlListProperty__1_1qt_1QQmlListProperty__)
(JNIEnv *__jni_env,
 jobject __jni_object)
{
    QTJAMBI_DEBUG_TRACE("(native) entering: QQmlListProperty::QtjambiQmlListProperty()");
    Q_UNUSED(__jni_env)
    Q_UNUSED(__jni_object)
    QtJambiShell_QQmlListProperty *__qt_this = new QtJambiShell_QQmlListProperty();
    QtJambiLink *__qt_java_link = qtjambi_construct_object(__jni_env, __jni_object, __qt_this, "QtjambiQmlListProperty");
    if (!__qt_java_link) {
        qWarning("object construction failed for type: QtjambiQmlListProperty");
        return;
    }
    __qt_java_link->setJavaOwnership(__jni_env, __jni_object);
    __qt_this->m_link = __qt_java_link;
    __qt_this->m_link->setCreatedByJava(true);
    __qt_this->m_vtable = qtjambi_setup_vtable(__jni_env, 
                        __jni_object, 
                        0, 0, 0, // no inconsistent functions
                        qtjambi_method_count, 
                        qtjambi_method_names, 
                        qtjambi_method_signatures
                        );
    QTJAMBI_DEBUG_TRACE("(native) -> leaving: QQmlListProperty::QtjambiQmlListProperty()");

}

// QQmlListProperty::QtjambiQmlListProperty(QObject * o, QList<QObject > & list)
extern "C" Q_DECL_EXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_org_qtjambi_qt_qml_QQmlListProperty__1_1qt_1QQmlListProperty_1QObject_1List__JLjava_util_List_2)
(JNIEnv *__jni_env,
 jobject __jni_object,
 jlong  o0,
 jobject list1)
{
    QTJAMBI_DEBUG_TRACE("(native) entering: QQmlListProperty::QtjambiQmlListProperty(QObject * o, QList<QObject > & list)");
    Q_UNUSED(__jni_env)
    Q_UNUSED(__jni_object)
    QObject*  __qt_o0 = (QObject* ) qtjambi_from_jlong(o0);
    QTJAMBI_EXCEPTION_CHECK(__jni_env);
    QList<QObject*> __qt_list1;
    StaticCache *sc = StaticCache::instance();
    sc->resolveQtJambiObject();
    if (list1 != 0) {
        jobjectArray __qt__array = qtjambi_collection_toArray(__jni_env, list1);
        jsize __qt__size = __jni_env->GetArrayLength(__qt__array);
        for (int i=0; i<__qt__size; ++i) {
            jobject __java_element = __jni_env->GetObjectArrayElement(__qt__array, i);
            __qt_list1 << qtjambi_to_qobject(__jni_env, __java_element);
            QTJAMBI_EXCEPTION_CHECK(__jni_env);
        }
    }
    QTJAMBI_EXCEPTION_CHECK(__jni_env);
    QTJAMBI_EXCEPTION_CHECK(__jni_env);
    QtJambiShell_QQmlListProperty *__qt_this = new QtJambiShell_QQmlListProperty((QObject* )__qt_o0, __qt_list1);
    QtJambiLink *__qt_java_link = qtjambi_construct_object(__jni_env, __jni_object, __qt_this, "QtjambiQmlListProperty");
    if (!__qt_java_link) {
        qWarning("object construction failed for type: QtjambiQmlListProperty");
        return;
    }
    __qt_java_link->setJavaOwnership(__jni_env, __jni_object);
    __qt_this->m_link = __qt_java_link;
    __qt_this->m_link->setCreatedByJava(true);
    __qt_this->m_vtable = qtjambi_setup_vtable(__jni_env, 
                        __jni_object, 
                        0, 0, 0, // no inconsistent functions
                        qtjambi_method_count, 
                        qtjambi_method_names, 
                        qtjambi_method_signatures
                        );
    QTJAMBI_DEBUG_TRACE("(native) -> leaving: QQmlListProperty::QtjambiQmlListProperty(QObject * o, QList<jobject > & list)");

}

// QQmlListProperty::append(QObject object)
extern "C" Q_DECL_EXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_org_qtjambi_qt_qml_QQmlListProperty__1_1qt_1append_1QObject__JLorg_qtjambi_qt_core_QObject_2)
(JNIEnv *__jni_env,
 jobject,
 jlong __this_nativeId,
 jobject object0)
{
    QTJAMBI_DEBUG_TRACE("(native) entering: QQmlListProperty::append(QObject object)");
    Q_UNUSED(__jni_env)
    Q_UNUSED(__this_nativeId)
    QTJAMBI_EXCEPTION_CHECK(__jni_env);
    QtJambiShell_QQmlListProperty *__qt_this = (QtJambiShell_QQmlListProperty *) qtjambi_from_jlong(__this_nativeId);
    QTJAMBI_EXCEPTION_CHECK(__jni_env);
    Q_ASSERT(__qt_this);
    bool __do_static_call = __this_nativeId ? ((QtJambiLink *) __this_nativeId)->createdByJava() : false;
    __qt_this->__override_append(qtjambi_to_qobject(__jni_env, object0), __do_static_call);
    QTJAMBI_DEBUG_TRACE("(native) -> leaving: QQmlListProperty::append(jobject object)");

}

// QQmlListProperty::at(int index)
extern "C" Q_DECL_EXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_org_qtjambi_qt_qml_QQmlListProperty__1_1qt_1at_1int__JI)
(JNIEnv *__jni_env,
 jobject,
 jlong __this_nativeId,
 jint index0)
{
    QTJAMBI_DEBUG_TRACE("(native) entering: QQmlListProperty::at(int index)");
    Q_UNUSED(__jni_env)
    Q_UNUSED(__this_nativeId)
    QtJambiShell_QQmlListProperty *__qt_this = (QtJambiShell_QQmlListProperty *) qtjambi_from_jlong(__this_nativeId);
    QTJAMBI_EXCEPTION_CHECK(__jni_env);
    Q_ASSERT(__qt_this);
    bool __do_static_call = __this_nativeId ? ((QtJambiLink *) __this_nativeId)->createdByJava() : false;
    QObject*  qt_return_value = __qt_this->__override_at((int )index0, __do_static_call);
    jobject __qt_return_value = qtjambi_from_QObject(__jni_env, qt_return_value);
    QTJAMBI_EXCEPTION_CHECK(__jni_env);
    QTJAMBI_DEBUG_TRACE("(native) -> leaving: QQmlListProperty::at(int index)");
    return __qt_return_value;
}

// QQmlListProperty::canAppend()
extern "C" Q_DECL_EXPORT jboolean JNICALL QTJAMBI_FUNCTION_PREFIX(Java_org_qtjambi_qt_qml_QQmlListProperty__1_1qt_1canAppend__J)
(JNIEnv *__jni_env,
 jobject,
 jlong __this_nativeId)
{
    QTJAMBI_DEBUG_TRACE("(native) entering: QQmlListProperty::canAppend()");
    Q_UNUSED(__jni_env)
    Q_UNUSED(__this_nativeId)
    QtJambiShell_QQmlListProperty *__qt_this = (QtJambiShell_QQmlListProperty *) qtjambi_from_jlong(__this_nativeId);
    QTJAMBI_EXCEPTION_CHECK(__jni_env);
    Q_ASSERT(__qt_this);
    bool __do_static_call = __this_nativeId ? ((QtJambiLink *) __this_nativeId)->createdByJava() : false;
    bool  __qt_return_value = __qt_this->__override_canAppend(__do_static_call);

    jboolean __java_return_value = (jboolean) __qt_return_value;
    QTJAMBI_EXCEPTION_CHECK(__jni_env);
    QTJAMBI_DEBUG_TRACE("(native) -> leaving: QQmlListProperty::canAppend()");
    return __java_return_value;
}

// QQmlListProperty::canAt()
extern "C" Q_DECL_EXPORT jboolean JNICALL QTJAMBI_FUNCTION_PREFIX(Java_org_qtjambi_qt_qml_QQmlListProperty__1_1qt_1canAt__J)
(JNIEnv *__jni_env,
 jobject,
 jlong __this_nativeId)
{
    QTJAMBI_DEBUG_TRACE("(native) entering: QQmlListProperty::canAt()");
    Q_UNUSED(__jni_env)
    Q_UNUSED(__this_nativeId)
    QtJambiShell_QQmlListProperty *__qt_this = (QtJambiShell_QQmlListProperty *) qtjambi_from_jlong(__this_nativeId);
    QTJAMBI_EXCEPTION_CHECK(__jni_env);
    Q_ASSERT(__qt_this);
    bool __do_static_call = __this_nativeId ? ((QtJambiLink *) __this_nativeId)->createdByJava() : false;
    bool  __qt_return_value = __qt_this->__override_canAt(__do_static_call);

    jboolean __java_return_value = (jboolean) __qt_return_value;
    QTJAMBI_EXCEPTION_CHECK(__jni_env);
    QTJAMBI_DEBUG_TRACE("(native) -> leaving: QQmlListProperty::canAt()");
    return __java_return_value;
}

// QQmlListProperty::canClear()
extern "C" Q_DECL_EXPORT jboolean JNICALL QTJAMBI_FUNCTION_PREFIX(Java_org_qtjambi_qt_qml_QQmlListProperty__1_1qt_1canClear__J)
(JNIEnv *__jni_env,
 jobject,
 jlong __this_nativeId)
{
    QTJAMBI_DEBUG_TRACE("(native) entering: QQmlListProperty::canClear()");
    Q_UNUSED(__jni_env)
    Q_UNUSED(__this_nativeId)
    QtJambiShell_QQmlListProperty *__qt_this = (QtJambiShell_QQmlListProperty *) qtjambi_from_jlong(__this_nativeId);
    QTJAMBI_EXCEPTION_CHECK(__jni_env);
    Q_ASSERT(__qt_this);
    bool __do_static_call = __this_nativeId ? ((QtJambiLink *) __this_nativeId)->createdByJava() : false;
    bool  __qt_return_value = __qt_this->__override_canClear(__do_static_call);

    jboolean __java_return_value = (jboolean) __qt_return_value;
    QTJAMBI_EXCEPTION_CHECK(__jni_env);
    QTJAMBI_DEBUG_TRACE("(native) -> leaving: QQmlListProperty::canClear()");
    return __java_return_value;
}

// QQmlListProperty::canCount()
extern "C" Q_DECL_EXPORT jboolean JNICALL QTJAMBI_FUNCTION_PREFIX(Java_org_qtjambi_qt_qml_QQmlListProperty__1_1qt_1canCount__J)
(JNIEnv *__jni_env,
 jobject,
 jlong __this_nativeId)
{
    QTJAMBI_DEBUG_TRACE("(native) entering: QQmlListProperty::canCount()");
    Q_UNUSED(__jni_env)
    Q_UNUSED(__this_nativeId)
    QtJambiShell_QQmlListProperty *__qt_this = (QtJambiShell_QQmlListProperty *) qtjambi_from_jlong(__this_nativeId);
    QTJAMBI_EXCEPTION_CHECK(__jni_env);
    Q_ASSERT(__qt_this);
    bool __do_static_call = __this_nativeId ? ((QtJambiLink *) __this_nativeId)->createdByJava() : false;
    bool  __qt_return_value = __qt_this->__override_canCount(__do_static_call);

    jboolean __java_return_value = (jboolean) __qt_return_value;
    QTJAMBI_EXCEPTION_CHECK(__jni_env);
    QTJAMBI_DEBUG_TRACE("(native) -> leaving: QQmlListProperty::canCount()");
    return __java_return_value;
}

// QQmlListProperty::clear()
extern "C" Q_DECL_EXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_org_qtjambi_qt_qml_QQmlListProperty__1_1qt_1clear__J)
(JNIEnv *__jni_env,
 jobject,
 jlong __this_nativeId)
{
    QTJAMBI_DEBUG_TRACE("(native) entering: QQmlListProperty::clear()");
    Q_UNUSED(__jni_env)
    Q_UNUSED(__this_nativeId)
    QtJambiShell_QQmlListProperty *__qt_this = (QtJambiShell_QQmlListProperty *) qtjambi_from_jlong(__this_nativeId);
    QTJAMBI_EXCEPTION_CHECK(__jni_env);
    Q_ASSERT(__qt_this);
    bool __do_static_call = __this_nativeId ? ((QtJambiLink *) __this_nativeId)->createdByJava() : false;
    __qt_this->__override_clear(__do_static_call);
    QTJAMBI_DEBUG_TRACE("(native) -> leaving: QQmlListProperty::clear()");

}

// QQmlListProperty::count()
extern "C" Q_DECL_EXPORT jint JNICALL QTJAMBI_FUNCTION_PREFIX(Java_org_qtjambi_qt_qml_QQmlListProperty__1_1qt_1count__J)
(JNIEnv *__jni_env,
 jobject,
 jlong __this_nativeId)
{
    QTJAMBI_DEBUG_TRACE("(native) entering: QQmlListProperty::count()");
    Q_UNUSED(__jni_env)
    Q_UNUSED(__this_nativeId)
    QtJambiShell_QQmlListProperty *__qt_this = (QtJambiShell_QQmlListProperty *) qtjambi_from_jlong(__this_nativeId);
    QTJAMBI_EXCEPTION_CHECK(__jni_env);
    Q_ASSERT(__qt_this);
    bool __do_static_call = __this_nativeId ? ((QtJambiLink *) __this_nativeId)->createdByJava() : false;
    int  __qt_return_value = __qt_this->__override_count(__do_static_call);

    jint __java_return_value = (jint) __qt_return_value;
    QTJAMBI_EXCEPTION_CHECK(__jni_env);
    QTJAMBI_DEBUG_TRACE("(native) -> leaving: QQmlListProperty::count()");
    return __java_return_value;
}

// QQmlListProperty::object()
extern "C" Q_DECL_EXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_org_qtjambi_qt_qml_QQmlListProperty__1_1qt_1object__J)
(JNIEnv *__jni_env,
 jobject,
 jlong __this_nativeId)
{
    QTJAMBI_DEBUG_TRACE("(native) entering: QQmlListProperty::object()");
    Q_UNUSED(__jni_env)
    Q_UNUSED(__this_nativeId)
    QtJambiShell_QQmlListProperty *__qt_this = (QtJambiShell_QQmlListProperty *) qtjambi_from_jlong(__this_nativeId);
    QTJAMBI_EXCEPTION_CHECK(__jni_env);
    Q_ASSERT(__qt_this);
    bool __do_static_call = __this_nativeId ? ((QtJambiLink *) __this_nativeId)->createdByJava() : false;
    QObject*  __qt_return_value = __qt_this->__override_object(__do_static_call);

    jobject __java_return_value = qtjambi_from_qobject(__jni_env, (QObject *) __qt_return_value, "QObject", "org/qtjambi/qt/core/");
    QTJAMBI_EXCEPTION_CHECK(__jni_env);
    QTJAMBI_DEBUG_TRACE("(native) -> leaving: QQmlListProperty::object()");
    return __java_return_value;
}

// emitting (AbstractMetaClass::NormalFunctions|AbstractMetaClass::AbstractFunctions writeFinalFunction)
// emitting Field accessors (writeFieldAccessors)
// emitting (writeFromNativeFunction)
extern "C" Q_DECL_EXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_org_qtjambi_qt_qml_QQmlListProperty_fromNativePointer)
(JNIEnv *__jni_env,
 jclass,
 jobject nativePointer)
{
    void *ptr = qtjambi_to_cpointer(__jni_env, nativePointer, 1);
    return qtjambi_from_object(__jni_env, ptr, "QQmlListProperty", "org/qtjambi/qt/qml/", true);
}
// emitting (writeFromArrayFunction)
extern "C" Q_DECL_EXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_org_qtjambi_qt_qml_QQmlListProperty_nativePointerArray)
(JNIEnv *__jni_env,
 jclass,
 jobjectArray array)
{
    return qtjambi_array_to_nativepointer(__jni_env,
                                         array,
                                         sizeof(QtjambiQmlListProperty));
}
// emitting (writeInterfaceCastFunction)
// emitting (writeSignalInitialization)
// emitting (writeJavaLangObjectOverrideFunctions)


