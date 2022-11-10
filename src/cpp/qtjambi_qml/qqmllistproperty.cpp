/****************************************************************************
**
** Copyright (C) 2009-2022 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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

#include <QtCore/QtGlobal>
#include <qtjambi/qtjambi_global.h>
#include <QtCore/QtCore>
#include <QtQml/QtQml>

#include "qqmllistproperty.h"
#include <qtjambi/qtjambi_core.h>
#include <qtjambi/qtjambi_application.h>
#include <qtjambi/qtjambi_qml.h>
#include <qtjambi/qtjambi_repository.h>
#include <qtjambi/qtjambi_templates.h>
#include <qtjambi/qtjambi_registry.h>
#include <qtjambi/qtjambi_cast.h>
#include <qtjambi/qtjambi_jobjectwrapper.h>
#include "qtjambi_qml_repository.h"

#ifndef QT_JAMBI_RUN
#include <QtQml/private/qqmllist_p.h>
#endif

// emitting (writeExtraFunctions)
// emitting (writeToStringFunction)
// emitting (writeCloneFunction)

extern "C" Q_DECL_EXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_qml_util_QtJambi_1LibraryUtilities_internalAccess)(JNIEnv *env, jclass cls){
    jobject result{0};
    QTJAMBI_TRY{
        result = qtjambi_get_internal_access(env, cls);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(env);
    }QTJAMBI_TRY_END
    return result;
}

void deleter_QQmlListProperty(void* ptr, bool){
    delete reinterpret_cast<QQmlListProperty<void> *>(ptr);
}

extern "C" Q_DECL_EXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_qml_QQmlListProperty_clone_1native)
(JNIEnv *__jni_env, jobject _this)
{
    jobject _result{nullptr};
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QQmlListProperty::clone(QQmlListProperty)")
    QTJAMBI_TRY{
        QQmlListProperty<void> *__qt_this = qtjambi_to_object<QQmlListProperty<void> >(__jni_env, _this);
        qtjambi_check_resource(__jni_env, __qt_this, typeid(QQmlListProperty<void>));
        _result = qtjambi_from_object(__jni_env, __qt_this, typeid(QQmlListProperty<void>), true);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
    return _result;
}

// new QQmlListProperty()
void __qt_create_new_QQmlListProperty_0(void* __qtjambi_ptr, JNIEnv*, jobject, jvalue*)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "new QQmlListProperty<void>()")
    QQmlListProperty<void> *__qt_this = new(__qtjambi_ptr) QQmlListProperty<void>();
    Q_UNUSED(__qt_this)
}

#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
typedef int list_int;
#else
typedef qsizetype list_int;
#endif

// new QQmlListProperty(QObject * o, QList<QObject > & list)
void __qt_create_new_QQmlListProperty_1(void* __qtjambi_ptr, JNIEnv* __jni_env, jobject, jvalue* __java_arguments)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "new QQmlListProperty<void>(QObject * o, QList<QObject > & list)")
    jobject o0 = __java_arguments[0].l;
    QList<void*>* list = qtjambi_to_object<QList<void*>>(__jni_env, __java_arguments[1].l);
    new(__qtjambi_ptr) QQmlListProperty<void>(qtjambi_to_qobject(__jni_env, o0), list);
}

struct FunctionUserData : QtJambiObjectData{
    struct Functions{
        JObjectWrapper appendFunction;
        JObjectWrapper countFunction;
        JObjectWrapper atFunction;
        JObjectWrapper clearFunction;
        JObjectWrapper replaceFunction;
        JObjectWrapper removeLastFunction;
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
        int metaType;
#else
        QMetaType metaType;
#endif
    };
    FunctionUserData() : QtJambiObjectData() {
    }

    QMap<int,Functions> functions;
    QTJAMBI_OBJECTUSERDATA_ID_IMPL(static,)

    static void append(QQmlListProperty<void> * list, void* element){
        jint hash = jint(qint64(list->data));
        FunctionUserData *functionData{nullptr};
        {
            QReadLocker locker(qtjambi_objectdata_lock());
            functionData = QTJAMBI_GET_OBJECTUSERDATA(FunctionUserData, list->object);
        }
        if(functionData && functionData->functions.contains(hash)){
            if(JNIEnv * env = qtjambi_current_environment()){
                QTJAMBI_JNI_LOCAL_FRAME(env, 200)
                jobject _object = qtjambi_from_QObject(env, list->object);
                if(jobject appendFunction = functionData->functions[hash].appendFunction.object()){
                    jobject _element;
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
                    if(QMetaType(functionData->functions[hash].metaType).flags() & QMetaType::PointerToQObject){
#else
                    if(functionData->functions[hash].metaType.flags() & QMetaType::PointerToQObject){
#endif
                        _element = qtjambi_from_QObject(env, reinterpret_cast<QObject*>(element));
                    }else{
                        _element = qtjambi_from_qvariant(env, QVariant(functionData->functions[hash].metaType, &element));
                    }
                    Java::Runtime::BiConsumer::accept(env, appendFunction, _object, _element);
                }
            }
        }
    }

    static void* at(QQmlListProperty<void> * list, list_int index) {
        jint hash = jint(qint64(list->data));
        FunctionUserData *functionData{nullptr};
        {
            QReadLocker locker(qtjambi_objectdata_lock());
            functionData = QTJAMBI_GET_OBJECTUSERDATA(FunctionUserData, list->object);
        }
        if(functionData && functionData->functions.contains(hash)){
            if(JNIEnv * env = qtjambi_current_environment()){
                QTJAMBI_JNI_LOCAL_FRAME(env, 200)
                jobject _object = qtjambi_from_QObject(env, list->object);
                if(jobject atFunction = functionData->functions[hash].atFunction.object()){
                    jobject result = Java::QtQml::QQmlListProperty$AtFunction::apply(env, atFunction, _object, jlong(index));
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
                    if(QMetaType(functionData->functions[hash].metaType).flags() & QMetaType::PointerToQObject){
#else
                    if(functionData->functions[hash].metaType.flags() & QMetaType::PointerToQObject){
#endif
                        return qtjambi_to_qobject(env, result);
                    }else{
                        QVariant variant = QVariant::fromValue(JObjectWrapper(env, result));
                        if(variant.convert(functionData->functions[hash].metaType)){
                            return *reinterpret_cast<void**>(variant.data());
                        }

                    }
                }
            }
        }
        return nullptr;
    }

    static list_int count(QQmlListProperty<void> * list) {
        jint hash = jint(qint64(list->data));
        FunctionUserData *functionData{nullptr};
        {
            QReadLocker locker(qtjambi_objectdata_lock());
            functionData = QTJAMBI_GET_OBJECTUSERDATA(FunctionUserData, list->object);
        }
        if(functionData && functionData->functions.contains(hash)){
            if(JNIEnv * env = qtjambi_current_environment()){
                QTJAMBI_JNI_LOCAL_FRAME(env, 200)
                jobject _object = qtjambi_from_QObject(env, list->object);
                if(jobject countFunction = functionData->functions[hash].countFunction.object()){
                    return Java::Runtime::ToLongFunction::applyAsLong(env, countFunction, _object);
                }
            }
        }
        return 0;
    }

    static void clear(QQmlListProperty<void> * list){
        jint hash = jint(qint64(list->data));
        FunctionUserData *functionData{nullptr};
        {
            QReadLocker locker(qtjambi_objectdata_lock());
            functionData = QTJAMBI_GET_OBJECTUSERDATA(FunctionUserData, list->object);
        }
        if(functionData && functionData->functions.contains(hash)){
            if(JNIEnv * env = qtjambi_current_environment()){
                QTJAMBI_JNI_LOCAL_FRAME(env, 200)
                jobject _object = qtjambi_from_QObject(env, list->object);
                if(jobject clearFunction = functionData->functions[hash].clearFunction.object()){
                    Java::Runtime::Consumer::accept(env, clearFunction, _object);
                }
            }
        }
    }

    static void slow_clear(QQmlListProperty<void> *list)
    {
        for (list_int i = 0, end = list->count(list); i < end; ++i)
            list->removeLast(list);
    }

#if QT_VERSION >= QT_VERSION_CHECK(5,15,0)
    static void replace(QQmlListProperty<void> * list, list_int idx, void *element){
        jint hash = jint(qint64(list->data));
        FunctionUserData *functionData{nullptr};
        {
            QReadLocker locker(qtjambi_objectdata_lock());
            functionData = QTJAMBI_GET_OBJECTUSERDATA(FunctionUserData, list->object);
        }
        if(JNIEnv * env = qtjambi_current_environment()){
            QTJAMBI_JNI_LOCAL_FRAME(env, 200)
            jobject _object = qtjambi_from_QObject(env, list->object);
            if(jobject replaceFunction = functionData->functions[hash].replaceFunction.object()){
                jobject _element;
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
                    if(QMetaType(functionData->functions[hash].metaType).flags() & QMetaType::PointerToQObject){
#else
                    if(functionData->functions[hash].metaType.flags() & QMetaType::PointerToQObject){
#endif
                    _element = qtjambi_from_QObject(env, reinterpret_cast<QObject*>(element));
                }else{
                    _element = qtjambi_from_qvariant(env, QVariant(functionData->functions[hash].metaType, &element));
                }
                Java::QtQml::QQmlListProperty$ReplaceFunction::accept(env, replaceFunction, _object, jlong(idx), _element);
            }
        }
    }

    static void removeLast(QQmlListProperty<void> * list){
        jint hash = jint(qint64(list->data));
        FunctionUserData *functionData{nullptr};
        {
            QReadLocker locker(qtjambi_objectdata_lock());
            functionData = QTJAMBI_GET_OBJECTUSERDATA(FunctionUserData, list->object);
        }
        if(functionData && functionData->functions.contains(hash)){
            if(JNIEnv * env = qtjambi_current_environment()){
                QTJAMBI_JNI_LOCAL_FRAME(env, 200)
                jobject _object = qtjambi_from_QObject(env, list->object);
                if(jobject removeLastFunction = functionData->functions[hash].removeLastFunction.object()){
                    Java::Runtime::Consumer::accept(env, removeLastFunction, _object);
                }
            }
        }
    }

    static void slow_replace(QQmlListProperty<void> *list, list_int idx, void *v)
    {
        const list_int length = list->count(list);
        if (idx < 0 || idx >= length)
            return;

        QVector<void *> stash;
        if (list->clear != slow_clear) {
            stash.reserve(length);
            for (list_int i = 0; i < length; ++i)
                stash.append(i == idx ? v : list->at(list, i));
            list->clear(list);
            for (void *item : qAsConst(stash))
                list->append(list, item);
        } else {
            stash.reserve(length - idx - 1);
            for (list_int i = length - 1; i > idx; --i) {
                stash.append(list->at(list, i));
                list->removeLast(list);
            }
            list->removeLast(list);
            list->append(list, v);
            while (!stash.isEmpty())
                list->append(list, stash.takeLast());
        }
    }

    static void slow_removeLast(QQmlListProperty<void> *list)
    {
        const list_int length = list->count(list) - 1;
        if (length < 0)
            return;
        QVector<void *> stash;
        stash.reserve(length);
        for (list_int i = 0; i < length; ++i)
            stash.append(list->at(list, i));
        list->clear(list);
        for (void *item : qAsConst(stash))
            list->append(list, item);
    }
#endif
};

void __qt_create_new_QQmlListProperty_2(void* __qtjambi_ptr, JNIEnv* __jni_env, jobject, jvalue* __java_arguments)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "new QQmlListProperty<void>(QObject * o, QList<QObject > & list)()")
    QObject* object = qtjambi_to_qobject(__jni_env, __java_arguments[1].l);
    const QMetaType& metaType = qtjambi_cast<const QMetaType&>(__jni_env, __java_arguments[0].l);
    jint hash = __java_arguments[8].i;
    FunctionUserData *functionData;
    {
        QReadLocker locker(qtjambi_objectdata_lock());
        functionData = QTJAMBI_GET_OBJECTUSERDATA(FunctionUserData, object);
    }
    if(!functionData){
        QWriteLocker locker(qtjambi_objectdata_lock());
        functionData = QTJAMBI_GET_OBJECTUSERDATA(FunctionUserData, object);
        if(!functionData)
            QTJAMBI_SET_OBJECTUSERDATA(FunctionUserData, object, functionData = new FunctionUserData());
    }
    if(!functionData->functions.contains(hash)){
        functionData->functions[hash] = FunctionUserData::Functions{
                JObjectWrapper(__jni_env, __java_arguments[2].l),
                JObjectWrapper(__jni_env, __java_arguments[3].l),
                JObjectWrapper(__jni_env, __java_arguments[4].l),
                JObjectWrapper(__jni_env, __java_arguments[5].l),
                JObjectWrapper(__jni_env, __java_arguments[6].l),
                JObjectWrapper(__jni_env, __java_arguments[7].l),
                metaType
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
                                .id()
#endif
                };
    }
    FunctionUserData::Functions* functions = &functionData->functions[hash];

    QQmlListProperty<void>* listProperty = new(__qtjambi_ptr) QQmlListProperty<void>();
    listProperty->object = object;
    listProperty->data = reinterpret_cast<void*>(qsizetype(hash));
    if(functions->appendFunction.object())
        listProperty->append = FunctionUserData::append;
    if(functions->atFunction.object())
        listProperty->at = FunctionUserData::at;
    if(functions->countFunction.object())
        listProperty->count = FunctionUserData::count;
    if(functions->clearFunction.object()){
        listProperty->clear = FunctionUserData::clear;
    }else if(functions->countFunction.object() && functions->removeLastFunction.object()){
        listProperty->clear = FunctionUserData::slow_clear;
    }
#if QT_VERSION >= QT_VERSION_CHECK(5,15,0)
    if(functions->replaceFunction.object()){
        listProperty->replace = FunctionUserData::replace;
    }else if(functions->appendFunction.object()
                && functions->countFunction.object()
                && functions->atFunction.object()
                && (functions->clearFunction.object() || functions->removeLastFunction.object())){
        listProperty->replace = FunctionUserData::slow_replace;
    }
    if(functions->removeLastFunction.object()){
        listProperty->removeLast = FunctionUserData::removeLast;
    }else if(functions->appendFunction.object()
                && functions->countFunction.object()
                && functions->atFunction.object()
                && functions->clearFunction.object()){
        listProperty->removeLast = FunctionUserData::slow_removeLast;
    }
#endif
}

// destruct QQmlListProperty
void __qt_destruct_QQmlListProperty(void* ptr)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "destruct QQmlListProperty")
    reinterpret_cast<QQmlListProperty<void>*>(ptr)->~QQmlListProperty();
}

// emitting (writeSignalFunction)
// emitting  (functionsInTargetLang writeFinalFunction)
// QQmlListProperty::QQmlListProperty()
extern "C" Q_DECL_EXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_qml_QQmlListProperty_initialize_1native_1plain)
(JNIEnv *__jni_env,
 jclass __jni_class, jobject __jni_object)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QQmlListProperty::QQmlListProperty()")
    QTJAMBI_TRY{
        qtjambi_initialize_native_object(__jni_env, __jni_class, __jni_object, &__qt_create_new_QQmlListProperty_0, sizeof(QQmlListProperty<void>), typeid(QQmlListProperty<void>), false, &deleter_QQmlListProperty, nullptr);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
}

// QQmlListProperty::QQmlListProperty(QObject * o, QList<QObject > & list)
extern "C" Q_DECL_EXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_qml_QQmlListProperty_initialize_1native_1list)
(JNIEnv *__jni_env,
 jclass __jni_class,
 jobject __jni_object,
 jobject o0,
 jobject list1)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QQmlListProperty::QQmlListProperty(QObject * o, QList<QObject > & list)")
    QTJAMBI_TRY{
        jvalue arguments[2];
        arguments[0].l = o0;
        arguments[1].l = list1;
        qtjambi_initialize_native_object(__jni_env, __jni_class, __jni_object, &__qt_create_new_QQmlListProperty_1, sizeof(QQmlListProperty<void>), typeid(QQmlListProperty<void>), false, &deleter_QQmlListProperty, arguments);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
}

extern "C" Q_DECL_EXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_qml_QQmlListProperty_initialize_1native_1functions)
(JNIEnv *__jni_env,
 jclass __jni_class,
 jobject __jni_object,
 jobject metaType,
 jobject o0,
 jobject appendFunction,
 jobject countFunction,
 jobject atFunction,
 jobject clearFunction,
 jobject replaceFunction,
 jobject removeLastFunction,
 jint hash)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QQmlListProperty::QQmlListProperty(QObject * o, QList<QObject > & list)")
    QTJAMBI_TRY{
        jvalue arguments[9];
        arguments[0].l = metaType;
        arguments[1].l = o0;
        arguments[2].l = appendFunction;
        arguments[3].l = countFunction;
        arguments[4].l = atFunction;
        arguments[5].l = clearFunction;
        arguments[6].l = replaceFunction;
        arguments[7].l = removeLastFunction;
        arguments[8].i = hash;
        qtjambi_initialize_native_object(__jni_env, __jni_class, __jni_object, &__qt_create_new_QQmlListProperty_2, sizeof(QQmlListProperty<void>), typeid(QQmlListProperty<void>), false, &deleter_QQmlListProperty, arguments);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
}

extern "C" Q_DECL_EXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_qml_QQmlListProperty_getListElementType)
(JNIEnv *env,
 jclass,
 QtJambiNativeID listNativeId)
{
    jobject result{nullptr};
    QTJAMBI_TRY{
        QPair<void*,AbstractContainerAccess*> container = qtjambi_container_from_nativeId(listNativeId);
        qtjambi_check_resource(env, container.first, typeid(QList<QVariant>));
        AbstractListAccess* containerAccess = dynamic_cast<AbstractListAccess*>(container.second);
        Q_ASSERT(containerAccess);
        if(!(containerAccess->elementMetaType().flags() & QMetaType::PointerToQObject)){
            result = qtjambi_cast<jobject>(env, containerAccess->elementMetaType());
        }
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(env);
    }QTJAMBI_TRY_END
    return result;
}

// QQmlListProperty::append(QObject object)
extern "C" Q_DECL_EXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_qml_QQmlListProperty_append)
(JNIEnv *__jni_env,
 jobject _this,
 jobject object0,
 QtJambiNativeID elementType)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QQmlListProperty::append(QObject object)")
    QTJAMBI_TRY{
        QQmlListProperty<void> *__qt_this = qtjambi_to_object<QQmlListProperty<void> >(__jni_env, _this);
        qtjambi_check_resource(__jni_env, __qt_this, typeid(QQmlListProperty<void>));
        if(__qt_this->append==FunctionUserData::append){
            jint hash = jint(qint64(__qt_this->data));
            FunctionUserData *functionData{nullptr};
            {
                QReadLocker locker(qtjambi_objectdata_lock());
                functionData = QTJAMBI_GET_OBJECTUSERDATA(FunctionUserData, __qt_this->object);
            }
            if(functionData && functionData->functions.contains(hash)){
                jobject _object = qtjambi_from_QObject(__jni_env, __qt_this->object);
                if(jobject appendFunction = functionData->functions[hash].appendFunction.object()){
                    Java::Runtime::BiConsumer::accept(__jni_env, appendFunction, _object, object0);
                }
            }
        }else if(__qt_this->append){
            void* obj;
            if(!elementType){
                obj = qtjambi_cast<QObject*>(__jni_env, object0);
            }else{
                const QMetaType& metaType = qtjambi_object_from_nativeId_deref<const QMetaType>(__jni_env, elementType);
                QVariant variant = QVariant::fromValue(JObjectWrapper(__jni_env, object0));
                if(variant.convert(metaType
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
                                   .id()
#endif
                                            )){
                    obj = *reinterpret_cast<void**>(variant.data());
                }else{
                    return;
                }
            }
            __qt_this->append(__qt_this, obj);
        }
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
}

// QQmlListProperty::replace(long index, QObject object)
extern "C" Q_DECL_EXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_qml_QQmlListProperty_replace)
(JNIEnv *__jni_env,
 jobject _this,
 jlong index,
 jobject object0,
 QtJambiNativeID elementType)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QQmlListProperty::append(QObject object)")
    QTJAMBI_TRY{
        QQmlListProperty<void> *__qt_this = qtjambi_to_object<QQmlListProperty<void> >(__jni_env, _this);
        qtjambi_check_resource(__jni_env, __qt_this, typeid(QQmlListProperty<void>));
        if(__qt_this->replace==FunctionUserData::replace){
            jint hash = jint(qint64(__qt_this->data));
            FunctionUserData *functionData{nullptr};
            {
                QReadLocker locker(qtjambi_objectdata_lock());
                functionData = QTJAMBI_GET_OBJECTUSERDATA(FunctionUserData, __qt_this->object);
            }
            if(JNIEnv * env = qtjambi_current_environment()){
                QTJAMBI_JNI_LOCAL_FRAME(env, 200)
                jobject _object = qtjambi_from_QObject(env, __qt_this->object);
                if(jobject replaceFunction = functionData->functions[hash].replaceFunction.object()){
                    Java::QtQml::QQmlListProperty$ReplaceFunction::accept(env, replaceFunction, _object, index, object0);
                }
            }
        }else if(__qt_this->replace){
            void* obj;
            if(!elementType){
                obj = qtjambi_cast<QObject*>(__jni_env, object0);
            }else{
                const QMetaType& metaType = qtjambi_object_from_nativeId_deref<const QMetaType>(__jni_env, elementType);
                QVariant variant = QVariant::fromValue(JObjectWrapper(__jni_env, object0));
                if(variant.convert(metaType
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
                                            .id()
#endif
                                            )){
                    obj = *reinterpret_cast<void**>(variant.data());
                }else{
                    return;
                }
            }
            __qt_this->replace(__qt_this, index, obj);
        }
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
}

// QQmlListProperty::at(int index)
extern "C" Q_DECL_EXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_qml_QQmlListProperty_at)
(JNIEnv *__jni_env,
 jobject _this,
 jlong index0,
 QtJambiNativeID elementType)
{
    jobject _result{nullptr};
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QQmlListProperty::at(int index)")
    QTJAMBI_TRY{
        QQmlListProperty<void> *__qt_this = qtjambi_to_object<QQmlListProperty<void> >(__jni_env, _this);
        qtjambi_check_resource(__jni_env, __qt_this, typeid(QQmlListProperty<void>));
        if(__qt_this->at==FunctionUserData::at){
            jint hash = jint(qint64(__qt_this->data));
            FunctionUserData *functionData{nullptr};
            {
                QReadLocker locker(qtjambi_objectdata_lock());
                functionData = QTJAMBI_GET_OBJECTUSERDATA(FunctionUserData, __qt_this->object);
            }
            if(functionData && functionData->functions.contains(hash)){
                jobject _object = qtjambi_from_QObject(__jni_env, __qt_this->object);
                if(jobject atFunction = functionData->functions[hash].atFunction.object()){
                    _result = Java::QtQml::QQmlListProperty$AtFunction::apply(__jni_env, atFunction, _object, index0);
                }
            }
        }else if(__qt_this->at){
            void*  qt_return_value = __qt_this->at(__qt_this, index0);
            if(!elementType){
                _result = qtjambi_cast<jobject>(__jni_env, reinterpret_cast<QObject*>(qt_return_value));
            }else{
                const QMetaType& metaType = qtjambi_object_from_nativeId_deref<const QMetaType>(__jni_env, elementType);
                _result = qtjambi_from_qvariant(__jni_env, QVariant(metaType
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
                                                                            .id()
#endif
                                                                            , &qt_return_value));
            }
        }
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
    return _result;
}

// QQmlListProperty::canAppend()
extern "C" Q_DECL_EXPORT jboolean JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_qml_QQmlListProperty_canAppend)
(JNIEnv * __jni_env,
 jobject _this)
{
    jboolean _result{false};
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QQmlListProperty::canAppend()")
    QTJAMBI_TRY{
        QQmlListProperty<void> *__qt_this = qtjambi_to_object<QQmlListProperty<void> >(__jni_env, _this);
        qtjambi_check_resource(__jni_env, __qt_this, typeid(QQmlListProperty<void>));
        _result = __qt_this->append!=nullptr;
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
    return _result;
}

// QQmlListProperty::canAt()
extern "C" Q_DECL_EXPORT jboolean JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_qml_QQmlListProperty_canAt)
(JNIEnv * __jni_env,
 jobject _this)
{
    jboolean _result{false};
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QQmlListProperty::canAt()")
    QTJAMBI_TRY{
        QQmlListProperty<void> *__qt_this = qtjambi_to_object<QQmlListProperty<void> >(__jni_env, _this);
        qtjambi_check_resource(__jni_env, __qt_this, typeid(QQmlListProperty<void>));
        _result = __qt_this->at!=nullptr;
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
    return _result;
}

// QQmlListProperty::canClear()
extern "C" Q_DECL_EXPORT jboolean JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_qml_QQmlListProperty_canClear)
(JNIEnv * __jni_env,
 jobject _this)
{
    jboolean _result{false};
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QQmlListProperty::canClear()")
    QTJAMBI_TRY{
        QQmlListProperty<void> *__qt_this = qtjambi_to_object<QQmlListProperty<void> >(__jni_env, _this);
        qtjambi_check_resource(__jni_env, __qt_this, typeid(QQmlListProperty<void>));
        _result = __qt_this->clear!=nullptr;
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
    return _result;
}

// QQmlListProperty::canCount()
extern "C" Q_DECL_EXPORT jboolean JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_qml_QQmlListProperty_canCount)
(JNIEnv * __jni_env,
 jobject _this)
{
    jboolean _result{false};
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QQmlListProperty::canCount()")
    QTJAMBI_TRY{
        QQmlListProperty<void> *__qt_this = qtjambi_to_object<QQmlListProperty<void> >(__jni_env, _this);
        qtjambi_check_resource(__jni_env, __qt_this, typeid(QQmlListProperty<void>));
        _result = __qt_this->count!=nullptr;
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
    return _result;
}

// QQmlListProperty::canRemoveLast()
extern "C" Q_DECL_EXPORT jboolean JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_qml_QQmlListProperty_canRemoveLast)
(JNIEnv * __jni_env,
 jobject _this)
{
    jboolean _result{false};
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QQmlListProperty::canCount()")
    QTJAMBI_TRY{
        QQmlListProperty<void> *__qt_this = qtjambi_to_object<QQmlListProperty<void> >(__jni_env, _this);
        qtjambi_check_resource(__jni_env, __qt_this, typeid(QQmlListProperty<void>));
        _result = __qt_this->removeLast!=nullptr;
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
    return _result;
}

// QQmlListProperty::canReplace()
extern "C" Q_DECL_EXPORT jboolean JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_qml_QQmlListProperty_canReplace)
(JNIEnv * __jni_env,
 jobject _this)
{
    jboolean _result{false};
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QQmlListProperty::canCount()")
    QTJAMBI_TRY{
        QQmlListProperty<void> *__qt_this = qtjambi_to_object<QQmlListProperty<void> >(__jni_env, _this);
        qtjambi_check_resource(__jni_env, __qt_this, typeid(QQmlListProperty<void>));
        _result = __qt_this->replace!=nullptr;
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
    return _result;
}

// QQmlListProperty::clear()
extern "C" Q_DECL_EXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_qml_QQmlListProperty_clear)
(JNIEnv * __jni_env,
 jobject _this)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QQmlListProperty::clear()")
    QTJAMBI_TRY{
        QQmlListProperty<void> *__qt_this = qtjambi_to_object<QQmlListProperty<void> >(__jni_env, _this);
        qtjambi_check_resource(__jni_env, __qt_this, typeid(QQmlListProperty<void>));
        if(__qt_this->clear)
            __qt_this->clear(__qt_this);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
}

// QQmlListProperty::removeLast()
extern "C" Q_DECL_EXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_qml_QQmlListProperty_removeLast)
(JNIEnv * __jni_env,
 jobject _this)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QQmlListProperty::clear()")
    QTJAMBI_TRY{
        QQmlListProperty<void> *__qt_this = qtjambi_to_object<QQmlListProperty<void> >(__jni_env, _this);
        qtjambi_check_resource(__jni_env, __qt_this, typeid(QQmlListProperty<void>));
        if(__qt_this->removeLast)
            __qt_this->removeLast(__qt_this);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
}

// QQmlListProperty::count()
extern "C" Q_DECL_EXPORT jlong JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_qml_QQmlListProperty_count)
(JNIEnv * __jni_env,
 jobject _this)
{
    jlong _result{0};
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QQmlListProperty::count()")
    QTJAMBI_TRY{
        QQmlListProperty<void> *__qt_this = qtjambi_to_object<QQmlListProperty<void> >(__jni_env, _this);
        qtjambi_check_resource(__jni_env, __qt_this, typeid(QQmlListProperty<void>));
        if(__qt_this->count)
            _result = __qt_this->count(__qt_this);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
    return _result;
}

// QQmlListProperty::object()
extern "C" Q_DECL_EXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_qml_QQmlListProperty_object)
(JNIEnv *__jni_env,
 jobject _this)
{
    jobject _result{nullptr};
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QQmlListProperty::object()")
    QTJAMBI_TRY{
        QQmlListProperty<void> *__qt_this = qtjambi_to_object<QQmlListProperty<void> >(__jni_env, _this);
        qtjambi_check_resource(__jni_env, __qt_this, typeid(QQmlListProperty<void>));
        _result = qtjambi_cast<jobject>(__jni_env, __qt_this->object);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
    return _result;
}

// emitting (AbstractMetaClass::NormalFunctions|AbstractMetaClass::AbstractFunctions writeFinalFunction)
// emitting Field accessors (writeFieldAccessors)
// emitting (writeInterfaceCastFunction)
// emitting (writeSignalInitialization)
// emitting (writeJavaLangObjectOverrideFunctions)

void initialize_meta_info_QQmlListProperty(){
    if(JNIEnv* env = qtjambi_current_environment()){
        QTJAMBI_JNI_LOCAL_FRAME(env, 300)
        if(Java::Runtime::Boolean::getBoolean(env, env->NewStringUTF("io.qt.enabled-qml-debugging-nowarn"))){
#if QT_VERSION < QT_VERSION_CHECK(6,4,0)
            QQmlDebuggingEnabler(false);
#else
            QQmlDebuggingEnabler::enableDebugging(false);
#endif
        }else if(Java::Runtime::Boolean::getBoolean(env, env->NewStringUTF("io.qt.enabled-qml-debugging"))){
#if QT_VERSION < QT_VERSION_CHECK(6,4,0)
            QQmlDebuggingEnabler(true);
#else
            QQmlDebuggingEnabler::enableDebugging(true);
#endif
        }
    }
    setQmlReportDestruction(&QQmlPrivate::qdeclarativeelement_destructor);
    const std::type_info& typeId = registerTypeInfo<QQmlListProperty<void>>("QQmlListProperty", "io/qt/qml/QQmlListProperty");
    registerOperators<QQmlListProperty<void>>();
    registerConstructorInfos(typeId, &__qt_destruct_QQmlListProperty, {ConstructorInfo(&__qt_create_new_QQmlListProperty_0, nullptr),
                                                                       ConstructorInfo(&__qt_create_new_QQmlListProperty_1, "Lio/qt/core/QObject;Lio/qt/core/QList;")} );
    registerDeleter(typeId, &deleter_QQmlListProperty);
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
    int metaTypeID = registerMetaType<QQmlListProperty<QObject>>("QQmlListProperty<QObject>",
                            QtMetaTypePrivate::QMetaTypeFunctionHelper<QQmlListProperty<void>>::Destruct,
                            [](void *where, const void *ptr) -> void *{
                                QQmlListProperty<void>* listProperty = new(where) QQmlListProperty<void>();
                                if (ptr){
                                    const QQmlListProperty<void>* _ptr = reinterpret_cast<const QQmlListProperty<void>*>(ptr);
                                    listProperty->object = _ptr->object;
                                    listProperty->data = _ptr->data;
                                    listProperty->append = _ptr->append;
                                    listProperty->at = _ptr->at;
                                    listProperty->clear = _ptr->clear;
                                    listProperty->count = _ptr->count;
#if QT_VERSION < QT_VERSION_CHECK(5,15,0)
                                    listProperty->dummy1 = _ptr->dummy1;
                                    listProperty->dummy2 = _ptr->dummy2;
#else
                                    listProperty->replace = _ptr->replace;
                                    listProperty->removeLast = _ptr->removeLast;
#endif
                                }
                                return listProperty;
                            }
                        );
    QMetaType::registerNormalizedTypedef("QQmlListProperty", metaTypeID);
#else
    int metaTypeID = registerMetaType<QQmlListProperty<QObject>>("QQmlListProperty<QObject>",
                            QtJambiPrivate::QMetaTypeInterfaceFunctions<QQmlListProperty<void>>::defaultCtr,
                            [](const QtPrivate::QMetaTypeInterface *, void *where, const void *ptr){
                                const QQmlListProperty<void>* _ptr = reinterpret_cast<const QQmlListProperty<void>*>(ptr);
                                new(where) QQmlListProperty<void>(*_ptr);
                            },
                            [](const QtPrivate::QMetaTypeInterface *, void *where, void *ptr){
                                QQmlListProperty<void>* _ptr = reinterpret_cast<QQmlListProperty<void>*>(ptr);
                                new(where) QQmlListProperty<void>(std::move(*_ptr));
                            }
                        );
    QMetaType::registerNormalizedTypedef("QQmlListProperty", QMetaType(metaTypeID));
#endif
    registerMetaTypeID(typeId, metaTypeID);
}

hash_type qHash(const QQmlListReference &value)
{
    QQmlListReferencePrivate* p = QQmlListReferencePrivate::get(const_cast<QQmlListReference*>(&value));
    if(!p){
        return 0;
    }
    hash_type hashCode = qHash(quintptr(value.object()));
    hashCode = hashCode * 31 + qHash(quintptr(p->property.data));
    hashCode = hashCode * 31 + qHash(value.canAppend());
    hashCode = hashCode * 31 + qHash(value.canAt());
    hashCode = hashCode * 31 + qHash(value.canClear());
    hashCode = hashCode * 31 + qHash(value.canCount());
    hashCode = hashCode * 31 + qHash(value.canRemoveLast());
    hashCode = hashCode * 31 + qHash(value.canReplace());
    return hashCode;
}
