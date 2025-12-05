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

#include <QtCore/qcompilerdetection.h>
QT_WARNING_DISABLE_DEPRECATED

#include "pch_p.h"

#if QT_VERSION >= QT_VERSION_CHECK(6,7,0)
jobject QtJambiAPI::convertQSpanToJavaObject(JNIEnv *env,
                                             QtJambiNativeID owner,
                                             AbstractSpanAccess* containerAccess,
                                             const void* begin,
                                             jlong size)
{
    jobject returned = nullptr;
    jobject obj = CoreAPI::javaObject(owner, env);
    returned = containerAccess->isConst() ? Java::QtCore::QConstSpan::newInstance(env, nullptr, obj) : Java::QtCore::QSpan::newInstance(env, nullptr, obj);
    QtJambiSpan* span = new QtJambiSpan{begin, qsizetype(size)};
    QSharedPointer<QtJambiLink> link = QtJambiLink::createLinkForNativeObject(env, returned, span,
                                                                                     LINK_NAME_ARG("QSpan")
                                                                                     owner,
                                                                                     QtJambiSpan::deleter, containerAccess);
    if(Q_UNLIKELY(!link)) {
        returned = nullptr;
        if(containerAccess)
            containerAccess->dispose();
    }
    return returned;
}

jobject QtJambiAPI::convertQSpanFromQListToJavaObject(JNIEnv *env,
                                             const void* span,
                                             CopyFunction copyFunction,
                                             PtrDeleterFunction destructor_function,
                                             AbstractListAccess* containerAccess,
                                             bool isConst){
    jobject list = QtJambiAPI::convertQListToJavaObject(env, InvalidNativeID, span, copyFunction, destructor_function, ListType::QList, containerAccess);
    return isConst ? Java::QtCore::QConstSpan::newInstance2(env, nullptr, list) : Java::QtCore::QSpan::newInstance2(env, nullptr, list);
}
#endif

jobject QtJambiAPI::convertQSequentialIteratorToJavaObject(JNIEnv *env,
                           QtJambiNativeID owner,
                           void* iteratorPtr,
                           PtrDeleterFunction destructor_function,
                           AbstractSequentialConstIteratorAccess* containerAccess)
{
    Q_ASSERT(containerAccess);
    jobject returned = nullptr;
    jobject obj = CoreAPI::javaObject(owner, env);
    bool isMutable = dynamic_cast<AbstractSequentialIteratorAccess*>(containerAccess);
    returned = isMutable ? Java::QtCore::QSequentialIterator::newInstance(env, nullptr, obj)
                         : Java::QtCore::QSequentialConstIterator::newInstance(env, nullptr, obj);
    QSharedPointer<QtJambiLink> link = QtJambiLink::createLinkForNativeObject(env, returned, iteratorPtr,
                                                                                       LINK_NAME_ARG(isMutable ? "QSequentialIterator" : "QSequentialConstIterator")
                                                                                       owner, destructor_function, containerAccess);
    if(Q_UNLIKELY(!link)) {
        returned = nullptr;
        containerAccess->dispose();
    }
    return returned;
}

jobject QtJambiAPI::convertQAssociativeIteratorToJavaObject(JNIEnv *env,
                                  QtJambiNativeID owner,
                                  void* iteratorPtr,
                                  PtrDeleterFunction destructor_function,
                                  AbstractAssociativeConstIteratorAccess* containerAccess)
{
    Q_ASSERT(containerAccess);
    jobject returned = nullptr;
    jobject obj = CoreAPI::javaObject(owner, env);
    bool isMutable = dynamic_cast<AbstractAssociativeIteratorAccess*>(containerAccess);
    returned = isMutable ? Java::QtCore::QAssociativeIterator::newInstance(env, nullptr, obj)
                         : Java::QtCore::QAssociativeConstIterator::newInstance(env, nullptr, obj);
    QSharedPointer<QtJambiLink> link = QtJambiLink::createLinkForNativeObject(env, returned, iteratorPtr,
                                                                                            LINK_NAME_ARG(isMutable ? "QAssociativeIterator" : "QAssociativeConstIterator")
                                                                                            owner, destructor_function, containerAccess);
    if(Q_UNLIKELY(!link)) {
        returned = nullptr;
        containerAccess->dispose();
    }
    return returned;
}

jobject QtJambiAPI::convertQListToJavaObject(JNIEnv *env,
                           QtJambiNativeID owner,
                           const void* listPtr,
                           CopyFunction copyFunction,
                           PtrDeleterFunction deleter,
                           ListType listType,
                           AbstractListAccess* containerAccess)
{
    Q_ASSERT(containerAccess);
    if(QtJambiAPI::fromNativeId(owner)==listPtr){
        if(jobject obj = CoreAPI::javaObject(owner, env)){
            switch(listType){
            case QtJambiAPI::ListType::QQueue:
                if(Java::QtCore::QQueue::isInstanceOf(env, obj)){
                    containerAccess->dispose();
                    return obj;
                }
                break;
#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
            case QtJambiAPI::ListType::QStack:
                if(Java::QtCore::QStack::isInstanceOf(env, obj)){
                    containerAccess->dispose();
                    return obj;
                }
                break;
#endif
            default:
                if(Java::QtCore::QList::isInstanceOf(env, obj)){
                    containerAccess->dispose();
                    return obj;
                }
                break;
            }
        }
    }else{
        for(const QSharedPointer<QtJambiLink>& link : QtJambiLink::findLinksForPointer(listPtr)){
            if(link && (!copyFunction || link->createdByJava())){
                jobject obj = link->getJavaObjectLocalRef(env);
                switch(listType){
                case QtJambiAPI::ListType::QQueue:
                    if(Java::QtCore::QQueue::isInstanceOf(env, obj)){
                        containerAccess->dispose();
                        return obj;
                    }
                    break;
    #if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
                case QtJambiAPI::ListType::QStack:
                    if(Java::QtCore::QStack::isInstanceOf(env, obj)){
                        containerAccess->dispose();
                        return obj;
                    }
                    break;
    #endif
                default:
                    if(Java::QtCore::QList::isInstanceOf(env, obj)){
                        containerAccess->dispose();
                        return obj;
                    }
                    break;
                }
            }
        }
    }
    if(listType==ListType::QList && containerAccess->elementMetaType().id()==QMetaType::QString){
        containerAccess->dispose();
        return QtJambiAPI::convertQStringListToJavaObject(env, owner, listPtr, copyFunction, deleter);
    }
    jobject returned = nullptr;
    CHECK_CONTAINER_ACCESS(env, containerAccess)
    QByteArray containerName;
    switch(listType){
    case QtJambiAPI::ListType::QQueue:
        returned = Java::QtCore::QQueue::newInstance(env, nullptr);
        containerName = "QQueue<";
        break;
#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
    case QtJambiAPI::ListType::QStack:
        returned = Java::QtCore::QStack::newInstance(env, nullptr);
        containerName = "QStack<";
        break;
#endif
    default:
        returned = Java::QtCore::QList::newInstance(env, nullptr);
        containerName = "QList<";
        break;
    }
    containerName += containerAccess->elementMetaType().name();
    containerName += ">";
    containerName = QMetaObject::normalizedType(containerName);

    QMetaType containerMetaType(containerAccess->registerContainer(containerName));
    QSharedPointer<QtJambiLink> link;
    if(Q_UNLIKELY(!!owner)){
        link = QtJambiLink::createLinkForNativeObject(env, returned, const_cast<void*>(listPtr),
                                                        LINK_NAME_META_TYPE_ARG(containerMetaType)
                                                        owner, containerAccess);
    }else if(deleter){
        if(copyFunction){
            link = QtJambiLink::createLinkForNativeObject(env, returned, copyFunction(listPtr),
                                                       LINK_NAME_META_TYPE_ARG(containerMetaType)
                                                       false, true, deleter, containerAccess);
        }else{
            link = QtJambiLink::createLinkForNativeObject(env, returned, const_cast<void*>(listPtr),
                                                       LINK_NAME_META_TYPE_ARG(containerMetaType)
                                                       false, false, deleter, containerAccess);
        }
    }else{
        if(copyFunction){
            link = QtJambiLink::createLinkForNativeObject(env, returned, copyFunction(listPtr),
                                                       LINK_NAME_META_TYPE_ARG(containerMetaType)
                                                       false, true, containerAccess);
        }else{
            link = QtJambiLink::createLinkForNativeObject(env, returned, const_cast<void*>(listPtr),
                                                       LINK_NAME_META_TYPE_ARG(containerMetaType)
                                                       false, false, containerAccess);
        }
    }
    if(Q_UNLIKELY(!link)) {
        returned = nullptr;
        containerAccess->dispose();
    }
    return returned;
}

jobject ContainerAPI::objectFromQList(JNIEnv *env,
                                      void*& listPtr,
                                      AbstractContainerAccess*& containerAccess
                                      )
{
    if(auto access = dynamic_cast<AbstractListAccess*>(containerAccess)){
        jobject result = objectFromQList(env, listPtr, access);
        containerAccess = result ? access : nullptr;
        return result;
    }
    return nullptr;
}

jobject ContainerAPI::objectFromQList(JNIEnv *env,
                           void*& listPtr,
                           AbstractListAccess*& containerAccess
                        )
{
    for(const QSharedPointer<QtJambiLink>& link : QtJambiLink::findLinksForPointer(listPtr)){
        if(link && link->createdByJava()){
            jobject obj = link->getJavaObjectLocalRef(env);
            if(Java::QtCore::QList::isInstanceOf(env, obj)){
                containerAccess->dispose();
                containerAccess = dynamic_cast<AbstractListAccess*>(link->containerAccess());
                listPtr = link->pointer();
                return obj;
            }
        }
    }
    QSharedPointer<QtJambiLink> link;
    jobject returned = nullptr;
    if(containerAccess->elementMetaType().id()==QMetaType::QString){
        containerAccess->dispose();
        containerAccess = QtJambiPrivate::QListAccess<QString>::newInstance();
        returned = Java::QtCore::QStringList::newInstance(env, nullptr);
        link = QtJambiLink::createLinkForNativeObject(env, returned, const_cast<void*>(listPtr),
                                                      LINK_NAME_ARG("QStringList")
                                                      false, false, containerAccess);
    }else{
        CHECK_CONTAINER_ACCESS(env, containerAccess)
        QByteArray containerName;
        returned = Java::QtCore::QList::newInstance(env, nullptr);
        containerName = "QList<";
        containerName += containerAccess->elementMetaType().name();
        containerName += ">";
        containerName = QMetaObject::normalizedType(containerName);
        containerAccess->registerContainer(containerName);
        link = QtJambiLink::createLinkForNativeObject(env, returned, const_cast<void*>(listPtr),
                                                      LINK_NAME_ARG(containerName)
                                                      false, false, containerAccess);
    }
    if(Q_UNLIKELY(!link)) {
        returned = nullptr;
        containerAccess->dispose();
        containerAccess = nullptr;
        listPtr = nullptr;
    }
    return returned;
}

#if QT_VERSION >= QT_VERSION_CHECK(6, 7, 0)
jobject ContainerAPI::objectFromQSpan(JNIEnv *env,
                                      void*& listPtr,
                                      AbstractContainerAccess*& containerAccess
                                      )
{
    if(auto access = dynamic_cast<AbstractSpanAccess*>(containerAccess)){
        jobject result = objectFromQSpan(env, listPtr, access);
        containerAccess = result ? access : nullptr;
        return result;
    }
    return nullptr;
}

jobject ContainerAPI::objectFromQSpan(JNIEnv *env,
                                      void*& listPtr,
                                      AbstractSpanAccess*& containerAccess
                                      )
{
    for(const QSharedPointer<QtJambiLink>& link : QtJambiLink::findLinksForPointer(listPtr)){
        if(link && link->createdByJava()){
            jobject obj = link->getJavaObjectLocalRef(env);
            if(Java::QtCore::QConstSpan::isInstanceOf(env, obj)){
                if(containerAccess->isConst() || Java::QtCore::QSpan::isInstanceOf(env, obj)){
                    containerAccess->dispose();
                    containerAccess = dynamic_cast<AbstractSpanAccess*>(link->containerAccess());
                    listPtr = link->pointer();
                    return obj;
                }
            }
        }
    }
    QSharedPointer<QtJambiLink> link;
    jobject returned = nullptr;
    //CHECK_CONTAINER_ACCESS(env, containerAccess)
    QByteArray containerName;
    containerName = "QSpan<";
    if(containerAccess->isConst()){
        containerName += "const ";
        returned = Java::QtCore::QConstSpan::newInstance(env, nullptr, nullptr);
    }else{
        returned = Java::QtCore::QSpan::newInstance(env, nullptr, nullptr);
    }
    containerName += containerAccess->elementMetaType().name();
    containerName += ">";
    containerName = QMetaObject::normalizedType(containerName);
    containerAccess->registerContainer(containerName);
    link = QtJambiLink::createLinkForNativeObject(env, returned, const_cast<void*>(listPtr),
                                                  LINK_NAME_ARG(containerName)
                                                  false, false, containerAccess);
    if(Q_UNLIKELY(!link)) {
        returned = nullptr;
        containerAccess->dispose();
        containerAccess = nullptr;
        listPtr = nullptr;
    }
    return returned;
}
#endif //QT_VERSION >= QT_VERSION_CHECK(6, 7, 0)

jobject convertQStringListToJavaObject(JNIEnv *__jni_env,
                                                      const QSharedPointer<char>& listPtr
                                                      );
jobject convertQStringListToJavaObject(JNIEnv *__jni_env,
                                                      const std::shared_ptr<char>& listPtr
                                                      );

template<template<typename> class SmartPointer>
jobject convertQListToJavaObject(JNIEnv *env,
                             const SmartPointer<char>& smartPointer,
                             QtJambiAPI::ListType listType,
                             AbstractListAccess* containerAccess
                        )
{
    void* listPtr = smartPointer.get();
    if(Q_UNLIKELY(!listPtr))
        return nullptr;
    for(const QSharedPointer<QtJambiLink>& link : QtJambiLink::findLinksForPointer(listPtr)){
        if(Q_LIKELY(link)){
            jobject obj = link->getJavaObjectLocalRef(env);
            switch(listType){
            case QtJambiAPI::ListType::QQueue:
                if(Java::QtCore::QQueue::isInstanceOf(env, obj)){
                    containerAccess->dispose();
                    return obj;
                }
                break;
#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
            case QtJambiAPI::ListType::QStack:
                if(Java::QtCore::QStack::isInstanceOf(env, obj)){
                    containerAccess->dispose();
                    return obj;
                }
                break;
#endif
            default:
                if(Java::QtCore::QList::isInstanceOf(env, obj)){
                    containerAccess->dispose();
                    return obj;
                }
                break;
            }
        }
    }
    if(listType==QtJambiAPI::ListType::QList && containerAccess->elementMetaType().id()==QMetaType::QString){
        containerAccess->dispose();
        return QtJambiAPI::convertQStringListToJavaObject(env, smartPointer);
    }

    jobject returned = nullptr;
    CHECK_CONTAINER_ACCESS(env, containerAccess)
    QByteArray containerName;
    switch(listType){
    case QtJambiAPI::ListType::QQueue:
        returned = Java::QtCore::QQueue::newInstance(env, nullptr);
        containerName = "QQueue<";
        break;
#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
    case QtJambiAPI::ListType::QStack:
        returned = Java::QtCore::QStack::newInstance(env, nullptr);
        containerName = "QStack<";
        break;
#endif
    default:
        returned = Java::QtCore::QList::newInstance(env, nullptr);
        containerName = "QList<";
        break;
    }
    containerName += containerAccess->elementMetaType().name();
    containerName += ">";
    containerName = QMetaObject::normalizedType(containerName);
    QMetaType containerMetaType(containerAccess->registerContainer(containerName));
    QSharedPointer<QtJambiLink> link = QtJambiLink::createLinkForSmartPointerToObject(env, returned,
                                                                                                LINK_NAME_META_TYPE_ARG(containerMetaType)
                                                                                                smartPointer, containerAccess);
    if(Q_UNLIKELY(!link)) {
        returned = nullptr;
        containerAccess->dispose();
    }
    return returned;
}

jobject QtJambiAPI::convertQListToJavaObject(JNIEnv *env,
                                             const QSharedPointer<char>& smartPointer,
                                             ListType listType,
                                             AbstractListAccess* containerAccess
                                             ){
    return ::convertQListToJavaObject<QSharedPointer>(env,
                                                   smartPointer,
                                                   listType,
                                                   containerAccess
                                                );
}

jobject QtJambiAPI::convertQListToJavaObject(JNIEnv *env,
                                             const std::shared_ptr<char>& smartPointer,
                                             ListType listType,
                                             AbstractListAccess* containerAccess
                                             ){
    return ::convertQListToJavaObject<std::shared_ptr>(env,
                                                    smartPointer,
                                                    listType,
                                                    containerAccess
                                                    );
}

jobject QtJambiAPI::convertQStringListToJavaObject(JNIEnv *env,
                           QtJambiNativeID owner,
                           const void* listPtr,
                           CopyFunction copyFunction,
                           PtrDeleterFunction deleter)
{
    if(QtJambiAPI::fromNativeId(owner)==listPtr){
        if(jobject obj = CoreAPI::javaObject(owner, env)){
            if(Java::QtCore::QStringList::isInstanceOf(env, obj)){
                return obj;
            }
        }
    }
    for(const QSharedPointer<QtJambiLink>& link : QtJambiLink::findLinksForPointer(listPtr)){
        if(link && (!copyFunction || link->createdByJava())){
            jobject obj = link->getJavaObjectLocalRef(env);
            if(Java::QtCore::QStringList::isInstanceOf(env, obj)){
                return obj;
            }
        }
    }
    jobject returned = nullptr;
    AbstractListAccess* containerAccess = QtJambiPrivate::QListAccess<QString>::newInstance();
    returned = Java::QtCore::QStringList::newInstance(env, nullptr);

    QSharedPointer<QtJambiLink> link;
    if(Q_UNLIKELY(!!owner)){
        link = QtJambiLink::createLinkForNativeObject(env, returned, const_cast<void*>(listPtr),
                                                        LINK_NAME_ARG("QStringList")
                                                        owner, containerAccess);
    }else if(deleter){
        if(copyFunction){
            link = QtJambiLink::createLinkForNativeObject(env, returned, copyFunction(listPtr),
                                                       LINK_NAME_ARG("QStringList")
                                                       false, true, deleter, containerAccess);
        }else{
            link = QtJambiLink::createLinkForNativeObject(env, returned, const_cast<void*>(listPtr),
                                                       LINK_NAME_ARG("QStringList")
                                                       false, false, deleter, containerAccess);
        }
    }else{
        if(copyFunction){
            link = QtJambiLink::createLinkForNativeObject(env, returned, copyFunction(listPtr),
                                                       LINK_NAME_ARG("QStringList")
                                                       false, true, containerAccess);
        }else{
            link = QtJambiLink::createLinkForNativeObject(env, returned, const_cast<void*>(listPtr),
                                                       LINK_NAME_ARG("QStringList")
                                                       false, false, containerAccess);
        }
    }
    if(Q_UNLIKELY(!link)) {
        returned = nullptr;
        containerAccess->dispose();
    }
    return returned;
}

template<template<typename> class SmartPointer>
jobject convertQStringListToJavaObject(JNIEnv *env,
                                                   const SmartPointer<char>& smartPointer)
{
    void* listPtr = smartPointer.get();
    if(Q_UNLIKELY(!listPtr))
        return nullptr;
    for(const QSharedPointer<QtJambiLink>& link : QtJambiLink::findLinksForPointer(listPtr)){
        if(Q_LIKELY(link)){
            jobject obj = link->getJavaObjectLocalRef(env);
            if(Java::QtCore::QStringList::isInstanceOf(env, obj)){
                return obj;
            }
        }
    }

    AbstractListAccess* containerAccess = QtJambiPrivate::QListAccess<QString>::newInstance();

    jobject returned = nullptr;
    returned = Java::QtCore::QStringList::newInstance(env, nullptr);
    QSharedPointer<QtJambiLink> link = QtJambiLink::createLinkForSmartPointerToObject(env, returned,
                                                                                              LINK_NAME_ARG("QStringList")
                                                                                              smartPointer, containerAccess);
    if(Q_UNLIKELY(!link)) {
        returned = nullptr;
        containerAccess->dispose();
    }
    return returned;
}

jobject QtJambiAPI::convertQStringListToJavaObject(JNIEnv *env,
                                                   const QSharedPointer<char>& smartPointer){
    return ::convertQStringListToJavaObject<QSharedPointer>(env, smartPointer);
}


jobject QtJambiAPI::convertQStringListToJavaObject(JNIEnv *env,
                                                   const std::shared_ptr<char>& smartPointer){
    return ::convertQStringListToJavaObject<std::shared_ptr>(env, smartPointer);
}

jobject QtJambiAPI::convertQSetToJavaObject(JNIEnv *env,
                         QtJambiNativeID owner,
                         const void* listPtr,
                         CopyFunction copyFunction,
                         PtrDeleterFunction deleter,
                         AbstractSetAccess* containerAccess)
{
    Q_ASSERT(containerAccess);
    if(QtJambiAPI::fromNativeId(owner)==listPtr){
        jobject obj = CoreAPI::javaObject(owner, env);
        if(Java::QtCore::QSet::isInstanceOf(env, obj)){
            containerAccess->dispose();
            return obj;
        }
    }
    for(const QSharedPointer<QtJambiLink>& link : QtJambiLink::findLinksForPointer(listPtr)){
        if(link && (!copyFunction || link->createdByJava())){
            jobject obj = link->getJavaObjectLocalRef(env);
            if(Java::QtCore::QSet::isInstanceOf(env, obj)){
                containerAccess->dispose();
                return obj;
            }
        }
    }

    jobject returned = nullptr;
    CHECK_CONTAINER_ACCESS(env, containerAccess)
    returned = Java::QtCore::QSet::newInstance(env, nullptr);
    QSharedPointer<QtJambiLink> link;
    QByteArray containerName = "QSet<";
    containerName += containerAccess->elementMetaType().name();
    containerName += ">";
    containerName = QMetaObject::normalizedType(containerName);
    QMetaType containerMetaType(containerAccess->registerContainer(containerName));
    if(Q_UNLIKELY(!!owner)){
        link = QtJambiLink::createLinkForNativeObject(env, returned, const_cast<void*>(listPtr),
                                                        LINK_NAME_META_TYPE_ARG(containerMetaType)
                                                        owner, containerAccess);
    }else if(deleter){
        if(copyFunction){
            link = QtJambiLink::createLinkForNativeObject(env, returned, copyFunction(listPtr),
                                                       LINK_NAME_META_TYPE_ARG(containerMetaType)
                                                       false, true, deleter, containerAccess);
        }else{
            link = QtJambiLink::createLinkForNativeObject(env, returned, const_cast<void*>(listPtr),
                                                       LINK_NAME_META_TYPE_ARG(containerMetaType)
                                                       false, false, deleter, containerAccess);
        }
    }else{
        if(copyFunction){
            link = QtJambiLink::createLinkForNativeObject(env, returned, copyFunction(listPtr),
                                                       LINK_NAME_META_TYPE_ARG(containerMetaType)
                                                       false, true, containerAccess);
        }else{
            link = QtJambiLink::createLinkForNativeObject(env, returned, const_cast<void*>(listPtr),
                                                       LINK_NAME_META_TYPE_ARG(containerMetaType)
                                                       false, false, containerAccess);
        }
    }
    if(Q_UNLIKELY(!link)) {
        returned = nullptr;
        containerAccess->dispose();
    }
    return returned;
}

template<template<typename> class SmartPointer>
jobject convertQSetToJavaObject(JNIEnv *env,
                                     const SmartPointer<char>& smartPointer,
                                     AbstractSetAccess* containerAccess
                                )
{
    void* listPtr = smartPointer.get();
    if(Q_UNLIKELY(!listPtr))
        return nullptr;
    for(const QSharedPointer<QtJambiLink>& link : QtJambiLink::findLinksForPointer(listPtr)){
        if(Q_LIKELY(link)){
            jobject obj = link->getJavaObjectLocalRef(env);
            if(Java::QtCore::QSet::isInstanceOf(env, obj)){
                containerAccess->dispose();
                return obj;
            }
        }
    }

    jobject returned = nullptr;
    CHECK_CONTAINER_ACCESS(env, containerAccess)
    returned = Java::QtCore::QSet::newInstance(env, nullptr);
    QByteArray containerName = "QSet<";
    containerName += containerAccess->elementMetaType().name();
    containerName += ">";
    containerName = QMetaObject::normalizedType(containerName);
    QMetaType containerMetaType(containerAccess->registerContainer(containerName));
    QSharedPointer<QtJambiLink> link = QtJambiLink::createLinkForSmartPointerToObject(env, returned,
                                                                                              LINK_NAME_META_TYPE_ARG(containerMetaType)
                                                                                              smartPointer, containerAccess);
    if(Q_UNLIKELY(!link)) {
        returned = nullptr;
        containerAccess->dispose();
    }
    return returned;
}

jobject QtJambiAPI::convertQSetToJavaObject(JNIEnv *env,
                                            const QSharedPointer<char>& smartPointer,
                                            AbstractSetAccess* containerAccess
                                            )
{
    return ::convertQSetToJavaObject<QSharedPointer>(env, smartPointer, containerAccess);
}

jobject QtJambiAPI::convertQSetToJavaObject(JNIEnv *env,
                                            const std::shared_ptr<char>& smartPointer,
                                            AbstractSetAccess* containerAccess
                                            )
{
    return ::convertQSetToJavaObject<std::shared_ptr>(env, smartPointer, containerAccess);
}

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
jobject QtJambiAPI::convertQLinkedListToJavaObject(JNIEnv *env,
                                     QtJambiNativeID owner,
                                     const void* listPtr,
                                     CopyFunction copyFunction,
                                     PtrDeleterFunction deleter,
                                     AbstractLinkedListAccess* containerAccess
                                )
{
    Q_ASSERT(containerAccess);
    if(QtJambiAPI::fromNativeId(owner)==listPtr){
        jobject obj = CoreAPI::javaObject(owner, env);
        if(Java::QtCore::QLinkedList::isInstanceOf(env, obj)){
            containerAccess->dispose();
            return obj;
        }
    }
    for(const QSharedPointer<QtJambiLink>& link : QtJambiLink::findLinksForPointer(listPtr)){
        jobject obj = link->getJavaObjectLocalRef(env);
        if(Java::QtCore::QLinkedList::isInstanceOf(env, obj)){
            containerAccess->dispose();
            return obj;
        }
    }

    jobject returned = nullptr;
    CHECK_CONTAINER_ACCESS(env, containerAccess)
    returned = Java::QtCore::QLinkedList::newInstance(env, nullptr);
    QSharedPointer<QtJambiLink> link;
    QByteArray containerName = "QLinkedList<";
    containerName += containerAccess->elementMetaType().name();
    containerName += ">";
    containerName = QMetaObject::normalizedType(containerName);
    QMetaType containerMetaType(containerAccess->registerContainer(containerName));
    if(Q_UNLIKELY(!!owner)){
        link = QtJambiLink::createLinkForNativeObject(env, returned, const_cast<void*>(listPtr),
                                                        LINK_NAME_META_TYPE_ARG(containerMetaType)
                                                        owner, containerAccess);
    }else if(deleter){
        if(copyFunction){
            link = QtJambiLink::createLinkForNativeObject(env, returned, copyFunction(listPtr),
                                                       LINK_NAME_META_TYPE_ARG(containerMetaType)
                                                       false, true, deleter, containerAccess);
        }else{
            link = QtJambiLink::createLinkForNativeObject(env, returned, const_cast<void*>(listPtr),
                                                       LINK_NAME_META_TYPE_ARG(containerMetaType)
                                                       false, false, deleter, containerAccess);
        }
    }else{
        if(copyFunction){
            link = QtJambiLink::createLinkForNativeObject(env, returned, copyFunction(listPtr),
                                                       LINK_NAME_META_TYPE_ARG(containerMetaType)
                                                       false, true, containerAccess);
        }else{
            link = QtJambiLink::createLinkForNativeObject(env, returned, const_cast<void*>(listPtr),
                                                       LINK_NAME_META_TYPE_ARG(containerMetaType)
                                                       false, false, containerAccess);
        }
    }
    if(Q_UNLIKELY(!link)) {
        returned = nullptr;
        containerAccess->dispose();
    }
    return returned;
}

template<template<typename> class SmartPointer>
jobject convertQLinkedListToJavaObject(JNIEnv *env,
                                     const SmartPointer<char>& smartPointer,
                                     AbstractLinkedListAccess* containerAccess
                                )
{
    void* listPtr = smartPointer.get();
    if(Q_UNLIKELY(!listPtr))
        return nullptr;
    for(const QSharedPointer<QtJambiLink>& link : QtJambiLink::findLinksForPointer(listPtr)){
        if(Q_LIKELY(link)){
            jobject obj = link->getJavaObjectLocalRef(env);
            if(Java::QtCore::QLinkedList::isInstanceOf(env, obj)){
                containerAccess->dispose();
                return obj;
            }
        }
    }

    jobject returned = nullptr;
    CHECK_CONTAINER_ACCESS(env, containerAccess)
    returned = Java::QtCore::QLinkedList::newInstance(env, nullptr);
    QByteArray containerName = "QLinkedList<";
    containerName += containerAccess->elementMetaType().name();
    containerName += ">";
    containerName = QMetaObject::normalizedType(containerName);
    QMetaType containerMetaType(containerAccess->registerContainer(containerName));
    QSharedPointer<QtJambiLink> link = QtJambiLink::createLinkForSmartPointerToObject(env, returned,
                                                                                              LINK_NAME_META_TYPE_ARG(containerMetaType)
                                                                                              smartPointer, containerAccess);
    if(Q_UNLIKELY(!link)) {
        returned = nullptr;
        containerAccess->dispose();
    }
    return returned;
}

jobject QtJambiAPI::convertQLinkedListToJavaObject(JNIEnv *env,
                                            const QSharedPointer<char>& smartPointer,
                                            AbstractLinkedListAccess* containerAccess
                                            )
{
    return ::convertQLinkedListToJavaObject<QSharedPointer>(env, smartPointer, containerAccess);
}

jobject QtJambiAPI::convertQLinkedListToJavaObject(JNIEnv *env,
                                            const std::shared_ptr<char>& smartPointer,
                                            AbstractLinkedListAccess* containerAccess
                                            )
{
    return ::convertQLinkedListToJavaObject<std::shared_ptr>(env, smartPointer, containerAccess);
}

jobject QtJambiAPI::convertQVectorToJavaObject(JNIEnv *env,
                             QtJambiNativeID owner,
                             const void* listPtr,
                             CopyFunction copyFunction,
                             PtrDeleterFunction deleter,
                             VectorType vectorType,
                             AbstractVectorAccess* containerAccess)
{
    Q_ASSERT(containerAccess);
    if(QtJambiAPI::fromNativeId(owner)==listPtr){
        jobject obj = CoreAPI::javaObject(owner, env);
        if(vectorType==QtJambiAPI::VectorType::QStack){
            if(Java::QtCore::QStack::isInstanceOf(env, obj)){
                containerAccess->dispose();
                return obj;
            }
        }else{
            if(Java::QtCore::QVector::isInstanceOf(env, obj)){
                containerAccess->dispose();
                return obj;
            }
        }
    }
    for(const QSharedPointer<QtJambiLink>& link : QtJambiLink::findLinksForPointer(listPtr)){
        jobject obj = link->getJavaObjectLocalRef(env);
        if(vectorType==QtJambiAPI::VectorType::QStack){
            if(Java::QtCore::QStack::isInstanceOf(env, obj)){
                containerAccess->dispose();
                return obj;
            }
        }else{
            if(Java::QtCore::QVector::isInstanceOf(env, obj)){
                containerAccess->dispose();
                return obj;
            }
        }
    }

    jobject returned = nullptr;
    CHECK_CONTAINER_ACCESS(env, containerAccess)
    QByteArray containerName;
    if(vectorType==QtJambiAPI::VectorType::QStack){
        returned = Java::QtCore::QStack::newInstance(env, nullptr);
        containerName = "QStack<";
    }else {
        returned = Java::QtCore::QVector::newInstance(env, nullptr);
        containerName = "QVector<";
    }
    containerName += containerAccess->elementMetaType().name();
    containerName += ">";
    containerName = QMetaObject::normalizedType(containerName);
    QMetaType containerMetaType(containerAccess->registerContainer(containerName));
    QSharedPointer<QtJambiLink> link;
    if(Q_UNLIKELY(!!owner)){
        link = QtJambiLink::createLinkForNativeObject(env, returned, const_cast<void*>(listPtr),
                                                        LINK_NAME_META_TYPE_ARG(containerMetaType)
                                                        owner, containerAccess);
    }else if(deleter){
        if(copyFunction){
            link = QtJambiLink::createLinkForNativeObject(env, returned, copyFunction(listPtr),
                                                       LINK_NAME_META_TYPE_ARG(containerMetaType)
                                                       false, true, deleter, containerAccess);
        }else{
            link = QtJambiLink::createLinkForNativeObject(env, returned, const_cast<void*>(listPtr),
                                                       LINK_NAME_META_TYPE_ARG(containerMetaType)
                                                       false, false, deleter, containerAccess);
        }
    }else{
        if(copyFunction){
            link = QtJambiLink::createLinkForNativeObject(env, returned, copyFunction(listPtr),
                                                       LINK_NAME_META_TYPE_ARG(containerMetaType)
                                                       false, true, containerAccess);
        }else{
            link = QtJambiLink::createLinkForNativeObject(env, returned, const_cast<void*>(listPtr),
                                                       LINK_NAME_META_TYPE_ARG(containerMetaType)
                                                       false, false, containerAccess);
        }
    }
    if(Q_UNLIKELY(!link)) {
        returned = nullptr;
        containerAccess->dispose();
    }
    return returned;
}

jobject ContainerAPI::objectFromQVector(JNIEnv *env,
                             void*& listPtr,
                             AbstractVectorAccess*& containerAccess)
{
    for(const QSharedPointer<QtJambiLink>& link : QtJambiLink::findLinksForPointer(listPtr)){
        if(link && link->createdByJava()){
            jobject obj = link->getJavaObjectLocalRef(env);
            if(Java::QtCore::QVector::isInstanceOf(env, obj)){
                containerAccess->dispose();
                containerAccess = dynamic_cast<AbstractVectorAccess*>(link->containerAccess());
                listPtr = link->pointer();
                return obj;
            }
        }
    }
    QSharedPointer<QtJambiLink> link;
    jobject returned = nullptr;
    CHECK_CONTAINER_ACCESS(env, containerAccess)
    QByteArray containerName;
    returned = Java::QtCore::QVector::newInstance(env, nullptr);
    containerName = "QVector<";
    containerName += containerAccess->elementMetaType().name();
    containerName += ">";
    containerName = QMetaObject::normalizedType(containerName);
    QMetaType containerMetaType(containerAccess->registerContainer(containerName));
    link = QtJambiLink::createLinkForNativeObject(env, returned, const_cast<void*>(listPtr),
                                               LINK_NAME_META_TYPE_ARG(containerMetaType)
                                               false, false, containerAccess);
    if(Q_UNLIKELY(!link)) {
        returned = nullptr;
        containerAccess->dispose();
        containerAccess = nullptr;
        listPtr = nullptr;
    }
    return returned;
}

jobject ContainerAPI::objectFromQVector(JNIEnv *env,
                                      void*& listPtr,
                                      AbstractContainerAccess*& containerAccess
                                      )
{
    if(auto access = dynamic_cast<AbstractVectorAccess*>(containerAccess)){
        jobject result = objectFromQVector(env, listPtr, access);
        containerAccess = result ? access : nullptr;
        return result;
    }
    return nullptr;
}

template<template<typename> class SmartPointer>
jobject convertQVectorToJavaObject(JNIEnv *env,
                            const SmartPointer<char>& smartPointer,
                            QtJambiAPI::VectorType vectorType,
                            AbstractVectorAccess* containerAccess
                )
{
    void* listPtr = smartPointer.get();
    if(Q_UNLIKELY(!listPtr))
        return nullptr;
    for(const QSharedPointer<QtJambiLink>& link : QtJambiLink::findLinksForPointer(listPtr)){
        if(Q_LIKELY(link)){
            jobject obj = link->getJavaObjectLocalRef(env);
            if(vectorType==QtJambiAPI::VectorType::QStack){
                if(Java::QtCore::QStack::isInstanceOf(env, obj)){
                    containerAccess->dispose();
                    return obj;
                }
            }else{
                if(Java::QtCore::QVector::isInstanceOf(env, obj)){
                    containerAccess->dispose();
                    return obj;
                }
            }
        }
    }

    jobject returned = nullptr;
    CHECK_CONTAINER_ACCESS(env, containerAccess)
    QByteArray containerName;
    if(vectorType==QtJambiAPI::VectorType::QStack){
        returned = Java::QtCore::QStack::newInstance(env, nullptr);
        containerName = "QStack<";
    }else{
        returned = Java::QtCore::QVector::newInstance(env, nullptr);
        containerName = "QVector<";
    }
    containerName += containerAccess->elementMetaType().name();
    containerName += ">";
    containerName = QMetaObject::normalizedType(containerName);
    QMetaType containerMetaType(containerAccess->registerContainer(containerName));
    QSharedPointer<QtJambiLink> link = QtJambiLink::createLinkForSmartPointerToObject(env, returned,
                                                                                              LINK_NAME_META_TYPE_ARG(containerMetaType)
                                                                                              smartPointer, containerAccess);
    if(Q_UNLIKELY(!link)) {
        returned = nullptr;
        containerAccess->dispose();
    }
    return returned;
}

jobject QtJambiAPI::convertQVectorToJavaObject(JNIEnv *env,
                                             const QSharedPointer<char>& smartPointer,
                                             VectorType listType,
                                             AbstractVectorAccess* containerAccess
                                             ){
    return ::convertQVectorToJavaObject<QSharedPointer>(env,
                                                      smartPointer,
                                                      listType,
                                                      containerAccess
                                                      );
}

jobject QtJambiAPI::convertQVectorToJavaObject(JNIEnv *env,
                                             const std::shared_ptr<char>& smartPointer,
                                             VectorType listType,
                                             AbstractVectorAccess* containerAccess
                                             ){
    return ::convertQVectorToJavaObject<std::shared_ptr>(env,
                                                       smartPointer,
                                                       listType,
                                                       containerAccess
                                                       );
}

#endif

jobject QtJambiAPI::convertQHashToJavaObject(JNIEnv *env,
                                     QtJambiNativeID owner,
                                     const void* listPtr,
                                     CopyFunction copyFunction,
                                     PtrDeleterFunction deleter,
                                     AbstractHashAccess* containerAccess
                                )
{
    Q_ASSERT(containerAccess);
    if(QtJambiAPI::fromNativeId(owner)==listPtr){
        jobject obj = CoreAPI::javaObject(owner, env);
        if(Java::QtCore::QHash::isInstanceOf(env, obj)){
            containerAccess->dispose();
            return obj;
        }
    }
    for(const QSharedPointer<QtJambiLink>& link : QtJambiLink::findLinksForPointer(listPtr)){
        jobject obj = link->getJavaObjectLocalRef(env);
        if(Java::QtCore::QHash::isInstanceOf(env, obj)){
            containerAccess->dispose();
            return obj;
        }
    }
    jobject returned = nullptr;
    CHECK_CONTAINER_ACCESS(env, containerAccess)
    returned = Java::QtCore::QHash::newInstance(env, nullptr);
    QByteArray containerName = "QHash<";
    containerName += containerAccess->keyMetaType().name();
    containerName += ",";
    containerName += containerAccess->valueMetaType().name();
    containerName += ">";
    containerName = QMetaObject::normalizedType(containerName);
    QMetaType containerMetaType(containerAccess->registerContainer(containerName));
    QSharedPointer<QtJambiLink> link;
    if(Q_UNLIKELY(!!owner)){
        link = QtJambiLink::createLinkForNativeObject(env, returned, const_cast<void*>(listPtr),
                                                        LINK_NAME_META_TYPE_ARG(containerMetaType)
                                                        owner, containerAccess);
    }else if(deleter){
        if(copyFunction){
            link = QtJambiLink::createLinkForNativeObject(env, returned, copyFunction(listPtr),
                                                       LINK_NAME_META_TYPE_ARG(containerMetaType)
                                                       false, true, deleter, containerAccess);
        }else{
            link = QtJambiLink::createLinkForNativeObject(env, returned, const_cast<void*>(listPtr),
                                                       LINK_NAME_META_TYPE_ARG(containerMetaType)
                                                       false, false, deleter, containerAccess);
        }
    }else{
        if(copyFunction){
            link = QtJambiLink::createLinkForNativeObject(env, returned, copyFunction(listPtr),
                                                       LINK_NAME_META_TYPE_ARG(containerMetaType)
                                                       false, true, containerAccess);
        }else{
            link = QtJambiLink::createLinkForNativeObject(env, returned, const_cast<void*>(listPtr),
                                                       LINK_NAME_META_TYPE_ARG(containerMetaType)
                                                       false, false, containerAccess);
        }
    }
    if(Q_UNLIKELY(!link)) {
        returned = nullptr;
        containerAccess->dispose();
    }
    return returned;
}

template<template<typename> class SmartPointer>
jobject convertQHashToJavaObject(JNIEnv *env,
                                     const SmartPointer<char>& smartPointer,
                                     AbstractHashAccess* containerAccess
                                )
{
    void* listPtr = smartPointer.get();
    if(Q_UNLIKELY(!listPtr))
        return nullptr;
    for(const QSharedPointer<QtJambiLink>& link : QtJambiLink::findLinksForPointer(listPtr)){
        if(Q_LIKELY(link)){
            jobject obj = link->getJavaObjectLocalRef(env);
            if(Java::QtCore::QHash::isInstanceOf(env, obj)){
                containerAccess->dispose();
                return obj;
            }
        }
    }

    jobject returned = nullptr;
    CHECK_CONTAINER_ACCESS(env, containerAccess)
    returned = Java::QtCore::QHash::newInstance(env, nullptr);
    QByteArray containerName = "QHash<";
    containerName += containerAccess->keyMetaType().name();
    containerName += ",";
    containerName += containerAccess->valueMetaType().name();
    containerName += ">";
    containerName = QMetaObject::normalizedType(containerName);
    QMetaType containerMetaType(containerAccess->registerContainer(containerName));
    QSharedPointer<QtJambiLink> link = QtJambiLink::createLinkForSmartPointerToObject(env, returned,
                                                                                              LINK_NAME_META_TYPE_ARG(containerMetaType)
                                                                                              smartPointer, containerAccess);
    if(Q_UNLIKELY(!link)) {
        returned = nullptr;
        containerAccess->dispose();
    }
    return returned;
}

jobject QtJambiAPI::convertQHashToJavaObject(JNIEnv *env,
                                            const QSharedPointer<char>& smartPointer,
                                            AbstractHashAccess* containerAccess
                                            )
{
    return ::convertQHashToJavaObject<QSharedPointer>(env, smartPointer, containerAccess);
}

jobject QtJambiAPI::convertQHashToJavaObject(JNIEnv *env,
                                            const std::shared_ptr<char>& smartPointer,
                                            AbstractHashAccess* containerAccess
                                            )
{
    return ::convertQHashToJavaObject<std::shared_ptr>(env, smartPointer, containerAccess);
}

jobject QtJambiAPI::convertQMultiHashToJavaObject(JNIEnv *env,
                                     QtJambiNativeID owner,
                                     const void* listPtr,
                                     CopyFunction copyFunction,
                                     PtrDeleterFunction deleter,
                                     AbstractMultiHashAccess* containerAccess
                                )
{
    Q_ASSERT(containerAccess);
    if(QtJambiAPI::fromNativeId(owner)==listPtr){
        jobject obj = CoreAPI::javaObject(owner, env);
        if(Java::QtCore::QMultiHash::isInstanceOf(env, obj)){
            containerAccess->dispose();
            return obj;
        }
    }
    for(const QSharedPointer<QtJambiLink>& link : QtJambiLink::findLinksForPointer(listPtr)){
        jobject obj = link->getJavaObjectLocalRef(env);
        if(Java::QtCore::QMultiHash::isInstanceOf(env, obj)){
            containerAccess->dispose();
            return obj;
        }
    }

    jobject returned = nullptr;
    CHECK_CONTAINER_ACCESS(env, containerAccess)
    returned = Java::QtCore::QMultiHash::newInstance(env, nullptr);
    QSharedPointer<QtJambiLink> link;
    QByteArray containerName = "QMultiHash<";
    containerName += containerAccess->keyMetaType().name();
    containerName += ",";
    containerName += containerAccess->valueMetaType().name();
    containerName += ">";
    containerName = QMetaObject::normalizedType(containerName);
    QMetaType containerMetaType(containerAccess->registerContainer(containerName));
    if(Q_UNLIKELY(!!owner)){
        link = QtJambiLink::createLinkForNativeObject(env, returned, const_cast<void*>(listPtr),
                                                        LINK_NAME_META_TYPE_ARG(containerMetaType)
                                                        owner, containerAccess);
    }else if(deleter){
        if(copyFunction){
            link = QtJambiLink::createLinkForNativeObject(env, returned, copyFunction(listPtr),
                                                       LINK_NAME_META_TYPE_ARG(containerMetaType)
                                                       false, true, deleter, containerAccess);
        }else{
            link = QtJambiLink::createLinkForNativeObject(env, returned, const_cast<void*>(listPtr),
                                                       LINK_NAME_META_TYPE_ARG(containerMetaType)
                                                       false, false, deleter, containerAccess);
        }
    }else{
        if(copyFunction){
            link = QtJambiLink::createLinkForNativeObject(env, returned, copyFunction(listPtr),
                                                       LINK_NAME_META_TYPE_ARG(containerMetaType)
                                                       false, true, containerAccess);
        }else{
            link = QtJambiLink::createLinkForNativeObject(env, returned, const_cast<void*>(listPtr),
                                                       LINK_NAME_META_TYPE_ARG(containerMetaType)
                                                       false, false, containerAccess);
        }
    }
    if(Q_UNLIKELY(!link)) {
        returned = nullptr;
        containerAccess->dispose();
    }
    return returned;
}

template<template<typename> class SmartPointer>
jobject convertQMultiHashToJavaObject(JNIEnv *env,
                                     const SmartPointer<char>& smartPointer,
                                     AbstractMultiHashAccess* containerAccess
                                )
{
    void* listPtr = smartPointer.get();
    if(Q_UNLIKELY(!listPtr))
        return nullptr;
    for(const QSharedPointer<QtJambiLink>& link : QtJambiLink::findLinksForPointer(listPtr)){
        if(Q_LIKELY(link)){
            jobject obj = link->getJavaObjectLocalRef(env);
            if(Java::QtCore::QMultiHash::isInstanceOf(env, obj)){
                containerAccess->dispose();
                return obj;
            }
        }
    }

    jobject returned = nullptr;
    CHECK_CONTAINER_ACCESS(env, containerAccess)
    returned = Java::QtCore::QMultiHash::newInstance(env, nullptr);
    QByteArray containerName = "QMultiHash<";
    containerName += containerAccess->keyMetaType().name();
    containerName += ",";
    containerName += containerAccess->valueMetaType().name();
    containerName += ">";
    containerName = QMetaObject::normalizedType(containerName);
    QMetaType containerMetaType(containerAccess->registerContainer(containerName));
    QSharedPointer<QtJambiLink> link = QtJambiLink::createLinkForSmartPointerToObject(env, returned,
                                                                                              LINK_NAME_META_TYPE_ARG(containerMetaType)
                                                                                              smartPointer, containerAccess);
    if(Q_UNLIKELY(!link)) {
        returned = nullptr;
        containerAccess->dispose();
    }
    return returned;
}

jobject QtJambiAPI::convertQMultiHashToJavaObject(JNIEnv *env,
                                             const QSharedPointer<char>& smartPointer,
                                             AbstractMultiHashAccess* containerAccess
                                             )
{
    return ::convertQMultiHashToJavaObject<QSharedPointer>(env, smartPointer, containerAccess);
}

jobject QtJambiAPI::convertQMultiHashToJavaObject(JNIEnv *env,
                                             const std::shared_ptr<char>& smartPointer,
                                             AbstractMultiHashAccess* containerAccess
                                             )
{
    return ::convertQMultiHashToJavaObject<std::shared_ptr>(env, smartPointer, containerAccess);
}

jobject QtJambiAPI::convertQMapToJavaObject(JNIEnv *env,
                                     QtJambiNativeID owner,
                                     const void* listPtr,
                                     CopyFunction copyFunction,
                                     PtrDeleterFunction deleter,
                                     AbstractMapAccess* containerAccess
                                )
{
    Q_ASSERT(containerAccess);
    if(QtJambiAPI::fromNativeId(owner)==listPtr){
        jobject obj = CoreAPI::javaObject(owner, env);
        if(Java::QtCore::QMap::isInstanceOf(env, obj)){
            containerAccess->dispose();
            return obj;
        }
    }
    for(const QSharedPointer<QtJambiLink>& link : QtJambiLink::findLinksForPointer(listPtr)){
        jobject obj = link->getJavaObjectLocalRef(env);
        if(Java::QtCore::QMap::isInstanceOf(env, obj)){
            containerAccess->dispose();
            return obj;
        }
    }
    jobject returned = nullptr;
    CHECK_CONTAINER_ACCESS(env, containerAccess)
    returned = Java::QtCore::QMap::newInstance(env, nullptr);
    QByteArray containerName = "QMap<";
    containerName += containerAccess->keyMetaType().name();
    containerName += ",";
    containerName += containerAccess->valueMetaType().name();
    containerName += ">";
    containerName = QMetaObject::normalizedType(containerName);
    QMetaType containerMetaType(containerAccess->registerContainer(containerName));
    QSharedPointer<QtJambiLink> link;
    if(Q_UNLIKELY(!!owner)){
        link = QtJambiLink::createLinkForNativeObject(env, returned, const_cast<void*>(listPtr),
                                                        LINK_NAME_META_TYPE_ARG(containerMetaType)
                                                        owner, containerAccess);
    }else if(deleter){
        if(copyFunction){
            link = QtJambiLink::createLinkForNativeObject(env, returned, copyFunction(listPtr),
                                                       LINK_NAME_META_TYPE_ARG(containerMetaType)
                                                       false, true, deleter, containerAccess);
        }else{
            link = QtJambiLink::createLinkForNativeObject(env, returned, const_cast<void*>(listPtr),
                                                       LINK_NAME_META_TYPE_ARG(containerMetaType)
                                                       false, false, deleter, containerAccess);
        }
    }else{
        if(copyFunction){
            link = QtJambiLink::createLinkForNativeObject(env, returned, copyFunction(listPtr),
                                                       LINK_NAME_META_TYPE_ARG(containerMetaType)
                                                       false, true, containerAccess);
        }else{
            link = QtJambiLink::createLinkForNativeObject(env, returned, const_cast<void*>(listPtr),
                                                       LINK_NAME_META_TYPE_ARG(containerMetaType)
                                                       false, false, containerAccess);
        }
    }
    if(Q_UNLIKELY(!link)) {
        returned = nullptr;
        containerAccess->dispose();
    }
    return returned;
}

template<template<typename> class SmartPointer>
jobject convertQMapToJavaObject(JNIEnv *env,
                                     const SmartPointer<char>& smartPointer,
                                     AbstractMapAccess* containerAccess
                                )
{
    void* listPtr = smartPointer.get();
    if(Q_UNLIKELY(!listPtr))
        return nullptr;
    for(const QSharedPointer<QtJambiLink>& link : QtJambiLink::findLinksForPointer(listPtr)){
        if(Q_LIKELY(link)){
            jobject obj = link->getJavaObjectLocalRef(env);
            if(Java::QtCore::QMap::isInstanceOf(env, obj)){
                containerAccess->dispose();
                return obj;
            }
        }
    }

    jobject returned = nullptr;
    CHECK_CONTAINER_ACCESS(env, containerAccess)
    returned = Java::QtCore::QMap::newInstance(env, nullptr);
    QByteArray containerName = "QMap<";
    containerName += containerAccess->keyMetaType().name();
    containerName += ",";
    containerName += containerAccess->valueMetaType().name();
    containerName += ">";
    containerName = QMetaObject::normalizedType(containerName);
    QMetaType containerMetaType(containerAccess->registerContainer(containerName));
    QSharedPointer<QtJambiLink> link = QtJambiLink::createLinkForSmartPointerToObject(env, returned,
                                                                                                LINK_NAME_META_TYPE_ARG(containerMetaType)
                                                                                                smartPointer, containerAccess);
    if(Q_UNLIKELY(!link)) {
        returned = nullptr;
        containerAccess->dispose();
    }
    return returned;
}

jobject QtJambiAPI::convertQMapToJavaObject(JNIEnv *env,
                                                  const QSharedPointer<char>& smartPointer,
                                                  AbstractMapAccess* containerAccess
                                                  )
{
    return ::convertQMapToJavaObject<QSharedPointer>(env, smartPointer, containerAccess);
}

jobject QtJambiAPI::convertQMapToJavaObject(JNIEnv *env,
                                                  const std::shared_ptr<char>& smartPointer,
                                                  AbstractMapAccess* containerAccess
                                                  )
{
    return ::convertQMapToJavaObject<std::shared_ptr>(env, smartPointer, containerAccess);
}

jobject QtJambiAPI::convertQMultiMapToJavaObject(JNIEnv *env,
                                     QtJambiNativeID owner,
                                     const void* listPtr,
                                     CopyFunction copyFunction,
                                     PtrDeleterFunction deleter,
                                     AbstractMultiMapAccess* containerAccess
                                )
{
    Q_ASSERT(containerAccess);
    if(Q_UNLIKELY(QtJambiAPI::fromNativeId(owner)==listPtr)){
        jobject obj = CoreAPI::javaObject(owner, env);
        if(Java::QtCore::QMultiMap::isInstanceOf(env, obj)){
            containerAccess->dispose();
            return obj;
        }
    }
    for(const QSharedPointer<QtJambiLink>& link : QtJambiLink::findLinksForPointer(listPtr)){
        jobject obj = link->getJavaObjectLocalRef(env);
        if(Java::QtCore::QMultiMap::isInstanceOf(env, obj)){
            containerAccess->dispose();
            return obj;
        }
    }

    jobject returned = nullptr;
    CHECK_CONTAINER_ACCESS(env, containerAccess)
    returned = Java::QtCore::QMultiMap::newInstance(env, nullptr);
    QByteArray containerName = "QMultiMap<";
    containerName += containerAccess->keyMetaType().name();
    containerName += ",";
    containerName += containerAccess->valueMetaType().name();
    containerName += ">";
    containerName = QMetaObject::normalizedType(containerName);
    QMetaType containerMetaType(containerAccess->registerContainer(containerName));
    QSharedPointer<QtJambiLink> link;
    if(Q_UNLIKELY(!!owner)){
        link = QtJambiLink::createLinkForNativeObject(env, returned, const_cast<void*>(listPtr),
                                                        LINK_NAME_META_TYPE_ARG(containerMetaType)
                                                        owner, containerAccess);
    }else if(deleter){
        if(copyFunction){
            link = QtJambiLink::createLinkForNativeObject(env, returned, copyFunction(listPtr),
                                                       LINK_NAME_META_TYPE_ARG(containerMetaType)
                                                       false, true, deleter, containerAccess);
        }else{
            link = QtJambiLink::createLinkForNativeObject(env, returned, const_cast<void*>(listPtr),
                                                       LINK_NAME_META_TYPE_ARG(containerMetaType)
                                                       false, false, deleter, containerAccess);
        }
    }else{
        if(copyFunction){
            link = QtJambiLink::createLinkForNativeObject(env, returned, copyFunction(listPtr),
                                                       LINK_NAME_META_TYPE_ARG(containerMetaType)
                                                       false, true, containerAccess);
        }else{
            link = QtJambiLink::createLinkForNativeObject(env, returned, const_cast<void*>(listPtr),
                                                       LINK_NAME_META_TYPE_ARG(containerMetaType)
                                                       false, false, containerAccess);
        }
    }
    if(Q_UNLIKELY(!link)) {
        returned = nullptr;
        containerAccess->dispose();
    }
    return returned;
}

template<template<typename> class SmartPointer>
jobject convertQMultiMapToJavaObject(JNIEnv *env,
                                     const SmartPointer<char>& smartPointer,
                                     AbstractMultiMapAccess* containerAccess
                                )
{
    void* listPtr = smartPointer.get();
    if(Q_UNLIKELY(!listPtr))
        return nullptr;
    for(const QSharedPointer<QtJambiLink>& link : QtJambiLink::findLinksForPointer(listPtr)){
        if(Q_LIKELY(link)){
            jobject obj = link->getJavaObjectLocalRef(env);
            if(Java::QtCore::QMultiMap::isInstanceOf(env, obj)){
                containerAccess->dispose();
                return obj;
            }
        }
    }

    jobject returned = nullptr;
    CHECK_CONTAINER_ACCESS(env, containerAccess)
    returned = Java::QtCore::QMultiMap::newInstance(env, nullptr);
    QByteArray containerName = "QMultiMap<";
    containerName += containerAccess->keyMetaType().name();
    containerName += ",";
    containerName += containerAccess->valueMetaType().name();
    containerName += ">";
    containerName = QMetaObject::normalizedType(containerName);
    QMetaType containerMetaType(containerAccess->registerContainer(containerName));
    QSharedPointer<QtJambiLink> link = QtJambiLink::createLinkForSmartPointerToObject(env, returned,
                                                                                                LINK_NAME_META_TYPE_ARG(containerMetaType)
                                                                                                smartPointer, containerAccess);
    if(Q_UNLIKELY(!link)) {
        returned = nullptr;
        containerAccess->dispose();
    }
    return returned;
}

jobject QtJambiAPI::convertQMultiMapToJavaObject(JNIEnv *env,
                                            const QSharedPointer<char>& smartPointer,
                                            AbstractMultiMapAccess* containerAccess
                                            )
{
    return ::convertQMultiMapToJavaObject<QSharedPointer>(env, smartPointer, containerAccess);
}

jobject QtJambiAPI::convertQMultiMapToJavaObject(JNIEnv *env,
                                            const std::shared_ptr<char>& smartPointer,
                                            AbstractMultiMapAccess* containerAccess
                                            )
{
    return ::convertQMultiMapToJavaObject<std::shared_ptr>(env, smartPointer, containerAccess);
}

template<typename T>
T& checkedUnref(T* t){
    Q_ASSERT(t);
    return *t;
}

void CoreAPI::initializeQList(JNIEnv *env, jobject object, jclass elementType, QtJambiNativeID elementMetaTypeId, jobject other){
    QMetaType& elementMetaType = checkedUnref(QtJambiAPI::objectFromNativeId<QMetaType>(elementMetaTypeId));
    if(elementMetaType.id()==QMetaType::UnknownType)
        Java::Runtime::IllegalArgumentException::throwNew(env, QStringLiteral("QMetaType::UnknownType cannot be type of %1.").arg("QList") QTJAMBI_STACKTRACEINFO );
    if(elementMetaType.id()==QMetaType::Void)
        Java::Runtime::IllegalArgumentException::throwNew(env, QStringLiteral("void cannot be type of %1.").arg("QList") QTJAMBI_STACKTRACEINFO );
    const SuperTypeInfos superTypeInfos = SuperTypeInfos::fromClass(env, env->GetObjectClass(object));
    if(superTypeInfos.size()>1)
        Java::Runtime::Error::throwNew(env, QStringLiteral("It is not permitted to create a derived type of %1 implementing any Qt interface.").arg("QList") QTJAMBI_STACKTRACEINFO );
    using namespace QtJambiPrivate;
    AbstractListAccess* containerAccess = nullptr;
    bool isNativeContainer = false;
    if(Java::QtCore::QList::isInstanceOf(env, other)){
        if(QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForJavaObject(env, other)){
            containerAccess = dynamic_cast<AbstractListAccess*>(link->containerAccess());
            if(containerAccess){
                containerAccess = containerAccess->clone();
                isNativeContainer = true;
            }
        }else{
            Java::QtJambi::QNoNativeResourcesException::throwNew(env, QStringLiteral("Incomplete object of type: %1").arg(QtJambiAPI::getObjectClassName(env, other).replace("$", ".")) QTJAMBI_STACKTRACEINFO );
        }
    }
    if(!containerAccess){
        containerAccess = dynamic_cast<AbstractListAccess*>(ContainerAccessAPI::createContainerAccess(SequentialContainerType::QList, elementMetaType));
        if(!containerAccess){
            elementType = getGlobalClassRef(env, elementType);
            QByteArray qTypeName = elementMetaType.name();
            size_t size = size_t(elementMetaType.sizeOf());
            bool isPointer = AbstractContainerAccess::isPointerType(elementMetaType);
#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
            size_t align = size_t(elementMetaType.alignOf());
#else
            bool isStaticType = false;
            size_t align = 0;
            {
                const std::type_info* typeId = getTypeByMetaType(elementMetaType.id());
                if(!typeId)
                    typeId = getTypeByQtName(qTypeName);
                if(typeId){
                    if(isFunctional(*typeId)){
                        QString typeName = QtJambiAPI::typeName(*typeId);
                        if(!typeName.startsWith("std::function") && (typeName.contains("(*)"))){
                            isPointer = true;
                        }
                    }
                    if(OptionalBool op = isRegisteredAsStaticType(*typeId))
                        isStaticType = op.value();
                    if(!isPointer)
                         align = getValueAlignment(*typeId);
                    if(align==0 && !isPointer)
                        align = QtJambiTypeManager::getInternalAlignment(qTypeName);
                }
            }
            if(align==0 && !isPointer)
                align = QtJambiTypeManager::getInternalAlignment(elementMetaType.name());
#endif
            QSharedPointer<AbstractContainerAccess> elementNestedContainerAccess = findContainerAccess(elementMetaType);
            QtJambiUtils::QHashFunction hashFunction = QtJambiTypeManager::findHashFunction(isPointer, elementMetaType.id());
            QtJambiUtils::InternalToExternalConverter internalToExternalConverter = QtJambiTypeManager::getInternalToExternalConverter(
                                                                                                                env,
                                                                                                                QLatin1String(qTypeName),
                                                                                                                elementMetaType,
                                                                                                                elementType,
                                                                                                                true
                                                                                                            );
            QtJambiUtils::ExternalToInternalConverter externalToInternalConverter = QtJambiTypeManager::getExternalToInternalConverter(
                                                                                                                env,
                                                                                                                elementType,
                                                                                                                QLatin1String(qTypeName),
                                                                                                                elementMetaType
                                                                                                            );
            const std::type_info* typeId = getTypeByQtName(elementMetaType.name());
            if(!typeId){
                typeId = getTypeByMetaType(elementMetaType);
            }
            PtrOwnerFunction elementOwnerFunction = nullptr;
            if(typeId)
                elementOwnerFunction = ContainerAPI::registeredOwnerFunction(*typeId);
            containerAccess = dynamic_cast<AbstractListAccess*>(ContainerAccessAPI::createContainerAccess(
                                                                    env, SequentialContainerType::QList,
                                                                    elementMetaType,
                                                                    align, size,
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
                                                                    isStaticType,
#endif
                                                                    isPointer,
                                                                    hashFunction,
                                                                    internalToExternalConverter,
                                                                    externalToInternalConverter,
                                                                    elementNestedContainerAccess,
                                                                    elementOwnerFunction));
        }
        isNativeContainer = other && ContainerAPI::testQList(env, other, elementMetaType);
    }
    CHECK_CONTAINER_ACCESS(env, containerAccess)
    void* listPtr;
    if(isNativeContainer){
        if(QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForJavaObject(env, other)){
            listPtr = containerAccess->createContainer(env, ConstContainerAndAccessInfo{link->getJavaObjectLocalRef(env), link->pointer(), link->containerAccess()});
        }else{
            if(Java::QtJambi::QtObjectInterface::isInstanceOf(env, other)){
                containerAccess->dispose();
                Java::QtJambi::QNoNativeResourcesException::throwNew(env, QStringLiteral("Incomplete object of type: %1").arg(QtJambiAPI::getObjectClassName(env, other).replace("$", ".")) QTJAMBI_STACKTRACEINFO );
            }
            listPtr = containerAccess->createContainer();
        }
    }else{
        listPtr = containerAccess->createContainer();
    }
    QByteArray name;
    name = "QList<";
    name += containerAccess->elementMetaType().name();
    name += ">";
    name = QMetaObject::normalizedType(name);
    QMetaType containerMetaType(containerAccess->registerContainer(name));
    QSharedPointer<QtJambiLink> link = QtJambiLink::createLinkForNativeObject(env, object, listPtr,
                                                                                   LINK_NAME_META_TYPE_ARG(containerMetaType)
                                                                                   true, true, containerAccess);
    if(Q_UNLIKELY(!link)) {
        containerAccess->deleteContainer(listPtr);
        containerAccess->dispose();
    }else if(!isNativeContainer && other){
        jobject iter = QtJambiAPI::iteratorOfJavaIterable(env, other);
        jint idx = 0;
        containerAccess->reserve(env, {object, listPtr}, QtJambiAPI::sizeOfJavaCollection(env, other));
        while(QtJambiAPI::hasJavaIteratorNext(env, iter)){
            containerAccess->insert(env, {object, listPtr}, idx++, 1, QtJambiAPI::nextOfJavaIterator(env, iter));
        }
    }
}

void CoreAPI::initializeQSet(JNIEnv *env, jobject object, jclass elementType, QtJambiNativeID elementMetaTypeId, jobject other){
    QMetaType& elementMetaType = checkedUnref(QtJambiAPI::objectFromNativeId<QMetaType>(elementMetaTypeId));
    if(elementMetaType.id()==QMetaType::UnknownType)
        Java::Runtime::IllegalArgumentException::throwNew(env, QStringLiteral("QMetaType::UnknownType cannot be type of %1.").arg("QSet") QTJAMBI_STACKTRACEINFO );
    if(elementMetaType.id()==QMetaType::QVariant)
        Java::Runtime::IllegalArgumentException::throwNew(env, QStringLiteral("QVariant cannot be type of %1.").arg("QSet") QTJAMBI_STACKTRACEINFO );
    if(elementMetaType.id()==QMetaType::Void)
        Java::Runtime::IllegalArgumentException::throwNew(env, QStringLiteral("void cannot be type of %1.").arg("QSet") QTJAMBI_STACKTRACEINFO );
    const SuperTypeInfos superTypeInfos = SuperTypeInfos::fromClass(env, env->GetObjectClass(object));
    if(superTypeInfos.size()>1)
        Java::Runtime::Error::throwNew(env, QStringLiteral("It is not permitted to create a derived type of %1 implementing any Qt interface.").arg("QSet") QTJAMBI_STACKTRACEINFO );
    using namespace QtJambiPrivate;
    AbstractSetAccess* containerAccess = nullptr;
    bool isNativeContainer = false;
    if(Java::QtCore::QSet::isInstanceOf(env, other)){
        if(QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForJavaObject(env, other)){
            containerAccess = dynamic_cast<AbstractSetAccess*>(link->containerAccess());
            if(containerAccess){
                containerAccess = containerAccess->clone();
                isNativeContainer = true;
            }
        }else{
            Java::QtJambi::QNoNativeResourcesException::throwNew(env, QStringLiteral("Incomplete object of type: %1").arg(QtJambiAPI::getObjectClassName(env, other).replace("$", ".")) QTJAMBI_STACKTRACEINFO );
        }
    }
    if(!containerAccess){
        switch(elementMetaType.id()){
            case QMetaType::VoidStar:
                containerAccess = QSetAccess<void*>::newInstance();
                break;
            case QMetaType::Bool:
                containerAccess = QSetAccess<bool>::newInstance();
                break;
            case QMetaType::Char:
            case QMetaType::SChar:
            case QMetaType::UChar:
                containerAccess = QSetAccess<qint8>::newInstance();
                break;
            case QMetaType::Short:
            case QMetaType::UShort:
                containerAccess = QSetAccess<qint16>::newInstance();
                break;
            case QMetaType::Int:
            case QMetaType::UInt:
                containerAccess = QSetAccess<qint32>::newInstance();
                break;
            case QMetaType::LongLong:
            case QMetaType::ULongLong:
                containerAccess = QSetAccess<qint64>::newInstance();
                break;
            case QMetaType::Double:
                containerAccess = QSetAccess<double>::newInstance();
                break;
            case QMetaType::Float:
                containerAccess = QSetAccess<float>::newInstance();
                break;
            case QMetaType::QChar:
                containerAccess = QSetAccess<QChar>::newInstance();
                break;
            case QMetaType::QString:
                containerAccess = QSetAccess<QString>::newInstance();
                break;
            case QMetaType::QObjectStar:
                containerAccess = QSetAccess<QObject*>::newInstance();
                break;
            default: {
                containerAccess = dynamic_cast<AbstractSetAccess*>(ContainerAccessAPI::createContainerAccess(SequentialContainerType::QSet, elementMetaType));
                if(!containerAccess){
                    elementType = getGlobalClassRef(env, elementType);
                    QByteArray qTypeName = elementMetaType.name();
                    size_t size = size_t(elementMetaType.sizeOf());
                    QSharedPointer<AbstractContainerAccess> elementNestedContainerAccess = findContainerAccess(elementMetaType);
                    bool isPointer = AbstractContainerAccess::isPointerType(elementMetaType);
#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
                    size_t align = size_t(elementMetaType.alignOf());
#else
                    bool isStaticType = false;
                    size_t align = 0;
                    {
                        const std::type_info* typeId = getTypeByMetaType(elementMetaType.id());
                        if(!typeId)
                            typeId = getTypeByQtName(qTypeName);
                        if(typeId){
                            if(isFunctional(*typeId)){
                                QString typeName = QtJambiAPI::typeName(*typeId);
                                if(!typeName.startsWith("std::function") && (typeName.contains("(*)"))){
                                    isPointer = true;
                                }
                            }
                            if(OptionalBool op = isRegisteredAsStaticType(*typeId))
                                isStaticType = op.value();
                            if(!isPointer)
                                 align = getValueAlignment(*typeId);
                        }
                    }
                    if(align==0 && !isPointer)
                        align = QtJambiTypeManager::getInternalAlignment(elementMetaType.name());
#endif
                    QtJambiUtils::InternalToExternalConverter internalToExternalConverter = QtJambiTypeManager::getInternalToExternalConverter(
                                                                                                                        env,
                                                                                                                        QLatin1String(qTypeName),
                                                                                                                        elementMetaType,
                                                                                                                        elementType,
                                                                                                                        true
                                                                                                                    );
                    QtJambiUtils::ExternalToInternalConverter externalToInternalConverter = QtJambiTypeManager::getExternalToInternalConverter(
                                                                                                                        env,
                                                                                                                        elementType,
                                                                                                                        QLatin1String(qTypeName),
                                                                                                                        elementMetaType
                                                                                                                    );
                    QtJambiUtils::QHashFunction hashFunction = QtJambiTypeManager::findHashFunction(isPointer, elementMetaType.id());
                    if(!hashFunction){
                        Java::QtJambi::QNoImplementationException::throwNew(env, QString("Unable to create QSet of %1 because of missing hash function.").arg(QtJambiAPI::getClassName(env, elementType).replace("$", ".")) QTJAMBI_STACKTRACEINFO );
                    }
                    const std::type_info* typeId = getTypeByQtName(elementMetaType.name());
                    if(!typeId){
                        typeId = getTypeByMetaType(elementMetaType);
                    }
                    PtrOwnerFunction elementOwnerFunction = nullptr;
                    if(typeId)
                        elementOwnerFunction = ContainerAPI::registeredOwnerFunction(*typeId);
                    containerAccess = dynamic_cast<AbstractSetAccess*>(ContainerAccessAPI::createContainerAccess(
                                                                           env, SequentialContainerType::QSet,
                                                                           elementMetaType,
                                                                           align, size,
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
                                                                           isStaticType,
#endif
                                                                           isPointer,
                                                                           hashFunction,
                                                                           internalToExternalConverter,
                                                                           externalToInternalConverter,
                                                                           elementNestedContainerAccess,
                                                                           elementOwnerFunction));
                }
            }
            break;
        }
        isNativeContainer = other && ContainerAPI::testQSet(env, other, elementMetaType);
    }
    CHECK_CONTAINER_ACCESS(env, containerAccess)
    void* listPtr;
    if(isNativeContainer){
        if(QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForJavaObject(env, other)){
            listPtr = containerAccess->createContainer(env, ConstContainerAndAccessInfo{link->getJavaObjectLocalRef(env), link->pointer(), link->containerAccess()});
        }else{
            if(Java::QtJambi::QtObjectInterface::isInstanceOf(env, other)){
                containerAccess->dispose();
                Java::QtJambi::QNoNativeResourcesException::throwNew(env, QStringLiteral("Incomplete object of type: %1").arg(QtJambiAPI::getObjectClassName(env, other).replace("$", ".")) QTJAMBI_STACKTRACEINFO );
            }
            listPtr = containerAccess->createContainer();
        }
    }else{
        listPtr = containerAccess->createContainer();
    }
    QByteArray name = "QSet<";
    name += containerAccess->elementMetaType().name();
    name += ">";
    name = QMetaObject::normalizedType(name);
    QMetaType containerMetaType(containerAccess->registerContainer(name));
    QSharedPointer<QtJambiLink> link = QtJambiLink::createLinkForNativeObject(env, object, listPtr,
                                                                                   LINK_NAME_META_TYPE_ARG(containerMetaType)
                                                                                   true, true, containerAccess);
    if(Q_UNLIKELY(!link)) {
        containerAccess->deleteContainer(listPtr);
        containerAccess->dispose();
    }else if(!isNativeContainer && other){
        jobject iter = QtJambiAPI::iteratorOfJavaIterable(env, other);
        while(QtJambiAPI::hasJavaIteratorNext(env, iter)){
            containerAccess->insert(env, {object, listPtr}, QtJambiAPI::nextOfJavaIterator(env, iter));
        }
    }
}

#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
void CoreAPI::initializeQLinkedList(JNIEnv *env, jobject object, jclass elementType, QtJambiNativeID elementMetaTypeId, jobject other){
    QMetaType& elementMetaType = checkedUnref(QtJambiAPI::objectFromNativeId<QMetaType>(elementMetaTypeId));
    if(elementMetaType.id()==QMetaType::UnknownType)
        Java::Runtime::IllegalArgumentException::throwNew(env, QStringLiteral("QMetaType::UnknownType cannot be type of %1.").arg("QLinkedList") QTJAMBI_STACKTRACEINFO );
    if(elementMetaType.id()==QMetaType::Void)
        Java::Runtime::IllegalArgumentException::throwNew(env, QStringLiteral("void cannot be type of %1.").arg("QLinkedList") QTJAMBI_STACKTRACEINFO );
    const SuperTypeInfos superTypeInfos = SuperTypeInfos::fromClass(env, env->GetObjectClass(object));
    if(superTypeInfos.size()>1)
        Java::Runtime::Error::throwNew(env, QStringLiteral("It is not permitted to create a derived type of %1 implementing any Qt interface.").arg("QLinkedList") QTJAMBI_STACKTRACEINFO );
    using namespace QtJambiPrivate;
    AbstractLinkedListAccess* containerAccess = nullptr;
    bool isNativeContainer = false;
    if(Java::QtCore::QLinkedList::isInstanceOf(env, other)){
        if(QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForJavaObject(env, other)){
            containerAccess = dynamic_cast<AbstractLinkedListAccess*>(link->containerAccess());
            if(containerAccess){
                containerAccess = containerAccess->clone();
                isNativeContainer = true;
            }
        }else{
            Java::QtJambi::QNoNativeResourcesException::throwNew(env, QStringLiteral("Incomplete object of type: %1").arg(QtJambiAPI::getObjectClassName(env, other).replace("$", ".")) QTJAMBI_STACKTRACEINFO );
        }
    }
    if(!containerAccess){
        switch(elementMetaType.id()){
            case QMetaType::VoidStar:
                containerAccess = QLinkedListAccess<void*>::newInstance();
                break;
            case QMetaType::Bool:
                containerAccess = QLinkedListAccess<bool>::newInstance();
                break;
            case QMetaType::Char:
            case QMetaType::SChar:
            case QMetaType::UChar:
                containerAccess = QLinkedListAccess<qint8>::newInstance();
                break;
            case QMetaType::Short:
            case QMetaType::UShort:
                containerAccess = QLinkedListAccess<qint16>::newInstance();
                break;
            case QMetaType::Int:
            case QMetaType::UInt:
                containerAccess = QLinkedListAccess<qint32>::newInstance();
                break;
            case QMetaType::LongLong:
            case QMetaType::ULongLong:
                containerAccess = QLinkedListAccess<qint64>::newInstance();
                break;
            case QMetaType::Double:
                containerAccess = QLinkedListAccess<double>::newInstance();
                break;
            case QMetaType::Float:
                containerAccess = QLinkedListAccess<float>::newInstance();
                break;
            case QMetaType::QChar:
                containerAccess = QLinkedListAccess<QChar>::newInstance();
                break;
            case QMetaType::QString:
                containerAccess = QLinkedListAccess<QString>::newInstance();
                break;
            case QMetaType::QVariant:
                containerAccess = QLinkedListAccess<QVariant>::newInstance();
                break;
            case QMetaType::QObjectStar:
                containerAccess = QLinkedListAccess<QObject*>::newInstance();
                break;
            default: {
                containerAccess = dynamic_cast<AbstractLinkedListAccess*>(ContainerAccessAPI::createContainerAccess(SequentialContainerType::QLinkedList, elementMetaType));
                if(!containerAccess){
                    elementType = getGlobalClassRef(env, elementType);
                    size_t size = size_t(elementMetaType.sizeOf());
                    bool isStaticType = false;
                    bool isPointer = AbstractContainerAccess::isPointerType(elementMetaType);
                    size_t align = 0;
                    {
                        const std::type_info* typeId = getTypeByMetaType(elementMetaType.id());
                        if(!typeId)
                            typeId = getTypeByQtName(elementMetaType.name());
                        if(typeId){
                            if(isFunctional(*typeId)){
                                QString typeName = QtJambiAPI::typeName(*typeId);
                                if(!typeName.startsWith("std::function") && (typeName.contains("(*)"))){
                                    isPointer = true;
                                }
                            }
                            if(OptionalBool op = isRegisteredAsStaticType(*typeId))
                                isStaticType = op.value();
                            if(!isPointer)
                                 align = getValueAlignment(*typeId);
                        }
                    }
                    if(align==0 && !isPointer)
                        align = QtJambiTypeManager::getInternalAlignment(elementMetaType.name());
                    QSharedPointer<AbstractContainerAccess> elementNestedContainerAccess = findContainerAccess(elementMetaType);
                    QtJambiUtils::QHashFunction hashFunction = QtJambiTypeManager::findHashFunction(isPointer, elementMetaType.id());
                    QtJambiUtils::InternalToExternalConverter internalToExternalConverter = QtJambiTypeManager::getInternalToExternalConverter(
                                                                                                                        env,
                                                                                                                        QLatin1String(elementMetaType.name()),
                                                                                                                        elementMetaType,
                                                                                                                        elementType,
                                                                                                                        true
                                                                                                                    );
                    QtJambiUtils::ExternalToInternalConverter externalToInternalConverter = QtJambiTypeManager::getExternalToInternalConverter(
                                                                                                                        env,
                                                                                                                        elementType,
                                                                                                                        QLatin1String(elementMetaType.name()),
                                                                                                                        elementMetaType
                                                                                                                    );
                    const std::type_info* typeId = getTypeByQtName(elementMetaType.name());
                    if(!typeId){
                        typeId = getTypeByMetaType(elementMetaType);
                    }
                    PtrOwnerFunction elementOwnerFunction = nullptr;
                    if(typeId)
                        elementOwnerFunction = ContainerAPI::registeredOwnerFunction(*typeId);
                    containerAccess = dynamic_cast<AbstractLinkedListAccess*>(ContainerAccessAPI::createContainerAccess(
                                                                                  env, SequentialContainerType::QLinkedList,
                                                                                  elementMetaType,
                                                                                  align, size, isStaticType,
                                                                                  isPointer,
                                                                                  hashFunction,
                                                                                  internalToExternalConverter,
                                                                                  externalToInternalConverter,
                                                                                  elementNestedContainerAccess,
                                                                                  elementOwnerFunction));
                }
            }
        }
        isNativeContainer = other && ContainerAPI::testQLinkedList(env, other, elementMetaType);
    }
    CHECK_CONTAINER_ACCESS(env, containerAccess)
    void* listPtr;
    if(isNativeContainer){
        if(QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForJavaObject(env, other)){
            listPtr = containerAccess->createContainer(env, ConstContainerAndAccessInfo{link->getJavaObjectLocalRef(env), link->pointer(), link->containerAccess()});
        }else{
            if(Java::QtJambi::QtObjectInterface::isInstanceOf(env, other)){
                containerAccess->dispose();
                Java::QtJambi::QNoNativeResourcesException::throwNew(env, QStringLiteral("Incomplete object of type: %1").arg(QtJambiAPI::getObjectClassName(env, other).replace("$", ".")) QTJAMBI_STACKTRACEINFO );
            }
            listPtr = containerAccess->createContainer();
        }
    }else{
        listPtr = containerAccess->createContainer();
    }
    QByteArray name = "QLinkedList<";
    name += containerAccess->elementMetaType().name();
    name += ">";
    name = QMetaObject::normalizedType(name);
    QMetaType containerMetaType(containerAccess->registerContainer(name));
    QSharedPointer<QtJambiLink> link = QtJambiLink::createLinkForNativeObject(env, object, listPtr,
                                                                                   LINK_NAME_META_TYPE_ARG(containerMetaType)
                                                                                   true, true, containerAccess);
    if(Q_UNLIKELY(!link)) {
        containerAccess->deleteContainer(listPtr);
        containerAccess->dispose();
    }else if(!isNativeContainer && other){
        jobject iter = QtJambiAPI::iteratorOfJavaIterable(env, other);
        while(QtJambiAPI::hasJavaIteratorNext(env, iter)){
            containerAccess->append(env, {object, listPtr}, QtJambiAPI::nextOfJavaIterator(env, iter));
        }
    }
}

void CoreAPI::initializeQVector(JNIEnv *env, jobject object, jclass elementType, QtJambiNativeID elementMetaTypeId, jobject other){
    QMetaType& elementMetaType = checkedUnref(QtJambiAPI::objectFromNativeId<QMetaType>(elementMetaTypeId));
    if(elementMetaType.id()==QMetaType::UnknownType)
        Java::Runtime::IllegalArgumentException::throwNew(env, QStringLiteral("QMetaType::UnknownType cannot be type of %1.").arg("QVector") QTJAMBI_STACKTRACEINFO );
    if(elementMetaType.id()==QMetaType::Void)
        Java::Runtime::IllegalArgumentException::throwNew(env, QStringLiteral("void cannot be type of %1.").arg("QVector") QTJAMBI_STACKTRACEINFO );
    const SuperTypeInfos superTypeInfos = SuperTypeInfos::fromClass(env, env->GetObjectClass(object));
    if(superTypeInfos.size()>1)
        Java::Runtime::Error::throwNew(env, QStringLiteral("It is not permitted to create a derived type of %1 implementing any Qt interface.").arg("QVector") QTJAMBI_STACKTRACEINFO );
    using namespace QtJambiPrivate;
    AbstractVectorAccess* containerAccess = nullptr;
    bool isNativeContainer = false;
    if(Java::QtCore::QVector::isInstanceOf(env, other)){
        if(QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForJavaObject(env, other)){
            containerAccess = dynamic_cast<AbstractVectorAccess*>(link->containerAccess());
            if(containerAccess){
                containerAccess = containerAccess->clone();
                isNativeContainer  = true;
            }
        }else{
            Java::QtJambi::QNoNativeResourcesException::throwNew(env, QStringLiteral("Incomplete object of type: %1").arg(QtJambiAPI::getObjectClassName(env, other).replace("$", ".")) QTJAMBI_STACKTRACEINFO );
        }
    }
    if(!containerAccess){
        switch(elementMetaType.id()){
            case QMetaType::VoidStar:
                containerAccess = QVectorAccess<void*>::newInstance();
                break;
            case QMetaType::Bool:
                containerAccess = QVectorAccess<bool>::newInstance();
                break;
            case QMetaType::Char:
            case QMetaType::SChar:
            case QMetaType::UChar:
                containerAccess = QVectorAccess<qint8>::newInstance();
                break;
            case QMetaType::Short:
            case QMetaType::UShort:
                containerAccess = QVectorAccess<qint16>::newInstance();
                break;
            case QMetaType::Int:
            case QMetaType::UInt:
                containerAccess = QVectorAccess<qint32>::newInstance();
                break;
            case QMetaType::LongLong:
            case QMetaType::ULongLong:
                containerAccess = QVectorAccess<qint64>::newInstance();
                break;
            case QMetaType::Double:
                containerAccess = QVectorAccess<double>::newInstance();
                break;
            case QMetaType::Float:
                containerAccess = QVectorAccess<float>::newInstance();
                break;
            case QMetaType::QChar:
                containerAccess = QVectorAccess<QChar>::newInstance();
                break;
            case QMetaType::QString:
                containerAccess = QVectorAccess<QString>::newInstance();
                break;
            case QMetaType::QVariant:
                containerAccess = QVectorAccess<QVariant>::newInstance();
                break;
            case QMetaType::QObjectStar:
                containerAccess = QVectorAccess<QObject*>::newInstance();
                break;
            default: {
                containerAccess = dynamic_cast<AbstractVectorAccess*>(ContainerAccessAPI::createContainerAccess(SequentialContainerType::QVector, elementMetaType));
                if(!containerAccess){
                    elementType = getGlobalClassRef(env, elementType);
                    size_t size = size_t(elementMetaType.sizeOf());
                    bool isStaticType = false;
                    bool isPointer = AbstractContainerAccess::isPointerType(elementMetaType);
                    size_t align = 0;
                    {
                        const std::type_info* typeId = getTypeByMetaType(elementMetaType.id());
                        if(!typeId)
                            typeId = getTypeByQtName(elementMetaType.name());
                        if(typeId){
                            if(isFunctional(*typeId)){
                                QString typeName = QtJambiAPI::typeName(*typeId);
                                if(!typeName.startsWith("std::function") && (typeName.contains("(*)"))){
                                    isPointer = true;
                                }
                            }
                            if(OptionalBool op = isRegisteredAsStaticType(*typeId))
                                isStaticType = op.value();
                            if(!isPointer)
                                 align = getValueAlignment(*typeId);
                        }
                    }
                    if(align==0 && !isPointer)
                        align = QtJambiTypeManager::getInternalAlignment(elementMetaType.name());
                    QSharedPointer<AbstractContainerAccess> elementNestedContainerAccess = findContainerAccess(elementMetaType);
                    QtJambiUtils::InternalToExternalConverter internalToExternalConverter = QtJambiTypeManager::getInternalToExternalConverter(
                                                                                                                        env,
                                                                                                                        QLatin1String(elementMetaType.name()),
                                                                                                                        elementMetaType,
                                                                                                                        elementType,
                                                                                                                        true
                                                                                                                    );
                    QtJambiUtils::ExternalToInternalConverter externalToInternalConverter = QtJambiTypeManager::getExternalToInternalConverter(
                                                                                                                        env,
                                                                                                                        elementType,
                                                                                                                        QLatin1String(elementMetaType.name()),
                                                                                                                        elementMetaType
                                                                                                                    );
                    QtJambiUtils::QHashFunction hashFunction = QtJambiTypeManager::findHashFunction(isPointer, elementMetaType.id());
                    const std::type_info* typeId = getTypeByQtName(elementMetaType.name());
                    if(!typeId){
                        typeId = getTypeByMetaType(elementMetaType);
                    }
                    PtrOwnerFunction elementOwnerFunction = nullptr;
                    if(typeId)
                        elementOwnerFunction = ContainerAPI::registeredOwnerFunction(*typeId);
                    containerAccess = dynamic_cast<AbstractVectorAccess*>(ContainerAccessAPI::createContainerAccess(
                                                                              env, SequentialContainerType::QVector,
                                                                              elementMetaType,
                                                                              align, size, isStaticType,
                                                                              isPointer,
                                                                              hashFunction,
                                                                              internalToExternalConverter,
                                                                              externalToInternalConverter,
                                                                              elementNestedContainerAccess,
                                                                              elementOwnerFunction));
                }
            }
            break;
        }
        isNativeContainer = other && ContainerAPI::testQVector(env, other, elementMetaType);
    }
    CHECK_CONTAINER_ACCESS(env, containerAccess)
    void* listPtr;
    if(isNativeContainer){
        if(QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForJavaObject(env, other)){
            listPtr = containerAccess->createContainer(env, ConstContainerAndAccessInfo{link->getJavaObjectLocalRef(env), link->pointer(), link->containerAccess()});
        }else{
            if(Java::QtJambi::QtObjectInterface::isInstanceOf(env, other)){
                containerAccess->dispose();
                Java::QtJambi::QNoNativeResourcesException::throwNew(env, QStringLiteral("Incomplete object of type: %1").arg(QtJambiAPI::getObjectClassName(env, other).replace("$", ".")) QTJAMBI_STACKTRACEINFO );
            }
            listPtr = containerAccess->createContainer();
        }
    }else{
        listPtr = containerAccess->createContainer();
    }
    QByteArray name = "QVector<";
    name += containerAccess->elementMetaType().name();
    name += ">";
    name = QMetaObject::normalizedType(name);
    QMetaType containerMetaType(containerAccess->registerContainer(name));
    QSharedPointer<QtJambiLink> link = QtJambiLink::createLinkForNativeObject(env, object, listPtr,
                                                                                   LINK_NAME_META_TYPE_ARG(containerMetaType)
                                                                                   true, true, containerAccess);
    if(Q_UNLIKELY(!link)) {
        containerAccess->deleteContainer(listPtr);
        containerAccess->dispose();
    }else if(!isNativeContainer && other){
        jobject iter = QtJambiAPI::iteratorOfJavaIterable(env, other);
        jint idx = 0;
        containerAccess->reserve(env, {object, listPtr}, QtJambiAPI::sizeOfJavaCollection(env, other));
        while(QtJambiAPI::hasJavaIteratorNext(env, iter)){
            containerAccess->insert(env, {object, listPtr}, idx++, 1, QtJambiAPI::nextOfJavaIterator(env, iter));
        }
    }
}
#endif

void CoreAPI::initializeQHash(JNIEnv *env, jobject object, jclass keyType, QtJambiNativeID keyMetaTypeId, jclass valueType, QtJambiNativeID valueMetaTypeId, jobject other){
    QMetaType& keyMetaType = checkedUnref(QtJambiAPI::objectFromNativeId<QMetaType>(keyMetaTypeId));
    QMetaType& valueMetaType = checkedUnref(QtJambiAPI::objectFromNativeId<QMetaType>(valueMetaTypeId));
    if(keyMetaType.id()==QMetaType::UnknownType)
        Java::Runtime::IllegalArgumentException::throwNew(env, QStringLiteral("QMetaType::UnknownType cannot be key type of %1.").arg("QHash") QTJAMBI_STACKTRACEINFO );
    if(keyMetaType.id()==QMetaType::Void)
        Java::Runtime::IllegalArgumentException::throwNew(env, QStringLiteral("void cannot be key type of %1.").arg("QHash") QTJAMBI_STACKTRACEINFO );
    if(valueMetaType.id()==QMetaType::UnknownType)
        Java::Runtime::IllegalArgumentException::throwNew(env, QStringLiteral("QMetaType::UnknownType cannot be value type of %1.").arg("QHash") QTJAMBI_STACKTRACEINFO );
    if(valueMetaType.id()==QMetaType::Void)
        Java::Runtime::IllegalArgumentException::throwNew(env, QStringLiteral("void cannot be value type of %1.").arg("QHash") QTJAMBI_STACKTRACEINFO );
    const SuperTypeInfos superTypeInfos = SuperTypeInfos::fromClass(env, env->GetObjectClass(object));
    if(superTypeInfos.size()>1)
        Java::Runtime::Error::throwNew(env, QStringLiteral("It is not permitted to create a derived type of %1 implementing any Qt interface.").arg("QHash") QTJAMBI_STACKTRACEINFO );
    keyType = getGlobalClassRef(env, keyType);
    valueType = getGlobalClassRef(env, valueType);
    using namespace QtJambiPrivate;
    AbstractHashAccess* containerAccess = nullptr;
    bool isNativeContainer = false;
    if(Java::QtCore::QHash::isInstanceOf(env, other)){
        if(QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForJavaObject(env, other)){
            containerAccess = dynamic_cast<AbstractHashAccess*>(link->containerAccess());
            if(containerAccess){
                containerAccess = containerAccess->clone();
                isNativeContainer = true;
            }
        }else{
            Java::QtJambi::QNoNativeResourcesException::throwNew(env, QStringLiteral("Incomplete object of type: %1").arg(QtJambiAPI::getObjectClassName(env, other).replace("$", ".")) QTJAMBI_STACKTRACEINFO );
        }
    }
    if(!containerAccess)
        containerAccess = dynamic_cast<AbstractHashAccess*>(ContainerAccessAPI::createContainerAccess(AssociativeContainerType::QHash, keyMetaType, valueMetaType));
    if(!containerAccess){
        size_t size1 = size_t(keyMetaType.sizeOf());
        bool isPointer1 = AbstractContainerAccess::isPointerType(keyMetaType);
    #if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
        size_t align1 = size_t(keyMetaType.alignOf());
    #else
        size_t align1 = 0;
        {
            const std::type_info* typeId = getTypeByMetaType(keyMetaType.id());
            if(!typeId)
                typeId = getTypeByQtName(keyMetaType.name());
            if(typeId){
                if(isFunctional(*typeId)){
                    QString typeName = QtJambiAPI::typeName(*typeId);
                    if(!typeName.startsWith("std::function") && (typeName.contains("(*)"))){
                        isPointer1 = true;
                    }
                }
                if(!isPointer1)
                     align1 = getValueAlignment(*typeId);
            }
        }
        if(align1==0 && !isPointer1)
            align1 = QtJambiTypeManager::getInternalAlignment(keyMetaType.name());
    #endif

        size_t size2 = size_t(valueMetaType.sizeOf());
        bool isPointer2 = AbstractContainerAccess::isPointerType(valueMetaType);
    #if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
        size_t align2 = size_t(valueMetaType.alignOf());
    #else
        size_t align2 = 0;
        {
            const std::type_info* typeId = getTypeByMetaType(valueMetaType.id());
            if(!typeId)
                typeId = getTypeByQtName(valueMetaType.name());
            if(typeId){
                if(isFunctional(*typeId)){
                    QString typeName = QtJambiAPI::typeName(*typeId);
                    if(!typeName.startsWith("std::function") && (typeName.contains("(*)"))){
                        isPointer2 = true;
                    }
                }
                if(!isPointer2)
                     align2 = getValueAlignment(*typeId);
            }
        }
        if(align2==0 && !isPointer2)
            align2 = QtJambiTypeManager::getInternalAlignment(valueMetaType.name());
    #endif

        QtJambiUtils::InternalToExternalConverter keyInternalToExternalConverter = QtJambiTypeManager::getInternalToExternalConverter(
                                                                                                            env,
                                                                                                            QLatin1String(keyMetaType.name()),
                                                                                                            keyMetaType,
                                                                                                            keyType,
                                                                                                            true
                                                                                                        );
        QtJambiUtils::ExternalToInternalConverter keyExternalToInternalConverter = QtJambiTypeManager::getExternalToInternalConverter(
                                                                                                            env,
                                                                                                            keyType,
                                                                                                            QLatin1String(keyMetaType.name()),
                                                                                                            keyMetaType
                                                                                                        );
        QtJambiUtils::InternalToExternalConverter valueInternalToExternalConverter = QtJambiTypeManager::getInternalToExternalConverter(
                                                                                                            env,
                                                                                                            QLatin1String(valueMetaType.name()),
                                                                                                            valueMetaType,
                                                                                                            valueType,
                                                                                                            true
                                                                                                        );
        QtJambiUtils::ExternalToInternalConverter valueExternalToInternalConverter = QtJambiTypeManager::getExternalToInternalConverter(
                                                                                                            env,
                                                                                                            valueType,
                                                                                                            QLatin1String(valueMetaType.name()),
                                                                                                            valueMetaType
                                                                                                        );
        QtJambiUtils::QHashFunction hashFunction1 = QtJambiTypeManager::findHashFunction(isPointer1, keyMetaType.id());
        QtJambiUtils::QHashFunction hashFunction2 = QtJambiTypeManager::findHashFunction(isPointer2, valueMetaType.id());
        if(!hashFunction1){
            Java::QtJambi::QNoImplementationException::throwNew(env, QString("Unable to create QHash for %1 because of missing hash function.").arg(QtJambiAPI::getClassName(env, keyType).replace("$", ".")) QTJAMBI_STACKTRACEINFO );
        }
        QSharedPointer<AbstractContainerAccess> keyNestedContainerAccess = findContainerAccess(keyMetaType);
        QSharedPointer<AbstractContainerAccess> valueNestedContainerAccess = findContainerAccess(valueMetaType);
        const std::type_info* typeId = getTypeByQtName(keyMetaType.name());
        if(!typeId){
            typeId = getTypeByMetaType(keyMetaType);
        }
        PtrOwnerFunction keyOwnerFunction = nullptr;
        if(typeId)
            keyOwnerFunction = ContainerAPI::registeredOwnerFunction(*typeId);
        typeId = getTypeByQtName(qPrintable(valueMetaType.name()));
        if(!typeId){
            typeId = getTypeByMetaType(valueMetaType);
        }
        PtrOwnerFunction valueOwnerFunction = nullptr;
        if(typeId)
            valueOwnerFunction = ContainerAPI::registeredOwnerFunction(*typeId);
        containerAccess = dynamic_cast<AbstractHashAccess*>(ContainerAccessAPI::createContainerAccess(
                                                                    env, AssociativeContainerType::QHash,
                                                                    keyMetaType,
                                                                    align1, size1,
                                                                    isPointer1,
                                                                    hashFunction1,
                                                                    keyInternalToExternalConverter,
                                                                    keyExternalToInternalConverter,
                                                                    keyNestedContainerAccess,
                                                                    keyOwnerFunction,
                                                                    valueMetaType,
                                                                    align2, size2,
                                                                    isPointer2,
                                                                    hashFunction2,
                                                                    valueInternalToExternalConverter,
                                                                    valueExternalToInternalConverter,
                                                                    valueNestedContainerAccess,
                                                                    valueOwnerFunction));
        isNativeContainer = other && ContainerAPI::testQHash(env, other, keyMetaType, valueMetaType);
    }
    CHECK_CONTAINER_ACCESS(env, containerAccess)
    void* listPtr;
    if(isNativeContainer){
        if(QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForJavaObject(env, other)){
            listPtr = containerAccess->createContainer(env, ConstContainerAndAccessInfo{link->getJavaObjectLocalRef(env), link->pointer(), link->containerAccess()});
        }else{
            if(Java::QtJambi::QtObjectInterface::isInstanceOf(env, other)){
                containerAccess->dispose();
                Java::QtJambi::QNoNativeResourcesException::throwNew(env, QStringLiteral("Incomplete object of type: %1").arg(QtJambiAPI::getObjectClassName(env, other).replace("$", ".")) QTJAMBI_STACKTRACEINFO );
            }
            listPtr = containerAccess->createContainer();
        }
    }else{
        listPtr = containerAccess->createContainer();
    }
    QByteArray name = "QHash<";
    name += containerAccess->keyMetaType().name();
    name += ",";
    name += containerAccess->valueMetaType().name();
    name += ">";
    name = QMetaObject::normalizedType(name);
    QMetaType containerMetaType(containerAccess->registerContainer(name));
    QSharedPointer<QtJambiLink> link = QtJambiLink::createLinkForNativeObject(env, object, listPtr,
                                                                                   LINK_NAME_META_TYPE_ARG(containerMetaType)
                                                                                   true, true, containerAccess);
    if(Q_UNLIKELY(!link)) {
        containerAccess->deleteContainer(listPtr);
        containerAccess->dispose();
    }else if(!isNativeContainer && other){
        jobject iter = QtJambiAPI::entrySetIteratorOfJavaMap(env, other);
        while(QtJambiAPI::hasJavaIteratorNext(env, iter)){
            jobject entry = QtJambiAPI::nextOfJavaIterator(env, iter);
            containerAccess->insert(env, {object, listPtr}, QtJambiAPI::keyOfJavaMapEntry(env, entry), QtJambiAPI::valueOfJavaMapEntry(env, entry));
        }
    }
}

void CoreAPI::initializeQMultiHash(JNIEnv *env, jobject object, jclass keyType, QtJambiNativeID keyMetaTypeId, jclass valueType, QtJambiNativeID valueMetaTypeId, jobject other){
    QMetaType& keyMetaType = checkedUnref(QtJambiAPI::objectFromNativeId<QMetaType>(keyMetaTypeId));
    QMetaType& valueMetaType = checkedUnref(QtJambiAPI::objectFromNativeId<QMetaType>(valueMetaTypeId));
    if(keyMetaType.id()==QMetaType::UnknownType)
        Java::Runtime::IllegalArgumentException::throwNew(env, QStringLiteral("QMetaType::UnknownType cannot be key type of %1.").arg("QMultiHash") QTJAMBI_STACKTRACEINFO );
    if(keyMetaType.id()==QMetaType::Void)
        Java::Runtime::IllegalArgumentException::throwNew(env, QStringLiteral("void cannot be key type of %1.").arg("QMultiHash") QTJAMBI_STACKTRACEINFO );
    if(valueMetaType.id()==QMetaType::UnknownType)
        Java::Runtime::IllegalArgumentException::throwNew(env, QStringLiteral("QMetaType::UnknownType cannot be value type of %1.").arg("QMultiHash") QTJAMBI_STACKTRACEINFO );
    if(valueMetaType.id()==QMetaType::Void)
        Java::Runtime::IllegalArgumentException::throwNew(env, QStringLiteral("void cannot be value type of %1.").arg("QMultiHash") QTJAMBI_STACKTRACEINFO );
    const SuperTypeInfos superTypeInfos = SuperTypeInfos::fromClass(env, env->GetObjectClass(object));
    if(superTypeInfos.size()>1)
        Java::Runtime::Error::throwNew(env, QStringLiteral("It is not permitted to create a derived type of %1 implementing any Qt interface.").arg("QMultiHash") QTJAMBI_STACKTRACEINFO );
    keyType = getGlobalClassRef(env, keyType);
    valueType = getGlobalClassRef(env, valueType);
    using namespace QtJambiPrivate;
    bool isNativeContainer = false;
    AbstractMultiHashAccess* containerAccess = nullptr;
    if(Java::QtCore::QMultiHash::isInstanceOf(env, other)){
        if(QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForJavaObject(env, other)){
            containerAccess = dynamic_cast<AbstractMultiHashAccess*>(link->containerAccess());
            if(containerAccess){
                containerAccess = containerAccess->clone();
                isNativeContainer = true;
            }
        }else{
            Java::QtJambi::QNoNativeResourcesException::throwNew(env, QStringLiteral("Incomplete object of type: %1").arg(QtJambiAPI::getObjectClassName(env, other).replace("$", ".")) QTJAMBI_STACKTRACEINFO );
        }
    }
    if(!containerAccess)
        containerAccess = dynamic_cast<AbstractMultiHashAccess*>(ContainerAccessAPI::createContainerAccess(AssociativeContainerType::QMultiHash, keyMetaType, valueMetaType));
    if(!containerAccess){
        size_t size1 = size_t(keyMetaType.sizeOf());
        bool isPointer1 = AbstractContainerAccess::isPointerType(keyMetaType);
    #if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
        size_t align1 = size_t(keyMetaType.alignOf());
    #else
        size_t align1 = 0;
        {
            const std::type_info* typeId = getTypeByMetaType(keyMetaType.id());
            if(!typeId)
                typeId = getTypeByQtName(keyMetaType.name());
            if(typeId){
                if(isFunctional(*typeId)){
                    QString typeName = QtJambiAPI::typeName(*typeId);
                    if(!typeName.startsWith("std::function") && (typeName.contains("(*)"))){
                        isPointer1 = true;
                    }
                }
                if(!isPointer1)
                     align1 = getValueAlignment(*typeId);
            }
        }
        if(align1==0 && !isPointer1)
            align1 = QtJambiTypeManager::getInternalAlignment(keyMetaType.name());
    #endif

        size_t size2 = size_t(valueMetaType.sizeOf());
        bool isPointer2 = AbstractContainerAccess::isPointerType(valueMetaType);
    #if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
        size_t align2 = size_t(valueMetaType.alignOf());
    #else
        size_t align2 = 0;
        {
            const std::type_info* typeId = getTypeByMetaType(valueMetaType.id());
            if(!typeId)
                typeId = getTypeByQtName(valueMetaType.name());
            if(typeId){
                if(isFunctional(*typeId)){
                    QString typeName = QtJambiAPI::typeName(*typeId);
                    if(!typeName.startsWith("std::function") && (typeName.contains("(*)"))){
                        isPointer2 = true;
                    }
                }
                if(!isPointer2)
                     align2 = getValueAlignment(*typeId);
            }
        }
        if(align2==0 && !isPointer2)
            align2 = QtJambiTypeManager::getInternalAlignment(valueMetaType.name());
    #endif

        QtJambiUtils::InternalToExternalConverter keyInternalToExternalConverter = QtJambiTypeManager::getInternalToExternalConverter(
                                                                                                            env,
                                                                                                            QLatin1String(keyMetaType.name()),
                                                                                                            keyMetaType,
                                                                                                            keyType,
                                                                                                            true
                                                                                                        );
        QtJambiUtils::ExternalToInternalConverter keyExternalToInternalConverter = QtJambiTypeManager::getExternalToInternalConverter(
                                                                                                            env,
                                                                                                            keyType,
                                                                                                            QLatin1String(keyMetaType.name()),
                                                                                                            keyMetaType
                                                                                                        );
        QtJambiUtils::InternalToExternalConverter valueInternalToExternalConverter = QtJambiTypeManager::getInternalToExternalConverter(
                                                                                                            env,
                                                                                                            QLatin1String(valueMetaType.name()),
                                                                                                            valueMetaType,
                                                                                                            valueType,
                                                                                                            true
                                                                                                        );
        QtJambiUtils::ExternalToInternalConverter valueExternalToInternalConverter = QtJambiTypeManager::getExternalToInternalConverter(
                                                                                                            env,
                                                                                                            valueType,
                                                                                                            QLatin1String(valueMetaType.name()),
                                                                                                            valueMetaType
                                                                                                        );

        QtJambiUtils::QHashFunction hashFunction1 = QtJambiTypeManager::findHashFunction(isPointer1, keyMetaType.id());
        QtJambiUtils::QHashFunction hashFunction2 = QtJambiTypeManager::findHashFunction(isPointer2, valueMetaType.id());
        QSharedPointer<AbstractContainerAccess> keyNestedContainerAccess = findContainerAccess(keyMetaType);
        QSharedPointer<AbstractContainerAccess> valueNestedContainerAccess = findContainerAccess(valueMetaType);
        const std::type_info* typeId = getTypeByQtName(keyMetaType.name());
        if(!typeId){
            typeId = getTypeByMetaType(keyMetaType);
        }
        PtrOwnerFunction keyOwnerFunction = nullptr;
        if(typeId)
            keyOwnerFunction = ContainerAPI::registeredOwnerFunction(*typeId);
        typeId = getTypeByQtName(qPrintable(valueMetaType.name()));
        if(!typeId){
            typeId = getTypeByMetaType(valueMetaType);
        }
        PtrOwnerFunction valueOwnerFunction = nullptr;
        if(typeId)
            valueOwnerFunction = ContainerAPI::registeredOwnerFunction(*typeId);
        containerAccess = dynamic_cast<AbstractMultiHashAccess*>(ContainerAccessAPI::createContainerAccess(
                                                                     env, AssociativeContainerType::QMultiHash,
                                                                     keyMetaType,
                                                                     align1, size1,
                                                                     isPointer1,
                                                                     hashFunction1,
                                                                     keyInternalToExternalConverter,
                                                                     keyExternalToInternalConverter,
                                                                     keyNestedContainerAccess,
                                                                     keyOwnerFunction,
                                                                     valueMetaType,
                                                                     align2, size2,
                                                                     isPointer2,
                                                                     hashFunction2,
                                                                     valueInternalToExternalConverter,
                                                                     valueExternalToInternalConverter,
                                                                     valueNestedContainerAccess,
                                                                     valueOwnerFunction));
        isNativeContainer = other && ContainerAPI::testQMultiHash(env, other, keyMetaType, valueMetaType);
    }
    CHECK_CONTAINER_ACCESS(env, containerAccess)

    void* listPtr;
    if(isNativeContainer){
        if(QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForJavaObject(env, other)){
            listPtr = containerAccess->createContainer(env, ConstContainerAndAccessInfo{link->getJavaObjectLocalRef(env), link->pointer(), link->containerAccess()});
        }else{
            if(Java::QtJambi::QtObjectInterface::isInstanceOf(env, other)){
                containerAccess->dispose();
                Java::QtJambi::QNoNativeResourcesException::throwNew(env, QStringLiteral("Incomplete object of type: %1").arg(QtJambiAPI::getObjectClassName(env, other).replace("$", ".")) QTJAMBI_STACKTRACEINFO );
            }
            listPtr = containerAccess->createContainer();
        }
    }else{
        listPtr = containerAccess->createContainer();
    }
    QByteArray name = "QMultiHash<";
    name += containerAccess->keyMetaType().name();
    name += ",";
    name += containerAccess->valueMetaType().name();
    name += ">";
    name = QMetaObject::normalizedType(name);
    QMetaType containerMetaType(containerAccess->registerContainer(name));
    QSharedPointer<QtJambiLink> link = QtJambiLink::createLinkForNativeObject(env, object, listPtr,
                                                                                   LINK_NAME_META_TYPE_ARG(containerMetaType)
                                                                                   true, true, containerAccess);
    if(Q_UNLIKELY(!link)) {
        containerAccess->deleteContainer(listPtr);
        containerAccess->dispose();
    }else if(!isNativeContainer && other){
        jobject iter = QtJambiAPI::entrySetIteratorOfJavaMap(env, other);
        while(QtJambiAPI::hasJavaIteratorNext(env, iter)){
            jobject entry = QtJambiAPI::nextOfJavaIterator(env, iter);
            jobject list = QtJambiAPI::valueOfJavaMapEntry(env, entry);
            jobject iter2 = QtJambiAPI::iteratorOfJavaIterable(env, list);
            while(QtJambiAPI::hasJavaIteratorNext(env, iter2)){
                containerAccess->insert(env, {object, listPtr}, QtJambiAPI::keyOfJavaMapEntry(env, entry), QtJambiAPI::nextOfJavaIterator(env, iter2));
            }
        }
    }
}

void CoreAPI::initializeQMap(JNIEnv *env, jobject object, jclass keyType, QtJambiNativeID keyMetaTypeId, jclass valueType, QtJambiNativeID valueMetaTypeId, jobject other){
    QMetaType& keyMetaType = checkedUnref(QtJambiAPI::objectFromNativeId<QMetaType>(keyMetaTypeId));
    QMetaType& valueMetaType = checkedUnref(QtJambiAPI::objectFromNativeId<QMetaType>(valueMetaTypeId));
    if(keyMetaType.id()==QMetaType::UnknownType)
        Java::Runtime::IllegalArgumentException::throwNew(env, QStringLiteral("QMetaType::UnknownType cannot be key type of %1.").arg("QMap") QTJAMBI_STACKTRACEINFO );
    if(keyMetaType.id()==QMetaType::Void)
        Java::Runtime::IllegalArgumentException::throwNew(env, QStringLiteral("void cannot be key type of %1.").arg("QMap") QTJAMBI_STACKTRACEINFO );
    if(valueMetaType.id()==QMetaType::UnknownType)
        Java::Runtime::IllegalArgumentException::throwNew(env, QStringLiteral("QMetaType::UnknownType cannot be value type of %1.").arg("QMap") QTJAMBI_STACKTRACEINFO );
    if(valueMetaType.id()==QMetaType::Void)
        Java::Runtime::IllegalArgumentException::throwNew(env, QStringLiteral("void cannot be value type of %1.").arg("QMap") QTJAMBI_STACKTRACEINFO );
    const SuperTypeInfos superTypeInfos = SuperTypeInfos::fromClass(env, env->GetObjectClass(object));
    if(superTypeInfos.size()>1)
        Java::Runtime::Error::throwNew(env, QStringLiteral("It is not permitted to create a derived type of %1 implementing any Qt interface.").arg("QMap") QTJAMBI_STACKTRACEINFO );
    keyType = getGlobalClassRef(env, keyType);
    valueType = getGlobalClassRef(env, valueType);
    using namespace QtJambiPrivate;
    bool isNativeContainer = false;
    AbstractMapAccess* containerAccess = nullptr;
    if(Java::QtCore::QMap::isInstanceOf(env, other)){
        if(QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForJavaObject(env, other)){
            containerAccess = dynamic_cast<AbstractMapAccess*>(link->containerAccess());
            if(containerAccess){
                containerAccess = containerAccess->clone();
                isNativeContainer = true;
            }
        }else{
            Java::QtJambi::QNoNativeResourcesException::throwNew(env, QStringLiteral("Incomplete object of type: %1").arg(QtJambiAPI::getObjectClassName(env, other).replace("$", ".")) QTJAMBI_STACKTRACEINFO );
        }
    }
    if(!containerAccess)
        containerAccess = dynamic_cast<AbstractMapAccess*>(ContainerAccessAPI::createContainerAccess(AssociativeContainerType::QMap, keyMetaType, valueMetaType));
    if(!containerAccess){
        size_t size1 = size_t(keyMetaType.sizeOf());
        bool isPointer1 = AbstractContainerAccess::isPointerType(keyMetaType);
    #if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
        size_t align1 = size_t(keyMetaType.alignOf());
    #else
        size_t align1 = 0;
        {
            const std::type_info* typeId = getTypeByMetaType(keyMetaType.id());
            if(!typeId)
                typeId = getTypeByQtName(keyMetaType.name());
            if(typeId){
                if(isFunctional(*typeId)){
                    QString typeName = QtJambiAPI::typeName(*typeId);
                    if(!typeName.startsWith("std::function") && (typeName.contains("(*)"))){
                        isPointer1 = true;
                    }
                }
                if(!isPointer1)
                     align1 = getValueAlignment(*typeId);
            }
        }
        if(align1==0 && !isPointer1)
            align1 = QtJambiTypeManager::getInternalAlignment(keyMetaType.name());
    #endif

        size_t size2 = size_t(valueMetaType.sizeOf());
        bool isPointer2 = AbstractContainerAccess::isPointerType(valueMetaType);
    #if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
        size_t align2 = size_t(valueMetaType.alignOf());
    #else
        size_t align2 = 0;
        {
            const std::type_info* typeId = getTypeByMetaType(valueMetaType.id());
            if(!typeId)
                typeId = getTypeByQtName(valueMetaType.name());
            if(typeId){
                if(isFunctional(*typeId)){
                    QString typeName = QtJambiAPI::typeName(*typeId);
                    if(!typeName.startsWith("std::function") && (typeName.contains("(*)"))){
                        isPointer2 = true;
                    }
                }
                if(!isPointer2)
                     align2 = getValueAlignment(*typeId);
            }
        }
        if(align2==0 && !isPointer2)
            align2 = QtJambiTypeManager::getInternalAlignment(valueMetaType.name());
    #endif

        QtJambiUtils::InternalToExternalConverter keyInternalToExternalConverter = QtJambiTypeManager::getInternalToExternalConverter(
                                                                                                            env,
                                                                                                            QLatin1String(keyMetaType.name()),
                                                                                                            keyMetaType,
                                                                                                            keyType,
                                                                                                            true
                                                                                                        );
        QtJambiUtils::ExternalToInternalConverter keyExternalToInternalConverter = QtJambiTypeManager::getExternalToInternalConverter(
                                                                                                            env,
                                                                                                            keyType,
                                                                                                            QLatin1String(keyMetaType.name()),
                                                                                                            keyMetaType
                                                                                                        );
        QtJambiUtils::InternalToExternalConverter valueInternalToExternalConverter = QtJambiTypeManager::getInternalToExternalConverter(
                                                                                                            env,
                                                                                                            QLatin1String(valueMetaType.name()),
                                                                                                            valueMetaType,
                                                                                                            valueType,
                                                                                                            true
                                                                                                        );
        QtJambiUtils::ExternalToInternalConverter valueExternalToInternalConverter = QtJambiTypeManager::getExternalToInternalConverter(
                                                                                                            env,
                                                                                                            valueType,
                                                                                                            QLatin1String(valueMetaType.name()),
                                                                                                            valueMetaType
                                                                                                        );

        QtJambiUtils::QHashFunction hashFunction1 = QtJambiTypeManager::findHashFunction(isPointer1, keyMetaType.id());
        QtJambiUtils::QHashFunction hashFunction2 = QtJambiTypeManager::findHashFunction(isPointer2, valueMetaType.id());
        QSharedPointer<AbstractContainerAccess> keyNestedContainerAccess = findContainerAccess(keyMetaType);
        QSharedPointer<AbstractContainerAccess> valueNestedContainerAccess = findContainerAccess(valueMetaType);
        const std::type_info* typeId = getTypeByQtName(keyMetaType.name());
        if(!typeId){
            typeId = getTypeByMetaType(keyMetaType);
        }
        PtrOwnerFunction keyOwnerFunction = nullptr;
        if(typeId)
            keyOwnerFunction = ContainerAPI::registeredOwnerFunction(*typeId);
        typeId = getTypeByQtName(qPrintable(valueMetaType.name()));
        if(!typeId){
            typeId = getTypeByMetaType(valueMetaType);
        }
        PtrOwnerFunction valueOwnerFunction = nullptr;
        if(typeId)
            valueOwnerFunction = ContainerAPI::registeredOwnerFunction(*typeId);
        containerAccess = dynamic_cast<AbstractMapAccess*>(ContainerAccessAPI::createContainerAccess(
                                                                    env, AssociativeContainerType::QMap,
                                                                    keyMetaType,
                                                                    align1, size1,
                                                                    isPointer1,
                                                                    hashFunction1,
                                                                    keyInternalToExternalConverter,
                                                                    keyExternalToInternalConverter,
                                                                    keyNestedContainerAccess,
                                                                    keyOwnerFunction,
                                                                    valueMetaType,
                                                                    align2, size2,
                                                                    isPointer2,
                                                                    hashFunction2,
                                                                    valueInternalToExternalConverter,
                                                                    valueExternalToInternalConverter,
                                                                    valueNestedContainerAccess,
                                                                    valueOwnerFunction));
        isNativeContainer = other && ContainerAPI::testQMap(env, other, keyMetaType, valueMetaType);
    }
    CHECK_CONTAINER_ACCESS(env, containerAccess)
    void* listPtr;
    if(isNativeContainer){
        if(QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForJavaObject(env, other)){
            listPtr = containerAccess->createContainer(env, ConstContainerAndAccessInfo{link->getJavaObjectLocalRef(env), link->pointer(), link->containerAccess()});
        }else{
            if(Java::QtJambi::QtObjectInterface::isInstanceOf(env, other)){
                containerAccess->dispose();
                Java::QtJambi::QNoNativeResourcesException::throwNew(env, QStringLiteral("Incomplete object of type: %1").arg(QtJambiAPI::getObjectClassName(env, other).replace("$", ".")) QTJAMBI_STACKTRACEINFO );
            }
            listPtr = containerAccess->createContainer();
        }
    }else{
        listPtr = containerAccess->createContainer();
    }
    QByteArray name = "QMap<";
    name += containerAccess->keyMetaType().name();
    name += ",";
    name += containerAccess->valueMetaType().name();
    name += ">";
    name = QMetaObject::normalizedType(name);
    QMetaType containerMetaType(containerAccess->registerContainer(name));
    QSharedPointer<QtJambiLink> link = QtJambiLink::createLinkForNativeObject(env, object, listPtr,
                                                                                   LINK_NAME_META_TYPE_ARG(containerMetaType)
                                                                                   true, true, containerAccess);
    if(Q_UNLIKELY(!link)) {
        containerAccess->deleteContainer(listPtr);
        containerAccess->dispose();
    }else if(!isNativeContainer && other){
        jobject iter = QtJambiAPI::entrySetIteratorOfJavaMap(env, other);
        while(QtJambiAPI::hasJavaIteratorNext(env, iter)){
            jobject entry = QtJambiAPI::nextOfJavaIterator(env, iter);
            containerAccess->insert(env, {object, listPtr}, QtJambiAPI::keyOfJavaMapEntry(env, entry), QtJambiAPI::valueOfJavaMapEntry(env, entry));
        }
    }
}

void CoreAPI::initializeQMultiMap(JNIEnv *env, jobject object, jclass keyType, QtJambiNativeID keyMetaTypeId, jclass valueType, QtJambiNativeID valueMetaTypeId, jobject other){
    QMetaType& keyMetaType = checkedUnref(QtJambiAPI::objectFromNativeId<QMetaType>(keyMetaTypeId));
    QMetaType& valueMetaType = checkedUnref(QtJambiAPI::objectFromNativeId<QMetaType>(valueMetaTypeId));
    if(keyMetaType.id()==QMetaType::UnknownType)
        Java::Runtime::IllegalArgumentException::throwNew(env, QStringLiteral("QMetaType::UnknownType cannot be key type of %1.").arg("QMultiMap") QTJAMBI_STACKTRACEINFO );
    if(keyMetaType.id()==QMetaType::Void)
        Java::Runtime::IllegalArgumentException::throwNew(env, QStringLiteral("void cannot be key type of %1.").arg("QMultiMap") QTJAMBI_STACKTRACEINFO );
    if(valueMetaType.id()==QMetaType::UnknownType)
        Java::Runtime::IllegalArgumentException::throwNew(env, QStringLiteral("QMetaType::UnknownType cannot be value type of %1.").arg("QMultiMap") QTJAMBI_STACKTRACEINFO );
    if(valueMetaType.id()==QMetaType::Void)
        Java::Runtime::IllegalArgumentException::throwNew(env, QStringLiteral("void cannot be value type of %1.").arg("QMultiMap") QTJAMBI_STACKTRACEINFO );
    const SuperTypeInfos superTypeInfos = SuperTypeInfos::fromClass(env, env->GetObjectClass(object));
    if(superTypeInfos.size()>1)
        Java::Runtime::Error::throwNew(env, QStringLiteral("It is not permitted to create a derived type of %1 implementing any Qt interface.").arg("QMultiMap") QTJAMBI_STACKTRACEINFO );
    keyType = getGlobalClassRef(env, keyType);
    valueType = getGlobalClassRef(env, valueType);
    using namespace QtJambiPrivate;
    bool isNativeContainer = false;
    AbstractMultiMapAccess* containerAccess = nullptr;
    if(Java::QtCore::QMultiMap::isInstanceOf(env, other)){
        if(QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForJavaObject(env, other)){
            containerAccess = dynamic_cast<AbstractMultiMapAccess*>(link->containerAccess());
            if(containerAccess){
                containerAccess = containerAccess->clone();
                isNativeContainer = true;
            }
        }else{
            Java::QtJambi::QNoNativeResourcesException::throwNew(env, QStringLiteral("Incomplete object of type: %1").arg(QtJambiAPI::getObjectClassName(env, other).replace("$", ".")) QTJAMBI_STACKTRACEINFO );
        }
    }
    if(!containerAccess)
        containerAccess = dynamic_cast<AbstractMultiMapAccess*>(ContainerAccessAPI::createContainerAccess(AssociativeContainerType::QMultiMap, keyMetaType, valueMetaType));
    if(!containerAccess){
        size_t size1 = size_t(keyMetaType.sizeOf());
        bool isPointer1 = AbstractContainerAccess::isPointerType(keyMetaType);
#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
        size_t align1 = size_t(keyMetaType.alignOf());
#else
        size_t align1 = 0;
        {
            const std::type_info* typeId = getTypeByMetaType(keyMetaType.id());
            if(!typeId)
                typeId = getTypeByQtName(keyMetaType.name());
            if(typeId){
                if(isFunctional(*typeId)){
                    QString typeName = QtJambiAPI::typeName(*typeId);
                    if(!typeName.startsWith("std::function") && (typeName.contains("(*)"))){
                        isPointer1 = true;
                    }
                }
                if(!isPointer1)
                     align1 = getValueAlignment(*typeId);
            }
        }
        if(align1==0 && !isPointer1)
            align1 = QtJambiTypeManager::getInternalAlignment(keyMetaType.name());
    #endif

        size_t size2 = size_t(valueMetaType.sizeOf());
        bool isPointer2 = AbstractContainerAccess::isPointerType(valueMetaType);
    #if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
        size_t align2 = size_t(valueMetaType.alignOf());
    #else
        size_t align2 = 0;
        {
            const std::type_info* typeId = getTypeByMetaType(valueMetaType.id());
            if(!typeId)
                typeId = getTypeByQtName(valueMetaType.name());
            if(typeId){
                if(isFunctional(*typeId)){
                    QString typeName = QtJambiAPI::typeName(*typeId);
                    if(!typeName.startsWith("std::function") && (typeName.contains("(*)"))){
                        isPointer2 = true;
                    }
                }
                if(!isPointer2)
                    align2 = getValueAlignment(*typeId);
            }
        }
        if(align2==0 && !isPointer2)
            align2 = QtJambiTypeManager::getInternalAlignment(valueMetaType.name());
    #endif

        QtJambiUtils::InternalToExternalConverter keyInternalToExternalConverter = QtJambiTypeManager::getInternalToExternalConverter(
                                                                                                            env,
                                                                                                            QLatin1String(keyMetaType.name()),
                                                                                                            keyMetaType,
                                                                                                            keyType,
                                                                                                            true
                                                                                                        );
        QtJambiUtils::ExternalToInternalConverter keyExternalToInternalConverter = QtJambiTypeManager::getExternalToInternalConverter(
                                                                                                            env,
                                                                                                            keyType,
                                                                                                            QLatin1String(keyMetaType.name()),
                                                                                                            keyMetaType
                                                                                                        );
        QtJambiUtils::InternalToExternalConverter valueInternalToExternalConverter = QtJambiTypeManager::getInternalToExternalConverter(
                                                                                                            env,
                                                                                                            QLatin1String(valueMetaType.name()),
                                                                                                            valueMetaType,
                                                                                                            valueType,
                                                                                                            true
                                                                                                        );
        QtJambiUtils::ExternalToInternalConverter valueExternalToInternalConverter = QtJambiTypeManager::getExternalToInternalConverter(
                                                                                                            env,
                                                                                                            valueType,
                                                                                                            QLatin1String(valueMetaType.name()),
                                                                                                            valueMetaType
                                                                                                        );

        QtJambiUtils::QHashFunction hashFunction1 = QtJambiTypeManager::findHashFunction(isPointer1, keyMetaType.id());
        QtJambiUtils::QHashFunction hashFunction2 = QtJambiTypeManager::findHashFunction(isPointer2, valueMetaType.id());
        QSharedPointer<AbstractContainerAccess> keyNestedContainerAccess = findContainerAccess(keyMetaType);
        QSharedPointer<AbstractContainerAccess> valueNestedContainerAccess = findContainerAccess(valueMetaType);
        const std::type_info* typeId = getTypeByQtName(keyMetaType.name());
        if(!typeId){
            typeId = getTypeByMetaType(keyMetaType);
        }
        PtrOwnerFunction keyOwnerFunction = nullptr;
        if(typeId)
            keyOwnerFunction = ContainerAPI::registeredOwnerFunction(*typeId);
        typeId = getTypeByQtName(qPrintable(valueMetaType.name()));
        if(!typeId){
            typeId = getTypeByMetaType(valueMetaType);
        }
        PtrOwnerFunction valueOwnerFunction = nullptr;
        if(typeId)
            valueOwnerFunction = ContainerAPI::registeredOwnerFunction(*typeId);
        containerAccess = dynamic_cast<AbstractMultiMapAccess*>(ContainerAccessAPI::createContainerAccess(
                                                                    env, AssociativeContainerType::QMultiMap,
                                                                    keyMetaType,
                                                                    align1, size1,
                                                                    isPointer1,
                                                                    hashFunction1,
                                                                    keyInternalToExternalConverter,
                                                                    keyExternalToInternalConverter,
                                                                    keyNestedContainerAccess,
                                                                    keyOwnerFunction,
                                                                    valueMetaType,
                                                                    align2, size2,
                                                                    isPointer2,
                                                                    hashFunction2,
                                                                    valueInternalToExternalConverter,
                                                                    valueExternalToInternalConverter,
                                                                    valueNestedContainerAccess,
                                                                    valueOwnerFunction));
        isNativeContainer = other && ContainerAPI::testQMultiMap(env, other, keyMetaType, valueMetaType);
    }
    CHECK_CONTAINER_ACCESS(env, containerAccess)
    void* listPtr;
    if(isNativeContainer){
        if(QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForJavaObject(env, other)){
            listPtr = containerAccess->createContainer(env, ConstContainerAndAccessInfo{link->getJavaObjectLocalRef(env), link->pointer(), link->containerAccess()});
        }else{
            if(Java::QtJambi::QtObjectInterface::isInstanceOf(env, other)){
                containerAccess->dispose();
                Java::QtJambi::QNoNativeResourcesException::throwNew(env, QStringLiteral("Incomplete object of type: %1").arg(QtJambiAPI::getObjectClassName(env, other).replace("$", ".")) QTJAMBI_STACKTRACEINFO );
            }
            listPtr = containerAccess->createContainer();
        }
    }else{
        listPtr = containerAccess->createContainer();
    }
    QByteArray name = "QMultiMap<";
    name += containerAccess->keyMetaType().name();
    name += ",";
    name += containerAccess->valueMetaType().name();
    name += ">";
    name = QMetaObject::normalizedType(name);
    QMetaType containerMetaType(containerAccess->registerContainer(name));
    QSharedPointer<QtJambiLink> link = QtJambiLink::createLinkForNativeObject(env, object, listPtr,
                                                                                   LINK_NAME_META_TYPE_ARG(containerMetaType)
                                                                                   true, true, containerAccess);
    if(Q_UNLIKELY(!link)) {
        containerAccess->deleteContainer(listPtr);
        containerAccess->dispose();
    }else{
        if(!isNativeContainer && other){
            jobject iter = QtJambiAPI::entrySetIteratorOfJavaMap(env, other);
            while(QtJambiAPI::hasJavaIteratorNext(env, iter)){
                jobject entry = QtJambiAPI::nextOfJavaIterator(env, iter);
                jobject list = QtJambiAPI::valueOfJavaMapEntry(env, entry);
                jobject iter2 = QtJambiAPI::iteratorOfJavaIterable(env, list);
                while(QtJambiAPI::hasJavaIteratorNext(env, iter2)){
                    containerAccess->insert(env, {object, listPtr}, QtJambiAPI::keyOfJavaMapEntry(env, entry), QtJambiAPI::nextOfJavaIterator(env, iter2));
                }
            }
        }
    }
}

bool compareMetaTypes(const QMetaType& typeA, const QMetaType& typeB){
    if(typeA==typeB)
        return true;
    if(typeA==QMetaType::fromType<char>()){
        return typeB==QMetaType::fromType<signed char>() || typeB==QMetaType::fromType<unsigned char>();
    }
    if(typeA==QMetaType::fromType<signed char>()){
        return typeB==QMetaType::fromType<char>() || typeB==QMetaType::fromType<unsigned char>();
    }
    if(typeA==QMetaType::fromType<unsigned char>()){
        return typeB==QMetaType::fromType<signed char>() || typeB==QMetaType::fromType<char>();
    }
    if(typeA==QMetaType::fromType<short>()){
        return typeB==QMetaType::fromType<signed short>() || typeB==QMetaType::fromType<unsigned short>();
    }
    if(typeA==QMetaType::fromType<signed short>()){
        return typeB==QMetaType::fromType<short>() || typeB==QMetaType::fromType<unsigned short>();
    }
    if(typeA==QMetaType::fromType<unsigned short>()){
        return typeB==QMetaType::fromType<signed short>() || typeB==QMetaType::fromType<short>();
    }
    if(typeA==QMetaType::fromType<int>()){
        return typeB==QMetaType::fromType<signed int>() || typeB==QMetaType::fromType<unsigned int>();
    }
    if(typeA==QMetaType::fromType<signed int>()){
        return typeB==QMetaType::fromType<int>() || typeB==QMetaType::fromType<unsigned int>();
    }
    if(typeA==QMetaType::fromType<unsigned int>()){
        return typeB==QMetaType::fromType<signed int>() || typeB==QMetaType::fromType<int>();
    }
    if(typeA==QMetaType::fromType<qint64>()){
        return typeB==QMetaType::fromType<quint64>();
    }
    if(typeA==QMetaType::fromType<quint64>()){
        return typeB==QMetaType::fromType<qint64>();
    }
#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
    if(typeA==QMetaType::fromType<QChar>()){
        return typeB==QMetaType::fromType<char16_t>();
    }
    if(typeA==QMetaType::fromType<char16_t>()){
        return typeB==QMetaType::fromType<QChar>();
    }
#endif
    return false;
}

#define TEST_COLLECTION1(NAME,JCLASS) \
bool ContainerAPI::testQ##NAME(JNIEnv *env, jobject collection, const QMetaType& expectedElementMetaType) {\
    if(collection && expectedElementMetaType.isValid()){\
        if(Java::QtCore::Q##JCLASS::isInstanceOf(env,collection)){\
            if(QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForJavaObject(env, collection)){\
                if(Abstract##NAME##Access* containerAccess = dynamic_cast<Abstract##NAME##Access*>(link->containerAccess()))\
                    return compareMetaTypes(containerAccess->elementMetaType(), expectedElementMetaType);\
            }else\
                Java::QtJambi::QNoNativeResourcesException::throwNew(env, QStringLiteral("Incomplete object of type: %1").arg(QtJambiAPI::getObjectClassName(env, collection).replace("$", ".")) QTJAMBI_STACKTRACEINFO );\
        }else if(!env->IsInstanceOf(collection, Java::Runtime::Collection::getClass(env)))\
            Java::Runtime::IllegalArgumentException::throwNew(env, QString("Wrong argument given: %1, expected: %2").arg(QtJambiAPI::getObjectClassName(env, collection).replace("$", "."), QtJambiAPI::getClassName(env, Java::Runtime::Collection::getClass(env)).replace("$", ".")) QTJAMBI_STACKTRACEINFO );\
    }\
    return false;\
}

#define TEST_COLLECTION2(NAME) \
bool ContainerAPI::testQ##NAME(JNIEnv *env, jobject collection, const std::type_info&, const QMetaType& expectedElementMetaType) {\
    return ContainerAPI::testQ##NAME(env, collection, expectedElementMetaType);\
}

#define TEST_MAP1(NAME) \
bool ContainerAPI::testQ##NAME(JNIEnv *env, jobject mapObject, const QMetaType& expectedKeyMetaType, const QMetaType& expectedValueMetaType) {\
    if(mapObject && expectedKeyMetaType.isValid() && expectedValueMetaType.isValid()){\
        if(Java::QtCore::Q##NAME::isInstanceOf(env, mapObject)){\
            if(QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForJavaObject(env, mapObject)){\
                if(Abstract##NAME##Access* containerAccess = dynamic_cast<Abstract##NAME##Access*>(link->containerAccess()))\
                    return compareMetaTypes(containerAccess->keyMetaType(), expectedKeyMetaType) && compareMetaTypes(containerAccess->valueMetaType(), expectedValueMetaType);\
            }else\
                Java::QtJambi::QNoNativeResourcesException::throwNew(env, QStringLiteral("Incomplete object of type: %1").arg(QtJambiAPI::getObjectClassName(env, mapObject).replace("$", ".")) QTJAMBI_STACKTRACEINFO );\
        }else if(!env->IsInstanceOf(mapObject, Java::Runtime::Map::getClass(env)))\
            Java::Runtime::IllegalArgumentException::throwNew(env, QString("Wrong argument given: %1, expected: %2").arg(QtJambiAPI::getObjectClassName(env, mapObject).replace("$", "."), QtJambiAPI::getClassName(env, Java::Runtime::Map::getClass(env)).replace("$", ".")) QTJAMBI_STACKTRACEINFO );\
    }\
    return false;\
}

#define TEST_MAP2(NAME) \
bool ContainerAPI::testQ##NAME(JNIEnv *env, jobject mapObject, const std::type_info&, const QMetaType& expectedKeyMetaType, const std::type_info&, const QMetaType& expectedValueMetaType) {\
    return ContainerAPI::testQ##NAME(env, mapObject, expectedKeyMetaType, expectedValueMetaType);\
}

#define TEST_MULTIMAP1(NAME) \
bool ContainerAPI::testQ##NAME(JNIEnv *env, jobject mapObject, const QMetaType& expectedKeyMetaType, const QMetaType& expectedValueMetaType) {\
    if(mapObject && expectedKeyMetaType.isValid() && expectedValueMetaType.isValid()){\
        if(Java::QtCore::Q##NAME::isInstanceOf(env, mapObject)){\
            if(QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForJavaObject(env, mapObject)){\
                if(Abstract##NAME##Access* containerAccess = dynamic_cast<Abstract##NAME##Access*>(link->containerAccess()))\
                    return compareMetaTypes(containerAccess->keyMetaType(), expectedKeyMetaType) && compareMetaTypes(containerAccess->valueMetaType(), expectedValueMetaType);\
            }else\
                Java::QtJambi::QNoNativeResourcesException::throwNew(env, QStringLiteral("Incomplete object of type: %1").arg(QtJambiAPI::getObjectClassName(env, mapObject).replace("$", ".")) QTJAMBI_STACKTRACEINFO );\
        }else if(!env->IsInstanceOf(mapObject, Java::Runtime::Map::getClass(env)))\
            Java::Runtime::IllegalArgumentException::throwNew(env, QString("Wrong argument given: %1, expected: %2").arg(QtJambiAPI::getObjectClassName(env, mapObject).replace("$", "."), QtJambiAPI::getClassName(env, Java::Runtime::Map::getClass(env)).replace("$", ".")) QTJAMBI_STACKTRACEINFO );\
    }\
    return false;\
}

#define TEST_MULTIMAP2(NAME) \
bool ContainerAPI::testQ##NAME(JNIEnv *env, jobject mapObject, const std::type_info&, const QMetaType& expectedKeyMetaType, const std::type_info&, const QMetaType& expectedValueMetaType) {\
    return ContainerAPI::testQ##NAME(env, mapObject, expectedKeyMetaType, expectedValueMetaType);\
}

#define GET_AS_COLLECTION1(NAME,JCLASS) \
bool ContainerAPI::getAsQ##NAME(JNIEnv *env, jobject collection, const QMetaType& expectedElementMetaType, void* &pointer) {\
    if(collection && expectedElementMetaType.isValid()){\
        if(Java::QtCore::Q##JCLASS::isInstanceOf(env,collection)){\
            if(QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForJavaObject(env, collection)){\
                if(Abstract##NAME##Access* containerAccess = dynamic_cast<Abstract##NAME##Access*>(link->containerAccess())){\
                    if(compareMetaTypes(containerAccess->elementMetaType(), expectedElementMetaType)){\
                        pointer = link->pointer();\
                        return true;\
                    }\
                }\
            }else\
                Java::QtJambi::QNoNativeResourcesException::throwNew(env, QStringLiteral("Incomplete object of type: %1").arg(QtJambiAPI::getObjectClassName(env, collection).replace("$", ".")) QTJAMBI_STACKTRACEINFO );\
        }else if(!env->IsInstanceOf(collection, Java::Runtime::Collection::getClass(env)))\
            Java::Runtime::IllegalArgumentException::throwNew(env, QString("Wrong argument given: %1, expected: %2").arg(QtJambiAPI::getObjectClassName(env, collection).replace("$", "."), QtJambiAPI::getClassName(env, Java::Runtime::Collection::getClass(env)).replace("$", ".")) QTJAMBI_STACKTRACEINFO );\
    }\
    return false;\
} \
bool ContainerAPI::getAsQ##NAME(JNIEnv *env, jobject collection, const QMetaType& expectedElementMetaType, void* &pointer, AbstractContainerAccess*& access) {\
    if(collection && expectedElementMetaType.isValid()){\
        if(Java::QtCore::Q##NAME::isInstanceOf(env,collection)){\
            if(QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForJavaObject(env, collection)){\
                if(Abstract##NAME##Access* containerAccess = dynamic_cast<Abstract##NAME##Access*>(link->containerAccess())){\
                    if(compareMetaTypes(containerAccess->elementMetaType(), expectedElementMetaType)){\
                        pointer = link->pointer();\
                        access = containerAccess;\
                        return true;\
                }\
            }\
        }else\
            Java::QtJambi::QNoNativeResourcesException::throwNew(env, QStringLiteral("Incomplete object of type: %1").arg(QtJambiAPI::getObjectClassName(env, collection).replace("$", ".")) QTJAMBI_STACKTRACEINFO );\
    }else if(!env->IsInstanceOf(collection, Java::Runtime::Collection::getClass(env)))\
        Java::Runtime::IllegalArgumentException::throwNew(env, QString("Wrong argument given: %1, expected: %2").arg(QtJambiAPI::getObjectClassName(env, collection).replace("$", "."), QtJambiAPI::getClassName(env, Java::Runtime::Collection::getClass(env)).replace("$", ".")) QTJAMBI_STACKTRACEINFO );\
    }\
    return false;\
}

#define GET_AS_COLLECTION2(NAME) \
bool ContainerAPI::getAsQ##NAME(JNIEnv *env, jobject collection, const std::type_info&, const QMetaType& expectedElementMetaType, void* &pointer) {\
        return ContainerAPI::getAsQ##NAME(env, collection, expectedElementMetaType, pointer);\
}

#define GET_AS_MAP1(NAME) \
bool ContainerAPI::getAsQ##NAME(JNIEnv *env, jobject mapObject, const QMetaType& expectedKeyMetaType, const QMetaType& expectedValueMetaType, void* &pointer) {\
    if(mapObject && expectedKeyMetaType.isValid() && expectedValueMetaType.isValid()){\
        if(Java::QtCore::Q##NAME::isInstanceOf(env, mapObject)){\
            if(QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForJavaObject(env, mapObject)){\
                if(Abstract##NAME##Access* containerAccess = dynamic_cast<Abstract##NAME##Access*>(link->containerAccess())){\
                    if(compareMetaTypes(containerAccess->keyMetaType(), expectedKeyMetaType) && compareMetaTypes(containerAccess->valueMetaType(), expectedValueMetaType)){\
                       pointer = link->pointer();\
                       return true;\
                    }\
                }\
            }else\
                Java::QtJambi::QNoNativeResourcesException::throwNew(env, QStringLiteral("Incomplete object of type: %1").arg(QtJambiAPI::getObjectClassName(env, mapObject).replace("$", ".")) QTJAMBI_STACKTRACEINFO );\
        }else if(!env->IsInstanceOf(mapObject, Java::Runtime::Map::getClass(env)))\
            Java::Runtime::IllegalArgumentException::throwNew(env, QString("Wrong argument given: %1, expected: %2").arg(QtJambiAPI::getObjectClassName(env, mapObject).replace("$", "."), QtJambiAPI::getClassName(env, Java::Runtime::Map::getClass(env)).replace("$", ".")) QTJAMBI_STACKTRACEINFO );\
    }\
    return false;\
}\
bool ContainerAPI::getAsQ##NAME(JNIEnv *env, jobject mapObject, const QMetaType& expectedKeyMetaType, const QMetaType& expectedValueMetaType, void* &pointer, AbstractContainerAccess*& access) {\
    if(mapObject && expectedKeyMetaType.isValid() && expectedValueMetaType.isValid()){\
        if(Java::QtCore::Q##NAME::isInstanceOf(env, mapObject)){\
            if(QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForJavaObject(env, mapObject)){\
                if(Abstract##NAME##Access* containerAccess = dynamic_cast<Abstract##NAME##Access*>(link->containerAccess())){\
                    if(compareMetaTypes(containerAccess->keyMetaType(), expectedKeyMetaType) && compareMetaTypes(containerAccess->valueMetaType(), expectedValueMetaType)){\
                        pointer = link->pointer();\
                        access = containerAccess;\
                        return true;\
                }\
            }\
        }else\
            Java::QtJambi::QNoNativeResourcesException::throwNew(env, QStringLiteral("Incomplete object of type: %1").arg(QtJambiAPI::getObjectClassName(env, mapObject).replace("$", ".")) QTJAMBI_STACKTRACEINFO );\
    }else if(!env->IsInstanceOf(mapObject, Java::Runtime::Map::getClass(env)))\
        Java::Runtime::IllegalArgumentException::throwNew(env, QString("Wrong argument given: %1, expected: %2").arg(QtJambiAPI::getObjectClassName(env, mapObject).replace("$", "."), QtJambiAPI::getClassName(env, Java::Runtime::Map::getClass(env)).replace("$", ".")) QTJAMBI_STACKTRACEINFO );\
    }\
    return false;\
}

#define GET_AS_MAP2(NAME) \
bool ContainerAPI::getAsQ##NAME(JNIEnv *env, jobject mapObject, const std::type_info&, const QMetaType& expectedKeyMetaType, const std::type_info&, const QMetaType& expectedValueMetaType, void* &pointer) {\
        return ContainerAPI::getAsQ##NAME(env, mapObject, expectedKeyMetaType, expectedValueMetaType, pointer);\
}

#define GET_AS_MULTIMAP1(NAME) \
bool ContainerAPI::getAsQ##NAME(JNIEnv *env, jobject mapObject, const QMetaType& expectedKeyMetaType, const QMetaType& expectedValueMetaType, void* &pointer) {\
    if(mapObject && expectedKeyMetaType.isValid() && expectedValueMetaType.isValid()){\
        if(Java::QtCore::Q##NAME::isInstanceOf(env, mapObject)){\
            if(QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForJavaObject(env, mapObject)){\
                if(Abstract##NAME##Access* containerAccess = dynamic_cast<Abstract##NAME##Access*>(link->containerAccess())){\
                    if(compareMetaTypes(containerAccess->keyMetaType(), expectedKeyMetaType) && compareMetaTypes(containerAccess->valueMetaType(), expectedValueMetaType)){\
                        pointer = link->pointer();\
                        return true;\
                    }\
                }\
            }else\
                Java::QtJambi::QNoNativeResourcesException::throwNew(env, QStringLiteral("Incomplete object of type: %1").arg(QtJambiAPI::getObjectClassName(env, mapObject).replace("$", ".")) QTJAMBI_STACKTRACEINFO );\
        }else if(!env->IsInstanceOf(mapObject, Java::Runtime::Map::getClass(env)))\
            Java::Runtime::IllegalArgumentException::throwNew(env, QString("Wrong argument given: %1, expected: %2").arg(QtJambiAPI::getObjectClassName(env, mapObject).replace("$", "."), QtJambiAPI::getClassName(env, Java::Runtime::Map::getClass(env)).replace("$", ".")) QTJAMBI_STACKTRACEINFO );\
    }\
    return false;\
}\
bool ContainerAPI::getAsQ##NAME(JNIEnv *env, jobject mapObject, const QMetaType& expectedKeyMetaType, const QMetaType& expectedValueMetaType, void* &pointer, AbstractContainerAccess*& access) {\
    if(mapObject && expectedKeyMetaType.isValid() && expectedValueMetaType.isValid()){\
        if(Java::QtCore::Q##NAME::isInstanceOf(env, mapObject)){\
            if(QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForJavaObject(env, mapObject)){\
                if(Abstract##NAME##Access* containerAccess = dynamic_cast<Abstract##NAME##Access*>(link->containerAccess())){\
                    if(compareMetaTypes(containerAccess->keyMetaType(), expectedKeyMetaType) && compareMetaTypes(containerAccess->valueMetaType(), expectedValueMetaType)){\
                        pointer = link->pointer();\
                        access = containerAccess;\
                        return true;\
                }\
            }\
        }else\
            Java::QtJambi::QNoNativeResourcesException::throwNew(env, QStringLiteral("Incomplete object of type: %1").arg(QtJambiAPI::getObjectClassName(env, mapObject).replace("$", ".")) QTJAMBI_STACKTRACEINFO );\
    }else if(!env->IsInstanceOf(mapObject, Java::Runtime::Map::getClass(env)))\
        Java::Runtime::IllegalArgumentException::throwNew(env, QString("Wrong argument given: %1, expected: %2").arg(QtJambiAPI::getObjectClassName(env, mapObject).replace("$", "."), QtJambiAPI::getClassName(env, Java::Runtime::Map::getClass(env)).replace("$", ".")) QTJAMBI_STACKTRACEINFO );\
    }\
    return false;\
}

#define GET_AS_MULTIMAP2(NAME) \
bool ContainerAPI::getAsQ##NAME(JNIEnv *env, jobject mapObject, const std::type_info&, const QMetaType& expectedKeyMetaType, const std::type_info&, const QMetaType& expectedValueMetaType, void* &pointer) {\
        return ContainerAPI::getAsQ##NAME(env, mapObject, expectedKeyMetaType, expectedValueMetaType, pointer);\
}

PtrOwnerFunction ContainerAPI::registeredOwnerFunction(const std::type_info& typeId){
    return ::registeredOwnerFunction(typeId);
}

bool ContainerAPI::testQQueue(JNIEnv *env, jobject collection, const QMetaType& expectedElementMetaType) {
    return ContainerAPI::testQList(env, collection, expectedElementMetaType);
}
bool ContainerAPI::getAsQQueue(JNIEnv *env, jobject collection, const QMetaType& expectedElementMetaType, void* &pointer) {
    return ContainerAPI::getAsQList(env, collection, expectedElementMetaType, pointer);
}
bool ContainerAPI::getAsQQueue(JNIEnv *env, jobject collection, const QMetaType& expectedElementMetaType, void* &pointer, AbstractContainerAccess*& access) {
    return ContainerAPI::getAsQList(env, collection, expectedElementMetaType, pointer, access);
}
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
TEST_COLLECTION1(Vector,Vector)
TEST_COLLECTION1(LinkedList,LinkedList)
TEST_COLLECTION2(Vector)
TEST_COLLECTION2(LinkedList)
bool ContainerAPI::testQStack(JNIEnv *env, jobject collection, const QMetaType& expectedElementMetaType) {
    return ContainerAPI::testQVector(env, collection, expectedElementMetaType);
}

GET_AS_COLLECTION1(Vector,Vector)
GET_AS_COLLECTION1(LinkedList,LinkedList)
GET_AS_COLLECTION2(Vector)
GET_AS_COLLECTION2(LinkedList)
bool ContainerAPI::getAsQStack(JNIEnv *env, jobject collection, const QMetaType& expectedElementMetaType, void* &pointer) {
    return ContainerAPI::getAsQVector(env, collection, expectedElementMetaType, pointer);
}
bool ContainerAPI::getAsQStack(JNIEnv *env, jobject collection, const QMetaType& expectedElementMetaType, void* &pointer, AbstractContainerAccess*& access) {
    return ContainerAPI::getAsQVector(env, collection, expectedElementMetaType, pointer, access);
}
#else
bool ContainerAPI::testQStack(JNIEnv *env, jobject collection, const QMetaType& expectedElementMetaType) {
    return ContainerAPI::testQList(env, collection, expectedElementMetaType);
}
bool ContainerAPI::getAsQStack(JNIEnv *env, jobject collection, const QMetaType& expectedElementMetaType, void* &pointer) {
    return ContainerAPI::getAsQList(env, collection, expectedElementMetaType,pointer);
}
bool ContainerAPI::getAsQStack(JNIEnv *env, jobject collection, const QMetaType& expectedElementMetaType, void* &pointer, AbstractContainerAccess*& access) {
    return ContainerAPI::getAsQList(env, collection, expectedElementMetaType, pointer, access);
}
#endif // QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
TEST_COLLECTION1(List,List)
TEST_COLLECTION1(Set,Set)
TEST_MULTIMAP1(MultiMap)
TEST_MULTIMAP1(MultiHash)
TEST_MAP1(Map)
TEST_MAP1(Hash)
TEST_COLLECTION2(List)
TEST_COLLECTION2(Set)
TEST_COLLECTION2(Queue)
TEST_COLLECTION2(Stack)
TEST_MULTIMAP2(MultiMap)
TEST_MULTIMAP2(MultiHash)
TEST_MAP2(Map)
TEST_MAP2(Hash)

GET_AS_COLLECTION1(List,List)
GET_AS_COLLECTION1(Set,Set)
GET_AS_MULTIMAP1(MultiMap)
GET_AS_MULTIMAP1(MultiHash)
GET_AS_MAP1(Map)
GET_AS_MAP1(Hash)
GET_AS_COLLECTION2(List)
GET_AS_COLLECTION2(Set)
GET_AS_COLLECTION2(Queue)
GET_AS_COLLECTION2(Stack)
GET_AS_MULTIMAP2(MultiMap)
GET_AS_MULTIMAP2(MultiHash)
GET_AS_MAP2(Map)
GET_AS_MAP2(Hash)

#if QT_VERSION >= QT_VERSION_CHECK(6, 7, 0)
TEST_COLLECTION1(Span,ConstSpan)
TEST_COLLECTION2(Span)
GET_AS_COLLECTION1(Span,ConstSpan)
GET_AS_COLLECTION2(Span)
#endif
