/****************************************************************************
**
** Copyright (C) 2009-2021 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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

#include <qglobal.h>
#include <cstring>

#include <QtCore/QAbstractItemModel>
#include <QtCore/QCoreApplication>
#include <QtCore/QDir>
#include <QtCore/QEvent>
#include <QtCore/QFileInfo>
#include <QtCore/QLibrary>
#include <QtCore/QMetaMethod>
#include <QtCore/QMetaObject>
#include <QtCore/QSettings>
#include <QtCore/QStringList>
#include <QtCore/QThread>
#include <QtCore/private/qthread_p.h>
#include <QtCore/QVariant>
#include <QtCore/QTextStream>
#include <QtCore/QMutex>
#include <QtCore/QAbstractEventDispatcher>
#include <QtCore/QCryptographicHash>

#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
QT_WARNING_DISABLE_DEPRECATED
#include <QtCore/QLinkedList>
#include <QtCore/QVector>
#endif

#if QT_VERSION < 0x050000
#include <QtGui/QStyleOption>
#endif
#include <stdio.h>
#include <string.h>
#ifdef Q_OS_WIN
#include <Windows.h>
#endif

#include "qtjambi_core.h"
#include "qtjambi_repository_p.h"
#include "qtjambi_containers.h"
#include "qtjambi_containeraccess.h"
#include "qtjambi_containeraccess_p.h"
#include "qtjambi_jobjectwrapper.h"
#include "qtjambi_registry_p.h"
#include "qtjambifunctiontable_p.h"
#include "qtjambilink_p.h"
#include "qtjambitypemanager_p.h"
#include "qtjambimetaobject_p.h"
#include "qtjambivariant_p.h"
#include "qtjambi_thread.h"
#include "qtjambi_interfaces_p.h"
#include "qtjambi_functionpointer.h"
#include "qtjambi_application.h"

#include "qtjambi_templates.h"
#include "qtjambi_cast.h"

#define EXCLUDE_GT_END(strg) strg //.endsWith(">") ? strg+" " : strg

jobject qtjambi_from_QIterator(JNIEnv *env,
                           QtJambiNativeID owner,
                           void* iteratorPtr,
                           PtrDeleterFunction destructor_function,
                           AbstractIteratorAccess* containerAccess)
{
    Q_ASSERT(containerAccess);
    jobject returned = nullptr;

    jobject obj = qtjambi_java_object_reference(owner, env);
    returned = Java::QtCore::QIterator::newInstance(env, obj);
    const QSharedPointer<QtJambiLink>& link = QtJambiLink::createLinkForOwnedContainer(env, returned, iteratorPtr,
                                                                                            LINK_NAME_ARG("QIterator")
                                                                                            owner, destructor_function, containerAccess);
    if (!link) {
        returned = nullptr;
        containerAccess->dispose();
    }
    return returned;
}

jobject qtjambi_from_QMapIterator(JNIEnv *env,
                                  QtJambiNativeID owner,
                                  void* iteratorPtr,
                                  PtrDeleterFunction destructor_function,
                                  AbstractBiIteratorAccess* containerAccess)
{
    Q_ASSERT(containerAccess);
    jobject returned = nullptr;

    jobject obj = qtjambi_java_object_reference(owner, env);
    returned = Java::QtCore::QMapIterator::newInstance(env, obj);
    const QSharedPointer<QtJambiLink>& link = QtJambiLink::createLinkForOwnedContainer(env, returned, iteratorPtr,
                                                                                            LINK_NAME_ARG("QMapIterator")
                                                                                            owner, destructor_function, containerAccess);
    if (!link) {
        returned = nullptr;
        containerAccess->dispose();
    }
    return returned;
}

jobject qtjambi_from_QList(JNIEnv *env,
                           QtJambiNativeID owner,
                           const void* listPtr,
                           CopyFunction copyFunction,
                           PtrDeleterFunction deleter,
                           ListType listType,
                           AbstractListAccess* containerAccess)
{
    Q_ASSERT(containerAccess);
    if(qtjambi_from_nativeId(owner)==listPtr){
        if(jobject obj = qtjambi_java_object_reference(owner, env)){
            switch(listType){
            case ListType::QQueue:
                if(Java::QtCore::QQueue::isInstanceOf(env, obj)){
                    return obj;
                }
                break;
#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
            case ListType::QStack:
                if(Java::QtCore::QStack::isInstanceOf(env, obj)){
                    return obj;
                }
                break;
#endif
            default:
                if(Java::QtCore::QList::isInstanceOf(env, obj)){
                    return obj;
                }
                break;
            }
        }
    }else{
        for(QSharedPointer<QtJambiLink> link : QtJambiLink::findLinksForPointer(listPtr)){
            if(link && (!copyFunction || link->createdByJava())){
                jobject obj = link->getJavaObjectLocalRef(env);
                switch(listType){
                case ListType::QQueue:
                    if(Java::QtCore::QQueue::isInstanceOf(env, obj)){
                        return obj;
                    }
                    break;
    #if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
                case ListType::QStack:
                    if(Java::QtCore::QStack::isInstanceOf(env, obj)){
                        return obj;
                    }
                    break;
    #endif
                default:
                    if(Java::QtCore::QList::isInstanceOf(env, obj)){
                        return obj;
                    }
                    break;
                }
            }
        }
    }
    jobject returned = nullptr;
    containerAccess = checkContainerAccess(env, containerAccess);
    QByteArray containerName;
    switch(listType){
    case ListType::QQueue:
        returned = Java::QtCore::QQueue::newInstance(env, nullptr);
        containerName = "QQueue<";
        break;
#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
    case ListType::QStack:
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
    if(!!owner){
        link = QtJambiLink::createLinkForOwnedContainer(env, returned, const_cast<void*>(listPtr),
                                                        LINK_NAME_META_TYPE_ARG(containerMetaType)
                                                        owner, containerAccess);
    }else if(deleter){
        if(copyFunction){
            link = QtJambiLink::createLinkForContainer(env, returned, copyFunction(listPtr),
                                                       LINK_NAME_META_TYPE_ARG(containerMetaType)
                                                       false, true, deleter, containerAccess);
        }else{
            link = QtJambiLink::createLinkForContainer(env, returned, const_cast<void*>(listPtr),
                                                       LINK_NAME_META_TYPE_ARG(containerMetaType)
                                                       false, false, deleter, containerAccess);
        }
    }else{
        if(copyFunction){
            link = QtJambiLink::createLinkForContainer(env, returned, copyFunction(listPtr),
                                                       containerMetaType,
                                                       false, true, containerAccess);
        }else{
            link = QtJambiLink::createLinkForContainer(env, returned, const_cast<void*>(listPtr),
                                                       containerMetaType,
                                                       false, false, containerAccess);
        }
    }
    if (!link) {
        returned = nullptr;
        containerAccess->dispose();
    }
    return returned;
}

jobject qtjambi_from_QList(JNIEnv *env,
                           const void* listPtr,
                           AbstractContainerAccess* containerAccess
                        )
{
    if(auto access = dynamic_cast<AbstractListAccess*>(containerAccess))
        return qtjambi_from_QList(env,
                                  InvalidNativeID,
                                  listPtr,
                                  nullptr,
                                  nullptr,
                                  ListType::QList,
                                  access
                             );
    return nullptr;
}

jobject qtjambi_from_QList(JNIEnv *env,
                             void* ptr_shared_pointer,
                             PointerDeleter sharedPointerDeleter,
                             PointerGetter sharedPointerGetter,
                             ListType listType,
                             AbstractListAccess* containerAccess
                        )
{
    Q_ASSERT(containerAccess);
    Q_ASSERT(sharedPointerGetter);
    Q_ASSERT(sharedPointerDeleter);
    if (!ptr_shared_pointer || !sharedPointerDeleter || !sharedPointerGetter)
        return nullptr;
    void* listPtr = sharedPointerGetter(ptr_shared_pointer);
    if (!listPtr)
        return nullptr;
    for(QSharedPointer<QtJambiLink> link : QtJambiLink::findLinksForPointer(listPtr)){
        if(link){
            jobject obj = link->getJavaObjectLocalRef(env);
            switch(listType){
            case ListType::QQueue:
                if(Java::QtCore::QQueue::isInstanceOf(env, obj)){
                    return obj;
                }
                break;
#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
            case ListType::QStack:
                if(Java::QtCore::QStack::isInstanceOf(env, obj)){
                    return obj;
                }
                break;
#endif
            default:
                if(Java::QtCore::QList::isInstanceOf(env, obj)){
                    return obj;
                }
                break;
            }
        }
    }

    jobject returned = nullptr;
    containerAccess = checkContainerAccess(env, containerAccess);
    QByteArray containerName;
    switch(listType){
    case ListType::QQueue:
        returned = Java::QtCore::QQueue::newInstance(env, nullptr);
        containerName = "QQueue<";
        break;
#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
    case ListType::QStack:
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
    const QSharedPointer<QtJambiLink>& link = QtJambiLink::createLinkForSharedPointerToContainer(env, returned,
                                                                                              LINK_NAME_META_TYPE_ARG(containerMetaType)
                                                                                              ptr_shared_pointer, sharedPointerDeleter, sharedPointerGetter, containerAccess);
    if (!link) {
        returned = nullptr;
        containerAccess->dispose();
    }
    return returned;
}

jobject qtjambi_from_QStringList(JNIEnv *env,
                           QtJambiNativeID owner,
                           const void* listPtr,
                           CopyFunction copyFunction,
                           PtrDeleterFunction deleter,
                           bool isConstant)
{
    if(qtjambi_from_nativeId(owner)==listPtr){
        if(jobject obj = qtjambi_java_object_reference(owner, env)){
            if(Java::QtCore::QStringList::isInstanceOf(env, obj)){
                return obj;
            }
        }
    }
    for(QSharedPointer<QtJambiLink> link : QtJambiLink::findLinksForPointer(listPtr)){
        if(link && (!copyFunction || link->createdByJava())){
            jobject obj = link->getJavaObjectLocalRef(env);
            if(Java::QtCore::QStringList::isInstanceOf(env, obj)){
                return obj;
            }
        }
    }
    jobject returned = nullptr;
    AbstractListAccess* containerAccess = isConstant
            ? QtJambiPrivate::QListAccess<QString,true>::newInstance()
            : QtJambiPrivate::QListAccess<QString,false>::newInstance();
    returned = Java::QtCore::QStringList::newInstance(env, nullptr);

    QSharedPointer<QtJambiLink> link;
    if(!!owner){
        link = QtJambiLink::createLinkForOwnedContainer(env, returned, const_cast<void*>(listPtr),
                                                        LINK_NAME_ARG("QStringList")
                                                        owner, containerAccess);
    }else if(deleter){
        if(copyFunction){
            link = QtJambiLink::createLinkForContainer(env, returned, copyFunction(listPtr),
                                                       LINK_NAME_ARG("QStringList")
                                                       false, true, deleter, containerAccess);
        }else{
            link = QtJambiLink::createLinkForContainer(env, returned, const_cast<void*>(listPtr),
                                                       LINK_NAME_ARG("QStringList")
                                                       false, false, deleter, containerAccess);
        }
    }else{
        if(copyFunction){
            link = QtJambiLink::createLinkForContainer(env, returned, copyFunction(listPtr),
                                                       QMetaType(QMetaType::QStringList),
                                                       false, true, containerAccess);
        }else{
            link = QtJambiLink::createLinkForContainer(env, returned, const_cast<void*>(listPtr),
                                                       QMetaType(QMetaType::QStringList),
                                                       false, false, containerAccess);
        }
    }
    if (!link) {
        returned = nullptr;
        containerAccess->dispose();
    }
    return returned;
}

jobject qtjambi_from_QStringList(JNIEnv *env,
                             void* ptr_shared_pointer,
                             PointerDeleter sharedPointerDeleter,
                             PointerGetter sharedPointerGetter,
                             bool isConstant
                        )
{
    Q_ASSERT(sharedPointerGetter);
    Q_ASSERT(sharedPointerDeleter);
    if (!ptr_shared_pointer || !sharedPointerDeleter || !sharedPointerGetter)
        return nullptr;
    void* listPtr = sharedPointerGetter(ptr_shared_pointer);
    if (!listPtr)
        return nullptr;
    for(QSharedPointer<QtJambiLink> link : QtJambiLink::findLinksForPointer(listPtr)){
        if(link){
            jobject obj = link->getJavaObjectLocalRef(env);
            if(Java::QtCore::QStringList::isInstanceOf(env, obj)){
                return obj;
            }
        }
    }

    AbstractListAccess* containerAccess = isConstant
            ? QtJambiPrivate::QListAccess<QString,true>::newInstance()
            : QtJambiPrivate::QListAccess<QString,false>::newInstance();

    jobject returned = nullptr;
    returned = Java::QtCore::QStringList::newInstance(env, nullptr);
    const QSharedPointer<QtJambiLink>& link = QtJambiLink::createLinkForSharedPointerToContainer(env, returned,
                                                                                              LINK_NAME_ARG("QStringList")
                                                                                              ptr_shared_pointer, sharedPointerDeleter, sharedPointerGetter, containerAccess);
    if (!link) {
        returned = nullptr;
        containerAccess->dispose();
    }
    return returned;
}

jobject qtjambi_from_QSet(JNIEnv *env,
                         QtJambiNativeID owner,
                         const void* listPtr,
                         CopyFunction copyFunction,
                         PtrDeleterFunction deleter,
                         AbstractSetAccess* containerAccess)
{
    Q_ASSERT(containerAccess);
    if(qtjambi_from_nativeId(owner)==listPtr){
        jobject obj = qtjambi_java_object_reference(owner, env);
        if(Java::QtCore::QSet::isInstanceOf(env, obj)){
            return obj;
        }
    }
    for(QSharedPointer<QtJambiLink> link : QtJambiLink::findLinksForPointer(listPtr)){
        if(link && (!copyFunction || link->createdByJava())){
            jobject obj = link->getJavaObjectLocalRef(env);
            if(Java::QtCore::QSet::isInstanceOf(env, obj)){
                return obj;
            }
        }
    }

    jobject returned = nullptr;
    containerAccess = checkContainerAccess(env, containerAccess);
    returned = Java::QtCore::QSet::newInstance(env, nullptr);
    QSharedPointer<QtJambiLink> link;
    QByteArray containerName = "QSet<";
    containerName += containerAccess->elementMetaType().name();
    containerName += ">";
    containerName = QMetaObject::normalizedType(containerName);
    QMetaType containerMetaType(containerAccess->registerContainer(containerName));
    if(!!owner){
        link = QtJambiLink::createLinkForOwnedContainer(env, returned, const_cast<void*>(listPtr),
                                                        LINK_NAME_META_TYPE_ARG(containerMetaType)
                                                        owner, containerAccess);
    }else if(deleter){
        if(copyFunction){
            link = QtJambiLink::createLinkForContainer(env, returned, copyFunction(listPtr),
                                                       LINK_NAME_META_TYPE_ARG(containerMetaType)
                                                       false, true, deleter, containerAccess);
        }else{
            link = QtJambiLink::createLinkForContainer(env, returned, const_cast<void*>(listPtr),
                                                       LINK_NAME_META_TYPE_ARG(containerMetaType)
                                                       false, false, deleter, containerAccess);
        }
    }else{
        if(copyFunction){
            link = QtJambiLink::createLinkForContainer(env, returned, copyFunction(listPtr),
                                                       containerMetaType,
                                                       false, true, containerAccess);
        }else{
            link = QtJambiLink::createLinkForContainer(env, returned, const_cast<void*>(listPtr),
                                                       containerMetaType,
                                                       false, false, containerAccess);
        }
    }
    if (!link) {
        returned = nullptr;
        containerAccess->dispose();
    }
    return returned;
}

jobject qtjambi_from_QSet(JNIEnv *env,
                         const void* listPtr,
                         AbstractContainerAccess* containerAccess)
{
    if(auto access = dynamic_cast<AbstractSetAccess*>(containerAccess))
        return qtjambi_from_QSet(env,
                                 InvalidNativeID,
                                 listPtr,
                                 nullptr,
                                 nullptr,
                                 access);
    return nullptr;
}

jobject qtjambi_from_QSet(JNIEnv *env,
                                     void* ptr_shared_pointer,
                                     PointerDeleter sharedPointerDeleter,
                                     PointerGetter sharedPointerGetter,
                                     AbstractSetAccess* containerAccess
                                )
{
    Q_ASSERT(containerAccess);
    Q_ASSERT(sharedPointerGetter);
    Q_ASSERT(sharedPointerDeleter);
    if (!ptr_shared_pointer || !sharedPointerDeleter || !sharedPointerGetter)
        return nullptr;
    void* listPtr = sharedPointerGetter(ptr_shared_pointer);
    if (!listPtr)
        return nullptr;
    for(QSharedPointer<QtJambiLink> link : QtJambiLink::findLinksForPointer(listPtr)){
        if(link){
            jobject obj = link->getJavaObjectLocalRef(env);
            if(Java::QtCore::QSet::isInstanceOf(env, obj)){
                return obj;
            }
        }
    }

    jobject returned = nullptr;
    containerAccess = checkContainerAccess(env, containerAccess);
    returned = Java::QtCore::QSet::newInstance(env, nullptr);
    QByteArray containerName = "QSet<";
    containerName += containerAccess->elementMetaType().name();
    containerName += ">";
    containerName = QMetaObject::normalizedType(containerName);
    QMetaType containerMetaType(containerAccess->registerContainer(containerName));
    const QSharedPointer<QtJambiLink>& link = QtJambiLink::createLinkForSharedPointerToContainer(env, returned,
                                                                                              LINK_NAME_META_TYPE_ARG(containerMetaType)
                                                                                              ptr_shared_pointer, sharedPointerDeleter, sharedPointerGetter, containerAccess);
    if (!link) {
        returned = nullptr;
        containerAccess->dispose();
    }
    return returned;
}

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
jobject qtjambi_from_QLinkedList(JNIEnv *env,
                                     QtJambiNativeID owner,
                                     const void* listPtr,
                                     CopyFunction copyFunction,
                                     PtrDeleterFunction deleter,
                                     AbstractLinkedListAccess* containerAccess
                                )
{
    Q_ASSERT(containerAccess);
    QT_WARNING_DISABLE_DEPRECATED
    if(qtjambi_from_nativeId(owner)==listPtr){
        jobject obj = qtjambi_java_object_reference(owner, env);
        if(Java::QtCore::QLinkedList::isInstanceOf(env, obj)){
            return obj;
        }
    }
    for(QSharedPointer<QtJambiLink> link : QtJambiLink::findLinksForPointer(listPtr)){
        jobject obj = link->getJavaObjectLocalRef(env);
        if(Java::QtCore::QLinkedList::isInstanceOf(env, obj)){
            return obj;
        }
    }

    jobject returned = nullptr;
    containerAccess = checkContainerAccess(env, containerAccess);
    returned = Java::QtCore::QLinkedList::newInstance(env, nullptr);
    QSharedPointer<QtJambiLink> link;
    QByteArray containerName = "QLinkedList<";
    containerName += containerAccess->elementMetaType().name();
    containerName += ">";
    containerName = QMetaObject::normalizedType(containerName);
    QMetaType containerMetaType(containerAccess->registerContainer(containerName));
    if(!!owner){
        link = QtJambiLink::createLinkForOwnedContainer(env, returned, const_cast<void*>(listPtr),
                                                        LINK_NAME_META_TYPE_ARG(containerMetaType)
                                                        owner, containerAccess);
    }else if(deleter){
        if(copyFunction){
            link = QtJambiLink::createLinkForContainer(env, returned, copyFunction(listPtr),
                                                       LINK_NAME_META_TYPE_ARG(containerMetaType)
                                                       false, true, deleter, containerAccess);
        }else{
            link = QtJambiLink::createLinkForContainer(env, returned, const_cast<void*>(listPtr),
                                                       LINK_NAME_META_TYPE_ARG(containerMetaType)
                                                       false, false, deleter, containerAccess);
        }
    }else{
        if(copyFunction){
            link = QtJambiLink::createLinkForContainer(env, returned, copyFunction(listPtr),
                                                       containerMetaType,
                                                       false, true, containerAccess);
        }else{
            link = QtJambiLink::createLinkForContainer(env, returned, const_cast<void*>(listPtr),
                                                       containerMetaType,
                                                       false, false, containerAccess);
        }
    }
    if (!link) {
        returned = nullptr;
        containerAccess->dispose();
    }
    return returned;
}

jobject qtjambi_from_QLinkedList(JNIEnv *env,
                                     void* ptr_shared_pointer,
                                     PointerDeleter sharedPointerDeleter,
                                     PointerGetter sharedPointerGetter,
                                     AbstractLinkedListAccess* containerAccess
                                )
{
    Q_ASSERT(containerAccess);
    Q_ASSERT(sharedPointerGetter);
    Q_ASSERT(sharedPointerDeleter);
    if (!ptr_shared_pointer || !sharedPointerDeleter || !sharedPointerGetter)
        return nullptr;
    void* listPtr = sharedPointerGetter(ptr_shared_pointer);
    if (!listPtr)
        return nullptr;
    for(QSharedPointer<QtJambiLink> link : QtJambiLink::findLinksForPointer(listPtr)){
        if(link){
            jobject obj = link->getJavaObjectLocalRef(env);
            if(Java::QtCore::QLinkedList::isInstanceOf(env, obj)){
                return obj;
            }
        }
    }

    jobject returned = nullptr;
    containerAccess = checkContainerAccess(env, containerAccess);
    returned = Java::QtCore::QLinkedList::newInstance(env, nullptr);
    QByteArray containerName = "QLinkedList<";
    containerName += containerAccess->elementMetaType().name();
    containerName += ">";
    containerName = QMetaObject::normalizedType(containerName);
    QMetaType containerMetaType(containerAccess->registerContainer(containerName));
    const QSharedPointer<QtJambiLink>& link = QtJambiLink::createLinkForSharedPointerToContainer(env, returned,
                                                                                              LINK_NAME_META_TYPE_ARG(containerMetaType)
                                                                                              ptr_shared_pointer, sharedPointerDeleter, sharedPointerGetter, containerAccess);
    if (!link) {
        returned = nullptr;
        containerAccess->dispose();
    }
    return returned;
}

jobject qtjambi_from_QVector(JNIEnv *env,
                             QtJambiNativeID owner,
                             const void* listPtr,
                             CopyFunction copyFunction,
                             PtrDeleterFunction deleter,
                             VectorType vectorType,
                             AbstractVectorAccess* containerAccess)
{
    Q_ASSERT(containerAccess);
    if(qtjambi_from_nativeId(owner)==listPtr){
        jobject obj = qtjambi_java_object_reference(owner, env);
        if(vectorType==VectorType::QStack){
            if(Java::QtCore::QStack::isInstanceOf(env, obj)){
                return obj;
            }
        }else{
            if(Java::QtCore::QVector::isInstanceOf(env, obj)){
                return obj;
            }
        }
    }
    for(QSharedPointer<QtJambiLink> link : QtJambiLink::findLinksForPointer(listPtr)){
        jobject obj = link->getJavaObjectLocalRef(env);
        if(vectorType==VectorType::QStack){
            if(Java::QtCore::QStack::isInstanceOf(env, obj)){
                return obj;
            }
        }else{
            if(Java::QtCore::QVector::isInstanceOf(env, obj)){
                return obj;
            }
        }
    }

    jobject returned = nullptr;
    containerAccess = checkContainerAccess(env, containerAccess);
    QByteArray containerName;
    if(vectorType==VectorType::QStack){
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
    if(!!owner){
        link = QtJambiLink::createLinkForOwnedContainer(env, returned, const_cast<void*>(listPtr),
                                                        LINK_NAME_META_TYPE_ARG(containerMetaType)
                                                        owner, containerAccess);
    }else if(deleter){
        if(copyFunction){
            link = QtJambiLink::createLinkForContainer(env, returned, copyFunction(listPtr),
                                                       LINK_NAME_META_TYPE_ARG(containerMetaType)
                                                       false, true, deleter, containerAccess);
        }else{
            link = QtJambiLink::createLinkForContainer(env, returned, const_cast<void*>(listPtr),
                                                       LINK_NAME_META_TYPE_ARG(containerMetaType)
                                                       false, false, deleter, containerAccess);
        }
    }else{
        if(copyFunction){
            link = QtJambiLink::createLinkForContainer(env, returned, copyFunction(listPtr),
                                                       containerMetaType,
                                                       false, true, containerAccess);
        }else{
            link = QtJambiLink::createLinkForContainer(env, returned, const_cast<void*>(listPtr),
                                                       containerMetaType,
                                                       false, false, containerAccess);
        }
    }
    if (!link) {
        returned = nullptr;
        containerAccess->dispose();
    }
    return returned;
}

jobject qtjambi_from_QVector(JNIEnv *env,
                             const void* listPtr,
                             AbstractContainerAccess* containerAccess)
{
    if(auto access = dynamic_cast<AbstractVectorAccess*>(containerAccess))
        return qtjambi_from_QVector(env,
                                    InvalidNativeID,
                                    listPtr,
                                    nullptr,
                                    nullptr,
                                    VectorType::QVector,
                                    access);
    return nullptr;
}

jobject qtjambi_from_QVector(JNIEnv *env,
                            void* ptr_shared_pointer,
                            PointerDeleter sharedPointerDeleter,
                            PointerGetter sharedPointerGetter,
                            VectorType vectorType,
                            AbstractVectorAccess* containerAccess
                )
{
    Q_ASSERT(containerAccess);
    Q_ASSERT(sharedPointerGetter);
    Q_ASSERT(sharedPointerDeleter);
    if (!ptr_shared_pointer || !sharedPointerDeleter || !sharedPointerGetter)
        return nullptr;
    void* listPtr = sharedPointerGetter(ptr_shared_pointer);
    if (!listPtr)
        return nullptr;
    for(QSharedPointer<QtJambiLink> link : QtJambiLink::findLinksForPointer(listPtr)){
        if(link){
            jobject obj = link->getJavaObjectLocalRef(env);
            if(vectorType==VectorType::QStack){
                if(Java::QtCore::QStack::isInstanceOf(env, obj)){
                    return obj;
                }
            }else{
                if(Java::QtCore::QVector::isInstanceOf(env, obj)){
                    return obj;
                }
            }
        }
    }

    jobject returned = nullptr;
    containerAccess = checkContainerAccess(env, containerAccess);
    QByteArray containerName;
    if(vectorType==VectorType::QStack){
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
    const QSharedPointer<QtJambiLink>& link = QtJambiLink::createLinkForSharedPointerToContainer(env, returned,
                                                                                              LINK_NAME_META_TYPE_ARG(containerMetaType)
                                                                                              ptr_shared_pointer, sharedPointerDeleter, sharedPointerGetter, containerAccess);
    if (!link) {
        returned = nullptr;
        containerAccess->dispose();
    }
    return returned;
}

#endif

jobject qtjambi_from_QHash(JNIEnv *env,
                                     QtJambiNativeID owner,
                                     const void* listPtr,
                                     CopyFunction copyFunction,
                                     PtrDeleterFunction deleter,
                                     AbstractHashAccess* containerAccess
                                )
{
    Q_ASSERT(containerAccess);
    if(qtjambi_from_nativeId(owner)==listPtr){
        jobject obj = qtjambi_java_object_reference(owner, env);
        if(Java::QtCore::QHash::isInstanceOf(env, obj)){
            return obj;
        }
    }
    for(QSharedPointer<QtJambiLink> link : QtJambiLink::findLinksForPointer(listPtr)){
        jobject obj = link->getJavaObjectLocalRef(env);
        if(Java::QtCore::QHash::isInstanceOf(env, obj)){
            return obj;
        }
    }
    jobject returned = nullptr;
    containerAccess = checkContainerAccess(env, containerAccess);
    returned = Java::QtCore::QHash::newInstance(env, nullptr);
    QByteArray containerName = "QHash<";
    containerName += containerAccess->keyMetaType().name();
    containerName += ",";
    containerName += containerAccess->valueMetaType().name();
    containerName += ">";
    containerName = QMetaObject::normalizedType(containerName);
    QMetaType containerMetaType(containerAccess->registerContainer(containerName));
    QSharedPointer<QtJambiLink> link;
    if(!!owner){
        link = QtJambiLink::createLinkForOwnedContainer(env, returned, const_cast<void*>(listPtr),
                                                        LINK_NAME_META_TYPE_ARG(containerMetaType)
                                                        owner, containerAccess);
    }else if(deleter){
        if(copyFunction){
            link = QtJambiLink::createLinkForContainer(env, returned, copyFunction(listPtr),
                                                       LINK_NAME_META_TYPE_ARG(containerMetaType)
                                                       false, true, deleter, containerAccess);
        }else{
            link = QtJambiLink::createLinkForContainer(env, returned, const_cast<void*>(listPtr),
                                                       LINK_NAME_META_TYPE_ARG(containerMetaType)
                                                       false, false, deleter, containerAccess);
        }
    }else{
        if(copyFunction){
            link = QtJambiLink::createLinkForContainer(env, returned, copyFunction(listPtr),
                                                       containerMetaType,
                                                       false, true, containerAccess);
        }else{
            link = QtJambiLink::createLinkForContainer(env, returned, const_cast<void*>(listPtr),
                                                       containerMetaType,
                                                       false, false, containerAccess);
        }
    }
    if (!link) {
        returned = nullptr;
        containerAccess->dispose();
    }
    return returned;
}

jobject qtjambi_from_QHash(JNIEnv *env,
                                     void* ptr_shared_pointer,
                                     PointerDeleter sharedPointerDeleter,
                                     PointerGetter sharedPointerGetter,
                                     AbstractHashAccess* containerAccess
                                )
{
    Q_ASSERT(containerAccess);
    Q_ASSERT(sharedPointerGetter);
    Q_ASSERT(sharedPointerDeleter);
    if (!ptr_shared_pointer || !sharedPointerDeleter || !sharedPointerGetter)
        return nullptr;
    void* listPtr = sharedPointerGetter(ptr_shared_pointer);
    if (!listPtr)
        return nullptr;
    for(QSharedPointer<QtJambiLink> link : QtJambiLink::findLinksForPointer(listPtr)){
        if(link){
            jobject obj = link->getJavaObjectLocalRef(env);
            if(Java::QtCore::QHash::isInstanceOf(env, obj)){
                return obj;
            }
        }
    }

    jobject returned = nullptr;
    containerAccess = checkContainerAccess(env, containerAccess);
    returned = Java::QtCore::QHash::newInstance(env, nullptr);
    QByteArray containerName = "QHash<";
    containerName += containerAccess->keyMetaType().name();
    containerName += ",";
    containerName += containerAccess->valueMetaType().name();
    containerName += ">";
    containerName = QMetaObject::normalizedType(containerName);
    QMetaType containerMetaType(containerAccess->registerContainer(containerName));
    const QSharedPointer<QtJambiLink>& link = QtJambiLink::createLinkForSharedPointerToContainer(env, returned,
                                                                                              LINK_NAME_META_TYPE_ARG(containerMetaType)
                                                                                              ptr_shared_pointer, sharedPointerDeleter, sharedPointerGetter, containerAccess);
    if (!link) {
        returned = nullptr;
        containerAccess->dispose();
    }
    return returned;
}

jobject qtjambi_from_QMultiHash(JNIEnv *env,
                                     QtJambiNativeID owner,
                                     const void* listPtr,
                                     CopyFunction copyFunction,
                                     PtrDeleterFunction deleter,
                                     AbstractMultiHashAccess* containerAccess
                                )
{
    Q_ASSERT(containerAccess);
    if(qtjambi_from_nativeId(owner)==listPtr){
        jobject obj = qtjambi_java_object_reference(owner, env);
        if(Java::QtCore::QMultiHash::isInstanceOf(env, obj)){
            return obj;
        }
    }
    for(QSharedPointer<QtJambiLink> link : QtJambiLink::findLinksForPointer(listPtr)){
        jobject obj = link->getJavaObjectLocalRef(env);
        if(Java::QtCore::QMultiHash::isInstanceOf(env, obj)){
            return obj;
        }
    }

    jobject returned = nullptr;
    containerAccess = checkContainerAccess(env, containerAccess);
    returned = Java::QtCore::QMultiHash::newInstance(env, nullptr);
    QSharedPointer<QtJambiLink> link;
    QByteArray containerName = "QMultiHash<";
    containerName += containerAccess->keyMetaType().name();
    containerName += ",";
    containerName += containerAccess->valueMetaType().name();
    containerName += ">";
    containerName = QMetaObject::normalizedType(containerName);
    QMetaType containerMetaType(containerAccess->registerContainer(containerName));
    if(!!owner){
        link = QtJambiLink::createLinkForOwnedContainer(env, returned, const_cast<void*>(listPtr),
                                                        LINK_NAME_META_TYPE_ARG(containerMetaType)
                                                        owner, containerAccess);
    }else if(deleter){
        if(copyFunction){
            link = QtJambiLink::createLinkForContainer(env, returned, copyFunction(listPtr),
                                                       LINK_NAME_META_TYPE_ARG(containerMetaType)
                                                       false, true, deleter, containerAccess);
        }else{
            link = QtJambiLink::createLinkForContainer(env, returned, const_cast<void*>(listPtr),
                                                       LINK_NAME_META_TYPE_ARG(containerMetaType)
                                                       false, false, deleter, containerAccess);
        }
    }else{
        if(copyFunction){
            link = QtJambiLink::createLinkForContainer(env, returned, copyFunction(listPtr),
                                                       containerMetaType,
                                                       false, true, containerAccess);
        }else{
            link = QtJambiLink::createLinkForContainer(env, returned, const_cast<void*>(listPtr),
                                                       containerMetaType,
                                                       false, false, containerAccess);
        }
    }
    if (!link) {
        returned = nullptr;
        containerAccess->dispose();
    }
    return returned;
}

jobject qtjambi_from_QMultiHash(JNIEnv *env,
                                     void* ptr_shared_pointer,
                                     PointerDeleter sharedPointerDeleter,
                                     PointerGetter sharedPointerGetter,
                                     AbstractMultiHashAccess* containerAccess
                                )
{
    Q_ASSERT(containerAccess);
    Q_ASSERT(sharedPointerGetter);
    Q_ASSERT(sharedPointerDeleter);
    if (!ptr_shared_pointer || !sharedPointerDeleter || !sharedPointerGetter)
        return nullptr;
    void* listPtr = sharedPointerGetter(ptr_shared_pointer);
    if (!listPtr)
        return nullptr;
    for(QSharedPointer<QtJambiLink> link : QtJambiLink::findLinksForPointer(listPtr)){
        if(link){
            jobject obj = link->getJavaObjectLocalRef(env);
            if(Java::QtCore::QMultiHash::isInstanceOf(env, obj)){
                return obj;
            }
        }
    }

    jobject returned = nullptr;
    containerAccess = checkContainerAccess(env, containerAccess);
    returned = Java::QtCore::QMultiHash::newInstance(env, nullptr);
    QByteArray containerName = "QMultiHash<";
    containerName += containerAccess->keyMetaType().name();
    containerName += ",";
    containerName += containerAccess->valueMetaType().name();
    containerName += ">";
    containerName = QMetaObject::normalizedType(containerName);
    QMetaType containerMetaType(containerAccess->registerContainer(containerName));
    const QSharedPointer<QtJambiLink>& link = QtJambiLink::createLinkForSharedPointerToContainer(env, returned,
                                                                                              LINK_NAME_META_TYPE_ARG(containerMetaType)
                                                                                              ptr_shared_pointer, sharedPointerDeleter, sharedPointerGetter, containerAccess);
    if (!link) {
        returned = nullptr;
        containerAccess->dispose();
    }
    return returned;
}

jobject qtjambi_from_QMap(JNIEnv *env,
                                     QtJambiNativeID owner,
                                     const void* listPtr,
                                     CopyFunction copyFunction,
                                     PtrDeleterFunction deleter,
                                     AbstractMapAccess* containerAccess
                                )
{
    Q_ASSERT(containerAccess);
    if(qtjambi_from_nativeId(owner)==listPtr){
        jobject obj = qtjambi_java_object_reference(owner, env);
        if(Java::QtCore::QMap::isInstanceOf(env, obj)){
            return obj;
        }
    }
    for(QSharedPointer<QtJambiLink> link : QtJambiLink::findLinksForPointer(listPtr)){
        jobject obj = link->getJavaObjectLocalRef(env);
        if(Java::QtCore::QMap::isInstanceOf(env, obj)){
            return obj;
        }
    }
    jobject returned = nullptr;
    containerAccess = checkContainerAccess(env, containerAccess);
    returned = Java::QtCore::QMap::newInstance(env, nullptr);
    QByteArray containerName = "QMap<";
    containerName += containerAccess->keyMetaType().name();
    containerName += ",";
    containerName += containerAccess->valueMetaType().name();
    containerName += ">";
    containerName = QMetaObject::normalizedType(containerName);
    QMetaType containerMetaType(containerAccess->registerContainer(containerName));
    QSharedPointer<QtJambiLink> link;
    if(!!owner){
        link = QtJambiLink::createLinkForOwnedContainer(env, returned, const_cast<void*>(listPtr),
                                                        LINK_NAME_META_TYPE_ARG(containerMetaType)
                                                        owner, containerAccess);
    }else if(deleter){
        if(copyFunction){
            link = QtJambiLink::createLinkForContainer(env, returned, copyFunction(listPtr),
                                                       LINK_NAME_META_TYPE_ARG(containerMetaType)
                                                       false, true, deleter, containerAccess);
        }else{
            link = QtJambiLink::createLinkForContainer(env, returned, const_cast<void*>(listPtr),
                                                       LINK_NAME_META_TYPE_ARG(containerMetaType)
                                                       false, false, deleter, containerAccess);
        }
    }else{
        if(copyFunction){
            link = QtJambiLink::createLinkForContainer(env, returned, copyFunction(listPtr),
                                                       containerMetaType,
                                                       false, true, containerAccess);
        }else{
            link = QtJambiLink::createLinkForContainer(env, returned, const_cast<void*>(listPtr),
                                                       containerMetaType,
                                                       false, false, containerAccess);
        }
    }
    if (!link) {
        returned = nullptr;
        containerAccess->dispose();
    }
    return returned;
}

jobject qtjambi_from_QMap(JNIEnv *env,
                                     void* ptr_shared_pointer,
                                     PointerDeleter sharedPointerDeleter,
                                     PointerGetter sharedPointerGetter,
                                     AbstractMapAccess* containerAccess
                                )
{
    Q_ASSERT(containerAccess);
    Q_ASSERT(sharedPointerGetter);
    Q_ASSERT(sharedPointerDeleter);
    if (!ptr_shared_pointer || !sharedPointerDeleter || !sharedPointerGetter)
        return nullptr;
    void* listPtr = sharedPointerGetter(ptr_shared_pointer);
    if (!listPtr)
        return nullptr;
    for(QSharedPointer<QtJambiLink> link : QtJambiLink::findLinksForPointer(listPtr)){
        if(link){
            jobject obj = link->getJavaObjectLocalRef(env);
            if(Java::QtCore::QMap::isInstanceOf(env, obj)){
                return obj;
            }
        }
    }

    jobject returned = nullptr;
    containerAccess = checkContainerAccess(env, containerAccess);
    returned = Java::QtCore::QMap::newInstance(env, nullptr);
    QByteArray containerName = "QMap<";
    containerName += containerAccess->keyMetaType().name();
    containerName += ",";
    containerName += containerAccess->valueMetaType().name();
    containerName += ">";
    containerName = QMetaObject::normalizedType(containerName);
    QMetaType containerMetaType(containerAccess->registerContainer(containerName));
    const QSharedPointer<QtJambiLink>& link = QtJambiLink::createLinkForSharedPointerToContainer(env, returned,
                                                                                                LINK_NAME_META_TYPE_ARG(containerMetaType)
                                                                                                ptr_shared_pointer, sharedPointerDeleter, sharedPointerGetter, containerAccess);
    if (!link) {
        returned = nullptr;
        containerAccess->dispose();
    }
    return returned;
}

jobject qtjambi_from_QMultiMap(JNIEnv *env,
                                     QtJambiNativeID owner,
                                     const void* listPtr,
                                     CopyFunction copyFunction,
                                     PtrDeleterFunction deleter,
                                     AbstractMultiMapAccess* containerAccess
                                )
{
    Q_ASSERT(containerAccess);
    if(qtjambi_from_nativeId(owner)==listPtr){
        jobject obj = qtjambi_java_object_reference(owner, env);
        if(Java::QtCore::QMultiMap::isInstanceOf(env, obj)){
            return obj;
        }
    }
    for(QSharedPointer<QtJambiLink> link : QtJambiLink::findLinksForPointer(listPtr)){
        jobject obj = link->getJavaObjectLocalRef(env);
        if(Java::QtCore::QMultiMap::isInstanceOf(env, obj)){
            return obj;
        }
    }

    jobject returned = nullptr;
    containerAccess = checkContainerAccess(env, containerAccess);
    returned = Java::QtCore::QMultiMap::newInstance(env, nullptr);
    QByteArray containerName = "QMultiMap<";
    containerName += containerAccess->keyMetaType().name();
    containerName += ",";
    containerName += containerAccess->valueMetaType().name();
    containerName += ">";
    containerName = QMetaObject::normalizedType(containerName);
    QMetaType containerMetaType(containerAccess->registerContainer(containerName));
    QSharedPointer<QtJambiLink> link;
    if(!!owner){
        link = QtJambiLink::createLinkForOwnedContainer(env, returned, const_cast<void*>(listPtr),
                                                        LINK_NAME_META_TYPE_ARG(containerMetaType)
                                                        owner, containerAccess);
    }else if(deleter){
        if(copyFunction){
            link = QtJambiLink::createLinkForContainer(env, returned, copyFunction(listPtr),
                                                       LINK_NAME_META_TYPE_ARG(containerMetaType)
                                                       false, true, deleter, containerAccess);
        }else{
            link = QtJambiLink::createLinkForContainer(env, returned, const_cast<void*>(listPtr),
                                                       LINK_NAME_META_TYPE_ARG(containerMetaType)
                                                       false, false, deleter, containerAccess);
        }
    }else{
        if(copyFunction){
            link = QtJambiLink::createLinkForContainer(env, returned, copyFunction(listPtr),
                                                       containerMetaType,
                                                       false, true, containerAccess);
        }else{
            link = QtJambiLink::createLinkForContainer(env, returned, const_cast<void*>(listPtr),
                                                       containerMetaType,
                                                       false, false, containerAccess);
        }
    }
    if (!link) {
        returned = nullptr;
        containerAccess->dispose();
    }
    return returned;
}

jobject qtjambi_from_QMultiMap(JNIEnv *env,
                                     void* ptr_shared_pointer,
                                     PointerDeleter sharedPointerDeleter,
                                     PointerGetter sharedPointerGetter,
                                     AbstractMultiMapAccess* containerAccess
                                )
{
    Q_ASSERT(containerAccess);
    Q_ASSERT(sharedPointerGetter);
    Q_ASSERT(sharedPointerDeleter);
    if (!ptr_shared_pointer || !sharedPointerDeleter || !sharedPointerGetter)
        return nullptr;
    void* listPtr = sharedPointerGetter(ptr_shared_pointer);
    if (!listPtr)
        return nullptr;
    for(QSharedPointer<QtJambiLink> link : QtJambiLink::findLinksForPointer(listPtr)){
        if(link){
            jobject obj = link->getJavaObjectLocalRef(env);
            if(Java::QtCore::QMultiMap::isInstanceOf(env, obj)){
                return obj;
            }
        }
    }

    jobject returned = nullptr;
    containerAccess = checkContainerAccess(env, containerAccess);
    returned = Java::QtCore::QMultiMap::newInstance(env, nullptr);
    QByteArray containerName = "QMultiMap<";
    containerName += containerAccess->keyMetaType().name();
    containerName += ",";
    containerName += containerAccess->valueMetaType().name();
    containerName += ">";
    containerName = QMetaObject::normalizedType(containerName);
    QMetaType containerMetaType(containerAccess->registerContainer(containerName));
    const QSharedPointer<QtJambiLink>& link = QtJambiLink::createLinkForSharedPointerToContainer(env, returned,
                                                                                                LINK_NAME_META_TYPE_ARG(containerMetaType)
                                                                                                ptr_shared_pointer, sharedPointerDeleter, sharedPointerGetter, containerAccess);
    if (!link) {
        returned = nullptr;
        containerAccess->dispose();
    }
    return returned;
}

template<typename T>
T& checkedUnref(T* t){
    Q_ASSERT(t);
    return *t;
}

int qtjambi_register_metatype(JNIEnv *env, jclass clazz, jboolean isPointer, jboolean isReference);

void initialize_QList(JNIEnv *env, jobject object, jclass elementType, QtJambiNativeID elementMetaTypeId, jobject other){
    QMetaType& elementMetaType = checkedUnref(qtjambi_object_from_nativeId<QMetaType>(elementMetaTypeId));
    SuperTypeInfos superTypeInfos = getSuperTypeInfos(env, env->GetObjectClass(object));
    if(superTypeInfos.size()>1)
        JavaException::raiseError(env, qPrintable( QString("It is not permitted to create a derived type of %1 implementing any Qt interface.").arg("QList") ) QTJAMBI_STACKTRACEINFO );
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
            Java::QtJambi::QNoNativeResourcesException::throwNew(env, QString("Incomplete object of type: %1").arg(qtjambi_object_class_name(env, other).replace("$", ".")) QTJAMBI_STACKTRACEINFO );
        }
    }
    if(!containerAccess){
        switch(elementMetaType.id()){
        case QMetaType::VoidStar:
            containerAccess = QListAccess<void*>::newInstance();
            break;
        case QMetaType::Bool:
            containerAccess = QListAccess<bool>::newInstance();
            break;
        case QMetaType::Char:
        case QMetaType::SChar:
        case QMetaType::UChar:
            containerAccess = QListAccess<qint8>::newInstance();
            break;
        case QMetaType::Short:
        case QMetaType::UShort:
            containerAccess = QListAccess<qint16>::newInstance();
            break;
        case QMetaType::Int:
        case QMetaType::UInt:
            containerAccess = QListAccess<qint32>::newInstance();
            break;
        case QMetaType::LongLong:
        case QMetaType::ULongLong:
            containerAccess = QListAccess<qint64>::newInstance();
            break;
        case QMetaType::Double:
            containerAccess = QListAccess<double>::newInstance();
            break;
        case QMetaType::Float:
            containerAccess = QListAccess<float>::newInstance();
            break;
        case QMetaType::QChar:
            containerAccess = QListAccess<QChar>::newInstance();
            break;
        case QMetaType::QString:
            containerAccess = QListAccess<QString>::newInstance();
            break;
        case QMetaType::QVariant:
            containerAccess = QListAccess<QVariant>::newInstance();
            break;
        case QMetaType::QObjectStar:
            containerAccess = QListAccess<QObject*>::newInstance();
            break;
        default: {
                elementType = getGlobalClassRef(env, elementType);
                QByteArray qTypeName = elementMetaType.name();
                size_t size = size_t(elementMetaType.sizeOf());
#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
                bool isPointer = elementMetaType.flags() & QMetaType::IsPointer;
                size_t align = size_t(elementMetaType.alignOf());
#else
                bool isStaticType = false;
                bool isPointer = qTypeName.endsWith("*");
                size_t align = 0;
                {
                    const std::type_info* typeId = getTypeByMetaType(elementMetaType.id());
                    if(!typeId)
                        typeId = getTypeByQtName(qTypeName);
                    if(typeId){
                        if(isFunctional(*typeId)){
                            QString typeName = qtjambi_type_name(*typeId);
                            if(!typeName.startsWith("std::function") && (typeName.contains("(*)"))){
                                isPointer = true;
                            }
                        }
                        if(const QtJambiTypeInfo* typeInfo = getQTypeInfo(*typeId))
                            isStaticType = typeInfo->isStatic;
                        if(!isPointer)
                            align = getValueAlignment(*typeId);
                    }
                }
#endif
                if(isPointer)
                    size = 0;
                InternalToExternalConverter internalToExternalConverter = QtJambiTypeManager::getInternalToExternalConverter(
                                                                                                                    env,
                                                                                                                    QLatin1String(qTypeName),
                                                                                                                    elementMetaType,
                                                                                                                    elementType,
                                                                                                                    true
                                                                                                                );
                ExternalToInternalConverter externalToInternalConverter = QtJambiTypeManager::getExternalToInternalConverter(
                                                                                                                    env,
                                                                                                                    elementType,
                                                                                                                    QLatin1String(qTypeName),
                                                                                                                    elementMetaType
                                                                                                                );

                ContainerAccessFactory accessFactory = ContainerAccessFactories::getAccessFactory(ContainerType::QList, align, size
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
                                                                                                     , isStaticType
#else
                                                                                                     , false
#endif
                                                                                                     );
                Q_ASSERT(accessFactory);
                QHashFunction hashFunction = QtJambiTypeManager::findHashFunction(isPointer, elementMetaType.id());
                containerAccess = dynamic_cast<AbstractListAccess*>(accessFactory(elementMetaType,
                                                                             hashFunction,
                                                                             internalToExternalConverter,
                                                                             externalToInternalConverter));
            }
            break;
        }
        containerAccess = checkContainerAccess(env, containerAccess);
        isNativeContainer = other && qtjambi_is_QList(env, other, elementMetaType);
    }
    void* listPtr;
    if(isNativeContainer){
        if(QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForJavaObject(env, other)){
            listPtr = containerAccess->copyContainer(link->pointer());
        }else{
            if(Java::QtJambi::QtObjectInterface::isInstanceOf(env, other)){
                containerAccess->dispose();
                Java::QtJambi::QNoNativeResourcesException::throwNew(env, QString("Incomplete object of type: %1").arg(qtjambi_object_class_name(env, other).replace("$", ".")) QTJAMBI_STACKTRACEINFO );
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
    const QSharedPointer<QtJambiLink>& link = QtJambiLink::createLinkForContainer(env, object, listPtr,
                                                                                   containerMetaType,
                                                                                   true, true, containerAccess);
    if (!link) {
        containerAccess->containerDeleter()(listPtr);
        containerAccess->dispose();
    }else if(!isNativeContainer && other){
        containerAccess->appendList(env, listPtr, other);
    }
}

void initialize_QSet(JNIEnv *env, jobject object, jclass elementType, QtJambiNativeID elementMetaTypeId, jobject other){
    QMetaType& elementMetaType = checkedUnref(qtjambi_object_from_nativeId<QMetaType>(elementMetaTypeId));
    SuperTypeInfos superTypeInfos = getSuperTypeInfos(env, env->GetObjectClass(object));
    if(superTypeInfos.size()>1)
        JavaException::raiseError(env, qPrintable( QString("It is not permitted to create a derived type of %1 implementing any Qt interface.").arg("QSet") ) QTJAMBI_STACKTRACEINFO );
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
            Java::QtJambi::QNoNativeResourcesException::throwNew(env, QString("Incomplete object of type: %1").arg(qtjambi_object_class_name(env, other).replace("$", ".")) QTJAMBI_STACKTRACEINFO );
        }
    }
    if(!containerAccess){
        {
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
                    elementType = getGlobalClassRef(env, elementType);
                    QByteArray qTypeName = elementMetaType.name();
                    size_t size = size_t(elementMetaType.sizeOf());
    #if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
                    bool isPointer = elementMetaType.flags() & QMetaType::IsPointer;
                    size_t align = size_t(elementMetaType.alignOf());
    #else
                    bool isStaticType = false;
                    bool isPointer = qTypeName.endsWith("*");
                    size_t align = 0;
                    {
                        const std::type_info* typeId = getTypeByMetaType(elementMetaType.id());
                        if(!typeId)
                            typeId = getTypeByQtName(qTypeName);
                        if(typeId){
                            if(isFunctional(*typeId)){
                                QString typeName = qtjambi_type_name(*typeId);
                                if(!typeName.startsWith("std::function") && (typeName.contains("(*)"))){
                                    isPointer = true;
                                }
                            }
                            if(const QtJambiTypeInfo* typeInfo = getQTypeInfo(*typeId))
                                isStaticType = typeInfo->isStatic;
                            if(!isPointer)
                                align = getValueAlignment(*typeId);
                        }
                    }
    #endif
                    if(isPointer)
                        size = 0;
                    InternalToExternalConverter internalToExternalConverter = QtJambiTypeManager::getInternalToExternalConverter(
                                                                                                                        env,
                                                                                                                        QLatin1String(qTypeName),
                                                                                                                        elementMetaType,
                                                                                                                        elementType,
                                                                                                                        true
                                                                                                                    );
                    ExternalToInternalConverter externalToInternalConverter = QtJambiTypeManager::getExternalToInternalConverter(
                                                                                                                        env,
                                                                                                                        elementType,
                                                                                                                        QLatin1String(qTypeName),
                                                                                                                        elementMetaType
                                                                                                                    );
                    ContainerAccessFactory accessFactory = ContainerAccessFactories::getAccessFactory(ContainerType::QSet, align, size
    #if QT_VERSION < QT_VERSION_CHECK(6,0,0)
                                                                                                                                    , isStaticType
    #else
                                                                                                                                    , false
    #endif
                                                                                                         );
                    Q_ASSERT(accessFactory);
                    QHashFunction hashFunction = QtJambiTypeManager::findHashFunction(isPointer, elementMetaType.id());
                    containerAccess = dynamic_cast<AbstractSetAccess*>(accessFactory(elementMetaType,
                                                                                 hashFunction,
                                                                                 internalToExternalConverter,
                                                                                 externalToInternalConverter));
                }
                break;
            }
        }
        containerAccess = checkContainerAccess(env, containerAccess);
        isNativeContainer = other && qtjambi_is_QSet(env, other, elementMetaType);
    }
    void* listPtr;
    if(isNativeContainer){
        if(QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForJavaObject(env, other)){
            listPtr = containerAccess->copyContainer(link->pointer());
        }else{
            if(Java::QtJambi::QtObjectInterface::isInstanceOf(env, other)){
                containerAccess->dispose();
                Java::QtJambi::QNoNativeResourcesException::throwNew(env, QString("Incomplete object of type: %1").arg(qtjambi_object_class_name(env, other).replace("$", ".")) QTJAMBI_STACKTRACEINFO );
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
    const QSharedPointer<QtJambiLink>& link = QtJambiLink::createLinkForContainer(env, object, listPtr,
                                                                                   containerMetaType,
                                                                                   true, true, containerAccess);
    if (!link) {
        containerAccess->containerDeleter()(listPtr);
        containerAccess->dispose();
    }else if(!isNativeContainer && other){
        jobject iter = qtjambi_collection_iterator(env, other);
        while(qtjambi_iterator_has_next(env, iter)){
            containerAccess->insert(env, listPtr, qtjambi_iterator_next(env, iter));
        }
    }
}

#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
void initialize_QLinkedList(JNIEnv *env, jobject object, jclass elementType, QtJambiNativeID elementMetaTypeId, jobject other){
    QMetaType& elementMetaType = checkedUnref(qtjambi_object_from_nativeId<QMetaType>(elementMetaTypeId));
    SuperTypeInfos superTypeInfos = getSuperTypeInfos(env, env->GetObjectClass(object));
    if(superTypeInfos.size()>1)
        JavaException::raiseError(env, qPrintable( QString("It is not permitted to create a derived type of %1 implementing any Qt interface.").arg("QLinkedList") ) QTJAMBI_STACKTRACEINFO );
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
            Java::QtJambi::QNoNativeResourcesException::throwNew(env, QString("Incomplete object of type: %1").arg(qtjambi_object_class_name(env, other).replace("$", ".")) QTJAMBI_STACKTRACEINFO );
        }
    }
    if(!containerAccess){
        {
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
                    elementType = getGlobalClassRef(env, elementType);
                    size_t size = size_t(elementMetaType.sizeOf());
                    bool isStaticType = false;
                    bool isPointer = elementMetaType.name().endsWith("*");
                    size_t align = 0;
                    {
                        const std::type_info* typeId = getTypeByMetaType(elementMetaType.id());
                        if(!typeId)
                            typeId = getTypeByQtName(elementMetaType.name());
                        if(typeId){
                            if(isFunctional(*typeId)){
                                QString typeName = qtjambi_type_name(*typeId);
                                if(!typeName.startsWith("std::function") && (typeName.contains("(*)"))){
                                    isPointer = true;
                                }
                            }
                            if(const QtJambiTypeInfo* typeInfo = getQTypeInfo(*typeId))
                                isStaticType = typeInfo->isStatic;
                            if(!isPointer)
                                align = getValueAlignment(*typeId);
                        }
                    }
                    if(isPointer)
                        size = 0;
                    InternalToExternalConverter internalToExternalConverter = QtJambiTypeManager::getInternalToExternalConverter(
                                                                                                                        env,
                                                                                                                        QLatin1String(elementMetaType.name()),
                                                                                                                        elementMetaType,
                                                                                                                        elementType,
                                                                                                                        true
                                                                                                                    );
                    ExternalToInternalConverter externalToInternalConverter = QtJambiTypeManager::getExternalToInternalConverter(
                                                                                                                        env,
                                                                                                                        elementType,
                                                                                                                        QLatin1String(elementMetaType.name()),
                                                                                                                        elementMetaType
                                                                                                                    );
                    ContainerAccessFactory accessFactory = ContainerAccessFactories::getAccessFactory(ContainerType::QLinkedList, align, size, isStaticType);
                    Q_ASSERT(accessFactory);
                    QHashFunction hashFunction = QtJambiTypeManager::findHashFunction(isPointer, elementMetaType.id());
                    containerAccess = dynamic_cast<AbstractLinkedListAccess*>(accessFactory(elementMetaType,
                                                                                 hashFunction,
                                                                                 internalToExternalConverter,
                                                                                 externalToInternalConverter));
                }
                break;
            }
        }
        containerAccess = checkContainerAccess(env, containerAccess);
        isNativeContainer = other && qtjambi_is_QLinkedList(env, other, elementMetaType);
    }
    void* listPtr;
    if(isNativeContainer){
        if(QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForJavaObject(env, other)){
            listPtr = containerAccess->copyContainer(link->pointer());
        }else{
            if(Java::QtJambi::QtObjectInterface::isInstanceOf(env, other)){
                containerAccess->dispose();
                Java::QtJambi::QNoNativeResourcesException::throwNew(env, QString("Incomplete object of type: %1").arg(qtjambi_object_class_name(env, other).replace("$", ".")) QTJAMBI_STACKTRACEINFO );
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
    const QSharedPointer<QtJambiLink>& link = QtJambiLink::createLinkForContainer(env, object, listPtr,
                                                                                   containerMetaType,
                                                                                   true, true, containerAccess);
    if (!link) {
        containerAccess->containerDeleter()(listPtr);
        containerAccess->dispose();
    }else if(!isNativeContainer && other){
        jobject iter = qtjambi_collection_iterator(env, other);
        while(qtjambi_iterator_has_next(env, iter)){
            containerAccess->append(env, listPtr, qtjambi_iterator_next(env, iter));
        }
    }
}

void initialize_QVector(JNIEnv *env, jobject object, jclass elementType, QtJambiNativeID elementMetaTypeId, jobject other){
    QMetaType& elementMetaType = checkedUnref(qtjambi_object_from_nativeId<QMetaType>(elementMetaTypeId));
    SuperTypeInfos superTypeInfos = getSuperTypeInfos(env, env->GetObjectClass(object));
    if(superTypeInfos.size()>1)
        JavaException::raiseError(env, qPrintable( QString("It is not permitted to create a derived type of %1 implementing any Qt interface.").arg("QVector") ) QTJAMBI_STACKTRACEINFO );
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
            Java::QtJambi::QNoNativeResourcesException::throwNew(env, QString("Incomplete object of type: %1").arg(qtjambi_object_class_name(env, other).replace("$", ".")) QTJAMBI_STACKTRACEINFO );
        }
    }
    if(!containerAccess){
        {
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
                    elementType = getGlobalClassRef(env, elementType);
                    size_t size = size_t(elementMetaType.sizeOf());
                    bool isStaticType = false;
                    bool isPointer = elementMetaType.name().endsWith("*");
                    size_t align = 0;
                    {
                        const std::type_info* typeId = getTypeByMetaType(elementMetaType.id());
                        if(!typeId)
                            typeId = getTypeByQtName(elementMetaType.name());
                        if(typeId){
                            if(isFunctional(*typeId)){
                                QString typeName = qtjambi_type_name(*typeId);
                                if(!typeName.startsWith("std::function") && (typeName.contains("(*)"))){
                                    isPointer = true;
                                }
                            }
                            if(const QtJambiTypeInfo* typeInfo = getQTypeInfo(*typeId))
                                isStaticType = typeInfo->isStatic;
                            if(!isPointer)
                                align = getValueAlignment(*typeId);
                        }
                    }
                    if(isPointer)
                        size = 0;
                    InternalToExternalConverter internalToExternalConverter = QtJambiTypeManager::getInternalToExternalConverter(
                                                                                                                        env,
                                                                                                                        QLatin1String(elementMetaType.name()),
                                                                                                                        elementMetaType,
                                                                                                                        elementType,
                                                                                                                        true
                                                                                                                    );
                    ExternalToInternalConverter externalToInternalConverter = QtJambiTypeManager::getExternalToInternalConverter(
                                                                                                                        env,
                                                                                                                        elementType,
                                                                                                                        QLatin1String(elementMetaType.name()),
                                                                                                                        elementMetaType
                                                                                                                    );
                    ContainerAccessFactory accessFactory = ContainerAccessFactories::getAccessFactory(ContainerType::QVector, align, size, isStaticType);
                    Q_ASSERT(accessFactory);
                    QHashFunction hashFunction = QtJambiTypeManager::findHashFunction(isPointer, elementMetaType.id());
                    containerAccess = dynamic_cast<AbstractVectorAccess*>(accessFactory(elementMetaType,
                                                                                 hashFunction,
                                                                                 internalToExternalConverter,
                                                                                 externalToInternalConverter));
                }
                break;
            }
        }
        containerAccess = checkContainerAccess(env, containerAccess);
        isNativeContainer = other && qtjambi_is_QVector(env, other, elementMetaType);
    }
    void* listPtr;
    if(isNativeContainer){
        if(QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForJavaObject(env, other)){
            listPtr = containerAccess->copyContainer(link->pointer());
        }else{
            if(Java::QtJambi::QtObjectInterface::isInstanceOf(env, other)){
                containerAccess->dispose();
                Java::QtJambi::QNoNativeResourcesException::throwNew(env, QString("Incomplete object of type: %1").arg(qtjambi_object_class_name(env, other).replace("$", ".")) QTJAMBI_STACKTRACEINFO );
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
    const QSharedPointer<QtJambiLink>& link = QtJambiLink::createLinkForContainer(env, object, listPtr,
                                                                                   containerMetaType,
                                                                                   true, true, containerAccess);
    if (!link) {
        containerAccess->containerDeleter()(listPtr);
        containerAccess->dispose();
    }else if(!isNativeContainer && other){
        containerAccess->appendVector(env, listPtr, other);
    }
}
#endif

void initialize_QHash(JNIEnv *env, jobject object, jclass keyType, QtJambiNativeID keyMetaTypeId, jclass valueType, QtJambiNativeID valueMetaTypeId, jobject other){
    QMetaType& keyMetaType = checkedUnref(qtjambi_object_from_nativeId<QMetaType>(keyMetaTypeId));
    QMetaType& valueMetaType = checkedUnref(qtjambi_object_from_nativeId<QMetaType>(valueMetaTypeId));
    SuperTypeInfos superTypeInfos = getSuperTypeInfos(env, env->GetObjectClass(object));
    if(superTypeInfos.size()>1)
        JavaException::raiseError(env, qPrintable( QString("It is not permitted to create a derived type of %1 implementing any Qt interface.").arg("QHash") ) QTJAMBI_STACKTRACEINFO );
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
            Java::QtJambi::QNoNativeResourcesException::throwNew(env, QString("Incomplete object of type: %1").arg(qtjambi_object_class_name(env, other).replace("$", ".")) QTJAMBI_STACKTRACEINFO );
        }
    }
    if(!containerAccess){
        size_t size1 = size_t(keyMetaType.sizeOf());
    #if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
        bool isPointer1 = keyMetaType.flags() & QMetaType::IsPointer;
        size_t align1 = size_t(keyMetaType.alignOf());
    #else
        bool isPointer1 = keyMetaType.name().endsWith("*");
        size_t align1 = 0;
        {
            const std::type_info* typeId = getTypeByMetaType(keyMetaType.id());
            if(!typeId)
                typeId = getTypeByQtName(keyMetaType.name());
            if(typeId){
                if(isFunctional(*typeId)){
                    QString typeName = qtjambi_type_name(*typeId);
                    if(!typeName.startsWith("std::function") && (typeName.contains("(*)"))){
                        isPointer1 = true;
                    }
                }
                align1 = getValueAlignment(*typeId);
            }
        }
    #endif

        size_t size2 = size_t(valueMetaType.sizeOf());
    #if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
        bool isPointer2 = valueMetaType.flags() & QMetaType::IsPointer;
        size_t align2 = size_t(valueMetaType.alignOf());
    #else
        bool isPointer2 = valueMetaType.name().endsWith("*");
        size_t align2 = 0;
        {
            const std::type_info* typeId = getTypeByMetaType(valueMetaType.id());
            if(!typeId)
                typeId = getTypeByQtName(valueMetaType.name());
            if(typeId){
                if(isFunctional(*typeId)){
                    QString typeName = qtjambi_type_name(*typeId);
                    if(!typeName.startsWith("std::function") && (typeName.contains("(*)"))){
                        isPointer2 = true;
                    }
                }
                align2 = getValueAlignment(*typeId);
            }
        }
    #endif

        if(!containerAccess){
            InternalToExternalConverter keyInternalToExternalConverter = QtJambiTypeManager::getInternalToExternalConverter(
                                                                                                                env,
                                                                                                                QLatin1String(keyMetaType.name()),
                                                                                                                keyMetaType,
                                                                                                                keyType,
                                                                                                                true
                                                                                                            );
            ExternalToInternalConverter keyExternalToInternalConverter = QtJambiTypeManager::getExternalToInternalConverter(
                                                                                                                env,
                                                                                                                keyType,
                                                                                                                QLatin1String(keyMetaType.name()),
                                                                                                                keyMetaType
                                                                                                            );
            InternalToExternalConverter valueInternalToExternalConverter = QtJambiTypeManager::getInternalToExternalConverter(
                                                                                                                env,
                                                                                                                QLatin1String(valueMetaType.name()),
                                                                                                                valueMetaType,
                                                                                                                valueType,
                                                                                                                true
                                                                                                            );
            ExternalToInternalConverter valueExternalToInternalConverter = QtJambiTypeManager::getExternalToInternalConverter(
                                                                                                                env,
                                                                                                                valueType,
                                                                                                                QLatin1String(valueMetaType.name()),
                                                                                                                valueMetaType
                                                                                                            );
            if(isPointer1)
                size1 = 0;
            if(isPointer2)
                size2 = 0;
            BiContainerAccessFactory accessFactory = ContainerAccessFactories::getAccessFactory(MapType::QHash, align1, size1, align2, size2);
            Q_ASSERT(accessFactory);
            QHashFunction hashFunction1 = QtJambiTypeManager::findHashFunction(isPointer1, keyMetaType.id());
            QHashFunction hashFunction2 = QtJambiTypeManager::findHashFunction(isPointer2, valueMetaType.id());
            containerAccess = dynamic_cast<AbstractHashAccess*>(accessFactory(
                                                                        keyMetaType,
                                                                        hashFunction1,
                                                                        keyInternalToExternalConverter,
                                                                        keyExternalToInternalConverter,
                                                                        valueMetaType,
                                                                        hashFunction2,
                                                                        valueInternalToExternalConverter,
                                                                        valueExternalToInternalConverter));
        }
        containerAccess = checkContainerAccess(env, containerAccess);
        isNativeContainer = other && qtjambi_is_QHash(env, other, keyMetaType, valueMetaType);
    }
    void* listPtr;
    if(isNativeContainer){
        if(QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForJavaObject(env, other)){
            listPtr = containerAccess->copyContainer(link->pointer());
        }else{
            if(Java::QtJambi::QtObjectInterface::isInstanceOf(env, other)){
                containerAccess->dispose();
                Java::QtJambi::QNoNativeResourcesException::throwNew(env, QString("Incomplete object of type: %1").arg(qtjambi_object_class_name(env, other).replace("$", ".")) QTJAMBI_STACKTRACEINFO );
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
    const QSharedPointer<QtJambiLink>& link = QtJambiLink::createLinkForContainer(env, object, listPtr,
                                                                                   containerMetaType,
                                                                                   true, true, containerAccess);
    if (!link) {
        containerAccess->containerDeleter()(listPtr);
        containerAccess->dispose();
    }else if(!isNativeContainer && other){
        jobject iter = qtjambi_map_entryset_iterator(env, other);
        while(qtjambi_iterator_has_next(env, iter)){
            jobject entry = qtjambi_iterator_next(env, iter);
            containerAccess->insert(env, listPtr, qtjambi_map$entry_key(env, entry), qtjambi_map$entry_value(env, entry));
        }
    }
}

void initialize_QMultiHash(JNIEnv *env, jobject object, jclass keyType, QtJambiNativeID keyMetaTypeId, jclass valueType, QtJambiNativeID valueMetaTypeId, jobject other){
    QMetaType& keyMetaType = checkedUnref(qtjambi_object_from_nativeId<QMetaType>(keyMetaTypeId));
    QMetaType& valueMetaType = checkedUnref(qtjambi_object_from_nativeId<QMetaType>(valueMetaTypeId));
    SuperTypeInfos superTypeInfos = getSuperTypeInfos(env, env->GetObjectClass(object));
    if(superTypeInfos.size()>1)
        JavaException::raiseError(env, qPrintable( QString("It is not permitted to create a derived type of %1 implementing any Qt interface.").arg("QMultiHash") ) QTJAMBI_STACKTRACEINFO );
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
            Java::QtJambi::QNoNativeResourcesException::throwNew(env, QString("Incomplete object of type: %1").arg(qtjambi_object_class_name(env, other).replace("$", ".")) QTJAMBI_STACKTRACEINFO );
        }
    }
    if(!containerAccess){
        size_t size1 = size_t(keyMetaType.sizeOf());
    #if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
        bool isPointer1 = keyMetaType.flags() & QMetaType::IsPointer;
        size_t align1 = size_t(keyMetaType.alignOf());
    #else
        bool isPointer1 = keyMetaType.name().endsWith("*");
        size_t align1 = 0;
        {
            const std::type_info* typeId = getTypeByMetaType(keyMetaType.id());
            if(!typeId)
                typeId = getTypeByQtName(keyMetaType.name());
            if(typeId){
                if(isFunctional(*typeId)){
                    QString typeName = qtjambi_type_name(*typeId);
                    if(!typeName.startsWith("std::function") && (typeName.contains("(*)"))){
                        isPointer1 = true;
                    }
                }
                align1 = getValueAlignment(*typeId);
            }
        }
    #endif

        size_t size2 = size_t(valueMetaType.sizeOf());
    #if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
        bool isPointer2 = valueMetaType.flags() & QMetaType::IsPointer;
        size_t align2 = size_t(valueMetaType.alignOf());
    #else
        bool isPointer2 = valueMetaType.name().endsWith("*");
        size_t align2 = 0;
        {
            const std::type_info* typeId = getTypeByMetaType(valueMetaType.id());
            if(!typeId)
                typeId = getTypeByQtName(valueMetaType.name());
            if(typeId){
                if(isFunctional(*typeId)){
                    QString typeName = qtjambi_type_name(*typeId);
                    if(!typeName.startsWith("std::function") && (typeName.contains("(*)"))){
                        isPointer2 = true;
                    }
                }
                align2 = getValueAlignment(*typeId);
            }
        }
    #endif

        if(!containerAccess){
            InternalToExternalConverter keyInternalToExternalConverter = QtJambiTypeManager::getInternalToExternalConverter(
                                                                                                                env,
                                                                                                                QLatin1String(keyMetaType.name()),
                                                                                                                keyMetaType,
                                                                                                                keyType,
                                                                                                                true
                                                                                                            );
            ExternalToInternalConverter keyExternalToInternalConverter = QtJambiTypeManager::getExternalToInternalConverter(
                                                                                                                env,
                                                                                                                keyType,
                                                                                                                QLatin1String(keyMetaType.name()),
                                                                                                                keyMetaType
                                                                                                            );
            InternalToExternalConverter valueInternalToExternalConverter = QtJambiTypeManager::getInternalToExternalConverter(
                                                                                                                env,
                                                                                                                QLatin1String(valueMetaType.name()),
                                                                                                                valueMetaType,
                                                                                                                valueType,
                                                                                                                true
                                                                                                            );
            ExternalToInternalConverter valueExternalToInternalConverter = QtJambiTypeManager::getExternalToInternalConverter(
                                                                                                                env,
                                                                                                                valueType,
                                                                                                                QLatin1String(valueMetaType.name()),
                                                                                                                valueMetaType
                                                                                                            );

            if(isPointer1)
                size1 = 0;
            if(isPointer2)
                size2 = 0;
            BiContainerAccessFactory accessFactory = ContainerAccessFactories::getAccessFactory(MapType::QMultiHash, align1, size1, align2, size2);
            QHashFunction hashFunction1 = QtJambiTypeManager::findHashFunction(isPointer1, keyMetaType.id());
            QHashFunction hashFunction2 = QtJambiTypeManager::findHashFunction(isPointer2, valueMetaType.id());
            containerAccess = dynamic_cast<AbstractMultiHashAccess*>(accessFactory(
                                                                        keyMetaType,
                                                                        hashFunction1,
                                                                        keyInternalToExternalConverter,
                                                                        keyExternalToInternalConverter,
                                                                        valueMetaType,
                                                                        hashFunction2,
                                                                        valueInternalToExternalConverter,
                                                                        valueExternalToInternalConverter));
        }
        containerAccess = checkContainerAccess(env, containerAccess);
        isNativeContainer = other && qtjambi_is_QMultiHash(env, other, keyMetaType, valueMetaType);
    }
    void* listPtr;
    if(isNativeContainer){
        if(QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForJavaObject(env, other)){
            listPtr = containerAccess->copyContainer(link->pointer());
        }else{
            if(Java::QtJambi::QtObjectInterface::isInstanceOf(env, other)){
                containerAccess->dispose();
                Java::QtJambi::QNoNativeResourcesException::throwNew(env, QString("Incomplete object of type: %1").arg(qtjambi_object_class_name(env, other).replace("$", ".")) QTJAMBI_STACKTRACEINFO );
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
    const QSharedPointer<QtJambiLink>& link = QtJambiLink::createLinkForContainer(env, object, listPtr,
                                                                                   containerMetaType,
                                                                                   true, true, containerAccess);
    if (!link) {
        containerAccess->containerDeleter()(listPtr);
        containerAccess->dispose();
    }else if(!isNativeContainer && other){
        jobject iter = qtjambi_map_entryset_iterator(env, other);
        while(qtjambi_iterator_has_next(env, iter)){
            jobject entry = qtjambi_iterator_next(env, iter);
            jobject list = qtjambi_map$entry_value(env, entry);
            jobject iter2 = qtjambi_collection_iterator(env, list);
            while(qtjambi_iterator_has_next(env, iter2)){
                containerAccess->insert(env, listPtr, qtjambi_map$entry_key(env, entry), qtjambi_iterator_next(env, iter2));
            }
        }
    }
}

void initialize_QMap(JNIEnv *env, jobject object, jclass keyType, QtJambiNativeID keyMetaTypeId, jclass valueType, QtJambiNativeID valueMetaTypeId, jobject other){
    QMetaType& keyMetaType = checkedUnref(qtjambi_object_from_nativeId<QMetaType>(keyMetaTypeId));
    QMetaType& valueMetaType = checkedUnref(qtjambi_object_from_nativeId<QMetaType>(valueMetaTypeId));
    SuperTypeInfos superTypeInfos = getSuperTypeInfos(env, env->GetObjectClass(object));
    if(superTypeInfos.size()>1)
        JavaException::raiseError(env, qPrintable( QString("It is not permitted to create a derived type of %1 implementing any Qt interface.").arg("QMap") ) QTJAMBI_STACKTRACEINFO );
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
            Java::QtJambi::QNoNativeResourcesException::throwNew(env, QString("Incomplete object of type: %1").arg(qtjambi_object_class_name(env, other).replace("$", ".")) QTJAMBI_STACKTRACEINFO );
        }
    }
    if(!containerAccess){
        size_t size1 = size_t(keyMetaType.sizeOf());
    #if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
        bool isPointer1 = keyMetaType.flags() & QMetaType::IsPointer;
        size_t align1 = size_t(keyMetaType.alignOf());
    #else
        bool isPointer1 = keyMetaType.name().endsWith("*");
        size_t align1 = 0;
        {
            const std::type_info* typeId = getTypeByMetaType(keyMetaType.id());
            if(!typeId)
                typeId = getTypeByQtName(keyMetaType.name());
            if(typeId){
                if(isFunctional(*typeId)){
                    QString typeName = qtjambi_type_name(*typeId);
                    if(!typeName.startsWith("std::function") && (typeName.contains("(*)"))){
                        isPointer1 = true;
                    }
                }
                align1 = getValueAlignment(*typeId);
            }
        }
    #endif

        size_t size2 = size_t(valueMetaType.sizeOf());
    #if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
        bool isPointer2 = valueMetaType.flags() & QMetaType::IsPointer;
        size_t align2 = size_t(valueMetaType.alignOf());
    #else
        bool isPointer2 = valueMetaType.name().endsWith("*");
        size_t align2 = 0;
        {
            const std::type_info* typeId = getTypeByMetaType(valueMetaType.id());
            if(!typeId)
                typeId = getTypeByQtName(valueMetaType.name());
            if(typeId){
                if(isFunctional(*typeId)){
                    QString typeName = qtjambi_type_name(*typeId);
                    if(!typeName.startsWith("std::function") && (typeName.contains("(*)"))){
                        isPointer2 = true;
                    }
                }
                align2 = getValueAlignment(*typeId);
            }
        }
    #endif

        if(!containerAccess){
            InternalToExternalConverter keyInternalToExternalConverter = QtJambiTypeManager::getInternalToExternalConverter(
                                                                                                                env,
                                                                                                                QLatin1String(keyMetaType.name()),
                                                                                                                keyMetaType,
                                                                                                                keyType,
                                                                                                                true
                                                                                                            );
            ExternalToInternalConverter keyExternalToInternalConverter = QtJambiTypeManager::getExternalToInternalConverter(
                                                                                                                env,
                                                                                                                keyType,
                                                                                                                QLatin1String(keyMetaType.name()),
                                                                                                                keyMetaType
                                                                                                            );
            InternalToExternalConverter valueInternalToExternalConverter = QtJambiTypeManager::getInternalToExternalConverter(
                                                                                                                env,
                                                                                                                QLatin1String(valueMetaType.name()),
                                                                                                                valueMetaType,
                                                                                                                valueType,
                                                                                                                true
                                                                                                            );
            ExternalToInternalConverter valueExternalToInternalConverter = QtJambiTypeManager::getExternalToInternalConverter(
                                                                                                                env,
                                                                                                                valueType,
                                                                                                                QLatin1String(valueMetaType.name()),
                                                                                                                valueMetaType
                                                                                                            );

            if(isPointer1)
                size1 = 0;
            if(isPointer2)
                size2 = 0;
            BiContainerAccessFactory accessFactory = ContainerAccessFactories::getAccessFactory(MapType::QMap, align1, size1, align2, size2);
            Q_ASSERT(accessFactory);
            QHashFunction hashFunction1 = QtJambiTypeManager::findHashFunction(isPointer1, keyMetaType.id());
            QHashFunction hashFunction2 = QtJambiTypeManager::findHashFunction(isPointer2, valueMetaType.id());
            containerAccess = dynamic_cast<AbstractMapAccess*>(accessFactory(
                                                                        keyMetaType,
                                                                        hashFunction1,
                                                                        keyInternalToExternalConverter,
                                                                        keyExternalToInternalConverter,
                                                                        valueMetaType,
                                                                        hashFunction2,
                                                                        valueInternalToExternalConverter,
                                                                        valueExternalToInternalConverter));
        }
        containerAccess = checkContainerAccess(env, containerAccess);
        isNativeContainer = other && qtjambi_is_QMap(env, other, keyMetaType, valueMetaType);
    }
    void* listPtr;
    if(isNativeContainer){
        if(QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForJavaObject(env, other)){
            listPtr = containerAccess->copyContainer(link->pointer());
        }else{
            if(Java::QtJambi::QtObjectInterface::isInstanceOf(env, other)){
                containerAccess->dispose();
                Java::QtJambi::QNoNativeResourcesException::throwNew(env, QString("Incomplete object of type: %1").arg(qtjambi_object_class_name(env, other).replace("$", ".")) QTJAMBI_STACKTRACEINFO );
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
    const QSharedPointer<QtJambiLink>& link = QtJambiLink::createLinkForContainer(env, object, listPtr,
                                                                                   containerMetaType,
                                                                                   true, true, containerAccess);
    if (!link) {
        containerAccess->containerDeleter()(listPtr);
        containerAccess->dispose();
    }else if(!isNativeContainer && other){
        jobject iter = qtjambi_map_entryset_iterator(env, other);
        while(qtjambi_iterator_has_next(env, iter)){
            jobject entry = qtjambi_iterator_next(env, iter);
            containerAccess->insert(env, listPtr, qtjambi_map$entry_key(env, entry), qtjambi_map$entry_value(env, entry));
        }
    }
}

void initialize_QMultiMap(JNIEnv *env, jobject object, jclass keyType, QtJambiNativeID keyMetaTypeId, jclass valueType, QtJambiNativeID valueMetaTypeId, jobject other){
    QMetaType& keyMetaType = checkedUnref(qtjambi_object_from_nativeId<QMetaType>(keyMetaTypeId));
    QMetaType& valueMetaType = checkedUnref(qtjambi_object_from_nativeId<QMetaType>(valueMetaTypeId));
    SuperTypeInfos superTypeInfos = getSuperTypeInfos(env, env->GetObjectClass(object));
    if(superTypeInfos.size()>1)
        JavaException::raiseError(env, qPrintable( QString("It is not permitted to create a derived type of %1 implementing any Qt interface.").arg("QMultiMap") ) QTJAMBI_STACKTRACEINFO );
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
            Java::QtJambi::QNoNativeResourcesException::throwNew(env, QString("Incomplete object of type: %1").arg(qtjambi_object_class_name(env, other).replace("$", ".")) QTJAMBI_STACKTRACEINFO );
        }
    }
    if(!containerAccess){
        size_t size1 = size_t(keyMetaType.sizeOf());
    #if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
        bool isPointer1 = keyMetaType.flags() & QMetaType::IsPointer;
        size_t align1 = size_t(keyMetaType.alignOf());
    #else
        bool isPointer1 = keyMetaType.name().endsWith("*");
        size_t align1 = 0;
        {
            const std::type_info* typeId = getTypeByMetaType(keyMetaType.id());
            if(!typeId)
                typeId = getTypeByQtName(keyMetaType.name());
            if(typeId){
                if(isFunctional(*typeId)){
                    QString typeName = qtjambi_type_name(*typeId);
                    if(!typeName.startsWith("std::function") && (typeName.contains("(*)"))){
                        isPointer1 = true;
                    }
                }
                align1 = getValueAlignment(*typeId);
            }
        }
    #endif

        size_t size2 = size_t(valueMetaType.sizeOf());
    #if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
        bool isPointer2 = valueMetaType.flags() & QMetaType::IsPointer;
        size_t align2 = size_t(valueMetaType.alignOf());
    #else
        bool isPointer2 = valueMetaType.name().endsWith("*");
        size_t align2 = 0;
        {
            const std::type_info* typeId = getTypeByMetaType(valueMetaType.id());
            if(!typeId)
                typeId = getTypeByQtName(valueMetaType.name());
            if(typeId){
                if(isFunctional(*typeId)){
                    QString typeName = qtjambi_type_name(*typeId);
                    if(!typeName.startsWith("std::function") && (typeName.contains("(*)"))){
                        isPointer2 = true;
                    }
                }
                align2 = getValueAlignment(*typeId);
            }
        }
    #endif

        if(!containerAccess){
            InternalToExternalConverter keyInternalToExternalConverter = QtJambiTypeManager::getInternalToExternalConverter(
                                                                                                                env,
                                                                                                                QLatin1String(keyMetaType.name()),
                                                                                                                keyMetaType,
                                                                                                                keyType,
                                                                                                                true
                                                                                                            );
            ExternalToInternalConverter keyExternalToInternalConverter = QtJambiTypeManager::getExternalToInternalConverter(
                                                                                                                env,
                                                                                                                keyType,
                                                                                                                QLatin1String(keyMetaType.name()),
                                                                                                                keyMetaType
                                                                                                            );
            InternalToExternalConverter valueInternalToExternalConverter = QtJambiTypeManager::getInternalToExternalConverter(
                                                                                                                env,
                                                                                                                QLatin1String(valueMetaType.name()),
                                                                                                                valueMetaType,
                                                                                                                valueType,
                                                                                                                true
                                                                                                            );
            ExternalToInternalConverter valueExternalToInternalConverter = QtJambiTypeManager::getExternalToInternalConverter(
                                                                                                                env,
                                                                                                                valueType,
                                                                                                                QLatin1String(valueMetaType.name()),
                                                                                                                valueMetaType
                                                                                                            );

            if(isPointer1)
                size1 = 0;
            if(isPointer2)
                size2 = 0;
            BiContainerAccessFactory accessFactory = ContainerAccessFactories::getAccessFactory(MapType::QMultiMap, align1, size1, align2, size2);
            Q_ASSERT(accessFactory);
            QHashFunction hashFunction1 = QtJambiTypeManager::findHashFunction(isPointer1, keyMetaType.id());
            QHashFunction hashFunction2 = QtJambiTypeManager::findHashFunction(isPointer2, valueMetaType.id());
            containerAccess = dynamic_cast<AbstractMultiMapAccess*>(accessFactory(
                                                                        keyMetaType,
                                                                        hashFunction1,
                                                                        keyInternalToExternalConverter,
                                                                        keyExternalToInternalConverter,
                                                                        valueMetaType,
                                                                        hashFunction2,
                                                                        valueInternalToExternalConverter,
                                                                        valueExternalToInternalConverter));
        }
        containerAccess = checkContainerAccess(env, containerAccess);
        isNativeContainer = other && qtjambi_is_QMultiMap(env, other, keyMetaType, valueMetaType);
    }
    void* listPtr;
    if(isNativeContainer){
        if(QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForJavaObject(env, other)){
            listPtr = containerAccess->copyContainer(link->pointer());
        }else{
            if(Java::QtJambi::QtObjectInterface::isInstanceOf(env, other)){
                containerAccess->dispose();
                Java::QtJambi::QNoNativeResourcesException::throwNew(env, QString("Incomplete object of type: %1").arg(qtjambi_object_class_name(env, other).replace("$", ".")) QTJAMBI_STACKTRACEINFO );
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
    const QSharedPointer<QtJambiLink>& link = QtJambiLink::createLinkForContainer(env, object, listPtr,
                                                                                   containerMetaType,
                                                                                   true, true, containerAccess);
    if (!link) {
        containerAccess->containerDeleter()(listPtr);
        containerAccess->dispose();
    }else{
        if(!isNativeContainer && other){
            jobject iter = qtjambi_map_entryset_iterator(env, other);
            while(qtjambi_iterator_has_next(env, iter)){
                jobject entry = qtjambi_iterator_next(env, iter);
                jobject list = qtjambi_map$entry_value(env, entry);
                jobject iter2 = qtjambi_collection_iterator(env, list);
                while(qtjambi_iterator_has_next(env, iter2)){
                    containerAccess->insert(env, listPtr, qtjambi_map$entry_key(env, entry), qtjambi_iterator_next(env, iter2));
                }
            }
        }
    }
}

#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
bool qtjambi_is_static_type(const QMetaType& metaType){
    {
        const std::type_info* typeId = getTypeByQtName(metaType.name());
        if(!typeId)
            typeId = getTypeByMetaType(metaType.id());
        if(typeId){
            if(const QtJambiTypeInfo* typeInfo = getQTypeInfo(*typeId))
                return typeInfo->isStatic;
        }
    }
    return QtJambiTypeManager::isStaticType(QLatin1String(metaType.name()));
}
#endif


typedef QMap<hash_type, ContainerAccessFactory> ContainerAccessFactoryHash;
typedef QMap<hash_type, BiContainerAccessFactory> BiContainerAccessFactoryHash;
Q_GLOBAL_STATIC(ContainerAccessFactoryHash, gContainerAccessFactoryHash)
Q_GLOBAL_STATIC(BiContainerAccessFactoryHash, gBiContainerAccessFactoryHash)

inline hash_type qHash(ContainerType containerType, size_t align, size_t size
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
                       , bool isStatic
#endif
) {
    if(size==0)
        align=0;
    switch(containerType){
    case ContainerType::QStack:
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
        containerType = ContainerType::QVector;
        break;
#endif
    case ContainerType::QQueue:
        containerType = ContainerType::QList;
        break;
    default: break;
    }
    QCryptographicHash hashGenerator(QCryptographicHash::Md5);
    hashGenerator.addData(reinterpret_cast<char*>(&containerType), sizeof(containerType));
    hashGenerator.addData(reinterpret_cast<char*>(&align), sizeof(align));
    hashGenerator.addData(reinterpret_cast<char*>(&size), sizeof(size));
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
    switch(containerType){
    case ContainerType::QList:
        if(size <= sizeof(void*)){
            hashGenerator.addData(reinterpret_cast<char*>(&isStatic), sizeof(isStatic));
        }
        break;
    default: break;
    }
#endif
    QByteArray result = hashGenerator.result();
    hash_type h;
    if(size_t(result.size())==sizeof(hash_type)){
        h = *reinterpret_cast<hash_type*>(result.data());
    }else{
        h = qHash(result);
    }
    return h;
}

inline hash_type qHash(MapType containerType, size_t align1, size_t size1, size_t align2, size_t size2)
{
    if(size1==0)
        align1=0;
    if(size2==0)
        align2=0;
    QCryptographicHash hashGenerator(QCryptographicHash::Md5);
    hashGenerator.addData(reinterpret_cast<char*>(&containerType), sizeof(containerType));
    hashGenerator.addData(reinterpret_cast<char*>(&align1), sizeof(align1));
    hashGenerator.addData(reinterpret_cast<char*>(&size1), sizeof(size1));
    hashGenerator.addData(reinterpret_cast<char*>(&align2), sizeof(align2));
    hashGenerator.addData(reinterpret_cast<char*>(&size2), sizeof(size2));
    QByteArray result = hashGenerator.result();
    hash_type h;
    if(size_t(result.size())==sizeof(hash_type)){
        h = *reinterpret_cast<hash_type*>(result.data());
    }else{
        h = qHash(result);
    }
    return h;
}

ContainerAccessFactory ContainerAccessFactories::getAccessFactory(ContainerType containerType, size_t align, size_t size, bool
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
                                                                  isStatic
#endif
                                                                            ){
    return gContainerAccessFactoryHash->value(qHash(containerType, align, size
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
                                                    , isStatic
#endif
                                            ), nullptr);
}

BiContainerAccessFactory ContainerAccessFactories::getAccessFactory(MapType containerType, size_t align1, size_t size1, size_t align2, size_t size2){
    return gBiContainerAccessFactoryHash->value(qHash(containerType, align1, size1, align2, size2), nullptr);
}

void ContainerAccessFactories::registerAccessFactory(ContainerType containerType, size_t align, size_t size, bool
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
                                                     isStatic
#endif
                                                     , ContainerAccessFactory factory){
    /*switch(containerType){
    case ContainerType::QList:
        printf("%s\n", qPrintable(QString("registerAccessFactory(QList,%1,%2,%3)").arg(align).arg(size).arg(isStatic)));
        break;
    case ContainerType::QQueue:
        printf("%s\n", qPrintable(QString("registerAccessFactory(QQueue,%1,%2,%3)").arg(align).arg(size).arg(isStatic)));
        break;
    case ContainerType::QLinkedList:
        printf("%s\n", qPrintable(QString("registerAccessFactory(QLinkedList,%1,%2,%3)").arg(align).arg(size).arg(isStatic)));
        break;
    case ContainerType::QSet:
        printf("%s\n", qPrintable(QString("registerAccessFactory(QSet,%1,%2,%3)").arg(align).arg(size).arg(isStatic)));
        break;
    case ContainerType::QStack:
        printf("%s\n", qPrintable(QString("registerAccessFactory(QStack,%1,%2,%3)").arg(align).arg(size).arg(isStatic)));
        break;
    case ContainerType::QVector:
        printf("%s\n", qPrintable(QString("registerAccessFactory(QVector,%1,%2,%3)").arg(align).arg(size).arg(isStatic)));
        break;
    }*/
    gContainerAccessFactoryHash->insert(qHash(containerType, align, size
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
                                              , isStatic
#endif
                                    ), factory);
}

void ContainerAccessFactories::registerAccessFactory(MapType containerType, size_t align1, size_t size1, size_t align2, size_t size2, BiContainerAccessFactory factory){
    gBiContainerAccessFactoryHash->insert(qHash(containerType, align1, size1, align2, size2), factory);
}

QPair<void*,AbstractContainerAccess*> qtjambi_container_from_nativeId(QtJambiNativeID nativeId){
    if(!!nativeId){
        QtJambiLink *lnk = reinterpret_cast<QtJambiLink *>(nativeId);
        return {lnk->pointer(), lnk->containerAccess()};
    }else{
        return {nullptr,nullptr};
    }
}
