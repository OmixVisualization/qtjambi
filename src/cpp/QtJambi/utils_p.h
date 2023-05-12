/****************************************************************************
**
** Copyright (C) 2009-2023 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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

#if !defined(QTJAMBI_UTILS_P_H) && !defined(QTJAMBI_GENERATOR_RUNNING)
#define QTJAMBI_UTILS_P_H

#include <QtCore/QtGlobal>

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
namespace QtPrivate{
    struct AbstractConverterFunction;
}

bool registerConverter(const QtPrivate::AbstractConverterFunction *f, int from, int to);
const QtPrivate::AbstractComparatorFunction * registeredComparator(int typeId);
const QtPrivate::AbstractDebugStreamFunction * registeredDebugStreamOperator(int typeId);
bool isLessThan(const QMetaType& keyMetaType, const void * ptr, const void* ptr2);
bool isEquals(const QMetaType& keyMetaType, const void * ptr, const void* ptr2);
#endif

#ifdef Q_OS_ANDROID
#define unique_id(id) qHash(QLatin1String((id).name()))
#define typeid_equals(t1, t2) unique_id(t1)==unique_id(t2)
#define typeid_not_equals(t1, t2) unique_id(t1)!=unique_id(t2)
#else
#define unique_id(id) (id).hash_code()
#define typeid_equals(t1, t2) t1==t2
#define typeid_not_equals(t1, t2) t1!=t2
#endif


QString getFunctionLibraryPath(QFunctionPointer function);

bool isValidArray(JNIEnv *env, jobject object, jclass contentType);

jobject resolveLongEnum(JNIEnv *env, jint hashCode, jclass enumClass, jlong value, jstring entryName);
jobject resolveByteEnum(JNIEnv *env, jint hashCode, jclass enumClass, jbyte value, jstring entryName);
jobject resolveShortEnum(JNIEnv *env, jint hashCode, jclass enumClass, jshort value, jstring entryName);
jobject resolveIntEnum(JNIEnv *env, jint hashCode, jclass enumClass, jint value, jstring entryName);

bool simpleEventNotify(void **data);
bool threadAffineEventNotify(void **data);
void enableThreadAffinity(bool enabled);

bool enabledDanglingPointerCheck(JNIEnv * env = nullptr);

const std::type_info* tryGetTypeInfo(JNIEnv *env, TypeInfoSupplier typeInfoSupplier, const void* ptr);
const std::type_info* checkedGetTypeInfo(TypeInfoSupplier typeInfoSupplier, const void* ptr);

typedef QObject*(*SmartPointerQObjectGetter)(const void *);
typedef std::function<void*(const void *)> SmartPointerGetterFunction;
typedef void(*SmartPointerDeleter)(void *, bool);

enum class NativeToJavaConversionMode{
    None,
    MakeCopyOfValues,
    TransferOwnership
};

jobject internal_convertNativeToJavaObject(JNIEnv *env, const void *qt_object, const std::type_info& typeId, const char *qtName, NativeToJavaConversionMode mode, bool *ok = nullptr);
jobject internal_convertSmartPointerToJavaObject(JNIEnv *env, const char *className,
                            void* ptr_shared_pointer, SmartPointerDeleter sharedPointerDeleter, SmartPointerGetterFunction sharedPointerGetter);
jobject internal_convertSmartPointerToJavaInterface(JNIEnv *env, const std::type_info& interfaceType,
                            void* ptr_shared_pointer, SmartPointerDeleter sharedPointerDeleter, SmartPointerGetterFunction sharedPointerGetter);

#endif // QTJAMBI_UTILS_P_H
