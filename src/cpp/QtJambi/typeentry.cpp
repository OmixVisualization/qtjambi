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

#include <QtCore/qcompilerdetection.h>
QT_WARNING_DISABLE_DEPRECATED

#include <QtCore/QUrl>
#include <QtCore/QModelIndex>
#include <QtCore/QMessageLogContext>
#include <QtCore/QCborValue>
#include <QtCore/QCborValueRef>
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
#include <QtCore/QXmlStreamStringRef>
#endif
#include "qtjambiapi.h"
#include "functionalbase.h"
#include "java_p.h"
#include "typeentry_p.h"
#include "registryutil_p.h"
#include "qtjambilink_p.h"
#include "qtjambishell_p.h"
#include "threadutils_p.h"
#include "qtjambimetaobject_p.h"
#include "qtjambi_cast.h"
#include "supertypeinfo_p.h"

bool isQObject(const std::type_info& typeId);

typedef QMap<size_t, QMap<QByteArray,QtJambiTypeEntryPtr>> TypeEntryHash;
Q_GLOBAL_STATIC(TypeEntryHash, gTypeEntryHash)
Q_GLOBAL_STATIC_WITH_ARGS(QReadWriteLock, gTypeEntryLock, (QReadWriteLock::Recursive))

const char* getJavaNameByFunctional(const char* qt_name);
QList<const PolymorphicIdHandler*> getPolymorphicIdHandlers(const std::type_info& polymorphicBaseTypeId);

void clear_type_entry(const std::type_info& typeId)
{
    QMap<QByteArray,QtJambiTypeEntryPtr> typeEntries;
    {
        QWriteLocker locker(gTypeEntryLock());
        if(gTypeEntryHash()->contains(unique_id(typeId))){
            typeEntries = gTypeEntryHash()->take(unique_id(typeId));
        }
    }
}

QtJambiTypeEntryPtr get_type_entry(JNIEnv* env, const std::type_info& typeId, bool recursive, const char* qtName)
{
    {
        QReadLocker locker(gTypeEntryLock());
        Q_UNUSED(locker)
        if(gTypeEntryHash()->contains(unique_id(typeId))){
            const QMap<QByteArray,QtJambiTypeEntryPtr>& entries = (*gTypeEntryHash())[unique_id(typeId)];
            if(qtName){
                if(QtJambiTypeEntryPtr result = entries.value(qtName)){
                    return result;
                }
            }else if(!entries.isEmpty()){
                return entries.first();
            }
        }
    }
    QtJambiTypeEntryPtr result;
    const char *qt_name = nullptr;
    EntryTypes entryType = getEntryType(typeId);
    if(entryType!=EntryTypes::Unspecific){
        QTJAMBI_JNI_LOCAL_FRAME(env, 512);
        qt_name = getQtName(typeId);
        const char *java_name = getJavaName(typeId);
        if(qtName && entryType==EntryTypes::FunctionalTypeInfo){
            if(QLatin1String(qtName)!=QLatin1String(qt_name)){
                qt_name = qtName;
                java_name = getJavaNameByFunctional(qtName);
            }
        }
        jclass java_class = JavaAPI::resolveClass(env, java_name);
        if(!java_class){
            JavaException::raiseError(env, "class cannot be found" QTJAMBI_STACKTRACEINFO );
            return QtJambiTypeEntryPtr();
        }

        switch(entryType){
        case EntryTypes::EnumTypeInfo:
        {
            if(recursive){
                const std::type_info* flagId = getFlagForEnum(typeId);
                if(flagId){
                    QtJambiTypeEntryPtr e = QtJambiTypeEntry::getTypeEntry(env, *flagId);
                    if(const FlagsTypeEntry* flagsType = dynamic_cast<const FlagsTypeEntry*>(e.data())){
                        return QtJambiTypeEntryPtr(flagsType->enumType());
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
            jthrowable exceptionOccurred = nullptr;
            jmethodID creator_method = JavaAPI::resolveMethod(env, "resolve", qPrintable(sig.arg(java_name)), java_class, true, &exceptionOccurred);
            if(exceptionOccurred)
                JavaException(env, exceptionOccurred).raise();
            Q_ASSERT(creator_method);
            result = new EnumTypeEntry(env, typeId, qt_name, java_name, java_class, creator_method, value_size);
            break;
        }
        case EntryTypes::FlagsTypeInfo:
        {
            size_t value_size = getValueSize(typeId);
            const std::type_info* flagId = getEnumForFlag(typeId);
            Q_ASSERT(flagId);
            jthrowable exceptionOccurred = nullptr;
            jmethodID creator_method = JavaAPI::resolveMethod(env, "<init>", "(I)V", java_class, false, &exceptionOccurred);
            if(exceptionOccurred)
                JavaException(env, exceptionOccurred).raise();
            Q_ASSERT(creator_method);
            auto e = get_type_entry(env, *flagId, false, nullptr);
            result = new FlagsTypeEntry(env, typeId, qt_name, java_name, java_class, creator_method, value_size, dynamic_cast<const EnumTypeEntry*>(e.data()));
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
                java_impl_class = JavaAPI::resolveClass(env, qPrintable(QString("%1$Impl").arg(java_name)));
                if(!java_impl_class){
                    JavaException::raiseError(env, "class cannot be found" QTJAMBI_STACKTRACEINFO );
                    return QtJambiTypeEntryPtr();
                }
                int modifiers = Java::Runtime::Class::getModifiers(env,java_impl_class);
                if(Java::Runtime::Modifier::isAbstract(env, modifiers)){
                    java_wrapper_class = JavaAPI::resolveClass(env, qPrintable(QString("%1$Impl$ConcreteWrapper").arg(java_name)));
                }else{
                    java_wrapper_class = java_impl_class;
                }
            }else{
                java_impl_class = java_class;
                int modifiers = Java::Runtime::Class::getModifiers(env,java_impl_class);
                if(Java::Runtime::Modifier::isAbstract(env, modifiers)){
                    java_wrapper_class = JavaAPI::resolveClass(env, qPrintable(QString("%1$ConcreteWrapper").arg(java_name)));
                }else{
                    java_wrapper_class = java_impl_class;
                }
            }
            creator_method = findInternalPrivateConstructor(env, java_wrapper_class);
            if(!creator_method){
                JavaException::raiseError(env, "constructor cannot be found" QTJAMBI_STACKTRACEINFO );
                return QtJambiTypeEntryPtr();
            }
            const QVector<const FunctionInfo>* _virtualFunctions = virtualFunctions(typeId);
            Destructor destructor = registeredDestructor(typeId);
            FunctionalResolver registered_functional_resolver = registeredFunctionalResolver(typeId);
            QMetaType qt_meta_type(registeredMetaTypeID(typeId));
            Q_ASSERT(qt_meta_type.isValid());
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
            QString typeName = QtJambiAPI::typeName(typeId);
            bool is_std_function = typeName.startsWith("std::function") || !typeName.contains("(*)");
#else
            bool is_std_function = !(qt_meta_type.flags() & QMetaType::IsPointer);
#endif
            result = new FunctionalTypeEntry(env, typeId, qt_name, java_name, java_class, creator_method, value_size, shell_size,
                                             java_impl_class, java_wrapper_class, _virtualFunctions, destructor, registered_functional_resolver, qt_meta_type, is_std_function);
            break;
        }
        case EntryTypes::ObjectTypeInfo:
        {
            const std::type_info* super_type = nullptr; // TODO
            size_t value_size = getValueSize(typeId);
            InterfaceOffsetInfo interfaceOffsetInfo;
            registeredInterfaceOffsets(typeId, &interfaceOffsetInfo);
            size_t shell_size = getShellSize(typeId);
            PtrDeleterFunction _deleter = deleter(typeId);
            PtrOwnerFunction owner_function = registeredOwnerFunction(typeId);
            const QVector<const FunctionInfo>* _virtualFunctions = virtualFunctions(typeId);
            Destructor destructor = registeredDestructor(typeId);
            int modifiers = Java::Runtime::Class::getModifiers(env,java_class);
            TypeInfoSupplier typeInfoSupplier = registeredTypeInfoSupplier(typeId);
            QList<const PolymorphicIdHandler*> polymorphicIdHandlers = getPolymorphicIdHandlers(typeId);
            if(Java::Runtime::Modifier::isAbstract(env, modifiers)){
                jclass java_wrapper_class = nullptr;
                try{
                    java_wrapper_class = JavaAPI::resolveClass(env, qPrintable(QString("%1$ConcreteWrapper").arg(java_name)));
                } catch (const JavaException&) {
                }
                jmethodID creator_method = nullptr;
                if(java_wrapper_class){
                    creator_method = findInternalPrivateConstructor(env, java_wrapper_class);
                    if(!creator_method){
                        JavaException::raiseError(env, "constructor cannot be found" QTJAMBI_STACKTRACEINFO );
                        return QtJambiTypeEntryPtr();
                    }
                }
                if(interfaceOffsetInfo.offsets.isEmpty()){
                    result = new ObjectAbstractTypeEntry(env, typeId, qt_name, java_name, java_class, creator_method, value_size, super_type,
                                                     shell_size, _deleter, owner_function, _virtualFunctions,
                                                     destructor, polymorphicIdHandlers, typeInfoSupplier, java_wrapper_class
                                                 );
                }else{
                    result = new ObjectAbstractIFTypeEntry(env, typeId, qt_name, java_name, java_class, creator_method, value_size, super_type,
                                                     shell_size, _deleter, owner_function, _virtualFunctions,
                                                     destructor, polymorphicIdHandlers, typeInfoSupplier, java_wrapper_class,
                                                     interfaceOffsetInfo.offsets, interfaceOffsetInfo.interfaces, interfaceOffsetInfo.inheritedInterfaces
                                                 );
                }
            }else{
                jmethodID creator_method = findInternalPrivateConstructor(env, java_class);
                if(!creator_method){
                    JavaException::raiseError(env, "constructor cannot be found" QTJAMBI_STACKTRACEINFO );
                    return QtJambiTypeEntryPtr();
                }
                if(interfaceOffsetInfo.offsets.isEmpty()){
                    if(typeid_equals(typeId, typeid(QMessageLogContext))){
                        result = new QMessageLogContextTypeEntry(env, typeId, qt_name, java_name, java_class, creator_method, value_size, super_type,
                                                                 shell_size, _deleter, owner_function, _virtualFunctions,
                                                                 destructor, polymorphicIdHandlers, typeInfoSupplier
                                                                 );
                    }else{
                        result = new ObjectTypeEntry(env, typeId, qt_name, java_name, java_class, creator_method, value_size, super_type,
                                                     shell_size, _deleter, owner_function, _virtualFunctions,
                                                     destructor, polymorphicIdHandlers, typeInfoSupplier
                                                 );
                    }
                }else{
                    result = new ObjectIFTypeEntry(env, typeId, qt_name, java_name, java_class, creator_method, value_size, super_type,
                                                   shell_size, _deleter, owner_function, _virtualFunctions,
                                                   destructor, polymorphicIdHandlers, typeInfoSupplier,
                                                   interfaceOffsetInfo.offsets, interfaceOffsetInfo.interfaces, interfaceOffsetInfo.inheritedInterfaces
                                                );
                }
            }
            break;
        }
        case EntryTypes::ValueTypeInfo:
        {
            const std::type_info* super_type = nullptr; // TODO
            size_t value_size = getValueSize(typeId);
            InterfaceOffsetInfo interfaceOffsetInfo;
            registeredInterfaceOffsets(typeId, &interfaceOffsetInfo);
            size_t shell_size = getShellSize(typeId);
            NewContainerAccessFunction containerAccessFactory = getSequentialContainerAccessFactory(typeId);
            PtrOwnerFunction owner_function = registeredOwnerFunction(typeId);
            const QVector<const FunctionInfo>* _virtualFunctions = virtualFunctions(typeId);
            PtrDeleterFunction _deleter = deleter(typeId);
            Destructor destructor = registeredDestructor(typeId);
            QMetaType qt_meta_type(registeredMetaTypeID(typeId));
            QList<const PolymorphicIdHandler*> polymorphicIdHandlers = getPolymorphicIdHandlers(typeId);
            TypeInfoSupplier typeInfoSupplier = registeredTypeInfoSupplier(typeId);

            int modifiers = Java::Runtime::Class::getModifiers(env,java_class);
            if(Java::Runtime::Modifier::isAbstract(env, modifiers)){
                jclass java_wrapper_class = nullptr;
                try{
                    java_wrapper_class = JavaAPI::resolveClass(env, qPrintable(QString("%1$ConcreteWrapper").arg(java_name)));
                } catch (const JavaException&) {
                }
                jmethodID creator_method = nullptr;
                if(java_wrapper_class){
                    creator_method = findInternalPrivateConstructor(env, java_wrapper_class);
                    if(!creator_method){
                        JavaException::raiseError(env, "constructor cannot be found" QTJAMBI_STACKTRACEINFO );
                        return QtJambiTypeEntryPtr();
                    }
                }
                if(containerAccessFactory){
                    Q_ASSERT(qt_meta_type.isValid());
                    if(interfaceOffsetInfo.offsets.isEmpty()){
                        result = new ObjectAbstractContainerTypeEntry(env, typeId, qt_name, java_name, java_class, creator_method, value_size, super_type,
                                                             shell_size, containerAccessFactory, _deleter, owner_function, _virtualFunctions,
                                                             destructor, polymorphicIdHandlers, typeInfoSupplier, qt_meta_type, java_wrapper_class
                                                         );
                    }else{
                        result = new ObjectAbstractContainerIFTypeEntry(env, typeId, qt_name, java_name, java_class, creator_method, value_size, super_type,
                                                             shell_size, containerAccessFactory, _deleter, owner_function, _virtualFunctions,
                                                             destructor, polymorphicIdHandlers, typeInfoSupplier, qt_meta_type, java_wrapper_class,
                                                             interfaceOffsetInfo.offsets, interfaceOffsetInfo.interfaces, interfaceOffsetInfo.inheritedInterfaces
                                                         );
                    }
                }else{
                    if(qt_meta_type.isValid()){
                        if(interfaceOffsetInfo.offsets.isEmpty()){
                            result = new ObjectAbstractValueTypeEntry(env, typeId, qt_name, java_name, java_class, creator_method, value_size, super_type,
                                                                 shell_size, _deleter, owner_function, _virtualFunctions,
                                                                 destructor, polymorphicIdHandlers, typeInfoSupplier, qt_meta_type, java_wrapper_class
                                                             );
                        }else{
                            result = new ObjectAbstractValueIFTypeEntry(env, typeId, qt_name, java_name, java_class, creator_method, value_size, super_type,
                                                                 shell_size, _deleter, owner_function, _virtualFunctions,
                                                                 destructor, polymorphicIdHandlers, typeInfoSupplier, qt_meta_type, java_wrapper_class,
                                                                 interfaceOffsetInfo.offsets, interfaceOffsetInfo.interfaces, interfaceOffsetInfo.inheritedInterfaces
                                                             );
                        }
                    }else{
                        PtrDeleterFunction _deleter = deleter(typeId);
                        Q_ASSERT(_deleter);
                        if(interfaceOffsetInfo.offsets.isEmpty()){
                            result = new ObjectAbstractTypeEntry(env, typeId, qt_name, java_name, java_class, creator_method, value_size, super_type,
                                                                 shell_size, _deleter, owner_function, _virtualFunctions,
                                                                 destructor, polymorphicIdHandlers, typeInfoSupplier, java_wrapper_class
                                                             );
                        }else{
                            result = new ObjectAbstractIFTypeEntry(env, typeId, qt_name, java_name, java_class, creator_method, value_size, super_type,
                                                                 shell_size, _deleter, owner_function, _virtualFunctions,
                                                                 destructor, polymorphicIdHandlers, typeInfoSupplier, java_wrapper_class,
                                                                 interfaceOffsetInfo.offsets, interfaceOffsetInfo.interfaces, interfaceOffsetInfo.inheritedInterfaces
                                                             );
                        }
                    }
                }
            }else{
                jmethodID creator_method = findInternalPrivateConstructor(env, java_class);
                if(!creator_method){
                    JavaException::raiseError(env, "constructor cannot be found" QTJAMBI_STACKTRACEINFO );
                    return QtJambiTypeEntryPtr();
                }
                if(containerAccessFactory){
                    Q_ASSERT(qt_meta_type.isValid());
                    if(interfaceOffsetInfo.offsets.isEmpty()){
                        result = new ObjectContainerTypeEntry(env, typeId, qt_name, java_name, java_class, creator_method, value_size, super_type,
                                                             shell_size, containerAccessFactory, _deleter, owner_function, _virtualFunctions,
                                                             destructor, polymorphicIdHandlers, typeInfoSupplier, qt_meta_type
                                                         );
                    }else{
                        result = new ObjectContainerIFTypeEntry(env, typeId, qt_name, java_name, java_class, creator_method, value_size, super_type,
                                                                shell_size, containerAccessFactory, _deleter, owner_function, _virtualFunctions,
                                                                destructor, polymorphicIdHandlers, typeInfoSupplier, qt_meta_type,
                                                                interfaceOffsetInfo.offsets, interfaceOffsetInfo.interfaces, interfaceOffsetInfo.inheritedInterfaces
                                                         );
                    }
                }else{
                    if(qt_meta_type.isValid()){
                        if(typeid_equals(typeId, typeid(QModelIndex))){
                            result = new QModelIndexTypeEntry(env, typeId, qt_name, java_name, java_class, creator_method, value_size, super_type,
                                                              shell_size, _deleter, owner_function, _virtualFunctions,
                                                              destructor, polymorphicIdHandlers, typeInfoSupplier, qt_meta_type
                                                             );
                        }else{
                            if(interfaceOffsetInfo.offsets.isEmpty()){
                                result = new ObjectValueTypeEntry(env, typeId, qt_name, java_name, java_class, creator_method, value_size, super_type,
                                                                  shell_size, _deleter, owner_function, _virtualFunctions,
                                                                  destructor, polymorphicIdHandlers, typeInfoSupplier, qt_meta_type
                                                                 );
                            }else{
                                result = new ObjectValueIFTypeEntry(env, typeId, qt_name, java_name, java_class, creator_method, value_size, super_type,
                                                                    shell_size, _deleter, owner_function, _virtualFunctions,
                                                                    destructor, polymorphicIdHandlers, typeInfoSupplier, qt_meta_type,
                                                                    interfaceOffsetInfo.offsets, interfaceOffsetInfo.interfaces, interfaceOffsetInfo.inheritedInterfaces
                                                                 );
                            }
                        }
                    }else{
                        PtrDeleterFunction _deleter = deleter(typeId);
                        Q_ASSERT(_deleter);
                        if(interfaceOffsetInfo.offsets.isEmpty()){
                            result = new ObjectTypeEntry(env, typeId, qt_name, java_name, java_class, creator_method, value_size, super_type,
                                                                 shell_size, _deleter, owner_function, _virtualFunctions,
                                                                 destructor, polymorphicIdHandlers, typeInfoSupplier
                                                             );
                        }else{
                            result = new ObjectIFTypeEntry(env, typeId, qt_name, java_name, java_class, creator_method, value_size, super_type,
                                                                 shell_size, _deleter, owner_function, _virtualFunctions,
                                                                 destructor, polymorphicIdHandlers, typeInfoSupplier,
                                                                 interfaceOffsetInfo.offsets, interfaceOffsetInfo.interfaces, interfaceOffsetInfo.inheritedInterfaces
                                                             );
                        }
                    }
                }
            }
            break;
        }
        case EntryTypes::InterfaceTypeInfo:
        {
            const std::type_info* super_type = nullptr; // TODO
            size_t value_size = getValueSize(typeId);
            jclass java_impl_class = JavaAPI::resolveClass(env, qPrintable(QString("%1$Impl").arg(java_name)));
            if(!java_impl_class){
                JavaException::raiseError(env, "class cannot be found" QTJAMBI_STACKTRACEINFO );
                return QtJambiTypeEntryPtr();
            }
            jclass java_wrapper_class = nullptr;
            int modifiers = Java::Runtime::Class::getModifiers(env,java_impl_class);
            if(Java::Runtime::Modifier::isAbstract(env, modifiers)){
                java_wrapper_class = JavaAPI::resolveClass(env, qPrintable(QString("%1$Impl$ConcreteWrapper").arg(java_name)));
                if(!java_wrapper_class){
                    JavaException::raiseError(env, "class cannot be found" QTJAMBI_STACKTRACEINFO );
                    return QtJambiTypeEntryPtr();
                }
            }else{
                java_wrapper_class = java_impl_class;
            }
            jmethodID creator_method = findInternalPrivateConstructor(env, java_wrapper_class);
            if(!creator_method){
                JavaException::raiseError(env, "constructor cannot be found" QTJAMBI_STACKTRACEINFO );
                return QtJambiTypeEntryPtr();
            }
            InterfaceOffsetInfo interfaceOffsetInfo;
            registeredInterfaceOffsets(typeId, &interfaceOffsetInfo);
            size_t shell_size = getShellSize(typeId);
            PtrDeleterFunction _deleter = deleter(typeId);
            PtrOwnerFunction owner_function = registeredOwnerFunction(typeId);
            const QVector<const FunctionInfo>* _virtualFunctions = virtualFunctions(typeId);
            Destructor destructor = registeredDestructor(typeId);
            TypeInfoSupplier typeInfoSupplier = registeredTypeInfoSupplier(typeId);
            QList<const PolymorphicIdHandler*> polymorphicIdHandlers;
            for(const PolymorphicIdHandler* handler : getPolymorphicIdHandlers(typeId)){
                jclass _java_class = JavaAPI::resolveClass(env, getJavaName(handler->m_targetTypeId));
                if(env->IsAssignableFrom(_java_class, java_class)){
                    polymorphicIdHandlers << handler;
                }
            }
            if(interfaceOffsetInfo.offsets.isEmpty()){
                result = new InterfaceTypeEntry(env, typeId,
                                            qt_name, java_name, java_class, creator_method,
                                            value_size, java_impl_class, java_wrapper_class,
                                            super_type, shell_size,
                                            _deleter, owner_function, _virtualFunctions, destructor, polymorphicIdHandlers, typeInfoSupplier );
            }else{
                result = new InterfaceIFTypeEntry(env, typeId,
                                            qt_name, java_name, java_class, creator_method,
                                            value_size, java_impl_class, java_wrapper_class,
                                            super_type, shell_size,
                                            _deleter, owner_function, _virtualFunctions, destructor, polymorphicIdHandlers, typeInfoSupplier,
                                            interfaceOffsetInfo.offsets, interfaceOffsetInfo.interfaces, interfaceOffsetInfo.inheritedInterfaces );
            }
            break;
        }
        case EntryTypes::InterfaceValueTypeInfo:
        {
            const std::type_info* super_type = nullptr; // TODO
            size_t value_size = getValueSize(typeId);
            jclass java_impl_class = JavaAPI::resolveClass(env, qPrintable(QString("%1$Impl").arg(java_name)));
            if(!java_impl_class){
                JavaException::raiseError(env, "class cannot be found" QTJAMBI_STACKTRACEINFO );
                return QtJambiTypeEntryPtr();
            }
            jclass java_wrapper_class = nullptr;
            int modifiers = Java::Runtime::Class::getModifiers(env,java_impl_class);
            if(Java::Runtime::Modifier::isAbstract(env, modifiers)){
                java_wrapper_class = JavaAPI::resolveClass(env, qPrintable(QString("%1$Impl$ConcreteWrapper").arg(java_name)));
                if(!java_wrapper_class){
                    JavaException::raiseError(env, "class cannot be found" QTJAMBI_STACKTRACEINFO );
                    return QtJambiTypeEntryPtr();
                }
            }else{
                java_wrapper_class = java_impl_class;
            }
            jmethodID creator_method = findInternalPrivateConstructor(env, java_wrapper_class);
            if(!creator_method){
                JavaException::raiseError(env, "constructor cannot be found" QTJAMBI_STACKTRACEINFO );
                return QtJambiTypeEntryPtr();
            }
            InterfaceOffsetInfo interfaceOffsetInfo;
            registeredInterfaceOffsets(typeId, &interfaceOffsetInfo);
            size_t shell_size = getShellSize(typeId);
            PtrDeleterFunction _deleter = deleter(typeId);
            PtrOwnerFunction owner_function = registeredOwnerFunction(typeId);
            const QVector<const FunctionInfo>* _virtualFunctions = virtualFunctions(typeId);
            Destructor destructor = registeredDestructor(typeId);
            TypeInfoSupplier typeInfoSupplier = registeredTypeInfoSupplier(typeId);
            QMetaType qt_meta_type(registeredMetaTypeID(typeId));
            Q_ASSERT(qt_meta_type.isValid());
            QList<const PolymorphicIdHandler*> polymorphicIdHandlers;
            for(const PolymorphicIdHandler* handler : getPolymorphicIdHandlers(typeId)){
                jclass _java_class = JavaAPI::resolveClass(env, getJavaName(handler->m_targetTypeId));
                if(env->IsAssignableFrom(_java_class, java_class)){
                    polymorphicIdHandlers << handler;
                }
            }

            if(interfaceOffsetInfo.offsets.isEmpty()){
                result = new InterfaceValueTypeEntry(env, typeId,
                                            qt_name, java_name, java_class, creator_method,
                                            value_size, java_impl_class, java_wrapper_class,
                                            super_type, shell_size, _deleter, owner_function, _virtualFunctions,
                                            destructor, polymorphicIdHandlers, typeInfoSupplier, qt_meta_type );
            }else{
                result = new InterfaceValueIFTypeEntry(env, typeId,
                                            qt_name, java_name, java_class, creator_method,
                                            value_size, java_impl_class, java_wrapper_class,
                                            super_type, shell_size, _deleter, owner_function, _virtualFunctions,
                                            destructor, polymorphicIdHandlers, typeInfoSupplier, qt_meta_type,
                                            interfaceOffsetInfo.offsets, interfaceOffsetInfo.interfaces, interfaceOffsetInfo.inheritedInterfaces );
            }
            break;
        }
        case EntryTypes::QObjectTypeInfo:
        {
            const std::type_info* super_type = nullptr; // TODO
            size_t value_size = getValueSize(typeId);
            size_t shell_size = getShellSize(typeId);
            InterfaceOffsetInfo interfaceOffsetInfo;
            registeredInterfaceOffsets(typeId, &interfaceOffsetInfo);
            const QVector<const FunctionInfo>* _virtualFunctions = virtualFunctions(typeId);
            const QMetaObject* original_meta_object = registeredOriginalMetaObject(typeId);
            int modifiers = Java::Runtime::Class::getModifiers(env,java_class);
            QList<const PolymorphicIdHandler*> polymorphicIdHandlers;
            for(const PolymorphicIdHandler* handler : getPolymorphicIdHandlers(typeId)){
                jclass _java_class = JavaAPI::resolveClass(env, getJavaName(handler->m_targetTypeId));
                if(env->IsAssignableFrom(_java_class, java_class)){
                    polymorphicIdHandlers << handler;
                }
            }
            if(Java::Runtime::Modifier::isAbstract(env, modifiers)){
                jclass java_wrapper_class = nullptr;
                try {
                    java_wrapper_class = JavaAPI::resolveClass(env, qPrintable(QString("%1$ConcreteWrapper").arg(java_name)));
                } catch (const JavaException&) {
                }
                jmethodID creator_method = nullptr;
                if(java_wrapper_class){
                    creator_method = findInternalPrivateConstructor(env, java_wrapper_class);
                    if(!creator_method){
                        JavaException::raiseError(env, "constructor cannot be found" QTJAMBI_STACKTRACEINFO );
                        return QtJambiTypeEntryPtr();
                    }
                }
                if(polymorphicIdHandlers.isEmpty()){
                    if(interfaceOffsetInfo.offsets.isEmpty()){
                        result = new QObjectAbstractTypeEntry(env, typeId, qt_name, java_name, java_class, creator_method,
                                                      value_size, super_type, shell_size,
                                                      _virtualFunctions, original_meta_object, java_wrapper_class
                                                 );
                    }else{
                        result = new QObjectAbstractIFTypeEntry(env, typeId, qt_name, java_name, java_class, creator_method,
                                                      value_size, super_type, shell_size,
                                                      _virtualFunctions, original_meta_object, java_wrapper_class,
                                                                interfaceOffsetInfo.offsets, interfaceOffsetInfo.interfaces, interfaceOffsetInfo.inheritedInterfaces
                                                 );
                    }
                }else{
                    if(interfaceOffsetInfo.offsets.isEmpty()){
                        result = new QObjectAbstractPolymorphicTypeEntry(env, typeId, qt_name, java_name, java_class, creator_method,
                                                      value_size, super_type, shell_size,
                                                      _virtualFunctions, original_meta_object, polymorphicIdHandlers, java_wrapper_class
                                                 );
                    }else{
                        result = new QObjectAbstractPolymorphicIFTypeEntry(env, typeId, qt_name, java_name, java_class, creator_method,
                                                                          value_size, super_type, shell_size,
                                                                          _virtualFunctions, original_meta_object, polymorphicIdHandlers, java_wrapper_class,
                                                                           interfaceOffsetInfo.offsets, interfaceOffsetInfo.interfaces, interfaceOffsetInfo.inheritedInterfaces
                                                 );
                    }
                }
            }else{
                jmethodID creator_method = findInternalPrivateConstructor(env, java_class);
                if(!creator_method){
                    JavaException::raiseError(env, "constructor cannot be found" QTJAMBI_STACKTRACEINFO );
                    return QtJambiTypeEntryPtr();
                }
                if(Java::QtCore::QThread::isAssignableFrom(env, java_class))
                    result = new QThreadTypeEntry(env, typeId, qt_name, java_name, java_class, creator_method,
                                                  value_size, super_type,
                                                  shell_size, _virtualFunctions, original_meta_object
                                             );
                else if(polymorphicIdHandlers.isEmpty()){
                    if(interfaceOffsetInfo.offsets.isEmpty()){
                        result = new QObjectTypeEntry(env, typeId, qt_name, java_name, java_class, creator_method,
                                                      value_size, super_type,
                                                      shell_size, _virtualFunctions, original_meta_object
                                                 );
                    }else{
                        result = new QObjectIFTypeEntry(env, typeId, qt_name, java_name, java_class, creator_method,
                                                      value_size, super_type,
                                                      shell_size, _virtualFunctions, original_meta_object,
                                                        interfaceOffsetInfo.offsets, interfaceOffsetInfo.interfaces, interfaceOffsetInfo.inheritedInterfaces
                                                 );
                    }
                }else{
                    if(interfaceOffsetInfo.offsets.isEmpty()){
                        result = new QObjectPolymorphicTypeEntry(env, typeId, qt_name, java_name, java_class, creator_method,
                                                  value_size, super_type,
                                                  shell_size, _virtualFunctions, original_meta_object, polymorphicIdHandlers
                                             );
                    }else{
                        result = new QObjectPolymorphicIFTypeEntry(env, typeId, qt_name, java_name, java_class, creator_method,
                                                                  value_size, super_type,
                                                                  shell_size, _virtualFunctions, original_meta_object, polymorphicIdHandlers,
                                                                   interfaceOffsetInfo.offsets, interfaceOffsetInfo.interfaces, interfaceOffsetInfo.inheritedInterfaces
                                             );
                    }
                }
            }
            break;
        }
        case EntryTypes::StringTypeInfo:
        {
            size_t value_size = getValueSize(typeId);
            if(typeid_equals(typeId, typeid(QString))){
                result = new StringTypeEntry(env, typeId, qt_name, java_name, java_class, value_size);
            }else{
                result = new StringUtilTypeEntry(env, typeId, qt_name, java_name, java_class, value_size);
            }
            break;
        }
        case EntryTypes::PrimitiveTypeInfo:
        {
            size_t value_size = getValueSize(typeId);
            if(typeid_equals(typeId, typeid(double)) || typeid_equals(typeId, typeid(jdouble))){
                result = new JDoubleTypeEntry(env, typeId, qt_name, java_name, java_class, value_size);
            }else if(typeid_equals(typeId, typeid(float)) || typeid_equals(typeId, typeid(jfloat))){
                result = new JFloatTypeEntry(env, typeId, qt_name, java_name, java_class, value_size);
            }else if(typeid_equals(typeId, typeid(bool)) || typeid_equals(typeId, typeid(jboolean))){
                result = new JBooleanTypeEntry(env, typeId, qt_name, java_name, java_class, value_size);
            }else if(typeid_equals(typeId, typeid(wchar_t)) || typeid_equals(typeId, typeid(QChar)) || typeid_equals(typeId, typeid(QLatin1Char)) || typeid_equals(typeId, typeid(jchar))){
                result = new JCharTypeEntry(env, typeId, qt_name, java_name, java_class, value_size);
            }else if(typeid_equals(typeId, typeid(qint32)) || typeid_equals(typeId, typeid(quint32)) || typeid_equals(typeId, typeid(jint))){
                result = new JIntTypeEntry(env, typeId, qt_name, java_name, java_class, value_size);
            }else if(typeid_equals(typeId, typeid(qint16)) || typeid_equals(typeId, typeid(quint16)) || typeid_equals(typeId, typeid(jshort))){
                result = new JShortTypeEntry(env, typeId, qt_name, java_name, java_class, value_size);
            }else if(typeid_equals(typeId, typeid(qint64)) || typeid_equals(typeId, typeid(quint64)) || typeid_equals(typeId, typeid(jlong))){
                result = new JLongTypeEntry(env, typeId, qt_name, java_name, java_class, value_size);
            }else if(typeid_equals(typeId, typeid(qint8)) || typeid_equals(typeId, typeid(quint8)) || typeid_equals(typeId, typeid(jbyte))){
                result = new JByteTypeEntry(env, typeId, qt_name, java_name, java_class, value_size);
            }
            break;
        }
        case EntryTypes::SpecialTypeInfo:
        {
            size_t value_size = getValueSize(typeId);
            if(typeid_equals(typeId, typeid(std::nullptr_t))){
                result = new NullptrTypeEntry(env, typeId, qt_name, nullptr, nullptr, value_size);
            }else if(typeid_equals(typeId, typeid(QUrl::FormattingOptions))){
                jthrowable exceptionOccurred = nullptr;
                jmethodID creator_method = JavaAPI::resolveMethod(env, "<init>", "(I)V", java_class, false, &exceptionOccurred);
                if(exceptionOccurred)
                    JavaException(env, exceptionOccurred).raise();
                Q_ASSERT(creator_method);
                result = new FlagsTypeEntry(env, typeId, qt_name, java_name, java_class, creator_method, value_size, nullptr);
            }else if(typeid_equals(typeId, typeid(QMetaObject::Connection))
                     || typeid_equals(typeId, typeid(QMetaObject))
                     || typeid_equals(typeId, typeid(JIteratorWrapper))
                     || typeid_equals(typeId, typeid(JCollectionWrapper))
                     || typeid_equals(typeId, typeid(JMapWrapper))
                     || typeid_equals(typeId, typeid(JObjectWrapper))
                     || typeid_equals(typeId, typeid(JEnumWrapper))){
                result = new MetaUtilTypeEntry(env, typeId, qt_name, java_name, java_class, value_size);
            }else if(typeid_equals(typeId, typeid(QVariant))){
                result = new QVariantTypeEntry(env, typeId, qt_name, java_name, java_class, value_size);
#if QT_VERSION >= 0x050C00
            }else if(typeid_equals(typeId, typeid(QCborValueRef))){
                result = new QCborValueRefTypeEntry(env, typeId, qt_name, java_name, java_class, value_size);
#endif
            }
            break;
        }
        default:
            return QtJambiTypeEntryPtr();
        }
    }

    {
        QWriteLocker locker(gTypeEntryLock());
        Q_UNUSED(locker)

        if(gTypeEntryHash()->contains(unique_id(typeId))){
            QMap<QByteArray,QtJambiTypeEntryPtr>& entries = (*gTypeEntryHash())[unique_id(typeId)];
            if(qtName){
                if(QtJambiTypeEntryPtr altresult = entries.value(qtName)){
                    return altresult;
                }
            }else if(!entries.isEmpty()){
                QtJambiTypeEntryPtr e = entries.first();
                return e;
            }
        }
        (*gTypeEntryHash())[unique_id(typeId)][qt_name] = result;
    }
    return result;
}

void clearTypeHandlersAtShutdown(JNIEnv *){
    TypeEntryHash typeEntryHash;
    {
        QWriteLocker locker(gTypeEntryLock());
        Q_UNUSED(locker)
        if(!gTypeEntryHash.isDestroyed())
            typeEntryHash.swap(*gTypeEntryHash);
    }
}

QtJambiTypeEntryPtr QtJambiTypeEntry::getTypeEntry(JNIEnv* env, const std::type_info& typeId)
{
    return get_type_entry(env, typeId, true, nullptr);
}

QtJambiTypeEntryPtr QtJambiTypeEntry::getTypeEntry(JNIEnv* env, const std::type_info& typeId, const char* qtName)
{
    return get_type_entry(env, typeId, true, qtName);
}

QtJambiTypeEntryPtr QtJambiTypeEntry::getTypeEntryByJavaName(JNIEnv* env, const char* java_name)
{
    const std::type_info* typeId = getTypeByJavaName(java_name);
    if(typeId)
        return getTypeEntry(env, *typeId);
    else return QtJambiTypeEntryPtr();
}

QtJambiTypeEntryPtr QtJambiTypeEntry::getTypeEntryByJavaName(JNIEnv* env, const QString& java_name)
{
    const std::type_info* typeId = getTypeByJavaName(java_name);
    if(typeId)
        return getTypeEntry(env, *typeId);
    else return QtJambiTypeEntryPtr();
}

QtJambiTypeEntryPtr QtJambiTypeEntry::getTypeEntryByQtName(JNIEnv* env, const char* qt_name)
{
    const std::type_info* typeId = getTypeByQtName(qt_name);
    if(typeId)
        return getTypeEntry(env, *typeId, qt_name);
    else  return QtJambiTypeEntryPtr();
}

QtJambiTypeEntryPtr QtJambiTypeEntry::getTypeEntryByIID(JNIEnv* env, const char* iid)
{
    const std::type_info* typeId = getInterfaceTypeForIID(iid);
    if(typeId)
        return getTypeEntry(env, *typeId);
    else  return QtJambiTypeEntryPtr();
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

QtJambiTypeEntry::NativeToJavaResult::NativeToJavaResult(bool success) : m_success(success) {}
QtJambiTypeEntry::NativeToJavaResult::NativeToJavaResult(const QSharedPointer<QtJambiLink>& link) : m_link(link), m_success(true) {}
QtJambiTypeEntry::NativeToJavaResult::operator bool() const{return m_success;}
bool QtJambiTypeEntry::NativeToJavaResult::operator!() const{return !m_success;}
const QSharedPointer<QtJambiLink>& QtJambiTypeEntry::NativeToJavaResult::link() const{return m_link;}

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

QtJambiTypeEntryPtr QtJambiTypeEntry::getFittingTypeEntry(JNIEnv *, const void *, qintptr&) const{
    return QtJambiTypeEntryPtr{this};
}

QtJambiTypeEntryPtr QtJambiTypeEntry::getFittingTypeEntry(JNIEnv *env, void *ptr_shared_pointer, const SmartPointerGetterFunction& sharedPointerGetter, qintptr& offset) const{
    const void* ptr = sharedPointerGetter && ptr_shared_pointer ? sharedPointerGetter(ptr_shared_pointer) : nullptr;
    return getFittingTypeEntry(env, ptr, offset);
}

const std::type_info* ObjectTypeEntry::superType() const
{
    return m_super_type;
}

const std::type_info* QObjectTypeEntry::superType() const
{
    return m_super_type;
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

jclass QObjectAbstractIFTypeEntry::creatableClass() const
{
    return m_java_wrapper_class;
}

jclass ObjectAbstractValueTypeEntry::creatableClass() const
{
    return m_java_wrapper_class;
}

jclass ObjectAbstractValueIFTypeEntry::creatableClass() const
{
    return m_java_wrapper_class;
}

jclass QObjectAbstractPolymorphicTypeEntry::creatableClass() const
{
    return m_java_wrapper_class;
}

jclass QObjectAbstractPolymorphicIFTypeEntry::creatableClass() const
{
    return m_java_wrapper_class;
}

jclass ObjectAbstractTypeEntry::creatableClass() const
{
    return m_java_wrapper_class;
}

jclass ObjectAbstractIFTypeEntry::creatableClass() const
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

uint InterfaceIFTypeEntry::offset(const std::type_info& toType) const{
    return m_interfaceOffsets.value(unique_id(toType), 0);
}

uint ObjectIFTypeEntry::offset(const std::type_info& toType) const{
    return m_interfaceOffsets.value(unique_id(toType), 0);
}

uint QObjectIFTypeEntry::offset(const std::type_info& toType) const{
    return m_interfaceOffsets.value(unique_id(toType), 0);
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
                   size_t shell_size,
                   PtrDeleterFunction deleter,
                   PtrOwnerFunction owner_function,
                   const QVector<const FunctionInfo>* virtualFunctions,
                   Destructor destructor,
                   const QList<const PolymorphicIdHandler*>& polymorphicHandlers,
                   TypeInfoSupplier typeInfoSupplier
                )
    : QtJambiTypeEntry(env, typeId, qt_name, java_name, java_class, creator_method, value_size),
      m_java_impl_class(java_impl_class ? getGlobalClassRef(env, java_impl_class) : nullptr),
      m_java_wrapper_class(java_wrapper_class ? getGlobalClassRef(env, java_wrapper_class) : nullptr),
      m_super_type(super_type),
      m_shell_size(shell_size),
      m_deleter(deleter),
      m_owner_function(owner_function),
      m_virtualFunctions(virtualFunctions),
      m_destructor(destructor),
      m_polymorphicIdHandlers(polymorphicHandlers),
    m_typeInfoSupplier(typeInfoSupplier)
{

}

PolymorphicTypeEntryInterface::~PolymorphicTypeEntryInterface() = default;

const QList<const PolymorphicIdHandler*>& InterfaceTypeEntry::polymorphicIdHandlers() const{
    return m_polymorphicIdHandlers;
}

const QList<const PolymorphicIdHandler*>& ObjectTypeEntry::polymorphicIdHandlers() const{
    return m_polymorphicIdHandlers;
}

const QList<const PolymorphicIdHandler*>& QObjectPolymorphicTypeEntry::polymorphicIdHandlers() const{
    return m_polymorphicIdHandlers;
}

const QList<const PolymorphicIdHandler*>& QObjectPolymorphicIFTypeEntry::polymorphicIdHandlers() const{
    return m_polymorphicIdHandlers;
}

QtJambiTypeEntryPtr getFittingTypeEntry(JNIEnv *env, const QObject *qt_object, qintptr& offset, const QObjectTypeEntry* entry, QList<const PolymorphicIdHandler*> polymorphicHandlers){
    if(qt_object){
        const std::type_info& type = entry->type();
        qintptr _offset = 0;
        const std::type_info* _typeId = tryGetTypeInfo(env, QtJambiPrivate::CheckPointer<QObject>::supplyType, qt_object);
        if(!_typeId){
            QLatin1String java_type(getJavaName(type));
            Java::QtJambi::QDanglingPointerException::throwNew(env, QString("Cannot convert dangling pointer to object of type %1").arg(QString(QLatin1String(java_type)).replace(QLatin1Char('/'), QLatin1Char('.')).replace(QLatin1Char('$'), QLatin1Char('.'))) QTJAMBI_STACKTRACEINFO );
        }
        if(_typeId && typeid_not_equals(*_typeId, type)){
            if(QtJambiTypeEntryPtr typeEntry = QtJambiTypeEntry::getTypeEntry(env, *_typeId)){
                offset = typeEntry->offset(type);
                return typeEntry;
            }
        }
        if(entry->originalMetaObject()){
            const QMetaObject *mo = QtJambiMetaObject::findFirstStaticMetaObject(qt_object->metaObject());
            if(entry->originalMetaObject()!=mo){
                const std::type_info* typeId = &entry->type();
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
                if(typeid_not_equals(*typeId, entry->type())){
                    _typeId = typeId;
                }
            }
        }
        if(_typeId && typeid_not_equals(*_typeId, type)){
            if(QtJambiTypeEntryPtr typeEntry = QtJambiTypeEntry::getTypeEntry(env, *_typeId)){
                if(const PolymorphicTypeEntryInterface* pt = dynamic_cast<const PolymorphicTypeEntryInterface*>(typeEntry.data())){
                    polymorphicHandlers = QList<const PolymorphicIdHandler*>(pt->polymorphicIdHandlers()) << polymorphicHandlers;
                }
            }
        }
        while(!polymorphicHandlers.isEmpty()){
            QList<const PolymorphicIdHandler*> _polymorphicHandlers;
            polymorphicHandlers.swap(_polymorphicHandlers);
            void *_object = const_cast<QObject *>(qt_object);
            for(const PolymorphicIdHandler* handler : _polymorphicHandlers){
                Q_ASSERT(handler->m_polymorphyHandler);
                _offset = 0;
                if(handler->m_polymorphyHandler(_object, _offset)) {
                    if(QtJambiTypeEntryPtr typeEntry = QtJambiTypeEntry::getTypeEntry(env, handler->m_targetTypeId)){
                        _typeId = &handler->m_targetTypeId;
                        offset += _offset;
                        if(const PolymorphicTypeEntryInterface* pt = dynamic_cast<const PolymorphicTypeEntryInterface*>(typeEntry.data())){
                            polymorphicHandlers = QList<const PolymorphicIdHandler*>(pt->polymorphicIdHandlers()) << polymorphicHandlers;
                        }
                        break;
                    }
                }
            }
        }
        if(_typeId && typeid_not_equals(*_typeId, type)){
            if(QtJambiTypeEntryPtr typeEntry = QtJambiTypeEntry::getTypeEntry(env, *_typeId)){
                return typeEntry;
            }
        }
    }
    return QtJambiTypeEntryPtr{entry};
}

QtJambiTypeEntryPtr getFittingTypeEntry(JNIEnv *env, const void *qt_object, qintptr& offset, const QtJambiTypeEntry* entry, QList<const PolymorphicIdHandler*> polymorphicHandlers, TypeInfoSupplier typeInfoSupplier){
    if(qt_object){
        const std::type_info& type = entry->type();
        qintptr _offset = 0;
        const std::type_info* _typeId = nullptr;
        if(typeInfoSupplier){
            _typeId = tryGetTypeInfo(env, typeInfoSupplier, qt_object);
            if(!_typeId){
                QLatin1String java_type(getJavaName(type));
                Java::QtJambi::QDanglingPointerException::throwNew(env, QString("Cannot convert dangling pointer to object of type %1").arg(QString(QLatin1String(java_type)).replace(QLatin1Char('/'), QLatin1Char('.')).replace(QLatin1Char('$'), QLatin1Char('.'))) QTJAMBI_STACKTRACEINFO );
            }
        }
        if(_typeId && typeid_not_equals(*_typeId, type)){
            if(QtJambiTypeEntryPtr typeEntry = QtJambiTypeEntry::getTypeEntry(env, *_typeId)){
                offset = typeEntry->offset(type);
                return typeEntry;
            }
        }
        while(!polymorphicHandlers.isEmpty()){
            QList<const PolymorphicIdHandler*> _polymorphicHandlers;
            polymorphicHandlers.swap(_polymorphicHandlers);
            void *_object = const_cast<void *>(qt_object);
            for(const PolymorphicIdHandler* handler : _polymorphicHandlers){
                Q_ASSERT(handler->m_polymorphyHandler);
                _offset = 0;
                if(handler->m_polymorphyHandler(_object, _offset)) {
                    if(QtJambiTypeEntryPtr typeEntry = QtJambiTypeEntry::getTypeEntry(env, handler->m_targetTypeId)){
                        _typeId = &handler->m_targetTypeId;
                        offset += _offset;
                        if(const PolymorphicTypeEntryInterface* pt = dynamic_cast<const PolymorphicTypeEntryInterface*>(typeEntry.data())){
                            polymorphicHandlers = QList<const PolymorphicIdHandler*>(pt->polymorphicIdHandlers()) << polymorphicHandlers;
                        }
                        break;
                    }
                }
            }
        }
        if(_typeId && typeid_not_equals(*_typeId, type)){
            if(QtJambiTypeEntryPtr typeEntry = QtJambiTypeEntry::getTypeEntry(env, *_typeId)){
                if(typeEntry->isQObject()){
                    if(const QObjectTypeEntry* pt = dynamic_cast<const QObjectTypeEntry*>(typeEntry.data())){
                        const void* ptr = qt_object;
                        if(_offset!=0){
                            ptr = reinterpret_cast<const char*>(ptr)-_offset;
                        }
                        qintptr __offset = 0;
                        QtJambiTypeEntryPtr _typeEntry = typeEntry->getFittingTypeEntry(env, ptr, __offset);
                        if(_typeEntry!=typeEntry){
                            offset += __offset;
                            typeEntry = _typeEntry;
                        }
                    }
                }
                return typeEntry;
            }
        }
    }
    return QtJambiTypeEntryPtr{entry};
}

QtJambiTypeEntryPtr InterfaceTypeEntry::getFittingTypeEntry(JNIEnv *env, const void *qt_object, qintptr& offset) const{
    return ::getFittingTypeEntry(env, qt_object, offset, this, m_polymorphicIdHandlers, m_typeInfoSupplier);
}

InterfaceIFTypeEntry::InterfaceIFTypeEntry(JNIEnv* env, const std::type_info& typeId,
                   const char *qt_name,
                   const char *java_name,
                   jclass java_class,
                   jmethodID creator_method,
                   size_t value_size,
                   jclass java_impl_class,
                   jclass java_wrapper_class,
                   const std::type_info* super_type,
                   size_t shell_size,
                   PtrDeleterFunction deleter,
                   PtrOwnerFunction owner_function,
                   const QVector<const FunctionInfo>* virtualFunctions,
                   Destructor destructor,
                   const QList<const PolymorphicIdHandler*>& polymorphicHandlers,
                   TypeInfoSupplier typeInfoSupplier,
                   const QMap<size_t,uint>& interfaceOffsets,
                   const QSet<size_t>& interfaceTypes,
                   const QMap<size_t, QSet<const std::type_info*>>& inheritedInterfaces
                )
    : InterfaceTypeEntry(env, typeId, qt_name, java_name, java_class, creator_method, value_size, java_impl_class, java_wrapper_class,
                       super_type, shell_size, deleter, owner_function, virtualFunctions, destructor,
                       polymorphicHandlers, typeInfoSupplier),
      m_interfaceOffsets(interfaceOffsets),
      m_interfaceTypes(interfaceTypes),
      m_inheritedInterfaces(inheritedInterfaces)
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
                   size_t shell_size,
                   PtrDeleterFunction deleter,
                   PtrOwnerFunction owner_function,
                   const QVector<const FunctionInfo>* virtualFunctions,
                   Destructor destructor,
                   const QList<const PolymorphicIdHandler*>& polymorphicHandlers,
                   TypeInfoSupplier typeInfoSupplier,
                   const QMetaType& qt_meta_type
                )
    : InterfaceTypeEntry(env, typeId, qt_name, java_name, java_class, creator_method, value_size, java_impl_class, java_wrapper_class,
                         super_type, shell_size, deleter, owner_function, virtualFunctions, destructor,
                         polymorphicHandlers, typeInfoSupplier),
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
                                         const QMetaType& qt_meta_type,
                                         bool is_std_function
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
                     ),
      m_is_std_function(is_std_function)
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
                size_t shell_size,
                PtrDeleterFunction deleter,
                PtrOwnerFunction owner_function,
                const QVector<const FunctionInfo>* virtualFunctions,
                Destructor destructor,
                const QList<const PolymorphicIdHandler*>& polymorphicHandlers,
                TypeInfoSupplier typeInfoSupplier
            )
    : QtJambiTypeEntry(env, typeId, qt_name, java_name, java_class, creator_method, value_size),
      m_super_type(super_type),
      m_shell_size(shell_size),
      m_deleter(deleter),
      m_owner_function(owner_function),
      m_virtualFunctions(virtualFunctions),
      m_destructor(destructor),
      m_polymorphicIdHandlers(polymorphicHandlers),
      m_typeInfoSupplier(typeInfoSupplier)
{

}

QtJambiTypeEntryPtr ObjectTypeEntry::getFittingTypeEntry(JNIEnv *env, const void *qt_object, qintptr& offset) const{
    return ::getFittingTypeEntry(env, qt_object, offset, this, m_polymorphicIdHandlers, m_typeInfoSupplier);
}

ObjectAbstractTypeEntry::ObjectAbstractTypeEntry(JNIEnv* env,
                const std::type_info& typeId,
                const char *qt_name,
                const char *java_name,
                jclass java_class,
                jmethodID creator_method,
                size_t value_size,
                const std::type_info* super_type,
                size_t shell_size,
                PtrDeleterFunction deleter,
                PtrOwnerFunction owner_function,
                const QVector<const FunctionInfo>* virtualFunctions,
                Destructor destructor,
                const QList<const PolymorphicIdHandler*>& polymorphicHandlers,
                TypeInfoSupplier typeInfoSupplier,
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
                     shell_size,
                     deleter,
                     owner_function,
                     virtualFunctions,
                     destructor,
                     polymorphicHandlers,
                     typeInfoSupplier
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
                size_t shell_size,
                PtrDeleterFunction deleter,
                PtrOwnerFunction owner_function,
                const QVector<const FunctionInfo>* virtualFunctions,
                Destructor destructor,
                const QList<const PolymorphicIdHandler*>& polymorphicHandlers,
                TypeInfoSupplier typeInfoSupplier,
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
                     shell_size,
                     deleter,
                     owner_function,
                     virtualFunctions,
                     destructor,
                     polymorphicHandlers,
                     typeInfoSupplier
                 ),
      m_qt_meta_type(qt_meta_type
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
                    .id()
#endif
                  )
{

}

ObjectContainerIFTypeEntry::ObjectContainerIFTypeEntry(JNIEnv* env,
                                                       const std::type_info& typeId,
                                                       const char *qt_name,
                                                       const char *java_name,
                                                       jclass java_class,
                                                       jmethodID creator_method,
                                                       size_t value_size,
                                                       const std::type_info* super_type,
                                                       size_t shell_size,
                                                       NewContainerAccessFunction containerAccessFactory,
                                                       PtrDeleterFunction deleter,
                                                       PtrOwnerFunction owner_function,
                                                       const QVector<const FunctionInfo>* virtualFunctions,
                                                       Destructor destructor,
                                                       const QList<const PolymorphicIdHandler*>& polymorphicHandlers,
                                                       TypeInfoSupplier typeInfoSupplier,
                                                       const QMetaType& qt_meta_type,
                                                       const QMap<size_t,uint>& interfaceOffsets,
                                                       const QSet<size_t>& interfaceTypes,
                                                       const QMap<size_t, QSet<const std::type_info*>>& inheritedInterfaces
                                                   )
                                           : ObjectValueIFTypeEntry(env,
                                                            typeId,
                                                            qt_name,
                                                            java_name,
                                                            java_class,
                                                            creator_method,
                                                            value_size,
                                                            super_type,
                                                            shell_size,
                                                            deleter,
                                                            owner_function,
                                                            virtualFunctions,
                                                            destructor,
                                                            polymorphicHandlers,
                                                            typeInfoSupplier,
                                                            qt_meta_type,
                                                            interfaceOffsets,
                                                            interfaceTypes,
                                                            inheritedInterfaces
                                                        ),
                                             m_containerAccessFactory(containerAccessFactory)
{

}

ObjectAbstractValueIFTypeEntry::ObjectAbstractValueIFTypeEntry(JNIEnv* env,
                const std::type_info& typeId,
                const char *qt_name,
                const char *java_name,
                jclass java_class,
                jmethodID creator_method,
                size_t value_size,
                const std::type_info* super_type,
                size_t shell_size,
                PtrDeleterFunction deleter,
                PtrOwnerFunction owner_function,
                const QVector<const FunctionInfo>* virtualFunctions,
                Destructor destructor,
                const QList<const PolymorphicIdHandler*>& polymorphicHandlers,
                TypeInfoSupplier typeInfoSupplier,
                const QMetaType& qt_meta_type,
                jclass java_wrapper_class,
                const QMap<size_t,uint>& interfaceOffsets,
                const QSet<size_t>& interfaceTypes,
                const QMap<size_t, QSet<const std::type_info*>>& inheritedInterfaces
            )
    : ObjectValueIFTypeEntry(env,
                     typeId,
                     qt_name,
                     java_name,
                     java_class,
                     creator_method,
                     value_size,
                     super_type,
                     shell_size,
                     deleter,
                     owner_function,
                     virtualFunctions,
                     destructor,
                     polymorphicHandlers,
                     typeInfoSupplier,
                     qt_meta_type,
                     interfaceOffsets,
                     interfaceTypes,
                     inheritedInterfaces
                 ),
      m_java_wrapper_class(java_wrapper_class ? getGlobalClassRef(env, java_wrapper_class) : nullptr)
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
                size_t shell_size,
                PtrDeleterFunction deleter,
                PtrOwnerFunction owner_function,
                const QVector<const FunctionInfo>* virtualFunctions,
                Destructor destructor,
                const QList<const PolymorphicIdHandler*>& polymorphicHandlers,
                TypeInfoSupplier typeInfoSupplier,
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
                     shell_size,
                     deleter,
                     owner_function,
                     virtualFunctions,
                     destructor,
                     polymorphicHandlers,
                     typeInfoSupplier,
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
                 size_t shell_size,
                 const QVector<const FunctionInfo>* virtualFunctions,
                 const QMetaObject* original_meta_object
            )
    : QtJambiTypeEntry(env, typeId, qt_name, java_name, java_class, creator_method, value_size),
      m_super_type(super_type),
      m_shell_size(shell_size),
      m_virtualFunctions(virtualFunctions),
      m_original_meta_object(original_meta_object),
      m_superTypeForCustomMetaObject(superTypeForCustomMetaObject(typeId))
{
}

QObjectIFTypeEntry::QObjectIFTypeEntry(JNIEnv* env,
                 const std::type_info& typeId,
                 const char *qt_name,
                 const char *java_name,
                 jclass java_class,
                 jmethodID creator_method,
                 size_t value_size,
                 const std::type_info* super_type,
                 size_t shell_size,
                 const QVector<const FunctionInfo>* virtualFunctions,
                 const QMetaObject* original_meta_object,
                 const QMap<size_t,uint>& interfaceOffsets,
                 const QSet<size_t>& interfaceTypes,
                 const QMap<size_t, QSet<const std::type_info*>>& inheritedInterfaces
            )
    : QObjectTypeEntry(env, typeId, qt_name, java_name, java_class, creator_method, value_size,
                       super_type, shell_size, virtualFunctions, original_meta_object),
      m_interfaceOffsets(interfaceOffsets),
      m_interfaceTypes(interfaceTypes),
      m_inheritedInterfaces(inheritedInterfaces)
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
                 size_t shell_size,
                 const QVector<const FunctionInfo>* virtualFunctions,
                 const QMetaObject* original_meta_object
            )
    : QObjectTypeEntry(env, typeId, qt_name, java_name, java_class, creator_method, value_size,
                       super_type, shell_size, virtualFunctions, original_meta_object)
{
}

namespace ThreadPrivate{
    jobject fromQThread(JNIEnv * env, jobject java_qthread, QThread *thread);
}

QtJambiTypeEntry::NativeToJavaResult QThreadTypeEntry::convertToJava(JNIEnv *env, const void *qt_object, bool makeCopyOfValues, jvalue* output, jValueType valueType) const
{
    QtJambiTypeEntry::NativeToJavaResult result = QObjectTypeEntry::convertToJava(env, qt_object, makeCopyOfValues, output, valueType);
    if(result){
        QThread* thread = const_cast<QThread*>(reinterpret_cast<const QThread*>(qt_object));
        if (thread){
            QThreadUserData* data;
            {
                QReadLocker locker(QtJambiLinkUserData::lock());
                data = QTJAMBI_GET_OBJECTUSERDATA(QThreadUserData, thread);
            }
            if(!data)
                ThreadPrivate::fromQThread(env, output->l, thread);
        }
    }
    return result;
}

bool QThreadTypeEntry::convertSharedPointerToJava(JNIEnv *env, void *ptr_shared_pointer, SmartPointerDeleter sharedPointerDeleter, const SmartPointerGetterFunction& sharedPointerGetter, jvalue* output, jValueType valueType) const
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
                ThreadPrivate::fromQThread(env, output->l, thread);
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
                 size_t shell_size,
                 const QVector<const FunctionInfo>* virtualFunctions,
                 const QMetaObject* original_meta_object,
                 jclass java_wrapper_class
            )
    : QObjectTypeEntry(env, typeId, qt_name, java_name, java_class, creator_method, value_size,
                      super_type, shell_size, virtualFunctions, original_meta_object),
      m_java_wrapper_class(java_wrapper_class ? getGlobalClassRef(env, java_wrapper_class) : nullptr)
{

}

QObjectAbstractIFTypeEntry::QObjectAbstractIFTypeEntry(JNIEnv* env,
                 const std::type_info& typeId,
                 const char *qt_name,
                 const char *java_name,
                 jclass java_class,
                 jmethodID creator_method,
                 size_t value_size,
                 const std::type_info* super_type,
                 size_t shell_size,
                 const QVector<const FunctionInfo>* virtualFunctions,
                 const QMetaObject* original_meta_object,
                 jclass java_wrapper_class,
                 const QMap<size_t,uint>& interfaceOffsets,
                 const QSet<size_t>& interfaceTypes,
                 const QMap<size_t, QSet<const std::type_info*>>& inheritedInterfaces
            )
    : QObjectIFTypeEntry(env, typeId, qt_name, java_name, java_class, creator_method, value_size,
                      super_type, shell_size, virtualFunctions, original_meta_object,
                      interfaceOffsets, interfaceTypes, inheritedInterfaces),
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
                 size_t shell_size,
                 const QVector<const FunctionInfo>* virtualFunctions,
                 const QMetaObject* original_meta_object,
                 const QList<const PolymorphicIdHandler*>& polymorphicIdHandlers,
                 jclass java_wrapper_class
            )
    : QObjectPolymorphicTypeEntry(env, typeId, qt_name, java_name, java_class, creator_method, value_size,
                      super_type, shell_size, virtualFunctions, original_meta_object, polymorphicIdHandlers),
      m_java_wrapper_class(java_wrapper_class ? getGlobalClassRef(env, java_wrapper_class) : nullptr)
{

}

QObjectAbstractPolymorphicIFTypeEntry::QObjectAbstractPolymorphicIFTypeEntry(JNIEnv* env,
                 const std::type_info& typeId,
                 const char *qt_name,
                 const char *java_name,
                 jclass java_class,
                 jmethodID creator_method,
                 size_t value_size,
                 const std::type_info* super_type,
                 size_t shell_size,
                 const QVector<const FunctionInfo>* virtualFunctions,
                 const QMetaObject* original_meta_object,
                 const QList<const PolymorphicIdHandler*>& polymorphicIdHandlers,
                 jclass java_wrapper_class,
                 const QMap<size_t,uint>& interfaceOffsets,
                 const QSet<size_t>& interfaceTypes,
                 const QMap<size_t, QSet<const std::type_info*>>& inheritedInterfaces
            )
    : QObjectPolymorphicIFTypeEntry(env, typeId, qt_name, java_name, java_class, creator_method, value_size,
                      super_type, shell_size, virtualFunctions, original_meta_object, polymorphicIdHandlers,
                                    interfaceOffsets, interfaceTypes, inheritedInterfaces),
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
                 size_t shell_size,
                 const QVector<const FunctionInfo>* virtualFunctions,
                 const QMetaObject* original_meta_object,
                 const QList<const PolymorphicIdHandler*>& polymorphicIdHandlers
            )
    : QObjectTypeEntry(env, typeId, qt_name, java_name, java_class, creator_method, value_size,
                       super_type, shell_size, virtualFunctions, original_meta_object),
      m_polymorphicIdHandlers(polymorphicIdHandlers)
{

}

QObjectPolymorphicIFTypeEntry::QObjectPolymorphicIFTypeEntry(JNIEnv* env,
                 const std::type_info& typeId,
                 const char *qt_name,
                 const char *java_name,
                 jclass java_class,
                 jmethodID creator_method,
                 size_t value_size,
                 const std::type_info* super_type,
                 size_t shell_size,
                 const QVector<const FunctionInfo>* virtualFunctions,
                 const QMetaObject* original_meta_object,
                 const QList<const PolymorphicIdHandler*>& polymorphicIdHandlers,
                 const QMap<size_t,uint>& interfaceOffsets,
                 const QSet<size_t>& interfaceTypes,
                 const QMap<size_t, QSet<const std::type_info*>>& inheritedInterfaces
            )
    : QObjectIFTypeEntry(env, typeId, qt_name, java_name, java_class, creator_method, value_size,
                       super_type, shell_size, virtualFunctions, original_meta_object,
                         interfaceOffsets, interfaceTypes, inheritedInterfaces),
      m_polymorphicIdHandlers(polymorphicIdHandlers)
{

}

QtJambiTypeEntry::NativeToJavaResult InterfaceTypeEntry::convertToJava(JNIEnv *env, const void *qt_object, bool, jvalue* output, jValueType javaType) const{
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
    if(!creatableClass() || !creatorMethod())
        JavaException::raiseError(env, "Uncreatable type" QTJAMBI_STACKTRACEINFO );
    output->l = env->NewObject(creatableClass(), creatorMethod(), nullptr);
    JavaException::check(env QTJAMBI_STACKTRACEINFO );
    const QSharedPointer<QtJambiLink>& link = QtJambiLink::createLinkForNativeObject(
        env,
        output->l,
        const_cast<void*>(qt_object),
        LINK_NAME_ARG(qtName())
        false,
        false,
        m_deleter,
        m_owner_function,
        QtJambiLink::Ownership::None
        );
    return {link};
}

bool InterfaceTypeEntry::convertSharedPointerToJava(JNIEnv *env, void *ptr_shared_pointer, SmartPointerDeleter sharedPointerDeleter, const SmartPointerGetterFunction& sharedPointerGetter, jvalue* output, jValueType javaType) const{
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
        if(link && !link->isSmartPointer()){
            jobject obj = link->getJavaObjectLocalRef(env);
            if(obj && env->IsInstanceOf(obj, javaClass())){
                output->l = obj;
                bool createdByJava = link->createdByJava();
                bool is_shell = link->isShell();
                QtJambiShellImpl* shell = nullptr;
                if(is_shell){
                    const SuperTypeInfos& infos = SuperTypeInfos::fromClass(env, env->GetObjectClass(output->l));
                    Q_ASSERT(infos.size());
                    shell = *reinterpret_cast<QtJambiShellImpl**>( quintptr(qt_object) + infos.at(0).size() );
                }
                jobject nativeLink = link->nativeLink(env);
                if(nativeLink){
                    Java::QtJambi::NativeUtility$NativeLink::reset(env, nativeLink);
                }
                PtrOwnerFunction registeredThreadAffinityFunction = nullptr;
                if(!link->isQObject()){
                    PointerToObjectLink* polink = static_cast<PointerToObjectLink*>(link.data());
                    registeredThreadAffinityFunction = polink->ownerFunction();
                }
                link->invalidate(env);
                link.clear();
                link = QtJambiLink::createLinkForSmartPointerToObject(env, output->l,
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
    if(!creatableClass() || !creatorMethod())
        JavaException::raiseError(env, "Uncreatable type" QTJAMBI_STACKTRACEINFO );
    output->l = env->NewObject(creatableClass(), creatorMethod(), nullptr);
    JavaException::check(env QTJAMBI_STACKTRACEINFO );
    const QSharedPointer<QtJambiLink>& link = QtJambiLink::createLinkForSmartPointerToObject(
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

bool InterfaceTypeEntry::convertToNative(JNIEnv *env, const jvalue& java_value, jValueType javaType, void * output, QtJambiScope*) const{
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
        Java::QtJambi::QNoNativeResourcesException::throwNew(env, QStringLiteral("Incomplete object of type: %1").arg(QtJambiAPI::getObjectClassName(env, java_value.l).replace("$", ".")) QTJAMBI_STACKTRACEINFO );
    return true;
}

QtJambiTypeEntry::NativeToJavaResult InterfaceIFTypeEntry::convertToJava(JNIEnv *env, const void *qt_object, bool, jvalue* output, jValueType javaType) const{
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
    if(!creatableClass() || !creatorMethod())
        JavaException::raiseError(env, "Uncreatable type" QTJAMBI_STACKTRACEINFO );
    output->l = env->NewObject(creatableClass(), creatorMethod(), nullptr);
    JavaException::check(env QTJAMBI_STACKTRACEINFO );
    const QSharedPointer<QtJambiLink>& link = QtJambiLink::createLinkForNativeObject(
        env,
        output->l,
        const_cast<void*>(qt_object),
        LINK_NAME_ARG(qtName())
        false,
        false,
        m_deleter,
        m_owner_function,
        QtJambiLink::Ownership::None,
        m_interfaceOffsets,
        m_interfaceTypes,
        m_inheritedInterfaces
        );
    Q_UNUSED(link)
    return true;
}

QtJambiTypeEntry::NativeToJavaResult InterfaceValueTypeEntry::convertToJava(JNIEnv *env, const void *qt_object, bool makeCopyOfValues, jvalue* output, jValueType javaType) const{
    if(javaType!=jValueType::l)
        JavaException::raiseIllegalArgumentException(env, "Cannot convert object type" QTJAMBI_STACKTRACEINFO );
    if (!qt_object){
        output->l = nullptr;
        return true;
    }
    if(!makeCopyOfValues){
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
    void *copy = makeCopyOfValues ? m_qt_meta_type.create(qt_object) : const_cast<void*>(qt_object);
    if (!copy){
        output->l = nullptr;
        return true;
    }
    if(!creatableClass() || !creatorMethod())
        JavaException::raiseError(env, "Uncreatable type" QTJAMBI_STACKTRACEINFO );
    output->l = env->NewObject(creatableClass(), creatorMethod(), nullptr);
    JavaException::check(env QTJAMBI_STACKTRACEINFO );
    if(makeCopyOfValues || !m_deleter){
        if(m_owner_function){
            return QtJambiLink::createLinkForNativeObject(
                    env,
                    output->l,
                    copy,
                    m_qt_meta_type,
                    false,
                    false,
                    m_owner_function,
                    makeCopyOfValues ? QtJambiLink::Ownership::Java : QtJambiLink::Ownership::None
                );
        }else{
            return QtJambiLink::createLinkForNativeObject(
                    env,
                    output->l,
                    copy,
                    m_qt_meta_type,
                    false,
                    false,
                    makeCopyOfValues ? QtJambiLink::Ownership::Java : QtJambiLink::Ownership::None
                );
        }
    }else{
        if(m_owner_function){
            return QtJambiLink::createLinkForNativeObject(
                    env,
                    output->l,
                    copy,
                    LINK_NAME_ARG(qtName())
                    false,
                    false,
                    m_deleter,
                    m_owner_function,
                    makeCopyOfValues ? QtJambiLink::Ownership::Java : QtJambiLink::Ownership::None
                );
        }else{
            return QtJambiLink::createLinkForNativeObject(
                    env,
                    output->l,
                    copy,
                    LINK_NAME_ARG(qtName())
                    false,
                    false,
                    m_deleter,
                    makeCopyOfValues ? QtJambiLink::Ownership::Java : QtJambiLink::Ownership::None
                );
        }
    }
}

bool InterfaceValueTypeEntry::convertSharedPointerToJava(JNIEnv *env, void *ptr_shared_pointer, SmartPointerDeleter sharedPointerDeleter, const SmartPointerGetterFunction& sharedPointerGetter, jvalue* output, jValueType javaType) const{
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
        if(link && !link->isSmartPointer()){
            jobject obj = link->getJavaObjectLocalRef(env);
            if(obj && env->IsInstanceOf(obj, javaClass())){
                output->l = obj;
                bool createdByJava = link->createdByJava();
                bool is_shell = link->isShell();
                QtJambiShellImpl* shell = nullptr;
                if(is_shell){
                    const SuperTypeInfos& infos = SuperTypeInfos::fromClass(env, env->GetObjectClass(output->l));
                    Q_ASSERT(infos.size());
                    shell = *reinterpret_cast<QtJambiShellImpl**>( quintptr(qt_object) + infos.at(0).size() );
                }
                jobject nativeLink = link->nativeLink(env);
                if(nativeLink){
                    Java::QtJambi::NativeUtility$NativeLink::reset(env, nativeLink);
                }
                PtrOwnerFunction registeredThreadAffinityFunction = nullptr;
                if(!link->isQObject()){
                    PointerToObjectLink* polink = static_cast<PointerToObjectLink*>(link.data());
                    registeredThreadAffinityFunction = polink->ownerFunction();
                }
                link->invalidate(env);
                link.clear();
                link = QtJambiLink::createLinkForSmartPointerToObject(env, output->l,
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
    if(!creatableClass() || !creatorMethod())
        JavaException::raiseError(env, "Uncreatable type" QTJAMBI_STACKTRACEINFO );
    output->l = env->NewObject(creatableClass(), creatorMethod(), nullptr);
    JavaException::check(env QTJAMBI_STACKTRACEINFO );
    const QSharedPointer<QtJambiLink>& link = QtJambiLink::createLinkForSmartPointerToObject(
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

InterfaceValueIFTypeEntry::InterfaceValueIFTypeEntry(JNIEnv* env, const std::type_info& typeId,
                          const char *qt_name,
                          const char *java_name,
                          jclass java_class,
                          jmethodID creator_method,
                          size_t value_size,
                          jclass java_impl_class,
                          jclass java_wrapper_class,
                          const std::type_info* super_type,
                          size_t shell_size,
                          PtrDeleterFunction deleter,
                          PtrOwnerFunction owner_function,
                          const QVector<const FunctionInfo>* virtualFunctions,
                          Destructor destructor,
                          const QList<const PolymorphicIdHandler*>& polymorphicHandlers,
                          TypeInfoSupplier typeInfoSupplier,
                          const QMetaType& qt_meta_type,
                          const QMap<size_t,uint>& interfaceOffsets,
                          const QSet<size_t>& interfaceTypes,
                          const QMap<size_t, QSet<const std::type_info*>>& inheritedInterfaces)
    : InterfaceIFTypeEntry(env,
                           typeId,
                           qt_name,
                           java_name,
                           java_class,
                           creator_method,
                           value_size,
                           java_impl_class,
                           java_wrapper_class,
                           super_type,
                           shell_size,
                           deleter,
                           owner_function,
                           virtualFunctions,
                           destructor,
                           polymorphicHandlers,
                           typeInfoSupplier,
                           interfaceOffsets,
                           interfaceTypes,
                           inheritedInterfaces
          ),
      m_qt_meta_type(qt_meta_type
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
                            .id()
#endif
                     ){}

QtJambiTypeEntry::NativeToJavaResult InterfaceValueIFTypeEntry::convertToJava(JNIEnv *env, const void *qt_object, bool makeCopyOfValues, jvalue* output, jValueType javaType) const{
    if(javaType!=jValueType::l)
        JavaException::raiseIllegalArgumentException(env, "Cannot convert object type" QTJAMBI_STACKTRACEINFO );
    if (!qt_object){
        output->l = nullptr;
        return true;
    }
    if(!makeCopyOfValues){
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
    void *copy = makeCopyOfValues ? m_qt_meta_type.create(qt_object) : const_cast<void*>(qt_object);
    if (!copy){
        output->l = nullptr;
        return true;
    }
    if(!creatableClass() || !creatorMethod())
        JavaException::raiseError(env, "Uncreatable type" QTJAMBI_STACKTRACEINFO );
    output->l = env->NewObject(creatableClass(), creatorMethod(), nullptr);
    JavaException::check(env QTJAMBI_STACKTRACEINFO );
    if(makeCopyOfValues || !m_deleter){
        if(m_owner_function){
            return QtJambiLink::createLinkForNativeObject(
                env,
                output->l,
                copy,
                m_qt_meta_type,
                false,
                false,
                m_owner_function,
                makeCopyOfValues ? QtJambiLink::Ownership::Java : QtJambiLink::Ownership::None,
                m_interfaceOffsets,
                m_interfaceTypes,
                m_inheritedInterfaces
                );
        }else{
            return QtJambiLink::createLinkForNativeObject(
                env,
                output->l,
                copy,
                m_qt_meta_type,
                false,
                false,
                makeCopyOfValues ? QtJambiLink::Ownership::Java : QtJambiLink::Ownership::None,
                m_interfaceOffsets,
                m_interfaceTypes,
                m_inheritedInterfaces
                );
        }
    }else{
        if(m_owner_function){
            return QtJambiLink::createLinkForNativeObject(
                env,
                output->l,
                copy,
                LINK_NAME_ARG(qtName())
                false,
                false,
                m_deleter,
                m_owner_function,
                makeCopyOfValues ? QtJambiLink::Ownership::Java : QtJambiLink::Ownership::None,
                m_interfaceOffsets,
                m_interfaceTypes,
                m_inheritedInterfaces
                );
        }else{
            return QtJambiLink::createLinkForNativeObject(
                env,
                output->l,
                copy,
                LINK_NAME_ARG(qtName())
                false,
                false,
                m_deleter,
                makeCopyOfValues ? QtJambiLink::Ownership::Java : QtJambiLink::Ownership::None,
                m_interfaceOffsets,
                m_interfaceTypes,
                m_inheritedInterfaces
                );
        }
    }
}

QtJambiTypeEntry::NativeToJavaResult ObjectTypeEntry::convertToJava(JNIEnv *env, const void *qt_object, bool, jvalue* output, jValueType javaType) const{
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
    if(!creatableClass() || !creatorMethod())
        JavaException::raiseError(env, "Uncreatable type" QTJAMBI_STACKTRACEINFO );
    output->l = env->NewObject(creatableClass(), creatorMethod(), nullptr);
    JavaException::check(env QTJAMBI_STACKTRACEINFO );
    return QtJambiLink::createLinkForNativeObject(
        env,
        output->l,
        const_cast<void*>(qt_object),
        LINK_NAME_ARG(qtName())
        false,
        false,
        m_deleter,
        m_owner_function,
        QtJambiLink::Ownership::None
        );
}

bool ObjectTypeEntry::convertSharedPointerToJava(JNIEnv *env, void *ptr_shared_pointer, SmartPointerDeleter sharedPointerDeleter, const SmartPointerGetterFunction& sharedPointerGetter, jvalue* output, jValueType javaType) const{
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
        if(link && !link->isSmartPointer()){
            jobject obj = link->getJavaObjectLocalRef(env);
            if(obj && env->IsInstanceOf(obj, javaClass())){
                output->l = obj;
                bool createdByJava = link->createdByJava();
                bool is_shell = link->isShell();
                QtJambiShellImpl* shell = nullptr;
                if(is_shell){
                    const SuperTypeInfos& infos = SuperTypeInfos::fromClass(env, env->GetObjectClass(output->l));
                    Q_ASSERT(infos.size());
                    shell = *reinterpret_cast<QtJambiShellImpl**>( quintptr(qt_object) + infos.at(0).size() );
                }
                jobject nativeLink = link->nativeLink(env);
                if(nativeLink){
                    Java::QtJambi::NativeUtility$NativeLink::reset(env, nativeLink);
                }
                PtrOwnerFunction registeredThreadAffinityFunction = nullptr;
                if(!link->isQObject()){
                    PointerToObjectLink* polink = static_cast<PointerToObjectLink*>(link.data());
                    registeredThreadAffinityFunction = polink->ownerFunction();
                }
                link->invalidate(env);
                link.clear();
                link = QtJambiLink::createLinkForSmartPointerToObject(env, output->l,
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
    if(!creatableClass() || !creatorMethod())
        JavaException::raiseError(env, "Uncreatable type" QTJAMBI_STACKTRACEINFO );
    output->l = env->NewObject(creatableClass(), creatorMethod(), nullptr);
    JavaException::check(env QTJAMBI_STACKTRACEINFO );
    const QSharedPointer<QtJambiLink>& link = QtJambiLink::createLinkForSmartPointerToObject(
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

bool ObjectTypeEntry::convertToNative(JNIEnv *env, const jvalue& java_value, jValueType javaType, void * output, QtJambiScope*) const{
    if(javaType!=jValueType::l)
        JavaException::raiseIllegalArgumentException(env, "Cannot convert to object type" QTJAMBI_STACKTRACEINFO );
    if(env->IsInstanceOf(java_value.l, this->javaClass())){
        if (QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForJavaObject(env, java_value.l)){
            Q_ASSERT(!link->isQObject());
            *reinterpret_cast<void**>(output) = link->pointer();
        }
        else if(Java::QtJambi::QtObjectInterface::isInstanceOf(env, java_value.l))
            Java::QtJambi::QNoNativeResourcesException::throwNew(env, QStringLiteral("Incomplete object of type: %1").arg(QtJambiAPI::getObjectClassName(env, java_value.l).replace("$", ".")) QTJAMBI_STACKTRACEINFO );
        return true;
    }else return false;
}

ObjectIFTypeEntry::ObjectIFTypeEntry(JNIEnv* env,
                const std::type_info& typeId,
                const char *qt_name,
                const char *java_name,
                jclass java_class,
                jmethodID creator_method,
                size_t value_size,
                const std::type_info* super_type,
                size_t shell_size,
                PtrDeleterFunction deleter,
                PtrOwnerFunction owner_function,
                const QVector<const FunctionInfo>* virtualFunctions,
                Destructor destructor,
                const QList<const PolymorphicIdHandler*>& polymorphicHandlers,
                TypeInfoSupplier typeInfoSupplier,
                const QMap<size_t,uint>& interfaceOffsets,
                const QSet<size_t>& interfaceTypes,
                const QMap<size_t, QSet<const std::type_info*>>& inheritedInterfaces)
    :    ObjectTypeEntry(env,
                         typeId,
                         qt_name,
                         java_name,
                         java_class,
                         creator_method,
                         value_size,
                         super_type,
                         shell_size,
                         deleter,
                         owner_function,
                         virtualFunctions,
                         destructor,
                         polymorphicHandlers,
                         typeInfoSupplier
                     ),
      m_interfaceOffsets(interfaceOffsets),
      m_interfaceTypes(interfaceTypes),
      m_inheritedInterfaces(inheritedInterfaces){
}

ObjectAbstractIFTypeEntry::ObjectAbstractIFTypeEntry(JNIEnv* env,
                const std::type_info& typeId,
                const char *qt_name,
                const char *java_name,
                jclass java_class,
                jmethodID creator_method,
                size_t value_size,
                const std::type_info* super_type,
                size_t shell_size,
                PtrDeleterFunction deleter,
                PtrOwnerFunction owner_function,
                const QVector<const FunctionInfo>* virtualFunctions,
                Destructor destructor,
                const QList<const PolymorphicIdHandler*>& polymorphicHandlers,
                TypeInfoSupplier typeInfoSupplier,
                jclass java_wrapper_class,
                const QMap<size_t,uint>& interfaceOffsets,
                const QSet<size_t>& interfaceTypes,
                const QMap<size_t, QSet<const std::type_info*>>& inheritedInterfaces)
    :     ObjectIFTypeEntry(env,
                            typeId,
                            qt_name,
                            java_name,
                            java_class,
                            creator_method,
                            value_size,
                            super_type,
                            shell_size,
                            deleter,
                            owner_function,
                            virtualFunctions,
                            destructor,
                            polymorphicHandlers,
                            typeInfoSupplier,
                            interfaceOffsets,
                            interfaceTypes,
                            inheritedInterfaces
                        ),
      m_java_wrapper_class(java_wrapper_class ? getGlobalClassRef(env, java_wrapper_class) : nullptr)
{}

QtJambiTypeEntry::NativeToJavaResult ObjectIFTypeEntry::convertToJava(JNIEnv *env, const void *qt_object, bool, jvalue* output, jValueType javaType) const{
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
    if(!creatableClass() || !creatorMethod())
        JavaException::raiseError(env, "Uncreatable type" QTJAMBI_STACKTRACEINFO );
    output->l = env->NewObject(creatableClass(), creatorMethod(), nullptr);
    JavaException::check(env QTJAMBI_STACKTRACEINFO );
    return QtJambiLink::createLinkForNativeObject(
        env,
        output->l,
        const_cast<void*>(qt_object),
        LINK_NAME_ARG(qtName())
        false,
        false,
        m_deleter,
        m_owner_function,
        QtJambiLink::Ownership::None,
        m_interfaceOffsets,
        m_interfaceTypes,
        m_inheritedInterfaces
        );
}

QtJambiTypeEntry::NativeToJavaResult ObjectValueTypeEntry::convertToJava(JNIEnv *env, const void *qt_object, bool makeCopyOfValues, jvalue* output, jValueType javaType) const{
    if(javaType!=jValueType::l)
        JavaException::raiseIllegalArgumentException(env, "Cannot convert object type" QTJAMBI_STACKTRACEINFO );
    if (!qt_object){
        output->l = nullptr;
        return true;
    }
    if(!makeCopyOfValues){
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
    void *copy = makeCopyOfValues ? m_qt_meta_type.create(qt_object) : const_cast<void*>(qt_object);
    if (!copy){
        output->l = nullptr;
        return true;
    }
    if(!creatableClass() || !creatorMethod())
        JavaException::raiseError(env, "Uncreatable type" QTJAMBI_STACKTRACEINFO );
    output->l = env->NewObject(creatableClass(), creatorMethod(), nullptr);
    JavaException::check(env QTJAMBI_STACKTRACEINFO );
    if(makeCopyOfValues || !m_deleter){
        if(m_owner_function){
            return QtJambiLink::createLinkForNativeObject(
                env,
                output->l,
                copy,
                m_qt_meta_type,
                false,
                false,
                m_owner_function,
                makeCopyOfValues ? QtJambiLink::Ownership::Java : QtJambiLink::Ownership::None
                );
        }else{
            return QtJambiLink::createLinkForNativeObject(
                env,
                output->l,
                copy,
                m_qt_meta_type,
                false,
                false,
                makeCopyOfValues ? QtJambiLink::Ownership::Java : QtJambiLink::Ownership::None
                );
        }
    }else{
        if(m_owner_function){
            return QtJambiLink::createLinkForNativeObject(
                env,
                output->l,
                copy,
                LINK_NAME_ARG(qtName())
                false,
                false,
                m_deleter,
                m_owner_function,
                makeCopyOfValues ? QtJambiLink::Ownership::Java : QtJambiLink::Ownership::None
                );
        }else{
            return QtJambiLink::createLinkForNativeObject(
                env,
                output->l,
                copy,
                LINK_NAME_ARG(qtName())
                false,
                false,
                m_deleter,
                makeCopyOfValues ? QtJambiLink::Ownership::Java : QtJambiLink::Ownership::None
                );
        }
    }
}

bool ObjectValueTypeEntry::convertSharedPointerToJava(JNIEnv *env, void *ptr_shared_pointer, SmartPointerDeleter sharedPointerDeleter, const SmartPointerGetterFunction& sharedPointerGetter, jvalue* output, jValueType javaType) const{
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
        if(link && !link->isSmartPointer()){
            jobject obj = link->getJavaObjectLocalRef(env);
            if(obj && env->IsInstanceOf(obj, javaClass())){
                output->l = obj;
                bool createdByJava = link->createdByJava();
                bool is_shell = link->isShell();
                QtJambiShellImpl* shell = nullptr;
                if(is_shell){
                    const SuperTypeInfos& infos = SuperTypeInfos::fromClass(env, env->GetObjectClass(output->l));
                    Q_ASSERT(infos.size());
                    shell = *reinterpret_cast<QtJambiShellImpl**>( quintptr(qt_object) + infos.at(0).size() );
                }
                jobject nativeLink = link->nativeLink(env);
                if(nativeLink){
                    Java::QtJambi::NativeUtility$NativeLink::reset(env, nativeLink);
                }
                PtrOwnerFunction registeredThreadAffinityFunction = nullptr;
                if(!link->isQObject()){
                    PointerToObjectLink* polink = static_cast<PointerToObjectLink*>(link.data());
                    registeredThreadAffinityFunction = polink->ownerFunction();
                }
                link->invalidate(env);
                link.clear();
                link = QtJambiLink::createLinkForSmartPointerToObject(env, output->l,
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
    if(!creatableClass() || !creatorMethod())
        JavaException::raiseError(env, "Uncreatable type" QTJAMBI_STACKTRACEINFO );
    output->l = env->NewObject(creatableClass(), creatorMethod(), nullptr);
    JavaException::check(env QTJAMBI_STACKTRACEINFO );
    const QSharedPointer<QtJambiLink>& link = QtJambiLink::createLinkForSmartPointerToObject(
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

ObjectValueIFTypeEntry::ObjectValueIFTypeEntry(JNIEnv* env,
                                                   const std::type_info& typeId,
                                                   const char *qt_name,
                                                   const char *java_name,
                                                   jclass java_class,
                                                   jmethodID creator_method,
                                                   size_t value_size,
                                                   const std::type_info* super_type,
                                                   size_t shell_size,
                                                   PtrDeleterFunction deleter,
                                                   PtrOwnerFunction owner_function,
                                                   const QVector<const FunctionInfo>* virtualFunctions,
                                                   Destructor destructor,
                                                   const QList<const PolymorphicIdHandler*>& polymorphicHandlers,
                                                   TypeInfoSupplier typeInfoSupplier,
                                                   const QMetaType& qt_meta_type,
                                                   const QMap<size_t,uint>& interfaceOffsets,
                                                   const QSet<size_t>& interfaceTypes,
                                                   const QMap<size_t, QSet<const std::type_info*>>& inheritedInterfaces
                                                   )
    : ObjectIFTypeEntry(env,
                        typeId,
                        qt_name,
                        java_name,
                        java_class,
                        creator_method,
                        value_size,
                        super_type,
                        shell_size,
                        deleter,
                        owner_function,
                        virtualFunctions,
                        destructor,
                        polymorphicHandlers,
                        typeInfoSupplier,
                        interfaceOffsets,
                        interfaceTypes,
                        inheritedInterfaces),
        m_qt_meta_type(qt_meta_type
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
                                    .id()
#endif
                       ){}

QtJambiTypeEntry::NativeToJavaResult ObjectValueIFTypeEntry::convertToJava(JNIEnv *env, const void *qt_object, bool makeCopyOfValues, jvalue* output, jValueType javaType) const{
    if(javaType!=jValueType::l)
        JavaException::raiseIllegalArgumentException(env, "Cannot convert object type" QTJAMBI_STACKTRACEINFO );
    if (!qt_object){
        output->l = nullptr;
        return true;
    }
    if(!makeCopyOfValues){
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
    void *copy = makeCopyOfValues ? m_qt_meta_type.create(qt_object) : const_cast<void*>(qt_object);
    if (!copy){
        output->l = nullptr;
        return true;
    }
    if(!creatableClass() || !creatorMethod())
        JavaException::raiseError(env, "Uncreatable type" QTJAMBI_STACKTRACEINFO );
    output->l = env->NewObject(creatableClass(), creatorMethod(), nullptr);
    JavaException::check(env QTJAMBI_STACKTRACEINFO );
    if(makeCopyOfValues || !m_deleter){
        if(m_owner_function){
            return QtJambiLink::createLinkForNativeObject(
                env,
                output->l,
                copy,
                m_qt_meta_type,
                false,
                false,
                m_owner_function,
                makeCopyOfValues ? QtJambiLink::Ownership::Java : QtJambiLink::Ownership::None,
                m_interfaceOffsets,
                m_interfaceTypes,
                m_inheritedInterfaces
                );
        }else{
            return QtJambiLink::createLinkForNativeObject(
                env,
                output->l,
                copy,
                m_qt_meta_type,
                false,
                false,
                makeCopyOfValues ? QtJambiLink::Ownership::Java : QtJambiLink::Ownership::None,
                m_interfaceOffsets,
                m_interfaceTypes,
                m_inheritedInterfaces
                );
        }
    }else{
        if(m_owner_function){
            return QtJambiLink::createLinkForNativeObject(
                env,
                output->l,
                copy,
                LINK_NAME_ARG(qtName())
                false,
                false,
                m_deleter,
                m_owner_function,
                makeCopyOfValues ? QtJambiLink::Ownership::Java : QtJambiLink::Ownership::None,
                m_interfaceOffsets,
                m_interfaceTypes,
                m_inheritedInterfaces
                );
        }else{
            return QtJambiLink::createLinkForNativeObject(
                env,
                output->l,
                copy,
                LINK_NAME_ARG(qtName())
                false,
                false,
                m_deleter,
                makeCopyOfValues ? QtJambiLink::Ownership::Java : QtJambiLink::Ownership::None,
                m_interfaceOffsets,
                m_interfaceTypes,
                m_inheritedInterfaces
                );
        }
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
                size_t shell_size,
                NewContainerAccessFunction containerAccessFactory,
                PtrDeleterFunction deleter,
                PtrOwnerFunction owner_function,
                const QVector<const FunctionInfo>* virtualFunctions,
                Destructor destructor,
                const QList<const PolymorphicIdHandler*>& polymorphicHandlers,
                TypeInfoSupplier typeInfoSupplier,
                const QMetaType& qt_meta_type)
    : ObjectValueTypeEntry(env,
                           typeId,
                           qt_name,
                           java_name,
                           java_class,
                           creator_method,
                           value_size,
                           super_type,
                           shell_size,
                           deleter,
                           owner_function,
                           virtualFunctions,
                           destructor,
                           polymorphicHandlers,
                           typeInfoSupplier,
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
                size_t shell_size,
                NewContainerAccessFunction containerAccessFactory,
                PtrDeleterFunction deleter,
                PtrOwnerFunction owner_function,
                const QVector<const FunctionInfo>* virtualFunctions,
                Destructor destructor,
                const QList<const PolymorphicIdHandler*>& polymorphicHandlers,
                TypeInfoSupplier typeInfoSupplier,
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
                           shell_size,
                           deleter,
                           owner_function,
                           virtualFunctions,
                           destructor,
                           polymorphicHandlers,
                           typeInfoSupplier,
                           qt_meta_type,
                           java_wrapper_class),
      m_containerAccessFactory(containerAccessFactory)
{}

QtJambiTypeEntry::NativeToJavaResult ObjectContainerTypeEntry::convertToJava(JNIEnv *env, const void *qt_object, bool makeCopyOfValues, jvalue* output, jValueType javaType) const{
    if(javaType!=jValueType::l)
        JavaException::raiseIllegalArgumentException(env, "Cannot convert object type" QTJAMBI_STACKTRACEINFO );
    if (!qt_object){
        output->l = nullptr;
        return true;
    }
    if(!makeCopyOfValues){
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
    void *copy = makeCopyOfValues ? m_qt_meta_type.create(qt_object) : const_cast<void*>(qt_object);
    if (!copy){
        output->l = nullptr;
        return true;
    }
    if(!creatableClass() || !creatorMethod())
        JavaException::raiseError(env, "Uncreatable type" QTJAMBI_STACKTRACEINFO );
    output->l = env->NewObject(creatableClass(), creatorMethod(), nullptr);
    JavaException::check(env QTJAMBI_STACKTRACEINFO );
    if(makeCopyOfValues || !m_deleter){
        return QtJambiLink::createLinkForNativeObject(
            env,
            output->l,
            copy,
            m_qt_meta_type,
            false,
            false,
            m_containerAccessFactory(),
            makeCopyOfValues ? QtJambiLink::Ownership::Java : QtJambiLink::Ownership::None
            );
    }else{
        return QtJambiLink::createLinkForNativeObject(
            env,
            output->l,
            copy,
            LINK_NAME_ARG(qtName())
            false,
            false,
            m_deleter,
            m_containerAccessFactory(),
            makeCopyOfValues ? QtJambiLink::Ownership::Java : QtJambiLink::Ownership::None
            );
    }
}

bool ObjectContainerTypeEntry::convertSharedPointerToJava(JNIEnv *env, void *ptr_shared_pointer, SmartPointerDeleter sharedPointerDeleter, const SmartPointerGetterFunction& sharedPointerGetter, jvalue* output, jValueType javaType) const{
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
        if(link && !link->isSmartPointer()){
            jobject obj = link->getJavaObjectLocalRef(env);
            if(obj && env->IsInstanceOf(obj, javaClass())){
                output->l = obj;
                bool is_shell = link->isShell();
                QtJambiShellImpl* shell = nullptr;
                if(is_shell){
                    const SuperTypeInfos& infos = SuperTypeInfos::fromClass(env, env->GetObjectClass(output->l));
                    Q_ASSERT(infos.size());
                    shell = *reinterpret_cast<QtJambiShellImpl**>( quintptr(qt_object) + infos.at(0).size() );
                }
                jobject nativeLink = link->nativeLink(env);
                if(nativeLink){
                    Java::QtJambi::NativeUtility$NativeLink::reset(env, nativeLink);
                }
                link->invalidate(env);
                link.clear();
                link = QtJambiLink::createLinkForSmartPointerToContainer(env, output->l,
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
    if(!creatableClass() || !creatorMethod())
        JavaException::raiseError(env, "Uncreatable type" QTJAMBI_STACKTRACEINFO );
    output->l = env->NewObject(creatableClass(), creatorMethod(), nullptr);
    JavaException::check(env QTJAMBI_STACKTRACEINFO );
    const QSharedPointer<QtJambiLink>& link = QtJambiLink::createLinkForSmartPointerToContainer(
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

QtJambiTypeEntry::NativeToJavaResult ObjectContainerIFTypeEntry::convertToJava(JNIEnv *env, const void *qt_object, bool makeCopyOfValues, jvalue* output, jValueType javaType) const{
    if(javaType!=jValueType::l)
        JavaException::raiseIllegalArgumentException(env, "Cannot convert object type" QTJAMBI_STACKTRACEINFO );
    if (!qt_object){
        output->l = nullptr;
        return true;
    }
    if(!makeCopyOfValues){
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
    void *copy = makeCopyOfValues ? m_qt_meta_type.create(qt_object) : const_cast<void*>(qt_object);
    if (!copy){
        output->l = nullptr;
        return true;
    }
    if(!creatableClass() || !creatorMethod())
        JavaException::raiseError(env, "Uncreatable type" QTJAMBI_STACKTRACEINFO );
    output->l = env->NewObject(creatableClass(), creatorMethod(), nullptr);
    JavaException::check(env QTJAMBI_STACKTRACEINFO );
    if(makeCopyOfValues || !m_deleter){
        return QtJambiLink::createLinkForNativeObject(
            env,
            output->l,
            copy,
            m_qt_meta_type,
            false,
            false,
            m_containerAccessFactory(),
            makeCopyOfValues ? QtJambiLink::Ownership::Java : QtJambiLink::Ownership::None,
            m_interfaceOffsets,
            m_interfaceTypes,
            m_inheritedInterfaces
            );
    }else{
        return QtJambiLink::createLinkForNativeObject(
            env,
            output->l,
            copy,
            LINK_NAME_ARG(qtName())
            false,
            false,
            m_deleter,
            m_containerAccessFactory(),
            makeCopyOfValues ? QtJambiLink::Ownership::Java : QtJambiLink::Ownership::None,
            m_interfaceOffsets,
            m_interfaceTypes,
            m_inheritedInterfaces
            );
    }
}

QtJambiTypeEntry::NativeToJavaResult ObjectAbstractContainerTypeEntry::convertToJava(JNIEnv *env, const void *qt_object, bool makeCopyOfValues, jvalue* output, jValueType javaType) const{
    if(javaType!=jValueType::l)
        JavaException::raiseIllegalArgumentException(env, "Cannot convert object type" QTJAMBI_STACKTRACEINFO );
    if (!qt_object){
        output->l = nullptr;
        return true;
    }
    if(!makeCopyOfValues){
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
    void *copy = makeCopyOfValues ? m_qt_meta_type.create(qt_object) : const_cast<void*>(qt_object);
    if (!copy){
        output->l = nullptr;
        return true;
    }
    if(!creatableClass() || !creatorMethod())
        JavaException::raiseError(env, "Uncreatable type" QTJAMBI_STACKTRACEINFO );
    output->l = env->NewObject(creatableClass(), creatorMethod(), nullptr);
    JavaException::check(env QTJAMBI_STACKTRACEINFO );
    if(makeCopyOfValues || !m_deleter){
        return QtJambiLink::createLinkForNativeObject(
            env,
            output->l,
            copy,
            m_qt_meta_type,
            false,
            false,
            m_containerAccessFactory(),
            makeCopyOfValues ? QtJambiLink::Ownership::Java : QtJambiLink::Ownership::None
            );
    }else{
        return QtJambiLink::createLinkForNativeObject(
            env,
            output->l,
            copy,
            LINK_NAME_ARG(qtName())
            false,
            false,
            m_deleter,
            m_containerAccessFactory(),
            makeCopyOfValues ? QtJambiLink::Ownership::Java : QtJambiLink::Ownership::None
            );
    }
}

bool ObjectAbstractContainerTypeEntry::convertSharedPointerToJava(JNIEnv *env, void *ptr_shared_pointer, SmartPointerDeleter sharedPointerDeleter, const SmartPointerGetterFunction& sharedPointerGetter, jvalue* output, jValueType javaType) const{
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
        if(link && !link->isSmartPointer()){
            jobject obj = link->getJavaObjectLocalRef(env);
            if(obj && env->IsInstanceOf(obj, javaClass())){
                output->l = obj;
                bool is_shell = link->isShell();
                QtJambiShellImpl* shell = nullptr;
                if(is_shell){
                    const SuperTypeInfos& infos = SuperTypeInfos::fromClass(env, env->GetObjectClass(output->l));
                    Q_ASSERT(infos.size());
                    shell = *reinterpret_cast<QtJambiShellImpl**>( quintptr(qt_object) + infos.at(0).size() );
                }
                jobject nativeLink = link->nativeLink(env);
                if(nativeLink){
                    Java::QtJambi::NativeUtility$NativeLink::reset(env, nativeLink);
                }
                link->invalidate(env);
                link.clear();
                link = QtJambiLink::createLinkForSmartPointerToContainer(env, output->l,
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
    if(!creatableClass() || !creatorMethod())
        JavaException::raiseError(env, "Uncreatable type" QTJAMBI_STACKTRACEINFO );
    output->l = env->NewObject(creatableClass(), creatorMethod(), nullptr);
    JavaException::check(env QTJAMBI_STACKTRACEINFO );
    const QSharedPointer<QtJambiLink>& link = QtJambiLink::createLinkForSmartPointerToContainer(
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

ObjectAbstractContainerIFTypeEntry::ObjectAbstractContainerIFTypeEntry(JNIEnv* env,
                const std::type_info& typeId,
                const char *qt_name,
                const char *java_name,
                jclass java_class,
                jmethodID creator_method,
                size_t value_size,
                const std::type_info* super_type,
                size_t shell_size,
                NewContainerAccessFunction containerAccessFactory,
                PtrDeleterFunction deleter,
                PtrOwnerFunction owner_function,
                const QVector<const FunctionInfo>* virtualFunctions,
                Destructor destructor,
                const QList<const PolymorphicIdHandler*>& polymorphicHandlers,
                TypeInfoSupplier typeInfoSupplier,
                const QMetaType& qt_meta_type,
                jclass java_wrapper_class,
                const QMap<size_t,uint>& interfaceOffsets,
                const QSet<size_t>& interfaceTypes,
                const QMap<size_t, QSet<const std::type_info*>>& inheritedInterfaces)
    : ObjectAbstractValueIFTypeEntry(env,
                           typeId,
                           qt_name,
                           java_name,
                           java_class,
                           creator_method,
                           value_size,
                           super_type,
                           shell_size,
                           deleter,
                           owner_function,
                           virtualFunctions,
                           destructor,
                           polymorphicHandlers,
                           typeInfoSupplier,
                           qt_meta_type,
                           java_wrapper_class,
                           interfaceOffsets,
                           interfaceTypes,
                           inheritedInterfaces),
      m_containerAccessFactory(containerAccessFactory)
{}

QtJambiTypeEntry::NativeToJavaResult ObjectAbstractContainerIFTypeEntry::convertToJava(JNIEnv *env, const void *qt_object, bool makeCopyOfValues, jvalue* output, jValueType javaType) const{
    if(javaType!=jValueType::l)
        JavaException::raiseIllegalArgumentException(env, "Cannot convert object type" QTJAMBI_STACKTRACEINFO );
    if (!qt_object){
        output->l = nullptr;
        return true;
    }
    if(!makeCopyOfValues){
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
    void *copy = makeCopyOfValues ? m_qt_meta_type.create(qt_object) : const_cast<void*>(qt_object);
    if (!copy){
        output->l = nullptr;
        return true;
    }
    if(!creatableClass() || !creatorMethod())
        JavaException::raiseError(env, "Uncreatable type" QTJAMBI_STACKTRACEINFO );
    output->l = env->NewObject(creatableClass(), creatorMethod(), nullptr);
    JavaException::check(env QTJAMBI_STACKTRACEINFO );
    if(makeCopyOfValues || !m_deleter){
        return QtJambiLink::createLinkForNativeObject(
            env,
            output->l,
            copy,
            m_qt_meta_type,
            false,
            false,
            m_containerAccessFactory(),
            makeCopyOfValues ? QtJambiLink::Ownership::Java : QtJambiLink::Ownership::None,
            m_interfaceOffsets,
            m_interfaceTypes,
            m_inheritedInterfaces
            );
    }else{
        return QtJambiLink::createLinkForNativeObject(
            env,
            output->l,
            copy,
            LINK_NAME_ARG(qtName())
            false,
            false,
            m_deleter,
            m_containerAccessFactory(),
            makeCopyOfValues ? QtJambiLink::Ownership::Java : QtJambiLink::Ownership::None,
            m_interfaceOffsets,
            m_interfaceTypes,
            m_inheritedInterfaces
            );
    }
}

QtJambiTypeEntryPtr QObjectTypeEntry::getFittingTypeEntry(JNIEnv *env, const void *qt_object, qintptr& offset) const{
    return ::getFittingTypeEntry(env, reinterpret_cast<const QObject*>(qt_object), offset, this, {});
}

const QSharedPointer<QtJambiLink>& QObjectTypeEntry::createLinkForNativeQObject(JNIEnv *env, jobject javaObject, QObject *object) const{
    return QtJambiLink::createLinkForNativeQObject(env, javaObject, object, false, false, m_superTypeForCustomMetaObject);
}

QtJambiTypeEntry::NativeToJavaResult QObjectTypeEntry::convertToJava(JNIEnv *env, const void *ptr, bool, jvalue* output, jValueType javaType) const{
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
            if (p && p->metaObject() && p->metaObject() != qt_object->metaObject()) {
                QTJAMBI_SET_OBJECTUSERDATA(QtJambiLinkUserData, qt_object, nullptr);
                locker.unlock();
                delete p;
                // It should already be split ownership, but in case it has been changed, we need to make sure the c++
                // object isn't deleted.
                Java::QtJambi::NativeUtility$NativeLink::reset(env, nativeLink);
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
    JavaException::check(env QTJAMBI_STACKTRACEINFO );
    return createLinkForNativeQObject(env, output->l, qt_object);
}

bool QObjectTypeEntry::convertSharedPointerToJava(JNIEnv *env, void *ptr_shared_pointer, SmartPointerDeleter sharedPointerDeleter, const SmartPointerGetterFunction& sharedPointerGetter, jvalue* output, jValueType javaType) const{
    if(javaType!=jValueType::l)
        JavaException::raiseIllegalArgumentException(env, "Cannot convert object type" QTJAMBI_STACKTRACEINFO );
    if (!ptr_shared_pointer || !sharedPointerDeleter || !sharedPointerGetter){
        output->l = nullptr;
        return true;
    }
    void* _ptr = sharedPointerGetter(ptr_shared_pointer);
    if (!_ptr){
        output->l = nullptr;
        return true;
    }
    QObject* qt_object = reinterpret_cast<QObject*>(_ptr);
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
            if (p && p->metaObject() && p->metaObject() != qt_object->metaObject()) {
                QTJAMBI_SET_OBJECTUSERDATA(QtJambiLinkUserData, qt_object, nullptr);
                locker.unlock();
                delete p;
                // It should already be split ownership, but in case it has been changed, we need to make sure the c++
                // object isn't deleted.
                Java::QtJambi::NativeUtility$NativeLink::reset(env, nativeLink);
                link->setSplitOwnership(env);
                link.clear();
                locker.relock();
            }
        }
        if(link && !link->isSmartPointer()){
            jobject obj = link->getJavaObjectLocalRef(env);
            if(obj && env->IsInstanceOf(obj, javaClass())){
                output->l = obj;
                bool createdByJava = link->createdByJava();
                bool is_shell = link->isShell();
                QtJambiShellImpl* shell = nullptr;
                if(is_shell){
                    const SuperTypeInfos& infos = SuperTypeInfos::fromClass(env, env->GetObjectClass(output->l));
                    Q_ASSERT(infos.size());
                    shell = *reinterpret_cast<QtJambiShellImpl**>( quintptr(qt_object) + infos.at(0).size() );
                }
                jobject nativeLink = link->nativeLink(env);
                if(nativeLink){
                    Java::QtJambi::NativeUtility$NativeLink::reset(env, nativeLink);
                }
                link->invalidate(env);
                link.clear();
                link = QtJambiLink::createLinkForSmartPointerToQObject(env, output->l, createdByJava, is_shell, ptr_shared_pointer, sharedPointerDeleter, sharedPointerGetter, m_superTypeForCustomMetaObject);
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
    if(!creatableClass() || !creatorMethod())
        JavaException::raiseError(env, "Uncreatable type" QTJAMBI_STACKTRACEINFO );
    output->l = env->NewObject(creatableClass(), creatorMethod(), nullptr);
    JavaException::check(env QTJAMBI_STACKTRACEINFO );
    QtJambiLink::createLinkForSmartPointerToQObject(env, output->l, false, false, ptr_shared_pointer, sharedPointerDeleter, sharedPointerGetter, m_superTypeForCustomMetaObject);
    return true;
}

const QSharedPointer<QtJambiLink>& QObjectIFTypeEntry::createLinkForNativeQObject(JNIEnv *env, jobject javaObject, QObject *object) const{
    return QtJambiLink::createLinkForNativeQObject(env, javaObject, object,
                                                   false, false, m_superTypeForCustomMetaObject,
                                                   m_interfaceOffsets, m_interfaceTypes, m_inheritedInterfaces);
}

QtJambiTypeEntryPtr QObjectPolymorphicTypeEntry::getFittingTypeEntry(JNIEnv *env, const void *qt_object, qintptr& offset) const{
    return ::getFittingTypeEntry(env, reinterpret_cast<const QObject*>(qt_object), offset, this, m_polymorphicIdHandlers);
}

const QSharedPointer<QtJambiLink>& QObjectPolymorphicTypeEntry::createLinkForNativeQObject(JNIEnv *env, jobject javaObject, QObject *object) const{
    return QtJambiLink::createLinkForNativeQObject(env, javaObject, object, false, false, m_superTypeForCustomMetaObject);
}

bool QObjectTypeEntry::convertToNative(JNIEnv *env, const jvalue& java_value, jValueType javaType, void * output, QtJambiScope*) const{
    if(javaType!=jValueType::l)
        JavaException::raiseIllegalArgumentException(env, "Cannot convert to object type" QTJAMBI_STACKTRACEINFO );
    if(env->IsInstanceOf(java_value.l, this->javaClass())){
        if (QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForJavaObject(env, java_value.l)){
            Q_ASSERT(link->isQObject());
            *reinterpret_cast<QObject**>(output) = link->qobject();
        }
        else if(Java::QtJambi::QtObjectInterface::isInstanceOf(env, java_value.l))
            Java::QtJambi::QNoNativeResourcesException::throwNew(env, QStringLiteral("Incomplete object of type: %1").arg(QtJambiAPI::getObjectClassName(env, java_value.l).replace("$", ".")) QTJAMBI_STACKTRACEINFO );
        return true;
    }else return false;
}

QtJambiTypeEntryPtr QObjectPolymorphicIFTypeEntry::getFittingTypeEntry(JNIEnv *env, const void *qt_object, qintptr& offset) const{
    return ::getFittingTypeEntry(env, reinterpret_cast<const QObject*>(qt_object), offset, this, m_polymorphicIdHandlers);
}

const QSharedPointer<QtJambiLink>& QObjectPolymorphicIFTypeEntry::createLinkForNativeQObject(JNIEnv *env, jobject javaObject, QObject *object) const{
    return QtJambiLink::createLinkForNativeQObject(env, javaObject, object, false, false, m_superTypeForCustomMetaObject,
                                                   m_interfaceOffsets, m_interfaceTypes, m_inheritedInterfaces);
}

QtJambiTypeEntry::NativeToJavaResult FunctionalTypeEntry::convertToJava(JNIEnv *env, const void *qt_object, bool, jvalue* output, jValueType javaType) const{
    if(javaType!=jValueType::l)
        JavaException::raiseIllegalArgumentException(env, "Cannot convert functional type" QTJAMBI_STACKTRACEINFO );
    if(m_registered_functional_resolver){
        output->l = m_registered_functional_resolver(env, m_is_std_function ? qt_object : &qt_object);
        if(output->l)
            return true;
    }
    if(!creatableClass() || !creatorMethod())
        JavaException::raiseError(env, "Uncreatable type" QTJAMBI_STACKTRACEINFO );
    output->l = env->NewObject(creatableClass(), creatorMethod(), nullptr);
    JavaException::check(env QTJAMBI_STACKTRACEINFO );
    return QtJambiLink::createLinkForNativeObject(
            env,
            output->l,
            m_qt_meta_type.create(m_is_std_function ? qt_object : &qt_object),
            m_qt_meta_type,
            false,
            false,
            QtJambiLink::Ownership::Java
        );
}

bool FunctionalTypeEntry::convertSharedPointerToJava(JNIEnv *env, void *qt_object, SmartPointerDeleter, const SmartPointerGetterFunction& sharedPointerGetter, jvalue* output, jValueType javaType) const{
    return convertToJava(env, sharedPointerGetter(qt_object), true, output, javaType);
}

bool FunctionalTypeEntry::convertToNative(JNIEnv *env, const jvalue& java_value, jValueType javaType, void * output, QtJambiScope*) const{
    if(javaType!=jValueType::l)
        JavaException::raiseIllegalArgumentException(env, "Cannot convert to functional type" QTJAMBI_STACKTRACEINFO );
    if(!env->IsInstanceOf(java_value.l, javaClass()))
        return false;
    if(java_value.l
        && (QtJambiAPI::getObjectClassName(env, java_value.l).endsWith("$ConcreteWrapper")
            || env->IsSameObject(env->GetObjectClass(java_value.l), creatableClass()))){
        const QSharedPointer<QtJambiLink>& link = QtJambiLink::findLinkForJavaInterface(env, java_value.l);
        if(link){
            if(m_is_std_function){
                m_qt_meta_type.destruct(output);
                m_qt_meta_type.construct(output, link->pointer());
            }else{
                *reinterpret_cast<void**>(output) = *reinterpret_cast<void**>(link->pointer());
            }
        }else if(java_value.l){
            Java::QtJambi::QNoNativeResourcesException::throwNew(env, QStringLiteral("Incomplete object of type: %1").arg(QLatin1String(this->javaName())) QTJAMBI_STACKTRACEINFO );
        }
    }else{
        if (QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForJavaInterface(env, java_value.l)){
            if(!link->isMultiInheritanceType() && env->IsInstanceOf(java_value.l, javaClass())){
                if(FunctionalBase* functionalBase = reinterpret_cast<FunctionalBase *>(link->pointer())){
                    functionalBase->getFunctional(env, output);
                }
            }else if(link->isInterfaceAvailable(type())){
                if(FunctionalBase* functionalBase = reinterpret_cast<FunctionalBase *>(link->typedPointer(type()))){
                    functionalBase->getFunctional(env, output);
                }
            }
        }else if(java_value.l){
            Java::QtJambi::QNoNativeResourcesException::throwNew(env, QStringLiteral("Incomplete object of type: %1").arg(QLatin1String(this->javaName())) QTJAMBI_STACKTRACEINFO );
        }
    }
    return true;
}

QtJambiTypeEntry::NativeToJavaResult EnumTypeEntry::convertToJava(JNIEnv *env, const void *qt_object, bool, jvalue* output, jValueType javaType) const{
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
        JavaException::check(env QTJAMBI_STACKTRACEINFO );
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

bool EnumTypeEntry::convertSharedPointerToJava(JNIEnv *env, void *qt_object, SmartPointerDeleter, const SmartPointerGetterFunction& sharedPointerGetter, jvalue* output, jValueType javaType) const{
    return convertToJava(env, sharedPointerGetter(qt_object), true, output, javaType);
}

bool EnumTypeEntry::convertToNative(JNIEnv *env, const jvalue& java_value, jValueType javaType, void * output, QtJambiScope*) const{
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

QtJambiTypeEntry::NativeToJavaResult FlagsTypeEntry::convertToJava(JNIEnv *env, const void *qt_object, bool, jvalue* output, jValueType javaType) const{
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

bool FlagsTypeEntry::convertSharedPointerToJava(JNIEnv *env, void *qt_object, SmartPointerDeleter, const SmartPointerGetterFunction& sharedPointerGetter, jvalue* output, jValueType javaType) const{
    return convertToJava(env, sharedPointerGetter(qt_object), true, output, javaType);
}

bool FlagsTypeEntry::convertToNative(JNIEnv *env, const jvalue& java_value, jValueType javaType, void * output, QtJambiScope*) const{
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

bool AbstractSimpleTypeEntry::convertSharedPointerToJava(JNIEnv *env, void *qt_object, SmartPointerDeleter, const SmartPointerGetterFunction& sharedPointerGetter, jvalue* output, jValueType javaType) const{
    return convertToJava(env, sharedPointerGetter(qt_object), true, output, javaType);
}

JLongTypeEntry::JLongTypeEntry(JNIEnv* env, const std::type_info& typeId, const char *qt_name, const char *java_name, jclass java_class, size_t value_size)
    : AbstractSimpleTypeEntry(env, typeId, qt_name, java_name, java_class, value_size)
{
}

StringTypeEntry::StringTypeEntry(JNIEnv* env, const std::type_info& typeId, const char *qt_name, const char *java_name, jclass java_class, size_t value_size)
    : AbstractSimpleTypeEntry(env, typeId, qt_name, java_name, java_class, value_size)
{
}

QtJambiTypeEntry::NativeToJavaResult StringTypeEntry::convertToJava(JNIEnv *env, const void *qt_object, bool, jvalue* output, jValueType javaType) const{
    const QString* strg = reinterpret_cast<const QString*>(qt_object);
    switch (javaType) {
    case jValueType::l:
        Q_ASSERT(strg->length()>=0);
        output->l = env->NewString(reinterpret_cast<const jchar *>(strg->constData()), jsize(strg->length()));
        JavaException::check(env QTJAMBI_STACKTRACEINFO );
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

bool StringTypeEntry::convertToNative(JNIEnv *env, const jvalue& java_value, jValueType javaType, void * output, QtJambiScope*) const{
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
            if(Java::QtCore::QString::isInstanceOf(env, java_value.l)){
                QString* strg = QtJambiAPI::convertJavaObjectToNative<QString>(env, java_value.l);
                QtJambiAPI::checkNullPointer(env, strg);
                *value = *strg;
            }else{
                jstring in = QtJambiAPI::toJavaString(env, java_value.l);
                int length = in ? env->GetStringLength(in) : 0;
                value->resize(length);
                if(length>0)
                    env->GetStringRegion(in, 0, length, reinterpret_cast<ushort*>(value->data()));
                JavaException::check(env QTJAMBI_STACKTRACEINFO );
            }
            break;
        case jValueType::d:
            *value = QString::number(java_value.d);
        case jValueType::f:
            *value = QString::number(java_value.f);
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

QtJambiTypeEntry::NativeToJavaResult QCborValueRefTypeEntry::convertToJava(JNIEnv *env, const void *qt_object, bool makeCopyOfValues, jvalue* output, jValueType javaType) const{
    const QCborValueRef* vref = reinterpret_cast<const QCborValueRef*>(qt_object);
    QtJambiTypeEntryPtr typeEntry = QtJambiTypeEntry::getTypeEntry(env, typeid(QCborValue), "QCborValue");
    Q_ASSERT(typeEntry);
    QCborValue value = *vref;
    return typeEntry->convertToJava(env, &value, makeCopyOfValues, output, javaType);
}

bool QCborValueRefTypeEntry::convertToNative(JNIEnv *env, const jvalue&, jValueType, void *, QtJambiScope*) const{
    JavaException::raiseError(env, "Cannot convert to QCborValueRef" QTJAMBI_STACKTRACEINFO );
    return false;
}
#endif

StringUtilTypeEntry::StringUtilTypeEntry(JNIEnv* env, const std::type_info& typeId, const char *qt_name, const char *java_name, jclass java_class, size_t value_size)
    : AbstractSimpleTypeEntry(env, typeId, qt_name, java_name, java_class, value_size)
{
}

QtJambiTypeEntry::NativeToJavaResult StringUtilTypeEntry::convertToJava(JNIEnv *env, const void *qt_object, bool, jvalue* output, jValueType javaType) const{
    if(typeid_equals(type(), typeid(QStringView))){
        const QStringView* sref = reinterpret_cast<const QStringView*>(qt_object);
        if(sref){
            switch (javaType) {
            case jValueType::l:
                Q_ASSERT(sref->length()>=0);
                output->l = env->NewString(reinterpret_cast<const jchar *>(sref->data()), jsize(sref->length()));
                JavaException::check(env QTJAMBI_STACKTRACEINFO );
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
    }else if(typeid_equals(type(), typeid(QStringRef))){
        const QStringRef* sref = reinterpret_cast<const QStringRef*>(qt_object);
        if(sref){
            switch (javaType) {
            case jValueType::l:
                Q_ASSERT(sref->length()>=0);
                output->l = env->NewString(reinterpret_cast<const jchar *>(sref->toString().constData()), jsize(sref->toString().length()));
                JavaException::check(env QTJAMBI_STACKTRACEINFO );
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
    }else if(typeid_equals(type(), typeid(QXmlStreamStringRef))){
        const QXmlStreamStringRef* xref = reinterpret_cast<const QXmlStreamStringRef*>(qt_object);
        if(xref){
            QStringRef sref(*xref);
            switch (javaType) {
            case jValueType::l:
                Q_ASSERT(sref.length()>=0);
                output->l = env->NewString(reinterpret_cast<const jchar *>(sref.toString().constData()), jsize(sref.toString().length()));
                JavaException::check(env QTJAMBI_STACKTRACEINFO );
                output->l = qtjambi_cast<jstring>(env, sref);
                break;
            case jValueType::z:
                output->z = !sref.isEmpty();
                break;
            case jValueType::c:
                output->c = sref.toString().isEmpty() ? 0 : sref.toString().at(0).unicode();
                break;
            default:
                output->j = sref.toString().toLong();
                break;
            }
        }
#else
    }else if(typeid_equals(type(), typeid(QAnyStringView))){
        const QAnyStringView* sref = reinterpret_cast<const QAnyStringView*>(qt_object);
        if(sref){
            switch (javaType) {
            case jValueType::l:
                output->l = QtJambiAPI::convertNativeToJavaObject(env, *sref);
                break;
            case jValueType::z:
                output->z = !sref->isEmpty();
                break;
            case jValueType::c:
                output->c = !sref->isEmpty() ? sref->front().unicode() : 0;
                break;
            default:
                output->j = sref->toString().toLong();
                break;
            }
        }
    }else if(typeid_equals(type(), typeid(QUtf8StringView))){
        const QUtf8StringView* sref = reinterpret_cast<const QUtf8StringView*>(qt_object);
        if(sref){
            switch (javaType) {
            case jValueType::l:
                output->l = qtjambi_cast<jstring>(env, *sref);
                break;
            case jValueType::z:
                output->z = !sref->isEmpty();
                break;
            case jValueType::c:
                output->c = !sref->isEmpty() ? sref->front() : 0;
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
                output->l = env->NewStringUTF(sref->data());
                JavaException::check(env QTJAMBI_STACKTRACEINFO );
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

bool StringUtilTypeEntry::convertToNative(JNIEnv *env, const jvalue&, jValueType, void *, QtJambiScope*) const{
    if(typeid_equals(type(), typeid(QStringView))){
        JavaException::raiseError(env, "Cannot convert to QStringView" QTJAMBI_STACKTRACEINFO );
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    }else if(typeid_equals(type(), typeid(QStringRef))){
        JavaException::raiseError(env, "Cannot convert to QStringRef" QTJAMBI_STACKTRACEINFO );
    }else if(typeid_equals(type(), typeid(QXmlStreamStringRef))){
        JavaException::raiseError(env, "Cannot convert to QXmlStreamStringRef" QTJAMBI_STACKTRACEINFO );
#else
    }else if(typeid_equals(type(), typeid(QAnyStringView))){
        JavaException::raiseError(env, "Cannot convert to QAnyStringView" QTJAMBI_STACKTRACEINFO );
    }else if(typeid_equals(type(), typeid(QUtf8StringView))){
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

QtJambiTypeEntry::NativeToJavaResult MetaUtilTypeEntry::convertToJava(JNIEnv *env, const void *qt_object, bool, jvalue* output, jValueType javaType) const{
    if(javaType!=jValueType::l)
        JavaException::raiseIllegalArgumentException(env, "Cannot convert to primitive value" QTJAMBI_STACKTRACEINFO );
    if(qt_object){
        if(typeid_equals(type(), typeid(QMetaObject::Connection))){
            static QMetaType metaTypeId(registeredMetaTypeID(typeid(QMetaObject::Connection)));
            output->l = Java::QtJambi::SignalUtility$NativeConnection::newInstance(env, nullptr);
            const QSharedPointer<QtJambiLink>& link = QtJambiLink::createLinkForNativeObject(
                    env,
                    output->l,
                    new QMetaObject::Connection(*reinterpret_cast<const QMetaObject::Connection*>(qt_object)),
                    metaTypeId,
                    false,
                    false,
                    QtJambiLink::Ownership::Java
                );
            Q_UNUSED(link)
        }else if(typeid_equals(type(), typeid(QMetaObject))){
            output->l = QtJambiMetaObject::convertToJavaObject(env, reinterpret_cast<const QMetaObject*>(qt_object));
        }else if(typeid_equals(type(), typeid(JIteratorWrapper))
                 || typeid_equals(type(), typeid(JCollectionWrapper))
                 || typeid_equals(type(), typeid(JMapWrapper))
                 || typeid_equals(type(), typeid(JObjectWrapper))
                 || typeid_equals(type(), typeid(JEnumWrapper))){
            output->l = env->NewLocalRef(reinterpret_cast<const JObjectWrapper*>(qt_object)->object());
        }
    }
    return true;
}

bool MetaUtilTypeEntry::convertToNative(JNIEnv *env, const jvalue& java_value, jValueType javaType, void *output, QtJambiScope*) const{
    if(javaType!=jValueType::l)
        JavaException::raiseIllegalArgumentException(env, "Cannot convert to primitive value" QTJAMBI_STACKTRACEINFO );
    if(typeid_equals(type(), typeid(QMetaObject::Connection))){
        if(!env->IsSameObject(java_value.l, nullptr)){
            if(!Java::QtCore::QMetaObject$Connection::isInstanceOf(env, java_value.l))
                return false;
            if (QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForJavaObject(env, java_value.l)){
                Q_ASSERT(!link->isQObject());
                *reinterpret_cast<QMetaObject::Connection*>(output) = *reinterpret_cast<QMetaObject::Connection*>(link->pointer());
            }
            else if(Java::QtJambi::QtObjectInterface::isInstanceOf(env, java_value.l))
                Java::QtJambi::QNoNativeResourcesException::throwNew(env, QStringLiteral("Incomplete object of type: %1").arg(QtJambiAPI::getObjectClassName(env, java_value.l).replace("$", ".")) QTJAMBI_STACKTRACEINFO );
        }else{
            *reinterpret_cast<QMetaObject::Connection*>(output) = QMetaObject::Connection();
        }
    }else if(typeid_equals(type(), typeid(QMetaObject))){
        if(!env->IsSameObject(java_value.l, nullptr)){
            if(!Java::QtCore::QMetaObject::isInstanceOf(env, java_value.l))
                return false;
            jlong ptr = Java::QtCore::QMetaObject::metaObjectPointer(env, java_value.l);
            *reinterpret_cast<const QMetaObject**>(output) = reinterpret_cast<const QMetaObject *>(ptr);
        }else{
            *reinterpret_cast<const QMetaObject**>(output) = nullptr;
        }
    }else if(typeid_equals(type(), typeid(JIteratorWrapper))
             || typeid_equals(type(), typeid(JCollectionWrapper))
             || typeid_equals(type(), typeid(JMapWrapper))
             || typeid_equals(type(), typeid(JObjectWrapper))
             || typeid_equals(type(), typeid(JEnumWrapper))){
        *reinterpret_cast<JObjectWrapper*>(output) = JObjectWrapper(env, java_value.l);
    }
    return true;
}

QModelIndexTypeEntry::QModelIndexTypeEntry(JNIEnv* env,
                                           const std::type_info& typeId,
                                           const char *qt_name,
                                           const char *java_name,
                                           jclass java_class,
                                           jmethodID creator_method,
                                           size_t value_size,
                                           const std::type_info* super_type,
                                           size_t shell_size,
                                           PtrDeleterFunction deleter,
                                           PtrOwnerFunction owner_function,
                                           const QVector<const FunctionInfo>* virtualFunctions,
                                           Destructor destructor,
                                           const QList<const PolymorphicIdHandler*>& polymorphicHandlers,
                                           TypeInfoSupplier typeInfoSupplier,
                                           const QMetaType& qt_meta_type)
    : ObjectValueTypeEntry(env,
                           typeId,
                           qt_name,
                           java_name,
                           java_class,
                           creator_method,
                           value_size,
                           super_type,
                           shell_size,
                           deleter,
                           owner_function,
                           virtualFunctions,
                           destructor,
                           polymorphicHandlers,
                           typeInfoSupplier,
                           qt_meta_type)
{
}

QtJambiTypeEntry::NativeToJavaResult QModelIndexTypeEntry::convertToJava(JNIEnv *env, const void *qt_object, bool makeCopyOfValues, jvalue* output, jValueType javaType) const{
    if(javaType!=jValueType::l)
        JavaException::raiseIllegalArgumentException(env, "Cannot convert object type" QTJAMBI_STACKTRACEINFO );
    if (!qt_object){
        output->l = nullptr;
        return true;
    }
    if(!makeCopyOfValues){
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
    void *copy = makeCopyOfValues ? m_qt_meta_type.create(qt_object) : const_cast<void*>(qt_object);
    if (!copy){
        output->l = nullptr;
        return true;
    }
    if(!creatableClass() || !creatorMethod())
        JavaException::raiseError(env, "Uncreatable type" QTJAMBI_STACKTRACEINFO );
    const QModelIndex* index = reinterpret_cast<const QModelIndex*>(qt_object);
    output->l = env->NewObject(creatableClass(), creatorMethod(), nullptr);
    JavaException::check(env QTJAMBI_STACKTRACEINFO );
    if(index->model()){
        if(m_owner_function){
            return QtJambiLink::createLinkForDependentObject(
                env,
                output->l,
                copy,
                m_qt_meta_type,
                false,
                false,
                m_owner_function,
                index->model(),
                makeCopyOfValues ? QtJambiLink::Ownership::Java : QtJambiLink::Ownership::None
                );
        }else{
            return QtJambiLink::createLinkForDependentObject(
                env,
                output->l,
                copy,
                m_qt_meta_type,
                false,
                false,
                index->model(),
                makeCopyOfValues ? QtJambiLink::Ownership::Java : QtJambiLink::Ownership::None
                );
        }
    }else{
        if(m_owner_function){
            return QtJambiLink::createLinkForNativeObject(
                env,
                output->l,
                copy,
                m_qt_meta_type,
                false,
                false,
                m_owner_function,
                makeCopyOfValues ? QtJambiLink::Ownership::Java : QtJambiLink::Ownership::None
                );
        }else{
            return QtJambiLink::createLinkForNativeObject(
                env,
                output->l,
                copy,
                m_qt_meta_type,
                false,
                false,
                makeCopyOfValues ? QtJambiLink::Ownership::Java : QtJambiLink::Ownership::None
                );
        }
    }
}

bool QModelIndexTypeEntry::convertSharedPointerToJava(JNIEnv *env, void *ptr_shared_pointer, SmartPointerDeleter sharedPointerDeleter, const SmartPointerGetterFunction& sharedPointerGetter, jvalue* output, jValueType valueType) const{
    bool result = ObjectValueTypeEntry::convertSharedPointerToJava(env, ptr_shared_pointer, sharedPointerDeleter, sharedPointerGetter, output, valueType);
    if(valueType==jValueType::l && result && ptr_shared_pointer){
        if(const QModelIndex* index = reinterpret_cast<const QModelIndex*>(sharedPointerGetter(ptr_shared_pointer))){
            if(index->model())
                QtJambiLink::registerDependentObject(index->model(), QtJambiLink::findLinkForJavaObject(env, output->l));
        }
    }
    return result;
}

QVariantTypeEntry::QVariantTypeEntry(JNIEnv* env, const std::type_info& typeId, const char *qt_name, const char *java_name, jclass java_class, size_t value_size)
    : AbstractSimpleTypeEntry(env, typeId, qt_name, java_name, java_class, value_size)
{
}

QtJambiTypeEntry::NativeToJavaResult QVariantTypeEntry::convertToJava(JNIEnv *env, const void *qt_object, bool, jvalue* output, jValueType javaType) const{
    if(javaType!=jValueType::l)
        JavaException::raiseIllegalArgumentException(env, "Cannot convert QVariant to primitive value" QTJAMBI_STACKTRACEINFO );
    output->l = QtJambiAPI::convertQVariantToJavaObject(env, *reinterpret_cast<const QVariant*>(qt_object));
    return true;
}

bool QVariantTypeEntry::convertToNative(JNIEnv *env, const jvalue& java_value, jValueType javaType, void *output, QtJambiScope*) const{
    if(javaType!=jValueType::l)
        JavaException::raiseIllegalArgumentException(env, "Cannot convert primitive value to QVariant" QTJAMBI_STACKTRACEINFO );
    *reinterpret_cast<QVariant*>(output) = QtJambiAPI::convertJavaObjectToQVariant(env, java_value.l);
    return true;
}

QtJambiTypeEntry::NativeToJavaResult JLongTypeEntry::convertToJava(JNIEnv *env, const void *qt_object, bool, jvalue* output, jValueType javaType) const{
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
            output->l = QtJambiAPI::toJavaLongObject(env, *value);
            break;
        }
    }
    return true;
}


QMessageLogContextTypeEntry::QMessageLogContextTypeEntry(JNIEnv* env,
                                 const std::type_info& typeId,
                                 const char *qt_name,
                                 const char *java_name,
                                 jclass java_class,
                                 jmethodID creator_method,
                                 size_t value_size,
                                 const std::type_info* super_type,
                                 size_t shell_size,
                                 PtrDeleterFunction deleter,
                                 PtrOwnerFunction owner_function,
                                 const QVector<const FunctionInfo>* virtualFunctions,
                                 Destructor destructor,
                                 const QList<const PolymorphicIdHandler*>& polymorphicHandlers,
                                 TypeInfoSupplier typeInfoSupplier
                                 )
    : ObjectTypeEntry(env, typeId, qt_name, java_name, java_class, creator_method, value_size, super_type, shell_size, deleter, owner_function, virtualFunctions, destructor, polymorphicHandlers, typeInfoSupplier)
{
}

QtJambiTypeEntry::NativeToJavaResult QMessageLogContextTypeEntry::convertToJava(JNIEnv *env, const void *qt_object, bool, jvalue* output, jValueType javaType) const{
    if(javaType!=jValueType::l)
        JavaException::raiseIllegalArgumentException(env, "Cannot convert object type" QTJAMBI_STACKTRACEINFO );
    if (!qt_object){
        output->l = nullptr;
        return true;
    }
    if(!creatableClass() || !creatorMethod())
        JavaException::raiseError(env, "Uncreatable type" QTJAMBI_STACKTRACEINFO );
    output->l = env->NewObject(creatableClass(), creatorMethod(), nullptr);
    JavaException::check(env QTJAMBI_STACKTRACEINFO );
    const QSharedPointer<QtJambiLink>& link = QtJambiLink::createLinkForNativeObject(
        env,
        output->l,
        const_cast<void*>(qt_object),
        LINK_NAME_ARG(qtName())
        false,
        false,
        m_deleter,
        m_owner_function,
        QtJambiLink::Ownership::None
        );
    link->disableDebugMessaging();
    return link;
}

bool JLongTypeEntry::convertToNative(JNIEnv *env, const jvalue& java_value, jValueType javaType, void * output, QtJambiScope*) const{
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
            *value = QtJambiAPI::fromJavaLongObject(env, java_value.l);
            break;
        }
    }
    return true;
}

JIntTypeEntry::JIntTypeEntry(JNIEnv* env, const std::type_info& typeId, const char *qt_name, const char *java_name, jclass java_class, size_t value_size)
    : AbstractSimpleTypeEntry(env, typeId, qt_name, java_name, java_class, value_size)
{
}

QtJambiTypeEntry::NativeToJavaResult JIntTypeEntry::convertToJava(JNIEnv *env, const void *qt_object, bool, jvalue* output, jValueType javaType) const{
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
            output->l = QtJambiAPI::toJavaIntegerObject(env, *value);
            break;
        }
    }
    return true;
}

bool JIntTypeEntry::convertToNative(JNIEnv *env, const jvalue& java_value, jValueType javaType, void * output, QtJambiScope*) const{
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
            *value = QtJambiAPI::fromJavaIntegerObject(env, java_value.l);
            break;
        }
    }
    return true;
}

JShortTypeEntry::JShortTypeEntry(JNIEnv* env, const std::type_info& typeId, const char *qt_name, const char *java_name, jclass java_class, size_t value_size)
    : AbstractSimpleTypeEntry(env, typeId, qt_name, java_name, java_class, value_size)
{
}

QtJambiTypeEntry::NativeToJavaResult JShortTypeEntry::convertToJava(JNIEnv *env, const void *qt_object, bool, jvalue* output, jValueType javaType) const{
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
            output->l = QtJambiAPI::toJavaShortObject(env, *value);
            break;
        }
    }
    return true;
}

bool JShortTypeEntry::convertToNative(JNIEnv *env, const jvalue& java_value, jValueType javaType, void * output, QtJambiScope*) const{
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
            *value = QtJambiAPI::fromJavaShortObject(env, java_value.l);
            break;
        }
    }
    return true;
}

JByteTypeEntry::JByteTypeEntry(JNIEnv* env, const std::type_info& typeId, const char *qt_name, const char *java_name, jclass java_class, size_t value_size)
    : AbstractSimpleTypeEntry(env, typeId, qt_name, java_name, java_class, value_size)
{
}

QtJambiTypeEntry::NativeToJavaResult JByteTypeEntry::convertToJava(JNIEnv *env, const void *qt_object, bool, jvalue* output, jValueType javaType) const{
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
            output->l = QtJambiAPI::toJavaByteObject(env, *value);
            break;
        }
    }
    return true;
}

bool JByteTypeEntry::convertToNative(JNIEnv *env, const jvalue& java_value, jValueType javaType, void * output, QtJambiScope*) const{
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
            *value = QtJambiAPI::fromJavaByteObject(env, java_value.l);
            break;
        }
    }
    return true;
}

JCharTypeEntry::JCharTypeEntry(JNIEnv* env, const std::type_info& typeId, const char *qt_name, const char *java_name, jclass java_class, size_t value_size)
    : AbstractSimpleTypeEntry(env, typeId, qt_name, java_name, java_class, value_size)
{
}

QtJambiTypeEntry::NativeToJavaResult JCharTypeEntry::convertToJava(JNIEnv *env, const void *qt_object, bool, jvalue* output, jValueType javaType) const{
    if(typeid_equals(type(), typeid(wchar_t)) || typeid_equals(type(), typeid(jchar))){
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
                output->l = QtJambiAPI::toJavaCharacterObject(env, *value);
                break;
            }
        }
    }else if(typeid_equals(type(), typeid(QChar))){
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
                output->l = QtJambiAPI::toJavaCharacterObject(env, value->cell());
                break;
            }
        }
    }else if(typeid_equals(type(), typeid(QLatin1Char))){
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
                output->l = QtJambiAPI::toJavaCharacterObject(env, value->unicode());
                break;
            }
        }
    }
    return true;
}

bool JCharTypeEntry::convertToNative(JNIEnv *env, const jvalue& java_value, jValueType javaType, void * output, QtJambiScope*) const{
    if(typeid_equals(type(), typeid(wchar_t)) || typeid_equals(type(), typeid(jchar))){
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
                *value = QtJambiAPI::fromJavaCharacterObject(env, java_value.l);
                break;
            }
        }
    }else if(typeid_equals(type(), typeid(QChar))){
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
                *value = QtJambiAPI::fromJavaCharacterObject(env, java_value.l);
                break;
            }
        }
    }else if(typeid_equals(type(), typeid(QLatin1Char))){
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
                *value = QLatin1Char(jbyte(QtJambiAPI::fromJavaCharacterObject(env, java_value.l)));
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

QtJambiTypeEntry::NativeToJavaResult JBooleanTypeEntry::convertToJava(JNIEnv *env, const void *qt_object, bool, jvalue* output, jValueType javaType) const{
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
            output->l = QtJambiAPI::toJavaBooleanObject(env, *value);
            break;
        }
    }
    return true;
}

bool JBooleanTypeEntry::convertToNative(JNIEnv *env, const jvalue& java_value, jValueType javaType, void * output, QtJambiScope*) const{
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
            *value = QtJambiAPI::fromJavaBooleanObject(env, java_value.l);
            break;
        }
    }
    return true;
}

JDoubleTypeEntry::JDoubleTypeEntry(JNIEnv* env, const std::type_info& typeId, const char *qt_name, const char *java_name, jclass java_class, size_t value_size)
    : AbstractSimpleTypeEntry(env, typeId, qt_name, java_name, java_class, value_size)
{
}

QtJambiTypeEntry::NativeToJavaResult JDoubleTypeEntry::convertToJava(JNIEnv *env, const void *qt_object, bool, jvalue* output, jValueType javaType) const{
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
            output->l = QtJambiAPI::toJavaDoubleObject(env, *value);
            break;
        }
    }
    return true;
}

bool JDoubleTypeEntry::convertToNative(JNIEnv *env, const jvalue& java_value, jValueType javaType, void * output, QtJambiScope*) const{
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
            *value = QtJambiAPI::fromJavaDoubleObject(env, java_value.l);
            break;
        }
    }
    return true;
}

JFloatTypeEntry::JFloatTypeEntry(JNIEnv* env, const std::type_info& typeId, const char *qt_name, const char *java_name, jclass java_class, size_t value_size)
    : AbstractSimpleTypeEntry(env, typeId, qt_name, java_name, java_class, value_size)
{
}

QtJambiTypeEntry::NativeToJavaResult JFloatTypeEntry::convertToJava(JNIEnv *env, const void *qt_object, bool, jvalue* output, jValueType javaType) const{
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
            output->l = QtJambiAPI::toJavaFloatObject(env, *value);
            break;
        }
    }
    return true;
}

bool JFloatTypeEntry::convertToNative(JNIEnv *env, const jvalue& java_value, jValueType javaType, void * output, QtJambiScope*) const{
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
            *value = QtJambiAPI::fromJavaFloatObject(env, java_value.l);
            break;
        }
    }
    return true;
}

NullptrTypeEntry::NullptrTypeEntry(JNIEnv* env, const std::type_info& typeId, const char *qt_name, const char *java_name, jclass java_class, size_t value_size)
    : AbstractSimpleTypeEntry(env, typeId, qt_name, java_name, java_class, value_size)
{
}

QtJambiTypeEntry::NativeToJavaResult NullptrTypeEntry::convertToJava(JNIEnv *, const void *, bool, jvalue* output, jValueType) const{
    output->l = nullptr;
    return true;
}

bool NullptrTypeEntry::convertToNative(JNIEnv *, const jvalue&, jValueType, void * output, QtJambiScope*) const{
    if(void** value = reinterpret_cast<void**>(output))
        *value = nullptr;
    return true;
}
