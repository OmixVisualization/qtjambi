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

#include "qtjambishell_QDeclarativeListProperty.h"
#include <qdeclarativelistproperty.h>
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

// emitting (writeShellConstructor)
QtJambiShell_QDeclarativeListProperty::QtJambiShell_QDeclarativeListProperty()
    : QtjambiDeclarativeListProperty(),
      m_vtable(0),
      m_link(0)
{
    QTJAMBI_DEBUG_TRACE("(shell) entering: QtJambiShell_QDeclarativeListProperty::QtJambiShell_QDeclarativeListProperty::QtJambiShell_QDeclarativeListProperty()");
    QTJAMBI_DEBUG_TRACE("(shell) leaving: QtJambiShell_QDeclarativeListProperty::QtJambiShell_QDeclarativeListProperty::QtJambiShell_QDeclarativeListProperty()");
}

QtJambiShell_QDeclarativeListProperty::QtJambiShell_QDeclarativeListProperty(QObject*  o0, QList<QObject*>&  list1)
    : QtjambiDeclarativeListProperty(o0, list1),
      m_vtable(0),
      m_link(0)
{
    QTJAMBI_DEBUG_TRACE("(shell) entering: QtJambiShell_QDeclarativeListProperty::QtJambiShell_QDeclarativeListProperty::QtJambiShell_QDeclarativeListProperty(QObject*  o0, QList<jobject >&  list1)");
    QTJAMBI_DEBUG_TRACE("(shell) leaving: QtJambiShell_QDeclarativeListProperty::QtJambiShell_QDeclarativeListProperty::QtJambiShell_QDeclarativeListProperty(QObject*  o0, QList<jobject >&  list1)");
}

// emitting (writeShellDestructor)
QtJambiShell_QDeclarativeListProperty::~QtJambiShell_QDeclarativeListProperty()
{
    QTJAMBI_DEBUG_TRACE("(shell) entering: QtJambiShell_QDeclarativeListProperty::~QtJambiShell_QDeclarativeListProperty()");
#ifdef QT_DEBUG
    if (m_vtable)
        m_vtable->deref();
#endif
    if (m_link) {
        JNIEnv *__jni_env = qtjambi_current_environment();
        if (__jni_env != 0) m_link->nativeShellObjectDestroyed(__jni_env);
    }
    QTJAMBI_DEBUG_TRACE("(shell) leaving: QtJambiShell_QDeclarativeListProperty::~QtJambiShell_QDeclarativeListProperty()");
}

// emitting Virtual overrides (virtualFunctions)
void QtJambiShell_QDeclarativeListProperty::append(QObject*  object0)
{
    QTJAMBI_DEBUG_TRACE("(shell) entering: QDeclarativeListProperty::append(jobject object)");
    jmethodID method_id = m_vtable->method(0);
    if (method_id) {
        JNIEnv *__jni_env = qtjambi_current_environment();
        if (!__jni_env) {
            QtjambiDeclarativeListProperty::append(object0);
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
            qtjambishell_throw_nullpointerexception(__jni_env, "QDeclarativeListProperty::append(jobject object)");
        }
        qtjambi_exception_check(__jni_env);
        __jni_env->PopLocalFrame(0);
        QTJAMBI_DEBUG_TRACE("(shell) -> leaving: QDeclarativeListProperty::append(jobject object)");
    } else {
        QTJAMBI_DEBUG_TRACE("(shell) -> super() and leaving: QDeclarativeListProperty::append(jobject object)");
        QtjambiDeclarativeListProperty::append(object0);
    }
}

QObject*  QtJambiShell_QDeclarativeListProperty::at(int  index0)
{
    QTJAMBI_DEBUG_TRACE("(shell) entering: QDeclarativeListProperty::at(int index)");
    jmethodID method_id = m_vtable->method(1);
    if (method_id) {
        JNIEnv *__jni_env = qtjambi_current_environment();
        if (!__jni_env) {
            return QtjambiDeclarativeListProperty::at((int )index0);
        }
        __jni_env->PushLocalFrame(100);
        QObject*  __qt_return_value;
        jobject __java_return_value_object = m_link->javaObject(__jni_env);
        if(__jni_env->IsSameObject(__java_return_value_object, 0) == JNI_FALSE) {
            jobject jobjectReturn = __jni_env->CallObjectMethod(__java_return_value_object, method_id, index0);
            __qt_return_value = qtjambi_to_qobject(__jni_env, jobjectReturn);
            QTJAMBI_EXCEPTION_CHECK(__jni_env);
        } else {
            qtjambishell_throw_nullpointerexception(__jni_env, "QDeclarativeListProperty::at(int index)");
            __qt_return_value =  Q_NULLPTR;
        }
        qtjambi_exception_check(__jni_env);
        __jni_env->PopLocalFrame(0);
        QTJAMBI_DEBUG_TRACE("(shell) -> leaving: QDeclarativeListProperty::at(int index)");
        return __qt_return_value;
    } else {
        QTJAMBI_DEBUG_TRACE("(shell) -> super() and leaving: QDeclarativeListProperty::at(int index)");
        return QtjambiDeclarativeListProperty::at((int )index0);
    }
}

bool  QtJambiShell_QDeclarativeListProperty::canAppend()
{
    QTJAMBI_DEBUG_TRACE("(shell) entering: QDeclarativeListProperty::canAppend()");
    jmethodID method_id = m_vtable->method(2);
    if (method_id) {
        JNIEnv *__jni_env = qtjambi_current_environment();
        if (!__jni_env) {
            return QtjambiDeclarativeListProperty::canAppend();
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
            qtjambishell_throw_nullpointerexception(__jni_env, "QDeclarativeListProperty::canAppend()");
            __qt_return_value =  false;
        }
        qtjambi_exception_check(__jni_env);
        __jni_env->PopLocalFrame(0);
        QTJAMBI_DEBUG_TRACE("(shell) -> leaving: QDeclarativeListProperty::canAppend()");
        return __qt_return_value;
    } else {
        QTJAMBI_DEBUG_TRACE("(shell) -> super() and leaving: QDeclarativeListProperty::canAppend()");
        return QtjambiDeclarativeListProperty::canAppend();
    }
}

bool  QtJambiShell_QDeclarativeListProperty::canAt()
{
    QTJAMBI_DEBUG_TRACE("(shell) entering: QDeclarativeListProperty::canAt()");
    jmethodID method_id = m_vtable->method(3);
    if (method_id) {
        JNIEnv *__jni_env = qtjambi_current_environment();
        if (!__jni_env) {
            return QtjambiDeclarativeListProperty::canAt();
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
            qtjambishell_throw_nullpointerexception(__jni_env, "QDeclarativeListProperty::canAt()");
            __qt_return_value =  false;
        }
        qtjambi_exception_check(__jni_env);
        __jni_env->PopLocalFrame(0);
        QTJAMBI_DEBUG_TRACE("(shell) -> leaving: QDeclarativeListProperty::canAt()");
        return __qt_return_value;
    } else {
        QTJAMBI_DEBUG_TRACE("(shell) -> super() and leaving: QDeclarativeListProperty::canAt()");
        return QtjambiDeclarativeListProperty::canAt();
    }
}

bool  QtJambiShell_QDeclarativeListProperty::canClear()
{
    QTJAMBI_DEBUG_TRACE("(shell) entering: QDeclarativeListProperty::canClear()");
    jmethodID method_id = m_vtable->method(4);
    if (method_id) {
        JNIEnv *__jni_env = qtjambi_current_environment();
        if (!__jni_env) {
            return QtjambiDeclarativeListProperty::canClear();
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
            qtjambishell_throw_nullpointerexception(__jni_env, "QDeclarativeListProperty::canClear()");
            __qt_return_value =  false;
        }
        qtjambi_exception_check(__jni_env);
        __jni_env->PopLocalFrame(0);
        QTJAMBI_DEBUG_TRACE("(shell) -> leaving: QDeclarativeListProperty::canClear()");
        return __qt_return_value;
    } else {
        QTJAMBI_DEBUG_TRACE("(shell) -> super() and leaving: QDeclarativeListProperty::canClear()");
        return QtjambiDeclarativeListProperty::canClear();
    }
}

bool  QtJambiShell_QDeclarativeListProperty::canCount()
{
    QTJAMBI_DEBUG_TRACE("(shell) entering: QDeclarativeListProperty::canCount()");
    jmethodID method_id = m_vtable->method(5);
    if (method_id) {
        JNIEnv *__jni_env = qtjambi_current_environment();
        if (!__jni_env) {
            return QtjambiDeclarativeListProperty::canCount();
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
            qtjambishell_throw_nullpointerexception(__jni_env, "QDeclarativeListProperty::canCount()");
            __qt_return_value =  false;
        }
        qtjambi_exception_check(__jni_env);
        __jni_env->PopLocalFrame(0);
        QTJAMBI_DEBUG_TRACE("(shell) -> leaving: QDeclarativeListProperty::canCount()");
        return __qt_return_value;
    } else {
        QTJAMBI_DEBUG_TRACE("(shell) -> super() and leaving: QDeclarativeListProperty::canCount()");
        return QtjambiDeclarativeListProperty::canCount();
    }
}

void QtJambiShell_QDeclarativeListProperty::clear()
{
    QTJAMBI_DEBUG_TRACE("(shell) entering: QDeclarativeListProperty::clear()");
    jmethodID method_id = m_vtable->method(6);
    if (method_id) {
        JNIEnv *__jni_env = qtjambi_current_environment();
        if (!__jni_env) {
            QtjambiDeclarativeListProperty::clear();
            return;
        }
        __jni_env->PushLocalFrame(100);
        jobject __java_return_value_object = m_link->javaObject(__jni_env);
        if(__jni_env->IsSameObject(__java_return_value_object, 0) == JNI_FALSE) {
            __jni_env->CallVoidMethod(__java_return_value_object, method_id);
            qtjambi_exception_check(__jni_env);
        } else {
            qtjambishell_throw_nullpointerexception(__jni_env, "QDeclarativeListProperty::clear()");
        }
        qtjambi_exception_check(__jni_env);
        __jni_env->PopLocalFrame(0);
        QTJAMBI_DEBUG_TRACE("(shell) -> leaving: QDeclarativeListProperty::clear()");
    } else {
        QTJAMBI_DEBUG_TRACE("(shell) -> super() and leaving: QDeclarativeListProperty::clear()");
        QtjambiDeclarativeListProperty::clear();
    }
}

int  QtJambiShell_QDeclarativeListProperty::count()
{
    QTJAMBI_DEBUG_TRACE("(shell) entering: QDeclarativeListProperty::count()");
    jmethodID method_id = m_vtable->method(7);
    if (method_id) {
        JNIEnv *__jni_env = qtjambi_current_environment();
        if (!__jni_env) {
            return QtjambiDeclarativeListProperty::count();
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
            qtjambishell_throw_nullpointerexception(__jni_env, "QDeclarativeListProperty::count()");
            __qt_return_value =  0;
        }
        qtjambi_exception_check(__jni_env);
        __jni_env->PopLocalFrame(0);
        QTJAMBI_DEBUG_TRACE("(shell) -> leaving: QDeclarativeListProperty::count()");
        return __qt_return_value;
    } else {
        QTJAMBI_DEBUG_TRACE("(shell) -> super() and leaving: QDeclarativeListProperty::count()");
        return QtjambiDeclarativeListProperty::count();
    }
}

QObject*  QtJambiShell_QDeclarativeListProperty::object()
{
    QTJAMBI_DEBUG_TRACE("(shell) entering: QDeclarativeListProperty::object()");
    jmethodID method_id = m_vtable->method(8);
    if (method_id) {
        JNIEnv *__jni_env = qtjambi_current_environment();
        if (!__jni_env) {
            return QtjambiDeclarativeListProperty::object();
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
            qtjambishell_throw_nullpointerexception(__jni_env, "QDeclarativeListProperty::object()");
            __qt_return_value =  0;
        }
        qtjambi_exception_check(__jni_env);
        __jni_env->PopLocalFrame(0);
        QTJAMBI_DEBUG_TRACE("(shell) -> leaving: QDeclarativeListProperty::object()");
        return __qt_return_value;
    } else {
        QTJAMBI_DEBUG_TRACE("(shell) -> super() and leaving: QDeclarativeListProperty::object()");
        return QtjambiDeclarativeListProperty::object();
    }
}

// emitting Functions in shell class (nonVirtualShellFunctions)
// emitting Public Override Functions (publicOverrideFunctions)
// emitting Virtual Override Functions (virtualOverrideFunctions)
void QtJambiShell_QDeclarativeListProperty::__override_append(QObject*  object0, bool static_call)
{
    if (static_call) {
        QtjambiDeclarativeListProperty::append(object0);
    } else {
        append(object0);
    }
}

QObject*  QtJambiShell_QDeclarativeListProperty::__override_at(int  index0, bool static_call)
{
    if (static_call) {
        return QtjambiDeclarativeListProperty::at((int )index0);
    } else {
        return at((int )index0);
    }
}

bool  QtJambiShell_QDeclarativeListProperty::__override_canAppend(bool static_call)
{
    if (static_call) {
        return QtjambiDeclarativeListProperty::canAppend();
    } else {
        return canAppend();
    }
}

bool  QtJambiShell_QDeclarativeListProperty::__override_canAt(bool static_call)
{
    if (static_call) {
        return QtjambiDeclarativeListProperty::canAt();
    } else {
        return canAt();
    }
}

bool  QtJambiShell_QDeclarativeListProperty::__override_canClear(bool static_call)
{
    if (static_call) {
        return QtjambiDeclarativeListProperty::canClear();
    } else {
        return canClear();
    }
}

bool  QtJambiShell_QDeclarativeListProperty::__override_canCount(bool static_call)
{
    if (static_call) {
        return QtjambiDeclarativeListProperty::canCount();
    } else {
        return canCount();
    }
}

void QtJambiShell_QDeclarativeListProperty::__override_clear(bool static_call)
{
    if (static_call) {
        QtjambiDeclarativeListProperty::clear();
    } else {
        clear();
    }
}

int  QtJambiShell_QDeclarativeListProperty::__override_count(bool static_call)
{
    if (static_call) {
        return QtjambiDeclarativeListProperty::count();
    } else {
        return count();
    }
}

QObject*  QtJambiShell_QDeclarativeListProperty::__override_object(bool static_call)
{
    if (static_call) {
        return QtjambiDeclarativeListProperty::object();
    } else {
        return object();
    }
}

// emitting (writeExtraFunctions)
// emitting (writeToStringFunction)
// emitting (writeCloneFunction)

extern "C" Q_DECL_EXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_org_qtjambi_qt_Declarative_QDeclarativeListProperty__1_1qt_1clone)
(JNIEnv *__jni_env, jobject, jlong __this_nativeId)
{
    QtjambiDeclarativeListProperty *__qt_this = (QtjambiDeclarativeListProperty *) qtjambi_from_jlong(__this_nativeId);
    QTJAMBI_EXCEPTION_CHECK(__jni_env);
    Q_ASSERT(__qt_this);
    QtjambiDeclarativeListProperty *res = __qt_this;
    return qtjambi_from_object(__jni_env, res, "QDeclarativeListProperty", "org/qtjambi/qt/declarative/", true);
}

// emitting (writeSignalFunction)
// emitting  (functionsInTargetLang writeFinalFunction)
// QDeclarativeListProperty::QtjambiDeclarativeListProperty()
extern "C" Q_DECL_EXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_org_qtjambi_qt_Declarative_QDeclarativeListProperty__1_1qt_1QDeclarativeListProperty__)
(JNIEnv *__jni_env,
 jobject __jni_object)
{
    QTJAMBI_DEBUG_TRACE("(native) entering: QDeclarativeListProperty::QtjambiDeclarativeListProperty()");
    Q_UNUSED(__jni_env)
    Q_UNUSED(__jni_object)
    QtJambiShell_QDeclarativeListProperty *__qt_this = new QtJambiShell_QDeclarativeListProperty();
    QtJambiLink *__qt_java_link = qtjambi_construct_object(__jni_env, __jni_object, __qt_this, "QtjambiDeclarativeListProperty");
    if (!__qt_java_link) {
        qWarning("object construction failed for type: QtjambiDeclarativeListProperty");
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
    QTJAMBI_DEBUG_TRACE("(native) -> leaving: QDeclarativeListProperty::QtjambiDeclarativeListProperty()");

}

// QDeclarativeListProperty::QtjambiDeclarativeListProperty(QObject * o, QList<QObject > & list)
extern "C" Q_DECL_EXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_org_qtjambi_qt_Declarative_QDeclarativeListProperty__1_1qt_1QDeclarativeListProperty_1QObject_1List__JLjava_util_List_2)
(JNIEnv *__jni_env,
 jobject __jni_object,
 jlong  o0,
 jobject list1)
{
    QTJAMBI_DEBUG_TRACE("(native) entering: QDeclarativeListProperty::QtjambiDeclarativeListProperty(QObject * o, QList<QObject > & list)");
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
    QtJambiShell_QDeclarativeListProperty *__qt_this = new QtJambiShell_QDeclarativeListProperty((QObject* )__qt_o0, __qt_list1);
    QtJambiLink *__qt_java_link = qtjambi_construct_object(__jni_env, __jni_object, __qt_this, "QtjambiDeclarativeListProperty");
    if (!__qt_java_link) {
        qWarning("object construction failed for type: QtjambiDeclarativeListProperty");
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
    QTJAMBI_DEBUG_TRACE("(native) -> leaving: QDeclarativeListProperty::QtjambiDeclarativeListProperty(QObject * o, QList<jobject > & list)");

}

// QDeclarativeListProperty::append(jobject object)
extern "C" Q_DECL_EXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_org_qtjambi_qt_Declarative_QDeclarativeListProperty__1_1qt_1append_1QObject__JLorg_qtjambi_qt_core_QObject_2)
(JNIEnv *__jni_env,
 jobject,
 jlong __this_nativeId,
 jobject object0)
{
    QTJAMBI_DEBUG_TRACE("(native) entering: QDeclarativeListProperty::append(jobject object)");
    Q_UNUSED(__jni_env)
    Q_UNUSED(__this_nativeId)
    QTJAMBI_EXCEPTION_CHECK(__jni_env);
    QtJambiShell_QDeclarativeListProperty *__qt_this = (QtJambiShell_QDeclarativeListProperty *) qtjambi_from_jlong(__this_nativeId);
    QTJAMBI_EXCEPTION_CHECK(__jni_env);
    Q_ASSERT(__qt_this);
    bool __do_static_call = __this_nativeId ? ((QtJambiLink *) __this_nativeId)->createdByJava() : false;
    __qt_this->__override_append(qtjambi_to_qobject(__jni_env, object0), __do_static_call);
    QTJAMBI_DEBUG_TRACE("(native) -> leaving: QDeclarativeListProperty::append(jobject object)");

}

// QDeclarativeListProperty::at(int index)
extern "C" Q_DECL_EXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_org_qtjambi_qt_Declarative_QDeclarativeListProperty__1_1qt_1at_1int__JI)
(JNIEnv *__jni_env,
 jobject,
 jlong __this_nativeId,
 jint index0)
{
    QTJAMBI_DEBUG_TRACE("(native) entering: QDeclarativeListProperty::at(int index)");
    Q_UNUSED(__jni_env)
    Q_UNUSED(__this_nativeId)
    QtJambiShell_QDeclarativeListProperty *__qt_this = (QtJambiShell_QDeclarativeListProperty *) qtjambi_from_jlong(__this_nativeId);
    QTJAMBI_EXCEPTION_CHECK(__jni_env);
    Q_ASSERT(__qt_this);
    bool __do_static_call = __this_nativeId ? ((QtJambiLink *) __this_nativeId)->createdByJava() : false;
    QObject*  qt_return_value = __qt_this->__override_at((int )index0, __do_static_call);
    jobject __qt_return_value = qtjambi_from_QObject(__jni_env, qt_return_value);
    QTJAMBI_EXCEPTION_CHECK(__jni_env);
    QTJAMBI_DEBUG_TRACE("(native) -> leaving: QDeclarativeListProperty::at(int index)");
    return __qt_return_value;
}

// QDeclarativeListProperty::canAppend()
extern "C" Q_DECL_EXPORT jboolean JNICALL QTJAMBI_FUNCTION_PREFIX(Java_org_qtjambi_qt_Declarative_QDeclarativeListProperty__1_1qt_1canAppend__J)
(JNIEnv *__jni_env,
 jobject,
 jlong __this_nativeId)
{
    QTJAMBI_DEBUG_TRACE("(native) entering: QDeclarativeListProperty::canAppend()");
    Q_UNUSED(__jni_env)
    Q_UNUSED(__this_nativeId)
    QtJambiShell_QDeclarativeListProperty *__qt_this = (QtJambiShell_QDeclarativeListProperty *) qtjambi_from_jlong(__this_nativeId);
    QTJAMBI_EXCEPTION_CHECK(__jni_env);
    Q_ASSERT(__qt_this);
    bool __do_static_call = __this_nativeId ? ((QtJambiLink *) __this_nativeId)->createdByJava() : false;
    bool  __qt_return_value = __qt_this->__override_canAppend(__do_static_call);

    jboolean __java_return_value = (jboolean) __qt_return_value;
    QTJAMBI_EXCEPTION_CHECK(__jni_env);
    QTJAMBI_DEBUG_TRACE("(native) -> leaving: QDeclarativeListProperty::canAppend()");
    return __java_return_value;
}

// QDeclarativeListProperty::canAt()
extern "C" Q_DECL_EXPORT jboolean JNICALL QTJAMBI_FUNCTION_PREFIX(Java_org_qtjambi_qt_Declarative_QDeclarativeListProperty__1_1qt_1canAt__J)
(JNIEnv *__jni_env,
 jobject,
 jlong __this_nativeId)
{
    QTJAMBI_DEBUG_TRACE("(native) entering: QDeclarativeListProperty::canAt()");
    Q_UNUSED(__jni_env)
    Q_UNUSED(__this_nativeId)
    QtJambiShell_QDeclarativeListProperty *__qt_this = (QtJambiShell_QDeclarativeListProperty *) qtjambi_from_jlong(__this_nativeId);
    QTJAMBI_EXCEPTION_CHECK(__jni_env);
    Q_ASSERT(__qt_this);
    bool __do_static_call = __this_nativeId ? ((QtJambiLink *) __this_nativeId)->createdByJava() : false;
    bool  __qt_return_value = __qt_this->__override_canAt(__do_static_call);

    jboolean __java_return_value = (jboolean) __qt_return_value;
    QTJAMBI_EXCEPTION_CHECK(__jni_env);
    QTJAMBI_DEBUG_TRACE("(native) -> leaving: QDeclarativeListProperty::canAt()");
    return __java_return_value;
}

// QDeclarativeListProperty::canClear()
extern "C" Q_DECL_EXPORT jboolean JNICALL QTJAMBI_FUNCTION_PREFIX(Java_org_qtjambi_qt_Declarative_QDeclarativeListProperty__1_1qt_1canClear__J)
(JNIEnv *__jni_env,
 jobject,
 jlong __this_nativeId)
{
    QTJAMBI_DEBUG_TRACE("(native) entering: QDeclarativeListProperty::canClear()");
    Q_UNUSED(__jni_env)
    Q_UNUSED(__this_nativeId)
    QtJambiShell_QDeclarativeListProperty *__qt_this = (QtJambiShell_QDeclarativeListProperty *) qtjambi_from_jlong(__this_nativeId);
    QTJAMBI_EXCEPTION_CHECK(__jni_env);
    Q_ASSERT(__qt_this);
    bool __do_static_call = __this_nativeId ? ((QtJambiLink *) __this_nativeId)->createdByJava() : false;
    bool  __qt_return_value = __qt_this->__override_canClear(__do_static_call);

    jboolean __java_return_value = (jboolean) __qt_return_value;
    QTJAMBI_EXCEPTION_CHECK(__jni_env);
    QTJAMBI_DEBUG_TRACE("(native) -> leaving: QDeclarativeListProperty::canClear()");
    return __java_return_value;
}

// QDeclarativeListProperty::canCount()
extern "C" Q_DECL_EXPORT jboolean JNICALL QTJAMBI_FUNCTION_PREFIX(Java_org_qtjambi_qt_Declarative_QDeclarativeListProperty__1_1qt_1canCount__J)
(JNIEnv *__jni_env,
 jobject,
 jlong __this_nativeId)
{
    QTJAMBI_DEBUG_TRACE("(native) entering: QDeclarativeListProperty::canCount()");
    Q_UNUSED(__jni_env)
    Q_UNUSED(__this_nativeId)
    QtJambiShell_QDeclarativeListProperty *__qt_this = (QtJambiShell_QDeclarativeListProperty *) qtjambi_from_jlong(__this_nativeId);
    QTJAMBI_EXCEPTION_CHECK(__jni_env);
    Q_ASSERT(__qt_this);
    bool __do_static_call = __this_nativeId ? ((QtJambiLink *) __this_nativeId)->createdByJava() : false;
    bool  __qt_return_value = __qt_this->__override_canCount(__do_static_call);

    jboolean __java_return_value = (jboolean) __qt_return_value;
    QTJAMBI_EXCEPTION_CHECK(__jni_env);
    QTJAMBI_DEBUG_TRACE("(native) -> leaving: QDeclarativeListProperty::canCount()");
    return __java_return_value;
}

// QDeclarativeListProperty::clear()
extern "C" Q_DECL_EXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_org_qtjambi_qt_Declarative_QDeclarativeListProperty__1_1qt_1clear__J)
(JNIEnv *__jni_env,
 jobject,
 jlong __this_nativeId)
{
    QTJAMBI_DEBUG_TRACE("(native) entering: QDeclarativeListProperty::clear()");
    Q_UNUSED(__jni_env)
    Q_UNUSED(__this_nativeId)
    QtJambiShell_QDeclarativeListProperty *__qt_this = (QtJambiShell_QDeclarativeListProperty *) qtjambi_from_jlong(__this_nativeId);
    QTJAMBI_EXCEPTION_CHECK(__jni_env);
    Q_ASSERT(__qt_this);
    bool __do_static_call = __this_nativeId ? ((QtJambiLink *) __this_nativeId)->createdByJava() : false;
    __qt_this->__override_clear(__do_static_call);
    QTJAMBI_DEBUG_TRACE("(native) -> leaving: QDeclarativeListProperty::clear()");

}

// QDeclarativeListProperty::count()
extern "C" Q_DECL_EXPORT jint JNICALL QTJAMBI_FUNCTION_PREFIX(Java_org_qtjambi_qt_Declarative_QDeclarativeListProperty__1_1qt_1count__J)
(JNIEnv *__jni_env,
 jobject,
 jlong __this_nativeId)
{
    QTJAMBI_DEBUG_TRACE("(native) entering: QDeclarativeListProperty::count()");
    Q_UNUSED(__jni_env)
    Q_UNUSED(__this_nativeId)
    QtJambiShell_QDeclarativeListProperty *__qt_this = (QtJambiShell_QDeclarativeListProperty *) qtjambi_from_jlong(__this_nativeId);
    QTJAMBI_EXCEPTION_CHECK(__jni_env);
    Q_ASSERT(__qt_this);
    bool __do_static_call = __this_nativeId ? ((QtJambiLink *) __this_nativeId)->createdByJava() : false;
    int  __qt_return_value = __qt_this->__override_count(__do_static_call);

    jint __java_return_value = (jint) __qt_return_value;
    QTJAMBI_EXCEPTION_CHECK(__jni_env);
    QTJAMBI_DEBUG_TRACE("(native) -> leaving: QDeclarativeListProperty::count()");
    return __java_return_value;
}

// QDeclarativeListProperty::object()
extern "C" Q_DECL_EXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_org_qtjambi_qt_Declarative_QDeclarativeListProperty__1_1qt_1object__J)
(JNIEnv *__jni_env,
 jobject,
 jlong __this_nativeId)
{
    QTJAMBI_DEBUG_TRACE("(native) entering: QDeclarativeListProperty::object()");
    Q_UNUSED(__jni_env)
    Q_UNUSED(__this_nativeId)
    QtJambiShell_QDeclarativeListProperty *__qt_this = (QtJambiShell_QDeclarativeListProperty *) qtjambi_from_jlong(__this_nativeId);
    QTJAMBI_EXCEPTION_CHECK(__jni_env);
    Q_ASSERT(__qt_this);
    bool __do_static_call = __this_nativeId ? ((QtJambiLink *) __this_nativeId)->createdByJava() : false;
    QObject*  __qt_return_value = __qt_this->__override_object(__do_static_call);

    jobject __java_return_value = qtjambi_from_qobject(__jni_env, (QObject *) __qt_return_value, "QObject", "org/qtjambi/qt/core/");
    QTJAMBI_EXCEPTION_CHECK(__jni_env);
    QTJAMBI_DEBUG_TRACE("(native) -> leaving: QDeclarativeListProperty::object()");
    return __java_return_value;
}

// emitting (AbstractMetaClass::NormalFunctions|AbstractMetaClass::AbstractFunctions writeFinalFunction)
// emitting Field accessors (writeFieldAccessors)
// emitting (writeFromNativeFunction)
extern "C" Q_DECL_EXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_org_qtjambi_qt_Declarative_QDeclarativeListProperty_fromNativePointer)
(JNIEnv *__jni_env,
 jclass,
 jobject nativePointer)
{
    void *ptr = qtjambi_to_cpointer(__jni_env, nativePointer, 1);
    return qtjambi_from_object(__jni_env, ptr, "QDeclarativeListProperty", "org/qtjambi/qt/declarative/", true);
}
// emitting (writeFromArrayFunction)
extern "C" Q_DECL_EXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_org_qtjambi_qt_Declarative_QDeclarativeListProperty_nativePointerArray)
(JNIEnv *__jni_env,
 jclass,
 jobjectArray array)
{
    return qtjambi_array_to_nativepointer(__jni_env,
                                         array,
                                         sizeof(QtjambiDeclarativeListProperty));
}
// emitting (writeInterfaceCastFunction)
// emitting (writeSignalInitialization)
// emitting (writeJavaLangObjectOverrideFunctions)


