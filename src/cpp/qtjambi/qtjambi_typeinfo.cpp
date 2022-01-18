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

#include <QtCore/qcompilerdetection.h>
QT_WARNING_DISABLE_DEPRECATED
#include <QtCore/QUrl>
#include <QtCore/QModelIndex>
#include <QtCore/QCborValue>
#include <QtCore/QCborValueRef>
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
#include <QtCore/QXmlStreamStringRef>
#endif
#include "qtjambi_core.h"
#include "qtjambi_repository_p.h"
#include "qtjambi_typeinfo_p.h"
#include "qtjambi_registry_p.h"
#include "qtjambi_interfaces_p.h"
#include "qtjambilink_p.h"
#include "qtjambishell_p.h"
#include "qtjambi_thread_p.h"

void qtjambi_to_qstring(QString& result, JNIEnv *env, jobject object);
JNIEnv *qtjambi_current_environment(bool initializeJavaThread);
const QMetaObject *qtjambi_find_first_static_metaobject(const QMetaObject *meta_object);
jmethodID resolveMethod(JNIEnv *env, const char *methodName, const char *signature, jclass clazz, bool isStatic);
bool isQObject(const std::type_info& typeId);

struct QtJambiShellAccess : QtJambiShellImpl{
    void overrideLink(const QSharedPointer<QtJambiLink>& link){
        QtJambiShellImpl::overrideLink(link);
    }
};

typedef QMap<size_t, QMap<QByteArray,const QtJambiTypeEntry*>> TypeEntryHash;
Q_GLOBAL_STATIC(TypeEntryHash, gTypeEntryHash)
Q_GLOBAL_STATIC_WITH_ARGS(QReadWriteLock, gTypeEntryLock, (QReadWriteLock::Recursive))

const char* getJavaNameByFunctional(const char* qt_name);
QList<const PolymorphicIdHandler*> getPolymorphicIdHandlers(const std::type_info& polymorphicBaseTypeId);

void clear_type_entry(const std::type_info& typeId)
{
    QWriteLocker locker(gTypeEntryLock());
    Q_UNUSED(locker)
    if(gTypeEntryHash()->contains(typeId.hash_code())){
        for(const QtJambiTypeEntry* entry : gTypeEntryHash()->take(typeId.hash_code()).values()){
            delete entry;
        }
    }
}

const QtJambiTypeEntry* get_type_entry(JNIEnv* env, const std::type_info& typeId, bool recursive, const char* qtName)
{
    {
        QReadLocker locker(gTypeEntryLock());
        Q_UNUSED(locker)
        if(gTypeEntryHash()->contains(typeId.hash_code())){
            const QMap<QByteArray,const QtJambiTypeEntry*>& entries = (*gTypeEntryHash())[typeId.hash_code()];
            if(qtName){
                if(const QtJambiTypeEntry* result = entries.value(qtName)){
                    return result;
                }
            }else if(!entries.isEmpty()){
                return entries.first();
            }
        }
    }
    const QtJambiTypeEntry* result = nullptr;
    const char *qt_name = nullptr;
    EntryTypes entryType = getEntryType(typeId);
    if(entryType!=EntryTypes::Unspecific){
        qt_name = getQtName(typeId);
        const char *java_name = getJavaName(typeId);
        if(qtName && entryType==EntryTypes::FunctionalTypeInfo){
            if(QLatin1String(qtName)!=QLatin1String(qt_name)){
                qt_name = qtName;
                java_name = getJavaNameByFunctional(qtName);
            }
        }
        jclass java_class = resolveClass(env, java_name);
        if(!java_class){
            JavaException::raiseError(env, "class cannot be found" QTJAMBI_STACKTRACEINFO );
            return nullptr;
        }

        switch(entryType){
        case EntryTypes::EnumTypeInfo:
        {
            if(recursive){
                const std::type_info* flagId = getFlagForEnum(typeId);
                if(flagId){
                    if(const FlagsTypeEntry* flagsType = dynamic_cast<const FlagsTypeEntry*>(QtJambiTypeEntry::getTypeEntry(env, *flagId))){
                        return flagsType->enumType();
                    }
                }
            }
            QString sig;
            size_t value_size = getValueSize(typeId);
            switch ( value_size ) {
            case 1:  sig = QLatin1String("(B)L%1;"); break;
            case 2:  sig = QLatin1String("(S)L%1;"); break;
            case 8:  sig = QLatin1String("(J)L%1;"); break;
            default: sig = QLatin1String("(I)L%1;"); break;
            }
            jmethodID creator_method = resolveMethod(env, "resolve", qPrintable(sig.arg(java_name)), java_class, true);
            Q_ASSERT(creator_method);
            result = new EnumTypeEntry(env, typeId, qt_name, java_name, java_class, creator_method, value_size);
            break;
        }
        case EntryTypes::FlagsTypeInfo:
        {
            size_t value_size = getValueSize(typeId);
            const std::type_info* flagId = getEnumForFlag(typeId);
            Q_ASSERT(flagId);
            jmethodID creator_method = resolveMethod(env, "<init>", "(I)V", java_class, false);
            Q_ASSERT(creator_method);
            result = new FlagsTypeEntry(env, typeId, qt_name, java_name, java_class, creator_method, value_size, dynamic_cast<const EnumTypeEntry*>(get_type_entry(env, *flagId, false, nullptr)));
            break;
        }
        case EntryTypes::FunctionalTypeInfo:
        {
            size_t value_size = getValueSize(typeId);
            size_t shell_size = getShellSize(typeId);
            jclass java_impl_class = nullptr;
            jclass java_wrapper_class = nullptr;
            jmethodID creator_method = nullptr;
            if(Java::Runtime::Class::isInterface(env, java_class)){
                java_impl_class = resolveClass(env, qPrintable(QString("%1$Impl").arg(java_name)));
                if(!java_impl_class){
                    JavaException::raiseError(env, "class cannot be found" QTJAMBI_STACKTRACEINFO );
                    return nullptr;
                }
                int modifiers = Java::Runtime::Class::getModifiers(env,java_impl_class);
                if(Java::Runtime::Modifier::isAbstract(env, modifiers)){
                    java_wrapper_class = resolveClass(env, qPrintable(QString("%1$Impl$ConcreteWrapper").arg(java_name)));
                }else{
                    java_wrapper_class = java_impl_class;
                }
            }else{
                java_impl_class = java_class;
                int modifiers = Java::Runtime::Class::getModifiers(env,java_impl_class);
                if(Java::Runtime::Modifier::isAbstract(env, modifiers)){
                    java_wrapper_class = resolveClass(env, qPrintable(QString("%1$ConcreteWrapper").arg(java_name)));
                }else{
                    java_wrapper_class = java_impl_class;
                }
            }
            creator_method = findInternalPrivateConstructor(env, java_wrapper_class);
            if(!creator_method){
                JavaException::raiseError(env, "constructor cannot be found" QTJAMBI_STACKTRACEINFO );
                return nullptr;
            }
            const QVector<const FunctionInfo>* _virtualFunctions = virtualFunctions(typeId);
            Destructor destructor = registeredDestructor(typeId);
            FunctionalResolver registered_functional_resolver = registeredFunctionalResolver(typeId);
            QMetaType qt_meta_type(registeredMetaTypeID(typeId));
            Q_ASSERT(qt_meta_type.isValid());

            result = new FunctionalTypeEntry(env, typeId, qt_name, java_name, java_class, creator_method, value_size, shell_size,
                                             java_impl_class, java_wrapper_class, _virtualFunctions, destructor, registered_functional_resolver, qt_meta_type);
            break;
        }
        case EntryTypes::ObjectTypeInfo:
        {
            const std::type_info* super_type = nullptr; // TODO
            QList<const std::type_info*> interface_types; // TODO
            size_t value_size = getValueSize(typeId);
            InterfaceOffsetInfo interfaceOffsetInfo;
            registeredInterfaceOffsets(typeId, &interfaceOffsetInfo);
            const QMap<size_t, uint>& interface_offsets = interfaceOffsetInfo.offsets;
            size_t shell_size = getShellSize(typeId);
            PtrDeleterFunction _deleter = deleter(typeId);
            PtrOwnerFunction owner_function = registeredOwnerFunction(typeId);
            const QVector<const FunctionInfo>* _virtualFunctions = virtualFunctions(typeId);
            Destructor destructor = registeredDestructor(typeId);
            int modifiers = Java::Runtime::Class::getModifiers(env,java_class);
            QList<const PolymorphicIdHandler*> polymorphicIdHandlers = getPolymorphicIdHandlers(typeId);
            if(Java::Runtime::Modifier::isAbstract(env, modifiers)){
                jclass java_wrapper_class = nullptr;
                try{
                    java_wrapper_class = resolveClass(env, qPrintable(QString("%1$ConcreteWrapper").arg(java_name)));
                } catch (const JavaException&) {
                }
                jmethodID creator_method = nullptr;
                if(java_wrapper_class){
                    creator_method = findInternalPrivateConstructor(env, java_wrapper_class);
                    if(!creator_method){
                        JavaException::raiseError(env, "constructor cannot be found" QTJAMBI_STACKTRACEINFO );
                        return nullptr;
                    }
                }
                result = new ObjectAbstractTypeEntry(env, typeId, qt_name, java_name, java_class, creator_method, value_size, super_type,
                                                     interface_types, interface_offsets,
                                                     shell_size, _deleter, owner_function, _virtualFunctions,
                                                     destructor, polymorphicIdHandlers, java_wrapper_class
                                                 );
            }else{
                jmethodID creator_method = findInternalPrivateConstructor(env, java_class);
                if(!creator_method){
                    JavaException::raiseError(env, "constructor cannot be found" QTJAMBI_STACKTRACEINFO );
                    return nullptr;
                }
                result = new ObjectTypeEntry(env, typeId, qt_name, java_name, java_class, creator_method, value_size, super_type,
                                                     interface_types, interface_offsets,
                                                     shell_size, _deleter, owner_function, _virtualFunctions,
                                                     destructor, polymorphicIdHandlers
                                                 );
            }
            break;
        }
        case EntryTypes::ValueTypeInfo:
        {
            const std::type_info* super_type = nullptr; // TODO
            QList<const std::type_info*> interface_types; // TODO
            size_t value_size = getValueSize(typeId);
            InterfaceOffsetInfo interfaceOffsetInfo;
            registeredInterfaceOffsets(typeId, &interfaceOffsetInfo);
            const QMap<size_t, uint>& interface_offsets = interfaceOffsetInfo.offsets;
            size_t shell_size = getShellSize(typeId);
            NewContainerAccessFunction containerAccessFactory = getContainerAccessFactory(typeId);
            PtrOwnerFunction owner_function = registeredOwnerFunction(typeId);
            const QVector<const FunctionInfo>* _virtualFunctions = virtualFunctions(typeId);
            Destructor destructor = registeredDestructor(typeId);
            QMetaType qt_meta_type(registeredMetaTypeID(typeId));
            Q_ASSERT(qt_meta_type.isValid());
            QList<const PolymorphicIdHandler*> polymorphicIdHandlers = getPolymorphicIdHandlers(typeId);

            int modifiers = Java::Runtime::Class::getModifiers(env,java_class);
            if(Java::Runtime::Modifier::isAbstract(env, modifiers)){
                jclass java_wrapper_class = nullptr;
                try{
                    java_wrapper_class = resolveClass(env, qPrintable(QString("%1$ConcreteWrapper").arg(java_name)));
                } catch (const JavaException&) {
                }
                jmethodID creator_method = nullptr;
                if(java_wrapper_class){
                    creator_method = findInternalPrivateConstructor(env, java_wrapper_class);
                    if(!creator_method){
                        JavaException::raiseError(env, "constructor cannot be found" QTJAMBI_STACKTRACEINFO );
                        return nullptr;
                    }
                }
                if(containerAccessFactory){
                    result = new ObjectAbstractContainerTypeEntry(env, typeId, qt_name, java_name, java_class, creator_method, value_size, super_type,
                                                         interface_types, interface_offsets,
                                                         shell_size, containerAccessFactory, owner_function, _virtualFunctions,
                                                         destructor, polymorphicIdHandlers, qt_meta_type, java_wrapper_class
                                                     );
                }else{
                    result = new ObjectAbstractValueTypeEntry(env, typeId, qt_name, java_name, java_class, creator_method, value_size, super_type,
                                                         interface_types, interface_offsets,
                                                         shell_size, owner_function, _virtualFunctions,
                                                         destructor, polymorphicIdHandlers, qt_meta_type, java_wrapper_class
                                                     );
                }
            }else{
                jmethodID creator_method = findInternalPrivateConstructor(env, java_class);
                if(!creator_method){
                    JavaException::raiseError(env, "constructor cannot be found" QTJAMBI_STACKTRACEINFO );
                    return nullptr;
                }
                if(containerAccessFactory){
                    result = new ObjectContainerTypeEntry(env, typeId, qt_name, java_name, java_class, creator_method, value_size, super_type,
                                                      interface_types, interface_offsets,
                                                      shell_size, containerAccessFactory, owner_function, _virtualFunctions,
                                                      destructor, polymorphicIdHandlers, qt_meta_type
                                                     );
                }else{
                    result = new ObjectValueTypeEntry(env, typeId, qt_name, java_name, java_class, creator_method, value_size, super_type,
                                                      interface_types, interface_offsets,
                                                      shell_size, owner_function, _virtualFunctions,
                                                      destructor, polymorphicIdHandlers, qt_meta_type
                                                     );
                }
            }
            break;
        }
        case EntryTypes::InterfaceTypeInfo:
        {
            const std::type_info* super_type = nullptr; // TODO
            QList<const std::type_info*> interface_types; // TODO
            size_t value_size = getValueSize(typeId);
            jclass java_impl_class = resolveClass(env, qPrintable(QString("%1$Impl").arg(java_name)));
            if(!java_impl_class){
                JavaException::raiseError(env, "class cannot be found" QTJAMBI_STACKTRACEINFO );
                return nullptr;
            }
            jclass java_wrapper_class = nullptr;
            int modifiers = Java::Runtime::Class::getModifiers(env,java_impl_class);
            if(Java::Runtime::Modifier::isAbstract(env, modifiers)){
                java_wrapper_class = resolveClass(env, qPrintable(QString("%1$Impl$ConcreteWrapper").arg(java_name)));
                if(!java_wrapper_class){
                    JavaException::raiseError(env, "class cannot be found" QTJAMBI_STACKTRACEINFO );
                    return nullptr;
                }
            }else{
                java_wrapper_class = java_impl_class;
            }
            jmethodID creator_method = findInternalPrivateConstructor(env, java_wrapper_class);
            if(!creator_method){
                JavaException::raiseError(env, "constructor cannot be found" QTJAMBI_STACKTRACEINFO );
                return nullptr;
            }
            InterfaceOffsetInfo interfaceOffsetInfo;
            registeredInterfaceOffsets(typeId, &interfaceOffsetInfo);
            const QMap<size_t, uint>& interface_offsets = interfaceOffsetInfo.offsets;
            size_t shell_size = getShellSize(typeId);
            PtrDeleterFunction _deleter = deleter(typeId);
            PtrOwnerFunction owner_function = registeredOwnerFunction(typeId);
            const QVector<const FunctionInfo>* _virtualFunctions = virtualFunctions(typeId);
            Destructor destructor = registeredDestructor(typeId);
            QList<const PolymorphicIdHandler*> polymorphicIdHandlers;
            for(const PolymorphicIdHandler* handler : getPolymorphicIdHandlers(typeId)){
                jclass _java_class = resolveClass(env, getJavaName(handler->m_targetTypeId));
                if(env->IsAssignableFrom(_java_class, java_class)){
                    polymorphicIdHandlers << handler;
                }
            }
            result = new InterfaceTypeEntry(env, typeId,
                                        qt_name, java_name, java_class, creator_method,
                                        value_size, java_impl_class, java_wrapper_class,
                                        super_type, interface_types, interface_offsets, shell_size,
                                        _deleter, owner_function, _virtualFunctions, destructor, polymorphicIdHandlers );
            break;
        }
        case EntryTypes::InterfaceValueTypeInfo:
        {
            const std::type_info* super_type = nullptr; // TODO
            QList<const std::type_info*> interface_types; // TODO
            size_t value_size = getValueSize(typeId);
            jclass java_impl_class = resolveClass(env, qPrintable(QString("%1$Impl").arg(java_name)));
            if(!java_impl_class){
                JavaException::raiseError(env, "class cannot be found" QTJAMBI_STACKTRACEINFO );
                return nullptr;
            }
            jclass java_wrapper_class = nullptr;
            int modifiers = Java::Runtime::Class::getModifiers(env,java_impl_class);
            if(Java::Runtime::Modifier::isAbstract(env, modifiers)){
                java_wrapper_class = resolveClass(env, qPrintable(QString("%1$Impl$ConcreteWrapper").arg(java_name)));
                if(!java_wrapper_class){
                    JavaException::raiseError(env, "class cannot be found" QTJAMBI_STACKTRACEINFO );
                    return nullptr;
                }
            }else{
                java_wrapper_class = java_impl_class;
            }
            jmethodID creator_method = findInternalPrivateConstructor(env, java_wrapper_class);
            if(!creator_method){
                JavaException::raiseError(env, "constructor cannot be found" QTJAMBI_STACKTRACEINFO );
                return nullptr;
            }
            InterfaceOffsetInfo interfaceOffsetInfo;
            registeredInterfaceOffsets(typeId, &interfaceOffsetInfo);
            const QMap<size_t, uint>& interface_offsets = interfaceOffsetInfo.offsets;
            size_t shell_size = getShellSize(typeId);
            PtrDeleterFunction _deleter = deleter(typeId);
            PtrOwnerFunction owner_function = registeredOwnerFunction(typeId);
            const QVector<const FunctionInfo>* _virtualFunctions = virtualFunctions(typeId);
            Destructor destructor = registeredDestructor(typeId);
            QMetaType qt_meta_type(registeredMetaTypeID(typeId));
            Q_ASSERT(qt_meta_type.isValid());
            QList<const PolymorphicIdHandler*> polymorphicIdHandlers;
            for(const PolymorphicIdHandler* handler : getPolymorphicIdHandlers(typeId)){
                jclass _java_class = resolveClass(env, getJavaName(handler->m_targetTypeId));
                if(env->IsAssignableFrom(_java_class, java_class)){
                    polymorphicIdHandlers << handler;
                }
            }

            result = new InterfaceValueTypeEntry(env, typeId,
                                        qt_name, java_name, java_class, creator_method,
                                        value_size, java_impl_class, java_wrapper_class,
                                        super_type, interface_types, interface_offsets,
                                        shell_size, _deleter, owner_function, _virtualFunctions,
                                        destructor, polymorphicIdHandlers, qt_meta_type );
            break;
        }
        case EntryTypes::QObjectTypeInfo:
        {
            const std::type_info* super_type = nullptr; // TODO
            QList<const std::type_info*> interface_types; // TODO
            size_t value_size = getValueSize(typeId);
            size_t shell_size = getShellSize(typeId);
            InterfaceOffsetInfo interfaceOffsetInfo;
            registeredInterfaceOffsets(typeId, &interfaceOffsetInfo);
            const QMap<size_t, uint>& interface_offsets = interfaceOffsetInfo.offsets;
            const QVector<const FunctionInfo>* _virtualFunctions = virtualFunctions(typeId);
            const QMetaObject* original_meta_object = registeredOriginalMetaObject(typeId);
            int modifiers = Java::Runtime::Class::getModifiers(env,java_class);
            QList<const PolymorphicIdHandler*> polymorphicIdHandlers;
            for(const PolymorphicIdHandler* handler : getPolymorphicIdHandlers(typeId)){
                jclass _java_class = resolveClass(env, getJavaName(handler->m_targetTypeId));
                if(env->IsAssignableFrom(_java_class, java_class)){
                    polymorphicIdHandlers << handler;
                }
            }
            if(Java::Runtime::Modifier::isAbstract(env, modifiers)){
                jclass java_wrapper_class = nullptr;
                try {
                    java_wrapper_class = resolveClass(env, qPrintable(QString("%1$ConcreteWrapper").arg(java_name)));
                } catch (const JavaException&) {
                }
                jmethodID creator_method = nullptr;
                if(java_wrapper_class){
                    creator_method = findInternalPrivateConstructor(env, java_wrapper_class);
                    if(!creator_method){
                        JavaException::raiseError(env, "constructor cannot be found" QTJAMBI_STACKTRACEINFO );
                        return nullptr;
                    }
                }
                if(polymorphicIdHandlers.isEmpty()){
                    result = new QObjectAbstractTypeEntry(env, typeId, qt_name, java_name, java_class, creator_method,
                                                  value_size, super_type, interface_types, interface_offsets, shell_size,
                                                  _virtualFunctions, original_meta_object, java_wrapper_class
                                             );
                }else{
                    result = new QObjectAbstractPolymorphicTypeEntry(env, typeId, qt_name, java_name, java_class, creator_method,
                                                  value_size, super_type, interface_types, interface_offsets, shell_size,
                                                  _virtualFunctions, original_meta_object, polymorphicIdHandlers, java_wrapper_class
                                             );
                }
            }else{
                jmethodID creator_method = findInternalPrivateConstructor(env, java_class);
                if(!creator_method){
                    JavaException::raiseError(env, "constructor cannot be found" QTJAMBI_STACKTRACEINFO );
                    return nullptr;
                }
                if(Java::QtCore::QThread::isAssignableFrom(env, java_class))
                    result = new QThreadTypeEntry(env, typeId, qt_name, java_name, java_class, creator_method,
                                                  value_size, super_type, interface_types, interface_offsets,
                                                  shell_size, _virtualFunctions, original_meta_object
                                             );
                else if(polymorphicIdHandlers.isEmpty()){
                    const std::type_info* otherTypeId = nullptr;
                    if(!original_meta_object
                            && strcmp(qt_name, "QRemoteObjectDynamicReplica")==0
                            && (otherTypeId = getTypeByQtName("QRemoteObjectReplica"))
                            && (original_meta_object = registeredOriginalMetaObject(*otherTypeId))){
                        result = new QObjectPendingTypeEntry(env, typeId, qt_name, java_name, java_class, creator_method,
                                                      value_size, super_type, interface_types, interface_offsets,
                                                      shell_size, _virtualFunctions, original_meta_object
                                                 );
                        original_meta_object = nullptr;
                    }else{
                        result = new QObjectTypeEntry(env, typeId, qt_name, java_name, java_class, creator_method,
                                                      value_size, super_type, interface_types, interface_offsets,
                                                      shell_size, _virtualFunctions, original_meta_object
                                                 );

                    }
                }else
                    result = new QObjectPolymorphicTypeEntry(env, typeId, qt_name, java_name, java_class, creator_method,
                                                  value_size, super_type, interface_types, interface_offsets,
                                                  shell_size, _virtualFunctions, original_meta_object, polymorphicIdHandlers
                                             );
            }
            break;
        }
        case EntryTypes::StringTypeInfo:
        {
            size_t value_size = getValueSize(typeId);
            if(typeId==typeid(QString)){
                result = new StringTypeEntry(env, typeId, qt_name, java_name, java_class, value_size);
            }else{
                result = new StringUtilTypeEntry(env, typeId, qt_name, java_name, java_class, value_size);
            }
            break;
        }
        case EntryTypes::PrimitiveTypeInfo:
        {
            size_t value_size = getValueSize(typeId);
            if(typeId==typeid(double) || typeId==typeid(jdouble)){
                result = new JDoubleTypeEntry(env, typeId, qt_name, java_name, java_class, value_size);
            }else if(typeId==typeid(float) || typeId==typeid(jfloat)){
                result = new JFloatTypeEntry(env, typeId, qt_name, java_name, java_class, value_size);
            }else if(typeId==typeid(bool) || typeId==typeid(jboolean)){
                result = new JBooleanTypeEntry(env, typeId, qt_name, java_name, java_class, value_size);
            }else if(typeId==typeid(wchar_t) || typeId==typeid(QChar) || typeId==typeid(QLatin1Char) || typeId==typeid(jchar)){
                result = new JCharTypeEntry(env, typeId, qt_name, java_name, java_class, value_size);
            }else if(typeId==typeid(qint32) || typeId==typeid(quint32) || typeId==typeid(jint)){
                result = new JIntTypeEntry(env, typeId, qt_name, java_name, java_class, value_size);
            }else if(typeId==typeid(qint16) || typeId==typeid(quint16) || typeId==typeid(jshort)){
                result = new JShortTypeEntry(env, typeId, qt_name, java_name, java_class, value_size);
            }else if(typeId==typeid(qint64) || typeId==typeid(quint64) || typeId==typeid(jlong)){
                result = new JLongTypeEntry(env, typeId, qt_name, java_name, java_class, value_size);
            }else if(typeId==typeid(qint8) || typeId==typeid(quint8) || typeId==typeid(jbyte)){
                result = new JByteTypeEntry(env, typeId, qt_name, java_name, java_class, value_size);
            }
            break;
        }
        case EntryTypes::SpecialTypeInfo:
        {
            size_t value_size = getValueSize(typeId);
            if(typeId==typeid(std::nullptr_t)){
                result = new NullptrTypeEntry(env, typeId, qt_name, nullptr, nullptr, value_size);
            }else if(typeId==typeid(QUrl::FormattingOptions)){
                jmethodID creator_method = resolveMethod(env, "<init>", "(I)V", java_class, false);
                Q_ASSERT(creator_method);
                result = new FlagsTypeEntry(env, typeId, qt_name, java_name, java_class, creator_method, value_size, nullptr);
            }else if(typeId==typeid(QMetaObject::Connection)
                     || typeId==typeid(QMetaObject)
                     || typeId==typeid(JIteratorWrapper)
                     || typeId==typeid(JCollectionWrapper)
                     || typeId==typeid(JMapWrapper)
                     || typeId==typeid(JObjectWrapper)
                     || typeId==typeid(JEnumWrapper)){
                result = new MetaUtilTypeEntry(env, typeId, qt_name, java_name, java_class, value_size);
            }else if(typeId==typeid(QModelIndex)){
                result = new QModelIndexTypeEntry(env, typeId, qt_name, java_name, java_class, value_size);
            }else if(typeId==typeid(QVariant)){
                result = new QVariantTypeEntry(env, typeId, qt_name, java_name, java_class, value_size);
#if QT_VERSION >= 0x050C00
            }else if(typeId==typeid(QCborValueRef)){
                result = new QCborValueRefTypeEntry(env, typeId, qt_name, java_name, java_class, value_size);
#endif
            }
            break;
        }
        default:
            return nullptr;
        }
    }

    {
        QWriteLocker locker(gTypeEntryLock());
        Q_UNUSED(locker)

        if(gTypeEntryHash()->contains(typeId.hash_code())){
            QMap<QByteArray,const QtJambiTypeEntry*>& entries = (*gTypeEntryHash())[typeId.hash_code()];
            if(qtName){
                if(const QtJambiTypeEntry* altresult = entries.value(qtName, nullptr)){
                    return altresult;
                }
            }else if(!entries.isEmpty()){
                return entries.first();
            }
        }
        (*gTypeEntryHash())[typeId.hash_code()][qt_name] = result;
    }
    return result;
}

void clear_typehandlers_at_shutdown(JNIEnv *){
    TypeEntryHash typeEntryHash;
    {
        QWriteLocker locker(gTypeEntryLock());
        Q_UNUSED(locker)
        typeEntryHash.swap(*gTypeEntryHash);
    }
    for(const QMap<QByteArray,const QtJambiTypeEntry*>& map : typeEntryHash){
        for(const QtJambiTypeEntry* entry : map){
            delete entry;
        }
    }
}

const QtJambiTypeEntry* QtJambiTypeEntry::getTypeEntry(JNIEnv* env, const std::type_info& typeId)
{
    return get_type_entry(env, typeId, true, nullptr);
}

const QtJambiTypeEntry* QtJambiTypeEntry::getTypeEntry(JNIEnv* env, const std::type_info& typeId, const char* qtName)
{
    return get_type_entry(env, typeId, true, qtName);
}

const QtJambiTypeEntry* QtJambiTypeEntry::getTypeEntryByJavaName(JNIEnv* env, const char* java_name)
{
    const std::type_info* typeId = getTypeByJavaName(java_name);
    if(typeId)
        return getTypeEntry(env, *typeId);
    else return nullptr;
}

const QtJambiTypeEntry* QtJambiTypeEntry::getTypeEntryByJavaName(JNIEnv* env, const QString& java_name)
{
    const std::type_info* typeId = getTypeByJavaName(java_name);
    if(typeId)
        return getTypeEntry(env, *typeId);
    else return nullptr;
}

const QtJambiTypeEntry* QtJambiTypeEntry::getTypeEntryByQtName(JNIEnv* env, const char* qt_name)
{
    const std::type_info* typeId = getTypeByQtName(qt_name);
    if(typeId)
        return getTypeEntry(env, *typeId, qt_name);
    else  return nullptr;
}

const QtJambiTypeEntry* QtJambiTypeEntry::getTypeEntryByIID(JNIEnv* env, const char* iid)
{
    const std::type_info* typeId = getInterfaceTypeForIID(iid);
    if(typeId)
        return getTypeEntry(env, *typeId);
    else  return nullptr;
}

bool QtJambiTypeEntry::isEnum() const {return false;}
bool QtJambiTypeEntry::isFlags() const {return false;}
bool QtJambiTypeEntry::isInterface() const {return false;}
bool QtJambiTypeEntry::isFunctional() const {return false;}
bool QtJambiTypeEntry::isQObject() const {return false;}
bool QtJambiTypeEntry::isObject() const {return false;}
bool QtJambiTypeEntry::isValue() const {return false;}

bool EnumTypeEntry::isEnum() const {return true;}
bool FlagsTypeEntry::isFlags() const {return true;}
bool InterfaceTypeEntry::isInterface() const {return true;}
bool FunctionalTypeEntry::isFunctional() const {return true;}
bool QObjectTypeEntry::isQObject() const {return true;}
bool ObjectTypeEntry::isObject() const {return true;}
bool InterfaceValueTypeEntry::isValue() const {return true;}
bool ObjectValueTypeEntry::isValue() const {return true;}
const QVector<const FunctionInfo>* ObjectTypeEntry::virtualFunctions() const { return m_virtualFunctions; }
const QVector<const FunctionInfo>* QObjectTypeEntry::virtualFunctions() const { return m_virtualFunctions; }
const QVector<const FunctionInfo>* InterfaceTypeEntry::virtualFunctions() const { return m_virtualFunctions; }
const QVector<const FunctionInfo>* FunctionalTypeEntry::virtualFunctions() const { return m_virtualFunctions; }

const FlagsTypeEntry* EnumTypeEntry::flagType() const
{
    return m_flagType;
}

const EnumTypeEntry* FlagsTypeEntry::enumType() const
{
    return m_enumType;
}

const std::type_info& QtJambiTypeEntry::type() const
{
    return m_typeId;
}

size_t QtJambiTypeEntry::valueSize() const
{
    return m_value_size;
}

uint QtJambiTypeEntry::offset(const std::type_info&) const{
    return 0;
}

const std::type_info* QtJambiTypeEntry::superType() const
{
    return nullptr;
}

const QList<const std::type_info*>& QtJambiTypeEntry::interfaceTypes() const
{
    static QList<const std::type_info*> emptyList = QList<const std::type_info*>();
    return emptyList;
}

jclass QtJambiTypeEntry::javaClass() const
{
    return m_java_class;
}

jclass QtJambiTypeEntry::creatableClass() const
{
    return m_java_class;
}

jmethodID QtJambiTypeEntry::creatorMethod() const
{
    return m_creator_method;
}

const std::type_info* ObjectTypeEntry::superType() const
{
    return m_super_type;
}

const QList<const std::type_info*>& ObjectTypeEntry::interfaceTypes() const
{
    return m_interface_types;
}

const std::type_info* QObjectTypeEntry::superType() const
{
    return m_super_type;
}

const QList<const std::type_info*>& QObjectTypeEntry::interfaceTypes() const
{
    return m_interface_types;
}

const std::type_info* InterfaceTypeEntry::superType() const
{
    return m_super_type;
}

size_t InterfaceTypeEntry::shellSize() const
{
    return m_shell_size;
}

size_t FunctionalTypeEntry::shellSize() const
{
    return m_shell_size;
}

size_t ObjectTypeEntry::shellSize() const
{
    return m_shell_size;
}

size_t QObjectTypeEntry::shellSize() const
{
    return m_shell_size;
}

const QList<const std::type_info*>& InterfaceTypeEntry::interfaceTypes() const
{
    return m_interface_types;
}

const char *QtJambiTypeEntry::qtName() const
{
    return m_qt_name;
}

const char *QtJambiTypeEntry::javaName() const
{
    return m_java_name;
}

jclass QObjectAbstractTypeEntry::creatableClass() const
{
    return m_java_wrapper_class;
}

jclass ObjectAbstractValueTypeEntry::creatableClass() const
{
    return m_java_wrapper_class;
}

jclass QObjectAbstractPolymorphicTypeEntry::creatableClass() const
{
    return m_java_wrapper_class;
}

jclass ObjectAbstractTypeEntry::creatableClass() const
{
    return m_java_wrapper_class;
}

jclass InterfaceTypeEntry::creatableClass() const
{
    return m_java_wrapper_class;
}

jclass InterfaceTypeEntry::implementationClass() const
{
    return m_java_impl_class;
}

const QMap<size_t, uint>& ObjectTypeEntry::interfaceOffsets() const
{
    return m_interface_offsets;
}

const QMap<size_t, uint>& InterfaceTypeEntry::interfaceOffsets() const
{
    return m_interface_offsets;
}

Destructor ObjectTypeEntry::destructor() const
{
    return m_destructor;
}

Destructor InterfaceTypeEntry::destructor() const
{
    return m_destructor;
}

Destructor FunctionalTypeEntry::destructor() const
{
    return m_destructor;
}

const QMetaObject* QObjectTypeEntry::originalMetaObject() const
{
    return m_original_meta_object;
}

uint InterfaceTypeEntry::offset(const std::type_info& toType) const{
    return m_interface_offsets.value(toType.hash_code(), 0);
}

uint ObjectTypeEntry::offset(const std::type_info& toType) const{
    return m_interface_offsets.value(toType.hash_code(), 0);
}

uint QObjectTypeEntry::offset(const std::type_info& toType) const{
    return m_interface_offsets.value(toType.hash_code(), 0);
}

FunctionalResolver FunctionalTypeEntry::registeredFunctionalResolver() const
{
    return m_registered_functional_resolver;
}

jclass FunctionalTypeEntry::creatableClass() const
{
    return m_java_wrapper_class;
}

QtJambiTypeEntry::QtJambiTypeEntry(JNIEnv* env, const std::type_info& typeId, const char *qt_name, const char *java_name, jclass java_class, jmethodID creator_method, size_t value_size)
    : m_typeId(typeId),
      m_qt_name(qt_name),
      m_java_name(java_name),
      m_java_class(java_class ? getGlobalClassRef(env, java_class) : nullptr),
      m_creator_method(creator_method),
      m_value_size(value_size)
{
}

QtJambiTypeEntry::~QtJambiTypeEntry()
{
}

EnumTypeEntry::EnumTypeEntry(JNIEnv* env, const std::type_info& typeId, const char *qt_name, const char *java_name, jclass java_class, jmethodID creator_method, size_t value_size)
    : QtJambiTypeEntry(env, typeId, qt_name, java_name, java_class, creator_method, value_size),
      m_flagType(nullptr)
{

}

FlagsTypeEntry::FlagsTypeEntry(JNIEnv* env, const std::type_info& typeId, const char *qt_name, const char *java_name, jclass java_class, jmethodID creator_method, size_t value_size, const EnumTypeEntry* enumType)
    : QtJambiTypeEntry(env, typeId, qt_name, java_name, java_class, creator_method, value_size),
      m_enumType(enumType)
{
    if(enumType)
        const_cast<EnumTypeEntry*>(enumType)->m_flagType = this;
}

InterfaceTypeEntry::~InterfaceTypeEntry()
{
}

InterfaceTypeEntry::InterfaceTypeEntry(JNIEnv* env, const std::type_info& typeId,
                   const char *qt_name,
                   const char *java_name,
                   jclass java_class,
                   jmethodID creator_method,
                   size_t value_size,
                   jclass java_impl_class,
                   jclass java_wrapper_class,
                   const std::type_info* super_type,
                   const QList<const std::type_info*>& interface_types,
                   const QMap<size_t, uint>& interface_offsets,
                   size_t shell_size,
                   PtrDeleterFunction deleter,
                   PtrOwnerFunction owner_function,
                   const QVector<const FunctionInfo>* virtualFunctions,
                   Destructor destructor,
                   const QList<const PolymorphicIdHandler*>& polymorphicHandlers
                )
    : QtJambiTypeEntry(env, typeId, qt_name, java_name, java_class, creator_method, value_size),
      m_java_impl_class(java_impl_class ? getGlobalClassRef(env, java_impl_class) : nullptr),
      m_java_wrapper_class(java_wrapper_class ? getGlobalClassRef(env, java_wrapper_class) : nullptr),
      m_super_type(super_type),
      m_interface_types(interface_types),
      m_interface_offsets(interface_offsets),
      m_shell_size(shell_size),
      m_deleter(deleter),
      m_owner_function(owner_function),
      m_virtualFunctions(virtualFunctions),
      m_destructor(destructor),
      m_polymorphicIdHandlers(polymorphicHandlers)
{

}

InterfaceValueTypeEntry::InterfaceValueTypeEntry(JNIEnv* env, const std::type_info& typeId,
                   const char *qt_name,
                   const char *java_name,
                   jclass java_class,
                   jmethodID creator_method,
                   size_t value_size,
                   jclass java_impl_class,
                   jclass java_wrapper_class,
                   const std::type_info* super_type,
                   const QList<const std::type_info*>& interface_types,
                   const QMap<size_t, uint>& interface_offsets,
                   size_t shell_size,
                   PtrDeleterFunction deleter,
                   PtrOwnerFunction owner_function,
                   const QVector<const FunctionInfo>* virtualFunctions,
                   Destructor destructor,
                   const QList<const PolymorphicIdHandler*>& polymorphicHandlers,
                   const QMetaType& qt_meta_type
                )
    : InterfaceTypeEntry(env, typeId, qt_name, java_name, java_class, creator_method, value_size, java_impl_class, java_wrapper_class,
                         super_type, interface_types, interface_offsets, shell_size, deleter, owner_function, virtualFunctions, destructor,
                         polymorphicHandlers),
      m_qt_meta_type(qt_meta_type
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
                     .id()
#endif
                     )
{

}

FunctionalTypeEntry::~FunctionalTypeEntry()
{
}

FunctionalTypeEntry::FunctionalTypeEntry(JNIEnv* env, const std::type_info& typeId,
                                         const char *qt_name,
                                         const char *java_name,
                                         jclass java_class,
                                         jmethodID creator_method,
                                         size_t value_size,
                                         size_t shell_size,
                                         jclass java_impl_class,
                                         jclass java_wrapper_class,
                                         const QVector<const FunctionInfo>* virtualFunctions,
                                         Destructor destructor,
                                         FunctionalResolver registered_functional_resolver,
                                         const QMetaType& qt_meta_type
                )
    : QtJambiTypeEntry(env, typeId, qt_name, java_name, java_class, creator_method, value_size),
      m_shell_size(shell_size),
      m_java_impl_class(java_impl_class ? getGlobalClassRef(env, java_impl_class) : nullptr),
      m_java_wrapper_class(java_wrapper_class ? getGlobalClassRef(env, java_wrapper_class) : nullptr),
      m_virtualFunctions(virtualFunctions),
      m_destructor(destructor),
      m_registered_functional_resolver(registered_functional_resolver),
      m_qt_meta_type(qt_meta_type
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
                        .id()
#endif
                     )
{

}

ObjectTypeEntry::ObjectTypeEntry(JNIEnv* env,
                const std::type_info& typeId,
                const char *qt_name,
                const char *java_name,
                jclass java_class,
                jmethodID creator_method,
                size_t value_size,
                const std::type_info* super_type,
                const QList<const std::type_info*>& interface_types,
                const QMap<size_t, uint>& interface_offsets,
                size_t shell_size,
                PtrDeleterFunction deleter,
                PtrOwnerFunction owner_function,
                const QVector<const FunctionInfo>* virtualFunctions,
                Destructor destructor,
                const QList<const PolymorphicIdHandler*>& polymorphicHandlers
            )
    : QtJambiTypeEntry(env, typeId, qt_name, java_name, java_class, creator_method, value_size),
      m_super_type(super_type),
      m_interface_types(interface_types),
      m_interface_offsets(interface_offsets),
      m_shell_size(shell_size),
      m_deleter(deleter),
      m_owner_function(owner_function),
      m_virtualFunctions(virtualFunctions),
      m_destructor(destructor),
      m_polymorphicIdHandlers(polymorphicHandlers)
{

}

ObjectAbstractTypeEntry::ObjectAbstractTypeEntry(JNIEnv* env,
                const std::type_info& typeId,
                const char *qt_name,
                const char *java_name,
                jclass java_class,
                jmethodID creator_method,
                size_t value_size,
                const std::type_info* super_type,
                const QList<const std::type_info*>& interface_types,
                const QMap<size_t, uint>& interface_offsets,
                size_t shell_size,
                PtrDeleterFunction deleter,
                PtrOwnerFunction owner_function,
                const QVector<const FunctionInfo>* virtualFunctions,
                Destructor destructor,
                const QList<const PolymorphicIdHandler*>& polymorphicHandlers,
                jclass java_wrapper_class
            )
    :ObjectTypeEntry(env,
                     typeId,
                     qt_name,
                     java_name,
                     java_class,
                     creator_method,
                     value_size,
                     super_type,
                     interface_types,
                     interface_offsets,
                     shell_size,
                     deleter,
                     owner_function,
                     virtualFunctions,
                     destructor,
                     polymorphicHandlers
                 ),
      m_java_wrapper_class(java_wrapper_class ? getGlobalClassRef(env, java_wrapper_class) : nullptr)
{

}

ObjectValueTypeEntry::ObjectValueTypeEntry(JNIEnv* env,
                const std::type_info& typeId,
                const char *qt_name,
                const char *java_name,
                jclass java_class,
                jmethodID creator_method,
                size_t value_size,
                const std::type_info* super_type,
                const QList<const std::type_info*>& interface_types,
                const QMap<size_t, uint>& interface_offsets,
                size_t shell_size,
                PtrOwnerFunction owner_function,
                const QVector<const FunctionInfo>* virtualFunctions,
                Destructor destructor,
                const QList<const PolymorphicIdHandler*>& polymorphicHandlers,
                const QMetaType& qt_meta_type
            )
    :ObjectTypeEntry(env,
                     typeId,
                     qt_name,
                     java_name,
                     java_class,
                     creator_method,
                     value_size,
                     super_type,
                     interface_types,
                     interface_offsets,
                     shell_size,
                     nullptr,
                     owner_function,
                     virtualFunctions,
                     destructor,
                     polymorphicHandlers
                 ),
      m_qt_meta_type(qt_meta_type
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
                    .id()
#endif
                  )
{

}

ObjectAbstractValueTypeEntry::ObjectAbstractValueTypeEntry(JNIEnv* env,
                const std::type_info& typeId,
                const char *qt_name,
                const char *java_name,
                jclass java_class,
                jmethodID creator_method,
                size_t value_size,
                const std::type_info* super_type,
                const QList<const std::type_info*>& interface_types,
                const QMap<size_t, uint>& interface_offsets,
                size_t shell_size,
                PtrOwnerFunction owner_function,
                const QVector<const FunctionInfo>* virtualFunctions,
                Destructor destructor,
                const QList<const PolymorphicIdHandler*>& polymorphicHandlers,
                const QMetaType& qt_meta_type,
                jclass java_wrapper_class
            )
    : ObjectValueTypeEntry(env,
                     typeId,
                     qt_name,
                     java_name,
                     java_class,
                     creator_method,
                     value_size,
                     super_type,
                     interface_types,
                     interface_offsets,
                     shell_size,
                     owner_function,
                     virtualFunctions,
                     destructor,
                     polymorphicHandlers,
                     qt_meta_type
                 ),
      m_java_wrapper_class(java_wrapper_class ? getGlobalClassRef(env, java_wrapper_class) : nullptr)
{

}

QObjectTypeEntry::QObjectTypeEntry(JNIEnv* env,
                 const std::type_info& typeId,
                 const char *qt_name,
                 const char *java_name,
                 jclass java_class,
                 jmethodID creator_method,
                 size_t value_size,
                 const std::type_info* super_type,
                 const QList<const std::type_info*>& interface_types,
                 const QMap<size_t, uint>& interface_offsets,
                 size_t shell_size,
                 const QVector<const FunctionInfo>* virtualFunctions,
                 const QMetaObject* original_meta_object
            )
    : QtJambiTypeEntry(env, typeId, qt_name, java_name, java_class, creator_method, value_size),
      m_super_type(super_type),
      m_interface_types(interface_types),
      m_interface_offsets(interface_offsets),
      m_shell_size(shell_size),
      m_virtualFunctions(virtualFunctions),
      m_original_meta_object(original_meta_object)
{

}

QThreadTypeEntry::QThreadTypeEntry(JNIEnv* env,
                 const std::type_info& typeId,
                 const char *qt_name,
                 const char *java_name,
                 jclass java_class,
                 jmethodID creator_method,
                 size_t value_size,
                 const std::type_info* super_type,
                 const QList<const std::type_info*>& interface_types,
                 const QMap<size_t, uint>& interface_offsets,
                 size_t shell_size,
                 const QVector<const FunctionInfo>* virtualFunctions,
                 const QMetaObject* original_meta_object
            )
    : QObjectTypeEntry(env, typeId, qt_name, java_name, java_class, creator_method, value_size,
                       super_type, interface_types, interface_offsets,
                       shell_size, virtualFunctions, original_meta_object)
{
}

jobject qtjambi_from_thread_impl(JNIEnv * env, jobject java_qthread, QThread *thread);

bool QThreadTypeEntry::convertToJava(JNIEnv *env, const void *qt_object, bool makeCopyOfValueTypes, bool cppOwnership, jvalue* output, jValueType valueType) const
{
    bool result = QObjectTypeEntry::convertToJava(env, qt_object, makeCopyOfValueTypes, cppOwnership, output, valueType);
    if(result){
        QThread* thread = const_cast<QThread*>(reinterpret_cast<const QThread*>(qt_object));
        if (thread){
            QThreadUserData* data;
            {
                QReadLocker locker(QtJambiLinkUserData::lock());
                data = QTJAMBI_GET_OBJECTUSERDATA(QThreadUserData, thread);
            }
            if(!data)
                qtjambi_from_thread_impl(env, output->l, thread);
        }
    }
    return result;
}

bool QThreadTypeEntry::convertSharedPointerToJava(JNIEnv *env, void *ptr_shared_pointer, PointerDeleter sharedPointerDeleter, PointerGetterFunction sharedPointerGetter, jvalue* output, jValueType valueType) const
{
    bool result = QObjectTypeEntry::convertSharedPointerToJava(env, ptr_shared_pointer, sharedPointerDeleter, sharedPointerGetter, output, valueType);
    if(result){
        QThread* thread = reinterpret_cast<QThread*>(sharedPointerGetter(ptr_shared_pointer));
        if (thread){
            QThreadUserData* data;
            {
                QReadLocker locker(QtJambiLinkUserData::lock());
                data = QTJAMBI_GET_OBJECTUSERDATA(QThreadUserData, thread);
            }
            if(!data)
                qtjambi_from_thread_impl(env, output->l, thread);
        }
    }
    return result;
}

QObjectAbstractTypeEntry::QObjectAbstractTypeEntry(JNIEnv* env,
                 const std::type_info& typeId,
                 const char *qt_name,
                 const char *java_name,
                 jclass java_class,
                 jmethodID creator_method,
                 size_t value_size,
                 const std::type_info* super_type,
                 const QList<const std::type_info*>& interface_types,
                 const QMap<size_t, uint>& interface_offsets,
                 size_t shell_size,
                 const QVector<const FunctionInfo>* virtualFunctions,
                 const QMetaObject* original_meta_object,
                 jclass java_wrapper_class
            )
    : QObjectTypeEntry(env, typeId, qt_name, java_name, java_class, creator_method, value_size,
                      super_type, interface_types, interface_offsets,
                      shell_size, virtualFunctions, original_meta_object),
      m_java_wrapper_class(java_wrapper_class ? getGlobalClassRef(env, java_wrapper_class) : nullptr)
{

}

QObjectAbstractPolymorphicTypeEntry::QObjectAbstractPolymorphicTypeEntry(JNIEnv* env,
                 const std::type_info& typeId,
                 const char *qt_name,
                 const char *java_name,
                 jclass java_class,
                 jmethodID creator_method,
                 size_t value_size,
                 const std::type_info* super_type,
                 const QList<const std::type_info*>& interface_types,
                 const QMap<size_t, uint>& interface_offsets,
                 size_t shell_size,
                 const QVector<const FunctionInfo>* virtualFunctions,
                 const QMetaObject* original_meta_object,
                 const QList<const PolymorphicIdHandler*>& polymorphicIdHandlers,
                 jclass java_wrapper_class
            )
    : QObjectPolymorphicTypeEntry(env, typeId, qt_name, java_name, java_class, creator_method, value_size,
                      super_type, interface_types, interface_offsets,
                      shell_size, virtualFunctions, original_meta_object, polymorphicIdHandlers),
      m_java_wrapper_class(java_wrapper_class ? getGlobalClassRef(env, java_wrapper_class) : nullptr)
{

}

QObjectPolymorphicTypeEntry::QObjectPolymorphicTypeEntry(JNIEnv* env,
                 const std::type_info& typeId,
                 const char *qt_name,
                 const char *java_name,
                 jclass java_class,
                 jmethodID creator_method,
                 size_t value_size,
                 const std::type_info* super_type,
                 const QList<const std::type_info*>& interface_types,
                 const QMap<size_t, uint>& interface_offsets,
                 size_t shell_size,
                 const QVector<const FunctionInfo>* virtualFunctions,
                 const QMetaObject* original_meta_object,
                 const QList<const PolymorphicIdHandler*>& polymorphicIdHandlers
            )
    : QObjectTypeEntry(env, typeId, qt_name, java_name, java_class, creator_method, value_size,
                       super_type, interface_types, interface_offsets,
                       shell_size, virtualFunctions, original_meta_object),
      m_polymorphicIdHandlers(polymorphicIdHandlers)
{

}

const std::type_info* qtjambi_resolve_polymorphy(const QList<const PolymorphicIdHandler*>& polymorphicIdHandlers, const void *object)
{
    if(object && !polymorphicIdHandlers.isEmpty()){
        void *_object = const_cast<void *>(object);
        for(const PolymorphicIdHandler* handler : polymorphicIdHandlers){
            Q_ASSERT(handler->m_polymorphyHandler);
            qintptr offset = 0;
            if(handler->m_polymorphyHandler(_object, offset)) {
                return &handler->m_targetTypeId;
            }
        }
    }
    return nullptr;
}

bool InterfaceTypeEntry::convertToJava(JNIEnv *env, const void *qt_object, bool makeCopyOfValueTypes, bool cppOwnership, jvalue* output, jValueType javaType) const{
    if(javaType!=jValueType::l)
        JavaException::raiseIllegalArgumentException(env, "Cannot convert object type" QTJAMBI_STACKTRACEINFO );
    if (!qt_object){
        output->l = nullptr;
        return true;
    }
    for(const QSharedPointer<QtJambiLink>& link : QtJambiLink::findLinksForPointer(qt_object)){
        if(link){
            jobject obj = link->getJavaObjectLocalRef(env);
            if(obj && env->IsInstanceOf(obj, javaClass())){
                output->l = obj;
                return true;
            }
        }
    }
    const std::type_info* _typeId = qtjambi_resolve_polymorphy(m_polymorphicIdHandlers, qt_object);
    if(_typeId && *_typeId!=type()){
        if(const QtJambiTypeEntry* typeEntry = QtJambiTypeEntry::getTypeEntry(env, *_typeId)){
            uint offset = typeEntry->offset(type());
            return typeEntry->convertToJava(env, reinterpret_cast<const void*>( qintptr(qt_object) - offset ), makeCopyOfValueTypes, cppOwnership, output, javaType);
        }else{
            return false;
        }
    }else{
        if(!creatableClass() || !creatorMethod())
            JavaException::raiseError(env, "Uncreatable type" QTJAMBI_STACKTRACEINFO );
        output->l = env->NewObject(creatableClass(), creatorMethod(), nullptr);
        qtjambi_throw_java_exception(env);
        const QSharedPointer<QtJambiLink>& link = QtJambiLink::createLinkForObject(
                env,
                output->l,
                const_cast<void*>(qt_object),
                LINK_NAME_ARG(qtName())
                false,
                false,
                m_deleter,
                m_owner_function
            );
        if(cppOwnership){
            link->setCppOwnership(env);
        }// split ownership
        return true;
    }
}

bool InterfaceTypeEntry::convertSharedPointerToJava(JNIEnv *env, void *ptr_shared_pointer, PointerDeleter sharedPointerDeleter, PointerGetterFunction sharedPointerGetter, jvalue* output, jValueType javaType) const{
    if(javaType!=jValueType::l)
        JavaException::raiseIllegalArgumentException(env, "Cannot convert object type" QTJAMBI_STACKTRACEINFO );
    if (!ptr_shared_pointer || !sharedPointerDeleter || !sharedPointerGetter){
        output->l = nullptr;
        return true;
    }
    void* qt_object = sharedPointerGetter(ptr_shared_pointer);
    if (!qt_object){
        output->l = nullptr;
        return true;
    }
    for(QSharedPointer<QtJambiLink>& link : QtJambiLink::findLinksForPointer(qt_object)){
        if(link && !link->isSharedPointer()){
            jobject obj = link->getJavaObjectLocalRef(env);
            if(obj && env->IsInstanceOf(obj, javaClass())){
                output->l = obj;
                bool createdByJava = link->createdByJava();
                bool is_shell = link->isShell();
                QtJambiShellAccess* shell = nullptr;
                if(is_shell){
                    const SuperTypeInfos& infos = getSuperTypeInfos(env, env->GetObjectClass(output->l));
                    Q_ASSERT(infos.size());
                    shell = *reinterpret_cast<QtJambiShellAccess**>( quintptr(qt_object) + infos.at(0).size() );
                }
                jobject nativeLink = link->nativeLink(env);
                if(nativeLink){
                    Java::QtJambi::QtJambiInternal$NativeLink::reset(env, nativeLink);
                }
                PtrOwnerFunction registeredThreadAffinityFunction = nullptr;
                if(!link->isQObject()){
                    PointerToObjectLink* polink = static_cast<PointerToObjectLink*>(link.data());
                    registeredThreadAffinityFunction = polink->ownerFunction();
                }
                link->invalidate(env);
                link.clear();
                link = QtJambiLink::createLinkForSharedPointerToObject(env, output->l,
                                                                           LINK_NAME_ARG(qtName())
                                                                           createdByJava,
                                                                           is_shell,
                                                                           registeredThreadAffinityFunction,
                                                                           ptr_shared_pointer,
                                                                           sharedPointerDeleter,
                                                                           sharedPointerGetter);
                if(shell){
                    shell->overrideLink(link);
                }
                env->DeleteLocalRef(nativeLink);
                return true;
            }
        }
    }
    const std::type_info* _typeId = qtjambi_resolve_polymorphy(m_polymorphicIdHandlers, qt_object);
    if(_typeId && *_typeId!=type()){
        if(const QtJambiTypeEntry* typeEntry = QtJambiTypeEntry::getTypeEntry(env, *_typeId)){
            PointerGetterFunction _sharedPointerGetter = sharedPointerGetter;
            uint offset = typeEntry->offset(type());
            if(offset>0){
                _sharedPointerGetter = [sharedPointerGetter,offset](void* sharedPtr)-> void* {
                    void* ptr = sharedPointerGetter(sharedPtr);
                    return reinterpret_cast<void*>( qintptr(ptr) - offset );
                };
            }
            return typeEntry->convertSharedPointerToJava(env, ptr_shared_pointer, sharedPointerDeleter, _sharedPointerGetter, output, javaType);
        }else{
            return false;
        }
    }else{
        if(!creatableClass() || !creatorMethod())
            JavaException::raiseError(env, "Uncreatable type" QTJAMBI_STACKTRACEINFO );
        output->l = env->NewObject(creatableClass(), creatorMethod(), nullptr);
        qtjambi_throw_java_exception(env);
        const QSharedPointer<QtJambiLink>& link = QtJambiLink::createLinkForSharedPointerToObject(
                env,
                output->l,
                LINK_NAME_ARG(qtName())
                false,
                false,
                m_owner_function,
                ptr_shared_pointer,
                sharedPointerDeleter,
                sharedPointerGetter
            );
        link->setJavaOwnership(env);
        return true;
    }
}

bool InterfaceTypeEntry::convertToNative(JNIEnv *env, const jvalue& java_value, jValueType javaType, void * output) const{
    if(javaType!=jValueType::l)
        JavaException::raiseIllegalArgumentException(env, "Cannot convert to object type" QTJAMBI_STACKTRACEINFO );
    if(java_value.l && !env->IsInstanceOf(java_value.l, javaClass()))
        return false;
    if (QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForJavaInterface(env, java_value.l)){
        if(!link->isMultiInheritanceType()){
            void *ptr = link->pointer();
            *reinterpret_cast<void**>(output) = ptr;
            return ptr;
        }else if(link->isInterfaceAvailable(type())){
            void *ptr = link->typedPointer(type());
            *reinterpret_cast<void**>(output) = ptr;
            return ptr;
        }else if(link->isQObject()){
            if(QObject* object = link->qobject()){
                void* ptr = object->qt_metacast(qtName());
                if(ptr){
                    *reinterpret_cast<void**>(output) = ptr;
                    return true;
                }else{
                    QByteArray typeName = qtName();
                    if(typeName.endsWith("*")){
                        typeName = typeName.chopped(1);
                        ptr = object->qt_metacast(typeName);
                        if(ptr){
                            *reinterpret_cast<void**>(output) = ptr;
                            return true;
                        }
                    }
                }
            }
        }
        return false;
    }
    else if(Java::QtJambi::QtObjectInterface::isInstanceOf(env, java_value.l))
        Java::QtJambi::QNoNativeResourcesException::throwNew(env, QString("Incomplete object of type: %1").arg(qtjambi_object_class_name(env, java_value.l).replace("$", ".")) QTJAMBI_STACKTRACEINFO );
    return true;
}

bool InterfaceValueTypeEntry::convertToJava(JNIEnv *env, const void *qt_object, bool makeCopyOfValueTypes, bool cppOwnership, jvalue* output, jValueType javaType) const{
    if(javaType!=jValueType::l)
        JavaException::raiseIllegalArgumentException(env, "Cannot convert object type" QTJAMBI_STACKTRACEINFO );
    if (!qt_object){
        output->l = nullptr;
        return true;
    }
    if(!makeCopyOfValueTypes){
        for(const QSharedPointer<QtJambiLink>& link : QtJambiLink::findLinksForPointer(qt_object)){
            if(link){
                jobject obj = link->getJavaObjectLocalRef(env);
                if(obj && env->IsInstanceOf(obj, javaClass())){
                    output->l = obj;
                    return true;
                }
            }
        }
    }
    const std::type_info* _typeId = qtjambi_resolve_polymorphy(m_polymorphicIdHandlers, qt_object);
    if(_typeId && *_typeId!=type()){
        if(const QtJambiTypeEntry* typeEntry = QtJambiTypeEntry::getTypeEntry(env, *_typeId)){
            uint offset = typeEntry->offset(type());
            return typeEntry->convertToJava(env, reinterpret_cast<const void*>( qintptr(qt_object) - offset ), makeCopyOfValueTypes, cppOwnership, output, javaType);
        }else{
            return false;
        }
    }else{
        void *copy = makeCopyOfValueTypes ? m_qt_meta_type.create(qt_object) : const_cast<void*>(qt_object);
        if (!copy){
            output->l = nullptr;
            return true;
        }
        if(!creatableClass() || !creatorMethod())
            JavaException::raiseError(env, "Uncreatable type" QTJAMBI_STACKTRACEINFO );
        output->l = env->NewObject(creatableClass(), creatorMethod(), nullptr);
        qtjambi_throw_java_exception(env);
        const QSharedPointer<QtJambiLink>& link = QtJambiLink::createLinkForObject(
                env,
                output->l,
                copy,
                m_qt_meta_type,
                false,
                false,
                m_owner_function
            );
        if(makeCopyOfValueTypes){
            link->setJavaOwnership(env);
        }else if(cppOwnership){
            link->setCppOwnership(env);
        }
        return true;
    }
}

bool InterfaceValueTypeEntry::convertSharedPointerToJava(JNIEnv *env, void *ptr_shared_pointer, PointerDeleter sharedPointerDeleter, PointerGetterFunction sharedPointerGetter, jvalue* output, jValueType javaType) const{
    if(javaType!=jValueType::l)
        JavaException::raiseIllegalArgumentException(env, "Cannot convert object type" QTJAMBI_STACKTRACEINFO );
    if (!ptr_shared_pointer || !sharedPointerDeleter || !sharedPointerGetter){
        output->l = nullptr;
        return true;
    }
    void* qt_object = sharedPointerGetter(ptr_shared_pointer);
    if (!qt_object){
        output->l = nullptr;
        return true;
    }

    for(QSharedPointer<QtJambiLink>& link : QtJambiLink::findLinksForPointer(qt_object)){
        if(link && !link->isSharedPointer()){
            jobject obj = link->getJavaObjectLocalRef(env);
            if(obj && env->IsInstanceOf(obj, javaClass())){
                output->l = obj;
                bool createdByJava = link->createdByJava();
                bool is_shell = link->isShell();
                QtJambiShellAccess* shell = nullptr;
                if(is_shell){
                    const SuperTypeInfos& infos = getSuperTypeInfos(env, env->GetObjectClass(output->l));
                    Q_ASSERT(infos.size());
                    shell = *reinterpret_cast<QtJambiShellAccess**>( quintptr(qt_object) + infos.at(0).size() );
                }
                jobject nativeLink = link->nativeLink(env);
                if(nativeLink){
                    Java::QtJambi::QtJambiInternal$NativeLink::reset(env, nativeLink);
                }
                PtrOwnerFunction registeredThreadAffinityFunction = nullptr;
                if(!link->isQObject()){
                    PointerToObjectLink* polink = static_cast<PointerToObjectLink*>(link.data());
                    registeredThreadAffinityFunction = polink->ownerFunction();
                }
                link->invalidate(env);
                link.clear();
                link = QtJambiLink::createLinkForSharedPointerToObject(env, output->l,
                                                                           LINK_NAME_ARG(qtName())
                                                                           createdByJava,
                                                                           is_shell,
                                                                           registeredThreadAffinityFunction,
                                                                           ptr_shared_pointer,
                                                                           sharedPointerDeleter,
                                                                           sharedPointerGetter);
                if(shell){
                    shell->overrideLink(link);
                }
                env->DeleteLocalRef(nativeLink);
                return true;
            }
        }
    }
    const std::type_info* _typeId = qtjambi_resolve_polymorphy(m_polymorphicIdHandlers, qt_object);
    if(_typeId && *_typeId!=type()){
        if(const QtJambiTypeEntry* typeEntry = QtJambiTypeEntry::getTypeEntry(env, *_typeId)){
            PointerGetterFunction _sharedPointerGetter = sharedPointerGetter;
            uint offset = m_interface_offsets.value(type().hash_code(), 0);
            if(offset>0){
                _sharedPointerGetter = [sharedPointerGetter,offset](void* sharedPtr)-> void* {
                    void* ptr = sharedPointerGetter(sharedPtr);
                    return reinterpret_cast<void*>( qintptr(ptr) - offset );
                };
            }
            return typeEntry->convertSharedPointerToJava(env, ptr_shared_pointer, sharedPointerDeleter, _sharedPointerGetter, output, javaType);
        }else{
            return false;
        }
    }else{
        if(!creatableClass() || !creatorMethod())
            JavaException::raiseError(env, "Uncreatable type" QTJAMBI_STACKTRACEINFO );
        output->l = env->NewObject(creatableClass(), creatorMethod(), nullptr);
        qtjambi_throw_java_exception(env);
        const QSharedPointer<QtJambiLink>& link = QtJambiLink::createLinkForSharedPointerToObject(
                env,
                output->l,
                LINK_NAME_ARG(qtName())
                false,
                false,
                m_owner_function,
                ptr_shared_pointer,
                sharedPointerDeleter,
                sharedPointerGetter
            );
        link->setJavaOwnership(env);
        return true;
    }
}

bool ObjectTypeEntry::convertToJava(JNIEnv *env, const void *qt_object, bool makeCopyOfValueTypes, bool cppOwnership, jvalue* output, jValueType javaType) const{
    if(javaType!=jValueType::l)
        JavaException::raiseIllegalArgumentException(env, "Cannot convert object type" QTJAMBI_STACKTRACEINFO );
    if (!qt_object){
        output->l = nullptr;
        return true;
    }
    for(const QSharedPointer<QtJambiLink>& link : QtJambiLink::findLinksForPointer(qt_object)){
        if(link){
            jobject obj = link->getJavaObjectLocalRef(env);
            if(obj && env->IsInstanceOf(obj, javaClass())){
                output->l = obj;
                return true;
            }
        }
    }
    const std::type_info* _typeId = qtjambi_resolve_polymorphy(m_polymorphicIdHandlers, qt_object);
    if(_typeId && *_typeId!=type()){
        if(const QtJambiTypeEntry* typeEntry = QtJambiTypeEntry::getTypeEntry(env, *_typeId)){
            return typeEntry->convertToJava(env, qt_object, makeCopyOfValueTypes, cppOwnership, output, javaType);
        }else{
            return false;
        }
    }else{
        if(!creatableClass() || !creatorMethod())
            JavaException::raiseError(env, "Uncreatable type" QTJAMBI_STACKTRACEINFO );
        output->l = env->NewObject(creatableClass(), creatorMethod(), nullptr);
        qtjambi_throw_java_exception(env);
        const QSharedPointer<QtJambiLink>& link = QtJambiLink::createLinkForObject(
                env,
                output->l,
                const_cast<void*>(qt_object),
                LINK_NAME_ARG(qtName())
                false,
                false,
                m_deleter,
                m_owner_function
            );
        if(cppOwnership){
            link->setCppOwnership(env);
        } // split ownership
        return true;
    }
}

bool ObjectTypeEntry::convertSharedPointerToJava(JNIEnv *env, void *ptr_shared_pointer, PointerDeleter sharedPointerDeleter, PointerGetterFunction sharedPointerGetter, jvalue* output, jValueType javaType) const{
    if(javaType!=jValueType::l)
        JavaException::raiseIllegalArgumentException(env, "Cannot convert object type" QTJAMBI_STACKTRACEINFO );
    if (!ptr_shared_pointer || !sharedPointerDeleter || !sharedPointerGetter){
        output->l = nullptr;
        return true;
    }
    void* qt_object = sharedPointerGetter(ptr_shared_pointer);
    if (!qt_object){
        output->l = nullptr;
        return true;
    }
    for(QSharedPointer<QtJambiLink>& link : QtJambiLink::findLinksForPointer(qt_object)){
        if(link && !link->isSharedPointer()){
            jobject obj = link->getJavaObjectLocalRef(env);
            if(obj && env->IsInstanceOf(obj, javaClass())){
                output->l = obj;
                bool createdByJava = link->createdByJava();
                bool is_shell = link->isShell();
                QtJambiShellAccess* shell = nullptr;
                if(is_shell){
                    const SuperTypeInfos& infos = getSuperTypeInfos(env, env->GetObjectClass(output->l));
                    Q_ASSERT(infos.size());
                    shell = *reinterpret_cast<QtJambiShellAccess**>( quintptr(qt_object) + infos.at(0).size() );
                }
                jobject nativeLink = link->nativeLink(env);
                if(nativeLink){
                    Java::QtJambi::QtJambiInternal$NativeLink::reset(env, nativeLink);
                }
                PtrOwnerFunction registeredThreadAffinityFunction = nullptr;
                if(!link->isQObject()){
                    PointerToObjectLink* polink = static_cast<PointerToObjectLink*>(link.data());
                    registeredThreadAffinityFunction = polink->ownerFunction();
                }
                link->invalidate(env);
                link.clear();
                link = QtJambiLink::createLinkForSharedPointerToObject(env, output->l,
                                                                           LINK_NAME_ARG(qtName())
                                                                           createdByJava,
                                                                           is_shell,
                                                                           registeredThreadAffinityFunction,
                                                                           ptr_shared_pointer,
                                                                           sharedPointerDeleter,
                                                                           sharedPointerGetter);
                if(shell){
                    shell->overrideLink(link);
                }
                env->DeleteLocalRef(nativeLink);
                return true;
            }
        }
    }
    const std::type_info* _typeId = qtjambi_resolve_polymorphy(m_polymorphicIdHandlers, qt_object);
    if(_typeId && *_typeId!=type()){
        if(const QtJambiTypeEntry* typeEntry = QtJambiTypeEntry::getTypeEntry(env, *_typeId)){
            PointerGetterFunction _sharedPointerGetter = sharedPointerGetter;
            uint offset = m_interface_offsets.value(_typeId->hash_code(), 0);
            if(offset>0){
                _sharedPointerGetter = [sharedPointerGetter,offset](void* sharedPtr)-> void* {
                    void* ptr = sharedPointerGetter(sharedPtr);
                    return reinterpret_cast<void*>( qintptr(ptr) - offset );
                };
            }
            return typeEntry->convertSharedPointerToJava(env, ptr_shared_pointer, sharedPointerDeleter, sharedPointerGetter, output, javaType);
        }else{
            return false;
        }
    }else{
        if(!creatableClass() || !creatorMethod())
            JavaException::raiseError(env, "Uncreatable type" QTJAMBI_STACKTRACEINFO );
        output->l = env->NewObject(creatableClass(), creatorMethod(), nullptr);
        qtjambi_throw_java_exception(env);
        const QSharedPointer<QtJambiLink>& link = QtJambiLink::createLinkForSharedPointerToObject(
                env,
                output->l,
                LINK_NAME_ARG(qtName())
                false,
                false,
                m_owner_function,
                ptr_shared_pointer,
                sharedPointerDeleter,
                sharedPointerGetter
            );
        link->setJavaOwnership(env);
        return true;
    }
}

bool ObjectTypeEntry::convertToNative(JNIEnv *env, const jvalue& java_value, jValueType javaType, void * output) const{
    if(javaType!=jValueType::l)
        JavaException::raiseIllegalArgumentException(env, "Cannot convert to object type" QTJAMBI_STACKTRACEINFO );
    if(env->IsInstanceOf(java_value.l, this->javaClass())){
        if (QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForJavaObject(env, java_value.l)){
            Q_ASSERT(!link->isQObject());
            *reinterpret_cast<void**>(output) = link->pointer();
        }
        else if(Java::QtJambi::QtObjectInterface::isInstanceOf(env, java_value.l))
            Java::QtJambi::QNoNativeResourcesException::throwNew(env, QString("Incomplete object of type: %1").arg(qtjambi_object_class_name(env, java_value.l).replace("$", ".")) QTJAMBI_STACKTRACEINFO );
        return true;
    }else return false;
}

bool ObjectValueTypeEntry::convertToJava(JNIEnv *env, const void *qt_object, bool makeCopyOfValueTypes, bool cppOwnership, jvalue* output, jValueType javaType) const{
    if(javaType!=jValueType::l)
        JavaException::raiseIllegalArgumentException(env, "Cannot convert object type" QTJAMBI_STACKTRACEINFO );
    if (!qt_object){
        output->l = nullptr;
        return true;
    }
    if(!makeCopyOfValueTypes){
        for(const QSharedPointer<QtJambiLink>& link : QtJambiLink::findLinksForPointer(qt_object)){
            if(link){
                jobject obj = link->getJavaObjectLocalRef(env);
                if(obj && env->IsInstanceOf(obj, javaClass())){
                    output->l = obj;
                    return true;
                }
            }
        }
    }
    const std::type_info* _typeId = qtjambi_resolve_polymorphy(m_polymorphicIdHandlers, qt_object);
    if(_typeId && *_typeId!=type()){
        if(const QtJambiTypeEntry* typeEntry = QtJambiTypeEntry::getTypeEntry(env, *_typeId)){
            return typeEntry->convertToJava(env, qt_object, makeCopyOfValueTypes, cppOwnership, output, javaType);
        }else{
            return false;
        }
    }else{
        void *copy = makeCopyOfValueTypes ? m_qt_meta_type.create(qt_object) : const_cast<void*>(qt_object);
        if (!copy){
            output->l = nullptr;
            return true;
        }
        if(!creatableClass() || !creatorMethod())
            JavaException::raiseError(env, "Uncreatable type" QTJAMBI_STACKTRACEINFO );
        output->l = env->NewObject(creatableClass(), creatorMethod(), nullptr);
        qtjambi_throw_java_exception(env);
        const QSharedPointer<QtJambiLink>& link =
                m_owner_function ?
                    QtJambiLink::createLinkForObject(
                        env,
                        output->l,
                        copy,
                        m_qt_meta_type,
                        false,
                        false,
                        m_owner_function
                    ) : QtJambiLink::createLinkForObject(
                        env,
                        output->l,
                        copy,
                        m_qt_meta_type,
                        false,
                        false
                    );
        if(makeCopyOfValueTypes){
            link->setJavaOwnership(env);
        }else if(cppOwnership){
            link->setCppOwnership(env);
        }
        return true;
    }
}

bool ObjectValueTypeEntry::convertSharedPointerToJava(JNIEnv *env, void *ptr_shared_pointer, PointerDeleter sharedPointerDeleter, PointerGetterFunction sharedPointerGetter, jvalue* output, jValueType javaType) const{
    if(javaType!=jValueType::l)
        JavaException::raiseIllegalArgumentException(env, "Cannot convert object type" QTJAMBI_STACKTRACEINFO );
    if (!ptr_shared_pointer || !sharedPointerDeleter || !sharedPointerGetter){
        output->l = nullptr;
        return true;
    }
    void* qt_object = sharedPointerGetter(ptr_shared_pointer);
    if (!qt_object){
        output->l = nullptr;
        return true;
    }
    for(QSharedPointer<QtJambiLink>& link : QtJambiLink::findLinksForPointer(qt_object)){
        if(link && !link->isSharedPointer()){
            jobject obj = link->getJavaObjectLocalRef(env);
            if(obj && env->IsInstanceOf(obj, javaClass())){
                output->l = obj;
                bool createdByJava = link->createdByJava();
                bool is_shell = link->isShell();
                QtJambiShellAccess* shell = nullptr;
                if(is_shell){
                    const SuperTypeInfos& infos = getSuperTypeInfos(env, env->GetObjectClass(output->l));
                    Q_ASSERT(infos.size());
                    shell = *reinterpret_cast<QtJambiShellAccess**>( quintptr(qt_object) + infos.at(0).size() );
                }
                jobject nativeLink = link->nativeLink(env);
                if(nativeLink){
                    Java::QtJambi::QtJambiInternal$NativeLink::reset(env, nativeLink);
                }
                PtrOwnerFunction registeredThreadAffinityFunction = nullptr;
                if(!link->isQObject()){
                    PointerToObjectLink* polink = static_cast<PointerToObjectLink*>(link.data());
                    registeredThreadAffinityFunction = polink->ownerFunction();
                }
                link->invalidate(env);
                link.clear();
                link = QtJambiLink::createLinkForSharedPointerToObject(env, output->l,
                                                                           LINK_NAME_ARG(qtName())
                                                                           createdByJava,
                                                                           is_shell,
                                                                           registeredThreadAffinityFunction,
                                                                           ptr_shared_pointer,
                                                                           sharedPointerDeleter,
                                                                           sharedPointerGetter);
                if(shell){
                    shell->overrideLink(link);
                }
                env->DeleteLocalRef(nativeLink);
                return true;
            }
        }
    }
    const std::type_info* _typeId = qtjambi_resolve_polymorphy(m_polymorphicIdHandlers, qt_object);
    if(_typeId && *_typeId!=type()){
        if(const QtJambiTypeEntry* typeEntry = QtJambiTypeEntry::getTypeEntry(env, *_typeId)){
            PointerGetterFunction _sharedPointerGetter = sharedPointerGetter;
            uint offset = m_interface_offsets.value(_typeId->hash_code(), 0);
            if(offset>0){
                _sharedPointerGetter = [sharedPointerGetter,offset](void* sharedPtr)-> void* {
                    void* ptr = sharedPointerGetter(sharedPtr);
                    return reinterpret_cast<void*>( qintptr(ptr) - offset );
                };
            }
            return typeEntry->convertSharedPointerToJava(env, ptr_shared_pointer, sharedPointerDeleter, sharedPointerGetter, output, javaType);
        }else{
            return false;
        }
    }else{
        if(!creatableClass() || !creatorMethod())
            JavaException::raiseError(env, "Uncreatable type" QTJAMBI_STACKTRACEINFO );
        output->l = env->NewObject(creatableClass(), creatorMethod(), nullptr);
        qtjambi_throw_java_exception(env);
        const QSharedPointer<QtJambiLink>& link = QtJambiLink::createLinkForSharedPointerToObject(
                env,
                output->l,
                LINK_NAME_ARG(qtName())
                false,
                false,
                m_owner_function,
                ptr_shared_pointer,
                sharedPointerDeleter,
                sharedPointerGetter
            );
        link->setJavaOwnership(env);
        return true;
    }
}

ObjectContainerTypeEntry::ObjectContainerTypeEntry(JNIEnv* env,
                const std::type_info& typeId,
                const char *qt_name,
                const char *java_name,
                jclass java_class,
                jmethodID creator_method,
                size_t value_size,
                const std::type_info* super_type,
                const QList<const std::type_info*>& interface_types,
                const QMap<size_t, uint>& interface_offsets,
                size_t shell_size,
                NewContainerAccessFunction containerAccessFactory,
                PtrOwnerFunction owner_function,
                const QVector<const FunctionInfo>* virtualFunctions,
                Destructor destructor,
                const QList<const PolymorphicIdHandler*>& polymorphicHandlers,
                const QMetaType& qt_meta_type)
    : ObjectValueTypeEntry(env,
                           typeId,
                           qt_name,
                           java_name,
                           java_class,
                           creator_method,
                           value_size,
                           super_type,
                           interface_types,
                           interface_offsets,
                           shell_size,
                           owner_function,
                           virtualFunctions,
                           destructor,
                           polymorphicHandlers,
                           qt_meta_type),
      m_containerAccessFactory(containerAccessFactory)
{}

ObjectAbstractContainerTypeEntry::ObjectAbstractContainerTypeEntry(JNIEnv* env,
                const std::type_info& typeId,
                const char *qt_name,
                const char *java_name,
                jclass java_class,
                jmethodID creator_method,
                size_t value_size,
                const std::type_info* super_type,
                const QList<const std::type_info*>& interface_types,
                const QMap<size_t, uint>& interface_offsets,
                size_t shell_size,
                NewContainerAccessFunction containerAccessFactory,
                PtrOwnerFunction owner_function,
                const QVector<const FunctionInfo>* virtualFunctions,
                Destructor destructor,
                const QList<const PolymorphicIdHandler*>& polymorphicHandlers,
                const QMetaType& qt_meta_type,
                jclass java_wrapper_class)
    : ObjectAbstractValueTypeEntry(env,
                           typeId,
                           qt_name,
                           java_name,
                           java_class,
                           creator_method,
                           value_size,
                           super_type,
                           interface_types,
                           interface_offsets,
                           shell_size,
                           owner_function,
                           virtualFunctions,
                           destructor,
                           polymorphicHandlers,
                           qt_meta_type,
                           java_wrapper_class),
      m_containerAccessFactory(containerAccessFactory)
{}

bool ObjectContainerTypeEntry::convertToJava(JNIEnv *env, const void *qt_object, bool makeCopyOfValueTypes, bool cppOwnership, jvalue* output, jValueType javaType) const{
    if(javaType!=jValueType::l)
        JavaException::raiseIllegalArgumentException(env, "Cannot convert object type" QTJAMBI_STACKTRACEINFO );
    if (!qt_object){
        output->l = nullptr;
        return true;
    }
    if(!makeCopyOfValueTypes){
        for(const QSharedPointer<QtJambiLink>& link : QtJambiLink::findLinksForPointer(qt_object)){
            if(link){
                jobject obj = link->getJavaObjectLocalRef(env);
                if(obj && env->IsInstanceOf(obj, javaClass())){
                    output->l = obj;
                    return true;
                }
            }
        }
    }
    const std::type_info* _typeId = qtjambi_resolve_polymorphy(m_polymorphicIdHandlers, qt_object);
    if(_typeId && *_typeId!=type()){
        if(const QtJambiTypeEntry* typeEntry = QtJambiTypeEntry::getTypeEntry(env, *_typeId)){
            return typeEntry->convertToJava(env, qt_object, makeCopyOfValueTypes, cppOwnership, output, javaType);
        }else{
            return false;
        }
    }else{
        void *copy = makeCopyOfValueTypes ? m_qt_meta_type.create(qt_object) : const_cast<void*>(qt_object);
        if (!copy){
            output->l = nullptr;
            return true;
        }
        if(!creatableClass() || !creatorMethod())
            JavaException::raiseError(env, "Uncreatable type" QTJAMBI_STACKTRACEINFO );
        output->l = env->NewObject(creatableClass(), creatorMethod(), nullptr);
        qtjambi_throw_java_exception(env);
        const QSharedPointer<QtJambiLink>& link = QtJambiLink::createLinkForContainer(
                        env,
                        output->l,
                        copy,
                        m_qt_meta_type,
                        false,
                        false,
                        m_containerAccessFactory()
                    );
        if(makeCopyOfValueTypes){
            link->setJavaOwnership(env);
        }else if(cppOwnership){
            link->setCppOwnership(env);
        }
        return true;
    }
}

bool ObjectContainerTypeEntry::convertSharedPointerToJava(JNIEnv *env, void *ptr_shared_pointer, PointerDeleter sharedPointerDeleter, PointerGetterFunction sharedPointerGetter, jvalue* output, jValueType javaType) const{
    if(javaType!=jValueType::l)
        JavaException::raiseIllegalArgumentException(env, "Cannot convert object type" QTJAMBI_STACKTRACEINFO );
    if (!ptr_shared_pointer || !sharedPointerDeleter || !sharedPointerGetter){
        output->l = nullptr;
        return true;
    }
    void* qt_object = sharedPointerGetter(ptr_shared_pointer);
    if (!qt_object){
        output->l = nullptr;
        return true;
    }
    for(QSharedPointer<QtJambiLink>& link : QtJambiLink::findLinksForPointer(qt_object)){
        if(link && !link->isSharedPointer()){
            jobject obj = link->getJavaObjectLocalRef(env);
            if(obj && env->IsInstanceOf(obj, javaClass())){
                output->l = obj;
                bool is_shell = link->isShell();
                QtJambiShellAccess* shell = nullptr;
                if(is_shell){
                    const SuperTypeInfos& infos = getSuperTypeInfos(env, env->GetObjectClass(output->l));
                    Q_ASSERT(infos.size());
                    shell = *reinterpret_cast<QtJambiShellAccess**>( quintptr(qt_object) + infos.at(0).size() );
                }
                jobject nativeLink = link->nativeLink(env);
                if(nativeLink){
                    Java::QtJambi::QtJambiInternal$NativeLink::reset(env, nativeLink);
                }
                link->invalidate(env);
                link.clear();
                link = QtJambiLink::createLinkForSharedPointerToContainer(env, output->l,
                                                                           LINK_NAME_ARG(qtName())
                                                                           ptr_shared_pointer,
                                                                           sharedPointerDeleter,
                                                                           sharedPointerGetter,
                                                                           m_containerAccessFactory());
                if(shell){
                    shell->overrideLink(link);
                }
                env->DeleteLocalRef(nativeLink);
                return true;
            }
        }
    }
    const std::type_info* _typeId = qtjambi_resolve_polymorphy(m_polymorphicIdHandlers, qt_object);
    if(_typeId && *_typeId!=type()){
        if(const QtJambiTypeEntry* typeEntry = QtJambiTypeEntry::getTypeEntry(env, *_typeId)){
            PointerGetterFunction _sharedPointerGetter = sharedPointerGetter;
            uint offset = m_interface_offsets.value(_typeId->hash_code(), 0);
            if(offset>0){
                _sharedPointerGetter = [sharedPointerGetter,offset](void* sharedPtr)-> void* {
                    void* ptr = sharedPointerGetter(sharedPtr);
                    return reinterpret_cast<void*>( qintptr(ptr) - offset );
                };
            }
            return typeEntry->convertSharedPointerToJava(env, ptr_shared_pointer, sharedPointerDeleter, sharedPointerGetter, output, javaType);
        }else{
            return false;
        }
    }else{
        if(!creatableClass() || !creatorMethod())
            JavaException::raiseError(env, "Uncreatable type" QTJAMBI_STACKTRACEINFO );
        output->l = env->NewObject(creatableClass(), creatorMethod(), nullptr);
        qtjambi_throw_java_exception(env);
        const QSharedPointer<QtJambiLink>& link = QtJambiLink::createLinkForSharedPointerToContainer(
                env,
                output->l,
                LINK_NAME_ARG(qtName())
                ptr_shared_pointer,
                sharedPointerDeleter,
                sharedPointerGetter,
                m_containerAccessFactory()
            );
        link->setJavaOwnership(env);
        return true;
    }
}

bool ObjectAbstractContainerTypeEntry::convertToJava(JNIEnv *env, const void *qt_object, bool makeCopyOfValueTypes, bool cppOwnership, jvalue* output, jValueType javaType) const{
    if(javaType!=jValueType::l)
        JavaException::raiseIllegalArgumentException(env, "Cannot convert object type" QTJAMBI_STACKTRACEINFO );
    if (!qt_object){
        output->l = nullptr;
        return true;
    }
    if(!makeCopyOfValueTypes){
        for(const QSharedPointer<QtJambiLink>& link : QtJambiLink::findLinksForPointer(qt_object)){
            if(link){
                jobject obj = link->getJavaObjectLocalRef(env);
                if(obj && env->IsInstanceOf(obj, javaClass())){
                    output->l = obj;
                    return true;
                }
            }
        }
    }
    const std::type_info* _typeId = qtjambi_resolve_polymorphy(m_polymorphicIdHandlers, qt_object);
    if(_typeId && *_typeId!=type()){
        if(const QtJambiTypeEntry* typeEntry = QtJambiTypeEntry::getTypeEntry(env, *_typeId)){
            return typeEntry->convertToJava(env, qt_object, makeCopyOfValueTypes, cppOwnership, output, javaType);
        }else{
            return false;
        }
    }else{
        void *copy = makeCopyOfValueTypes ? m_qt_meta_type.create(qt_object) : const_cast<void*>(qt_object);
        if (!copy){
            output->l = nullptr;
            return true;
        }
        if(!creatableClass() || !creatorMethod())
            JavaException::raiseError(env, "Uncreatable type" QTJAMBI_STACKTRACEINFO );
        output->l = env->NewObject(creatableClass(), creatorMethod(), nullptr);
        qtjambi_throw_java_exception(env);
        const QSharedPointer<QtJambiLink>& link = QtJambiLink::createLinkForContainer(
                        env,
                        output->l,
                        copy,
                        m_qt_meta_type,
                        false,
                        false,
                        m_containerAccessFactory()
                    );
        if(makeCopyOfValueTypes){
            link->setJavaOwnership(env);
        }else if(cppOwnership){
            link->setCppOwnership(env);
        }
        return true;
    }
}

bool ObjectAbstractContainerTypeEntry::convertSharedPointerToJava(JNIEnv *env, void *ptr_shared_pointer, PointerDeleter sharedPointerDeleter, PointerGetterFunction sharedPointerGetter, jvalue* output, jValueType javaType) const{
    if(javaType!=jValueType::l)
        JavaException::raiseIllegalArgumentException(env, "Cannot convert object type" QTJAMBI_STACKTRACEINFO );
    if (!ptr_shared_pointer || !sharedPointerDeleter || !sharedPointerGetter){
        output->l = nullptr;
        return true;
    }
    void* qt_object = sharedPointerGetter(ptr_shared_pointer);
    if (!qt_object){
        output->l = nullptr;
        return true;
    }
    for(QSharedPointer<QtJambiLink>& link : QtJambiLink::findLinksForPointer(qt_object)){
        if(link && !link->isSharedPointer()){
            jobject obj = link->getJavaObjectLocalRef(env);
            if(obj && env->IsInstanceOf(obj, javaClass())){
                output->l = obj;
                bool is_shell = link->isShell();
                QtJambiShellAccess* shell = nullptr;
                if(is_shell){
                    const SuperTypeInfos& infos = getSuperTypeInfos(env, env->GetObjectClass(output->l));
                    Q_ASSERT(infos.size());
                    shell = *reinterpret_cast<QtJambiShellAccess**>( quintptr(qt_object) + infos.at(0).size() );
                }
                jobject nativeLink = link->nativeLink(env);
                if(nativeLink){
                    Java::QtJambi::QtJambiInternal$NativeLink::reset(env, nativeLink);
                }
                link->invalidate(env);
                link.clear();
                link = QtJambiLink::createLinkForSharedPointerToContainer(env, output->l,
                                                                           LINK_NAME_ARG(qtName())
                                                                           ptr_shared_pointer,
                                                                           sharedPointerDeleter,
                                                                           sharedPointerGetter,
                                                                           m_containerAccessFactory());
                if(shell){
                    shell->overrideLink(link);
                }
                env->DeleteLocalRef(nativeLink);
                return true;
            }
        }
    }
    const std::type_info* _typeId = qtjambi_resolve_polymorphy(m_polymorphicIdHandlers, qt_object);
    if(_typeId && *_typeId!=type()){
        if(const QtJambiTypeEntry* typeEntry = QtJambiTypeEntry::getTypeEntry(env, *_typeId)){
            PointerGetterFunction _sharedPointerGetter = sharedPointerGetter;
            uint offset = m_interface_offsets.value(_typeId->hash_code(), 0);
            if(offset>0){
                _sharedPointerGetter = [sharedPointerGetter,offset](void* sharedPtr)-> void* {
                    void* ptr = sharedPointerGetter(sharedPtr);
                    return reinterpret_cast<void*>( qintptr(ptr) - offset );
                };
            }
            return typeEntry->convertSharedPointerToJava(env, ptr_shared_pointer, sharedPointerDeleter, sharedPointerGetter, output, javaType);
        }else{
            return false;
        }
    }else{
        if(!creatableClass() || !creatorMethod())
            JavaException::raiseError(env, "Uncreatable type" QTJAMBI_STACKTRACEINFO );
        output->l = env->NewObject(creatableClass(), creatorMethod(), nullptr);
        qtjambi_throw_java_exception(env);
        const QSharedPointer<QtJambiLink>& link = QtJambiLink::createLinkForSharedPointerToContainer(
                env,
                output->l,
                LINK_NAME_ARG(qtName())
                ptr_shared_pointer,
                sharedPointerDeleter,
                sharedPointerGetter,
                m_containerAccessFactory()
            );
        link->setJavaOwnership(env);
        return true;
    }
}

bool QObjectTypeEntry::convertToJava(JNIEnv *env, const void *ptr, bool makeCopyOfValueTypes, bool cppOwnership, jvalue* output, jValueType javaType) const{
    if(javaType!=jValueType::l)
        JavaException::raiseIllegalArgumentException(env, "Cannot convert object type" QTJAMBI_STACKTRACEINFO );
    QObject* qt_object = reinterpret_cast<QObject*>( const_cast<void*>(ptr) );
    if (!qt_object){
        output->l = nullptr;
        return true;
    }

    if(QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForQObject(qt_object)){
        jobject nativeLink = link->nativeLink(env);
        QScopedPointer<JObjectSynchronizer> synchronizer(new JObjectSynchronizer(env, nativeLink));
        // Since QObjects are created in a class based on virtual function calls,
        // if they at some point during their constructor are converted to Java,
        // the Java object will get the wrong class. In order to fix this as well
        // as possible, we replace the java object if it turns out it has previously
        // been created using a different metaObject than the current. This should
        // at least make the brokeness identical to that of C++, and we can't do this
        // better than C++ since we depend on C++ to do it.
        if(!link->createdByJava()){
            QWriteLocker locker(QtJambiLinkUserData::lock());
            QtJambiLinkUserData *p = QTJAMBI_GET_OBJECTUSERDATA(QtJambiLinkUserData, qt_object);
            if (p && p->metaObject() != qt_object->metaObject()) {
                QTJAMBI_SET_OBJECTUSERDATA(QtJambiLinkUserData, qt_object, nullptr);
                locker.unlock();
                delete p;
                // It should already be split ownership, but in case it has been changed, we need to make sure the c++
                // object isn't deleted.
                Java::QtJambi::QtJambiInternal$NativeLink::reset(env, nativeLink);
                link->setSplitOwnership(env);
                link.clear();
                locker.relock();
            }
        }
        if(link){
            output->l = link->getJavaObjectLocalRef(env);
            if(!output->l && link->ownership()==QtJambiLink::Ownership::Split){
                {
                    bool isInvalidated = false;
                    {
                        QWriteLocker locker(QtJambiLinkUserData::lock());
                        if(QtJambiLinkUserData *p = QTJAMBI_GET_OBJECTUSERDATA(QtJambiLinkUserData, qt_object)){
                            QTJAMBI_SET_OBJECTUSERDATA(QtJambiLinkUserData, qt_object, nullptr);
                            locker.unlock();
                            delete p;
                            isInvalidated = true;
                            locker.relock();
                        }
                    }
                    if(!isInvalidated)
                        link->invalidate(env);
                    link.clear();
                }
            }else return true;
        }
    }

    if(this->m_original_meta_object){
        const QMetaObject *mo = qtjambi_find_first_static_metaobject(qt_object->metaObject());
        if(this->m_original_meta_object!=mo){
            const std::type_info* typeId = &type();
            // Search the hierarchy of classes for a class that has been mapped to Java.
            // Prefer the requested class if no other can be found.
            // Only return objects of subclasses of the requested class
            // If the requested class is not in the object's hierarchy, then we prefer
            // the requested class (this means it's basically a proper subclass of the
            // requested class, and thus probably the concrete wrapper, but atleast a
            // more specific version than anything we can find)
            while (mo) {
                const std::type_info* _typeId = getTypeByMetaObject(mo);
                if(_typeId){
                    if(*typeId==*_typeId){
                        // Never go further down the hierarchy than the requested class
                        break;
                    }else{
                        typeId = _typeId;
                        break;
                    }
                }
                mo = mo->superClass();
            }
            if(*typeId!=type()){
                if(const QtJambiTypeEntry* typeEntry = QtJambiTypeEntry::getTypeEntry(env, *typeId)){
                    return typeEntry->convertToJava(env, qt_object, makeCopyOfValueTypes, cppOwnership, output, javaType);
                }else{
                    return false;
                }
            }
        }
    }
    if(!creatableClass() || !creatorMethod())
        JavaException::raiseError(env, "Uncreatable type" QTJAMBI_STACKTRACEINFO );
    output->l = env->NewObject(creatableClass(), creatorMethod(), nullptr);
    qtjambi_throw_java_exception(env);
    QtJambiLink::createLinkForQObject(env, output->l, qt_object, false, false);
    return true;
}

bool QObjectTypeEntry::convertSharedPointerToJava(JNIEnv *env, void *ptr_shared_pointer, PointerDeleter sharedPointerDeleter, PointerGetterFunction sharedPointerGetter, jvalue* output, jValueType javaType) const{
    if(javaType!=jValueType::l)
        JavaException::raiseIllegalArgumentException(env, "Cannot convert object type" QTJAMBI_STACKTRACEINFO );
    if (!ptr_shared_pointer || !sharedPointerDeleter || !sharedPointerGetter){
        output->l = nullptr;
        return true;
    }
    QObject* qt_object = reinterpret_cast<QObject*>(sharedPointerGetter(ptr_shared_pointer));
    if (!qt_object){
        output->l = nullptr;
        return true;
    }

    if(QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForQObject(qt_object)){
        jobject nativeLink = link->nativeLink(env);
        QScopedPointer<JObjectSynchronizer> synchronizer(new JObjectSynchronizer(env, nativeLink));
        // Since QObjects are created in a class based on virtual function calls,
        // if they at some point during their constructor are converted to Java,
        // the Java object will get the wrong class. In order to fix this as well
        // as possible, we replace the java object if it turns out it has previously
        // been created using a different metaObject than the current. This should
        // at least make the brokeness identical to that of C++, and we can't do this
        // better than C++ since we depend on C++ to do it.
        if(!link->createdByJava()){
            QWriteLocker locker(QtJambiLinkUserData::lock());
            QtJambiLinkUserData *p = QTJAMBI_GET_OBJECTUSERDATA(QtJambiLinkUserData, qt_object);
            if (p && p->metaObject() != qt_object->metaObject()) {
                QTJAMBI_SET_OBJECTUSERDATA(QtJambiLinkUserData, qt_object, nullptr);
                locker.unlock();
                delete p;
                // It should already be split ownership, but in case it has been changed, we need to make sure the c++
                // object isn't deleted.
                Java::QtJambi::QtJambiInternal$NativeLink::reset(env, nativeLink);
                link->setSplitOwnership(env);
                link.clear();
                locker.relock();
            }
        }
        if(link && !link->isSharedPointer()){
            jobject obj = link->getJavaObjectLocalRef(env);
            if(obj && env->IsInstanceOf(obj, javaClass())){
                output->l = obj;
                bool createdByJava = link->createdByJava();
                bool is_shell = link->isShell();
                QtJambiShellAccess* shell = nullptr;
                if(is_shell){
                    const SuperTypeInfos& infos = getSuperTypeInfos(env, env->GetObjectClass(output->l));
                    Q_ASSERT(infos.size());
                    shell = *reinterpret_cast<QtJambiShellAccess**>( quintptr(qt_object) + infos.at(0).size() );
                }
                jobject nativeLink = link->nativeLink(env);
                if(nativeLink){
                    Java::QtJambi::QtJambiInternal$NativeLink::reset(env, nativeLink);
                }
                link->invalidate(env);
                link.clear();
                PointerQObjectGetterFunction pointerGetter = [sharedPointerGetter](void* ptr) -> QObject* {
                    return reinterpret_cast<QObject*>( sharedPointerGetter(ptr) );
                };
                link = QtJambiLink::createLinkForSharedPointerToQObject(env, output->l, createdByJava, is_shell, ptr_shared_pointer, sharedPointerDeleter, pointerGetter);
                if(shell){
                    shell->overrideLink(link);
                }
                env->DeleteLocalRef(nativeLink);
                return true;
            }
        }
        if(link){
            output->l = link->getJavaObjectLocalRef(env);
            if(!output->l && link->ownership()==QtJambiLink::Ownership::Split){
                {
                    bool isInvalidated = false;
                    {
                        QWriteLocker locker(QtJambiLinkUserData::lock());
                        if(QtJambiLinkUserData *p = QTJAMBI_GET_OBJECTUSERDATA(QtJambiLinkUserData, qt_object)){
                            QTJAMBI_SET_OBJECTUSERDATA(QtJambiLinkUserData, qt_object, nullptr);
                            locker.unlock();
                            isInvalidated = true;
                            delete p;
                            locker.relock();
                        }
                    }
                    if(!isInvalidated)
                        link->invalidate(env);
                    link.clear();
                }
            }else return true;
        }
    }

    if(this->m_original_meta_object){
        const QMetaObject *mo = qtjambi_find_first_static_metaobject(qt_object->metaObject());
        if(this->m_original_meta_object!=mo){
            const std::type_info* typeId = &type();
            // Search the hierarchy of classes for a class that has been mapped to Java.
            // Prefer the requested class if no other can be found.
            // Only return objects of subclasses of the requested class
            // If the requested class is not in the object's hierarchy, then we prefer
            // the requested class (this means it's basically a proper subclass of the
            // requested class, and thus probably the concrete wrapper, but atleast a
            // more specific version than anything we can find)
            while (mo) {
                const std::type_info* _typeId = getTypeByMetaObject(mo);
                if(_typeId){
                    if(*typeId==*_typeId){
                        // Never go further down the hierarchy than the requested class
                        break;
                    }else{
                        typeId = _typeId;
                        break;
                    }
                }
                mo = mo->superClass();
            }
            if(*typeId!=type()){
                if(const QtJambiTypeEntry* typeEntry = QtJambiTypeEntry::getTypeEntry(env, *typeId)){
                    return typeEntry->convertSharedPointerToJava(env, ptr_shared_pointer, sharedPointerDeleter, sharedPointerGetter, output, javaType);
                }else{
                    return false;
                }
            }
        }
    }
    if(!creatableClass() || !creatorMethod())
        JavaException::raiseError(env, "Uncreatable type" QTJAMBI_STACKTRACEINFO );
    output->l = env->NewObject(creatableClass(), creatorMethod(), nullptr);
    qtjambi_throw_java_exception(env);
    PointerQObjectGetterFunction pointerGetter = [sharedPointerGetter](void* ptr) -> QObject* {
        return reinterpret_cast<QObject*>( sharedPointerGetter(ptr) );
    };
    QtJambiLink::createLinkForSharedPointerToQObject(env, output->l, false, false, ptr_shared_pointer, sharedPointerDeleter, pointerGetter);
    return true;
}

QObjectPendingTypeEntry::QObjectPendingTypeEntry(JNIEnv* env,
                 const std::type_info& typeId,
                 const char *qt_name,
                 const char *java_name,
                 jclass java_class,
                 jmethodID creator_method,
                 size_t value_size,
                 const std::type_info* super_type,
                 const QList<const std::type_info*>& interface_types,
                 const QMap<size_t, uint>& interface_offsets,
                 size_t shell_size,
                 const QVector<const FunctionInfo>* virtualFunctions,
                 const QMetaObject* metaObject
            )
    : QObjectTypeEntry(env, typeId, qt_name, java_name, java_class, creator_method, value_size,
                       super_type, interface_types, interface_offsets,
                       shell_size, virtualFunctions, nullptr),
      m_metaObject(metaObject)
{
}

bool QObjectPendingTypeEntry::convertToJava(JNIEnv *env, const void *ptr, bool, bool, jvalue* output, jValueType javaType) const{
    if(javaType!=jValueType::l)
        JavaException::raiseIllegalArgumentException(env, "Cannot convert object type" QTJAMBI_STACKTRACEINFO );
    QObject* qt_object = reinterpret_cast<QObject*>( const_cast<void*>(ptr) );
    if (!qt_object){
        output->l = nullptr;
        return true;
    }

    if(QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForQObject(qt_object)){
        jobject nativeLink = link->nativeLink(env);
        QScopedPointer<JObjectSynchronizer> synchronizer(new JObjectSynchronizer(env, nativeLink));
        // Since QObjects are created in a class based on virtual function calls,
        // if they at some point during their constructor are converted to Java,
        // the Java object will get the wrong class. In order to fix this as well
        // as possible, we replace the java object if it turns out it has previously
        // been created using a different metaObject than the current. This should
        // at least make the brokeness identical to that of C++, and we can't do this
        // better than C++ since we depend on C++ to do it.
        if(!link->createdByJava()){
            QWriteLocker locker(QtJambiLinkUserData::lock());
            QtJambiLinkUserData *p = QTJAMBI_GET_OBJECTUSERDATA(QtJambiLinkUserData, qt_object);
            if (p && p->metaObject() != qt_object->metaObject()) {
                QTJAMBI_SET_OBJECTUSERDATA(QtJambiLinkUserData, qt_object, nullptr);
                locker.unlock();
                delete p;
                // It should already be split ownership, but in case it has been changed, we need to make sure the c++
                // object isn't deleted.
                Java::QtJambi::QtJambiInternal$NativeLink::reset(env, nativeLink);
                link->setSplitOwnership(env);
                link.clear();
                locker.relock();
            }
        }
        if(link){
            output->l = link->getJavaObjectLocalRef(env);
            if(!output->l && link->ownership()==QtJambiLink::Ownership::Split){
                {
                    bool isInvalidated = false;
                    {
                        QWriteLocker locker(QtJambiLinkUserData::lock());
                        if(QtJambiLinkUserData *p = QTJAMBI_GET_OBJECTUSERDATA(QtJambiLinkUserData, qt_object)){
                            QTJAMBI_SET_OBJECTUSERDATA(QtJambiLinkUserData, qt_object, nullptr);
                            locker.unlock();
                            delete p;
                            isInvalidated = true;
                            locker.relock();
                        }
                    }
                    if(!isInvalidated)
                        link->invalidate(env);
                    link.clear();
                }
            }else return true;
        }
    }
    if(!creatableClass() || !creatorMethod())
        JavaException::raiseError(env, "Uncreatable type" QTJAMBI_STACKTRACEINFO );
    output->l = env->NewObject(creatableClass(), creatorMethod(), nullptr);
    qtjambi_throw_java_exception(env);
    QtJambiLink::createLinkForPendingQObject(env, output->l, m_metaObject, qt_object, false, false);
    return true;
}

bool QObjectPolymorphicTypeEntry::convertToJava(JNIEnv *env, const void *ptr, bool makeCopyOfValueTypes, bool cppOwnership, jvalue* output, jValueType javaType) const{
    if(javaType!=jValueType::l)
        JavaException::raiseIllegalArgumentException(env, "Cannot convert object type" QTJAMBI_STACKTRACEINFO );
    QObject* qt_object = reinterpret_cast<QObject*>( const_cast<void*>(ptr) );
    if (!qt_object){
        output->l = nullptr;
        return true;
    }

    if(QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForQObject(qt_object)){
        jobject nativeLink = link->nativeLink(env);
        QScopedPointer<JObjectSynchronizer> synchronizer(new JObjectSynchronizer(env, nativeLink));
        // Since QObjects are created in a class based on virtual function calls,
        // if they at some point during their constructor are converted to Java,
        // the Java object will get the wrong class. In order to fix this as well
        // as possible, we replace the java object if it turns out it has previously
        // been created using a different metaObject than the current. This should
        // at least make the brokeness identical to that of C++, and we can't do this
        // better than C++ since we depend on C++ to do it.
        if(!link->createdByJava()){
            QWriteLocker locker(QtJambiLinkUserData::lock());
            QtJambiLinkUserData *p = QTJAMBI_GET_OBJECTUSERDATA(QtJambiLinkUserData, qt_object);
            if (p && p->metaObject() != qt_object->metaObject()) {
                QTJAMBI_SET_OBJECTUSERDATA(QtJambiLinkUserData, qt_object, nullptr);
                locker.unlock();
                delete p;
                // It should already be split ownership, but in case it has been changed, we need to make sure the c++
                // object isn't deleted.
                Java::QtJambi::QtJambiInternal$NativeLink::reset(env, nativeLink);
                link->setSplitOwnership(env);
                link.clear();
                locker.relock();
            }
        }
        if(link){
            output->l = link->getJavaObjectLocalRef(env);
            if(!output->l && link->ownership()==QtJambiLink::Ownership::Split){
                {
                    bool isInvalidated = false;
                    {
                        QWriteLocker locker(QtJambiLinkUserData::lock());
                        if(QtJambiLinkUserData *p = QTJAMBI_GET_OBJECTUSERDATA(QtJambiLinkUserData, qt_object)){
                            QTJAMBI_SET_OBJECTUSERDATA(QtJambiLinkUserData, qt_object, nullptr);
                            locker.unlock();
                            isInvalidated = true;
                            delete p;
                            locker.relock();
                        }
                    }
                    if(!isInvalidated)
                        link->invalidate(env);
                    link.clear();
                }
            }else return true;
        }
    }

    const std::type_info* _typeId = qtjambi_resolve_polymorphy(m_polymorphicIdHandlers, qt_object);
    if(_typeId && *_typeId!=type()){
        if(const QtJambiTypeEntry* typeEntry = QtJambiTypeEntry::getTypeEntry(env, *_typeId)){
            return typeEntry->convertToJava(env, qt_object, makeCopyOfValueTypes, cppOwnership, output, javaType);
        }else{
            return false;
        }
    }

    const QMetaObject *mo = qtjambi_find_first_static_metaobject(qt_object->metaObject());
    if(this->originalMetaObject() && this->originalMetaObject()!=mo){
        const std::type_info* typeId = &type();
        // Search the hierarchy of classes for a class that has been mapped to Java.
        // Prefer the requested class if no other can be found.
        // Only return objects of subclasses of the requested class
        // If the requested class is not in the object's hierarchy, then we prefer
        // the requested class (this means it's basically a proper subclass of the
        // requested class, and thus probably the concrete wrapper, but atleast a
        // more specific version than anything we can find)
        while (mo) {
            const std::type_info* _typeId = getTypeByMetaObject(mo);
            if(_typeId){
                if(*typeId==*_typeId){
                    // Never go further down the hierarchy than the requested class
                    break;
                }else{
                    typeId = _typeId;
                    break;
                }
            }
            mo = mo->superClass();
        }
        if(*typeId!=type()){
            if(const QtJambiTypeEntry* typeEntry = QtJambiTypeEntry::getTypeEntry(env, *typeId)){
                return typeEntry->convertToJava(env, qt_object, makeCopyOfValueTypes, cppOwnership, output, javaType);
            }else{
                return false;
            }
        }
    }
    if(!creatableClass() || !creatorMethod())
        JavaException::raiseError(env, "Uncreatable type" QTJAMBI_STACKTRACEINFO );
    output->l = env->NewObject(creatableClass(), creatorMethod(), nullptr);
    qtjambi_throw_java_exception(env);
    QtJambiLink::createLinkForQObject(env, output->l, qt_object, false, false);
    return true;
}

bool QObjectPolymorphicTypeEntry::convertSharedPointerToJava(JNIEnv *env, void *ptr_shared_pointer, PointerDeleter sharedPointerDeleter, PointerGetterFunction sharedPointerGetter, jvalue* output, jValueType javaType) const{
    if(javaType!=jValueType::l)
        JavaException::raiseIllegalArgumentException(env, "Cannot convert object type" QTJAMBI_STACKTRACEINFO );
    if (!ptr_shared_pointer || !sharedPointerDeleter || !sharedPointerGetter){
        output->l = nullptr;
        return true;
    }
    QObject* qt_object = reinterpret_cast<QObject*>(sharedPointerGetter(ptr_shared_pointer));
    if (!qt_object){
        output->l = nullptr;
        return true;
    }

    if(QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForQObject(qt_object)){
        jobject nativeLink = link->nativeLink(env);
        QScopedPointer<JObjectSynchronizer> synchronizer(new JObjectSynchronizer(env, nativeLink));
        // Since QObjects are created in a class based on virtual function calls,
        // if they at some point during their constructor are converted to Java,
        // the Java object will get the wrong class. In order to fix this as well
        // as possible, we replace the java object if it turns out it has previously
        // been created using a different metaObject than the current. This should
        // at least make the brokeness identical to that of C++, and we can't do this
        // better than C++ since we depend on C++ to do it.
        if(!link->createdByJava()){
            QWriteLocker locker(QtJambiLinkUserData::lock());
            QtJambiLinkUserData *p = QTJAMBI_GET_OBJECTUSERDATA(QtJambiLinkUserData, qt_object);
            if (p && p->metaObject() != qt_object->metaObject()) {
                QTJAMBI_SET_OBJECTUSERDATA(QtJambiLinkUserData, qt_object, nullptr);
                locker.unlock();
                delete p;
                // It should already be split ownership, but in case it has been changed, we need to make sure the c++
                // object isn't deleted.
                Java::QtJambi::QtJambiInternal$NativeLink::reset(env, nativeLink);
                link->setSplitOwnership(env);
                link.clear();
                locker.relock();
            }
        }
        if(link && !link->isSharedPointer()){
            jobject obj = link->getJavaObjectLocalRef(env);
            if(obj && env->IsInstanceOf(obj, javaClass())){
                output->l = obj;
                bool createdByJava = link->createdByJava();
                bool is_shell = link->isShell();
                QtJambiShellAccess* shell = nullptr;
                if(is_shell){
                    const SuperTypeInfos& infos = getSuperTypeInfos(env, env->GetObjectClass(output->l));
                    Q_ASSERT(infos.size());
                    shell = *reinterpret_cast<QtJambiShellAccess**>( quintptr(qt_object) + infos.at(0).size() );
                }
                jobject nativeLink = link->nativeLink(env);
                if(nativeLink){
                    Java::QtJambi::QtJambiInternal$NativeLink::reset(env, nativeLink);
                }
                link->invalidate(env);
                link.clear();
                PointerQObjectGetterFunction pointerGetter = [sharedPointerGetter](void* ptr) -> QObject* {
                    return reinterpret_cast<QObject*>( sharedPointerGetter(ptr) );
                };
                link = QtJambiLink::createLinkForSharedPointerToQObject(env, output->l, createdByJava, is_shell, ptr_shared_pointer, sharedPointerDeleter, pointerGetter);
                if(shell){
                    shell->overrideLink(link);
                }
                env->DeleteLocalRef(nativeLink);
                return true;
            }
        }
        if(link){
            output->l = link->getJavaObjectLocalRef(env);
            if(!output->l && link->ownership()==QtJambiLink::Ownership::Split){
                {
                    bool isInvalidated = false;
                    {
                        QWriteLocker locker(QtJambiLinkUserData::lock());
                        if(QtJambiLinkUserData *p = QTJAMBI_GET_OBJECTUSERDATA(QtJambiLinkUserData, qt_object)){
                            QTJAMBI_SET_OBJECTUSERDATA(QtJambiLinkUserData, qt_object, nullptr);
                            locker.unlock();
                            isInvalidated = true;
                            delete p;
                            locker.relock();
                        }
                    }
                    if(!isInvalidated)
                        link->invalidate(env);
                    link.clear();
                }
            }else return true;
        }
    }

    const std::type_info* _typeId = qtjambi_resolve_polymorphy(m_polymorphicIdHandlers, qt_object);
    if(_typeId && *_typeId!=type()){
        if(const QtJambiTypeEntry* typeEntry = QtJambiTypeEntry::getTypeEntry(env, *_typeId)){
            return typeEntry->convertSharedPointerToJava(env, ptr_shared_pointer, sharedPointerDeleter, sharedPointerGetter, output, javaType);
        }else{
            return false;
        }
    }

    const QMetaObject *mo = qtjambi_find_first_static_metaobject(qt_object->metaObject());
    if(this->originalMetaObject() && this->originalMetaObject()!=mo){
        const std::type_info* typeId = &type();
        // Search the hierarchy of classes for a class that has been mapped to Java.
        // Prefer the requested class if no other can be found.
        // Only return objects of subclasses of the requested class
        // If the requested class is not in the object's hierarchy, then we prefer
        // the requested class (this means it's basically a proper subclass of the
        // requested class, and thus probably the concrete wrapper, but atleast a
        // more specific version than anything we can find)
        while (mo) {
            const std::type_info* _typeId = getTypeByMetaObject(mo);
            if(_typeId){
                if(*typeId==*_typeId){
                    // Never go further down the hierarchy than the requested class
                    break;
                }else{
                    typeId = _typeId;
                    break;
                }
            }
            mo = mo->superClass();
        }
        if(*typeId!=type()){
            if(const QtJambiTypeEntry* typeEntry = QtJambiTypeEntry::getTypeEntry(env, *typeId)){
                return typeEntry->convertSharedPointerToJava(env, ptr_shared_pointer, sharedPointerDeleter, sharedPointerGetter, output, javaType);
            }else{
                return false;
            }
        }
    }
    if(!creatableClass() || !creatorMethod())
        JavaException::raiseError(env, "Uncreatable type" QTJAMBI_STACKTRACEINFO );
    output->l = env->NewObject(creatableClass(), creatorMethod(), nullptr);
    qtjambi_throw_java_exception(env);
    PointerQObjectGetterFunction pointerGetter = [sharedPointerGetter](void* ptr) -> QObject* {
        return reinterpret_cast<QObject*>( sharedPointerGetter(ptr) );
    };
    QtJambiLink::createLinkForSharedPointerToQObject(env, output->l, false, false, ptr_shared_pointer, sharedPointerDeleter, pointerGetter);
    return true;
}

bool QObjectTypeEntry::convertToNative(JNIEnv *env, const jvalue& java_value, jValueType javaType, void * output) const{
    if(javaType!=jValueType::l)
        JavaException::raiseIllegalArgumentException(env, "Cannot convert to object type" QTJAMBI_STACKTRACEINFO );
    if(env->IsInstanceOf(java_value.l, this->javaClass())){
        if (QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForJavaObject(env, java_value.l)){
            Q_ASSERT(link->isQObject());
            *reinterpret_cast<QObject**>(output) = link->qobject();
        }
        else if(Java::QtJambi::QtObjectInterface::isInstanceOf(env, java_value.l))
            Java::QtJambi::QNoNativeResourcesException::throwNew(env, QString("Incomplete object of type: %1").arg(qtjambi_object_class_name(env, java_value.l).replace("$", ".")) QTJAMBI_STACKTRACEINFO );
        return true;
    }else return false;
}

bool FunctionalTypeEntry::convertToJava(JNIEnv *env, const void *qt_object, bool makeCopyOfValueTypes, bool cppOwnership, jvalue* output, jValueType javaType) const{
    if(javaType!=jValueType::l)
        JavaException::raiseIllegalArgumentException(env, "Cannot convert functional type" QTJAMBI_STACKTRACEINFO );
    Q_UNUSED(makeCopyOfValueTypes)
    Q_UNUSED(cppOwnership)
    if(m_registered_functional_resolver){
        output->l = m_registered_functional_resolver(env, qt_object);
        if(output->l)
            return true;
    }
    if(!creatableClass() || !creatorMethod())
        JavaException::raiseError(env, "Uncreatable type" QTJAMBI_STACKTRACEINFO );
    output->l = env->NewObject(creatableClass(), creatorMethod(), nullptr);
    qtjambi_throw_java_exception(env);
    QtJambiLink::createLinkForObject(
            env,
            output->l,
            m_qt_meta_type.create(qt_object),
            m_qt_meta_type,
            false,
            false
        )->setJavaOwnership(env);
    return true;
}

bool FunctionalTypeEntry::convertSharedPointerToJava(JNIEnv *env, void *qt_object, PointerDeleter, PointerGetterFunction sharedPointerGetter, jvalue* output, jValueType javaType) const{
    return convertToJava(env, sharedPointerGetter(qt_object), true, false, output, javaType);
}

bool FunctionalTypeEntry::convertToNative(JNIEnv *env, const jvalue& java_value, jValueType javaType, void * output) const{
    if(javaType!=jValueType::l)
        JavaException::raiseIllegalArgumentException(env, "Cannot convert to functional type" QTJAMBI_STACKTRACEINFO );
    if(!env->IsInstanceOf(java_value.l, javaClass()))
        return false;
    if(java_value.l
        && (qtjambi_object_class_name(env, java_value.l).endsWith("$ConcreteWrapper")
            || env->IsSameObject(env->GetObjectClass(java_value.l), creatableClass()))){
        const QSharedPointer<QtJambiLink>& link = QtJambiLink::findLinkForJavaInterface(env, java_value.l);
        if(link){
            *reinterpret_cast<void**>(output) = link->pointer();
        }else if(java_value.l){
            JavaException::raiseQNoNativeResourcesException(env, qPrintable(QString("Incomplete object of type: %1").arg(QLatin1String(this->javaName()))) QTJAMBI_STACKTRACEINFO );
        }
    }else{
        if (QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForJavaInterface(env, java_value.l)){
            if(!link->isMultiInheritanceType() && env->IsInstanceOf(java_value.l, javaClass())){
                if(FunctionalBase* functionalBase = reinterpret_cast<FunctionalBase *>(link->pointer())){
                    functionalBase->getFunctional(env, *reinterpret_cast<void**>(output));
                }
            }else if(link->isInterfaceAvailable(type())){
                if(FunctionalBase* functionalBase = reinterpret_cast<FunctionalBase *>(link->typedPointer(type()))){
                    functionalBase->getFunctional(env, *reinterpret_cast<void**>(output));
                }
            }
        }else if(java_value.l){
            JavaException::raiseQNoNativeResourcesException(env, qPrintable(QString("Incomplete object of type: %1").arg(QLatin1String(this->javaName()))) QTJAMBI_STACKTRACEINFO );
        }
    }
    return true;
}

bool EnumTypeEntry::convertToJava(JNIEnv *env, const void *qt_object, bool, bool, jvalue* output, jValueType javaType) const{
    switch (javaType) {
    case jValueType::l:
        if(!creatableClass() || !creatorMethod())
            JavaException::raiseError(env, "Uncreatable type" QTJAMBI_STACKTRACEINFO );
        switch ( this->valueSize() ) {
        case 1:  output->l = env->CallStaticObjectMethod(creatableClass(), creatorMethod(), *reinterpret_cast<const qint8*>(qt_object)); break;
        case 2:  output->l = env->CallStaticObjectMethod(creatableClass(), creatorMethod(), *reinterpret_cast<const qint16*>(qt_object)); break;
        case 4:  output->l = env->CallStaticObjectMethod(creatableClass(), creatorMethod(), *reinterpret_cast<const qint32*>(qt_object)); break;
        default: output->l = env->CallStaticObjectMethod(creatableClass(), creatorMethod(), *reinterpret_cast<const qint64*>(qt_object)); break;
        }
        qtjambi_throw_java_exception(env);
        break;
    case jValueType::s:
    case jValueType::b:
    case jValueType::j:
    case jValueType::i:
        switch ( this->valueSize() ) {
        case 1:  output->j = *reinterpret_cast<const qint8*>(qt_object); break;
        case 2:  output->j = *reinterpret_cast<const qint16*>(qt_object); break;
        case 4:  output->j = *reinterpret_cast<const qint32*>(qt_object); break;
        default: output->j = *reinterpret_cast<const qint64*>(qt_object); break;
        }
        break;
    case jValueType::z:
        switch ( this->valueSize() ) {
        case 1:  output->z = 0!=*reinterpret_cast<const qint8*>(qt_object); break;
        case 2:  output->z = 0!=*reinterpret_cast<const qint16*>(qt_object); break;
        case 4:  output->z = 0!=*reinterpret_cast<const qint32*>(qt_object); break;
        default: output->z = 0!=*reinterpret_cast<const qint64*>(qt_object); break;
        }
        break;
    default:
        JavaException::raiseIllegalArgumentException(env, "Cannot convert enum type" QTJAMBI_STACKTRACEINFO );
        break;
    }
    return true;
}

bool EnumTypeEntry::convertSharedPointerToJava(JNIEnv *env, void *qt_object, PointerDeleter, PointerGetterFunction sharedPointerGetter, jvalue* output, jValueType javaType) const{
    return convertToJava(env, sharedPointerGetter(qt_object), true, false, output, javaType);
}

bool EnumTypeEntry::convertToNative(JNIEnv *env, const jvalue& java_value, jValueType javaType, void * output) const{
    switch ( this->valueSize() ) {
    case 1:
        {
            qint8* value = reinterpret_cast<qint8*>(output);
            switch (javaType) {
            case jValueType::s:
            case jValueType::b:
            case jValueType::j:
            case jValueType::i:
                *value = qint8(java_value.j);
                break;
            case jValueType::l:
                try{
                    *value = !java_value.l ? 0 : Java::QtJambi::QtByteEnumerator::value(env, java_value.l);
                }catch(...){
                    if(Java::Runtime::Number::isInstanceOf(env,java_value.l)){
                        *value = Java::Runtime::Number::byteValue(env, java_value.l);
                    }else if(env->IsSameObject(nullptr, java_value.l)){
                        *value = 0;
                    }else {
                        return false;
                    }
                }
                break;
            default:
                return false;
            }
        }
        break;
    case 2:
        {
            qint16* value = reinterpret_cast<qint16*>(output);
            switch (javaType) {
            case jValueType::s:
            case jValueType::b:
            case jValueType::j:
            case jValueType::i:
                *value = qint16(java_value.j);
                break;
            case jValueType::l:
                try{
                    *value = !java_value.l ? 0 : Java::QtJambi::QtShortEnumerator::value(env, java_value.l);
                }catch(...){
                    if(Java::Runtime::Number::isInstanceOf(env,java_value.l)){
                        *value = Java::Runtime::Number::shortValue(env, java_value.l);
                    }else if(env->IsSameObject(nullptr, java_value.l)){
                        *value = 0;
                    }else {
                        return false;
                    }
                }
                break;
            default:
                return false;
            }
        }
        break;
    case 8:
        {
            qint64* value = reinterpret_cast<qint64*>(output);
            switch (javaType) {
            case jValueType::s:
            case jValueType::b:
            case jValueType::j:
            case jValueType::i:
                *value = qint64(java_value.j);
                break;
            case jValueType::l:
                try{
                    *value = !java_value.l ? 0 : Java::QtJambi::QtLongEnumerator::value(env, java_value.l);
                }catch(...){
                    if(Java::Runtime::Number::isInstanceOf(env,java_value.l)){
                        *value = Java::Runtime::Number::longValue(env, java_value.l);
                    }else if(env->IsSameObject(nullptr, java_value.l)){
                        *value = 0;
                    }else {
                        return false;
                    }
                }
                break;
            default:
                return false;
            }
        }
        break;
    default:
        {
            qint32* value = reinterpret_cast<qint32*>(output);
            switch (javaType) {
            case jValueType::s:
            case jValueType::b:
            case jValueType::j:
            case jValueType::i:
                *value = qint32(java_value.j);
                break;
            case jValueType::l:
                try{
                    *value = !java_value.l ? 0 : Java::QtJambi::QtEnumerator::value(env, java_value.l);
                }catch(...){
                    if(Java::Runtime::Number::isInstanceOf(env,java_value.l)){
                        *value = Java::Runtime::Number::intValue(env, java_value.l);
                    }else if(env->IsSameObject(nullptr, java_value.l)){
                        *value = 0;
                    }else {
                        return false;
                    }
                }
                break;
            default:
                return false;
            }
        }
        break;
    }
    return true;
}

bool FlagsTypeEntry::convertToJava(JNIEnv *env, const void *qt_object, bool, bool, jvalue* output, jValueType javaType) const{
    switch (javaType) {
    case jValueType::l:
        if(!creatableClass() || !creatorMethod())
            JavaException::raiseError(env, "Uncreatable type" QTJAMBI_STACKTRACEINFO );
        output->l = env->NewObject(creatableClass(), creatorMethod(), static_cast<int>(*reinterpret_cast<const int*>(qt_object)));
        break;
    case jValueType::s:
    case jValueType::b:
    case jValueType::j:
    case jValueType::i:
        output->i = *reinterpret_cast<const int*>(qt_object);
        break;
    default:
        return false;
    }
    return true;
}

bool FlagsTypeEntry::convertSharedPointerToJava(JNIEnv *env, void *qt_object, PointerDeleter, PointerGetterFunction sharedPointerGetter, jvalue* output, jValueType javaType) const{
    return convertToJava(env, sharedPointerGetter(qt_object), true, false, output, javaType);
}

bool FlagsTypeEntry::convertToNative(JNIEnv *env, const jvalue& java_value, jValueType javaType, void * output) const{
    int* value = reinterpret_cast<int*>(output);
    if(value){
        switch (javaType) {
        case jValueType::s:
        case jValueType::b:
        case jValueType::j:
        case jValueType::i:
            *value = java_value.i;
            break;
        case jValueType::l:
            try{
                *value = !java_value.l ? 0 : Java::QtJambi::QFlags::value(env, java_value.l);
            }catch(...){
                if(Java::Runtime::Number::isInstanceOf(env,java_value.l)){
                    *value = Java::Runtime::Number::intValue(env, java_value.l);
                }else if(env->IsSameObject(nullptr, java_value.l)){
                    *value = 0;
                }else {
                    return false;
                }
            }
            break;
        default:
            return false;
        }
    }
    return true;
}

AbstractSimpleTypeEntry::AbstractSimpleTypeEntry(JNIEnv* env, const std::type_info& typeId, const char *qt_name, const char *java_name, jclass java_class, size_t value_size)
    : QtJambiTypeEntry(env, typeId, qt_name, java_name, java_class, nullptr, value_size)
{
}

bool AbstractSimpleTypeEntry::convertSharedPointerToJava(JNIEnv *env, void *qt_object, PointerDeleter, PointerGetterFunction sharedPointerGetter, jvalue* output, jValueType javaType) const{
    return convertToJava(env, sharedPointerGetter(qt_object), true, false, output, javaType);
}

JLongTypeEntry::JLongTypeEntry(JNIEnv* env, const std::type_info& typeId, const char *qt_name, const char *java_name, jclass java_class, size_t value_size)
    : AbstractSimpleTypeEntry(env, typeId, qt_name, java_name, java_class, value_size)
{
}

StringTypeEntry::StringTypeEntry(JNIEnv* env, const std::type_info& typeId, const char *qt_name, const char *java_name, jclass java_class, size_t value_size)
    : AbstractSimpleTypeEntry(env, typeId, qt_name, java_name, java_class, value_size)
{
}

bool StringTypeEntry::convertToJava(JNIEnv *env, const void *qt_object, bool, bool, jvalue* output, jValueType javaType) const{
    const QString* strg = reinterpret_cast<const QString*>(qt_object);
    switch (javaType) {
    case jValueType::l:
        output->l = strg ? qtjambi_from_qstring(env, *strg) : nullptr;
        break;
    case jValueType::z:
        output->z = strg && !strg->isEmpty();
        break;
    case jValueType::c:
        output->c = strg && !strg->isEmpty() ? strg->at(0).unicode() : 0;
        break;
    default:
        output->j = strg ? strg->toLong() : 0;
        break;
    }
    return true;
}

bool StringTypeEntry::convertToNative(JNIEnv *env, const jvalue& java_value, jValueType javaType, void * output) const{
    QString* value = reinterpret_cast<QString*>(output);
    if(value){
        switch (javaType) {
        case jValueType::z:
            *value = java_value.z ? QLatin1String("true") : QLatin1String("false");
            break;
        case jValueType::c:
            *value = QChar(java_value.c);
            break;
        case jValueType::l:
            if(!env->IsInstanceOf(java_value.l, javaClass()))
                return false;
            qtjambi_to_qstring(*value, env, java_value.l);
            break;
        default:
            *value = QString::number(java_value.j);
            break;
        }
    }
    return true;
}

#if QT_VERSION >= 0x050C00
QCborValueRefTypeEntry::QCborValueRefTypeEntry(JNIEnv* env, const std::type_info& typeId, const char *qt_name, const char *java_name, jclass java_class, size_t value_size)
    : AbstractSimpleTypeEntry(env, typeId, qt_name, java_name, java_class, value_size)
{
}

bool QCborValueRefTypeEntry::convertToJava(JNIEnv *env, const void *qt_object, bool makeCopyOfValueTypes, bool cppOwnership, jvalue* output, jValueType javaType) const{
    const QCborValueRef* vref = reinterpret_cast<const QCborValueRef*>(qt_object);
    const QtJambiTypeEntry* typeEntry = QtJambiTypeEntry::getTypeEntry(env, typeid(QCborValue), "QCborValue");
    Q_ASSERT(typeEntry);
    QCborValue value = *vref;
    return typeEntry->convertToJava(env, &value, makeCopyOfValueTypes, cppOwnership, output, javaType);
}

bool QCborValueRefTypeEntry::convertToNative(JNIEnv *env, const jvalue&, jValueType, void *) const{
    JavaException::raiseError(env, "Cannot convert to QCborValueRef" QTJAMBI_STACKTRACEINFO );
    return false;
}
#endif

StringUtilTypeEntry::StringUtilTypeEntry(JNIEnv* env, const std::type_info& typeId, const char *qt_name, const char *java_name, jclass java_class, size_t value_size)
    : AbstractSimpleTypeEntry(env, typeId, qt_name, java_name, java_class, value_size)
{
}

bool StringUtilTypeEntry::convertToJava(JNIEnv *env, const void *qt_object, bool, bool, jvalue* output, jValueType javaType) const{
    if(type()==typeid(QStringView)){
        const QStringView* sref = reinterpret_cast<const QStringView*>(qt_object);
        if(sref){
            switch (javaType) {
            case jValueType::l:
                output->l = qtjambi_from_qstringview(env, *sref);
                break;
            case jValueType::z:
                output->z = !sref->isEmpty();
                break;
            case jValueType::c:
                output->c = !sref->toString().isEmpty() ? sref->toString().at(0).unicode() : 0;
                break;
            default:
                output->j = sref->toString().toLong();
                break;
            }
        }
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    }else if(type()==typeid(QStringRef)){
        const QStringRef* sref = reinterpret_cast<const QStringRef*>(qt_object);
        if(sref){
            switch (javaType) {
            case jValueType::l:
                output->l = qtjambi_from_qstringref(env, *sref);
                break;
            case jValueType::z:
                output->z = !sref->isEmpty();
                break;
            case jValueType::c:
                output->c = sref->toString().isEmpty() ? 0 : sref->toString().at(0).unicode();
                break;
            default:
                output->j = sref->toString().toLong();
                break;
            }
        }
    }else if(type()==typeid(QXmlStreamStringRef)){
        const QXmlStreamStringRef* sref = reinterpret_cast<const QXmlStreamStringRef*>(qt_object);
        if(sref){
            switch (javaType) {
            case jValueType::l:
                output->l = qtjambi_from_qstringref(env, QStringRef(*sref));
                break;
            case jValueType::z:
                output->z = !QStringRef(*sref).isEmpty();
                break;
            case jValueType::c:
                output->c = QStringRef(*sref).toString().isEmpty() ? 0 : QStringRef(*sref).toString().at(0).unicode();
                break;
            default:
                output->j = QStringRef(*sref).toString().toLong();
                break;
            }
        }
#else
    }else if(type()==typeid(QAnyStringView)){
        const QAnyStringView* sref = reinterpret_cast<const QAnyStringView*>(qt_object);
        if(sref){
            switch (javaType) {
            case jValueType::l:
                jstring qtjambi_from_qanystringview(JNIEnv *env, QAnyStringView s);
                output->l = qtjambi_from_qanystringview(env, *sref);
                break;
            case jValueType::z:
                output->z = !sref->isEmpty();
                break;
            case jValueType::c:
                output->c = !sref->toString().isEmpty() ? sref->toString().at(0).unicode() : 0;
                break;
            default:
                output->j = sref->toString().toLong();
                break;
            }
        }
    }else if(type()==typeid(QUtf8StringView)){
        const QUtf8StringView* sref = reinterpret_cast<const QUtf8StringView*>(qt_object);
        if(sref){
            switch (javaType) {
            case jValueType::l:
                jstring qtjambi_from_qutf8stringview(JNIEnv *env, QUtf8StringView s);
                output->l = qtjambi_from_qutf8stringview(env, *sref);
                break;
            case jValueType::z:
                output->z = !sref->isEmpty();
                break;
            case jValueType::c:
                output->c = !sref->toString().isEmpty() ? sref->toString().at(0).unicode() : 0;
                break;
            default:
                output->j = sref->toString().toLong();
                break;
            }
        }
#endif // QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    }else{
        const QLatin1String* sref = reinterpret_cast<const QLatin1String*>(qt_object);
        if(sref){
            switch (javaType) {
            case jValueType::l:
                output->l = qtjambi_from_qstring(env, *sref);
                break;
            case jValueType::z:
                output->z = !sref->isEmpty();
                break;
            case jValueType::c:
                output->c = !sref->isEmpty() ? sref->at(0).unicode() : 0;
                break;
            default:
                output->j = QString(*sref).toLong();
                break;
            }
        }
    }
    return true;
}

bool StringUtilTypeEntry::convertToNative(JNIEnv *env, const jvalue&, jValueType, void *) const{
    if(type()==typeid(QStringView)){
        JavaException::raiseError(env, "Cannot convert to QStringView" QTJAMBI_STACKTRACEINFO );
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    }else if(type()==typeid(QStringRef)){
        JavaException::raiseError(env, "Cannot convert to QStringRef" QTJAMBI_STACKTRACEINFO );
    }else if(type()==typeid(QXmlStreamStringRef)){
        JavaException::raiseError(env, "Cannot convert to QXmlStreamStringRef" QTJAMBI_STACKTRACEINFO );
#else
    }else if(type()==typeid(QAnyStringView)){
        JavaException::raiseError(env, "Cannot convert to QAnyStringView" QTJAMBI_STACKTRACEINFO );
    }else if(type()==typeid(QUtf8StringView)){
        JavaException::raiseError(env, "Cannot convert to QUtf8StringView" QTJAMBI_STACKTRACEINFO );
#endif // QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    }else {
        JavaException::raiseError(env, "Cannot convert to QLatin1String" QTJAMBI_STACKTRACEINFO );
    }
    return false;
}

MetaUtilTypeEntry::MetaUtilTypeEntry(JNIEnv* env, const std::type_info& typeId, const char *qt_name, const char *java_name, jclass java_class, size_t value_size)
    : AbstractSimpleTypeEntry(env, typeId, qt_name, java_name, java_class, value_size)
{
}

jobject qtjambi_from_QMetaObject(JNIEnv *env, const QMetaObject *metaObject);

jobject qtjambi_from_QMetaObject$Connection(JNIEnv *env, const QMetaObject::Connection& connection){
    static QMetaType metaTypeId(registeredMetaTypeID(typeid(QMetaObject::Connection)));
    jobject result = Java::QtJambi::QtJambiSignals$NativeConnection::newInstance(env, nullptr);
    const QSharedPointer<QtJambiLink>& link = QtJambiLink::createLinkForObject(
            env,
            result,
            new QMetaObject::Connection(connection),
            metaTypeId,
            false,
            false
        );
    link->setJavaOwnership(env);
    return result;
}

QMetaObject::Connection qtjambi_to_QMetaObject$Connection(JNIEnv *env, jobject object){
    if (QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForJavaObject(env, object)){
        Q_ASSERT(!link->isQObject());
        return *reinterpret_cast<QMetaObject::Connection*>(link->pointer());
    }
    else if(Java::QtJambi::QtObjectInterface::isInstanceOf(env, object))
        Java::QtJambi::QNoNativeResourcesException::throwNew(env, QString("Incomplete object of type: %1").arg(qtjambi_object_class_name(env, object).replace("$", ".")) QTJAMBI_STACKTRACEINFO );
    return QMetaObject::Connection();
}

const QMetaObject *qtjambi_to_QMetaObject(JNIEnv *env, jobject object){
    if(!object)
        return nullptr;
    jlong ptr = Java::QtCore::QMetaObject::metaObjectPointer(env, object);
    return reinterpret_cast<const QMetaObject *>(ptr);
}

bool MetaUtilTypeEntry::convertToJava(JNIEnv *env, const void *qt_object, bool, bool, jvalue* output, jValueType javaType) const{
    if(javaType!=jValueType::l)
        JavaException::raiseIllegalArgumentException(env, "Cannot convert to primitive value" QTJAMBI_STACKTRACEINFO );
    if(qt_object){
        if(type()==typeid(QMetaObject::Connection)){
            output->l = qtjambi_from_QMetaObject$Connection(env, *reinterpret_cast<const QMetaObject::Connection*>(qt_object));
        }else if(type()==typeid(QMetaObject)){
            output->l = qtjambi_from_QMetaObject(env, reinterpret_cast<const QMetaObject*>(qt_object));
        }else if(type()==typeid(JIteratorWrapper)
                 || type()==typeid(JCollectionWrapper)
                 || type()==typeid(JMapWrapper)
                 || type()==typeid(JObjectWrapper)
                 || type()==typeid(JEnumWrapper)){
            output->l = env->NewLocalRef(reinterpret_cast<const JObjectWrapper*>(qt_object)->object());
        }
    }
    return true;
}

bool MetaUtilTypeEntry::convertToNative(JNIEnv *env, const jvalue& java_value, jValueType javaType, void *output) const{
    if(javaType!=jValueType::l)
        JavaException::raiseIllegalArgumentException(env, "Cannot convert to primitive value" QTJAMBI_STACKTRACEINFO );
    if(type()==typeid(QMetaObject::Connection)){
        if(java_value.l && !Java::QtCore::QMetaObject$Connection::isInstanceOf(env, java_value.l))
            return false;
        *reinterpret_cast<QMetaObject::Connection*>(output) = qtjambi_to_QMetaObject$Connection(env, java_value.l);
    }else if(type()==typeid(QMetaObject)){
        if(java_value.l && !Java::QtCore::QMetaObject::isInstanceOf(env, java_value.l))
            return false;
        *reinterpret_cast<const QMetaObject**>(output) = qtjambi_to_QMetaObject(env, java_value.l);
    }else if(type()==typeid(JIteratorWrapper)
             || type()==typeid(JCollectionWrapper)
             || type()==typeid(JMapWrapper)
             || type()==typeid(JObjectWrapper)
             || type()==typeid(JEnumWrapper)){
        *reinterpret_cast<JObjectWrapper*>(output) = JObjectWrapper(env, java_value.l);
    }
    return true;
}

QModelIndexTypeEntry::QModelIndexTypeEntry(JNIEnv* env, const std::type_info& typeId, const char *qt_name, const char *java_name, jclass java_class, size_t value_size)
    : AbstractSimpleTypeEntry(env, typeId, qt_name, java_name, java_class, value_size)
{
}

bool QModelIndexTypeEntry::convertToJava(JNIEnv *env, const void *qt_object, bool, bool, jvalue* output, jValueType javaType) const{
    if(javaType!=jValueType::l)
        JavaException::raiseIllegalArgumentException(env, "Cannot convert QModelIndex to primitive value" QTJAMBI_STACKTRACEINFO );
    output->l = qtjambi_from_QModelIndex(env, *reinterpret_cast<const QModelIndex*>(qt_object));
    return true;
}

bool QModelIndexTypeEntry::convertToNative(JNIEnv *env, const jvalue& java_value, jValueType javaType, void *output) const{
    if(javaType!=jValueType::l)
        JavaException::raiseIllegalArgumentException(env, "Cannot convert primitive value to QModelIndex" QTJAMBI_STACKTRACEINFO );
    if(java_value.l && !Java::QtCore::QModelIndex::isInstanceOf(env, java_value.l))
        return false;
    *reinterpret_cast<QModelIndex*>(output) = qtjambi_to_QModelIndex(env, java_value.l);
    return true;
}

QVariantTypeEntry::QVariantTypeEntry(JNIEnv* env, const std::type_info& typeId, const char *qt_name, const char *java_name, jclass java_class, size_t value_size)
    : AbstractSimpleTypeEntry(env, typeId, qt_name, java_name, java_class, value_size)
{
}

bool QVariantTypeEntry::convertToJava(JNIEnv *env, const void *qt_object, bool, bool, jvalue* output, jValueType javaType) const{
    if(javaType!=jValueType::l)
        JavaException::raiseIllegalArgumentException(env, "Cannot convert QVariant to primitive value" QTJAMBI_STACKTRACEINFO );
    output->l = qtjambi_from_qvariant(env, *reinterpret_cast<const QVariant*>(qt_object));
    return true;
}

bool QVariantTypeEntry::convertToNative(JNIEnv *env, const jvalue& java_value, jValueType javaType, void *output) const{
    if(javaType!=jValueType::l)
        JavaException::raiseIllegalArgumentException(env, "Cannot convert primitive value to QVariant" QTJAMBI_STACKTRACEINFO );
    *reinterpret_cast<QVariant*>(output) = qtjambi_to_qvariant(env, java_value.l);
    return true;
}

bool JLongTypeEntry::convertToJava(JNIEnv *env, const void *qt_object, bool, bool, jvalue* output, jValueType javaType) const{
    const jlong* value = reinterpret_cast<const jlong*>(qt_object);
    if(value){
        switch (javaType) {
        case jValueType::z:
            output->z = *value!=0;
            break;
        case jValueType::b:
            output->b = jbyte(*value);
            break;
        case jValueType::c:
            output->c = jchar(*value);
            break;
        case jValueType::s:
            output->s = jshort(*value);
            break;
        case jValueType::i:
            output->i = jint(*value);
            break;
        case jValueType::j:
            output->j = *value;
            break;
        case jValueType::f:
            output->f = jfloat(*value);
            break;
        case jValueType::d:
            output->d = jdouble(*value);
            break;
        case jValueType::l:
            output->l = qtjambi_from_long(env, *value);
            break;
        }
    }
    return true;
}

bool JLongTypeEntry::convertToNative(JNIEnv *env, const jvalue& java_value, jValueType javaType, void * output) const{
    jlong* value = reinterpret_cast<jlong*>(output);
    if(value){
        switch (javaType) {
        case jValueType::z:
            *value = java_value.z;
            break;
        case jValueType::b:
            *value = java_value.b;
            break;
        case jValueType::c:
            *value = java_value.c;
            break;
        case jValueType::s:
            *value = java_value.s;
            break;
        case jValueType::i:
            *value = java_value.i;
            break;
        case jValueType::j:
            *value = java_value.j;
            break;
        case jValueType::f:
            *value = jlong(java_value.f);
            break;
        case jValueType::d:
            *value = jlong(java_value.d);
            break;
        case jValueType::l:
            if(java_value.l && !Java::Runtime::Byte::isInstanceOf(env, java_value.l)
                     && !Java::Runtime::Short::isInstanceOf(env, java_value.l)
                     && !Java::Runtime::Integer::isInstanceOf(env, java_value.l)
                     && !Java::Runtime::Long::isInstanceOf(env, java_value.l))
                return false;
            *value = qtjambi_to_long(env, java_value.l);
            break;
        }
    }
    return true;
}

JIntTypeEntry::JIntTypeEntry(JNIEnv* env, const std::type_info& typeId, const char *qt_name, const char *java_name, jclass java_class, size_t value_size)
    : AbstractSimpleTypeEntry(env, typeId, qt_name, java_name, java_class, value_size)
{
}

bool JIntTypeEntry::convertToJava(JNIEnv *env, const void *qt_object, bool, bool, jvalue* output, jValueType javaType) const{
    const jint* value = reinterpret_cast<const jint*>(qt_object);
    if(value){
        switch (javaType) {
        case jValueType::z:
            output->z = *value!=0;
            break;
        case jValueType::b:
            output->b = jbyte(*value);
            break;
        case jValueType::c:
            output->c = jchar(*value);
            break;
        case jValueType::s:
            output->s = jshort(*value);
            break;
        case jValueType::i:
            output->i = jint(*value);
            break;
        case jValueType::j:
            output->j = jlong(*value);
            break;
        case jValueType::f:
            output->f = jfloat(*value);
            break;
        case jValueType::d:
            output->d = jdouble(*value);
            break;
        case jValueType::l:
            output->l = qtjambi_from_int(env, *value);
            break;
        }
    }
    return true;
}

bool JIntTypeEntry::convertToNative(JNIEnv *env, const jvalue& java_value, jValueType javaType, void * output) const{
    jint* value = reinterpret_cast<jint*>(output);
    if(value){
        switch (javaType) {
        case jValueType::z:
            *value = java_value.z;
            break;
        case jValueType::b:
            *value = java_value.b;
            break;
        case jValueType::c:
            *value = java_value.c;
            break;
        case jValueType::s:
            *value = java_value.s;
            break;
        case jValueType::i:
            *value = java_value.i;
            break;
        case jValueType::j:
            *value = jint(java_value.j);
            break;
        case jValueType::f:
            *value = jint(java_value.f);
            break;
        case jValueType::d:
            *value = jint(java_value.d);
            break;
        case jValueType::l:
            if(java_value.l && !Java::Runtime::Byte::isInstanceOf(env, java_value.l)
                     && !Java::Runtime::Short::isInstanceOf(env, java_value.l)
                     && !Java::Runtime::Integer::isInstanceOf(env, java_value.l))
                return false;
            *value = qtjambi_to_int(env, java_value.l);
            break;
        }
    }
    return true;
}

JShortTypeEntry::JShortTypeEntry(JNIEnv* env, const std::type_info& typeId, const char *qt_name, const char *java_name, jclass java_class, size_t value_size)
    : AbstractSimpleTypeEntry(env, typeId, qt_name, java_name, java_class, value_size)
{
}

bool JShortTypeEntry::convertToJava(JNIEnv *env, const void *qt_object, bool, bool, jvalue* output, jValueType javaType) const{
    const jshort* value = reinterpret_cast<const jshort*>(qt_object);
    if(value){
        switch (javaType) {
        case jValueType::z:
            output->z = *value!=0;
            break;
        case jValueType::b:
            output->b = jbyte(*value);
            break;
        case jValueType::c:
            output->c = jchar(*value);
            break;
        case jValueType::s:
            output->s = jshort(*value);
            break;
        case jValueType::i:
            output->i = jint(*value);
            break;
        case jValueType::j:
            output->j = jlong(*value);
            break;
        case jValueType::f:
            output->f = jfloat(*value);
            break;
        case jValueType::d:
            output->d = jdouble(*value);
            break;
        case jValueType::l:
            output->l = qtjambi_from_short(env, *value);
            break;
        }
    }
    return true;
}

bool JShortTypeEntry::convertToNative(JNIEnv *env, const jvalue& java_value, jValueType javaType, void * output) const{
    jshort* value = reinterpret_cast<jshort*>(output);
    if(value){
        switch (javaType) {
        case jValueType::z:
            *value = java_value.z;
            break;
        case jValueType::b:
            *value = java_value.b;
            break;
        case jValueType::c:
            *value = jshort(java_value.c);
            break;
        case jValueType::s:
            *value = java_value.s;
            break;
        case jValueType::i:
            *value = jshort(java_value.i);
            break;
        case jValueType::j:
            *value = jshort(java_value.j);
            break;
        case jValueType::f:
            *value = jshort(java_value.f);
            break;
        case jValueType::d:
            *value = jshort(java_value.d);
            break;
        case jValueType::l:
            if(java_value.l && !Java::Runtime::Byte::isInstanceOf(env, java_value.l)
                     && !Java::Runtime::Short::isInstanceOf(env, java_value.l)
                     && !Java::Runtime::Integer::isInstanceOf(env, java_value.l))
                return false;
            *value = qtjambi_to_short(env, java_value.l);
            break;
        }
    }
    return true;
}

JByteTypeEntry::JByteTypeEntry(JNIEnv* env, const std::type_info& typeId, const char *qt_name, const char *java_name, jclass java_class, size_t value_size)
    : AbstractSimpleTypeEntry(env, typeId, qt_name, java_name, java_class, value_size)
{
}

bool JByteTypeEntry::convertToJava(JNIEnv *env, const void *qt_object, bool, bool, jvalue* output, jValueType javaType) const{
    const jbyte* value = reinterpret_cast<const jbyte*>(qt_object);
    if(value){
        switch (javaType) {
        case jValueType::z:
            output->z = *value!=0;
            break;
        case jValueType::b:
            output->b = jbyte(*value);
            break;
        case jValueType::c:
            output->c = jchar(*value);
            break;
        case jValueType::s:
            output->s = jshort(*value);
            break;
        case jValueType::i:
            output->i = jint(*value);
            break;
        case jValueType::j:
            output->j = jlong(*value);
            break;
        case jValueType::f:
            output->f = jfloat(*value);
            break;
        case jValueType::d:
            output->d = jdouble(*value);
            break;
        case jValueType::l:
            output->l = qtjambi_from_byte(env, *value);
            break;
        }
    }
    return true;
}

bool JByteTypeEntry::convertToNative(JNIEnv *env, const jvalue& java_value, jValueType javaType, void * output) const{
    jbyte* value = reinterpret_cast<jbyte*>(output);
    if(value){
        switch (javaType) {
        case jValueType::z:
            *value = jbyte(java_value.z);
            break;
        case jValueType::b:
            *value = java_value.b;
            break;
        case jValueType::c:
            *value = jbyte(java_value.c);
            break;
        case jValueType::s:
            *value = jbyte(java_value.s);
            break;
        case jValueType::i:
            *value = jbyte(java_value.i);
            break;
        case jValueType::j:
            *value = jbyte(java_value.j);
            break;
        case jValueType::f:
            *value = jbyte(java_value.f);
            break;
        case jValueType::d:
            *value = jbyte(java_value.d);
            break;
        case jValueType::l:
            if(java_value.l && !Java::Runtime::Byte::isInstanceOf(env, java_value.l)
                     && !Java::Runtime::Short::isInstanceOf(env, java_value.l)
                     && !Java::Runtime::Integer::isInstanceOf(env, java_value.l))
                return false;
            *value = qtjambi_to_byte(env, java_value.l);
            break;
        }
    }
    return true;
}

JCharTypeEntry::JCharTypeEntry(JNIEnv* env, const std::type_info& typeId, const char *qt_name, const char *java_name, jclass java_class, size_t value_size)
    : AbstractSimpleTypeEntry(env, typeId, qt_name, java_name, java_class, value_size)
{
}

bool JCharTypeEntry::convertToJava(JNIEnv *env, const void *qt_object, bool, bool, jvalue* output, jValueType javaType) const{
    if(type()==typeid(wchar_t) || type()==typeid(jchar)){
        const jchar* value = reinterpret_cast<const jchar*>(qt_object);
        if(value){
            switch (javaType) {
            case jValueType::z:
                output->z = *value!=0;
                break;
            case jValueType::b:
                output->b = jbyte(*value);
                break;
            case jValueType::c:
                output->c = jchar(*value);
                break;
            case jValueType::s:
                output->s = jshort(*value);
                break;
            case jValueType::i:
                output->i = jint(*value);
                break;
            case jValueType::j:
                output->j = jlong(*value);
                break;
            case jValueType::f:
                output->f = jfloat(*value);
                break;
            case jValueType::d:
                output->d = jdouble(*value);
                break;
            case jValueType::l:
                output->l = qtjambi_from_char(env, *value);
                break;
            }
        }
    }else if(type()==typeid(QChar)){
        const QChar* value = reinterpret_cast<const QChar*>(qt_object);
        if(value){
            switch (javaType) {
            case jValueType::z:
                output->z = value->cell()!=0;
                break;
            case jValueType::b:
                output->b = jbyte(value->cell());
                break;
            case jValueType::c:
                output->c = jchar(value->cell());
                break;
            case jValueType::s:
                output->s = jshort(value->cell());
                break;
            case jValueType::i:
                output->i = jint(value->cell());
                break;
            case jValueType::j:
                output->j = jlong(value->cell());
                break;
            case jValueType::f:
                output->f = jfloat(value->cell());
                break;
            case jValueType::d:
                output->d = jdouble(value->cell());
                break;
            case jValueType::l:
                output->l = qtjambi_from_char(env, value->cell());
                break;
            }
        }
    }else if(type()==typeid(QLatin1Char)){
        const QLatin1Char* value = reinterpret_cast<const QLatin1Char*>(qt_object);
        if(value){
            switch (javaType) {
            case jValueType::z:
                output->z = value->unicode()!=0;
                break;
            case jValueType::b:
                output->b = jbyte(value->unicode());
                break;
            case jValueType::c:
                output->c = jchar(value->unicode());
                break;
            case jValueType::s:
                output->s = jshort(value->unicode());
                break;
            case jValueType::i:
                output->i = jint(value->unicode());
                break;
            case jValueType::j:
                output->j = jlong(value->unicode());
                break;
            case jValueType::f:
                output->f = jfloat(value->unicode());
                break;
            case jValueType::d:
                output->d = jdouble(value->unicode());
                break;
            case jValueType::l:
                output->l = qtjambi_from_char(env, value->unicode());
                break;
            }
        }
    }
    return true;
}

bool JCharTypeEntry::convertToNative(JNIEnv *env, const jvalue& java_value, jValueType javaType, void * output) const{
    if(type()==typeid(wchar_t) || type()==typeid(jchar)){
        jchar* value = reinterpret_cast<jchar*>(output);
        if(value){
            switch (javaType) {
            case jValueType::z:
                *value = jchar(java_value.z);
                break;
            case jValueType::b:
                *value = jchar(java_value.b);
                break;
            case jValueType::c:
                *value = jchar(java_value.c);
                break;
            case jValueType::s:
                *value = jchar(java_value.s);
                break;
            case jValueType::i:
                *value = jchar(java_value.i);
                break;
            case jValueType::j:
                *value = jchar(java_value.j);
                break;
            case jValueType::f:
                *value = jchar(java_value.f);
                break;
            case jValueType::d:
                *value = jchar(java_value.d);
                break;
            case jValueType::l:
                if(java_value.l && !Java::Runtime::Character::isInstanceOf(env, java_value.l))
                    return false;
                *value = qtjambi_to_char(env, java_value.l);
                break;
            }
        }
    }else if(type()==typeid(QChar)){
        QChar* value = reinterpret_cast<QChar*>(output);
        if(value){
            switch (javaType) {
            case jValueType::z:
                *value = jchar(java_value.z);
                break;
            case jValueType::b:
                *value = jchar(java_value.b);
                break;
            case jValueType::c:
                *value = jchar(java_value.c);
                break;
            case jValueType::s:
                *value = jchar(java_value.s);
                break;
            case jValueType::i:
                *value = jchar(java_value.i);
                break;
            case jValueType::j:
                *value = jchar(java_value.j);
                break;
            case jValueType::f:
                *value = jchar(java_value.f);
                break;
            case jValueType::d:
                *value = jchar(java_value.d);
                break;
            case jValueType::l:
                if(java_value.l && !Java::Runtime::Character::isInstanceOf(env, java_value.l))
                    return false;
                *value = qtjambi_to_char(env, java_value.l);
                break;
            }
        }
    }else if(type()==typeid(QLatin1Char)){
        QLatin1Char* value = reinterpret_cast<QLatin1Char*>(output);
        if(value){
            switch (javaType) {
            case jValueType::z:
                *value = QLatin1Char(jbyte(java_value.z));
                break;
            case jValueType::b:
                *value = QLatin1Char(jbyte(java_value.b));
                break;
            case jValueType::c:
                *value = QLatin1Char(jbyte(java_value.c));
                break;
            case jValueType::s:
                *value = QLatin1Char(jbyte(java_value.s));
                break;
            case jValueType::i:
                *value = QLatin1Char(jbyte(java_value.i));
                break;
            case jValueType::j:
                *value = QLatin1Char(jbyte(java_value.j));
                break;
            case jValueType::f:
                *value = QLatin1Char(jbyte(java_value.f));
                break;
            case jValueType::d:
                *value = QLatin1Char(jbyte(java_value.d));
                break;
            case jValueType::l:
                if(java_value.l && !Java::Runtime::Character::isInstanceOf(env, java_value.l))
                    return false;
                *value = QLatin1Char(jbyte(qtjambi_to_char(env, java_value.l)));
                break;
            }
        }
    }
    return true;
}

JBooleanTypeEntry::JBooleanTypeEntry(JNIEnv* env, const std::type_info& typeId, const char *qt_name, const char *java_name, jclass java_class, size_t value_size)
    : AbstractSimpleTypeEntry(env, typeId, qt_name, java_name, java_class, value_size)
{
}

bool JBooleanTypeEntry::convertToJava(JNIEnv *env, const void *qt_object, bool, bool, jvalue* output, jValueType javaType) const{
    const jboolean* value = reinterpret_cast<const jboolean*>(qt_object);
    if(value){
        switch (javaType) {
        case jValueType::z:
            output->z = *value;
            break;
        case jValueType::b:
            output->b = jbyte(*value);
            break;
        case jValueType::c:
            output->c = jchar(*value);
            break;
        case jValueType::s:
            output->s = jshort(*value);
            break;
        case jValueType::i:
            output->i = jint(*value);
            break;
        case jValueType::j:
            output->j = jlong(*value);
            break;
        case jValueType::f:
            output->f = jfloat(*value);
            break;
        case jValueType::d:
            output->d = jdouble(*value);
            break;
        case jValueType::l:
            output->l = qtjambi_from_boolean(env, *value);
            break;
        }
    }
    return true;
}

bool JBooleanTypeEntry::convertToNative(JNIEnv *env, const jvalue& java_value, jValueType javaType, void * output) const{
    jboolean* value = reinterpret_cast<jboolean*>(output);
    if(value){
        switch (javaType) {
        case jValueType::z:
            *value = jboolean(java_value.z);
            break;
        case jValueType::b:
            *value = jboolean(java_value.b);
            break;
        case jValueType::c:
            *value = jboolean(java_value.c);
            break;
        case jValueType::s:
            *value = jboolean(java_value.s);
            break;
        case jValueType::i:
            *value = jboolean(java_value.i);
            break;
        case jValueType::j:
            *value = jboolean(java_value.j);
            break;
        case jValueType::f:
            *value = jboolean(java_value.f);
            break;
        case jValueType::d:
            *value = jboolean(java_value.d);
            break;
        case jValueType::l:
            if(java_value.l && !Java::Runtime::Boolean::isInstanceOf(env, java_value.l))
                return false;
            *value = qtjambi_to_boolean(env, java_value.l);
            break;
        }
    }
    return true;
}

JDoubleTypeEntry::JDoubleTypeEntry(JNIEnv* env, const std::type_info& typeId, const char *qt_name, const char *java_name, jclass java_class, size_t value_size)
    : AbstractSimpleTypeEntry(env, typeId, qt_name, java_name, java_class, value_size)
{
}

bool JDoubleTypeEntry::convertToJava(JNIEnv *env, const void *qt_object, bool, bool, jvalue* output, jValueType javaType) const{
    const jdouble* value = reinterpret_cast<const jdouble*>(qt_object);
    if(value){
        switch (javaType) {
        case jValueType::z:
            output->z = jboolean(*value);
            break;
        case jValueType::b:
            output->b = jbyte(*value);
            break;
        case jValueType::c:
            output->c = jchar(*value);
            break;
        case jValueType::s:
            output->s = jshort(*value);
            break;
        case jValueType::i:
            output->i = jint(*value);
            break;
        case jValueType::j:
            output->j = jlong(*value);
            break;
        case jValueType::f:
            output->f = jfloat(*value);
            break;
        case jValueType::d:
            output->d = jdouble(*value);
            break;
        case jValueType::l:
            output->l = qtjambi_from_double(env, *value);
            break;
        }
    }
    return true;
}

bool JDoubleTypeEntry::convertToNative(JNIEnv *env, const jvalue& java_value, jValueType javaType, void * output) const{
    jdouble* value = reinterpret_cast<jdouble*>(output);
    if(value){
        switch (javaType) {
        case jValueType::z:
            *value = java_value.z;
            break;
        case jValueType::b:
            *value = java_value.b;
            break;
        case jValueType::c:
            *value = java_value.c;
            break;
        case jValueType::s:
            *value = java_value.s;
            break;
        case jValueType::i:
            *value = java_value.i;
            break;
        case jValueType::j:
            *value = jdouble(java_value.j);
            break;
        case jValueType::f:
            *value = jdouble(java_value.f);
            break;
        case jValueType::d:
            *value = jdouble(java_value.d);
            break;
        case jValueType::l:
            if(java_value.l && !Java::Runtime::Number::isInstanceOf(env, java_value.l))
                return false;
            *value = qtjambi_to_double(env, java_value.l);
            break;
        }
    }
    return true;
}

JFloatTypeEntry::JFloatTypeEntry(JNIEnv* env, const std::type_info& typeId, const char *qt_name, const char *java_name, jclass java_class, size_t value_size)
    : AbstractSimpleTypeEntry(env, typeId, qt_name, java_name, java_class, value_size)
{
}

bool JFloatTypeEntry::convertToJava(JNIEnv *env, const void *qt_object, bool, bool, jvalue* output, jValueType javaType) const{
    const jfloat* value = reinterpret_cast<const jfloat*>(qt_object);
    if(value){
        switch (javaType) {
        case jValueType::z:
            output->z = jboolean(*value);
            break;
        case jValueType::b:
            output->b = jbyte(*value);
            break;
        case jValueType::c:
            output->c = jchar(*value);
            break;
        case jValueType::s:
            output->s = jshort(*value);
            break;
        case jValueType::i:
            output->i = jint(*value);
            break;
        case jValueType::j:
            output->j = jlong(*value);
            break;
        case jValueType::f:
            output->f = jfloat(*value);
            break;
        case jValueType::d:
            output->d = jdouble(*value);
            break;
        case jValueType::l:
            output->l = qtjambi_from_float(env, *value);
            break;
        }
    }
    return true;
}

bool JFloatTypeEntry::convertToNative(JNIEnv *env, const jvalue& java_value, jValueType javaType, void * output) const{
    jfloat* value = reinterpret_cast<jfloat*>(output);
    if(value){
        switch (javaType) {
        case jValueType::z:
            *value = java_value.z;
            break;
        case jValueType::b:
            *value = java_value.b;
            break;
        case jValueType::c:
            *value = java_value.c;
            break;
        case jValueType::s:
            *value = java_value.s;
            break;
        case jValueType::i:
            *value = jfloat(java_value.i);
            break;
        case jValueType::j:
            *value = jfloat(java_value.j);
            break;
        case jValueType::f:
            *value = java_value.f;
            break;
        case jValueType::d:
            *value = jfloat(java_value.d);
            break;
        case jValueType::l:
            if(java_value.l && !Java::Runtime::Integer::isInstanceOf(env, java_value.l)
                            && !Java::Runtime::Byte::isInstanceOf(env, java_value.l)
                            && !Java::Runtime::Short::isInstanceOf(env, java_value.l)
                            && !Java::Runtime::Float::isInstanceOf(env, java_value.l))
                return false;
            *value = qtjambi_to_float(env, java_value.l);
            break;
        }
    }
    return true;
}

NullptrTypeEntry::NullptrTypeEntry(JNIEnv* env, const std::type_info& typeId, const char *qt_name, const char *java_name, jclass java_class, size_t value_size)
    : AbstractSimpleTypeEntry(env, typeId, qt_name, java_name, java_class, value_size)
{
}

bool NullptrTypeEntry::convertToJava(JNIEnv *, const void *, bool, bool, jvalue* output, jValueType) const{
    output->l = nullptr;
    return true;
}

bool NullptrTypeEntry::convertToNative(JNIEnv *, const jvalue&, jValueType, void * output) const{
    void** value = reinterpret_cast<void**>(output);
    *value = nullptr;
    return true;
}
