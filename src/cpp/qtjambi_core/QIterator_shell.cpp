/****************************************************************************
**
** Copyright (C) 2009-2020 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
**
** This file is part of Qt Jambi.
**
** ** $BEGIN_LICENSE$
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

#include <QtCore/QByteArrayList>
#include <QtCore/QQueue>
#include <QtCore/QQueue>
#include <QtCore/QStringList>
#include <QtCore/QVariant>

#include <qtjambi/qtjambi_core.h>
#include <qtjambi/qtjambi_repository.h>

// emitting (writeExtraFunctions)
// emitting (writeToStringFunction)
// emitting (writeSignalFunction)
// emitting  (functionsInTargetLang writeFinalFunction)
// Iterator::operator*() const
extern "C" Q_DECL_EXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QMapIterator__1_1qt_1QMapIterator_1key__JJ)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId,
 jlong keyFunction)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "Iterator::key() const")
    try{
        void *__qt_this = qtjambi_from_nativeId(__this_nativeId);
        qtjambi_check_resource(__jni_env, __qt_this, typeid(QMap<QVariant,QVariant>::const_iterator));
        return reinterpret_cast<QMapIteratorKeyFunction>(keyFunction)(__jni_env, __qt_this);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
    return nullptr;
}

extern "C" Q_DECL_EXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QIterator__1_1qt_1QIterator_1value__JJ)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId,
 jlong valueFunction)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "Iterator::operator*() const")
    try{
        void *__qt_this = qtjambi_from_nativeId(__this_nativeId);
        qtjambi_check_resource(__jni_env, __qt_this, typeid(QList<QVariant>::const_iterator));
        return reinterpret_cast<QIteratorValueFunction>(valueFunction)(__jni_env, __qt_this);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
    return nullptr;
}

// Iterator::operator++()
extern "C" Q_DECL_EXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QIterator__1_1qt_1QIterator_1increment__JJ)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId,
 jlong incrementFunction)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "Iterator::operator++()")
    try{
        void *__qt_this = qtjambi_from_nativeId(__this_nativeId);
        qtjambi_check_resource(__jni_env, __qt_this, typeid(QList<QVariant>::const_iterator));
        reinterpret_cast<QIteratorIncrementFunction>(incrementFunction)(__qt_this);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
}

// Iterator::operator--()
extern "C" Q_DECL_EXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QIterator__1_1qt_1QIterator_1decrement__JJ)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId,
 jlong decrementFunction)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "Iterator::operator--()")
    try{
        void *__qt_this = qtjambi_from_nativeId(__this_nativeId);
        qtjambi_check_resource(__jni_env, __qt_this, typeid(QList<QVariant>::const_iterator));
        reinterpret_cast<QIteratorDecrementFunction>(decrementFunction)(__qt_this);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
}

// Iterator::operator<(const Iterator & other) const
extern "C" Q_DECL_EXPORT jboolean JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QIterator__1_1qt_1QIterator_1lessThan__JJJ)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId,
 QtJambiNativeID other0,
 jlong lessThanFunction)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "Iterator::operator<(const Iterator & other) const")
    try{
        void *__qt_this = qtjambi_from_nativeId(__this_nativeId);
        qtjambi_check_resource(__jni_env, __qt_this, typeid(QList<QVariant>::const_iterator));
        void *__qt_other0 = qtjambi_from_nativeId(other0);
        qtjambi_check_resource(__jni_env, __qt_other0, typeid(QList<QVariant>::const_iterator));
        return reinterpret_cast<QIteratorLessThanFunction>(lessThanFunction)(__qt_this, __qt_other0);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
    return false;
}

// Iterator::operator==(const Iterator & o) const
extern "C" Q_DECL_EXPORT jboolean JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QIterator__1_1qt_1QIterator_1operator_1equal__JJJ)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId,
 QtJambiNativeID o0,
 jlong equalsFunction)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "Iterator::operator==(const Iterator & o) const")
    try{
        void *__qt_this = qtjambi_from_nativeId(__this_nativeId);
        qtjambi_check_resource(__jni_env, __qt_this, typeid(QList<QVariant>::const_iterator));
        void *__qt_o0 = qtjambi_from_nativeId(o0);
        qtjambi_check_resource(__jni_env, __qt_o0, typeid(QList<QVariant>::const_iterator));
        return reinterpret_cast<QIteratorEqualsFunction>(equalsFunction)(__qt_this, __qt_o0);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
    return false;
}

// emitting (AbstractMetaClass::NormalFunctions|AbstractMetaClass::AbstractFunctions writeFinalFunction)
// emitting Field accessors (writeFieldAccessors)
// emitting (writeInterfaceCastFunction)
// emitting (writeSignalInitialization)
// emitting (writeJavaLangObjectOverrideFunctions)


