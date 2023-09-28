/****************************************************************************
**
** Copyright (C) 2009-2023 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
**
** This file is part of Qt Jambi.
**
** $BEGIN_LICENSE$
**
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
**
** $END_LICENSE$
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

#include <QtCore/QReadWriteLock>
#include <QtCore/QList>
#include "qtjambiapi.h"
#include "jobjectwrapper.h"
#include "coreapi.h"
#include "java_p.h"
#include "supertypeinfo_p.h"
#include "qtjambilink_p.h"
#include "qtjambi_cast.h"

Q_GLOBAL_STATIC_WITH_ARGS(QReadWriteLock, gPointerLock, (QReadWriteLock::Recursive))
typedef QHash<quintptr, QList<JObjectWrapper>> ObjectsByFunctionPointer;
Q_GLOBAL_STATIC(ObjectsByFunctionPointer, gObjectsByFunctionPointer)

static int MAX_NARGS = 256;

struct ffi_type {
    size_t size;
    short alignment;
    short type;
    ffi_type const** elements;
    ffi_type() : size(0), alignment(1), type(13), elements(nullptr) {}
    ffi_type(size_t _size,
            short _alignment,
            short _type,
             ffi_type const** _elements) : size(_size), alignment(_alignment), type(_type), elements(_elements) {}
};

#define FFI_TYPE_VOID       0
#define FFI_TYPE_INT        1
#define FFI_TYPE_FLOAT      2
#define FFI_TYPE_DOUBLE     3
#define FFI_TYPE_UINT8      5
#define FFI_TYPE_SINT8      6
#define FFI_TYPE_UINT16     7
#define FFI_TYPE_SINT16     8
#define FFI_TYPE_UINT32     9
#define FFI_TYPE_SINT32     10
#define FFI_TYPE_UINT64     11
#define FFI_TYPE_SINT64     12
#define FFI_TYPE_STRUCT     13
#define FFI_TYPE_POINTER    14
#define FFI_TYPE_COMPLEX    15

#define FFI_TYPEDEF(name, type, id)\
static const ffi_type ffi_type_##name = { \
  sizeof(type), \
  alignof(type), \
  id, nullptr \
}

static const ffi_type ffi_type_void = {
  1, 1, FFI_TYPE_VOID, nullptr
};

FFI_TYPEDEF(quint8, quint8, FFI_TYPE_UINT8);
FFI_TYPEDEF(qint8, qint8, FFI_TYPE_SINT8);
FFI_TYPEDEF(quint16, quint16, FFI_TYPE_UINT16);
FFI_TYPEDEF(qint16, qint16, FFI_TYPE_SINT16);
FFI_TYPEDEF(quint32, quint32, FFI_TYPE_UINT32);
FFI_TYPEDEF(qint32, qint32, FFI_TYPE_SINT32);
FFI_TYPEDEF(quint64, quint64, FFI_TYPE_UINT64);
FFI_TYPEDEF(qint64, qint64, FFI_TYPE_SINT64);

FFI_TYPEDEF(pointer, void*, FFI_TYPE_POINTER);

FFI_TYPEDEF(float, float, FFI_TYPE_FLOAT);
FFI_TYPEDEF(double, double, FFI_TYPE_DOUBLE);

void ffi_funtion_dispatch(JNIEnv *env, void* func, jobjectArray args, ffi_type *return_type, void *presult);

struct Cleanup : QSharedData{
    Cleanup(std::function<void()>&& _task) :QSharedData(), task(std::move(_task)){}
    ~Cleanup(){ if(task) task(); }
    std::function<void()> task;
};

void delete_callback_pointer(void* ptr, bool){
    delete reinterpret_cast<QFunctionPointer*>(ptr);
}

void convertArgumentList(QVector<QSharedDataPointer<Cleanup>>& cleaners, QVector<std::function<void(JNIEnv *)>>& successActions, JNIEnv * __jni_env, jobjectArray arguments, jsize length){
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    static QMetaType jniEnvType(QMetaType::type("JNIEnv*"));
    static QMetaType jniObjType(QMetaType::type("jobject"));
    static QMetaType stringViewType(QMetaType::type("QStringView"));
#else
    static QMetaType jniEnvType = QMetaType::fromName("JNIEnv*");
    static QMetaType jniObjType = QMetaType::fromName("jobject");
    static QMetaType stringViewType = QMetaType::fromName("QStringView");
#endif
    for(jsize i = 0; i<length; ++i){
        jobject arg = __jni_env->GetObjectArrayElement(arguments, i);
        jobject val = nullptr;
        jint argPointerOrReference = 0;
        const QMetaType* argMetaType = nullptr;
        jclass argClassType = nullptr;
        bool resolved = false;
        bool isReferenceMetaType = false;
        QScopedPointer<const QMetaType> _argMetaType;
        if(Java::QtCore::QGenericArgument::isInstanceOf(__jni_env, arg)){
            argMetaType = qtjambi_cast<QMetaType*>(__jni_env, Java::QtCore::QGenericArgumentType::metaType(__jni_env, arg));
            if(argMetaType)
                isReferenceMetaType = QByteArray(argMetaType->name()).endsWith("&");
            argPointerOrReference = Java::QtCore::QGenericArgumentType::pointerOrReference(__jni_env, arg);
            argClassType = Java::QtCore::QGenericArgumentType::classType(__jni_env, arg);
            val = arg = Java::QtCore::QGenericArgument::value(__jni_env, arg);
            if(argMetaType){
                if(argMetaType->id()==QMetaType::Nullptr){
                    arg = Java::JNA::Pointer::newInstance(__jni_env, 0);
                    resolved = true;
                }else if(jniEnvType==*argMetaType){
                    arg = Java::JNA::Pointer::newInstance(__jni_env, jlong(__jni_env));
                    resolved = true;
                }else if(jniObjType==*argMetaType){
                    arg = Java::JNA::Pointer::newInstance(__jni_env, jlong(val));
                    resolved = true;
                }
            }
        }else{
            val = arg;
            if(val){
                argClassType = __jni_env->GetObjectClass(val);
                if(!Java::Runtime::Class::isArray(__jni_env, argClassType)){
                    argMetaType = qtjambi_cast<const QMetaType*>(__jni_env, Java::QtCore::QGenericArgument::metaType(__jni_env, val));
                    if(!argMetaType->isValid()){
                        argMetaType = nullptr;
                    }else
                        argClassType = CoreAPI::getClassForMetaType(__jni_env, *argMetaType);
                }
            }else{
                arg = Java::JNA::Pointer::newInstance(__jni_env, 0);
                resolved = true;
            }
        }
        if(!resolved && argPointerOrReference==2 && argClassType){
            QFunctionPointer ptr = nullptr;
            if(val){
                if(QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForJavaInterface(__jni_env, val)){
                    if(DeletablePointerToObjectLink* plink = dynamic_cast<DeletablePointerToObjectLink*>(link.get())){
                        if(plink->pointer() && plink->deleterFunction()==&delete_callback_pointer){
                            ptr = *reinterpret_cast<QFunctionPointer*>(plink->pointer());
                            resolved = true;
                        }
                    }
                }
                if(!resolved){
                    jobject ih = Java::QtCore::QFunctionPointerUtil::createCallbackProxy(__jni_env, Java::JNA::CallbackProxy::getClass(__jni_env), argClassType, val);
                    jobject callbackProxy = Java::QtCore::QFunctionPointerUtil$CppToJavaInvocationHandler::proxy(__jni_env, ih);
                    jobject method = __jni_env->ToReflectedMethod(Java::JNA::CallbackProxy::getClass(__jni_env), Java::JNA::CallbackProxy::callbackMethod_ID(__jni_env), false);
                    jlong peer = Java::JNA::Native::createNativeCallback(__jni_env,
                                                                  callbackProxy,
                                                                  method,
                                                                  Java::JNA::CallbackProxy::getParameterTypes(__jni_env, callbackProxy),
                                                                  Java::JNA::CallbackProxy::getReturnType(__jni_env, callbackProxy),
                                                                  0,
                                                                  0,
                                                                  __jni_env->NewStringUTF("UTF-8"));
                    ptr = *reinterpret_cast<QFunctionPointer*>(peer);
                    QtJambiLink::createLinkForNativeObject(
                            __jni_env,
                            val,
                            new QFunctionPointer(ptr),
                            LINK_NAME_ARG(nullptr)
                            false,
                            false,
                            &delete_callback_pointer,
                            QtJambiLink::Ownership::Java
                        );
                    {
                        QWriteLocker locker(gPointerLock());
                        (*gObjectsByFunctionPointer())[quintptr(ptr)] << JObjectWrapper(__jni_env, val);
                    }
                    Java::QtCore::QFunctionPointerUtil$CppToJavaInvocationHandler::set_peer(__jni_env, ih, peer);
                    Java::QtCore::QFunctionPointerUtil::registerCleanup(__jni_env, val, ih);
                }
            }
            arg = Java::JNA::Pointer::newInstance(__jni_env, ptr);
            resolved = true;
        }
        if(!resolved){
            if(Java::Runtime::String::isInstanceOf(__jni_env, val)){
                if(argMetaType){
                    if(argMetaType->id()==QMetaType::Char && argPointerOrReference==-2){
                        const char* chars = __jni_env->GetStringUTFChars(jstring(val), nullptr);
                        arg = Java::JNA::Pointer::newInstance(__jni_env, jlong(chars));
                        resolved = true;
                        cleaners.append(QSharedDataPointer<Cleanup>{new Cleanup{[val, chars, __jni_env](){ __jni_env->ReleaseStringUTFChars(jstring(val), chars); }}});
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
                    }else if(argMetaType->id()==QMetaType::Char16 && argPointerOrReference==-2){
                        const jchar* chars = __jni_env->GetStringChars(jstring(val), nullptr);
                        arg = Java::JNA::Pointer::newInstance(__jni_env, jlong(chars));
                        resolved = true;
                        cleaners.append(QSharedDataPointer<Cleanup>{new Cleanup{[val, chars, __jni_env](){ __jni_env->ReleaseStringChars(jstring(val), chars); }}});
#endif
                    }else if(argMetaType->id()==QMetaType::QByteArray){
                        const char* chars = __jni_env->GetStringUTFChars(jstring(val), nullptr);
                        QByteArray* byteArray = new QByteArray(chars);
                        __jni_env->ReleaseStringUTFChars(jstring(val), chars);
                        void* ptr = byteArray;
                        if(argPointerOrReference==0){
                            arg = __jni_env->NewDirectByteBuffer(ptr, jsize(sizeof(QByteArray)));
                            cleaners.append(QSharedDataPointer<Cleanup>{new Cleanup{[ptr](){ operator delete (ptr); }}});
                        }else{
                            arg = Java::JNA::Pointer::newInstance(__jni_env, jlong(ptr));
                            cleaners.append(QSharedDataPointer<Cleanup>{new Cleanup{[byteArray](){ delete byteArray; }}});
                        }
                    }else if(*argMetaType==stringViewType){
                        const jchar* chars = __jni_env->GetStringChars(jstring(val), nullptr);
                        resolved = true;
                        QStringView* strg = new QStringView(chars);
                        void* ptr = strg;
                        if(argPointerOrReference==0){
                            arg = __jni_env->NewDirectByteBuffer(ptr, jsize(sizeof(QStringView)));
                            cleaners.append(QSharedDataPointer<Cleanup>{new Cleanup{[ptr, val, chars, __jni_env](){ operator delete (ptr); __jni_env->ReleaseStringChars(jstring(val), chars); }}});
                        }else{
                            arg = Java::JNA::Pointer::newInstance(__jni_env, jlong(ptr));
                            cleaners.append(QSharedDataPointer<Cleanup>{new Cleanup{[strg, val, chars, __jni_env](){ delete strg; __jni_env->ReleaseStringChars(jstring(val), chars); }}});
                        }
                    }
                }
                if(!resolved){
                    QString* string = new QString;
                    *string = qtjambi_cast<QString>(__jni_env, val);
                    void* ptr = string;
                    if(argPointerOrReference==0){
                        arg = __jni_env->NewDirectByteBuffer(ptr, jsize(sizeof(QString)));
                        cleaners.append(QSharedDataPointer<Cleanup>{new Cleanup{[ptr](){ operator delete (ptr); }}});
                    }else{
                        arg = Java::JNA::Pointer::newInstance(__jni_env, jlong(ptr));
                        cleaners.append(QSharedDataPointer<Cleanup>{new Cleanup{[string](){ delete string; }}});
                    }
                }
            }else if(Java::Runtime::Class::isArray(__jni_env, __jni_env->GetObjectClass(val))
                     && Java::Runtime::String::isSameClass(__jni_env, Java::Runtime::Class::getComponentType(__jni_env, __jni_env->GetObjectClass(val)))
                     && (!argMetaType || QByteArray("QString&")==argMetaType->name() || QByteArray("QString &")==argMetaType->name()
                                      || QByteArray("QString*")==argMetaType->name() || QByteArray("QString *")==argMetaType->name()
                                      || QByteArray("const char**")==argMetaType->name())){
                jsize length = __jni_env->GetArrayLength(jobjectArray(val));
                if(argMetaType && QByteArray("const char**")==argMetaType->name()){
                    const char** ptr = new const char*[size_t(length)];
                    cleaners.append(QSharedDataPointer<Cleanup>{new Cleanup{[ptr](){ delete[] ptr; }}});
                    for(jsize i = 0; i<length; ++i){
                        jstring strg = jstring(__jni_env->GetObjectArrayElement(jobjectArray(val), i));
                        const char* chars = __jni_env->GetStringUTFChars(strg, nullptr);
                        ptr[i] = chars;
                        cleaners.append(QSharedDataPointer<Cleanup>{new Cleanup{[strg, chars, __jni_env](){ __jni_env->ReleaseStringUTFChars(strg, chars); }}});
                    }
                    arg = Java::JNA::Pointer::newInstance(__jni_env, jlong(ptr));
                    resolved = true;
                }else if(argMetaType && (QByteArray("QString&")==argMetaType->name() || QByteArray("QString &")==argMetaType->name())){
                    if(length!=1){
                        JavaException::raiseIllegalArgumentException(__jni_env, QStringLiteral("String array of size 1 expected, given: %1").arg(length) QTJAMBI_STACKTRACEINFO );
                    }else{
                        jstring strg = jstring(__jni_env->GetObjectArrayElement(jobjectArray(val), 0));
                        if(!strg)
                            JavaException::raiseIllegalArgumentException(__jni_env, QStringLiteral("Argument %1: null not expected as String array entry.").arg(i+1) QTJAMBI_STACKTRACEINFO );
                        QString* string = new QString;
                        cleaners.append(QSharedDataPointer<Cleanup>{new Cleanup{[string](){ delete string; }}});
                        *string = qtjambi_cast<QString>(__jni_env, strg);
                        arg = Java::JNA::Pointer::newInstance(__jni_env, jlong(string));
                        resolved = true;
                        successActions.append([string, val](JNIEnv * env){
                            jstring strg = jstring(env->GetObjectArrayElement(jobjectArray(val), 0));
                            jstring newStrg = qtjambi_cast<jstring>(env, *string);
                            if(!Java::Runtime::Objects::equals(env, strg, newStrg)){
                                env->SetObjectArrayElement(jobjectArray(val), 0, newStrg);
                            }
                        });
                    }
                }else{
                    QString* strings = new QString[size_t(length)];
                    cleaners.append(QSharedDataPointer<Cleanup>{new Cleanup{[strings](){ delete[] strings; }}});
                    for(jsize i = 0; i<length; ++i){
                        jstring strg = jstring(__jni_env->GetObjectArrayElement(jobjectArray(val), i));
                        strings[i] = qtjambi_cast<QString>(__jni_env, strg);
                    }
                    arg = Java::JNA::Pointer::newInstance(__jni_env, jlong(strings));
                    resolved = true;
                    if(argPointerOrReference!=-2
                            || (argMetaType && (QByteArray("QString*")==argMetaType->name() || QByteArray("QString *")==argMetaType->name()))){
                        successActions.append([strings, length, val](JNIEnv * env){
                            for(jsize i = 0; i<length; ++i){
                                jstring strg = jstring(env->GetObjectArrayElement(jobjectArray(val), i));
                                jstring newStrg = qtjambi_cast<jstring>(env, strings[i]);
                                if(!Java::Runtime::Objects::equals(env, strg, newStrg)){
                                    env->SetObjectArrayElement(jobjectArray(val), i, newStrg);
                                }
                            }
                        });
                    }
                }
            }else if(argPointerOrReference==0 && !isReferenceMetaType && Java::QtJambi::QFlags::isInstanceOf(__jni_env, val)){
                arg = Java::Runtime::Integer::valueOf(__jni_env, Java::QtJambi::QFlags::value(__jni_env, val));
            }else if(argPointerOrReference==0 && !isReferenceMetaType && Java::QtJambi::QtEnumerator::isInstanceOf(__jni_env, val)){
                arg = Java::Runtime::Integer::valueOf(__jni_env, Java::QtJambi::QtEnumerator::value(__jni_env, val));
            }else if(argPointerOrReference==0 && !isReferenceMetaType && Java::QtJambi::QtByteEnumerator::isInstanceOf(__jni_env, val)){
                arg = Java::Runtime::Byte::valueOf(__jni_env, Java::QtJambi::QtByteEnumerator::value(__jni_env, val));
            }else if(argPointerOrReference==0 && !isReferenceMetaType && Java::QtJambi::QtShortEnumerator::isInstanceOf(__jni_env, val)){
                arg = Java::Runtime::Short::valueOf(__jni_env, Java::QtJambi::QtShortEnumerator::value(__jni_env, val));
            }else if(argPointerOrReference==0 && !isReferenceMetaType && Java::QtJambi::QtLongEnumerator::isInstanceOf(__jni_env, val)){
                arg = Java::Runtime::Long::valueOf(__jni_env, Java::QtJambi::QtLongEnumerator::value(__jni_env, val));
            }else if(argPointerOrReference!=0
                    || isReferenceMetaType
                    || (!Java::Runtime::Byte::isInstanceOf(__jni_env, val)
                     && !Java::Runtime::Short::isInstanceOf(__jni_env, val)
                     && !Java::Runtime::Integer::isInstanceOf(__jni_env, val)
                     && !Java::Runtime::Long::isInstanceOf(__jni_env, val)
                     && !Java::Runtime::Float::isInstanceOf(__jni_env, val)
                     && !Java::Runtime::Double::isInstanceOf(__jni_env, val)
                     && !Java::Runtime::Character::isInstanceOf(__jni_env, val)
                     && !Java::Runtime::Boolean::isInstanceOf(__jni_env, val)
                     && !Java::Runtime::Buffer::isInstanceOf(__jni_env, val)
                     && !Java::JNA::Pointer::isInstanceOf(__jni_env, val)
                     && !Java::JNA::Structure::isInstanceOf(__jni_env, val)
                     && !(argClassType
                          && Java::Runtime::Class::isArray(__jni_env, argClassType)
                          && Java::Runtime::Class::isPrimitive(__jni_env, Java::Runtime::Class::getComponentType(__jni_env, argClassType))))){
                const std::type_info* typeId = nullptr;
                size_t size = 0;
                if(argPointerOrReference==0
                         && !isReferenceMetaType
                         && Java::Runtime::Class::isArray(__jni_env, __jni_env->GetObjectClass(val))){
                    jsize length = __jni_env->GetArrayLength(jobjectArray(val));
                    jclass argClassType = Java::Runtime::Class::getComponentType(__jni_env, __jni_env->GetObjectClass(val));
                    const QMetaType* argMetaType = nullptr;
                    QScopedPointer<const QMetaType> _argMetaType;
                    for(jsize i = 0; i<length; ++i){
                        jobject entry = __jni_env->GetObjectArrayElement(jobjectArray(val), i);
                        argMetaType = qtjambi_cast<const QMetaType*>(__jni_env, Java::QtCore::QGenericArgument::metaType(__jni_env, entry));
                        if(!argMetaType->isValid()){
                            argClassType = __jni_env->GetObjectClass(entry);
                            argMetaType = nullptr;
                        }else{
                            argClassType = CoreAPI::getClassForMetaType(__jni_env, *argMetaType);
                            break;
                        }
                    }
                    if(!argMetaType)
                        _argMetaType.reset(argMetaType = new QMetaType(registerMetaType(__jni_env, argClassType, false, false)));
                    if(!argMetaType->isValid())
                        _argMetaType.reset(argMetaType = nullptr);

                    {
                        const SuperTypeInfos& infos = SuperTypeInfos::fromClass(__jni_env, argClassType);
                        if(infos.isEmpty()){
                            if(argMetaType){
                                typeId = getTypeByMetaType(*argMetaType);
                                size = size_t(argMetaType->sizeOf());
                            }else{
                                Java::QtJambi::QUnsuccessfulInvocationException::throwNew(__jni_env, QString("Type %1 not supported.").arg(QtJambiAPI::getClassName(__jni_env, argClassType).replace(QLatin1Char('$'), QLatin1Char('.'))) QTJAMBI_STACKTRACEINFO );
                            }
                        }else{
                            typeId = &infos.first().typeId();
                            size = infos.first().size();
                        }
                    }
                    resolved = false;
                    if(typeId){
                        bool isValueFunctional = false;
                        switch(getEntryType(*typeId)){
                        case EntryTypes::FunctionalTypeInfo:
                            isValueFunctional = true;
                            if(OptionalBool op = isRegisteredAsPointerType(*typeId)){
                                if(op.value()){
                                    void** array = length==0 ? nullptr : new void*[size_t(length)];
                                    cleaners.append(QSharedDataPointer<Cleanup>{new Cleanup{[array](){ delete[] array; }}});
                                    QVector<void*> control;
                                    for(jsize i = 0; i<length; ++i){
                                        jobject entry = __jni_env->GetObjectArrayElement(jobjectArray(val), i);
                                        void* ptr = nullptr;
                                        if(!entry){
                                            array[i] = nullptr;
                                        }else if(QtJambiAPI::convertJavaToNative(__jni_env, *typeId, entry, ptr)){
                                            array[i] = *reinterpret_cast<void**>(ptr);
                                        }else{
                                            Java::QtJambi::QUnsuccessfulInvocationException::throwNew(__jni_env, QString("Type %1 not supported.").arg(QtJambiAPI::getObjectClassName(__jni_env, entry).replace(QLatin1Char('$'), QLatin1Char('.'))) QTJAMBI_STACKTRACEINFO );
                                        }
                                        control << array[i];
                                    }
                                    arg = Java::JNA::Pointer::newInstance(__jni_env, jlong(array));
                                    successActions.append([array, control, length, val, typeId](JNIEnv * env){
                                        for(jsize i = 0; i<length; ++i){
                                            if(control[i] != array[i]){
                                                env->SetObjectArrayElement(jobjectArray(val), i, internal_convertNativeToJavaObject(env, array[i], *typeId, nullptr, NativeToJavaConversionMode::None));
                                            }
                                        }
                                    });
                                    resolved = true;
                                    break;
                                }
                            }
                            Q_FALLTHROUGH();
                        case EntryTypes::ObjectTypeInfo:
                        case EntryTypes::InterfaceTypeInfo:
                        case EntryTypes::QObjectTypeInfo:
                            if(!isValueFunctional && !resolved){
                                resolved = true;
                                QVector<void*> control;
                                void** array = length==0 ? nullptr : new void*[size_t(length)];
                                cleaners.append(QSharedDataPointer<Cleanup>{new Cleanup{[array](){ delete[] array; }}});
                                for(jsize i = 0; i<length; ++i){
                                    jobject entry = __jni_env->GetObjectArrayElement(jobjectArray(val), i);
                                    if(!entry){
                                        array[i] = nullptr;
                                    }else if(QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForJavaInterface(__jni_env, entry)){
                                        array[i] = link->pointer();
                                    }else if(!QtJambiAPI::convertJavaToNative(__jni_env, *typeId, entry, array+i)){
                                        Java::QtJambi::QUnsuccessfulInvocationException::throwNew(__jni_env, QString("Type %1 not supported.").arg(QtJambiAPI::getObjectClassName(__jni_env, entry).replace(QLatin1Char('$'), QLatin1Char('.'))) QTJAMBI_STACKTRACEINFO );
                                    }
                                    control << array[i];
                                }
                                arg = Java::JNA::Pointer::newInstance(__jni_env, jlong(array));
                                successActions.append([array, control, length, val, typeId](JNIEnv * env){
                                    for(jsize i = 0; i<length; ++i){
                                        if(control[i] != array[i]){
                                            env->SetObjectArrayElement(jobjectArray(val), i, internal_convertNativeToJavaObject(env, array[i], *typeId, nullptr, NativeToJavaConversionMode::None));
                                        }
                                    }
                                });
                                break;
                            }
                            Q_FALLTHROUGH();
                        case EntryTypes::InterfaceValueTypeInfo:
                        case EntryTypes::ValueTypeInfo:
                        case EntryTypes::FlagsTypeInfo:
                        case EntryTypes::SpecialTypeInfo:
                        case EntryTypes::StringTypeInfo:
                        case EntryTypes::PrimitiveTypeInfo:
                        {
                            resolved = true;
                            if(!argMetaType)
                                _argMetaType.reset(argMetaType = new QMetaType(registeredMetaTypeID(*typeId)));
                            if(!argMetaType->isValid())
                                _argMetaType.reset(argMetaType = nullptr);
                            if(!argMetaType){
                                Java::QtJambi::QUnsuccessfulInvocationException::throwNew(__jni_env, QString("Type %1 not supported.").arg(QtJambiAPI::getClassName(__jni_env, argClassType).replace(QLatin1Char('$'), QLatin1Char('.'))) QTJAMBI_STACKTRACEINFO );
                                return;
                            }
                            char* array = length==0 ? nullptr : new char[size_t(length) * size];
                            try{
                                for(jsize i = 0; i<length; ++i){
                                    jobject entry = __jni_env->GetObjectArrayElement(jobjectArray(val), i);
                                    void * ptr = array+size_t(i)*size;
                                    if(QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForJavaInterface(__jni_env, entry)){
                                        argMetaType->construct(ptr, link->pointer());
                                    }else{
                                        argMetaType->construct(ptr, nullptr);
                                        if(!QtJambiAPI::convertJavaToNative(__jni_env, *typeId, entry, ptr)){
                                            if(Java::Runtime::Map::isInstanceOf(__jni_env, entry) || Java::Runtime::Collection::isInstanceOf(__jni_env, entry)){
                                                QVariant variant = CoreAPI::convertCheckedObjectToQVariant(__jni_env, entry, *argMetaType);
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
                                                if(variant.metaType()==*argMetaType || variant.convert(*argMetaType)){
#else
                                                if(variant.userType()==argMetaType->id() || variant.convert(argMetaType->id())){
#endif
                                                    argMetaType->destruct(ptr);
                                                    argMetaType->construct(ptr, variant.data());
                                                }else{
                                                    Java::QtJambi::QUnsuccessfulInvocationException::throwNew(__jni_env, QString("Type %1 not supported.").arg(QtJambiAPI::getObjectClassName(__jni_env, entry).replace(QLatin1Char('$'), QLatin1Char('.'))) QTJAMBI_STACKTRACEINFO );
                                                }
                                            }else{
                                                Java::QtJambi::QUnsuccessfulInvocationException::throwNew(__jni_env, QString("Type %1 not supported.").arg(QtJambiAPI::getObjectClassName(__jni_env, entry).replace(QLatin1Char('$'), QLatin1Char('.'))) QTJAMBI_STACKTRACEINFO );
                                            }
                                        }
                                    }
                                    int metaId = argMetaType->id();
                                    cleaners.append(QSharedDataPointer<Cleanup>{new Cleanup{[metaId,ptr](){ QMetaType(metaId).destruct(ptr); }}});
                                }
                                cleaners.append(QSharedDataPointer<Cleanup>{new Cleanup{[array](){ delete[] array; }}});
                            }catch(...){
                                cleaners.append(QSharedDataPointer<Cleanup>{new Cleanup{[array](){ delete[] array; }}});
                                throw;
                            }
                            arg = Java::JNA::Pointer::newInstance(__jni_env, jlong(array));
                            int metaType = argMetaType->id();
                            successActions.append([array, length, val, typeId, size, metaType](JNIEnv * env){
                                for(jsize i = 0; i<length; ++i){
                                    jobject obj = env->GetObjectArrayElement(jobjectArray(val), i);
                                    if(QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForJavaInterface(env, obj)){
                                        if(link->pointer() && !link->isShell()){
                                            QMetaType argMetaType(metaType);
                                            argMetaType.destruct(link->pointer());
                                            argMetaType.construct(link->pointer(), array+size_t(i)*size);
                                            continue;
                                        }
                                    }
                                    jobject newObj = internal_convertNativeToJavaObject(env, array+size_t(i)*size, *typeId, nullptr, NativeToJavaConversionMode::MakeCopyOfValues);
                                    if(!Java::Runtime::Objects::equals(env, obj, newObj)){
                                        env->SetObjectArrayElement(jobjectArray(val), i, newObj);
                                    }
                                }
                            });
                        }
                            break;
                        default:
                            break;
                        }
                    }
                    if(!resolved){
                        if(argMetaType){
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
                            if(argMetaType->name().endsWith("*")
                                    || argMetaType->name().contains("(*)")
                                    || argMetaType->name().contains("(__cdecl*)")){
#else
                            if(argMetaType->flags() & QMetaType::IsPointer){
#endif
                                QVector<void*> control;
                                void** array = length==0 ? nullptr : new void*[size_t(length)];
                                cleaners.append(QSharedDataPointer<Cleanup>{new Cleanup{[array](){ delete[] array; }}});
                                for(jsize i = 0; i<length; ++i){
                                    jobject entry = __jni_env->GetObjectArrayElement(jobjectArray(val), i);
                                    if(!entry){
                                        array[i] = nullptr;
                                    }else if(QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForJavaInterface(__jni_env, entry)){
                                        array[i] = link->pointer();
                                    }else if(!typeId || !QtJambiAPI::convertJavaToNative(__jni_env, *typeId, entry, array+i)){
                                        Java::QtJambi::QUnsuccessfulInvocationException::throwNew(__jni_env, QString("Type %1 not supported.").arg(QtJambiAPI::getObjectClassName(__jni_env, entry).replace(QLatin1Char('$'), QLatin1Char('.'))) QTJAMBI_STACKTRACEINFO );
                                    }
                                    control << array[i];
                                }
                                arg = Java::JNA::Pointer::newInstance(__jni_env, jlong(array));
                                if(typeId){
                                    successActions.append([array, control, length, val, typeId](JNIEnv * env){
                                        for(jsize i = 0; i<length; ++i){
                                            if(control[i] != array[i]){
                                                env->SetObjectArrayElement(jobjectArray(val), i, internal_convertNativeToJavaObject(env, array[i], *typeId, nullptr, NativeToJavaConversionMode::None));
                                            }
                                        }
                                    });
                                }else{
                                    successActions.append([array, control, length, val, argClassType](JNIEnv * env){
                                        for(jsize i = 0; i<length; ++i){
                                            if(control[i] != array[i]){
                                                env->SetObjectArrayElement(jobjectArray(val), i, QtJambiAPI::convertNativeToJavaObjectAsWrapper(env, array[i], argClassType));
                                            }
                                        }
                                    });
                                }
                            }else{
                                resolved = true;
                                if(!argMetaType)
                                    _argMetaType.reset(argMetaType = new QMetaType(registeredMetaTypeID(*typeId)));
                                if(!argMetaType->isValid())
                                    _argMetaType.reset(argMetaType = nullptr);
                                if(!argMetaType){
                                    Java::QtJambi::QUnsuccessfulInvocationException::throwNew(__jni_env, QString("Type %1 not supported.").arg(QtJambiAPI::getClassName(__jni_env, argClassType).replace(QLatin1Char('$'), QLatin1Char('.'))) QTJAMBI_STACKTRACEINFO );
                                    return;
                                }
                                char* array = length==0 ? nullptr : new char[size_t(length) * size];
                                try{
                                    for(jsize i = 0; i<length; ++i){
                                        jobject entry = __jni_env->GetObjectArrayElement(jobjectArray(val), i);
                                        void * ptr = array+size_t(i)*size;
                                        if(QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForJavaInterface(__jni_env, entry)){
                                            argMetaType->construct(ptr, link->pointer());
                                        }else{
                                            argMetaType->construct(ptr, nullptr);
                                            if(!typeId || !QtJambiAPI::convertJavaToNative(__jni_env, *typeId, entry, ptr)){
                                                if(Java::Runtime::Map::isInstanceOf(__jni_env, entry) || Java::Runtime::Collection::isInstanceOf(__jni_env, entry)){
                                                    QVariant variant = CoreAPI::convertCheckedObjectToQVariant(__jni_env, entry, *argMetaType);
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
                                                    if(variant.metaType()==*argMetaType || variant.convert(*argMetaType)){
#else
                                                    if(variant.userType()==argMetaType->id() || variant.convert(argMetaType->id())){
#endif
                                                        argMetaType->destruct(ptr);
                                                        argMetaType->construct(ptr, variant.data());
                                                    }else{
                                                        Java::QtJambi::QUnsuccessfulInvocationException::throwNew(__jni_env, QString("Type %1 not supported.").arg(QtJambiAPI::getObjectClassName(__jni_env, entry).replace(QLatin1Char('$'), QLatin1Char('.'))) QTJAMBI_STACKTRACEINFO );
                                                    }
                                                }else{
                                                    Java::QtJambi::QUnsuccessfulInvocationException::throwNew(__jni_env, QString("Type %1 not supported.").arg(QtJambiAPI::getObjectClassName(__jni_env, entry).replace(QLatin1Char('$'), QLatin1Char('.'))) QTJAMBI_STACKTRACEINFO );
                                                }
                                            }
                                        }
                                        int metaId = argMetaType->id();
                                        cleaners.append(QSharedDataPointer<Cleanup>{new Cleanup{[metaId,ptr](){ QMetaType(metaId).destruct(ptr); }}});
                                    }
                                    cleaners.append(QSharedDataPointer<Cleanup>{new Cleanup{[array](){ delete[] array; }}});
                                }catch(...){
                                    cleaners.append(QSharedDataPointer<Cleanup>{new Cleanup{[array](){ delete[] array; }}});
                                    throw;
                                }
                                arg = Java::JNA::Pointer::newInstance(__jni_env, jlong(array));
                                int metaType = argMetaType->id();
                                if(typeId){
                                    successActions.append([array, length, val, typeId, size, metaType](JNIEnv * env){
                                        for(jsize i = 0; i<length; ++i){
                                            jobject obj = env->GetObjectArrayElement(jobjectArray(val), i);
                                            if(QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForJavaInterface(env, obj)){
                                                if(link->pointer() && !link->isShell()){
                                                    QMetaType argMetaType(metaType);
                                                    argMetaType.destruct(link->pointer());
                                                    argMetaType.construct(link->pointer(), array+size_t(i)*size);
                                                    continue;
                                                }
                                            }
                                            jobject newObj = internal_convertNativeToJavaObject(env, array+size_t(i)*size, *typeId, nullptr, NativeToJavaConversionMode::MakeCopyOfValues);
                                            if(!Java::Runtime::Objects::equals(env, obj, newObj)){
                                                env->SetObjectArrayElement(jobjectArray(val), i, newObj);
                                            }
                                        }
                                    });
                                }else{
                                    successActions.append([array, length, val, argClassType, size, metaType](JNIEnv * env){
                                        for(jsize i = 0; i<length; ++i){
                                            jobject obj = env->GetObjectArrayElement(jobjectArray(val), i);
                                            if(QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForJavaInterface(env, obj)){
                                                if(link->pointer() && !link->isShell()){
                                                    QMetaType argMetaType(metaType);
                                                    argMetaType.destruct(link->pointer());
                                                    argMetaType.construct(link->pointer(), array+size_t(i)*size);
                                                    continue;
                                                }
                                            }
                                            jobject newObj = QtJambiAPI::convertNativeToJavaObjectAsCopy(env, array+size_t(i)*size, argClassType);
                                            if(!Java::Runtime::Objects::equals(env, obj, newObj)){
                                                env->SetObjectArrayElement(jobjectArray(val), i, newObj);
                                            }
                                        }
                                    });
                                }
                            }
                        }else{
                            Java::QtJambi::QUnsuccessfulInvocationException::throwNew(__jni_env, QString("Type %1 not supported.").arg(QtJambiAPI::getClassName(__jni_env, argClassType).replace(QLatin1Char('$'), QLatin1Char('.'))) QTJAMBI_STACKTRACEINFO );
                        }
                    }
                    resolved = true;
                }else{
                    {
                        const SuperTypeInfos& infos = SuperTypeInfos::fromClass(__jni_env, argClassType);
                        if(infos.isEmpty()){
                            if(argMetaType){
                                typeId = getTypeByMetaType(*argMetaType);
                                size = size_t(argMetaType->sizeOf());
                            }else{
                                Java::QtJambi::QUnsuccessfulInvocationException::throwNew(__jni_env, QString("Type %1 not supported.").arg(QtJambiAPI::getClassName(__jni_env, argClassType).replace(QLatin1Char('$'), QLatin1Char('.'))) QTJAMBI_STACKTRACEINFO );
                            }
                        }else{
                            typeId = &infos.first().typeId();
                            size = infos.first().size();
                        }
                    }
                    void* ptr = nullptr;
                    if(typeId){
                        bool isValueFunctional = false;
                        switch(getEntryType(*typeId)){
                        case EntryTypes::FunctionalTypeInfo:
                            isValueFunctional = true;
                            if(OptionalBool op = isRegisteredAsPointerType(*typeId)){
                                if(op.value()){
                                    if(QtJambiAPI::convertJavaToNative(__jni_env, *typeId, val, &ptr)){
                                        arg = Java::JNA::Pointer::newInstance(__jni_env, *reinterpret_cast<void**>(ptr));
                                    }else{
                                        Java::QtJambi::QUnsuccessfulInvocationException::throwNew(__jni_env, QString("Type %1 not supported.").arg(QtJambiAPI::getClassName(__jni_env, argClassType).replace(QLatin1Char('$'), QLatin1Char('.'))) QTJAMBI_STACKTRACEINFO );
                                    }
                                    resolved = true;
                                    break;
                                }
                            }
                            Q_FALLTHROUGH();
                        case EntryTypes::ObjectTypeInfo:
                        case EntryTypes::InterfaceTypeInfo:
                        case EntryTypes::QObjectTypeInfo:
                            if(!isValueFunctional && !resolved){
                                if(QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForJavaInterface(__jni_env, val)){
                                    arg = Java::JNA::Pointer::newInstance(__jni_env, link->pointer());
                                }else if(QtJambiAPI::convertJavaToNative(__jni_env, *typeId, val, &ptr)){
                                    arg = Java::JNA::Pointer::newInstance(__jni_env, ptr);
                                }else{
                                    Java::QtJambi::QUnsuccessfulInvocationException::throwNew(__jni_env, QString("Type %1 not supported.").arg(QtJambiAPI::getClassName(__jni_env, argClassType).replace(QLatin1Char('$'), QLatin1Char('.'))) QTJAMBI_STACKTRACEINFO );
                                }
                                resolved = true;
                                break;
                            }
                            Q_FALLTHROUGH();
                        case EntryTypes::InterfaceValueTypeInfo:
                        case EntryTypes::ValueTypeInfo:
                        {
                            if(QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForJavaInterface(__jni_env, val)){
                                ptr = link->pointer();
                                if(!argMetaType){
                                    if(PointerToObjectLink* plink = dynamic_cast<PointerToObjectLink*>(link.get())){
                                        argMetaType = &plink->metaType();
                                        if(!argMetaType->isValid())
                                            argMetaType = nullptr;
                                    }
                                }
                                if(argPointerOrReference==0 && !isReferenceMetaType){
                                    if(!argMetaType)
                                        _argMetaType.reset(argMetaType = new QMetaType(registeredMetaTypeID(*typeId)));
                                    if(!argMetaType->isValid())
                                        _argMetaType.reset(argMetaType = nullptr);
                                    if(argMetaType){
                                        ptr = argMetaType->create(ptr);
                                        arg = __jni_env->NewDirectByteBuffer(ptr, jsize(size));
                                        cleaners.append(QSharedDataPointer<Cleanup>{new Cleanup{[ptr](){ operator delete (ptr); }}});
                                    }else{
                                        Java::QtJambi::QUnsuccessfulInvocationException::throwNew(__jni_env, QString("Type %1 not supported.").arg(QtJambiAPI::getClassName(__jni_env, argClassType).replace(QLatin1Char('$'), QLatin1Char('.'))) QTJAMBI_STACKTRACEINFO );
                                    }
                                }else{
                                    arg = Java::JNA::Pointer::newInstance(__jni_env, ptr);
                                }
                            }else{
                                if(!argMetaType)
                                    _argMetaType.reset(argMetaType = new QMetaType(registeredMetaTypeID(*typeId)));
                                if(!argMetaType->isValid())
                                    _argMetaType.reset(argMetaType = nullptr);
                                if(argMetaType){
                                    ptr = argMetaType->create(nullptr);
                                    if(!QtJambiAPI::convertJavaToNative(__jni_env, *typeId, val, ptr)){
                                        if(Java::Runtime::Map::isInstanceOf(__jni_env, val) || Java::Runtime::Collection::isInstanceOf(__jni_env, val)){
                                            QVariant variant = CoreAPI::convertCheckedObjectToQVariant(__jni_env, val, *argMetaType);
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
                                            if(variant.metaType()==*argMetaType || variant.convert(*argMetaType)){
#else
                                            if(variant.userType()==argMetaType->id() || variant.convert(argMetaType->id())){
#endif
                                                argMetaType->destruct(ptr);
                                                argMetaType->construct(ptr, variant.data());
                                            }else{
                                                argMetaType->destroy(ptr);
                                                Java::QtJambi::QUnsuccessfulInvocationException::throwNew(__jni_env, QString("Type %1 not supported.").arg(QtJambiAPI::getObjectClassName(__jni_env, val).replace(QLatin1Char('$'), QLatin1Char('.'))) QTJAMBI_STACKTRACEINFO );
                                                return;
                                            }
                                        }else{
                                            argMetaType->destroy(ptr);
                                            Java::QtJambi::QUnsuccessfulInvocationException::throwNew(__jni_env, QString("Type %1 not supported.").arg(QtJambiAPI::getObjectClassName(__jni_env, val).replace(QLatin1Char('$'), QLatin1Char('.'))) QTJAMBI_STACKTRACEINFO );
                                            return;
                                        }
                                    }
                                    if(argPointerOrReference==0 && !isReferenceMetaType){
                                        arg = __jni_env->NewDirectByteBuffer(ptr, jsize(size));
                                        cleaners.append(QSharedDataPointer<Cleanup>{new Cleanup{[ptr](){ operator delete (ptr); }}});
                                    }else{
                                        arg = Java::JNA::Pointer::newInstance(__jni_env, ptr);
                                        cleaners.append(QSharedDataPointer<Cleanup>{new Cleanup{[argMetaType, ptr](){ argMetaType->destroy(ptr); }}});
                                    }
                                }else{
                                    Java::QtJambi::QUnsuccessfulInvocationException::throwNew(__jni_env, QString("Type %1 not supported.").arg(QtJambiAPI::getObjectClassName(__jni_env, val).replace(QLatin1Char('$'), QLatin1Char('.'))) QTJAMBI_STACKTRACEINFO );
                                }
                            }
                            resolved = true;
                        }
                        break;
                        case EntryTypes::FlagsTypeInfo:
                        case EntryTypes::SpecialTypeInfo:
                        case EntryTypes::StringTypeInfo:
                        case EntryTypes::PrimitiveTypeInfo:
                            {
                                if(!argMetaType)
                                    _argMetaType.reset(argMetaType = new QMetaType(registeredMetaTypeID(*typeId)));
                                if(!argMetaType->isValid())
                                    _argMetaType.reset(argMetaType = nullptr);
                                if(argPointerOrReference==0 && !isReferenceMetaType){
                                    if(size>0){
                                        if(argMetaType)
                                            ptr = argMetaType->create(nullptr);
                                        else
                                            ptr = operator new(size);
                                        if(QtJambiAPI::convertJavaToNative(__jni_env, *typeId, val, ptr)){
                                            arg = __jni_env->NewDirectByteBuffer(ptr, jsize(size));
                                            cleaners.append(QSharedDataPointer<Cleanup>{new Cleanup{[ptr](){ operator delete (ptr); }}});
                                        }else{
                                            Java::QtJambi::QUnsuccessfulInvocationException::throwNew(__jni_env, QString("Type %1 not supported.").arg(QtJambiAPI::getClassName(__jni_env, argClassType).replace(QLatin1Char('$'), QLatin1Char('.'))) QTJAMBI_STACKTRACEINFO );
                                        }
                                    }else{
                                        Java::QtJambi::QUnsuccessfulInvocationException::throwNew(__jni_env, QString("Type %1 not supported.").arg(QtJambiAPI::getClassName(__jni_env, argClassType).replace(QLatin1Char('$'), QLatin1Char('.'))) QTJAMBI_STACKTRACEINFO );
                                    }
                                }else if(QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForJavaInterface(__jni_env, val)){
                                    arg = Java::JNA::Pointer::newInstance(__jni_env, link->pointer());
                                }else{
                                    if(size>0){
                                        if(argMetaType){
                                            ptr = argMetaType->create(nullptr);
                                            if(QtJambiAPI::convertJavaToNative(__jni_env, *typeId, val, ptr)){
                                                arg = Java::JNA::Pointer::newInstance(__jni_env, ptr);
                                                cleaners.append(QSharedDataPointer<Cleanup>{new Cleanup{[argMetaType, ptr](){ argMetaType->destroy(ptr); }}});
                                                if(argPointerOrReference!=-2 || isReferenceMetaType){
                                                    if(Java::QtJambi::QFlags::isInstanceOf(__jni_env, val)){
                                                        successActions.append([arg, val](JNIEnv* env){
                                                            if(int* flag = reinterpret_cast<int*>(Java::JNA::Pointer::peer(env, arg))){
                                                                Java::QtJambi::QFlags::setValue(env, val, *flag);
                                                            }
                                                        });
                                                    }
                                                }
                                            }else{
                                                Java::QtJambi::QUnsuccessfulInvocationException::throwNew(__jni_env, QString("Type %1 not supported.").arg(QtJambiAPI::getClassName(__jni_env, argClassType).replace(QLatin1Char('$'), QLatin1Char('.'))) QTJAMBI_STACKTRACEINFO );
                                            }
                                        }else{
                                            if(PtrDeleterFunction dlt = deleter(*typeId)){
                                                ptr = operator new(size);
                                                if(QtJambiAPI::convertJavaToNative(__jni_env, *typeId, val, ptr)){
                                                    arg = Java::JNA::Pointer::newInstance(__jni_env, ptr);
                                                    cleaners.append(QSharedDataPointer<Cleanup>{new Cleanup{[dlt, ptr](){ dlt(ptr,false); }}});
                                                }else{
                                                    Java::QtJambi::QUnsuccessfulInvocationException::throwNew(__jni_env, QString("Type %1 not supported.").arg(QtJambiAPI::getClassName(__jni_env, argClassType).replace(QLatin1Char('$'), QLatin1Char('.'))) QTJAMBI_STACKTRACEINFO );
                                                }
                                            }else{
                                                Java::QtJambi::QUnsuccessfulInvocationException::throwNew(__jni_env, QString("Type %1 not supported.").arg(QtJambiAPI::getClassName(__jni_env, argClassType).replace(QLatin1Char('$'), QLatin1Char('.'))) QTJAMBI_STACKTRACEINFO );
                                            }
                                        }
                                    }else{
                                        Java::QtJambi::QUnsuccessfulInvocationException::throwNew(__jni_env, QString("Type %1 not supported.").arg(QtJambiAPI::getClassName(__jni_env, argClassType).replace(QLatin1Char('$'), QLatin1Char('.'))) QTJAMBI_STACKTRACEINFO );
                                    }
                                }
                                resolved = true;
                            }
                            break;
                        default:
                            break;
                        }
                    }
                    if(!resolved){
                        if(argPointerOrReference!=0 || isReferenceMetaType){
                            if(QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForJavaInterface(__jni_env, val)){
                                arg = Java::JNA::Pointer::newInstance(__jni_env, link->pointer());
                            }else{
                                arg = Java::JNA::Pointer::newInstance(__jni_env, val);
                            }
                        }else if(argMetaType){
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
                            if(argMetaType->name().endsWith("*")
                                    || argMetaType->name().contains("(*)")
                                    || argMetaType->name().contains("(__cdecl*)")){
#else
                            if(argMetaType->flags() & QMetaType::IsPointer){
#endif
                                if(QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForJavaInterface(__jni_env, val)){
                                    arg = Java::JNA::Pointer::newInstance(__jni_env, link->pointer());
                                }else if(typeId && QtJambiAPI::convertJavaToNative(__jni_env, *typeId, arg, &ptr)){
                                    arg = Java::JNA::Pointer::newInstance(__jni_env, ptr);
                                }else{
                                    Java::QtJambi::QUnsuccessfulInvocationException::throwNew(__jni_env, QString("Type %1 not supported.").arg(QtJambiAPI::getClassName(__jni_env, argClassType).replace(QLatin1Char('$'), QLatin1Char('.'))) QTJAMBI_STACKTRACEINFO );
                                }
                            }else{
                                if(QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForJavaInterface(__jni_env, val)){
                                    ptr = link->pointer();
                                    if(argPointerOrReference==0 && !isReferenceMetaType){
                                        ptr = argMetaType->create(ptr);
                                        arg = __jni_env->NewDirectByteBuffer(ptr, jsize(size));
                                        cleaners.append(QSharedDataPointer<Cleanup>{new Cleanup{[ptr](){ operator delete (ptr); }}});
                                    }else{
                                        arg = Java::JNA::Pointer::newInstance(__jni_env, ptr);
                                    }
                                }else{
                                    ptr = argMetaType->create(nullptr);
                                    if(!typeId || !QtJambiAPI::convertJavaToNative(__jni_env, *typeId, val, ptr)){
                                        if(Java::Runtime::Map::isInstanceOf(__jni_env, val) || Java::Runtime::Collection::isInstanceOf(__jni_env, val)){
                                            QVariant variant = CoreAPI::convertCheckedObjectToQVariant(__jni_env, val, *argMetaType);
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
                                            if(variant.metaType()==*argMetaType || variant.convert(*argMetaType)){
#else
                                            if(variant.userType()==argMetaType->id() || variant.convert(argMetaType->id())){
#endif
                                                argMetaType->destruct(ptr);
                                                argMetaType->construct(ptr, variant.data());
                                            }else{
                                                argMetaType->destroy(ptr);
                                                Java::QtJambi::QUnsuccessfulInvocationException::throwNew(__jni_env, QString("Type %1 not supported.").arg(QtJambiAPI::getObjectClassName(__jni_env, val).replace(QLatin1Char('$'), QLatin1Char('.'))) QTJAMBI_STACKTRACEINFO );
                                                return;
                                            }
                                        }else{
                                            argMetaType->destroy(ptr);
                                            Java::QtJambi::QUnsuccessfulInvocationException::throwNew(__jni_env, QString("Type %1 not supported.").arg(QtJambiAPI::getObjectClassName(__jni_env, val).replace(QLatin1Char('$'), QLatin1Char('.'))) QTJAMBI_STACKTRACEINFO );
                                            return;
                                        }
                                    }
                                    if(argPointerOrReference==0 && !isReferenceMetaType){
                                        arg = __jni_env->NewDirectByteBuffer(ptr, jsize(size));
                                        cleaners.append(QSharedDataPointer<Cleanup>{new Cleanup{[ptr](){ operator delete (ptr); }}});
                                    }else{
                                        arg = Java::JNA::Pointer::newInstance(__jni_env, ptr);
                                        cleaners.append(QSharedDataPointer<Cleanup>{new Cleanup{[argMetaType, ptr](){ argMetaType->destroy(ptr); }}});
                                    }
                                }
                            }
                        }else{
                            if(QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForJavaInterface(__jni_env, val)){
                                if(PointerToObjectLink* plink = dynamic_cast<PointerToObjectLink*>(link.get())){
                                    argMetaType = &plink->metaType();
                                    if(argMetaType->isValid()){
                                        ptr = argMetaType->create(link->pointer());
                                        arg = __jni_env->NewDirectByteBuffer(ptr, jsize(size));
                                        cleaners.append(QSharedDataPointer<Cleanup>{new Cleanup{[ptr](){ operator delete (ptr); }}});
                                    }else{
                                        Java::QtJambi::QUnsuccessfulInvocationException::throwNew(__jni_env, QString("Type %1 not supported.").arg(QtJambiAPI::getClassName(__jni_env, argClassType).replace(QLatin1Char('$'), QLatin1Char('.'))) QTJAMBI_STACKTRACEINFO );
                                    }
                                }
                            }else{
                                Java::QtJambi::QUnsuccessfulInvocationException::throwNew(__jni_env, QString("Type %1 not supported.").arg(QtJambiAPI::getClassName(__jni_env, argClassType).replace(QLatin1Char('$'), QLatin1Char('.'))) QTJAMBI_STACKTRACEINFO );
                            }
                        }
                    }
                }// all others are allowed
            }
        }
        Q_UNUSED(resolved)
        __jni_env->SetObjectArrayElement(arguments, i, arg);
    }
}

#ifdef QTJAMBI_STACKTRACE
#define QTJAMBI_STACKTRACEINFO_DECL_USE_ENV(env) env QTJAMBI_STACKTRACEINFO
#else
#define QTJAMBI_STACKTRACEINFO_DECL_USE_ENV(env)
#endif

jobject CoreAPI::invokeFunctionPointer(JNIEnv * __jni_env, QFunctionPointer __qt_this, jobject returnType, jobjectArray arguments){
    jsize length = __jni_env->IsSameObject(arguments, nullptr) ? 0 : __jni_env->GetArrayLength(arguments);
    QMetaType* returnMetaType = nullptr;
    jint returnPointerOrReference = 0;
    jclass returnClassType = nullptr;
    const std::type_info* returnTypeId = nullptr;
    jclass jnaReturnType = nullptr;
    bool isFunctionPointer = false;
    bool isReferenceMetaType = false;
    if(Java::QtCore::QGenericArgumentType::isInstanceOf(__jni_env, returnType)){
        returnMetaType = qtjambi_cast<QMetaType*>(__jni_env, Java::QtCore::QGenericArgumentType::metaType(__jni_env, returnType));
        if(returnMetaType && !returnMetaType->isValid())
            returnMetaType = nullptr;
        if(returnMetaType)
            isReferenceMetaType = QByteArray(returnMetaType->name()).endsWith("&");
        returnPointerOrReference = Java::QtCore::QGenericArgumentType::pointerOrReference(__jni_env, returnType);
        returnClassType = Java::QtCore::QGenericArgumentType::classType(__jni_env, returnType);
    }else if(Java::Runtime::Class::isInstanceOf(__jni_env, returnType)){
        returnClassType = jclass(returnType);
    }else{
        returnType = nullptr;
    }
    bool returnValueResolved = false;
    if((!returnMetaType && (__jni_env->IsSameObject(returnClassType, nullptr)
            || __jni_env->IsSameObject(returnClassType, Java::Runtime::Void::getClass(__jni_env))
            || __jni_env->IsSameObject(returnClassType, Java::Runtime::Void::primitiveType(__jni_env))))
            || (returnMetaType && returnPointerOrReference==0 && !isReferenceMetaType && returnMetaType->id()==QMetaType::Void)){
        switch(length){
        case 0:
            (*__qt_this)();
            return nullptr;
        case 1:
            {
                jobject arg = __jni_env->GetObjectArrayElement(arguments, 0);
                if(Java::Runtime::Byte::isInstanceOf(__jni_env, arg)){
                    reinterpret_cast<void(*)(jbyte)>(*__qt_this)(qtjambi_cast<jbyte>(__jni_env, arg));
                    return nullptr;
                }else if(Java::Runtime::Short::isInstanceOf(__jni_env, arg)){
                    reinterpret_cast<void(*)(jshort)>(*__qt_this)(qtjambi_cast<jshort>(__jni_env, arg));
                    return nullptr;
                }else if(Java::Runtime::Integer::isInstanceOf(__jni_env, arg)){
                    reinterpret_cast<void(*)(jint)>(*__qt_this)(qtjambi_cast<jint>(__jni_env, arg));
                    return nullptr;
                }else if(Java::Runtime::Long::isInstanceOf(__jni_env, arg)){
                    reinterpret_cast<void(*)(jlong)>(*__qt_this)(qtjambi_cast<jlong>(__jni_env, arg));
                    return nullptr;
                }else if(Java::Runtime::Double::isInstanceOf(__jni_env, arg)){
                    reinterpret_cast<void(*)(double)>(*__qt_this)(qtjambi_cast<double>(__jni_env, arg));
                    return nullptr;
                }else if(Java::Runtime::Float::isInstanceOf(__jni_env, arg)){
                    reinterpret_cast<void(*)(float)>(*__qt_this)(qtjambi_cast<float>(__jni_env, arg));
                    return nullptr;
                }else if(Java::Runtime::Boolean::isInstanceOf(__jni_env, arg)){
                    reinterpret_cast<void(*)(bool)>(*__qt_this)(qtjambi_cast<bool>(__jni_env, arg));
                    return nullptr;
                }else{
                    const QMetaType* metaType = qtjambi_cast<const QMetaType*>(__jni_env, Java::QtCore::QGenericArgument::metaType(__jni_env, arg));
                    if(Java::QtCore::QGenericArgument::isInstanceOf(__jni_env, arg))
                        arg = Java::QtCore::QGenericArgument::value(__jni_env, arg);
                    if(metaType && AbstractContainerAccess::isPointerType(*metaType)){
                        QVariant variant = qtjambi_cast<QVariant>(__jni_env, arg);
                        if(variant.isValid() && variant.convert(
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
                                                                metaType->id()
#else
                                                                *metaType
#endif
                                                                )){
                            void*const* ptr = reinterpret_cast<void*const*>(variant.data());
                            reinterpret_cast<void(*)(const void*)>(*__qt_this)(*ptr);
                            return nullptr;
                        }
                    }
                }
            }
        }
        jnaReturnType = Java::Runtime::Void::primitiveType(__jni_env);
        returnValueResolved = true;
    }else if((!returnMetaType && (__jni_env->IsSameObject(returnClassType, Java::Runtime::Integer::getClass(__jni_env))
                                || __jni_env->IsSameObject(returnClassType, Java::Runtime::Integer::primitiveType(__jni_env))))
             || (returnMetaType && returnPointerOrReference==0 && !isReferenceMetaType && ( returnMetaType->id()==QMetaType::Int
                                                                    || returnMetaType->id()==QMetaType::UInt
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
                                                                    || returnMetaType->id()==QMetaType::Char32
#endif
                                                                    ))){
        switch(length){
        case 0:
            return QtJambiAPI::toJavaIntegerObject(__jni_env, reinterpret_cast<jint(*)()>(*__qt_this)());
        case 1:
            {
                jobject arg = __jni_env->GetObjectArrayElement(arguments, 0);
                if(Java::Runtime::Byte::isInstanceOf(__jni_env, arg)){
                    return QtJambiAPI::toJavaIntegerObject(__jni_env, reinterpret_cast<jint(*)(jbyte)>(*__qt_this)(qtjambi_cast<jbyte>(__jni_env, arg)));
                }else if(Java::Runtime::Short::isInstanceOf(__jni_env, arg)){
                    return QtJambiAPI::toJavaIntegerObject(__jni_env, reinterpret_cast<jint(*)(jshort)>(*__qt_this)(qtjambi_cast<jshort>(__jni_env, arg)));
                }else if(Java::Runtime::Integer::isInstanceOf(__jni_env, arg)){
                    return QtJambiAPI::toJavaIntegerObject(__jni_env, reinterpret_cast<jint(*)(jint)>(*__qt_this)(qtjambi_cast<jint>(__jni_env, arg)));
                }else if(Java::Runtime::Long::isInstanceOf(__jni_env, arg)){
                    return QtJambiAPI::toJavaIntegerObject(__jni_env, reinterpret_cast<jint(*)(jlong)>(*__qt_this)(qtjambi_cast<jlong>(__jni_env, arg)));
                }else if(Java::Runtime::Double::isInstanceOf(__jni_env, arg)){
                    return QtJambiAPI::toJavaIntegerObject(__jni_env, reinterpret_cast<jint(*)(double)>(*__qt_this)(qtjambi_cast<double>(__jni_env, arg)));
                }else if(Java::Runtime::Float::isInstanceOf(__jni_env, arg)){
                    return QtJambiAPI::toJavaIntegerObject(__jni_env, reinterpret_cast<jint(*)(float)>(*__qt_this)(qtjambi_cast<float>(__jni_env, arg)));
                }else if(Java::Runtime::Boolean::isInstanceOf(__jni_env, arg)){
                    return QtJambiAPI::toJavaIntegerObject(__jni_env, reinterpret_cast<jint(*)(bool)>(*__qt_this)(qtjambi_cast<bool>(__jni_env, arg)));
                }else{
                    const QMetaType* metaType = qtjambi_cast<const QMetaType*>(__jni_env, Java::QtCore::QGenericArgument::metaType(__jni_env, arg));
                    if(Java::QtCore::QGenericArgument::isInstanceOf(__jni_env, arg))
                        arg = Java::QtCore::QGenericArgument::value(__jni_env, arg);
                    if(metaType && AbstractContainerAccess::isPointerType(*metaType)){
                        QVariant variant = qtjambi_cast<QVariant>(__jni_env, arg);
                        if(variant.isValid() && variant.convert(
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
                                                                metaType->id()
#else
                                                                *metaType
#endif
                                                                )){
                            void*const* ptr = reinterpret_cast<void*const*>(variant.data());
                            return QtJambiAPI::toJavaIntegerObject(__jni_env, reinterpret_cast<jint(*)(const void*)>(*__qt_this)(*ptr));
                        }
                    }
                }
            }
        }
        jnaReturnType = Java::Runtime::Integer::primitiveType(__jni_env);
        returnValueResolved = true;
    }else if(!returnMetaType && Java::QtJambi::QtEnumerator::isAssignableFrom(__jni_env, returnClassType)){
        switch(length){
        case 0:
            return QtJambiAPI::convertEnumToJavaObject(__jni_env, reinterpret_cast<qint32(*)()>(*__qt_this)(), returnClassType);
        case 1:
            {
                jobject arg = __jni_env->GetObjectArrayElement(arguments, 0);
                if(Java::Runtime::Byte::isInstanceOf(__jni_env, arg)){
                    return QtJambiAPI::convertEnumToJavaObject(__jni_env, reinterpret_cast<qint32(*)(jbyte)>(*__qt_this)(qtjambi_cast<jbyte>(__jni_env, arg)), returnClassType);
                }else if(Java::Runtime::Short::isInstanceOf(__jni_env, arg)){
                    return QtJambiAPI::convertEnumToJavaObject(__jni_env, reinterpret_cast<qint32(*)(jshort)>(*__qt_this)(qtjambi_cast<jshort>(__jni_env, arg)), returnClassType);
                }else if(Java::Runtime::Integer::isInstanceOf(__jni_env, arg)){
                    return QtJambiAPI::convertEnumToJavaObject(__jni_env, reinterpret_cast<qint32(*)(jint)>(*__qt_this)(qtjambi_cast<jint>(__jni_env, arg)), returnClassType);
                }else if(Java::Runtime::Long::isInstanceOf(__jni_env, arg)){
                    return QtJambiAPI::convertEnumToJavaObject(__jni_env, reinterpret_cast<qint32(*)(jlong)>(*__qt_this)(qtjambi_cast<jlong>(__jni_env, arg)), returnClassType);
                }else if(Java::Runtime::Double::isInstanceOf(__jni_env, arg)){
                    return QtJambiAPI::convertEnumToJavaObject(__jni_env, reinterpret_cast<qint32(*)(double)>(*__qt_this)(qtjambi_cast<double>(__jni_env, arg)), returnClassType);
                }else if(Java::Runtime::Float::isInstanceOf(__jni_env, arg)){
                    return QtJambiAPI::convertEnumToJavaObject(__jni_env, reinterpret_cast<qint32(*)(float)>(*__qt_this)(qtjambi_cast<float>(__jni_env, arg)), returnClassType);
                }else if(Java::Runtime::Boolean::isInstanceOf(__jni_env, arg)){
                    return QtJambiAPI::convertEnumToJavaObject(__jni_env, reinterpret_cast<qint32(*)(bool)>(*__qt_this)(qtjambi_cast<bool>(__jni_env, arg)), returnClassType);
                }else{
                    const QMetaType* metaType = qtjambi_cast<const QMetaType*>(__jni_env, Java::QtCore::QGenericArgument::metaType(__jni_env, arg));
                    if(Java::QtCore::QGenericArgument::isInstanceOf(__jni_env, arg))
                        arg = Java::QtCore::QGenericArgument::value(__jni_env, arg);
                    if(metaType && AbstractContainerAccess::isPointerType(*metaType)){
                        QVariant variant = qtjambi_cast<QVariant>(__jni_env, arg);
                        if(variant.isValid() && variant.convert(
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
                                                                metaType->id()
#else
                                                                *metaType
#endif
                                                                )){
                            void*const* ptr = reinterpret_cast<void*const*>(variant.data());
                            return QtJambiAPI::convertEnumToJavaObject(__jni_env, reinterpret_cast<qint32(*)(const void*)>(*__qt_this)(*ptr), returnClassType);
                        }
                    }
                }
            }
        }
        jnaReturnType = Java::Runtime::Integer::primitiveType(__jni_env);
        returnValueResolved = true;
    }else if(!returnMetaType && Java::QtJambi::QtByteEnumerator::isAssignableFrom(__jni_env, returnClassType)){
        switch(length){
        case 0:
            return QtJambiAPI::convertEnumToJavaObject(__jni_env, reinterpret_cast<qint8(*)()>(*__qt_this)(), returnClassType);
        case 1:
            {
                jobject arg = __jni_env->GetObjectArrayElement(arguments, 0);
                if(Java::Runtime::Byte::isInstanceOf(__jni_env, arg)){
                    return QtJambiAPI::convertEnumToJavaObject(__jni_env, reinterpret_cast<qint8(*)(jbyte)>(*__qt_this)(qtjambi_cast<jbyte>(__jni_env, arg)), returnClassType);
                }else if(Java::Runtime::Short::isInstanceOf(__jni_env, arg)){
                    return QtJambiAPI::convertEnumToJavaObject(__jni_env, reinterpret_cast<qint8(*)(jshort)>(*__qt_this)(qtjambi_cast<jshort>(__jni_env, arg)), returnClassType);
                }else if(Java::Runtime::Integer::isInstanceOf(__jni_env, arg)){
                    return QtJambiAPI::convertEnumToJavaObject(__jni_env, reinterpret_cast<qint8(*)(jint)>(*__qt_this)(qtjambi_cast<jint>(__jni_env, arg)), returnClassType);
                }else if(Java::Runtime::Long::isInstanceOf(__jni_env, arg)){
                    return QtJambiAPI::convertEnumToJavaObject(__jni_env, reinterpret_cast<qint8(*)(jlong)>(*__qt_this)(qtjambi_cast<jlong>(__jni_env, arg)), returnClassType);
                }else if(Java::Runtime::Double::isInstanceOf(__jni_env, arg)){
                    return QtJambiAPI::convertEnumToJavaObject(__jni_env, reinterpret_cast<qint8(*)(double)>(*__qt_this)(qtjambi_cast<double>(__jni_env, arg)), returnClassType);
                }else if(Java::Runtime::Float::isInstanceOf(__jni_env, arg)){
                    return QtJambiAPI::convertEnumToJavaObject(__jni_env, reinterpret_cast<qint8(*)(float)>(*__qt_this)(qtjambi_cast<float>(__jni_env, arg)), returnClassType);
                }else if(Java::Runtime::Boolean::isInstanceOf(__jni_env, arg)){
                    return QtJambiAPI::convertEnumToJavaObject(__jni_env, reinterpret_cast<qint8(*)(bool)>(*__qt_this)(qtjambi_cast<bool>(__jni_env, arg)), returnClassType);
                }else{
                    const QMetaType* metaType = qtjambi_cast<const QMetaType*>(__jni_env, Java::QtCore::QGenericArgument::metaType(__jni_env, arg));
                    if(Java::QtCore::QGenericArgument::isInstanceOf(__jni_env, arg))
                        arg = Java::QtCore::QGenericArgument::value(__jni_env, arg);
                    if(metaType && AbstractContainerAccess::isPointerType(*metaType)){
                        QVariant variant = qtjambi_cast<QVariant>(__jni_env, arg);
                        if(variant.isValid() && variant.convert(
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
                                                                metaType->id()
#else
                                                                *metaType
#endif
                                                                )){
                            void*const* ptr = reinterpret_cast<void*const*>(variant.data());
                            return QtJambiAPI::convertEnumToJavaObject(__jni_env, reinterpret_cast<qint8(*)(const void*)>(*__qt_this)(*ptr), returnClassType);
                        }
                    }
                }
            }
        }
        jnaReturnType = Java::Runtime::Byte::primitiveType(__jni_env);
        returnValueResolved = true;
    }else if(!returnMetaType && Java::QtJambi::QtShortEnumerator::isAssignableFrom(__jni_env, returnClassType)){
        switch(length){
        case 0:
            return QtJambiAPI::convertEnumToJavaObject(__jni_env, reinterpret_cast<qint16(*)()>(*__qt_this)(), returnClassType);
        case 1:
            {
                jobject arg = __jni_env->GetObjectArrayElement(arguments, 0);
                if(Java::Runtime::Byte::isInstanceOf(__jni_env, arg)){
                    return QtJambiAPI::convertEnumToJavaObject(__jni_env, reinterpret_cast<qint16(*)(jbyte)>(*__qt_this)(qtjambi_cast<jbyte>(__jni_env, arg)), returnClassType);
                }else if(Java::Runtime::Short::isInstanceOf(__jni_env, arg)){
                    return QtJambiAPI::convertEnumToJavaObject(__jni_env, reinterpret_cast<qint16(*)(jshort)>(*__qt_this)(qtjambi_cast<jshort>(__jni_env, arg)), returnClassType);
                }else if(Java::Runtime::Integer::isInstanceOf(__jni_env, arg)){
                    return QtJambiAPI::convertEnumToJavaObject(__jni_env, reinterpret_cast<qint16(*)(jint)>(*__qt_this)(qtjambi_cast<jint>(__jni_env, arg)), returnClassType);
                }else if(Java::Runtime::Long::isInstanceOf(__jni_env, arg)){
                    return QtJambiAPI::convertEnumToJavaObject(__jni_env, reinterpret_cast<qint16(*)(jlong)>(*__qt_this)(qtjambi_cast<jlong>(__jni_env, arg)), returnClassType);
                }else if(Java::Runtime::Double::isInstanceOf(__jni_env, arg)){
                    return QtJambiAPI::convertEnumToJavaObject(__jni_env, reinterpret_cast<qint16(*)(double)>(*__qt_this)(qtjambi_cast<double>(__jni_env, arg)), returnClassType);
                }else if(Java::Runtime::Float::isInstanceOf(__jni_env, arg)){
                    return QtJambiAPI::convertEnumToJavaObject(__jni_env, reinterpret_cast<qint16(*)(float)>(*__qt_this)(qtjambi_cast<float>(__jni_env, arg)), returnClassType);
                }else if(Java::Runtime::Boolean::isInstanceOf(__jni_env, arg)){
                    return QtJambiAPI::convertEnumToJavaObject(__jni_env, reinterpret_cast<qint16(*)(bool)>(*__qt_this)(qtjambi_cast<bool>(__jni_env, arg)), returnClassType);
                }else{
                    const QMetaType* metaType = qtjambi_cast<const QMetaType*>(__jni_env, Java::QtCore::QGenericArgument::metaType(__jni_env, arg));
                    if(Java::QtCore::QGenericArgument::isInstanceOf(__jni_env, arg))
                        arg = Java::QtCore::QGenericArgument::value(__jni_env, arg);
                    if(metaType && AbstractContainerAccess::isPointerType(*metaType)){
                        QVariant variant = qtjambi_cast<QVariant>(__jni_env, arg);
                        if(variant.isValid() && variant.convert(
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
                                                                metaType->id()
#else
                                                                *metaType
#endif
                                                                )){
                            void*const* ptr = reinterpret_cast<void*const*>(variant.data());
                            return QtJambiAPI::convertEnumToJavaObject(__jni_env, reinterpret_cast<qint16(*)(const void*)>(*__qt_this)(*ptr), returnClassType);
                        }
                    }
                }
            }
        }
        jnaReturnType = Java::Runtime::Short::primitiveType(__jni_env);
        returnValueResolved = true;
    }else if(!returnMetaType && Java::QtJambi::QtLongEnumerator::isAssignableFrom(__jni_env, returnClassType)){
        switch(length){
        case 0:
            return QtJambiAPI::convertEnumToJavaObject(__jni_env, reinterpret_cast<qint64(*)()>(*__qt_this)(), returnClassType);
        case 1:
            {
                jobject arg = __jni_env->GetObjectArrayElement(arguments, 0);
                if(Java::Runtime::Byte::isInstanceOf(__jni_env, arg)){
                    return QtJambiAPI::convertEnumToJavaObject(__jni_env, reinterpret_cast<qint64(*)(jbyte)>(*__qt_this)(qtjambi_cast<jbyte>(__jni_env, arg)), returnClassType);
                }else if(Java::Runtime::Short::isInstanceOf(__jni_env, arg)){
                    return QtJambiAPI::convertEnumToJavaObject(__jni_env, reinterpret_cast<qint64(*)(jshort)>(*__qt_this)(qtjambi_cast<jshort>(__jni_env, arg)), returnClassType);
                }else if(Java::Runtime::Integer::isInstanceOf(__jni_env, arg)){
                    return QtJambiAPI::convertEnumToJavaObject(__jni_env, reinterpret_cast<qint64(*)(jint)>(*__qt_this)(qtjambi_cast<jint>(__jni_env, arg)), returnClassType);
                }else if(Java::Runtime::Long::isInstanceOf(__jni_env, arg)){
                    return QtJambiAPI::convertEnumToJavaObject(__jni_env, reinterpret_cast<qint64(*)(jlong)>(*__qt_this)(qtjambi_cast<jlong>(__jni_env, arg)), returnClassType);
                }else if(Java::Runtime::Double::isInstanceOf(__jni_env, arg)){
                    return QtJambiAPI::convertEnumToJavaObject(__jni_env, reinterpret_cast<qint64(*)(double)>(*__qt_this)(qtjambi_cast<double>(__jni_env, arg)), returnClassType);
                }else if(Java::Runtime::Float::isInstanceOf(__jni_env, arg)){
                    return QtJambiAPI::convertEnumToJavaObject(__jni_env, reinterpret_cast<qint64(*)(float)>(*__qt_this)(qtjambi_cast<float>(__jni_env, arg)), returnClassType);
                }else if(Java::Runtime::Boolean::isInstanceOf(__jni_env, arg)){
                    return QtJambiAPI::convertEnumToJavaObject(__jni_env, reinterpret_cast<qint64(*)(bool)>(*__qt_this)(qtjambi_cast<bool>(__jni_env, arg)), returnClassType);
                }else{
                    const QMetaType* metaType = qtjambi_cast<const QMetaType*>(__jni_env, Java::QtCore::QGenericArgument::metaType(__jni_env, arg));
                    if(Java::QtCore::QGenericArgument::isInstanceOf(__jni_env, arg))
                        arg = Java::QtCore::QGenericArgument::value(__jni_env, arg);
                    if(metaType && AbstractContainerAccess::isPointerType(*metaType)){
                        QVariant variant = qtjambi_cast<QVariant>(__jni_env, arg);
                        if(variant.isValid() && variant.convert(
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
                                                                metaType->id()
#else
                                                                *metaType
#endif
                                                                )){
                            void*const* ptr = reinterpret_cast<void*const*>(variant.data());
                            return QtJambiAPI::convertEnumToJavaObject(__jni_env, reinterpret_cast<qint64(*)(const void*)>(*__qt_this)(*ptr), returnClassType);
                        }
                    }
                }
            }
        }
        jnaReturnType = Java::Runtime::Long::primitiveType(__jni_env);
        returnValueResolved = true;
    }else if((!returnMetaType && (__jni_env->IsSameObject(returnClassType, Java::Runtime::Byte::getClass(__jni_env))
                                    || __jni_env->IsSameObject(returnClassType, Java::Runtime::Byte::primitiveType(__jni_env))))
             || (returnMetaType && returnPointerOrReference==0 && !isReferenceMetaType && ( returnMetaType->id()==QMetaType::Char
                                                                    || returnMetaType->id()==QMetaType::UChar
                                                                    || returnMetaType->id()==QMetaType::SChar ))){
        switch(length){
        case 0:
            return QtJambiAPI::toJavaByteObject(__jni_env, reinterpret_cast<jbyte(*)()>(*__qt_this)());
        case 1:
            {
                jobject arg = __jni_env->GetObjectArrayElement(arguments, 0);
                if(Java::Runtime::Byte::isInstanceOf(__jni_env, arg)){
                    return QtJambiAPI::toJavaByteObject(__jni_env, reinterpret_cast<jbyte(*)(jbyte)>(*__qt_this)(qtjambi_cast<jbyte>(__jni_env, arg)));
                }else if(Java::Runtime::Short::isInstanceOf(__jni_env, arg)){
                    return QtJambiAPI::toJavaByteObject(__jni_env, reinterpret_cast<jbyte(*)(jshort)>(*__qt_this)(qtjambi_cast<jshort>(__jni_env, arg)));
                }else if(Java::Runtime::Integer::isInstanceOf(__jni_env, arg)){
                    return QtJambiAPI::toJavaByteObject(__jni_env, reinterpret_cast<jbyte(*)(jint)>(*__qt_this)(qtjambi_cast<jint>(__jni_env, arg)));
                }else if(Java::Runtime::Long::isInstanceOf(__jni_env, arg)){
                    return QtJambiAPI::toJavaByteObject(__jni_env, reinterpret_cast<jbyte(*)(jlong)>(*__qt_this)(qtjambi_cast<jlong>(__jni_env, arg)));
                }else if(Java::Runtime::Double::isInstanceOf(__jni_env, arg)){
                    return QtJambiAPI::toJavaByteObject(__jni_env, reinterpret_cast<jbyte(*)(double)>(*__qt_this)(qtjambi_cast<double>(__jni_env, arg)));
                }else if(Java::Runtime::Float::isInstanceOf(__jni_env, arg)){
                    return QtJambiAPI::toJavaByteObject(__jni_env, reinterpret_cast<jbyte(*)(float)>(*__qt_this)(qtjambi_cast<float>(__jni_env, arg)));
                }else if(Java::Runtime::Boolean::isInstanceOf(__jni_env, arg)){
                    return QtJambiAPI::toJavaByteObject(__jni_env, reinterpret_cast<jbyte(*)(bool)>(*__qt_this)(qtjambi_cast<bool>(__jni_env, arg)));
                }else{
                    const QMetaType* metaType = qtjambi_cast<const QMetaType*>(__jni_env, Java::QtCore::QGenericArgument::metaType(__jni_env, arg));
                    if(Java::QtCore::QGenericArgument::isInstanceOf(__jni_env, arg))
                        arg = Java::QtCore::QGenericArgument::value(__jni_env, arg);
                    if(metaType && AbstractContainerAccess::isPointerType(*metaType)){
                        QVariant variant = qtjambi_cast<QVariant>(__jni_env, arg);
                        if(variant.isValid() && variant.convert(
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
                                                                metaType->id()
#else
                                                                *metaType
#endif
                                                                )){
                            void*const* ptr = reinterpret_cast<void*const*>(variant.data());
                            return QtJambiAPI::toJavaByteObject(__jni_env, reinterpret_cast<jbyte(*)(const void*)>(*__qt_this)(*ptr));
                        }
                    }
                }
            }
        }
        jnaReturnType = Java::Runtime::Byte::primitiveType(__jni_env);
        returnValueResolved = true;
    }else if((!returnMetaType && (__jni_env->IsSameObject(returnClassType, Java::Runtime::Short::getClass(__jni_env))
                                    || __jni_env->IsSameObject(returnClassType, Java::Runtime::Short::primitiveType(__jni_env))))
             || (returnMetaType && returnPointerOrReference==0 && !isReferenceMetaType && ( returnMetaType->id()==QMetaType::Short
                                                                    || returnMetaType->id()==QMetaType::UShort))){
        switch(length){
        case 0:
            return QtJambiAPI::toJavaShortObject(__jni_env, reinterpret_cast<jshort(*)()>(*__qt_this)());
        case 1:
            {
                jobject arg = __jni_env->GetObjectArrayElement(arguments, 0);
                if(Java::Runtime::Byte::isInstanceOf(__jni_env, arg)){
                    return QtJambiAPI::toJavaShortObject(__jni_env, reinterpret_cast<jshort(*)(jbyte)>(*__qt_this)(qtjambi_cast<jbyte>(__jni_env, arg)));
                }else if(Java::Runtime::Short::isInstanceOf(__jni_env, arg)){
                    return QtJambiAPI::toJavaShortObject(__jni_env, reinterpret_cast<jshort(*)(jshort)>(*__qt_this)(qtjambi_cast<jshort>(__jni_env, arg)));
                }else if(Java::Runtime::Integer::isInstanceOf(__jni_env, arg)){
                    return QtJambiAPI::toJavaShortObject(__jni_env, reinterpret_cast<jshort(*)(jint)>(*__qt_this)(qtjambi_cast<jint>(__jni_env, arg)));
                }else if(Java::Runtime::Long::isInstanceOf(__jni_env, arg)){
                    return QtJambiAPI::toJavaShortObject(__jni_env, reinterpret_cast<jshort(*)(jlong)>(*__qt_this)(qtjambi_cast<jlong>(__jni_env, arg)));
                }else if(Java::Runtime::Double::isInstanceOf(__jni_env, arg)){
                    return QtJambiAPI::toJavaShortObject(__jni_env, reinterpret_cast<jshort(*)(double)>(*__qt_this)(qtjambi_cast<double>(__jni_env, arg)));
                }else if(Java::Runtime::Float::isInstanceOf(__jni_env, arg)){
                    return QtJambiAPI::toJavaShortObject(__jni_env, reinterpret_cast<jshort(*)(float)>(*__qt_this)(qtjambi_cast<float>(__jni_env, arg)));
                }else if(Java::Runtime::Boolean::isInstanceOf(__jni_env, arg)){
                    return QtJambiAPI::toJavaShortObject(__jni_env, reinterpret_cast<jshort(*)(bool)>(*__qt_this)(qtjambi_cast<bool>(__jni_env, arg)));
                }else{
                    const QMetaType* metaType = qtjambi_cast<const QMetaType*>(__jni_env, Java::QtCore::QGenericArgument::metaType(__jni_env, arg));
                    if(Java::QtCore::QGenericArgument::isInstanceOf(__jni_env, arg))
                        arg = Java::QtCore::QGenericArgument::value(__jni_env, arg);
                    if(metaType && AbstractContainerAccess::isPointerType(*metaType)){
                        QVariant variant = qtjambi_cast<QVariant>(__jni_env, arg);
                        if(variant.isValid() && variant.convert(
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
                                                                metaType->id()
#else
                                                                *metaType
#endif
                                                                )){
                            void*const* ptr = reinterpret_cast<void*const*>(variant.data());
                            return QtJambiAPI::toJavaShortObject(__jni_env, reinterpret_cast<jshort(*)(const void*)>(*__qt_this)(*ptr));
                        }
                    }
                }
            }
        }
        jnaReturnType = Java::Runtime::Short::primitiveType(__jni_env);
        returnValueResolved = true;
    }else if((!returnMetaType && (__jni_env->IsSameObject(returnClassType, Java::Runtime::Long::getClass(__jni_env))
                                     || __jni_env->IsSameObject(returnClassType, Java::Runtime::Long::primitiveType(__jni_env))))
             || (returnMetaType && returnPointerOrReference==0 && !isReferenceMetaType && ( returnMetaType->id()==QMetaType::LongLong
                                                                    || returnMetaType->id()==QMetaType::ULongLong))){
        switch(length){
        case 0:
            return QtJambiAPI::toJavaLongObject(__jni_env, reinterpret_cast<jlong(*)()>(*__qt_this)());
        case 1:
            {
                jobject arg = __jni_env->GetObjectArrayElement(arguments, 0);
                if(Java::Runtime::Byte::isInstanceOf(__jni_env, arg)){
                    return QtJambiAPI::toJavaLongObject(__jni_env, reinterpret_cast<jlong(*)(jbyte)>(*__qt_this)(qtjambi_cast<jbyte>(__jni_env, arg)));
                }else if(Java::Runtime::Short::isInstanceOf(__jni_env, arg)){
                    return QtJambiAPI::toJavaLongObject(__jni_env, reinterpret_cast<jlong(*)(jshort)>(*__qt_this)(qtjambi_cast<jshort>(__jni_env, arg)));
                }else if(Java::Runtime::Integer::isInstanceOf(__jni_env, arg)){
                    return QtJambiAPI::toJavaLongObject(__jni_env, reinterpret_cast<jlong(*)(jint)>(*__qt_this)(qtjambi_cast<jint>(__jni_env, arg)));
                }else if(Java::Runtime::Long::isInstanceOf(__jni_env, arg)){
                    return QtJambiAPI::toJavaLongObject(__jni_env, reinterpret_cast<jlong(*)(jlong)>(*__qt_this)(qtjambi_cast<jlong>(__jni_env, arg)));
                }else if(Java::Runtime::Double::isInstanceOf(__jni_env, arg)){
                    return QtJambiAPI::toJavaLongObject(__jni_env, reinterpret_cast<jlong(*)(double)>(*__qt_this)(qtjambi_cast<double>(__jni_env, arg)));
                }else if(Java::Runtime::Float::isInstanceOf(__jni_env, arg)){
                    return QtJambiAPI::toJavaLongObject(__jni_env, reinterpret_cast<jlong(*)(float)>(*__qt_this)(qtjambi_cast<float>(__jni_env, arg)));
                }else if(Java::Runtime::Boolean::isInstanceOf(__jni_env, arg)){
                    return QtJambiAPI::toJavaLongObject(__jni_env, reinterpret_cast<jlong(*)(bool)>(*__qt_this)(qtjambi_cast<bool>(__jni_env, arg)));
                }else{
                    const QMetaType* metaType = qtjambi_cast<const QMetaType*>(__jni_env, Java::QtCore::QGenericArgument::metaType(__jni_env, arg));
                    if(Java::QtCore::QGenericArgument::isInstanceOf(__jni_env, arg))
                        arg = Java::QtCore::QGenericArgument::value(__jni_env, arg);
                    if(metaType && AbstractContainerAccess::isPointerType(*metaType)){
                        QVariant variant = qtjambi_cast<QVariant>(__jni_env, arg);
                        if(variant.isValid() && variant.convert(
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
                                                                metaType->id()
#else
                                                                *metaType
#endif
                                                                )){
                            void*const* ptr = reinterpret_cast<void*const*>(variant.data());
                            return QtJambiAPI::toJavaLongObject(__jni_env, reinterpret_cast<jlong(*)(const void*)>(*__qt_this)(*ptr));
                        }
                    }
                }
            }
        }
        jnaReturnType = Java::Runtime::Long::primitiveType(__jni_env);
        returnValueResolved = true;
    }else if((!returnMetaType && (__jni_env->IsSameObject(returnClassType, Java::Runtime::Float::getClass(__jni_env))
                                    || __jni_env->IsSameObject(returnClassType, Java::Runtime::Float::primitiveType(__jni_env))))
             || (returnMetaType && returnPointerOrReference==0 && !isReferenceMetaType && returnMetaType->id()==QMetaType::Float)){
        switch(length){
        case 0:
            return QtJambiAPI::toJavaFloatObject(__jni_env, reinterpret_cast<float(*)()>(*__qt_this)());
        case 1:
            {
                jobject arg = __jni_env->GetObjectArrayElement(arguments, 0);
                if(Java::Runtime::Byte::isInstanceOf(__jni_env, arg)){
                    return QtJambiAPI::toJavaFloatObject(__jni_env, reinterpret_cast<float(*)(jbyte)>(*__qt_this)(qtjambi_cast<jbyte>(__jni_env, arg)));
                }else if(Java::Runtime::Short::isInstanceOf(__jni_env, arg)){
                    return QtJambiAPI::toJavaFloatObject(__jni_env, reinterpret_cast<float(*)(jshort)>(*__qt_this)(qtjambi_cast<jshort>(__jni_env, arg)));
                }else if(Java::Runtime::Integer::isInstanceOf(__jni_env, arg)){
                    return QtJambiAPI::toJavaFloatObject(__jni_env, reinterpret_cast<float(*)(jint)>(*__qt_this)(qtjambi_cast<jint>(__jni_env, arg)));
                }else if(Java::Runtime::Long::isInstanceOf(__jni_env, arg)){
                    return QtJambiAPI::toJavaFloatObject(__jni_env, reinterpret_cast<float(*)(jlong)>(*__qt_this)(qtjambi_cast<jlong>(__jni_env, arg)));
                }else if(Java::Runtime::Double::isInstanceOf(__jni_env, arg)){
                    return QtJambiAPI::toJavaFloatObject(__jni_env, reinterpret_cast<float(*)(double)>(*__qt_this)(qtjambi_cast<double>(__jni_env, arg)));
                }else if(Java::Runtime::Float::isInstanceOf(__jni_env, arg)){
                    return QtJambiAPI::toJavaFloatObject(__jni_env, reinterpret_cast<float(*)(float)>(*__qt_this)(qtjambi_cast<float>(__jni_env, arg)));
                }else if(Java::Runtime::Boolean::isInstanceOf(__jni_env, arg)){
                    return QtJambiAPI::toJavaFloatObject(__jni_env, reinterpret_cast<float(*)(bool)>(*__qt_this)(qtjambi_cast<bool>(__jni_env, arg)));
                }else{
                    const QMetaType* metaType = qtjambi_cast<const QMetaType*>(__jni_env, Java::QtCore::QGenericArgument::metaType(__jni_env, arg));
                    if(Java::QtCore::QGenericArgument::isInstanceOf(__jni_env, arg))
                        arg = Java::QtCore::QGenericArgument::value(__jni_env, arg);
                    if(metaType && AbstractContainerAccess::isPointerType(*metaType)){
                        QVariant variant = qtjambi_cast<QVariant>(__jni_env, arg);
                        if(variant.isValid() && variant.convert(
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
                                                                metaType->id()
#else
                                                                *metaType
#endif
                                                                )){
                            void*const* ptr = reinterpret_cast<void*const*>(variant.data());
                            return QtJambiAPI::toJavaFloatObject(__jni_env, reinterpret_cast<float(*)(const void*)>(*__qt_this)(*ptr));
                        }
                    }
                }
            }
        }
        jnaReturnType = Java::Runtime::Float::primitiveType(__jni_env);
        returnValueResolved = true;
    }else if((!returnMetaType && (__jni_env->IsSameObject(returnClassType, Java::Runtime::Double::getClass(__jni_env))
                                     || __jni_env->IsSameObject(returnClassType, Java::Runtime::Double::primitiveType(__jni_env))))
             || (returnMetaType && returnPointerOrReference==0 && !isReferenceMetaType && returnMetaType->id()==QMetaType::Double)){
        switch(length){
        case 0:
            return QtJambiAPI::toJavaDoubleObject(__jni_env, reinterpret_cast<double(*)()>(*__qt_this)());
        case 1:
            {
                jobject arg = __jni_env->GetObjectArrayElement(arguments, 0);
                if(Java::Runtime::Byte::isInstanceOf(__jni_env, arg)){
                    return QtJambiAPI::toJavaDoubleObject(__jni_env, reinterpret_cast<double(*)(jbyte)>(*__qt_this)(qtjambi_cast<jbyte>(__jni_env, arg)));
                }else if(Java::Runtime::Short::isInstanceOf(__jni_env, arg)){
                    return QtJambiAPI::toJavaDoubleObject(__jni_env, reinterpret_cast<double(*)(jshort)>(*__qt_this)(qtjambi_cast<jshort>(__jni_env, arg)));
                }else if(Java::Runtime::Integer::isInstanceOf(__jni_env, arg)){
                    return QtJambiAPI::toJavaDoubleObject(__jni_env, reinterpret_cast<double(*)(jint)>(*__qt_this)(qtjambi_cast<jint>(__jni_env, arg)));
                }else if(Java::Runtime::Long::isInstanceOf(__jni_env, arg)){
                    return QtJambiAPI::toJavaDoubleObject(__jni_env, reinterpret_cast<double(*)(jlong)>(*__qt_this)(qtjambi_cast<jlong>(__jni_env, arg)));
                }else if(Java::Runtime::Double::isInstanceOf(__jni_env, arg)){
                    return QtJambiAPI::toJavaDoubleObject(__jni_env, reinterpret_cast<double(*)(double)>(*__qt_this)(qtjambi_cast<double>(__jni_env, arg)));
                }else if(Java::Runtime::Float::isInstanceOf(__jni_env, arg)){
                    return QtJambiAPI::toJavaDoubleObject(__jni_env, reinterpret_cast<double(*)(float)>(*__qt_this)(qtjambi_cast<float>(__jni_env, arg)));
                }else if(Java::Runtime::Boolean::isInstanceOf(__jni_env, arg)){
                    return QtJambiAPI::toJavaDoubleObject(__jni_env, reinterpret_cast<double(*)(bool)>(*__qt_this)(qtjambi_cast<bool>(__jni_env, arg)));
                }else{
                    const QMetaType* metaType = qtjambi_cast<const QMetaType*>(__jni_env, Java::QtCore::QGenericArgument::metaType(__jni_env, arg));
                    if(Java::QtCore::QGenericArgument::isInstanceOf(__jni_env, arg))
                        arg = Java::QtCore::QGenericArgument::value(__jni_env, arg);
                    if(metaType && AbstractContainerAccess::isPointerType(*metaType)){
                        QVariant variant = qtjambi_cast<QVariant>(__jni_env, arg);
                        if(variant.isValid() && variant.convert(
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
                                                                metaType->id()
#else
                                                                *metaType
#endif
                                                                )){
                            void*const* ptr = reinterpret_cast<void*const*>(variant.data());
                            return QtJambiAPI::toJavaDoubleObject(__jni_env, reinterpret_cast<double(*)(const void*)>(*__qt_this)(*ptr));
                        }
                    }
                }
            }
        }
        jnaReturnType = Java::Runtime::Double::primitiveType(__jni_env);
        returnValueResolved = true;
    }else if((!returnMetaType && (__jni_env->IsSameObject(returnClassType, Java::Runtime::Boolean::getClass(__jni_env))
                                     || __jni_env->IsSameObject(returnClassType, Java::Runtime::Boolean::primitiveType(__jni_env))))
             || (returnMetaType && returnPointerOrReference==0 && !isReferenceMetaType && returnMetaType->id()==QMetaType::Bool)){
        switch(length){
        case 0:
            return QtJambiAPI::toJavaBooleanObject(__jni_env, reinterpret_cast<bool(*)()>(*__qt_this)());
        case 1:
            {
                jobject arg = __jni_env->GetObjectArrayElement(arguments, 0);
                if(Java::Runtime::Byte::isInstanceOf(__jni_env, arg)){
                    return QtJambiAPI::toJavaBooleanObject(__jni_env, reinterpret_cast<bool(*)(jbyte)>(*__qt_this)(qtjambi_cast<jbyte>(__jni_env, arg)));
                }else if(Java::Runtime::Short::isInstanceOf(__jni_env, arg)){
                    return QtJambiAPI::toJavaBooleanObject(__jni_env, reinterpret_cast<bool(*)(jshort)>(*__qt_this)(qtjambi_cast<jshort>(__jni_env, arg)));
                }else if(Java::Runtime::Integer::isInstanceOf(__jni_env, arg)){
                    return QtJambiAPI::toJavaBooleanObject(__jni_env, reinterpret_cast<bool(*)(jint)>(*__qt_this)(qtjambi_cast<jint>(__jni_env, arg)));
                }else if(Java::Runtime::Long::isInstanceOf(__jni_env, arg)){
                    return QtJambiAPI::toJavaBooleanObject(__jni_env, reinterpret_cast<bool(*)(jlong)>(*__qt_this)(qtjambi_cast<jlong>(__jni_env, arg)));
                }else if(Java::Runtime::Double::isInstanceOf(__jni_env, arg)){
                    return QtJambiAPI::toJavaBooleanObject(__jni_env, reinterpret_cast<bool(*)(double)>(*__qt_this)(qtjambi_cast<double>(__jni_env, arg)));
                }else if(Java::Runtime::Float::isInstanceOf(__jni_env, arg)){
                    return QtJambiAPI::toJavaBooleanObject(__jni_env, reinterpret_cast<bool(*)(float)>(*__qt_this)(qtjambi_cast<float>(__jni_env, arg)));
                }else if(Java::Runtime::Boolean::isInstanceOf(__jni_env, arg)){
                    return QtJambiAPI::toJavaBooleanObject(__jni_env, reinterpret_cast<bool(*)(bool)>(*__qt_this)(qtjambi_cast<bool>(__jni_env, arg)));
                }else{
                    const QMetaType* metaType = qtjambi_cast<const QMetaType*>(__jni_env, Java::QtCore::QGenericArgument::metaType(__jni_env, arg));
                    if(Java::QtCore::QGenericArgument::isInstanceOf(__jni_env, arg))
                        arg = Java::QtCore::QGenericArgument::value(__jni_env, arg);
                    if(metaType && AbstractContainerAccess::isPointerType(*metaType)){
                        QVariant variant = qtjambi_cast<QVariant>(__jni_env, arg);
                        if(variant.isValid() && variant.convert(
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
                                                                metaType->id()
#else
                                                                *metaType
#endif
                                                                )){
                            void*const* ptr = reinterpret_cast<void*const*>(variant.data());
                            return QtJambiAPI::toJavaBooleanObject(__jni_env, reinterpret_cast<bool(*)(const void*)>(*__qt_this)(*ptr));
                        }
                    }
                }
            }
        }
        jnaReturnType = Java::Runtime::Boolean::primitiveType(__jni_env);
        returnValueResolved = true;
    }else if((!returnMetaType && (__jni_env->IsSameObject(returnClassType, Java::Runtime::Character::getClass(__jni_env))
                                    || __jni_env->IsSameObject(returnClassType, Java::Runtime::Character::primitiveType(__jni_env))))
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
             || (returnMetaType && returnPointerOrReference==0 && !isReferenceMetaType && returnMetaType->id()==QMetaType::Char16)
#endif
             ){
        switch(length){
        case 0:
            return QtJambiAPI::toJavaCharacterObject(__jni_env, reinterpret_cast<jchar(*)()>(*__qt_this)());
        case 1:
            {
                jobject arg = __jni_env->GetObjectArrayElement(arguments, 0);
                if(Java::Runtime::Byte::isInstanceOf(__jni_env, arg)){
                    return QtJambiAPI::toJavaCharacterObject(__jni_env, reinterpret_cast<jchar(*)(jbyte)>(*__qt_this)(qtjambi_cast<jbyte>(__jni_env, arg)));
                }else if(Java::Runtime::Short::isInstanceOf(__jni_env, arg)){
                    return QtJambiAPI::toJavaCharacterObject(__jni_env, reinterpret_cast<jchar(*)(jshort)>(*__qt_this)(qtjambi_cast<jshort>(__jni_env, arg)));
                }else if(Java::Runtime::Integer::isInstanceOf(__jni_env, arg)){
                    return QtJambiAPI::toJavaCharacterObject(__jni_env, reinterpret_cast<jchar(*)(jint)>(*__qt_this)(qtjambi_cast<jint>(__jni_env, arg)));
                }else if(Java::Runtime::Long::isInstanceOf(__jni_env, arg)){
                    return QtJambiAPI::toJavaCharacterObject(__jni_env, reinterpret_cast<jchar(*)(jlong)>(*__qt_this)(qtjambi_cast<jlong>(__jni_env, arg)));
                }else if(Java::Runtime::Double::isInstanceOf(__jni_env, arg)){
                    return QtJambiAPI::toJavaCharacterObject(__jni_env, reinterpret_cast<jchar(*)(double)>(*__qt_this)(qtjambi_cast<double>(__jni_env, arg)));
                }else if(Java::Runtime::Float::isInstanceOf(__jni_env, arg)){
                    return QtJambiAPI::toJavaCharacterObject(__jni_env, reinterpret_cast<jchar(*)(float)>(*__qt_this)(qtjambi_cast<float>(__jni_env, arg)));
                }else if(Java::Runtime::Boolean::isInstanceOf(__jni_env, arg)){
                    return QtJambiAPI::toJavaCharacterObject(__jni_env, reinterpret_cast<jchar(*)(bool)>(*__qt_this)(qtjambi_cast<bool>(__jni_env, arg)));
                }else{
                    const QMetaType* metaType = qtjambi_cast<const QMetaType*>(__jni_env, Java::QtCore::QGenericArgument::metaType(__jni_env, arg));
                    if(Java::QtCore::QGenericArgument::isInstanceOf(__jni_env, arg))
                        arg = Java::QtCore::QGenericArgument::value(__jni_env, arg);
                    if(metaType && AbstractContainerAccess::isPointerType(*metaType)){
                        QVariant variant = qtjambi_cast<QVariant>(__jni_env, arg);
                        if(variant.isValid() && variant.convert(
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
                                                                metaType->id()
#else
                                                                *metaType
#endif
                                                                )){
                            void*const* ptr = reinterpret_cast<void*const*>(variant.data());
                            return QtJambiAPI::toJavaCharacterObject(__jni_env, reinterpret_cast<jchar(*)(const void*)>(*__qt_this)(*ptr));
                        }
                    }
                }
            }
        }
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
        if(returnMetaType && returnPointerOrReference==0 && !isReferenceMetaType && returnMetaType->id()==QMetaType::Char16){
            jnaReturnType = Java::Runtime::Character::primitiveType(__jni_env);
            returnValueResolved = true;
        }
#endif
    }else if((returnMetaType && AbstractContainerAccess::isPointerType(*returnMetaType))
             || AbstractContainerAccess::isPointerType(QMetaType(registerMetaType(__jni_env, returnClassType, false, false)))){
        switch(length){
        case 0:
            return QtJambiAPI::convertNativeToJavaObjectAsWrapper(__jni_env, reinterpret_cast<void*(*)()>(*__qt_this)(), returnClassType);
        case 1:
            {
                jobject arg = __jni_env->GetObjectArrayElement(arguments, 0);
                if(Java::Runtime::Byte::isInstanceOf(__jni_env, arg)){
                    return QtJambiAPI::convertNativeToJavaObjectAsWrapper(__jni_env, reinterpret_cast<void*(*)(jbyte)>(*__qt_this)(qtjambi_cast<jbyte>(__jni_env, arg)), returnClassType);
                }else if(Java::Runtime::Short::isInstanceOf(__jni_env, arg)){
                    return QtJambiAPI::convertNativeToJavaObjectAsWrapper(__jni_env, reinterpret_cast<void*(*)(jshort)>(*__qt_this)(qtjambi_cast<jshort>(__jni_env, arg)), returnClassType);
                }else if(Java::Runtime::Integer::isInstanceOf(__jni_env, arg)){
                    return QtJambiAPI::convertNativeToJavaObjectAsWrapper(__jni_env, reinterpret_cast<void*(*)(jint)>(*__qt_this)(qtjambi_cast<jint>(__jni_env, arg)), returnClassType);
                }else if(Java::Runtime::Long::isInstanceOf(__jni_env, arg)){
                    return QtJambiAPI::convertNativeToJavaObjectAsWrapper(__jni_env, reinterpret_cast<void*(*)(jlong)>(*__qt_this)(qtjambi_cast<jlong>(__jni_env, arg)), returnClassType);
                }else if(Java::Runtime::Double::isInstanceOf(__jni_env, arg)){
                    return QtJambiAPI::convertNativeToJavaObjectAsWrapper(__jni_env, reinterpret_cast<void*(*)(double)>(*__qt_this)(qtjambi_cast<double>(__jni_env, arg)), returnClassType);
                }else if(Java::Runtime::Float::isInstanceOf(__jni_env, arg)){
                    return QtJambiAPI::convertNativeToJavaObjectAsWrapper(__jni_env, reinterpret_cast<void*(*)(float)>(*__qt_this)(qtjambi_cast<float>(__jni_env, arg)), returnClassType);
                }else if(Java::Runtime::Boolean::isInstanceOf(__jni_env, arg)){
                    return QtJambiAPI::convertNativeToJavaObjectAsWrapper(__jni_env, reinterpret_cast<void*(*)(bool)>(*__qt_this)(qtjambi_cast<bool>(__jni_env, arg)), returnClassType);
                }else{
                    const QMetaType* metaType = qtjambi_cast<const QMetaType*>(__jni_env, Java::QtCore::QGenericArgument::metaType(__jni_env, arg));
                    if(Java::QtCore::QGenericArgument::isInstanceOf(__jni_env, arg))
                        arg = Java::QtCore::QGenericArgument::value(__jni_env, arg);
                    if(metaType && AbstractContainerAccess::isPointerType(*metaType)){
                        QVariant variant = qtjambi_cast<QVariant>(__jni_env, arg);
                        if(variant.isValid() && variant.convert(
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
                                                                metaType->id()
#else
                                                                *metaType
#endif
                                                                )){
                            void*const* ptr = reinterpret_cast<void*const*>(variant.data());
                            return QtJambiAPI::convertNativeToJavaObjectAsWrapper(__jni_env, reinterpret_cast<void*(*)(const void*)>(*__qt_this)(*ptr), returnClassType);
                        }
                    }
                }
            }
        }
    }
    try{
        Java::JNA::Native::getClass(__jni_env);
    }catch(const JavaException& exn){
        JavaException exn2(__jni_env, Java::QtJambi::QUnsuccessfulInvocationException::newInstanceWithMessage(__jni_env, "Unable to load Java Native Access library. Please add JNA to class path."));
        exn2.addSuppressed(__jni_env, exn);
        exn2.raise();
    }
    ffi_type __ffi_type;

    QScopedPointer<QMetaType> _returnMetaType;
    if(!returnValueResolved){
        size_t size = 0;
        short alignment = 1;
        const SuperTypeInfos& infos = SuperTypeInfos::fromClass(__jni_env, returnClassType);
        if(infos.isEmpty()){
            if(!returnMetaType)
                _returnMetaType.reset(returnMetaType = new QMetaType(registerMetaType(__jni_env, returnClassType, false, false)));
            if(!returnMetaType->isValid())
                _returnMetaType.reset(returnMetaType = nullptr);
            if(returnMetaType){
                returnTypeId = getTypeByMetaType(*returnMetaType);
                size = size_t(returnMetaType->sizeOf());
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
                alignment = short(returnMetaType->alignOf());
#endif
            }
        }else{
            returnTypeId = &infos.first().typeId();
            size = infos.first().size();
            alignment = short(getValueAlignment(*returnTypeId));
        }
        if(returnTypeId){
            switch(getEntryType(*returnTypeId)){
            case EntryTypes::FunctionalTypeInfo:
                if(OptionalBool op = isRegisteredAsPointerType(*returnTypeId)){
                    if(op.value()){
                        jnaReturnType = Java::JNA::Pointer::getClass(__jni_env);
                        isFunctionPointer = true;
                    }else{
                        if(returnPointerOrReference==0 && !isReferenceMetaType){
                            if(size==0)
                                Java::QtJambi::QUnsuccessfulInvocationException::throwNew(__jni_env, QString("Type %1 not supported.").arg(QtJambiAPI::getClassName(__jni_env, returnClassType).replace(QLatin1Char('$'), QLatin1Char('.'))) QTJAMBI_STACKTRACEINFO );
                            __ffi_type.size = size;
                            __ffi_type.alignment = alignment;
                            if(!returnMetaType)
                                _returnMetaType.reset(returnMetaType = new QMetaType(registeredMetaTypeID(*returnTypeId)));
                            if(!returnMetaType->isValid())
                                _returnMetaType.reset(returnMetaType = nullptr);
                        }else{
                            jnaReturnType = Java::JNA::Pointer::getClass(__jni_env);
                        }
                    }
                }else{
                    jnaReturnType = Java::JNA::Pointer::getClass(__jni_env);
                }
                break;
            case EntryTypes::ObjectTypeInfo:
            case EntryTypes::InterfaceTypeInfo:
            case EntryTypes::QObjectTypeInfo:
                jnaReturnType = Java::JNA::Pointer::getClass(__jni_env);
                break;
            case EntryTypes::FlagsTypeInfo:
            case EntryTypes::InterfaceValueTypeInfo:
            case EntryTypes::ValueTypeInfo:
            case EntryTypes::SpecialTypeInfo:
            case EntryTypes::StringTypeInfo:
            case EntryTypes::PrimitiveTypeInfo:
                if(returnPointerOrReference==0 && !isReferenceMetaType){
                    if(size==0)
                        Java::QtJambi::QUnsuccessfulInvocationException::throwNew(__jni_env, QString("Type %1 not supported.").arg(QtJambiAPI::getClassName(__jni_env, returnClassType).replace(QLatin1Char('$'), QLatin1Char('.'))) QTJAMBI_STACKTRACEINFO );
                    __ffi_type.size = size;
                    __ffi_type.alignment = alignment;
                    if(!returnMetaType)
                        _returnMetaType.reset(returnMetaType = new QMetaType(registeredMetaTypeID(*returnTypeId)));
                    if(!returnMetaType->isValid())
                        _returnMetaType.reset(returnMetaType = nullptr);
                }else{
                    jnaReturnType = Java::JNA::Pointer::getClass(__jni_env);
                }
                break;
            default:
                if(!returnMetaType)
                    _returnMetaType.reset(returnMetaType = new QMetaType(registeredMetaTypeID(*returnTypeId)));
                if(!returnMetaType->isValid())
                    _returnMetaType.reset(returnMetaType = nullptr);
                if(returnMetaType){
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
                    if(returnMetaType->name().endsWith("*")
                            || returnMetaType->name().contains("(*)")
                            || returnMetaType->name().contains("(__cdecl*)")){
#else
                    if(returnMetaType->flags() & QMetaType::IsPointer){
#endif
                        jnaReturnType = Java::JNA::Pointer::getClass(__jni_env);
                    }else{
                        __ffi_type.size = size;
                        __ffi_type.alignment = alignment;
                    }
                }else{
                    Java::QtJambi::QUnsuccessfulInvocationException::throwNew(__jni_env, QString("Type %1 not supported.").arg(QtJambiAPI::getClassName(__jni_env, returnClassType).replace(QLatin1Char('$'), QLatin1Char('.'))) QTJAMBI_STACKTRACEINFO );
                    return nullptr;
                }
                break;
            }
        }else if(returnPointerOrReference!=0 || isReferenceMetaType){
            jnaReturnType = Java::JNA::Pointer::getClass(__jni_env);
        }else if(returnMetaType){
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
            if(returnMetaType->name().endsWith("*")
                    || returnMetaType->name().contains("(*)")
                    || returnMetaType->name().contains("(__cdecl*)")){
#else
            if(returnMetaType->flags() & QMetaType::IsPointer){
#endif
                jnaReturnType = Java::JNA::Pointer::getClass(__jni_env);
            }else{
                __ffi_type.size = size;
                __ffi_type.alignment = alignment;
            }
        }else{
            Java::QtJambi::QUnsuccessfulInvocationException::throwNew(__jni_env, QString("Type %1 not supported.").arg(QtJambiAPI::getClassName(__jni_env, returnClassType).replace(QLatin1Char('$'), QLatin1Char('.'))) QTJAMBI_STACKTRACEINFO );
            return nullptr;
        }
    }
    QVector<QSharedDataPointer<Cleanup>> cleaners;
    QVector<std::function<void(JNIEnv *)>> successActions;
    convertArgumentList(cleaners, successActions, __jni_env, arguments, length);
    jobject jnaFunction = Java::JNA::Function::getFunction(__jni_env, Java::JNA::Pointer::newInstance(__jni_env, jlong(__qt_this)));
    jobject result = nullptr;
    if(__ffi_type.size>0){
        if(__ffi_type.alignment == 0)
            __ffi_type.alignment = 1;
        const ffi_type* element = nullptr;
        __ffi_type.elements = &element;
        void* ptr;
        bool isAllocated = false;
#ifdef Q_OS_WINDOWS
        __ffi_type.size += (__ffi_type.size % 8) + 1;
        ptr = operator new(__ffi_type.size);
        isAllocated = true;
        memset(ptr, 0, __ffi_type.size);
        //ffi_funtion_dispatch(__jni_env, reinterpret_cast<void*>(__qt_this), arguments, &__ffi_type, ptr);
        Java::JNA::Native::invokeStructure(__jni_env, jnaFunction, jlong(__qt_this), 0, arguments, jlong(ptr), jlong(&__ffi_type));
#else
        jvalue val;
        switch(__ffi_type.size){
        case 1:
            val.b = Java::Runtime::Number::byteValue(__jni_env, Java::JNA::Function::invoke(__jni_env, jnaFunction, Java::Runtime::Byte::primitiveType(__jni_env), arguments));
            ptr = &val.b;
            break;
        case 2:
            val.s = Java::Runtime::Number::shortValue(__jni_env, Java::JNA::Function::invoke(__jni_env, jnaFunction, Java::Runtime::Short::primitiveType(__jni_env), arguments));
            ptr = &val.s;
            break;
        case 4:
            val.i = Java::Runtime::Number::intValue(__jni_env, Java::JNA::Function::invoke(__jni_env, jnaFunction, Java::Runtime::Integer::primitiveType(__jni_env), arguments));
            ptr = &val.i;
            break;
        case 8:
            val.j = Java::Runtime::Number::longValue(__jni_env, Java::JNA::Function::invoke(__jni_env, jnaFunction, Java::Runtime::Long::primitiveType(__jni_env), arguments));
            ptr = &val.j;
            break;
        default:
            if(returnMetaType){
                Java::QtJambi::QUnsuccessfulInvocationException::throwNew(__jni_env, QString("Type %1 not supported as return type.").arg(QLatin1String(returnMetaType->name())) QTJAMBI_STACKTRACEINFO );
            }else{
                Java::QtJambi::QUnsuccessfulInvocationException::throwNew(__jni_env, QString("Type %1 not supported as return type.").arg(QtJambiAPI::getClassName(__jni_env, returnClassType).replace(QLatin1Char('$'), QLatin1Char('.'))) QTJAMBI_STACKTRACEINFO );
            }
            ptr = operator new(__ffi_type.size);
            isAllocated = true;
            memset(ptr, 0, __ffi_type.size);
            //ffi_funtion_dispatch(__jni_env, reinterpret_cast<void*>(__qt_this), arguments, &__ffi_type, ptr);
            Java::JNA::Native::invokeStructure(__jni_env, jnaFunction, jlong(__qt_this), 0, arguments, jlong(ptr), jlong(&__ffi_type));
            break;
        }
#endif
        if(returnTypeId){
            result = internal_convertNativeToJavaObject(__jni_env, ptr, *returnTypeId, nullptr, !isAllocated ? NativeToJavaConversionMode::MakeCopyOfValues : NativeToJavaConversionMode::None);
        }else {
            if(!isAllocated){
                result = QtJambiAPI::convertNativeToJavaObjectAsCopy(__jni_env, ptr, returnClassType);
            }else{
                result = QtJambiAPI::convertNativeToJavaObjectAsWrapper(__jni_env, ptr, returnClassType);
            }
        }
        if(result && returnClassType && __jni_env->IsInstanceOf(result, returnClassType)){
            if(QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForJavaInterface(__jni_env, result)){
                if(link->pointer()!=ptr){
                    if(returnMetaType)
                        returnMetaType->destruct(ptr);
                    if(isAllocated)
                        operator delete(ptr);
                }
            }else{
                if(returnMetaType)
                    returnMetaType->destruct(ptr);
                if(isAllocated)
                    operator delete(ptr);
            }
        }else if(returnMetaType){
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
            result = QtJambiAPI::convertQVariantToJavaObject(__jni_env, QVariant(returnMetaType->id(), ptr));
#else
            result = QtJambiAPI::convertQVariantToJavaObject(__jni_env, QVariant(*returnMetaType, ptr));
#endif
        }
    }else{
        result = Java::JNA::Function::invoke(__jni_env, jnaFunction, jnaReturnType, arguments);
        if(Java::JNA::Pointer::isSameClass(__jni_env, jnaReturnType)){
            if(Java::JNA::Pointer::isInstanceOf(__jni_env, result)){
                void* ptr = reinterpret_cast<void*>(Java::JNA::Pointer::peer(__jni_env, result));
                if(returnPointerOrReference==2 && returnClassType){
                    QFunctionPointer fp = reinterpret_cast<QFunctionPointer>(ptr);
                    bool resolved = false;
                    {
                        QReadLocker locker(gPointerLock());
                        if(gObjectsByFunctionPointer->contains(quintptr(fp))){
                            QList<JObjectWrapper>& objects = (*gObjectsByFunctionPointer())[quintptr(fp)];
                            for(JObjectWrapper& wrapper : objects){
                                if(__jni_env->IsInstanceOf(wrapper.object(), returnClassType)){
                                    result = __jni_env->NewLocalRef(wrapper.object());
                                    resolved = true;
                                }
                            }
                        }
                    }
                    if(!resolved){
                        result = Java::QtCore::QFunctionPointerUtil::createProxy(__jni_env, returnClassType);
                        QtJambiLink::createLinkForNativeObject(
                                __jni_env,
                                result,
                                new QFunctionPointer(fp),
                                LINK_NAME_ARG(nullptr)
                                false,
                                false,
                                &delete_callback_pointer,
                                QtJambiLink::Ownership::Java
                            );
                        QWriteLocker locker(gPointerLock());
                        (*gObjectsByFunctionPointer())[quintptr(fp)] << JObjectWrapper(__jni_env, result);
                    }
                }else{
                    void* _ptr = ptr;
                    if(isFunctionPointer){
                        _ptr = &ptr;
                    }
                    if(returnTypeId){
                        result = internal_convertNativeToJavaObject(__jni_env, _ptr, *returnTypeId, nullptr, isFunctionPointer || (!isReferenceMetaType && (returnPointerOrReference==0 || returnPointerOrReference==-2)) ? NativeToJavaConversionMode::MakeCopyOfValues : NativeToJavaConversionMode::None);
                    }else {
                        if(isFunctionPointer || (!isReferenceMetaType && (returnPointerOrReference==0 || returnPointerOrReference==-2))){
                            result = QtJambiAPI::convertNativeToJavaObjectAsCopy(__jni_env, _ptr, returnClassType);
                        }else{
                            result = QtJambiAPI::convertNativeToJavaObjectAsWrapper(__jni_env, _ptr, returnClassType);
                        }
                    }
                }
            }
        }else if(!returnMetaType && Java::QtJambi::QtEnumerator::isAssignableFrom(__jni_env, returnClassType)){
            if(Java::Runtime::Number::isInstanceOf(__jni_env, result)){
                result = QtJambiAPI::convertEnumToJavaObject(__jni_env, qint32(Java::Runtime::Number::intValue(__jni_env, result)), returnClassType);
            }else
                result = nullptr;
        }else if(!returnMetaType && Java::QtJambi::QtShortEnumerator::isAssignableFrom(__jni_env, returnClassType)){
            if(Java::Runtime::Number::isInstanceOf(__jni_env, result)){
                result = QtJambiAPI::convertEnumToJavaObject(__jni_env, qint16(Java::Runtime::Number::shortValue(__jni_env, result)), returnClassType);
            }else
                result = nullptr;
        }else if(!returnMetaType && Java::QtJambi::QtByteEnumerator::isAssignableFrom(__jni_env, returnClassType)){
            if(Java::Runtime::Number::isInstanceOf(__jni_env, result)){
                result = QtJambiAPI::convertEnumToJavaObject(__jni_env, qint8(Java::Runtime::Number::byteValue(__jni_env, result)), returnClassType);
            }else
                result = nullptr;
        }else if(!returnMetaType && Java::QtJambi::QtLongEnumerator::isAssignableFrom(__jni_env, returnClassType)){
            if(Java::Runtime::Number::isInstanceOf(__jni_env, result)){
                result = QtJambiAPI::convertEnumToJavaObject(__jni_env, qint64(Java::Runtime::Number::longValue(__jni_env, result)), returnClassType);
            }else
                result = nullptr;
        }
    }
    for(auto action : successActions){
        action(__jni_env);
        JavaException::check(__jni_env QTJAMBI_STACKTRACEINFO );
    }
    return result;
}

jclass CoreAPI::getFunctionPointerReturnType(JNIEnv * __jni_env, jobject returnType){
    QMetaType* returnMetaType = nullptr;
    jint returnPointerOrReference = 0;
    jclass returnClassType = nullptr;
    const std::type_info* returnTypeId = nullptr;
    bool isReferenceMetaType = false;
    returnMetaType = qtjambi_cast<QMetaType*>(__jni_env, Java::QtCore::QGenericArgumentType::metaType(__jni_env, returnType));
    if(returnMetaType && !returnMetaType->isValid())
        returnMetaType = nullptr;
    if(returnMetaType)
        isReferenceMetaType = QByteArray(returnMetaType->name()).endsWith("&");
    returnPointerOrReference = Java::QtCore::QGenericArgumentType::pointerOrReference(__jni_env, returnType);
    returnClassType = Java::QtCore::QGenericArgumentType::classType(__jni_env, returnType);
    if(returnPointerOrReference==2 && returnClassType){
        return Java::JNA::Pointer::getClass(__jni_env);
    }else if((!returnMetaType && (__jni_env->IsSameObject(returnClassType, nullptr)
            || __jni_env->IsSameObject(returnClassType, Java::Runtime::Void::getClass(__jni_env))
            || __jni_env->IsSameObject(returnClassType, Java::Runtime::Void::primitiveType(__jni_env))))
            || (returnMetaType && returnPointerOrReference==0 && !isReferenceMetaType && returnMetaType->id()==QMetaType::Void)){
        return Java::Runtime::Void::primitiveType(__jni_env);
    }else if((!returnMetaType && (__jni_env->IsSameObject(returnClassType, Java::Runtime::Integer::getClass(__jni_env))
                                || __jni_env->IsSameObject(returnClassType, Java::Runtime::Integer::primitiveType(__jni_env))))
             || (returnMetaType && returnPointerOrReference==0 && !isReferenceMetaType && ( returnMetaType->id()==QMetaType::Int
                                                                    || returnMetaType->id()==QMetaType::UInt
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
                                                                    || returnMetaType->id()==QMetaType::Char32
#endif
                                                                    ))){
        return Java::Runtime::Integer::primitiveType(__jni_env);
    }else if(!returnMetaType && Java::QtJambi::QtEnumerator::isAssignableFrom(__jni_env, returnClassType)){
        return Java::Runtime::Integer::primitiveType(__jni_env);
    }else if(!returnMetaType && Java::QtJambi::QtByteEnumerator::isAssignableFrom(__jni_env, returnClassType)){
        return Java::Runtime::Byte::primitiveType(__jni_env);
    }else if(!returnMetaType && Java::QtJambi::QtShortEnumerator::isAssignableFrom(__jni_env, returnClassType)){
        return Java::Runtime::Short::primitiveType(__jni_env);
    }else if(!returnMetaType && Java::QtJambi::QtLongEnumerator::isAssignableFrom(__jni_env, returnClassType)){
        return Java::Runtime::Long::primitiveType(__jni_env);
    }else if((!returnMetaType && (__jni_env->IsSameObject(returnClassType, Java::Runtime::Byte::getClass(__jni_env))
                                    || __jni_env->IsSameObject(returnClassType, Java::Runtime::Byte::primitiveType(__jni_env))))
             || (returnMetaType && returnPointerOrReference==0 && !isReferenceMetaType && ( returnMetaType->id()==QMetaType::Char
                                                                    || returnMetaType->id()==QMetaType::UChar
                                                                    || returnMetaType->id()==QMetaType::SChar ))){
        return Java::Runtime::Byte::primitiveType(__jni_env);
    }else if((!returnMetaType && (__jni_env->IsSameObject(returnClassType, Java::Runtime::Short::getClass(__jni_env))
                                    || __jni_env->IsSameObject(returnClassType, Java::Runtime::Short::primitiveType(__jni_env))))
             || (returnMetaType && returnPointerOrReference==0 && !isReferenceMetaType && ( returnMetaType->id()==QMetaType::Short
                                                                    || returnMetaType->id()==QMetaType::UShort))){
        return Java::Runtime::Short::primitiveType(__jni_env);
    }else if((!returnMetaType && (__jni_env->IsSameObject(returnClassType, Java::Runtime::Long::getClass(__jni_env))
                                     || __jni_env->IsSameObject(returnClassType, Java::Runtime::Long::primitiveType(__jni_env))))
             || (returnMetaType && returnPointerOrReference==0 && !isReferenceMetaType && ( returnMetaType->id()==QMetaType::LongLong
                                                                    || returnMetaType->id()==QMetaType::ULongLong))){
        return Java::Runtime::Long::primitiveType(__jni_env);
    }else if((!returnMetaType && (__jni_env->IsSameObject(returnClassType, Java::Runtime::Float::getClass(__jni_env))
                                    || __jni_env->IsSameObject(returnClassType, Java::Runtime::Float::primitiveType(__jni_env))))
             || (returnMetaType && returnPointerOrReference==0 && !isReferenceMetaType && returnMetaType->id()==QMetaType::Float)){
        return Java::Runtime::Float::primitiveType(__jni_env);
    }else if((!returnMetaType && (__jni_env->IsSameObject(returnClassType, Java::Runtime::Double::getClass(__jni_env))
                                     || __jni_env->IsSameObject(returnClassType, Java::Runtime::Double::primitiveType(__jni_env))))
             || (returnMetaType && returnPointerOrReference==0 && !isReferenceMetaType && returnMetaType->id()==QMetaType::Double)){
        return Java::Runtime::Double::primitiveType(__jni_env);
    }else if((!returnMetaType && (__jni_env->IsSameObject(returnClassType, Java::Runtime::Boolean::getClass(__jni_env))
                                     || __jni_env->IsSameObject(returnClassType, Java::Runtime::Boolean::primitiveType(__jni_env))))
             || (returnMetaType && returnPointerOrReference==0 && !isReferenceMetaType && returnMetaType->id()==QMetaType::Bool)){
        return Java::Runtime::Boolean::primitiveType(__jni_env);
    }else if((!returnMetaType && (__jni_env->IsSameObject(returnClassType, Java::Runtime::Character::getClass(__jni_env))
                                    || __jni_env->IsSameObject(returnClassType, Java::Runtime::Character::primitiveType(__jni_env))))
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
             || (returnMetaType && returnPointerOrReference==0 && !isReferenceMetaType && returnMetaType->id()==QMetaType::Char16)
#endif
             ){
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
        if(returnMetaType && returnPointerOrReference==0 && !isReferenceMetaType && returnMetaType->id()==QMetaType::Char16){
            return Java::Runtime::Character::primitiveType(__jni_env);
        }
#endif
    }

    QScopedPointer<QMetaType> _returnMetaType;
    size_t size = 0;
    const SuperTypeInfos& infos = SuperTypeInfos::fromClass(__jni_env, returnClassType);
    if(infos.isEmpty()){
        if(!returnMetaType)
            _returnMetaType.reset(returnMetaType = new QMetaType(registerMetaType(__jni_env, returnClassType, false, false)));
        if(!returnMetaType->isValid())
            _returnMetaType.reset(returnMetaType = nullptr);
        if(returnMetaType){
            returnTypeId = getTypeByMetaType(*returnMetaType);
            size = size_t(returnMetaType->sizeOf());
        }
    }else{
        returnTypeId = &infos.first().typeId();
        size = infos.first().size();
    }
    if(returnTypeId){
        switch(getEntryType(*returnTypeId)){
        case EntryTypes::FunctionalTypeInfo:
            if(OptionalBool op = isRegisteredAsPointerType(*returnTypeId)){
                if(op.value()){
                    return Java::JNA::Pointer::getClass(__jni_env);
                }else{
                    if(returnPointerOrReference==0 && !isReferenceMetaType){
                        switch(size){
                        case sizeof(jbyte):
                            return Java::Runtime::Byte::primitiveType(__jni_env);
                        case sizeof(jshort):
                            return Java::Runtime::Short::primitiveType(__jni_env);
                        case sizeof(jint):
                            return Java::Runtime::Integer::primitiveType(__jni_env);
                        case sizeof(jlong):
                            return Java::Runtime::Long::primitiveType(__jni_env);
                        default:
                            Java::QtJambi::QUnsuccessfulInvocationException::throwNew(__jni_env, QString("Type %1 not supported.").arg(QtJambiAPI::getClassName(__jni_env, returnClassType).replace(QLatin1Char('$'), QLatin1Char('.'))) QTJAMBI_STACKTRACEINFO );
                            return nullptr;
                        }
                    }else{
                        return Java::JNA::Pointer::getClass(__jni_env);
                    }
                }
            }else{
                return Java::JNA::Pointer::getClass(__jni_env);
            }
        case EntryTypes::ObjectTypeInfo:
        case EntryTypes::InterfaceTypeInfo:
        case EntryTypes::QObjectTypeInfo:
            return Java::JNA::Pointer::getClass(__jni_env);
        case EntryTypes::FlagsTypeInfo:
        case EntryTypes::InterfaceValueTypeInfo:
        case EntryTypes::ValueTypeInfo:
        case EntryTypes::SpecialTypeInfo:
        case EntryTypes::StringTypeInfo:
        case EntryTypes::PrimitiveTypeInfo:
            if(returnPointerOrReference==0 && !isReferenceMetaType){
                switch(size){
                case sizeof(jbyte):
                    return Java::Runtime::Byte::primitiveType(__jni_env);
                case sizeof(jshort):
                    return Java::Runtime::Short::primitiveType(__jni_env);
                case sizeof(jint):
                    return Java::Runtime::Integer::primitiveType(__jni_env);
                case sizeof(jlong):
                    return Java::Runtime::Long::primitiveType(__jni_env);
                default:
                    Java::QtJambi::QUnsuccessfulInvocationException::throwNew(__jni_env, QString("Type %1 not supported.").arg(QtJambiAPI::getClassName(__jni_env, returnClassType).replace(QLatin1Char('$'), QLatin1Char('.'))) QTJAMBI_STACKTRACEINFO );
                    return nullptr;
                }
            }else{
                return Java::JNA::Pointer::getClass(__jni_env);
            }
        default:
            if(returnMetaType){
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
                if(returnMetaType->name().endsWith("*")
                        || returnMetaType->name().contains("(*)")
                        || returnMetaType->name().contains("(__cdecl*)")){
#else
                if(returnMetaType->flags() & QMetaType::IsPointer){
#endif
                    return Java::JNA::Pointer::getClass(__jni_env);
                }else{
                    switch(returnMetaType->sizeOf()){
                    case sizeof(jbyte):
                        return Java::Runtime::Byte::primitiveType(__jni_env);
                    case sizeof(jshort):
                        return Java::Runtime::Short::primitiveType(__jni_env);
                    case sizeof(jint):
                        return Java::Runtime::Integer::primitiveType(__jni_env);
                    case sizeof(jlong):
                        return Java::Runtime::Long::primitiveType(__jni_env);
                    default:
                        Java::QtJambi::QUnsuccessfulInvocationException::throwNew(__jni_env, QString("Type %1 not supported.").arg(QtJambiAPI::getClassName(__jni_env, returnClassType).replace(QLatin1Char('$'), QLatin1Char('.'))) QTJAMBI_STACKTRACEINFO );
                        return nullptr;
                    }
                }
            }else{
                Java::QtJambi::QUnsuccessfulInvocationException::throwNew(__jni_env, QString("Type %1 not supported.").arg(QtJambiAPI::getClassName(__jni_env, returnClassType).replace(QLatin1Char('$'), QLatin1Char('.'))) QTJAMBI_STACKTRACEINFO );
                return nullptr;
            }
        }
    }else if(returnPointerOrReference!=0 || isReferenceMetaType){
        return Java::JNA::Pointer::getClass(__jni_env);
    }else if(returnMetaType){
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
        if(returnMetaType->name().endsWith("*")
                || returnMetaType->name().contains("(*)")
                || returnMetaType->name().contains("(__cdecl*)")){
#else
        if(returnMetaType->flags() & QMetaType::IsPointer){
#endif
            return Java::JNA::Pointer::getClass(__jni_env);
        }else{
            switch(returnMetaType->sizeOf()){
            case sizeof(jbyte):
                return Java::Runtime::Byte::primitiveType(__jni_env);
            case sizeof(jshort):
                return Java::Runtime::Short::primitiveType(__jni_env);
            case sizeof(jint):
                return Java::Runtime::Integer::primitiveType(__jni_env);
            case sizeof(jlong):
                return Java::Runtime::Long::primitiveType(__jni_env);
            default:
                Java::QtJambi::QUnsuccessfulInvocationException::throwNew(__jni_env, QString("Type %1 not supported.").arg(QtJambiAPI::getClassName(__jni_env, returnClassType).replace(QLatin1Char('$'), QLatin1Char('.'))) QTJAMBI_STACKTRACEINFO );
                return nullptr;
            }
        }
    }else{
        Java::QtJambi::QUnsuccessfulInvocationException::throwNew(__jni_env, QString("Type %1 not supported.").arg(QtJambiAPI::getClassName(__jni_env, returnClassType).replace(QLatin1Char('$'), QLatin1Char('.'))) QTJAMBI_STACKTRACEINFO );
        return nullptr;
    }
}

jobject CoreAPI::convertFunctionPointerReturn(JNIEnv * __jni_env, jobject returnType, jobject result){
    const QMetaType* returnMetaType = qtjambi_cast<QMetaType*>(__jni_env, Java::QtCore::QGenericArgumentType::metaType(__jni_env, returnType));
    if(returnMetaType && !returnMetaType->isValid())
        returnMetaType = nullptr;
    bool isReferenceMetaType = false;
    if(returnMetaType)
        isReferenceMetaType = QByteArray(returnMetaType->name()).endsWith("&");
    jint returnPointerOrReference = Java::QtCore::QGenericArgumentType::pointerOrReference(__jni_env, returnType);
    jclass returnClassType = Java::QtCore::QGenericArgumentType::classType(__jni_env, returnType);
    if(returnPointerOrReference==2 && returnClassType){
        QFunctionPointer ptr = nullptr;
        if(result){
            bool resolved = false;
            if(QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForJavaInterface(__jni_env, result)){
                if(DeletablePointerToObjectLink* plink = dynamic_cast<DeletablePointerToObjectLink*>(link.get())){
                    if(plink->pointer() && plink->deleterFunction()==&delete_callback_pointer){
                        ptr = *reinterpret_cast<QFunctionPointer*>(plink->pointer());
                        resolved = true;
                    }
                }
            }
            if(!resolved){
                jobject ih = Java::QtCore::QFunctionPointerUtil::createCallbackProxy(__jni_env, Java::JNA::CallbackProxy::getClass(__jni_env), returnClassType, result);
                jobject callbackProxy = Java::QtCore::QFunctionPointerUtil$CppToJavaInvocationHandler::proxy(__jni_env, ih);
                jobject method = __jni_env->ToReflectedMethod(Java::JNA::CallbackProxy::getClass(__jni_env), Java::JNA::CallbackProxy::callbackMethod_ID(__jni_env), false);
                jlong peer = Java::JNA::Native::createNativeCallback(__jni_env,
                                                              callbackProxy,
                                                              method,
                                                              Java::JNA::CallbackProxy::getParameterTypes(__jni_env, callbackProxy),
                                                              Java::JNA::CallbackProxy::getReturnType(__jni_env, callbackProxy),
                                                              0,
                                                              0,
                                                              __jni_env->NewStringUTF("UTF-8"));
                ptr = *reinterpret_cast<QFunctionPointer*>(peer);
                QFunctionPointer fp = reinterpret_cast<QFunctionPointer>(ptr);
                QtJambiLink::createLinkForNativeObject(
                        __jni_env,
                        result,
                        new QFunctionPointer(ptr),
                        LINK_NAME_ARG(nullptr)
                        false,
                        false,
                        &delete_callback_pointer,
                        QtJambiLink::Ownership::Java
                    );
                {
                    QWriteLocker locker(gPointerLock());
                    (*gObjectsByFunctionPointer())[quintptr(fp)] << JObjectWrapper(__jni_env, result);
                }
                Java::QtCore::QFunctionPointerUtil$CppToJavaInvocationHandler::set_peer(__jni_env, ih, peer);
                Java::QtCore::QFunctionPointerUtil::registerCleanup(__jni_env, result, ih);
            }
        }
        return Java::JNA::Pointer::newInstance(__jni_env, ptr);
    }else if((!returnMetaType && (__jni_env->IsSameObject(returnClassType, nullptr)
            || Java::Runtime::Void::isSameClass(__jni_env, returnClassType)
            || Java::Runtime::Void::isPrimitiveType(__jni_env, returnClassType)))
            || (returnMetaType && returnPointerOrReference==0 && !isReferenceMetaType && returnMetaType->id()==QMetaType::Void)){
        return result;
    }else if((!returnMetaType && (Java::Runtime::Integer::isSameClass(__jni_env, returnClassType)
                                || Java::Runtime::Integer::isPrimitiveType(__jni_env, returnClassType)))
             || (returnMetaType && returnPointerOrReference==0 && !isReferenceMetaType && ( returnMetaType->id()==QMetaType::Int
                                                                    || returnMetaType->id()==QMetaType::UInt
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
                                                                    || returnMetaType->id()==QMetaType::Char32
#endif
                                                                    ))){
        return result;
    }else if(!returnMetaType && Java::QtJambi::QtEnumerator::isAssignableFrom(__jni_env, returnClassType)){
        if(Java::QtJambi::QtEnumerator::isInstanceOf(__jni_env, result)){
            return Java::Runtime::Integer::valueOf(__jni_env, Java::QtJambi::QtEnumerator::value(__jni_env, result));
        }else
            return nullptr;
    }else if(!returnMetaType && Java::QtJambi::QtShortEnumerator::isAssignableFrom(__jni_env, returnClassType)){
        if(Java::QtJambi::QtShortEnumerator::isInstanceOf(__jni_env, result)){
            return Java::Runtime::Short::valueOf(__jni_env, Java::QtJambi::QtShortEnumerator::value(__jni_env, result));
        }else
            return nullptr;
    }else if(!returnMetaType && Java::QtJambi::QtByteEnumerator::isAssignableFrom(__jni_env, returnClassType)){
        if(Java::QtJambi::QtByteEnumerator::isInstanceOf(__jni_env, result)){
            return Java::Runtime::Integer::valueOf(__jni_env, Java::QtJambi::QtByteEnumerator::value(__jni_env, result));
        }else
            return nullptr;
    }else if(!returnMetaType && Java::QtJambi::QtLongEnumerator::isAssignableFrom(__jni_env, returnClassType)){
        if(Java::QtJambi::QtLongEnumerator::isInstanceOf(__jni_env, result)){
            return Java::Runtime::Integer::valueOf(__jni_env, Java::QtJambi::QtLongEnumerator::value(__jni_env, result));
        }else
            return nullptr;
    }else if((!returnMetaType && (Java::Runtime::Byte::isSameClass(__jni_env, returnClassType)
                                    || Java::Runtime::Byte::isPrimitiveType(__jni_env, returnClassType)))
             || (returnMetaType && returnPointerOrReference==0 && !isReferenceMetaType && ( returnMetaType->id()==QMetaType::Char
                                                                    || returnMetaType->id()==QMetaType::UChar
                                                                    || returnMetaType->id()==QMetaType::SChar ))){
        return result;
    }else if((!returnMetaType && (Java::Runtime::Short::isSameClass(__jni_env, returnClassType)
                                    || Java::Runtime::Short::isPrimitiveType(__jni_env, returnClassType)))
             || (returnMetaType && returnPointerOrReference==0 && !isReferenceMetaType && ( returnMetaType->id()==QMetaType::Short
                                                                    || returnMetaType->id()==QMetaType::UShort))){
        return result;
    }else if((!returnMetaType && (Java::Runtime::Long::isSameClass(__jni_env, returnClassType)
                                     || Java::Runtime::Long::isPrimitiveType(__jni_env, returnClassType)))
             || (returnMetaType && returnPointerOrReference==0 && !isReferenceMetaType && ( returnMetaType->id()==QMetaType::LongLong
                                                                    || returnMetaType->id()==QMetaType::ULongLong))){
        return result;
    }else if((!returnMetaType && (Java::Runtime::Float::isSameClass(__jni_env, returnClassType)
                                    || Java::Runtime::Float::isPrimitiveType(__jni_env, returnClassType)))
             || (returnMetaType && returnPointerOrReference==0 && !isReferenceMetaType && returnMetaType->id()==QMetaType::Float)){
        return result;
    }else if((!returnMetaType && (Java::Runtime::Double::isSameClass(__jni_env, returnClassType)
                                     || Java::Runtime::Double::isPrimitiveType(__jni_env, returnClassType)))
             || (returnMetaType && returnPointerOrReference==0 && !isReferenceMetaType && returnMetaType->id()==QMetaType::Double)){
        return result;
    }else if((!returnMetaType && (Java::Runtime::Boolean::isSameClass(__jni_env, returnClassType)
                                     || Java::Runtime::Boolean::isPrimitiveType(__jni_env, returnClassType)))
             || (returnMetaType && returnPointerOrReference==0 && !isReferenceMetaType && returnMetaType->id()==QMetaType::Bool)){
        return result;
    }else if((!returnMetaType && (Java::Runtime::Character::isSameClass(__jni_env, returnClassType)
                                    || Java::Runtime::Character::isPrimitiveType(__jni_env, returnClassType)))
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
             || (returnMetaType && returnPointerOrReference==0 && !isReferenceMetaType && returnMetaType->id()==QMetaType::Char16)
#endif
             ){
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
        if(returnMetaType && returnPointerOrReference==0 && !isReferenceMetaType && returnMetaType->id()==QMetaType::Char16){
            return result;
        }
#endif
    }
    const std::type_info* returnTypeId = nullptr;

    QScopedPointer<const QMetaType> _returnMetaType;
    const SuperTypeInfos& infos = SuperTypeInfos::fromClass(__jni_env, returnClassType);
    if(infos.isEmpty()){
        if(!returnMetaType)
            _returnMetaType.reset(returnMetaType = new QMetaType(registerMetaType(__jni_env, returnClassType, false, false)));
        if(!returnMetaType->isValid())
            _returnMetaType.reset(returnMetaType = nullptr);
        if(returnMetaType){
            returnTypeId = getTypeByMetaType(*returnMetaType);
        }
    }else{
        returnTypeId = &infos.first().typeId();
    }
    void* ptr = nullptr;
    if(returnTypeId){
        bool isValueFunctional = false;
        switch(getEntryType(*returnTypeId)){
        case EntryTypes::FunctionalTypeInfo:
            isValueFunctional = true;
            if(OptionalBool op = isRegisteredAsPointerType(*returnTypeId)){
                if(op.value()){
                    if(QtJambiAPI::convertJavaToNative(__jni_env, *returnTypeId, result, &ptr)){
                        return Java::JNA::Pointer::newInstance(__jni_env, *reinterpret_cast<void**>(ptr));
                    }else{
                        Java::QtJambi::QUnsuccessfulInvocationException::throwNew(__jni_env, QString("Type %1 not supported.").arg(QtJambiAPI::getClassName(__jni_env, returnClassType).replace(QLatin1Char('$'), QLatin1Char('.'))) QTJAMBI_STACKTRACEINFO );
                        return nullptr;
                    }
                }
            }
            Q_FALLTHROUGH();
        case EntryTypes::ObjectTypeInfo:
        case EntryTypes::InterfaceTypeInfo:
        case EntryTypes::QObjectTypeInfo:
            if(!isValueFunctional){
                if(QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForJavaInterface(__jni_env, result)){
                    return Java::JNA::Pointer::newInstance(__jni_env, link->pointer());
                }else if(QtJambiAPI::convertJavaToNative(__jni_env, *returnTypeId, result, &ptr)){
                    return Java::JNA::Pointer::newInstance(__jni_env, ptr);
                }else{
                    Java::QtJambi::QUnsuccessfulInvocationException::throwNew(__jni_env, QString("Type %1 not supported.").arg(QtJambiAPI::getClassName(__jni_env, returnClassType).replace(QLatin1Char('$'), QLatin1Char('.'))) QTJAMBI_STACKTRACEINFO );
                    return nullptr;
                }
            }
            Q_FALLTHROUGH();
        case EntryTypes::FlagsTypeInfo:
        case EntryTypes::InterfaceValueTypeInfo:
        case EntryTypes::ValueTypeInfo:
        case EntryTypes::SpecialTypeInfo:
        case EntryTypes::StringTypeInfo:
        case EntryTypes::PrimitiveTypeInfo:
            if(QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForJavaInterface(__jni_env, result)){
                ptr = link->pointer();
                if(!returnMetaType){
                    if(PointerToObjectLink* plink = dynamic_cast<PointerToObjectLink*>(link.get())){
                        returnMetaType = &plink->metaType();
                        if(!returnMetaType->isValid())
                            returnMetaType = nullptr;
                    }
                }
                if(returnPointerOrReference==0 && !isReferenceMetaType){
                    if(!returnMetaType)
                        _returnMetaType.reset(returnMetaType = new QMetaType(registeredMetaTypeID(*returnTypeId)));
                    if(!returnMetaType->isValid())
                        _returnMetaType.reset(returnMetaType = nullptr);
                    if(returnMetaType){
                        switch(returnMetaType->sizeOf()){
                        case sizeof(jbyte):{
                                jbyte data = 0;
                                returnMetaType->construct(&data, ptr);
                                return Java::Runtime::Byte::valueOf(__jni_env, data);
                            }
                        case sizeof(jshort):{
                                jshort data = 0;
                                returnMetaType->construct(&data, ptr);
                                return Java::Runtime::Short::valueOf(__jni_env, data);
                            }
                        case sizeof(jint):{
                                jint data = 0;
                                returnMetaType->construct(&data, ptr);
                                return Java::Runtime::Integer::valueOf(__jni_env, data);
                            }
                        case sizeof(jlong):{
                                jlong data = 0;
                                returnMetaType->construct(&data, ptr);
                                return Java::Runtime::Long::valueOf(__jni_env, data);
                            }
                        default:
                            Java::QtJambi::QUnsuccessfulInvocationException::throwNew(__jni_env, QString("Type %1 not supported.").arg(QtJambiAPI::getClassName(__jni_env, returnClassType).replace(QLatin1Char('$'), QLatin1Char('.'))) QTJAMBI_STACKTRACEINFO );
                        }
                    }else{
                        Java::QtJambi::QUnsuccessfulInvocationException::throwNew(__jni_env, QString("Type %1 not supported.").arg(QtJambiAPI::getClassName(__jni_env, returnClassType).replace(QLatin1Char('$'), QLatin1Char('.'))) QTJAMBI_STACKTRACEINFO );
                    }
                }else{
                    return Java::JNA::Pointer::newInstance(__jni_env, ptr);
                }
            }else{
                if(!returnMetaType)
                    _returnMetaType.reset(returnMetaType = new QMetaType(registeredMetaTypeID(*returnTypeId)));
                if(!returnMetaType->isValid())
                    _returnMetaType.reset(returnMetaType = nullptr);
                if(returnMetaType){
                    ptr = returnMetaType->create(nullptr);
                    if(!QtJambiAPI::convertJavaToNative(__jni_env, *returnTypeId, result, ptr)){
                        if(Java::Runtime::Map::isInstanceOf(__jni_env, result) || Java::Runtime::Collection::isInstanceOf(__jni_env, result)){
                            QVariant variant = CoreAPI::convertCheckedObjectToQVariant(__jni_env, result, *returnMetaType);
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
                            if(variant.metaType()==*returnMetaType || variant.convert(*returnMetaType)){
#else
                            if(variant.userType()==returnMetaType->id() || variant.convert(returnMetaType->id())){
#endif
                                returnMetaType->destruct(ptr);
                                returnMetaType->construct(ptr, variant.data());
                            }else{
                                returnMetaType->destroy(ptr);
                                Java::QtJambi::QUnsuccessfulInvocationException::throwNew(__jni_env, QString("Type %1 not supported.").arg(QtJambiAPI::getObjectClassName(__jni_env, result).replace(QLatin1Char('$'), QLatin1Char('.'))) QTJAMBI_STACKTRACEINFO );
                                return nullptr;
                            }
                        }else{
                            returnMetaType->destroy(ptr);
                            Java::QtJambi::QUnsuccessfulInvocationException::throwNew(__jni_env, QString("Type %1 not supported.").arg(QtJambiAPI::getObjectClassName(__jni_env, result).replace(QLatin1Char('$'), QLatin1Char('.'))) QTJAMBI_STACKTRACEINFO );
                            return nullptr;
                        }
                    }
                    if(returnPointerOrReference==0 && !isReferenceMetaType){
                        switch(returnMetaType->sizeOf()){
                        case sizeof(jbyte):{
                                jbyte data = 0;
                                returnMetaType->construct(&data, ptr);
                                returnMetaType->destroy(ptr);
                                return Java::Runtime::Byte::valueOf(__jni_env, data);
                            }
                        case sizeof(jshort):{
                                jshort data = 0;
                                returnMetaType->construct(&data, ptr);
                                returnMetaType->destroy(ptr);
                                return Java::Runtime::Short::valueOf(__jni_env, data);
                            }
                        case sizeof(jint):{
                                jint data = 0;
                                returnMetaType->construct(&data, ptr);
                                returnMetaType->destroy(ptr);
                                return Java::Runtime::Integer::valueOf(__jni_env, data);
                            }
                        case sizeof(jlong):{
                                jlong data = 0;
                                returnMetaType->construct(&data, ptr);
                                returnMetaType->destroy(ptr);
                                return Java::Runtime::Long::valueOf(__jni_env, data);
                            }
                        default:
                            returnMetaType->destroy(ptr);
                            Java::QtJambi::QUnsuccessfulInvocationException::throwNew(__jni_env, QString("Type %1 not supported.").arg(QtJambiAPI::getClassName(__jni_env, returnClassType).replace(QLatin1Char('$'), QLatin1Char('.'))) QTJAMBI_STACKTRACEINFO );
                        }
                    }else{
                        return Java::JNA::Pointer::newInstance(__jni_env, ptr);
                        //cleaners.append(QSharedDataPointer<Cleanup>{new Cleanup{[returnMetaType, ptr](){ returnMetaType->destroy(ptr); }}});
                    }
                }else{
                    Java::QtJambi::QUnsuccessfulInvocationException::throwNew(__jni_env, QString("Type %1 not supported.").arg(QtJambiAPI::getObjectClassName(__jni_env, result).replace(QLatin1Char('$'), QLatin1Char('.'))) QTJAMBI_STACKTRACEINFO );
                }
            }
            return nullptr;
        default:
            break;
        }
    }else if(returnPointerOrReference!=0 || isReferenceMetaType){
        if(QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForJavaInterface(__jni_env, result)){
            return Java::JNA::Pointer::newInstance(__jni_env, link->pointer());
        }else{
            return Java::JNA::Pointer::newInstance(__jni_env, result);
        }
    }else if(returnMetaType){
        if(QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForJavaInterface(__jni_env, result)){
            ptr = link->pointer();
            if(returnPointerOrReference==0 && !isReferenceMetaType){
                switch(returnMetaType->sizeOf()){
                case sizeof(jbyte):{
                        jbyte data = 0;
                        returnMetaType->construct(&data, ptr);
                        return Java::Runtime::Byte::valueOf(__jni_env, data);
                    }
                case sizeof(jshort):{
                        jshort data = 0;
                        returnMetaType->construct(&data, ptr);
                        return Java::Runtime::Short::valueOf(__jni_env, data);
                    }
                case sizeof(jint):{
                        jint data = 0;
                        returnMetaType->construct(&data, ptr);
                        return Java::Runtime::Integer::valueOf(__jni_env, data);
                    }
                case sizeof(jlong):{
                        jlong data = 0;
                        returnMetaType->construct(&data, ptr);
                        return Java::Runtime::Long::valueOf(__jni_env, data);
                    }
                default:
                    returnMetaType->destroy(ptr);
                    Java::QtJambi::QUnsuccessfulInvocationException::throwNew(__jni_env, QString("Type %1 not supported.").arg(QtJambiAPI::getClassName(__jni_env, returnClassType).replace(QLatin1Char('$'), QLatin1Char('.'))) QTJAMBI_STACKTRACEINFO );
                }
            }else{
                return Java::JNA::Pointer::newInstance(__jni_env, ptr);
            }
        }else{
            ptr = returnMetaType->create(nullptr);
            if(!returnTypeId || !QtJambiAPI::convertJavaToNative(__jni_env, *returnTypeId, result, ptr)){
                if(Java::Runtime::Map::isInstanceOf(__jni_env, result) || Java::Runtime::Collection::isInstanceOf(__jni_env, result)){
                    QVariant variant = CoreAPI::convertCheckedObjectToQVariant(__jni_env, result, *returnMetaType);
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
                    if(variant.metaType()==*returnMetaType || variant.convert(*returnMetaType)){
#else
                    if(variant.userType()==returnMetaType->id() || variant.convert(returnMetaType->id())){
#endif
                        returnMetaType->destruct(ptr);
                        returnMetaType->construct(ptr, variant.data());
                    }else{
                        returnMetaType->destroy(ptr);
                        Java::QtJambi::QUnsuccessfulInvocationException::throwNew(__jni_env, QString("Type %1 not supported.").arg(QtJambiAPI::getObjectClassName(__jni_env, result).replace(QLatin1Char('$'), QLatin1Char('.'))) QTJAMBI_STACKTRACEINFO );
                        return nullptr;
                    }
                }else{
                    returnMetaType->destroy(ptr);
                    Java::QtJambi::QUnsuccessfulInvocationException::throwNew(__jni_env, QString("Type %1 not supported.").arg(QtJambiAPI::getObjectClassName(__jni_env, result).replace(QLatin1Char('$'), QLatin1Char('.'))) QTJAMBI_STACKTRACEINFO );
                    return nullptr;
                }
            }
            if(returnPointerOrReference==0 && !isReferenceMetaType){
                switch(returnMetaType->sizeOf()){
                case sizeof(jbyte):{
                        jbyte data = 0;
                        returnMetaType->construct(&data, ptr);
                        returnMetaType->destroy(ptr);
                        return Java::Runtime::Byte::valueOf(__jni_env, data);
                    }
                case sizeof(jshort):{
                        jshort data = 0;
                        returnMetaType->construct(&data, ptr);
                        returnMetaType->destroy(ptr);
                        return Java::Runtime::Short::valueOf(__jni_env, data);
                    }
                case sizeof(jint):{
                        jint data = 0;
                        returnMetaType->construct(&data, ptr);
                        returnMetaType->destroy(ptr);
                        return Java::Runtime::Integer::valueOf(__jni_env, data);
                    }
                case sizeof(jlong):{
                        jlong data = 0;
                        returnMetaType->construct(&data, ptr);
                        returnMetaType->destroy(ptr);
                        return Java::Runtime::Long::valueOf(__jni_env, data);
                    }
                default:
                    returnMetaType->destroy(ptr);
                    Java::QtJambi::QUnsuccessfulInvocationException::throwNew(__jni_env, QString("Type %1 not supported.").arg(QtJambiAPI::getClassName(__jni_env, returnClassType).replace(QLatin1Char('$'), QLatin1Char('.'))) QTJAMBI_STACKTRACEINFO );
                }
            }else{
                return Java::JNA::Pointer::newInstance(__jni_env, ptr);
                //cleaners.append(QSharedDataPointer<Cleanup>{new Cleanup{[returnMetaType, ptr](){ returnMetaType->destroy(ptr); }}});
            }
        }
    }else{
        if(QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForJavaInterface(__jni_env, result)){
            if(PointerToObjectLink* plink = dynamic_cast<PointerToObjectLink*>(link.get())){
                returnMetaType = &plink->metaType();
                if(returnMetaType->isValid()){
                    switch(returnMetaType->sizeOf()){
                    case sizeof(jbyte):{
                            jbyte data = 0;
                            returnMetaType->construct(&data, plink->pointer());
                            return Java::Runtime::Byte::valueOf(__jni_env, data);
                        }
                    case sizeof(jshort):{
                            jshort data = 0;
                            returnMetaType->construct(&data, plink->pointer());
                            return Java::Runtime::Short::valueOf(__jni_env, data);
                        }
                    case sizeof(jint):{
                            jint data = 0;
                            returnMetaType->construct(&data, plink->pointer());
                            return Java::Runtime::Integer::valueOf(__jni_env, data);
                        }
                    case sizeof(jlong):{
                            jlong data = 0;
                            returnMetaType->construct(&data, plink->pointer());
                            return Java::Runtime::Long::valueOf(__jni_env, data);
                        }
                    default:
                        Java::QtJambi::QUnsuccessfulInvocationException::throwNew(__jni_env, QString("Type %1 not supported.").arg(QtJambiAPI::getClassName(__jni_env, returnClassType).replace(QLatin1Char('$'), QLatin1Char('.'))) QTJAMBI_STACKTRACEINFO );
                    }
                }else{
                    Java::QtJambi::QUnsuccessfulInvocationException::throwNew(__jni_env, QString("Type %1 not supported.").arg(QtJambiAPI::getClassName(__jni_env, returnClassType).replace(QLatin1Char('$'), QLatin1Char('.'))) QTJAMBI_STACKTRACEINFO );
                }
            }
        }else{
            Java::QtJambi::QUnsuccessfulInvocationException::throwNew(__jni_env, QString("Type %1 not supported.").arg(QtJambiAPI::getClassName(__jni_env, returnClassType).replace(QLatin1Char('$'), QLatin1Char('.'))) QTJAMBI_STACKTRACEINFO );
        }
    }
    return result;
}

void CoreAPI::getFunctionPointerParameterTypes(JNIEnv * __jni_env, jobjectArray argumentTypes, jobjectArray parameterTypes, jboolean){
    jsize length = __jni_env->GetArrayLength(argumentTypes);
    for(jsize i = 0; i<length; ++i){
        jobject argType = __jni_env->GetObjectArrayElement(argumentTypes, i);
        jint argPointerOrReference = 0;
        const QMetaType* argMetaType = nullptr;
        jclass argClassType = nullptr;
        bool isReferenceMetaType = false;
        QScopedPointer<const QMetaType> _argMetaType;
        argMetaType = qtjambi_cast<QMetaType*>(__jni_env, Java::QtCore::QGenericArgumentType::metaType(__jni_env, argType));
        if(argMetaType)
            isReferenceMetaType = QByteArray(argMetaType->name()).endsWith("&");
        argPointerOrReference = Java::QtCore::QGenericArgumentType::pointerOrReference(__jni_env, argType);
        argClassType = Java::QtCore::QGenericArgumentType::classType(__jni_env, argType);
        if(argMetaType && argMetaType->id()==QMetaType::Nullptr){
            __jni_env->SetObjectArrayElement(parameterTypes, i, Java::JNA::Pointer::getClass(__jni_env));
            continue;
        }
        if(argPointerOrReference==2 && argClassType){
            __jni_env->SetObjectArrayElement(parameterTypes, i, Java::JNA::Pointer::getClass(__jni_env));
            continue;
        }
        if(Java::Runtime::String::isSameClass(__jni_env, argClassType)){
            if(argMetaType){
                if(argMetaType->id()==QMetaType::Char && argPointerOrReference==-2){
                    __jni_env->SetObjectArrayElement(parameterTypes, i, Java::JNA::Pointer::getClass(__jni_env));
                    continue;
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
                }else if(argMetaType->id()==QMetaType::Char16 && argPointerOrReference==-2){
                    __jni_env->SetObjectArrayElement(parameterTypes, i, Java::JNA::Pointer::getClass(__jni_env));
                    continue;
#endif
                }
            }
            if(argPointerOrReference==0){
                Java::QtJambi::QUnsuccessfulInvocationException::throwNew(__jni_env, QString("Type %1 not supported without pointer or reference arithmetic.").arg(QtJambiAPI::getClassName(__jni_env, argClassType).replace(QLatin1Char('$'), QLatin1Char('.'))) QTJAMBI_STACKTRACEINFO );
            }else{
                __jni_env->SetObjectArrayElement(parameterTypes, i, Java::JNA::Pointer::getClass(__jni_env));
            }
            continue;
        }else if(Java::Runtime::Class::isArray(__jni_env, argClassType)
                 && Java::Runtime::String::isSameClass(__jni_env, Java::Runtime::Class::getComponentType(__jni_env, argClassType))
                 && (!argMetaType || QByteArray("QString&")==argMetaType->name() || QByteArray("QString &")==argMetaType->name()
                                  || QByteArray("QString*")==argMetaType->name() || QByteArray("QString *")==argMetaType->name()
                                  || QByteArray("const char**")==argMetaType->name())){
            __jni_env->SetObjectArrayElement(parameterTypes, i, Java::JNA::Pointer::getClass(__jni_env));
            continue;
        }else if(argPointerOrReference==0 && !isReferenceMetaType && Java::QtJambi::QFlags::isAssignableFrom(__jni_env, argClassType)){
            __jni_env->SetObjectArrayElement(parameterTypes, i, Java::Runtime::Integer::getClass(__jni_env));
            continue;
        }else if(argPointerOrReference==0 && !isReferenceMetaType && Java::QtJambi::QtEnumerator::isAssignableFrom(__jni_env, argClassType)){
            __jni_env->SetObjectArrayElement(parameterTypes, i, Java::Runtime::Integer::getClass(__jni_env));
            continue;
        }else if(argPointerOrReference==0 && !isReferenceMetaType && Java::QtJambi::QtByteEnumerator::isAssignableFrom(__jni_env, argClassType)){
            __jni_env->SetObjectArrayElement(parameterTypes, i, Java::Runtime::Byte::getClass(__jni_env));
            continue;
        }else if(argPointerOrReference==0 && !isReferenceMetaType && Java::QtJambi::QtShortEnumerator::isAssignableFrom(__jni_env, argClassType)){
            __jni_env->SetObjectArrayElement(parameterTypes, i, Java::Runtime::Short::getClass(__jni_env));
            continue;
        }else if(argPointerOrReference==0 && !isReferenceMetaType && Java::QtJambi::QtLongEnumerator::isAssignableFrom(__jni_env, argClassType)){
            __jni_env->SetObjectArrayElement(parameterTypes, i, Java::Runtime::Long::getClass(__jni_env));
            continue;
        }else if((argPointerOrReference==0
                  && !isReferenceMetaType
                  && (Java::Runtime::Byte::isAssignableFrom(__jni_env, argClassType)
                   || Java::Runtime::Short::isAssignableFrom(__jni_env, argClassType)
                   || Java::Runtime::Integer::isAssignableFrom(__jni_env, argClassType)
                   || Java::Runtime::Long::isAssignableFrom(__jni_env, argClassType)
                   || Java::Runtime::Float::isAssignableFrom(__jni_env, argClassType)
                   || Java::Runtime::Double::isAssignableFrom(__jni_env, argClassType)
                   || Java::Runtime::Character::isAssignableFrom(__jni_env, argClassType)
                   || Java::Runtime::Boolean::isAssignableFrom(__jni_env, argClassType)
                   || Java::Runtime::Byte::isPrimitiveType(__jni_env, argClassType)
                   || Java::Runtime::Short::isPrimitiveType(__jni_env, argClassType)
                   || Java::Runtime::Integer::isPrimitiveType(__jni_env, argClassType)
                   || Java::Runtime::Long::isPrimitiveType(__jni_env, argClassType)
                   || Java::Runtime::Float::isPrimitiveType(__jni_env, argClassType)
                   || Java::Runtime::Double::isPrimitiveType(__jni_env, argClassType)
                   || Java::Runtime::Character::isPrimitiveType(__jni_env, argClassType)
                   || Java::Runtime::Boolean::isPrimitiveType(__jni_env, argClassType)
                   || Java::JNA::Pointer::isAssignableFrom(__jni_env, argClassType)
                   || Java::JNA::Structure::isAssignableFrom(__jni_env, argClassType)))
                ||
                 (argClassType
                 && Java::Runtime::Class::isArray(__jni_env, argClassType)
                 && Java::Runtime::Class::isPrimitive(__jni_env, Java::Runtime::Class::getComponentType(__jni_env, argClassType)))){
            __jni_env->SetObjectArrayElement(parameterTypes, i, argClassType);
            continue;
        }else{
            const std::type_info* typeId = nullptr;
            if(argPointerOrReference==0
                     && !isReferenceMetaType
                     && Java::Runtime::Class::isArray(__jni_env, argClassType)){
                jclass _argClassType = argClassType;
                jclass argClassType = Java::Runtime::Class::getComponentType(__jni_env, _argClassType);
                const QMetaType* argMetaType = nullptr;
                QScopedPointer<const QMetaType> _argMetaType;
                if(!argMetaType)
                    _argMetaType.reset(argMetaType = new QMetaType(registerMetaType(__jni_env, argClassType, false, false)));
                if(!argMetaType->isValid())
                    _argMetaType.reset(argMetaType = nullptr);

                {
                    const SuperTypeInfos& infos = SuperTypeInfos::fromClass(__jni_env, argClassType);
                    if(infos.isEmpty()){
                        if(argMetaType){
                            typeId = getTypeByMetaType(*argMetaType);
                        }else{
                            Java::QtJambi::QUnsuccessfulInvocationException::throwNew(__jni_env, QString("Type %1 not supported.").arg(QtJambiAPI::getClassName(__jni_env, argClassType).replace(QLatin1Char('$'), QLatin1Char('.'))) QTJAMBI_STACKTRACEINFO );
                        }
                    }else{
                        typeId = &infos.first().typeId();
                    }
                }
                if(typeId){
                    bool isValueFunctional = false;
                    switch(getEntryType(*typeId)){
                    case EntryTypes::FunctionalTypeInfo:
                        isValueFunctional = true;
                        if(OptionalBool op = isRegisteredAsPointerType(*typeId)){
                            if(op.value()){
                                __jni_env->SetObjectArrayElement(parameterTypes, i, Java::JNA::Pointer::getClass(__jni_env));
                                continue;
                            }
                        }
                        Q_FALLTHROUGH();
                    case EntryTypes::ObjectTypeInfo:
                    case EntryTypes::InterfaceTypeInfo:
                    case EntryTypes::QObjectTypeInfo:
                        if(!isValueFunctional){
                            __jni_env->SetObjectArrayElement(parameterTypes, i, Java::JNA::Pointer::getClass(__jni_env));
                            continue;
                        }
                        Q_FALLTHROUGH();
                    case EntryTypes::InterfaceValueTypeInfo:
                    case EntryTypes::ValueTypeInfo:
                    case EntryTypes::FlagsTypeInfo:
                    case EntryTypes::SpecialTypeInfo:
                    case EntryTypes::StringTypeInfo:
                    case EntryTypes::PrimitiveTypeInfo:
                        if(!argMetaType)
                            _argMetaType.reset(argMetaType = new QMetaType(registeredMetaTypeID(*typeId)));
                        if(!argMetaType->isValid())
                            _argMetaType.reset(argMetaType = nullptr);
                        if(!argMetaType)
                            Java::QtJambi::QUnsuccessfulInvocationException::throwNew(__jni_env, QString("Type %1 not supported.").arg(QtJambiAPI::getClassName(__jni_env, argClassType).replace(QLatin1Char('$'), QLatin1Char('.'))) QTJAMBI_STACKTRACEINFO );
                        __jni_env->SetObjectArrayElement(parameterTypes, i, Java::JNA::Pointer::getClass(__jni_env));
                        continue;
                    default:
                        break;
                    }
                }
                if(argMetaType){
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
                    if(argMetaType->name().endsWith("*")
                            || argMetaType->name().contains("(*)")
                            || argMetaType->name().contains("(__cdecl*)")){
#else
                    if(argMetaType->flags() & QMetaType::IsPointer){
#endif
                        __jni_env->SetObjectArrayElement(parameterTypes, i, Java::JNA::Pointer::getClass(__jni_env));
                    }else{
                        if(!argMetaType)
                            _argMetaType.reset(argMetaType = new QMetaType(registeredMetaTypeID(*typeId)));
                        if(!argMetaType->isValid())
                            _argMetaType.reset(argMetaType = nullptr);
                        if(!argMetaType)
                            Java::QtJambi::QUnsuccessfulInvocationException::throwNew(__jni_env, QString("Type %1 not supported.").arg(QtJambiAPI::getClassName(__jni_env, argClassType).replace(QLatin1Char('$'), QLatin1Char('.'))) QTJAMBI_STACKTRACEINFO );
                        __jni_env->SetObjectArrayElement(parameterTypes, i, Java::JNA::Pointer::getClass(__jni_env));
                    }
                    continue;
                }else{
                    Java::QtJambi::QUnsuccessfulInvocationException::throwNew(__jni_env, QString("Type %1 not supported.").arg(QtJambiAPI::getClassName(__jni_env, argClassType).replace(QLatin1Char('$'), QLatin1Char('.'))) QTJAMBI_STACKTRACEINFO );
                }
            }else{
                size_t size = 0;
                {
                    const SuperTypeInfos& infos = SuperTypeInfos::fromClass(__jni_env, argClassType);
                    if(infos.isEmpty()){
                        if(argMetaType){
                            typeId = getTypeByMetaType(*argMetaType);
                            size = size_t(argMetaType->sizeOf());
                        }
                    }else{
                        typeId = &infos.first().typeId();
                        size = infos.first().size();
                    }
                }
                if(typeId){
                    bool isValueFunctional = false;
                    switch(getEntryType(*typeId)){
                    case EntryTypes::FunctionalTypeInfo:
                        isValueFunctional = true;
                        if(OptionalBool op = isRegisteredAsPointerType(*typeId)){
                            if(op.value()){
                                __jni_env->SetObjectArrayElement(parameterTypes, i, Java::JNA::Pointer::getClass(__jni_env));
                                continue;
                            }
                        }
                        Q_FALLTHROUGH();
                    case EntryTypes::ObjectTypeInfo:
                    case EntryTypes::InterfaceTypeInfo:
                    case EntryTypes::QObjectTypeInfo:
                        if(!isValueFunctional){
                            __jni_env->SetObjectArrayElement(parameterTypes, i, Java::JNA::Pointer::getClass(__jni_env));
                            continue;
                        }
                        Q_FALLTHROUGH();
                    case EntryTypes::InterfaceValueTypeInfo:
                    case EntryTypes::ValueTypeInfo:
                        if(!argMetaType)
                            _argMetaType.reset(argMetaType = new QMetaType(registeredMetaTypeID(*typeId)));
                        if(!argMetaType->isValid())
                            _argMetaType.reset(argMetaType = nullptr);
                        if(argMetaType){
                            if(argPointerOrReference==0 && !isReferenceMetaType){
                                switch(argMetaType->sizeOf()){
                                case sizeof(jbyte):
                                    __jni_env->SetObjectArrayElement(parameterTypes, i, Java::Runtime::Byte::primitiveType(__jni_env));
                                    break;
                                case sizeof(jshort):
                                    __jni_env->SetObjectArrayElement(parameterTypes, i, Java::Runtime::Short::primitiveType(__jni_env));
                                    break;
                                case sizeof(jint):
                                    __jni_env->SetObjectArrayElement(parameterTypes, i, Java::Runtime::Integer::primitiveType(__jni_env));
                                    break;
                                case sizeof(jlong):
                                    __jni_env->SetObjectArrayElement(parameterTypes, i, Java::Runtime::Long::primitiveType(__jni_env));
                                    break;
                                default:
                                    Java::QtJambi::QUnsuccessfulInvocationException::throwNew(__jni_env, QString("Type %1 not supported without pointer or reference arithmetic.").arg(QtJambiAPI::getClassName(__jni_env, argClassType).replace(QLatin1Char('$'), QLatin1Char('.'))) QTJAMBI_STACKTRACEINFO );
                                    break;
                                }
                                continue;
                            }else{
                                __jni_env->SetObjectArrayElement(parameterTypes, i, Java::JNA::Pointer::getClass(__jni_env));
                                continue;
                            }
                        }else{
                            Java::QtJambi::QUnsuccessfulInvocationException::throwNew(__jni_env, QString("Type %1 not supported.").arg(QtJambiAPI::getClassName(__jni_env, argClassType).replace(QLatin1Char('$'), QLatin1Char('.'))) QTJAMBI_STACKTRACEINFO );
                        }
                    break;
                    case EntryTypes::FlagsTypeInfo:
                    case EntryTypes::SpecialTypeInfo:
                    case EntryTypes::StringTypeInfo:
                    case EntryTypes::PrimitiveTypeInfo:
                        {
                            if(!argMetaType)
                                _argMetaType.reset(argMetaType = new QMetaType(registeredMetaTypeID(*typeId)));
                            if(!argMetaType->isValid())
                                _argMetaType.reset(argMetaType = nullptr);
                            if(argPointerOrReference==0 && !isReferenceMetaType){
                                switch(argMetaType->sizeOf()){
                                case sizeof(jbyte):
                                    __jni_env->SetObjectArrayElement(parameterTypes, i, Java::Runtime::Byte::primitiveType(__jni_env));
                                    break;
                                case sizeof(jshort):
                                    __jni_env->SetObjectArrayElement(parameterTypes, i, Java::Runtime::Short::primitiveType(__jni_env));
                                    break;
                                case sizeof(jint):
                                    __jni_env->SetObjectArrayElement(parameterTypes, i, Java::Runtime::Integer::primitiveType(__jni_env));
                                    break;
                                case sizeof(jlong):
                                    __jni_env->SetObjectArrayElement(parameterTypes, i, Java::Runtime::Long::primitiveType(__jni_env));
                                    break;
                                default:
                                    Java::QtJambi::QUnsuccessfulInvocationException::throwNew(__jni_env, QString("Type %1 not supported without pointer or reference arithmetic.").arg(QtJambiAPI::getClassName(__jni_env, argClassType).replace(QLatin1Char('$'), QLatin1Char('.'))) QTJAMBI_STACKTRACEINFO );
                                    break;
                                }
                                continue;
                            }else{
                                if(size>0){
                                    if(argMetaType){
                                        __jni_env->SetObjectArrayElement(parameterTypes, i, Java::JNA::Pointer::getClass(__jni_env));
                                        continue;
                                    }else{
                                        if(deleter(*typeId)!=nullptr){
                                            __jni_env->SetObjectArrayElement(parameterTypes, i, Java::JNA::Pointer::getClass(__jni_env));
                                            continue;
                                        }else{
                                            Java::QtJambi::QUnsuccessfulInvocationException::throwNew(__jni_env, QString("Type %1 not supported.").arg(QtJambiAPI::getClassName(__jni_env, argClassType).replace(QLatin1Char('$'), QLatin1Char('.'))) QTJAMBI_STACKTRACEINFO );
                                        }
                                    }
                                }else{
                                    Java::QtJambi::QUnsuccessfulInvocationException::throwNew(__jni_env, QString("Type %1 not supported.").arg(QtJambiAPI::getClassName(__jni_env, argClassType).replace(QLatin1Char('$'), QLatin1Char('.'))) QTJAMBI_STACKTRACEINFO );
                                }
                            }
                        }
                        break;
                    default:
                        break;
                    }
                }
                if(argMetaType){
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
                    if(argMetaType->name().endsWith("*")
                            || argMetaType->name().contains("(*)")
                            || argMetaType->name().contains("(__cdecl*)")){
#else
                    if(argMetaType->flags() & QMetaType::IsPointer){
#endif
                        __jni_env->SetObjectArrayElement(parameterTypes, i, Java::JNA::Pointer::getClass(__jni_env));
                    }else{
                        if(argPointerOrReference==0 && !isReferenceMetaType){
                            switch(argMetaType->sizeOf()){
                            case sizeof(jbyte):
                                __jni_env->SetObjectArrayElement(parameterTypes, i, Java::Runtime::Byte::primitiveType(__jni_env));
                                break;
                            case sizeof(jshort):
                                __jni_env->SetObjectArrayElement(parameterTypes, i, Java::Runtime::Short::primitiveType(__jni_env));
                                break;
                            case sizeof(jint):
                                __jni_env->SetObjectArrayElement(parameterTypes, i, Java::Runtime::Integer::primitiveType(__jni_env));
                                break;
                            case sizeof(jlong):
                                __jni_env->SetObjectArrayElement(parameterTypes, i, Java::Runtime::Long::primitiveType(__jni_env));
                                break;
                            default:
                                Java::QtJambi::QUnsuccessfulInvocationException::throwNew(__jni_env, QString("Type %1 not supported without pointer or reference arithmetic.").arg(QtJambiAPI::getClassName(__jni_env, argClassType).replace(QLatin1Char('$'), QLatin1Char('.'))) QTJAMBI_STACKTRACEINFO );
                                break;
                            }
                        }else{
                            __jni_env->SetObjectArrayElement(parameterTypes, i, Java::JNA::Pointer::getClass(__jni_env));
                        }
                    }
                }else if(argPointerOrReference!=0 || isReferenceMetaType){
                    __jni_env->SetObjectArrayElement(parameterTypes, i, Java::JNA::Pointer::getClass(__jni_env));
                }else{
                    Java::QtJambi::QUnsuccessfulInvocationException::throwNew(__jni_env, QString("Type %1 not supported without pointer or reference arithmetic.").arg(QtJambiAPI::getClassName(__jni_env, argClassType).replace(QLatin1Char('$'), QLatin1Char('.'))) QTJAMBI_STACKTRACEINFO );
                }
                continue;
            }// all others are allowed
        }
        __jni_env->SetObjectArrayElement(parameterTypes, i, argClassType);
    }
}

void CoreAPI::convertFunctionPointerParameters(JNIEnv * __jni_env, jobjectArray argumentTypes, jobjectArray args, jobjectArray convertedArgs, jboolean isVarArgs, jboolean forward){
    jsize length = __jni_env->GetArrayLength(argumentTypes);
    jsize varArgLength = isVarArgs ? __jni_env->GetArrayLength(args) : length;
    jobjectArray varArgument = nullptr;
    jsize j = 0;
    for(jsize i = 0; i<varArgLength; ++i){
        jobject argType = __jni_env->GetObjectArrayElement(argumentTypes, qMin(i, length-1));
        QScopedPointer<const QMetaType> _argMetaType;
        const QMetaType* argMetaType = qtjambi_cast<QMetaType*>(__jni_env, Java::QtCore::QGenericArgumentType::metaType(__jni_env, argType));
        bool isReferenceMetaType = false;
        if(argMetaType)
            isReferenceMetaType = QByteArray(argMetaType->name()).endsWith("&");
        jint argPointerOrReference = Java::QtCore::QGenericArgumentType::pointerOrReference(__jni_env, argType);
        jclass argClassType = Java::QtCore::QGenericArgumentType::classType(__jni_env, argType);
        jobject oldValue = __jni_env->GetObjectArrayElement(args, i);

        jobject convertedValue = nullptr;
        const std::type_info* typeId = nullptr;
        bool isFunctionPointer = false;
        const SuperTypeInfos& infos = SuperTypeInfos::fromClass(__jni_env, argClassType);
        if(infos.isEmpty()){
            if(!argMetaType)
                _argMetaType.reset(argMetaType = new QMetaType(registerMetaType(__jni_env, argClassType, false, false)));
            if(!argMetaType->isValid())
                _argMetaType.reset(argMetaType = nullptr);
            if(argMetaType){
                typeId = getTypeByMetaType(*argMetaType);
            }
        }else{
            typeId = &infos.first().typeId();
        }
        bool isPrimitive = false;
        if(typeId){
            isPrimitive = getEntryType(*typeId)==EntryTypes::PrimitiveTypeInfo;
            switch(getEntryType(*typeId)){
            case EntryTypes::FunctionalTypeInfo:
                if(OptionalBool op = isRegisteredAsPointerType(*typeId)){
                    if(op.value()){
                        isFunctionPointer = true;
                    }
                }
                break;
            case EntryTypes::InterfaceValueTypeInfo:
            case EntryTypes::ValueTypeInfo:
            case EntryTypes::FlagsTypeInfo:
            case EntryTypes::SpecialTypeInfo:
            case EntryTypes::StringTypeInfo:
            case EntryTypes::PrimitiveTypeInfo:
                if(!argMetaType)
                    _argMetaType.reset(argMetaType = new QMetaType(registeredMetaTypeID(*typeId)));
                if(!argMetaType->isValid())
                    _argMetaType.reset(argMetaType = nullptr);
                break;
            default: break;
            }
        }else if(argMetaType){
            isPrimitive = argMetaType->id()==QMetaType::Int
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
                    || argMetaType->id()==QMetaType::Char16
                    || argMetaType->id()==QMetaType::Char32
#endif
                    || argMetaType->id()==QMetaType::Char
                    || argMetaType->id()==QMetaType::LongLong
                    || argMetaType->id()==QMetaType::Short
                    || argMetaType->id()==QMetaType::UInt
                    || argMetaType->id()==QMetaType::UChar
                    || argMetaType->id()==QMetaType::SChar
                    || argMetaType->id()==QMetaType::ULongLong
                    || argMetaType->id()==QMetaType::UShort
                    || argMetaType->id()==QMetaType::Bool
                    || argMetaType->id()==QMetaType::QChar;
        }
        if(forward){
            if(!__jni_env->IsSameObject(oldValue, nullptr)){
                if(Java::JNA::Pointer::isInstanceOf(__jni_env, oldValue)){
                    void* ptr = reinterpret_cast<void*>(Java::JNA::Pointer::peer(__jni_env, oldValue));
                    if(argPointerOrReference==2 && argClassType){
                        QFunctionPointer fp = reinterpret_cast<QFunctionPointer>(ptr);
                        bool resolved = false;
                        {
                            QReadLocker locker(gPointerLock());
                            if(gObjectsByFunctionPointer->contains(quintptr(fp))){
                                QList<JObjectWrapper>& objects = (*gObjectsByFunctionPointer())[quintptr(fp)];
                                for(JObjectWrapper& wrapper : objects){
                                    if(__jni_env->IsInstanceOf(wrapper.object(), argClassType)){
                                        convertedValue = __jni_env->NewLocalRef(wrapper.object());
                                        resolved = true;
                                    }
                                }
                            }
                        }
                        if(!resolved){
                            convertedValue = Java::QtCore::QFunctionPointerUtil::createProxy(__jni_env, argClassType);
                            QtJambiLink::createLinkForNativeObject(
                                    __jni_env,
                                    convertedValue,
                                    new QFunctionPointer(fp),
                                    LINK_NAME_ARG(nullptr)
                                    false,
                                    false,
                                    &delete_callback_pointer,
                                    QtJambiLink::Ownership::Java
                                );
                            QWriteLocker locker(gPointerLock());
                            (*gObjectsByFunctionPointer())[quintptr(fp)] << JObjectWrapper(__jni_env, convertedValue);
                        }
                    }else{
                        void* _ptr = ptr;
                        if(isFunctionPointer){
                            _ptr = &ptr;
                        }
                        if(typeId){
                            convertedValue = internal_convertNativeToJavaObject(__jni_env, _ptr, *typeId, nullptr, isFunctionPointer || (!isReferenceMetaType && (argPointerOrReference==0 || argPointerOrReference==-2)) ? NativeToJavaConversionMode::MakeCopyOfValues : NativeToJavaConversionMode::None);
                        }else {
                            if(isFunctionPointer || (!isReferenceMetaType && (argPointerOrReference==0 || argPointerOrReference==-2))){
                                convertedValue = QtJambiAPI::convertNativeToJavaObjectAsCopy(__jni_env, _ptr, argClassType);
                            }else{
                                convertedValue = QtJambiAPI::convertNativeToJavaObjectAsWrapper(__jni_env, _ptr, argClassType);
                            }
                        }
                    }
                }else if(Java::QtJambi::QtEnumerator::isAssignableFrom(__jni_env, argClassType)){
                    if(Java::Runtime::Number::isInstanceOf(__jni_env, oldValue)){
                        convertedValue = QtJambiAPI::convertEnumToJavaObject(__jni_env, qint32(Java::Runtime::Number::intValue(__jni_env, oldValue)), argClassType);
                    }else if(Java::JNA::Pointer::isInstanceOf(__jni_env, oldValue)){
                        qint32* ptr = reinterpret_cast<qint32*>(Java::JNA::Pointer::peer(__jni_env, oldValue));
                        convertedValue = QtJambiAPI::convertEnumToJavaObject(__jni_env, *ptr, argClassType);
                    }else
                        convertedValue = nullptr;
                }else if(Java::QtJambi::QFlags::isAssignableFrom(__jni_env, argClassType)){
                    if(Java::Runtime::Number::isInstanceOf(__jni_env, oldValue)){
                        convertedValue = QtJambiAPI::convertQFlagsToJavaObject(__jni_env, qint32(Java::Runtime::Number::intValue(__jni_env, oldValue)), argClassType);
                    }else if(Java::JNA::Pointer::isInstanceOf(__jni_env, oldValue)){
                        qint32* ptr = reinterpret_cast<qint32*>(Java::JNA::Pointer::peer(__jni_env, oldValue));
                        convertedValue = QtJambiAPI::convertQFlagsToJavaObject(__jni_env, *ptr, argClassType);
                    }else
                        convertedValue = nullptr;
                }else if(Java::QtJambi::QtShortEnumerator::isAssignableFrom(__jni_env, argClassType)){
                    if(Java::Runtime::Number::isInstanceOf(__jni_env, oldValue)){
                        convertedValue = QtJambiAPI::convertEnumToJavaObject(__jni_env, qint16(Java::Runtime::Number::shortValue(__jni_env, oldValue)), argClassType);
                    }else if(Java::JNA::Pointer::isInstanceOf(__jni_env, oldValue)){
                        qint16* ptr = reinterpret_cast<qint16*>(Java::JNA::Pointer::peer(__jni_env, oldValue));
                        convertedValue = QtJambiAPI::convertEnumToJavaObject(__jni_env, *ptr, argClassType);
                    }else
                        convertedValue = nullptr;
                }else if(Java::QtJambi::QtByteEnumerator::isAssignableFrom(__jni_env, argClassType)){
                    if(Java::Runtime::Number::isInstanceOf(__jni_env, oldValue)){
                        convertedValue = QtJambiAPI::convertEnumToJavaObject(__jni_env, qint8(Java::Runtime::Number::byteValue(__jni_env, oldValue)), argClassType);
                    }else if(Java::JNA::Pointer::isInstanceOf(__jni_env, oldValue)){
                        qint8* ptr = reinterpret_cast<qint8*>(Java::JNA::Pointer::peer(__jni_env, oldValue));
                        convertedValue = QtJambiAPI::convertEnumToJavaObject(__jni_env, *ptr, argClassType);
                    }else
                        convertedValue = nullptr;
                }else if(Java::QtJambi::QtLongEnumerator::isAssignableFrom(__jni_env, argClassType)){
                    if(Java::Runtime::Number::isInstanceOf(__jni_env, oldValue)){
                        convertedValue = QtJambiAPI::convertEnumToJavaObject(__jni_env, qint64(Java::Runtime::Number::longValue(__jni_env, oldValue)), argClassType);
                    }else if(Java::JNA::Pointer::isInstanceOf(__jni_env, oldValue)){
                        qint64* ptr = reinterpret_cast<qint64*>(Java::JNA::Pointer::peer(__jni_env, oldValue));
                        convertedValue = QtJambiAPI::convertEnumToJavaObject(__jni_env, *ptr, argClassType);
                    }else
                        convertedValue = nullptr;
                }else if(isPrimitive && (Java::Runtime::Byte::isInstanceOf(__jni_env, oldValue)
                                         || Java::Runtime::Short::isInstanceOf(__jni_env, oldValue)
                                         || Java::Runtime::Integer::isInstanceOf(__jni_env, oldValue)
                                         || Java::Runtime::Long::isInstanceOf(__jni_env, oldValue)
                                         || Java::Runtime::Float::isInstanceOf(__jni_env, oldValue)
                                         || Java::Runtime::Double::isInstanceOf(__jni_env, oldValue)
                                         || Java::Runtime::Boolean::isInstanceOf(__jni_env, oldValue)
                                         || Java::Runtime::Character::isInstanceOf(__jni_env, oldValue))){
                    convertedValue = oldValue;
                }else if(argMetaType){
                    if(QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForJavaInterface(__jni_env, oldValue)){
                        switch(argMetaType->sizeOf()){
                        case sizeof(jbyte):{
                                jbyte data = 0;
                                argMetaType->construct(&data, link->pointer());
                                convertedValue = Java::Runtime::Byte::valueOf(__jni_env, data);
                                break;
                            }
                        case sizeof(jshort):{
                                jshort data = 0;
                                argMetaType->construct(&data, link->pointer());
                                convertedValue = Java::Runtime::Short::valueOf(__jni_env, data);
                                break;
                            }
                        case sizeof(jint):{
                                jint data = 0;
                                argMetaType->construct(&data, link->pointer());
                                convertedValue = Java::Runtime::Integer::valueOf(__jni_env, data);
                                break;
                            }
                        case sizeof(jlong):{
                                jlong data = 0;
                                argMetaType->construct(&data, link->pointer());
                                convertedValue = Java::Runtime::Long::valueOf(__jni_env, data);
                                break;
                            }
                        default:
                            Java::QtJambi::QUnsuccessfulInvocationException::throwNew(__jni_env, QString("Type %1 not supported.").arg(QtJambiAPI::getObjectClassName(__jni_env, oldValue).replace(QLatin1Char('$'), QLatin1Char('.'))) QTJAMBI_STACKTRACEINFO );
                        }
                    }else{
                        QVariant variant = CoreAPI::convertCheckedObjectToQVariant(__jni_env, oldValue, *argMetaType);
    #if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
                        if(variant.metaType()==*argMetaType || variant.convert(*argMetaType)){
    #else
                        if(variant.userType()==argMetaType->id() || variant.convert(argMetaType->id())){
    #endif
                            switch(argMetaType->sizeOf()){
                            case sizeof(jbyte):{
                                    jbyte data = 0;
                                    argMetaType->construct(&data, variant.data());
                                    convertedValue = Java::Runtime::Byte::valueOf(__jni_env, data);
                                    break;
                                }
                            case sizeof(jshort):{
                                    jshort data = 0;
                                    argMetaType->construct(&data, variant.data());
                                    convertedValue = Java::Runtime::Short::valueOf(__jni_env, data);
                                    break;
                                }
                            case sizeof(jint):{
                                    jint data = 0;
                                    argMetaType->construct(&data, variant.data());
                                    convertedValue = Java::Runtime::Integer::valueOf(__jni_env, data);
                                    break;
                                }
                            case sizeof(jlong):{
                                    jlong data = 0;
                                    argMetaType->construct(&data, variant.data());
                                    convertedValue = Java::Runtime::Long::valueOf(__jni_env, data);
                                    break;
                                }
                            default:
                                Java::QtJambi::QUnsuccessfulInvocationException::throwNew(__jni_env, QString("Type %1 not supported.").arg(QtJambiAPI::getObjectClassName(__jni_env, oldValue).replace(QLatin1Char('$'), QLatin1Char('.'))) QTJAMBI_STACKTRACEINFO );
                            }
                        }else{
                            Java::QtJambi::QUnsuccessfulInvocationException::throwNew(__jni_env, QString("Type %1 not supported.").arg(QtJambiAPI::getObjectClassName(__jni_env, oldValue).replace(QLatin1Char('$'), QLatin1Char('.'))) QTJAMBI_STACKTRACEINFO );
                        }
                    }
                }else{
                    // missing: check primitives, primitive arrays and convert object arrays!
                    convertedValue = oldValue;
                }
            }
            if(i<length-1 || !isVarArgs){
                __jni_env->SetObjectArrayElement(convertedArgs, i+1, convertedValue);
            }else if(i==length-1){
                varArgument = __jni_env->NewObjectArray(varArgLength-i, argClassType, nullptr);
                __jni_env->SetObjectArrayElement(convertedArgs, i+1, varArgument);
                __jni_env->SetObjectArrayElement(varArgument, j++, convertedValue);
            }else{
                __jni_env->SetObjectArrayElement(varArgument, j++, convertedValue);
            }
        }
    }
}

void CoreAPI::disposeFunctionPointer(JNIEnv * __jni_env, jlong peer){
    Java::JNA::Native::freeNativeCallback(__jni_env, peer);
}

jobject CoreAPI::castFunctionPointer(JNIEnv * env, jobject function, jclass functionalInterface){
    if(env->IsInstanceOf(function, functionalInterface))
        return function;
    jclass sourceClass = env->GetObjectClass(function);
    const std::type_info* sourceTypeId = nullptr;
    const std::type_info* targetTypeId = nullptr;
    {
        const SuperTypeInfos& infos = SuperTypeInfos::fromClass(env, functionalInterface);
        if(infos.isEmpty()){
            targetTypeId = getTypeByJavaName(QtJambiAPI::getClassName(env, functionalInterface).replace('.', '/'));
        }else{
            targetTypeId = &infos.first().typeId();
        }
    }
    {
        const SuperTypeInfos& infos = SuperTypeInfos::fromClass(env, sourceClass);
        if(infos.isEmpty()){
            sourceTypeId = getTypeByJavaName(QtJambiAPI::getClassName(env, sourceClass).replace('.', '/'));
        }else{
            sourceTypeId = &infos.first().typeId();
        }
    }
    if(targetTypeId){
        if(isFunctional(*targetTypeId)){
            if(OptionalBool op = isRegisteredAsPointerType(*targetTypeId)){
                if(op.value()){
                    if(sourceTypeId){
                        if(isFunctional(*sourceTypeId)){
                            if(OptionalBool op = isRegisteredAsPointerType(*sourceTypeId)){
                                if(op.value()){
                                    if(QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForJavaInterface(env, function)){
                                        QFunctionPointer ptr = nullptr;
                                        if(QtJambiAPI::convertJavaToNative(env, *sourceTypeId, function, &ptr)){
                                            {
                                                QReadLocker locker(gPointerLock());
                                                if(gObjectsByFunctionPointer->contains(quintptr(ptr))){
                                                    QList<JObjectWrapper>& objects = (*gObjectsByFunctionPointer())[quintptr(ptr)];
                                                    for(JObjectWrapper& wrapper : objects){
                                                        if(env->IsInstanceOf(wrapper.object(), functionalInterface))
                                                            return env->NewLocalRef(wrapper.object());
                                                    }
                                                }
                                            }
                                            jobject result = QtJambiAPI::convertNativeToJavaObjectAsCopy(env, &ptr, *targetTypeId);
                                            QWriteLocker locker(gPointerLock());
                                            (*gObjectsByFunctionPointer())[quintptr(ptr)] << JObjectWrapper(env, result);
                                            return result;
                                        }else{
                                            Java::Runtime::ClassCastException::throwNew(env, QStringLiteral("Unable to convert object of type %1 to function pointer.").arg(QtJambiAPI::getObjectClassName(env, function)) QTJAMBI_STACKTRACEINFO );
                                            return nullptr;
                                        }
                                    }else{
                                        Java::QtJambi::QNoNativeResourcesException::throwNew(env, QStringLiteral("Incomplete object of type: %1").arg(QtJambiAPI::getClassName(env, sourceClass)) QTJAMBI_STACKTRACEINFO );
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    if(sourceTypeId){
        if(isFunctional(*sourceTypeId)){
            if(OptionalBool op = isRegisteredAsPointerType(*sourceTypeId)){
                if(op.value()){
                    if(QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForJavaInterface(env, function)){
                        if(env->IsInstanceOf(link->getJavaObjectLocalRef(env), functionalInterface)){
                            return function;
                        }else{
                            QFunctionPointer ptr = nullptr;
                            if(QtJambiAPI::convertJavaToNative(env, *sourceTypeId, function, &ptr) && ptr){
                                if(Java::QtCore::QFunctionPointer::isSameClass(env, functionalInterface)){
                                    return qtjambi_cast<jobject>(env, ptr, "QFunctionPointer");
                                }else{
                                    {
                                        QReadLocker locker(gPointerLock());
                                        if(gObjectsByFunctionPointer->contains(quintptr(ptr))){
                                            QList<JObjectWrapper>& objects = (*gObjectsByFunctionPointer())[quintptr(ptr)];
                                            for(JObjectWrapper& wrapper : objects){
                                                if(env->IsInstanceOf(wrapper.object(), functionalInterface))
                                                    return env->NewLocalRef(wrapper.object());
                                            }
                                        }
                                    }
                                    try{
                                        Java::JNA::Native::getClass(env);
                                    }catch(const JavaException& exn){
                                        JavaException exn2(env, Java::QtJambi::QUnsuccessfulInvocationException::newInstanceWithMessage(env, "Unable to load Java Native Access library. Please add JNA to class path."));
                                        exn2.addSuppressed(env, exn);
                                        exn2.raise();
                                    }
                                    jobject result = Java::QtCore::QFunctionPointerUtil::createProxy(env, functionalInterface);
                                    QtJambiLink::createLinkForNativeObject(
                                            env,
                                            result,
                                            new QFunctionPointer(ptr),
                                            LINK_NAME_ARG(nullptr)
                                            false,
                                            false,
                                            &delete_callback_pointer,
                                            QtJambiLink::Ownership::Java
                                        );
                                    QWriteLocker locker(gPointerLock());
                                    (*gObjectsByFunctionPointer())[quintptr(ptr)] << JObjectWrapper(env, result);
                                    return result;
                                }
                            }else{
                                Java::Runtime::ClassCastException::throwNew(env, QStringLiteral("Unable to convert object of type %1 to function pointer.").arg(QtJambiAPI::getObjectClassName(env, function)) QTJAMBI_STACKTRACEINFO );
                                return nullptr;
                            }
                        }
                    }else{
                        Java::QtJambi::QNoNativeResourcesException::throwNew(env, QStringLiteral("Incomplete object of type: %1").arg(QtJambiAPI::getClassName(env, sourceClass)) QTJAMBI_STACKTRACEINFO );
                    }
                }
            }
        }
    }
    if(QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForJavaInterface(env, function)){
        if(DeletablePointerToObjectLink* plink = dynamic_cast<DeletablePointerToObjectLink*>(link.get())){
            if(plink->deleterFunction()==&delete_callback_pointer){
                QFunctionPointer* ptr = nullptr;
                if(const QSharedPointer<QtJambiLink>& link = QtJambiLink::findLinkForJavaInterface(env, function)){
                    ptr = reinterpret_cast<QFunctionPointer*>(link->pointer());
                    if(ptr && *ptr){
                        if(Java::QtCore::QFunctionPointer::isSameClass(env, functionalInterface)){
                            return qtjambi_cast<jobject>(env, *ptr, "QFunctionPointer");
                        }else{
                            {
                                QReadLocker locker(gPointerLock());
                                if(gObjectsByFunctionPointer->contains(quintptr(*ptr))){
                                    QList<JObjectWrapper>& objects = (*gObjectsByFunctionPointer())[quintptr(*ptr)];
                                    for(JObjectWrapper& wrapper : objects){
                                        if(env->IsInstanceOf(wrapper.object(), functionalInterface))
                                            return env->NewLocalRef(wrapper.object());
                                    }
                                }
                            }
                            try{
                                Java::JNA::Native::getClass(env);
                            }catch(const JavaException& exn){
                                JavaException exn2(env, Java::QtJambi::QUnsuccessfulInvocationException::newInstanceWithMessage(env, "Unable to load Java Native Access library. Please add JNA to class path."));
                                exn2.addSuppressed(env, exn);
                                exn2.raise();
                            }
                            jobject result = Java::QtCore::QFunctionPointerUtil::createProxy(env, functionalInterface);
                            QtJambiLink::createLinkForNativeObject(
                                    env,
                                    result,
                                    new QFunctionPointer(*ptr),
                                    LINK_NAME_ARG(nullptr)
                                    false,
                                    false,
                                    &delete_callback_pointer,
                                    QtJambiLink::Ownership::Java
                                );
                            QWriteLocker locker(gPointerLock());
                            (*gObjectsByFunctionPointer())[quintptr(*ptr)] << JObjectWrapper(env, result);
                            return result;
                        }
                    }
                }else{
                    Java::Runtime::ClassCastException::throwNew(env, QStringLiteral("Unable to convert object of type %1 to function pointer.").arg(QtJambiAPI::getObjectClassName(env, function)) QTJAMBI_STACKTRACEINFO );
                    return nullptr;
                }
            }
        }
        Java::Runtime::IllegalArgumentException::throwNew(env, QStringLiteral("Object of type %1 is not a function pointer type.").arg(QtJambiAPI::getClassName(env, sourceClass).replace('$', '.')) QTJAMBI_STACKTRACEINFO );
    }else{
        Java::QtJambi::QNoNativeResourcesException::throwNew(env, QStringLiteral("Incomplete object of type: %1").arg(QtJambiAPI::getClassName(env, sourceClass)) QTJAMBI_STACKTRACEINFO );
    }
    return nullptr;
}

enum ffi_status {
  FFI_OK = 0,
  FFI_BAD_TYPEDEF,
  FFI_BAD_ABI,
  FFI_BAD_ARGTYPE
};

struct ffi_cif {
  jint abi;
  unsigned nargs;
  ffi_type **arg_types;
  ffi_type *rtype;
  unsigned bytes;
  unsigned flags;
};

void ffi_funtion_dispatch(JNIEnv *env, void* func, jobjectArray args, ffi_type *return_type, void *presult)
{
    Q_UNUSED(ffi_type_void)
    Q_UNUSED(ffi_type_quint8)
    Q_UNUSED(ffi_type_quint64)
    int i;
    jsize nargs;
    QScopedArrayPointer<jvalue> c_args;
    QScopedArrayPointer<ffi_type const*> arg_types;
    QScopedArrayPointer<void*> arg_values;
//  int fixed_args = (flags & USE_VARARGS) >> 7;

    nargs = env->GetArrayLength(args);

    if (nargs > MAX_NARGS) {
        //snprintf(msg, sizeof(msg), "Too many arguments (max %ld)", MAX_NARGS);
        //throwByName(env, EUnsupportedOperation, msg);
        return;
    }

    if(nargs>0){
        c_args.reset(new jvalue[size_t(nargs)]);
        arg_types.reset(new ffi_type const*[size_t(nargs)]);
        arg_values.reset(new void*[size_t(nargs)]);
    }

    for (i = 0; i < nargs; i++) {
        jobject arg = env->GetObjectArrayElement(args, i);

        if (arg == nullptr) {
            c_args[i].l = nullptr;
            arg_types[i] = &ffi_type_pointer;
            arg_values[i] = &c_args[i].l;
        }
        else if (Java::Runtime::Boolean::isInstanceOf(env, arg)) {
            c_args[i].i = Java::Runtime::Boolean::booleanValue(env, arg);
            arg_types[i] = &ffi_type_quint32;
            arg_values[i] = &c_args[i].i;
        }
        else if (Java::Runtime::Byte::isInstanceOf(env, arg)) {
            c_args[i].b = Java::Runtime::Number::byteValue(env, arg);
            arg_types[i] = &ffi_type_qint8;
            arg_values[i] = &c_args[i].b;
        }
        else if (Java::Runtime::Short::isInstanceOf(env, arg)) {
            c_args[i].s = Java::Runtime::Number::shortValue(env, arg);
            arg_types[i] = &ffi_type_qint16;
            arg_values[i] = &c_args[i].s;
        }
        else if (Java::Runtime::Character::isInstanceOf(env, arg)) {
            if (sizeof(wchar_t) == 2) {
                c_args[i].c = Java::Runtime::Character::charValue(env, arg);
                arg_types[i] = &ffi_type_quint16;
                arg_values[i] = &c_args[i].c;
            }
            else if (sizeof(wchar_t) == 4) {
                c_args[i].i = Java::Runtime::Character::charValue(env, arg);
                arg_types[i] = &ffi_type_quint32;
                arg_values[i] = &c_args[i].i;
            }
            else {
                Java::Runtime::UnsupportedOperationException::throwNew(env, QString("Unsupported wchar_t size (%1)").arg(sizeof(wchar_t)) QTJAMBI_STACKTRACEINFO );
                return;
            }
        }
        else if (Java::Runtime::Integer::isInstanceOf(env, arg)) {
            c_args[i].i = Java::Runtime::Number::intValue(env, arg);
            arg_types[i] = &ffi_type_qint32;
            arg_values[i] = &c_args[i].i;
        }
        else if (Java::Runtime::Long::isInstanceOf(env, arg)) {
            c_args[i].j = Java::Runtime::Number::longValue(env, arg);
            arg_types[i] = &ffi_type_qint64;
            arg_values[i] = &c_args[i].j;
        }
        else if (Java::Runtime::Float::isInstanceOf(env, arg)) {
            c_args[i].f = Java::Runtime::Number::floatValue(env, arg);
            arg_types[i] = &ffi_type_float;
            arg_values[i] = &c_args[i].f;
        }
        else if (Java::Runtime::Double::isInstanceOf(env, arg)) {
            c_args[i].d = Java::Runtime::Number::doubleValue(env, arg);
            arg_types[i] = &ffi_type_double;
            arg_values[i] = &c_args[i].d;
        }
        else if (Java::JNA::Pointer::isInstanceOf(env, arg)) {
            c_args[i].l = jobject(Java::JNA::Pointer::peer(env, arg));
            arg_types[i] = &ffi_type_pointer;
            arg_values[i] = &c_args[i].l;
        }
        else if (Java::Runtime::Buffer::isInstanceOf(env, arg)) {
            c_args[i].l = jobject(env->GetDirectBufferAddress(arg));
            arg_types[i] = &ffi_type_pointer;
            arg_values[i] = &c_args[i].l;
        /*
        if (c_args[i].l == nullptr) {
        c_args[i].l =
        getBufferArray(arg, &array_elements[array_count].array,
        &array_elements[array_count].elems,
        (void**)&array_elements[array_count].release);
        if (c_args[i].l == nullptr) {
        throw_type = EIllegalArgument;
        throw_msg = "Buffer arguments must be direct or have a primitive backing array";
        goto cleanup;
        }
        ++array_count;
        }*/
        }
        /*
        else if ((array_pt = getArrayComponentType(arg)) != 0
        && array_pt != 'L') {
        void *ptr = nullptr;
        release_t release = nullptr;

        #define GET_ELEMS(TYPE) do {ptr=env->Get##TYPE##ArrayElements(env,arg,nullptr); release=(void*)env->Release##TYPE##ArrayElements; }while(0)
        switch(array_pt) {
        case 'Z': GET_ELEMS(Boolean); break;
        case 'B': GET_ELEMS(Byte); break;
        case 'C': GET_ELEMS(Char); break;
        case 'S': GET_ELEMS(Short); break;
        case 'I': GET_ELEMS(Int); break;
        case 'J': GET_ELEMS(Long); break;
        case 'F': GET_ELEMS(Float); break;
        case 'D': GET_ELEMS(Double); break;
        }
        if (!ptr) {
        throw_type = EOutOfMemory;
        throw_msg = "Could not obtain memory for primitive buffer";
        goto cleanup;
        }
        c_args[i].l = ptr;
        arg_types[i] = &ffi_type_pointer;
        arg_values[i] = &c_args[i].l;
        array_elements[array_count].array = arg;
        array_elements[array_count].elems = ptr;
        array_elements[array_count++].release = release;
        }*/
        else {
            // Anything else, pass directly as a pointer
            c_args[i].l = jobject(arg);
            arg_types[i] = &ffi_type_pointer;
            arg_values[i] = &c_args[i].l;
        }
    }

  /*switch (callconv) {
  case CALLCONV_C:
    abi = FFI_DEFAULT_ABI;
    break;
#ifdef _WIN32
  case CALLCONV_STDCALL:
#if defined(_WIN64) || defined(_WIN32_WCE)
    // Ignore requests for stdcall on win64/wince
    abi = FFI_DEFAULT_ABI;
#else
    abi = FFI_STDCALL;
#endif
    break;
#endif // _WIN32
  default:
    abi = (int)callconv;
    if (!(abi > FFI_FIRST_ABI && abi < FFI_LAST_ABI)) {
      snprintf(msg, sizeof(msg),
               "Unrecognized calling convention: %d", abi);
      throw_type = EIllegalArgument;
      throw_msg = msg;
      goto cleanup;
    }
    break;
  }*/

    jlong cif = Java::JNA::Native::ffi_prep_cif(env, 0, nargs, return_type, arg_types.get());
    if(cif==0){
        Java::Runtime::RuntimeException::throwNew(env, "Error during native call setup." QTJAMBI_STACKTRACEINFO );
    }else{
        Java::JNA::Native::ffi_call(env, cif, func, presult, arg_values.get());
        Java::JNA::Native::free(env, cif);
    }
//  status =
//          fixed_args
//    ? ffi_prep_cif_var(&cif, abi, fixed_args, nargs, return_type, arg_types)
//    :
//  if (!ffi_error("Native call setup", status)) {
//    PSTART();
//    if ((flags & THROW_LAST_ERROR) != 0) {
//      SET_LAST_ERROR(0);
//    }
//    ffi_call(&cif, FFI_FN(func), presult, arg_values);
//    {
//      int err = GET_LAST_ERROR();
//      JNA_set_last_error(err);
//      if ((flags & THROW_LAST_ERROR) && err) {
//        char emsg[MSG_SIZE - 3 /* literal characters */ - 10 /* max length of %d */];
//        snprintf(msg, sizeof(msg), "[%d] %s", err, STR_ERROR(err, emsg, sizeof(emsg)));
//        throw_type = ELastError;
//        throw_msg = msg;
//      }
//    }

//    PROTECTED_END(do { throw_type=EError;throw_msg="Invalid memory access";} while(0));
//  }

// cleanup:

//  // Release array elements
//  for (i=0;i < array_count;i++) {
//    array_elements[i].release(array_elements[i].array,
//                              array_elements[i].elems, 0);
//  }

//  // Must raise any exception *after* all other JNI operations
//  if (throw_type) {
//    throwByName(throw_type, throw_msg);
//  }
}
