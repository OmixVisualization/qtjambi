/****************************************************************************
**
** Copyright (C) 2009-2026 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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
#include "containeraccess_export_list.h"
#include "containeraccess_export_stringlist.h"
#include "containeraccess_export_bytearraylist.h"
#include "containeraccess_export_map.h"
#include "containeraccess_export_hash.h"
#include "containeraccess_export_pair.h"

struct QtJambiPrivate::ContainerRefPrivate {
    QSharedPointer<QtJambiLink> m_link;
};

QSharedPointer<QtJambiPrivate::ContainerRefPrivate> QtJambiPrivate::getContainerReference(QtJambiNativeID containerId){
    if(QSharedPointer<QtJambiLink> link = QtJambiLink::fromNativeId(containerId)){
        return QSharedPointer<ContainerRefPrivate>{new ContainerRefPrivate{std::move(link)}};
    }else return {};
}

QtJambiNativeID QtJambiPrivate::nativeId(const QSharedPointer<ContainerRefPrivate>& container){
    if(container && container->m_link){
        return QtJambiNativeID(quintptr(container->m_link.get()));
    }
    return QtJambiNativeID::Invalid;
}

void* QtJambiPrivate::getContainer(const QSharedPointer<ContainerRefPrivate>& container){
    if(container){
        return container->m_link->pointer();
    }
    return nullptr;
}

QSharedPointer<QtJambiLink> getLink(const QSharedPointer<QtJambiPrivate::ContainerRefPrivate>& container){
    if(container){
        return container->m_link;
    }
    return nullptr;
}

bool QtJambiPrivate::compareEquals(const QSharedPointer<ContainerRefPrivate>& a,const QSharedPointer<ContainerRefPrivate>& b){
    if(a==b)
        return true;
    if(a && b){
        return a->m_link==b->m_link;
    }
    return false;
}

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
                                                                                     QtJambiLink::fromNativeId(owner),
                                                                                     &QtJambiAPI::deletePointer<QtJambiSpan>, containerAccess);
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

jobject QtJambiPrivate::convertIteratorToJavaObject(JNIEnv *env,
                                                const QSharedPointer<QtJambiLink>& owner,
                                                void* iteratorPtr,
                                                PtrDeleterFunction destructor_function,
                                                AbstractSequentialConstIteratorAccess* containerAccess)
{
    Q_ASSERT(containerAccess);
    jobject returned = nullptr;
    bool isMutable = containerAccess->isMutableIterable();
    returned = isMutable ? Java::QtCore::QSequentialIterator::newInstance(env, nullptr)
                         : Java::QtCore::QSequentialConstIterator::newInstance(env, nullptr);
    QSharedPointer<QtJambiLink> link = QtJambiLink::createLinkForNativeObject(env, returned, iteratorPtr,
                                                                              LINK_NAME_ARG(isMutable ? "QSequentialIterator" : "QSequentialConstIterator")
                                                                              owner, destructor_function, containerAccess);
    if(Q_UNLIKELY(!link)) {
        returned = nullptr;
        containerAccess->dispose();
    }
    return returned;
}

jobject QtJambiPrivate::convertMultiHashIteratorToJavaObject(JNIEnv *env,
                                                            const QSharedPointer<QtJambiLink>& owner,
                                                            void* iteratorPtr,
                                                            PtrDeleterFunction destructor_function,
                                                            AbstractAssociativeConstIteratorAccess* containerAccess)
{
    Q_ASSERT(containerAccess);
    jobject returned = nullptr;
    bool isMutable = containerAccess->isMutableIterable();
    returned = isMutable ? Java::QtCore::QMultiHash$Iterator::newInstance(env, nullptr)
                         : Java::QtCore::QMultiHash$ConstIterator::newInstance(env, nullptr);
    QSharedPointer<QtJambiLink> link = QtJambiLink::createLinkForNativeObject(env, returned, iteratorPtr,
                                                                              LINK_NAME_ARG(isMutable ? "QMultiHash::iterator" : "QMultiHash::const_iterator")
                                                                              owner, destructor_function, containerAccess);
    if(Q_UNLIKELY(!link)) {
        returned = nullptr;
        containerAccess->dispose();
    }
    return returned;
}

jobject QtJambiPrivate::convertHashIteratorToJavaObject(JNIEnv *env,
                                                       const QSharedPointer<QtJambiLink>& owner,
                                                       void* iteratorPtr,
                                                       PtrDeleterFunction destructor_function,
                                                       AbstractAssociativeConstIteratorAccess* containerAccess)
{
    Q_ASSERT(containerAccess);
    jobject returned = nullptr;
    bool isMutable = containerAccess->isMutableIterable();
    returned = isMutable ? Java::QtCore::QHash$Iterator::newInstance(env, nullptr)
                         : Java::QtCore::QHash$ConstIterator::newInstance(env, nullptr);
    QSharedPointer<QtJambiLink> link = QtJambiLink::createLinkForNativeObject(env, returned, iteratorPtr,
                                                                              LINK_NAME_ARG(isMutable ? "QHash::iterator" : "QHash::const_iterator")
                                                                              owner, destructor_function, containerAccess);
    if(Q_UNLIKELY(!link)) {
        returned = nullptr;
        containerAccess->dispose();
    }
    return returned;
}

jobject QtJambiPrivate::convertMultiMapIteratorToJavaObject(JNIEnv *env,
                                                       const QSharedPointer<QtJambiLink>& owner,
                                                       void* iteratorPtr,
                                                       PtrDeleterFunction destructor_function,
                                                       AbstractAssociativeConstIteratorAccess* containerAccess)
{
    Q_ASSERT(containerAccess);
    jobject returned = nullptr;
    bool isMutable = containerAccess->isMutableIterable();
    returned = isMutable ? Java::QtCore::QMultiMap$Iterator::newInstance(env, nullptr)
                         : Java::QtCore::QMultiMap$ConstIterator::newInstance(env, nullptr);
    QSharedPointer<QtJambiLink> link = QtJambiLink::createLinkForNativeObject(env, returned, iteratorPtr,
                                                                              LINK_NAME_ARG(isMutable ? "QMultiMap::iterator" : "QMultiMap::const_iterator")
                                                                              owner, destructor_function, containerAccess);
    if(Q_UNLIKELY(!link)) {
        returned = nullptr;
        containerAccess->dispose();
    }
    return returned;
}

jobject QtJambiPrivate::convertMapIteratorToJavaObject(JNIEnv *env,
                                                    const QSharedPointer<QtJambiLink>& owner,
                                                    void* iteratorPtr,
                                                    PtrDeleterFunction destructor_function,
                                                    AbstractAssociativeConstIteratorAccess* containerAccess)
{
    Q_ASSERT(containerAccess);
    jobject returned = nullptr;
    bool isMutable = containerAccess->isMutableIterable();
    returned = isMutable ? Java::QtCore::QMap$Iterator::newInstance(env, nullptr)
                         : Java::QtCore::QMap$ConstIterator::newInstance(env, nullptr);
    QSharedPointer<QtJambiLink> link = QtJambiLink::createLinkForNativeObject(env, returned, iteratorPtr,
                                                                              LINK_NAME_ARG(isMutable ? "QMap::iterator" : "QMap::const_iterator")
                                                                              owner, destructor_function, containerAccess);
    if(Q_UNLIKELY(!link)) {
        returned = nullptr;
        containerAccess->dispose();
    }
    return returned;
}

jobject QtJambiPrivate::convertMapKeyValueIteratorToJavaObject(JNIEnv *env,
                                                    const QSharedPointer<QtJambiLink>& owner,
                                                    void* iteratorPtr,
                                                    PtrDeleterFunction destructor_function,
                                                    AbstractSequentialConstIteratorAccess* containerAccess)
{
    Q_ASSERT(containerAccess);
    jobject returned = nullptr;
    bool isMutable = containerAccess->isMutableIterable();
    returned = isMutable ? Java::QtCore::QMap$KeyValueIterator::newInstance(env, nullptr)
                         : Java::QtCore::QMap$ConstKeyValueIterator::newInstance(env, nullptr);
    QSharedPointer<QtJambiLink> link = QtJambiLink::createLinkForNativeObject(env, returned, iteratorPtr,
                                                                              LINK_NAME_ARG(isMutable ? "QMap::key_value_iterator" : "QMap::const_key_value_iterator")
                                                                              owner, destructor_function, containerAccess);
    if(Q_UNLIKELY(!link)) {
        returned = nullptr;
        containerAccess->dispose();
    }
    return returned;
}

jobject QtJambiPrivate::convertHashKeyValueIteratorToJavaObject(JNIEnv *env,
                                                    const QSharedPointer<QtJambiLink>& owner,
                                                    void* iteratorPtr,
                                                    PtrDeleterFunction destructor_function,
                                                    AbstractSequentialConstIteratorAccess* containerAccess)
{
    Q_ASSERT(containerAccess);
    jobject returned = nullptr;
    bool isMutable = containerAccess->isMutableIterable();
    returned = isMutable ? Java::QtCore::QHash$KeyValueIterator::newInstance(env, nullptr)
                         : Java::QtCore::QHash$ConstKeyValueIterator::newInstance(env, nullptr);
    QSharedPointer<QtJambiLink> link = QtJambiLink::createLinkForNativeObject(env, returned, iteratorPtr,
                                                                              LINK_NAME_ARG(isMutable ? "QHash::key_value_iterator" : "QHash::const_key_value_iterator")
                                                                              owner, destructor_function, containerAccess);
    if(Q_UNLIKELY(!link)) {
        returned = nullptr;
        containerAccess->dispose();
    }
    return returned;
}

jobject QtJambiPrivate::convertMultiMapKeyValueIteratorToJavaObject(JNIEnv *env,
                                                    const QSharedPointer<QtJambiLink>& owner,
                                                    void* iteratorPtr,
                                                    PtrDeleterFunction destructor_function,
                                                    AbstractSequentialConstIteratorAccess* containerAccess)
{
    Q_ASSERT(containerAccess);
    jobject returned = nullptr;
    bool isMutable = containerAccess->isMutableIterable();
    returned = isMutable ? Java::QtCore::QMultiMap$KeyValueIterator::newInstance(env, nullptr)
                         : Java::QtCore::QMultiMap$ConstKeyValueIterator::newInstance(env, nullptr);
    QSharedPointer<QtJambiLink> link = QtJambiLink::createLinkForNativeObject(env, returned, iteratorPtr,
                                                                              LINK_NAME_ARG(isMutable ? "QMultiMap::key_value_iterator" : "QMultiMap::const_key_value_iterator")
                                                                              owner, destructor_function, containerAccess);
    if(Q_UNLIKELY(!link)) {
        returned = nullptr;
        containerAccess->dispose();
    }
    return returned;
}

jobject QtJambiPrivate::convertMultiHashKeyValueIteratorToJavaObject(JNIEnv *env,
                                                    const QSharedPointer<QtJambiLink>& owner,
                                                    void* iteratorPtr,
                                                    PtrDeleterFunction destructor_function,
                                                    AbstractSequentialConstIteratorAccess* containerAccess)
{
    Q_ASSERT(containerAccess);
    jobject returned = nullptr;
    bool isMutable = containerAccess->isMutableIterable();
    returned = isMutable ? Java::QtCore::QMultiHash$KeyValueIterator::newInstance(env, nullptr)
                         : Java::QtCore::QMultiHash$ConstKeyValueIterator::newInstance(env, nullptr);
    QSharedPointer<QtJambiLink> link = QtJambiLink::createLinkForNativeObject(env, returned, iteratorPtr,
                                                                              LINK_NAME_ARG(isMutable ? "QMultiHash::key_value_iterator" : "QMultiHash::const_key_value_iterator")
                                                                              owner, destructor_function, containerAccess);
    if(Q_UNLIKELY(!link)) {
        returned = nullptr;
        containerAccess->dispose();
    }
    return returned;
}

jobject QtJambiPrivate::convertMapKeyIteratorToJavaObject(JNIEnv *env,
                                                    const QSharedPointer<QtJambiLink>& owner,
                                                    void* iteratorPtr,
                                                    PtrDeleterFunction destructor_function,
                                                    AbstractSequentialConstIteratorAccess* containerAccess)
{
    Q_ASSERT(containerAccess);
    jobject returned = nullptr;
    returned = Java::QtCore::QMap$KeyIterator::newInstance(env, nullptr);
    QSharedPointer<QtJambiLink> link = QtJambiLink::createLinkForNativeObject(env, returned, iteratorPtr,
                                                                              LINK_NAME_ARG("QMap::key_iterator")
                                                                              owner, destructor_function, containerAccess);
    if(Q_UNLIKELY(!link)) {
        returned = nullptr;
        containerAccess->dispose();
    }
    return returned;
}

jobject QtJambiPrivate::convertHashKeyIteratorToJavaObject(JNIEnv *env,
                                                    const QSharedPointer<QtJambiLink>& owner,
                                                    void* iteratorPtr,
                                                    PtrDeleterFunction destructor_function,
                                                    AbstractSequentialConstIteratorAccess* containerAccess)
{
    Q_ASSERT(containerAccess);
    jobject returned = nullptr;
    returned = Java::QtCore::QHash$KeyIterator::newInstance(env, nullptr);
    QSharedPointer<QtJambiLink> link = QtJambiLink::createLinkForNativeObject(env, returned, iteratorPtr,
                                                                              LINK_NAME_ARG("QHash::key_iterator")
                                                                              owner, destructor_function, containerAccess);
    if(Q_UNLIKELY(!link)) {
        returned = nullptr;
        containerAccess->dispose();
    }
    return returned;
}

jobject QtJambiPrivate::convertMultiMapKeyIteratorToJavaObject(JNIEnv *env,
                                                    const QSharedPointer<QtJambiLink>& owner,
                                                    void* iteratorPtr,
                                                    PtrDeleterFunction destructor_function,
                                                    AbstractSequentialConstIteratorAccess* containerAccess)
{
    Q_ASSERT(containerAccess);
    jobject returned = nullptr;
    returned = Java::QtCore::QMultiMap$KeyIterator::newInstance(env, nullptr);
    QSharedPointer<QtJambiLink> link = QtJambiLink::createLinkForNativeObject(env, returned, iteratorPtr,
                                                                              LINK_NAME_ARG("QMultiMap::key_iterator")
                                                                              owner, destructor_function, containerAccess);
    if(Q_UNLIKELY(!link)) {
        returned = nullptr;
        containerAccess->dispose();
    }
    return returned;
}

jobject QtJambiPrivate::convertMultiHashKeyIteratorToJavaObject(JNIEnv *env,
                                                    const QSharedPointer<QtJambiLink>& owner,
                                                    void* iteratorPtr,
                                                    PtrDeleterFunction destructor_function,
                                                    AbstractSequentialConstIteratorAccess* containerAccess)
{
    Q_ASSERT(containerAccess);
    jobject returned = nullptr;
    returned = Java::QtCore::QMultiHash$KeyIterator::newInstance(env, nullptr);
    QSharedPointer<QtJambiLink> link = QtJambiLink::createLinkForNativeObject(env, returned, iteratorPtr,
                                                                              LINK_NAME_ARG("QMultiHash::key_iterator")
                                                                              owner, destructor_function, containerAccess);
    if(Q_UNLIKELY(!link)) {
        returned = nullptr;
        containerAccess->dispose();
    }
    return returned;
}

jobject QtJambiPrivate::convertListIteratorToJavaObject(JNIEnv *env,
                                                       const QSharedPointer<QtJambiLink>& owner,
                                                       void* iteratorPtr,
                                                       PtrDeleterFunction destructor_function,
                                                       AbstractSequentialConstIteratorAccess* containerAccess)
{
    Q_ASSERT(containerAccess);
    jobject returned = nullptr;
    bool isMutable = containerAccess->isMutableIterable();
    returned = isMutable ? Java::QtCore::QList$Iterator::newInstance(env, nullptr)
                         : Java::QtCore::QList$ConstIterator::newInstance(env, nullptr);
    QSharedPointer<QtJambiLink> link = QtJambiLink::createLinkForNativeObject(env, returned, iteratorPtr,
                                                                              LINK_NAME_ARG(isMutable ? "QList::iterator" : "QList::const_iterator")
                                                                              owner, destructor_function, containerAccess);
    if(Q_UNLIKELY(!link)) {
        returned = nullptr;
        containerAccess->dispose();
    }
    return returned;
}

jobject QtJambiPrivate::convertListReverseIteratorToJavaObject(JNIEnv *env,
                                                       const QSharedPointer<QtJambiLink>& owner,
                                                       void* iteratorPtr,
                                                       PtrDeleterFunction destructor_function,
                                                       AbstractSequentialConstIteratorAccess* containerAccess)
{
    Q_ASSERT(containerAccess);
    jobject returned = nullptr;
    bool isMutable = containerAccess->isMutableIterable();
    returned = isMutable ? Java::QtCore::QList$ReverseIterator::newInstance(env, nullptr)
                         : Java::QtCore::QList$ConstReverseIterator::newInstance(env, nullptr);
    QSharedPointer<QtJambiLink> link = QtJambiLink::createLinkForNativeObject(env, returned, iteratorPtr,
                                                                              LINK_NAME_ARG(isMutable ? "QList::reverse_iterator" : "QList::const_reverse_iterator")
                                                                              owner, destructor_function, containerAccess);
    if(Q_UNLIKELY(!link)) {
        returned = nullptr;
        containerAccess->dispose();
    }
    return returned;
}

jobject QtJambiPrivate::convertSetIteratorToJavaObject(JNIEnv *env,
                                                        const QSharedPointer<QtJambiLink>& owner,
                                                        void* iteratorPtr,
                                                        PtrDeleterFunction destructor_function,
                                                        AbstractSequentialConstIteratorAccess* containerAccess)
{
    Q_ASSERT(containerAccess);
    jobject returned = nullptr;
    bool isMutable = containerAccess->isMutableIterable();
    returned = isMutable ? Java::QtCore::QSet$Iterator::newInstance(env, nullptr)
                         : Java::QtCore::QSet$ConstIterator::newInstance(env, nullptr);
    QSharedPointer<QtJambiLink> link = QtJambiLink::createLinkForNativeObject(env, returned, iteratorPtr,
                                                                              LINK_NAME_ARG(isMutable ? "QSet::iterator" : "QSet::const_iterator")
                                                                              owner, destructor_function, containerAccess);
    if(Q_UNLIKELY(!link)) {
        returned = nullptr;
        containerAccess->dispose();
    }
    return returned;
}

jobject QtJambiPrivate::convertSpanIteratorToJavaObject(JNIEnv *env,
                                                        const QSharedPointer<QtJambiLink>& owner,
                                                        void* iteratorPtr,
                                                        PtrDeleterFunction destructor_function,
                                                        AbstractSequentialConstIteratorAccess* containerAccess)
{
    Q_ASSERT(containerAccess);
    jobject returned = nullptr;
    bool isMutable = containerAccess->isMutableIterable();
    returned = isMutable ? Java::QtCore::QSpan$Iterator::newInstance(env, nullptr)
                         : Java::QtCore::QConstSpan$ConstIterator::newInstance(env, nullptr);
    QSharedPointer<QtJambiLink> link = QtJambiLink::createLinkForNativeObject(env, returned, iteratorPtr,
                                                                              LINK_NAME_ARG(isMutable ? "QSpan::iterator" : "QSpan::const_iterator")
                                                                              owner, destructor_function, containerAccess);
    if(Q_UNLIKELY(!link)) {
        returned = nullptr;
        containerAccess->dispose();
    }
    return returned;
}

jobject QtJambiPrivate::convertSpanReverseIteratorToJavaObject(JNIEnv *env,
                                                        const QSharedPointer<QtJambiLink>& owner,
                                                        void* iteratorPtr,
                                                        PtrDeleterFunction destructor_function,
                                                        AbstractSequentialConstIteratorAccess* containerAccess)
{
    Q_ASSERT(containerAccess);
    jobject returned = nullptr;
    bool isMutable = containerAccess->isMutableIterable();
    returned = isMutable ? Java::QtCore::QSpan$ReverseIterator::newInstance(env, nullptr)
                         : Java::QtCore::QConstSpan$ConstReverseIterator::newInstance(env, nullptr);
    QSharedPointer<QtJambiLink> link = QtJambiLink::createLinkForNativeObject(env, returned, iteratorPtr,
                                                                              LINK_NAME_ARG(isMutable ? "QSpan::reverse_iterator" : "QSpan::const_reverse_iterator")
                                                                              owner, destructor_function, containerAccess);
    if(Q_UNLIKELY(!link)) {
        returned = nullptr;
        containerAccess->dispose();
    }
    return returned;
}

jobject QtJambiPrivate::convertIteratorToJavaObject(JNIEnv *env,
                                                const QSharedPointer<QtJambiLink>& owner,
                                                void* iteratorPtr,
                                                PtrDeleterFunction destructor_function,
                                                AbstractAssociativeConstIteratorAccess* containerAccess)
{
    Q_ASSERT(containerAccess);
    jobject returned = nullptr;
    bool isMutable = containerAccess->isMutableIterable();
    returned = isMutable ? Java::QtCore::QAssociativeIterator::newInstance(env, nullptr)
                         : Java::QtCore::QAssociativeConstIterator::newInstance(env, nullptr);
    QSharedPointer<QtJambiLink> link = QtJambiLink::createLinkForNativeObject(env, returned, iteratorPtr,
                                                                              LINK_NAME_ARG(isMutable ? "QAssociativeIterator" : "QAssociativeConstIterator")
                                                                              owner, destructor_function, containerAccess);
    if(Q_UNLIKELY(!link)) {
        returned = nullptr;
        containerAccess->dispose();
    }
    return returned;
}

jobject QtJambiAPI::convertIteratorToJavaObject(JNIEnv *env,
                                                QtJambiNativeID owner,
                                                void* iteratorPtr,
                                                PtrDeleterFunction destructor_function,
                                                AbstractSequentialConstIteratorAccess* containerAccess)
{
    Q_ASSERT(containerAccess);
    jobject returned = nullptr;
    jobject obj = CoreAPI::javaObject(owner, env);
    bool isMutable = containerAccess->isMutableIterable();
    InPlaceInitializer initializer(nullptr, nullptr, 0, 0, {}, {JObjectWrapper(env, obj)});
    jobject ipc = Java::QtJambi::QtConstructInPlace::newInstance(env, jlong(&initializer));
    returned = isMutable ? Java::QtCore::QSequentialIterator::newInstance2(env, ipc)
                         : Java::QtCore::QSequentialConstIterator::newInstance2(env, ipc);
    JavaException::check(env QTJAMBI_STACKTRACEINFO);
    Java::QtJambi::QtConstructInPlace::set_native_id(env, ipc, 0);
    initializer.reset(env);
    QSharedPointer<QtJambiLink> link = QtJambiLink::createLinkForNativeObject(env, returned, iteratorPtr,
                                                                              LINK_NAME_ARG(isMutable ? "QSequentialIterator" : "QSequentialConstIterator")
                                                                              QtJambiLink::fromNativeId(owner), destructor_function, containerAccess);
    if(Q_UNLIKELY(!link)) {
        returned = nullptr;
        containerAccess->dispose();
    }
    return returned;
}

jobject QtJambiAPI::convertIteratorToJavaObject(JNIEnv *env,
                                                QtJambiNativeID owner,
                                                void* iteratorPtr,
                                                PtrDeleterFunction destructor_function,
                                                AbstractAssociativeConstIteratorAccess* containerAccess)
{
    Q_ASSERT(containerAccess);
    jobject returned = nullptr;
    jobject obj = CoreAPI::javaObject(owner, env);
    bool isMutable = containerAccess->isMutableIterable();
    InPlaceInitializer initializer(nullptr, nullptr, 0, 0, {}, {JObjectWrapper(env, obj)});
    jobject ipc = Java::QtJambi::QtConstructInPlace::newInstance(env, jlong(&initializer));
    returned = isMutable ? Java::QtCore::QAssociativeIterator::newInstance2(env, ipc)
                         : Java::QtCore::QAssociativeConstIterator::newInstance2(env, ipc);
    JavaException::check(env QTJAMBI_STACKTRACEINFO);
    Java::QtJambi::QtConstructInPlace::set_native_id(env, ipc, 0);
    initializer.reset(env);
    QSharedPointer<QtJambiLink> link = QtJambiLink::createLinkForNativeObject(env, returned, iteratorPtr,
                                                                              LINK_NAME_ARG(isMutable ? "QAssociativeIterator" : "QAssociativeConstIterator")
                                                                              QtJambiLink::fromNativeId(owner), destructor_function, containerAccess);
    if(Q_UNLIKELY(!link)) {
        returned = nullptr;
        containerAccess->dispose();
    }
    return returned;
}

jobject QtJambiAPI::convertIteratorToJavaObject(JNIEnv *env,
                                                jobject owner,
                                                void* iteratorPtr,
                                                PtrDeleterFunction destructor_function,
                                                AbstractSequentialConstIteratorAccess* containerAccess)
{
    Q_ASSERT(containerAccess);
    jobject returned = nullptr;
    bool isMutable = containerAccess->isMutableIterable();
    InPlaceInitializer initializer(nullptr, nullptr, 0, 0, {}, {JObjectWrapper(env, owner)});
    jobject ipc = Java::QtJambi::QtConstructInPlace::newInstance(env, jlong(&initializer));
    returned = isMutable ? Java::QtCore::QSequentialIterator::newInstance2(env, ipc)
                         : Java::QtCore::QSequentialConstIterator::newInstance2(env, ipc);
    JavaException::check(env QTJAMBI_STACKTRACEINFO);
    Java::QtJambi::QtConstructInPlace::set_native_id(env, ipc, 0);
    initializer.reset(env);
    QSharedPointer<QtJambiLink> link = QtJambiLink::createLinkForNativeObject(env, returned, iteratorPtr,
                                                                              LINK_NAME_ARG(isMutable ? "QSequentialIterator" : "QSequentialConstIterator")
                                                                              QtJambiLink::findLinkForJavaObject(env, owner), destructor_function, containerAccess);
    if(Q_UNLIKELY(!link)) {
        returned = nullptr;
        containerAccess->dispose();
    }
    return returned;
}

jobject QtJambiAPI::convertIteratorToJavaObject(JNIEnv *env,
                                                jobject owner,
                                                void* iteratorPtr,
                                                PtrDeleterFunction destructor_function,
                                                AbstractAssociativeConstIteratorAccess* containerAccess)
{
    Q_ASSERT(containerAccess);
    jobject returned = nullptr;
    bool isMutable = containerAccess->isMutableIterable();
    InPlaceInitializer initializer(nullptr, nullptr, 0, 0, {}, {JObjectWrapper(env, owner)});
    jobject ipc = Java::QtJambi::QtConstructInPlace::newInstance(env, jlong(&initializer));
    returned = isMutable ? Java::QtCore::QAssociativeIterator::newInstance2(env, ipc)
                         : Java::QtCore::QAssociativeConstIterator::newInstance2(env, ipc);
    JavaException::check(env QTJAMBI_STACKTRACEINFO);
    Java::QtJambi::QtConstructInPlace::set_native_id(env, ipc, 0);
    initializer.reset(env);
    QSharedPointer<QtJambiLink> link = QtJambiLink::createLinkForNativeObject(env, returned, iteratorPtr,
                                                                              LINK_NAME_ARG(isMutable ? "QAssociativeIterator" : "QAssociativeConstIterator")
                                                                              QtJambiLink::findLinkForJavaObject(env, owner), destructor_function, containerAccess);
    if(Q_UNLIKELY(!link)) {
        returned = nullptr;
        containerAccess->dispose();
    }
    return returned;
}

jobject QtJambiAPI::convertIteratorToJavaObject(JNIEnv *env,
                                                void* iteratorPtr,
                                                PtrDeleterFunction destructor_function,
                                                AbstractSequentialConstIteratorAccess* containerAccess)
{
    Q_ASSERT(containerAccess);
    jobject returned = nullptr;
    bool isMutable = containerAccess->isMutableIterable();
    returned = isMutable ? Java::QtCore::QSequentialIterator::newInstance(env, nullptr)
                         : Java::QtCore::QSequentialConstIterator::newInstance(env, nullptr);
    QSharedPointer<QtJambiLink> link = QtJambiLink::createLinkForNativeObject(env,
                                                                              returned,
                                                                              iteratorPtr,
                                                                              LINK_NAME_ARG(isMutable ? "QSequentialIterator" : "QSequentialConstIterator")
                                                                              false, true,
                                                                              destructor_function,
                                                                              containerAccess,
                                                                              QtJambiLink::Ownership::Java);
    if(Q_UNLIKELY(!link)) {
        returned = nullptr;
        containerAccess->dispose();
    }
    return returned;
}

jobject QtJambiAPI::convertListIteratorToJavaObject(JNIEnv *env,
                                                    void* iteratorPtr,
                                                    PtrDeleterFunction destructor_function,
                                                    AbstractSequentialConstIteratorAccess* containerAccess)
{
    Q_ASSERT(containerAccess);
    jobject returned = nullptr;
    bool isMutable = containerAccess->isMutableIterable();
    returned = isMutable ? Java::QtCore::QList$Iterator::newInstance(env, nullptr)
                         : Java::QtCore::QList$ConstIterator::newInstance(env, nullptr);
    QSharedPointer<QtJambiLink> link = QtJambiLink::createLinkForNativeObject(env,
                                                                              returned,
                                                                              iteratorPtr,
                                                                              LINK_NAME_ARG(isMutable ? "QList::iterator" : "QList::const_iterator")
                                                                              false, true,
                                                                              destructor_function,
                                                                              containerAccess,
                                                                              QtJambiLink::Ownership::Java);
    if(Q_UNLIKELY(!link)) {
        returned = nullptr;
        containerAccess->dispose();
    }
    return returned;
}

jobject QtJambiAPI::convertListReverseIteratorToJavaObject(JNIEnv *env,
                                                    void* iteratorPtr,
                                                    PtrDeleterFunction destructor_function,
                                                    AbstractSequentialConstIteratorAccess* containerAccess)
{
    Q_ASSERT(containerAccess);
    jobject returned = nullptr;
    bool isMutable = containerAccess->isMutableIterable();
    returned = isMutable ? Java::QtCore::QList$ReverseIterator::newInstance(env, nullptr)
                         : Java::QtCore::QList$ConstReverseIterator::newInstance(env, nullptr);
    QSharedPointer<QtJambiLink> link = QtJambiLink::createLinkForNativeObject(env,
                                                                              returned,
                                                                              iteratorPtr,
                                                                              LINK_NAME_ARG(isMutable ? "QList::reverse_iterator" : "QList::const_reverse_iterator")
                                                                              false, true,
                                                                              destructor_function,
                                                                              containerAccess,
                                                                              QtJambiLink::Ownership::Java);
    if(Q_UNLIKELY(!link)) {
        returned = nullptr;
        containerAccess->dispose();
    }
    return returned;
}

jobject QtJambiAPI::convertSpanIteratorToJavaObject(JNIEnv *env,
                                                   void* iteratorPtr,
                                                   PtrDeleterFunction destructor_function,
                                                   AbstractSequentialConstIteratorAccess* containerAccess)
{
    Q_ASSERT(containerAccess);
    jobject returned = nullptr;
    bool isMutable = containerAccess->isMutableIterable();
    returned = isMutable ? Java::QtCore::QSpan$Iterator::newInstance(env, nullptr)
                         : Java::QtCore::QConstSpan$ConstIterator::newInstance(env, nullptr);
    QSharedPointer<QtJambiLink> link = QtJambiLink::createLinkForNativeObject(env,
                                                                              returned,
                                                                              iteratorPtr,
                                                                              LINK_NAME_ARG(isMutable ? "QSpan::iterator" : "QSpan::const_iterator")
                                                                              false, true,
                                                                              destructor_function,
                                                                              containerAccess,
                                                                              QtJambiLink::Ownership::Java);
    if(Q_UNLIKELY(!link)) {
        returned = nullptr;
        containerAccess->dispose();
    }
    return returned;
}

jobject QtJambiAPI::convertSpanReverseIteratorToJavaObject(JNIEnv *env,
                                                   void* iteratorPtr,
                                                   PtrDeleterFunction destructor_function,
                                                   AbstractSequentialConstIteratorAccess* containerAccess)
{
    Q_ASSERT(containerAccess);
    jobject returned = nullptr;
    bool isMutable = containerAccess->isMutableIterable();
    returned = isMutable ? Java::QtCore::QSpan$ReverseIterator::newInstance(env, nullptr)
                         : Java::QtCore::QConstSpan$ConstReverseIterator::newInstance(env, nullptr);
    QSharedPointer<QtJambiLink> link = QtJambiLink::createLinkForNativeObject(env,
                                                                              returned,
                                                                              iteratorPtr,
                                                                              LINK_NAME_ARG(isMutable ? "QSpan::reverse_iterator" : "QSpan::const_reverse_iterator")
                                                                              false, true,
                                                                              destructor_function,
                                                                              containerAccess,
                                                                              QtJambiLink::Ownership::Java);
    if(Q_UNLIKELY(!link)) {
        returned = nullptr;
        containerAccess->dispose();
    }
    return returned;
}

jobject QtJambiAPI::convertSetIteratorToJavaObject(JNIEnv *env,
                                                void* iteratorPtr,
                                                PtrDeleterFunction destructor_function,
                                                AbstractSequentialConstIteratorAccess* containerAccess)
{
    Q_ASSERT(containerAccess);
    jobject returned = nullptr;
    bool isMutable = containerAccess->isMutableIterable();
    returned = isMutable ? Java::QtCore::QSet$Iterator::newInstance(env, nullptr)
                         : Java::QtCore::QSet$ConstIterator::newInstance(env, nullptr);
    QSharedPointer<QtJambiLink> link = QtJambiLink::createLinkForNativeObject(env,
                                                                              returned,
                                                                              iteratorPtr,
                                                                              LINK_NAME_ARG(isMutable ? "QSet::iterator" : "QSet::const_iterator")
                                                                              false, true,
                                                                              destructor_function,
                                                                              containerAccess,
                                                                              QtJambiLink::Ownership::Java);
    if(Q_UNLIKELY(!link)) {
        returned = nullptr;
        containerAccess->dispose();
    }
    return returned;
}

jobject QtJambiAPI::convertHashIteratorToJavaObject(JNIEnv *env,
                                                    void* iteratorPtr,
                                                    PtrDeleterFunction destructor_function,
                                                    AbstractAssociativeConstIteratorAccess* containerAccess)
{
    Q_ASSERT(containerAccess);
    jobject returned = nullptr;
    bool isMutable = containerAccess->isMutableIterable();
    returned = isMutable ? Java::QtCore::QHash$Iterator::newInstance(env, nullptr)
                         : Java::QtCore::QHash$ConstIterator::newInstance(env, nullptr);
    QSharedPointer<QtJambiLink> link = QtJambiLink::createLinkForNativeObject(env,
                                                                              returned,
                                                                              iteratorPtr,
                                                                              LINK_NAME_ARG(isMutable ? "QHash::iterator" : "QHash::const_iterator")
                                                                              false, true,
                                                                              destructor_function,
                                                                              containerAccess,
                                                                              QtJambiLink::Ownership::Java);
    if(Q_UNLIKELY(!link)) {
        returned = nullptr;
        containerAccess->dispose();
    }
    return returned;
}

jobject QtJambiAPI::convertMapIteratorToJavaObject(JNIEnv *env,
                                                   void* iteratorPtr,
                                                   PtrDeleterFunction destructor_function,
                                                   AbstractAssociativeConstIteratorAccess* containerAccess)
{
    Q_ASSERT(containerAccess);
    jobject returned = nullptr;
    bool isMutable = containerAccess->isMutableIterable();
    returned = isMutable ? Java::QtCore::QMap$Iterator::newInstance(env, nullptr)
                         : Java::QtCore::QMap$ConstIterator::newInstance(env, nullptr);
    QSharedPointer<QtJambiLink> link = QtJambiLink::createLinkForNativeObject(env,
                                                                              returned,
                                                                              iteratorPtr,
                                                                              LINK_NAME_ARG(isMutable ? "QMap::iterator" : "QMap::const_iterator")
                                                                              false, true,
                                                                              destructor_function,
                                                                              containerAccess,
                                                                              QtJambiLink::Ownership::Java);
    if(Q_UNLIKELY(!link)) {
        returned = nullptr;
        containerAccess->dispose();
    }
    return returned;
}

jobject QtJambiAPI::convertMultiHashIteratorToJavaObject(JNIEnv *env,
                                                   void* iteratorPtr,
                                                   PtrDeleterFunction destructor_function,
                                                   AbstractAssociativeConstIteratorAccess* containerAccess)
{
    Q_ASSERT(containerAccess);
    jobject returned = nullptr;
    bool isMutable = containerAccess->isMutableIterable();
    returned = isMutable ? Java::QtCore::QMultiHash$Iterator::newInstance(env, nullptr)
                         : Java::QtCore::QMultiHash$ConstIterator::newInstance(env, nullptr);
    QSharedPointer<QtJambiLink> link = QtJambiLink::createLinkForNativeObject(env,
                                                                              returned,
                                                                              iteratorPtr,
                                                                              LINK_NAME_ARG(isMutable ? "QMultiHash::iterator" : "QMultiHash::const_iterator")
                                                                              false, true,
                                                                              destructor_function,
                                                                              containerAccess,
                                                                              QtJambiLink::Ownership::Java);
    if(Q_UNLIKELY(!link)) {
        returned = nullptr;
        containerAccess->dispose();
    }
    return returned;
}

jobject QtJambiAPI::convertMultiMapIteratorToJavaObject(JNIEnv *env,
                                                void* iteratorPtr,
                                                PtrDeleterFunction destructor_function,
                                                AbstractAssociativeConstIteratorAccess* containerAccess)
{
    Q_ASSERT(containerAccess);
    jobject returned = nullptr;
    bool isMutable = containerAccess->isMutableIterable();
    returned = isMutable ? Java::QtCore::QMultiMap$Iterator::newInstance(env, nullptr)
                         : Java::QtCore::QMultiMap$ConstIterator::newInstance(env, nullptr);
    QSharedPointer<QtJambiLink> link = QtJambiLink::createLinkForNativeObject(env,
                                                                              returned,
                                                                              iteratorPtr,
                                                                              LINK_NAME_ARG(isMutable ? "QMultiMap::iterator" : "QMultiMap::const_iterator")
                                                                              false, true,
                                                                              destructor_function,
                                                                              containerAccess,
                                                                              QtJambiLink::Ownership::Java);
    if(Q_UNLIKELY(!link)) {
        returned = nullptr;
        containerAccess->dispose();
    }
    return returned;
}

jobject QtJambiAPI::convertMapKeyIteratorToJavaObject(JNIEnv *env,
                                                   void* iteratorPtr,
                                                   PtrDeleterFunction destructor_function,
                                                   AbstractSequentialConstIteratorAccess* containerAccess)
{
    Q_ASSERT(containerAccess);
    jobject returned = nullptr;
    returned = Java::QtCore::QMap$KeyIterator::newInstance(env, nullptr);
    QSharedPointer<QtJambiLink> link = QtJambiLink::createLinkForNativeObject(env,
                                                                              returned,
                                                                              iteratorPtr,
                                                                              LINK_NAME_ARG("QMap::key_iterator")
                                                                              false, true,
                                                                              destructor_function,
                                                                              containerAccess,
                                                                              QtJambiLink::Ownership::Java);
    if(Q_UNLIKELY(!link)) {
        returned = nullptr;
        containerAccess->dispose();
    }
    return returned;
}

jobject QtJambiAPI::convertHashKeyIteratorToJavaObject(JNIEnv *env,
                                                   void* iteratorPtr,
                                                   PtrDeleterFunction destructor_function,
                                                   AbstractSequentialConstIteratorAccess* containerAccess)
{
    Q_ASSERT(containerAccess);
    jobject returned = nullptr;
    returned = Java::QtCore::QHash$KeyIterator::newInstance(env, nullptr);
    QSharedPointer<QtJambiLink> link = QtJambiLink::createLinkForNativeObject(env,
                                                                              returned,
                                                                              iteratorPtr,
                                                                              LINK_NAME_ARG("QHash::key_iterator")
                                                                              false, true,
                                                                              destructor_function,
                                                                              containerAccess,
                                                                              QtJambiLink::Ownership::Java);
    if(Q_UNLIKELY(!link)) {
        returned = nullptr;
        containerAccess->dispose();
    }
    return returned;
}

jobject QtJambiAPI::convertMultiMapKeyIteratorToJavaObject(JNIEnv *env,
                                                   void* iteratorPtr,
                                                   PtrDeleterFunction destructor_function,
                                                   AbstractSequentialConstIteratorAccess* containerAccess)
{
    Q_ASSERT(containerAccess);
    jobject returned = nullptr;
    returned = Java::QtCore::QMultiMap$KeyIterator::newInstance(env, nullptr);
    QSharedPointer<QtJambiLink> link = QtJambiLink::createLinkForNativeObject(env,
                                                                              returned,
                                                                              iteratorPtr,
                                                                              LINK_NAME_ARG("QMultiMap::key_iterator")
                                                                              false, true,
                                                                              destructor_function,
                                                                              containerAccess,
                                                                              QtJambiLink::Ownership::Java);
    if(Q_UNLIKELY(!link)) {
        returned = nullptr;
        containerAccess->dispose();
    }
    return returned;
}

jobject QtJambiAPI::convertMultiHashKeyIteratorToJavaObject(JNIEnv *env,
                                                   void* iteratorPtr,
                                                   PtrDeleterFunction destructor_function,
                                                   AbstractSequentialConstIteratorAccess* containerAccess)
{
    Q_ASSERT(containerAccess);
    jobject returned = nullptr;
    returned = Java::QtCore::QMultiHash$KeyIterator::newInstance(env, nullptr);
    QSharedPointer<QtJambiLink> link = QtJambiLink::createLinkForNativeObject(env,
                                                                              returned,
                                                                              iteratorPtr,
                                                                              LINK_NAME_ARG("QMultiHash::key_iterator")
                                                                              false, true,
                                                                              destructor_function,
                                                                              containerAccess,
                                                                              QtJambiLink::Ownership::Java);
    if(Q_UNLIKELY(!link)) {
        returned = nullptr;
        containerAccess->dispose();
    }
    return returned;
}

jobject QtJambiAPI::convertMapKeyValueIteratorToJavaObject(JNIEnv *env,
                                                   void* iteratorPtr,
                                                   PtrDeleterFunction destructor_function,
                                                   AbstractSequentialConstIteratorAccess* containerAccess)
{
    Q_ASSERT(containerAccess);
    jobject returned = nullptr;
    bool isMutable = containerAccess->isMutableIterable();
    returned = isMutable ? Java::QtCore::QMap$KeyValueIterator::newInstance(env, nullptr)
                         : Java::QtCore::QMap$ConstKeyValueIterator::newInstance(env, nullptr);
    QSharedPointer<QtJambiLink> link = QtJambiLink::createLinkForNativeObject(env,
                                                                              returned,
                                                                              iteratorPtr,
                                                                              LINK_NAME_ARG(isMutable ? "QMap::key_value_iterator" : "QMap::const_key_value_iterator")
                                                                              false, true,
                                                                              destructor_function,
                                                                              containerAccess,
                                                                              QtJambiLink::Ownership::Java);
    if(Q_UNLIKELY(!link)) {
        returned = nullptr;
        containerAccess->dispose();
    }
    return returned;
}

jobject QtJambiAPI::convertMultiMapKeyValueIteratorToJavaObject(JNIEnv *env,
                                                   void* iteratorPtr,
                                                   PtrDeleterFunction destructor_function,
                                                   AbstractSequentialConstIteratorAccess* containerAccess)
{
    Q_ASSERT(containerAccess);
    jobject returned = nullptr;
    bool isMutable = containerAccess->isMutableIterable();
    returned = isMutable ? Java::QtCore::QMultiMap$KeyValueIterator::newInstance(env, nullptr)
                         : Java::QtCore::QMultiMap$ConstKeyValueIterator::newInstance(env, nullptr);
    QSharedPointer<QtJambiLink> link = QtJambiLink::createLinkForNativeObject(env,
                                                                              returned,
                                                                              iteratorPtr,
                                                                              LINK_NAME_ARG(isMutable ? "QMultiMap::key_value_iterator" : "QMultiMap::const_key_value_iterator")
                                                                              false, true,
                                                                              destructor_function,
                                                                              containerAccess,
                                                                              QtJambiLink::Ownership::Java);
    if(Q_UNLIKELY(!link)) {
        returned = nullptr;
        containerAccess->dispose();
    }
    return returned;
}

jobject QtJambiAPI::convertHashKeyValueIteratorToJavaObject(JNIEnv *env,
                                                   void* iteratorPtr,
                                                   PtrDeleterFunction destructor_function,
                                                   AbstractSequentialConstIteratorAccess* containerAccess)
{
    Q_ASSERT(containerAccess);
    jobject returned = nullptr;
    bool isMutable = containerAccess->isMutableIterable();
    returned = isMutable ? Java::QtCore::QHash$KeyValueIterator::newInstance(env, nullptr)
                         : Java::QtCore::QHash$ConstKeyValueIterator::newInstance(env, nullptr);
    QSharedPointer<QtJambiLink> link = QtJambiLink::createLinkForNativeObject(env,
                                                                              returned,
                                                                              iteratorPtr,
                                                                              LINK_NAME_ARG(isMutable ? "QHash::key_value_iterator" : "QHash::const_key_value_iterator")
                                                                              false, true,
                                                                              destructor_function,
                                                                              containerAccess,
                                                                              QtJambiLink::Ownership::Java);
    if(Q_UNLIKELY(!link)) {
        returned = nullptr;
        containerAccess->dispose();
    }
    return returned;
}

jobject QtJambiAPI::convertMultiHashKeyValueIteratorToJavaObject(JNIEnv *env,
                                                   void* iteratorPtr,
                                                   PtrDeleterFunction destructor_function,
                                                   AbstractSequentialConstIteratorAccess* containerAccess)
{
    Q_ASSERT(containerAccess);
    jobject returned = nullptr;
    bool isMutable = containerAccess->isMutableIterable();
    returned = isMutable ? Java::QtCore::QMultiHash$KeyValueIterator::newInstance(env, nullptr)
                         : Java::QtCore::QMultiHash$ConstKeyValueIterator::newInstance(env, nullptr);
    QSharedPointer<QtJambiLink> link = QtJambiLink::createLinkForNativeObject(env,
                                                                              returned,
                                                                              iteratorPtr,
                                                                              LINK_NAME_ARG(isMutable ? "QMultiHash::key_value_iterator" : "QMultiHash::const_key_value_iterator")
                                                                              false, true,
                                                                              destructor_function,
                                                                              containerAccess,
                                                                              QtJambiLink::Ownership::Java);
    if(Q_UNLIKELY(!link)) {
        returned = nullptr;
        containerAccess->dispose();
    }
    return returned;
}

jobject QtJambiAPI::convertIteratorToJavaObject(JNIEnv *env,
                                                void* iteratorPtr,
                                                PtrDeleterFunction destructor_function,
                                                AbstractAssociativeConstIteratorAccess* containerAccess)
{
    Q_ASSERT(containerAccess);
    jobject returned = nullptr;
    bool isMutable = containerAccess->isMutableIterable();
    returned = isMutable ? Java::QtCore::QAssociativeIterator::newInstance(env, nullptr)
                         : Java::QtCore::QAssociativeConstIterator::newInstance(env, nullptr);
    QSharedPointer<QtJambiLink> link = QtJambiLink::createLinkForNativeObject(env,
                                                                              returned,
                                                                              iteratorPtr,
                                                                              LINK_NAME_ARG(isMutable ? "QAssociativeIterator" : "QAssociativeConstIterator")
                                                                              false, true,
                                                                              destructor_function,
                                                                              containerAccess,
                                                                              QtJambiLink::Ownership::Java);
    if(Q_UNLIKELY(!link)) {
        returned = nullptr;
        containerAccess->dispose();
    }
    return returned;
}

jobject createIterator(JNIEnv *env, const char* java_name, jobject obj){
    jclass java_class = JavaAPI::resolveClass(env, java_name);
    Q_ASSERT(java_class);
    jmethodID creator_method = nullptr;
    if(java_class){
        creator_method = env->GetMethodID(java_class, "<init>", "io/qt/QtObject");
        if(env->ExceptionCheck()){
            env->ExceptionClear();
        }
        if(!creator_method){
            JavaException::raiseError(env, QStringLiteral(u"internal private constructor cannot be found in class %1").arg(QString(java_name).replace('/', '.').replace('$', '.')) QTJAMBI_STACKTRACEINFO );
        }
    }
    return env->NewObject(java_class, creator_method, obj);
}

jobject createIterator(JNIEnv *env, const char* java_name){
    jclass java_class = JavaAPI::resolveClass(env, java_name);
    Q_ASSERT(java_class);
    jmethodID creator_method = nullptr;
    if(java_class){
        creator_method = findInternalPrivateConstructor(env, java_class);
        if(!creator_method){
            JavaException::raiseError(env, QStringLiteral(u"internal private constructor cannot be found in class %1").arg(QString(java_name).replace('/', '.').replace('$', '.')) QTJAMBI_STACKTRACEINFO );
        }
    }
    return env->NewObject(java_class, creator_method, nullptr);
}

jobject QtJambiPrivate::convertIteratorToJavaObject(JNIEnv *env,
                                                    const std::type_info& containerTypeId,
                                                    const std::type_info& iteratorTypeId,
                                                    const QSharedPointer<QtJambiLink>& owner,
                                                    void* iteratorPtr,
                                                    PtrDeleterFunction destructor_function,
                                                    AbstractSequentialConstIteratorAccess* containerAccess)
{
    Q_ASSERT(containerAccess);
    jobject returned = nullptr;
    auto[qt_name,java_name] = iteratorJavaType(containerTypeId, iteratorTypeId);
    returned = createIterator(env, java_name);
    QSharedPointer<QtJambiLink> link = QtJambiLink::createLinkForNativeObject(env, returned, iteratorPtr,
                                                                              LINK_NAME_ARG(qt_name)
                                                                              owner, destructor_function, containerAccess);
    if(Q_UNLIKELY(!link)) {
        returned = nullptr;
        containerAccess->dispose();
    }
    return returned;
}

jobject QtJambiPrivate::convertIteratorToJavaObject(JNIEnv *env,
                                                    const std::type_info& containerTypeId,
                                                    const std::type_info& iteratorTypeId,
                                                    const QSharedPointer<QtJambiLink>& owner,
                                                    void* iteratorPtr,
                                                    PtrDeleterFunction destructor_function,
                                                    AbstractAssociativeConstIteratorAccess* containerAccess)
{
    Q_ASSERT(containerAccess);
    jobject returned = nullptr;
    auto[qt_name,java_name] = iteratorJavaType(containerTypeId, iteratorTypeId);
    returned = createIterator(env, java_name);
    QSharedPointer<QtJambiLink> link = QtJambiLink::createLinkForNativeObject(env, returned, iteratorPtr,
                                                                              LINK_NAME_ARG(qt_name)
                                                                              owner, destructor_function, containerAccess);
    if(Q_UNLIKELY(!link)) {
        returned = nullptr;
        containerAccess->dispose();
    }
    return returned;
}

jobject QtJambiPrivate::convertIteratorToJavaObject(JNIEnv *env,
                                                    const std::type_info& containerTypeId,
                                                    const std::type_info& iteratorTypeId,
                                                    const QSharedPointer<ContainerRefPrivate>& owner,
                                                    void* iteratorPtr,
                                                    PtrDeleterFunction destructor_function,
                                                    AbstractSequentialConstIteratorAccess* containerAccess)
{
    Q_ASSERT(containerAccess);
    jobject returned = nullptr;
    auto[qt_name,java_name] = iteratorJavaType(containerTypeId, iteratorTypeId);
    returned = createIterator(env, java_name);
    QSharedPointer<QtJambiLink> link = QtJambiLink::createLinkForNativeObject(env, returned, iteratorPtr,
                                                                              LINK_NAME_ARG(qt_name)
                                                                              getLink(owner), destructor_function, containerAccess);
    if(Q_UNLIKELY(!link)) {
        returned = nullptr;
        containerAccess->dispose();
    }
    return returned;
}

jobject QtJambiPrivate::convertListIteratorToJavaObject(JNIEnv *env,
                                                        const QSharedPointer<ContainerRefPrivate>& owner,
                                                        void* iteratorPtr,
                                                        PtrDeleterFunction destructor_function,
                                                        AbstractSequentialConstIteratorAccess* containerAccess)
{
    Q_ASSERT(containerAccess);
    jobject returned = nullptr;
    bool isMutable = containerAccess->isMutableIterable();
    returned = isMutable ? Java::QtCore::QList$Iterator::newInstance(env, nullptr)
                         : Java::QtCore::QList$ConstIterator::newInstance(env, nullptr);
    QSharedPointer<QtJambiLink> link = QtJambiLink::createLinkForNativeObject(env, returned, iteratorPtr,
                                                                              LINK_NAME_ARG(isMutable ? "QList::iterator" : "QList::const_iterator")
                                                                              getLink(owner), destructor_function, containerAccess);
    if(Q_UNLIKELY(!link)) {
        returned = nullptr;
        containerAccess->dispose();
    }
    return returned;
}

jobject QtJambiPrivate::convertListReverseIteratorToJavaObject(JNIEnv *env,
                                                        const QSharedPointer<ContainerRefPrivate>& owner,
                                                        void* iteratorPtr,
                                                        PtrDeleterFunction destructor_function,
                                                        AbstractSequentialConstIteratorAccess* containerAccess)
{
    Q_ASSERT(containerAccess);
    jobject returned = nullptr;
    bool isMutable = containerAccess->isMutableIterable();
    returned = isMutable ? Java::QtCore::QList$ReverseIterator::newInstance(env, nullptr)
                         : Java::QtCore::QList$ConstReverseIterator::newInstance(env, nullptr);
    QSharedPointer<QtJambiLink> link = QtJambiLink::createLinkForNativeObject(env, returned, iteratorPtr,
                                                                              LINK_NAME_ARG(isMutable ? "QList::reverse_iterator" : "QList::const_reverse_iterator")
                                                                              getLink(owner), destructor_function, containerAccess);
    if(Q_UNLIKELY(!link)) {
        returned = nullptr;
        containerAccess->dispose();
    }
    return returned;
}

jobject QtJambiPrivate::convertSetIteratorToJavaObject(JNIEnv *env,
                                                        const QSharedPointer<ContainerRefPrivate>& owner,
                                                        void* iteratorPtr,
                                                        PtrDeleterFunction destructor_function,
                                                        AbstractSequentialConstIteratorAccess* containerAccess)
{
    Q_ASSERT(containerAccess);
    jobject returned = nullptr;
    bool isMutable = containerAccess->isMutableIterable();
    returned = isMutable ? Java::QtCore::QSet$Iterator::newInstance(env, nullptr)
                         : Java::QtCore::QSet$ConstIterator::newInstance(env, nullptr);
    QSharedPointer<QtJambiLink> link = QtJambiLink::createLinkForNativeObject(env, returned, iteratorPtr,
                                                                              LINK_NAME_ARG(isMutable ? "QSet::iterator" : "QSet::const_iterator")
                                                                              getLink(owner), destructor_function, containerAccess);
    if(Q_UNLIKELY(!link)) {
        returned = nullptr;
        containerAccess->dispose();
    }
    return returned;
}

jobject QtJambiPrivate::convertSpanIteratorToJavaObject(JNIEnv *env,
                                                       const QSharedPointer<ContainerRefPrivate>& owner,
                                                       void* iteratorPtr,
                                                       PtrDeleterFunction destructor_function,
                                                       AbstractSequentialConstIteratorAccess* containerAccess)
{
    Q_ASSERT(containerAccess);
    jobject returned = nullptr;
    bool isMutable = containerAccess->isMutableIterable();
    returned = isMutable ? Java::QtCore::QSpan$Iterator::newInstance(env, nullptr)
                         : Java::QtCore::QConstSpan$ConstIterator::newInstance(env, nullptr);
    QSharedPointer<QtJambiLink> link = QtJambiLink::createLinkForNativeObject(env, returned, iteratorPtr,
                                                                              LINK_NAME_ARG(isMutable ? "QSpan::iterator" : "QSpan::const_iterator")
                                                                              getLink(owner), destructor_function, containerAccess);
    if(Q_UNLIKELY(!link)) {
        returned = nullptr;
        containerAccess->dispose();
    }
    return returned;
}

jobject QtJambiPrivate::convertSpanReverseIteratorToJavaObject(JNIEnv *env,
                                                       const QSharedPointer<ContainerRefPrivate>& owner,
                                                       void* iteratorPtr,
                                                       PtrDeleterFunction destructor_function,
                                                       AbstractSequentialConstIteratorAccess* containerAccess)
{
    Q_ASSERT(containerAccess);
    jobject returned = nullptr;
    bool isMutable = containerAccess->isMutableIterable();
    returned = isMutable ? Java::QtCore::QSpan$ReverseIterator::newInstance(env, nullptr)
                         : Java::QtCore::QConstSpan$ConstReverseIterator::newInstance(env, nullptr);
    QSharedPointer<QtJambiLink> link = QtJambiLink::createLinkForNativeObject(env, returned, iteratorPtr,
                                                                              LINK_NAME_ARG(isMutable ? "QSpan::reverse_iterator" : "QSpan::const_reverse_iterator")
                                                                              getLink(owner), destructor_function, containerAccess);
    if(Q_UNLIKELY(!link)) {
        returned = nullptr;
        containerAccess->dispose();
    }
    return returned;
}

jobject QtJambiPrivate::convertIteratorToJavaObject(JNIEnv *env,
                                                    const std::type_info& containerTypeId,
                                                    const std::type_info& iteratorTypeId,
                                                    const QSharedPointer<ContainerRefPrivate>& owner,
                                                    void* iteratorPtr,
                                                    PtrDeleterFunction destructor_function,
                                                    AbstractAssociativeConstIteratorAccess* containerAccess)
{
    Q_ASSERT(containerAccess);
    jobject returned = nullptr;
    auto[qt_name,java_name] = iteratorJavaType(containerTypeId, iteratorTypeId);
    returned = createIterator(env, java_name);
    QSharedPointer<QtJambiLink> link = QtJambiLink::createLinkForNativeObject(env, returned, iteratorPtr,
                                                                              LINK_NAME_ARG(qt_name)
                                                                              getLink(owner), destructor_function, containerAccess);
    if(Q_UNLIKELY(!link)) {
        returned = nullptr;
        containerAccess->dispose();
    }
    return returned;
}

jobject QtJambiPrivate::convertMapKeyIteratorToJavaObject(JNIEnv *env,
                                                        const QSharedPointer<ContainerRefPrivate>& owner,
                                                        void* iteratorPtr,
                                                        PtrDeleterFunction destructor_function,
                                                        AbstractSequentialConstIteratorAccess* containerAccess)
{
    Q_ASSERT(containerAccess);
    jobject returned = nullptr;
    returned = Java::QtCore::QMap$KeyIterator::newInstance(env, nullptr);
    QSharedPointer<QtJambiLink> link = QtJambiLink::createLinkForNativeObject(env, returned, iteratorPtr,
                                                                              LINK_NAME_ARG("QMap::key_iterator")
                                                                              getLink(owner), destructor_function, containerAccess);
    if(Q_UNLIKELY(!link)) {
        returned = nullptr;
        containerAccess->dispose();
    }
    return returned;
}

jobject QtJambiPrivate::convertMapIteratorToJavaObject(JNIEnv *env,
                                                        const QSharedPointer<ContainerRefPrivate>& owner,
                                                        void* iteratorPtr,
                                                        PtrDeleterFunction destructor_function,
                                                        AbstractAssociativeConstIteratorAccess* containerAccess)
{
    Q_ASSERT(containerAccess);
    jobject returned = nullptr;
    bool isMutable = containerAccess->isMutableIterable();
    returned = isMutable ? Java::QtCore::QMap$Iterator::newInstance(env, nullptr)
                         : Java::QtCore::QMap$ConstIterator::newInstance(env, nullptr);
    QSharedPointer<QtJambiLink> link = QtJambiLink::createLinkForNativeObject(env, returned, iteratorPtr,
                                                                              LINK_NAME_ARG(isMutable ? "QMap::iterator" : "QMap::const_iterator")
                                                                              getLink(owner), destructor_function, containerAccess);
    if(Q_UNLIKELY(!link)) {
        returned = nullptr;
        containerAccess->dispose();
    }
    return returned;
}

jobject QtJambiPrivate::convertMultiMapKeyIteratorToJavaObject(JNIEnv *env,
                                                        const QSharedPointer<ContainerRefPrivate>& owner,
                                                        void* iteratorPtr,
                                                        PtrDeleterFunction destructor_function,
                                                        AbstractSequentialConstIteratorAccess* containerAccess)
{
    Q_ASSERT(containerAccess);
    jobject returned = nullptr;
    returned = Java::QtCore::QMultiMap$KeyIterator::newInstance(env, nullptr);
    QSharedPointer<QtJambiLink> link = QtJambiLink::createLinkForNativeObject(env, returned, iteratorPtr,
                                                                              LINK_NAME_ARG("QMultiMap::key_iterator")
                                                                              getLink(owner), destructor_function, containerAccess);
    if(Q_UNLIKELY(!link)) {
        returned = nullptr;
        containerAccess->dispose();
    }
    return returned;
}

jobject QtJambiPrivate::convertMultiMapIteratorToJavaObject(JNIEnv *env,
                                                       const QSharedPointer<ContainerRefPrivate>& owner,
                                                       void* iteratorPtr,
                                                       PtrDeleterFunction destructor_function,
                                                       AbstractAssociativeConstIteratorAccess* containerAccess)
{
    Q_ASSERT(containerAccess);
    jobject returned = nullptr;
    bool isMutable = containerAccess->isMutableIterable();
    returned = isMutable ? Java::QtCore::QMultiMap$Iterator::newInstance(env, nullptr)
                         : Java::QtCore::QMultiMap$ConstIterator::newInstance(env, nullptr);
    QSharedPointer<QtJambiLink> link = QtJambiLink::createLinkForNativeObject(env, returned, iteratorPtr,
                                                                              LINK_NAME_ARG(isMutable ? "QMultiMap::iterator" : "QMultiMap::const_iterator")
                                                                              getLink(owner), destructor_function, containerAccess);
    if(Q_UNLIKELY(!link)) {
        returned = nullptr;
        containerAccess->dispose();
    }
    return returned;
}

jobject QtJambiPrivate::convertHashKeyIteratorToJavaObject(JNIEnv *env,
                                                        const QSharedPointer<ContainerRefPrivate>& owner,
                                                        void* iteratorPtr,
                                                        PtrDeleterFunction destructor_function,
                                                        AbstractSequentialConstIteratorAccess* containerAccess)
{
    Q_ASSERT(containerAccess);
    jobject returned = nullptr;
    returned = Java::QtCore::QHash$KeyIterator::newInstance(env, nullptr);
    QSharedPointer<QtJambiLink> link = QtJambiLink::createLinkForNativeObject(env, returned, iteratorPtr,
                                                                              LINK_NAME_ARG("QHash::key_iterator")
                                                                              getLink(owner), destructor_function, containerAccess);
    if(Q_UNLIKELY(!link)) {
        returned = nullptr;
        containerAccess->dispose();
    }
    return returned;
}

jobject QtJambiPrivate::convertHashIteratorToJavaObject(JNIEnv *env,
                                                       const QSharedPointer<ContainerRefPrivate>& owner,
                                                       void* iteratorPtr,
                                                       PtrDeleterFunction destructor_function,
                                                       AbstractAssociativeConstIteratorAccess* containerAccess)
{
    Q_ASSERT(containerAccess);
    jobject returned = nullptr;
    bool isMutable = containerAccess->isMutableIterable();
    returned = isMutable ? Java::QtCore::QHash$Iterator::newInstance(env, nullptr)
                         : Java::QtCore::QHash$ConstIterator::newInstance(env, nullptr);
    QSharedPointer<QtJambiLink> link = QtJambiLink::createLinkForNativeObject(env, returned, iteratorPtr,
                                                                              LINK_NAME_ARG(isMutable ? "QHash::iterator" : "QHash::const_iterator")
                                                                              getLink(owner), destructor_function, containerAccess);
    if(Q_UNLIKELY(!link)) {
        returned = nullptr;
        containerAccess->dispose();
    }
    return returned;
}

jobject QtJambiPrivate::convertMultiHashKeyIteratorToJavaObject(JNIEnv *env,
                                                        const QSharedPointer<ContainerRefPrivate>& owner,
                                                        void* iteratorPtr,
                                                        PtrDeleterFunction destructor_function,
                                                        AbstractSequentialConstIteratorAccess* containerAccess)
{
    Q_ASSERT(containerAccess);
    jobject returned = nullptr;
    returned = Java::QtCore::QMultiHash$KeyIterator::newInstance(env, nullptr);
    QSharedPointer<QtJambiLink> link = QtJambiLink::createLinkForNativeObject(env, returned, iteratorPtr,
                                                                              LINK_NAME_ARG("QMultiHash::key_iterator")
                                                                              getLink(owner), destructor_function, containerAccess);
    if(Q_UNLIKELY(!link)) {
        returned = nullptr;
        containerAccess->dispose();
    }
    return returned;
}

jobject QtJambiPrivate::convertMultiHashIteratorToJavaObject(JNIEnv *env,
                                                            const QSharedPointer<ContainerRefPrivate>& owner,
                                                            void* iteratorPtr,
                                                            PtrDeleterFunction destructor_function,
                                                            AbstractAssociativeConstIteratorAccess* containerAccess)
{
    Q_ASSERT(containerAccess);
    jobject returned = nullptr;
    bool isMutable = containerAccess->isMutableIterable();
    returned = isMutable ? Java::QtCore::QMultiHash$Iterator::newInstance(env, nullptr)
                         : Java::QtCore::QMultiHash$ConstIterator::newInstance(env, nullptr);
    QSharedPointer<QtJambiLink> link = QtJambiLink::createLinkForNativeObject(env, returned, iteratorPtr,
                                                                              LINK_NAME_ARG(isMutable ? "QMultiHash::iterator" : "QMultiHash::const_iterator")
                                                                              getLink(owner), destructor_function, containerAccess);
    if(Q_UNLIKELY(!link)) {
        returned = nullptr;
        containerAccess->dispose();
    }
    return returned;
}

jobject QtJambiPrivate::convertHashKeyValueIteratorToJavaObject(JNIEnv *env,
                                                       const QSharedPointer<ContainerRefPrivate>& owner,
                                                       void* iteratorPtr,
                                                       PtrDeleterFunction destructor_function,
                                                       AbstractSequentialConstIteratorAccess* containerAccess)
{
    Q_ASSERT(containerAccess);
    jobject returned = nullptr;
    bool isMutable = containerAccess->isMutableIterable();
    returned = isMutable ? Java::QtCore::QHash$KeyValueIterator::newInstance(env, nullptr)
                         : Java::QtCore::QHash$ConstKeyValueIterator::newInstance(env, nullptr);
    QSharedPointer<QtJambiLink> link = QtJambiLink::createLinkForNativeObject(env, returned, iteratorPtr,
                                                                              LINK_NAME_ARG(isMutable ? "QHash::key_value_iterator" : "QHash::const_key_value_iterator")
                                                                              getLink(owner), destructor_function, containerAccess);
    if(Q_UNLIKELY(!link)) {
        returned = nullptr;
        containerAccess->dispose();
    }
    return returned;
}

jobject QtJambiPrivate::convertMapKeyValueIteratorToJavaObject(JNIEnv *env,
                                                       const QSharedPointer<ContainerRefPrivate>& owner,
                                                       void* iteratorPtr,
                                                       PtrDeleterFunction destructor_function,
                                                       AbstractSequentialConstIteratorAccess* containerAccess)
{
    Q_ASSERT(containerAccess);
    jobject returned = nullptr;
    bool isMutable = containerAccess->isMutableIterable();
    returned = isMutable ? Java::QtCore::QMap$KeyValueIterator::newInstance(env, nullptr)
                         : Java::QtCore::QMap$ConstKeyValueIterator::newInstance(env, nullptr);
    QSharedPointer<QtJambiLink> link = QtJambiLink::createLinkForNativeObject(env, returned, iteratorPtr,
                                                                              LINK_NAME_ARG(isMutable ? "QMap::key_value_iterator" : "QMap::const_key_value_iterator")
                                                                              getLink(owner), destructor_function, containerAccess);
    if(Q_UNLIKELY(!link)) {
        returned = nullptr;
        containerAccess->dispose();
    }
    return returned;
}

jobject QtJambiPrivate::convertMultiHashKeyValueIteratorToJavaObject(JNIEnv *env,
                                                       const QSharedPointer<ContainerRefPrivate>& owner,
                                                       void* iteratorPtr,
                                                       PtrDeleterFunction destructor_function,
                                                       AbstractSequentialConstIteratorAccess* containerAccess)
{
    Q_ASSERT(containerAccess);
    jobject returned = nullptr;
    bool isMutable = containerAccess->isMutableIterable();
    returned = isMutable ? Java::QtCore::QMultiHash$KeyValueIterator::newInstance(env, nullptr)
                         : Java::QtCore::QMultiHash$ConstKeyValueIterator::newInstance(env, nullptr);
    QSharedPointer<QtJambiLink> link = QtJambiLink::createLinkForNativeObject(env, returned, iteratorPtr,
                                                                              LINK_NAME_ARG(isMutable ? "QMultiHash::key_value_iterator" : "QMultiHash::const_key_value_iterator")
                                                                              getLink(owner), destructor_function, containerAccess);
    if(Q_UNLIKELY(!link)) {
        returned = nullptr;
        containerAccess->dispose();
    }
    return returned;
}

jobject QtJambiPrivate::convertMultiMapKeyValueIteratorToJavaObject(JNIEnv *env,
                                                       const QSharedPointer<ContainerRefPrivate>& owner,
                                                       void* iteratorPtr,
                                                       PtrDeleterFunction destructor_function,
                                                       AbstractSequentialConstIteratorAccess* containerAccess)
{
    Q_ASSERT(containerAccess);
    jobject returned = nullptr;
    bool isMutable = containerAccess->isMutableIterable();
    returned = isMutable ? Java::QtCore::QMultiMap$KeyValueIterator::newInstance(env, nullptr)
                         : Java::QtCore::QMultiMap$ConstKeyValueIterator::newInstance(env, nullptr);
    QSharedPointer<QtJambiLink> link = QtJambiLink::createLinkForNativeObject(env, returned, iteratorPtr,
                                                                              LINK_NAME_ARG(isMutable ? "QMultiMap::key_value_iterator" : "QMultiMap::const_key_value_iterator")
                                                                              getLink(owner), destructor_function, containerAccess);
    if(Q_UNLIKELY(!link)) {
        returned = nullptr;
        containerAccess->dispose();
    }
    return returned;
}

jobject QtJambiAPI::convertIteratorToJavaObject(JNIEnv *env,
                                                const std::type_info& containerTypeId,
                                                const std::type_info& iteratorTypeId,
                                                QtJambiNativeID owner,
                                                void* iteratorPtr,
                                                PtrDeleterFunction destructor_function,
                                                AbstractSequentialConstIteratorAccess* containerAccess)
{
    Q_ASSERT(containerAccess);
    jobject returned = nullptr;
    auto[qt_name,java_name] = iteratorJavaType(containerTypeId, iteratorTypeId);
    jobject obj = CoreAPI::javaObject(owner, env);
    returned = createIterator(env, java_name, obj);
    QSharedPointer<QtJambiLink> link = QtJambiLink::createLinkForNativeObject(env, returned, iteratorPtr,
                                                                              LINK_NAME_ARG(qt_name)
                                                                              QtJambiLink::fromNativeId(owner), destructor_function, containerAccess);
    if(Q_UNLIKELY(!link)) {
        returned = nullptr;
        containerAccess->dispose();
    }
    return returned;
}

jobject QtJambiAPI::convertIteratorToJavaObject(JNIEnv *env,
                                                const std::type_info& containerTypeId,
                                                const std::type_info& iteratorTypeId,
                                                QtJambiNativeID owner,
                                                void* iteratorPtr,
                                                PtrDeleterFunction destructor_function,
                                                AbstractAssociativeConstIteratorAccess* containerAccess)
{
    Q_ASSERT(containerAccess);
    jobject returned = nullptr;
    auto[qt_name,java_name] = iteratorJavaType(containerTypeId, iteratorTypeId);
    returned = createIterator(env, java_name);
    QSharedPointer<QtJambiLink> link = QtJambiLink::createLinkForNativeObject(env, returned, iteratorPtr,
                                                                              LINK_NAME_ARG(qt_name)
                                                                              QtJambiLink::fromNativeId(owner), destructor_function, containerAccess);
    if(Q_UNLIKELY(!link)) {
        returned = nullptr;
        containerAccess->dispose();
    }
    return returned;
}

jobject QtJambiAPI::convertIteratorToJavaObject(JNIEnv *env,
                                                const std::type_info& containerTypeId,
                                                const std::type_info& iteratorTypeId,
                                                jobject owner,
                                                void* iteratorPtr,
                                                PtrDeleterFunction destructor_function,
                                                AbstractSequentialConstIteratorAccess* containerAccess)
{
    Q_ASSERT(containerAccess);
    jobject returned = nullptr;
    auto[qt_name,java_name] = iteratorJavaType(containerTypeId, iteratorTypeId);
    returned = createIterator(env, java_name, owner);
    QSharedPointer<QtJambiLink> link = QtJambiLink::createLinkForNativeObject(env, returned, iteratorPtr,
                                                                              LINK_NAME_ARG(qt_name)
                                                                              QtJambiLink::findLinkForJavaObject(env, owner), destructor_function, containerAccess);
    if(Q_UNLIKELY(!link)) {
        returned = nullptr;
        containerAccess->dispose();
    }
    return returned;
}

jobject QtJambiAPI::convertIteratorToJavaObject(JNIEnv *env,
                                                const std::type_info& containerTypeId,
                                                const std::type_info& iteratorTypeId,
                                                jobject owner,
                                                void* iteratorPtr,
                                                PtrDeleterFunction destructor_function,
                                                AbstractAssociativeConstIteratorAccess* containerAccess)
{
    Q_ASSERT(containerAccess);
    jobject returned = nullptr;
    auto[qt_name,java_name] = iteratorJavaType(containerTypeId, iteratorTypeId);
    returned = createIterator(env, java_name, owner);
    QSharedPointer<QtJambiLink> link = QtJambiLink::createLinkForNativeObject(env, returned, iteratorPtr,
                                                                              LINK_NAME_ARG(qt_name)
                                                                              QtJambiLink::findLinkForJavaObject(env, owner), destructor_function, containerAccess);
    if(Q_UNLIKELY(!link)) {
        returned = nullptr;
        containerAccess->dispose();
    }
    return returned;
}

jobject QtJambiAPI::convertIteratorToJavaObject(JNIEnv *env,
                                                const std::type_info& containerTypeId,
                                                const std::type_info& iteratorTypeId,
                                                void* iteratorPtr,
                                                PtrDeleterFunction destructor_function,
                                                AbstractSequentialConstIteratorAccess* containerAccess)
{
    Q_ASSERT(containerAccess);
    jobject returned = nullptr;
    auto[qt_name,java_name] = iteratorJavaType(containerTypeId, iteratorTypeId);
    returned = createIterator(env, java_name);
    QSharedPointer<QtJambiLink> link = QtJambiLink::createLinkForNativeObject(env,
                                                                              returned,
                                                                              iteratorPtr,
                                                                              LINK_NAME_ARG(qt_name)
                                                                              false, true,
                                                                              destructor_function,
                                                                              containerAccess,
                                                                              QtJambiLink::Ownership::Java);
    if(Q_UNLIKELY(!link)) {
        returned = nullptr;
        containerAccess->dispose();
    }
    return returned;
}

jobject QtJambiAPI::convertIteratorToJavaObject(JNIEnv *env,
                                                const std::type_info& containerTypeId,
                                                const std::type_info& iteratorTypeId,
                                                void* iteratorPtr,
                                                PtrDeleterFunction destructor_function,
                                                AbstractAssociativeConstIteratorAccess* containerAccess)
{
    Q_ASSERT(containerAccess);
    jobject returned = nullptr;
    auto[qt_name,java_name] = iteratorJavaType(containerTypeId, iteratorTypeId);
    returned = createIterator(env, java_name);
    QSharedPointer<QtJambiLink> link = QtJambiLink::createLinkForNativeObject(env,
                                                                              returned,
                                                                              iteratorPtr,
                                                                              LINK_NAME_ARG(qt_name)
                                                                              false, true,
                                                                              destructor_function,
                                                                              containerAccess,
                                                                              QtJambiLink::Ownership::Java);
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
            case QtJambiAPI::ListType::QStack:
                if(Java::QtCore::QStack::isInstanceOf(env, obj)){
                    containerAccess->dispose();
                    return obj;
                }
                break;
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
                case QtJambiAPI::ListType::QStack:
                    if(Java::QtCore::QStack::isInstanceOf(env, obj)){
                        containerAccess->dispose();
                        return obj;
                    }
                    break;
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
    QByteArray containerName;
    switch(listType){
    case QtJambiAPI::ListType::QQueue:
        returned = Java::QtCore::QQueue::newInstance(env, nullptr);
        containerName = "QQueue<";
        break;
    case QtJambiAPI::ListType::QStack:
        returned = Java::QtCore::QStack::newInstance(env, nullptr);
        containerName = "QStack<";
        break;
    default:
        returned = Java::QtCore::QList::newInstance(env, nullptr);
        containerName = "QList<";
        break;
    }
    containerName += containerAccess->elementMetaType().name();
    containerName += ">";
    containerName = QMetaObject::normalizedType(containerName);

    QMetaType containerMetaType(containerAccess->registerContainer(containerName));
    Q_UNUSED(containerMetaType)
    QSharedPointer<QtJambiLink> link;
    if(QSharedPointer<QtJambiLink> _owner = QtJambiLink::fromNativeId(owner)){
        link = QtJambiLink::createLinkForNativeObject(env, returned, const_cast<void*>(listPtr),
                                                        LINK_NAME_META_TYPE_ARG(containerMetaType)
                                                        _owner, containerAccess);
    }else if(deleter){
        if(copyFunction){
            link = QtJambiLink::createLinkForNativeObject(env, returned, copyFunction(listPtr),
                                                       LINK_NAME_META_TYPE_ARG(containerMetaType)
                                                       false, true, deleter, containerAccess, QtJambiLink::Ownership::Java);
        }else{
            link = QtJambiLink::createLinkForNativeObject(env, returned, const_cast<void*>(listPtr),
                                                       LINK_NAME_META_TYPE_ARG(containerMetaType)
                                                       false, false, deleter, containerAccess, QtJambiLink::Ownership::None);
        }
    }else{
        if(copyFunction){
            link = QtJambiLink::createLinkForNativeObject(env, returned, copyFunction(listPtr),
                                                       LINK_NAME_META_TYPE_ARG(containerMetaType)
                                                       false, true, containerAccess, QtJambiLink::Ownership::Java);
        }else{
            link = QtJambiLink::createLinkForNativeObject(env, returned, const_cast<void*>(listPtr),
                                                       LINK_NAME_META_TYPE_ARG(containerMetaType)
                                                       false, false, containerAccess, QtJambiLink::Ownership::None);
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
    if(containerAccess && containerAccess->isList()){
        auto access = static_cast<AbstractListAccess*>(containerAccess);
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
                containerAccess = link->containerAccess() && link->containerAccess()->isList() ? static_cast<AbstractListAccess*>(link->containerAccess()) : nullptr;
                listPtr = link->pointer();
                return obj;
            }
        }
    }
    QSharedPointer<QtJambiLink> link;
    jobject returned = nullptr;
    if(containerAccess->elementMetaType().id()==QMetaType::QString){
        containerAccess->dispose();
        containerAccess = QListAccess<QString>::newInstance();
        returned = Java::QtCore::QStringList::newInstance(env, nullptr);
        link = QtJambiLink::createLinkForNativeObject(env, returned, const_cast<void*>(listPtr),
                                                      LINK_NAME_ARG("QStringList")
                                                      false, false, containerAccess, QtJambiLink::Ownership::None);
    }else{
        QByteArray containerName;
        returned = Java::QtCore::QList::newInstance(env, nullptr);
        containerName = "QList<";
        containerName += containerAccess->elementMetaType().name();
        containerName += ">";
        containerName = QMetaObject::normalizedType(containerName);
        containerAccess->registerContainer(containerName);
        link = QtJambiLink::createLinkForNativeObject(env, returned, const_cast<void*>(listPtr),
                                                      LINK_NAME_ARG(containerName)
                                                      false, false, containerAccess, QtJambiLink::Ownership::None);
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
    if(containerAccess && containerAccess->isSpan()){
        auto access = static_cast<AbstractSpanAccess*>(containerAccess);
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
            if(Java::QtCore::QConstSpan::isInstanceOf(env, obj) && containerAccess){
                if(containerAccess->isConst() || Java::QtCore::QSpan::isInstanceOf(env, obj)){
                    containerAccess->dispose();
                    containerAccess = link->containerAccess() && link->containerAccess()->isSpan() ? static_cast<AbstractSpanAccess*>(link->containerAccess()) : nullptr;
                    listPtr = link->pointer();
                    return obj;
                }
            }
        }
    }
    QSharedPointer<QtJambiLink> link;
    jobject returned = nullptr;
    QByteArray containerName;
    containerName = "QSpan<";
    if(containerAccess && containerAccess->isConst()){
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
                                                  false, false, containerAccess, QtJambiLink::Ownership::None);
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
            case QtJambiAPI::ListType::QStack:
                if(Java::QtCore::QStack::isInstanceOf(env, obj)){
                    containerAccess->dispose();
                    return obj;
                }
                break;
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
    QByteArray containerName;
    switch(listType){
    case QtJambiAPI::ListType::QQueue:
        returned = Java::QtCore::QQueue::newInstance(env, nullptr);
        containerName = "QQueue<";
        break;
    case QtJambiAPI::ListType::QStack:
        returned = Java::QtCore::QStack::newInstance(env, nullptr);
        containerName = "QStack<";
        break;
    default:
        returned = Java::QtCore::QList::newInstance(env, nullptr);
        containerName = "QList<";
        break;
    }
    containerName += containerAccess->elementMetaType().name();
    containerName += ">";
    containerName = QMetaObject::normalizedType(containerName);
    QMetaType containerMetaType(containerAccess->registerContainer(containerName));
    Q_UNUSED(containerMetaType)
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
    AbstractListAccess* containerAccess = QListAccess<QString>::newInstance();
    returned = Java::QtCore::QStringList::newInstance(env, nullptr);

    QSharedPointer<QtJambiLink> link;
    if(QSharedPointer<QtJambiLink> _owner = QtJambiLink::fromNativeId(owner)){
        link = QtJambiLink::createLinkForNativeObject(env, returned, const_cast<void*>(listPtr),
                                                        LINK_NAME_ARG("QStringList")
                                                        _owner, containerAccess);
    }else if(deleter){
        if(copyFunction){
            link = QtJambiLink::createLinkForNativeObject(env, returned, copyFunction(listPtr),
                                                       LINK_NAME_ARG("QStringList")
                                                       false, true, deleter, containerAccess, QtJambiLink::Ownership::Java);
        }else{
            link = QtJambiLink::createLinkForNativeObject(env, returned, const_cast<void*>(listPtr),
                                                       LINK_NAME_ARG("QStringList")
                                                       false, false, deleter, containerAccess, QtJambiLink::Ownership::None);
        }
    }else{
        if(copyFunction){
            link = QtJambiLink::createLinkForNativeObject(env, returned, copyFunction(listPtr),
                                                       LINK_NAME_ARG("QStringList")
                                                       false, true, containerAccess, QtJambiLink::Ownership::Java);
        }else{
            link = QtJambiLink::createLinkForNativeObject(env, returned, const_cast<void*>(listPtr),
                                                       LINK_NAME_ARG("QStringList")
                                                       false, false, containerAccess, QtJambiLink::Ownership::None);
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

    AbstractListAccess* containerAccess = QListAccess<QString>::newInstance();

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
    returned = Java::QtCore::QSet::newInstance(env, nullptr);
    QSharedPointer<QtJambiLink> link;
    QByteArray containerName = "QSet<";
    containerName += containerAccess->elementMetaType().name();
    containerName += ">";
    containerName = QMetaObject::normalizedType(containerName);
    QMetaType containerMetaType(containerAccess->registerContainer(containerName));
    Q_UNUSED(containerMetaType)
    if(QSharedPointer<QtJambiLink> _owner = QtJambiLink::fromNativeId(owner)){
        link = QtJambiLink::createLinkForNativeObject(env, returned, const_cast<void*>(listPtr),
                                                        LINK_NAME_META_TYPE_ARG(containerMetaType)
                                                        _owner, containerAccess);
    }else if(deleter){
        if(copyFunction){
            link = QtJambiLink::createLinkForNativeObject(env, returned, copyFunction(listPtr),
                                                       LINK_NAME_META_TYPE_ARG(containerMetaType)
                                                       false, true, deleter, containerAccess, QtJambiLink::Ownership::Java);
        }else{
            link = QtJambiLink::createLinkForNativeObject(env, returned, const_cast<void*>(listPtr),
                                                       LINK_NAME_META_TYPE_ARG(containerMetaType)
                                                       false, false, deleter, containerAccess, QtJambiLink::Ownership::None);
        }
    }else{
        if(copyFunction){
            link = QtJambiLink::createLinkForNativeObject(env, returned, copyFunction(listPtr),
                                                       LINK_NAME_META_TYPE_ARG(containerMetaType)
                                                       false, true, containerAccess, QtJambiLink::Ownership::Java);
        }else{
            link = QtJambiLink::createLinkForNativeObject(env, returned, const_cast<void*>(listPtr),
                                                       LINK_NAME_META_TYPE_ARG(containerMetaType)
                                                       false, false, containerAccess, QtJambiLink::Ownership::None);
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
    returned = Java::QtCore::QSet::newInstance(env, nullptr);
    QByteArray containerName = "QSet<";
    containerName += containerAccess->elementMetaType().name();
    containerName += ">";
    containerName = QMetaObject::normalizedType(containerName);
    QMetaType containerMetaType(containerAccess->registerContainer(containerName));
    Q_UNUSED(containerMetaType)
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
    returned = Java::QtCore::QHash::newInstance(env, nullptr);
    QByteArray containerName = "QHash<";
    containerName += containerAccess->keyMetaType().name();
    containerName += ",";
    containerName += containerAccess->valueMetaType().name();
    containerName += ">";
    containerName = QMetaObject::normalizedType(containerName);
    QMetaType containerMetaType(containerAccess->registerContainer(containerName));
    Q_UNUSED(containerMetaType)
    QSharedPointer<QtJambiLink> link;
    if(QSharedPointer<QtJambiLink> _owner = QtJambiLink::fromNativeId(owner)){
        link = QtJambiLink::createLinkForNativeObject(env, returned, const_cast<void*>(listPtr),
                                                        LINK_NAME_META_TYPE_ARG(containerMetaType)
                                                        _owner, containerAccess);
    }else if(deleter){
        if(copyFunction){
            link = QtJambiLink::createLinkForNativeObject(env, returned, copyFunction(listPtr),
                                                       LINK_NAME_META_TYPE_ARG(containerMetaType)
                                                       false, true, deleter, containerAccess, QtJambiLink::Ownership::Java);
        }else{
            link = QtJambiLink::createLinkForNativeObject(env, returned, const_cast<void*>(listPtr),
                                                       LINK_NAME_META_TYPE_ARG(containerMetaType)
                                                       false, false, deleter, containerAccess, QtJambiLink::Ownership::None);
        }
    }else{
        if(copyFunction){
            link = QtJambiLink::createLinkForNativeObject(env, returned, copyFunction(listPtr),
                                                       LINK_NAME_META_TYPE_ARG(containerMetaType)
                                                       false, true, containerAccess, QtJambiLink::Ownership::Java);
        }else{
            link = QtJambiLink::createLinkForNativeObject(env, returned, const_cast<void*>(listPtr),
                                                       LINK_NAME_META_TYPE_ARG(containerMetaType)
                                                       false, false, containerAccess, QtJambiLink::Ownership::None);
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
    returned = Java::QtCore::QHash::newInstance(env, nullptr);
    QByteArray containerName = "QHash<";
    containerName += containerAccess->keyMetaType().name();
    containerName += ",";
    containerName += containerAccess->valueMetaType().name();
    containerName += ">";
    containerName = QMetaObject::normalizedType(containerName);
    QMetaType containerMetaType(containerAccess->registerContainer(containerName));
    Q_UNUSED(containerMetaType)
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
    returned = Java::QtCore::QMultiHash::newInstance(env, nullptr);
    QSharedPointer<QtJambiLink> link;
    QByteArray containerName = "QMultiHash<";
    containerName += containerAccess->keyMetaType().name();
    containerName += ",";
    containerName += containerAccess->valueMetaType().name();
    containerName += ">";
    containerName = QMetaObject::normalizedType(containerName);
    QMetaType containerMetaType(containerAccess->registerContainer(containerName));
    Q_UNUSED(containerMetaType)
    if(QSharedPointer<QtJambiLink> _owner = QtJambiLink::fromNativeId(owner)){
        link = QtJambiLink::createLinkForNativeObject(env, returned, const_cast<void*>(listPtr),
                                                        LINK_NAME_META_TYPE_ARG(containerMetaType)
                                                        _owner, containerAccess);
    }else if(deleter){
        if(copyFunction){
            link = QtJambiLink::createLinkForNativeObject(env, returned, copyFunction(listPtr),
                                                       LINK_NAME_META_TYPE_ARG(containerMetaType)
                                                       false, true, deleter, containerAccess, QtJambiLink::Ownership::Java);
        }else{
            link = QtJambiLink::createLinkForNativeObject(env, returned, const_cast<void*>(listPtr),
                                                       LINK_NAME_META_TYPE_ARG(containerMetaType)
                                                       false, false, deleter, containerAccess, QtJambiLink::Ownership::None);
        }
    }else{
        if(copyFunction){
            link = QtJambiLink::createLinkForNativeObject(env, returned, copyFunction(listPtr),
                                                       LINK_NAME_META_TYPE_ARG(containerMetaType)
                                                       false, true, containerAccess, QtJambiLink::Ownership::Java);
        }else{
            link = QtJambiLink::createLinkForNativeObject(env, returned, const_cast<void*>(listPtr),
                                                       LINK_NAME_META_TYPE_ARG(containerMetaType)
                                                       false, false, containerAccess, QtJambiLink::Ownership::None);
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
    returned = Java::QtCore::QMultiHash::newInstance(env, nullptr);
    QByteArray containerName = "QMultiHash<";
    containerName += containerAccess->keyMetaType().name();
    containerName += ",";
    containerName += containerAccess->valueMetaType().name();
    containerName += ">";
    containerName = QMetaObject::normalizedType(containerName);
    QMetaType containerMetaType(containerAccess->registerContainer(containerName));
    Q_UNUSED(containerMetaType)
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
    returned = Java::QtCore::QMap::newInstance(env, nullptr);
    QByteArray containerName = "QMap<";
    containerName += containerAccess->keyMetaType().name();
    containerName += ",";
    containerName += containerAccess->valueMetaType().name();
    containerName += ">";
    containerName = QMetaObject::normalizedType(containerName);
    QMetaType containerMetaType(containerAccess->registerContainer(containerName));
    Q_UNUSED(containerMetaType)
    QSharedPointer<QtJambiLink> link;
    if(QSharedPointer<QtJambiLink> _owner = QtJambiLink::fromNativeId(owner)){
        link = QtJambiLink::createLinkForNativeObject(env, returned, const_cast<void*>(listPtr),
                                                        LINK_NAME_META_TYPE_ARG(containerMetaType)
                                                        _owner, containerAccess);
    }else if(deleter){
        if(copyFunction){
            link = QtJambiLink::createLinkForNativeObject(env, returned, copyFunction(listPtr),
                                                       LINK_NAME_META_TYPE_ARG(containerMetaType)
                                                       false, true, deleter, containerAccess, QtJambiLink::Ownership::Java);
        }else{
            link = QtJambiLink::createLinkForNativeObject(env, returned, const_cast<void*>(listPtr),
                                                       LINK_NAME_META_TYPE_ARG(containerMetaType)
                                                       false, false, deleter, containerAccess, QtJambiLink::Ownership::None);
        }
    }else{
        if(copyFunction){
            link = QtJambiLink::createLinkForNativeObject(env, returned, copyFunction(listPtr),
                                                       LINK_NAME_META_TYPE_ARG(containerMetaType)
                                                       false, true, containerAccess, QtJambiLink::Ownership::Java);
        }else{
            link = QtJambiLink::createLinkForNativeObject(env, returned, const_cast<void*>(listPtr),
                                                       LINK_NAME_META_TYPE_ARG(containerMetaType)
                                                       false, false, containerAccess, QtJambiLink::Ownership::None);
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
    returned = Java::QtCore::QMap::newInstance(env, nullptr);
    QByteArray containerName = "QMap<";
    containerName += containerAccess->keyMetaType().name();
    containerName += ",";
    containerName += containerAccess->valueMetaType().name();
    containerName += ">";
    containerName = QMetaObject::normalizedType(containerName);
    QMetaType containerMetaType(containerAccess->registerContainer(containerName));
    Q_UNUSED(containerMetaType)
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
    returned = Java::QtCore::QMultiMap::newInstance(env, nullptr);
    QByteArray containerName = "QMultiMap<";
    containerName += containerAccess->keyMetaType().name();
    containerName += ",";
    containerName += containerAccess->valueMetaType().name();
    containerName += ">";
    containerName = QMetaObject::normalizedType(containerName);
    QMetaType containerMetaType(containerAccess->registerContainer(containerName));
    Q_UNUSED(containerMetaType)
    QSharedPointer<QtJambiLink> link;
    if(QSharedPointer<QtJambiLink> _owner = QtJambiLink::fromNativeId(owner)){
        link = QtJambiLink::createLinkForNativeObject(env, returned, const_cast<void*>(listPtr),
                                                        LINK_NAME_META_TYPE_ARG(containerMetaType)
                                                        _owner, containerAccess);
    }else if(deleter){
        if(copyFunction){
            link = QtJambiLink::createLinkForNativeObject(env, returned, copyFunction(listPtr),
                                                       LINK_NAME_META_TYPE_ARG(containerMetaType)
                                                       false, true, deleter, containerAccess, QtJambiLink::Ownership::Java);
        }else{
            link = QtJambiLink::createLinkForNativeObject(env, returned, const_cast<void*>(listPtr),
                                                       LINK_NAME_META_TYPE_ARG(containerMetaType)
                                                       false, false, deleter, containerAccess, QtJambiLink::Ownership::None);
        }
    }else{
        if(copyFunction){
            link = QtJambiLink::createLinkForNativeObject(env, returned, copyFunction(listPtr),
                                                       LINK_NAME_META_TYPE_ARG(containerMetaType)
                                                       false, true, containerAccess, QtJambiLink::Ownership::Java);
        }else{
            link = QtJambiLink::createLinkForNativeObject(env, returned, const_cast<void*>(listPtr),
                                                       LINK_NAME_META_TYPE_ARG(containerMetaType)
                                                       false, false, containerAccess, QtJambiLink::Ownership::None);
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
    returned = Java::QtCore::QMultiMap::newInstance(env, nullptr);
    QByteArray containerName = "QMultiMap<";
    containerName += containerAccess->keyMetaType().name();
    containerName += ",";
    containerName += containerAccess->valueMetaType().name();
    containerName += ">";
    containerName = QMetaObject::normalizedType(containerName);
    QMetaType containerMetaType(containerAccess->registerContainer(containerName));
    Q_UNUSED(containerMetaType)
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

void CoreAPI::initializeIterator(JNIEnv * env, jobject _this, jobject other, jboolean targetConst){
    QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForJavaObject(env, other);
    if(!link){
        JavaException::raise<Java::QtJambi::QNoNativeResourcesException>(env, QStringLiteral("Incomplete object of type: %1").arg(QtJambiAPI::getObjectClassNamePrintable(env, other)) QTJAMBI_STACKTRACEINFO );
    }
    AbstractSequentialConstIteratorAccess* containerAccess = nullptr;
    if(link->containerAccess() && link->containerAccess()->isSequentialConstIterator())
        containerAccess = static_cast<AbstractSequentialConstIteratorAccess*>(link->containerAccess());
    void* newIterator{nullptr};
    if(containerAccess){
        if(targetConst){
            std::pair<void*,AbstractSequentialConstIteratorAccess*> pair = static_cast<AbstractSequentialIteratorAccess*>(containerAccess)->createConstIterator(link->pointer());
            if(pair.first){
                newIterator = pair.first;
                containerAccess = pair.second;
            }else
                JavaException::raise<Java::Runtime::RuntimeException>(env, QStringLiteral("Unable to clone iterator type: %1").arg(QtJambiAPI::getObjectClassNamePrintable(env, other)) QTJAMBI_STACKTRACEINFO );
        }else{
            if(!containerAccess->canCopy())
                JavaException::raise<Java::Runtime::RuntimeException>(env, QStringLiteral("Unable to clone iterator type: %1").arg(QtJambiAPI::getObjectClassNamePrintable(env, other)) QTJAMBI_STACKTRACEINFO );
            containerAccess = containerAccess->clone();
            Q_ASSERT(containerAccess);
            newIterator = containerAccess->createContainer(env, ConstContainerAndAccessInfo{other, link->pointer(), link->containerAccess()});
        }
    }else{
        JavaException::raise<Java::QtJambi::QNoNativeResourcesException>(env, QStringLiteral("Incomplete object of type: %1").arg(QtJambiAPI::getObjectClassNamePrintable(env, other)) QTJAMBI_STACKTRACEINFO );
    }
    QByteArray name;
    name = "QIterator<";
    name += containerAccess->valueMetaType().name();
    name += ">";
    name = QMetaObject::normalizedType(name);
    QSharedPointer<QtJambiLink> newLink = QtJambiLink::createLinkForNativeObject(env, _this, newIterator,
                                                                              LINK_NAME_ARG(name)
                                                                              true, true, containerAccess, QtJambiLink::Ownership::Java);
    if(Q_UNLIKELY(!newLink)) {
        containerAccess->deleteContainer(newIterator);
        containerAccess->dispose();
    }
}

#define QTJAMBI_CONTAINER_CAST(Type, target, source) \
Q_ASSERT(source->is##Type());\
    Abstract##Type##Access* target = static_cast<Abstract##Type##Access*>(source);

void CoreAPI::initializeQList(JNIEnv *env, jobject object, QtJambiNativeID beginId, QtJambiNativeID endId, int associativeMapMode){
    using namespace QtJambiPrivate;
    AbstractListAccess* containerAccess = nullptr;
    QPair<void*,AbstractContainerAccess*> beginPair = ContainerAPI::fromNativeId(beginId);
    QPair<void*,AbstractContainerAccess*> endPair = ContainerAPI::fromNativeId(endId);
    Q_ASSERT(!endPair.first || endPair.second->isSequentialConstIterator());
    if(beginPair.second->isAssociativeConstIterator() && associativeMapMode!=0){
        QTJAMBI_CONTAINER_CAST(AssociativeConstIterator, beginAccess, beginPair.second);
        if(associativeMapMode<0){
            const QMetaType& elementMetaType = beginAccess->keyMetaType();
            if(elementMetaType.id()==QMetaType::UnknownType)
                JavaException::raiseIllegalArgumentException(env, QStringLiteral("QMetaType::UnknownType cannot be type of %1.").arg("QList") QTJAMBI_STACKTRACEINFO );
            if(elementMetaType.id()==QMetaType::Void)
                JavaException::raiseIllegalArgumentException(env, QStringLiteral("void cannot be type of %1.").arg("QList") QTJAMBI_STACKTRACEINFO );
            const SuperTypeInfos superTypeInfos = SuperTypeInfos::fromClass(env, env->GetObjectClass(object));
            if(superTypeInfos.size()>1)
                JavaException::raiseError(env, QStringLiteral("It is not permitted to create a derived type of %1 implementing any Qt interface.").arg("QList") QTJAMBI_STACKTRACEINFO );
            {
                auto _containerAccess = createContainerAccess(SequentialContainerType::QList, elementMetaType);
                if(_containerAccess && _containerAccess->isList())
                    containerAccess = static_cast<AbstractListAccess*>(_containerAccess);
            }
            if(!containerAccess){
                jclass elementType = CoreAPI::getClassForMetaType(env, elementMetaType);
                elementType = getGlobalClassRef(env, elementType);
                QByteArray qTypeName = elementMetaType.name();
                size_t size = size_t(elementMetaType.sizeOf());
                bool isPointer = AbstractContainerAccess::isPointerType(elementMetaType);
                size_t align = size_t(elementMetaType.alignOf());
                QSharedPointer<AbstractContainerAccess> elementNestedContainerAccess = findContainerAccess(elementMetaType);
                QtJambiUtils::QHashFunction hashFunction = QtJambiTypeManager::findHashFunction(isPointer, elementMetaType);
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
                auto _containerAccess = createContainerAccess(
                    env, SequentialContainerType::QList,
                    elementMetaType,
                    align, size,
                    isPointer,
                    hashFunction,
                    internalToExternalConverter,
                    externalToInternalConverter,
                    elementNestedContainerAccess,
                    elementOwnerFunction);
                if(_containerAccess && _containerAccess->isList())
                    containerAccess = static_cast<AbstractListAccess*>(_containerAccess);
            }
            void* listPtr = containerAccess->createContainer();
            QByteArray name;
            name = "QList<";
            name += containerAccess->elementMetaType().name();
            name += ">";
            name = QMetaObject::normalizedType(name);
            QMetaType containerMetaType(containerAccess->registerContainer(name));
            Q_UNUSED(containerMetaType)
            QSharedPointer<QtJambiLink> link = QtJambiLink::createLinkForNativeObject(env, object, listPtr,
                                                                                      LINK_NAME_META_TYPE_ARG(containerMetaType)
                                                                                      true, true, containerAccess, QtJambiLink::Ownership::Java);
            if(Q_UNLIKELY(!link)) {
                containerAccess->deleteContainer(listPtr);
                containerAccess->dispose();
            }else{
                if(endPair.first){
                    std::optional<size_t> size = beginAccess->distance(beginPair.first, endPair.first);
                    if(size.has_value())
                        containerAccess->reserve(listPtr, size.value());
                    bool useJava = containerAccess->asRC();
                    bool equals = beginAccess->equals(beginPair.first, endPair.first);
                    if(!equals && !useJava){
                        std::optional<const void*> key = beginAccess->key(beginPair.first);
                        useJava = !key.has_value();
                    }
                    if(useJava){
                        ContainerInfo ci{object,listPtr};
                        qsizetype size = containerAccess->size(env, listPtr);
                        while(!equals){
                            jobject key = beginAccess->key(env, beginPair.first);
                            containerAccess->insert(env, ci, size, 1, key);
                            beginAccess->increment(beginPair.first);
                            equals = beginAccess->equals(beginPair.first, endPair.first);
                            ++size;
                        }
                    }else{
                        while(!equals){
                            std::optional<const void*> key = beginAccess->key(beginPair.first);
                            if(key.has_value()){
                                containerAccess->append(listPtr, key.value());
                            }
                            beginAccess->increment(beginPair.first);
                            equals = beginAccess->equals(beginPair.first, endPair.first);
                        }
                    }
                }else{
                    bool useJava = containerAccess->asRC();
                    std::optional<bool> isValid = beginAccess->isValid(beginPair.first);
                    if(isValid.has_value() && !isValid.value() && !useJava){
                        std::optional<const void*> key = beginAccess->key(beginPair.first);
                        useJava = !key.has_value();
                    }
                    if(useJava){
                        ContainerInfo ci{object,listPtr};
                        qsizetype size = containerAccess->size(env, listPtr);
                        while(isValid.has_value() && !isValid.value()){
                            jobject key = beginAccess->key(env, beginPair.first);
                            containerAccess->insert(env, ci, size, 1, key);
                            beginAccess->increment(beginPair.first);
                            isValid = beginAccess->isValid(beginPair.first);
                            ++size;
                        }
                    }else{
                        while(isValid.has_value() && !isValid.value()){
                            std::optional<const void*> key = beginAccess->key(beginPair.first);
                            if(key.has_value()){
                                containerAccess->append(listPtr, key.value());
                            }
                            beginAccess->increment(beginPair.first);
                            isValid = beginAccess->isValid(beginPair.first);
                        }
                    }
                }
            }
        }else{
            AbstractPairAccess* pairAccess = nullptr;
            const QMetaType& keyMetaType = beginAccess->keyMetaType();
            const QMetaType& valueMetaType = beginAccess->valueMetaType();
            if(keyMetaType.id()==QMetaType::UnknownType)
                JavaException::raiseIllegalArgumentException(env, QStringLiteral("QMetaType::UnknownType cannot be key type of %1.").arg("QPair") QTJAMBI_STACKTRACEINFO );
            if(keyMetaType.id()==QMetaType::Void)
                JavaException::raiseIllegalArgumentException(env, QStringLiteral("void cannot be key type of %1.").arg("QPair") QTJAMBI_STACKTRACEINFO );
            if(valueMetaType.id()==QMetaType::UnknownType)
                JavaException::raiseIllegalArgumentException(env, QStringLiteral("QMetaType::UnknownType cannot be value type of %1.").arg("QPair") QTJAMBI_STACKTRACEINFO );
            if(valueMetaType.id()==QMetaType::Void)
                JavaException::raiseIllegalArgumentException(env, QStringLiteral("void cannot be value type of %1.").arg("QPair") QTJAMBI_STACKTRACEINFO );
            if(!pairAccess){
                auto _pairAccess = createContainerAccess(AssociativeContainerType::QPair, keyMetaType, valueMetaType);
                if(_pairAccess && _pairAccess->isMap())
                    pairAccess = static_cast<AbstractPairAccess*>(_pairAccess);
            }
            if(!pairAccess){
                size_t size1 = size_t(keyMetaType.sizeOf());
                bool isPointer1 = AbstractContainerAccess::isPointerType(keyMetaType);
                size_t align1 = size_t(keyMetaType.alignOf());

                size_t size2 = size_t(valueMetaType.sizeOf());
                bool isPointer2 = AbstractContainerAccess::isPointerType(valueMetaType);
                size_t align2 = size_t(valueMetaType.alignOf());

                jclass keyType = CoreAPI::getClassForMetaType(env, keyMetaType);
                jclass valueType = CoreAPI::getClassForMetaType(env, valueMetaType);
                keyType = getGlobalClassRef(env, keyType);
                valueType = getGlobalClassRef(env, valueType);

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
                QtJambiUtils::QHashFunction hashFunction1 = QtJambiTypeManager::findHashFunction(isPointer1, keyMetaType);
                QtJambiUtils::QHashFunction hashFunction2 = QtJambiTypeManager::findHashFunction(isPointer2, valueMetaType);
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
                auto _pairAccess = createContainerAccess(
                    env, AssociativeContainerType::QPair,
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
                    valueOwnerFunction);
                if(_pairAccess && _pairAccess->isPair())
                    pairAccess = static_cast<AbstractPairAccess*>(_pairAccess);
            }
            QByteArray name = "QPair<";
            name += pairAccess->firstMetaType().name();
            name += ",";
            name += pairAccess->secondMetaType().name();
            name += ">";
            name = QMetaObject::normalizedType(name);
            QMetaType elementMetaType(pairAccess->registerContainer(name));
            Q_UNUSED(elementMetaType)

            if(elementMetaType.id()==QMetaType::UnknownType)
                JavaException::raiseIllegalArgumentException(env, QStringLiteral("QMetaType::UnknownType cannot be type of %1.").arg("QList") QTJAMBI_STACKTRACEINFO );
            if(elementMetaType.id()==QMetaType::Void)
                JavaException::raiseIllegalArgumentException(env, QStringLiteral("void cannot be type of %1.").arg("QList") QTJAMBI_STACKTRACEINFO );
            const SuperTypeInfos superTypeInfos = SuperTypeInfos::fromClass(env, env->GetObjectClass(object));
            if(superTypeInfos.size()>1)
                JavaException::raiseError(env, QStringLiteral("It is not permitted to create a derived type of %1 implementing any Qt interface.").arg("QList") QTJAMBI_STACKTRACEINFO );
            {
                auto _containerAccess = createContainerAccess(SequentialContainerType::QList, elementMetaType);
                if(_containerAccess && _containerAccess->isList())
                    containerAccess = static_cast<AbstractListAccess*>(_containerAccess);
            }
            if(!containerAccess){
                jclass elementType = CoreAPI::getClassForMetaType(env, elementMetaType);
                elementType = getGlobalClassRef(env, elementType);
                QByteArray qTypeName = elementMetaType.name();
                size_t size = size_t(elementMetaType.sizeOf());
                bool isPointer = AbstractContainerAccess::isPointerType(elementMetaType);
                size_t align = size_t(elementMetaType.alignOf());
                QSharedPointer<AbstractContainerAccess> elementNestedContainerAccess = findContainerAccess(elementMetaType);
                QtJambiUtils::QHashFunction hashFunction = QtJambiTypeManager::findHashFunction(isPointer, elementMetaType);
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
                auto _containerAccess = createContainerAccess(
                    env, SequentialContainerType::QList,
                    elementMetaType,
                    align, size,
                    isPointer,
                    hashFunction,
                    internalToExternalConverter,
                    externalToInternalConverter,
                    elementNestedContainerAccess,
                    elementOwnerFunction);
                if(_containerAccess && _containerAccess->isList())
                    containerAccess = static_cast<AbstractListAccess*>(_containerAccess);
            }
            void* listPtr = containerAccess->createContainer();
            name = "QList<";
            name += containerAccess->elementMetaType().name();
            name += ">";
            name = QMetaObject::normalizedType(name);
            QMetaType containerMetaType(containerAccess->registerContainer(name));
            Q_UNUSED(containerMetaType)
            QSharedPointer<QtJambiLink> link = QtJambiLink::createLinkForNativeObject(env, object, listPtr,
                                                                                      LINK_NAME_META_TYPE_ARG(containerMetaType)
                                                                                      true, true, containerAccess, QtJambiLink::Ownership::Java);
            if(Q_UNLIKELY(!link)) {
                containerAccess->deleteContainer(listPtr);
                containerAccess->dispose();
            }else{
                if(endPair.first){
                    bool useJava = containerAccess->asRC();
                    bool equals = beginAccess->equals(beginPair.first, endPair.first);
                    if(!equals && !useJava){
                        std::optional<const void*> value = beginAccess->value(beginPair.first);
                        useJava = !value.has_value();
                    }
                    if(useJava){
                        std::optional<size_t> size = beginAccess->distance(beginPair.first, endPair.first);
                        if(size.has_value())
                            containerAccess->reserve(listPtr, size.value());
                        ContainerInfo ci{object,listPtr};
                        qsizetype i = 0;
                        while(!equals){
                            jobject key = beginAccess->key(env, beginPair.first);
                            jobject value = beginAccess->value(env, beginPair.first);
                            containerAccess->insert(env, ci, i, 1, Java::QtCore::QPair::newInstance(env, key, value));
                            beginAccess->increment(beginPair.first);
                            equals = beginAccess->equals(beginPair.first, endPair.first);
                            ++i;
                        }
                    }else{
                        std::optional<size_t> size = beginAccess->distance(beginPair.first, endPair.first);
                        qsizetype i = 0;
                        if(size.has_value() && size.value()>0){
                            containerAccess->resize(listPtr, size.value());
                            while(!equals){
                                std::optional<const void*> key = beginAccess->key(beginPair.first);
                                std::optional<const void*> value = beginAccess->value(beginPair.first);
                                if(key.has_value() && value.has_value()){
                                    void* pair = containerAccess->at(listPtr, i);
                                    pairAccess->setFirst(pair, key.value());
                                    pairAccess->setSecond(pair, value.value());
                                }
                                beginAccess->increment(beginPair.first);
                                equals = beginAccess->equals(beginPair.first, endPair.first);
                                ++i;
                            }
                        }else{
                            while(!equals){
                                std::optional<const void*> key = beginAccess->key(beginPair.first);
                                std::optional<const void*> value = beginAccess->value(beginPair.first);
                                containerAccess->resize(listPtr, i+1);
                                if(key.has_value() && value.has_value()){
                                    void* pair = containerAccess->at(listPtr, i);
                                    pairAccess->setFirst(pair, key.value());
                                    pairAccess->setSecond(pair, value.value());
                                }
                                beginAccess->increment(beginPair.first);
                                equals = beginAccess->equals(beginPair.first, endPair.first);
                                ++i;
                            }
                        }
                    }
                }else{
                    bool useJava = containerAccess->asRC();
                    std::optional<bool> isValid = beginAccess->isValid(beginPair.first);
                    if(isValid.has_value() && !isValid.value() && !useJava){
                        std::optional<const void*> value = beginAccess->value(beginPair.first);
                        useJava = !value.has_value();
                    }
                    if(useJava){
                        ContainerInfo ci{object,listPtr};
                        qsizetype i = 0;
                        while(isValid.has_value() && !isValid.value()){
                            jobject key = beginAccess->key(env, beginPair.first);
                            jobject value = beginAccess->value(env, beginPair.first);
                            containerAccess->insert(env, ci, i, 1, Java::QtCore::QPair::newInstance(env, key, value));
                            beginAccess->increment(beginPair.first);
                            isValid = beginAccess->isValid(beginPair.first);
                            ++i;
                        }
                    }else{
                        qsizetype i = 0;
                        while(isValid.has_value() && !isValid.value()){
                            std::optional<const void*> key = beginAccess->key(beginPair.first);
                            std::optional<const void*> value = beginAccess->value(beginPair.first);
                            containerAccess->resize(listPtr, i+1);
                            if(key.has_value() && value.has_value()){
                                void* pair = containerAccess->at(listPtr, i);
                                pairAccess->setFirst(pair, key.value());
                                pairAccess->setSecond(pair, value.value());
                            }
                            beginAccess->increment(beginPair.first);
                            isValid = beginAccess->isValid(beginPair.first);
                            ++i;
                        }
                    }
                }
            }
        }
    }else{
        QTJAMBI_CONTAINER_CAST(SequentialConstIterator, beginAccess, beginPair.second);
        const QMetaType& elementMetaType = beginAccess->valueMetaType();
        if(elementMetaType.id()==QMetaType::UnknownType)
            JavaException::raiseIllegalArgumentException(env, QStringLiteral("QMetaType::UnknownType cannot be type of %1.").arg("QList") QTJAMBI_STACKTRACEINFO );
        if(elementMetaType.id()==QMetaType::Void)
            JavaException::raiseIllegalArgumentException(env, QStringLiteral("void cannot be type of %1.").arg("QList") QTJAMBI_STACKTRACEINFO );
        const SuperTypeInfos superTypeInfos = SuperTypeInfos::fromClass(env, env->GetObjectClass(object));
        if(superTypeInfos.size()>1)
            JavaException::raiseError(env, QStringLiteral("It is not permitted to create a derived type of %1 implementing any Qt interface.").arg("QList") QTJAMBI_STACKTRACEINFO );
        {
            auto _containerAccess = createContainerAccess(SequentialContainerType::QList, elementMetaType);
            if(_containerAccess && _containerAccess->isList())
                containerAccess = static_cast<AbstractListAccess*>(_containerAccess);
        }
        if(!containerAccess){
            jclass elementType = CoreAPI::getClassForMetaType(env, elementMetaType);
            elementType = getGlobalClassRef(env, elementType);
            QByteArray qTypeName = elementMetaType.name();
            size_t size = size_t(elementMetaType.sizeOf());
            bool isPointer = AbstractContainerAccess::isPointerType(elementMetaType);
            size_t align = size_t(elementMetaType.alignOf());
            QSharedPointer<AbstractContainerAccess> elementNestedContainerAccess = findContainerAccess(elementMetaType);
            QtJambiUtils::QHashFunction hashFunction = QtJambiTypeManager::findHashFunction(isPointer, elementMetaType);
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
            auto _containerAccess = createContainerAccess(
                env, SequentialContainerType::QList,
                elementMetaType,
                align, size,
                isPointer,
                hashFunction,
                internalToExternalConverter,
                externalToInternalConverter,
                elementNestedContainerAccess,
                elementOwnerFunction);
            if(_containerAccess && _containerAccess->isList())
                containerAccess = static_cast<AbstractListAccess*>(_containerAccess);
        }
        void* listPtr = containerAccess->createContainer();
        QByteArray name;
        name = "QList<";
        name += containerAccess->elementMetaType().name();
        name += ">";
        name = QMetaObject::normalizedType(name);
        QMetaType containerMetaType(containerAccess->registerContainer(name));
        Q_UNUSED(containerMetaType)
        QSharedPointer<QtJambiLink> link = QtJambiLink::createLinkForNativeObject(env, object, listPtr,
                                                                                  LINK_NAME_META_TYPE_ARG(containerMetaType)
                                                                                  true, true, containerAccess, QtJambiLink::Ownership::Java);
        if(Q_UNLIKELY(!link)) {
            containerAccess->deleteContainer(listPtr);
            containerAccess->dispose();
        }else{
            if(endPair.first){
                bool useJava = containerAccess->asRC();
                bool equals = beginAccess->equals(beginPair.first, endPair.first);
                if(!equals && !useJava){
                    std::optional<const void*> value = beginAccess->value(beginPair.first);
                    useJava = !value.has_value();
                }
                if(useJava){
                    ContainerInfo ci{object,listPtr};
                    std::optional<size_t> size = beginAccess->distance(beginPair.first, endPair.first);
                    if(size.has_value())
                        containerAccess->reserve(listPtr, size.value());
                    qsizetype i = 0;
                    while(!equals){
                        jobject value = beginAccess->value(env, beginPair.first);
                        containerAccess->insert(env, ci, i, 1, value);
                        beginAccess->increment(beginPair.first);
                        equals = beginAccess->equals(beginPair.first, endPair.first);
                        ++i;
                    }
                }else{
                    while(!equals){
                        std::optional<const void*> value = beginAccess->value(beginPair.first);
                        if(value.has_value()){
                            containerAccess->append(listPtr, value.value());
                        }
                        beginAccess->increment(beginPair.first);
                        equals = beginAccess->equals(beginPair.first, endPair.first);
                    }
                }
            }else{
                bool useJava = containerAccess->asRC();
                std::optional<bool> isValid = beginAccess->isValid(beginPair.first);
                if(isValid.has_value() && !isValid.value() && !useJava){
                    std::optional<const void*> value = beginAccess->value(beginPair.first);
                    useJava = !value.has_value();
                }
                if(useJava){
                    ContainerInfo ci{object,listPtr};
                    qsizetype i = 0;
                    while(isValid.has_value() && !isValid.value()){
                        jobject value = beginAccess->value(env, beginPair.first);
                        containerAccess->insert(env, ci, i, 1, value);
                        beginAccess->increment(beginPair.first);
                        isValid = beginAccess->isValid(beginPair.first);
                        ++i;
                    }
                }else{
                    while(isValid.has_value() && !isValid.value()){
                        std::optional<const void*> value = beginAccess->value(beginPair.first);
                        if(value.has_value()){
                            containerAccess->append(listPtr, value.value());
                        }
                        beginAccess->increment(beginPair.first);
                        isValid = beginAccess->isValid(beginPair.first);
                    }
                }
            }
        }
    }
}

void CoreAPI::initializeQSet(JNIEnv *env, jobject object, QtJambiNativeID beginId, QtJambiNativeID endId){
    using namespace QtJambiPrivate;
    AbstractSetAccess* containerAccess = nullptr;
    QPair<void*,AbstractContainerAccess*> beginPair = ContainerAPI::fromNativeId(beginId);
    QPair<void*,AbstractContainerAccess*> endPair = ContainerAPI::fromNativeId(endId);
    QTJAMBI_CONTAINER_CAST(SequentialConstIterator, beginAccess, beginPair.second);
    Q_ASSERT(!endPair.first || endPair.second->isSequentialConstIterator());
    const QMetaType& elementMetaType = beginAccess->valueMetaType();
    if(elementMetaType.id()==QMetaType::UnknownType)
        JavaException::raiseIllegalArgumentException(env, QStringLiteral("QMetaType::UnknownType cannot be type of %1.").arg("QSet") QTJAMBI_STACKTRACEINFO );
    if(elementMetaType.id()==QMetaType::Void)
        JavaException::raiseIllegalArgumentException(env, QStringLiteral("void cannot be type of %1.").arg("QSet") QTJAMBI_STACKTRACEINFO );
    const SuperTypeInfos superTypeInfos = SuperTypeInfos::fromClass(env, env->GetObjectClass(object));
    if(superTypeInfos.size()>1)
        JavaException::raiseError(env, QStringLiteral("It is not permitted to create a derived type of %1 implementing any Qt interface.").arg("QSet") QTJAMBI_STACKTRACEINFO );
    if(!containerAccess){
        {
            auto _containerAccess = createContainerAccess(SequentialContainerType::QSet, elementMetaType);
            if(_containerAccess && _containerAccess->isSet())
                containerAccess = static_cast<AbstractSetAccess*>(_containerAccess);
        }
        if(!containerAccess){
            jclass elementType = CoreAPI::getClassForMetaType(env, elementMetaType);
            elementType = getGlobalClassRef(env, elementType);
            QByteArray qTypeName = elementMetaType.name();
            size_t size = size_t(elementMetaType.sizeOf());
            bool isPointer = AbstractContainerAccess::isPointerType(elementMetaType);
            size_t align = size_t(elementMetaType.alignOf());
            QSharedPointer<AbstractContainerAccess> elementNestedContainerAccess = findContainerAccess(elementMetaType);
            QtJambiUtils::QHashFunction hashFunction = QtJambiTypeManager::findHashFunction(isPointer, elementMetaType);
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
            auto _containerAccess = createContainerAccess(
                env, SequentialContainerType::QSet,
                elementMetaType,
                align, size,
                isPointer,
                hashFunction,
                internalToExternalConverter,
                externalToInternalConverter,
                elementNestedContainerAccess,
                elementOwnerFunction);
            if(_containerAccess && _containerAccess->isSet())
                containerAccess = static_cast<AbstractSetAccess*>(_containerAccess);
        }
    }
    void* listPtr = containerAccess->createContainer();
    QByteArray name;
    name = "QSet<";
    name += containerAccess->elementMetaType().name();
    name += ">";
    name = QMetaObject::normalizedType(name);
    QMetaType containerMetaType(containerAccess->registerContainer(name));
    Q_UNUSED(containerMetaType)
    QSharedPointer<QtJambiLink> link = QtJambiLink::createLinkForNativeObject(env, object, listPtr,
                                                                              LINK_NAME_META_TYPE_ARG(containerMetaType)
                                                                              true, true, containerAccess, QtJambiLink::Ownership::Java);
    if(Q_UNLIKELY(!link)) {
        containerAccess->deleteContainer(listPtr);
        containerAccess->dispose();
    }else{
        if(endPair.first){
            std::optional<size_t> size = beginAccess->distance(beginPair.first, endPair.first);
            if(size.has_value())
                containerAccess->reserve(listPtr, size.value());
            bool useJava = containerAccess->asRC();
            bool equals = beginAccess->equals(beginPair.first, endPair.first);
            if(!equals && !useJava){
                std::optional<const void*> value = beginAccess->value(beginPair.first);
                useJava = !value.has_value();
            }
            if(useJava){
                ContainerInfo ci{object,listPtr};
                while(!equals){
                    jobject value = beginAccess->value(env, beginPair.first);
                    containerAccess->insert(env, ci, value);
                    beginAccess->increment(beginPair.first);
                    equals = beginAccess->equals(beginPair.first, endPair.first);
                }
            }else{
                while(!equals){
                    std::optional<const void*> value = beginAccess->value(beginPair.first);
                    if(value.has_value()){
                        containerAccess->insert(listPtr, value.value());
                    }
                    beginAccess->increment(beginPair.first);
                    equals = beginAccess->equals(beginPair.first, endPair.first);
                }
            }
        }else{
            bool useJava = containerAccess->asRC();
            std::optional<bool> isValid = beginAccess->isValid(beginPair.first);
            if(isValid.has_value() && !isValid.value() && !useJava){
                std::optional<const void*> value = beginAccess->value(beginPair.first);
                useJava = !value.has_value();
            }
            if(useJava){
                ContainerInfo ci{object,listPtr};
                while(isValid.has_value() && !isValid.value()){
                    jobject value = beginAccess->value(env, beginPair.first);
                    containerAccess->insert(env, ci, value);
                    beginAccess->increment(beginPair.first);
                    isValid = beginAccess->isValid(beginPair.first);
                }
            }else{
                while(isValid.has_value() && !isValid.value()){
                    std::optional<const void*> value = beginAccess->value(beginPair.first);
                    if(value.has_value()){
                        containerAccess->insert(listPtr, value.value());
                    }
                    beginAccess->increment(beginPair.first);
                    isValid = beginAccess->isValid(beginPair.first);
                }
            }
        }
    }
}

void CoreAPI::initializeQList(JNIEnv *env, jobject object, jclass elementType, QtJambiNativeID elementMetaTypeId, jobject other){
    using namespace QtJambiPrivate;
    AbstractListAccess* containerAccess = nullptr;
    bool isNativeContainer = false;
    if(Java::QtCore::QList::isInstanceOf(env, other)){
        if(QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForJavaObject(env, other)){
            if(link->containerAccess() && link->containerAccess()->isList())
                containerAccess = static_cast<AbstractListAccess*>(link->containerAccess());
            if(containerAccess){
                containerAccess = containerAccess->clone();
                isNativeContainer = true;
            }
        }else{
            JavaException::raise<Java::QtJambi::QNoNativeResourcesException>(env, QStringLiteral("Incomplete object of type: %1").arg(QtJambiAPI::getObjectClassNamePrintable(env, other)) QTJAMBI_STACKTRACEINFO );
        }
    }
    if(!containerAccess || elementMetaTypeId!=InvalidNativeID){
        const QMetaType& elementMetaType = ::qtjambi_cast<const QMetaType&>(elementMetaTypeId);
        if(elementMetaType.id()==QMetaType::UnknownType)
            JavaException::raiseIllegalArgumentException(env, QStringLiteral("QMetaType::UnknownType cannot be type of %1.").arg("QList") QTJAMBI_STACKTRACEINFO );
        if(elementMetaType.id()==QMetaType::Void)
            JavaException::raiseIllegalArgumentException(env, QStringLiteral("void cannot be type of %1.").arg("QList") QTJAMBI_STACKTRACEINFO );
        const SuperTypeInfos superTypeInfos = SuperTypeInfos::fromClass(env, env->GetObjectClass(object));
        if(superTypeInfos.size()>1)
            JavaException::raiseError(env, QStringLiteral("It is not permitted to create a derived type of %1 implementing any Qt interface.").arg("QList") QTJAMBI_STACKTRACEINFO );
        if(!containerAccess){
            {
                auto _containerAccess = createContainerAccess(SequentialContainerType::QList, elementMetaType);
                if(_containerAccess && _containerAccess->isList())
                    containerAccess = static_cast<AbstractListAccess*>(_containerAccess);
            }
            if(!containerAccess){
                if(!elementType)
                    elementType = CoreAPI::getClassForMetaType(env, elementMetaType);
                elementType = getGlobalClassRef(env, elementType);
                QByteArray qTypeName = elementMetaType.name();
                size_t size = size_t(elementMetaType.sizeOf());
                bool isPointer = AbstractContainerAccess::isPointerType(elementMetaType);
                size_t align = size_t(elementMetaType.alignOf());
                QSharedPointer<AbstractContainerAccess> elementNestedContainerAccess = findContainerAccess(elementMetaType);
                QtJambiUtils::QHashFunction hashFunction = QtJambiTypeManager::findHashFunction(isPointer, elementMetaType);
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
                auto _containerAccess = createContainerAccess(
                    env, SequentialContainerType::QList,
                    elementMetaType,
                    align, size,
                    isPointer,
                    hashFunction,
                    internalToExternalConverter,
                    externalToInternalConverter,
                    elementNestedContainerAccess,
                    elementOwnerFunction);
                if(_containerAccess && _containerAccess->isList())
                    containerAccess = static_cast<AbstractListAccess*>(_containerAccess);
            }
            isNativeContainer = Java::Runtime::Collection::isInstanceOf(env, other) && ContainerAPI::testQList(env, other, elementMetaType);
        }
    }
    void* listPtr;
    if(isNativeContainer){
        if(QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForJavaObject(env, other)){
            listPtr = containerAccess->createContainer(env, ConstContainerAndAccessInfo{link->getJavaObjectLocalRef(env), link->pointer(), link->containerAccess()});
        }else{
            if(Java::QtJambi::QtObjectInterface::isInstanceOf(env, other)){
                containerAccess->dispose();
                JavaException::raise<Java::QtJambi::QNoNativeResourcesException>(env, QStringLiteral("Incomplete object of type: %1").arg(QtJambiAPI::getObjectClassNamePrintable(env, other)) QTJAMBI_STACKTRACEINFO );
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
    Q_UNUSED(containerMetaType)
    QSharedPointer<QtJambiLink> link = QtJambiLink::createLinkForNativeObject(env, object, listPtr,
                                                                                   LINK_NAME_META_TYPE_ARG(containerMetaType)
                                                                                   true, true, containerAccess, QtJambiLink::Ownership::Java);
    if(Q_UNLIKELY(!link)) {
        containerAccess->deleteContainer(listPtr);
        containerAccess->dispose();
    }else if(!isNativeContainer && other){
        jobject iter = QtJambiAPI::iteratorOfJavaIterable(env, other);
        jint idx = 0;
        if(Java::Runtime::Collection::isInstanceOf(env, other))
            containerAccess->reserve(env, {object, listPtr}, QtJambiAPI::sizeOfJavaCollection(env, other));
        while(QtJambiAPI::hasJavaIteratorNext(env, iter)){
            containerAccess->insert(env, {object, listPtr}, idx++, 1, QtJambiAPI::nextOfJavaIterator(env, iter));
        }
    }
}

void CoreAPI::initializeQSet(JNIEnv *env, jobject object, jclass elementType, QtJambiNativeID elementMetaTypeId, jobject other){
    using namespace QtJambiPrivate;
    AbstractSetAccess* containerAccess = nullptr;
    bool isNativeContainer = false;
    if(Java::QtCore::QSet::isInstanceOf(env, other)){
        if(QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForJavaObject(env, other)){
            if(link->containerAccess() && link->containerAccess()->isSet())
                containerAccess = static_cast<AbstractSetAccess*>(link->containerAccess());
            if(containerAccess){
                containerAccess = containerAccess->clone();
                isNativeContainer = true;
            }
        }else{
            JavaException::raise<Java::QtJambi::QNoNativeResourcesException>(env, QStringLiteral("Incomplete object of type: %1").arg(QtJambiAPI::getObjectClassNamePrintable(env, other)) QTJAMBI_STACKTRACEINFO );
        }
    }
    if(!containerAccess || elementMetaTypeId!=InvalidNativeID){
        const QMetaType& elementMetaType = ::qtjambi_cast<const QMetaType&>(elementMetaTypeId);
        if(elementMetaType.id()==QMetaType::UnknownType)
            JavaException::raiseIllegalArgumentException(env, QStringLiteral("QMetaType::UnknownType cannot be type of %1.").arg("QSet") QTJAMBI_STACKTRACEINFO );
        if(elementMetaType.id()==QMetaType::QVariant)
            JavaException::raiseIllegalArgumentException(env, QStringLiteral("QVariant cannot be type of %1.").arg("QSet") QTJAMBI_STACKTRACEINFO );
        if(elementMetaType.id()==QMetaType::Void)
            JavaException::raiseIllegalArgumentException(env, QStringLiteral("void cannot be type of %1.").arg("QSet") QTJAMBI_STACKTRACEINFO );
        const SuperTypeInfos superTypeInfos = SuperTypeInfos::fromClass(env, env->GetObjectClass(object));
        if(superTypeInfos.size()>1)
            JavaException::raiseError(env, QStringLiteral("It is not permitted to create a derived type of %1 implementing any Qt interface.").arg("QSet") QTJAMBI_STACKTRACEINFO );
        if(!containerAccess){
            {
                auto _containerAccess = createContainerAccess(SequentialContainerType::QSet, elementMetaType);
                if(_containerAccess && _containerAccess->isSet())
                    containerAccess = static_cast<AbstractSetAccess*>(_containerAccess);
            }
            if(!containerAccess){
                if(!elementType)
                    elementType = CoreAPI::getClassForMetaType(env, elementMetaType);
                elementType = getGlobalClassRef(env, elementType);
                QByteArray qTypeName = elementMetaType.name();
                size_t size = size_t(elementMetaType.sizeOf());
                QSharedPointer<AbstractContainerAccess> elementNestedContainerAccess = findContainerAccess(elementMetaType);
                bool isPointer = AbstractContainerAccess::isPointerType(elementMetaType);
                size_t align = size_t(elementMetaType.alignOf());
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
                QtJambiUtils::QHashFunction hashFunction = QtJambiTypeManager::findHashFunction(isPointer, elementMetaType);
                if(!hashFunction){
                    JavaException::raiseQNoImplementationException(env, QString("Unable to create QSet of %1 because of missing hash function.").arg(QtJambiAPI::getClassNamePrintable(env, elementType)) QTJAMBI_STACKTRACEINFO );
                }
                const std::type_info* typeId = getTypeByQtName(elementMetaType.name());
                if(!typeId){
                    typeId = getTypeByMetaType(elementMetaType);
                }
                PtrOwnerFunction elementOwnerFunction = nullptr;
                if(typeId)
                    elementOwnerFunction = ContainerAPI::registeredOwnerFunction(*typeId);
                auto _containerAccess = createContainerAccess(
                                                                       env, SequentialContainerType::QSet,
                                                                       elementMetaType,
                                                                       align, size,
                                                                       isPointer,
                                                                       hashFunction,
                                                                       internalToExternalConverter,
                                                                       externalToInternalConverter,
                                                                       elementNestedContainerAccess,
                                                                       elementOwnerFunction);
                if(_containerAccess && _containerAccess->isSet())
                    containerAccess = static_cast<AbstractSetAccess*>(_containerAccess);
            }
            isNativeContainer = Java::Runtime::Collection::isInstanceOf(env, other) && ContainerAPI::testQSet(env, other, elementMetaType);
        }
    }
    void* listPtr;
    if(isNativeContainer){
        if(QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForJavaObject(env, other)){
            listPtr = containerAccess->createContainer(env, ConstContainerAndAccessInfo{link->getJavaObjectLocalRef(env), link->pointer(), link->containerAccess()});
        }else{
            if(Java::QtJambi::QtObjectInterface::isInstanceOf(env, other)){
                containerAccess->dispose();
                JavaException::raise<Java::QtJambi::QNoNativeResourcesException>(env, QStringLiteral("Incomplete object of type: %1").arg(QtJambiAPI::getObjectClassNamePrintable(env, other)) QTJAMBI_STACKTRACEINFO );
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
    Q_UNUSED(containerMetaType)
    QSharedPointer<QtJambiLink> link = QtJambiLink::createLinkForNativeObject(env, object, listPtr,
                                                                                   LINK_NAME_META_TYPE_ARG(containerMetaType)
                                                                                   true, true, containerAccess, QtJambiLink::Ownership::Java);
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

void CoreAPI::initializeQHash(JNIEnv *env, jobject object, jclass keyType, QtJambiNativeID keyMetaTypeId, jclass valueType, QtJambiNativeID valueMetaTypeId, jobject other){
    using namespace QtJambiPrivate;
    AbstractHashAccess* containerAccess = nullptr;
    bool isNativeContainer = false;
    if(Java::QtCore::QHash::isInstanceOf(env, other)){
        if(QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForJavaObject(env, other)){
            if(link->containerAccess() && link->containerAccess()->isHash())
                containerAccess = static_cast<AbstractHashAccess*>(link->containerAccess());
            if(containerAccess){
                containerAccess = containerAccess->clone();
                isNativeContainer = true;
            }
        }else{
            JavaException::raise<Java::QtJambi::QNoNativeResourcesException>(env, QStringLiteral("Incomplete object of type: %1").arg(QtJambiAPI::getObjectClassNamePrintable(env, other)) QTJAMBI_STACKTRACEINFO );
        }
    }
    if(!containerAccess || keyMetaTypeId!=InvalidNativeID || valueMetaTypeId!=InvalidNativeID){
        const QMetaType& keyMetaType = ::qtjambi_cast<const QMetaType&>(keyMetaTypeId);
        const QMetaType& valueMetaType = ::qtjambi_cast<const QMetaType&>(valueMetaTypeId);
        if(keyMetaType.id()==QMetaType::UnknownType)
            JavaException::raiseIllegalArgumentException(env, QStringLiteral("QMetaType::UnknownType cannot be key type of %1.").arg("QHash") QTJAMBI_STACKTRACEINFO );
        if(keyMetaType.id()==QMetaType::Void)
            JavaException::raiseIllegalArgumentException(env, QStringLiteral("void cannot be key type of %1.").arg("QHash") QTJAMBI_STACKTRACEINFO );
        if(valueMetaType.id()==QMetaType::UnknownType)
            JavaException::raiseIllegalArgumentException(env, QStringLiteral("QMetaType::UnknownType cannot be value type of %1.").arg("QHash") QTJAMBI_STACKTRACEINFO );
        if(valueMetaType.id()==QMetaType::Void)
            JavaException::raiseIllegalArgumentException(env, QStringLiteral("void cannot be value type of %1.").arg("QHash") QTJAMBI_STACKTRACEINFO );
        const SuperTypeInfos superTypeInfos = SuperTypeInfos::fromClass(env, env->GetObjectClass(object));
        if(superTypeInfos.size()>1)
            JavaException::raiseError(env, QStringLiteral("It is not permitted to create a derived type of %1 implementing any Qt interface.").arg("QHash") QTJAMBI_STACKTRACEINFO );
        if(!containerAccess){
            auto _containerAccess = createContainerAccess(AssociativeContainerType::QHash, keyMetaType, valueMetaType);
            if(_containerAccess && _containerAccess->isHash())
                containerAccess = static_cast<AbstractHashAccess*>(_containerAccess);
        }
        if(!containerAccess){
            size_t size1 = size_t(keyMetaType.sizeOf());
            bool isPointer1 = AbstractContainerAccess::isPointerType(keyMetaType);
            size_t align1 = size_t(keyMetaType.alignOf());

            size_t size2 = size_t(valueMetaType.sizeOf());
            bool isPointer2 = AbstractContainerAccess::isPointerType(valueMetaType);
            size_t align2 = size_t(valueMetaType.alignOf());

            if(!keyType)
                keyType = CoreAPI::getClassForMetaType(env, keyMetaType);
            keyType = getGlobalClassRef(env, keyType);
            if(!valueType)
                valueType = CoreAPI::getClassForMetaType(env, valueMetaType);
            valueType = getGlobalClassRef(env, valueType);

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
            QtJambiUtils::QHashFunction hashFunction1 = QtJambiTypeManager::findHashFunction(isPointer1, keyMetaType);
            QtJambiUtils::QHashFunction hashFunction2 = QtJambiTypeManager::findHashFunction(isPointer2, valueMetaType);
            if(!hashFunction1){
                JavaException::raiseQNoImplementationException(env, QString("Unable to create QHash for %1 because of missing hash function.").arg(QtJambiAPI::getClassNamePrintable(env, keyType)) QTJAMBI_STACKTRACEINFO );
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
            auto _containerAccess = createContainerAccess(
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
                                                                        valueOwnerFunction);
            if(_containerAccess && _containerAccess->isHash())
                containerAccess = static_cast<AbstractHashAccess*>(_containerAccess);
            isNativeContainer = Java::Runtime::Map::isInstanceOf(env, other) && ContainerAPI::testQHash(env, other, keyMetaType, valueMetaType);
        }
    }
    void* listPtr;
    if(isNativeContainer){
        if(QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForJavaObject(env, other)){
            listPtr = containerAccess->createContainer(env, ConstContainerAndAccessInfo{link->getJavaObjectLocalRef(env), link->pointer(), link->containerAccess()});
        }else{
            if(Java::QtJambi::QtObjectInterface::isInstanceOf(env, other)){
                containerAccess->dispose();
                JavaException::raise<Java::QtJambi::QNoNativeResourcesException>(env, QStringLiteral("Incomplete object of type: %1").arg(QtJambiAPI::getObjectClassNamePrintable(env, other)) QTJAMBI_STACKTRACEINFO );
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
    Q_UNUSED(containerMetaType)
    QSharedPointer<QtJambiLink> link = QtJambiLink::createLinkForNativeObject(env, object, listPtr,
                                                                                   LINK_NAME_META_TYPE_ARG(containerMetaType)
                                                                                   true, true, containerAccess, QtJambiLink::Ownership::Java);
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

void CoreAPI::initializeQHash(JNIEnv *env, jobject object, QtJambiNativeID beginId, QtJambiNativeID endId){
    using namespace QtJambiPrivate;
    AbstractHashAccess* containerAccess = nullptr;
    QPair<void*,AbstractContainerAccess*> beginPair = ContainerAPI::fromNativeId(beginId);
    QPair<void*,AbstractContainerAccess*> endPair = ContainerAPI::fromNativeId(endId);
    QTJAMBI_CONTAINER_CAST(AssociativeConstIterator, beginAccess, beginPair.second);
    Q_ASSERT(!endPair.first || endPair.second->isAssociativeConstIterator());
    const QMetaType& keyMetaType = beginAccess->keyMetaType();
    const QMetaType& valueMetaType = beginAccess->valueMetaType();
    if(keyMetaType.id()==QMetaType::UnknownType)
        JavaException::raiseIllegalArgumentException(env, QStringLiteral("QMetaType::UnknownType cannot be key type of %1.").arg("QHash") QTJAMBI_STACKTRACEINFO );
    if(keyMetaType.id()==QMetaType::Void)
        JavaException::raiseIllegalArgumentException(env, QStringLiteral("void cannot be key type of %1.").arg("QHash") QTJAMBI_STACKTRACEINFO );
    if(valueMetaType.id()==QMetaType::UnknownType)
        JavaException::raiseIllegalArgumentException(env, QStringLiteral("QMetaType::UnknownType cannot be value type of %1.").arg("QHash") QTJAMBI_STACKTRACEINFO );
    if(valueMetaType.id()==QMetaType::Void)
        JavaException::raiseIllegalArgumentException(env, QStringLiteral("void cannot be value type of %1.").arg("QHash") QTJAMBI_STACKTRACEINFO );
    const SuperTypeInfos superTypeInfos = SuperTypeInfos::fromClass(env, env->GetObjectClass(object));
    if(superTypeInfos.size()>1)
        JavaException::raiseError(env, QStringLiteral("It is not permitted to create a derived type of %1 implementing any Qt interface.").arg("QHash") QTJAMBI_STACKTRACEINFO );
    if(!containerAccess){
        auto _containerAccess = createContainerAccess(AssociativeContainerType::QHash, keyMetaType, valueMetaType);
        if(_containerAccess && _containerAccess->isHash())
            containerAccess = static_cast<AbstractHashAccess*>(_containerAccess);
    }
    if(!containerAccess){
        size_t size1 = size_t(keyMetaType.sizeOf());
        bool isPointer1 = AbstractContainerAccess::isPointerType(keyMetaType);
        size_t align1 = size_t(keyMetaType.alignOf());

        size_t size2 = size_t(valueMetaType.sizeOf());
        bool isPointer2 = AbstractContainerAccess::isPointerType(valueMetaType);
        size_t align2 = size_t(valueMetaType.alignOf());

        jclass keyType = CoreAPI::getClassForMetaType(env, keyMetaType);
        jclass valueType = CoreAPI::getClassForMetaType(env, valueMetaType);
        keyType = getGlobalClassRef(env, keyType);
        valueType = getGlobalClassRef(env, valueType);

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
        QtJambiUtils::QHashFunction hashFunction1 = QtJambiTypeManager::findHashFunction(isPointer1, keyMetaType);
        QtJambiUtils::QHashFunction hashFunction2 = QtJambiTypeManager::findHashFunction(isPointer2, valueMetaType);
        if(!hashFunction1){
            JavaException::raiseQNoImplementationException(env, QString("Unable to create QHash for %1 because of missing hash function.").arg(QtJambiAPI::getClassNamePrintable(env, keyType)) QTJAMBI_STACKTRACEINFO );
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
        auto _containerAccess = createContainerAccess(
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
            valueOwnerFunction);
        if(_containerAccess && _containerAccess->isHash())
            containerAccess = static_cast<AbstractHashAccess*>(_containerAccess);
    }
    void* listPtr = containerAccess->createContainer();
    QByteArray name = "QHash<";
    name += containerAccess->keyMetaType().name();
    name += ",";
    name += containerAccess->valueMetaType().name();
    name += ">";
    name = QMetaObject::normalizedType(name);
    QMetaType containerMetaType(containerAccess->registerContainer(name));
    Q_UNUSED(containerMetaType)
    QSharedPointer<QtJambiLink> link = QtJambiLink::createLinkForNativeObject(env, object, listPtr,
                                                                              LINK_NAME_META_TYPE_ARG(containerMetaType)
                                                                              true, true, containerAccess, QtJambiLink::Ownership::Java);
    if(Q_UNLIKELY(!link)) {
        containerAccess->deleteContainer(listPtr);
        containerAccess->dispose();
    }else{
        if(endPair.first){
            std::optional<size_t> size = beginAccess->distance(beginPair.first, endPair.first);
            if(size.has_value())
                containerAccess->reserve(listPtr, size.value());
            bool useJava = containerAccess->asRC();
            bool equals = beginAccess->equals(beginPair.first, endPair.first);
            if(!equals && !useJava){
                std::optional<const void*> key = beginAccess->key(beginPair.first);
                std::optional<const void*> value = beginAccess->value(beginPair.first);
                useJava = !key.has_value() || !value.has_value();
            }
            if(useJava){
                ContainerInfo ci{object,listPtr};
                while(!equals){
                    jobject key = beginAccess->key(env, beginPair.first);
                    jobject value = beginAccess->value(env, beginPair.first);
                    containerAccess->insert(env, ci, key, value);
                    beginAccess->increment(beginPair.first);
                    equals = beginAccess->equals(beginPair.first, endPair.first);
                }
            }else{
                while(!equals){
                    std::optional<const void*> key = beginAccess->key(beginPair.first);
                    std::optional<const void*> value = beginAccess->value(beginPair.first);
                    if(key.has_value() && value.has_value()){
                        containerAccess->insert(listPtr, key.value(), value.value());
                    }
                    beginAccess->increment(beginPair.first);
                    equals = beginAccess->equals(beginPair.first, endPair.first);
                }
            }
        }else{
            bool useJava = containerAccess->asRC();
            std::optional<bool> isValid = beginAccess->isValid(beginPair.first);
            if(isValid.has_value() && !isValid.value() && !useJava){
                std::optional<const void*> key = beginAccess->key(beginPair.first);
                std::optional<const void*> value = beginAccess->value(beginPair.first);
                useJava = !key.has_value() || !value.has_value();
            }
            if(useJava){
                ContainerInfo ci{object,listPtr};
                while(isValid.has_value() && !isValid.value()){
                    jobject key = beginAccess->key(env, beginPair.first);
                    jobject value = beginAccess->value(env, beginPair.first);
                    containerAccess->insert(env, ci, key, value);
                    beginAccess->increment(beginPair.first);
                    isValid = beginAccess->isValid(beginPair.first);
                }
            }else{
                while(isValid.has_value() && !isValid.value()){
                    std::optional<const void*> key = beginAccess->key(beginPair.first);
                    std::optional<const void*> value = beginAccess->value(beginPair.first);
                    if(key.has_value() && value.has_value()){
                        containerAccess->insert(listPtr, key.value(), value.value());
                    }
                    beginAccess->increment(beginPair.first);
                    isValid = beginAccess->isValid(beginPair.first);
                }
            }
        }
    }
}

void CoreAPI::initializeQMultiHash(JNIEnv *env, jobject object, jclass keyType, QtJambiNativeID keyMetaTypeId, jclass valueType, QtJambiNativeID valueMetaTypeId, jobject other){
    using namespace QtJambiPrivate;
    bool isNativeContainer = false;
    AbstractMultiHashAccess* containerAccess = nullptr;
    if(Java::QtCore::QMultiHash::isInstanceOf(env, other)){
        if(QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForJavaObject(env, other)){
            if(link->containerAccess() && link->containerAccess()->isMultiHash())
                containerAccess = static_cast<AbstractMultiHashAccess*>(link->containerAccess());
            if(containerAccess){
                containerAccess = containerAccess->clone();
                isNativeContainer = true;
            }
        }else{
            JavaException::raise<Java::QtJambi::QNoNativeResourcesException>(env, QStringLiteral("Incomplete object of type: %1").arg(QtJambiAPI::getObjectClassNamePrintable(env, other)) QTJAMBI_STACKTRACEINFO );
        }
    }
    if(!containerAccess || keyMetaTypeId!=InvalidNativeID || valueMetaTypeId!=InvalidNativeID){
        const QMetaType& keyMetaType = ::qtjambi_cast<const QMetaType&>(keyMetaTypeId);
        const QMetaType& valueMetaType = ::qtjambi_cast<const QMetaType&>(valueMetaTypeId);
        if(keyMetaType.id()==QMetaType::UnknownType)
            JavaException::raiseIllegalArgumentException(env, QStringLiteral("QMetaType::UnknownType cannot be key type of %1.").arg("QMultiHash") QTJAMBI_STACKTRACEINFO );
        if(keyMetaType.id()==QMetaType::Void)
            JavaException::raiseIllegalArgumentException(env, QStringLiteral("void cannot be key type of %1.").arg("QMultiHash") QTJAMBI_STACKTRACEINFO );
        if(valueMetaType.id()==QMetaType::UnknownType)
            JavaException::raiseIllegalArgumentException(env, QStringLiteral("QMetaType::UnknownType cannot be value type of %1.").arg("QMultiHash") QTJAMBI_STACKTRACEINFO );
        if(valueMetaType.id()==QMetaType::Void)
            JavaException::raiseIllegalArgumentException(env, QStringLiteral("void cannot be value type of %1.").arg("QMultiHash") QTJAMBI_STACKTRACEINFO );
        const SuperTypeInfos superTypeInfos = SuperTypeInfos::fromClass(env, env->GetObjectClass(object));
        if(superTypeInfos.size()>1)
            JavaException::raiseError(env, QStringLiteral("It is not permitted to create a derived type of %1 implementing any Qt interface.").arg("QMultiHash") QTJAMBI_STACKTRACEINFO );
        if(!containerAccess){
            auto _containerAccess = createContainerAccess(AssociativeContainerType::QMultiHash, keyMetaType, valueMetaType);
            if(_containerAccess && _containerAccess->isMultiHash())
                containerAccess = static_cast<AbstractMultiHashAccess*>(_containerAccess);
        }
        if(!containerAccess){
            size_t size1 = size_t(keyMetaType.sizeOf());
            bool isPointer1 = AbstractContainerAccess::isPointerType(keyMetaType);
            size_t align1 = size_t(keyMetaType.alignOf());

            size_t size2 = size_t(valueMetaType.sizeOf());
            bool isPointer2 = AbstractContainerAccess::isPointerType(valueMetaType);
            size_t align2 = size_t(valueMetaType.alignOf());

            if(!keyType)
                keyType = CoreAPI::getClassForMetaType(env, keyMetaType);
            keyType = getGlobalClassRef(env, keyType);
            if(!valueType)
                valueType = CoreAPI::getClassForMetaType(env, valueMetaType);
            valueType = getGlobalClassRef(env, valueType);

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

            QtJambiUtils::QHashFunction hashFunction1 = QtJambiTypeManager::findHashFunction(isPointer1, keyMetaType);
            QtJambiUtils::QHashFunction hashFunction2 = QtJambiTypeManager::findHashFunction(isPointer2, valueMetaType);
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
            auto _containerAccess = createContainerAccess(
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
                                                                         valueOwnerFunction);
            if(_containerAccess && _containerAccess->isMultiHash())
                containerAccess = static_cast<AbstractMultiHashAccess*>(_containerAccess);
            isNativeContainer = Java::Runtime::Map::isInstanceOf(env, other) && ContainerAPI::testQMultiHash(env, other, keyMetaType, valueMetaType);
        }
    }

    void* listPtr;
    if(isNativeContainer){
        if(QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForJavaObject(env, other)){
            listPtr = containerAccess->createContainer(env, ConstContainerAndAccessInfo{link->getJavaObjectLocalRef(env), link->pointer(), link->containerAccess()});
        }else{
            if(Java::QtJambi::QtObjectInterface::isInstanceOf(env, other)){
                containerAccess->dispose();
                JavaException::raise<Java::QtJambi::QNoNativeResourcesException>(env, QStringLiteral("Incomplete object of type: %1").arg(QtJambiAPI::getObjectClassNamePrintable(env, other)) QTJAMBI_STACKTRACEINFO );
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
    Q_UNUSED(containerMetaType)
    QSharedPointer<QtJambiLink> link = QtJambiLink::createLinkForNativeObject(env, object, listPtr,
                                                                                   LINK_NAME_META_TYPE_ARG(containerMetaType)
                                                                                   true, true, containerAccess, QtJambiLink::Ownership::Java);
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

void CoreAPI::initializeQMultiHash(JNIEnv *env, jobject object, QtJambiNativeID beginId, QtJambiNativeID endId){
    using namespace QtJambiPrivate;
    AbstractMultiHashAccess* containerAccess = nullptr;
    QPair<void*,AbstractContainerAccess*> beginPair = ContainerAPI::fromNativeId(beginId);
    QPair<void*,AbstractContainerAccess*> endPair = ContainerAPI::fromNativeId(endId);
    QTJAMBI_CONTAINER_CAST(AssociativeConstIterator, beginAccess, beginPair.second);
    Q_ASSERT(!endPair.first || endPair.second->isAssociativeConstIterator());
    const QMetaType& keyMetaType = beginAccess->keyMetaType();
    const QMetaType& valueMetaType = beginAccess->valueMetaType();
    if(keyMetaType.id()==QMetaType::UnknownType)
        JavaException::raiseIllegalArgumentException(env, QStringLiteral("QMetaType::UnknownType cannot be key type of %1.").arg("QMultiHash") QTJAMBI_STACKTRACEINFO );
    if(keyMetaType.id()==QMetaType::Void)
        JavaException::raiseIllegalArgumentException(env, QStringLiteral("void cannot be key type of %1.").arg("QMultiHash") QTJAMBI_STACKTRACEINFO );
    if(valueMetaType.id()==QMetaType::UnknownType)
        JavaException::raiseIllegalArgumentException(env, QStringLiteral("QMetaType::UnknownType cannot be value type of %1.").arg("QMultiHash") QTJAMBI_STACKTRACEINFO );
    if(valueMetaType.id()==QMetaType::Void)
        JavaException::raiseIllegalArgumentException(env, QStringLiteral("void cannot be value type of %1.").arg("QMultiHash") QTJAMBI_STACKTRACEINFO );
    const SuperTypeInfos superTypeInfos = SuperTypeInfos::fromClass(env, env->GetObjectClass(object));
    if(superTypeInfos.size()>1)
        JavaException::raiseError(env, QStringLiteral("It is not permitted to create a derived type of %1 implementing any Qt interface.").arg("QMultiHash") QTJAMBI_STACKTRACEINFO );
    if(!containerAccess){
        auto _containerAccess = createContainerAccess(AssociativeContainerType::QMultiHash, keyMetaType, valueMetaType);
        if(_containerAccess && _containerAccess->isMultiHash())
            containerAccess = static_cast<AbstractMultiHashAccess*>(_containerAccess);
    }
    if(!containerAccess){
        size_t size1 = size_t(keyMetaType.sizeOf());
        bool isPointer1 = AbstractContainerAccess::isPointerType(keyMetaType);
        size_t align1 = size_t(keyMetaType.alignOf());

        size_t size2 = size_t(valueMetaType.sizeOf());
        bool isPointer2 = AbstractContainerAccess::isPointerType(valueMetaType);
        size_t align2 = size_t(valueMetaType.alignOf());

        jclass keyType = CoreAPI::getClassForMetaType(env, keyMetaType);
        jclass valueType = CoreAPI::getClassForMetaType(env, valueMetaType);
        keyType = getGlobalClassRef(env, keyType);
        valueType = getGlobalClassRef(env, valueType);

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
        QtJambiUtils::QHashFunction hashFunction1 = QtJambiTypeManager::findHashFunction(isPointer1, keyMetaType);
        QtJambiUtils::QHashFunction hashFunction2 = QtJambiTypeManager::findHashFunction(isPointer2, valueMetaType);
        if(!hashFunction1){
            JavaException::raiseQNoImplementationException(env, QString("Unable to create QMultiHash for %1 because of missing hash function.").arg(QtJambiAPI::getClassNamePrintable(env, keyType)) QTJAMBI_STACKTRACEINFO );
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
        auto _containerAccess = createContainerAccess(
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
            valueOwnerFunction);
        if(_containerAccess && _containerAccess->isMultiHash())
            containerAccess = static_cast<AbstractMultiHashAccess*>(_containerAccess);
    }
    void* listPtr = containerAccess->createContainer();
    QByteArray name = "QMultiHash<";
    name += containerAccess->keyMetaType().name();
    name += ",";
    name += containerAccess->valueMetaType().name();
    name += ">";
    name = QMetaObject::normalizedType(name);
    QMetaType containerMetaType(containerAccess->registerContainer(name));
    Q_UNUSED(containerMetaType)
    QSharedPointer<QtJambiLink> link = QtJambiLink::createLinkForNativeObject(env, object, listPtr,
                                                                              LINK_NAME_META_TYPE_ARG(containerMetaType)
                                                                              true, true, containerAccess, QtJambiLink::Ownership::Java);
    if(Q_UNLIKELY(!link)) {
        containerAccess->deleteContainer(listPtr);
        containerAccess->dispose();
    }else{
        if(endPair.first){
            std::optional<size_t> size = beginAccess->distance(beginPair.first, endPair.first);
            if(size.has_value())
                containerAccess->reserve(listPtr, size.value());
            bool useJava = containerAccess->asRC();
            bool equals = beginAccess->equals(beginPair.first, endPair.first);
            if(!equals && !useJava){
                std::optional<const void*> key = beginAccess->key(beginPair.first);
                std::optional<const void*> value = beginAccess->value(beginPair.first);
                useJava = !key.has_value() || !value.has_value();
            }
            if(useJava){
                ContainerInfo ci{object,listPtr};
                while(!equals){
                    jobject key = beginAccess->key(env, beginPair.first);
                    jobject value = beginAccess->value(env, beginPair.first);
                    containerAccess->insert(env, ci, key, value);
                    beginAccess->increment(beginPair.first);
                    equals = beginAccess->equals(beginPair.first, endPair.first);
                }
            }else{
                while(!equals){
                    std::optional<const void*> key = beginAccess->key(beginPair.first);
                    std::optional<const void*> value = beginAccess->value(beginPair.first);
                    if(key.has_value() && value.has_value()){
                        containerAccess->insert(listPtr, key.value(), value.value());
                    }
                    beginAccess->increment(beginPair.first);
                    equals = beginAccess->equals(beginPair.first, endPair.first);
                }
            }
        }else{
            bool useJava = containerAccess->asRC();
            std::optional<bool> isValid = beginAccess->isValid(beginPair.first);
            if(isValid.has_value() && !isValid.value() && !useJava){
                std::optional<const void*> key = beginAccess->key(beginPair.first);
                std::optional<const void*> value = beginAccess->value(beginPair.first);
                useJava = !key.has_value() || !value.has_value();
            }
            if(useJava){
                ContainerInfo ci{object,listPtr};
                while(isValid.has_value() && !isValid.value()){
                    jobject key = beginAccess->key(env, beginPair.first);
                    jobject value = beginAccess->value(env, beginPair.first);
                    containerAccess->insert(env, ci, key, value);
                    beginAccess->increment(beginPair.first);
                    isValid = beginAccess->isValid(beginPair.first);
                }
            }else{
                while(isValid.has_value() && !isValid.value()){
                    std::optional<const void*> key = beginAccess->key(beginPair.first);
                    std::optional<const void*> value = beginAccess->value(beginPair.first);
                    if(key.has_value() && value.has_value()){
                        containerAccess->insert(listPtr, key.value(), value.value());
                    }
                    beginAccess->increment(beginPair.first);
                    isValid = beginAccess->isValid(beginPair.first);
                }
            }
        }
    }
}

void CoreAPI::initializeQMap(JNIEnv *env, jobject object, jclass keyType, QtJambiNativeID keyMetaTypeId, jclass valueType, QtJambiNativeID valueMetaTypeId, jobject other){
    using namespace QtJambiPrivate;
    bool isNativeContainer = false;
    AbstractMapAccess* containerAccess = nullptr;
    if(Java::QtCore::QMap::isInstanceOf(env, other)){
        if(QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForJavaObject(env, other)){
            if(link->containerAccess() && link->containerAccess()->isMap())
                containerAccess = static_cast<AbstractMapAccess*>(link->containerAccess());
            if(containerAccess){
                containerAccess = containerAccess->clone();
                isNativeContainer = true;
            }
        }else{
            JavaException::raise<Java::QtJambi::QNoNativeResourcesException>(env, QStringLiteral("Incomplete object of type: %1").arg(QtJambiAPI::getObjectClassNamePrintable(env, other)) QTJAMBI_STACKTRACEINFO );
        }
    }
    if(!containerAccess || keyMetaTypeId!=InvalidNativeID || valueMetaTypeId!=InvalidNativeID){
        const QMetaType& keyMetaType = ::qtjambi_cast<const QMetaType&>(keyMetaTypeId);
        const QMetaType& valueMetaType = ::qtjambi_cast<const QMetaType&>(valueMetaTypeId);
        if(keyMetaType.id()==QMetaType::UnknownType)
            JavaException::raiseIllegalArgumentException(env, QStringLiteral("QMetaType::UnknownType cannot be key type of %1.").arg("QMap") QTJAMBI_STACKTRACEINFO );
        if(keyMetaType.id()==QMetaType::Void)
            JavaException::raiseIllegalArgumentException(env, QStringLiteral("void cannot be key type of %1.").arg("QMap") QTJAMBI_STACKTRACEINFO );
        if(valueMetaType.id()==QMetaType::UnknownType)
            JavaException::raiseIllegalArgumentException(env, QStringLiteral("QMetaType::UnknownType cannot be value type of %1.").arg("QMap") QTJAMBI_STACKTRACEINFO );
        if(valueMetaType.id()==QMetaType::Void)
            JavaException::raiseIllegalArgumentException(env, QStringLiteral("void cannot be value type of %1.").arg("QMap") QTJAMBI_STACKTRACEINFO );
        const SuperTypeInfos superTypeInfos = SuperTypeInfos::fromClass(env, env->GetObjectClass(object));
        if(superTypeInfos.size()>1)
            JavaException::raiseError(env, QStringLiteral("It is not permitted to create a derived type of %1 implementing any Qt interface.").arg("QMap") QTJAMBI_STACKTRACEINFO );
        if(!containerAccess){
            auto _containerAccess = createContainerAccess(AssociativeContainerType::QMap, keyMetaType, valueMetaType);
            if(_containerAccess && _containerAccess->isMap())
                containerAccess = static_cast<AbstractMapAccess*>(_containerAccess);
        }
        if(!containerAccess){
            size_t size1 = size_t(keyMetaType.sizeOf());
            bool isPointer1 = AbstractContainerAccess::isPointerType(keyMetaType);
            size_t align1 = size_t(keyMetaType.alignOf());

            size_t size2 = size_t(valueMetaType.sizeOf());
            bool isPointer2 = AbstractContainerAccess::isPointerType(valueMetaType);
            size_t align2 = size_t(valueMetaType.alignOf());

            if(!keyType)
                keyType = CoreAPI::getClassForMetaType(env, keyMetaType);
            keyType = getGlobalClassRef(env, keyType);
            if(!valueType)
                valueType = CoreAPI::getClassForMetaType(env, valueMetaType);
            valueType = getGlobalClassRef(env, valueType);

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

            QtJambiUtils::QHashFunction hashFunction1 = QtJambiTypeManager::findHashFunction(isPointer1, keyMetaType);
            QtJambiUtils::QHashFunction hashFunction2 = QtJambiTypeManager::findHashFunction(isPointer2, valueMetaType);
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
            auto _containerAccess = createContainerAccess(
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
                valueOwnerFunction);
            if(_containerAccess && _containerAccess->isMap())
                containerAccess = static_cast<AbstractMapAccess*>(_containerAccess);
            isNativeContainer = Java::Runtime::Map::isInstanceOf(env, other) && ContainerAPI::testQMap(env, other, keyMetaType, valueMetaType);
        }
    }
    void* listPtr;
    if(isNativeContainer){
        if(QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForJavaObject(env, other)){
            listPtr = containerAccess->createContainer(env, ConstContainerAndAccessInfo{link->getJavaObjectLocalRef(env), link->pointer(), link->containerAccess()});
        }else{
            if(Java::QtJambi::QtObjectInterface::isInstanceOf(env, other)){
                containerAccess->dispose();
                JavaException::raise<Java::QtJambi::QNoNativeResourcesException>(env, QStringLiteral("Incomplete object of type: %1").arg(QtJambiAPI::getObjectClassNamePrintable(env, other)) QTJAMBI_STACKTRACEINFO );
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
    Q_UNUSED(containerMetaType)
    QSharedPointer<QtJambiLink> link = QtJambiLink::createLinkForNativeObject(env, object, listPtr,
                                                                                   LINK_NAME_META_TYPE_ARG(containerMetaType)
                                                                                   true, true, containerAccess, QtJambiLink::Ownership::Java);
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

void CoreAPI::initializeQMap(JNIEnv *env, jobject object, QtJambiNativeID beginId, QtJambiNativeID endId){
    using namespace QtJambiPrivate;
    AbstractMapAccess* containerAccess = nullptr;
    QPair<void*,AbstractContainerAccess*> beginPair = ContainerAPI::fromNativeId(beginId);
    QPair<void*,AbstractContainerAccess*> endPair = ContainerAPI::fromNativeId(endId);
    QTJAMBI_CONTAINER_CAST(AssociativeConstIterator, beginAccess, beginPair.second);
    Q_ASSERT(!endPair.first || endPair.second->isAssociativeConstIterator());
    const QMetaType& keyMetaType = beginAccess->keyMetaType();
    const QMetaType& valueMetaType = beginAccess->valueMetaType();
    if(keyMetaType.id()==QMetaType::UnknownType)
        JavaException::raiseIllegalArgumentException(env, QStringLiteral("QMetaType::UnknownType cannot be key type of %1.").arg("QMap") QTJAMBI_STACKTRACEINFO );
    if(keyMetaType.id()==QMetaType::Void)
        JavaException::raiseIllegalArgumentException(env, QStringLiteral("void cannot be key type of %1.").arg("QMap") QTJAMBI_STACKTRACEINFO );
    if(valueMetaType.id()==QMetaType::UnknownType)
        JavaException::raiseIllegalArgumentException(env, QStringLiteral("QMetaType::UnknownType cannot be value type of %1.").arg("QMap") QTJAMBI_STACKTRACEINFO );
    if(valueMetaType.id()==QMetaType::Void)
        JavaException::raiseIllegalArgumentException(env, QStringLiteral("void cannot be value type of %1.").arg("QMap") QTJAMBI_STACKTRACEINFO );
    const SuperTypeInfos superTypeInfos = SuperTypeInfos::fromClass(env, env->GetObjectClass(object));
    if(superTypeInfos.size()>1)
        JavaException::raiseError(env, QStringLiteral("It is not permitted to create a derived type of %1 implementing any Qt interface.").arg("QMap") QTJAMBI_STACKTRACEINFO );
    if(!containerAccess){
        auto _containerAccess = createContainerAccess(AssociativeContainerType::QMap, keyMetaType, valueMetaType);
        if(_containerAccess && _containerAccess->isMap())
            containerAccess = static_cast<AbstractMapAccess*>(_containerAccess);
    }
    if(!containerAccess){
        size_t size1 = size_t(keyMetaType.sizeOf());
        bool isPointer1 = AbstractContainerAccess::isPointerType(keyMetaType);
        size_t align1 = size_t(keyMetaType.alignOf());

        size_t size2 = size_t(valueMetaType.sizeOf());
        bool isPointer2 = AbstractContainerAccess::isPointerType(valueMetaType);
        size_t align2 = size_t(valueMetaType.alignOf());

        jclass keyType = CoreAPI::getClassForMetaType(env, keyMetaType);
        jclass valueType = CoreAPI::getClassForMetaType(env, valueMetaType);
        keyType = getGlobalClassRef(env, keyType);
        valueType = getGlobalClassRef(env, valueType);

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
        QtJambiUtils::QHashFunction hashFunction1 = QtJambiTypeManager::findHashFunction(isPointer1, keyMetaType);
        QtJambiUtils::QHashFunction hashFunction2 = QtJambiTypeManager::findHashFunction(isPointer2, valueMetaType);
        if(!hashFunction1){
            JavaException::raiseQNoImplementationException(env, QString("Unable to create QMap for %1 because of missing hash function.").arg(QtJambiAPI::getClassNamePrintable(env, keyType)) QTJAMBI_STACKTRACEINFO );
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
        auto _containerAccess = createContainerAccess(
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
            valueOwnerFunction);
        if(_containerAccess && _containerAccess->isMap())
            containerAccess = static_cast<AbstractMapAccess*>(_containerAccess);
    }
    void* listPtr = containerAccess->createContainer();
    QByteArray name = "QMap<";
    name += containerAccess->keyMetaType().name();
    name += ",";
    name += containerAccess->valueMetaType().name();
    name += ">";
    name = QMetaObject::normalizedType(name);
    QMetaType containerMetaType(containerAccess->registerContainer(name));
    Q_UNUSED(containerMetaType)
    QSharedPointer<QtJambiLink> link = QtJambiLink::createLinkForNativeObject(env, object, listPtr,
                                                                              LINK_NAME_META_TYPE_ARG(containerMetaType)
                                                                              true, true, containerAccess, QtJambiLink::Ownership::Java);
    if(Q_UNLIKELY(!link)) {
        containerAccess->deleteContainer(listPtr);
        containerAccess->dispose();
    }else{
        bool useJava = containerAccess->asRC();
        if(endPair.first){
            bool equals = beginAccess->equals(beginPair.first, endPair.first);
            if(!equals && !useJava){
                std::optional<const void*> key = beginAccess->key(beginPair.first);
                std::optional<const void*> value = beginAccess->value(beginPair.first);
                useJava = !key.has_value() || !value.has_value();
            }
            if(useJava){
                ContainerInfo ci{object,listPtr};
                while(!equals){
                    jobject key = beginAccess->key(env, beginPair.first);
                    jobject value = beginAccess->value(env, beginPair.first);
                    containerAccess->insert(env, ci, key, value);
                    beginAccess->increment(beginPair.first);
                    equals = beginAccess->equals(beginPair.first, endPair.first);
                }
            }else{
                while(!equals){
                    std::optional<const void*> key = beginAccess->key(beginPair.first);
                    std::optional<const void*> value = beginAccess->value(beginPair.first);
                    if(key.has_value() && value.has_value()){
                        containerAccess->insert(listPtr, key.value(), value.value());
                    }
                    beginAccess->increment(beginPair.first);
                    equals = beginAccess->equals(beginPair.first, endPair.first);
                }
            }
        }else{
            std::optional<bool> isValid = beginAccess->isValid(beginPair.first);
            if(isValid.has_value() && !isValid.value() && !useJava){
                std::optional<const void*> key = beginAccess->key(beginPair.first);
                std::optional<const void*> value = beginAccess->value(beginPair.first);
                useJava = !key.has_value() || !value.has_value();
            }
            if(useJava){
                ContainerInfo ci{object,listPtr};
                while(isValid.has_value() && !isValid.value()){
                    jobject key = beginAccess->key(env, beginPair.first);
                    jobject value = beginAccess->value(env, beginPair.first);
                    containerAccess->insert(env, ci, key, value);
                    beginAccess->increment(beginPair.first);
                    isValid = beginAccess->isValid(beginPair.first);
                }
            }else{
                while(isValid.has_value() && !isValid.value()){
                    std::optional<const void*> key = beginAccess->key(beginPair.first);
                    std::optional<const void*> value = beginAccess->value(beginPair.first);
                    if(key.has_value() && value.has_value()){
                        containerAccess->insert(listPtr, key.value(), value.value());
                    }
                    beginAccess->increment(beginPair.first);
                    isValid = beginAccess->isValid(beginPair.first);
                }
            }
        }
    }
}

void CoreAPI::initializeQMultiMap(JNIEnv *env, jobject object, jclass keyType, QtJambiNativeID keyMetaTypeId, jclass valueType, QtJambiNativeID valueMetaTypeId, jobject other){
    using namespace QtJambiPrivate;
    bool isNativeContainer = false;
    AbstractMultiMapAccess* containerAccess = nullptr;
    if(Java::QtCore::QMultiMap::isInstanceOf(env, other)){
        if(QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForJavaObject(env, other)){
            if(link->containerAccess() && link->containerAccess()->isMultiMap()){
                containerAccess = static_cast<AbstractMultiMapAccess*>(link->containerAccess())->clone();
                isNativeContainer = true;
            }
        }else{
            JavaException::raise<Java::QtJambi::QNoNativeResourcesException>(env, QStringLiteral("Incomplete object of type: %1").arg(QtJambiAPI::getObjectClassNamePrintable(env, other)) QTJAMBI_STACKTRACEINFO );
        }
    }
    if(!containerAccess || keyMetaTypeId!=InvalidNativeID || valueMetaTypeId!=InvalidNativeID){
        const QMetaType& keyMetaType = ::qtjambi_cast<const QMetaType&>(keyMetaTypeId);
        const QMetaType& valueMetaType = ::qtjambi_cast<const QMetaType&>(valueMetaTypeId);
        if(keyMetaType.id()==QMetaType::UnknownType)
            JavaException::raiseIllegalArgumentException(env, QStringLiteral("QMetaType::UnknownType cannot be key type of %1.").arg("QMultiMap") QTJAMBI_STACKTRACEINFO );
        if(keyMetaType.id()==QMetaType::Void)
            JavaException::raiseIllegalArgumentException(env, QStringLiteral("void cannot be key type of %1.").arg("QMultiMap") QTJAMBI_STACKTRACEINFO );
        if(valueMetaType.id()==QMetaType::UnknownType)
            JavaException::raiseIllegalArgumentException(env, QStringLiteral("QMetaType::UnknownType cannot be value type of %1.").arg("QMultiMap") QTJAMBI_STACKTRACEINFO );
        if(valueMetaType.id()==QMetaType::Void)
            JavaException::raiseIllegalArgumentException(env, QStringLiteral("void cannot be value type of %1.").arg("QMultiMap") QTJAMBI_STACKTRACEINFO );
        const SuperTypeInfos superTypeInfos = SuperTypeInfos::fromClass(env, env->GetObjectClass(object));
        if(superTypeInfos.size()>1)
            JavaException::raiseError(env, QStringLiteral("It is not permitted to create a derived type of %1 implementing any Qt interface.").arg("QMultiMap") QTJAMBI_STACKTRACEINFO );
        if(!containerAccess){
            auto _containerAccess = createContainerAccess(AssociativeContainerType::QMultiMap, keyMetaType, valueMetaType);
            if(_containerAccess && _containerAccess->isMultiMap())
                containerAccess = static_cast<AbstractMultiMapAccess*>(_containerAccess);
        }
        if(!containerAccess){
            size_t size1 = size_t(keyMetaType.sizeOf());
            bool isPointer1 = AbstractContainerAccess::isPointerType(keyMetaType);
            size_t align1 = size_t(keyMetaType.alignOf());

            size_t size2 = size_t(valueMetaType.sizeOf());
            bool isPointer2 = AbstractContainerAccess::isPointerType(valueMetaType);
            size_t align2 = size_t(valueMetaType.alignOf());

            if(!keyType)
                keyType = CoreAPI::getClassForMetaType(env, keyMetaType);
            keyType = getGlobalClassRef(env, keyType);
            if(!valueType)
                valueType = CoreAPI::getClassForMetaType(env, valueMetaType);
            valueType = getGlobalClassRef(env, valueType);

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

            QtJambiUtils::QHashFunction hashFunction1 = QtJambiTypeManager::findHashFunction(isPointer1, keyMetaType);
            QtJambiUtils::QHashFunction hashFunction2 = QtJambiTypeManager::findHashFunction(isPointer2, valueMetaType);
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
            auto _containerAccess = createContainerAccess(
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
                valueOwnerFunction);
            if(_containerAccess && _containerAccess->isMultiMap())
                containerAccess = static_cast<AbstractMultiMapAccess*>(_containerAccess);
            isNativeContainer = Java::Runtime::Map::isInstanceOf(env, other) && ContainerAPI::testQMultiMap(env, other, keyMetaType, valueMetaType);
        }
    }
    void* listPtr;
    if(isNativeContainer){
        if(QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForJavaObject(env, other)){
            listPtr = containerAccess->createContainer(env, ConstContainerAndAccessInfo{link->getJavaObjectLocalRef(env), link->pointer(), link->containerAccess()});
        }else{
            if(Java::QtJambi::QtObjectInterface::isInstanceOf(env, other)){
                containerAccess->dispose();
                JavaException::raise<Java::QtJambi::QNoNativeResourcesException>(env, QStringLiteral("Incomplete object of type: %1").arg(QtJambiAPI::getObjectClassNamePrintable(env, other)) QTJAMBI_STACKTRACEINFO );
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
    Q_UNUSED(containerMetaType)
    QSharedPointer<QtJambiLink> link = QtJambiLink::createLinkForNativeObject(env, object, listPtr,
                                                                              LINK_NAME_META_TYPE_ARG(containerMetaType)
                                                                              true, true, containerAccess, QtJambiLink::Ownership::Java);
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

void CoreAPI::initializeQMultiMap(JNIEnv *env, jobject object, QtJambiNativeID beginId, QtJambiNativeID endId){
    using namespace QtJambiPrivate;
    AbstractMultiMapAccess* containerAccess = nullptr;
    QPair<void*,AbstractContainerAccess*> beginPair = ContainerAPI::fromNativeId(beginId);
    QPair<void*,AbstractContainerAccess*> endPair = ContainerAPI::fromNativeId(endId);
    QTJAMBI_CONTAINER_CAST(AssociativeConstIterator, beginAccess, beginPair.second);
    Q_ASSERT(!endPair.first || endPair.second->isAssociativeConstIterator());
    const QMetaType& keyMetaType = beginAccess->keyMetaType();
    const QMetaType& valueMetaType = beginAccess->valueMetaType();
    if(keyMetaType.id()==QMetaType::UnknownType)
        JavaException::raiseIllegalArgumentException(env, QStringLiteral("QMetaType::UnknownType cannot be key type of %1.").arg("QMultiMap") QTJAMBI_STACKTRACEINFO );
    if(keyMetaType.id()==QMetaType::Void)
        JavaException::raiseIllegalArgumentException(env, QStringLiteral("void cannot be key type of %1.").arg("QMultiMap") QTJAMBI_STACKTRACEINFO );
    if(valueMetaType.id()==QMetaType::UnknownType)
        JavaException::raiseIllegalArgumentException(env, QStringLiteral("QMetaType::UnknownType cannot be value type of %1.").arg("QMultiMap") QTJAMBI_STACKTRACEINFO );
    if(valueMetaType.id()==QMetaType::Void)
        JavaException::raiseIllegalArgumentException(env, QStringLiteral("void cannot be value type of %1.").arg("QMultiMap") QTJAMBI_STACKTRACEINFO );
    const SuperTypeInfos superTypeInfos = SuperTypeInfos::fromClass(env, env->GetObjectClass(object));
    if(superTypeInfos.size()>1)
        JavaException::raiseError(env, QStringLiteral("It is not permitted to create a derived type of %1 implementing any Qt interface.").arg("QMultiMap") QTJAMBI_STACKTRACEINFO );
    if(!containerAccess){
        auto _containerAccess = createContainerAccess(AssociativeContainerType::QMultiMap, keyMetaType, valueMetaType);
        if(_containerAccess && _containerAccess->isMultiMap())
            containerAccess = static_cast<AbstractMultiMapAccess*>(_containerAccess);
    }
    if(!containerAccess){
        size_t size1 = size_t(keyMetaType.sizeOf());
        bool isPointer1 = AbstractContainerAccess::isPointerType(keyMetaType);
        size_t align1 = size_t(keyMetaType.alignOf());

        size_t size2 = size_t(valueMetaType.sizeOf());
        bool isPointer2 = AbstractContainerAccess::isPointerType(valueMetaType);
        size_t align2 = size_t(valueMetaType.alignOf());

        jclass keyType = CoreAPI::getClassForMetaType(env, keyMetaType);
        jclass valueType = CoreAPI::getClassForMetaType(env, valueMetaType);
        keyType = getGlobalClassRef(env, keyType);
        valueType = getGlobalClassRef(env, valueType);

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
        QtJambiUtils::QHashFunction hashFunction1 = QtJambiTypeManager::findHashFunction(isPointer1, keyMetaType);
        QtJambiUtils::QHashFunction hashFunction2 = QtJambiTypeManager::findHashFunction(isPointer2, valueMetaType);
        if(!hashFunction1){
            JavaException::raiseQNoImplementationException(env, QString("Unable to create QMultiMap for %1 because of missing hash function.").arg(QtJambiAPI::getClassNamePrintable(env, keyType)) QTJAMBI_STACKTRACEINFO );
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
        auto _containerAccess = createContainerAccess(
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
            valueOwnerFunction);
        if(_containerAccess && _containerAccess->isMultiMap())
            containerAccess = static_cast<AbstractMultiMapAccess*>(_containerAccess);
    }
    void* listPtr = containerAccess->createContainer();
    QByteArray name = "QMultiMap<";
    name += containerAccess->keyMetaType().name();
    name += ",";
    name += containerAccess->valueMetaType().name();
    name += ">";
    name = QMetaObject::normalizedType(name);
    QMetaType containerMetaType(containerAccess->registerContainer(name));
    Q_UNUSED(containerMetaType)
    QSharedPointer<QtJambiLink> link = QtJambiLink::createLinkForNativeObject(env, object, listPtr,
                                                                              LINK_NAME_META_TYPE_ARG(containerMetaType)
                                                                              true, true, containerAccess, QtJambiLink::Ownership::Java);
    if(Q_UNLIKELY(!link)) {
        containerAccess->deleteContainer(listPtr);
        containerAccess->dispose();
    }else{
        bool useJava = containerAccess->asRC();
        if(endPair.first){
            bool equals = beginAccess->equals(beginPair.first, endPair.first);
            if(!equals && !useJava){
                std::optional<const void*> key = beginAccess->key(beginPair.first);
                std::optional<const void*> value = beginAccess->value(beginPair.first);
                useJava = !key.has_value() || !value.has_value();
            }
            if(useJava){
                ContainerInfo ci{object,listPtr};
                while(!equals){
                    jobject key = beginAccess->key(env, beginPair.first);
                    jobject value = beginAccess->value(env, beginPair.first);
                    containerAccess->insert(env, ci, key, value);
                    beginAccess->increment(beginPair.first);
                    equals = beginAccess->equals(beginPair.first, endPair.first);
                }
            }else{
                while(!equals){
                    std::optional<const void*> key = beginAccess->key(beginPair.first);
                    std::optional<const void*> value = beginAccess->value(beginPair.first);
                    if(key.has_value() && value.has_value()){
                        containerAccess->insert(listPtr, key.value(), value.value());
                    }
                    beginAccess->increment(beginPair.first);
                    equals = beginAccess->equals(beginPair.first, endPair.first);
                }
            }
        }else{
            std::optional<bool> isValid = beginAccess->isValid(beginPair.first);
            if(isValid.has_value() && !isValid.value() && !useJava){
                std::optional<const void*> key = beginAccess->key(beginPair.first);
                std::optional<const void*> value = beginAccess->value(beginPair.first);
                useJava = !key.has_value() || !value.has_value();
            }
            if(useJava){
                ContainerInfo ci{object,listPtr};
                while(isValid.has_value() && !isValid.value()){
                    jobject key = beginAccess->key(env, beginPair.first);
                    jobject value = beginAccess->value(env, beginPair.first);
                    containerAccess->insert(env, ci, key, value);
                    beginAccess->increment(beginPair.first);
                    isValid = beginAccess->isValid(beginPair.first);
                }
            }else{
                while(isValid.has_value() && !isValid.value()){
                    std::optional<const void*> key = beginAccess->key(beginPair.first);
                    std::optional<const void*> value = beginAccess->value(beginPair.first);
                    if(key.has_value() && value.has_value()){
                        containerAccess->insert(listPtr, key.value(), value.value());
                    }
                    beginAccess->increment(beginPair.first);
                    isValid = beginAccess->isValid(beginPair.first);
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
    if(typeA==QMetaType::fromType<QChar>()){
        return typeB==QMetaType::fromType<char16_t>();
    }
    if(typeA==QMetaType::fromType<char16_t>()){
        return typeB==QMetaType::fromType<QChar>();
    }
    {
        QtJambiStorage* storage = getQtJambiStorage();
        QReadLocker locker(storage->registryLock());
        auto iter = storage->metaTypeByNativeMetaType().constFind(typeA);
        if(iter!=storage->metaTypeByNativeMetaType().constEnd()){
            return *iter==typeB;
        }
        iter = storage->metaTypeByNativeMetaType().constFind(typeB);
        if(iter!=storage->metaTypeByNativeMetaType().constEnd()){
            return *iter==typeA;
        }
    }
    return false;
}

#define TEST_COLLECTION1(NAME,JCLASS) \
bool ContainerAPI::testQ##NAME(JNIEnv *env, jobject collection, const QMetaType& expectedElementMetaType) {\
    if(collection && expectedElementMetaType.isValid()){\
        if(Java::QtCore::Q##JCLASS::isInstanceOf(env,collection)){\
            if(QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForJavaObject(env, collection)){\
                if(link->containerAccess() && link->containerAccess()->is##NAME()){\
                    Abstract##NAME##Access* containerAccess = static_cast<Abstract##NAME##Access*>(link->containerAccess());\
                    return compareMetaTypes(containerAccess->elementMetaType(), expectedElementMetaType);\
                }\
            }else\
                JavaException::raise<Java::QtJambi::QNoNativeResourcesException>(env, QStringLiteral("Incomplete object of type: %1").arg(QtJambiAPI::getObjectClassNamePrintable(env, collection)) QTJAMBI_STACKTRACEINFO );\
        }else if(!env->IsInstanceOf(collection, Java::Runtime::Collection::getClass(env)))\
            JavaException::raiseIllegalArgumentException(env, QString("Wrong argument given: %1, expected: %2").arg(QtJambiAPI::getObjectClassNamePrintable(env, collection), QtJambiAPI::getClassNamePrintable(env, Java::Runtime::Collection::getClass(env))) QTJAMBI_STACKTRACEINFO );\
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
                if(link->containerAccess() && link->containerAccess()->is##NAME()){\
                    Abstract##NAME##Access* containerAccess = static_cast<Abstract##NAME##Access*>(link->containerAccess());\
                    return compareMetaTypes(containerAccess->keyMetaType(), expectedKeyMetaType) && compareMetaTypes(containerAccess->valueMetaType(), expectedValueMetaType);\
                }\
            }else\
                JavaException::raise<Java::QtJambi::QNoNativeResourcesException>(env, QStringLiteral("Incomplete object of type: %1").arg(QtJambiAPI::getObjectClassNamePrintable(env, mapObject)) QTJAMBI_STACKTRACEINFO );\
        }else if(!env->IsInstanceOf(mapObject, Java::Runtime::Map::getClass(env)))\
            JavaException::raiseIllegalArgumentException(env, QString("Wrong argument given: %1, expected: %2").arg(QtJambiAPI::getObjectClassNamePrintable(env, mapObject), QtJambiAPI::getClassNamePrintable(env, Java::Runtime::Map::getClass(env))) QTJAMBI_STACKTRACEINFO );\
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
                if(link->containerAccess() && link->containerAccess()->is##NAME()){\
                    Abstract##NAME##Access* containerAccess = static_cast<Abstract##NAME##Access*>(link->containerAccess());\
                    return compareMetaTypes(containerAccess->keyMetaType(), expectedKeyMetaType) && compareMetaTypes(containerAccess->valueMetaType(), expectedValueMetaType);\
                }\
            }else\
                JavaException::raise<Java::QtJambi::QNoNativeResourcesException>(env, QStringLiteral("Incomplete object of type: %1").arg(QtJambiAPI::getObjectClassNamePrintable(env, mapObject)) QTJAMBI_STACKTRACEINFO );\
        }else if(!env->IsInstanceOf(mapObject, Java::Runtime::Map::getClass(env)))\
            JavaException::raiseIllegalArgumentException(env, QString("Wrong argument given: %1, expected: %2").arg(QtJambiAPI::getObjectClassNamePrintable(env, mapObject), QtJambiAPI::getClassNamePrintable(env, Java::Runtime::Map::getClass(env))) QTJAMBI_STACKTRACEINFO );\
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
                if(link->containerAccess() && link->containerAccess()->is##NAME()){\
                    Abstract##NAME##Access* containerAccess = static_cast<Abstract##NAME##Access*>(link->containerAccess());\
                    if(compareMetaTypes(containerAccess->elementMetaType(), expectedElementMetaType)){\
                        pointer = link->pointer();\
                        return true;\
                    }\
                }\
            }else\
                JavaException::raise<Java::QtJambi::QNoNativeResourcesException>(env, QStringLiteral("Incomplete object of type: %1").arg(QtJambiAPI::getObjectClassNamePrintable(env, collection)) QTJAMBI_STACKTRACEINFO );\
        }else if(!env->IsInstanceOf(collection, Java::Runtime::Collection::getClass(env)))\
            JavaException::raiseIllegalArgumentException(env, QString("Wrong argument given: %1, expected: %2").arg(QtJambiAPI::getObjectClassNamePrintable(env, collection), QtJambiAPI::getClassNamePrintable(env, Java::Runtime::Collection::getClass(env))) QTJAMBI_STACKTRACEINFO );\
    }\
    return false;\
} \
bool ContainerAPI::getAsQ##NAME(JNIEnv *env, jobject collection, const QMetaType& expectedElementMetaType, void* &pointer, AbstractContainerAccess*& access) {\
    if(collection && expectedElementMetaType.isValid()){\
        if(Java::QtCore::Q##NAME::isInstanceOf(env,collection)){\
            if(QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForJavaObject(env, collection)){\
                if(link->containerAccess() && link->containerAccess()->is##NAME()){\
                    Abstract##NAME##Access* containerAccess = static_cast<Abstract##NAME##Access*>(link->containerAccess());\
                    if(compareMetaTypes(containerAccess->elementMetaType(), expectedElementMetaType)){\
                        pointer = link->pointer();\
                        access = containerAccess;\
                        return true;\
                }\
            }\
        }else\
            JavaException::raise<Java::QtJambi::QNoNativeResourcesException>(env, QStringLiteral("Incomplete object of type: %1").arg(QtJambiAPI::getObjectClassNamePrintable(env, collection)) QTJAMBI_STACKTRACEINFO );\
    }else if(!env->IsInstanceOf(collection, Java::Runtime::Collection::getClass(env)))\
        JavaException::raiseIllegalArgumentException(env, QString("Wrong argument given: %1, expected: %2").arg(QtJambiAPI::getObjectClassNamePrintable(env, collection), QtJambiAPI::getClassNamePrintable(env, Java::Runtime::Collection::getClass(env))) QTJAMBI_STACKTRACEINFO );\
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
                if(link->containerAccess() && link->containerAccess()->is##NAME()){\
                    Abstract##NAME##Access* containerAccess = static_cast<Abstract##NAME##Access*>(link->containerAccess());\
                    if(compareMetaTypes(containerAccess->keyMetaType(), expectedKeyMetaType) && compareMetaTypes(containerAccess->valueMetaType(), expectedValueMetaType)){\
                       pointer = link->pointer();\
                       return true;\
                    }\
                }\
            }else\
                JavaException::raise<Java::QtJambi::QNoNativeResourcesException>(env, QStringLiteral("Incomplete object of type: %1").arg(QtJambiAPI::getObjectClassNamePrintable(env, mapObject)) QTJAMBI_STACKTRACEINFO );\
        }else if(!env->IsInstanceOf(mapObject, Java::Runtime::Map::getClass(env)))\
            JavaException::raiseIllegalArgumentException(env, QString("Wrong argument given: %1, expected: %2").arg(QtJambiAPI::getObjectClassNamePrintable(env, mapObject), QtJambiAPI::getClassNamePrintable(env, Java::Runtime::Map::getClass(env))) QTJAMBI_STACKTRACEINFO );\
    }\
    return false;\
}\
bool ContainerAPI::getAsQ##NAME(JNIEnv *env, jobject mapObject, const QMetaType& expectedKeyMetaType, const QMetaType& expectedValueMetaType, void* &pointer, AbstractContainerAccess*& access) {\
    if(mapObject && expectedKeyMetaType.isValid() && expectedValueMetaType.isValid()){\
        if(Java::QtCore::Q##NAME::isInstanceOf(env, mapObject)){\
            if(QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForJavaObject(env, mapObject)){\
                if(link->containerAccess() && link->containerAccess()->is##NAME()){\
                    Abstract##NAME##Access* containerAccess = static_cast<Abstract##NAME##Access*>(link->containerAccess());\
                    if(compareMetaTypes(containerAccess->keyMetaType(), expectedKeyMetaType) && compareMetaTypes(containerAccess->valueMetaType(), expectedValueMetaType)){\
                        pointer = link->pointer();\
                        access = containerAccess;\
                        return true;\
                }\
            }\
        }else\
            JavaException::raise<Java::QtJambi::QNoNativeResourcesException>(env, QStringLiteral("Incomplete object of type: %1").arg(QtJambiAPI::getObjectClassNamePrintable(env, mapObject)) QTJAMBI_STACKTRACEINFO );\
    }else if(!env->IsInstanceOf(mapObject, Java::Runtime::Map::getClass(env)))\
        JavaException::raiseIllegalArgumentException(env, QString("Wrong argument given: %1, expected: %2").arg(QtJambiAPI::getObjectClassNamePrintable(env, mapObject), QtJambiAPI::getClassNamePrintable(env, Java::Runtime::Map::getClass(env))) QTJAMBI_STACKTRACEINFO );\
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
                if(link->containerAccess() && link->containerAccess()->is##NAME()){\
                    Abstract##NAME##Access* containerAccess = static_cast<Abstract##NAME##Access*>(link->containerAccess());\
                    if(compareMetaTypes(containerAccess->keyMetaType(), expectedKeyMetaType) && compareMetaTypes(containerAccess->valueMetaType(), expectedValueMetaType)){\
                        pointer = link->pointer();\
                        return true;\
                    }\
                }\
            }else\
                JavaException::raise<Java::QtJambi::QNoNativeResourcesException>(env, QStringLiteral("Incomplete object of type: %1").arg(QtJambiAPI::getObjectClassNamePrintable(env, mapObject)) QTJAMBI_STACKTRACEINFO );\
        }else if(!env->IsInstanceOf(mapObject, Java::Runtime::Map::getClass(env)))\
            JavaException::raiseIllegalArgumentException(env, QString("Wrong argument given: %1, expected: %2").arg(QtJambiAPI::getObjectClassNamePrintable(env, mapObject), QtJambiAPI::getClassNamePrintable(env, Java::Runtime::Map::getClass(env))) QTJAMBI_STACKTRACEINFO );\
    }\
    return false;\
}\
bool ContainerAPI::getAsQ##NAME(JNIEnv *env, jobject mapObject, const QMetaType& expectedKeyMetaType, const QMetaType& expectedValueMetaType, void* &pointer, AbstractContainerAccess*& access) {\
    if(mapObject && expectedKeyMetaType.isValid() && expectedValueMetaType.isValid()){\
        if(Java::QtCore::Q##NAME::isInstanceOf(env, mapObject)){\
            if(QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForJavaObject(env, mapObject)){\
                if(link->containerAccess() && link->containerAccess()->is##NAME()){\
                    Abstract##NAME##Access* containerAccess = static_cast<Abstract##NAME##Access*>(link->containerAccess());\
                    if(compareMetaTypes(containerAccess->keyMetaType(), expectedKeyMetaType) && compareMetaTypes(containerAccess->valueMetaType(), expectedValueMetaType)){\
                        pointer = link->pointer();\
                        access = containerAccess;\
                        return true;\
                }\
            }\
        }else\
            JavaException::raise<Java::QtJambi::QNoNativeResourcesException>(env, QStringLiteral("Incomplete object of type: %1").arg(QtJambiAPI::getObjectClassNamePrintable(env, mapObject)) QTJAMBI_STACKTRACEINFO );\
    }else if(!env->IsInstanceOf(mapObject, Java::Runtime::Map::getClass(env)))\
        JavaException::raiseIllegalArgumentException(env, QString("Wrong argument given: %1, expected: %2").arg(QtJambiAPI::getObjectClassNamePrintable(env, mapObject), QtJambiAPI::getClassNamePrintable(env, Java::Runtime::Map::getClass(env))) QTJAMBI_STACKTRACEINFO );\
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
bool ContainerAPI::testQStack(JNIEnv *env, jobject collection, const QMetaType& expectedElementMetaType) {
    return ContainerAPI::testQList(env, collection, expectedElementMetaType);
}
bool ContainerAPI::getAsQStack(JNIEnv *env, jobject collection, const QMetaType& expectedElementMetaType, void* &pointer) {
    return ContainerAPI::getAsQList(env, collection, expectedElementMetaType,pointer);
}
bool ContainerAPI::getAsQStack(JNIEnv *env, jobject collection, const QMetaType& expectedElementMetaType, void* &pointer, AbstractContainerAccess*& access) {
    return ContainerAPI::getAsQList(env, collection, expectedElementMetaType, pointer, access);
}
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
