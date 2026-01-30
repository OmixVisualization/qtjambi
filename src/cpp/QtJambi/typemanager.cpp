/****************************************************************************
**
** Copyright (C) 2009-2026 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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

#include <QtCore/qcompilerdetection.h>
QT_WARNING_DISABLE_DEPRECATED
QT_WARNING_DISABLE_GCC("-Wignored-qualifiers")
QT_WARNING_DISABLE_CLANG("-Wignored-qualifiers")
QT_WARNING_DISABLE_GCC("-Winit-list-lifetime")

#include "pch_p.h"

Q_LOGGING_CATEGORY(CATEGORY, "io.qtjambi.typemanager", QtWarningMsg)

const char* getInterface(const char*qt_interface);
const char* getQtNameByFunctional(const char* java_name);
jobject convertNativeToJavaObjectOfClass(JNIEnv *env, const void *qt_object, const QByteArray& className, jclass clazz, const InterfaceOffsetInfo* interfaceOffsets, const QMetaType& metaType, NativeToJavaConversionMode mode, bool *ok);

#if defined(QTJAMBI_GENERIC_ACCESS)
struct ContainerTypeInfo{
    bool isPointer;
    QMetaType metaType;
    QtJambiUtils::QHashFunction hashFunction;
    QtPrivate::QMetaTypeInterface::DefaultCtrFn defaultCtr;
    QtPrivate::QMetaTypeInterface::CopyCtrFn copyCtr;
    QtPrivate::QMetaTypeInterface::MoveCtrFn moveCtr;
    QtPrivate::QMetaTypeInterface::DtorFn dtor;
    QtPrivate::QMetaTypeInterface::EqualsFn equals;
    QtPrivate::QMetaTypeInterface::LessThanFn lessThan;
    QtPrivate::QMetaTypeInterface::DebugStreamFn debugStream;
    QtPrivate::QMetaTypeInterface::DataStreamOutFn dataStreamOut;
    QtPrivate::QMetaTypeInterface::DataStreamInFn dataStreamIn;
};

struct BiContainerTypeInfo{
    bool isPointer1;
    QMetaType metaType1;
    QtJambiUtils::QHashFunction hashFunction1;
    bool isPointer2;
    QMetaType metaType2;
    QtJambiUtils::QHashFunction hashFunction2;
    QtPrivate::QMetaTypeInterface::DefaultCtrFn defaultCtr;
    QtPrivate::QMetaTypeInterface::CopyCtrFn copyCtr;
    QtPrivate::QMetaTypeInterface::MoveCtrFn moveCtr;
    QtPrivate::QMetaTypeInterface::DtorFn dtor;
    QtPrivate::QMetaTypeInterface::EqualsFn equals;
    QtPrivate::QMetaTypeInterface::LessThanFn lessThan;
    QtPrivate::QMetaTypeInterface::DebugStreamFn debugStream;
    QtPrivate::QMetaTypeInterface::DataStreamOutFn dataStreamOut;
    QtPrivate::QMetaTypeInterface::DataStreamInFn dataStreamIn;
};
#endif

jobject convertQObjectSmartPointerToJavaObject(JNIEnv *env, const char *className,
                                                    const std::shared_ptr<QObject>& ptr_shared_pointer);
jobject convertQObjectSmartPointerToJavaObject(JNIEnv *env, const char *className,
                                                    const QSharedPointer<QObject>& ptr_shared_pointer);

QMetaType registerMetaTypeImpl(const std::type_info* typeId,
                     const std::type_info* nonPointerTypeId,
                     const QByteArray& typeName,
                     QtPrivate::QMetaTypeInterface::DefaultCtrFn defaultCtr,
                     QtPrivate::QMetaTypeInterface::CopyCtrFn copyCtr,
                     QtPrivate::QMetaTypeInterface::MoveCtrFn moveCtr,
                     QtPrivate::QMetaTypeInterface::DtorFn dtor,
                     QtPrivate::QMetaTypeInterface::EqualsFn equals,
                     QtPrivate::QMetaTypeInterface::LessThanFn lessThan,
                     QtPrivate::QMetaTypeInterface::DebugStreamFn debugStream,
                     QtPrivate::QMetaTypeInterface::DataStreamOutFn dataStreamOutFn,
                     QtPrivate::QMetaTypeInterface::DataStreamInFn dataStreamInFn,
                     QtPrivate::QMetaTypeInterface::LegacyRegisterOp legacyRegisterOp,
                     uint size,
                     ushort align,
                     int builtInTypeId,
                     QMetaType::TypeFlags flags,
                     const QMetaObject *metaObject,
                     AfterRegistrationFunction afterRegistrationFunction,
                     AbstractContainerAccess* access,
                     const QSharedPointer<AbstractContainerAccess>& sharedAccess);

void QtJambiTypeManager::splitClassName(QString& className, QString& package, const QString &qualifiedName, QChar separator){
    auto idx = qualifiedName.lastIndexOf(separator);
    if (idx >= 0){
        className = qualifiedName.mid(idx + 1);
        package = qualifiedName.left(idx + 1);
    }else{
        className = qualifiedName;
        package = QString();
    }
}

bool isPointerType(const QString& typeName){
    if(typeName.endsWith("*"))
        return true;
    else{
        int idx = typeName.indexOf("(*)");
        if(idx<0)
            idx = typeName.indexOf("(__cdecl*)");
        if(idx>0){
            QString prefix = typeName.left(idx);
            return prefix.count('<')==prefix.count('>');
        }
    }
    return false;
}

void reduceMapInstantiations(QStringList& instantiations){
    if(instantiations.size()>2){
        QStringList parameterTypes;
        QString tmp;
        while(!instantiations.isEmpty()){
            if(tmp.isEmpty()){
                QString i1 = instantiations.takeFirst();
                auto opener = i1.count('<');
                auto closer = i1.count('>');
                if(opener == closer){
                    parameterTypes << i1;
                }else{
                    tmp = i1;
                }
            }else{
                tmp += QLatin1String(",") + instantiations.takeFirst();
                auto opener = tmp.count('<');
                auto closer = tmp.count('>');
                if(opener == closer){
                    parameterTypes << tmp;
                    tmp.clear();
                }
            }
        }
        if(!tmp.isEmpty()){
            parameterTypes << tmp;
        }
        instantiations = parameterTypes;
    }
    if(instantiations.size()==2){
        instantiations[0] = instantiations[0].trimmed();
        instantiations[1] = instantiations[1].trimmed();
    }
}

void insertHashFunctionByMetaType(const QtPrivate::QMetaTypeInterface * type, const QtJambiUtils::QHashFunction& fct){
    QtJambiStorage* storage = getQtJambiStorage();
    {
        QWriteLocker locker(storage->lock());
        storage->hashFunctionByMetaTypeHash().insert(type, fct);
    }
}

void insertHashFunctionByMetaType(const QtPrivate::QMetaTypeInterface * type, QtJambiUtils::QHashFunction&& fct){
    QtJambiStorage* storage = getQtJambiStorage();
    {
        QWriteLocker locker(storage->lock());
        storage->hashFunctionByMetaTypeHash().insert(type, std::move(fct));
    }
}

bool QtJambiTypeManager::isQtSubClass(JNIEnv *env, const QString &className, jobject classLoader) {
    return !closestQtSuperclass(env, className, classLoader).isEmpty();
}

bool QtJambiTypeManager::isQtClass(JNIEnv *env, const QString &className, jobject classLoader) {
    jclass clazz = JavaAPI::resolveClass(env, qPrintable(className), classLoader);
    return (clazz != nullptr && Java::QtJambi::ClassAnalyzerUtility::isGeneratedClass(env, clazz));
}

bool QtJambiTypeManager::isQObjectSubclass(JNIEnv *env, const QString &className, jobject classLoader) {
    jclass clazz = JavaAPI::resolveClass(env, qPrintable(className), classLoader);
    return (clazz != nullptr && Java::QtCore::QObject::isAssignableFrom(env, clazz));
}

QString QtJambiTypeManager::closestQtSuperclass(JNIEnv *env, const QString &className, jobject classLoader) {
    jclass clazz = JavaAPI::resolveClass(env, qPrintable(className), classLoader);
    const SuperTypeInfos& infos = SuperTypeInfos::fromClass(env, clazz);
    if (!infos.isEmpty())
        return QtJambiAPI::getClassName(env, infos.first().javaClass()).replace(QLatin1Char('.'), QLatin1Char('/'));
    else
        return QString();
}

QtJambiUtils::QHashFunction hashFunctionByMetaType(const QtPrivate::QMetaTypeInterface * metaType)
{
    if(metaType){
        QtJambiStorage* storage = getQtJambiStorage();
        {
            QReadLocker locker(storage->lock());
            return storage->hashFunctionByMetaTypeHash().value(metaType, QtJambiUtils::QHashFunction());
        }
    }
    return QtJambiUtils::QHashFunction();
}

QtJambiTypeManager::TypePattern QtJambiTypeManager::valueTypePattern(const QString &javaName) {
    static const QHash<QString, TypePattern> valueTypeHash{
        {"boolean",TypePattern(Boolean | Primitive)},
        {"byte",TypePattern(Byte | Primitive)},
        {"char",TypePattern(Char | Primitive)},
        {"double",TypePattern(Double | Primitive)},
        {"float",TypePattern(Float | Primitive)},
        {"int",TypePattern(Integer | Primitive)},
        {"long",TypePattern(Long | Primitive)},
        {"short",TypePattern(Short | Primitive)},
        {"void",TypePattern(Void | Primitive)},
        {"java/lang/Void",TypePattern(Void)},
        {"java/lang/Boolean",TypePattern(Boolean)},
        {"java/lang/Byte",TypePattern(Byte)},
        {"java/lang/Character",TypePattern(Char)},
        {"java/lang/Double",TypePattern(Double)},
        {"java/lang/Float",TypePattern(Float)},
        {"java/lang/Integer",TypePattern(Integer)},
        {"java/lang/Long",TypePattern(Long)},
        {"java/lang/Short",TypePattern(Short)},
        {"java/lang/String",TypePattern(String)}
    };
    quint64 type = Value;
    TypePattern t = valueTypeHash.value(javaName, None);
    return TypePattern(type | t);
}

QString QtJambiTypeManager::processInternalTypeName(const QString &_internalTypeName, PointerType &_pointerType) {
    if(_internalTypeName.startsWith("QSharedPointer<") && _internalTypeName.endsWith(">")){
        _pointerType = PointerType::SharedPointer;
        return _internalTypeName.mid(15, _internalTypeName.length() - 16)+"*";
    }else if(_internalTypeName.startsWith("QWeakPointer<") && _internalTypeName.endsWith(">")){
        _pointerType = PointerType::WeakPointer;
        return _internalTypeName.mid(13, _internalTypeName.length() - 14)+"*";
    }else if(_internalTypeName.startsWith("QScopedArrayPointer<") && _internalTypeName.endsWith(">")){
        _pointerType = PointerType::ScopedArrayPointer;
        return _internalTypeName.mid(20, _internalTypeName.length() - 21)+"*";
    }else if(_internalTypeName.startsWith("QScopedPointer<") && _internalTypeName.endsWith(">")){
        _pointerType = PointerType::ScopedPointer;
        return _internalTypeName.mid(15, _internalTypeName.length() - 16)+"*";
    }else if(_internalTypeName.startsWith("QPointer<") && _internalTypeName.endsWith(">")){
        _pointerType = PointerType::TrackingPointer;
        return _internalTypeName.mid(9, _internalTypeName.length() - 10)+"*";
    }else if(_internalTypeName.startsWith("std::shared_ptr<") && _internalTypeName.endsWith(">")){
        _pointerType = PointerType::shared_ptr;
        return _internalTypeName.mid(16, _internalTypeName.length() - 17)+"*";
    }else if(_internalTypeName.startsWith("std::weak_ptr<") && _internalTypeName.endsWith(">")){
        _pointerType = PointerType::weak_ptr;
        return _internalTypeName.mid(15, _internalTypeName.length() - 16)+"*";
    }else if(_internalTypeName.startsWith("std::unique_ptr<") && _internalTypeName.endsWith(">")){
        _pointerType = PointerType::unique_ptr;
        return _internalTypeName.mid(16, _internalTypeName.length() - 17)+"*";
    }else if(_internalTypeName.startsWith("std::initializer_list<") && _internalTypeName.endsWith(">")){
        _pointerType = PointerType::initializer_list;
        return _internalTypeName.mid(22, _internalTypeName.length() - 23);
    }
    return _internalTypeName;
}

/*!
    Parses a signature and returns a vector containing the names of the return and argument types
    in the signature. The first entry in the returned QVector will contain the name of the
    return type, the subsequent entries will contain the names of the argument types in order.
    If \a name is different from null, the string it points to will contain the name of the
    function after when the call to parseSignature() returns.
*/
QStringList QtJambiTypeManager::parseSignature(const QString &signature, QString *name) {
    QString tmp(signature.trimmed());
    tmp.replace(QLatin1Char('.'), QLatin1Char('/'));

    QString returnType = QLatin1String("void");
    auto idxFirstSpace = tmp.indexOf(QLatin1Char(' '));
    auto idxFirstParen = tmp.indexOf(QLatin1Char('('));

    // Does it have a return type?
    if (idxFirstSpace >= 0 && idxFirstSpace < idxFirstParen) {
        returnType = tmp.left(idxFirstSpace).trimmed();
        tmp = tmp.remove(0, idxFirstSpace + 1).trimmed();
    }

    QStringList returned;
    returned.append(returnType);

    idxFirstParen = tmp.indexOf(QLatin1Char('('));
    if (name != nullptr)
        *name = tmp.left(idxFirstParen).trimmed();

    tmp = tmp.remove(0, idxFirstParen + 1).trimmed();
    auto idxComma = tmp.indexOf(QLatin1Char(','));
    while (idxComma >= 0) {
        QString argType(tmp.left(idxComma).trimmed());
        if (argType.length() > 0)
            returned.append(argType);

        tmp = tmp.remove(0, idxComma + 1);
        idxComma = tmp.indexOf(QLatin1Char(','));
    }

    idxComma = tmp.indexOf(QLatin1Char(')'));
    QString argType(tmp.left(idxComma).trimmed());
    if (argType.length() > 0)
        returned.append(argType);

    return returned;
}

QList<const QtPrivate::QMetaTypeInterface *> registeredCustomMetaTypesForJavaClass(const QByteArray& javaClass);

EntryTypes getEntryType(const std::type_info& typeId);

QString QtJambiTypeManager::getInternalTypeName(JNIEnv* env, jclass externalClass, bool useNextSuperclass) {
    // First we try a simple look up. This will handle any type in the type system,
    // so: primitives, boxed primitives and direct mappings of Qt types
    if(Java::Runtime::Void::isPrimitiveType(env, externalClass))
        return QLatin1String("void");
    else if(Java::Runtime::Object::isSameClass(env, externalClass)
            || Java::QtCore::QVariant::isSameClass(env, externalClass)
            || Java::QtCore::QVariant$Null::isSameClass(env, externalClass))
        return QLatin1String("QVariant");
    else if(Java::Runtime::Byte::isPrimitiveType(env, externalClass) || Java::Runtime::Byte::isSameClass(env, externalClass))
        return QLatin1String(QMetaType::fromType<qint8>().name());
    else if(Java::Runtime::Short::isPrimitiveType(env, externalClass) || Java::Runtime::Short::isSameClass(env, externalClass))
        return QLatin1String(QMetaType::fromType<qint16>().name());
    else if(Java::Runtime::Integer::isPrimitiveType(env, externalClass) || Java::Runtime::Integer::isSameClass(env, externalClass))
        return QLatin1String(QMetaType::fromType<qint32>().name());
    else if(Java::Runtime::Long::isPrimitiveType(env, externalClass) || Java::Runtime::Long::isSameClass(env, externalClass))
        return QLatin1String(QMetaType::fromType<qint64>().name());
    else if(Java::Runtime::Float::isPrimitiveType(env, externalClass) || Java::Runtime::Float::isSameClass(env, externalClass))
        return QLatin1String(QMetaType::fromType<float>().name());
    else if(Java::Runtime::Double::isPrimitiveType(env, externalClass) || Java::Runtime::Double::isSameClass(env, externalClass))
        return QLatin1String(QMetaType::fromType<double>().name());
    else if(Java::Runtime::Character::isPrimitiveType(env, externalClass) || Java::Runtime::Character::isSameClass(env, externalClass))
        return QLatin1String(QMetaType::fromType<QChar>().name());
    else if(Java::Runtime::Boolean::isPrimitiveType(env, externalClass) || Java::Runtime::Boolean::isSameClass(env, externalClass))
        return QLatin1String(QMetaType::fromType<bool>().name());
    else if(Java::QtJambi::QNativePointer::isSameClass(env, externalClass))
        return QLatin1String(QMetaType::fromType<void*>().name());
    else if(Java::Runtime::Class::isArray(env, externalClass)){
        jclass componentType = Java::Runtime::Class::getComponentType(env, externalClass);
        if(Java::Runtime::Integer::isPrimitiveType(env, componentType)){
            return QLatin1String("JIntArrayWrapper");
        }else if(Java::Runtime::Short::isPrimitiveType(env, componentType)){
            return QLatin1String("JShortArrayWrapper");
        }else if(Java::Runtime::Byte::isPrimitiveType(env, componentType)){
            return QLatin1String("JByteArrayWrapper");
        }else if(Java::Runtime::Long::isPrimitiveType(env, componentType)){
            return QLatin1String("JLongArrayWrapper");
        }else if(Java::Runtime::Character::isPrimitiveType(env, componentType)){
            return QLatin1String("JCharArrayWrapper");
        }else if(Java::Runtime::Boolean::isPrimitiveType(env, componentType)){
            return QLatin1String("JBooleanArrayWrapper");
        }else if(Java::Runtime::Double::isPrimitiveType(env, componentType)){
            return QLatin1String("JDoubleArrayWrapper");
        }else if(Java::Runtime::Float::isPrimitiveType(env, componentType)){
            return QLatin1String("JFloatArrayWrapper");
        }else{
            return QLatin1String("JObjectArrayWrapper");
        }
    }else if(Java::QtCore::QMetaObject$Connection::isAssignableFrom(env, externalClass)){
        return QLatin1String("QMetaObject::Connection");
    }else if(Java::Runtime::ArrayList::isAssignableFrom(env, externalClass)){ // avoiding to return std::vector
        return QLatin1String("JCollectionWrapper");
    }else {
        QString externalTypeName = QtJambiAPI::getClassName(env, externalClass).replace(QLatin1Char('.'), QLatin1Char('/'));
        if(externalTypeName.endsWith("$ConcreteWrapper"))
            externalTypeName = externalTypeName.chopped(16);
        if(externalTypeName.endsWith("$Impl"))
            externalTypeName = externalTypeName.chopped(5);
        const std::type_info* typeId = getTypeByJavaName(externalTypeName);
        if(typeId){
            QMetaType metaType = registeredMetaType(*typeId);
            QString qtName = metaType.name();
            EntryTypes entryType = getEntryType(*typeId);
            if(entryType==EntryTypes::StdFunctionTypeInfo || entryType==EntryTypes::FunctionPointerTypeInfo){
                if(const char* functionalName = getQtNameByFunctional(qPrintable(externalTypeName))){
                    if(qtName!=functionalName){
                        QMetaType _metaType = QMetaType::fromName(functionalName);
                        if(_metaType==metaType){
                            const QtPrivate::QMetaTypeInterface *iface = metaType.iface();
                            QtPrivate::QMetaTypeInterface* metaTypeInterface = new QtPrivate::QMetaTypeInterface{
                                    iface->revision,
                                    iface->alignment,
                                    iface->size,
                                    iface->flags,
                                    0,
                                    iface->metaObjectFn,
                                    functionalName,
                                    iface->defaultCtr,
                                    iface->copyCtr,
                                    iface->moveCtr,
                                    iface->dtor,
                                    iface->equals,
                                    iface->lessThan,
                                    iface->debugStream,
                                    iface->dataStreamOut,
                                    iface->dataStreamIn,
                                    iface->legacyRegisterOp
                            };
                            _metaType = QMetaType(metaTypeInterface);
                            if(_metaType!=metaType){
                                metaType = _metaType;
                                registerJavaClassForCustomMetaType(env, metaType, JavaAPI::resolveClass(env, qPrintable(externalTypeName)));
                            }
                        }else{
                            metaType = _metaType;
                        }
                        qtName = QLatin1String(functionalName);
                    }
#ifdef Q_CC_MSVC
                }else{
                    qtName = qtName.replace("*__cdecl(", "*(")
                        .replace("&__cdecl(", "&(")
                        .replace(" __cdecl(", "(")
                        .replace("(__cdecl*)", "(*)")
                        .replace("(__cdecl&)", "(&)")
                        .replace("__cdecl", "")
                        .replace("(class ", "(")
                        .replace(",class ", ",")
                        .replace("(enum class ", "(")
                        .replace(",enum class ", ",")
                        .replace("(enum ", "(")
                        .replace(",enum ", ",")
                        .replace("(struct ", "(")
                        .replace(",struct ", ",");
#endif
                }
            }
            if(metaType.isValid()){
                switch(entryType){
                case EntryTypes::ObjectTypeInfo:
                case EntryTypes::QObjectTypeInfo:
                case EntryTypes::InterfaceTypeInfo:
                    if(!qtName.endsWith(QLatin1String("*")))
                        qtName += QLatin1String("*");
                    break;
                default:break;
                }
                return qtName;
            }else{
                qtName = getQtName(*typeId);
                switch(getEntryType(*typeId)){
                case EntryTypes::ObjectTypeInfo:
                case EntryTypes::QObjectTypeInfo:
                case EntryTypes::InterfaceTypeInfo:
                    qtName += QLatin1String("*");
                    break;
                default:break;
                }
            }
            return qtName;
        }else{
            QList<const QtPrivate::QMetaTypeInterface *> metaTypeIDs = registeredCustomMetaTypesForJavaClass(externalTypeName.toLatin1());
            if(metaTypeIDs.size()==1 && QMetaType(metaTypeIDs[0]).isValid()){
                return QLatin1String(QMetaType(metaTypeIDs[0]).name());
            }
            QString internalTypeName = QString(externalTypeName).replace(QLatin1String("/"), QLatin1String("::")).replace(QLatin1String("$"), QLatin1String("::"));
            if(QMetaType::type(qPrintable(internalTypeName))!=QMetaType::UnknownType){
                return internalTypeName;
            }
            internalTypeName += QLatin1String("*");
            if(QMetaType::type(qPrintable(internalTypeName))!=QMetaType::UnknownType){
                return internalTypeName;
            }
            internalTypeName = QString(externalTypeName).replace(QLatin1String("/"), QLatin1String("::")).replace(QLatin1String("$"), QLatin1String("::")) + QLatin1String("&");
            if(QMetaType::type(qPrintable(internalTypeName))!=QMetaType::UnknownType){
                return internalTypeName;
            }
            const SuperTypeInfos superType = SuperTypeInfos::fromClass(env, externalClass);
            if(useNextSuperclass && !superType.isEmpty()){
                QString qtName = QLatin1String(superType.first().qtName());
                switch(getEntryType(superType.first().typeId())){
                case EntryTypes::ObjectTypeInfo:
                case EntryTypes::QObjectTypeInfo:
                case EntryTypes::InterfaceTypeInfo:
                    return qtName + "*";
                default:
                    return qtName;
                }
            }
        }
    }
    return {};
}

QString QtJambiTypeManager::getInternalTypeName(JNIEnv* env, const QString &externalTypeName, jobject classLoader, bool useNextSuperclass) {
    jclass cls = nullptr;
    try{
        if(!externalTypeName.contains("*"))
            cls = JavaAPI::resolveClass(env, qPrintable(externalTypeName), classLoader);
    }catch(...){}
    if(cls){
        return getInternalTypeName(env, cls, useNextSuperclass);
    }else{
        return QString();
    }
}

QString QtJambiTypeManager::getExternalTypeName(JNIEnv *env, const QString &internalTypeName, const QMetaObject * metaObject, const QMetaType& metaType){
    QString result = getExternalTypeName(env, internalTypeName, metaType);
    if(result.isEmpty()){
        const QMetaObject * _metaObject = metaObject;
        while(_metaObject){
            result = getExternalTypeName(env, QLatin1String(_metaObject->className())+QLatin1String("::")+internalTypeName, metaType);
            if(!result.isEmpty()){
                break;
            }
            _metaObject = _metaObject->superClass();
        }
    }
    if(result.isEmpty()){
        if(metaType.isValid()){
            if(metaType.flags() & QMetaType::IsEnumeration){
                switch(metaType.sizeOf()){
                case 1:  return "io/qt/core/QMetaType$GenericByteEnumerator";
                case 2:  return "io/qt/core/QMetaType$GenericShortEnumerator";
                case 8:  return "io/qt/core/QMetaType$GenericLongEnumerator";
                default: return "io/qt/core/QMetaType$GenericEnumerator";
                }
            }else if(metaType.flags() & QMetaType::PointerToQObject
                    || metaType.flags() & QMetaType::SharedPointerToQObject
                    || metaType.flags() & QMetaType::WeakPointerToQObject){
                return QLatin1String("io/qt/core/QObject");
            }else if(metaType.flags() & QMetaType::PointerToGadget){
                return QLatin1String("io/qt/core/QMetaType$GenericGadget");
            }else if(isPointerType(internalTypeName)){
                return QLatin1String("io/qt/core/QMetaType$GenericObject");
            }else{
                return QLatin1String("io/qt/core/QMetaType$GenericValue");
            }
        }else if(isPointerType(internalTypeName)){
            return QLatin1String("io/qt/QNativePointer");
        }
    }
    return result;
}

jclass registeredJavaClassForCustomMetaType(QMetaType metaType);
const char* registeredClassNameForFunctionalMetaType(const QMetaType& metaType);

QString QtJambiTypeManager::getExternalTypeName(JNIEnv* environment, const QString &_internalTypeName, const QMetaType& metaType) {
    switch(metaType.id()){
    case QMetaType::Void: return "void";
    case QMetaType::SChar:
    case QMetaType::Char:
    case QMetaType::UChar: return "byte";
    case QMetaType::UShort:
    case QMetaType::Short: return "short";
    case QMetaType::Char32:
    case QMetaType::Int:
    case QMetaType::UInt: return "int";
    case QMetaType::Char16:
    case QMetaType::QChar: return "char";
    case QMetaType::Float: return "float";
    case QMetaType::Double: return "double";
    case QMetaType::QString: return "java/lang/String";
    case QMetaType::Long:
    case QMetaType::ULong:
        if(metaType.sizeOf()==4)
            return "int";
        else
            return "long";
    case QMetaType::LongLong:
    case QMetaType::ULongLong: return "long";
    case QMetaType::Bool: return "boolean";
    case QMetaType::QStringList: return "io/qt/core/QStringList";
    case QMetaType::QByteArrayList: return "io/qt/core/QList";
    case QMetaType::QVariantMap: return "io/qt/core/QMap";
    case QMetaType::QVariantHash: return "io/qt/core/QHash";
    case QMetaType::QVariantList: return "io/qt/core/QList";
    case QMetaType::QVariantPair: return "io/qt/core/QPair";
    case QMetaType::VoidStar: return "io/qt/QNativePointer";
    default:
        break;
    }
    if(_internalTypeName==QLatin1String("void*"))
        return "io/qt/QNativePointer";
    else if (_internalTypeName=="QVariant")
        return QLatin1String("java/lang/Object");
    else if (_internalTypeName=="JMapWrapper")
        return QLatin1String("java/util/Map");
    else if (_internalTypeName=="JCollectionWrapper")
        return QLatin1String("java/util/Collection");
    else if (_internalTypeName=="JIntArrayWrapper")
        return QLatin1String("[I");
    else if (_internalTypeName=="JByteArrayWrapper")
        return QLatin1String("[B");
    else if (_internalTypeName=="JLongArrayWrapper")
        return QLatin1String("[J");
    else if (_internalTypeName=="JShortArrayWrapper")
        return QLatin1String("[S");
    else if (_internalTypeName=="JCharArrayWrapper")
        return QLatin1String("[C");
    else if (_internalTypeName=="JBooleanArrayWrapper")
        return QLatin1String("[Z");
    else if (_internalTypeName=="JDoubleArrayWrapper")
        return QLatin1String("[D");
    else if (_internalTypeName=="JFloatArrayWrapper")
        return QLatin1String("[F");
    else if (_internalTypeName=="JObjectArrayWrapper")
        return QLatin1String("[Ljava/lang/Object;");
    else if (_internalTypeName=="JObjectWrapper")
        return QLatin1String("java/lang/Object");
    else if (_internalTypeName=="JQFlagsWrapper")
        return QLatin1String("io/qt/QFlags");
    else if ((_internalTypeName.startsWith("std::pair<") || _internalTypeName.startsWith("QPair<")) && _internalTypeName.endsWith(">"))
        return QLatin1String("io/qt/core/QPair");
    else if (_internalTypeName.startsWith("QMap<") && _internalTypeName.endsWith(">"))
        return QLatin1String("io/qt/core/QMap");
    else if (_internalTypeName.startsWith("QMultiMap<") && _internalTypeName.endsWith(">"))
        return QLatin1String("io/qt/core/QMultiMap");
    else if (_internalTypeName.startsWith("QHash<") && _internalTypeName.endsWith(">"))
        return QLatin1String("io/qt/core/QHash");
    else if (_internalTypeName.startsWith("QMultiHash<") && _internalTypeName.endsWith(">"))
        return QLatin1String("io/qt/core/QMultiHash");
    else if (_internalTypeName.startsWith("QLinkedList<") && _internalTypeName.endsWith(">"))
        return QLatin1String("java/util/LinkedList");
    else if (_internalTypeName.startsWith("QList<") && _internalTypeName.endsWith(">"))
        return QLatin1String("io/qt/core/QList");
    else if ((_internalTypeName.startsWith("QSpan<const ") || _internalTypeName.startsWith("std::span<const ")) && _internalTypeName.endsWith(">"))
        return QLatin1String("io/qt/core/QConstSpan");
    else if ((_internalTypeName.startsWith("QSpan<") || _internalTypeName.startsWith("std::span<")) && _internalTypeName.endsWith(">"))
        return QLatin1String("io/qt/core/QSpan");
    else if (_internalTypeName.startsWith("QSet<") && _internalTypeName.endsWith(">"))
        return QLatin1String("io/qt/core/QSet");
    else if (_internalTypeName.startsWith("QQueue<") && _internalTypeName.endsWith(">"))
        return QLatin1String("io/qt/core/QQueue");
    else if (_internalTypeName.startsWith("QVector<") && _internalTypeName.endsWith(">"))
        return QLatin1String("io/qt/core/QList");
    else if (_internalTypeName.startsWith("QStack<") && _internalTypeName.endsWith(">"))
        return QLatin1String("io/qt/core/QStack");
    else if (_internalTypeName.startsWith("std::vector<") && _internalTypeName.endsWith(">"))
        return QLatin1String("java/util/ArrayList");
    else if (_internalTypeName.startsWith("std::map<") && _internalTypeName.endsWith(">"))
        return QLatin1String("java/util/HashMap");
    else if (_internalTypeName.startsWith("std::set<") && _internalTypeName.endsWith(">"))
        return QLatin1String("java/util/HashSet");
    if(const char* name = registeredClassNameForFunctionalMetaType(metaType))
        return QLatin1String(name);
    const std::type_info* typeId = metaType.isValid() ? getTypeByMetaType(metaType) : nullptr;
    QString javaName;
    if(typeId){
        javaName = getJavaName(*typeId);
        if(!javaName.isEmpty())
            return javaName;
    }
    // Handle types in the type system, so: primitives and direct mappings.
    // Primitives need to be converted to objects for return types, so we
    // have to do some more work for them.
    PointerType pointerType = PointerType::NoPointer;
    QString internalTypeName = processInternalTypeName(_internalTypeName, pointerType);
    if(internalTypeName.startsWith("const ")){
        internalTypeName = internalTypeName.mid(6);
    }
    if(internalTypeName.endsWith("&")){
        internalTypeName.chop(1);
        internalTypeName = internalTypeName.trimmed();
    }
    bool isPointer = isPointerType(internalTypeName);
    if(internalTypeName.endsWith("*")){
        internalTypeName.chop(1);
        internalTypeName = internalTypeName.trimmed();
    }
    QStringList instantiations;
    {
        auto idx = internalTypeName.indexOf("<");
        if(idx>0 && internalTypeName.endsWith(">") && !internalTypeName.startsWith("QFlags<")){
            QString instantiationTypes = internalTypeName.mid(idx+1);
            instantiationTypes.chop(1);
            instantiations = instantiationTypes.split(",");
            internalTypeName = internalTypeName.left(idx);
        }
    }
    typeId = getTypeByQtName(internalTypeName);
    if(!typeId && !instantiations.isEmpty()){
        typeId = getTypeByQtName(QString(internalTypeName)+"<"+instantiations.join(",")+">");
    }
    if(typeId){
        javaName = getJavaName(*typeId);
        if(pointerType!=PointerType::NoPointer && pointerType!=PointerType::initializer_list){
            if(typeid_equals(*typeId, typeid(QString))){
                javaName = "io/qt/core/QString";
            }else if(typeid_equals(*typeId, typeid(QVariant))){
                javaName = "io/qt/core/QVariant";
            }
        }
    }
    if(javaName.isEmpty())
        javaName = QLatin1String(getInterface(qPrintable(internalTypeName)));// TODO adapt!!
    if(javaName.isEmpty()){
        if(!metaType.isValid()){
            QMetaType _metaType = QMetaType::fromName(internalTypeName.toLatin1());
            if(_metaType.isValid()){
                if(isPointer)
                    internalTypeName += "*";
                return getExternalTypeName(environment, internalTypeName, _metaType);
            }
        }
        if(metaType.isValid()
                && metaType.id()!=QMetaType::Nullptr
                && metaType.id()!=QMetaType::Void){
            if(jclass clazz = registeredJavaClassForCustomMetaType(metaType))
                return QtJambiAPI::getClassName(environment, clazz).replace('.', '/');
            if(QMetaType::canConvert(QMetaType(metaType), QMetaType(QMetaType::QVariantList))){
                javaName = "io/qt/core/QList";
            }else if(QMetaType::canConvert(QMetaType(metaType), QMetaType(QMetaType::QVariantMap))){
                javaName = "io/qt/core/QMap";
            }else if(QMetaType::canConvert(QMetaType(metaType), QMetaType(QMetaType::QVariantHash))){
                javaName = "io/qt/core/QHash";
            }
        }
    }
    if(!javaName.isEmpty() && pointerType==PointerType::initializer_list){
        //javaName = javaName+"[]";
        if(javaName=="int"){
            javaName = "[I";
        }else if(javaName=="byte"){
            javaName = "[B";
        }else if(javaName=="boolean"){
            javaName = "[Z";
        }else if(javaName=="long"){
            javaName = "[L";
        }else if(javaName=="short"){
            javaName = "[S";
        }else if(javaName=="char"){
            javaName = "[C";
        }else if(javaName=="double"){
            javaName = "[D";
        }else if(javaName=="float"){
            javaName = "[F";
        }else if(!javaName.startsWith("[")){
            javaName = "[L" + javaName + ";";
        }else{
            javaName = "[" + javaName;
        }
    }
    if(!javaName.isEmpty())
        return javaName;

    if(!metaType.isValid()){
        if(internalTypeName.contains("::") && javaName.isEmpty()){
            QStringList splitted = internalTypeName.split("::");
            QString enclosedTypeName = splitted.takeLast();
            QString enclosingTypeName = splitted.join("::");
            int enclosingMetaType = QMetaType::type(qPrintable(enclosingTypeName));
            const QMetaObject *metaObject = nullptr;
            if(enclosingMetaType != QMetaType::UnknownType){
                metaObject = QMetaType::metaObjectForType(enclosingMetaType);
            }
            if(!metaObject){
                metaObject = QtJambiMetaObject::findMetaObject(qPrintable(enclosingTypeName));
            }
            if(metaObject){
                QMetaEnum enumerator = metaObject->enumerator(metaObject->indexOfEnumerator(qPrintable(enclosedTypeName)));
                if(enumerator.isValid()){
                    return "int";
                }
            }
        }
    }else {
        QtJambiStorage* storage = getQtJambiStorage();
        QMetaEnum enumerator;
        {
            QReadLocker locker(storage->lock());
            enumerator = storage->metaEnumByMetaTypeHash().value(metaType.iface());
        }
        if(enumerator.isValid()){
            if(enumerator.isFlag()){
#if QT_VERSION >= QT_VERSION_CHECK(6, 9, 0)
                if(enumerator.is64Bit())
                    return "io/qt/core/QMetaType$GenericLongFlags";
#endif
                return "io/qt/core/QMetaType$GenericFlags";
            }else{
                switch(metaType.sizeOf()){
                case 1:  return "io/qt/core/QMetaType$GenericByteEnumerator";
                case 2:  return "io/qt/core/QMetaType$GenericShortEnumerator";
                case 8:  return "io/qt/core/QMetaType$GenericLongEnumerator";
                default: return "io/qt/core/QMetaType$GenericEnumerator";
                }
            }
        }
        if(!metaType.metaObject() && internalTypeName.contains("::")){
            QStringList splitted = internalTypeName.split("::");
            QString enclosedTypeName = splitted.takeLast();
            QString enclosingTypeName = splitted.join("::");
            int enclosingMetaType = QMetaType::type(qPrintable(enclosingTypeName));
            const QMetaObject *metaObject = nullptr;
            if(enclosingMetaType != QMetaType::UnknownType){
                metaObject = QMetaType::metaObjectForType(enclosingMetaType);
            }
            if(!metaObject){
                metaObject = QtJambiMetaObject::findMetaObject(qPrintable(enclosingTypeName));
            }
            if(metaObject){
                QMetaEnum enumerator = metaObject->enumerator(metaObject->indexOfEnumerator(qPrintable(enclosedTypeName)));
                if(enumerator.isValid()){
                    {
                        QWriteLocker locker(storage->lock());
                        storage->metaEnumByMetaTypeHash().insert(metaType.iface(), enumerator);
                    }
                    if(enumerator.isFlag()){
#if QT_VERSION >= QT_VERSION_CHECK(6, 9, 0)
                        if(enumerator.is64Bit())
                            return "io/qt/core/QMetaType$GenericLongFlags";
#endif
                        return "io/qt/core/QMetaType$GenericFlags";
                    }else{
                        switch(metaType.sizeOf()){
                        case 1:  return "io/qt/core/QMetaType$GenericByteEnumerator";
                        case 2:  return "io/qt/core/QMetaType$GenericShortEnumerator";
                        case 8:  return "io/qt/core/QMetaType$GenericLongEnumerator";
                        default: return "io/qt/core/QMetaType$GenericEnumerator";
                        }
                    }
                }
            }
        }
        if(const QMetaObject *metaObjectForParameter = metaType.metaObject()){
            if(metaObjectForParameter->className()==internalTypeName){
                metaObjectForParameter = metaObjectForParameter->superClass();
                while(metaObjectForParameter){
                    QString internalTypeName = QLatin1String(metaObjectForParameter->className());
                    if(isPointer)
                        internalTypeName += "*";
                    QString externalTypeName = getExternalTypeName(environment, internalTypeName);
                    if(!externalTypeName.isEmpty())
                        return externalTypeName;
                    metaObjectForParameter = metaObjectForParameter->superClass();
                }
            }else if(internalTypeName.contains("::")){
                QMetaEnum enumerator = metaObjectForParameter->enumerator(metaObjectForParameter->indexOfEnumerator(qPrintable(internalTypeName.split("::").last())));
                if(enumerator.isValid()){
                    {
                        QWriteLocker locker(storage->lock());
                        storage->metaEnumByMetaTypeHash().insert(metaType.iface(), enumerator);
                    }
                    if(enumerator.isFlag()){
#if QT_VERSION >= QT_VERSION_CHECK(6, 9, 0)
                        if(enumerator.is64Bit())
                            return "io/qt/core/QMetaType$GenericLongFlags";
#endif
                        return "io/qt/core/QMetaType$GenericFlags";
                    }else{
                        switch(metaType.sizeOf()){
                        case 1:  return "io/qt/core/QMetaType$GenericByteEnumerator";
                        case 2:  return "io/qt/core/QMetaType$GenericShortEnumerator";
                        case 8:  return "io/qt/core/QMetaType$GenericLongEnumerator";
                        default: return "io/qt/core/QMetaType$GenericEnumerator";
                        }
                    }
                }
            }
        }else{
            QString metaTypeName(QLatin1String(metaType.name()));
            if(metaTypeName!=internalTypeName && metaTypeName!=_internalTypeName){
                return getExternalTypeName(environment, metaTypeName, metaType);
            }
        }
        if(metaType.isValid()){
            if(metaType.flags() & QMetaType::IsEnumeration){
                switch(metaType.sizeOf()){
                case 1:  return "io/qt/core/QMetaType$GenericByteEnumerator";
                case 2:  return "io/qt/core/QMetaType$GenericShortEnumerator";
                case 8:  return "io/qt/core/QMetaType$GenericLongEnumerator";
                default: return "io/qt/core/QMetaType$GenericEnumerator";
                }
            }else if(metaType.flags() & QMetaType::PointerToQObject
                    || metaType.flags() & QMetaType::SharedPointerToQObject
                    || metaType.flags() & QMetaType::WeakPointerToQObject){
                return QLatin1String("io/qt/core/QObject");
            }else if(metaType.flags() & QMetaType::PointerToGadget){
                return QLatin1String("io/qt/core/QMetaType$GenericGadget");
            }else if(isPointer){
                return QLatin1String("io/qt/core/QMetaType$GenericObject");
            }else{
                return QLatin1String("io/qt/core/QMetaType$GenericValue");
            }
        }
    }
    return QString();
}

bool isPolymorphicBase(const char *lookup);

void QtJambiTypeManager::checkClassName(QObject* qobject, QString className, QString &_externalClassName, QString &_externalClassPackage){
    const QMetaObject *mo = QtJambiMetaObject::findFirstStaticMetaObject(qobject->metaObject());
    while (mo != nullptr) {
        // Never go further down the hierarchy than the requested class
        if (qstrcmp(qPrintable(className), mo->className()) == 0)
            break;

        QString javaName;
        if(const std::type_info* typeId = getTypeByMetaObject(mo)){
            javaName = QLatin1String(getJavaName(*typeId));
        }

        if (javaName.length() > 0) {
            QString javaClassName;
            QString javaPackageName;
            splitClassName(javaClassName, javaPackageName, javaName);

            // Make sure the requested class is a superclass of this one
            while (mo != nullptr && qstrcmp(mo->className(), qPrintable(className)) == 0)
                mo = mo->superClass();

            // If we found the requested class in the hierarchy, then choose the most
            // specialized class. Otherwise, just keep the requested class.
            if (mo != nullptr) {
                _externalClassName = javaClassName;
                _externalClassPackage = javaPackageName;
                mo = nullptr;
            }
        } else {
            mo = mo->superClass();
        }
    }
}

#if QT_VERSION < QT_VERSION_CHECK(6, 4, 0)
size_t qHash(const QMetaType& metaType, size_t seed = 0){
    return qHash(metaType.id(), seed);
}
#endif

size_t computeHash(JNIEnv* env,
                   const QString &internalTypeName,
                   const QMetaType& internalMetaType,
                   jclass externalClass,
                   bool allowValuePointers){
    if(internalMetaType.isValid())
        return qHashMulti(0, internalMetaType, externalClass ? Java::Runtime::Object::hashCode(env, externalClass) : 0, allowValuePointers);
    else
        return qHashMulti(0, internalTypeName, externalClass ? Java::Runtime::Object::hashCode(env, externalClass) : 0, allowValuePointers);
}

size_t computeHash(JNIEnv* env, jclass externalClass, const QString &internalTypeName, const QMetaType& internalMetaType) {
    if(internalMetaType.isValid())
        return qHashMulti(0, internalMetaType, externalClass ? Java::Runtime::Object::hashCode(env, externalClass) : 0);
    else
        return qHashMulti(0, internalTypeName, externalClass ? Java::Runtime::Object::hashCode(env, externalClass) : 0);
}

QtJambiUtils::InternalToExternalConverter QtJambiTypeManager::tryGetInternalToExternalConverter(
        JNIEnv* env,
        const QString &internalTypeName,
        const QMetaType& internalMetaType,
        jclass externalClass,
        bool allowValuePointers){
    QtJambiStorage* storage = getQtJambiStorage();
    {
        size_t key = computeHash(env, internalTypeName, internalMetaType, externalClass, allowValuePointers);
        {
            QReadLocker locker(storage->lock());
            if(storage->internalToExternalConverters().contains(key))
                return storage->internalToExternalConverters()[key];
        }
        if(internalMetaType.isValid() && !QMetaType(internalMetaType).iface()->metaObjectFn){
            QtJambiUtils::InternalToExternalConverter converter = getInternalToExternalConverterImpl(
                        env,
                        internalTypeName,
                        internalMetaType,
                        externalClass,
                        allowValuePointers
                    );
            Q_ASSERT(converter);
            QWriteLocker locker(storage->lock());
            if(storage->internalToExternalConverters().contains(key))
                return storage->internalToExternalConverters()[key];
            storage->internalToExternalConverters()[key] = converter;
            return converter;
        }
    }
    return QtJambiUtils::InternalToExternalConverter();
}

QtJambiUtils::InternalToExternalConverter QtJambiTypeManager::getInternalToExternalConverter(
                               JNIEnv* env,
                               const QString &internalTypeName,
                               const QMetaType& internalMetaType,
                               jclass externalClass,
                               bool allowValuePointers){
    QtJambiStorage* storage = getQtJambiStorage();
    size_t key = computeHash(env, internalTypeName, internalMetaType, externalClass, allowValuePointers);
    {
        QReadLocker locker(storage->lock());
        if(storage->internalToExternalConverters().contains(key))
            return storage->internalToExternalConverters()[key];
    }
    QtJambiUtils::InternalToExternalConverter converter = getInternalToExternalConverterImpl(
                env,
                internalTypeName,
                internalMetaType,
                externalClass,
                allowValuePointers
            );
    Q_ASSERT(converter);
    {
        QWriteLocker locker(storage->lock());
        if(storage->internalToExternalConverters().contains(key))
            return storage->internalToExternalConverters()[key];
        storage->internalToExternalConverters()[key] = converter;
    }
    return converter;
}

QtJambiUtils::InternalToExternalConverter QtJambiTypeManager::getInternalToExternalConverterImpl(
                               JNIEnv* _env,
                               QString internalTypeName,
                               const QMetaType& internalMetaType,
                               jclass externalClass,
                               bool allowValuePointers){
    PointerType pointerType = PointerType::NoPointer;
    internalTypeName = processInternalTypeName(internalTypeName, pointerType);

    if (internalMetaType.id() == QMetaType::QVariant
            || (pointerType!=PointerType::NoPointer && internalTypeName=="QVariant*")) {
        switch(pointerType){
        case PointerType::NoPointer:
            return [](JNIEnv* env, QtJambiScope*, const void* in, jvalue& p, bool)->bool{
                        p.l = QtJambiAPI::convertQVariantToJavaObject(env, *reinterpret_cast<const QVariant*>(in));
                        return true;
                    };
            ///if(Java::QtCore::QVariant::isSameClass(_env,externalClass)){
        case PointerType::SharedPointer:
            return [](JNIEnv* env, QtJambiScope*, const void* in, jvalue& p, bool)->bool{
                const QSharedPointer<QVariant>* ptr = reinterpret_cast<const QSharedPointer<QVariant>*>(in);
                if(ptr){
                    p.l = QtJambiAPI::convertSmartPointerToJavaObject(env, *ptr);
                }
                return true;
            };
        case PointerType::shared_ptr:
            return [](JNIEnv* env, QtJambiScope*, const void* in, jvalue& p, bool)->bool{
                const std::shared_ptr<QVariant>* ptr = reinterpret_cast<const std::shared_ptr<QVariant>*>(in);
                if(ptr){
                    p.l = QtJambiAPI::convertSmartPointerToJavaObject(env, *ptr);
                }
                return true;
            };
        case PointerType::WeakPointer:
            return [](JNIEnv* env, QtJambiScope*, const void* in, jvalue& p, bool)->bool{
                const QWeakPointer<QVariant>* ptr = reinterpret_cast<const QWeakPointer<QVariant>*>(in);
                if(ptr){
                    if(const QSharedPointer<QVariant> _ptr{*ptr})
                        p.l = QtJambiAPI::convertSmartPointerToJavaObject(env, _ptr);
                }
                return true;
            };
        case PointerType::weak_ptr:
            return [](JNIEnv* env, QtJambiScope*, const void* in, jvalue& p, bool)->bool{
                const std::weak_ptr<QVariant>* ptr = reinterpret_cast<const std::weak_ptr<QVariant>*>(in);
                if(ptr){
                    if(const std::shared_ptr<QVariant> _ptr{*ptr})
                        p.l = QtJambiAPI::convertSmartPointerToJavaObject(env, _ptr);
                }
                return true;
            };
        default:
            return ParameterTypeInfo::default_internalToExternalConverter();
        }
    }else if ( internalMetaType.id() == QMetaType::Nullptr
               || internalMetaType.id() == QMetaType::Void
               || Java::Runtime::Void::isSameClass(_env, externalClass)
               || Java::Runtime::Void::isPrimitiveType(_env, externalClass)) {
        return [](JNIEnv*, QtJambiScope*, const void*, jvalue& p, bool)->bool{
            p.l = nullptr;
            return true;
        };
    }else if(_env->IsSameObject(externalClass, nullptr)){
        return [](JNIEnv*, QtJambiScope*, const void*, jvalue&, bool)->bool{
            return false;
        };
    }else if(JObjectValueWrapper::isValueType(internalMetaType)){
        return [](JNIEnv* env, QtJambiScope*, const void* in, jvalue& p, bool)->bool{
            p.l = reinterpret_cast<const JObjectValueWrapper*>(in)->value(env);
            return true;
        };
    } else if (internalMetaType==registeredMetaType(typeid(JObjectWrapper))
               || internalMetaType==registeredMetaType(typeid(JCollectionWrapper))
               || internalMetaType==registeredMetaType(typeid(JMapWrapper))
               || internalMetaType==registeredMetaType(typeid(JObjectArrayWrapper))
               || internalMetaType==registeredMetaType(typeid(JIntArrayWrapper))
               || internalMetaType==registeredMetaType(typeid(JLongArrayWrapper))
               || internalMetaType==registeredMetaType(typeid(JShortArrayWrapper))
               || internalMetaType==registeredMetaType(typeid(JByteArrayWrapper))
               || internalMetaType==registeredMetaType(typeid(JBooleanArrayWrapper))
               || internalMetaType==registeredMetaType(typeid(JCharArrayWrapper))
               || internalMetaType==registeredMetaType(typeid(JFloatArrayWrapper))
               || internalMetaType==registeredMetaType(typeid(JDoubleArrayWrapper))
               || isJObjectWrappedMetaType(internalMetaType)
               || isNativeWrapperMetaType(internalMetaType)
               ) {
        return [](JNIEnv* env, QtJambiScope*, const void* in, jvalue& p, bool)->bool{
            p.l = reinterpret_cast<const JObjectWrapper *>(in)->object(env);
            return true;
        };
    }else if(pointerType!=PointerType::NoPointer
             && (internalTypeName=="JObjectWrapper*"
                 || internalTypeName=="JCollectionWrapper*"
                 || internalTypeName=="JMapWrapper*"
                 || internalTypeName=="JObjectArrayWrapper*"
                 || internalTypeName=="JIntArrayWrapper*"
                 || internalTypeName=="JLongArrayWrapper*"
                 || internalTypeName=="JByteArrayWrapper*"
                 || internalTypeName=="JBooleanArrayWrapper*"
                 || internalTypeName=="JCharArrayWrapper*"
                 || internalTypeName=="JCharArrayWrapper*"
                 || internalTypeName=="JDoubleArrayWrapper*"
                 || (internalTypeName.startsWith("JObjectValueWrapper<") && internalTypeName.endsWith(">*"))
                 || (internalTypeName.startsWith("JObjectWrapper<") && internalTypeName.endsWith(">*")))){
        switch(pointerType){
        case PointerType::SharedPointer:
            return [](JNIEnv* env, QtJambiScope*, const void* in, jvalue& p, bool)->bool{
                const QSharedPointer<JObjectWrapper>* ptr = reinterpret_cast<const QSharedPointer<JObjectWrapper>*>(in);
                if(ptr)
                    p.l = (*ptr)->object(env);
                return true;
            };
        case PointerType::shared_ptr:
            return [](JNIEnv* env, QtJambiScope*, const void* in, jvalue& p, bool)->bool{
                const std::shared_ptr<JObjectWrapper>* ptr = reinterpret_cast<const std::shared_ptr<JObjectWrapper>*>(in);
                if(ptr)
                    p.l = (*ptr)->object(env);
                return true;
            };
        case PointerType::WeakPointer:
            return [](JNIEnv* env, QtJambiScope*, const void* in, jvalue& p, bool)->bool{
                const QWeakPointer<JObjectWrapper>* ptr = reinterpret_cast<const QWeakPointer<JObjectWrapper>*>(in);
                if(ptr){
                    if(const QSharedPointer<JObjectWrapper> _ptr{*ptr})
                        p.l = _ptr->object(env);
                }
                return true;
            };
        case PointerType::weak_ptr:
            return [](JNIEnv* env, QtJambiScope*, const void* in, jvalue& p, bool)->bool{
                const std::weak_ptr<JObjectWrapper>* ptr = reinterpret_cast<const std::weak_ptr<JObjectWrapper>*>(in);
                if(ptr){
                    if(const std::shared_ptr<JObjectWrapper> _ptr{*ptr})
                        p.l = _ptr->object(env);
                }
                return true;
            };
        default:
            return ParameterTypeInfo::default_internalToExternalConverter();
        }
    }
    bool isArrayClass = externalClass ? Java::Runtime::Class::isArray(_env, externalClass) : false;

    if(isArrayClass && (pointerType==PointerType::initializer_list || internalTypeName.startsWith("initializer_list<"))){
        jclass componentClass = Java::Runtime::Class::getComponentType(_env, externalClass);
        QString javaTypeName = QtJambiAPI::getClassName(_env, jclass(componentClass)).replace(".", "/");
        Q_UNUSED(javaTypeName)
        QString _internalTypeName = internalTypeName;
        if(_internalTypeName.startsWith("initializer_list<"))
            _internalTypeName = _internalTypeName.mid(22, _internalTypeName.length() - 23);
        if(const std::type_info* typeId = getTypeByQtName(_internalTypeName)){
            QMetaType metaType = QMetaType::fromName(qPrintable(_internalTypeName));
            if(!metaType.isValid()){
                metaType = registeredMetaType(*typeId);
            }
            QPair<size_t,size_t> sizeAlign = getValueSizeAndAlignment(*typeId);
            if(sizeAlign.first>0){
                QMetaType _metaType(metaType);
                QtJambiUtils::InternalToExternalConverter internalToExternalComponentConverter = getInternalToExternalConverter(
                                                                _env,
                                                                _internalTypeName,
                                                                _metaType,
                                                                componentClass,
                                                                true);
                if(Java::Runtime::Byte::isPrimitiveType(_env,componentClass)){
                    return [internalToExternalComponentConverter, size = sizeAlign.first](JNIEnv* env, QtJambiScope* scope, const void* in, jvalue& p, bool forceBoxedType)->bool{
                        const std::initializer_list<char>* il = reinterpret_cast<const std::initializer_list<char>*>(in);
                        const char* begin = il->begin();
                        bool result = true;
                        jbyteArray array = env->NewByteArray(jsize(il->size()/size));
                        jbyte* elements = env->GetByteArrayElements(array, nullptr);
                        jsize j=0;
                        for (size_t i=0; i<il->size(); i+=size) {
                            jvalue el;
                            el.l = nullptr;
                            result &= internalToExternalComponentConverter(env, scope, &begin[i], el, forceBoxedType);
                            elements[j++] = el.b;
                        }
                        env->ReleaseByteArrayElements(array, elements, 0);
                        p.l = array;
                        return result;
                    };
                }else if(Java::Runtime::Short::isPrimitiveType(_env,componentClass)){
                    return [internalToExternalComponentConverter, size = sizeAlign.first](JNIEnv* env, QtJambiScope* scope, const void* in, jvalue& p, bool forceBoxedType)->bool{
                        const std::initializer_list<char>* il = reinterpret_cast<const std::initializer_list<char>*>(in);
                        const char* begin = il->begin();
                        bool result = true;
                        jshortArray array = env->NewShortArray(jsize(il->size()/size));
                        jshort* elements = env->GetShortArrayElements(array, nullptr);
                        jsize j=0;
                        for (size_t i=0; i<il->size(); i+=size) {
                            jvalue el;
                            el.l = nullptr;
                            result &= internalToExternalComponentConverter(env, scope, &begin[i], el, forceBoxedType);
                            elements[j++] = el.s;
                        }
                        env->ReleaseShortArrayElements(array, elements, 0);
                        p.l = array;
                        return result;
                    };
                }else if(Java::Runtime::Integer::isPrimitiveType(_env,componentClass)){
                    return [internalToExternalComponentConverter, size = sizeAlign.first](JNIEnv* env, QtJambiScope* scope, const void* in, jvalue& p, bool forceBoxedType)->bool{
                        const std::initializer_list<char>* il = reinterpret_cast<const std::initializer_list<char>*>(in);
                        const char* begin = il->begin();
                        bool result = true;
                        jintArray array = env->NewIntArray(jsize(il->size()/size));
                        jint* elements = env->GetIntArrayElements(array, nullptr);
                        jsize j=0;
                        for (size_t i=0; i<il->size(); i+=size) {
                            jvalue el;
                            el.l = nullptr;
                            result &= internalToExternalComponentConverter(env, scope, &begin[i], el, forceBoxedType);
                            elements[j++] = el.i;
                        }
                        env->ReleaseIntArrayElements(array, elements, 0);
                        p.l = array;
                        return result;
                    };
                }else if(Java::Runtime::Long::isPrimitiveType(_env,componentClass)){
                    return [internalToExternalComponentConverter, size = sizeAlign.first](JNIEnv* env, QtJambiScope* scope, const void* in, jvalue& p, bool forceBoxedType)->bool{
                        const std::initializer_list<char>* il = reinterpret_cast<const std::initializer_list<char>*>(in);
                        const char* begin = il->begin();
                        bool result = true;
                        jlongArray array = env->NewLongArray(jsize(il->size()/size));
                        jlong* elements = env->GetLongArrayElements(array, nullptr);
                        jsize j=0;
                        for (size_t i=0; i<il->size(); i+=size) {
                            jvalue el;
                            el.l = nullptr;
                            result &= internalToExternalComponentConverter(env, scope, &begin[i], el, forceBoxedType);
                            elements[j++] = el.j;
                        }
                        env->ReleaseLongArrayElements(array, elements, 0);
                        p.l = array;
                        return result;
                    };
                }else if(Java::Runtime::Float::isPrimitiveType(_env,componentClass)){
                    return [internalToExternalComponentConverter, size = sizeAlign.first](JNIEnv* env, QtJambiScope* scope, const void* in, jvalue& p, bool forceBoxedType)->bool{
                        const std::initializer_list<char>* il = reinterpret_cast<const std::initializer_list<char>*>(in);
                        const char* begin = il->begin();
                        bool result = true;
                        jfloatArray array = env->NewFloatArray(jsize(il->size()/size));
                        jfloat* elements = env->GetFloatArrayElements(array, nullptr);
                        jsize j=0;
                        for (size_t i=0; i<il->size(); i+=size) {
                            jvalue el;
                            el.l = nullptr;
                            result &= internalToExternalComponentConverter(env, scope, &begin[i], el, forceBoxedType);
                            elements[j++] = el.f;
                        }
                        env->ReleaseFloatArrayElements(array, elements, 0);
                        p.l = array;
                        return result;
                    };
                }else if(Java::Runtime::Double::isPrimitiveType(_env,componentClass)){
                    return [internalToExternalComponentConverter, size = sizeAlign.first](JNIEnv* env, QtJambiScope* scope, const void* in, jvalue& p, bool forceBoxedType)->bool{
                        const std::initializer_list<char>* il = reinterpret_cast<const std::initializer_list<char>*>(in);
                        const char* begin = il->begin();
                        bool result = true;
                        jdoubleArray array = env->NewDoubleArray(jsize(il->size()/size));
                        jdouble* elements = env->GetDoubleArrayElements(array, nullptr);
                        jsize j=0;
                        for (size_t i=0; i<il->size(); i+=size) {
                            jvalue el;
                            el.l = nullptr;
                            result &= internalToExternalComponentConverter(env, scope, &begin[i], el, forceBoxedType);
                            elements[j++] = el.d;
                        }
                        env->ReleaseDoubleArrayElements(array, elements, 0);
                        p.l = array;
                        return result;
                    };
                }else if(Java::Runtime::Character::isPrimitiveType(_env,componentClass)){
                    return [internalToExternalComponentConverter, size = sizeAlign.first](JNIEnv* env, QtJambiScope* scope, const void* in, jvalue& p, bool forceBoxedType)->bool{
                        const std::initializer_list<char>* il = reinterpret_cast<const std::initializer_list<char>*>(in);
                        const char* begin = il->begin();
                        bool result = true;
                        jcharArray array = env->NewCharArray(jsize(il->size()/size));
                        jchar* elements = env->GetCharArrayElements(array, nullptr);
                        jsize j=0;
                        for (size_t i=0; i<il->size(); i+=size) {
                            jvalue el;
                            el.l = nullptr;
                            result &= internalToExternalComponentConverter(env, scope, &begin[i], el, forceBoxedType);
                            elements[j++] = el.c;
                        }
                        env->ReleaseCharArrayElements(array, elements, 0);
                        p.l = array;
                        return result;
                    };
                }else if(Java::Runtime::Boolean::isPrimitiveType(_env,componentClass)){
                    return [internalToExternalComponentConverter, size = sizeAlign.first](JNIEnv* env, QtJambiScope* scope, const void* in, jvalue& p, bool forceBoxedType)->bool{
                        const std::initializer_list<char>* il = reinterpret_cast<const std::initializer_list<char>*>(in);
                        const char* begin = il->begin();
                        bool result = true;
                        jbooleanArray array = env->NewBooleanArray(jsize(il->size()/size));
                        jboolean* elements = env->GetBooleanArrayElements(array, nullptr);
                        jsize j=0;
                        for (size_t i=0; i<il->size(); i+=size) {
                            jvalue el;
                            el.l = nullptr;
                            result &= internalToExternalComponentConverter(env, scope, &begin[i], el, forceBoxedType);
                            elements[j++] = el.z;
                        }
                        env->ReleaseBooleanArrayElements(array, elements, 0);
                        p.l = array;
                        return result;
                    };
                }else{
                    componentClass = getGlobalClassRef(_env, componentClass);
                    return [internalToExternalComponentConverter, size = sizeAlign.first, componentClass](JNIEnv* env, QtJambiScope* scope, const void* in, jvalue& p, bool forceBoxedType)->bool{
                        const std::initializer_list<char>* il = reinterpret_cast<const std::initializer_list<char>*>(in);
                        const char* begin = il->begin();
                        bool result = true;
                        jobjectArray array = env->NewObjectArray(jsize(il->size()/size), componentClass, nullptr);
                        jsize j=0;
                        for (size_t i=0; i<il->size(); i+=size) {
                            jvalue el;
                            el.l = nullptr;
                            result &= internalToExternalComponentConverter(env, scope, &begin[i], el, forceBoxedType);
                            env->SetObjectArrayElement(array, j++, el.l);
                        }
                        p.l = array;
                        return result;
                    };
                }
            }
        }
    }else if(Java::Runtime::Integer::isPrimitiveType(_env,externalClass) || Java::Runtime::Integer::isSameClass(_env,externalClass)){
        switch(pointerType){
        case PointerType::NoPointer:{
            return [](JNIEnv* env, QtJambiScope*, const void* in, jvalue& p, bool forceBoxedType)->bool{
                memcpy(&p.i, in, 4);
                if(forceBoxedType){
                    p.l = QtJambiAPI::toJavaIntegerObject(env, p.i);
                }
                return true;
            };
        }break;
        case PointerType::SharedPointer:{
            return [](JNIEnv* env, QtJambiScope*, const void* in, jvalue& p, bool forceBoxedType)->bool{
                const QSharedPointer<jint>* ptr = reinterpret_cast<const QSharedPointer<jint>*>(in);
                if(ptr && *ptr){
                    p.i = *ptr->get();
                    if(forceBoxedType){
                        p.l = QtJambiAPI::toJavaIntegerObject(env, p.i);
                    }
                }
                return true;
            };
        }break;
        case PointerType::shared_ptr:{
            return [](JNIEnv* env, QtJambiScope*, const void* in, jvalue& p, bool forceBoxedType)->bool{
                const std::shared_ptr<jint>* ptr = reinterpret_cast<const std::shared_ptr<jint>*>(in);
                if(ptr && *ptr){
                    p.i = *ptr->get();
                    if(forceBoxedType){
                        p.l = QtJambiAPI::toJavaIntegerObject(env, p.i);
                    }
                }
                return true;
            };
        }break;
        default:
            return ParameterTypeInfo::default_internalToExternalConverter();
        }
    }else if(Java::Runtime::Long::isPrimitiveType(_env,externalClass) || Java::Runtime::Long::isSameClass(_env,externalClass)){
        switch(pointerType){
        case PointerType::NoPointer:{
            return [](JNIEnv* env, QtJambiScope*, const void* in, jvalue& p, bool forceBoxedType)->bool{
                memcpy(&p.j, in, 8);
                if(forceBoxedType){
                    p.l = QtJambiAPI::toJavaLongObject(env, p.j);
                }
                return true;
            };
        }break;
        case PointerType::SharedPointer:{
            return [](JNIEnv* env, QtJambiScope*, const void* in, jvalue& p, bool forceBoxedType)->bool{
                const QSharedPointer<jlong>* ptr = reinterpret_cast<const QSharedPointer<jlong>*>(in);
                if(ptr && *ptr){
                    p.j = *ptr->get();
                    if(forceBoxedType){
                        p.l = QtJambiAPI::toJavaLongObject(env, p.j);
                    }
                }
                return true;
            };
        }break;
        case PointerType::shared_ptr:{
            return [](JNIEnv* env, QtJambiScope*, const void* in, jvalue& p, bool forceBoxedType)->bool{
                const std::shared_ptr<jlong>* ptr = reinterpret_cast<const std::shared_ptr<jlong>*>(in);
                if(ptr && *ptr){
                    p.j = *ptr->get();
                    if(forceBoxedType){
                        p.l = QtJambiAPI::toJavaLongObject(env, p.j);
                    }
                }
                return true;
            };
        }break;
        default:
            return ParameterTypeInfo::default_internalToExternalConverter();
        }
    }else if(Java::Runtime::Short::isPrimitiveType(_env,externalClass) || Java::Runtime::Short::isSameClass(_env,externalClass)){
        switch(pointerType){
        case PointerType::NoPointer:{
            return [](JNIEnv* env, QtJambiScope*, const void* in, jvalue& p, bool forceBoxedType)->bool{
                memcpy(&p.s, in, 2);
                if(forceBoxedType){
                    p.l = QtJambiAPI::toJavaShortObject(env, p.s);
                }
                return true;
            };
        }break;
        case PointerType::SharedPointer:{
            return [](JNIEnv* env, QtJambiScope*, const void* in, jvalue& p, bool forceBoxedType)->bool{
                const QSharedPointer<jshort>* ptr = reinterpret_cast<const QSharedPointer<jshort>*>(in);
                if(ptr && *ptr){
                    p.s = *ptr->get();
                    if(forceBoxedType){
                        p.l = QtJambiAPI::toJavaShortObject(env, p.s);
                    }
                }
                return true;
            };
        }break;
        case PointerType::shared_ptr:{
            return [](JNIEnv* env, QtJambiScope*, const void* in, jvalue& p, bool forceBoxedType)->bool{
                const std::shared_ptr<jshort>* ptr = reinterpret_cast<const std::shared_ptr<jshort>*>(in);
                if(ptr && *ptr){
                    p.s = *ptr->get();
                    if(forceBoxedType){
                        p.l = QtJambiAPI::toJavaShortObject(env, p.s);
                    }
                }
                return true;
            };
        }break;
        default:
            return ParameterTypeInfo::default_internalToExternalConverter();
        }
    }else if(Java::Runtime::Byte::isPrimitiveType(_env,externalClass) || Java::Runtime::Byte::isSameClass(_env,externalClass)){
        switch(pointerType){
        case PointerType::NoPointer:{
            return [](JNIEnv* env, QtJambiScope*, const void* in, jvalue& p, bool forceBoxedType)->bool{
                memcpy(&p.b, in, 1);
                if(forceBoxedType){
                    p.l = QtJambiAPI::toJavaByteObject(env, p.b);
                }
                return true;
            };
        }break;
        case PointerType::SharedPointer:{
            return [](JNIEnv* env, QtJambiScope*, const void* in, jvalue& p, bool forceBoxedType)->bool{
                const QSharedPointer<jbyte>* ptr = reinterpret_cast<const QSharedPointer<jbyte>*>(in);
                if(ptr && *ptr){
                    p.b = *ptr->get();
                    if(forceBoxedType){
                        p.l = QtJambiAPI::toJavaByteObject(env, p.b);
                    }
                }
                return true;
            };
        }break;
        case PointerType::shared_ptr:{
            return [](JNIEnv* env, QtJambiScope*, const void* in, jvalue& p, bool forceBoxedType)->bool{
                const std::shared_ptr<jbyte>* ptr = reinterpret_cast<const std::shared_ptr<jbyte>*>(in);
                if(ptr && *ptr){
                    p.b = *ptr->get();
                    if(forceBoxedType){
                        p.l = QtJambiAPI::toJavaLongObject(env, p.b);
                    }
                }
                return true;
            };
        }break;
        default:
            return ParameterTypeInfo::default_internalToExternalConverter();
        }
    }else if(Java::Runtime::Boolean::isPrimitiveType(_env,externalClass) || Java::Runtime::Boolean::isSameClass(_env,externalClass)){
        switch(pointerType){
        case PointerType::NoPointer:{
            return [](JNIEnv* env, QtJambiScope*, const void* in, jvalue& p, bool forceBoxedType)->bool{
                memcpy(&p.z, in, sizeof(bool));
                if(forceBoxedType){
                    p.l = QtJambiAPI::toJavaBooleanObject(env, p.z);
                }
                return true;
            };
        }break;
        case PointerType::SharedPointer:{
            return [](JNIEnv* env, QtJambiScope*, const void* in, jvalue& p, bool forceBoxedType)->bool{
                const QSharedPointer<bool>* ptr = reinterpret_cast<const QSharedPointer<bool>*>(in);
                if(ptr && *ptr){
                    p.z = *ptr->get();
                    if(forceBoxedType){
                        p.l = QtJambiAPI::toJavaBooleanObject(env, p.z);
                    }
                }
                return true;
            };
        }break;
        case PointerType::shared_ptr:{
            return [](JNIEnv* env, QtJambiScope*, const void* in, jvalue& p, bool forceBoxedType)->bool{
                const std::shared_ptr<bool>* ptr = reinterpret_cast<const std::shared_ptr<bool>*>(in);
                if(ptr && *ptr){
                    p.z = *ptr->get();
                    if(forceBoxedType){
                        p.l = QtJambiAPI::toJavaBooleanObject(env, p.z);
                    }
                }
                return true;
            };
        }break;
        default:
            return ParameterTypeInfo::default_internalToExternalConverter();
        }
    }else if(Java::Runtime::Character::isPrimitiveType(_env,externalClass) || Java::Runtime::Character::isSameClass(_env,externalClass)){
        switch(pointerType){
        case PointerType::NoPointer:{
            if(internalMetaType.id()==QMetaType::QChar){
                return [](JNIEnv* env, QtJambiScope*, const void* in, jvalue& p, bool forceBoxedType)->bool{
                    const QChar* qchr = reinterpret_cast<const QChar*>(in);
                    p.c = qchr->unicode();
                    if(forceBoxedType){
                        p.l = QtJambiAPI::toJavaCharacterObject(env, p.c);
                    }
                    return true;
                };
            }else{
                return [](JNIEnv* env, QtJambiScope*, const void* in, jvalue& p, bool forceBoxedType)->bool{
                    memcpy(&p.c, in, 2);
                    if(forceBoxedType){
                        p.l = QtJambiAPI::toJavaCharacterObject(env, p.c);
                    }
                    return true;
                };
            }
        }break;
        case PointerType::SharedPointer:{
            if(internalMetaType.id()==QMetaType::QChar){
                return [](JNIEnv* env, QtJambiScope*, const void* in, jvalue& p, bool forceBoxedType)->bool{
                    const QSharedPointer<QChar>* ptr = reinterpret_cast<const QSharedPointer<QChar>*>(in);
                    if(ptr && *ptr){
                        p.c = (*ptr)->unicode();
                        if(forceBoxedType){
                            p.l = QtJambiAPI::toJavaCharacterObject(env, p.c);
                        }
                    }
                    return true;
                };
            }else{
                return [](JNIEnv* env, QtJambiScope*, const void* in, jvalue& p, bool forceBoxedType)->bool{
                    const QSharedPointer<jchar>* ptr = reinterpret_cast<const QSharedPointer<jchar>*>(in);
                    if(ptr && *ptr){
                        p.c = *ptr->get();
                        if(forceBoxedType){
                            p.l = QtJambiAPI::toJavaCharacterObject(env, p.c);
                        }
                    }
                    return true;
                };
            }
        }break;
        case PointerType::shared_ptr:{
            if(internalMetaType.id()==QMetaType::QChar){
                return [](JNIEnv* env, QtJambiScope*, const void* in, jvalue& p, bool forceBoxedType)->bool{
                    const std::shared_ptr<QChar>* ptr = reinterpret_cast<const std::shared_ptr<QChar>*>(in);
                    if(ptr && *ptr){
                        p.c = (*ptr)->unicode();
                        if(forceBoxedType){
                            p.l = QtJambiAPI::toJavaCharacterObject(env, p.c);
                        }
                    }
                    return true;
                };
            }else{
                return [](JNIEnv* env, QtJambiScope*, const void* in, jvalue& p, bool forceBoxedType)->bool{
                    const std::shared_ptr<jchar>* ptr = reinterpret_cast<const std::shared_ptr<jchar>*>(in);
                    if(ptr && *ptr){
                        p.c = *ptr->get();
                        if(forceBoxedType){
                            p.l = QtJambiAPI::toJavaCharacterObject(env, p.c);
                        }
                    }
                    return true;
                };
            }
        }break;
        default:
            return ParameterTypeInfo::default_internalToExternalConverter();
        }
    }else if(Java::Runtime::Float::isPrimitiveType(_env,externalClass) || Java::Runtime::Float::isSameClass(_env,externalClass)){
        switch(pointerType){
        case PointerType::NoPointer:{
            return [](JNIEnv* env, QtJambiScope*, const void* in, jvalue& p, bool forceBoxedType)->bool{
                memcpy(&p.f, in, 4);
                if(forceBoxedType){
                    p.l = QtJambiAPI::toJavaFloatObject(env, p.f);
                }
                return true;
            };
        }break;
        case PointerType::SharedPointer:{
            return [](JNIEnv* env, QtJambiScope*, const void* in, jvalue& p, bool forceBoxedType)->bool{
                const QSharedPointer<jfloat>* ptr = reinterpret_cast<const QSharedPointer<jfloat>*>(in);
                if(ptr && *ptr){
                    p.f = *ptr->get();
                    if(forceBoxedType){
                        p.l = QtJambiAPI::toJavaFloatObject(env, p.f);
                    }
                }
                return true;
            };
        }break;
        case PointerType::shared_ptr:{
            return [](JNIEnv* env, QtJambiScope*, const void* in, jvalue& p, bool forceBoxedType)->bool{
                const std::shared_ptr<jfloat>* ptr = reinterpret_cast<const std::shared_ptr<jfloat>*>(in);
                if(ptr && *ptr){
                    p.f = *ptr->get();
                    if(forceBoxedType){
                        p.l = QtJambiAPI::toJavaFloatObject(env, p.f);
                    }
                }
                return true;
            };
        }break;
        default:
            return ParameterTypeInfo::default_internalToExternalConverter();
        }
    }else if(Java::Runtime::Double::isPrimitiveType(_env,externalClass) || Java::Runtime::Double::isSameClass(_env,externalClass)){
        switch(pointerType){
        case PointerType::NoPointer:{
            return [](JNIEnv* env, QtJambiScope*, const void* in, jvalue& p, bool forceBoxedType)->bool{
                memcpy(&p.d, in, 8);
                if(forceBoxedType){
                    p.l = QtJambiAPI::toJavaDoubleObject(env, p.d);
                }
                return true;
            };
        }break;
        case PointerType::SharedPointer:{
            return [](JNIEnv* env, QtJambiScope*, const void* in, jvalue& p, bool forceBoxedType)->bool{
                const QSharedPointer<jdouble>* ptr = reinterpret_cast<const QSharedPointer<jdouble>*>(in);
                if(ptr && *ptr){
                    p.d = *ptr->get();
                    if(forceBoxedType){
                        p.l = QtJambiAPI::toJavaDoubleObject(env, p.d);
                    }
                }
                return true;
            };
        }break;
        case PointerType::shared_ptr:{
            return [](JNIEnv* env, QtJambiScope*, const void* in, jvalue& p, bool forceBoxedType)->bool{
                const std::shared_ptr<jdouble>* ptr = reinterpret_cast<const std::shared_ptr<jdouble>*>(in);
                if(ptr && *ptr){
                    p.d = *ptr->get();
                    if(forceBoxedType){
                        p.l = QtJambiAPI::toJavaDoubleObject(env, p.d);
                    }
                }
                return true;
            };
        }break;
        default:
            return ParameterTypeInfo::default_internalToExternalConverter();
        }
    }else if(Java::Runtime::String::isSameClass(_env,externalClass)){
        switch(pointerType){
        case PointerType::NoPointer:{
            return [](JNIEnv* env, QtJambiScope*, const void* in, jvalue& p, bool)->bool{
                const QString *strp = reinterpret_cast<const QString *>(in);
                p.l = qtjambi_cast<jstring>(env, *strp);
                return true;
            };
        }break;
        case PointerType::SharedPointer:{
            return [](JNIEnv* env, QtJambiScope*, const void* in, jvalue& p, bool)->bool{
                const QSharedPointer<QString>* ptr = reinterpret_cast<const QSharedPointer<QString>*>(in);
                if(ptr && *ptr){
                    p.l = qtjambi_cast<jstring>(env, **ptr);
                }
                return true;
            };
        }break;
        case PointerType::shared_ptr:{
            return [](JNIEnv* env, QtJambiScope*, const void* in, jvalue& p, bool)->bool{
                const std::shared_ptr<QString>* ptr = reinterpret_cast<const std::shared_ptr<QString>*>(in);
                if(ptr && *ptr){
                    p.l = qtjambi_cast<jstring>(env, **ptr);
                }
                return true;
            };
        }break;
        default:
            return ParameterTypeInfo::default_internalToExternalConverter();
        }
    }else if(Java::Runtime::CharSequence::isSameClass(_env,externalClass) || Java::QtCore::QString::isSameClass(_env,externalClass)){
        switch(pointerType){
        case PointerType::NoPointer:{
            return [](JNIEnv* env, QtJambiScope*, const void* in, jvalue& p, bool)->bool{
                const QString *strp = reinterpret_cast<const QString *>(in);
                p.l = QtJambiAPI::convertQStringToJavaObject(env, *strp);
                return true;
            };
        }break;
        case PointerType::SharedPointer:{
            return [](JNIEnv* env, QtJambiScope*, const void* in, jvalue& p, bool)->bool{
                const QSharedPointer<QString>* ptr = reinterpret_cast<const QSharedPointer<QString>*>(in);
                if(ptr){
                    p.l = QtJambiAPI::convertSmartPointerToJavaObject(env, *ptr);
                }
                return true;
            };
        }break;
        case PointerType::shared_ptr:{
            return [](JNIEnv* env, QtJambiScope*, const void* in, jvalue& p, bool)->bool{
                const std::shared_ptr<QString>* ptr = reinterpret_cast<const std::shared_ptr<QString>*>(in);
                if(ptr){
                    p.l = QtJambiAPI::convertSmartPointerToJavaObject(env, *ptr);
                }
                return true;
            };
        }break;
        case PointerType::WeakPointer:{
            return [](JNIEnv* env, QtJambiScope*, const void* in, jvalue& p, bool)->bool{
                const QWeakPointer<QString>* ptr = reinterpret_cast<const QWeakPointer<QString>*>(in);
                if(ptr){
                    if(const QSharedPointer<QString> _ptr{*ptr})
                        p.l = QtJambiAPI::convertSmartPointerToJavaObject(env, _ptr);
                }
                return true;
            };
        }break;
        case PointerType::weak_ptr:{
            return [](JNIEnv* env, QtJambiScope*, const void* in, jvalue& p, bool)->bool{
                const std::weak_ptr<QString>* ptr = reinterpret_cast<const std::weak_ptr<QString>*>(in);
                if(ptr){
                    if(const std::shared_ptr<QString> _ptr{*ptr})
                        p.l = QtJambiAPI::convertSmartPointerToJavaObject(env, _ptr);
                }
                return true;
            };
        }break;
        default:
            return ParameterTypeInfo::default_internalToExternalConverter();
        }
    }else if(Java::QtJambi::QNativePointer::isSameClass(_env,externalClass)){
        if(pointerType==PointerType::NoPointer){
            return [](JNIEnv* env, QtJambiScope*, const void* in, jvalue& p, bool)->bool{
                const void * const*in_p = reinterpret_cast<const void * const*>(in);
                p.l = QtJambiAPI::convertNativeToQNativePointer(env, *in_p, QNativePointer::Type::Pointer, -1, 1);
                return true;
            };
        }
    } else if (Java::QtJambi::QFlags::isAssignableFrom(_env,externalClass)) {
        if (internalMetaType==registeredMetaType(typeid(JObjectWrapper))) {
            return [](JNIEnv* env, QtJambiScope*, const void* in, jvalue& p, bool)->bool{
                p.l = reinterpret_cast<const JObjectWrapper *>(in)->object(env);
                return true;
            };
        }else if (Java::QtCore::QMetaType$GenericFlags::isSameClass(_env,externalClass)) {
            return [internalMetaType](JNIEnv* env, QtJambiScope*, const void* in, jvalue& p, bool)->bool{
                jint value = *reinterpret_cast<const qint32*>(in);
                p.l = Java::QtCore::QMetaType$GenericFlags::newInstance(env, internalMetaType.id(), value);
                return true;
            };
#if QT_VERSION >= QT_VERSION_CHECK(6, 9, 0)
        }else if (Java::QtCore::QMetaType$GenericLongFlags::isSameClass(_env,externalClass)) {
            return [internalMetaType](JNIEnv* env, QtJambiScope*, const void* in, jvalue& p, bool)->bool{
                jlong value = *reinterpret_cast<const qint64*>(in);
                p.l = Java::QtCore::QMetaType$GenericLongFlags::newInstance(env, internalMetaType.id(), value);
                return true;
            };
#endif
        }else{
            externalClass = getGlobalClassRef(_env, externalClass);
            switch(pointerType){
            case PointerType::NoPointer:{
                if(internalMetaType.sizeOf()==sizeof(jlong)){
                    return [externalClass](JNIEnv* env, QtJambiScope*, const void* in, jvalue& p, bool)->bool{
                        p.l = CoreAPI::convertQFlagsToJavaObject(env, *reinterpret_cast<const jlong *>(in), externalClass);
                        return p.l!=nullptr;
                    };
                }else{
                    return [externalClass](JNIEnv* env, QtJambiScope*, const void* in, jvalue& p, bool)->bool{
                        p.l = CoreAPI::convertQFlagsToJavaObject(env, *reinterpret_cast<const jint *>(in), externalClass);
                        return p.l!=nullptr;
                    };
                }
            }break;
            case PointerType::SharedPointer:{
                return [externalClass](JNIEnv* env, QtJambiScope*, const void* in, jvalue& p, bool)->bool{
                    const QSharedPointer<jint>* ptr = reinterpret_cast<const QSharedPointer<jint>*>(in);
                    if(ptr && *ptr){
                        p.l = CoreAPI::convertQFlagsToJavaObject(env, **ptr, externalClass);
                    }
                    return true;
                };
            }break;
            case PointerType::shared_ptr:{
                return [externalClass](JNIEnv* env, QtJambiScope*, const void* in, jvalue& p, bool)->bool{
                    const std::shared_ptr<jint>* ptr = reinterpret_cast<const std::shared_ptr<jint>*>(in);
                    if(ptr && *ptr){
                        p.l = CoreAPI::convertQFlagsToJavaObject(env, **ptr, externalClass);
                    }
                    return true;
                };
            }break;
            default:
                return ParameterTypeInfo::default_internalToExternalConverter();
            }
        }
    } else if(Java::Runtime::Enum::isAssignableFrom(_env,externalClass)
              || Java::QtJambi::QtEnumerator::isAssignableFrom(_env,externalClass)
              || Java::QtJambi::QtLongEnumerator::isAssignableFrom(_env,externalClass)
              || Java::QtJambi::QtShortEnumerator::isAssignableFrom(_env,externalClass)
              || Java::QtJambi::QtByteEnumerator::isAssignableFrom(_env,externalClass)
              || (internalMetaType.flags() & QMetaType::IsEnumeration)){
        if (internalMetaType==registeredMetaType(typeid(JObjectWrapper))) {
            return [](JNIEnv* env, QtJambiScope*, const void* in, jvalue& p, bool)->bool{
                p.l = reinterpret_cast<const JObjectWrapper *>(in)->object(env);
                return true;
            };
        }else if (Java::QtCore::QMetaType$GenericEnumerator::isSameClass(_env,externalClass)) {
            if(pointerType!=PointerType::NoPointer)
                return ParameterTypeInfo::default_internalToExternalConverter();
            return [internalMetaType](JNIEnv* env, QtJambiScope*, const void* in, jvalue& p, bool)->bool{
                jint index = -1;
                jint value = *reinterpret_cast<const int*>(in);
                jstring name = nullptr;
                QMetaEnum enm;
                QtJambiStorage* storage = getQtJambiStorage();
                {
                    QReadLocker locker(storage->lock());
                    enm = storage->metaEnumByMetaTypeHash().value(internalMetaType.iface());
                }
                if(enm.isValid()){
                    for(int i=0; i<enm.keyCount(); ++i){
                        if(enm.value(i)==value){
                            index = i;
                            break;
                        }
                    }
                    if(index>=0){
                        if(const char* _name = enm.key(index))
                            name = env->NewStringUTF(_name);
                    }
                }
                p.l = Java::QtCore::QMetaType$GenericEnumerator::newInstance(env, internalMetaType.id(), index, value, name);
                return true;
            };
        } else if (Java::QtCore::QMetaType$GenericShortEnumerator::isSameClass(_env,externalClass)) {
            if(pointerType!=PointerType::NoPointer)
                return ParameterTypeInfo::default_internalToExternalConverter();
            return [internalMetaType](JNIEnv* env, QtJambiScope*, const void* in, jvalue& p, bool)->bool{
                jint index = -1;
                jshort value = *reinterpret_cast<const qint16*>(in);
                jstring name = nullptr;
                QMetaEnum enm;
                QtJambiStorage* storage = getQtJambiStorage();
                {
                    QReadLocker locker(storage->lock());
                    enm = storage->metaEnumByMetaTypeHash().value(internalMetaType.iface());
                }
                if(enm.isValid()){
                    for(int i=0; i<enm.keyCount(); ++i){
                        if(enm.value(i)==value){
                            index = i;
                            break;
                        }
                    }
                    if(index>=0){
                        if(const char* _name = enm.key(index))
                            name = env->NewStringUTF(_name);
                    }
                }
                p.l = Java::QtCore::QMetaType$GenericShortEnumerator::newInstance(env, internalMetaType.id(), index, value, name);
                return true;
            };
        } else if (Java::QtCore::QMetaType$GenericByteEnumerator::isSameClass(_env,externalClass)) {
            if(pointerType!=PointerType::NoPointer)
                return ParameterTypeInfo::default_internalToExternalConverter();
            return [internalMetaType](JNIEnv* env, QtJambiScope*, const void* in, jvalue& p, bool)->bool{
                jint index = -1;
                jbyte value = *reinterpret_cast<const qint8*>(in);
                jstring name = nullptr;
                QMetaEnum enm;
                QtJambiStorage* storage = getQtJambiStorage();
                {
                    QReadLocker locker(storage->lock());
                    enm = storage->metaEnumByMetaTypeHash().value(internalMetaType.iface());
                }
                if(enm.isValid()){
                    for(int i=0; i<enm.keyCount(); ++i){
                        if(enm.value(i)==value){
                            index = i;
                            break;
                        }
                    }
                    if(index>=0){
                        if(const char* _name = enm.key(index))
                            name = env->NewStringUTF(_name);
                    }
                }
                p.l = Java::QtCore::QMetaType$GenericByteEnumerator::newInstance(env, internalMetaType.id(), index, value, name);
                return true;
            };
        } else if (Java::QtCore::QMetaType$GenericLongEnumerator::isSameClass(_env,externalClass)) {
            if(pointerType!=PointerType::NoPointer)
                return ParameterTypeInfo::default_internalToExternalConverter();
            return [internalMetaType](JNIEnv* env, QtJambiScope*, const void* in, jvalue& p, bool)->bool{
                jint index = -1;
                jlong value = *reinterpret_cast<const qint64*>(in);
                jstring name = nullptr;
                QMetaEnum enm;
                QtJambiStorage* storage = getQtJambiStorage();
                {
                    QReadLocker locker(storage->lock());
                    enm = storage->metaEnumByMetaTypeHash().value(internalMetaType.iface());
                }
                if(enm.isValid()){
                    for(int i=0; i<enm.keyCount(); ++i){
                        if(enm.value(i)==value){
                            index = i;
                            break;
                        }
                    }
                    if(index>=0){
                        if(const char* _name = enm.key(index))
                            name = env->NewStringUTF(_name);
                    }
                }
                p.l = Java::QtCore::QMetaType$GenericLongEnumerator::newInstance(env, internalMetaType.id(), index, value, name);
                return true;
            };
        }else{
            externalClass = getGlobalClassRef(_env, externalClass);
            switch(pointerType){
            case PointerType::NoPointer:{
                if (Java::QtJambi::QtEnumerator::isAssignableFrom(_env,externalClass)) {
                    return [externalClass](JNIEnv* env, QtJambiScope*, const void* in, jvalue& p, bool)->bool{
                        p.l = CoreAPI::convertEnumToJavaObject(env, *reinterpret_cast<const qint32 *>(in), externalClass);
                        return p.l!=nullptr;
                    };
                }else if (Java::QtJambi::QtShortEnumerator::isAssignableFrom(_env,externalClass)) {
                    return [externalClass](JNIEnv* env, QtJambiScope*, const void* in, jvalue& p, bool)->bool{
                        p.l = CoreAPI::convertEnumToJavaObject(env, *reinterpret_cast<const qint16 *>(in), externalClass);
                        return p.l!=nullptr;
                    };
                }else if (Java::QtJambi::QtByteEnumerator::isAssignableFrom(_env,externalClass)) {
                    return [externalClass](JNIEnv* env, QtJambiScope*, const void* in, jvalue& p, bool)->bool{
                        p.l = CoreAPI::convertEnumToJavaObject(env, *reinterpret_cast<const qint8 *>(in), externalClass);
                        return p.l!=nullptr;
                    };
                }else if (Java::QtJambi::QtLongEnumerator::isAssignableFrom(_env,externalClass)) {
                    return [externalClass](JNIEnv* env, QtJambiScope*, const void* in, jvalue& p, bool)->bool{
                        p.l = CoreAPI::convertEnumToJavaObject(env, *reinterpret_cast<const qint64 *>(in), externalClass);
                        return p.l!=nullptr;
                    };
                }else if (Java::Runtime::Enum::isAssignableFrom(_env,externalClass)) {
                    return [externalClass](JNIEnv* env, QtJambiScope*, const void* in, jvalue& p, bool)->bool{
                        jobjectArray enumConstants = Java::Runtime::Class::getEnumConstants(env, externalClass);
                        p.l = env->GetObjectArrayElement(enumConstants, *reinterpret_cast<const qint32 *>(in));
                        return p.l!=nullptr;
                    };
                }else if(internalMetaType.sizeOf()==sizeof(qint8)){
                    return [](JNIEnv* env, QtJambiScope*, const void* in, jvalue& p, bool forceBoxedType)->bool{
                        memcpy(&p.i, in, 1);
                        if(forceBoxedType){
                            p.l = QtJambiAPI::toJavaByteObject(env, p.i);
                        }
                        return true;
                    };
                }else if(internalMetaType.sizeOf()==sizeof(qint16)){
                    return [](JNIEnv* env, QtJambiScope*, const void* in, jvalue& p, bool forceBoxedType)->bool{
                        memcpy(&p.i, in, 2);
                        if(forceBoxedType){
                            p.l = QtJambiAPI::toJavaShortObject(env, p.i);
                        }
                        return true;
                    };
                }else if(internalMetaType.sizeOf()==sizeof(qint64)){
                    return [](JNIEnv* env, QtJambiScope*, const void* in, jvalue& p, bool forceBoxedType)->bool{
                        memcpy(&p.i, in, 8);
                        if(forceBoxedType){
                            p.l = QtJambiAPI::toJavaLongObject(env, p.i);
                        }
                        return true;
                    };
                }else{
                    return [](JNIEnv* env, QtJambiScope*, const void* in, jvalue& p, bool forceBoxedType)->bool{
                        memcpy(&p.i, in, 4);
                        if(forceBoxedType){
                            p.l = QtJambiAPI::toJavaIntegerObject(env, p.i);
                        }
                        return true;
                    };
                }
            }break;
            case PointerType::SharedPointer:{
                if (Java::QtJambi::QtEnumerator::isAssignableFrom(_env,externalClass)) {
                    return [externalClass](JNIEnv* env, QtJambiScope*, const void* in, jvalue& p, bool)->bool{
                        const QSharedPointer<qint32>* ptr = reinterpret_cast<const QSharedPointer<qint32>*>(in);
                        if(ptr && *ptr){
                            p.l = CoreAPI::convertEnumToJavaObject(env, **ptr, externalClass);
                        }
                        return true;
                    };
                }else if (Java::QtJambi::QtShortEnumerator::isAssignableFrom(_env,externalClass)) {
                    return [externalClass](JNIEnv* env, QtJambiScope*, const void* in, jvalue& p, bool)->bool{
                        const QSharedPointer<qint16>* ptr = reinterpret_cast<const QSharedPointer<qint16>*>(in);
                        if(ptr && *ptr){
                            p.l = CoreAPI::convertEnumToJavaObject(env, **ptr, externalClass);
                        }
                        return true;
                    };
                }else if (Java::QtJambi::QtByteEnumerator::isAssignableFrom(_env,externalClass)) {
                    return [externalClass](JNIEnv* env, QtJambiScope*, const void* in, jvalue& p, bool)->bool{
                        const QSharedPointer<qint8>* ptr = reinterpret_cast<const QSharedPointer<qint8>*>(in);
                        if(ptr && *ptr){
                            p.l = CoreAPI::convertEnumToJavaObject(env, **ptr, externalClass);
                        }
                        return true;
                    };
                }else if (Java::QtJambi::QtLongEnumerator::isAssignableFrom(_env,externalClass)) {
                    return [externalClass](JNIEnv* env, QtJambiScope*, const void* in, jvalue& p, bool)->bool{
                        const QSharedPointer<qint64>* ptr = reinterpret_cast<const QSharedPointer<qint64>*>(in);
                        if(ptr && *ptr){
                            p.l = CoreAPI::convertEnumToJavaObject(env, **ptr, externalClass);
                        }
                        return true;
                    };
                }else if (Java::Runtime::Enum::isAssignableFrom(_env,externalClass)) {
                    return [externalClass](JNIEnv* env, QtJambiScope*, const void* in, jvalue& p, bool)->bool{
                        const QSharedPointer<jint>* ptr = reinterpret_cast<const QSharedPointer<jint>*>(in);
                        if(ptr && *ptr){
                            jobjectArray enumConstants = Java::Runtime::Class::getEnumConstants(env, externalClass);
                            p.l = env->GetObjectArrayElement(enumConstants, *reinterpret_cast<const qint32 *>(in));
                        }
                        return true;
                    };
                }else if(internalMetaType.sizeOf()==sizeof(qint8)){
                    return [](JNIEnv* env, QtJambiScope*, const void* in, jvalue& p, bool forceBoxedType)->bool{
                        const QSharedPointer<jbyte>* ptr = reinterpret_cast<const QSharedPointer<jbyte>*>(in);
                        if(ptr && *ptr){
                            p.b = *ptr->get();
                            if(forceBoxedType){
                                p.l = QtJambiAPI::toJavaByteObject(env, p.b);
                            }
                        }
                        return true;
                    };
                }else if(internalMetaType.sizeOf()==sizeof(qint16)){
                    return [](JNIEnv* env, QtJambiScope*, const void* in, jvalue& p, bool forceBoxedType)->bool{
                        const QSharedPointer<jshort>* ptr = reinterpret_cast<const QSharedPointer<jshort>*>(in);
                        if(ptr && *ptr){
                            p.s = *ptr->get();
                            if(forceBoxedType){
                                p.l = QtJambiAPI::toJavaShortObject(env, p.s);
                            }
                        }
                        return true;
                    };
                }else if(internalMetaType.sizeOf()==sizeof(qint64)){
                    return [](JNIEnv* env, QtJambiScope*, const void* in, jvalue& p, bool forceBoxedType)->bool{
                        const QSharedPointer<jlong>* ptr = reinterpret_cast<const QSharedPointer<jlong>*>(in);
                        if(ptr && *ptr){
                            p.j = *ptr->get();
                            if(forceBoxedType){
                                p.l = QtJambiAPI::toJavaLongObject(env, p.j);
                            }
                        }
                        return true;
                    };
                }else{
                    return [](JNIEnv* env, QtJambiScope*, const void* in, jvalue& p, bool forceBoxedType)->bool{
                        const QSharedPointer<jint>* ptr = reinterpret_cast<const QSharedPointer<jint>*>(in);
                        if(ptr && *ptr){
                            p.i = *ptr->get();
                            if(forceBoxedType){
                                p.l = QtJambiAPI::toJavaIntegerObject(env, p.i);
                            }
                        }
                        return true;
                    };
                }
            }break;
            case PointerType::shared_ptr:{
                if (Java::QtJambi::QtEnumerator::isAssignableFrom(_env,externalClass)) {
                    return [externalClass](JNIEnv* env, QtJambiScope*, const void* in, jvalue& p, bool)->bool{
                        const std::shared_ptr<qint32>* ptr = reinterpret_cast<const std::shared_ptr<qint32>*>(in);
                        if(ptr && *ptr){
                            p.l = CoreAPI::convertEnumToJavaObject(env, **ptr, externalClass);
                        }
                        return true;
                    };
                }else if (Java::QtJambi::QtShortEnumerator::isAssignableFrom(_env,externalClass)) {
                    return [externalClass](JNIEnv* env, QtJambiScope*, const void* in, jvalue& p, bool)->bool{
                        const std::shared_ptr<qint16>* ptr = reinterpret_cast<const std::shared_ptr<qint16>*>(in);
                        if(ptr && *ptr){
                            p.l = CoreAPI::convertEnumToJavaObject(env, **ptr, externalClass);
                        }
                        return true;
                    };
                }else if (Java::QtJambi::QtByteEnumerator::isAssignableFrom(_env,externalClass)) {
                    return [externalClass](JNIEnv* env, QtJambiScope*, const void* in, jvalue& p, bool)->bool{
                        const std::shared_ptr<qint8>* ptr = reinterpret_cast<const std::shared_ptr<qint8>*>(in);
                        if(ptr && *ptr){
                            p.l = CoreAPI::convertEnumToJavaObject(env, **ptr, externalClass);
                        }
                        return true;
                    };
                }else if (Java::QtJambi::QtLongEnumerator::isAssignableFrom(_env,externalClass)) {
                    return [externalClass](JNIEnv* env, QtJambiScope*, const void* in, jvalue& p, bool)->bool{
                        const std::shared_ptr<qint64>* ptr = reinterpret_cast<const std::shared_ptr<qint64>*>(in);
                        if(ptr && *ptr){
                            p.l = CoreAPI::convertEnumToJavaObject(env, **ptr, externalClass);
                        }
                        return true;
                    };
                }else if (Java::Runtime::Enum::isAssignableFrom(_env,externalClass)) {
                    return [externalClass](JNIEnv* env, QtJambiScope*, const void* in, jvalue& p, bool)->bool{
                        const std::shared_ptr<jint>* ptr = reinterpret_cast<const std::shared_ptr<jint>*>(in);
                        if(ptr && *ptr){
                            jobjectArray enumConstants = Java::Runtime::Class::getEnumConstants(env, externalClass);
                            p.l = env->GetObjectArrayElement(enumConstants, *reinterpret_cast<const qint32 *>(in));
                        }
                        return true;
                    };
                }else if(internalMetaType.sizeOf()==sizeof(qint8)){
                    return [](JNIEnv* env, QtJambiScope*, const void* in, jvalue& p, bool forceBoxedType)->bool{
                        const std::shared_ptr<jbyte>* ptr = reinterpret_cast<const std::shared_ptr<jbyte>*>(in);
                        if(ptr && *ptr){
                            p.b = *ptr->get();
                            if(forceBoxedType){
                                p.l = QtJambiAPI::toJavaByteObject(env, p.b);
                            }
                        }
                        return true;
                    };
                }else if(internalMetaType.sizeOf()==sizeof(qint16)){
                    return [](JNIEnv* env, QtJambiScope*, const void* in, jvalue& p, bool forceBoxedType)->bool{
                        const std::shared_ptr<jshort>* ptr = reinterpret_cast<const std::shared_ptr<jshort>*>(in);
                        if(ptr && *ptr){
                            p.s = *ptr->get();
                            if(forceBoxedType){
                                p.l = QtJambiAPI::toJavaShortObject(env, p.s);
                            }
                        }
                        return true;
                    };
                }else if(internalMetaType.sizeOf()==sizeof(qint64)){
                    return [](JNIEnv* env, QtJambiScope*, const void* in, jvalue& p, bool forceBoxedType)->bool{
                        const std::shared_ptr<jlong>* ptr = reinterpret_cast<const std::shared_ptr<jlong>*>(in);
                        if(ptr && *ptr){
                            p.j = *ptr->get();
                            if(forceBoxedType){
                                p.l = QtJambiAPI::toJavaLongObject(env, p.j);
                            }
                        }
                        return true;
                    };
                }else{
                    return [](JNIEnv* env, QtJambiScope*, const void* in, jvalue& p, bool forceBoxedType)->bool{
                        const std::shared_ptr<jint>* ptr = reinterpret_cast<const std::shared_ptr<jint>*>(in);
                        if(ptr && *ptr){
                            p.i = *ptr->get();
                            if(forceBoxedType){
                                p.l = QtJambiAPI::toJavaIntegerObject(env, p.i);
                            }
                        }
                        return true;
                    };
                }
            }break;
            default:
                return ParameterTypeInfo::default_internalToExternalConverter();
            }
        }
    } else if (Java::QtCore::QMetaType$GenericValue::isSameClass(_env,externalClass)) {
        if(pointerType!=PointerType::NoPointer)
            return ParameterTypeInfo::default_internalToExternalConverter();
        return [internalMetaType](JNIEnv* env, QtJambiScope*, const void* in, jvalue& p, bool)->bool{
            p.l = QtJambiAPI::convertNativeToJavaOwnedObjectAsWrapper(env, internalMetaType.create(in), Java::QtCore::QMetaType$GenericValue::getClass(env));
            if(p.l){
                Java::QtCore::QMetaType$GenericValue::set_type(env, p.l, internalMetaType.id());
            }
            return true;
        };
    } else if (Java::QtCore::QMetaType$GenericObject::isSameClass(_env,externalClass)) {
        if(pointerType!=PointerType::NoPointer)
            return ParameterTypeInfo::default_internalToExternalConverter();
        return [internalMetaType](JNIEnv* env, QtJambiScope* scope, const void* in, jvalue& p, bool)->bool{
            p.l = QtJambiAPI::convertNativeToJavaObjectAsWrapper(env, in, Java::QtCore::QMetaType$GenericObject::getClass(env));
            if(p.l){
                Java::QtCore::QMetaType$GenericObject::set_type(env, p.l, internalMetaType.id());
                if(scope)
                    scope->addObjectInvalidation(env, p.l);
            }
            return true;
        };
    } else if (Java::QtCore::QMetaType$GenericGadget::isSameClass(_env,externalClass)) {
        if(pointerType!=PointerType::NoPointer)
            return ParameterTypeInfo::default_internalToExternalConverter();
        if(internalMetaType.flags() & QMetaType::PointerToGadget){
            return [internalMetaType](JNIEnv* env, QtJambiScope* scope, const void* in, jvalue& p, bool)->bool{
                p.l = QtJambiAPI::convertNativeToJavaObjectAsWrapper(env, in, Java::QtCore::QMetaType$GenericGadget::getClass(env));
                if(p.l){
                    Java::QtCore::QMetaType$GenericGadget::set_type(env, p.l, internalMetaType.id());
                    jobject mo = qtjambi_cast<jobject>(env, internalMetaType.metaObject());
                    Java::QtJambi::QtGadget::set_staticMetaObject(env, p.l, mo);
                    if(scope)
                        scope->addObjectInvalidation(env, p.l);
                }
                return true;
            };
        }else{
            return [internalMetaType](JNIEnv* env, QtJambiScope*, const void* in, jvalue& p, bool)->bool{
                p.l = QtJambiAPI::convertNativeToJavaObjectAsWrapper(env, internalMetaType.create(in), Java::QtCore::QMetaType$GenericGadget::getClass(env));
                if(p.l){
                    Java::QtCore::QMetaType$GenericGadget::set_type(env, p.l, internalMetaType.id());
                    jobject mo = qtjambi_cast<jobject>(env, internalMetaType.metaObject());
                    Java::QtJambi::QtGadget::set_staticMetaObject(env, p.l, mo);
                }
                return true;
            };
        }
    }else if (Java::QtCore::QModelIndex::isAssignableFrom(_env,externalClass)) {
        if(pointerType!=PointerType::NoPointer)
            return ParameterTypeInfo::default_internalToExternalConverter();
        if (allowValuePointers && internalTypeName.contains(QLatin1Char('*'))){
            return [](JNIEnv* env, QtJambiScope*, const void* in, jvalue& p, bool)->bool{
                if(in){
                    QModelIndex const* ptr = *reinterpret_cast<QModelIndex const* const*>(in);
                    p.l = ptr ? qtjambi_cast<jobject>(env, *ptr) : nullptr;
                }
                return true;
            };
        }else{
            return [](JNIEnv* env, QtJambiScope*, const void* in, jvalue& p, bool)->bool{
                p.l = qtjambi_cast<jobject>(env, *reinterpret_cast<QModelIndex const*>(in));
                return true;
            };
        }
    }else if (Java::QtCore::QMetaObject::isAssignableFrom(_env,externalClass)) {
        if(pointerType!=PointerType::NoPointer)
            return ParameterTypeInfo::default_internalToExternalConverter();
        if (allowValuePointers && internalTypeName.contains(QLatin1Char('*'))){
            return [](JNIEnv* env, QtJambiScope*, const void* in, jvalue& p, bool)->bool{
                if(in){
                    p.l = qtjambi_cast<jobject>(env, *reinterpret_cast<QMetaObject const* const*>(in));
                }
                return true;
            };
        }else{
            return [](JNIEnv* env, QtJambiScope*, const void* in, jvalue& p, bool)->bool{
                p.l = qtjambi_cast<jobject>(env, reinterpret_cast<QMetaObject const*>(in));
                return true;
            };
        }
    }else if (Java::QtCore::QMetaObject$Connection::isAssignableFrom(_env,externalClass)) {
        switch(pointerType){
        case PointerType::NoPointer:{
            return [](JNIEnv* env, QtJambiScope*, const void* in, jvalue& p, bool)->bool{
                if(in){
                    p.l = qtjambi_cast<jobject>(env, *reinterpret_cast<const QMetaObject::Connection*>(in));
                }
                return true;
            };
        }break;
        case PointerType::SharedPointer:{
            return [](JNIEnv* env, QtJambiScope*, const void* in, jvalue& p, bool)->bool{
                const QSharedPointer<QMetaObject::Connection>* ptr = reinterpret_cast<const QSharedPointer<QMetaObject::Connection>*>(in);
                if(ptr){
                    p.l = QtJambiAPI::convertSmartPointerToJavaObject(env, *ptr);
                }
                return true;
            };
        }break;
        case PointerType::shared_ptr:{
            return [](JNIEnv* env, QtJambiScope*, const void* in, jvalue& p, bool)->bool{
                const std::shared_ptr<QMetaObject::Connection>* ptr = reinterpret_cast<const std::shared_ptr<QMetaObject::Connection>*>(in);
                if(ptr){
                    p.l = QtJambiAPI::convertSmartPointerToJavaObject(env, *ptr);
                }
                return true;
            };
        }break;
        case PointerType::WeakPointer:{
            return [](JNIEnv* env, QtJambiScope*, const void* in, jvalue& p, bool)->bool{
                const QWeakPointer<QMetaObject::Connection>* ptr = reinterpret_cast<const QWeakPointer<QMetaObject::Connection>*>(in);
                if(ptr){
                    if(const QSharedPointer<QMetaObject::Connection> _ptr{*ptr})
                        p.l = QtJambiAPI::convertSmartPointerToJavaObject(env, _ptr);
                }
                return true;
            };
        }break;
        case PointerType::weak_ptr:{
            return [](JNIEnv* env, QtJambiScope*, const void* in, jvalue& p, bool)->bool{
                const std::weak_ptr<QMetaObject::Connection>* ptr = reinterpret_cast<const std::weak_ptr<QMetaObject::Connection>*>(in);
                if(ptr){
                    if(const std::shared_ptr<QMetaObject::Connection> _ptr{*ptr})
                        p.l = QtJambiAPI::convertSmartPointerToJavaObject(env, _ptr);
                }
                return true;
            };
        }break;
        default:
            return ParameterTypeInfo::default_internalToExternalConverter();
        }
    }else if((internalMetaType.id()==QMetaType::QStringList
             || internalTypeName=="QStringList"
             || (pointerType!=PointerType::NoPointer && internalTypeName=="QStringList*")
             || internalTypeName=="QList<QString>"
             || (pointerType!=PointerType::NoPointer && internalTypeName=="QList<QString>*"))
             &&
             ( Java::QtCore::QList::isAssignableFrom(_env,externalClass)
               || Java::Runtime::List::isSameClass(_env,externalClass)
               || Java::Runtime::Collection::isSameClass(_env,externalClass)
               || Java::Runtime::Object::isSameClass(_env,externalClass))
             ){
        switch(pointerType){
        case PointerType::NoPointer:{
            return [](JNIEnv* env, QtJambiScope*, const void* in, jvalue& p, bool)->bool{
                if(in){
                    p.l = qtjambi_cast<jobject>(env, *reinterpret_cast<const QStringList*>(in));
                }
                return true;
            };
        }break;
        case PointerType::SharedPointer:{
            return [](JNIEnv* env, QtJambiScope*, const void* in, jvalue& p, bool)->bool{
                const QSharedPointer<QStringList>* ptr = reinterpret_cast<const QSharedPointer<QStringList>*>(in);
                if(ptr){
                    p.l = QtJambiAPI::convertSmartPointerToJavaObject(env, *ptr);
                }
                return true;
            };
        }break;
        case PointerType::shared_ptr:{
            return [](JNIEnv* env, QtJambiScope*, const void* in, jvalue& p, bool)->bool{
                const std::shared_ptr<QStringList>* ptr = reinterpret_cast<const std::shared_ptr<QStringList>*>(in);
                if(ptr){
                    p.l = QtJambiAPI::convertSmartPointerToJavaObject(env, *ptr);
                }
                return true;
            };
        }break;
        case PointerType::WeakPointer:{
            return [](JNIEnv* env, QtJambiScope*, const void* in, jvalue& p, bool)->bool{
                const QWeakPointer<QStringList>* ptr = reinterpret_cast<const QWeakPointer<QStringList>*>(in);
                if(ptr){
                    if(const QSharedPointer<QStringList> _ptr{*ptr})
                        p.l = QtJambiAPI::convertSmartPointerToJavaObject(env, _ptr);
                }
                return true;
            };
        }break;
        case PointerType::weak_ptr:{
            return [](JNIEnv* env, QtJambiScope*, const void* in, jvalue& p, bool)->bool{
                const std::weak_ptr<QStringList>* ptr = reinterpret_cast<const std::weak_ptr<QStringList>*>(in);
                if(ptr){
                    if(const std::shared_ptr<QStringList> _ptr{*ptr})
                        p.l = QtJambiAPI::convertSmartPointerToJavaObject(env, _ptr);
                }
                return true;
            };
        }break;
        default:
            return ParameterTypeInfo::default_internalToExternalConverter();
        }
    }else if((internalMetaType.id()==QMetaType::QByteArrayList
             || internalTypeName=="QByteArrayList"
             || (pointerType!=PointerType::NoPointer && internalTypeName=="QByteArrayList*")
             || internalTypeName=="QList<QByteArray>"
             || (pointerType!=PointerType::NoPointer && internalTypeName=="QList<QByteArray>*")
             )
             &&
             ( Java::QtCore::QList::isAssignableFrom(_env,externalClass)
               || Java::Runtime::List::isSameClass(_env,externalClass)
               || Java::Runtime::Collection::isSameClass(_env,externalClass)
               || Java::Runtime::Object::isSameClass(_env,externalClass))
             ){
        switch(pointerType){
        case PointerType::NoPointer:{
            return [](JNIEnv* env, QtJambiScope*, const void* in, jvalue& p, bool)->bool{
                if(in){
                    p.l = qtjambi_cast<jobject>(env, *reinterpret_cast<const QByteArrayList*>(in));
                }
                return true;
            };
        }break;
        case PointerType::SharedPointer:{
            return [](JNIEnv* env, QtJambiScope*, const void* in, jvalue& p, bool)->bool{
                const QSharedPointer<QByteArrayList>* ptr = reinterpret_cast<const QSharedPointer<QByteArrayList>*>(in);
                if(ptr){
                    p.l = QtJambiAPI::convertSmartPointerToJavaObject(env, *ptr);
                }
                return true;
            };
        }break;
        case PointerType::shared_ptr:{
            return [](JNIEnv* env, QtJambiScope*, const void* in, jvalue& p, bool)->bool{
                const std::shared_ptr<QByteArrayList>* ptr = reinterpret_cast<const std::shared_ptr<QByteArrayList>*>(in);
                if(ptr){
                    p.l = QtJambiAPI::convertSmartPointerToJavaObject(env, *ptr);
                }
                return true;
            };
        }break;
        case PointerType::WeakPointer:{
            return [](JNIEnv* env, QtJambiScope*, const void* in, jvalue& p, bool)->bool{
                const QWeakPointer<QByteArrayList>* ptr = reinterpret_cast<const QWeakPointer<QByteArrayList>*>(in);
                if(ptr){
                    if(const QSharedPointer<QByteArrayList> _ptr{*ptr})
                        p.l = QtJambiAPI::convertSmartPointerToJavaObject(env, _ptr);
                }
                return true;
            };
        }break;
        case PointerType::weak_ptr:{
            return [](JNIEnv* env, QtJambiScope*, const void* in, jvalue& p, bool)->bool{
                const std::weak_ptr<QByteArrayList>* ptr = reinterpret_cast<const std::weak_ptr<QByteArrayList>*>(in);
                if(ptr){
                    if(const std::shared_ptr<QByteArrayList> _ptr{*ptr})
                        p.l = QtJambiAPI::convertSmartPointerToJavaObject(env, _ptr);
                }
                return true;
            };
        }break;
        default:
            return ParameterTypeInfo::default_internalToExternalConverter();
        }
    }else if((internalMetaType.id()==QMetaType::QVariantList
              || internalTypeName=="QVariantList"
              || (pointerType!=PointerType::NoPointer && internalTypeName=="QVariantList*")
              || internalTypeName=="QList<QVariant>"
              || (pointerType!=PointerType::NoPointer && internalTypeName=="QList<QVariant>*")
              )
             &&
             ( Java::QtCore::QList::isAssignableFrom(_env,externalClass)
               || Java::Runtime::List::isSameClass(_env,externalClass)
               || Java::Runtime::Collection::isSameClass(_env,externalClass)
               || Java::Runtime::Object::isSameClass(_env,externalClass))
             && pointerType==PointerType::NoPointer
             ){
        switch(pointerType){
        case PointerType::NoPointer:{
            return [](JNIEnv* env, QtJambiScope*, const void* in, jvalue& p, bool)->bool{
                if(in){
                    p.l = qtjambi_cast<jobject>(env, *reinterpret_cast<const QVariantList*>(in));
                }
                return true;
            };
        }break;
        case PointerType::SharedPointer:{
            return [](JNIEnv* env, QtJambiScope*, const void* in, jvalue& p, bool)->bool{
                const QSharedPointer<QVariantList>* ptr = reinterpret_cast<const QSharedPointer<QVariantList>*>(in);
                if(ptr){
                    p.l = QtJambiAPI::convertSmartPointerToJavaObject(env, *ptr);
                }
                return true;
            };
        }break;
        case PointerType::shared_ptr:{
            return [](JNIEnv* env, QtJambiScope*, const void* in, jvalue& p, bool)->bool{
                const std::shared_ptr<QVariantList>* ptr = reinterpret_cast<const std::shared_ptr<QVariantList>*>(in);
                if(ptr){
                    p.l = QtJambiAPI::convertSmartPointerToJavaObject(env, *ptr);
                }
                return true;
            };
        }break;
        case PointerType::WeakPointer:{
            return [](JNIEnv* env, QtJambiScope*, const void* in, jvalue& p, bool)->bool{
                const QWeakPointer<QVariantList>* ptr = reinterpret_cast<const QWeakPointer<QVariantList>*>(in);
                if(ptr){
                    if(const QSharedPointer<QVariantList> _ptr{*ptr})
                        p.l = QtJambiAPI::convertSmartPointerToJavaObject(env, _ptr);
                }
                return true;
            };
        }break;
        case PointerType::weak_ptr:{
            return [](JNIEnv* env, QtJambiScope*, const void* in, jvalue& p, bool)->bool{
                const std::weak_ptr<QVariantList>* ptr = reinterpret_cast<const std::weak_ptr<QVariantList>*>(in);
                if(ptr){
                    if(const std::shared_ptr<QVariantList> _ptr{*ptr})
                        p.l = QtJambiAPI::convertSmartPointerToJavaObject(env, _ptr);
                }
                return true;
            };
        }break;
        default:
            return ParameterTypeInfo::default_internalToExternalConverter();
        }
    }else if(internalTypeName.startsWith("QQmlListProperty<")){
        auto idx = internalTypeName.indexOf("<");
        const QString instantiation = internalTypeName.mid(idx+1).chopped(1).trimmed();
        QMetaType memberMetaType = QMetaType::fromName(qPrintable(instantiation+"*"));
        if(const std::type_info* t = getTypeByQtName(instantiation)){
            if(const char* javaName = getJavaName(*t)){
                jclass clazz = JavaAPI::resolveClass(_env, javaName);
                if(!memberMetaType.isValid()){
                    const QtPrivate::QMetaTypeInterface* qObjectMetaTypeInterface = QMetaType(QMetaType::QObjectStar).iface();
                    memberMetaType = createMetaType(qPrintable(instantiation+"*"),
                                                            true,
                                                            qObjectMetaTypeInterface->defaultCtr,
                                                            qObjectMetaTypeInterface->copyCtr,
                                                            qObjectMetaTypeInterface->moveCtr,
                                                            qObjectMetaTypeInterface->dtor,
                                                            qObjectMetaTypeInterface->equals,
                                                            qObjectMetaTypeInterface->lessThan,
                                                            qObjectMetaTypeInterface->debugStream,
                                                            qObjectMetaTypeInterface->dataStreamOut,
                                                            qObjectMetaTypeInterface->dataStreamIn,
                                                            qObjectMetaTypeInterface->legacyRegisterOp,
                                                            qObjectMetaTypeInterface->size,
                                                            qObjectMetaTypeInterface->alignment,
                                                            0,
                                                            QMetaType::TypeFlags(qObjectMetaTypeInterface->flags),
                                                            registeredOriginalMetaObject(*t),
                                                            nullptr);
                    memberMetaType.id();
                    registerJavaClassForCustomMetaType(_env, memberMetaType, clazz);
                    registerConverterVariant(_env, memberMetaType, QLatin1String(memberMetaType.name()), javaName, clazz);
                }
            }
        }
        QByteArray className = QtJambiAPI::getClassName(_env, externalClass).replace(".", "/").toLatin1();
        externalClass = getGlobalClassRef(_env, externalClass, className);
        return [className, externalClass, memberMetaTypeId = memberMetaType.id(), internalMetaType = QMetaType::fromName("QQmlListProperty<QObject>")](JNIEnv* env, QtJambiScope*, const void* in, jvalue& p, bool)->bool{
            if(in){
                bool ok = false;
                p.l = convertNativeToJavaObjectOfClass(env, in, className, externalClass, nullptr, internalMetaType, NativeToJavaConversionMode::MakeCopyOfValues, &ok);
                QMetaType memberMetaType(memberMetaTypeId);
                if(p.l && memberMetaType.isValid()){
                    QtJambiAPI::setQQmlListPropertyElementType(env, p.l, qtjambi_cast<jobject>(env, memberMetaType));
                }
            }
            return true;
        };
    }else if( (
                (
                    internalTypeName.startsWith("QList<")
                    &&
                    ( Java::QtCore::QList::isAssignableFrom(_env,externalClass)
                      || Java::Runtime::List::isSameClass(_env,externalClass)
                      || Java::Runtime::Collection::isSameClass(_env,externalClass)
                      || Java::Runtime::Object::isSameClass(_env,externalClass))
                )
#if QT_VERSION >= QT_VERSION_CHECK(6,7,0)
                ||
                (
                  (internalTypeName.startsWith("QSpan<") || internalTypeName.startsWith("std::span<"))
                  &&
                  Java::QtCore::AbstractSpan::isAssignableFrom(_env,externalClass)
                )
#endif
                ||
                (
                    internalTypeName.startsWith("QQueue<")
                    &&
                    ( Java::QtCore::QQueue::isAssignableFrom(_env,externalClass)
                      || Java::QtCore::QList::isSameClass(_env,externalClass)
                      || Java::Runtime::Queue::isSameClass(_env,externalClass)
                      || Java::Runtime::Collection::isSameClass(_env,externalClass)
                      || Java::Runtime::Object::isSameClass(_env,externalClass))
                )
                ||
                (
                    internalTypeName.startsWith("QStack<")
                    &&
                    ( Java::QtCore::QStack::isAssignableFrom(_env,externalClass)
                     || Java::QtCore::QList::isSameClass(_env,externalClass)
                     || Java::Runtime::Deque::isSameClass(_env,externalClass)
                     || Java::Runtime::Collection::isSameClass(_env,externalClass)
                     || Java::Runtime::Object::isSameClass(_env,externalClass))
                )
                ||
                (
                    internalTypeName.startsWith("QSet<")
                    &&
                    ( Java::QtCore::QSet::isAssignableFrom(_env,externalClass)
                     || Java::Runtime::Set::isSameClass(_env,externalClass)
                     || Java::Runtime::Collection::isSameClass(_env,externalClass)
                     || Java::Runtime::Object::isSameClass(_env,externalClass))
                )
             ) && (internalTypeName.endsWith(">")
                   || (pointerType!=PointerType::NoPointer && internalTypeName.endsWith(">*")))){
        auto idx = internalTypeName.indexOf("<");
        SequentialContainerType containerType;
        if(internalTypeName.startsWith("QStack<")){
            containerType = SequentialContainerType::QStack;
        }else if(internalTypeName.startsWith("QQueue<")){
            containerType = SequentialContainerType::QQueue;
        }else if(internalTypeName.startsWith("QSet<")){
            containerType = SequentialContainerType::QSet;
#if QT_VERSION >= QT_VERSION_CHECK(6,7,0)
        }else if(internalTypeName.startsWith("QSpan<const ")
                 || internalTypeName.startsWith("std::span<const ")){
            containerType = SequentialContainerType::QConstSpan;
        }else if(internalTypeName.startsWith("QSpan<")
                 || internalTypeName.startsWith("std::span<")){
            containerType = SequentialContainerType::QSpan;
#endif
        }else{
            containerType = SequentialContainerType::QList;
        }
        QString container = internalTypeName.mid(0, idx);
        Q_UNUSED(container)
        QString instantiation = internalTypeName.mid(idx+1).chopped(1).trimmed();
        QByteArray clazz;
        QMetaType memberMetaType(QMetaType::UnknownType);
        bool isPointer = isPointerType(instantiation);
        QPair<size_t,size_t> sizeAlign = {0,0};
        const std::type_info* t;
        if(isPointer){
            t = getTypeByQtName(instantiation.chopped(1));
            memberMetaType = QMetaType::fromName(qPrintable(instantiation));
            if(t){
                clazz = getJavaName(*t);
            }else{
                if(!memberMetaType.isValid()){
                    clazz = getExternalTypeName(_env, instantiation, memberMetaType).toUtf8();
                }else{
                    clazz = getExternalTypeName(_env, memberMetaType.name(), memberMetaType).toUtf8();
                }
            }
        }else{
            t = getTypeByQtName(instantiation);
            if(t){
                clazz = getJavaName(*t);
                memberMetaType = registeredMetaType(*t);
                if(memberMetaType.isValid()){
                    auto sz = memberMetaType.sizeOf();
                    sizeAlign.first = sz<0 ? 0 : size_t(sz);
                    if(memberMetaType.alignOf()!=0)
                        sizeAlign.second = memberMetaType.alignOf();
                }else{
                    sizeAlign = getValueSizeAndAlignment(*t);
                }
            }else{
                memberMetaType = QMetaType::fromName(qPrintable(instantiation));
                if(memberMetaType.isValid()){
                    auto sz = memberMetaType.sizeOf();
                    sizeAlign.first = sz<0 ? 0 : size_t(sz);
                    if(memberMetaType.alignOf()!=0)
                        sizeAlign.second = memberMetaType.alignOf();
                }
                if(sizeAlign.first==0){
                    sizeAlign.first = getInternalSize(instantiation);
                }
                if(sizeAlign.second==0){
                    sizeAlign.second = getInternalAlignment(instantiation);
                }
                if(!memberMetaType.isValid()){
                    clazz = getExternalTypeName(_env, instantiation, memberMetaType).toUtf8();
                }else{
                    clazz = getExternalTypeName(_env, memberMetaType.name(), memberMetaType).toUtf8();
                }
            }
        }

        if(!isPointer && !memberMetaType.isValid()){
            memberMetaType = QMetaType::fromName(qPrintable(instantiation));
            if(memberMetaType.isValid()){
                auto sz = memberMetaType.sizeOf();
                sizeAlign.first = sz<0 ? 0 : size_t(sz);
                if(memberMetaType.alignOf()!=0)
                    sizeAlign.second = memberMetaType.alignOf();
            }
            if(sizeAlign.first==0){
                sizeAlign.first = getInternalSize(instantiation);
            }
            if(sizeAlign.second==0){
                sizeAlign.second = getInternalAlignment(instantiation);
            }
        }
        AbstractContainerAccess* containerAccess = ContainerAccessAPI::createContainerAccess(containerType, memberMetaType);

        if(!clazz.isEmpty()){
            jclass elementClass = JavaAPI::resolveClass(_env, clazz);
            if(!containerAccess){
                QtJambiUtils::InternalToExternalConverter memberConverter = getInternalToExternalConverter(
                                                                _env,
                                                                instantiation,
                                                                memberMetaType,
                                                                elementClass);
                QtJambiUtils::ExternalToInternalConverter memberReConverter = getExternalToInternalConverter(
                                                                _env,
                                                                elementClass,
                                                                instantiation, memberMetaType);
                PtrOwnerFunction memberOwnerFunction = nullptr;
                if(!isPointer){
                    const std::type_info* typeId = getTypeByQtName(qPrintable(instantiation));
                    if(!typeId){
                        typeId = getTypeByMetaType(memberMetaType);
                    }
                    if(typeId)
                        memberOwnerFunction = ContainerAPI::registeredOwnerFunction(*typeId);
                    if(!memberMetaType.isValid()){
                        memberMetaType = QMetaType::fromName(qPrintable(instantiation));
                        sizeAlign.second = memberMetaType.alignOf();
                        sizeAlign.first = memberMetaType.sizeOf();
                        containerAccess = ContainerAccessAPI::createContainerAccess(containerType, memberMetaType);
                    }
                }else{
                    if(!memberMetaType.isValid()){
                        const std::type_info* typeId = nullptr;
                        {
                            QString _internalTypeName = instantiation;
                            if(_internalTypeName.startsWith("const "))
                                _internalTypeName = _internalTypeName.mid(6);
                            if(_internalTypeName.endsWith("*") || _internalTypeName.endsWith("&"))
                                _internalTypeName.chop(1);
                            typeId = getTypeByQtName(_internalTypeName);
                        }
                        if(!typeId){
                            typeId = getTypeByJavaName(QtJambiAPI::getClassName(_env, externalClass).replace(".", "/"));
                        }
                        const QMetaObject *metaObject = nullptr;
                        if(typeId)
                            metaObject = registeredOriginalMetaObject(*typeId);
                        QMetaType::TypeFlags flags;
#if QT_VERSION < QT_VERSION_CHECK(6, 9, 0)
                        if(Java::QtCore::QObject::isAssignableFrom(_env, externalClass)){
                            flags = QMetaType::TypeFlags(QtPrivate::QMetaTypeTypeFlags<QObject*>::Flags);
                        }else{
                            flags = QMetaType::TypeFlags(QtPrivate::QMetaTypeTypeFlags<void*>::Flags);
#else
                        if(Java::QtCore::QObject::isAssignableFrom(_env, externalClass)){
                            flags = QMetaType::TypeFlags(QtPrivate::QMetaTypeForType<QObject*>::flags());
                        }else{
                            flags = QMetaType::TypeFlags(QtPrivate::QMetaTypeForType<void*>::flags());
#endif
                            if(metaObject)
                                flags.setFlag(QMetaType::PointerToGadget);
                        }
                        memberMetaType = registerMetaTypeImpl(nullptr,
                                                                             nullptr,
                                                                             instantiation.toUtf8(),
                                                                          /*.defaultCtr=*/ QtPrivate::QMetaTypeForType<void*>::getDefaultCtr(),
                                                                          /*.copyCtr=*/ QtPrivate::QMetaTypeForType<void*>::getCopyCtr(),
                                                                          /*.moveCtr=*/ QtPrivate::QMetaTypeForType<void*>::getMoveCtr(),
                                                                          /*.dtor=*/ QtPrivate::QMetaTypeForType<void*>::getDtor(),
                                                                          /*.equals=*/ QtPrivate::QEqualityOperatorForType<void*>::equals,
                                                                          /*.lessThan=*/ QtPrivate::QLessThanOperatorForType<void*>::lessThan,
                                                                          /*.debugStream=*/ QtPrivate::QDebugStreamOperatorForType<void*>::debugStream,
                                                                          /*.dataStreamOut=*/ QtPrivate::QDataStreamOperatorForType<void*>::dataStreamOut,
                                                                          /*.dataStreamIn=*/ QtPrivate::QDataStreamOperatorForType<void*>::dataStreamIn,
                                                                          /*.legacyRegisterOp=*/ QtPrivate::QMetaTypeForType<void*>::getLegacyRegister(),
                                                                             uint(sizeAlign.first = sizeof(void*)),
                                                                             ushort(sizeAlign.second = alignof(void*)),
                                                                             0,
                                                                             flags,
                                                                             metaObject,
                                                                             nullptr,
                                                                             nullptr,
                                                                            {});
                    }
                }

                if(!containerAccess && memberMetaType.isValid()){
                    QtJambiUtils::QHashFunction hashFunction = QtJambiTypeManager::findHashFunction(isPointer, memberMetaType);
                    if(containerType == SequentialContainerType::QSet
                            && !hashFunction){
                        if(clazz.startsWith("io/qt/core/QMetaType$Generic")){
                            hashFunction = [](const void*, size_t seed) -> size_t{ return seed; };
                        }else{
                            Java::Runtime::IllegalArgumentException::throwNew(_env, QStringLiteral("Unable to use %1 in QSet.").arg(QLatin1String(clazz.replace('/', '.').replace('$', '.'))) QTJAMBI_STACKTRACEINFO );
                        }
                    }
                    QSharedPointer<AbstractContainerAccess> memberNestedContainerAccess = findContainerAccess(memberMetaType);
                    containerAccess = ContainerAccessAPI::createContainerAccess(_env, containerType, memberMetaType,
                                                                      sizeAlign.second, sizeAlign.first,
                                                                      isPointer,
                                                                      hashFunction,
                                                                      memberConverter,
                                                                      memberReConverter,
                                                                      memberNestedContainerAccess,
                                                                      memberOwnerFunction);
                }
            }
            if(containerAccess){
#if defined(QTJAMBI_GENERIC_ACCESS)
                switch(containerType){
                case SequentialContainerType::QStack:
#if QT_VERSION >= QT_VERSION_CHECK(6,7,0)
                case SequentialContainerType::QSpan:
                case SequentialContainerType::QConstSpan:
                    if(pointerType!=PointerType::NoPointer)
                        return ParameterTypeInfo::default_internalToExternalConverter();
                    containerAccess = dynamic_cast<AbstractSpanAccess*>(containerAccess);
                    break;
#endif
                case SequentialContainerType::QQueue:
                case SequentialContainerType::QList:
                    containerAccess = checkContainerAccess(_env, dynamic_cast<AbstractListAccess*>(containerAccess));
                    break;
                case SequentialContainerType::QSet:
                    containerAccess = checkContainerAccess(_env, dynamic_cast<AbstractSetAccess*>(containerAccess));
                    break;
                default:
                    return ParameterTypeInfo::default_internalToExternalConverter();
                }
#endif //defined(QTJAMBI_GENERIC_ACCESS)
                if(!containerAccess)
                    return ParameterTypeInfo::default_internalToExternalConverter();
                QSharedPointer<AbstractContainerAccess> sharedAccess(containerAccess, &containerDisposer);
                switch(pointerType){
                case PointerType::NoPointer:{
                    return [LINK_NAME_ARG(internalTypeName) containerType, sharedAccess]
                            (JNIEnv* env, QtJambiScope*, const void* in, jvalue& p, bool)->bool{
                        if(in){
                            AbstractContainerAccess* access = sharedAccess->clone();
                            switch(containerType){
#if QT_VERSION >= QT_VERSION_CHECK(6,7,0)
                            case SequentialContainerType::QSpan:
                                p.l = Java::QtCore::QSpan::newInstance(env, nullptr, nullptr);
                                break;
                            case SequentialContainerType::QConstSpan:
                                p.l = Java::QtCore::QConstSpan::newInstance(env, nullptr, nullptr);
                                break;
#endif
                            case SequentialContainerType::QStack:
                                p.l = Java::QtCore::QStack::newInstance(env, nullptr);
                                break;
                            case SequentialContainerType::QQueue:
                                p.l = Java::QtCore::QQueue::newInstance(env, nullptr);
                                break;
                            case SequentialContainerType::QSet:
                                p.l = Java::QtCore::QSet::newInstance(env, nullptr);
                                break;
                            default:
                                p.l = Java::QtCore::QList::newInstance(env, nullptr);
                                break;
                            }
                            void* ptr = access->createContainer(in);
                            QSharedPointer<QtJambiLink> link = QtJambiLink::createLinkForNativeObject(env, p.l, ptr,
                                                                                                      LINK_NAME_ARG(internalTypeName.toLatin1())
                                                                                                      false/*created_by_java*/, true/*copy*/, access, QtJambiLink::Ownership::Java);
                            if (!link) {
                                p.l = nullptr;
                                if(access)
                                    access->dispose();
                            }
                            return !!link;
                        }
                        return true;
                    };
                }break;
                case PointerType::SharedPointer:{
                    return [containerType, sharedAccess](JNIEnv* env, QtJambiScope*, const void* in, jvalue& p, bool)->bool{
                        const QSharedPointer<char>* ptr = reinterpret_cast<const QSharedPointer<char>*>(in);
                        if(ptr){
                            AbstractContainerAccess* access = sharedAccess->clone();
                            switch(containerType){
                            case SequentialContainerType::QQueue:
                                p.l = QtJambiAPI::convertQListToJavaObject(env, *ptr, QtJambiAPI::ListType::QQueue, static_cast<AbstractListAccess*>(access));
                                break;
                            case SequentialContainerType::QSet:
                                p.l = QtJambiAPI::convertQSetToJavaObject(env, *ptr, static_cast<AbstractSetAccess*>(access));
                                break;
                            case SequentialContainerType::QStack:
                                p.l = QtJambiAPI::convertQListToJavaObject(env, *ptr, QtJambiAPI::ListType::QStack, static_cast<AbstractListAccess*>(access));
                                break;
                            default:
                                p.l = QtJambiAPI::convertQListToJavaObject(env, *ptr, QtJambiAPI::ListType::QList, static_cast<AbstractListAccess*>(access));
                                break;
                            }
                        }
                        return true;
                    };
                }break;
                case PointerType::shared_ptr:{
                    return [containerType, sharedAccess](JNIEnv* env, QtJambiScope*, const void* in, jvalue& p, bool)->bool{
                        const std::shared_ptr<char>* ptr = reinterpret_cast<const std::shared_ptr<char>*>(in);
                        if(ptr){
                            AbstractContainerAccess* access = sharedAccess->clone();
                            switch(containerType){
                            case SequentialContainerType::QQueue:
                                p.l = QtJambiAPI::convertQListToJavaObject(env, *ptr, QtJambiAPI::ListType::QQueue, static_cast<AbstractListAccess*>(access));
                                break;
                            case SequentialContainerType::QSet:
                                p.l = QtJambiAPI::convertQSetToJavaObject(env, *ptr, static_cast<AbstractSetAccess*>(access));
                                break;
                            case SequentialContainerType::QStack:
                                p.l = QtJambiAPI::convertQListToJavaObject(env, *ptr, QtJambiAPI::ListType::QStack, static_cast<AbstractListAccess*>(access));
                                break;
                            default:
                                p.l = QtJambiAPI::convertQListToJavaObject(env, *ptr, QtJambiAPI::ListType::QList, static_cast<AbstractListAccess*>(access));
                                break;
                            }
                        }
                        return true;
                    };
                }break;
                case PointerType::WeakPointer:{
                    return [containerType, sharedAccess](JNIEnv* env, QtJambiScope*, const void* in, jvalue& p, bool)->bool{
                        const QWeakPointer<char>* ptr = reinterpret_cast<const QWeakPointer<char>*>(in);
                        if(ptr){
                            if(const QSharedPointer<char> _ptr = *ptr){
                                AbstractContainerAccess* access = sharedAccess->clone();
                                switch(containerType){
                                case SequentialContainerType::QQueue:
                                    p.l = QtJambiAPI::convertQListToJavaObject(env, _ptr, QtJambiAPI::ListType::QQueue, static_cast<AbstractListAccess*>(access));
                                    break;
                                case SequentialContainerType::QSet:
                                    p.l = QtJambiAPI::convertQSetToJavaObject(env, _ptr, static_cast<AbstractSetAccess*>(access));
                                    break;
                                case SequentialContainerType::QStack:
                                    p.l = QtJambiAPI::convertQListToJavaObject(env, _ptr, QtJambiAPI::ListType::QStack, static_cast<AbstractListAccess*>(access));
                                    break;
                                default:
                                    p.l = QtJambiAPI::convertQListToJavaObject(env, _ptr, QtJambiAPI::ListType::QList, static_cast<AbstractListAccess*>(access));
                                    break;
                                }
                            }
                        }
                        return true;
                    };
                }break;
                case PointerType::weak_ptr:{
                    return [containerType, sharedAccess](JNIEnv* env, QtJambiScope*, const void* in, jvalue& p, bool)->bool{
                        const std::weak_ptr<char>* ptr = reinterpret_cast<const std::weak_ptr<char>*>(in);
                        if(ptr){
                            if(const std::shared_ptr<char> _ptr{*ptr}){
                                AbstractContainerAccess* access = sharedAccess->clone();
                                switch(containerType){
                                case SequentialContainerType::QQueue:
                                    p.l = QtJambiAPI::convertQListToJavaObject(env, _ptr, QtJambiAPI::ListType::QQueue, static_cast<AbstractListAccess*>(access));
                                    break;
                                case SequentialContainerType::QSet:
                                    p.l = QtJambiAPI::convertQSetToJavaObject(env, _ptr, static_cast<AbstractSetAccess*>(access));
                                    break;
                                case SequentialContainerType::QStack:
                                    p.l = QtJambiAPI::convertQListToJavaObject(env, _ptr, QtJambiAPI::ListType::QStack, static_cast<AbstractListAccess*>(access));
                                    break;
                                default:
                                    p.l = QtJambiAPI::convertQListToJavaObject(env, _ptr, QtJambiAPI::ListType::QList, static_cast<AbstractListAccess*>(access));
                                    break;
                                }
                            }
                        }
                        return true;
                    };
                }break;
                default:
                    return ParameterTypeInfo::default_internalToExternalConverter();
                }
            }
        }
        return ParameterTypeInfo::default_internalToExternalConverter();
    }else if((internalMetaType.id()==QMetaType::QVariantMap
              || internalTypeName=="QVariantMap"
              || (pointerType!=PointerType::NoPointer && internalTypeName=="QVariantMap*")
              || internalTypeName=="QMap<QString,QVariant>"
              || (pointerType!=PointerType::NoPointer && internalTypeName=="QMap<QString,QVariant>*"))
             &&
             ( Java::QtCore::QMap::isAssignableFrom(_env,externalClass)
               || Java::Runtime::NavigableMap::isSameClass(_env,externalClass)
               || Java::Runtime::Map::isSameClass(_env,externalClass)
               || Java::Runtime::Object::isSameClass(_env,externalClass))
             ){
        switch(pointerType){
        case PointerType::NoPointer:{
            return [](JNIEnv* env, QtJambiScope*, const void* in, jvalue& p, bool)->bool{
                if(in){
                    p.l = qtjambi_cast<jobject>(env, *reinterpret_cast<const QVariantMap*>(in));
                }
                return true;
            };
        }break;
        case PointerType::SharedPointer:{
            return [](JNIEnv* env, QtJambiScope*, const void* in, jvalue& p, bool)->bool{
                const QSharedPointer<QVariantMap>* ptr = reinterpret_cast<const QSharedPointer<QVariantMap>*>(in);
                if(ptr){
                    p.l = QtJambiAPI::convertSmartPointerToJavaObject(env, *ptr);
                }
                return true;
            };
        }break;
        case PointerType::shared_ptr:{
            return [](JNIEnv* env, QtJambiScope*, const void* in, jvalue& p, bool)->bool{
                const std::shared_ptr<QVariantMap>* ptr = reinterpret_cast<const std::shared_ptr<QVariantMap>*>(in);
                if(ptr){
                    p.l = QtJambiAPI::convertSmartPointerToJavaObject(env, *ptr);
                }
                return true;
            };
        }break;
        case PointerType::WeakPointer:{
            return [](JNIEnv* env, QtJambiScope*, const void* in, jvalue& p, bool)->bool{
                const QWeakPointer<QVariantMap>* ptr = reinterpret_cast<const QWeakPointer<QVariantMap>*>(in);
                if(ptr){
                    if(const QSharedPointer<QVariantMap> _ptr{*ptr})
                        p.l = QtJambiAPI::convertSmartPointerToJavaObject(env, _ptr);
                }
                return true;
            };
        }break;
        case PointerType::weak_ptr:{
            return [](JNIEnv* env, QtJambiScope*, const void* in, jvalue& p, bool)->bool{
                const std::weak_ptr<QVariantMap>* ptr = reinterpret_cast<const std::weak_ptr<QVariantMap>*>(in);
                if(ptr){
                    if(const std::shared_ptr<QVariantMap> _ptr{*ptr})
                        p.l = QtJambiAPI::convertSmartPointerToJavaObject(env, _ptr);
                }
                return true;
            };
        }break;
        default:
            return ParameterTypeInfo::default_internalToExternalConverter();
        }
    }else if((internalMetaType.id()==QMetaType::QVariantHash
              || internalTypeName=="QVariantHash"
              || (pointerType!=PointerType::NoPointer && internalTypeName=="QVariantHash*")
              || internalTypeName=="QHash<QString,QVariant>"
              || (pointerType!=PointerType::NoPointer && internalTypeName=="QHash<QString,QVariant>*"))
             &&
             ( Java::QtCore::QHash::isAssignableFrom(_env,externalClass)
               || Java::Runtime::Map::isSameClass(_env,externalClass)
               || Java::Runtime::Object::isSameClass(_env,externalClass))
             ){
        switch(pointerType){
        case PointerType::NoPointer:{
            return [](JNIEnv* env, QtJambiScope*, const void* in, jvalue& p, bool)->bool{
                if(in){
                    p.l = qtjambi_cast<jobject>(env, *reinterpret_cast<const QVariantHash*>(in));
                }
                return true;
            };
        }break;
        case PointerType::SharedPointer:{
            return [](JNIEnv* env, QtJambiScope*, const void* in, jvalue& p, bool)->bool{
                const QSharedPointer<QVariantHash>* ptr = reinterpret_cast<const QSharedPointer<QVariantHash>*>(in);
                if(ptr){
                    p.l = QtJambiAPI::convertSmartPointerToJavaObject(env, *ptr);
                }
                return true;
            };
        }break;
        case PointerType::shared_ptr:{
            return [](JNIEnv* env, QtJambiScope*, const void* in, jvalue& p, bool)->bool{
                const std::shared_ptr<QVariantHash>* ptr = reinterpret_cast<const std::shared_ptr<QVariantHash>*>(in);
                if(ptr){
                    p.l = QtJambiAPI::convertSmartPointerToJavaObject(env, *ptr);
                }
                return true;
            };
        }break;
        case PointerType::WeakPointer:{
            return [](JNIEnv* env, QtJambiScope*, const void* in, jvalue& p, bool)->bool{
                const QWeakPointer<QVariantHash>* ptr = reinterpret_cast<const QWeakPointer<QVariantHash>*>(in);
                if(ptr){
                    if(const QSharedPointer<QVariantHash> _ptr{*ptr})
                        p.l = QtJambiAPI::convertSmartPointerToJavaObject(env, _ptr);
                }
                return true;
            };
        }break;
        case PointerType::weak_ptr:{
            return [](JNIEnv* env, QtJambiScope*, const void* in, jvalue& p, bool)->bool{
                const std::weak_ptr<QVariantHash>* ptr = reinterpret_cast<const std::weak_ptr<QVariantHash>*>(in);
                if(ptr){
                    if(const std::shared_ptr<QVariantHash> _ptr{*ptr})
                        p.l = QtJambiAPI::convertSmartPointerToJavaObject(env, _ptr);
                }
                return true;
            };
        }break;
        default:
            return ParameterTypeInfo::default_internalToExternalConverter();
        }
    }else if((internalMetaType.id()==QMetaType::QVariantPair
              || internalTypeName=="QVariantPair"
              || (pointerType!=PointerType::NoPointer && internalTypeName=="QVariantPair*")
              || internalTypeName=="QPair<QVariant,QVariant>"
              || (pointerType!=PointerType::NoPointer && internalTypeName=="QPair<QVariant,QVariant>*"))
             &&
             ( Java::QtCore::QPair::isSameClass(_env,externalClass)
               || Java::Runtime::Object::isSameClass(_env,externalClass))
             && pointerType==PointerType::NoPointer
             ){
        switch(pointerType){
        case PointerType::NoPointer:{
            return [](JNIEnv* env, QtJambiScope*, const void* in, jvalue& p, bool)->bool{
                if(in){
                    p.l = qtjambi_cast<jobject>(env, *reinterpret_cast<const QVariantPair*>(in));
                }
                return true;
            };
        }break;
        case PointerType::SharedPointer:{
            return [](JNIEnv* env, QtJambiScope*, const void* in, jvalue& p, bool)->bool{
                const QSharedPointer<QVariantPair>* ptr = reinterpret_cast<const QSharedPointer<QVariantPair>*>(in);
                if(ptr){
                    p.l = QtJambiAPI::convertSmartPointerToJavaObject(env, *ptr);
                }
                return true;
            };
        }break;
        case PointerType::shared_ptr:{
            return [](JNIEnv* env, QtJambiScope*, const void* in, jvalue& p, bool)->bool{
                const std::shared_ptr<QVariantPair>* ptr = reinterpret_cast<const std::shared_ptr<QVariantPair>*>(in);
                if(ptr){
                    p.l = QtJambiAPI::convertSmartPointerToJavaObject(env, *ptr);
                }
                return true;
            };
        }break;
        case PointerType::WeakPointer:{
            return [](JNIEnv* env, QtJambiScope*, const void* in, jvalue& p, bool)->bool{
                const QWeakPointer<QVariantPair>* ptr = reinterpret_cast<const QWeakPointer<QVariantPair>*>(in);
                if(ptr){
                    if(const QSharedPointer<QVariantPair> _ptr{*ptr})
                        p.l = QtJambiAPI::convertSmartPointerToJavaObject(env, _ptr);
                }
                return true;
            };
        }break;
        case PointerType::weak_ptr:{
            return [](JNIEnv* env, QtJambiScope*, const void* in, jvalue& p, bool)->bool{
                const std::weak_ptr<QVariantPair>* ptr = reinterpret_cast<const std::weak_ptr<QVariantPair>*>(in);
                if(ptr){
                    if(const std::shared_ptr<QVariantPair> _ptr{*ptr})
                        p.l = QtJambiAPI::convertSmartPointerToJavaObject(env, _ptr);
                }
                return true;
            };
        }break;
        default:
            return ParameterTypeInfo::default_internalToExternalConverter();
        }
    }else if( ( (
                    internalTypeName.startsWith("QMap<")
                    &&
                    ( Java::QtCore::QMap::isAssignableFrom(_env,externalClass)
                      || Java::Runtime::NavigableMap::isSameClass(_env,externalClass)
                      || Java::Runtime::Map::isSameClass(_env,externalClass)
                      || Java::Runtime::Object::isSameClass(_env,externalClass))
                )
                ||
                (
                    internalTypeName.startsWith("QMultiMap<")
                    &&
                    ( Java::QtCore::QMultiMap::isAssignableFrom(_env,externalClass)
                      || Java::Runtime::NavigableMap::isSameClass(_env,externalClass)
                      || Java::Runtime::Map::isSameClass(_env,externalClass)
                      || Java::Runtime::Object::isSameClass(_env,externalClass))
                )
                ||
                (
                    internalTypeName.startsWith("QHash<")
                    &&
                    (Java::QtCore::QHash::isAssignableFrom(_env,externalClass)
                     || Java::Runtime::Map::isSameClass(_env,externalClass)
                     || Java::Runtime::Object::isSameClass(_env,externalClass))
                )
                ||
                (
                    internalTypeName.startsWith("QMultiHash<")
                    &&
                    (Java::QtCore::QMultiHash::isAssignableFrom(_env,externalClass)
                     || Java::Runtime::Map::isSameClass(_env,externalClass)
                     || Java::Runtime::Object::isSameClass(_env,externalClass))
                )
                ||
                (
                    (internalTypeName.startsWith("QPair<") || internalTypeName.startsWith("std::pair<"))
                    &&
                    (Java::QtCore::QPair::isSameClass(_env,externalClass)
                     || Java::Runtime::Object::isSameClass(_env,externalClass))
                )
             ) && (internalTypeName.endsWith(">")
                   || (pointerType!=PointerType::NoPointer && internalTypeName.endsWith(">*")))){
        auto idx = internalTypeName.indexOf("<");
        AssociativeContainerType mapType;
        if(internalTypeName.startsWith("QMap<")){
            mapType = AssociativeContainerType::QMap;
        }else if(internalTypeName.startsWith("QMultiMap<")){
             mapType = AssociativeContainerType::QMultiMap;
        }else if(internalTypeName.startsWith("QMultiHash<")){
             mapType = AssociativeContainerType::QMultiHash;
        }else if(internalTypeName.startsWith("QPair<") || internalTypeName.startsWith("std::pair<")){
             mapType = AssociativeContainerType::QPair;
        }else{
            mapType = AssociativeContainerType::QHash;
        }
        QString container = internalTypeName.mid(0, idx);
        Q_UNUSED(container)
        QStringList instantiations = internalTypeName.mid(idx+1).chopped(1).trimmed().split(",");
        reduceMapInstantiations(instantiations);
        if(instantiations.size()==2){
            const std::type_info* t1;
            bool isPointer1 = isPointerType(instantiations[0]);
            if(isPointer1){
                t1 = getTypeByQtName(instantiations[0].chopped(1));
            }else{
                t1 = getTypeByQtName(instantiations[0]);
            }
            QByteArray clazz1;
            QPair<size_t,size_t> sizeAlign1 = {0,0};
            QMetaType memberMetaType1(QMetaType::UnknownType);
            if(isPointer1){
                memberMetaType1 = QMetaType(QMetaType::type(qPrintable(instantiations[0])));
                if(t1){
                    clazz1 = getJavaName(*t1);
                }else{
                    if(!memberMetaType1.isValid()){
                        clazz1 = getExternalTypeName(_env, instantiations[0], memberMetaType1).toUtf8();
                    }else{
                        clazz1 = getExternalTypeName(_env, memberMetaType1.name(), memberMetaType1).toUtf8();
                    }
                }
            }else{
                if(t1){
                    clazz1 = getJavaName(*t1);
                    memberMetaType1 = registeredMetaType(*t1);
                    if(memberMetaType1.isValid()){
                        auto sz = memberMetaType1.sizeOf();
                        sizeAlign1.first = sz<0 ? 0 : size_t(sz);
                        if(memberMetaType1.alignOf()!=0)
                            sizeAlign1.second = memberMetaType1.alignOf();
                    }else{
                        sizeAlign1 = getValueSizeAndAlignment(*t1);
                    }
                }
                if(!memberMetaType1.isValid()){
                    memberMetaType1 = QMetaType::fromName(qPrintable(instantiations[0]));
                    if(memberMetaType1.isValid()){
                        auto sz = memberMetaType1.sizeOf();
                        sizeAlign1.first = sz<0 ? 0 : size_t(sz);
                        if(memberMetaType1.alignOf()!=0)
                            sizeAlign1.second = memberMetaType1.alignOf();
                    }
                    if(sizeAlign1.first==0){
                        sizeAlign1.first = getInternalSize(instantiations[0]);
                    }
                    if(sizeAlign1.second==0){
                        sizeAlign1.second = getInternalAlignment(instantiations[0]);
                    }
                    if(!memberMetaType1.isValid()){
                        clazz1 = getExternalTypeName(_env, instantiations[0], memberMetaType1).toUtf8();
                    }else{
                        clazz1 = getExternalTypeName(_env, memberMetaType1.name(), memberMetaType1).toUtf8();
                    }
                }
            }
            const std::type_info* t2;
            bool isPointer2 = isPointerType(instantiations[1]);
            if(isPointer2){
                t2 = getTypeByQtName(instantiations[1].chopped(1));
            }else{
                t2 = getTypeByQtName(instantiations[1]);
            }
            QByteArray clazz2;
            QPair<size_t,size_t> sizeAlign2 = {0,0};
            QMetaType memberMetaType2(QMetaType::UnknownType);
            if(isPointer2){
                memberMetaType2 = QMetaType::fromName(qPrintable(instantiations[1]));
                if(t2){
                    clazz2 = getJavaName(*t2);
                }else{
                    if(!memberMetaType2.isValid()){
                        clazz2 = getExternalTypeName(_env, instantiations[1], memberMetaType2).toUtf8();
                    }else{
                        clazz2 = getExternalTypeName(_env, memberMetaType2.name(), memberMetaType2).toUtf8();
                    }
                }
            }else{
                if(t2){
                    clazz2 = getJavaName(*t2);
                    sizeAlign2 = getValueSizeAndAlignment(*t2);
                    memberMetaType2 = registeredMetaType(*t2);
                    if(sizeAlign2.first==0 && memberMetaType2.isValid()){
                        auto sz = memberMetaType2.sizeOf();
                        sizeAlign2.first = sz<0 ? 0 : size_t(sz);
                        if(memberMetaType2.alignOf()!=0)
                            sizeAlign2.second = size_t(memberMetaType2.alignOf());
                    }
                }else{
                    memberMetaType2 = QMetaType(QMetaType::type(qPrintable(instantiations[1])));
                    if(memberMetaType2.isValid()){
                        auto sz = memberMetaType2.sizeOf();
                        sizeAlign2.first = sz<0 ? 0 : size_t(sz);
                        if(memberMetaType2.alignOf()!=0)
                            sizeAlign2.second = size_t(memberMetaType2.alignOf());
                    }
                    if(sizeAlign2.first==0){
                        sizeAlign2.first = getInternalSize(instantiations[1]);
                    }
                    if(sizeAlign2.second==0){
                        sizeAlign2.second = getInternalAlignment(instantiations[1]);
                    }
                    if(!memberMetaType2.isValid()){
                        clazz2 = getExternalTypeName(_env, instantiations[1], memberMetaType2).toUtf8();
                    }else{
                        clazz2 = getExternalTypeName(_env, memberMetaType2.name(), memberMetaType2).toUtf8();
                    }
                }
            }

            if(!clazz1.isEmpty() && !clazz2.isEmpty()){
                jclass keyClass = JavaAPI::resolveClass(_env, clazz1);
                if(!isPointer1 && !memberMetaType1.isValid()){
                    memberMetaType1 = QMetaType::fromName(qPrintable(instantiations[0]));
                    if(memberMetaType1.isValid()){
                        auto sz = memberMetaType1.sizeOf();
                        sizeAlign1.first = sz<0 ? 0 : size_t(sz);
                        if(memberMetaType1.alignOf()!=0)
                            sizeAlign1.second = size_t(memberMetaType1.alignOf());
                    }
                    if(sizeAlign1.first==0){
                        sizeAlign1.first = getInternalSize(instantiations[0]);
                    }
                    if(sizeAlign1.second==0){
                        sizeAlign1.second = getInternalAlignment(instantiations[0]);
                    }
                }
                jclass valueClass = JavaAPI::resolveClass(_env, clazz2);
                if(!isPointer2 && !memberMetaType2.isValid()){
                    memberMetaType2 = QMetaType::fromName(qPrintable(instantiations[1]));
                    if(memberMetaType2.isValid()){
                        auto sz = memberMetaType2.sizeOf();
                        sizeAlign2.first = sz<0 ? 0 : size_t(sz);
                        if(memberMetaType2.alignOf()!=0)
                            sizeAlign2.second = size_t(memberMetaType2.alignOf());
                    }
                    if(sizeAlign2.first==0){
                        sizeAlign2.first = getInternalSize(instantiations[1]);
                    }
                    if(sizeAlign2.second==0){
                        sizeAlign2.second = getInternalAlignment(instantiations[1]);
                    }
                }
                AbstractContainerAccess* containerAccess = ContainerAccessAPI::createContainerAccess(mapType, memberMetaType1, memberMetaType2);
                if(!containerAccess){
                    QtJambiUtils::InternalToExternalConverter memberConverter1 = getInternalToExternalConverter(
                                                                    _env,
                                                                    instantiations[0],
                                                                    memberMetaType1,
                                                                    keyClass);
                    QtJambiUtils::ExternalToInternalConverter memberReConverter1 = getExternalToInternalConverter(
                                                                    _env,
                                                                    keyClass,
                                                                    instantiations[0], memberMetaType1);
                    QtJambiUtils::InternalToExternalConverter memberConverter2 = getInternalToExternalConverter(
                                                                    _env,
                                                                    instantiations[1],
                                                                    memberMetaType2,
                                                                    valueClass);
                    QtJambiUtils::ExternalToInternalConverter memberReConverter2 = getExternalToInternalConverter(
                                                                    _env,
                                                                    valueClass,
                                                                    instantiations[1], memberMetaType2);
                    bool tryAgain = false;
                    if(!memberMetaType1.isValid()){
                        const std::type_info* typeId = nullptr;
                        {
                            QString _internalTypeName = instantiations[0];
                            if(_internalTypeName.startsWith("const "))
                                _internalTypeName = _internalTypeName.mid(6);
                            if(_internalTypeName.endsWith("*") || _internalTypeName.endsWith("&"))
                                _internalTypeName.chop(1);
                            typeId = getTypeByQtName(_internalTypeName);
                        }
                        if(!typeId){
                            typeId = getTypeByJavaName(QtJambiAPI::getClassName(_env, externalClass).replace(".", "/"));
                        }
                        const QMetaObject *metaObject = nullptr;
                        if(typeId)
                            metaObject = registeredOriginalMetaObject(*typeId);
                        QMetaType::TypeFlags flags;
#if QT_VERSION < QT_VERSION_CHECK(6, 9, 0)
                        if(Java::QtCore::QObject::isAssignableFrom(_env, externalClass)){
                            flags = QMetaType::TypeFlags(QtPrivate::QMetaTypeTypeFlags<QObject*>::Flags);
                        }else{
                            flags = QMetaType::TypeFlags(QtPrivate::QMetaTypeTypeFlags<void*>::Flags);
#else
                        if(Java::QtCore::QObject::isAssignableFrom(_env, externalClass)){
                            flags = QMetaType::TypeFlags(QtPrivate::QMetaTypeForType<QObject*>::flags());
                        }else{
                            flags = QMetaType::TypeFlags(QtPrivate::QMetaTypeForType<void*>::flags());
#endif
                            if(metaObject)
                                flags.setFlag(QMetaType::PointerToGadget);
                        }
                        memberMetaType1 = registerMetaTypeImpl(nullptr,
                                                                            nullptr,
                                                                            instantiations[0].toUtf8(),
                                                                          /*.defaultCtr=*/ QtPrivate::QMetaTypeForType<void*>::getDefaultCtr(),
                                                                          /*.copyCtr=*/ QtPrivate::QMetaTypeForType<void*>::getCopyCtr(),
                                                                          /*.moveCtr=*/ QtPrivate::QMetaTypeForType<void*>::getMoveCtr(),
                                                                          /*.dtor=*/ QtPrivate::QMetaTypeForType<void*>::getDtor(),
                                                                          /*.equals=*/ QtPrivate::QEqualityOperatorForType<void*>::equals,
                                                                          /*.lessThan=*/ QtPrivate::QLessThanOperatorForType<void*>::lessThan,
                                                                          /*.debugStream=*/ QtPrivate::QDebugStreamOperatorForType<void*>::debugStream,
                                                                          /*.dataStreamOut=*/ QtPrivate::QDataStreamOperatorForType<void*>::dataStreamOut,
                                                                          /*.dataStreamIn=*/ QtPrivate::QDataStreamOperatorForType<void*>::dataStreamIn,
                                                                          /*.legacyRegisterOp=*/ QtPrivate::QMetaTypeForType<void*>::getLegacyRegister(),
                                                                             uint(sizeAlign1.first = sizeof(void*)),
                                                                             ushort(sizeAlign1.second = alignof(void*)),
                                                                             0,
                                                                             flags,
                                                                             metaObject,
                                                                             nullptr,
                                                                             nullptr,
                                                                            {});
                        tryAgain = true;
                    }
                    if(!memberMetaType2.isValid()){
                        const std::type_info* typeId = nullptr;
                        {
                            QString _internalTypeName = instantiations[1];
                            if(_internalTypeName.startsWith("const "))
                                _internalTypeName = _internalTypeName.mid(6);
                            if(_internalTypeName.endsWith("*") || _internalTypeName.endsWith("&"))
                                _internalTypeName.chop(1);
                            typeId = getTypeByQtName(_internalTypeName);
                        }
                        if(!typeId){
                            typeId = getTypeByJavaName(QtJambiAPI::getClassName(_env, externalClass).replace(".", "/"));
                        }
                        const QMetaObject *metaObject = nullptr;
                        if(typeId)
                            metaObject = registeredOriginalMetaObject(*typeId);
                        QMetaType::TypeFlags flags;
#if QT_VERSION < QT_VERSION_CHECK(6, 9, 0)
                        if(Java::QtCore::QObject::isAssignableFrom(_env, externalClass)){
                            flags = QMetaType::TypeFlags(QtPrivate::QMetaTypeTypeFlags<QObject*>::Flags);
                        }else{
                            flags = QMetaType::TypeFlags(QtPrivate::QMetaTypeTypeFlags<void*>::Flags);
#else
                        if(Java::QtCore::QObject::isAssignableFrom(_env, externalClass)){
                            flags = QMetaType::TypeFlags(QtPrivate::QMetaTypeForType<QObject*>::flags());
                        }else{
                            flags = QMetaType::TypeFlags(QtPrivate::QMetaTypeForType<void*>::flags());
#endif
                            if(metaObject)
                                flags.setFlag(QMetaType::PointerToGadget);
                        }
                        memberMetaType2 = registerMetaTypeImpl(nullptr,
                                                                            nullptr,
                                                                            instantiations[1].toUtf8(),
                                                                          /*.defaultCtr=*/ QtPrivate::QMetaTypeForType<void*>::getDefaultCtr(),
                                                                          /*.copyCtr=*/ QtPrivate::QMetaTypeForType<void*>::getCopyCtr(),
                                                                          /*.moveCtr=*/ QtPrivate::QMetaTypeForType<void*>::getMoveCtr(),
                                                                          /*.dtor=*/ QtPrivate::QMetaTypeForType<void*>::getDtor(),
                                                                          /*.equals=*/ QtPrivate::QEqualityOperatorForType<void*>::equals,
                                                                          /*.lessThan=*/ QtPrivate::QLessThanOperatorForType<void*>::lessThan,
                                                                          /*.debugStream=*/ QtPrivate::QDebugStreamOperatorForType<void*>::debugStream,
                                                                          /*.dataStreamOut=*/ QtPrivate::QDataStreamOperatorForType<void*>::dataStreamOut,
                                                                          /*.dataStreamIn=*/ QtPrivate::QDataStreamOperatorForType<void*>::dataStreamIn,
                                                                          /*.legacyRegisterOp=*/ QtPrivate::QMetaTypeForType<void*>::getLegacyRegister(),
                                                                             uint(sizeAlign2.first = sizeof(void*)),
                                                                             ushort(sizeAlign2.second = alignof(void*)),
                                                                             0,
                                                                             flags,
                                                                             metaObject,
                                                                             nullptr,
                                                                             nullptr,
                                                                            {});
                        tryAgain = true;
                    }
                    if(tryAgain)
                        containerAccess = ContainerAccessAPI::createContainerAccess(mapType, memberMetaType1, memberMetaType2);

                    if(!containerAccess && memberMetaType1.isValid() && memberMetaType2.isValid()){
                        QtJambiUtils::QHashFunction hashFunction1 = findHashFunction(isPointer1, memberMetaType1);
                        QtJambiUtils::QHashFunction hashFunction2 = findHashFunction(isPointer2, memberMetaType2);
                        if(mapType == AssociativeContainerType::QHash
                                && !hashFunction1){
                            if(clazz1.startsWith("io/qt/core/QMetaType$Generic"))
                                hashFunction1 = [](const void*, size_t seed) -> size_t{ return seed; };
                            else
                                Java::Runtime::IllegalArgumentException::throwNew(_env, QStringLiteral("Unable to use %1 as hash key.").arg(QLatin1String(clazz1.replace('/', '.').replace('$', '.'))) QTJAMBI_STACKTRACEINFO );
                        }
                        QSharedPointer<AbstractContainerAccess> memberNestedContainerAccess1 = findContainerAccess(memberMetaType1);
                        QSharedPointer<AbstractContainerAccess> memberNestedContainerAccess2 = findContainerAccess(memberMetaType2);
                        const std::type_info* typeId = getTypeByQtName(qPrintable(instantiations[0]));
                        if(!typeId){
                            typeId = getTypeByMetaType(memberMetaType1);
                        }
                        PtrOwnerFunction memberOwnerFunction1 = nullptr;
                        if(typeId)
                            memberOwnerFunction1 = ContainerAPI::registeredOwnerFunction(*typeId);
                        typeId = getTypeByQtName(qPrintable(instantiations[1]));
                        if(!typeId){
                            typeId = getTypeByMetaType(memberMetaType2);
                        }
                        PtrOwnerFunction memberOwnerFunction2 = nullptr;
                        if(typeId)
                            memberOwnerFunction2 = ContainerAPI::registeredOwnerFunction(*typeId);
                        containerAccess = ContainerAccessAPI::createContainerAccess(_env, mapType,
                                                                          memberMetaType1,
                                                                          sizeAlign1.second, sizeAlign1.first,
                                                                          isPointer1,
                                                                          hashFunction1,
                                                                          memberConverter1,
                                                                          memberReConverter1,
                                                                          memberNestedContainerAccess1,
                                                                          memberOwnerFunction1,
                                                                          memberMetaType2,
                                                                          sizeAlign2.second, sizeAlign2.first,
                                                                          isPointer2,
                                                                          hashFunction2,
                                                                          memberConverter2,
                                                                          memberReConverter2,
                                                                          memberNestedContainerAccess2,
                                                                          memberOwnerFunction2);
                    }
                }
                if(containerAccess){
#if defined(QTJAMBI_GENERIC_ACCESS)
                    switch(mapType){
                    case AssociativeContainerType::QHash:
                        containerAccess = checkContainerAccess(_env, dynamic_cast<AbstractHashAccess*>(containerAccess));
                        break;
                    case AssociativeContainerType::QMap:
                        containerAccess = checkContainerAccess(_env, dynamic_cast<AbstractMapAccess*>(containerAccess));
                        break;
                    case AssociativeContainerType::QMultiMap:
                        containerAccess = checkContainerAccess(_env, dynamic_cast<AbstractMultiMapAccess*>(containerAccess));
                        break;
                    case AssociativeContainerType::QMultiHash:
                        containerAccess = checkContainerAccess(_env, dynamic_cast<AbstractMultiHashAccess*>(containerAccess));
                        break;
                    default:
                        break;
                    }
#endif //defined(QTJAMBI_GENERIC_ACCESS)
                    QSharedPointer<AbstractContainerAccess> sharedAccess(containerAccess, &containerDisposer);
                    if(mapType == AssociativeContainerType::QPair){
                        QSharedPointer<AbstractPairAccess> pairAccess(sharedAccess.dynamicCast<AbstractPairAccess>());
                        switch(pointerType){
                        case PointerType::NoPointer:{
                            return [pairAccess]
                                    (JNIEnv* env, QtJambiScope*, const void* in, jvalue& p, bool)->bool{
                                if(in){
                                    jobject first = pairAccess->first(env, in);
                                    jobject second = pairAccess->second(env, in);
                                    p.l = Java::QtCore::QPair::newInstance(env, first, second);
                                }
                                return true;
                            };
                        }break;
                        case PointerType::SharedPointer:{
                            return [pairAccess](JNIEnv* env, QtJambiScope*, const void* in, jvalue& p, bool)->bool{
                                const QSharedPointer<char>* ptr = reinterpret_cast<const QSharedPointer<char>*>(in);
                                if(ptr && *ptr){
                                    jobject first = pairAccess->first(env, ptr->get());
                                    jobject second = pairAccess->second(env, ptr->get());
                                    p.l = Java::QtCore::QPair::newInstance(env, first, second);
                                }
                                return true;
                            };
                        }break;
                        case PointerType::shared_ptr:{
                            return [pairAccess](JNIEnv* env, QtJambiScope*, const void* in, jvalue& p, bool)->bool{
                                const std::shared_ptr<char>* ptr = reinterpret_cast<const std::shared_ptr<char>*>(in);
                                if(ptr && *ptr){
                                    jobject first = pairAccess->first(env, ptr->get());
                                    jobject second = pairAccess->second(env, ptr->get());
                                    p.l = Java::QtCore::QPair::newInstance(env, first, second);
                                }
                                return true;
                            };
                        }break;
                        case PointerType::WeakPointer:{
                            return [pairAccess](JNIEnv* env, QtJambiScope*, const void* in, jvalue& p, bool)->bool{
                                const QWeakPointer<char>* ptr = reinterpret_cast<const QWeakPointer<char>*>(in);
                                if(ptr){
                                    if(const QSharedPointer<char> _ptr{*ptr}){
                                        jobject first = pairAccess->first(env, _ptr.get());
                                        jobject second = pairAccess->second(env, _ptr.get());
                                        p.l = Java::QtCore::QPair::newInstance(env, first, second);
                                    }
                                }
                                return true;
                            };
                        }break;
                        case PointerType::weak_ptr:{
                            return [pairAccess](JNIEnv* env, QtJambiScope*, const void* in, jvalue& p, bool)->bool{
                                const std::weak_ptr<char>* ptr = reinterpret_cast<const std::weak_ptr<char>*>(in);
                                if(ptr){
                                    if(const std::shared_ptr<char> _ptr{*ptr}){
                                        jobject first = pairAccess->first(env, _ptr.get());
                                        jobject second = pairAccess->second(env, _ptr.get());
                                        p.l = Java::QtCore::QPair::newInstance(env, first, second);
                                    }
                                }
                                return true;
                            };
                        }break;
                        default:
                            return ParameterTypeInfo::default_internalToExternalConverter();
                        }
                    }else{
                        switch(pointerType){
                        case PointerType::NoPointer:{
                            return [LINK_NAME_ARG(internalTypeName) mapType, sharedAccess]
                                    (JNIEnv* env, QtJambiScope*, const void* in, jvalue& p, bool)->bool{
                                if(in){
                                    AbstractContainerAccess* access = sharedAccess->clone();
                                    switch(mapType){
                                    case AssociativeContainerType::QMap:
                                        p.l = Java::QtCore::QMap::newInstance(env, nullptr);
                                        break;
                                    case AssociativeContainerType::QMultiMap:
                                        p.l = Java::QtCore::QMultiMap::newInstance(env, nullptr);
                                        break;
                                    case AssociativeContainerType::QMultiHash:
                                        p.l = Java::QtCore::QMultiHash::newInstance(env, nullptr);
                                        break;
                                    default:
                                        p.l = Java::QtCore::QHash::newInstance(env, nullptr);
                                        break;
                                    }
                                    void* ptr = access->createContainer(in);
                                    QSharedPointer<QtJambiLink> link = QtJambiLink::createLinkForNativeObject(env, p.l, ptr,
                                                                                                              LINK_NAME_ARG(internalTypeName.toLatin1())
                                                                                                              false/*created_by_java*/, true/*copy*/, access, QtJambiLink::Ownership::Java);
                                    if (!link) {
                                        p.l = nullptr;
                                        if(access)
                                            access->dispose();
                                    }
                                    return !!link;
                                }
                                return true;
                            };
                        }break;
                        case PointerType::SharedPointer:{
                            return [mapType, sharedAccess](JNIEnv* env, QtJambiScope*, const void* in, jvalue& p, bool)->bool{
                                const QSharedPointer<char>* ptr = reinterpret_cast<const QSharedPointer<char>*>(in);
                                if(ptr && *ptr){
                                    AbstractContainerAccess* access = sharedAccess->clone();
                                    switch(mapType){
                                    case AssociativeContainerType::QMap:
                                        p.l = QtJambiAPI::convertQMapToJavaObject(env, *ptr, static_cast<AbstractMapAccess*>(access));
                                        break;
                                    case AssociativeContainerType::QMultiMap:
                                        p.l = QtJambiAPI::convertQMultiMapToJavaObject(env, *ptr, static_cast<AbstractMultiMapAccess*>(access));
                                        break;
                                    case AssociativeContainerType::QMultiHash:
                                        p.l = QtJambiAPI::convertQMultiHashToJavaObject(env, *ptr, static_cast<AbstractMultiHashAccess*>(access));
                                        break;
                                    default:
                                        p.l = QtJambiAPI::convertQHashToJavaObject(env, *ptr, static_cast<AbstractHashAccess*>(access));
                                        break;
                                    }
                                }
                                return true;
                            };
                        }break;
                        case PointerType::shared_ptr:{
                            return [mapType, sharedAccess](JNIEnv* env, QtJambiScope*, const void* in, jvalue& p, bool)->bool{
                                const std::shared_ptr<char>* ptr = reinterpret_cast<const std::shared_ptr<char>*>(in);
                                if(ptr && *ptr){
                                    AbstractContainerAccess* access = sharedAccess->clone();
                                    switch(mapType){
                                    case AssociativeContainerType::QMap:
                                        p.l = QtJambiAPI::convertQMapToJavaObject(env, *ptr, static_cast<AbstractMapAccess*>(access));
                                        break;
                                    case AssociativeContainerType::QMultiMap:
                                        p.l = QtJambiAPI::convertQMultiMapToJavaObject(env, *ptr, static_cast<AbstractMultiMapAccess*>(access));
                                        break;
                                    case AssociativeContainerType::QMultiHash:
                                        p.l = QtJambiAPI::convertQMultiHashToJavaObject(env, *ptr, static_cast<AbstractMultiHashAccess*>(access));
                                        break;
                                    default:
                                        p.l = QtJambiAPI::convertQHashToJavaObject(env, *ptr, static_cast<AbstractHashAccess*>(access));
                                        break;
                                    }
                                }
                                return true;
                            };
                        }break;
                        case PointerType::WeakPointer:{
                            return [mapType, sharedAccess](JNIEnv* env, QtJambiScope*, const void* in, jvalue& p, bool)->bool{
                                const QWeakPointer<char>* ptr = reinterpret_cast<const QWeakPointer<char>*>(in);
                                if(ptr){
                                    if(const QSharedPointer<char> _ptr{*ptr}){
                                        AbstractContainerAccess* access = sharedAccess->clone();
                                        switch(mapType){
                                        case AssociativeContainerType::QMap:
                                            p.l = QtJambiAPI::convertQMapToJavaObject(env, _ptr, static_cast<AbstractMapAccess*>(access));
                                            break;
                                        case AssociativeContainerType::QMultiMap:
                                            p.l = QtJambiAPI::convertQMultiMapToJavaObject(env, _ptr, static_cast<AbstractMultiMapAccess*>(access));
                                            break;
                                        case AssociativeContainerType::QMultiHash:
                                            p.l = QtJambiAPI::convertQMultiHashToJavaObject(env, _ptr, static_cast<AbstractMultiHashAccess*>(access));
                                            break;
                                        default:
                                            p.l = QtJambiAPI::convertQHashToJavaObject(env, _ptr, static_cast<AbstractHashAccess*>(access));
                                            break;
                                        }
                                    }
                                }
                                return true;
                            };
                        }break;
                        case PointerType::weak_ptr:{
                            return [mapType, sharedAccess](JNIEnv* env, QtJambiScope*, const void* in, jvalue& p, bool)->bool{
                                const std::weak_ptr<char>* ptr = reinterpret_cast<const std::weak_ptr<char>*>(in);
                                if(ptr){
                                    if(const std::shared_ptr<char> _ptr{*ptr}){
                                        AbstractContainerAccess* access = sharedAccess->clone();
                                        switch(mapType){
                                        case AssociativeContainerType::QMap:
                                            p.l = QtJambiAPI::convertQMapToJavaObject(env, _ptr, static_cast<AbstractMapAccess*>(access));
                                            break;
                                        case AssociativeContainerType::QMultiMap:
                                            p.l = QtJambiAPI::convertQMultiMapToJavaObject(env, _ptr, static_cast<AbstractMultiMapAccess*>(access));
                                            break;
                                        case AssociativeContainerType::QMultiHash:
                                            p.l = QtJambiAPI::convertQMultiHashToJavaObject(env, _ptr, static_cast<AbstractMultiHashAccess*>(access));
                                            break;
                                        default:
                                            p.l = QtJambiAPI::convertQHashToJavaObject(env, _ptr, static_cast<AbstractHashAccess*>(access));
                                            break;
                                        }
                                    }
                                }
                                return true;
                            };
                        }break;
                        default:
                            return ParameterTypeInfo::default_internalToExternalConverter();
                        }
                    }
                }
            }
        }
        return [](JNIEnv*, QtJambiScope*, const void*, jvalue&, bool)->bool{
            return false;
        };
    }else if ([&]() -> bool {
               bool result = Java::QtJambi::ClassAnalyzerUtility::isGeneratedClass(_env, externalClass);
               if(!result){
                   result = !SuperTypeInfos::fromClass(_env, externalClass).isEmpty();
               }
               return result;
          }()) {
        // If we're dealing with a QObject, then we try to find the original java instance
        const std::type_info* typeId = nullptr;
        {
            QString _internalTypeName = internalTypeName;
            if(_internalTypeName.startsWith("const "))
                _internalTypeName = _internalTypeName.mid(6);
            if(_internalTypeName.endsWith("*") || _internalTypeName.endsWith("&"))
                _internalTypeName.chop(1);
            typeId = getTypeByQtName(_internalTypeName);
        }
        if(typeId){
            if(getEntryType(*typeId)==EntryTypes::Unspecific){
                QByteArray className = getJavaName(*typeId);
                if(className.isEmpty()){
                    typeId = nullptr;
                }
            }
        }
        if(!typeId){
            typeId = getTypeByJavaName(QtJambiAPI::getClassName(_env, externalClass).replace(".", "/"));
        }
        if(!typeId && internalMetaType.id()!=QMetaType::UnknownType){
            QByteArray metaTypeName = internalMetaType.name();
            if(metaTypeName.startsWith("const "))
                metaTypeName = metaTypeName.mid(6);
            if(metaTypeName.endsWith("*") || metaTypeName.endsWith("&"))
                metaTypeName.chop(1);
            typeId = getTypeByQtName(metaTypeName.data());
            if(!typeId){
                typeId = getTypeByMetaType(internalMetaType);
            }
        }
        if(!typeId){
            const SuperTypeInfos superTypeInfos = SuperTypeInfos::fromClass(_env, externalClass);
            if(!superTypeInfos.isEmpty()){
                typeId = &superTypeInfos[0].typeId();
            }
        }
        const char* functionalName = registeredClassNameForFunctionalMetaType(internalMetaType);
        if(typeId){
            if (Java::QtCore::QObject::isAssignableFrom(_env,externalClass)) {
                switch(pointerType){
                case PointerType::NoPointer:
                    return [typeId](JNIEnv* env, QtJambiScope*, const void* in, jvalue& p, bool)->bool{
                        QObject * const *ptr = reinterpret_cast<QObject * const *>(in);
                        if(ptr){
                            QObject * qobject = *ptr;
                            p.l = QtJambiAPI::convertQObjectToJavaObject(env, qobject, *typeId);
                        }
                        return true;
                    };
                case PointerType::SharedPointer:
                    return [typeId](JNIEnv* env, QtJambiScope*, const void* in, jvalue& p, bool)->bool{
                        const QSharedPointer<QObject>* ptr = reinterpret_cast<const QSharedPointer<QObject>*>(in);
                        if(ptr){
                            p.l = QtJambiAPI::convertSmartPointerToJavaObject(env, *typeId, *ptr);
                        }
                        return true;
                    };
                case PointerType::WeakPointer:
                    return [typeId](JNIEnv* env, QtJambiScope*, const void* in, jvalue& p, bool)->bool{
                        const QWeakPointer<QObject>* ptr = reinterpret_cast<const QWeakPointer<QObject>*>(in);
                        if(ptr){
                            p.l = QtJambiAPI::convertSmartPointerToJavaObject(env, *typeId, *ptr);
                        }
                        return true;
                    };
                case PointerType::shared_ptr:
                    return [typeId](JNIEnv* env, QtJambiScope*, const void* in, jvalue& p, bool)->bool{
                        const std::shared_ptr<QObject>* ptr = reinterpret_cast<const std::shared_ptr<QObject>*>(in);
                        if(ptr){
                            p.l = QtJambiAPI::convertSmartPointerToJavaObject(env, *typeId, *ptr);
                        }
                        return true;
                    };
                case PointerType::weak_ptr:
                    return [typeId](JNIEnv* env, QtJambiScope*, const void* in, jvalue& p, bool)->bool{
                        const std::weak_ptr<QObject>* ptr = reinterpret_cast<const std::weak_ptr<QObject>*>(in);
                        if(ptr){
                            p.l = QtJambiAPI::convertSmartPointerToJavaObject(env, *typeId, std::shared_ptr<QObject>(*ptr));
                        }
                        return true;
                    };
                case PointerType::TrackingPointer:
                    return [typeId](JNIEnv* env, QtJambiScope*, const void* in, jvalue& p, bool)->bool{
                        const QPointer<QObject>* ptr = reinterpret_cast<const QPointer<QObject>*>(in);
                        if(ptr){
                            QObject* obj = const_cast<QPointer<QObject>*>(ptr)->data();
                            p.l = QtJambiAPI::convertQObjectToJavaObject(env, obj, *typeId);
                            QtJambiAPI::setJavaOwnershipForTopLevelObject(env, obj);
                        }
                        return true;
                    };
                case PointerType::ScopedPointer:
                    return [typeId](JNIEnv* env, QtJambiScope*, const void* in, jvalue& p, bool)->bool{
                        const QScopedPointer<QObject>* ptr = reinterpret_cast<const QScopedPointer<QObject>*>(in);
                        if(ptr){
                            QObject* obj = const_cast<QScopedPointer<QObject>*>(ptr)->take();
                            p.l = QtJambiAPI::convertQObjectToJavaObject(env, obj, *typeId);
                            QtJambiAPI::setJavaOwnershipForTopLevelObject(env, obj);
                        }
                        return true;
                    };
                case PointerType::unique_ptr:
                    return [typeId](JNIEnv* env, QtJambiScope*, const void* in, jvalue& p, bool)->bool{
                        const std::unique_ptr<QObject>* ptr = reinterpret_cast<const std::unique_ptr<QObject>*>(in);
                        if(ptr){
                            QObject* obj = const_cast<std::unique_ptr<QObject>*>(ptr)->release();
                            p.l = QtJambiAPI::convertQObjectToJavaObject(env, obj, *typeId);
                            QtJambiAPI::setJavaOwnershipForTopLevelObject(env, obj);
                        }
                        return true;
                    };
                case PointerType::ScopedArrayPointer:
                    qCWarning(CATEGORY) << "Cannot convert QScopedArrayPointer to Java.";
                    return ParameterTypeInfo::default_internalToExternalConverter();
                default:
                    return ParameterTypeInfo::default_internalToExternalConverter();
                }
            } else if(isInterface(*typeId)){
                switch(pointerType){
                case PointerType::NoPointer:
                    if (internalTypeName.endsWith(QLatin1Char('*'))){
                        return [typeId](JNIEnv* env, QtJambiScope* scope, const void* in, jvalue& p, bool)->bool{
                            void * const *ptr = reinterpret_cast<void * const *>(in);
                            if(ptr){
                                // If we found a link for the object, we use the java object
                                // from the link.
                                bool found = false;
                                for(const QSharedPointer<QtJambiLink>& link : QtJambiLink::findLinksForPointer(*ptr)) {
                                    if(link){
                                        jobject obj = link->getJavaObjectLocalRef(env);
                                        if(!obj && link->ownership()==QtJambiLink::Ownership::Split){
                                            link->invalidate(env);
                                            p.l = QtJambiAPI::convertNativeToJavaObjectAsWrapper(env, *ptr, *typeId);
                                            found = true;
                                        }else{
                                            QByteArray className = getJavaName(*typeId);
                                            jclass targetType = nullptr;
                                            try{
                                                targetType = JavaAPI::resolveClass(env, className);
                                            }catch(...){}
                                            if(targetType && env->IsInstanceOf(obj, targetType)){
                                                p.l = obj;
                                                found = true;
                                            }
                                        }
                                    }
                                }
                                if(!found){
                                    p.l = QtJambiAPI::convertNativeToJavaObjectAsWrapper(env, *ptr, *typeId);
                                    if(scope)
                                        scope->addObjectInvalidation(env, p.l);
                                }
                            }
                            return true;
                        };
                    }else{
                        return [typeId](JNIEnv* env, QtJambiScope*, const void* in, jvalue& p, bool)->bool{
                            if(in){
                                p.l = QtJambiAPI::convertNativeToJavaObjectAsCopy(env, in, *typeId);
                            }
                            return true;
                        };
                    }
                case PointerType::SharedPointer:
                    return [typeId](JNIEnv* env, QtJambiScope*, const void* in, jvalue& p, bool)->bool{
                        const QSharedPointer<char>* ptr = reinterpret_cast<const QSharedPointer<char>*>(in);
                        if(ptr){
                            p.l = QtJambiAPI::convertSmartPointerToJavaObject(env, *typeId, *ptr);
                        }
                        return true;
                    };
                case PointerType::WeakPointer:
                    return [typeId](JNIEnv* env, QtJambiScope*, const void* in, jvalue& p, bool)->bool{
                        const QWeakPointer<char>* ptr = reinterpret_cast<const QWeakPointer<char>*>(in);
                        if(ptr){
                            p.l = QtJambiAPI::convertSmartPointerToJavaObject(env, *typeId, *ptr);
                        }
                        return true;
                    };
                case PointerType::shared_ptr:
                    return [typeId](JNIEnv* env, QtJambiScope*, const void* in, jvalue& p, bool)->bool{
                        const std::shared_ptr<char>* ptr = reinterpret_cast<const std::shared_ptr<char>*>(in);
                        if(ptr){
                            p.l = QtJambiAPI::convertSmartPointerToJavaObject(env, *typeId, *ptr);
                        }
                        return true;
                    };
                case PointerType::weak_ptr:
                    return [typeId](JNIEnv* env, QtJambiScope*, const void* in, jvalue& p, bool)->bool{
                        const std::weak_ptr<char>* ptr = reinterpret_cast<const std::weak_ptr<char>*>(in);
                        if(ptr){
                            p.l = QtJambiAPI::convertSmartPointerToJavaObject(env, *typeId, std::shared_ptr<char>(*ptr));
                        }
                        return true;
                    };
                case PointerType::ScopedPointer:
                    return [typeId](JNIEnv* env, QtJambiScope*, const void* in, jvalue& p, bool)->bool{
                        const QScopedPointer<void*>* ptr = reinterpret_cast<const QScopedPointer<void*>*>(in);
                        if(ptr){
                            p.l = QtJambiAPI::convertNativeToJavaObjectAsWrapper(env, const_cast<QScopedPointer<void*>*>(ptr)->take(), *typeId);
                            QtJambiAPI::setJavaOwnership(env, p.l);
                        }
                        return true;
                    };
                case PointerType::unique_ptr:
                    return [typeId](JNIEnv* env, QtJambiScope*, const void* in, jvalue& p, bool)->bool{
                        const std::unique_ptr<void*>* ptr = reinterpret_cast<const std::unique_ptr<void*>*>(in);
                        if(ptr){
                            p.l = QtJambiAPI::convertNativeToJavaObjectAsWrapper(env, const_cast<std::unique_ptr<void*>*>(ptr)->release(), *typeId);
                            QtJambiAPI::setJavaOwnership(env, p.l);
                        }
                        return true;
                    };
                case PointerType::TrackingPointer:
                    qCWarning(CATEGORY) << "Cannot convert QPointer of non-QObject to Java.";
                    return ParameterTypeInfo::default_internalToExternalConverter();
                case PointerType::ScopedArrayPointer:
                    qCWarning(CATEGORY) << "Cannot convert QScopedArrayPointer to Java.";
                    return ParameterTypeInfo::default_internalToExternalConverter();
                default:
                    return ParameterTypeInfo::default_internalToExternalConverter();
                }
            }else {
                switch(pointerType){
                case PointerType::NoPointer:
                    if (internalTypeName.endsWith(QLatin1Char('*')) || isFunctionPointer(*typeId) ){
                        if(functionalName){
                            return [typeId, functionalName, qtName = getQtNameByFunctional(functionalName)](JNIEnv* env, QtJambiScope* scope, const void* in, jvalue& p, bool)->bool{
                                void * const *ptr = reinterpret_cast<void * const *>(in);
                                if(ptr){
                                    // If we found a link for the object, we use the java object
                                    // from the link.
                                    bool found = false;
                                    for(const QSharedPointer<QtJambiLink>& link : QtJambiLink::findLinksForPointer(*ptr)) {
                                        jobject obj = link->getJavaObjectLocalRef(env);
                                        if(!obj && link->ownership()==QtJambiLink::Ownership::Split){
                                            link->invalidate(env);
                                            p.l = QtJambiAPI::convertNativeToJavaObjectAsWrapper(env, in, *typeId, qtName);
                                            found = true;
                                            if(scope)
                                                scope->addObjectInvalidation(env, p.l);
                                        }else{
                                            jclass targetType = nullptr;
                                            try{
                                                targetType = JavaAPI::resolveClass(env, functionalName);
                                            }catch(...){}
                                            if(targetType && env->IsInstanceOf(obj, targetType)){
                                                p.l = obj;
                                                found = true;
                                            }
                                        }
                                    }
                                    if(!found){
                                        p.l = QtJambiAPI::convertNativeToJavaObjectAsWrapper(env, *ptr, *typeId, qtName);
                                        if(scope)
                                            scope->addObjectInvalidation(env, p.l);
                                    }
                                }
                                return true;
                            };
                        }else{
                            return [typeId](JNIEnv* env, QtJambiScope* scope, const void* in, jvalue& p, bool)->bool{
                                void * const *ptr = reinterpret_cast<void * const *>(in);
                                if(ptr){
                                    // If we found a link for the object, we use the java object
                                    // from the link.
                                    bool found = false;
                                    for(const QSharedPointer<QtJambiLink>& link : QtJambiLink::findLinksForPointer(*ptr)) {
                                        jobject obj = link->getJavaObjectLocalRef(env);
                                        if(!obj && link->ownership()==QtJambiLink::Ownership::Split){
                                            link->invalidate(env);
                                            p.l = QtJambiAPI::convertNativeToJavaObjectAsWrapper(env, in, *typeId);
                                            found = true;
                                            if(scope)
                                                scope->addObjectInvalidation(env, p.l);
                                        }else{
                                            QByteArray className = getJavaName(*typeId);
                                            jclass targetType = nullptr;
                                            try{
                                                targetType = JavaAPI::resolveClass(env, className);
                                            }catch(...){}
                                            if(targetType && env->IsInstanceOf(obj, targetType)){
                                                p.l = obj;
                                                found = true;
                                            }
                                        }
                                    }
                                    if(!found){
                                        p.l = QtJambiAPI::convertNativeToJavaObjectAsWrapper(env, *ptr, *typeId);
                                        if(scope)
                                            scope->addObjectInvalidation(env, p.l);
                                    }
                                }
                                return true;
                            };
                        }
                    }else{
                        if(internalTypeName.endsWith(QLatin1Char('&'))){
                            return [typeId](JNIEnv* env, QtJambiScope* scope, const void* in, jvalue& p, bool)->bool{
                                if(in){
                                    // If we found a link for the object, we use the java object
                                    // from the link.
                                    bool found = false;
                                    for(const QSharedPointer<QtJambiLink>& link : QtJambiLink::findLinksForPointer(in)) {
                                        jobject obj = link->getJavaObjectLocalRef(env);
                                        if(!obj && link->ownership()==QtJambiLink::Ownership::Split){
                                            link->invalidate(env);
                                            p.l = QtJambiAPI::convertNativeToJavaObjectAsWrapper(env, in, *typeId);
                                            found = true;
                                            if(scope)
                                                scope->addObjectInvalidation(env, p.l);
                                        }else{
                                            QByteArray className = getJavaName(*typeId);
                                            jclass targetType = nullptr;
                                            try{
                                                targetType = JavaAPI::resolveClass(env, className);
                                            }catch(...){}
                                            if(targetType && env->IsInstanceOf(obj, targetType)){
                                                p.l = obj;
                                                found = true;
                                            }
                                        }
                                    }
                                    if(!found){
                                        p.l = QtJambiAPI::convertNativeToJavaObjectAsWrapper(env, in, *typeId);
                                        if(scope)
                                            scope->addObjectInvalidation(env, p.l);
                                    }
                                }
                                return true;
                            };
                        }else if(functionalName){
                            return [typeId, qtName = getQtNameByFunctional(functionalName)](JNIEnv* env, QtJambiScope*, const void* in, jvalue& p, bool)->bool{
                                if(in){
                                    p.l = QtJambiAPI::convertNativeToJavaObjectAsCopy(env, in, *typeId, qtName);
                                }
                                return true;
                            };
                        }else{
                            return [typeId](JNIEnv* env, QtJambiScope*, const void* in, jvalue& p, bool)->bool{
                                if(in){
                                    p.l = QtJambiAPI::convertNativeToJavaObjectAsCopy(env, in, *typeId);
                                }
                                return true;
                            };
                        }
                    }
                case PointerType::SharedPointer:
                    return [typeId](JNIEnv* env, QtJambiScope*, const void* in, jvalue& p, bool)->bool{
                        const QSharedPointer<char>* ptr = reinterpret_cast<const QSharedPointer<char>*>(in);
                        if(ptr){
                            p.l = QtJambiAPI::convertSmartPointerToJavaObject(env, *typeId, *ptr);
                        }
                        return true;
                    };
                case PointerType::WeakPointer:
                    return [typeId](JNIEnv* env, QtJambiScope*, const void* in, jvalue& p, bool)->bool{
                        const QWeakPointer<char>* ptr = reinterpret_cast<const QWeakPointer<char>*>(in);
                        if(ptr){
                            p.l = QtJambiAPI::convertSmartPointerToJavaObject(env, *typeId, *ptr);
                        }
                        return true;
                    };
                case PointerType::shared_ptr:
                    return [typeId](JNIEnv* env, QtJambiScope*, const void* in, jvalue& p, bool)->bool{
                        const std::shared_ptr<char>* ptr = reinterpret_cast<const std::shared_ptr<char>*>(in);
                        if(ptr){
                            p.l = QtJambiAPI::convertSmartPointerToJavaObject(env, *typeId, *ptr);
                        }
                        return true;
                    };
                case PointerType::weak_ptr:
                    return [typeId](JNIEnv* env, QtJambiScope*, const void* in, jvalue& p, bool)->bool{
                        const std::weak_ptr<char>* ptr = reinterpret_cast<const std::weak_ptr<char>*>(in);
                        if(ptr){
                            p.l = QtJambiAPI::convertSmartPointerToJavaObject(env, *typeId, std::shared_ptr<char>(*ptr));
                        }
                        return true;
                    };
                case PointerType::ScopedPointer:
                    return [typeId](JNIEnv* env, QtJambiScope*, const void* in, jvalue& p, bool)->bool{
                        const QScopedPointer<void*>* ptr = reinterpret_cast<const QScopedPointer<void*>*>(in);
                        if(ptr){
                            p.l = QtJambiAPI::convertNativeToJavaObjectAsWrapper(env, const_cast<QScopedPointer<void*>*>(ptr)->take(), *typeId);
                            QtJambiAPI::setJavaOwnership(env, p.l);
                        }
                        return true;
                    };
                case PointerType::unique_ptr:
                    return [typeId](JNIEnv* env, QtJambiScope*, const void* in, jvalue& p, bool)->bool{
                        const std::unique_ptr<void*>* ptr = reinterpret_cast<const std::unique_ptr<void*>*>(in);
                        if(ptr){
                            p.l = QtJambiAPI::convertNativeToJavaObjectAsWrapper(env, const_cast<std::unique_ptr<void*>*>(ptr)->release(), *typeId);
                            QtJambiAPI::setJavaOwnership(env, p.l);
                        }
                        return true;
                    };
                case PointerType::TrackingPointer:
                    qCWarning(CATEGORY) << "Cannot convert QPointer of non-QObject to Java.";
                    return ParameterTypeInfo::default_internalToExternalConverter();
                case PointerType::ScopedArrayPointer:
                    qCWarning(CATEGORY) << "Cannot convert QScopedArrayPointer to Java.";
                    return ParameterTypeInfo::default_internalToExternalConverter();
                default:
                    return ParameterTypeInfo::default_internalToExternalConverter();
                }
            }
        }else{
            QByteArray className = QtJambiAPI::getClassName(_env, externalClass).replace(".", "/").toLatin1();
            externalClass = getGlobalClassRef(_env, externalClass, className);
            if (Java::QtCore::QObject::isAssignableFrom(_env,externalClass)) {
                switch(pointerType){
                case PointerType::NoPointer:
                    return [externalClass](JNIEnv* env, QtJambiScope*, const void* in, jvalue& p, bool)->bool{
                        QObject * const *ptr = reinterpret_cast<QObject * const *>(in);
                        if(ptr){
                            QObject * qobject = *ptr;
                            p.l = QtJambiAPI::convertQObjectToJavaObject(env, qobject, externalClass);
                        }
                        return true;
                    };
                case PointerType::SharedPointer:
                    return [className](JNIEnv* env, QtJambiScope*, const void* in, jvalue& p, bool)->bool{
                        const QSharedPointer<QObject>* ptr = reinterpret_cast<const QSharedPointer<QObject>*>(in);
                        if(ptr){
                            p.l = convertQObjectSmartPointerToJavaObject(env, className, *ptr);
                        }
                        return true;
                    };
                case PointerType::WeakPointer:
                    return [className](JNIEnv* env, QtJambiScope*, const void* in, jvalue& p, bool)->bool{
                        const QWeakPointer<QObject>* ptr = reinterpret_cast<const QWeakPointer<QObject>*>(in);
                        if(ptr){
                            p.l = convertQObjectSmartPointerToJavaObject(env, className, *ptr);
                        }
                        return true;
                    };
                case PointerType::shared_ptr:
                    return [className](JNIEnv* env, QtJambiScope*, const void* in, jvalue& p, bool)->bool{
                        const std::shared_ptr<QObject>* ptr = reinterpret_cast<const std::shared_ptr<QObject>*>(in);
                        if(ptr){
                            p.l = convertQObjectSmartPointerToJavaObject(env, className, *ptr);
                        }
                        return true;
                    };
                case PointerType::weak_ptr:
                    return [className](JNIEnv* env, QtJambiScope*, const void* in, jvalue& p, bool)->bool{
                        const std::weak_ptr<QObject>* ptr = reinterpret_cast<const std::weak_ptr<QObject>*>(in);
                        if(ptr){
                            p.l = convertQObjectSmartPointerToJavaObject(env, className, std::shared_ptr<QObject>(*ptr));
                        }
                        return true;
                    };
                case PointerType::TrackingPointer:
                    return [externalClass](JNIEnv* env, QtJambiScope*, const void* in, jvalue& p, bool)->bool{
                        const QPointer<QObject>* ptr = reinterpret_cast<const QPointer<QObject>*>(in);
                        if(ptr){
                            QObject* obj = const_cast<QPointer<QObject>*>(ptr)->data();
                            p.l = QtJambiAPI::convertQObjectToJavaObject(env, obj, externalClass);
                            QtJambiAPI::setJavaOwnershipForTopLevelObject(env, obj);
                        }
                        return true;
                    };
                case PointerType::ScopedPointer:
                    return [externalClass](JNIEnv* env, QtJambiScope*, const void* in, jvalue& p, bool)->bool{
                        const QScopedPointer<QObject>* ptr = reinterpret_cast<const QScopedPointer<QObject>*>(in);
                        if(ptr){
                            QObject* obj = const_cast<QScopedPointer<QObject>*>(ptr)->take();
                            p.l = QtJambiAPI::convertQObjectToJavaObject(env, obj, externalClass);
                            QtJambiAPI::setJavaOwnershipForTopLevelObject(env, obj);
                        }
                        return true;
                    };
                case PointerType::unique_ptr:
                    return [externalClass](JNIEnv* env, QtJambiScope*, const void* in, jvalue& p, bool)->bool{
                        const std::unique_ptr<QObject>* ptr = reinterpret_cast<const std::unique_ptr<QObject>*>(in);
                        if(ptr){
                            QObject* obj = const_cast<std::unique_ptr<QObject>*>(ptr)->release();
                            p.l = QtJambiAPI::convertQObjectToJavaObject(env, obj, externalClass);
                            QtJambiAPI::setJavaOwnershipForTopLevelObject(env, obj);
                        }
                        return true;
                    };
                case PointerType::ScopedArrayPointer:
                    qCWarning(CATEGORY) << "Cannot convert QScopedArrayPointer to Java.";
                    return ParameterTypeInfo::default_internalToExternalConverter();
                default:
                    return ParameterTypeInfo::default_internalToExternalConverter();
                }
            } else {
                const SuperTypeInfos superTypes = SuperTypeInfos::fromClass(_env, externalClass);
                bool forcePointerConversion = false;
                if(!superTypes.isEmpty()){
                    EntryTypes entryType = getEntryType(superTypes[0].typeId());
                    switch(entryType){
                    case EntryTypes::ObjectTypeInfo:
                    case EntryTypes::QObjectTypeInfo:
                    case EntryTypes::InterfaceTypeInfo:
                        forcePointerConversion = true;
                        break;
                    default: break;
                    }
                }
                switch(pointerType){
                case PointerType::NoPointer:
                    if (internalTypeName.contains(QLatin1Char('*')) || forcePointerConversion){
                        return [externalClass](JNIEnv* env, QtJambiScope* scope, const void* in, jvalue& p, bool)->bool{
                            void * const *ptr = reinterpret_cast<void * const *>(in);
                            if(ptr){
                                // If we found a link for the object, we use the java object
                                // from the link.
                                bool found = false;
                                for(const QSharedPointer<QtJambiLink>& link : QtJambiLink::findLinksForPointer(*ptr)) {
                                    jobject obj = link->getJavaObjectLocalRef(env);
                                    if(!obj && link->ownership()==QtJambiLink::Ownership::Split){
                                        link->invalidate(env);
                                        p.l = QtJambiAPI::convertNativeToJavaObjectAsWrapper(env, *ptr, externalClass);
                                        found = true;
                                        if(scope)
                                            scope->addObjectInvalidation(env, p.l);
                                    }else{
                                        if(env->IsInstanceOf(obj, externalClass)){
                                            p.l = obj;
                                            found = true;
                                        }
                                    }
                                }
                                if(!found){
                                    p.l = QtJambiAPI::convertNativeToJavaObjectAsWrapper(env, *ptr, externalClass);
                                    if(scope)
                                        scope->addObjectInvalidation(env, p.l);
                                }
                            }
                            return true;
                        };
                    }else if(internalTypeName.endsWith(QLatin1Char('&'))){
                        return [externalClass](JNIEnv* env, QtJambiScope* scope, const void* in, jvalue& p, bool)->bool{
                            if(in){
                                // If we found a link for the object, we use the java object
                                // from the link.
                                bool found = false;
                                for(const QSharedPointer<QtJambiLink>& link : QtJambiLink::findLinksForPointer(in)) {
                                    jobject obj = link->getJavaObjectLocalRef(env);
                                    if(!obj && link->ownership()==QtJambiLink::Ownership::Split){
                                        link->invalidate(env);
                                        p.l = QtJambiAPI::convertNativeToJavaObjectAsWrapper(env, in, externalClass);
                                        found = true;
                                        if(scope)
                                            scope->addObjectInvalidation(env, p.l);
                                    }else{
                                        if(env->IsInstanceOf(obj, externalClass)){
                                            p.l = obj;
                                            found = true;
                                        }
                                    }
                                }
                                if(!found){
                                    p.l = QtJambiAPI::convertNativeToJavaObjectAsWrapper(env, in, externalClass);
                                    if(scope)
                                        scope->addObjectInvalidation(env, p.l);
                                }
                            }
                            return true;
                        };
                    }else{
                        return [externalClass](JNIEnv* env, QtJambiScope*, const void* in, jvalue& p, bool)->bool{
                            if(in){
                                p.l = QtJambiAPI::convertNativeToJavaObjectAsCopy(env, in, externalClass);
                            }
                            return true;
                        };
                    }
                case PointerType::SharedPointer:
                    return [className](JNIEnv* env, QtJambiScope*, const void* in, jvalue& p, bool)->bool{
                        const QSharedPointer<char>* ptr = reinterpret_cast<const QSharedPointer<char>*>(in);
                        if(ptr){
                            p.l = convertSmartPointerToJavaObject(env, className, *ptr);
                        }
                        return true;
                    };
                case PointerType::WeakPointer:
                    return [className](JNIEnv* env, QtJambiScope*, const void* in, jvalue& p, bool)->bool{
                        const QWeakPointer<char>* ptr = reinterpret_cast<const QWeakPointer<char>*>(in);
                        if(ptr){
                            p.l = convertSmartPointerToJavaObject(env, className, *ptr);
                        }
                        return true;
                    };
                case PointerType::shared_ptr:
                    return [className](JNIEnv* env, QtJambiScope*, const void* in, jvalue& p, bool)->bool{
                        const std::shared_ptr<char>* ptr = reinterpret_cast<const std::shared_ptr<char>*>(in);
                        if(ptr){
                            p.l = convertSmartPointerToJavaObject(env, className, *ptr);
                        }
                        return true;
                    };
                case PointerType::weak_ptr:
                    return [className](JNIEnv* env, QtJambiScope*, const void* in, jvalue& p, bool)->bool{
                        const std::weak_ptr<char>* ptr = reinterpret_cast<const std::weak_ptr<char>*>(in);
                        if(ptr){
                            p.l = convertSmartPointerToJavaObject(env, className, std::shared_ptr<char>(*ptr));
                        }
                        return true;
                    };
                case PointerType::ScopedPointer:
                    return [externalClass](JNIEnv* env, QtJambiScope*, const void* in, jvalue& p, bool)->bool{
                        const QScopedPointer<void*>* ptr = reinterpret_cast<const QScopedPointer<void*>*>(in);
                        if(ptr){
                            p.l = QtJambiAPI::convertNativeToJavaObjectAsWrapper(env, const_cast<QScopedPointer<void*>*>(ptr)->take(), externalClass);
                            QtJambiAPI::setJavaOwnership(env, p.l);
                        }
                        return true;
                    };
                case PointerType::unique_ptr:
                    return [externalClass](JNIEnv* env, QtJambiScope*, const void* in, jvalue& p, bool)->bool{
                        const std::unique_ptr<void*>* ptr = reinterpret_cast<const std::unique_ptr<void*>*>(in);
                        if(ptr){
                            p.l = QtJambiAPI::convertNativeToJavaObjectAsWrapper(env, const_cast<std::unique_ptr<void*>*>(ptr)->release(), externalClass);
                            QtJambiAPI::setJavaOwnership(env, p.l);
                        }
                        return true;
                    };
                case PointerType::TrackingPointer:
                    qCWarning(CATEGORY) << "Cannot convert QPointer of non-QObject to Java.";
                    return ParameterTypeInfo::default_internalToExternalConverter();
                case PointerType::ScopedArrayPointer:
                    qCWarning(CATEGORY) << "Cannot convert QScopedArrayPointer to Java.";
                    return ParameterTypeInfo::default_internalToExternalConverter();
                default:
                    return ParameterTypeInfo::default_internalToExternalConverter();
                }
            }
        }
    }else if(const QMetaObject* metaObject = internalMetaType.metaObject()){
        if(metaObject->inherits(&QObject::staticMetaObject)){
            return [](JNIEnv* env, QtJambiScope*, const void* in, jvalue& p, bool)->bool{
                const QVariant & variant = *reinterpret_cast<const QVariant*>(in);
                p.l = QtJambiAPI::convertQObjectToJavaObject(env, variant.value<QObject*>());
                return true;
            };
        }else if(const std::type_info* interfaceTypeId = getTypeByQtName(metaObject->className())){
            return [interfaceTypeId](JNIEnv* env, QtJambiScope* scope, const void* in, jvalue& p, bool)->bool{
                p.l = QtJambiAPI::convertNativeToJavaObjectAsCopy(env, in, *interfaceTypeId);
                if(scope)
                    scope->addObjectInvalidation(env, p.l);
                return true;
            };
        }else if(const std::type_info* interfaceTypeId = getTypeByMetaType(internalMetaType)){
            return [interfaceTypeId](JNIEnv* env, QtJambiScope*, const void* in, jvalue& p, bool)->bool{
                p.l = QtJambiAPI::convertNativeToJavaObjectAsCopy(env, in, *interfaceTypeId);
                return true;
            };
        }
    }

    qCWarning(CATEGORY) << "Cannot convert to type " << QtJambiAPI::getClassName(_env, externalClass).replace("$", ".") << " from " << internalTypeName << ".";

    return ParameterTypeInfo::default_internalToExternalConverter();
}

QVariant int_for_QtEnumerator_or_QFlags(JNIEnv* env, jobject enum_value) {
    if (env->IsSameObject(enum_value, nullptr))
        return 0;

    QVariant result;
#if QT_VERSION >= QT_VERSION_CHECK(6, 9, 0)
    if (Java::QtJambi::QLongFlags::isInstanceOf(env, enum_value)) {
        result = QVariant::fromValue<qint64>(Java::QtJambi::QLongFlags::longValue(env,enum_value));
    }else
#endif
    if (Java::QtJambi::QFlags::isInstanceOf(env, enum_value)) {
        result = QVariant::fromValue<qint32>(Java::QtJambi::QFlags::intValue(env,enum_value));
    }else if (Java::QtJambi::QtEnumerator::isInstanceOf(env, enum_value)) {
        result = QVariant::fromValue<qint32>(Java::QtJambi::QtEnumerator::value(env,enum_value));
    }else if (Java::QtJambi::QtShortEnumerator::isInstanceOf(env, enum_value)) {
        result = QVariant::fromValue<qint16>(Java::QtJambi::QtShortEnumerator::value(env,enum_value));
    }else if (Java::QtJambi::QtByteEnumerator::isInstanceOf(env, enum_value)) {
        result = QVariant::fromValue<qint8>(Java::QtJambi::QtByteEnumerator::value(env,enum_value));
    }else if (Java::QtJambi::QtLongEnumerator::isInstanceOf(env, enum_value)) {
        result = QVariant::fromValue<qint64>(Java::QtJambi::QtLongEnumerator::value(env,enum_value));
    }else if(Java::Runtime::Long::isInstanceOf(env, enum_value)){
        result = QVariant::fromValue<qint64>(Java::Runtime::Number::longValue(env, enum_value));
    }else if(Java::Runtime::Integer::isInstanceOf(env, enum_value)){
        result = QVariant::fromValue<qint32>(Java::Runtime::Number::intValue(env, enum_value));
    }else if(Java::Runtime::Short::isInstanceOf(env, enum_value)){
        result = QVariant::fromValue<qint16>(Java::Runtime::Number::shortValue(env, enum_value));
    }else if(Java::Runtime::Byte::isInstanceOf(env, enum_value)){
        result = QVariant::fromValue<qint8>(Java::Runtime::Number::byteValue(env, enum_value));
    }else  if(Java::Runtime::Enum::isInstanceOf(env, enum_value)){
        result = QVariant::fromValue<qint32>(Java::Runtime::Enum::ordinal(env,enum_value));
    }
    return result;
}

QtJambiUtils::ExternalToInternalConverter QtJambiTypeManager::tryGetExternalToInternalConverter(JNIEnv* env, jclass externalClass, const QString &internalTypeName, const QMetaType& internalMetaType) {
    QtJambiStorage* storage = getQtJambiStorage();
    {
        size_t key = computeHash(env, externalClass, internalTypeName, internalMetaType);
        {
            QReadLocker locker(storage->lock());
            if(storage->externalToInternalConverters().contains(key))
                return storage->externalToInternalConverters()[key];
        }
        if(internalMetaType.isValid() && !QMetaType(internalMetaType).iface()->metaObjectFn){
            QtJambiUtils::ExternalToInternalConverter converter = getExternalToInternalConverterImpl(env, externalClass, internalTypeName, internalMetaType);
            Q_ASSERT(converter);
            QWriteLocker locker(storage->lock());
            if(storage->externalToInternalConverters().contains(key))
                return storage->externalToInternalConverters()[key];
            storage->externalToInternalConverters()[key] = converter;
            return converter;
        }
    }
    return QtJambiUtils::ExternalToInternalConverter();
}

QtJambiUtils::ExternalToInternalConverter QtJambiTypeManager::getExternalToInternalConverter(JNIEnv* env, jclass externalClass, const QString &internalTypeName, const QMetaType& internalMetaType) {
    QtJambiStorage* storage = getQtJambiStorage();
    {
        size_t key = computeHash(env, externalClass, internalTypeName, internalMetaType);
        {
            QReadLocker locker(storage->lock());
            if(storage->externalToInternalConverters().contains(key))
                return storage->externalToInternalConverters()[key];
        }
        QtJambiUtils::ExternalToInternalConverter converter = getExternalToInternalConverterImpl(env, externalClass, internalTypeName, internalMetaType);
        Q_ASSERT(converter);
        QWriteLocker locker(storage->lock());
        if(storage->externalToInternalConverters().contains(key))
            return storage->externalToInternalConverters()[key];
        storage->externalToInternalConverters()[key] = converter;
        return converter;
    }
}

QtJambiUtils::ExternalToInternalConverter QtJambiTypeManager::getExternalToInternalConverterImpl(JNIEnv* _env, jclass externalClass, QString internalTypeName, const QMetaType& internalMetaType) {
    PointerType pointerType = PointerType::NoPointer;
    internalTypeName = processInternalTypeName(internalTypeName, pointerType);
    if(_env->GetObjectRefType(externalClass)!=JNIGlobalRefType){
        externalClass = getGlobalClassRef(_env, externalClass);
    }

    if ( internalMetaType.id() == QMetaType::Nullptr
               || internalMetaType.id() == QMetaType::Void
               || Java::Runtime::Void::isSameClass(_env, externalClass)
               || Java::Runtime::Void::isPrimitiveType(_env, externalClass)) {
        return [](JNIEnv*, QtJambiScope*, jvalue, void* &, jValueType) -> bool{
            return true;
        };
    }else if(_env->IsSameObject(externalClass, nullptr)){
        return [](JNIEnv*, QtJambiScope*, jvalue, void* &, jValueType) -> bool{
            return false;
        };
    }else if(JObjectValueWrapper::isValueType(internalMetaType)){
        QMetaType _internalMetaType = internalMetaType;
        return [_internalMetaType, externalClass](JNIEnv* env, QtJambiScope* scope, jvalue val, void* &out, jValueType) -> bool{
            if(val.l && !env->IsInstanceOf(val.l, externalClass))
                Java::Runtime::IllegalArgumentException::throwNew(env, QString("Wrong argument given: %1, expected: %2").arg(QtJambiAPI::getObjectClassName(env, val.l).replace("$", "."), QtJambiAPI::getClassName(env, externalClass).replace("$", ".")) QTJAMBI_STACKTRACEINFO );
            if(scope && !out){
                JObjectValueWrapper* ptr;
                out = ptr = new JObjectValueWrapper;
                scope->addDeletion(ptr);
            }
            if(!out)
                return false;
            *reinterpret_cast<JObjectValueWrapper*>(out) = JObjectValueWrapper::create(env, val.l, _internalMetaType);
            return true;
        };
    }else if(isJObjectWrappedMetaType(internalMetaType)){
        return [externalClass](JNIEnv* env, QtJambiScope* scope, jvalue val, void* &out, jValueType) -> bool{
            if(val.l && !env->IsInstanceOf(val.l, externalClass))
                Java::Runtime::IllegalArgumentException::throwNew(env, QString("Wrong argument given: %1, expected: %2").arg(QtJambiAPI::getObjectClassName(env, val.l).replace("$", "."), QtJambiAPI::getClassName(env, externalClass).replace("$", ".")) QTJAMBI_STACKTRACEINFO );
            if(scope && !out){
                JObjectWrapper* ptr;
                out = ptr = new JObjectWrapper;
                scope->addDeletion(ptr);
            }
            if(!out)
                return false;
            *reinterpret_cast<JObjectWrapper*>(out) = JObjectWrapper(env, val.l);
            return true;
        };
    }

    bool isArrayClass = Java::Runtime::Class::isArray(_env, externalClass);

    if (internalMetaType.id() == QMetaType::QVariant) {
        return [](JNIEnv* env, QtJambiScope* scope, jvalue val, void* &out, jValueType valueType) -> bool{
            if(scope && !out){
                QVariant* ptr;
                out = ptr = new QVariant();
                scope->addDeletion(ptr);
            }
            if(!out)
                return false;
            switch(valueType){
            case jValueType::l:
                *reinterpret_cast<QVariant*>(out) = QtJambiAPI::convertJavaObjectToQVariant(env, val.l);
                break;
            case jValueType::z:
                *reinterpret_cast<QVariant*>(out) = QVariant::fromValue<bool>(val.z);
                break;
            case jValueType::b:
                *reinterpret_cast<QVariant*>(out) = QVariant::fromValue<qint8>(val.b);
                break;
            case jValueType::s:
                *reinterpret_cast<QVariant*>(out) = QVariant::fromValue<qint16>(val.s);
                break;
            case jValueType::i:
                *reinterpret_cast<QVariant*>(out) = QVariant::fromValue<qint32>(val.i);
                break;
            case jValueType::j:
                *reinterpret_cast<QVariant*>(out) = QVariant::fromValue<qint64>(val.j);
                break;
            case jValueType::f:
                *reinterpret_cast<QVariant*>(out) = QVariant::fromValue<float>(val.f);
                break;
            case jValueType::d:
                *reinterpret_cast<QVariant*>(out) = QVariant::fromValue<double>(val.d);
                break;
            case jValueType::c:
                *reinterpret_cast<QVariant*>(out) = QVariant::fromValue<ushort>(val.c);
                break;
            }
            return true;
        };
    }

    if(internalMetaType==registeredMetaType(typeid(JCollectionWrapper))){
        return [externalClass](JNIEnv* env, QtJambiScope* scope, jvalue val, void* &out, jValueType) -> bool{
            if(val.l && !env->IsInstanceOf(val.l, externalClass))
                Java::Runtime::IllegalArgumentException::throwNew(env, QString("Wrong argument given: %1, expected: %2").arg(QtJambiAPI::getObjectClassName(env, val.l).replace("$", "."), QtJambiAPI::getClassName(env, externalClass).replace("$", ".")) QTJAMBI_STACKTRACEINFO );
            if(scope && !out){
                JObjectWrapper* ptr;
                out = ptr = new JCollectionWrapper(env, val.l);
                scope->addDeletion(ptr);
                return true;
            }
            if(!out)
                return false;
            reinterpret_cast<JCollectionWrapper*>(out)->assign(env, val.l);
            return true;
        };
    }else if(internalMetaType==registeredMetaType(typeid(JMapWrapper))){
        return [externalClass](JNIEnv* env, QtJambiScope* scope, jvalue val, void* &out, jValueType) -> bool{
            if(val.l && !env->IsInstanceOf(val.l, externalClass))
                Java::Runtime::IllegalArgumentException::throwNew(env, QString("Wrong argument given: %1, expected: %2").arg(QtJambiAPI::getObjectClassName(env, val.l).replace("$", "."), QtJambiAPI::getClassName(env, externalClass).replace("$", ".")) QTJAMBI_STACKTRACEINFO );
            if(scope && !out){
                JObjectWrapper* ptr;
                out = ptr = new JMapWrapper(env, val.l);
                scope->addDeletion(ptr);
                return true;
            }
            if(!out)
                return false;
            reinterpret_cast<JMapWrapper*>(out)->assign(env, val.l);
            return true;
        };
    }else if(internalMetaType==registeredMetaType(typeid(JObjectArrayWrapper))){
        return [externalClass](JNIEnv* env, QtJambiScope* scope, jvalue val, void* &out, jValueType) -> bool{
            if(val.l && !env->IsInstanceOf(val.l, externalClass))
                Java::Runtime::IllegalArgumentException::throwNew(env, QString("Wrong argument given: %1, expected: %2").arg(QtJambiAPI::getObjectClassName(env, val.l).replace("$", "."), QtJambiAPI::getClassName(env, externalClass).replace("$", ".")) QTJAMBI_STACKTRACEINFO );
            if(scope && !out){
                JObjectWrapper* ptr;
                out = ptr = new JObjectArrayWrapper(env, jobjectArray(val.l));
                scope->addDeletion(ptr);
                return true;
            }
            if(!out)
                return false;
            reinterpret_cast<JObjectArrayWrapper*>(out)->assign(env, jobjectArray(val.l));
            return true;
        };
    }else if(internalMetaType==registeredMetaType(typeid(JIntArrayWrapper))){
        return [externalClass](JNIEnv* env, QtJambiScope* scope, jvalue val, void* &out, jValueType) -> bool{
            if(val.l && !env->IsInstanceOf(val.l, externalClass))
                Java::Runtime::IllegalArgumentException::throwNew(env, QString("Wrong argument given: %1, expected: %2").arg(QtJambiAPI::getObjectClassName(env, val.l).replace("$", "."), QtJambiAPI::getClassName(env, externalClass).replace("$", ".")) QTJAMBI_STACKTRACEINFO );
            if(scope && !out){
                JObjectWrapper* ptr;
                out = ptr = new JIntArrayWrapper(env, jintArray(val.l));
                scope->addDeletion(ptr);
                return true;
            }
            if(!out)
                return false;
            reinterpret_cast<JIntArrayWrapper*>(out)->assign(env, jintArray(val.l));
            return true;
        };
    }else if(internalMetaType==registeredMetaType(typeid(JLongArrayWrapper))){
        return [externalClass](JNIEnv* env, QtJambiScope* scope, jvalue val, void* &out, jValueType) -> bool{
            if(val.l && !env->IsInstanceOf(val.l, externalClass))
                Java::Runtime::IllegalArgumentException::throwNew(env, QString("Wrong argument given: %1, expected: %2").arg(QtJambiAPI::getObjectClassName(env, val.l).replace("$", "."), QtJambiAPI::getClassName(env, externalClass).replace("$", ".")) QTJAMBI_STACKTRACEINFO );
            if(scope && !out){
                JObjectWrapper* ptr;
                out = ptr = new JLongArrayWrapper(env, jlongArray(val.l));
                scope->addDeletion(ptr);
                return true;
            }
            if(!out)
                return false;
            reinterpret_cast<JLongArrayWrapper*>(out)->assign(env, jlongArray(val.l));
            return true;
        };
    }else if(internalMetaType==registeredMetaType(typeid(JShortArrayWrapper))){
        return [externalClass](JNIEnv* env, QtJambiScope* scope, jvalue val, void* &out, jValueType) -> bool{
            if(val.l && !env->IsInstanceOf(val.l, externalClass))
                Java::Runtime::IllegalArgumentException::throwNew(env, QString("Wrong argument given: %1, expected: %2").arg(QtJambiAPI::getObjectClassName(env, val.l).replace("$", "."), QtJambiAPI::getClassName(env, externalClass).replace("$", ".")) QTJAMBI_STACKTRACEINFO );
            if(scope && !out){
                JObjectWrapper* ptr;
                out = ptr = new JShortArrayWrapper(env, jshortArray(val.l));
                scope->addDeletion(ptr);
                return true;
            }
            if(!out)
                return false;
            reinterpret_cast<JShortArrayWrapper*>(out)->assign(env, jshortArray(val.l));
            return true;
        };
    }else if(internalMetaType==registeredMetaType(typeid(JByteArrayWrapper))){
        return [externalClass](JNIEnv* env, QtJambiScope* scope, jvalue val, void* &out, jValueType) -> bool{
            if(val.l && !env->IsInstanceOf(val.l, externalClass))
                Java::Runtime::IllegalArgumentException::throwNew(env, QString("Wrong argument given: %1, expected: %2").arg(QtJambiAPI::getObjectClassName(env, val.l).replace("$", "."), QtJambiAPI::getClassName(env, externalClass).replace("$", ".")) QTJAMBI_STACKTRACEINFO );
            if(scope && !out){
                JObjectWrapper* ptr;
                out = ptr = new JByteArrayWrapper(env, jbyteArray(val.l));
                scope->addDeletion(ptr);
                return true;
            }
            if(!out)
                return false;
            reinterpret_cast<JByteArrayWrapper*>(out)->assign(env, jbyteArray(val.l));
            return true;
        };
    }else if(internalMetaType==registeredMetaType(typeid(JBooleanArrayWrapper))){
        return [externalClass](JNIEnv* env, QtJambiScope* scope, jvalue val, void* &out, jValueType) -> bool{
            if(val.l && !env->IsInstanceOf(val.l, externalClass))
                Java::Runtime::IllegalArgumentException::throwNew(env, QString("Wrong argument given: %1, expected: %2").arg(QtJambiAPI::getObjectClassName(env, val.l).replace("$", "."), QtJambiAPI::getClassName(env, externalClass).replace("$", ".")) QTJAMBI_STACKTRACEINFO );
            if(scope && !out){
                JObjectWrapper* ptr;
                out = ptr = new JBooleanArrayWrapper(env, jbooleanArray(val.l));
                scope->addDeletion(ptr);
                return true;
            }
            if(!out)
                return false;
            reinterpret_cast<JBooleanArrayWrapper*>(out)->assign(env, jbooleanArray(val.l));
            return true;
        };
    }else if(internalMetaType==registeredMetaType(typeid(JCharArrayWrapper))){
        return [externalClass](JNIEnv* env, QtJambiScope* scope, jvalue val, void* &out, jValueType) -> bool{
            if(val.l && !env->IsInstanceOf(val.l, externalClass))
                Java::Runtime::IllegalArgumentException::throwNew(env, QString("Wrong argument given: %1, expected: %2").arg(QtJambiAPI::getObjectClassName(env, val.l).replace("$", "."), QtJambiAPI::getClassName(env, externalClass).replace("$", ".")) QTJAMBI_STACKTRACEINFO );
            if(scope && !out){
                JObjectWrapper* ptr;
                out = ptr = new JCharArrayWrapper(env, jcharArray(val.l));
                scope->addDeletion(ptr);
                return true;
            }
            if(!out)
                return false;
            reinterpret_cast<JCharArrayWrapper*>(out)->assign(env, jcharArray(val.l));
            return true;
        };
    }else if(internalMetaType==registeredMetaType(typeid(JFloatArrayWrapper))){
        return [externalClass](JNIEnv* env, QtJambiScope* scope, jvalue val, void* &out, jValueType) -> bool{
            if(val.l && !env->IsInstanceOf(val.l, externalClass))
                Java::Runtime::IllegalArgumentException::throwNew(env, QString("Wrong argument given: %1, expected: %2").arg(QtJambiAPI::getObjectClassName(env, val.l).replace("$", "."), QtJambiAPI::getClassName(env, externalClass).replace("$", ".")) QTJAMBI_STACKTRACEINFO );
            if(scope && !out){
                JObjectWrapper* ptr;
                out = ptr = new JFloatArrayWrapper(env, jfloatArray(val.l));
                scope->addDeletion(ptr);
                return true;
            }
            if(!out)
                return false;
            reinterpret_cast<JFloatArrayWrapper*>(out)->assign(env, jfloatArray(val.l));
            return true;
        };
    }else if(internalMetaType==registeredMetaType(typeid(JDoubleArrayWrapper))){
        return [externalClass](JNIEnv* env, QtJambiScope* scope, jvalue val, void* &out, jValueType) -> bool{
            if(val.l && !env->IsInstanceOf(val.l, externalClass))
                Java::Runtime::IllegalArgumentException::throwNew(env, QString("Wrong argument given: %1, expected: %2").arg(QtJambiAPI::getObjectClassName(env, val.l).replace("$", "."), QtJambiAPI::getClassName(env, externalClass).replace("$", ".")) QTJAMBI_STACKTRACEINFO );
            if(scope && !out){
                JObjectWrapper* ptr;
                out = ptr = new JDoubleArrayWrapper(env, jdoubleArray(val.l));
                scope->addDeletion(ptr);
                return true;
            }
            if(!out)
                return false;
            reinterpret_cast<JDoubleArrayWrapper*>(out)->assign(env, jdoubleArray(val.l));
            return true;
        };
    }else if(internalMetaType==registeredMetaType(typeid(JObjectWrapper))){
        return [externalClass](JNIEnv* env, QtJambiScope* scope, jvalue val, void* &out, jValueType) -> bool{
            if(val.l && !env->IsInstanceOf(val.l, externalClass))
                Java::Runtime::IllegalArgumentException::throwNew(env, QString("Wrong argument given: %1, expected: %2").arg(QtJambiAPI::getObjectClassName(env, val.l).replace("$", "."), QtJambiAPI::getClassName(env, externalClass).replace("$", ".")) QTJAMBI_STACKTRACEINFO );
            if(scope && !out){
                JObjectWrapper* ptr;
                out = ptr = new JObjectWrapper(env, val.l);
                scope->addDeletion(ptr);
                return true;
            }
            if(!out)
                return false;
            reinterpret_cast<JObjectWrapper*>(out)->assign(env, val.l);
            return true;
        };
    }else if(Java::Runtime::Integer::isPrimitiveType(_env,externalClass) || Java::Runtime::Integer::isSameClass(_env,externalClass)){
        if(!internalMetaType.isValid() || internalMetaType==QMetaType::fromType<qint32>() || internalMetaType==QMetaType::fromType<quint32>()){
            return [](JNIEnv* env, QtJambiScope* scope, jvalue val, void* &out, jValueType valueType) -> bool{
                jint v;
                switch(valueType){
                case jValueType::l:
                    if(val.l
                            && !Java::Runtime::Byte::isInstanceOf(env, val.l)
                            && !Java::Runtime::Short::isInstanceOf(env, val.l)
                            && !Java::Runtime::Integer::isInstanceOf(env, val.l))
                        Java::Runtime::IllegalArgumentException::throwNew(env, QString("Wrong argument given: %1, expected: int").arg(QtJambiAPI::getObjectClassName(env, val.l).replace("$", ".")) QTJAMBI_STACKTRACEINFO );
                    v = QtJambiAPI::fromJavaIntegerObject(env, val.l);
                    break;
                default:
                    v = val.i;
                    break;
                }
                if(scope && !out){
                    qint32 * ptr;
                    out = ptr = new qint32;
                    scope->addDeletion(ptr);
                }
                if(!out)
                    return false;
                memcpy(out, &v, 4);
                return true;
            };
        }else{
            return [internalMetaType](JNIEnv* env, QtJambiScope* scope, jvalue val, void* &out, jValueType valueType) -> bool{
                jint v;
                switch(valueType){
                case jValueType::l:
                    if(val.l
                            && !Java::Runtime::Byte::isInstanceOf(env, val.l)
                            && !Java::Runtime::Short::isInstanceOf(env, val.l)
                            && !Java::Runtime::Integer::isInstanceOf(env, val.l))
                        Java::Runtime::IllegalArgumentException::throwNew(env, QString("Wrong argument given: %1, expected: int").arg(QtJambiAPI::getObjectClassName(env, val.l).replace("$", ".")) QTJAMBI_STACKTRACEINFO );
                    v = QtJambiAPI::fromJavaIntegerObject(env, val.l);
                    break;
                default:
                    v = val.i;
                    break;
                }
                if(scope && !out){
                    void* ptr;
                    out = ptr = internalMetaType.create(&v);
                    scope->addDeletion(internalMetaType, ptr);
                }else{
                    if(!out)
                        return false;
                    internalMetaType.destruct(out);
                    QByteArray ba;
                    bool saved;
                    /* write the copy to the stream */ {
                        QDataStream stream(&ba, QIODevice::WriteOnly);
                        saved = QMetaType::fromType<qint32>().save(stream, &v);
                    }

                    /* read it back into the destination */
                    if(saved){
                        QDataStream stream(&ba, QIODevice::ReadOnly);
                        internalMetaType.load(stream, out);
                    }else if(internalMetaType.construct(out, &v)!=out){
                        return false;
                    }
                }
                return true;
            };
        }
    }else if(Java::Runtime::Long::isPrimitiveType(_env,externalClass) || Java::Runtime::Long::isSameClass(_env,externalClass)){
        if(!internalMetaType.isValid() || internalMetaType==QMetaType::fromType<qint64>() || internalMetaType==QMetaType::fromType<quint64>()){
            return [](JNIEnv* env, QtJambiScope* scope, jvalue val, void* &out, jValueType valueType) -> bool{
                jlong v;
                switch(valueType){
                case jValueType::l:
                    if(val.l
                            && !Java::Runtime::Byte::isInstanceOf(env, val.l)
                            && !Java::Runtime::Short::isInstanceOf(env, val.l)
                            && !Java::Runtime::Integer::isInstanceOf(env, val.l)
                            && !Java::Runtime::Long::isInstanceOf(env, val.l))
                        Java::Runtime::IllegalArgumentException::throwNew(env, QString("Wrong argument given: %1, expected: long").arg(QtJambiAPI::getObjectClassName(env, val.l).replace("$", ".")) QTJAMBI_STACKTRACEINFO );
                    v = QtJambiAPI::fromJavaLongObject(env, val.l);
                    break;
                default:
                    v = val.j;
                    break;
                }
                if(scope && !out){
                    qint64 *ptr;
                    out = ptr = new qint64(v);
                    scope->addDeletion(ptr);
                }
                if(!out)
                    return false;
                memcpy(out, &v, 8);
                return true;
            };
        }else{
            return [internalMetaType](JNIEnv* env, QtJambiScope* scope, jvalue val, void* &out, jValueType valueType) -> bool{
                jlong v;
                switch(valueType){
                case jValueType::l:
                    if(val.l
                            && !Java::Runtime::Byte::isInstanceOf(env, val.l)
                            && !Java::Runtime::Short::isInstanceOf(env, val.l)
                            && !Java::Runtime::Integer::isInstanceOf(env, val.l)
                            && !Java::Runtime::Long::isInstanceOf(env, val.l))
                        Java::Runtime::IllegalArgumentException::throwNew(env, QString("Wrong argument given: %1, expected: long").arg(QtJambiAPI::getObjectClassName(env, val.l).replace("$", ".")) QTJAMBI_STACKTRACEINFO );
                    v = QtJambiAPI::fromJavaLongObject(env, val.l);
                    break;
                default:
                    v = val.j;
                    break;
                }
                if(scope && !out){
                    void* ptr;
                    out = ptr = internalMetaType.create(&v);
                    scope->addDeletion(internalMetaType, ptr);
                }else{
                    if(!out)
                        return false;
                    internalMetaType.destruct(out);
                    QByteArray ba;
                    bool saved;
                    /* write the copy to the stream */ {
                        QDataStream stream(&ba, QIODevice::WriteOnly);
                        saved = QMetaType::fromType<qint64>().save(stream, &v);
                    }

                    /* read it back into the destination */
                    if(saved){
                        QDataStream stream(&ba, QIODevice::ReadOnly);
                        internalMetaType.load(stream, out);
                    }else if(internalMetaType.construct(out, &ba)!=out){
                        return false;
                    }
                }
                return true;
            };
        }
    }else if(Java::Runtime::Short::isPrimitiveType(_env,externalClass) || Java::Runtime::Short::isSameClass(_env,externalClass)){
        if(!internalMetaType.isValid() || internalMetaType.id()==qMetaTypeId<qint16>() || internalMetaType.id()==qMetaTypeId<quint16>()){
            return [](JNIEnv* env, QtJambiScope* scope, jvalue val, void* &out, jValueType valueType) -> bool{
                jshort v;
                switch(valueType){
                case jValueType::l:
                    if(val.l
                            && !Java::Runtime::Byte::isInstanceOf(env, val.l)
                            && !Java::Runtime::Short::isInstanceOf(env, val.l))
                        Java::Runtime::IllegalArgumentException::throwNew(env, QString("Wrong argument given: %1, expected: short").arg(QtJambiAPI::getObjectClassName(env, val.l).replace("$", ".")) QTJAMBI_STACKTRACEINFO );
                    v = QtJambiAPI::fromJavaShortObject(env, val.l);
                    break;
                default:
                    v = val.s;
                    break;
                }
                if(scope && !out){
                    qint16* ptr;
                    out = ptr = new qint16(v);
                    scope->addDeletion(ptr);
                }
                if(!out)
                    return false;
                memcpy(out, &v, 2);
                return true;
            };
        }else{
            return [internalMetaType](JNIEnv* env, QtJambiScope* scope, jvalue val, void* &out, jValueType valueType) -> bool{
                jshort v;
                switch(valueType){
                case jValueType::l:
                    if(val.l
                            && !Java::Runtime::Byte::isInstanceOf(env, val.l)
                            && !Java::Runtime::Short::isInstanceOf(env, val.l))
                        Java::Runtime::IllegalArgumentException::throwNew(env, QString("Wrong argument given: %1, expected: short").arg(QtJambiAPI::getObjectClassName(env, val.l).replace("$", ".")) QTJAMBI_STACKTRACEINFO );
                    v = QtJambiAPI::fromJavaShortObject(env, val.l);
                    break;
                default:
                    v = val.s;
                    break;
                }
                if(scope && !out){
                    void* ptr;
                    out = ptr = internalMetaType.create(&v);
                    scope->addDeletion(internalMetaType, ptr);
                }else{
                    if(!out)
                        return false;
                    internalMetaType.destruct(out);
                    QByteArray ba;
                    bool saved;
                    /* write the copy to the stream */ {
                        QDataStream stream(&ba, QIODevice::WriteOnly);
                        saved = QMetaType::fromType<qint16>().save(stream, &v);
                    }

                    /* read it back into the destination */
                    if(saved){
                        QDataStream stream(&ba, QIODevice::ReadOnly);
                        internalMetaType.load(stream, out);
                    }else if(internalMetaType.construct(out, &ba)!=out){
                        return false;
                    }
                }
                return true;
            };
        }
    }else if(Java::Runtime::Byte::isPrimitiveType(_env,externalClass) || Java::Runtime::Byte::isSameClass(_env,externalClass)){
        if(!internalMetaType.isValid() || internalMetaType.id()==qMetaTypeId<qint8>() || internalMetaType.id()==qMetaTypeId<quint8>()){
            return [](JNIEnv* env, QtJambiScope* scope, jvalue val, void* &out, jValueType valueType) -> bool{
                jbyte v;
                switch(valueType){
                case jValueType::l:
                    if(val.l && !Java::Runtime::Byte::isInstanceOf(env, val.l))
                        Java::Runtime::IllegalArgumentException::throwNew(env, QString("Wrong argument given: %1, expected: byte").arg(QtJambiAPI::getObjectClassName(env, val.l).replace("$", ".")) QTJAMBI_STACKTRACEINFO );
                    v = QtJambiAPI::fromJavaByteObject(env, val.l);
                    break;
                default:
                    v = val.b;
                    break;
                }
                if(scope && !out){
                    qint8* ptr;
                    out = ptr = new qint8(v);
                    scope->addDeletion(ptr);
                }
                if(!out)
                    return false;
                memcpy(out, &v, 1);
                return true;
            };
        }else{
            return [internalMetaType](JNIEnv* env, QtJambiScope* scope, jvalue val, void* &out, jValueType valueType) -> bool{
                jbyte v;
                switch(valueType){
                case jValueType::l:
                    if(val.l && !Java::Runtime::Byte::isInstanceOf(env, val.l))
                        Java::Runtime::IllegalArgumentException::throwNew(env, QString("Wrong argument given: %1, expected: byte").arg(QtJambiAPI::getObjectClassName(env, val.l).replace("$", ".")) QTJAMBI_STACKTRACEINFO );
                    v = QtJambiAPI::fromJavaByteObject(env, val.l);
                    break;
                default:
                    v = val.b;
                    break;
                }
                if(scope && !out){
                    void* ptr;
                    out = ptr = internalMetaType.create(&v);
                    scope->addDeletion(internalMetaType, ptr);
                }else{
                    if(!out)
                        return false;
                    internalMetaType.destruct(out);
                    QByteArray ba;
                    bool saved;
                    /* write the copy to the stream */ {
                        QDataStream stream(&ba, QIODevice::WriteOnly);
                        saved = QMetaType::fromType<qint8>().save(stream, &v);
                    }

                    /* read it back into the destination */
                    if(saved){
                        QDataStream stream(&ba, QIODevice::ReadOnly);
                        internalMetaType.load(stream, out);
                    }else if(internalMetaType.construct(out, &ba)!=out){
                        return false;
                    }
                }
                return true;
            };
        }
    }else if(Java::Runtime::Boolean::isPrimitiveType(_env,externalClass) || Java::Runtime::Boolean::isSameClass(_env,externalClass)){
        if(!internalMetaType.isValid() || internalMetaType==QMetaType::fromType<bool>()){
            return [](JNIEnv* env, QtJambiScope* scope, jvalue val, void* &out, jValueType valueType) -> bool{
                jboolean v;
                switch(valueType){
                case jValueType::l:
                    if(val.l && !Java::Runtime::Boolean::isInstanceOf(env, val.l))
                        Java::Runtime::IllegalArgumentException::throwNew(env, QString("Wrong argument given: %1, expected: boolean").arg(QtJambiAPI::getObjectClassName(env, val.l).replace("$", ".")) QTJAMBI_STACKTRACEINFO );
                    v = QtJambiAPI::fromJavaBooleanObject(env, val.l);
                    break;
                default:
                    v = val.z;
                    break;
                }
                if(scope && !out){
                    bool* ptr;
                    out = ptr = new bool(v);
                    scope->addDeletion(ptr);
                }
                if(!out)
                    return false;
                *reinterpret_cast<bool*>(out) = v;
                return true;
            };
        }else{
            return [internalMetaType](JNIEnv* env, QtJambiScope* scope, jvalue val, void* &out, jValueType valueType) -> bool{
                jboolean v;
                switch(valueType){
                case jValueType::l:
                    if(val.l && !Java::Runtime::Boolean::isInstanceOf(env, val.l))
                        Java::Runtime::IllegalArgumentException::throwNew(env, QString("Wrong argument given: %1, expected: boolean").arg(QtJambiAPI::getObjectClassName(env, val.l).replace("$", ".")) QTJAMBI_STACKTRACEINFO );
                    v = QtJambiAPI::fromJavaBooleanObject(env, val.l);
                    break;
                default:
                    v = val.z;
                    break;
                }
                if(scope && !out){
                    void* ptr;
                    out = ptr = internalMetaType.create(&v);
                    scope->addDeletion(internalMetaType, ptr);
                }else{
                    if(!out)
                        return false;
                    internalMetaType.destruct(out);
                    QByteArray ba;
                    bool saved;
                    /* write the copy to the stream */ {
                        QDataStream stream(&ba, QIODevice::WriteOnly);
                        saved = QMetaType::fromType<bool>().save(stream, &v);
                    }

                    /* read it back into the destination */
                    if(saved){
                        QDataStream stream(&ba, QIODevice::ReadOnly);
                        internalMetaType.load(stream, out);
                    }else if(internalMetaType.construct(out, &ba)!=out){
                        return false;
                    }
                }
                return true;
            };
        }
    }else if(Java::Runtime::Character::isPrimitiveType(_env,externalClass) || Java::Runtime::Character::isSameClass(_env,externalClass)){
        if(internalMetaType.id()==qMetaTypeId<QChar>()){
            return [](JNIEnv* env, QtJambiScope* scope, jvalue val, void* &out, jValueType valueType) -> bool{
                jchar v;
                switch(valueType){
                case jValueType::l:
                    if(val.l && !Java::Runtime::Character::isInstanceOf(env, val.l))
                        Java::Runtime::IllegalArgumentException::throwNew(env, QString("Wrong argument given: %1, expected: char").arg(QtJambiAPI::getObjectClassName(env, val.l).replace("$", ".")) QTJAMBI_STACKTRACEINFO );
                    v = QtJambiAPI::fromJavaCharacterObject(env, val.l);
                    break;
                default:
                    v = val.c;
                    break;
                }
                if(scope && !out){
                    QChar* ptr;
                    out = ptr = new QChar(v);
                    scope->addDeletion(ptr);
                }
                if(!out)
                    return false;
                *reinterpret_cast<QChar*>(out) = v;
                return true;
            };
        }else if(!internalMetaType.isValid() || internalMetaType==QMetaType::fromType<unsigned short>()){
            return [](JNIEnv* env, QtJambiScope* scope, jvalue val, void* &out, jValueType valueType) -> bool{
                jchar v;
                switch(valueType){
                case jValueType::l:
                    if(val.l && !Java::Runtime::Character::isInstanceOf(env, val.l))
                        Java::Runtime::IllegalArgumentException::throwNew(env, QString("Wrong argument given: %1, expected: char").arg(QtJambiAPI::getObjectClassName(env, val.l).replace("$", ".")) QTJAMBI_STACKTRACEINFO );
                    v = QtJambiAPI::fromJavaCharacterObject(env, val.l);
                    break;
                default:
                    v = val.c;
                    break;
                }
                if(scope && !out){
                    unsigned short* ptr;
                    out = ptr = new unsigned short(v);
                    scope->addDeletion(ptr);
                }
                if(!out)
                    return false;
                *reinterpret_cast<unsigned short*>(out) = v;
                return true;
            };
        }else{
            return [internalMetaType](JNIEnv* env, QtJambiScope* scope, jvalue val, void* &out, jValueType valueType) -> bool{
                jchar v;
                switch(valueType){
                case jValueType::l:
                    if(val.l && !Java::Runtime::Character::isInstanceOf(env, val.l))
                        Java::Runtime::IllegalArgumentException::throwNew(env, QString("Wrong argument given: %1, expected: char").arg(QtJambiAPI::getObjectClassName(env, val.l).replace("$", ".")) QTJAMBI_STACKTRACEINFO );
                    v = QtJambiAPI::fromJavaCharacterObject(env, val.l);
                    break;
                default:
                    v = val.c;
                    break;
                }
                if(scope && !out){
                    void* ptr;
                    out = ptr = internalMetaType.create(&v);
                    scope->addDeletion(internalMetaType, ptr);
                }else{
                    if(!out)
                        return false;
                    internalMetaType.destruct(out);
                    QByteArray ba;
                    bool saved;
                    /* write the copy to the stream */ {
                        QDataStream stream(&ba, QIODevice::WriteOnly);
                        saved = QMetaType::fromType<unsigned short>().save(stream, &v);
                    }

                    /* read it back into the destination */
                    if(saved){
                        QDataStream stream(&ba, QIODevice::ReadOnly);
                        internalMetaType.load(stream, out);
                    }else if(internalMetaType.construct(out, &ba)!=out){
                        return false;
                    }
                }
                return true;
            };
        }
    }else if(Java::Runtime::Float::isPrimitiveType(_env,externalClass) || Java::Runtime::Float::isSameClass(_env,externalClass)){
        if(!internalMetaType.isValid() || internalMetaType==QMetaType::fromType<float>()){
            return [](JNIEnv* env, QtJambiScope* scope, jvalue val, void* &out, jValueType valueType) -> bool{
                jfloat v;
                switch(valueType){
                case jValueType::l:
                    if(val.l
                            && !Java::Runtime::Byte::isInstanceOf(env, val.l)
                            && !Java::Runtime::Short::isInstanceOf(env, val.l)
                            && !Java::Runtime::Integer::isInstanceOf(env, val.l)
                            && !Java::Runtime::Float::isInstanceOf(env, val.l))
                        Java::Runtime::IllegalArgumentException::throwNew(env, QString("Wrong argument given: %1, expected: float").arg(QtJambiAPI::getObjectClassName(env, val.l).replace("$", ".")) QTJAMBI_STACKTRACEINFO );
                    v = QtJambiAPI::fromJavaFloatObject(env, val.l);
                    break;
                default:
                    v = val.f;
                    break;
                }
                if(scope && !out){
                    float* ptr;
                    out = ptr = new float(v);
                    scope->addDeletion(ptr);
                }
                if(!out)
                    return false;
                *reinterpret_cast<float*>(out) = v;
                return true;
            };
        }else{
            return [internalMetaType](JNIEnv* env, QtJambiScope* scope, jvalue val, void* &out, jValueType valueType) -> bool{
                jfloat v;
                switch(valueType){
                case jValueType::l:
                    if(val.l
                            && !Java::Runtime::Byte::isInstanceOf(env, val.l)
                            && !Java::Runtime::Short::isInstanceOf(env, val.l)
                            && !Java::Runtime::Integer::isInstanceOf(env, val.l)
                            && !Java::Runtime::Float::isInstanceOf(env, val.l))
                        Java::Runtime::IllegalArgumentException::throwNew(env, QString("Wrong argument given: %1, expected: float").arg(QtJambiAPI::getObjectClassName(env, val.l).replace("$", ".")) QTJAMBI_STACKTRACEINFO );
                    v = QtJambiAPI::fromJavaFloatObject(env, val.l);
                    break;
                default:
                    v = val.f;
                    break;
                }
                if(scope && !out){
                    void* ptr;
                    out = ptr = internalMetaType.create(&v);
                    scope->addDeletion(internalMetaType, ptr);
                }else{
                    if(!out)
                        return false;
                    internalMetaType.destruct(out);
                    QByteArray ba;
                    bool saved;
                    /* write the copy to the stream */ {
                        QDataStream stream(&ba, QIODevice::WriteOnly);
                        saved = QMetaType::fromType<float>().save(stream, &v);
                    }

                    /* read it back into the destination */
                    if(saved){
                        QDataStream stream(&ba, QIODevice::ReadOnly);
                        internalMetaType.load(stream, out);
                    }else if(internalMetaType.construct(out, &ba)!=out){
                        return false;
                    }
                }
                return true;
            };
        }
    }else if(Java::Runtime::Double::isPrimitiveType(_env,externalClass) || Java::Runtime::Double::isSameClass(_env,externalClass)){
        if(!internalMetaType.isValid() || internalMetaType.id()==QMetaType::Double){
            return [](JNIEnv* env, QtJambiScope* scope, jvalue val, void* &out, jValueType valueType) -> bool{
                jdouble v;
                switch(valueType){
                case jValueType::l:
                    if(val.l && !Java::Runtime::Number::isInstanceOf(env, val.l))
                        Java::Runtime::IllegalArgumentException::throwNew(env, QString("Wrong argument given: %1, expected: double").arg(QtJambiAPI::getObjectClassName(env, val.l).replace("$", ".")) QTJAMBI_STACKTRACEINFO );
                    v = QtJambiAPI::fromJavaDoubleObject(env, val.l);
                    break;
                default:
                    v = val.d;
                    break;
                }
                if(scope && !out){
                    double* ptr;
                    out = ptr = new double(v);
                    scope->addDeletion(ptr);
                }
                if(!out)
                    return false;
                *reinterpret_cast<double*>(out) = v;
                return true;
            };
        }else{
            return [internalMetaType](JNIEnv* env, QtJambiScope* scope, jvalue val, void* &out, jValueType valueType) -> bool{
                jdouble v;
                switch(valueType){
                case jValueType::l:
                    if(val.l && !Java::Runtime::Number::isInstanceOf(env, val.l))
                        Java::Runtime::IllegalArgumentException::throwNew(env, QString("Wrong argument given: %1, expected: double").arg(QtJambiAPI::getObjectClassName(env, val.l).replace("$", ".")) QTJAMBI_STACKTRACEINFO );
                    v = QtJambiAPI::fromJavaDoubleObject(env, val.l);
                    break;
                default:
                    v = val.d;
                    break;
                }
                if(scope && !out){
                    void* ptr;
                    out = ptr = internalMetaType.create(&v);
                    scope->addDeletion(internalMetaType, ptr);
                }else{
                    if(!out)
                        return false;
                    internalMetaType.destruct(out);
                    QByteArray ba;
                    bool saved;
                    /* write the copy to the stream */ {
                        QDataStream stream(&ba, QIODevice::WriteOnly);
                        saved = QMetaType::fromType<double>().save(stream, &v);
                    }

                    /* read it back into the destination */
                    if(saved){
                        QDataStream stream(&ba, QIODevice::ReadOnly);
                        internalMetaType.load(stream, out);
                    }else if(internalMetaType.construct(out, &ba)!=out){
                        return false;
                    }
                }
                return true;
            };
        }
    }else if(Java::Runtime::String::isSameClass(_env,externalClass)){
        if(!internalMetaType.isValid() || internalMetaType.id()==qMetaTypeId<QString>()){
            return [](JNIEnv* env, QtJambiScope* scope, jvalue val, void* &out, jValueType) -> bool{
                if(scope && !out){
                    QString* ptr;
                    out = ptr = new QString;
                    scope->addDeletion(ptr);
                }
                if(!out)
                    return false;
                if(val.l && !Java::Runtime::String::isInstanceOf(env, val.l))
                    Java::Runtime::IllegalArgumentException::throwNew(env, QString("Wrong argument given: %1, expected: java.lang.String").arg(QtJambiAPI::getObjectClassName(env, val.l).replace("$", ".")) QTJAMBI_STACKTRACEINFO );
                jstring strg = reinterpret_cast<jstring>(val.l);
                *reinterpret_cast<QString*>(out) = qtjambi_cast<QString>(env, strg);
                return true;
            };
        }else{
            return [internalMetaType](JNIEnv* env, QtJambiScope* scope, jvalue val, void* &out, jValueType) -> bool{
                if(val.l && !Java::Runtime::String::isInstanceOf(env, val.l))
                    Java::Runtime::IllegalArgumentException::throwNew(env, QString("Wrong argument given: %1, expected: java.lang.String").arg(QtJambiAPI::getObjectClassName(env, val.l).replace("$", ".")) QTJAMBI_STACKTRACEINFO );
                jstring strg = reinterpret_cast<jstring>(val.l);
                QString s(qtjambi_cast<QString>(env, strg));
                if(scope && !out){
                    void* ptr;
                    out = ptr = internalMetaType.create(&s);
                    scope->addDeletion(QMetaType(internalMetaType), ptr);
                }else{
                    if(!out)
                        return false;
                    internalMetaType.destruct(out);
                    QByteArray ba;
                    bool saved;
                    /* write the copy to the stream */ {
                        QDataStream stream(&ba, QIODevice::WriteOnly);
                        saved = QMetaType::fromType<QString>().save(stream, &s);
                    }

                    /* read it back into the destination */
                    if(saved){
                        QDataStream stream(&ba, QIODevice::ReadOnly);
                        internalMetaType.load(stream, out);
                    }else if(internalMetaType.construct(out, &s)!=out){
                        return false;
                    }
                }
                return true;
            };
        }
    }else if(Java::Runtime::CharSequence::isSameClass(_env,externalClass)){
        if(!internalMetaType.isValid() || internalMetaType.id()==qMetaTypeId<QString>()){
            return [](JNIEnv* env, QtJambiScope* scope, jvalue val, void* &out, jValueType) -> bool{
                if(scope && !out){
                    QString* ptr;
                    out = ptr = new QString;
                    scope->addDeletion(ptr);
                }
                if(!out)
                    return false;
                if(val.l && !Java::Runtime::CharSequence::isInstanceOf(env, val.l))
                    Java::Runtime::IllegalArgumentException::throwNew(env, QString("Wrong argument given: %1, expected: java.lang.CharSequence").arg(QtJambiAPI::getObjectClassName(env, val.l).replace("$", ".")) QTJAMBI_STACKTRACEINFO );
                *reinterpret_cast<QString*>(out) = qtjambi_cast<QString>(env, val.l);
                return true;
            };
        }else{
            return [internalMetaType](JNIEnv* env, QtJambiScope* scope, jvalue val, void* &out, jValueType) -> bool{
                if(val.l && !Java::Runtime::CharSequence::isInstanceOf(env, val.l))
                    Java::Runtime::IllegalArgumentException::throwNew(env, QString("Wrong argument given: %1, expected: java.lang.String").arg(QtJambiAPI::getObjectClassName(env, val.l).replace("$", ".")) QTJAMBI_STACKTRACEINFO );
                QString s(qtjambi_cast<QString>(env, val.l));
                if(scope && !out){
                    void* ptr;
                    out = ptr = internalMetaType.create(&s);
                    scope->addDeletion(internalMetaType, ptr);
                }else{
                    if(!out)
                        return false;
                    internalMetaType.destruct(out);
                    QByteArray ba;
                    bool saved;
                    /* write the copy to the stream */ {
                        QDataStream stream(&ba, QIODevice::WriteOnly);
                        saved = QMetaType::fromType<QString>().save(stream, &s);
                    }

                    /* read it back into the destination */
                    if(saved){
                        QDataStream stream(&ba, QIODevice::ReadOnly);
                        internalMetaType.load(stream, out);
                    }else if(internalMetaType.construct(out, &s)!=out){
                        return false;
                    }
                }
                return true;
            };
        }
    }else if(Java::QtJambi::QNativePointer::isSameClass(_env,externalClass)){
        if(!internalMetaType.isValid()){
            return [](JNIEnv* env, QtJambiScope* scope, jvalue val, void* &out, jValueType) -> bool{
                if(val.l && !Java::QtJambi::QNativePointer::isInstanceOf(env, val.l))
                    Java::Runtime::IllegalArgumentException::throwNew(env, QString("Wrong argument given: %1, expected: QNativePointer").arg(QtJambiAPI::getObjectClassName(env, val.l).replace("$", ".")) QTJAMBI_STACKTRACEINFO );
                if(scope && !out){
                    void** ptr;
                    out = ptr = new void*(!val.l ? nullptr : QtJambiAPI::convertQNativePointerToNative(env, val.l));
                    scope->addDeletion(ptr);
                    return true;
                }else if(out){
                    *reinterpret_cast<void**>(out) = !val.l ? nullptr : QtJambiAPI::convertQNativePointerToNative(env, val.l);
                    return true;
                }
                return false;
            };
        }else{
            return [internalMetaType](JNIEnv* env, QtJambiScope* scope, jvalue val, void* &out, jValueType) -> bool{
                if(val.l && !Java::QtJambi::QNativePointer::isInstanceOf(env, val.l))
                    Java::Runtime::IllegalArgumentException::throwNew(env, QString("Wrong argument given: %1, expected: QNativePointer").arg(QtJambiAPI::getObjectClassName(env, val.l).replace("$", ".")) QTJAMBI_STACKTRACEINFO );
                void* nptr = !val.l ? nullptr : QtJambiAPI::convertQNativePointerToNative(env, val.l);
                if(scope && !out){
                    void* ptr;
                    out = ptr = internalMetaType.create(nptr);
                    scope->addDeletion(internalMetaType, ptr);
                }else if(out){
                    internalMetaType.destruct(out);
                    if(internalMetaType.construct(out, nptr)!=out){
                        QByteArray ba;
                        bool saved;
                        /* write the copy to the stream */ {
                            QDataStream stream(&ba, QIODevice::WriteOnly);
                            saved = internalMetaType.save(stream, nptr);
                        }

                        /* read it back into the destination */
                        if(saved){
                            QDataStream stream(&ba, QIODevice::ReadOnly);
                            internalMetaType.load(stream, out);
                        }else{
                            return false;
                        }
                    }
                }else{
                    out = nptr;
                }
                return true;
            };
        }
    } else if (Java::QtCore::QMetaType$GenericValue::isSameClass(_env,externalClass)) {
        if(internalMetaType.flags() & QMetaType::IsPointer){
            return [internalMetaType](JNIEnv* env, QtJambiScope* scope, jvalue val, void* &out, jValueType) -> bool{
                void* ptr = nullptr;
                if(val.l){
                    if(!Java::QtCore::QMetaType$GenericValue::isInstanceOf(env, val.l)){
                        QString className = QtJambiAPI::getObjectClassName(env, val.l);
                        bool matches = false;
                        if(const std::type_info* typeId = getTypeByJavaName(className)){
                            if(registeredMetaType(*typeId)==internalMetaType)
                                matches = true;
                        }
                        if(!matches){
                            for(auto mtype : registeredCustomMetaTypesForJavaClass(className.toLatin1())){
                                if(QMetaType(mtype)==internalMetaType)
                                    matches = true;
                            }
                        }
                        if(!matches)
                            Java::Runtime::IllegalArgumentException::throwNew(env, QString("Wrong argument given: %1, expected: %2").arg(QtJambiAPI::getObjectClassName(env, val.l).replace("$", "."), QtJambiAPI::getClassName(env, Java::QtCore::QMetaType$GenericValue::getClass(env)).replace("$", ".")) QTJAMBI_STACKTRACEINFO );
                    }else{
                        QMetaType given = qtjambi_cast<QMetaType>(env, Java::QtCore::QMetaType$GenericTypeInterface::metaType(env, val.l));
                        if(internalMetaType!=given){
                            Java::Runtime::IllegalArgumentException::throwNew(env, QStringLiteral("Wrong argument given: %1, expected: %2").arg(given.name(), internalMetaType.name()) QTJAMBI_STACKTRACEINFO );
                        }
                    }
                    if(QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForJavaObject(env, val.l)){
                        ptr = link->pointer();
                    }else{
                        Java::QtJambi::QNoNativeResourcesException::throwNew(env, QStringLiteral("Incomplete object of type: %1").arg(QtJambiAPI::getObjectClassName(env, val.l).replace("$", ".")) QTJAMBI_STACKTRACEINFO );
                    }
                }
                if(scope && !out){
                    void** pptr;
                    out = pptr = new void*(ptr);
                    scope->addDeletion(pptr);
                }else if(out){
                    *reinterpret_cast<void**>(out) = ptr;
                }
                return out;
            };
        }else{
            return [internalMetaType](JNIEnv* env, QtJambiScope* scope, jvalue val, void* &out, jValueType) -> bool{
                if(val.l){
                    if(!Java::QtCore::QMetaType$GenericValue::isInstanceOf(env, val.l)){
                        QString className = QtJambiAPI::getObjectClassName(env, val.l);
                        bool matches = false;
                        if(const std::type_info* typeId = getTypeByJavaName(className)){
                            if(registeredMetaType(*typeId)==internalMetaType)
                                matches = true;
                        }
                        if(!matches){
                            for(auto mtype : registeredCustomMetaTypesForJavaClass(className.toLatin1())){
                                if(QMetaType(mtype)==internalMetaType)
                                    matches = true;
                            }
                        }
                        if(!matches)
                            Java::Runtime::IllegalArgumentException::throwNew(env, QString("Wrong argument given: %1, expected: %2").arg(QtJambiAPI::getObjectClassName(env, val.l).replace("$", "."), QtJambiAPI::getClassName(env, Java::QtCore::QMetaType$GenericValue::getClass(env)).replace("$", ".")) QTJAMBI_STACKTRACEINFO );
                    }else{
                        QMetaType given = qtjambi_cast<QMetaType>(env, Java::QtCore::QMetaType$GenericTypeInterface::metaType(env, val.l));
                        if(internalMetaType!=given){
                            Java::Runtime::IllegalArgumentException::throwNew(env, QStringLiteral("Wrong argument given: %1, expected: %2").arg(given.name(), internalMetaType.name()) QTJAMBI_STACKTRACEINFO );
                        }
                    }
                    if(QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForJavaObject(env, val.l)){
                        void* nptr = link->pointer();
                        if(scope && !out){
                            void* ptr;
                            out = ptr = internalMetaType.create(nptr);
                            scope->addDeletion(internalMetaType, ptr);
                        }else if(out){
                            internalMetaType.destruct(out);
                            if(internalMetaType.construct(out, nptr)!=out){
                                QByteArray ba;
                                bool saved;
                                /* write the copy to the stream */ {
                                    QDataStream stream(&ba, QIODevice::WriteOnly);
                                    saved = internalMetaType.save(stream, nptr);
                                }

                                /* read it back into the destination */
                                if(saved){
                                    QDataStream stream(&ba, QIODevice::ReadOnly);
                                    internalMetaType.load(stream, out);
                                }else{
                                    return false;
                                }
                            }
                        }else{
                            out = nptr;
                        }
                    }else{
                        Java::QtJambi::QNoNativeResourcesException::throwNew(env, QStringLiteral("Incomplete object of type: %1").arg(QtJambiAPI::getObjectClassName(env, val.l).replace("$", ".")) QTJAMBI_STACKTRACEINFO );
                    }
                    return true;
                }else{
                    if(scope && !out){
                        void* ptr;
                        out = ptr = internalMetaType.create();
                        scope->addDeletion(internalMetaType, ptr);
                    }
                    return out;
                }
            };
        }
    } else if (Java::QtCore::QMetaType$GenericObject::isSameClass(_env,externalClass)) {
        if(internalMetaType.flags() & QMetaType::IsPointer){
            return [internalMetaType](JNIEnv* env, QtJambiScope* scope, jvalue val, void* &out, jValueType) -> bool{
                void* ptr = nullptr;
                if(val.l){
                    if(!Java::QtCore::QMetaType$GenericObject::isInstanceOf(env, val.l)){
                        QString className = QtJambiAPI::getObjectClassName(env, val.l);
                        bool matches = false;
                        if(const std::type_info* typeId = getTypeByJavaName(className)){
                            if(registeredMetaType(*typeId)==internalMetaType)
                                matches = true;
                        }
                        if(!matches){
                            for(auto mtype : registeredCustomMetaTypesForJavaClass(className.toLatin1())){
                                if(QMetaType(mtype)==internalMetaType)
                                    matches = true;
                            }
                        }
                        if(!matches)
                            Java::Runtime::IllegalArgumentException::throwNew(env, QString("Wrong argument given: %1, expected: %2").arg(QtJambiAPI::getObjectClassName(env, val.l).replace("$", "."), QtJambiAPI::getClassName(env, Java::QtCore::QMetaType$GenericObject::getClass(env)).replace("$", ".")) QTJAMBI_STACKTRACEINFO );
                    }else{
                        QMetaType given = qtjambi_cast<QMetaType>(env, Java::QtCore::QMetaType$GenericTypeInterface::metaType(env, val.l));
                        if(internalMetaType!=given){
                            Java::Runtime::IllegalArgumentException::throwNew(env, QStringLiteral("Wrong argument given: %1, expected: %2").arg(given.name(), internalMetaType.name()) QTJAMBI_STACKTRACEINFO );
                        }
                    }
                    if(QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForJavaObject(env, val.l)){
                        ptr = link->pointer();
                    }else{
                        Java::QtJambi::QNoNativeResourcesException::throwNew(env, QStringLiteral("Incomplete object of type: %1").arg(QtJambiAPI::getObjectClassName(env, val.l).replace("$", ".")) QTJAMBI_STACKTRACEINFO );
                    }
                }
                if(scope && !out){
                    void** pptr;
                    out = pptr = new void*(ptr);
                    scope->addDeletion(pptr);
                }else if(out){
                    *reinterpret_cast<void**>(out) = ptr;
                }
                return out;
            };
        }else{
            return [internalMetaType](JNIEnv* env, QtJambiScope* scope, jvalue val, void* &out, jValueType) -> bool{
                if(val.l){
                    if(!Java::QtCore::QMetaType$GenericObject::isInstanceOf(env, val.l)){
                        QString className = QtJambiAPI::getObjectClassName(env, val.l);
                        bool matches = false;
                        if(const std::type_info* typeId = getTypeByJavaName(className)){
                            if(registeredMetaType(*typeId)==internalMetaType)
                                matches = true;
                        }
                        if(!matches){
                            for(auto mtype : registeredCustomMetaTypesForJavaClass(className.toLatin1())){
                                if(QMetaType(mtype)==internalMetaType)
                                    matches = true;
                            }
                        }
                        if(!matches)
                            Java::Runtime::IllegalArgumentException::throwNew(env, QString("Wrong argument given: %1, expected: %2").arg(QtJambiAPI::getObjectClassName(env, val.l).replace("$", "."), QtJambiAPI::getClassName(env, Java::QtCore::QMetaType$GenericObject::getClass(env)).replace("$", ".")) QTJAMBI_STACKTRACEINFO );
                    }else{
                        QMetaType given = qtjambi_cast<QMetaType>(env, Java::QtCore::QMetaType$GenericTypeInterface::metaType(env, val.l));
                        if(internalMetaType!=given){
                            Java::Runtime::IllegalArgumentException::throwNew(env, QStringLiteral("Wrong argument given: %1, expected: %2").arg(given.name(), internalMetaType.name()) QTJAMBI_STACKTRACEINFO );
                        }
                    }
                    if(QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForJavaObject(env, val.l)){
                        void* nptr = link->pointer();
                        if(scope && !out){
                            void* ptr;
                            out = ptr = internalMetaType.create(nptr);
                            scope->addDeletion(internalMetaType, ptr);
                        }else if(out){
                            internalMetaType.destruct(out);
                            if(internalMetaType.construct(out, nptr)!=out){
                                QByteArray ba;
                                bool saved;
                                /* write the copy to the stream */ {
                                    QDataStream stream(&ba, QIODevice::WriteOnly);
                                    saved = internalMetaType.save(stream, nptr);
                                }

                                /* read it back into the destination */
                                if(saved){
                                    QDataStream stream(&ba, QIODevice::ReadOnly);
                                    internalMetaType.load(stream, out);
                                }else{
                                    return false;
                                }
                            }
                        }else{
                            out = nptr;
                        }
                    }else{
                        Java::QtJambi::QNoNativeResourcesException::throwNew(env, QStringLiteral("Incomplete object of type: %1").arg(QtJambiAPI::getObjectClassName(env, val.l).replace("$", ".")) QTJAMBI_STACKTRACEINFO );
                    }
                    return true;
                }else{
                    if(scope && !out){
                        void* ptr;
                        out = ptr = internalMetaType.create();
                        scope->addDeletion(internalMetaType, ptr);
                    }
                    return out;
                }
            };
        }
    } else if (Java::QtCore::QMetaType$GenericGadget::isSameClass(_env,externalClass)) {
        if(pointerType==PointerType::NoPointer && internalTypeName.endsWith("&")){
            return [internalMetaType](JNIEnv* env, QtJambiScope*, jvalue val, void* &out, jValueType) -> bool{
                if(val.l){
                    if(!Java::QtCore::QMetaType$GenericGadget::isInstanceOf(env, val.l)){
                        QString className = QtJambiAPI::getObjectClassName(env, val.l);
                        bool matches = false;
                        if(const std::type_info* typeId = getTypeByJavaName(className)){
                            if(registeredMetaType(*typeId)==internalMetaType)
                                matches = true;
                        }
                        if(!matches){
                            for(auto mtype : registeredCustomMetaTypesForJavaClass(className.toLatin1())){
                                if(QMetaType(mtype)==internalMetaType)
                                    matches = true;
                            }
                        }
                        if(!matches)
                            Java::Runtime::IllegalArgumentException::throwNew(env, QString("Wrong argument given: %1, expected: %2").arg(QtJambiAPI::getObjectClassName(env, val.l).replace("$", "."), QtJambiAPI::getClassName(env, Java::QtCore::QMetaType$GenericGadget::getClass(env)).replace("$", ".")) QTJAMBI_STACKTRACEINFO );
                    }else{
                        QMetaType given = qtjambi_cast<QMetaType>(env, Java::QtCore::QMetaType$GenericTypeInterface::metaType(env, val.l));
                        if(internalMetaType!=given){
                            Java::Runtime::IllegalArgumentException::throwNew(env, QStringLiteral("Wrong argument given: %1, expected: %2").arg(given.name(), internalMetaType.name()) QTJAMBI_STACKTRACEINFO );
                        }
                    }
                    if(QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForJavaObject(env, val.l)){
                        out = link->pointer();
                    }else{
                        Java::QtJambi::QNoNativeResourcesException::throwNew(env, QStringLiteral("Incomplete object of type: %1").arg(QtJambiAPI::getObjectClassName(env, val.l).replace("$", ".")) QTJAMBI_STACKTRACEINFO );
                    }
                }else{
                    out = nullptr;
                }
                return true;
            };
        }else{
            return [internalMetaType](JNIEnv* env, QtJambiScope* scope, jvalue val, void* &out, jValueType) -> bool{
                void* ptr = nullptr;
                if(val.l){
                    if(!Java::QtCore::QMetaType$GenericGadget::isInstanceOf(env, val.l)){
                        QString className = QtJambiAPI::getObjectClassName(env, val.l);
                        bool matches = false;
                        if(const std::type_info* typeId = getTypeByJavaName(className)){
                            if(registeredMetaType(*typeId)==internalMetaType)
                                matches = true;
                        }
                        if(!matches){
                            for(auto mtype : registeredCustomMetaTypesForJavaClass(className.toLatin1())){
                                if(QMetaType(mtype)==internalMetaType)
                                    matches = true;
                            }
                        }
                        if(!matches)
                            Java::Runtime::IllegalArgumentException::throwNew(env, QString("Wrong argument given: %1, expected: %2").arg(QtJambiAPI::getObjectClassName(env, val.l).replace("$", "."), QtJambiAPI::getClassName(env, Java::QtCore::QMetaType$GenericGadget::getClass(env)).replace("$", ".")) QTJAMBI_STACKTRACEINFO );
                    }else{
                        QMetaType given = qtjambi_cast<QMetaType>(env, Java::QtCore::QMetaType$GenericTypeInterface::metaType(env, val.l));
                        if(internalMetaType!=given){
                            Java::Runtime::IllegalArgumentException::throwNew(env, QStringLiteral("Wrong argument given: %1, expected: %2").arg(given.name(), internalMetaType.name()) QTJAMBI_STACKTRACEINFO );
                        }
                    }
                    if(QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForJavaObject(env, val.l)){
                        ptr = link->pointer();
                    }else{
                        Java::QtJambi::QNoNativeResourcesException::throwNew(env, QStringLiteral("Incomplete object of type: %1").arg(QtJambiAPI::getObjectClassName(env, val.l).replace("$", ".")) QTJAMBI_STACKTRACEINFO );
                    }
                }
                if(scope && !out){
                    void** pptr;
                    out = pptr = new void*(ptr);
                    scope->addDeletion(pptr);
                }else if(out){
                    *reinterpret_cast<void**>(out) = nullptr;
                }
                return out;
            };
        }
    }else if(Java::QtJambi::QFlags::isAssignableFrom(_env,externalClass)){
        if(internalMetaType==QMetaType::fromType<JObjectWrapper>()){
            return [externalClass](JNIEnv* env, QtJambiScope* scope, jvalue val, void* &out, jValueType valueType) -> bool{
                jobject v;
                switch(valueType){
                case jValueType::l:
                    if(val.l && !env->IsInstanceOf(val.l, externalClass))
                        Java::Runtime::IllegalArgumentException::throwNew(env, QString("Wrong argument given: %1, expected: %2").arg(QtJambiAPI::getObjectClassName(env, val.l).replace("$", "."), QtJambiAPI::getClassName(env, externalClass).replace("$", ".")) QTJAMBI_STACKTRACEINFO );
                    v = val.l;
                    break;
                default:
                    v = QtJambiAPI::toJavaIntegerObject(env, val.i);
                    break;
                }
                if(scope && !out){
                    JObjectWrapper* ptr;
                    out = ptr = new JObjectWrapper;
                    scope->addDeletion(ptr);
                }
                if(!out)
                    return false;
                *reinterpret_cast<JObjectWrapper*>(out) = JObjectWrapper(env, v);
                return true;
            };
        }else if(Java::QtCore::QMetaType$GenericFlags::isSameClass(_env,externalClass)){
            return [internalMetaType](JNIEnv* env, QtJambiScope* scope, jvalue val, void* &out, jValueType valueType) -> bool{
                switch(valueType){
                case jValueType::l:
                    if(Java::QtCore::QMetaType$GenericFlags::isInstanceOf(env,val.l)){
                        QMetaType given = qtjambi_cast<QMetaType>(env, Java::QtCore::QMetaType$GenericTypeInterface::metaType(env, val.l));
                        if(internalMetaType!=given){
                            Java::Runtime::IllegalArgumentException::throwNew(env, QStringLiteral("Wrong argument given: %1, expected: %2").arg(given.name(), internalMetaType.name()) QTJAMBI_STACKTRACEINFO );
                        }
                        if(scope && !out){
                            qint32* ptr;
                            out = ptr = new qint32;
                            scope->addDeletion(ptr);
                        }
                        if(!out)
                            return false;
                        *reinterpret_cast<qint32*>(out) = int_for_QtEnumerator_or_QFlags(env, val.l).value<qint32>();
                    }else{
                        if(val.l){
                            QString className = QtJambiAPI::getObjectClassName(env, val.l);
                            bool matches = false;
                            if(const std::type_info* typeId = getTypeByJavaName(className)){
                                if(registeredMetaType(*typeId)==internalMetaType)
                                    matches = true;
                            }
                            if(!matches){
                                for(auto mtype : registeredCustomMetaTypesForJavaClass(className.toLatin1())){
                                    if(QMetaType(mtype)==internalMetaType)
                                        matches = true;
                                }
                            }
                            if(!matches && !env->IsInstanceOf(val.l, Java::Runtime::Number::getClass(env)))
                                Java::Runtime::IllegalArgumentException::throwNew(env, QString("Wrong argument given: %1, expected: %2").arg(QtJambiAPI::getObjectClassName(env, val.l).replace("$", "."), "any number") QTJAMBI_STACKTRACEINFO );
                        }
                        if(scope && !out){
                            qint32* ptr;
                            out = ptr = new qint32;
                            scope->addDeletion(ptr);
                        }
                        if(!out)
                            return false;
                        *reinterpret_cast<qint32*>(out) = !val.l ? 0 : int_for_QtEnumerator_or_QFlags(env, val.l).value<qint32>();
                    }
                    break;
                default:
                    if(scope && !out){
                        qint32* ptr;
                        out = ptr = new qint32;
                        scope->addDeletion(ptr);
                    }
                    if(!out)
                        return false;
                    *reinterpret_cast<qint32*>(out) = val.i;
                    break;
                }
                return true;
            };
#if QT_VERSION >= QT_VERSION_CHECK(6, 9, 0)
        }else if(Java::QtCore::QMetaType$GenericLongFlags::isSameClass(_env,externalClass)){
            return [internalMetaType](JNIEnv* env, QtJambiScope* scope, jvalue val, void* &out, jValueType valueType) -> bool{
                switch(valueType){
                case jValueType::l:
                    if(Java::QtCore::QMetaType$GenericLongFlags::isInstanceOf(env,val.l)){
                        QMetaType given = qtjambi_cast<QMetaType>(env, Java::QtCore::QMetaType$GenericTypeInterface::metaType(env, val.l));
                        if(internalMetaType!=given){
                            Java::Runtime::IllegalArgumentException::throwNew(env, QStringLiteral("Wrong argument given: %1, expected: %2").arg(given.name(), internalMetaType.name()) QTJAMBI_STACKTRACEINFO );
                        }
                        if(scope && !out){
                            qint64* ptr;
                            out = ptr = new qint64;
                            scope->addDeletion(ptr);
                        }
                        if(!out)
                            return false;
                        *reinterpret_cast<qint64*>(out) = int_for_QtEnumerator_or_QFlags(env, val.l).value<qint64>();
                    }else{
                        if(val.l){
                            QString className = QtJambiAPI::getObjectClassName(env, val.l);
                            bool matches = false;
                            if(const std::type_info* typeId = getTypeByJavaName(className)){
                                if(registeredMetaType(*typeId)==internalMetaType)
                                    matches = true;
                            }
                            if(!matches){
                                for(auto mtype : registeredCustomMetaTypesForJavaClass(className.toLatin1())){
                                    if(QMetaType(mtype)==internalMetaType)
                                        matches = true;
                                }
                            }
                            if(!matches && !env->IsInstanceOf(val.l, Java::Runtime::Number::getClass(env)))
                                Java::Runtime::IllegalArgumentException::throwNew(env, QString("Wrong argument given: %1, expected: %2").arg(QtJambiAPI::getObjectClassName(env, val.l).replace("$", "."), "any number") QTJAMBI_STACKTRACEINFO );
                        }
                        if(scope && !out){
                            qint64* ptr;
                            out = ptr = new qint64;
                            scope->addDeletion(ptr);
                        }
                        if(!out)
                            return false;
                        *reinterpret_cast<qint64*>(out) = !val.l ? 0 : int_for_QtEnumerator_or_QFlags(env, val.l).value<qint64>();
                    }
                    break;
                default:
                    if(scope && !out){
                        qint64* ptr;
                        out = ptr = new qint64;
                        scope->addDeletion(ptr);
                    }
                    if(!out)
                        return false;
                    *reinterpret_cast<qint64*>(out) = val.j;
                    break;
                }
                return true;
            };
#endif
        }else {
            return [internalMetaType, externalClass](JNIEnv* env, QtJambiScope* scope, jvalue val, void* &out, jValueType valueType) -> bool{
                switch(valueType){
                case jValueType::l:
                    if(Java::QtCore::QMetaType$GenericTypeInterface::isInstanceOf(env,val.l)){
                        QMetaType given = qtjambi_cast<QMetaType>(env, Java::QtCore::QMetaType$GenericTypeInterface::metaType(env, val.l));
                        if(internalMetaType!=given){
                            Java::Runtime::IllegalArgumentException::throwNew(env, QStringLiteral("Wrong argument given: %1, expected: %2").arg(given.name(), internalMetaType.name()) QTJAMBI_STACKTRACEINFO );
                        }
                        if(scope && !out){
                            qint32* ptr;
                            out = ptr = new qint32;
                            scope->addDeletion(ptr);
                        }
                        if(!out)
                            return false;
                        *reinterpret_cast<qint32*>(out) = int_for_QtEnumerator_or_QFlags(env, val.l).value<qint32>();
                    }else{
                        if(val.l && !env->IsInstanceOf(val.l, externalClass))
                            Java::Runtime::IllegalArgumentException::throwNew(env, QString("Wrong argument given: %1, expected: %2").arg(QtJambiAPI::getObjectClassName(env, val.l).replace("$", "."), QtJambiAPI::getClassName(env, externalClass).replace("$", ".")) QTJAMBI_STACKTRACEINFO );
                        if(scope && !out){
                            qint32* ptr;
                            out = ptr = new qint32;
                            scope->addDeletion(ptr);
                        }
                        if(!out)
                            return false;
                        *reinterpret_cast<qint32*>(out) = !val.l ? 0 : int_for_QtEnumerator_or_QFlags(env, val.l).value<qint32>();
                    }
                    break;
                default:
                    if(scope && !out){
                        qint32* ptr;
                        out = ptr = new qint32;
                        scope->addDeletion(ptr);
                    }
                    if(!out)
                        return false;
                    *reinterpret_cast<qint32*>(out) = val.i;
                    break;
                }
                return true;
            };
        }
    }else if((internalMetaType.flags() & QMetaType::IsEnumeration)
             || Java::QtJambi::QtEnumerator::isAssignableFrom(_env,externalClass)
             || Java::QtJambi::QtLongEnumerator::isAssignableFrom(_env,externalClass)
             || Java::QtJambi::QtShortEnumerator::isAssignableFrom(_env,externalClass)
             || Java::QtJambi::QtByteEnumerator::isAssignableFrom(_env,externalClass)
             || Java::Runtime::Enum::isAssignableFrom(_env,externalClass)){
        if(internalMetaType==QMetaType::fromType<JObjectWrapper>()){
            return [](JNIEnv* env, QtJambiScope* scope, jvalue val, void* &out, jValueType valueType) -> bool{
                jobject v;
                switch(valueType){
                case jValueType::l:
                    v = val.l;
                    break;
                default:
                    v = QtJambiAPI::toJavaIntegerObject(env, val.i);
                    break;
                }
                if(scope && !out){
                    JObjectWrapper* ptr;
                    out = ptr = new JObjectWrapper;
                    scope->addDeletion(ptr);
                }
                if(!out)
                    return false;
                *reinterpret_cast<JObjectWrapper*>(out) = JObjectWrapper(env, v);
                return true;
            };
        }else if(internalMetaType.isValid()){
            if(Java::QtCore::QMetaType$GenericEnumerator::isSameClass(_env, externalClass)
                    || Java::QtCore::QMetaType$GenericByteEnumerator::isSameClass(_env, externalClass)
                    || Java::QtCore::QMetaType$GenericShortEnumerator::isSameClass(_env, externalClass)
                    || Java::QtCore::QMetaType$GenericLongEnumerator::isSameClass(_env, externalClass)){
                return [internalMetaType](JNIEnv* env, QtJambiScope* scope, jvalue val, void* &out, jValueType valueType) -> bool{
                    QVariant value;
                    switch(valueType){
                    case jValueType::l:{
                        if(Java::QtCore::QMetaType$GenericTypeInterface::isInstanceOf(env,val.l)){
                            QMetaType given = qtjambi_cast<QMetaType>(env, Java::QtCore::QMetaType$GenericTypeInterface::metaType(env, val.l));
                            if(internalMetaType!=given){
                                Java::Runtime::IllegalArgumentException::throwNew(env, QStringLiteral("Wrong argument given: %1, expected: %2").arg(given.name(), internalMetaType.name()) QTJAMBI_STACKTRACEINFO );
                            }
                            value = int_for_QtEnumerator_or_QFlags(env, val.l);
                        }else if(Java::Runtime::Number::isInstanceOf(env, val.l)){
                            value = int_for_QtEnumerator_or_QFlags(env, val.l);
                        }else if(val.l){
                            QString className = QtJambiAPI::getObjectClassName(env, val.l);
                            bool matches = false;
                            if(const std::type_info* typeId = getTypeByJavaName(className)){
                                if(registeredMetaType(*typeId)==internalMetaType)
                                    matches = true;
                            }
                            if(!matches){
                                for(auto mtype : registeredCustomMetaTypesForJavaClass(className.toLatin1())){
                                    if(QMetaType(mtype)==internalMetaType)
                                        matches = true;
                                }
                            }
                            if(!matches
                                    && !Java::Runtime::Enum::isInstanceOf(env, val.l))
                                Java::Runtime::IllegalArgumentException::throwNew(env, QString("Wrong argument given: %1, expected: %2").arg(QtJambiAPI::getObjectClassName(env, val.l).replace("$", "."), "any number or enum") QTJAMBI_STACKTRACEINFO );
                            value = int_for_QtEnumerator_or_QFlags(env, val.l);
                        }else{
                            value = QVariant::fromValue(0);
                        }
                        break;
                    }
                    case jValueType::z:
                        value = QVariant::fromValue<bool>(val.z);
                    break;
                    case jValueType::b:
                        value = QVariant::fromValue<qint16>(val.b);
                    break;
                    case jValueType::s:
                        value = QVariant::fromValue<qint16>(val.s);
                    break;
                    case jValueType::i:
                        value = QVariant::fromValue<qint32>(val.i);
                    break;
                    case jValueType::j:
                        value = QVariant::fromValue<qint64>(val.j);
                    break;
                    case jValueType::f:
                        value = QVariant::fromValue<float>(val.f);
                    break;
                    case jValueType::d:
                        value = QVariant::fromValue<double>(val.d);
                    break;
                    case jValueType::c:
                        value = QVariant::fromValue<ushort>(val.c);
                    break;
                    }
                    if(scope && !out){
                        out = internalMetaType.create(value.data());
                        void* ptr = out;
                        scope->addDeletion(internalMetaType, ptr);
                    }else{
                        if(!out)
                            return false;
                        internalMetaType.destruct(out);
                        if(internalMetaType.construct(out, value.data())!=out){
                            QByteArray ba;
                            bool saved;
                            /* write the copy to the stream */ {
                                QDataStream stream(&ba, QIODevice::WriteOnly);
                                saved = internalMetaType.save(stream, value.data());
                            }

                            /* read it back into the destination */
                            if(saved){
                                QDataStream stream(&ba, QIODevice::ReadOnly);
                                internalMetaType.load(stream, out);
                            }else{
                                return false;
                            }
                        }
                    }
                    return true;
                };
            }else{
                return [internalMetaType, externalClass](JNIEnv* env, QtJambiScope* scope, jvalue val, void* &out, jValueType valueType) -> bool{
                    QVariant value;
                    switch(valueType){
                    case jValueType::l:{
                        if(Java::QtCore::QMetaType$GenericTypeInterface::isInstanceOf(env,val.l)){
                            QMetaType given = qtjambi_cast<QMetaType>(env, Java::QtCore::QMetaType$GenericTypeInterface::metaType(env, val.l));
                            if(internalMetaType!=given){
                                Java::Runtime::IllegalArgumentException::throwNew(env, QStringLiteral("Wrong argument given: %1, expected: %2").arg(given.name(), internalMetaType.name()) QTJAMBI_STACKTRACEINFO );
                            }
                            value = int_for_QtEnumerator_or_QFlags(env, val.l);
                        }else{
                            if(val.l && !env->IsInstanceOf(val.l, externalClass) && !Java::Runtime::Number::isInstanceOf(env, val.l))
                                Java::Runtime::IllegalArgumentException::throwNew(env, QString("Wrong argument given: %1, expected: %2").arg(QtJambiAPI::getObjectClassName(env, val.l).replace("$", "."), QtJambiAPI::getClassName(env, externalClass).replace("$", ".")) QTJAMBI_STACKTRACEINFO );
                            value = !val.l ? 0 : int_for_QtEnumerator_or_QFlags(env, val.l);
                        }
                        break;
                    }
                    case jValueType::z:
                        value = QVariant::fromValue<bool>(val.z);
                    break;
                    case jValueType::b:
                        value = QVariant::fromValue<qint16>(val.b);
                    break;
                    case jValueType::s:
                        value = QVariant::fromValue<qint16>(val.s);
                    break;
                    case jValueType::i:
                        value = QVariant::fromValue<qint32>(val.i);
                    break;
                    case jValueType::j:
                        value = QVariant::fromValue<qint64>(val.j);
                    break;
                    case jValueType::f:
                        value = QVariant::fromValue<float>(val.f);
                    break;
                    case jValueType::d:
                        value = QVariant::fromValue<double>(val.d);
                    break;
                    case jValueType::c:
                        value = QVariant::fromValue<ushort>(val.c);
                    break;
                    }
                    if(scope && !out){
                        out = internalMetaType.create(value.data());
                        void* ptr = out;
                        scope->addDeletion(internalMetaType, ptr);
                    }else{
                        if(!out)
                            return false;
                        internalMetaType.destruct(out);
                        if(internalMetaType.construct(out, value.data())!=out){
                            QByteArray ba;
                            bool saved;
                            /* write the copy to the stream */ {
                                QDataStream stream(&ba, QIODevice::WriteOnly);
                                saved = internalMetaType.save(stream, value.data());
                            }

                            /* read it back into the destination */
                            if(saved){
                                QDataStream stream(&ba, QIODevice::ReadOnly);
                                internalMetaType.load(stream, out);
                            }else{
                                return false;
                            }
                        }
                    }
                    return true;
                };
            }
        }else if(Java::QtJambi::QtEnumerator::isAssignableFrom(_env,externalClass)
                 || Java::Runtime::Enum::isAssignableFrom(_env,externalClass)){
            return [externalClass](JNIEnv* env, QtJambiScope* scope, jvalue val, void* &out, jValueType valueType) -> bool{
                switch(valueType){
                case jValueType::l:
                    if(val.l && !env->IsInstanceOf(val.l, externalClass))
                        Java::Runtime::IllegalArgumentException::throwNew(env, QString("Wrong argument given: %1, expected: %2").arg(QtJambiAPI::getObjectClassName(env, val.l).replace("$", "."), QtJambiAPI::getClassName(env, externalClass).replace("$", ".")) QTJAMBI_STACKTRACEINFO );
                    if(scope && !out){
                        qint32* ptr;
                        out = ptr = new qint32;
                        scope->addDeletion(ptr);
                    }
                    if(!out)
                        return false;
                    *reinterpret_cast<qint32*>(out) = !val.l ? 0 : int_for_QtEnumerator_or_QFlags(env, val.l).value<qint32>();
                    break;
                default:
                    if(scope && !out){
                        qint32* ptr;
                        out = ptr = new qint32;
                        scope->addDeletion(ptr);
                    }
                    if(!out)
                        return false;
                    *reinterpret_cast<qint32*>(out) = val.i;
                    break;
                }
                return true;
            };
        }else if (Java::QtJambi::QtLongEnumerator::isAssignableFrom(_env,externalClass)) {
            return [externalClass](JNIEnv* env, QtJambiScope* scope, jvalue val, void* &out, jValueType valueType) -> bool{
                switch(valueType){
                case jValueType::l:
                    if(val.l && !env->IsInstanceOf(val.l, externalClass))
                        Java::Runtime::IllegalArgumentException::throwNew(env, QString("Wrong argument given: %1, expected: %2").arg(QtJambiAPI::getObjectClassName(env, val.l).replace("$", "."), QtJambiAPI::getClassName(env, externalClass).replace("$", ".")) QTJAMBI_STACKTRACEINFO );
                    if(scope && !out){
                        qint64* ptr;
                        out = ptr = new qint64;
                        scope->addDeletion(ptr);
                    }
                    if(!out)
                        return false;
                    *reinterpret_cast<qint64*>(out) = !val.l ? 0 : int_for_QtEnumerator_or_QFlags(env, val.l).value<qint64>();
                    break;
                default:
                    if(scope && !out){
                        qint64* ptr;
                        out = ptr = new qint64;
                        scope->addDeletion(ptr);
                    }
                    if(!out)
                        return false;
                    *reinterpret_cast<qint64*>(out) = val.j;
                    break;
                }
                return true;
            };
        }else if (Java::QtJambi::QtShortEnumerator::isAssignableFrom(_env,externalClass)) {
            return [externalClass](JNIEnv* env, QtJambiScope* scope, jvalue val, void* &out, jValueType valueType) -> bool{
                switch(valueType){
                    case jValueType::l:
                    if(val.l && !env->IsInstanceOf(val.l, externalClass))
                        Java::Runtime::IllegalArgumentException::throwNew(env, QString("Wrong argument given: %1, expected: %2").arg(QtJambiAPI::getObjectClassName(env, val.l).replace("$", "."), QtJambiAPI::getClassName(env, externalClass).replace("$", ".")) QTJAMBI_STACKTRACEINFO );
                    if(scope && !out){
                        qint16* ptr;
                        out = ptr = new qint16;
                        scope->addDeletion(ptr);
                    }
                    if(!out)
                        return false;
                    *reinterpret_cast<qint16*>(out) = !val.l ? 0 : int_for_QtEnumerator_or_QFlags(env, val.l).value<qint16>();
                    break;
                default:
                    if(scope && !out){
                        qint16* ptr;
                        out = ptr = new qint16;
                        scope->addDeletion(ptr);
                    }
                    if(!out)
                        return false;
                    *reinterpret_cast<qint16*>(out) = val.s;
                    break;
                }
                return true;
            };
        }else if (Java::QtJambi::QtByteEnumerator::isAssignableFrom(_env,externalClass)) {
            return [externalClass](JNIEnv* env, QtJambiScope* scope, jvalue val, void* &out, jValueType valueType) -> bool{
                switch(valueType){
                    case jValueType::l:
                    if(val.l && !env->IsInstanceOf(val.l, externalClass))
                        Java::Runtime::IllegalArgumentException::throwNew(env, QString("Wrong argument given: %1, expected: %2").arg(QtJambiAPI::getObjectClassName(env, val.l).replace("$", "."), QtJambiAPI::getClassName(env, externalClass).replace("$", ".")) QTJAMBI_STACKTRACEINFO );
                    if(scope && !out){
                        qint8* ptr;
                        out = ptr = new qint8;
                        scope->addDeletion(ptr);
                    }
                    if(!out)
                        return false;
                    *reinterpret_cast<qint8*>(out) = !val.l ? 0 : int_for_QtEnumerator_or_QFlags(env, val.l).value<qint8>();
                    break;
                default:
                    if(scope && !out){
                        qint16* ptr;
                        out = ptr = new qint16;
                        scope->addDeletion(ptr);
                    }
                    if(!out)
                        return false;
                    *reinterpret_cast<qint16*>(out) = val.b;
                    break;
                }
                return true;
            };
        }
    }else if (pointerType != PointerType::NoPointer) {
        struct DeleterDeleter{
            PtrDeleterFunction deleter_function;
            void operator()(char* ptr){
                deleter_function(ptr, false);
            }
        };
        struct MetaTypeDeleter{
            QMetaType elementMetaType;
            void operator()(char* ptr){
                elementMetaType.destroy(ptr);
            }
        };

        QString instantiation = internalTypeName.chopped(1);
        QMetaType memberMetaType(QMetaType::UnknownType);
        const std::type_info* t = getTypeByQtName(instantiation);
        PtrDeleterFunction deleter_function{nullptr};
        bool isPointer{false};
        if(t){
            deleter_function = deleter(*t);
            EntryTypes entryType = getEntryType(*t);
            switch(entryType){
            case EntryTypes::QObjectTypeInfo:
            case EntryTypes::ObjectTypeInfo:
            case EntryTypes::InterfaceTypeInfo:
                memberMetaType = QMetaType::fromName(qPrintable(instantiation+"*"));
                isPointer = true;
                break;
            default:
                memberMetaType = registeredMetaType(*t);
                break;
            }
        }else{
            memberMetaType = QMetaType::fromName(qPrintable(instantiation));
            if(!memberMetaType.isValid()){
                memberMetaType = QMetaType::fromName(qPrintable(instantiation+"*"));
                isPointer = true;
            }
        }
        if(memberMetaType.isValid())
            isPointer = memberMetaType.flags() & QMetaType::IsPointer;
        bool isQObject = Java::QtCore::QObject::isAssignableFrom(_env, externalClass);
        QtJambiUtils::ExternalToInternalConverter memberReConverter = getExternalToInternalConverter(
                                                        _env,
                                                        externalClass,
                                                        instantiation, memberMetaType);
        switch(pointerType){
        case PointerType::SharedPointer:{
            if(isQObject){
                return [externalClass](JNIEnv* env, QtJambiScope* scope, jvalue val, void* &out, jValueType jvt)->bool{
                    if(jvt!=jValueType::l)
                        return false;
                    if(val.l && !env->IsInstanceOf(val.l, externalClass))
                        Java::Runtime::IllegalArgumentException::throwNew(env, QString("Wrong argument given: %1, expected: %2").arg(QtJambiAPI::getObjectClassName(env, val.l).replace("$", "."), QtJambiAPI::getClassName(env, externalClass).replace("$", ".")) QTJAMBI_STACKTRACEINFO );
                    if(scope && !out){
                        QSharedPointer<QObject>* ptr;
                        out = ptr = new QSharedPointer<QObject>;
                        scope->addDeletion(ptr);
                    }
                    if(!out)
                        return false;
                    QSharedPointer<QObject>& smartPointer = *reinterpret_cast<QSharedPointer<QObject>*>(out);
                    smartPointer = QtJambiAPI::convertJavaObjectToQSharedPointer(env, val.l);
                    return true;
                };
            }else if(t){
                return [externalClass, t](JNIEnv* env, QtJambiScope* scope, jvalue val, void* &out, jValueType jvt)->bool{
                    if(jvt!=jValueType::l)
                        return false;
                    if(val.l && !env->IsInstanceOf(val.l, externalClass))
                        Java::Runtime::IllegalArgumentException::throwNew(env, QString("Wrong argument given: %1, expected: %2").arg(QtJambiAPI::getObjectClassName(env, val.l).replace("$", "."), QtJambiAPI::getClassName(env, externalClass).replace("$", ".")) QTJAMBI_STACKTRACEINFO );
                    if(scope && !out){
                        QSharedPointer<char>* ptr;
                        out = ptr = new QSharedPointer<char>;
                        scope->addDeletion(ptr);
                    }
                    if(!out)
                        return false;
                    QSharedPointer<char>& smartPointer = *reinterpret_cast<QSharedPointer<char>*>(out);
                    smartPointer = QtJambiAPI::convertJavaObjectToQSharedPointer(env, t, val.l);
                    return true;
                };
            }else if(!isPointer && memberMetaType.isValid()){
                return [externalClass, memberReConverter, memberMetaType](JNIEnv* env, QtJambiScope* scope, jvalue val, void* &out, jValueType jvt)->bool{
                    if(jvt!=jValueType::l)
                        return false;
                    if(val.l && !env->IsInstanceOf(val.l, externalClass))
                        Java::Runtime::IllegalArgumentException::throwNew(env, QString("Wrong argument given: %1, expected: %2").arg(QtJambiAPI::getObjectClassName(env, val.l).replace("$", "."), QtJambiAPI::getClassName(env, externalClass).replace("$", ".")) QTJAMBI_STACKTRACEINFO );
                    if(scope && !out){
                        QSharedPointer<char>* ptr;
                        out = ptr = new QSharedPointer<char>;
                        scope->addDeletion(ptr);
                    }
                    if(!out)
                        return false;
                    QSharedPointer<char>& smartPointer = *reinterpret_cast<QSharedPointer<char>*>(out);
                    if(QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForJavaObject(env, val.l)){
                        link.clear();
                        smartPointer = QtJambiAPI::convertJavaObjectToQSharedPointer(env, nullptr, val.l);
                        return true;
                    }else{
                        char* ptr{nullptr};
                        void* out{&ptr};
                        if(memberReConverter(env, nullptr, val, out, jValueType::l)){
                            smartPointer.reset(ptr, MetaTypeDeleter{memberMetaType});
                        }
                        return true;
                    }
                    return false;
                };
            }else if(deleter_function){
                return [externalClass, memberReConverter, deleter_function](JNIEnv* env, QtJambiScope* scope, jvalue val, void* &out, jValueType jvt)->bool{
                    if(jvt!=jValueType::l)
                        return false;
                    if(val.l && !env->IsInstanceOf(val.l, externalClass))
                        Java::Runtime::IllegalArgumentException::throwNew(env, QString("Wrong argument given: %1, expected: %2").arg(QtJambiAPI::getObjectClassName(env, val.l).replace("$", "."), QtJambiAPI::getClassName(env, externalClass).replace("$", ".")) QTJAMBI_STACKTRACEINFO );
                    if(scope && !out){
                        QSharedPointer<char>* ptr;
                        out = ptr = new QSharedPointer<char>;
                        scope->addDeletion(ptr);
                    }
                    if(!out)
                        return false;
                    QSharedPointer<char>& smartPointer = *reinterpret_cast<QSharedPointer<char>*>(out);
                    if(QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForJavaObject(env, val.l)){
                        link.clear();
                        smartPointer = QtJambiAPI::convertJavaObjectToQSharedPointer(env, nullptr, val.l);
                        return true;
                    }else if(deleter_function){
                        char* ptr{nullptr};
                        void* out{&ptr};
                        if(memberReConverter(env, nullptr, val, out, jValueType::l)){
                            smartPointer.reset(ptr, DeleterDeleter{deleter_function});
                        }
                        return true;
                    }
                    return false;
                };
            }else{
                return [externalClass, t, memberReConverter](JNIEnv* env, QtJambiScope* scope, jvalue val, void* &out, jValueType jvt)->bool{
                    if(jvt!=jValueType::l)
                        return false;
                    if(val.l && !env->IsInstanceOf(val.l, externalClass))
                        Java::Runtime::IllegalArgumentException::throwNew(env, QString("Wrong argument given: %1, expected: %2").arg(QtJambiAPI::getObjectClassName(env, val.l).replace("$", "."), QtJambiAPI::getClassName(env, externalClass).replace("$", ".")) QTJAMBI_STACKTRACEINFO );
                    if(scope && !out){
                        QSharedPointer<char>* ptr;
                        out = ptr = new QSharedPointer<char>;
                        scope->addDeletion(ptr);
                    }
                    if(!out)
                        return false;
                    QSharedPointer<char>& smartPointer = *reinterpret_cast<QSharedPointer<char>*>(out);
                    if(QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForJavaObject(env, val.l)){
                        link.clear();
                        smartPointer = QtJambiAPI::convertJavaObjectToQSharedPointer(env, t, val.l);
                        return true;
                    }
                    return false;
                };
            }
        }
            break;
        case PointerType::WeakPointer:{
            if(isQObject){
                return [externalClass](JNIEnv* env, QtJambiScope* scope, jvalue val, void* &out, jValueType jvt)->bool{
                    if(jvt!=jValueType::l)
                        return false;
                    if(val.l && !env->IsInstanceOf(val.l, externalClass))
                        Java::Runtime::IllegalArgumentException::throwNew(env, QString("Wrong argument given: %1, expected: %2").arg(QtJambiAPI::getObjectClassName(env, val.l).replace("$", "."), QtJambiAPI::getClassName(env, externalClass).replace("$", ".")) QTJAMBI_STACKTRACEINFO );
                    if(scope && !out){
                        QWeakPointer<QObject>* ptr;
                        out = ptr = new QWeakPointer<QObject>;
                        scope->addDeletion(ptr);
                    }
                    if(!out)
                        return false;
                    QWeakPointer<QObject>& smartPointer = *reinterpret_cast<QWeakPointer<QObject>*>(out);
                    smartPointer = QtJambiAPI::convertJavaObjectToQWeakPointer(env, val.l);
                    return true;
                };
            }else{
                return [externalClass,t](JNIEnv* env, QtJambiScope* scope, jvalue val, void* &out, jValueType jvt)->bool{
                    if(jvt!=jValueType::l)
                        return false;
                    if(val.l && !env->IsInstanceOf(val.l, externalClass))
                        Java::Runtime::IllegalArgumentException::throwNew(env, QString("Wrong argument given: %1, expected: %2").arg(QtJambiAPI::getObjectClassName(env, val.l).replace("$", "."), QtJambiAPI::getClassName(env, externalClass).replace("$", ".")) QTJAMBI_STACKTRACEINFO );
                    if(scope && !out){
                        QWeakPointer<char>* ptr;
                        out = ptr = new QWeakPointer<char>;
                        scope->addDeletion(ptr);
                    }
                    if(!out)
                        return false;
                    QWeakPointer<char>& smartPointer = *reinterpret_cast<QWeakPointer<char>*>(out);
                    if(QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForJavaObject(env, val.l)){
                        link.clear();
                        smartPointer = QtJambiAPI::convertJavaObjectToQWeakPointer(env, t, val.l);
                        return true;
                    }
                    return false;
                };
            }
        }
            break;
        case PointerType::TrackingPointer:{
            if(isQObject){
                return [externalClass](JNIEnv* env, QtJambiScope* scope, jvalue val, void* &out, jValueType jvt)->bool{
                    if(jvt!=jValueType::l)
                        return false;
                    if(val.l && !env->IsInstanceOf(val.l, externalClass))
                        Java::Runtime::IllegalArgumentException::throwNew(env, QString("Wrong argument given: %1, expected: %2").arg(QtJambiAPI::getObjectClassName(env, val.l).replace("$", "."), QtJambiAPI::getClassName(env, externalClass).replace("$", ".")) QTJAMBI_STACKTRACEINFO );
                    if(scope && !out){
                        QPointer<QObject>* ptr;
                        out = ptr = new QPointer<QObject>;
                        scope->addDeletion(ptr);
                    }
                    if(!out)
                        return false;
                    QPointer<QObject>& smartPointer = *reinterpret_cast<QPointer<QObject>*>(out);
                    smartPointer = QtJambiAPI::convertJavaObjectToQObject(env, val.l);
                    return true;
                };
            }else{
                qCWarning(CATEGORY) << "Cannot convert QPointer of non-QObject to Java.";
                return ParameterTypeInfo::default_externalToInternalConverter();
            }
            break;
        }
        case PointerType::ScopedPointer:{
            if(isQObject){
                return [externalClass](JNIEnv* env, QtJambiScope* scope, jvalue val, void* &out, jValueType jvt)->bool{
                    if(jvt!=jValueType::l)
                        return false;
                    if(val.l && !env->IsInstanceOf(val.l, externalClass))
                        Java::Runtime::IllegalArgumentException::throwNew(env, QString("Wrong argument given: %1, expected: %2").arg(QtJambiAPI::getObjectClassName(env, val.l).replace("$", "."), QtJambiAPI::getClassName(env, externalClass).replace("$", ".")) QTJAMBI_STACKTRACEINFO );
                    if(scope && !out){
                        QScopedPointer<QObject>* ptr;
                        out = ptr = new QScopedPointer<QObject>;
                        scope->addDeletion(ptr);
                    }
                    if(!out)
                        return false;
                    QScopedPointer<QObject>& smartPointer = *reinterpret_cast<QScopedPointer<QObject>*>(out);
                    QObject* data = smartPointer.take();
                    smartPointer.reset(QtJambiAPI::convertJavaObjectToQObject(env, val.l));
                    if(data)
                        delete data;
                    return true;
                };
            }else if(!isPointer && memberMetaType.isValid()){
                return [externalClass,t,memberReConverter,memberMetaType](JNIEnv* env, QtJambiScope* scope, jvalue val, void* &out, jValueType jvt)->bool{
                    if(jvt!=jValueType::l)
                        return false;
                    if(val.l && !env->IsInstanceOf(val.l, externalClass))
                        Java::Runtime::IllegalArgumentException::throwNew(env, QString("Wrong argument given: %1, expected: %2").arg(QtJambiAPI::getObjectClassName(env, val.l).replace("$", "."), QtJambiAPI::getClassName(env, externalClass).replace("$", ".")) QTJAMBI_STACKTRACEINFO );
                    if(scope && !out){
                        QScopedPointer<char>* ptr;
                        out = ptr = new QScopedPointer<char>;
                        scope->addDeletion(ptr);
                    }
                    if(!out)
                        return false;
                    QScopedPointer<char>& smartPointer = *reinterpret_cast<QScopedPointer<char>*>(out);
                    if(QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForJavaObject(env, val.l)){
                        void* data = smartPointer.take();
                        smartPointer.reset(reinterpret_cast<char*>(t ? link->typedPointer(*t) : link->pointer()));
                        if(data)
                            memberMetaType.destroy(data);
                        return true;
                    }else{
                        char* ptr{nullptr};
                        void* out{&ptr};
                        if(memberReConverter(env, nullptr, val, out, jValueType::l)){
                            void* data = smartPointer.take();
                            smartPointer.reset(ptr);
                            if(data)
                                memberMetaType.destroy(data);
                        }
                        return true;
                    }
                    return false;
                };
            }else if(deleter_function){
                return [externalClass,t,memberReConverter,deleter_function](JNIEnv* env, QtJambiScope* scope, jvalue val, void* &out, jValueType jvt)->bool{
                    if(jvt!=jValueType::l)
                        return false;
                    if(val.l && !env->IsInstanceOf(val.l, externalClass))
                        Java::Runtime::IllegalArgumentException::throwNew(env, QString("Wrong argument given: %1, expected: %2").arg(QtJambiAPI::getObjectClassName(env, val.l).replace("$", "."), QtJambiAPI::getClassName(env, externalClass).replace("$", ".")) QTJAMBI_STACKTRACEINFO );
                    if(scope && !out){
                        QScopedPointer<char>* ptr;
                        out = ptr = new QScopedPointer<char>;
                        scope->addDeletion(ptr);
                    }
                    if(!out)
                        return false;
                    QScopedPointer<char>& smartPointer = *reinterpret_cast<QScopedPointer<char>*>(out);
                    if(QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForJavaObject(env, val.l)){
                        void* data = smartPointer.take();
                        smartPointer.reset(reinterpret_cast<char*>(t ? link->typedPointer(*t) : link->pointer()));
                        if(data)
                            deleter_function(data, false);
                        return true;
                    }else if(deleter_function){
                        char* ptr{nullptr};
                        void* out{&ptr};
                        if(memberReConverter(env, nullptr, val, out, jValueType::l)){
                            void* data = smartPointer.take();
                            smartPointer.reset(ptr);
                            if(data)
                                deleter_function(data, false);
                        }
                        return true;
                    }
                    return false;
                };
            }
            break;
        }
        case PointerType::shared_ptr:{
            if(isQObject){
                return [externalClass](JNIEnv* env, QtJambiScope* scope, jvalue val, void* &out, jValueType jvt)->bool{
                    if(jvt!=jValueType::l)
                        return false;
                    if(val.l && !env->IsInstanceOf(val.l, externalClass))
                        Java::Runtime::IllegalArgumentException::throwNew(env, QString("Wrong argument given: %1, expected: %2").arg(QtJambiAPI::getObjectClassName(env, val.l).replace("$", "."), QtJambiAPI::getClassName(env, externalClass).replace("$", ".")) QTJAMBI_STACKTRACEINFO );
                    if(scope && !out){
                        std::shared_ptr<QObject>* ptr;
                        out = ptr = new std::shared_ptr<QObject>;
                        scope->addDeletion(ptr);
                    }
                    if(!out)
                        return false;
                    std::shared_ptr<QObject>& smartPointer = *reinterpret_cast<std::shared_ptr<QObject>*>(out);
                    smartPointer = QtJambiAPI::convertJavaObjectToSharedPtr(env, val.l);
                    return true;
                };
            }else if(t){
                return [externalClass,t](JNIEnv* env, QtJambiScope* scope, jvalue val, void* &out, jValueType jvt)->bool{
                    if(jvt!=jValueType::l)
                        return false;
                    if(val.l && !env->IsInstanceOf(val.l, externalClass))
                        Java::Runtime::IllegalArgumentException::throwNew(env, QString("Wrong argument given: %1, expected: %2").arg(QtJambiAPI::getObjectClassName(env, val.l).replace("$", "."), QtJambiAPI::getClassName(env, externalClass).replace("$", ".")) QTJAMBI_STACKTRACEINFO );
                    if(scope && !out){
                        std::shared_ptr<char>* ptr;
                        out = ptr = new std::shared_ptr<char>;
                        scope->addDeletion(ptr);
                    }
                    if(!out)
                        return false;
                    std::shared_ptr<char>& smartPointer = *reinterpret_cast<std::shared_ptr<char>*>(out);
                    smartPointer = QtJambiAPI::convertJavaObjectToSharedPtr(env, t, val.l);
                    return true;
                };
            }else if(isPointer && memberMetaType.isValid()){
                return [externalClass,memberReConverter,memberMetaType](JNIEnv* env, QtJambiScope* scope, jvalue val, void* &out, jValueType jvt)->bool{
                    if(jvt!=jValueType::l)
                        return false;
                    if(val.l && !env->IsInstanceOf(val.l, externalClass))
                        Java::Runtime::IllegalArgumentException::throwNew(env, QString("Wrong argument given: %1, expected: %2").arg(QtJambiAPI::getObjectClassName(env, val.l).replace("$", "."), QtJambiAPI::getClassName(env, externalClass).replace("$", ".")) QTJAMBI_STACKTRACEINFO );
                    if(scope && !out){
                        std::shared_ptr<char>* ptr;
                        out = ptr = new std::shared_ptr<char>;
                        scope->addDeletion(ptr);
                    }
                    if(!out)
                        return false;
                    std::shared_ptr<char>& smartPointer = *reinterpret_cast<std::shared_ptr<char>*>(out);
                    if(QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForJavaObject(env, val.l)){
                        link.clear();
                        smartPointer = QtJambiAPI::convertJavaObjectToSharedPtr(env, nullptr, val.l);
                        return true;
                    }else{
                        char* ptr{nullptr};
                        void* out{&ptr};
                        if(memberReConverter(env, nullptr, val, out, jValueType::l)){
                            smartPointer.reset(ptr, MetaTypeDeleter{memberMetaType});
                        }
                        return true;
                    }
                    return false;
                };
            }else if(deleter_function){
                return [externalClass,memberReConverter,deleter_function](JNIEnv* env, QtJambiScope* scope, jvalue val, void* &out, jValueType jvt)->bool{
                    if(jvt!=jValueType::l)
                        return false;
                    if(val.l && !env->IsInstanceOf(val.l, externalClass))
                        Java::Runtime::IllegalArgumentException::throwNew(env, QString("Wrong argument given: %1, expected: %2").arg(QtJambiAPI::getObjectClassName(env, val.l).replace("$", "."), QtJambiAPI::getClassName(env, externalClass).replace("$", ".")) QTJAMBI_STACKTRACEINFO );
                    if(scope && !out){
                        std::shared_ptr<char>* ptr;
                        out = ptr = new std::shared_ptr<char>;
                        scope->addDeletion(ptr);
                    }
                    if(!out)
                        return false;
                    std::shared_ptr<char>& smartPointer = *reinterpret_cast<std::shared_ptr<char>*>(out);
                    if(QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForJavaObject(env, val.l)){
                        link.clear();
                        smartPointer = QtJambiAPI::convertJavaObjectToSharedPtr(env, nullptr, val.l);
                        return true;
                    }else if(deleter_function){
                        char* ptr{nullptr};
                        void* out{&ptr};
                        if(memberReConverter(env, nullptr, val, out, jValueType::l)){
                            smartPointer.reset(ptr, DeleterDeleter{deleter_function});
                        }
                        return true;
                    }
                    return false;
                };
            }else{
                return [externalClass,t](JNIEnv* env, QtJambiScope* scope, jvalue val, void* &out, jValueType jvt)->bool{
                    if(jvt!=jValueType::l)
                        return false;
                    if(val.l && !env->IsInstanceOf(val.l, externalClass))
                        Java::Runtime::IllegalArgumentException::throwNew(env, QString("Wrong argument given: %1, expected: %2").arg(QtJambiAPI::getObjectClassName(env, val.l).replace("$", "."), QtJambiAPI::getClassName(env, externalClass).replace("$", ".")) QTJAMBI_STACKTRACEINFO );
                    if(scope && !out){
                        std::shared_ptr<char>* ptr;
                        out = ptr = new std::shared_ptr<char>;
                        scope->addDeletion(ptr);
                    }
                    if(!out)
                        return false;
                    std::shared_ptr<char>& smartPointer = *reinterpret_cast<std::shared_ptr<char>*>(out);
                    if(QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForJavaObject(env, val.l)){
                        link.clear();
                        smartPointer = QtJambiAPI::convertJavaObjectToSharedPtr(env, t, val.l);
                        return true;
                    }
                    return false;
                };
            }
            break;
        }
        case PointerType::weak_ptr:{
            if(isQObject){
                return [externalClass](JNIEnv* env, QtJambiScope* scope, jvalue val, void* &out, jValueType jvt)->bool{
                    if(jvt!=jValueType::l)
                        return false;
                    if(val.l && !env->IsInstanceOf(val.l, externalClass))
                        Java::Runtime::IllegalArgumentException::throwNew(env, QString("Wrong argument given: %1, expected: %2").arg(QtJambiAPI::getObjectClassName(env, val.l).replace("$", "."), QtJambiAPI::getClassName(env, externalClass).replace("$", ".")) QTJAMBI_STACKTRACEINFO );
                    if(scope && !out){
                        std::weak_ptr<QObject>* ptr;
                        out = ptr = new std::weak_ptr<QObject>;
                        scope->addDeletion(ptr);
                    }
                    if(!out)
                        return false;
                    std::weak_ptr<QObject>& smartPointer = *reinterpret_cast<std::weak_ptr<QObject>*>(out);
                    smartPointer = QtJambiAPI::convertJavaObjectToWeakPtr(env, val.l);
                    return true;
                };
            }else{
                return [externalClass,t](JNIEnv* env, QtJambiScope* scope, jvalue val, void* &out, jValueType jvt)->bool{
                    if(jvt!=jValueType::l)
                        return false;
                    if(val.l && !env->IsInstanceOf(val.l, externalClass))
                        Java::Runtime::IllegalArgumentException::throwNew(env, QString("Wrong argument given: %1, expected: %2").arg(QtJambiAPI::getObjectClassName(env, val.l).replace("$", "."), QtJambiAPI::getClassName(env, externalClass).replace("$", ".")) QTJAMBI_STACKTRACEINFO );
                    if(scope && !out){
                        std::weak_ptr<char>* ptr;
                        out = ptr = new std::weak_ptr<char>;
                        scope->addDeletion(ptr);
                    }
                    if(!out)
                        return false;
                    std::weak_ptr<char>& smartPointer = *reinterpret_cast<std::weak_ptr<char>*>(out);
                    if(QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForJavaObject(env, val.l)){
                        link.clear();
                        smartPointer = QtJambiAPI::convertJavaObjectToWeakPtr(env, t, val.l);
                        return true;
                    }
                    return false;
                };
            }
            break;
        }
        case PointerType::ScopedArrayPointer:
            qCWarning(CATEGORY) << "Cannot convert QScopedArrayPointer to Java.";
            return ParameterTypeInfo::default_externalToInternalConverter();
        default: break;
        }
    }else if (internalMetaType.id()==QMetaType::QStringList
              || internalTypeName=="QStringList"
              || internalTypeName=="QList<QString>") {
        return [](JNIEnv* env, QtJambiScope* scope, jvalue val, void* &out, jValueType t) -> bool{
            if(t!=jValueType::l)
                return false;
            if(val.l && !env->IsInstanceOf(val.l, Java::Runtime::Collection::getClass(env)))
                Java::Runtime::IllegalArgumentException::throwNew(env, QString("Wrong argument given: %1, expected: %2").arg(QtJambiAPI::getObjectClassName(env, val.l).replace("$", "."), QtJambiAPI::getClassName(env, Java::Runtime::Collection::getClass(env))) QTJAMBI_STACKTRACEINFO );
            if(env->IsInstanceOf(val.l, Java::QtCore::QList::getClass(env))){
                if(QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForJavaObject(env, val.l)){
                    if(auto containerAccess = dynamic_cast<AbstractListAccess*>(link->containerAccess())){
                        if(containerAccess->elementMetaType().id()==QMetaType::QString){
                            if(out){
                                containerAccess->assign(out, link->pointer());
                                return true;
                            }else if(scope){
                                auto _containerAccess = containerAccess->clone();
                                CHECK_CONTAINER_ACCESS(env, _containerAccess)
                                out = _containerAccess->createContainer(env, ConstContainerAndAccessInfo{link->getJavaObjectLocalRef(env), link->pointer(), link->containerAccess()});
                                scope->addFinalAction([_containerAccess,out](){
                                    _containerAccess->deleteContainer(out);
                                    _containerAccess->dispose();
                                });
                                return true;
                            }else{
                                out = link->pointer();
                                return true;
                            }
                        }else{
                            Java::Runtime::IllegalArgumentException::throwNew(env, QString("Wrong argument given: QList<%1>, expected: QList<QString>").arg(QLatin1String(containerAccess->elementMetaType().name())) QTJAMBI_STACKTRACEINFO );
                        }
                    }
                }else{
                    Java::QtJambi::QNoNativeResourcesException::throwNew(env, QStringLiteral("Incomplete object of type: %1").arg(QtJambiAPI::getObjectClassName(env, val.l).replace("$", ".")) QTJAMBI_STACKTRACEINFO );
                }
            }
            QList<jstring> content;
            if(val.l){
                jobject iter = QtJambiAPI::iteratorOfJavaIterable(env, val.l);
                while(QtJambiAPI::hasJavaIteratorNext(env, iter)){
                    jobject next = QtJambiAPI::nextOfJavaIterator(env, iter);
                    if(next && !Java::Runtime::String::isInstanceOf(env, next))
                        Java::Runtime::IllegalArgumentException::throwNew(env, QString("Wrong collection content given: %1, expected: %2").arg(QtJambiAPI::getObjectClassName(env, next).replace("$", "."), QtJambiAPI::getClassName(env, Java::Runtime::String::getClass(env))) QTJAMBI_STACKTRACEINFO );
                    content << jstring(next);
                }
            }
            if(scope && !out){
                QStringList* ptr;
                out = ptr = new QStringList;
                scope->addDeletion(ptr);
            }
            if(!out)
                return false;
            {
                QStringList* list = reinterpret_cast<QStringList*>(out);
                for(jstring next : std::as_const(content)){
                    list->append(qtjambi_cast<QString>(env, next));
                }
            }
            return true;
        };
    }else if (internalMetaType.id()==QMetaType::QByteArrayList
              || internalTypeName=="QByteArrayList"
              || internalTypeName=="QList<QByteArray>") {
        return [](JNIEnv* env, QtJambiScope* scope, jvalue val, void* &out, jValueType t) -> bool{
            if(t!=jValueType::l)
                return false;
            if(val.l && !env->IsInstanceOf(val.l, Java::Runtime::Collection::getClass(env)))
                Java::Runtime::IllegalArgumentException::throwNew(env, QString("Wrong argument given: %1, expected: %2").arg(QtJambiAPI::getObjectClassName(env, val.l).replace("$", "."), QtJambiAPI::getClassName(env, Java::Runtime::Collection::getClass(env))) QTJAMBI_STACKTRACEINFO );
            if(env->IsInstanceOf(val.l, Java::QtCore::QList::getClass(env))){
                if(QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForJavaObject(env, val.l)){
                    if(auto containerAccess = dynamic_cast<AbstractListAccess*>(link->containerAccess())){
                        if(containerAccess->elementMetaType().id()==QMetaType::QByteArray){
                            if(out){
                                containerAccess->assign(out, link->pointer());
                                return true;
                            }else if(scope){
                                auto _containerAccess = containerAccess->clone();
                                CHECK_CONTAINER_ACCESS(env, _containerAccess)
                                out = _containerAccess->createContainer(env, ConstContainerAndAccessInfo{link->getJavaObjectLocalRef(env), link->pointer(), link->containerAccess()});
                                scope->addFinalAction([_containerAccess,out](){
                                    _containerAccess->deleteContainer(out);
                                    _containerAccess->dispose();
                                });
                                return true;
                            }else{
                                out = link->pointer();
                                return true;
                            }
                        }else{
                            Java::Runtime::IllegalArgumentException::throwNew(env, QString("Wrong argument given: QList<%1>, expected: QList<QByteArray>").arg(QLatin1String(containerAccess->elementMetaType().name())) QTJAMBI_STACKTRACEINFO );
                        }
                    }
                }else{
                    Java::QtJambi::QNoNativeResourcesException::throwNew(env, QStringLiteral("Incomplete object of type: %1").arg(QtJambiAPI::getObjectClassName(env, val.l).replace("$", ".")) QTJAMBI_STACKTRACEINFO );
                }
            }
            QList<jobject> content;
            if(val.l){
                jobject iter = QtJambiAPI::iteratorOfJavaIterable(env, val.l);
                while(QtJambiAPI::hasJavaIteratorNext(env, iter)){
                    jobject next = QtJambiAPI::nextOfJavaIterator(env, iter);
                    if(next && !env->IsInstanceOf(next, Java::QtCore::QByteArray::getClass(env)))
                        Java::Runtime::IllegalArgumentException::throwNew(env, QString("Wrong collection content given: %1, expected: %2").arg(QtJambiAPI::getObjectClassName(env, next).replace("$", "."), QtJambiAPI::getClassName(env, Java::QtCore::QByteArray::getClass(env))) QTJAMBI_STACKTRACEINFO );
                    content << next;
                }
            }
            if(scope && !out){
                QByteArrayList* ptr;
                out = ptr = new QByteArrayList;
                scope->addDeletion(ptr);
            }
            if(!out)
                return false;
            if(val.l){
                QByteArrayList* list = reinterpret_cast<QByteArrayList*>(out);
                for(jobject next : std::as_const(content)){
                    list->append(qtjambi_cast<QByteArray>(env, next));
                }
            }
            return true;
        };
    }else if (internalMetaType.id()==QMetaType::QVariantList
              || internalTypeName=="QVariantList"
              || internalTypeName=="QList<QVariant>") {
        return [](JNIEnv* env, QtJambiScope* scope, jvalue val, void* &out, jValueType t) -> bool{
            if(t!=jValueType::l)
                return false;
            if(val.l && !env->IsInstanceOf(val.l, Java::Runtime::Collection::getClass(env)))
                Java::Runtime::IllegalArgumentException::throwNew(env, QString("Wrong argument given: %1, expected: %2").arg(QtJambiAPI::getObjectClassName(env, val.l).replace("$", "."), QtJambiAPI::getClassName(env, Java::Runtime::Collection::getClass(env))) QTJAMBI_STACKTRACEINFO );
            if(env->IsInstanceOf(val.l, Java::QtCore::QList::getClass(env))){
                if(QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForJavaObject(env, val.l)){
                    if(auto containerAccess = dynamic_cast<AbstractListAccess*>(link->containerAccess())){
                        if(containerAccess->elementMetaType().id()==QMetaType::QVariant){
                            if(out){
                                containerAccess->assign(out, link->pointer());
                                return true;
                            }else if(scope){
                                auto _containerAccess = containerAccess->clone();
                                CHECK_CONTAINER_ACCESS(env, _containerAccess)
                                out = _containerAccess->createContainer(env, ConstContainerAndAccessInfo{link->getJavaObjectLocalRef(env), link->pointer(), link->containerAccess()});
                                scope->addFinalAction([_containerAccess,out](){
                                    _containerAccess->deleteContainer(out);
                                    _containerAccess->dispose();
                                });
                                return true;
                            }else{
                                out = link->pointer();
                                return true;
                            }
                        }
                    }
                }else{
                    Java::QtJambi::QNoNativeResourcesException::throwNew(env, QStringLiteral("Incomplete object of type: %1").arg(QtJambiAPI::getObjectClassName(env, val.l).replace("$", ".")) QTJAMBI_STACKTRACEINFO );
                }
            }
            if(scope && !out){
                QVariantList* ptr;
                out = ptr = new QVariantList;
                scope->addDeletion(ptr);
            }
            if(!out)
                return false;
            if(val.l){
                QVariantList* list = reinterpret_cast<QVariantList*>(out);
                jobject iter = QtJambiAPI::iteratorOfJavaIterable(env, val.l);
                while(QtJambiAPI::hasJavaIteratorNext(env, iter)){
                    jobject next = QtJambiAPI::nextOfJavaIterator(env, iter);
                    list->append(qtjambi_cast<QVariant>(env, next));
                }
            }
            return true;
        };
    }else if (internalMetaType.id()==QMetaType::QVariantMap
              || internalTypeName=="QVariantMap"
              || internalTypeName=="QMap<QString,QVariant>") {
        return [](JNIEnv* env, QtJambiScope* scope, jvalue val, void* &out, jValueType t) -> bool{
            if(t!=jValueType::l)
                return false;
            if(val.l && !env->IsInstanceOf(val.l, Java::Runtime::Map::getClass(env)))
                Java::Runtime::IllegalArgumentException::throwNew(env, QString("Wrong argument given: %1, expected: %2").arg(QtJambiAPI::getObjectClassName(env, val.l).replace("$", "."), QtJambiAPI::getClassName(env, Java::Runtime::Map::getClass(env))) QTJAMBI_STACKTRACEINFO );
            if(env->IsInstanceOf(val.l, Java::QtCore::QMap::getClass(env))){
                if(QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForJavaObject(env, val.l)){
                    if(auto containerAccess = dynamic_cast<AbstractMapAccess*>(link->containerAccess())){
                        if(containerAccess->keyMetaType().id()==QMetaType::QString){
                            if(containerAccess->valueMetaType().id()==QMetaType::QVariant){
                                if(out){
                                    containerAccess->assign(out, link->pointer());
                                    return true;
                                }else if(scope){
                                    auto _containerAccess = containerAccess->clone();
                                    CHECK_CONTAINER_ACCESS(env, _containerAccess)
                                    out = _containerAccess->createContainer(env, ConstContainerAndAccessInfo{link->getJavaObjectLocalRef(env), link->pointer(), link->containerAccess()});
                                    scope->addFinalAction([_containerAccess,out](){
                                        _containerAccess->deleteContainer(out);
                                        _containerAccess->dispose();
                                    });
                                    return true;
                                }else{
                                    out = link->pointer();
                                    return true;
                                }
                            }
                        }else{
                            Java::Runtime::IllegalArgumentException::throwNew(env, QString("Wrong argument given: QMap<%1,QVariant>, expected: QMap<QString,QVariant>").arg(QLatin1String(containerAccess->keyMetaType().name())) QTJAMBI_STACKTRACEINFO );
                        }
                    }
                }else{
                    Java::QtJambi::QNoNativeResourcesException::throwNew(env, QStringLiteral("Incomplete object of type: %1").arg(QtJambiAPI::getObjectClassName(env, val.l).replace("$", ".")) QTJAMBI_STACKTRACEINFO );
                }
            }
            QList<jobject> content;
            if(val.l){
                jobject iter = QtJambiAPI::entrySetIteratorOfJavaMap(env, val.l);
                while(QtJambiAPI::hasJavaIteratorNext(env, iter)){
                    jobject next = QtJambiAPI::nextOfJavaIterator(env, iter);
                    jobject key = QtJambiAPI::keyOfJavaMapEntry(env, next);
                    if(key && !Java::Runtime::String::isInstanceOf(env, key))
                        Java::Runtime::IllegalArgumentException::throwNew(env, QString("Wrong map key given: %1, expected: %2").arg(QtJambiAPI::getObjectClassName(env, key).replace("$", "."), QtJambiAPI::getClassName(env, Java::Runtime::String::getClass(env))) QTJAMBI_STACKTRACEINFO );
                    content << next;
                }
            }
            if(scope && !out){
                QMap<QString,QVariant>* ptr;
                out = ptr = new QMap<QString,QVariant>;
                scope->addDeletion(ptr);
            }
            if(!out)
                return false;
            {
                QMap<QString,QVariant>* map = reinterpret_cast<QMap<QString,QVariant>*>(out);
                for(jobject next : std::as_const(content)){
                    jobject key = QtJambiAPI::keyOfJavaMapEntry(env, next);
                    jobject value = QtJambiAPI::valueOfJavaMapEntry(env, next);
                    map->insert(qtjambi_cast<QString>(env, key), QtJambiAPI::convertJavaObjectToQVariant(env, value));
                }
            }
            return true;
        };
    }else if (internalMetaType.id()==QMetaType::QVariantHash
              || internalTypeName=="QVariantHash"
              || internalTypeName=="QHash<QString,QVariant>") {
        return [](JNIEnv* env, QtJambiScope* scope, jvalue val, void* &out, jValueType t) -> bool{
            if(t!=jValueType::l)
                return false;
            if(val.l && !env->IsInstanceOf(val.l, Java::Runtime::Map::getClass(env)))
                Java::Runtime::IllegalArgumentException::throwNew(env, QString("Wrong argument given: %1, expected: %2").arg(QtJambiAPI::getObjectClassName(env, val.l).replace("$", "."), QtJambiAPI::getClassName(env, Java::Runtime::Map::getClass(env))) QTJAMBI_STACKTRACEINFO );
            if(env->IsInstanceOf(val.l, Java::QtCore::QHash::getClass(env))){
                if(QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForJavaObject(env, val.l)){
                    if(auto containerAccess = dynamic_cast<AbstractMapAccess*>(link->containerAccess())){
                        if(containerAccess->keyMetaType().id()==QMetaType::QString){
                            if(containerAccess->valueMetaType().id()==QMetaType::QVariant){
                                if(out){
                                    containerAccess->assign(out, link->pointer());
                                    return true;
                                }else if(scope){
                                    auto _containerAccess = containerAccess->clone();
                                    CHECK_CONTAINER_ACCESS(env, _containerAccess)
                                    out = _containerAccess->createContainer(env, ConstContainerAndAccessInfo{link->getJavaObjectLocalRef(env), link->pointer(), link->containerAccess()});
                                    scope->addFinalAction([_containerAccess,out](){
                                        _containerAccess->deleteContainer(out);
                                        _containerAccess->dispose();
                                    });
                                    return true;
                                }else{
                                    out = link->pointer();
                                    return true;
                                }
                            }
                        }else{
                            Java::Runtime::IllegalArgumentException::throwNew(env, QString("Wrong argument given: QHash<%1,QVariant>, expected: QHash<QString,QVariant>").arg(QLatin1String(containerAccess->keyMetaType().name())) QTJAMBI_STACKTRACEINFO );
                        }
                    }
                }else{
                    Java::QtJambi::QNoNativeResourcesException::throwNew(env, QStringLiteral("Incomplete object of type: %1").arg(QtJambiAPI::getObjectClassName(env, val.l).replace("$", ".")) QTJAMBI_STACKTRACEINFO );
                }
            }
            QList<jobject> content;
            if(val.l){
                jobject iter = QtJambiAPI::entrySetIteratorOfJavaMap(env, val.l);
                while(QtJambiAPI::hasJavaIteratorNext(env, iter)){
                    jobject next = QtJambiAPI::nextOfJavaIterator(env, iter);
                    jobject key = QtJambiAPI::keyOfJavaMapEntry(env, next);
                    if(key && !Java::Runtime::String::isInstanceOf(env, key))
                        Java::Runtime::IllegalArgumentException::throwNew(env, QString("Wrong map key given: %1, expected: %2").arg(QtJambiAPI::getObjectClassName(env, key).replace("$", "."), QtJambiAPI::getClassName(env, Java::Runtime::String::getClass(env))) QTJAMBI_STACKTRACEINFO );
                    content << next;
                }
            }
            if(scope && !out){
                QHash<QString,QVariant>* ptr;
                out = ptr = new QHash<QString,QVariant>;
                scope->addDeletion(ptr);
            }
            if(!out)
                return false;
            {
                QHash<QString,QVariant>* map = reinterpret_cast<QHash<QString,QVariant>*>(out);
                for(jobject next : std::as_const(content)){
                    jobject key = QtJambiAPI::keyOfJavaMapEntry(env, next);
                    jobject value = QtJambiAPI::valueOfJavaMapEntry(env, next);
                    map->insert(qtjambi_cast<QString>(env, key), QtJambiAPI::convertJavaObjectToQVariant(env, value));
                }
            }
            return true;
        };
    }else if(internalMetaType.id()==QMetaType::QVariantPair
              || internalTypeName=="QVariantPair"
              || internalTypeName=="QPair<QVariant,QVariant>"){
        return [](JNIEnv* env, QtJambiScope* scope, jvalue in, void* &out, jValueType valueType) -> bool{
            bool response = false;
            if(valueType==jValueType::l){
                if(in.l && !env->IsInstanceOf(in.l, Java::QtCore::QPair::getClass(env)))
                    Java::Runtime::IllegalArgumentException::throwNew(env, QString("Wrong argument given: %1, expected: %2").arg(QtJambiAPI::getObjectClassName(env, in.l).replace("$", "."), QtJambiAPI::getClassName(env, Java::QtCore::QPair::getClass(env)).replace("$", ".")) QTJAMBI_STACKTRACEINFO );
                if(!out){
                    if(scope){
                        out = new QVariantPair();
                    }
                }
                if(out && in.l){
                    response = true;
                    jobject first = Java::QtCore::QPair::first(env, in.l);
                    jobject second = Java::QtCore::QPair::second(env, in.l);
                    reinterpret_cast<QVariantPair*>(out)->first = qtjambi_cast<QVariant>(env, first);
                    reinterpret_cast<QVariantPair*>(out)->second = qtjambi_cast<QVariant>(env, second);
                }
            }
            return response;
        };
#if defined(QTJAMBI_LIGHTWEIGHT_MODELINDEX)
    }else if (Java::QtCore::QModelIndex::isAssignableFrom(_env,externalClass) && (!internalMetaType.isValid() || QByteArray("QModelIndex")==internalMetaType.name() || QByteArray("const QModelIndex&")==internalMetaType.name())) {
        return [](JNIEnv* env, QtJambiScope* scope, jvalue val, void* &out, jValueType) -> bool{
            if(val.l && !Java::QtCore::QModelIndex::isInstanceOf(env, val.l))
                Java::Runtime::IllegalArgumentException::throwNew(env, QString("Wrong argument given: %1, expected: %2").arg(QtJambiAPI::getObjectClassName(env, val.l).replace("$", "."), QtJambiAPI::getClassName(env, Java::QtCore::QMetaObject::getClass(env))) QTJAMBI_STACKTRACEINFO );
            if(scope && !out){
                QModelIndex* ptr;
                out = ptr = new QModelIndex;
                scope->addDeletion(ptr);
            }
            if(!out)
                return false;
            *reinterpret_cast<QModelIndex*>(out) = qtjambi_cast<QModelIndex>(env, val.l);
            return true;
        };
#endif
    }else if (Java::QtCore::QMetaObject::isAssignableFrom(_env,externalClass) && (!internalMetaType.isValid() || QByteArray("QMetaObject*")==internalMetaType.name() || QByteArray("const QMetaObject*")==internalMetaType.name())) {
        return [](JNIEnv* env, QtJambiScope* scope, jvalue val, void* &out, jValueType) -> bool{
            if(val.l && !Java::QtCore::QMetaObject::isInstanceOf(env, val.l))
                Java::Runtime::IllegalArgumentException::throwNew(env, QString("Wrong argument given: %1, expected: %2").arg(QtJambiAPI::getObjectClassName(env, val.l).replace("$", "."), QtJambiAPI::getClassName(env, Java::QtCore::QMetaObject::getClass(env))) QTJAMBI_STACKTRACEINFO );
            if(scope && !out){
                QMetaObject** ptr;
                out = ptr = new QMetaObject*;
                scope->addDeletion(ptr);
            }
            if(!out)
                return false;
            *reinterpret_cast<const QMetaObject**>(out) = qtjambi_cast<const QMetaObject*>(env, val.l);
            return true;
        };
    }else if (Java::QtCore::QMetaObject$Connection::isAssignableFrom(_env,externalClass) && (!internalMetaType.isValid() || QByteArray("QMetaObject::Connection")==internalMetaType.name())) {
        return [](JNIEnv* env, QtJambiScope* scope, jvalue val, void* &out, jValueType) -> bool{
            if(val.l && !env->IsInstanceOf(val.l, Java::QtCore::QMetaObject$Connection::getClass(env)))
                Java::Runtime::IllegalArgumentException::throwNew(env, QString("Wrong argument given: %1, expected: %2").arg(QtJambiAPI::getObjectClassName(env, val.l).replace("$", "."), QtJambiAPI::getClassName(env, Java::QtCore::QMetaObject$Connection::getClass(env)).replace("$", ".")) QTJAMBI_STACKTRACEINFO );
            if(scope && !out){
                QMetaObject::Connection* ptr;
                out = ptr = new QMetaObject::Connection;
                scope->addDeletion(ptr);
            }
            if(!out)
                return false;
            *reinterpret_cast<QMetaObject::Connection*>(out) = qtjambi_cast<QMetaObject::Connection>(env, val.l);
            return true;
        };
    }else if(isArrayClass && (pointerType==PointerType::initializer_list || internalTypeName.startsWith("initializer_list<"))){
        jclass componentClass = Java::Runtime::Class::getComponentType(_env,externalClass);
        QString javaTypeName = QtJambiAPI::getClassName(_env, jclass(componentClass)).replace(".", "/");
        Q_UNUSED(javaTypeName)
        QString _internalTypeName = internalTypeName;
        if(_internalTypeName.startsWith("initializer_list<"))
            _internalTypeName = _internalTypeName.mid(22, _internalTypeName.length() - 23);
        if(const std::type_info* typeId = getTypeByQtName(_internalTypeName)){
            QMetaType metaType = QMetaType::fromName(qPrintable(_internalTypeName));
            if(!metaType.isValid()){
                metaType = registeredMetaType(*typeId);
            }
            QPair<size_t,size_t> sizeAlign = getValueSizeAndAlignment(*typeId);
            if(sizeAlign.first>0){
                if(Java::Runtime::Byte::isPrimitiveType(_env,componentClass)){
                    return [](JNIEnv* env, QtJambiScope* scope, jvalue in, void* &out, jValueType) -> bool {
                                if(scope){
                                    if(in.l){
                                        jclass objectClass = env->GetObjectClass(in.l);
                                        if(!Java::Runtime::Class::isArray(env, objectClass) || !Java::Runtime::Byte::isPrimitiveType(env, Java::Runtime::Class::getComponentType(env, objectClass)))
                                            Java::Runtime::IllegalArgumentException::throwNew(env, QString("Wrong argument given: %1, expected: byte[]").arg(QtJambiAPI::getClassName(env, objectClass)) QTJAMBI_STACKTRACEINFO );
                                        if(!out){
                                            std::initializer_list<jbyte>* _out = new std::initializer_list<jbyte>;
                                            out = _out;
                                            scope->addDeletion(_out);
                                        }
                                        jsize arrayLength = env->GetArrayLength(jarray(in.l));
                                        jbyte* data = new jbyte[size_t(arrayLength)];
                                        env->GetByteArrayRegion(jbyteArray(in.l), 0, arrayLength, data);
                                        scope->addArrayDeletion(data);
#ifdef Q_CC_MSVC
                                        *reinterpret_cast<std::initializer_list<jbyte>*>(out) = std::initializer_list<jbyte>(data, data + size_t(arrayLength));
#else
                                        struct jbyte_initializer_list_access{
                                            jbyte* _M_array;
                                            size_t _M_len;
                                        };
                                        reinterpret_cast<jbyte_initializer_list_access*>(out)->_M_array = data;
                                        reinterpret_cast<jbyte_initializer_list_access*>(out)->_M_len = size_t(arrayLength);
#endif
                                    }
                                }else{
                                    return false;
                                }
                                return true;
                            };
                }else if(Java::Runtime::Short::isPrimitiveType(_env,componentClass)){
                    return [](JNIEnv* env, QtJambiScope* scope, jvalue in, void* &out, jValueType) -> bool {
                                if(scope){
                                    if(in.l){
                                        jclass objectClass = env->GetObjectClass(in.l);
                                        if(!Java::Runtime::Class::isArray(env, objectClass) || !Java::Runtime::Short::isPrimitiveType(env, Java::Runtime::Class::getComponentType(env, objectClass)))
                                            Java::Runtime::IllegalArgumentException::throwNew(env, QString("Wrong argument given: %1, expected: short[]").arg(QtJambiAPI::getClassName(env, objectClass)) QTJAMBI_STACKTRACEINFO );
                                        if(!out){
                                            std::initializer_list<jshort>* _out = new std::initializer_list<jshort>;
                                            out = _out;
                                            scope->addDeletion(_out);
                                        }
                                        jsize arrayLength = env->GetArrayLength(jarray(in.l));
                                        jshort* data = new jshort[size_t(arrayLength)];
                                        env->GetShortArrayRegion(jshortArray(in.l), 0, arrayLength, data);
                                        scope->addArrayDeletion(data);
#ifdef Q_CC_MSVC
                                        *reinterpret_cast<std::initializer_list<jshort>*>(out) = std::initializer_list<jshort>(data, data + size_t(arrayLength));
#else
                                        struct jshort_initializer_list_access{
                                            jshort* _M_array;
                                            size_t _M_len;
                                        };
                                        reinterpret_cast<jshort_initializer_list_access*>(out)->_M_array = data;
                                        reinterpret_cast<jshort_initializer_list_access*>(out)->_M_len = size_t(arrayLength);
#endif
                                    }
                                }else{
                                    return false;
                                }
                                return true;
                            };
                }else if(Java::Runtime::Integer::isPrimitiveType(_env,componentClass)){
                    return [](JNIEnv* env, QtJambiScope* scope, jvalue in, void* &out, jValueType) -> bool {
                                if(scope){
                                    if(in.l){
                                        jclass objectClass = env->GetObjectClass(in.l);
                                        if(!Java::Runtime::Class::isArray(env, objectClass) || !Java::Runtime::Integer::isPrimitiveType(env, Java::Runtime::Class::getComponentType(env, objectClass)))
                                            Java::Runtime::IllegalArgumentException::throwNew(env, QString("Wrong argument given: %1, expected: int[]").arg(QtJambiAPI::getClassName(env, objectClass)) QTJAMBI_STACKTRACEINFO );
                                        if(!out){
                                            std::initializer_list<jint>* _out = new std::initializer_list<jint>;
                                            out = _out;
                                            scope->addDeletion(_out);
                                        }
                                        jsize arrayLength = env->GetArrayLength(jarray(in.l));
                                        jint* data = new jint[size_t(arrayLength)];
                                        env->GetIntArrayRegion(jintArray(in.l), 0, arrayLength, data);
                                        scope->addArrayDeletion(data);
#ifdef Q_CC_MSVC
                                        *reinterpret_cast<std::initializer_list<jint>*>(out) = std::initializer_list<jint>(data, data + size_t(arrayLength));
#else
                                        struct jint_initializer_list_access{
                                            jint* _M_array;
                                            size_t _M_len;
                                        };
                                        reinterpret_cast<jint_initializer_list_access*>(out)->_M_array = data;
                                        reinterpret_cast<jint_initializer_list_access*>(out)->_M_len = size_t(arrayLength);
#endif
                                    }
                                }else{
                                    return false;
                                }
                                return true;
                            };
                }else if(Java::Runtime::Long::isPrimitiveType(_env,componentClass)){
                    return [](JNIEnv* env, QtJambiScope* scope, jvalue in, void* &out, jValueType) -> bool {
                                if(scope){
                                    if(in.l){
                                        jclass objectClass = env->GetObjectClass(in.l);
                                        if(!Java::Runtime::Class::isArray(env, objectClass) || !Java::Runtime::Long::isPrimitiveType(env, Java::Runtime::Class::getComponentType(env, objectClass)))
                                            Java::Runtime::IllegalArgumentException::throwNew(env, QString("Wrong argument given: %1, expected: long[]").arg(QtJambiAPI::getClassName(env, objectClass)) QTJAMBI_STACKTRACEINFO );
                                        if(!out){
                                            std::initializer_list<jlong>* _out = new std::initializer_list<jlong>;
                                            out = _out;
                                            scope->addDeletion(_out);
                                        }
                                        jsize arrayLength = env->GetArrayLength(jarray(in.l));
                                        jlong* data = new jlong[size_t(arrayLength)];
                                        env->GetLongArrayRegion(jlongArray(in.l), 0, arrayLength, data);
                                        scope->addArrayDeletion(data);
#ifdef Q_CC_MSVC
                                        *reinterpret_cast<std::initializer_list<jlong>*>(out) = std::initializer_list<jlong>(data, data + size_t(arrayLength));
#else
                                        struct jlong_initializer_list_access{
                                            jlong* _M_array;
                                            size_t _M_len;
                                        };
                                        reinterpret_cast<jlong_initializer_list_access*>(out)->_M_array = data;
                                        reinterpret_cast<jlong_initializer_list_access*>(out)->_M_len = size_t(arrayLength);
#endif
                                    }
                                }else{
                                    return false;
                                }
                                return true;
                            };
                }else if(Java::Runtime::Float::isPrimitiveType(_env,componentClass)){
                    return [](JNIEnv* env, QtJambiScope* scope, jvalue in, void* &out, jValueType) -> bool {
                                if(scope){
                                    if(in.l){
                                        jclass objectClass = env->GetObjectClass(in.l);
                                        if(!Java::Runtime::Class::isArray(env, objectClass) || !Java::Runtime::Float::isPrimitiveType(env, Java::Runtime::Class::getComponentType(env, objectClass)))
                                            Java::Runtime::IllegalArgumentException::throwNew(env, QString("Wrong argument given: %1, expected: float[]").arg(QtJambiAPI::getClassName(env, objectClass)) QTJAMBI_STACKTRACEINFO );
                                        if(!out){
                                            std::initializer_list<jfloat>* _out = new std::initializer_list<jfloat>;
                                            out = _out;
                                            scope->addDeletion(_out);
                                        }
                                        jsize arrayLength = env->GetArrayLength(jarray(in.l));
                                        jfloat* data = new jfloat[size_t(arrayLength)];
                                        env->GetFloatArrayRegion(jfloatArray(in.l), 0, arrayLength, data);
                                        scope->addArrayDeletion(data);
#ifdef Q_CC_MSVC
                                        *reinterpret_cast<std::initializer_list<jfloat>*>(out) = std::initializer_list<jfloat>(data, data + size_t(arrayLength));
#else
                                        struct jfloat_initializer_list_access{
                                            jfloat* _M_array;
                                            size_t _M_len;
                                        };
                                        reinterpret_cast<jfloat_initializer_list_access*>(out)->_M_array = data;
                                        reinterpret_cast<jfloat_initializer_list_access*>(out)->_M_len = size_t(arrayLength);
#endif
                                    }
                                }else{
                                    return false;
                                }
                                return true;
                            };
                }else if(Java::Runtime::Double::isPrimitiveType(_env,componentClass)){
                    return [](JNIEnv* env, QtJambiScope* scope, jvalue in, void* &out, jValueType) -> bool {
                                if(scope){
                                    if(in.l){
                                        jclass objectClass = env->GetObjectClass(in.l);
                                        if(!Java::Runtime::Class::isArray(env, objectClass) || !Java::Runtime::Double::isPrimitiveType(env, Java::Runtime::Class::getComponentType(env, objectClass)))
                                            Java::Runtime::IllegalArgumentException::throwNew(env, QString("Wrong argument given: %1, expected: double[]").arg(QtJambiAPI::getClassName(env, objectClass)) QTJAMBI_STACKTRACEINFO );
                                        if(!out){
                                            std::initializer_list<jdouble>* _out = new std::initializer_list<jdouble>;
                                            out = _out;
                                            scope->addDeletion(_out);
                                        }
                                        jsize arrayLength = env->GetArrayLength(jarray(in.l));
                                        jdouble* data = new jdouble[size_t(arrayLength)];
                                        env->GetDoubleArrayRegion(jdoubleArray(in.l), 0, arrayLength, data);
                                        scope->addArrayDeletion(data);
#ifdef Q_CC_MSVC
                                        *reinterpret_cast<std::initializer_list<jdouble>*>(out) = std::initializer_list<jdouble>(data, data + size_t(arrayLength));
#else
                                        struct jdouble_initializer_list_access{
                                            jdouble* _M_array;
                                            size_t _M_len;
                                        };
                                        reinterpret_cast<jdouble_initializer_list_access*>(out)->_M_array = data;
                                        reinterpret_cast<jdouble_initializer_list_access*>(out)->_M_len = size_t(arrayLength);
#endif
                                    }
                                }else{
                                    return false;
                                }
                                return true;
                            };
                }else if(Java::Runtime::Character::isPrimitiveType(_env,componentClass)){
                    return [](JNIEnv* env, QtJambiScope* scope, jvalue in, void* &out, jValueType) -> bool {
                                if(scope){
                                    if(in.l){
                                        jclass objectClass = env->GetObjectClass(in.l);
                                        if(!Java::Runtime::Class::isArray(env, objectClass) || !Java::Runtime::Character::isPrimitiveType(env, Java::Runtime::Class::getComponentType(env, objectClass)))
                                            Java::Runtime::IllegalArgumentException::throwNew(env, QString("Wrong argument given: %1, expected: char[]").arg(QtJambiAPI::getClassName(env, objectClass)) QTJAMBI_STACKTRACEINFO );
                                        if(!out){
                                            std::initializer_list<jchar>* _out = new std::initializer_list<jchar>;
                                            out = _out;
                                            scope->addDeletion(_out);
                                        }
                                        jsize arrayLength = env->GetArrayLength(jarray(in.l));
                                        jchar* data = new jchar[size_t(arrayLength)];
                                        env->GetCharArrayRegion(jcharArray(in.l), 0, arrayLength, data);
                                        scope->addArrayDeletion(data);
#ifdef Q_CC_MSVC
                                        *reinterpret_cast<std::initializer_list<jchar>*>(out) = std::initializer_list<jchar>(data, data + size_t(arrayLength));
#else
                                        struct jchar_initializer_list_access{
                                            jchar* _M_array;
                                            size_t _M_len;
                                        };
                                        reinterpret_cast<jchar_initializer_list_access*>(out)->_M_array = data;
                                        reinterpret_cast<jchar_initializer_list_access*>(out)->_M_len = size_t(arrayLength);
#endif
                                    }
                                }else{
                                    return false;
                                }
                                return true;
                            };
                }else if(Java::Runtime::Boolean::isPrimitiveType(_env,componentClass)){
                    return [](JNIEnv* env, QtJambiScope* scope, jvalue in, void* &out, jValueType) -> bool {
                                if(scope){
                                    if(in.l){
                                        jclass objectClass = env->GetObjectClass(in.l);
                                        if(!Java::Runtime::Class::isArray(env, objectClass) || !Java::Runtime::Boolean::isPrimitiveType(env, Java::Runtime::Class::getComponentType(env, objectClass)))
                                            Java::Runtime::IllegalArgumentException::throwNew(env, QString("Wrong argument given: %1, expected: boolean[]").arg(QtJambiAPI::getClassName(env, objectClass)) QTJAMBI_STACKTRACEINFO );

                                        if(!out){
                                            std::initializer_list<jboolean>* _out = new std::initializer_list<jboolean>;
                                            out = _out;
                                            scope->addDeletion(_out);
                                        }
                                        jsize arrayLength = env->GetArrayLength(jarray(in.l));
                                        bool* data = new bool[size_t(arrayLength)];
                                        jboolean* _data = new jboolean[size_t(arrayLength)];
                                        env->GetBooleanArrayRegion(jbooleanArray(in.l), 0, arrayLength, _data);
                                        for (jsize i=0; i<arrayLength; ++i) {
                                            data[i] = _data[i];
                                        }
                                        delete[] _data;
                                        scope->addArrayDeletion(data);
#ifdef Q_CC_MSVC
                                        *reinterpret_cast<std::initializer_list<bool>*>(out) = std::initializer_list<bool>(data, data + size_t(arrayLength));
#else
                                        struct bool_initializer_list_access{
                                            bool* _M_array;
                                            size_t _M_len;
                                        };
                                        reinterpret_cast<bool_initializer_list_access*>(out)->_M_array = data;
                                        reinterpret_cast<bool_initializer_list_access*>(out)->_M_len = size_t(arrayLength);
#endif
                                    }
                                }else{
                                    return false;
                                }
                                return true;
                            };
                }else{
                    componentClass = getGlobalClassRef(_env, componentClass);
                    QtJambiUtils::ExternalToInternalConverter externalToInternalComponentConverter = getExternalToInternalConverter(
                                                                    _env,
                                                                    componentClass,
                                                                    _internalTypeName,
                                                                    metaType);
                    return [size = sizeAlign.first, externalToInternalComponentConverter, componentClass](JNIEnv* env, QtJambiScope* scope, jvalue in, void* &out, jValueType) -> bool {
                                if(scope){
                                    if(in.l){
                                        jclass objectClass = env->GetObjectClass(in.l);
                                        if(!Java::Runtime::Class::isArray(env, objectClass) || !env->IsAssignableFrom(Java::Runtime::Class::getComponentType(env, objectClass), componentClass))
                                            Java::Runtime::IllegalArgumentException::throwNew(env, QString("Object of type %1 not allowed. Array expected.").arg(QtJambiAPI::getClassName(env, objectClass)) QTJAMBI_STACKTRACEINFO );
                                        if(!out){
                                            std::initializer_list<char>* _out = new std::initializer_list<char>;
                                            out = _out;
                                            scope->addDeletion(_out);
                                        }
                                        jsize arrayLength = env->GetArrayLength(jarray(in.l));
                                        char* data = new char[size * size_t(arrayLength)];
                                        for (jsize i=0; i<arrayLength; ++i) {
                                            void* ptr = &data[size * size_t(i)];
                                            jvalue val;
                                            val.l = env->GetObjectArrayElement(jobjectArray(in.l), i);
                                            externalToInternalComponentConverter(env, scope, val, ptr, jValueType::l);
                                        }
                                        scope->addArrayDeletion(data);
#ifdef Q_CC_MSVC
                                        *reinterpret_cast<std::initializer_list<char>*>(out) = std::initializer_list<char>(data, data + size * size_t(arrayLength));
#else
                                        struct initializer_list_access{
                                            char* _M_array;
                                            size_t _M_len;
                                        };
                                        reinterpret_cast<initializer_list_access*>(out)->_M_array = data;
                                        reinterpret_cast<initializer_list_access*>(out)->_M_len = size * size_t(arrayLength);
#endif
                                    }
                                }else{
                                    return false;
                                }
                                return true;
                            };
                }
            }
        }
    }else{
        if(const QMetaObject* metaObject = internalMetaType.metaObject()){
            if(metaObject->inherits(&QObject::staticMetaObject)){
                return [metaObject,internalMetaType, externalClass](JNIEnv* env, QtJambiScope* scope, jvalue val, void* &out, jValueType) -> bool{
                    if(Java::QtCore::QMetaType$GenericTypeInterface::isInstanceOf(env,val.l)){
                        QMetaType given = qtjambi_cast<QMetaType>(env, Java::QtCore::QMetaType$GenericTypeInterface::metaType(env, val.l));
                        if(internalMetaType!=given){
                            Java::Runtime::IllegalArgumentException::throwNew(env, QStringLiteral("Wrong argument given: %1, expected: %2").arg(given.name(), internalMetaType.name()) QTJAMBI_STACKTRACEINFO );
                        }
                    }else{
                        if(val.l && !env->IsInstanceOf(val.l, externalClass))
                            Java::Runtime::IllegalArgumentException::throwNew(env, QString("Wrong argument given: %1, expected: %2").arg(QtJambiAPI::getObjectClassName(env, val.l).replace("$", "."), QtJambiAPI::getClassName(env, externalClass).replace("$", ".")) QTJAMBI_STACKTRACEINFO );
                    }
                    if(!val.l){
                        QObject* object = nullptr;
                        if(scope && !out){
                            QObject** optr;
                            out = optr = new QObject*(object);
                            scope->addDeletion(optr);
                        }else if(out)
                            *reinterpret_cast<QObject**>(out) = object;
                        return true;
                    }else if(QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForJavaObject(env, val.l)){
                        QObject* object;
                        if(link->isQObject()){
                            object = link->qobject();
                        }else{
                            object = reinterpret_cast<QObject*>(link->pointer());
                        }
                        if(object && !object->metaObject()->inherits(metaObject)){
                            JavaException::raiseIllegalArgumentException(env, qPrintable(QString("Wrong argument given: %1, expected: %2").arg(object->metaObject()->className(), metaObject->className())) QTJAMBI_STACKTRACEINFO );
                        }
                        object = metaObject->cast(object);
                        if(scope && !out){
                            QObject** optr;
                            out = optr = new QObject*(object);
                            scope->addDeletion(optr);
                        }else if(out)
                            *reinterpret_cast<QObject**>(out) = object;
                        return out;
                    }else{
                        Java::QtJambi::QNoNativeResourcesException::throwNew(env, QStringLiteral("Incomplete object of type: %1").arg(QtJambiAPI::getObjectClassName(env, val.l).replace("$", ".")) QTJAMBI_STACKTRACEINFO );
                        return false;
                    }
                };
            }
        }
        if( (internalTypeName.startsWith("QVector<")
             || internalTypeName.startsWith("QStack<")
             || internalTypeName.startsWith("QList<")
             || internalTypeName.startsWith("QQueue<")
#if QT_VERSION >= QT_VERSION_CHECK(6, 7, 0)
             || internalTypeName.startsWith("QSpan<")
             || internalTypeName.startsWith("std::span<")
#endif
             || internalTypeName.startsWith("QSet<")
             )
                && internalTypeName.endsWith(">")
                && (Java::Runtime::Collection::isAssignableFrom(_env,externalClass)
                    || Java::Runtime::Object::isSameClass(_env,externalClass)
#if QT_VERSION >= QT_VERSION_CHECK(6, 7, 0)
                    || internalTypeName.startsWith("QSpan<")
                    || internalTypeName.startsWith("std::span<")
#endif
                    ) ){
            auto idx = internalTypeName.indexOf("<");
            SequentialContainerType containerType;
            if(internalTypeName.startsWith("QStack<")){
                containerType = SequentialContainerType::QStack;
            }else if(internalTypeName.startsWith("QQueue<")){
                containerType = SequentialContainerType::QQueue;
            }else if(internalTypeName.startsWith("QSet<")){
                containerType = SequentialContainerType::QSet;
#if QT_VERSION >= QT_VERSION_CHECK(6, 7, 0)
            }else if(internalTypeName.startsWith("QSpan<const ") || internalTypeName.startsWith("std::span<const ")){
                containerType = SequentialContainerType::QConstSpan;
            }else if(internalTypeName.startsWith("QSpan<") || internalTypeName.startsWith("std::span<")){
#if !defined(Q_OS_ANDROID)
                containerType = SequentialContainerType::QSpan;
#else
                containerType = SequentialContainerType::QConstSpan;
#endif
#endif
            }else{
                containerType = SequentialContainerType::QList;
            }
            QString container = internalTypeName.mid(0, idx);
            Q_UNUSED(container)
            QString instantiation = internalTypeName.mid(idx+1).chopped(1).trimmed();
            bool isPointer = isPointerType(instantiation);
            QByteArray clazz;
            QPair<size_t,size_t> sizeAlign = {0,0};
            QMetaType memberMetaType(QMetaType::UnknownType);
            const std::type_info* t;
            if(isPointer){
                t = getTypeByQtName(instantiation.chopped(1));
                memberMetaType = QMetaType::fromName(qPrintable(instantiation));
                if(t){
                    clazz = getJavaName(*t);
                }else{
                    if(!memberMetaType.isValid()){
                        clazz = getExternalTypeName(_env, instantiation, memberMetaType).toUtf8();
                    }else{
                        clazz = getExternalTypeName(_env, memberMetaType.name(), memberMetaType).toUtf8();
                    }
                }
            }else{
                t = getTypeByQtName(instantiation);
                if(t){
                    clazz = getJavaName(*t);
                    memberMetaType = registeredMetaType(*t);
                    if(memberMetaType.isValid()){
                        auto sz = memberMetaType.sizeOf();
                        sizeAlign.first = sz<0 ? 0 : size_t(sz);
                        if(memberMetaType.alignOf()!=0)
                            sizeAlign.second = size_t(memberMetaType.alignOf());
                    }else{
                        sizeAlign = getValueSizeAndAlignment(*t);
                    }
                }else{
                    memberMetaType = QMetaType::fromName(qPrintable(instantiation));
                    if(memberMetaType.isValid()){
                        auto sz = memberMetaType.sizeOf();
                        sizeAlign.first = sz<0 ? 0 : size_t(sz);
                        if(memberMetaType.alignOf()!=0)
                            sizeAlign.second = size_t(memberMetaType.alignOf());
                    }
                    if(sizeAlign.first==0){
                        sizeAlign.first = getInternalSize(instantiation);
                    }
                    if(sizeAlign.second==0){
                        sizeAlign.second = getInternalAlignment(instantiation);
                    }
                    if(!memberMetaType.isValid()){
                        clazz = getExternalTypeName(_env, instantiation, memberMetaType).toUtf8();
                    }else{
                        clazz = getExternalTypeName(_env, memberMetaType.name(), memberMetaType).toUtf8();
                    }
                }
            }

            if(!isPointer && !memberMetaType.isValid()){
                memberMetaType = QMetaType::fromName(qPrintable(instantiation));
                if(memberMetaType.isValid()){
                    auto sz = memberMetaType.sizeOf();
                    sizeAlign.first = sz<0 ? 0 : size_t(sz);
                    if(memberMetaType.alignOf()!=0)
                        sizeAlign.second = size_t(memberMetaType.alignOf());
                }
                if(sizeAlign.first==0){
                    sizeAlign.first = getInternalSize(instantiation);
                }
                if(sizeAlign.second==0){
                    sizeAlign.second = getInternalAlignment(instantiation);
                }
            }

            if(!clazz.isEmpty()){
                {
                    AbstractContainerAccess* containerAccess = ContainerAccessAPI::createContainerAccess(containerType, memberMetaType);
                    jclass elementClass = JavaAPI::resolveClass(_env, clazz);
                    typedef bool (*IsContainerFunction)(JNIEnv *, jobject, const QMetaType&, void*& pointer, AbstractContainerAccess*& access);
                    typedef void (*ContainerAppendFunction)(AbstractContainerAccess*, JNIEnv *, const ContainerInfo&, jobject);

                    IsContainerFunction isContainerFunction = nullptr;
                    ContainerAppendFunction containerAppendFunction = nullptr;

                    switch(containerType){
                    case SequentialContainerType::QSet:
                        isContainerFunction = ContainerAPI::getAsQSet;
                        containerAppendFunction = [](AbstractContainerAccess* access, JNIEnv * env, const ContainerInfo& container, jobject value){
                            if(AbstractSetAccess* _access = dynamic_cast<AbstractSetAccess*>(access))
                                _access->insert(env, container, value);
                        };
                        break;
                    case SequentialContainerType::QStack:
                    case SequentialContainerType::QQueue:
                    case SequentialContainerType::QList:
                        isContainerFunction = ContainerAPI::getAsQList;
                        containerAppendFunction = [](AbstractContainerAccess* access, JNIEnv * env, const ContainerInfo& container, jobject value){
                            if(AbstractListAccess* _access = dynamic_cast<AbstractListAccess*>(access))
                                _access->insert(env, container, _access->size(env, container.container), 1, value);
                        };
                        break;
#if QT_VERSION >= QT_VERSION_CHECK(6,7,0)
                    case SequentialContainerType::QConstSpan:
                    case SequentialContainerType::QSpan:
                        isContainerFunction = ContainerAPI::getAsQSpan;
                        break;
#endif //QT_VERSION >= QT_VERSION_CHECK(6,7,0)
                    }

                    if(!containerAccess){
                        QtJambiUtils::InternalToExternalConverter memberConverter = getInternalToExternalConverter(
                                                                        _env,
                                                                        instantiation,
                                                                        memberMetaType,
                                                                        elementClass);
                        QtJambiUtils::ExternalToInternalConverter memberReConverter = getExternalToInternalConverter(
                                                                        _env,
                                                                        elementClass,
                                                                        instantiation, memberMetaType);
                        PtrOwnerFunction memberOwnerFunction = nullptr;
                        if(!isPointer){
                            const std::type_info* typeId = getTypeByQtName(qPrintable(instantiation));
                            if(!typeId){
                                typeId = getTypeByMetaType(memberMetaType);
                            }
                            if(typeId)
                                memberOwnerFunction = ContainerAPI::registeredOwnerFunction(*typeId);
                            if(!memberMetaType.isValid()){
                                memberMetaType = QMetaType::fromName(qPrintable(instantiation));
                                sizeAlign.first = memberMetaType.sizeOf();
                                if(memberMetaType.alignOf()!=0)
                                    sizeAlign.second = size_t(memberMetaType.alignOf());
                                containerAccess = ContainerAccessAPI::createContainerAccess(containerType, memberMetaType);
                            }
                        }else{
                            if(!memberMetaType.isValid()){
                                const std::type_info* typeId = nullptr;
                                {
                                    QString _internalTypeName = instantiation;
                                    if(_internalTypeName.startsWith("const "))
                                        _internalTypeName = _internalTypeName.mid(6);
                                    if(_internalTypeName.endsWith("*") || _internalTypeName.endsWith("&"))
                                        _internalTypeName.chop(1);
                                    typeId = getTypeByQtName(_internalTypeName);
                                }
                                if(!typeId){
                                    typeId = getTypeByJavaName(QtJambiAPI::getClassName(_env, externalClass).replace(".", "/"));
                                }
                                const QMetaObject *metaObject = nullptr;
                                if(typeId)
                                    metaObject = registeredOriginalMetaObject(*typeId);
                                QMetaType::TypeFlags flags;
#if QT_VERSION < QT_VERSION_CHECK(6, 9, 0)
                                if(Java::QtCore::QObject::isAssignableFrom(_env, externalClass)){
                                    flags = QMetaType::TypeFlags(QtPrivate::QMetaTypeTypeFlags<QObject*>::Flags);
                                }else{
                                    flags = QMetaType::TypeFlags(QtPrivate::QMetaTypeTypeFlags<void*>::Flags);
#else
                                if(Java::QtCore::QObject::isAssignableFrom(_env, externalClass)){
                                    flags = QMetaType::TypeFlags(QtPrivate::QMetaTypeForType<QObject*>::flags());
                                }else{
                                    flags = QMetaType::TypeFlags(QtPrivate::QMetaTypeForType<void*>::flags());
#endif
                                    if(metaObject)
                                        flags.setFlag(QMetaType::PointerToGadget);
                                }
                                memberMetaType = registerMetaTypeImpl(nullptr,
                                                                                    nullptr,
                                                                                    instantiation.toUtf8(),
                                                                                  /*.defaultCtr=*/ QtPrivate::QMetaTypeForType<void*>::getDefaultCtr(),
                                                                                  /*.copyCtr=*/ QtPrivate::QMetaTypeForType<void*>::getCopyCtr(),
                                                                                  /*.moveCtr=*/ QtPrivate::QMetaTypeForType<void*>::getMoveCtr(),
                                                                                  /*.dtor=*/ QtPrivate::QMetaTypeForType<void*>::getDtor(),
                                                                                  /*.equals=*/ QtPrivate::QEqualityOperatorForType<void*>::equals,
                                                                                  /*.lessThan=*/ QtPrivate::QLessThanOperatorForType<void*>::lessThan,
                                                                                  /*.debugStream=*/ QtPrivate::QDebugStreamOperatorForType<void*>::debugStream,
                                                                                  /*.dataStreamOut=*/ QtPrivate::QDataStreamOperatorForType<void*>::dataStreamOut,
                                                                                  /*.dataStreamIn=*/ QtPrivate::QDataStreamOperatorForType<void*>::dataStreamIn,
                                                                                  /*.legacyRegisterOp=*/ QtPrivate::QMetaTypeForType<void*>::getLegacyRegister(),
                                                                                     uint(sizeAlign.first = sizeof(void*)),
                                                                                     ushort(sizeAlign.second = alignof(void*)),
                                                                                     0,
                                                                                     flags,
                                                                                     metaObject,
                                                                                     nullptr,
                                                                                     nullptr,
                                                                                    {});
                            }
                        }
                        if(!containerAccess && memberMetaType.isValid()){
                            QtJambiUtils::QHashFunction hashFunction = QtJambiTypeManager::findHashFunction(isPointer, memberMetaType);
                            QSharedPointer<AbstractContainerAccess> memberNestedContainerAccess = findContainerAccess(memberMetaType);
                            containerAccess = ContainerAccessAPI::createContainerAccess(_env, containerType,
                                                                              memberMetaType,
                                                                              sizeAlign.second, sizeAlign.first,
                                                                              isPointer,
                                                                              hashFunction,
                                                                              memberConverter,
                                                                              memberReConverter,
                                                                              memberNestedContainerAccess,
                                                                              memberOwnerFunction);
                        }
                    }
                    if(containerAccess){
                        containerAccess->registerContainer(internalTypeName.toLatin1());
#if defined(QTJAMBI_GENERIC_ACCESS)
                        switch(containerType){
                        case SequentialContainerType::QStack:
                        case SequentialContainerType::QQueue:
                        case SequentialContainerType::QList:
                            containerAccess = checkContainerAccess(_env, dynamic_cast<AbstractListAccess*>(containerAccess));
                            break;
                        case SequentialContainerType::QSet:
                            containerAccess = checkContainerAccess(_env, dynamic_cast<AbstractSetAccess*>(containerAccess));
                            break;
                        case SequentialContainerType::QConstSpan:
                        case SequentialContainerType::QSpan:
                            containerAccess = dynamic_cast<AbstractSpanAccess*>(containerAccess);
                            break;
                        }
#endif //defined(QTJAMBI_GENERIC_ACCESS)
#if QT_VERSION >= QT_VERSION_CHECK(6,7,0)
                        switch(containerType){
                        case SequentialContainerType::QConstSpan:
                        case SequentialContainerType::QSpan:
                            return [memberMetaType, containerType](JNIEnv* env, QtJambiScope* scope, jvalue in, void* &out, jValueType valueType) -> bool{
                                bool response = false;
                                if(valueType==jValueType::l){
                                    if(scope && !out){
                                        QtJambiSpan* ptr;
                                        out = ptr = new QtJambiSpan;
                                        scope->addDeletion(ptr);
                                    }
                                    if(!out)
                                        return false;
                                    QtJambiSpan* ptr = reinterpret_cast<QtJambiSpan*>(out);
                                    if(QtJambiAPI::isQSpanObject(env, in.l)){
                                        QPair<void*,jlong> data = QtJambiAPI::fromQSpanObject(env, in.l, containerType==SequentialContainerType::QConstSpan, memberMetaType);
                                        if(scope)
                                            scope->addFinalAction([env, in](){QtJambiAPI::commitQSpanObject(env, in.l);});
                                        if(data.first && data.second>0){
                                            ptr->begin = data.first;
                                            ptr->size = data.second;
                                        }else{
                                            ptr->begin = nullptr;
                                            ptr->size = 0;
                                        }
                                    }else{
                                        ptr->begin = nullptr;
                                        ptr->size = 0;
                                    }
                                    response = true;
                                }
                                return response;
                            };
                            break;
                        default:
#endif //QT_VERSION >= QT_VERSION_CHECK(6,7,0)
                        {
                            if(Java::Runtime::Map::isAssignableFrom(_env, elementClass)){
                                elementClass = getGlobalClassRef(_env, Java::Runtime::Map::getClass(_env));
                            }else if(Java::Runtime::Collection::isAssignableFrom(_env, elementClass)){
                                elementClass = getGlobalClassRef(_env, Java::Runtime::Collection::getClass(_env));
                            }else if(Java::Runtime::Byte::isPrimitiveType(_env, elementClass)){
                                elementClass = getGlobalClassRef(_env, Java::Runtime::Byte::getClass(_env));
                            }else if(Java::Runtime::Short::isPrimitiveType(_env, elementClass)){
                                elementClass = getGlobalClassRef(_env, Java::Runtime::Short::getClass(_env));
                            }else if(Java::Runtime::Integer::isPrimitiveType(_env, elementClass)){
                                elementClass = getGlobalClassRef(_env, Java::Runtime::Integer::getClass(_env));
                            }else if(Java::Runtime::Long::isPrimitiveType(_env, elementClass)){
                                elementClass = getGlobalClassRef(_env, Java::Runtime::Long::getClass(_env));
                            }else if(Java::Runtime::Character::isPrimitiveType(_env, elementClass)){
                                elementClass = getGlobalClassRef(_env, Java::Runtime::Character::getClass(_env));
                            }else if(Java::Runtime::Boolean::isPrimitiveType(_env, elementClass)){
                                elementClass = getGlobalClassRef(_env, Java::Runtime::Boolean::getClass(_env));
                            }else if(Java::Runtime::Float::isPrimitiveType(_env, elementClass)){
                                elementClass = getGlobalClassRef(_env, Java::Runtime::Float::getClass(_env));
                            }else if(Java::Runtime::Double::isPrimitiveType(_env, elementClass)){
                                elementClass = getGlobalClassRef(_env, Java::Runtime::Double::getClass(_env));
                            }
                            QSharedPointer<AbstractContainerAccess> sharedAccess(containerAccess, &containerDisposer);
                            return [sharedAccess,
                                    containerAppendFunction,
                                    elementClass,
                                    memberMetaType,
                                    isContainerFunction](JNIEnv* env, QtJambiScope* scope, jvalue in, void* &out, jValueType valueType) -> bool{
                                bool response = false;
                                if(valueType==jValueType::l){
                                    if(in.l && !env->IsInstanceOf(in.l, Java::Runtime::Collection::getClass(env)))
                                        Java::Runtime::IllegalArgumentException::throwNew(env, QString("Wrong argument given: %1, expected: %2").arg(QtJambiAPI::getObjectClassName(env, in.l).replace("$", "."), QtJambiAPI::getClassName(env, Java::Runtime::Collection::getClass(env)).replace("$", ".")) QTJAMBI_STACKTRACEINFO );
                                    bool deleteAccess = true;
                                    Q_ASSERT(env->GetObjectRefType(elementClass)==JNIGlobalRefType);
                                    AbstractContainerAccess* access = sharedAccess->clone();
                                    void* pointer{nullptr};
                                    AbstractContainerAccess* otherAccess{nullptr};
                                    if(!in.l){
                                        if(!out){
                                            if(scope){
                                                out = access->createContainer();
                                                scope->addFinalAction([access,out](){
                                                    access->deleteContainer(out);
                                                    access->dispose();
                                                });
                                                deleteAccess = false;
                                            }
                                        }
                                        response = true;
                                    }else if(isContainerFunction(env, in.l, memberMetaType, pointer, otherAccess)){
                                        if(out){
                                            access->assign(out, pointer);
                                        }else if(scope){
                                            out = access->createContainer(env, ConstContainerAndAccessInfo{in.l, pointer, otherAccess});
                                            scope->addFinalAction([access,out](){
                                                access->deleteContainer(out);
                                                access->dispose();
                                            });
                                            deleteAccess = false;
                                        }else{
                                            out = pointer;
                                        }
                                        response = true;
                                    }else{
                                        if(!out){
                                            if(scope){
                                                out = access->createContainer();
                                                scope->addFinalAction([access,out](){
                                                    access->deleteContainer(out);
                                                    access->dispose();
                                                });
                                                deleteAccess = false;
                                            }
                                        }
                                        if(out){
                                            QList<jobject> content;
                                            jobject iterator = QtJambiAPI::iteratorOfJavaIterable(env, in.l);
                                            while(QtJambiAPI::hasJavaIteratorNext(env, iterator)) {
                                                jobject element = QtJambiAPI::nextOfJavaIterator(env, iterator);
                                                if(element && !env->IsInstanceOf(element, elementClass))
                                                    Java::Runtime::IllegalArgumentException::throwNew(env, QString("Wrong collection content given: %1, expected: %2").arg(QtJambiAPI::getObjectClassName(env, element).replace("$", "."), QtJambiAPI::getClassName(env, elementClass).replace("$", ".")) QTJAMBI_STACKTRACEINFO );
                                                content << element;
                                            }
                                            for(const QSharedPointer<QtJambiLink>& link : QtJambiLink::findLinksForPointer(out)){
                                                if(auto _access = link->containerAccess()){
                                                    response = true;
                                                    jobject obj = link->getJavaObjectLocalRef(env);
                                                    for(jobject element : std::as_const(content)) {
                                                        containerAppendFunction(_access, env, {obj, out}, element);
                                                    }
                                                    break;
                                                }
                                            }
                                            if(!response){
                                                response = true;
                                                for(jobject element : std::as_const(content)) {
                                                    containerAppendFunction(access, env, {nullptr, out}, element);
                                                }
                                            }
                                        }
                                    }
                                    if(deleteAccess)
                                        access->dispose();
                                }
                                return response;
                            };
                        }
#if QT_VERSION >= QT_VERSION_CHECK(6,7,0)
                        }
#endif //QT_VERSION >= QT_VERSION_CHECK(6,7,0)
                    }
                }
            }
            return [](JNIEnv*, QtJambiScope*, jvalue, void* &, jValueType) -> bool{
                return false;
            };
        }else if( ((internalTypeName.startsWith("QMap<")
                   || internalTypeName.startsWith("QMultiMap<")
                   || internalTypeName.startsWith("QHash<")
                   || internalTypeName.startsWith("QMultiHash<"))
                      && internalTypeName.endsWith(">")
                      && (Java::Runtime::Map::isAssignableFrom(_env,externalClass) || Java::Runtime::Object::isSameClass(_env,externalClass)))
                  || ((internalTypeName.startsWith("QPair<") || internalTypeName.startsWith("std::pair<"))
                      && internalTypeName.endsWith(">")
                      && (Java::QtCore::QPair::isAssignableFrom(_env,externalClass)
                          || Java::Runtime::Object::isSameClass(_env,externalClass)
            ))){
            auto idx = internalTypeName.indexOf("<");
            AssociativeContainerType mapType;
            if(internalTypeName.startsWith("QMap<")){
                 mapType = AssociativeContainerType::QMap;
            }else if(internalTypeName.startsWith("QMultiMap<")){
                 mapType = AssociativeContainerType::QMultiMap;
            }else if(internalTypeName.startsWith("QMultiHash<")){
                 mapType = AssociativeContainerType::QMultiHash;
            }else if(internalTypeName.startsWith("QPair<") || internalTypeName.startsWith("std::pair<")){
                 mapType = AssociativeContainerType::QPair;
            }else{
                 mapType = AssociativeContainerType::QHash;
            }
            QString container = internalTypeName.mid(0, idx);
            Q_UNUSED(container)
            QStringList instantiations = internalTypeName.mid(idx+1).chopped(1).trimmed().split(",");
            reduceMapInstantiations(instantiations);
            if(instantiations.size()==2){
                bool isPointer1 = isPointerType(instantiations[0]);
                QByteArray clazz1;
                QPair<size_t,size_t> sizeAlign1 = {0,0};
                QMetaType memberMetaType1(QMetaType::UnknownType);
                const std::type_info* t1(nullptr);
                if(isPointer1){
                    t1 = getTypeByQtName(instantiations[0].chopped(1));
                    memberMetaType1 = QMetaType::fromName(qPrintable(instantiations[0]));
                    if(t1){
                        clazz1 = getJavaName(*t1);
                    }else{
                        if(!memberMetaType1.isValid()){
                            clazz1 = getExternalTypeName(_env, instantiations[0], memberMetaType1).toUtf8();
                        }else{
                            clazz1 = getExternalTypeName(_env, memberMetaType1.name(), memberMetaType1).toUtf8();
                        }
                    }
                }else{
                    t1 = getTypeByQtName(instantiations[0]);
                    if(t1){
                        clazz1 = getJavaName(*t1);
                        memberMetaType1 = registeredMetaType(*t1);
                        if(memberMetaType1.isValid()){
                            auto sz = memberMetaType1.sizeOf();
                            sizeAlign1.first = sz<0 ? 0 : size_t(sz);
                            if(memberMetaType1.alignOf()!=0)
                                sizeAlign1.second = size_t(memberMetaType1.alignOf());
                        }else{
                            sizeAlign1 = getValueSizeAndAlignment(*t1);
                        }
                    }
                    if(!memberMetaType1.isValid()){
                        memberMetaType1 = QMetaType::fromName(qPrintable(instantiations[0]));
                        if(memberMetaType1.isValid()){
                            auto sz = memberMetaType1.sizeOf();
                            sizeAlign1.first = sz<0 ? 0 : size_t(sz);
                            if(memberMetaType1.alignOf()!=0)
                                sizeAlign1.second = size_t(memberMetaType1.alignOf());
                        }
                        if(sizeAlign1.first==0){
                            sizeAlign1.first = getInternalSize(instantiations[0]);
                        }
                        if(sizeAlign1.second==0){
                            sizeAlign1.second = getInternalAlignment(instantiations[0]);
                        }
                        if(!memberMetaType1.isValid()){
                            clazz1 = getExternalTypeName(_env, instantiations[0], memberMetaType1).toUtf8();
                        }else{
                            clazz1 = getExternalTypeName(_env, memberMetaType1.name(), memberMetaType1).toUtf8();
                        }
                    }
                }
                const std::type_info* t2;
                bool isPointer2 = isPointerType(instantiations[1]);
                if(isPointer2){
                    t2 = getTypeByQtName(instantiations[1].chopped(1));
                }else{
                    t2 = getTypeByQtName(instantiations[1]);
                }
                QByteArray clazz2;
                QPair<size_t,size_t> sizeAlign2 = {0,0};
                QMetaType memberMetaType2(QMetaType::UnknownType);
                if(isPointer2){
                    memberMetaType2 = QMetaType(QMetaType::type(qPrintable(instantiations[1])));
                    if(t2){
                        clazz2 = getJavaName(*t2);
                    }else{
                        if(!memberMetaType2.isValid()){
                            clazz2 = getExternalTypeName(_env, instantiations[1], memberMetaType2).toUtf8();
                        }else{
                            clazz2 = getExternalTypeName(_env, memberMetaType2.name(), memberMetaType2).toUtf8();
                        }
                    }
                }else{
                    if(t2){
                        clazz2 = getJavaName(*t2);
                        memberMetaType2 = registeredMetaType(*t2);
                        if(memberMetaType2.isValid()){
                            auto sz = memberMetaType2.sizeOf();
                            sizeAlign2.first = sz<0 ? 0 : size_t(sz);
                            if(memberMetaType2.alignOf()!=0)
                                sizeAlign2.second = size_t(memberMetaType2.alignOf());
                        }else{
                            sizeAlign2 = getValueSizeAndAlignment(*t2);
                        }
                    }else{
                        memberMetaType2 = QMetaType::fromName(qPrintable(instantiations[1]));
                        if(memberMetaType2.isValid()){
                            auto sz = memberMetaType2.sizeOf();
                            sizeAlign2.first = sz<0 ? 0 : size_t(sz);
                            if(memberMetaType2.alignOf()!=0)
                                sizeAlign2.second = size_t(memberMetaType2.alignOf());
                        }
                        if(sizeAlign2.first==0){
                            sizeAlign2.first = getInternalSize(instantiations[1]);
                        }
                        if(sizeAlign2.second==0){
                            sizeAlign2.second = getInternalAlignment(instantiations[1]);
                        }
                        if(!memberMetaType2.isValid()){
                            clazz2 = getExternalTypeName(_env, instantiations[1], memberMetaType2).toUtf8();
                        }else{
                            clazz2 = getExternalTypeName(_env, memberMetaType2.name(), memberMetaType2).toUtf8();
                        }
                    }
                }
                if(!clazz1.isEmpty() && !clazz2.isEmpty()){
                    jclass keyClass = JavaAPI::resolveClass(_env, clazz1);
                    if(!isPointer1 && !memberMetaType1.isValid()){
                        memberMetaType1 = QMetaType::fromName(qPrintable(instantiations[0]));
                        if(memberMetaType1.isValid()){
                            auto sz = memberMetaType1.sizeOf();
                            sizeAlign1.first = sz<0 ? 0 : size_t(sz);
                            if(memberMetaType1.alignOf()!=0)
                                sizeAlign1.second = size_t(memberMetaType1.alignOf());
                        }
                        if(sizeAlign1.first==0){
                            sizeAlign1.first = getInternalSize(instantiations[0]);
                        }
                        if(sizeAlign1.second==0){
                            sizeAlign1.second = getInternalAlignment(instantiations[0]);
                        }
                    }
                    jclass valueClass = JavaAPI::resolveClass(_env, clazz2);
                    if(!isPointer2 && !memberMetaType2.isValid()){
                        memberMetaType2 = QMetaType::fromName(qPrintable(instantiations[1]));
                        if(memberMetaType2.isValid()){
                            auto sz = memberMetaType2.sizeOf();
                            sizeAlign2.first = sz<0 ? 0 : size_t(sz);
                            if(memberMetaType2.alignOf()!=0)
                                sizeAlign2.second = size_t(memberMetaType2.alignOf());
                        }
                        if(sizeAlign2.first==0){
                            sizeAlign2.first = getInternalSize(instantiations[1]);
                        }
                        if(sizeAlign2.second==0){
                            sizeAlign2.second = getInternalAlignment(instantiations[1]);
                        }
                    }
                    AbstractContainerAccess* containerAccess = ContainerAccessAPI::createContainerAccess(mapType, memberMetaType1, memberMetaType2);
                    if(!containerAccess){
                        QtJambiUtils::InternalToExternalConverter memberConverter1 = getInternalToExternalConverter(
                                                                        _env,
                                                                        instantiations[0],
                                                                        memberMetaType1,
                                                                        keyClass);
                        QtJambiUtils::ExternalToInternalConverter memberReConverter1 = getExternalToInternalConverter(
                                                                        _env,
                                                                        keyClass,
                                                                        instantiations[0], memberMetaType1);
                        QtJambiUtils::InternalToExternalConverter memberConverter2 = getInternalToExternalConverter(
                                                                        _env,
                                                                        instantiations[1],
                                                                        memberMetaType2,
                                                                        valueClass);
                        QtJambiUtils::ExternalToInternalConverter memberReConverter2 = getExternalToInternalConverter(
                                                                        _env,
                                                                        valueClass,
                                                                        instantiations[1], memberMetaType2);
                        bool tryAgain = false;
                        if(!memberMetaType1.isValid()){
                            const std::type_info* typeId = nullptr;
                            {
                                QString _internalTypeName = instantiations[0];
                                if(_internalTypeName.startsWith("const "))
                                    _internalTypeName = _internalTypeName.mid(6);
                                if(_internalTypeName.endsWith("*") || _internalTypeName.endsWith("&"))
                                    _internalTypeName.chop(1);
                                typeId = getTypeByQtName(_internalTypeName);
                            }
                            if(!typeId){
                                typeId = getTypeByJavaName(QtJambiAPI::getClassName(_env, externalClass).replace(".", "/"));
                            }
                            const QMetaObject *metaObject = nullptr;
                            if(typeId)
                                metaObject = registeredOriginalMetaObject(*typeId);
                            QMetaType::TypeFlags flags;
#if QT_VERSION < QT_VERSION_CHECK(6, 9, 0)
                            if(Java::QtCore::QObject::isAssignableFrom(_env, externalClass)){
                                flags = QMetaType::TypeFlags(QtPrivate::QMetaTypeTypeFlags<QObject*>::Flags);
                            }else{
                                flags = QMetaType::TypeFlags(QtPrivate::QMetaTypeTypeFlags<void*>::Flags);
#else
                            if(Java::QtCore::QObject::isAssignableFrom(_env, externalClass)){
                                flags = QMetaType::TypeFlags(QtPrivate::QMetaTypeForType<QObject*>::flags());
                            }else{
                                flags = QMetaType::TypeFlags(QtPrivate::QMetaTypeForType<void*>::flags());
#endif
                                if(metaObject)
                                    flags.setFlag(QMetaType::PointerToGadget);
                            }
                            memberMetaType1 = registerMetaTypeImpl(nullptr,
                                                                                 nullptr,
                                                                                 instantiations[0].toUtf8(),
                                                                              /*.defaultCtr=*/ QtPrivate::QMetaTypeForType<void*>::getDefaultCtr(),
                                                                              /*.copyCtr=*/ QtPrivate::QMetaTypeForType<void*>::getCopyCtr(),
                                                                              /*.moveCtr=*/ QtPrivate::QMetaTypeForType<void*>::getMoveCtr(),
                                                                              /*.dtor=*/ QtPrivate::QMetaTypeForType<void*>::getDtor(),
                                                                              /*.equals=*/ QtPrivate::QEqualityOperatorForType<void*>::equals,
                                                                              /*.lessThan=*/ QtPrivate::QLessThanOperatorForType<void*>::lessThan,
                                                                              /*.debugStream=*/ QtPrivate::QDebugStreamOperatorForType<void*>::debugStream,
                                                                              /*.dataStreamOut=*/ QtPrivate::QDataStreamOperatorForType<void*>::dataStreamOut,
                                                                              /*.dataStreamIn=*/ QtPrivate::QDataStreamOperatorForType<void*>::dataStreamIn,
                                                                              /*.legacyRegisterOp=*/ QtPrivate::QMetaTypeForType<void*>::getLegacyRegister(),
                                                                                 uint(sizeAlign1.first = sizeof(void*)),
                                                                                 ushort(sizeAlign1.second = alignof(void*)),
                                                                                 0,
                                                                                 flags,
                                                                                 metaObject,
                                                                                 nullptr,
                                                                                 nullptr,
                                                                                {});
                            tryAgain = true;
                        }
                        if(!memberMetaType2.isValid()){
                            const std::type_info* typeId = nullptr;
                            {
                                QString _internalTypeName = instantiations[1];
                                if(_internalTypeName.startsWith("const "))
                                    _internalTypeName = _internalTypeName.mid(6);
                                if(_internalTypeName.endsWith("*") || _internalTypeName.endsWith("&"))
                                    _internalTypeName.chop(1);
                                typeId = getTypeByQtName(_internalTypeName);
                            }
                            if(!typeId){
                                typeId = getTypeByJavaName(QtJambiAPI::getClassName(_env, externalClass).replace(".", "/"));
                            }
                            const QMetaObject *metaObject = nullptr;
                            if(typeId)
                                metaObject = registeredOriginalMetaObject(*typeId);
                            QMetaType::TypeFlags flags;
#if QT_VERSION < QT_VERSION_CHECK(6, 9, 0)
                            if(Java::QtCore::QObject::isAssignableFrom(_env, externalClass)){
                                flags = QMetaType::TypeFlags(QtPrivate::QMetaTypeTypeFlags<QObject*>::Flags);
                            }else{
                                flags = QMetaType::TypeFlags(QtPrivate::QMetaTypeTypeFlags<void*>::Flags);
#else
                            if(Java::QtCore::QObject::isAssignableFrom(_env, externalClass)){
                                flags = QMetaType::TypeFlags(QtPrivate::QMetaTypeForType<QObject*>::flags());
                            }else{
                                flags = QMetaType::TypeFlags(QtPrivate::QMetaTypeForType<void*>::flags());
#endif
                                if(metaObject)
                                    flags.setFlag(QMetaType::PointerToGadget);
                            }
                            memberMetaType2 = registerMetaTypeImpl(nullptr,
                                                                                 nullptr,
                                                                                 instantiations[1].toUtf8(),
                                                                              /*.defaultCtr=*/ QtPrivate::QMetaTypeForType<void*>::getDefaultCtr(),
                                                                              /*.copyCtr=*/ QtPrivate::QMetaTypeForType<void*>::getCopyCtr(),
                                                                              /*.moveCtr=*/ QtPrivate::QMetaTypeForType<void*>::getMoveCtr(),
                                                                              /*.dtor=*/ QtPrivate::QMetaTypeForType<void*>::getDtor(),
                                                                              /*.equals=*/ QtPrivate::QEqualityOperatorForType<void*>::equals,
                                                                              /*.lessThan=*/ QtPrivate::QLessThanOperatorForType<void*>::lessThan,
                                                                              /*.debugStream=*/ QtPrivate::QDebugStreamOperatorForType<void*>::debugStream,
                                                                              /*.dataStreamOut=*/ QtPrivate::QDataStreamOperatorForType<void*>::dataStreamOut,
                                                                              /*.dataStreamIn=*/ QtPrivate::QDataStreamOperatorForType<void*>::dataStreamIn,
                                                                              /*.legacyRegisterOp=*/ QtPrivate::QMetaTypeForType<void*>::getLegacyRegister(),
                                                                                 uint(sizeAlign2.first = sizeof(void*)),
                                                                                 ushort(sizeAlign2.second = alignof(void*)),
                                                                                 0,
                                                                                 flags,
                                                                                 metaObject,
                                                                                 nullptr,
                                                                                 nullptr,
                                                                                {});
                            tryAgain = true;
                        }
                        if(tryAgain)
                            containerAccess = ContainerAccessAPI::createContainerAccess(mapType, memberMetaType1, memberMetaType2);
                        if(!containerAccess && memberMetaType1.isValid() && memberMetaType2.isValid()){
                            QtJambiUtils::QHashFunction hashFunction1 = findHashFunction(isPointer1, memberMetaType1);
                            QtJambiUtils::QHashFunction hashFunction2 = findHashFunction(isPointer2, memberMetaType2);
                            QSharedPointer<AbstractContainerAccess> memberNestedContainerAccess1 = findContainerAccess(memberMetaType1);
                            QSharedPointer<AbstractContainerAccess> memberNestedContainerAccess2 = findContainerAccess(memberMetaType2);
                            const std::type_info* typeId = getTypeByQtName(qPrintable(instantiations[0]));
                            if(!typeId){
                                typeId = getTypeByMetaType(memberMetaType1);
                            }
                            PtrOwnerFunction memberOwnerFunction1 = nullptr;
                            if(typeId)
                                memberOwnerFunction1 = ContainerAPI::registeredOwnerFunction(*typeId);
                            typeId = getTypeByQtName(qPrintable(instantiations[1]));
                            if(!typeId){
                                typeId = getTypeByMetaType(memberMetaType2);
                            }
                            PtrOwnerFunction memberOwnerFunction2 = nullptr;
                            if(typeId)
                                memberOwnerFunction2 = ContainerAPI::registeredOwnerFunction(*typeId);
                            containerAccess = ContainerAccessAPI::createContainerAccess(_env, mapType,
                                                                              memberMetaType1,
                                                                              sizeAlign1.second, sizeAlign1.first,
                                                                              isPointer1,
                                                                              hashFunction1,
                                                                              memberConverter1,
                                                                              memberReConverter1,
                                                                              memberNestedContainerAccess1,
                                                                              memberOwnerFunction1,
                                                                              memberMetaType2,
                                                                              sizeAlign2.second, sizeAlign2.first,
                                                                              isPointer2,
                                                                              hashFunction2,
                                                                              memberConverter2,
                                                                              memberReConverter2,
                                                                              memberNestedContainerAccess2,
                                                                              memberOwnerFunction2);
                        }
                    }
                    if(containerAccess){
                        QMetaType containerMetaType = containerAccess->registerContainer(internalTypeName.toLatin1());
                        Q_UNUSED(containerMetaType)
#if defined(QTJAMBI_GENERIC_ACCESS)
                        switch(mapType){
                        case AssociativeContainerType::QHash:
                            containerAccess = checkContainerAccess(_env, dynamic_cast<AbstractHashAccess*>(containerAccess));
                            break;
                        case AssociativeContainerType::QMap:
                            containerAccess = checkContainerAccess(_env, dynamic_cast<AbstractMapAccess*>(containerAccess));
                            break;
                        case AssociativeContainerType::QMultiMap:
                            containerAccess = checkContainerAccess(_env, dynamic_cast<AbstractMultiMapAccess*>(containerAccess));
                            break;
                        case AssociativeContainerType::QMultiHash:
                            containerAccess = checkContainerAccess(_env, dynamic_cast<AbstractMultiHashAccess*>(containerAccess));
                            break;
                        default:
                            break;
                        }
#endif //defined(QTJAMBI_GENERIC_ACCESS)
                        QSharedPointer<AbstractContainerAccess> sharedAccess(containerAccess, &containerDisposer);

                        if(Java::Runtime::Map::isAssignableFrom(_env, keyClass)){
                            keyClass = getGlobalClassRef(_env, Java::Runtime::Map::getClass(_env));
                        }else if(Java::Runtime::Collection::isAssignableFrom(_env, keyClass)){
                            keyClass = getGlobalClassRef(_env, Java::Runtime::Collection::getClass(_env));
                        }else if(Java::Runtime::Byte::isPrimitiveType(_env, keyClass)){
                            keyClass = getGlobalClassRef(_env, Java::Runtime::Byte::getClass(_env));
                        }else if(Java::Runtime::Short::isPrimitiveType(_env, keyClass)){
                            keyClass = getGlobalClassRef(_env, Java::Runtime::Short::getClass(_env));
                        }else if(Java::Runtime::Integer::isPrimitiveType(_env, keyClass)){
                            keyClass = getGlobalClassRef(_env, Java::Runtime::Integer::getClass(_env));
                        }else if(Java::Runtime::Long::isPrimitiveType(_env, keyClass)){
                            keyClass = getGlobalClassRef(_env, Java::Runtime::Long::getClass(_env));
                        }else if(Java::Runtime::Character::isPrimitiveType(_env, keyClass)){
                            keyClass = getGlobalClassRef(_env, Java::Runtime::Character::getClass(_env));
                        }else if(Java::Runtime::Boolean::isPrimitiveType(_env, keyClass)){
                            keyClass = getGlobalClassRef(_env, Java::Runtime::Boolean::getClass(_env));
                        }else if(Java::Runtime::Float::isPrimitiveType(_env, keyClass)){
                            keyClass = getGlobalClassRef(_env, Java::Runtime::Float::getClass(_env));
                        }else if(Java::Runtime::Double::isPrimitiveType(_env, keyClass)){
                            keyClass = getGlobalClassRef(_env, Java::Runtime::Double::getClass(_env));
                        }
                        if(Java::Runtime::Map::isAssignableFrom(_env, valueClass)){
                            valueClass = getGlobalClassRef(_env, Java::Runtime::Map::getClass(_env));
                        }else if(Java::Runtime::Collection::isAssignableFrom(_env, valueClass)){
                            valueClass = getGlobalClassRef(_env, Java::Runtime::Collection::getClass(_env));
                        }else if(Java::Runtime::Byte::isPrimitiveType(_env, valueClass)){
                            valueClass = getGlobalClassRef(_env, Java::Runtime::Byte::getClass(_env));
                        }else if(Java::Runtime::Short::isPrimitiveType(_env, valueClass)){
                            valueClass = getGlobalClassRef(_env, Java::Runtime::Short::getClass(_env));
                        }else if(Java::Runtime::Integer::isPrimitiveType(_env, valueClass)){
                            valueClass = getGlobalClassRef(_env, Java::Runtime::Integer::getClass(_env));
                        }else if(Java::Runtime::Long::isPrimitiveType(_env, valueClass)){
                            valueClass = getGlobalClassRef(_env, Java::Runtime::Long::getClass(_env));
                        }else if(Java::Runtime::Character::isPrimitiveType(_env, valueClass)){
                            valueClass = getGlobalClassRef(_env, Java::Runtime::Character::getClass(_env));
                        }else if(Java::Runtime::Boolean::isPrimitiveType(_env, valueClass)){
                            valueClass = getGlobalClassRef(_env, Java::Runtime::Boolean::getClass(_env));
                        }else if(Java::Runtime::Float::isPrimitiveType(_env, valueClass)){
                            valueClass = getGlobalClassRef(_env, Java::Runtime::Float::getClass(_env));
                        }else if(Java::Runtime::Double::isPrimitiveType(_env, valueClass)){
                            valueClass = getGlobalClassRef(_env, Java::Runtime::Double::getClass(_env));
                        }

                        if(mapType==AssociativeContainerType::QPair){
                            QSharedPointer<AbstractPairAccess> pairAccess(sharedAccess.dynamicCast<AbstractPairAccess>());
                            return [pairAccess,keyClass,valueClass](JNIEnv* env, QtJambiScope* scope, jvalue in, void* &out, jValueType valueType) -> bool{
                                bool response = false;
                                if(valueType==jValueType::l){
                                    jobject first = nullptr;
                                    jobject second = nullptr;
                                    if(in.l){
                                        if(!env->IsInstanceOf(in.l, Java::QtCore::QPair::getClass(env)))
                                            Java::Runtime::IllegalArgumentException::throwNew(env, QString("Wrong argument given: %1, expected: QPair<%2,%3>").arg(QtJambiAPI::getObjectClassName(env, in.l).replace("$", "."), QtJambiAPI::getClassName(env, keyClass).replace("$", "."), QtJambiAPI::getClassName(env, valueClass).replace("$", ".")) QTJAMBI_STACKTRACEINFO );
                                        first = Java::QtCore::QPair::first(env, in.l);
                                        second = Java::QtCore::QPair::second(env, in.l);
                                        if(first && !env->IsInstanceOf(first, keyClass))
                                            Java::Runtime::IllegalArgumentException::throwNew(env, QString("Wrong QPair.first given: %1, expected: %2").arg(QtJambiAPI::getObjectClassName(env, first).replace("$", "."), QtJambiAPI::getClassName(env, keyClass).replace("$", ".")) QTJAMBI_STACKTRACEINFO );
                                        if(second && !env->IsInstanceOf(second, valueClass))
                                            Java::Runtime::IllegalArgumentException::throwNew(env, QString("Wrong QPair.second given: %1, expected: %2").arg(QtJambiAPI::getObjectClassName(env, second).replace("$", "."), QtJambiAPI::getClassName(env, valueClass).replace("$", ".")) QTJAMBI_STACKTRACEINFO );
                                    }
                                    if(!out){
                                        if(scope){
                                            out = pairAccess->createContainer();
                                        }
                                    }
                                    if(out){
                                        response = true;
                                        pairAccess->setFirst(env, out, first);
                                        pairAccess->setSecond(env, out, second);
                                    }
                                }
                                return response;
                            };
                        }else{
                            typedef bool (*IsBiContainerFunction)(JNIEnv *, jobject, const QMetaType&, const QMetaType&, void*& pointer, AbstractContainerAccess*& access);
                            typedef void (*ContainerInsertFunction)(AbstractContainerAccess*, JNIEnv *, const ContainerInfo&, jobject, jobject);

                            IsBiContainerFunction isContainerFunction = ContainerAPI::getAsQMap;
                            ContainerInsertFunction containerInsertFunction = [](AbstractContainerAccess* access, JNIEnv * env, const ContainerInfo& container, jobject key, jobject value){
                                if(AbstractMapAccess* _access = dynamic_cast<AbstractMapAccess*>(access))
                                    _access->insert(env, container, key, value);
                            };

                            switch(mapType){
                            case AssociativeContainerType::QMultiMap:
                                isContainerFunction = ContainerAPI::getAsQMultiMap;
                                containerInsertFunction = [](AbstractContainerAccess* access, JNIEnv * env, const ContainerInfo& container, jobject key, jobject value){
                                    jobject iterator = QtJambiAPI::iteratorOfJavaIterable(env, value);
                                    QList<jobject> values;
                                    while(QtJambiAPI::hasJavaIteratorNext(env, iterator)) {
                                        jobject val = QtJambiAPI::nextOfJavaIterator(env, iterator);
                                        values.prepend(val);
                                    }
                                    if(AbstractMultiMapAccess* _access = dynamic_cast<AbstractMultiMapAccess*>(access))
                                        for(jobject val : values)
                                            _access->insert(env, container, key, val);
                                };
                                break;
                            case AssociativeContainerType::QMultiHash:
                                isContainerFunction = ContainerAPI::getAsQMultiHash;
                                containerInsertFunction = [](AbstractContainerAccess* access, JNIEnv * env, const ContainerInfo& container, jobject key, jobject value){
                                    jobject iterator = QtJambiAPI::iteratorOfJavaIterable(env, value);
                                    QList<jobject> values;
                                    while(QtJambiAPI::hasJavaIteratorNext(env, iterator)) {
                                        jobject val = QtJambiAPI::nextOfJavaIterator(env, iterator);
                                        values.prepend(val);
                                    }
                                    if(AbstractMultiHashAccess* _access = dynamic_cast<AbstractMultiHashAccess*>(access))
                                        for(jobject val : values)
                                            _access->insert(env, container, key, val);
                                };
                                break;
                            case AssociativeContainerType::QHash:
                                isContainerFunction = ContainerAPI::getAsQHash;
                                containerInsertFunction = [](AbstractContainerAccess* access, JNIEnv * env, const ContainerInfo& container, jobject key, jobject value){
                                    if(AbstractHashAccess* _access = dynamic_cast<AbstractHashAccess*>(access))
                                        _access->insert(env, container, key, value);
                                };
                                break;
                            default:
                                break;
                            }

                            return [sharedAccess,
                                    containerInsertFunction,
                                    keyClass,
                                    memberMetaType1,
                                    valueClass,
                                    memberMetaType2,
                                    isContainerFunction](JNIEnv* env, QtJambiScope* scope, jvalue in, void* &out, jValueType valueType) -> bool{
                                bool response = false;
                                AbstractContainerAccess* otherAccess{nullptr};
                                void* pointer{nullptr};
                                if(valueType==jValueType::l){
                                    if(in.l && !env->IsInstanceOf(in.l, Java::Runtime::Map::getClass(env)))
                                        Java::Runtime::IllegalArgumentException::throwNew(env, QString("Wrong argument given: %1, expected: %2").arg(QtJambiAPI::getObjectClassName(env, in.l).replace("$", "."), QtJambiAPI::getClassName(env, Java::Runtime::Map::getClass(env)).replace("$", ".")) QTJAMBI_STACKTRACEINFO );
                                    bool deleteAccess = true;
                                    Q_ASSERT(env->GetObjectRefType(keyClass)==JNIGlobalRefType);
                                    Q_ASSERT(env->GetObjectRefType(valueClass)==JNIGlobalRefType);
                                    AbstractContainerAccess* access = sharedAccess->clone();
                                    if(!in.l){
                                        if(!out){
                                            if(scope){
                                                out = access->createContainer();
                                                scope->addFinalAction([access,out](){
                                                    access->deleteContainer(out);
                                                    access->dispose();
                                                });
                                                deleteAccess = false;
                                            }
                                        }
                                        response = true;
                                    }else if(isContainerFunction(env, in.l, memberMetaType1, memberMetaType2, pointer, otherAccess)){
                                        if(out){
                                            access->assign(out, pointer);
                                        }else if(scope){
                                            out = access->createContainer(env, ConstContainerAndAccessInfo{in.l, pointer, otherAccess});
                                            scope->addFinalAction([access,out](){
                                                access->deleteContainer(out);
                                                access->dispose();
                                            });
                                            deleteAccess = false;
                                        }else{
                                            out = pointer;
                                        }
                                        response = true;
                                    }else{
                                        if(!out){
                                            if(scope){
                                                out = access->createContainer();
                                                scope->addFinalAction([access,out](){
                                                    access->deleteContainer(out);
                                                    access->dispose();
                                                });
                                                deleteAccess = false;
                                            }
                                        }
                                        if(out){
                                            QList<jobject> content;
                                            if(in.l){
                                                jobject iter = QtJambiAPI::entrySetIteratorOfJavaMap(env, in.l);
                                                while(QtJambiAPI::hasJavaIteratorNext(env, iter)){
                                                    jobject next = QtJambiAPI::nextOfJavaIterator(env, iter);
                                                    jobject key = QtJambiAPI::keyOfJavaMapEntry(env, next);
                                                    jobject value = QtJambiAPI::valueOfJavaMapEntry(env, next);
                                                    if(key && !env->IsInstanceOf(key, keyClass))
                                                        Java::Runtime::IllegalArgumentException::throwNew(env, QString("Wrong map key given: %1, expected: %2").arg(QtJambiAPI::getObjectClassName(env, key).replace("$", "."), QtJambiAPI::getClassName(env, keyClass).replace("$", ".")) QTJAMBI_STACKTRACEINFO );
                                                    if(value && !env->IsInstanceOf(value, valueClass))
                                                        Java::Runtime::IllegalArgumentException::throwNew(env, QString("Wrong map value given: %1, expected: %2").arg(QtJambiAPI::getObjectClassName(env, value).replace("$", "."), QtJambiAPI::getClassName(env, valueClass).replace("$", ".")) QTJAMBI_STACKTRACEINFO );
                                                    content << next;
                                                }
                                            }
                                            for(const QSharedPointer<QtJambiLink>& link : QtJambiLink::findLinksForPointer(out)){
                                                if(auto _access = link->containerAccess()){
                                                    response = true;
                                                    jobject obj = link->getJavaObjectLocalRef(env);
                                                    for(jobject entry : std::as_const(content)) {
                                                        jobject key = QtJambiAPI::keyOfJavaMapEntry(env, entry);
                                                        jobject val = QtJambiAPI::valueOfJavaMapEntry(env, entry);
                                                        containerInsertFunction(_access, env, {obj, out}, key, val);
                                                    }
                                                    break;
                                                }
                                            }
                                            if(!response){
                                                response = true;
                                                for(jobject entry : std::as_const(content)) {
                                                    jobject key = QtJambiAPI::keyOfJavaMapEntry(env, entry);
                                                    jobject val = QtJambiAPI::valueOfJavaMapEntry(env, entry);
                                                    containerInsertFunction(access, env, {nullptr, out}, key, val);
                                                }
                                            }
                                        }
                                    }
                                    if(deleteAccess)
                                        access->dispose();
                                }
                                return response;
                            };
                        }
                    }
                }
                return [](JNIEnv*, QtJambiScope*, jvalue, void* &, jValueType) -> bool{
                    return false;
                };
            }
        }else if([&]() -> bool {
                 bool result = Java::QtJambi::ClassAnalyzerUtility::isGeneratedClass(_env, externalClass);
                 if(!result){
                     result = !SuperTypeInfos::fromClass(_env, externalClass).isEmpty();
                 }
                 return result;
            }()){
            const std::type_info* typeId = nullptr;
            {
                QString _internalTypeName = internalTypeName;
                if(_internalTypeName.startsWith("const "))
                    _internalTypeName = _internalTypeName.mid(6);
                if(_internalTypeName.endsWith("*") || _internalTypeName.endsWith("&"))
                    _internalTypeName.chop(1);
                typeId = getTypeByQtName(_internalTypeName);
            }
            if(!typeId){
                typeId = getTypeByJavaName(QtJambiAPI::getClassName(_env, externalClass).replace(".", "/"));
            }
            if(!typeId && internalMetaType.isValid()){
                QByteArray metaTypeName = internalMetaType.name();
                if(metaTypeName.startsWith("const "))
                    metaTypeName = metaTypeName.mid(6);
                if(metaTypeName.endsWith("*") || metaTypeName.endsWith("&"))
                    metaTypeName.chop(1);
                typeId = getTypeByQtName(metaTypeName.data());
                if(!typeId){
                    typeId = getTypeByMetaType(internalMetaType);
                }
            }
            if(!typeId){
                const SuperTypeInfos superTypeInfos = SuperTypeInfos::fromClass(_env, externalClass);
                if(!superTypeInfos.isEmpty()){
                    typeId = &superTypeInfos[0].typeId();
                }
            }
            if(typeId){
                bool isInterface = false;
                bool isPointer = false;
                bool isValue = false;
                bool isFunctionPointer = false;
                bool isStdFunction = false;
                EntryTypes entryType = getEntryType(*typeId);
                switch(entryType){
                case EntryTypes::FunctionPointerTypeInfo:
                    isFunctionPointer = true;
                    break;
                case EntryTypes::StdFunctionTypeInfo:
                    isStdFunction = true;
                    break;
                case EntryTypes::ValueTypeInfo:
                case EntryTypes::InterfaceValueTypeInfo:
                    isValue = true;
                    break;
                default:
                    break;
                }
                switch(entryType){
                case EntryTypes::InterfaceTypeInfo:
                case EntryTypes::InterfaceValueTypeInfo:
                    isInterface = true;
                    break;
                default:
                    break;
                }
                switch(getEntryType(*typeId)){
                case EntryTypes::ObjectTypeInfo:
                case EntryTypes::QObjectTypeInfo:
                case EntryTypes::InterfaceTypeInfo:
                    isPointer = true;
                    break;
                default:
                    break;
                }
                bool isQmlList = internalTypeName.startsWith("QQmlListProperty<");
                QMetaType _internalMetaType = internalMetaType;
                if(isQmlList && !internalMetaType.isValid()){
                    _internalMetaType = registeredMetaType(*typeId);
                }
                if(internalTypeName.endsWith("&") || !(isValue || isPointer || internalTypeName.endsWith("*") || isQmlList)){
                    if(isInterface){
                        return [typeId, externalClass](JNIEnv* env, QtJambiScope*, jvalue val, void* &out, jValueType) -> bool{
                            if(val.l){
                                if(val.l && !env->IsInstanceOf(val.l, externalClass))
                                    Java::Runtime::IllegalArgumentException::throwNew(env, QString("Wrong argument given: %1, expected: %2").arg(QtJambiAPI::getObjectClassName(env, val.l).replace("$", "."), QtJambiAPI::getClassName(env, externalClass).replace("$", ".")) QTJAMBI_STACKTRACEINFO );
                                out = QtJambiLink::findPointerForJavaInterface(env, val.l, *typeId);
                            }else{
                                out = nullptr;
                            }
                            return true;
                        };
                    }else if(isStdFunction){
                        return [typeId, externalClass, metaType = _internalMetaType](JNIEnv* env, QtJambiScope* scope, jvalue val, void* &out, jValueType) -> bool{
                            if(val.l){
                                if(val.l && !env->IsInstanceOf(val.l, externalClass))
                                    Java::Runtime::IllegalArgumentException::throwNew(env, QString("Wrong argument given: %1, expected: %2").arg(QtJambiAPI::getObjectClassName(env, val.l).replace("$", "."), QtJambiAPI::getClassName(env, externalClass).replace("$", ".")) QTJAMBI_STACKTRACEINFO );
                                if(QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForJavaInterface(env, val.l)){
                                    if(!out){
                                        if(scope){
                                            out = metaType.create();
                                            scope->addDeletion(metaType, out);
                                        }
                                    }
                                    if(QtJambiAPI::getObjectClassName(env, val.l).endsWith("$ConcreteWrapper")
                                            || env->IsSameObject(env->GetObjectClass(val.l), externalClass)){
                                        metaType.destruct(out);
                                        metaType.construct(out, link->pointer());
                                    }else{
                                        if(!link->isMultiInheritanceType()){
                                            if(FunctionalBase* functionalBase = reinterpret_cast<FunctionalBase *>(link->pointer())){
                                                functionalBase->getFunctional(env, out);
                                            }
                                        }else if(link->isInterfaceAvailable(*typeId)){
                                            if(FunctionalBase* functionalBase = reinterpret_cast<FunctionalBase *>(link->typedPointer(*typeId))){
                                                functionalBase->getFunctional(env, out);
                                            }
                                        }
                                    }
                                }else{
                                    Java::QtJambi::QNoNativeResourcesException::throwNew(env, QStringLiteral("Incomplete object of type: %1").arg(QtJambiAPI::getObjectClassName(env, val.l).replace("$", ".")) QTJAMBI_STACKTRACEINFO );
                                }
                            }else{
                                out = nullptr;
                            }
                            return true;
                        };
                    }else if(isFunctionPointer){
                        return [typeId, externalClass](JNIEnv* env, QtJambiScope* scope, jvalue val, void* &out, jValueType) -> bool{
                            if(val.l){
                                if(val.l && !env->IsInstanceOf(val.l, externalClass))
                                    Java::Runtime::IllegalArgumentException::throwNew(env, QString("Wrong argument given: %1, expected: %2").arg(QtJambiAPI::getObjectClassName(env, val.l).replace("$", "."), QtJambiAPI::getClassName(env, externalClass).replace("$", ".")) QTJAMBI_STACKTRACEINFO );
                                if(QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForJavaInterface(env, val.l)){
                                    if(!out){
                                        if(scope){
                                            out = new void*();
                                            scope->addDeletion(reinterpret_cast<void**>(out));
                                        }else{
                                            return false;
                                        }
                                    }
                                    if(QtJambiAPI::getObjectClassName(env, val.l).endsWith("$ConcreteWrapper")
                                            || env->IsSameObject(env->GetObjectClass(val.l), externalClass)){
                                        *reinterpret_cast<void**>(out) = *reinterpret_cast<void**>(link->pointer());
                                    }else{
                                        if(!link->isMultiInheritanceType()){
                                            if(FunctionalBase* functionalBase = reinterpret_cast<FunctionalBase *>(link->pointer())){
                                                functionalBase->getFunctional(env, out);
                                            }
                                        }else if(link->isInterfaceAvailable(*typeId)){
                                            if(FunctionalBase* functionalBase = reinterpret_cast<FunctionalBase *>(link->typedPointer(*typeId))){
                                                functionalBase->getFunctional(env, out);
                                            }
                                        }
                                    }
                                    if(!*reinterpret_cast<void**>(out)){
                                        QString funTypeName = QtJambiAPI::typeName(*typeId);
                                        Java::Runtime::ClassCastException::throwNew(env, QStringLiteral("Unable to convert java object of type '%1' to function pointer '%2'.").arg(QtJambiAPI::getClassName(env, externalClass).replace('$', '.'), funTypeName) QTJAMBI_STACKTRACEINFO );
                                    }
                                }else{
                                    Java::QtJambi::QNoNativeResourcesException::throwNew(env, QStringLiteral("Incomplete object of type: %1").arg(QtJambiAPI::getObjectClassName(env, val.l).replace("$", ".")) QTJAMBI_STACKTRACEINFO );
                                }
                            }else{
                                out = nullptr;
                            }
                            return true;
                        };
                    }else{
                        return [externalClass](JNIEnv* env, QtJambiScope*, jvalue val, void* &out, jValueType) -> bool{
                            if(val.l){
                                if(val.l && !env->IsInstanceOf(val.l, externalClass))
                                    Java::Runtime::IllegalArgumentException::throwNew(env, QString("Wrong argument given: %1, expected: %2").arg(QtJambiAPI::getObjectClassName(env, val.l).replace("$", "."), QtJambiAPI::getClassName(env, externalClass).replace("$", ".")) QTJAMBI_STACKTRACEINFO );
                                if(QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForJavaObject(env, val.l)){
                                    out = link->pointer();
                                }else{
                                    Java::QtJambi::QNoNativeResourcesException::throwNew(env, QStringLiteral("Incomplete object of type: %1").arg(QtJambiAPI::getObjectClassName(env, val.l).replace("$", ".")) QTJAMBI_STACKTRACEINFO );
                                }
                            }else{
                                out = nullptr;
                            }
                            return true;
                        };
                    }
                }else if((isQmlList || isValue) && !internalTypeName.endsWith("*")){
                    if(isInterface){
                        return [_internalMetaType, typeId, externalClass](JNIEnv* env, QtJambiScope* scope, jvalue val, void* &out, jValueType) -> bool{
                            void* ptr = nullptr;
                            if(val.l){
                                if(Java::QtCore::QMetaType$GenericTypeInterface::isInstanceOf(env,val.l)){
                                    QMetaType given = qtjambi_cast<QMetaType>(env, Java::QtCore::QMetaType$GenericTypeInterface::metaType(env, val.l));
                                    if(_internalMetaType!=given){
                                        Java::Runtime::IllegalArgumentException::throwNew(env, QStringLiteral("Wrong argument given: %1, expected: %2").arg(given.name(), _internalMetaType.name()) QTJAMBI_STACKTRACEINFO );
                                    }
                                }else{
                                    if(val.l && !env->IsInstanceOf(val.l, externalClass))
                                        Java::Runtime::IllegalArgumentException::throwNew(env, QStringLiteral("Wrong argument given: %1, expected: %2").arg(QtJambiAPI::getObjectClassName(env, val.l).replace("$", "."), QtJambiAPI::getClassName(env, externalClass).replace("$", ".")) QTJAMBI_STACKTRACEINFO );
                                }
                                ptr = QtJambiLink::findPointerForJavaInterface(env, val.l, *typeId);
                            }
                            if(scope && !out){
                                out = _internalMetaType.create(ptr);
                                void* pptr = out;
                                scope->addDeletion(_internalMetaType, pptr);
                            }else if(out){
                                _internalMetaType.destruct(out);
                                if(_internalMetaType.construct(out, ptr)!=out){
                                    if(ptr){
                                        QByteArray ba;
                                        bool saved;
                                        /* write the copy to the stream */ {
                                            QDataStream stream(&ba, QIODevice::WriteOnly);
                                            saved = _internalMetaType.save(stream, ptr);
                                        }

                                        /* read it back into the destination */
                                        if(saved){
                                            QDataStream stream(&ba, QIODevice::ReadOnly);
                                            _internalMetaType.load(stream, out);
                                        }else{
                                            return false;
                                        }
                                    }
                                }
                            }
                            return out;
                        };
                    }else{
                        return [_internalMetaType, externalClass](JNIEnv* env, QtJambiScope* scope, jvalue val, void* &out, jValueType) -> bool{
                            void* ptr = nullptr;
                            if(val.l){
                                if(Java::QtCore::QMetaType$GenericTypeInterface::isInstanceOf(env,val.l)){
                                    QMetaType given = qtjambi_cast<QMetaType>(env, Java::QtCore::QMetaType$GenericTypeInterface::metaType(env, val.l));
                                    if(_internalMetaType!=given){
                                        Java::Runtime::IllegalArgumentException::throwNew(env, QStringLiteral("Wrong argument given: %1, expected: %2").arg(given.name(), _internalMetaType.name()) QTJAMBI_STACKTRACEINFO );
                                    }
                                    if(QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForJavaObject(env, val.l)){
                                        ptr = link->pointer();
                                    }else{
                                        Java::QtJambi::QNoNativeResourcesException::throwNew(env, QStringLiteral("Incomplete object of type: %1").arg(QtJambiAPI::getObjectClassName(env, val.l).replace("$", ".")) QTJAMBI_STACKTRACEINFO );
                                    }
                                }else{
                                    if(val.l && !env->IsInstanceOf(val.l, externalClass))
                                        Java::Runtime::IllegalArgumentException::throwNew(env, QStringLiteral("Wrong argument given: %1, expected: %2").arg(QtJambiAPI::getObjectClassName(env, val.l).replace("$", "."), QtJambiAPI::getClassName(env, externalClass).replace("$", ".")) QTJAMBI_STACKTRACEINFO );
                                    if(QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForJavaObject(env, val.l)){
                                        ptr = link->pointer();
                                    }else{
                                        Java::QtJambi::QNoNativeResourcesException::throwNew(env, QStringLiteral("Incomplete object of type: %1").arg(QtJambiAPI::getObjectClassName(env, val.l).replace("$", ".")) QTJAMBI_STACKTRACEINFO );
                                    }
                                }
                            }
                            if(scope && !out){
                                out = _internalMetaType.create(ptr);
                                void* pptr = out;
                                scope->addDeletion(_internalMetaType, pptr);
                            }else if(out){
                                _internalMetaType.destruct(out);
                                if(_internalMetaType.construct(out, ptr)!=out){
                                    if(ptr){
                                        QByteArray ba;
                                        bool saved;
                                        /* write the copy to the stream */ {
                                            QDataStream stream(&ba, QIODevice::WriteOnly);
                                            saved = _internalMetaType.save(stream, ptr);
                                        }

                                        /* read it back into the destination */
                                        if(saved){
                                            QDataStream stream(&ba, QIODevice::ReadOnly);
                                            _internalMetaType.load(stream, out);
                                        }else{
                                            return false;
                                        }
                                    }
                                }
                            }
                            return out;
                        };
                    }
                }else{
                    if(isInterface){
                        return [typeId,externalClass](JNIEnv* env, QtJambiScope* scope, jvalue val, void* &out, jValueType) -> bool{
                            void* ptr = nullptr;
                            if(val.l){
                                if(val.l && !env->IsInstanceOf(val.l, externalClass))
                                    Java::Runtime::IllegalArgumentException::throwNew(env, QString("Wrong argument given: %1, expected: %2").arg(QtJambiAPI::getObjectClassName(env, val.l).replace("$", "."), QtJambiAPI::getClassName(env, externalClass).replace("$", ".")) QTJAMBI_STACKTRACEINFO );
                                ptr = QtJambiLink::findPointerForJavaInterface(env, val.l, *typeId);
                            }
                            if(scope && !out){
                                void** pptr;
                                out = pptr = new void*(ptr);
                                scope->addDeletion(pptr);
                            }else if(out)
                                *reinterpret_cast<void**>(out) = ptr;
                            return out;
                        };
                    }else{
                        return [externalClass](JNIEnv* env, QtJambiScope* scope, jvalue val, void* &out, jValueType) -> bool{
                            void* ptr = nullptr;
                            if(val.l){
                                if(val.l && !env->IsInstanceOf(val.l, externalClass))
                                    Java::Runtime::IllegalArgumentException::throwNew(env, QString("Wrong argument given: %1, expected: %2").arg(QtJambiAPI::getObjectClassName(env, val.l).replace("$", "."), QtJambiAPI::getClassName(env, externalClass).replace("$", ".")) QTJAMBI_STACKTRACEINFO );
                                if(QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForJavaObject(env, val.l)){
                                    ptr = link->pointer();
                                }else{
                                    Java::QtJambi::QNoNativeResourcesException::throwNew(env, QStringLiteral("Incomplete object of type: %1").arg(QtJambiAPI::getObjectClassName(env, val.l).replace("$", ".")) QTJAMBI_STACKTRACEINFO );
                                }
                            }
                            if(scope && !out){
                                void** pptr;
                                out = pptr = new void*(ptr);
                                scope->addDeletion(pptr);
                            }else if(out)
                                *reinterpret_cast<void**>(out) = ptr;
                            return out;
                        };
                    }
                }
            }
        }
    }

    qCWarning(CATEGORY) << "Cannot convert from type " << QtJambiAPI::getClassName(_env, externalClass).replace("$", ".") << " to " << internalTypeName << ".";
    return ParameterTypeInfo::default_externalToInternalConverter();
}

size_t QtJambiTypeManager::getInternalAlignment(const QString &internalTypeName)
{
    if(isPointerType(internalTypeName)){
        return Q_ALIGNOF(void*);
    }
    size_t align;
    align = size_t(QMetaType::fromName(internalTypeName.toLatin1()).alignOf());
    if(align<=0 && (internalTypeName.startsWith("QVector<")
                || internalTypeName.startsWith("QStack<")
                || internalTypeName.startsWith("QList<")
                || internalTypeName.startsWith("QQueue<")
                || internalTypeName.startsWith("QSet<")
                || internalTypeName.startsWith("QMap<")
                || internalTypeName.startsWith("QHash<")
                || internalTypeName.startsWith("QMultiMap<")
                || internalTypeName.startsWith("QMultiHash<")
         ) && internalTypeName.endsWith(">") ){
        align = alignof(QList<void*>);
    }
    return align;
}

size_t QtJambiTypeManager::getInternalSize(const QString &internalTypeName){
    if(isPointerType(internalTypeName)){
        return sizeof(void*);
    }
    int metaType = QMetaType::type(qPrintable(internalTypeName));
    if(metaType!=QMetaType::UnknownType){
        int sz = QMetaType::sizeOf(metaType);
        if(sz>0){
            return size_t(sz);
        }
    }
    if(const std::type_info* t = getTypeByQtName(internalTypeName)){
        size_t sz = getValueSizeAndAlignment(*t).first;
        if(sz>0){
            return sz;
        }
    }
    if(internalTypeName.startsWith("QPair<") && internalTypeName.endsWith(">")){
        QStringList instantiations = internalTypeName.chopped(1).mid(6).trimmed().split(",");
        if(instantiations.size()==2){
            instantiations[0] = instantiations[0].trimmed();
            instantiations[1] = instantiations[0].trimmed();
            const std::type_info* t1;
            if(isPointerType(instantiations[0])){
                t1 = getTypeByQtName(instantiations[0].chopped(1));
            }else{
                t1 = getTypeByQtName(instantiations[0]);
            }
            const std::type_info* t2;
            if(isPointerType(instantiations[1])){
                t2 = getTypeByQtName(instantiations[1].chopped(1));
            }else{
                t2 = getTypeByQtName(instantiations[1]);
            }
            size_t size1 = 0;
            size_t size2 = 0;
            if(t1){
                size1 = isPointerType(instantiations[0]) ? sizeof(void*) : getValueSizeAndAlignment(*t1).first;
            }else{
                int metaType1 = QMetaType::type(qPrintable(instantiations[0]));
                if(metaType1 != QMetaType::UnknownType){
                    int sz = QMetaType::sizeOf(metaType1);
                    size1 = sz<0 ? 0 : size_t(sz);
                }
                if(size1==0){
                    size1 = getInternalSize(instantiations[0]);
                }
            }
            if(t2){
                size2 = isPointerType(instantiations[1]) ? sizeof(void*) : getValueSizeAndAlignment(*t2).first;
            }else{
                int metaType2 = QMetaType::type(qPrintable(instantiations[1]));
                if(metaType2 != QMetaType::UnknownType){
                    int sz = QMetaType::sizeOf(metaType2);
                    size2 = sz<0 ? 0 : size_t(sz);
                }
                if(size2==0){
                    size2 = getInternalSize(instantiations[1]);
                }
            }
            if(size1>0 && size2>0){
                return size1+size2;
            }
        }
    }else if( (internalTypeName.startsWith("QVector<")
            || internalTypeName.startsWith("QStack<")
            || internalTypeName.startsWith("QList<")
            || internalTypeName.startsWith("QQueue<")
            || internalTypeName.startsWith("QSet<")
            || internalTypeName.startsWith("QMap<")
            || internalTypeName.startsWith("QHash<")
            || internalTypeName.startsWith("QMultiMap<")
            || internalTypeName.startsWith("QMultiHash<")
        ) && internalTypeName.endsWith(">") ){
        return sizeof(QList<void*>);
    }
    return 0;
}

bool lessThan(JNIEnv *env, jobject k1, jobject k2);

QtJambiUtils::QHashFunction QtJambiTypeManager::findHashFunction(bool isPointer, QMetaType metaType)
{
    if(isPointer){
        return &ContainerAccessAPI::pointerHashFunction;
    }else{
        QByteArray metaTypeName = QMetaType(metaType).name();
        if(metaTypeName.startsWith("const "))
            metaTypeName = metaTypeName.mid(6);
        if(metaTypeName.endsWith("*") || metaTypeName.endsWith("&"))
            return &ContainerAccessAPI::pointerHashFunction;
        const std::type_info* typeId = getTypeByQtName(metaTypeName.data());
        if(!typeId){
            typeId = getTypeByMetaType(metaType);
        }
        QtJambiUtils::QHashFunction f;
        if(typeId)
            f = registeredHashFunction(*typeId);
        if(!f)
            f = hashFunctionByMetaType(metaType.iface());
        if(!f){
            if(metaType.flags() & QMetaType::IsEnumeration){
                switch(metaType.sizeOf()){
                case sizeof(qint8): f = [](const void* ptr, size_t seed) -> size_t{ return !ptr ? 0 : qHash(*reinterpret_cast<const qint8*>(ptr), seed);}; break;
                case sizeof(qint16): f = [](const void* ptr, size_t seed) -> size_t{ return !ptr ? 0 : qHash(*reinterpret_cast<const qint16*>(ptr), seed);}; break;
                case sizeof(qint32): f = [](const void* ptr, size_t seed) -> size_t{ return !ptr ? 0 : qHash(*reinterpret_cast<const qint32*>(ptr), seed);}; break;
                case sizeof(qint64): f = [](const void* ptr, size_t seed) -> size_t{ return !ptr ? 0 : qHash(*reinterpret_cast<const qint64*>(ptr), seed);}; break;
                }
            }
        }
        if(!f && metaTypeName.startsWith("QFlags<"))
            f = [](const void* ptr, size_t seed) -> size_t{ return !ptr ? 0 : qHash(*reinterpret_cast<const qint32*>(ptr), seed);};
        if(!f && isJObjectWrappedMetaType(QMetaType(metaType)))
            f = registeredHashFunction(typeid(JObjectWrapper));
        if(!f && isNativeWrapperMetaType(QMetaType(metaType)))
            f = registeredHashFunction(typeid(JObjectWrapper));
        return f;
    }
}

#if defined(QTJAMBI_GENERIC_ACCESS)

struct MetaData{
    MetaData(const QMetaType& _metaType,
             const QtJambiUtils::QHashFunction& _hashFunction);
    MetaData(QMetaType&& _metaType = QMetaType(),
             QtJambiUtils::QHashFunction&& _hashFunction = QtJambiUtils::QHashFunction());
    MetaData(const MetaData& d);
    MetaData(MetaData&& d);
    QMetaType metaType;
    QtJambiUtils::QHashFunction hashFunction;
};

struct Storages{
    QThreadStorage<MetaData> storage[2];
};

Q_GLOBAL_STATIC(Storages, gCurrentMetaData)

MetaData::MetaData(const QMetaType& _metaType,
         const QtJambiUtils::QHashFunction& _hashFunction
        )
    : metaType(_metaType),
      hashFunction(_hashFunction)
{}

MetaData::MetaData(QMetaType&& _metaType,
         QtJambiUtils::QHashFunction&& _hashFunction)
    : metaType(std::move(_metaType)),
      hashFunction(std::move(_hashFunction))
{}

MetaData::MetaData(const MetaData& d)
    : metaType(d.metaType),
      hashFunction(d.hashFunction)
{}

MetaData::MetaData(MetaData&& d)
    : metaType(std::move(d.metaType)),
      hashFunction(std::move(d.hashFunction))
{}

const MetaData& currentMetaData(uint i)
{
    return gCurrentMetaData->storage[i].localData();
}

void setCurrentMetaData(uint i, const MetaData& metaData)
{
    gCurrentMetaData->storage[i].setLocalData(metaData);
}

void setCurrentMetaData(uint i, MetaData&& metaData)
{
    gCurrentMetaData->storage[i].setLocalData(std::move(metaData));
}

namespace ContainerAccessAPI{

AbstractMetaTypeInfoLocker::AbstractMetaTypeInfoLocker(int index, const QMetaType& _metaType, const QtJambiUtils::QHashFunction& _hashFunction)
    : m_index(index)
{
    const MetaData& md = currentMetaData(index);
    m_metaType = md.metaType;
    m_hashFunction = md.hashFunction;
    setCurrentMetaData(m_index, MetaData(_metaType, _hashFunction));
}

AbstractMetaTypeInfoLocker::~AbstractMetaTypeInfoLocker(){
    setCurrentMetaData(m_index, MetaData(m_metaType, m_hashFunction));
}

void constructContainerElement(uint i, void* ptr, const void* copy)
{
    const MetaData& metaData = currentMetaData(i);
    Q_ASSERT(metaData.metaType.isValid());
//    if(copy)
//        fprintf(stderr, "copy %s from %p at %p\n", metaData.metaType.name().data(), copy, ptr);
//    else
//        fprintf(stderr, "creating %s at %p\n", metaData.metaType.name().data(), ptr);
    metaData.metaType.construct(ptr, copy);
}

void destructContainerElement(uint i, void* ptr)
{
    const MetaData& metaData = currentMetaData(i);
    Q_ASSERT(metaData.metaType.isValid());
//    fprintf(stderr, "delete %s at %p\n", QMetaType::typeName(metaData.metaType), ptr);
    metaData.metaType.destruct(ptr);
}

QDebug containerElementDebugStream(QDebug debug, uint i, const void* ptr)
{
    const MetaData& metaData = currentMetaData(i);
    Q_ASSERT(metaData.metaType.isValid());
    if(!QMetaType(metaData.metaType).debugStream(debug, ptr)){
        if(metaData.metaType.flags() & QMetaType::IsEnumeration){
            switch(metaData.metaType.sizeOf()){
            case 1: return debug << metaData.metaType.name() << "(" << *reinterpret_cast<const qint8*>(ptr) << ")";
            case 2: return debug << metaData.metaType.name() << "(" << *reinterpret_cast<const qint16*>(ptr) << ")";
            case 4: return debug << metaData.metaType.name() << "(" << *reinterpret_cast<const qint32*>(ptr) << ")";
            case 8: return debug << metaData.metaType.name() << "(" << *reinterpret_cast<const qint64*>(ptr) << ")";
            default: break;
            }
        }
    }
    return debug;
}

QDataStream & containerElementDataStreamOut(QDataStream & stream, uint i, const void* ptr)
{
    const MetaData& metaData = currentMetaData(i);
    Q_ASSERT(metaData.metaType.isValid());
    if(!metaData.metaType.save(stream, ptr)){
        if(metaData.metaType.flags() & QMetaType::IsEnumeration){
            switch(metaData.metaType.sizeOf()){
            case 1: return stream << *reinterpret_cast<const qint8*>(ptr);
            case 2: return stream << *reinterpret_cast<const qint16*>(ptr);
            case 4: return stream << *reinterpret_cast<const qint32*>(ptr);
            case 8: return stream << *reinterpret_cast<const qint64*>(ptr);
            default: break;
            }
        }
    }
    return stream;
}

QDataStream & containerElementDataStreamIn(QDataStream & stream, uint i, void* ptr)
{
    const MetaData& metaData = currentMetaData(i);
    Q_ASSERT(metaData.metaType.isValid());
    if(!metaData.metaType.load(stream, ptr)){
        if(metaData.metaType.flags() & QMetaType::IsEnumeration){
            switch(metaData.metaType.sizeOf()){
            case 1: return stream >> *reinterpret_cast<qint8*>(ptr);
            case 2: return stream >> *reinterpret_cast<qint16*>(ptr);
            case 4: return stream >> *reinterpret_cast<qint32*>(ptr);
            case 8: return stream >> *reinterpret_cast<qint64*>(ptr);
            default: break;
            }
        }
    }
    return stream;
}

size_t containerElementHash(uint i, const void* ptr, size_t seed)
{
    const MetaData& metaData = currentMetaData(i);
    if(metaData.hashFunction){
        return metaData.hashFunction(ptr, seed);
    }else{
        if(metaData.metaType.flags() & QMetaType::IsEnumeration){
            switch(metaData.metaType.sizeOf()){
            case 1: return ::qHash(*reinterpret_cast<const qint8*>(ptr));
            case 2: return ::qHash(*reinterpret_cast<const qint16*>(ptr));
            case 4: return ::qHash(*reinterpret_cast<const qint32*>(ptr));
            case 8: return ::qHash(*reinterpret_cast<const qint64*>(ptr));
            default: break;
            }
        }else if((metaData.metaType.flags() & QMetaType::PointerToGadget) || metaData.metaType.flags() & QMetaType::PointerToQObject){
            return ::qHash(*reinterpret_cast<void*const*>(ptr));
        }
    }
    return 0;
}

bool containerElementEqual(uint i, const void* ptr, const void* ptr2)
{
    const MetaData& metaData = currentMetaData(i);
    Q_ASSERT(metaData.metaType.isValid());
    return metaData.metaType.equals(ptr, ptr2);
}

bool containerElementLess(uint i, const void* ptr, const void* ptr2)
{
    const MetaData& metaData = currentMetaData(i);
    Q_ASSERT(metaData.metaType.isValid());
    QPartialOrdering po = metaData.metaType.compare(ptr, ptr2);
    return po == QPartialOrdering::Less;
}

}//namespace ContainerAccessAPI

void qtjambi_container_destruct(const QtPrivate::QMetaTypeInterface * metaTypeInterface, void* ptr){
    QtJambiStorage* storage = getQtJambiStorage();
    {
        QReadLocker locker(storage->lock());
        if(storage->containerTypeInfos().contains(metaTypeInterface)){
            const ContainerTypeInfo& info = storage->containerTypeInfos()[metaTypeInterface];
            locker.unlock();
            auto sg = qScopeGuard([&](){locker.relock();});
            if(info.isPointer){
                info.dtor(metaTypeInterface, ptr);
            }else{
                MetaData oldMetaData = currentMetaData(0);
                setCurrentMetaData(0, MetaData(info.metaType, info.hashFunction));
                info.dtor(metaTypeInterface, ptr);
                setCurrentMetaData(0, oldMetaData);
            }
        }else if(storage->biContainerTypeInfos().contains(metaTypeInterface)){
            const BiContainerTypeInfo& info = storage->biContainerTypeInfos()[metaTypeInterface];
            locker.unlock();
            auto sg = qScopeGuard([&](){locker.relock();});
            if(info.isPointer1 && info.isPointer2){
                info.dtor(metaTypeInterface, ptr);
            }else if(info.isPointer1){
                MetaData oldMetaData1 = currentMetaData(1);
                setCurrentMetaData(1, MetaData(info.metaType2, info.hashFunction2));
                info.dtor(metaTypeInterface, ptr);
                setCurrentMetaData(1, oldMetaData1);
            }else if(info.isPointer2){
                MetaData oldMetaData0 = currentMetaData(0);
                setCurrentMetaData(0, MetaData(info.metaType1, info.hashFunction1));
                info.dtor(metaTypeInterface, ptr);
                setCurrentMetaData(0, oldMetaData0);
            }else{
                MetaData oldMetaData0 = currentMetaData(0);
                MetaData oldMetaData1 = currentMetaData(1);
                setCurrentMetaData(0, MetaData(info.metaType1, info.hashFunction1));
                setCurrentMetaData(1, MetaData(info.metaType2, info.hashFunction2));
                info.dtor(metaTypeInterface, ptr);
                setCurrentMetaData(1, oldMetaData1);
                setCurrentMetaData(0, oldMetaData0);
            }
        }
    }
}

void container_default_construct(const QtPrivate::QMetaTypeInterface * metaTypeInterface, void *where){
    QtJambiStorage* storage = getQtJambiStorage();
    {
        QReadLocker locker(storage->lock());
        if(storage->containerTypeInfos().contains(metaTypeInterface)){
            const ContainerTypeInfo& info = storage->containerTypeInfos()[metaTypeInterface];
            locker.unlock();
            auto sg = qScopeGuard([&](){locker.relock();});
            if(info.isPointer){
                info.defaultCtr(metaTypeInterface, where);
            }else{
                const ContainerTypeInfo& info = storage->containerTypeInfos()[metaTypeInterface];
                MetaData oldMetaData = currentMetaData(0);
                setCurrentMetaData(0, MetaData(info.metaType, info.hashFunction));
                info.defaultCtr(metaTypeInterface, where);
                setCurrentMetaData(0, oldMetaData);
            }
        }else if(storage->biContainerTypeInfos().contains(metaTypeInterface)){
            const BiContainerTypeInfo& info = storage->biContainerTypeInfos()[metaTypeInterface];
            locker.unlock();
            auto sg = qScopeGuard([&](){locker.relock();});
            if(info.isPointer1 && info.isPointer2){
                info.defaultCtr(metaTypeInterface, where);
            }else if(info.isPointer1){
                MetaData oldMetaData1 = currentMetaData(1);
                setCurrentMetaData(1, MetaData(info.metaType2, info.hashFunction2));
                info.defaultCtr(metaTypeInterface, where);
                setCurrentMetaData(1, oldMetaData1);
            }else if(info.isPointer2){
                MetaData oldMetaData0 = currentMetaData(0);
                setCurrentMetaData(0, MetaData(info.metaType1, info.hashFunction1));
                info.defaultCtr(metaTypeInterface, where);
                setCurrentMetaData(0, oldMetaData0);
            }else{
                MetaData oldMetaData0 = currentMetaData(0);
                MetaData oldMetaData1 = currentMetaData(1);
                setCurrentMetaData(0, MetaData(info.metaType1, info.hashFunction1));
                setCurrentMetaData(1, MetaData(info.metaType2, info.hashFunction2));
                info.defaultCtr(metaTypeInterface, where);
                setCurrentMetaData(1, oldMetaData1);
                setCurrentMetaData(0, oldMetaData0);
            }
        }
    }
}

void qtjambi_container_copy_construct(const QtPrivate::QMetaTypeInterface * metaTypeInterface, void *where, const void * copy){
    QtJambiStorage* storage = getQtJambiStorage();
    {
        QReadLocker locker(storage->lock());
        if(storage->containerTypeInfos().contains(metaTypeInterface)){
            const ContainerTypeInfo& info = storage->containerTypeInfos()[metaTypeInterface];
            locker.unlock();
            auto sg = qScopeGuard([&](){locker.relock();});
            if(info.isPointer){
                info.copyCtr(metaTypeInterface, where, copy);
            }else{
                MetaData oldMetaData = currentMetaData(0);
                setCurrentMetaData(0, MetaData(info.metaType, info.hashFunction));
                info.copyCtr(metaTypeInterface, where, copy);
                setCurrentMetaData(0, oldMetaData);
            }
        }else if(storage->biContainerTypeInfos().contains(metaTypeInterface)){
            const BiContainerTypeInfo& info = storage->biContainerTypeInfos()[metaTypeInterface];
            locker.unlock();
            auto sg = qScopeGuard([&](){locker.relock();});
            if(info.isPointer1 && info.isPointer2){
                info.copyCtr(metaTypeInterface, where, copy);
            }else if(info.isPointer1){
                MetaData oldMetaData1 = currentMetaData(1);
                setCurrentMetaData(1, MetaData(info.metaType2, info.hashFunction2));
                info.copyCtr(metaTypeInterface, where, copy);
                setCurrentMetaData(1, oldMetaData1);
            }else if(info.isPointer2){
                MetaData oldMetaData0 = currentMetaData(0);
                setCurrentMetaData(0, MetaData(info.metaType1, info.hashFunction1));
                info.copyCtr(metaTypeInterface, where, copy);
                setCurrentMetaData(0, oldMetaData0);
            }else{
                MetaData oldMetaData0 = currentMetaData(0);
                MetaData oldMetaData1 = currentMetaData(1);
                setCurrentMetaData(0, MetaData(info.metaType1, info.hashFunction1));
                setCurrentMetaData(1, MetaData(info.metaType2, info.hashFunction2));
                info.copyCtr(metaTypeInterface, where, copy);
                setCurrentMetaData(1, oldMetaData1);
                setCurrentMetaData(0, oldMetaData0);
            }
        }
    }
}

void qtjambi_container_move_construct(const QtPrivate::QMetaTypeInterface * metaTypeInterface, void *where, void * copy){
    QtJambiStorage* storage = getQtJambiStorage();
    {
        QReadLocker locker(storage->lock());
        if(storage->containerTypeInfos().contains(metaTypeInterface)){
            const ContainerTypeInfo& info = storage->containerTypeInfos()[metaTypeInterface];
            locker.unlock();
            auto sg = qScopeGuard([&](){locker.relock();});
            if(info.isPointer){
                info.moveCtr(metaTypeInterface, where, copy);
            }else{
                MetaData oldMetaData = currentMetaData(0);
                setCurrentMetaData(0, MetaData(info.metaType, info.hashFunction));
                info.moveCtr(metaTypeInterface, where, copy);
                setCurrentMetaData(0, oldMetaData);
            }
        }else if(storage->biContainerTypeInfos().contains(metaTypeInterface)){
            const BiContainerTypeInfo& info = storage->biContainerTypeInfos()[metaTypeInterface];
            locker.unlock();
            auto sg = qScopeGuard([&](){locker.relock();});
            if(info.isPointer1 && info.isPointer2){
                info.moveCtr(metaTypeInterface, where, copy);
            }else if(info.isPointer1){
                MetaData oldMetaData1 = currentMetaData(1);
                setCurrentMetaData(1, MetaData(info.metaType2, info.hashFunction2));
                info.moveCtr(metaTypeInterface, where, copy);
                setCurrentMetaData(1, oldMetaData1);
            }else if(info.isPointer2){
                MetaData oldMetaData0 = currentMetaData(0);
                setCurrentMetaData(0, MetaData(info.metaType1, info.hashFunction1));
                info.moveCtr(metaTypeInterface, where, copy);
                setCurrentMetaData(0, oldMetaData0);
            }else{
                MetaData oldMetaData0 = currentMetaData(0);
                MetaData oldMetaData1 = currentMetaData(1);
                setCurrentMetaData(0, MetaData(info.metaType1, info.hashFunction1));
                setCurrentMetaData(1, MetaData(info.metaType2, info.hashFunction2));
                info.moveCtr(metaTypeInterface, where, copy);
                setCurrentMetaData(1, oldMetaData1);
                setCurrentMetaData(0, oldMetaData0);
            }
        }
    }
}

bool qtjambi_container_equals(const QtPrivate::QMetaTypeInterface * metaTypeInterface, const void *container1, const void *container2){
    QtJambiStorage* storage = getQtJambiStorage();
    {
        QReadLocker locker(storage->lock());
        if(storage->containerTypeInfos().contains(metaTypeInterface)){
            const ContainerTypeInfo& info = storage->containerTypeInfos()[metaTypeInterface];
            locker.unlock();
            auto sg = qScopeGuard([&](){locker.relock();});
            if(info.equals){
                if(info.isPointer){
                    return info.equals(metaTypeInterface, container1, container2);
                }else{
                    MetaData oldMetaData = currentMetaData(0);
                    setCurrentMetaData(0, MetaData(info.metaType, info.hashFunction));
                    bool result = info.equals(metaTypeInterface, container1, container2);
                    setCurrentMetaData(0, oldMetaData);
                    return result;
                }
            }
        }else if(storage->biContainerTypeInfos().contains(metaTypeInterface)){
            const BiContainerTypeInfo& info = storage->biContainerTypeInfos()[metaTypeInterface];
            locker.unlock();
            auto sg = qScopeGuard([&](){locker.relock();});
            if(info.equals){
                bool result;
                if(info.isPointer1 && info.isPointer2){
                    result = info.equals(metaTypeInterface, container1, container2);
                }else if(info.isPointer1){
                    MetaData oldMetaData1 = currentMetaData(1);
                    setCurrentMetaData(1, MetaData(info.metaType2, info.hashFunction2));
                    result = info.equals(metaTypeInterface, container1, container2);
                    setCurrentMetaData(1, oldMetaData1);
                }else if(info.isPointer2){
                    MetaData oldMetaData0 = currentMetaData(0);
                    setCurrentMetaData(0, MetaData(info.metaType1, info.hashFunction1));
                    result = info.equals(metaTypeInterface, container1, container2);
                    setCurrentMetaData(0, oldMetaData0);
                }else{
                    MetaData oldMetaData0 = currentMetaData(0);
                    MetaData oldMetaData1 = currentMetaData(1);
                    setCurrentMetaData(0, MetaData(info.metaType1, info.hashFunction1));
                    setCurrentMetaData(1, MetaData(info.metaType2, info.hashFunction2));
                    result = info.equals(metaTypeInterface, container1, container2);
                    setCurrentMetaData(1, oldMetaData1);
                    setCurrentMetaData(0, oldMetaData0);
                }
                return result;
            }
        }
    }
    return false;
}

bool container_lessThan(const QtPrivate::QMetaTypeInterface * metaTypeInterface, const void *container1, const void *container2){
    QtJambiStorage* storage = getQtJambiStorage();
    {
        QReadLocker locker(storage->lock());
        if(storage->containerTypeInfos().contains(metaTypeInterface)){
            const ContainerTypeInfo& info = storage->containerTypeInfos()[metaTypeInterface];
            locker.unlock();
            auto sg = qScopeGuard([&](){locker.relock();});
            if(info.lessThan){
                if(info.isPointer){
                    return info.lessThan(metaTypeInterface, container1, container2);
                }else{
                    MetaData oldMetaData = currentMetaData(0);
                    setCurrentMetaData(0, MetaData(info.metaType, info.hashFunction));
                    bool result = info.lessThan(metaTypeInterface, container1, container2);
                    setCurrentMetaData(0, oldMetaData);
                    return result;
                }
            }
        }else if(storage->biContainerTypeInfos().contains(metaTypeInterface)){
            const BiContainerTypeInfo& info = storage->biContainerTypeInfos()[metaTypeInterface];
            locker.unlock();
            auto sg = qScopeGuard([&](){locker.relock();});
            if(info.lessThan){
                bool result;
                if(info.isPointer1 && info.isPointer2){
                    result = info.lessThan(metaTypeInterface, container1, container2);
                }else if(info.isPointer1){
                    MetaData oldMetaData1 = currentMetaData(1);
                    setCurrentMetaData(1, MetaData(info.metaType2, info.hashFunction2));
                    result = info.lessThan(metaTypeInterface, container1, container2);
                    setCurrentMetaData(1, oldMetaData1);
                }else if(info.isPointer2){
                    MetaData oldMetaData0 = currentMetaData(0);
                    setCurrentMetaData(0, MetaData(info.metaType1, info.hashFunction1));
                    result = info.lessThan(metaTypeInterface, container1, container2);
                    setCurrentMetaData(0, oldMetaData0);
                }else{
                    MetaData oldMetaData0 = currentMetaData(0);
                    MetaData oldMetaData1 = currentMetaData(1);
                    setCurrentMetaData(0, MetaData(info.metaType1, info.hashFunction1));
                    setCurrentMetaData(1, MetaData(info.metaType2, info.hashFunction2));
                    result = info.lessThan(metaTypeInterface, container1, container2);
                    setCurrentMetaData(1, oldMetaData1);
                    setCurrentMetaData(0, oldMetaData0);
                }
                return result;
            }
        }
    }
    return false;
}

void qtjambi_container_debugStream(const QtPrivate::QMetaTypeInterface * metaTypeInterface, QDebug & debug, const void *container){
    QtJambiStorage* storage = getQtJambiStorage();
    {
        QReadLocker locker(storage->lock());
        if(storage->containerTypeInfos().contains(metaTypeInterface)){
            const ContainerTypeInfo& info = storage->containerTypeInfos()[metaTypeInterface];
            locker.unlock();
            auto sg = qScopeGuard([&](){locker.relock();});
            if(info.debugStream){
                if(info.isPointer){
                    info.debugStream(metaTypeInterface, debug, container);
                }else{
                    MetaData oldMetaData = currentMetaData(0);
                    setCurrentMetaData(0, MetaData(info.metaType, info.hashFunction));
                    info.debugStream(metaTypeInterface, debug, container);
                    setCurrentMetaData(0, oldMetaData);
                }
            }
        }else if(storage->biContainerTypeInfos().contains(metaTypeInterface)){
            const BiContainerTypeInfo& info = storage->biContainerTypeInfos()[metaTypeInterface];
            locker.unlock();
            auto sg = qScopeGuard([&](){locker.relock();});
            if(info.debugStream){
                if(info.isPointer1 && info.isPointer2){
                    info.debugStream(metaTypeInterface, debug, container);
                }else if(info.isPointer1){
                    MetaData oldMetaData1 = currentMetaData(1);
                    setCurrentMetaData(1, MetaData(info.metaType2, info.hashFunction2));
                    info.debugStream(metaTypeInterface, debug, container);
                    setCurrentMetaData(1, oldMetaData1);
                }else if(info.isPointer2){
                    MetaData oldMetaData0 = currentMetaData(0);
                    setCurrentMetaData(0, MetaData(info.metaType1, info.hashFunction1));
                    info.debugStream(metaTypeInterface, debug, container);
                    setCurrentMetaData(0, oldMetaData0);
                }else{
                    MetaData oldMetaData0 = currentMetaData(0);
                    MetaData oldMetaData1 = currentMetaData(1);
                    setCurrentMetaData(0, MetaData(info.metaType1, info.hashFunction1));
                    setCurrentMetaData(1, MetaData(info.metaType2, info.hashFunction2));
                    info.debugStream(metaTypeInterface, debug, container);
                    setCurrentMetaData(1, oldMetaData1);
                    setCurrentMetaData(0, oldMetaData0);
                }
            }
        }
    }
}

void qtjambi_container_dataStreamOut(const QtPrivate::QMetaTypeInterface * metaTypeInterface, QDataStream & dataStream, const void *container){
    QtJambiStorage* storage = getQtJambiStorage();
    {
        QReadLocker locker(storage->lock());
        if(storage->containerTypeInfos().contains(metaTypeInterface)){
            const ContainerTypeInfo& info = storage->containerTypeInfos()[metaTypeInterface];
            locker.unlock();
            auto sg = qScopeGuard([&](){locker.relock();});
            if(info.dataStreamOut){
                if(info.isPointer){
                    info.dataStreamOut(metaTypeInterface, dataStream, container);
                }else{
                    MetaData oldMetaData = currentMetaData(0);
                    setCurrentMetaData(0, MetaData(info.metaType, info.hashFunction));
                    info.dataStreamOut(metaTypeInterface, dataStream, container);
                    setCurrentMetaData(0, oldMetaData);
                }
            }
        }else if(storage->biContainerTypeInfos().contains(metaTypeInterface)){
            const BiContainerTypeInfo& info = storage->biContainerTypeInfos()[metaTypeInterface];
            locker.unlock();
            auto sg = qScopeGuard([&](){locker.relock();});
            if(info.dataStreamOut){
                if(info.isPointer1 && info.isPointer2){
                    info.dataStreamOut(metaTypeInterface, dataStream, container);
                }else if(info.isPointer1){
                    MetaData oldMetaData1 = currentMetaData(1);
                    setCurrentMetaData(1, MetaData(info.metaType2, info.hashFunction2));
                    info.dataStreamOut(metaTypeInterface, dataStream, container);
                    setCurrentMetaData(1, oldMetaData1);
                }else if(info.isPointer2){
                    MetaData oldMetaData0 = currentMetaData(0);
                    setCurrentMetaData(0, MetaData(info.metaType1, info.hashFunction1));
                    info.dataStreamOut(metaTypeInterface, dataStream, container);
                    setCurrentMetaData(0, oldMetaData0);
                }else{
                    MetaData oldMetaData0 = currentMetaData(0);
                    MetaData oldMetaData1 = currentMetaData(1);
                    setCurrentMetaData(0, MetaData(info.metaType1, info.hashFunction1));
                    setCurrentMetaData(1, MetaData(info.metaType2, info.hashFunction2));
                    info.dataStreamOut(metaTypeInterface, dataStream, container);
                    setCurrentMetaData(1, oldMetaData1);
                    setCurrentMetaData(0, oldMetaData0);
                }
            }
        }
    }
}

void qtjambi_container_dataStreamIn(const QtPrivate::QMetaTypeInterface * metaTypeInterface, QDataStream & dataStream, void *container){
    QtJambiStorage* storage = getQtJambiStorage();
    {
        QReadLocker locker(storage->lock());
        if(storage->containerTypeInfos().contains(metaTypeInterface)){
            const ContainerTypeInfo& info = storage->containerTypeInfos()[metaTypeInterface];
            locker.unlock();
            auto sg = qScopeGuard([&](){locker.relock();});
            if(info.dataStreamIn){
                if(info.isPointer){
                    info.dataStreamIn(metaTypeInterface, dataStream, container);
                }else{
                    MetaData oldMetaData = currentMetaData(0);
                    setCurrentMetaData(0, MetaData(info.metaType, info.hashFunction));
                    info.dataStreamIn(metaTypeInterface, dataStream, container);
                    setCurrentMetaData(0, oldMetaData);
                }
            }
        }else if(storage->biContainerTypeInfos().contains(metaTypeInterface)){
            const BiContainerTypeInfo& info = storage->biContainerTypeInfos()[metaTypeInterface];
            locker.unlock();
            auto sg = qScopeGuard([&](){locker.relock();});
            if(info.dataStreamIn){
                if(info.isPointer1 && info.isPointer2){
                    info.dataStreamIn(metaTypeInterface, dataStream, container);
                }else if(info.isPointer1){
                    MetaData oldMetaData1 = currentMetaData(1);
                    setCurrentMetaData(1, MetaData(info.metaType2, info.hashFunction2));
                    info.dataStreamIn(metaTypeInterface, dataStream, container);
                    setCurrentMetaData(1, oldMetaData1);
                }else if(info.isPointer2){
                    MetaData oldMetaData0 = currentMetaData(0);
                    setCurrentMetaData(0, MetaData(info.metaType1, info.hashFunction1));
                    info.dataStreamIn(metaTypeInterface, dataStream, container);
                    setCurrentMetaData(0, oldMetaData0);
                }else{
                    MetaData oldMetaData0 = currentMetaData(0);
                    MetaData oldMetaData1 = currentMetaData(1);
                    setCurrentMetaData(0, MetaData(info.metaType1, info.hashFunction1));
                    setCurrentMetaData(1, MetaData(info.metaType2, info.hashFunction2));
                    info.dataStreamIn(metaTypeInterface, dataStream, container);
                    setCurrentMetaData(1, oldMetaData1);
                    setCurrentMetaData(0, oldMetaData0);
                }
            }
        }
    }
}

size_t computeHash(const QtMetaContainerPrivate::QMetaSequenceInterface& defaultInterface, const QMetaType& elementMetaType){
    QList<quintptr> range{quintptr(&defaultInterface),quintptr(elementMetaType.id())};
    return qHashRange(range.begin(), range.end());
}

size_t computeHash(const QtMetaContainerPrivate::QMetaAssociationInterface& defaultInterface, const QMetaType& keyMetaType, const QMetaType& valueMetaType){
    QList<quintptr> range{quintptr(&defaultInterface),quintptr(keyMetaType.id()),quintptr(valueMetaType.id())};
    return qHashRange(range.begin(), range.end());
}

const QtMetaContainerPrivate::QMetaSequenceInterface& qtjambi_find_meta_sequence(QMetaType elementMetaType, const QtMetaContainerPrivate::QMetaSequenceInterface& defaultInterface)
{
    QtJambiStorage* storage = getQtJambiStorage();
    {
        size_t hash = computeHash(defaultInterface, elementMetaType);
        {
            QReadLocker locker(storage->lock());
            if(storage->metaSequenceHash().contains(hash))
               return storage->metaSequenceHash()[hash];
        }
        {
            QWriteLocker locker(storage->lock());
            if(storage->metaSequenceHash().contains(hash))
               return storage->metaSequenceHash()[hash];
            QtMetaContainerPrivate::QMetaSequenceInterface d(defaultInterface);
            d.valueMetaType = elementMetaType.iface();
            storage->metaSequenceHash().insert(hash, d);
            return storage->metaSequenceHash()[hash];
        }
    }
}

const QtMetaContainerPrivate::QMetaAssociationInterface& qtjambi_find_meta_association(QMetaType metaType1, QtJambiUtils::QHashFunction hashFunction1, QMetaType metaType2, QtJambiUtils::QHashFunction hashFunction2, const QtMetaContainerPrivate::QMetaAssociationInterface& defaultInterface)
{
    QtJambiStorage* storage = getQtJambiStorage();
    {
        size_t hash = computeHash(defaultInterface, metaType1, metaType2);
        {
            QReadLocker locker(storage->lock());
            if(storage->metaAssociationHash().contains(hash))
               return storage->metaAssociationHash()[hash];
        }
        {
            QWriteLocker locker(storage->lock());
            if(storage->metaAssociationHash().contains(hash))
               return storage->metaAssociationHash()[hash];
            QtMetaContainerPrivate::QMetaAssociationInterface d(defaultInterface);
            d.keyMetaType = metaType1.iface();
            d.mappedMetaType = metaType2.iface();

            if(auto insertKeyFn = d.insertKeyFn){
                d.insertKeyFn = qtjambi_function_pointer<16,void(void *, const void *)>([insertKeyFn,metaType1,hashFunction1,metaType2,hashFunction2](void *c, const void *k) {
                    MetaData oldMetaData1 = currentMetaData(0);
                    setCurrentMetaData(0, MetaData(metaType1, hashFunction1));
                    MetaData oldMetaData2 = currentMetaData(1);
                    setCurrentMetaData(1, MetaData(metaType2, hashFunction2));
                    insertKeyFn(c,k);
                    setCurrentMetaData(0, oldMetaData1);
                    setCurrentMetaData(1, oldMetaData2);
                }, hash);
            }

            if(auto removeKeyFn = d.removeKeyFn){
                d.removeKeyFn = qtjambi_function_pointer<16,void(void *, const void *)>([removeKeyFn,metaType1,hashFunction1,metaType2,hashFunction2](void *c, const void *k) {
                    MetaData oldMetaData1 = currentMetaData(0);
                    setCurrentMetaData(0, MetaData(metaType1, hashFunction1));
                    MetaData oldMetaData2 = currentMetaData(1);
                    setCurrentMetaData(1, MetaData(metaType2, hashFunction2));
                    removeKeyFn(c,k);
                    setCurrentMetaData(0, oldMetaData1);
                    setCurrentMetaData(1, oldMetaData2);
                }, hash);
            }

            if(auto containsKeyFn = d.containsKeyFn){
                d.containsKeyFn = qtjambi_function_pointer<16,bool(const void *, const void *)>([containsKeyFn,metaType1,hashFunction1,metaType2,hashFunction2](const void *c, const void *k) -> bool {
                    MetaData oldMetaData1 = currentMetaData(0);
                    setCurrentMetaData(0, MetaData(metaType1, hashFunction1));
                    MetaData oldMetaData2 = currentMetaData(1);
                    setCurrentMetaData(1, MetaData(metaType2, hashFunction2));
                    bool result = containsKeyFn(c,k);
                    setCurrentMetaData(0, oldMetaData1);
                    setCurrentMetaData(1, oldMetaData2);
                    return result;
                }, hash);
            }

            if(auto mappedAtKeyFn = d.mappedAtKeyFn){
                d.mappedAtKeyFn = qtjambi_function_pointer<16,void(const void *, const void *, void *)>([mappedAtKeyFn,metaType1,hashFunction1,metaType2,hashFunction2](const void *c, const void *k, void *r) {
                    MetaData oldMetaData1 = currentMetaData(0);
                    setCurrentMetaData(0, MetaData(metaType1, hashFunction1));
                    MetaData oldMetaData2 = currentMetaData(1);
                    setCurrentMetaData(1, MetaData(metaType2, hashFunction2));
                    mappedAtKeyFn(c,k, r);
                    setCurrentMetaData(0, oldMetaData1);
                    setCurrentMetaData(1, oldMetaData2);
                }, hash);
            }

            if(auto setMappedAtKeyFn = d.setMappedAtKeyFn){
                d.setMappedAtKeyFn = qtjambi_function_pointer<16,void(void *, const void *, const void *)>([setMappedAtKeyFn,metaType1,hashFunction1,metaType2,hashFunction2](void *c, const void *k, const void *r) {
                    MetaData oldMetaData1 = currentMetaData(0);
                    setCurrentMetaData(0, MetaData(metaType1, hashFunction1));
                    MetaData oldMetaData2 = currentMetaData(1);
                    setCurrentMetaData(1, MetaData(metaType2, hashFunction2));
                    setMappedAtKeyFn(c,k, r);
                    setCurrentMetaData(0, oldMetaData1);
                    setCurrentMetaData(1, oldMetaData2);
                }, hash);
            }

            if(auto createIteratorAtKeyFn = d.createIteratorAtKeyFn){
                d.createIteratorAtKeyFn = qtjambi_function_pointer<16,void*(void *, const void *)>([createIteratorAtKeyFn,metaType1,hashFunction1,metaType2,hashFunction2](void *c, const void *k) -> void* {
                    MetaData oldMetaData1 = currentMetaData(0);
                    setCurrentMetaData(0, MetaData(metaType1, hashFunction1));
                    MetaData oldMetaData2 = currentMetaData(1);
                    setCurrentMetaData(1, MetaData(metaType2, hashFunction2));
                    void* result = createIteratorAtKeyFn(c,k);
                    setCurrentMetaData(0, oldMetaData1);
                    setCurrentMetaData(1, oldMetaData2);
                    return result;
                }, hash);
            }

            if(auto createConstIteratorAtKeyFn = d.createConstIteratorAtKeyFn){
                d.createConstIteratorAtKeyFn = qtjambi_function_pointer<16,void*(const void *, const void *)>([createConstIteratorAtKeyFn,metaType1,hashFunction1,metaType2,hashFunction2](const void *c, const void *k) -> void* {
                    MetaData oldMetaData1 = currentMetaData(0);
                    setCurrentMetaData(0, MetaData(metaType1, hashFunction1));
                    MetaData oldMetaData2 = currentMetaData(1);
                    setCurrentMetaData(1, MetaData(metaType2, hashFunction2));
                    void* result = createConstIteratorAtKeyFn(c,k);
                    setCurrentMetaData(0, oldMetaData1);
                    setCurrentMetaData(1, oldMetaData2);
                    return result;
                }, hash);
            }

            if(auto keyAtIteratorFn = d.keyAtIteratorFn){
                d.keyAtIteratorFn = qtjambi_function_pointer<16,void(const void *, void *)>([keyAtIteratorFn,metaType1,hashFunction1,metaType2,hashFunction2](const void *c, void *k) {
                    MetaData oldMetaData1 = currentMetaData(0);
                    setCurrentMetaData(0, MetaData(metaType1, hashFunction1));
                    MetaData oldMetaData2 = currentMetaData(1);
                    setCurrentMetaData(1, MetaData(metaType2, hashFunction2));
                    keyAtIteratorFn(c,k);
                    setCurrentMetaData(0, oldMetaData1);
                    setCurrentMetaData(1, oldMetaData2);
                }, hash);
            }

            if(auto keyAtConstIteratorFn = d.keyAtConstIteratorFn){
                d.keyAtConstIteratorFn = qtjambi_function_pointer<16,void(const void *, void *)>([keyAtConstIteratorFn,metaType1,hashFunction1,metaType2,hashFunction2](const void *c, void *k) {
                    MetaData oldMetaData1 = currentMetaData(0);
                    setCurrentMetaData(0, MetaData(metaType1, hashFunction1));
                    MetaData oldMetaData2 = currentMetaData(1);
                    setCurrentMetaData(1, MetaData(metaType2, hashFunction2));
                    keyAtConstIteratorFn(c,k);
                    setCurrentMetaData(0, oldMetaData1);
                    setCurrentMetaData(1, oldMetaData2);
                }, hash);
            }

            if(auto mappedAtIteratorFn = d.mappedAtIteratorFn){
                d.mappedAtIteratorFn = qtjambi_function_pointer<16,void(const void *, void *)>([mappedAtIteratorFn,metaType1,hashFunction1,metaType2,hashFunction2](const void *c, void *k) {
                    MetaData oldMetaData1 = currentMetaData(0);
                    setCurrentMetaData(0, MetaData(metaType1, hashFunction1));
                    MetaData oldMetaData2 = currentMetaData(1);
                    setCurrentMetaData(1, MetaData(metaType2, hashFunction2));
                    mappedAtIteratorFn(c,k);
                    setCurrentMetaData(0, oldMetaData1);
                    setCurrentMetaData(1, oldMetaData2);
                }, hash);
            }

            if(auto mappedAtConstIteratorFn = d.mappedAtConstIteratorFn){
                d.mappedAtConstIteratorFn = qtjambi_function_pointer<16,void(const void *, void *)>([mappedAtConstIteratorFn,metaType1,hashFunction1,metaType2,hashFunction2](const void *c, void *k) {
                    MetaData oldMetaData1 = currentMetaData(0);
                    setCurrentMetaData(0, MetaData(metaType1, hashFunction1));
                    MetaData oldMetaData2 = currentMetaData(1);
                    setCurrentMetaData(1, MetaData(metaType2, hashFunction2));
                    mappedAtConstIteratorFn(c,k);
                    setCurrentMetaData(0, oldMetaData1);
                    setCurrentMetaData(1, oldMetaData2);
                }, hash);
            }

            if(auto setMappedAtIteratorFn = d.setMappedAtIteratorFn){
                d.setMappedAtIteratorFn = qtjambi_function_pointer<16,void(const void *, const void *)>([setMappedAtIteratorFn,metaType1,hashFunction1,metaType2,hashFunction2](const void *c, const void *k) {
                    MetaData oldMetaData1 = currentMetaData(0);
                    setCurrentMetaData(0, MetaData(metaType1, hashFunction1));
                    MetaData oldMetaData2 = currentMetaData(1);
                    setCurrentMetaData(1, MetaData(metaType2, hashFunction2));
                    setMappedAtIteratorFn(c,k);
                    setCurrentMetaData(0, oldMetaData1);
                    setCurrentMetaData(1, oldMetaData2);
                }, hash);
            }

                if(auto eraseKeyAtIteratorFn = d.eraseKeyAtIteratorFn){
                d.eraseKeyAtIteratorFn = qtjambi_function_pointer<16,void(void *, const void *)>([eraseKeyAtIteratorFn,metaType1,hashFunction1,metaType2,hashFunction2](void *c, const void *k) {
                    MetaData oldMetaData1 = currentMetaData(0);
                    setCurrentMetaData(0, MetaData(metaType1, hashFunction1));
                    MetaData oldMetaData2 = currentMetaData(1);
                    setCurrentMetaData(1, MetaData(metaType2, hashFunction2));
                    eraseKeyAtIteratorFn(c,k);
                    setCurrentMetaData(0, oldMetaData1);
                    setCurrentMetaData(1, oldMetaData2);
                }, hash);
            }

            storage->metaAssociationHash().insert(hash, d);
            return storage->metaAssociationHash()[hash];
        }
    }
}

QMetaType QtJambiPrivate::registerSequentialContainerType(const QByteArray& typeName, size_t containerSize, size_t containerAlign, bool isPointer, const QMetaType& metaType,
                                      QtPrivate::QMetaTypeInterface::DefaultCtrFn defaultCtr,
                                      QtPrivate::QMetaTypeInterface::CopyCtrFn copyCtr,
                                      QtPrivate::QMetaTypeInterface::MoveCtrFn moveCtr,
                                      QtPrivate::QMetaTypeInterface::DtorFn dtor,
                                      QtPrivate::QMetaTypeInterface::EqualsFn equals,
                                      QtPrivate::QMetaTypeInterface::LessThanFn lessThan,
                                      QtPrivate::QMetaTypeInterface::DebugStreamFn debugStream,
                                      QtPrivate::QMetaTypeInterface::DataStreamOutFn dataStreamOut,
                                      QtPrivate::QMetaTypeInterface::DataStreamInFn dataStreamIn,
                                      const QtMetaContainerPrivate::QMetaSequenceInterface *defaultInterface,
                                      MetaSequenceIteratorFactory createMetaSequenceIterator,
                                      MetaSequenceConstIteratorFactory createMetaSequenceConstIterator,
                                      HashWrapper hashWrapper,
                                      AbstractSequentialAccess* access)
{
    QtJambiUtils::QHashFunction hashFunction = QtJambiTypeManager::findHashFunction(isPointer, metaType);
    if(!const_cast<QMetaType&>(metaType).iface()->equals
            && !(metaType.flags() & QMetaType::IsPointer)
            && !(metaType.flags() & QMetaType::IsEnumeration))
        equals = nullptr;
    lessThan = nullptr;
    if(!const_cast<QMetaType&>(metaType).iface()->debugStream
            && !(metaType.flags() & QMetaType::IsPointer)
            && !(metaType.flags() & QMetaType::IsEnumeration))
        debugStream = nullptr;
    if(!const_cast<QMetaType&>(metaType).iface()->dataStreamIn
                && !(metaType.flags() & QMetaType::IsEnumeration))
        dataStreamIn = nullptr;
    if(!const_cast<QMetaType&>(metaType).iface()->dataStreamOut
                && !(metaType.flags() & QMetaType::IsEnumeration))
        dataStreamOut = nullptr;
    QMetaType newMetaType = createMetaType(typeName,
                                            true,
                                            /*.defaultCtr=*/ defaultCtr ? &container_default_construct : nullptr,
                                            /*.copyCtr=*/ copyCtr ? &qtjambi_container_copy_construct : nullptr,
                                            /*.moveCtr=*/ moveCtr ? &qtjambi_container_move_construct : nullptr,
                                            /*.dtor=*/ dtor ? &qtjambi_container_destruct : nullptr,
                                            /*.equals=*/ equals ? &qtjambi_container_equals : nullptr,
                                            /*.lessThan=*/ lessThan ? &container_lessThan : nullptr,
                                            /*.debugStream=*/ debugStream ? &qtjambi_container_debugStream : nullptr,
                                            /*.dataStreamOut=*/ dataStreamOut ? &qtjambi_container_dataStreamOut : nullptr,
                                            /*.dataStreamIn=*/ dataStreamIn ? &qtjambi_container_dataStreamIn : nullptr,
                                            /*.legacyRegisterOp=*/ nullptr,
                                            /*.size=*/ uint(containerSize),
                                            /*.alignment=*/ ushort(containerAlign),
                                            /*.typeId=*/ QMetaType::UnknownType,
                                            /*.flags=*/ QMetaType::TypeFlags(QMetaType::RelocatableType | QMetaType::NeedsDestruction | QMetaType::NeedsConstruction),
                                            /*.metaObject=*/ nullptr,
                                            /*.metaObject=*/ nullptr
                                    );
    storage->containerTypeInfos().insert(newMetaType.iface(), {isPointer,
                                                    metaType,
                                                    hashFunction,
                                                    defaultCtr,
                                                    copyCtr,
                                                    moveCtr,
                                                    dtor,
                                                    equals,
                                                    lessThan,
                                                    debugStream,
                                                    dataStreamOut,
                                                    dataStreamIn});
    if(defaultInterface){
        const QMetaType to = QMetaType::fromType<QIterable<QMetaSequence>>();
        if (createMetaSequenceConstIterator && !QMetaType::hasRegisteredConverterFunction(newMetaType, to)) {
            QMetaType _metaType = metaType;
            QMetaType::registerConverterFunction([_metaType, defaultInterface, createMetaSequenceConstIterator](const void *src, void *target) -> bool {
                const QtMetaContainerPrivate::QMetaSequenceInterface &d = qtjambi_find_meta_sequence(_metaType, *defaultInterface);
                return createMetaSequenceConstIterator(src, target, &d);
            }, newMetaType, to);
        }
        if (createMetaSequenceIterator && !QMetaType::hasRegisteredMutableViewFunction(newMetaType, to)) {
            QMetaType _metaType = metaType;
            QMetaType::registerMutableViewFunction([_metaType, defaultInterface, createMetaSequenceIterator](void *src, void *target) -> bool {
                const QtMetaContainerPrivate::QMetaSequenceInterface &d = qtjambi_find_meta_sequence(_metaType, *defaultInterface);
                return createMetaSequenceIterator(src, target, &d);
            }, newMetaType, to);
        }
    }
    if(!hashFunctionByMetaType(newMetaType.iface())){
        insertHashFunctionByMetaType(newMetaType.iface(),
                                            [metaType, hashFunction, hashWrapper]
                                            (const void* ptr, size_t seed)->size_t{
                                                if(ptr){
                                                    MetaData oldMetaData = currentMetaData(0);
                                                    setCurrentMetaData(0, MetaData(metaType, hashFunction));
                                                    size_t result = hashWrapper(ptr, seed, hashFunction);
                                                    setCurrentMetaData(0, oldMetaData);
                                                    return result;
                                                }else{
                                                    return 0;
                                                }
                                            });
    }
    registerContainerAccess(newMetaType, access);
    return newMetaType;
}

QMetaType QtJambiPrivate::registerAssociativeContainerType(const QByteArray& typeName,
                                      size_t containerSize,
                                      size_t containerAlign,
                                      bool isPointer1, const QMetaType& metaType1,
                                      bool isPointer2, const QMetaType& metaType2,
                                      QtPrivate::QMetaTypeInterface::DefaultCtrFn defaultCtr,
                                      QtPrivate::QMetaTypeInterface::CopyCtrFn copyCtr,
                                      QtPrivate::QMetaTypeInterface::MoveCtrFn moveCtr,
                                      QtPrivate::QMetaTypeInterface::DtorFn dtor,
                                      QtPrivate::QMetaTypeInterface::EqualsFn equals,
                                      QtPrivate::QMetaTypeInterface::LessThanFn lessThan,
                                      QtPrivate::QMetaTypeInterface::DebugStreamFn debugStream,
                                      QtPrivate::QMetaTypeInterface::DataStreamOutFn dataStreamOut,
                                      QtPrivate::QMetaTypeInterface::DataStreamInFn dataStreamIn,
                                      const QtMetaContainerPrivate::QMetaAssociationInterface *defaultInterface,
                                      MetaAssociationIteratorFactory createMetaAssociationIterator,
                                      MetaAssociationConstIteratorFactory createMetaAssociationConstIterator,
                                      PairAccessFactory pairAccessFactory,
                                      HashWrapper2 hashWrapper,
                                      AbstractContainerAccess* access)
{
    QtJambiUtils::QHashFunction hashFunction1 = QtJambiTypeManager::findHashFunction(isPointer1, metaType1);
    QtJambiUtils::QHashFunction hashFunction2 = QtJambiTypeManager::findHashFunction(isPointer2, metaType2);
    if((!const_cast<QMetaType&>(metaType1).iface()->equals
        && !(metaType1.flags() & QMetaType::IsPointer)
        && !(metaType1.flags() & QMetaType::IsEnumeration))
        || (!const_cast<QMetaType&>(metaType2).iface()->equals
            && !(metaType2.flags() & QMetaType::IsPointer)
            && !(metaType2.flags() & QMetaType::IsEnumeration)))
        equals = nullptr;
    lessThan = nullptr;
    if((!const_cast<QMetaType&>(metaType1).iface()->debugStream
        && !(metaType1.flags() & QMetaType::IsPointer)
        && !(metaType1.flags() & QMetaType::IsEnumeration))
        || (!const_cast<QMetaType&>(metaType2).iface()->debugStream
            && !(metaType2.flags() & QMetaType::IsPointer)
            && !(metaType2.flags() & QMetaType::IsEnumeration)))
        debugStream = nullptr;
    if((!const_cast<QMetaType&>(metaType1).iface()->dataStreamIn
        && !(metaType1.flags() & QMetaType::IsEnumeration))
            || (!const_cast<QMetaType&>(metaType2).iface()->dataStreamIn
                && !(metaType2.flags() & QMetaType::IsEnumeration)))
        dataStreamIn = nullptr;
    if((!const_cast<QMetaType&>(metaType1).iface()->dataStreamOut
        && !(metaType1.flags() & QMetaType::IsEnumeration))
            || (!const_cast<QMetaType&>(metaType2).iface()->dataStreamOut
                && !(metaType2.flags() & QMetaType::IsEnumeration)))
        dataStreamOut = nullptr;
    QMetaType newMetaType = createMetaType(typeName,
                                            true,
                                            /*.defaultCtr=*/ defaultCtr ? &container_default_construct : nullptr,
                                            /*.copyCtr=*/ copyCtr ? &qtjambi_container_copy_construct : nullptr,
                                            /*.moveCtr=*/ moveCtr ? &qtjambi_container_move_construct : nullptr,
                                            /*.dtor=*/ dtor ? &qtjambi_container_destruct : nullptr,
                                            /*.equals=*/ equals ? &qtjambi_container_equals : nullptr,
                                            /*.lessThan=*/ lessThan ? &container_lessThan : nullptr,
                                            /*.debugStream=*/ debugStream ? &qtjambi_container_debugStream : nullptr,
                                            /*.dataStreamOut=*/ dataStreamOut ? &qtjambi_container_dataStreamOut : nullptr,
                                            /*.dataStreamIn=*/ dataStreamIn ? &qtjambi_container_dataStreamIn : nullptr,
                                            /*.legacyRegisterOp=*/ nullptr,
                                            /*.size=*/ uint(containerSize),
                                            /*.alignment=*/ ushort(containerAlign),
                                            /*.typeId=*/ QMetaType::UnknownType,
                                            /*.flags=*/ QMetaType::TypeFlags(QMetaType::RelocatableType | QMetaType::NeedsDestruction | QMetaType::NeedsConstruction),
                                            /*.metaObject=*/ nullptr,
                                            /*.metaObject=*/ nullptr);
    QtJambiStorage* storage = getQtJambiStorage();
    {
        QWriteLocker locker(storage->lock());
        storage->biContainerTypeInfos().insert(newMetaType.iface(), {isPointer1,
                                                      metaType1,
                                                      hashFunction1,
                                                      isPointer2,
                                                      metaType2,
                                                      hashFunction2,
                                                      defaultCtr,
                                                      copyCtr,
                                                      moveCtr,
                                                      dtor,
                                                      equals,
                                                      lessThan,
                                                      debugStream,
                                                      dataStreamOut,
                                                      dataStreamIn});
    }
    if(createMetaAssociationIterator){
        if(defaultInterface){
            static const QMetaType to = QMetaType::fromType<QIterable<QMetaAssociation>>();
            if (createMetaAssociationConstIterator && !QMetaType::hasRegisteredConverterFunction(newMetaType, to)) {
                QMetaType _metaType1 = metaType1;
                QMetaType _metaType2 = metaType2;
                QMetaType::registerConverterFunction([_metaType1, hashFunction1, _metaType2, hashFunction2, defaultInterface, createMetaAssociationConstIterator](const void *src, void *target) -> bool {
                    const QtMetaContainerPrivate::QMetaAssociationInterface &d = qtjambi_find_meta_association(_metaType1, hashFunction1, _metaType2, hashFunction2, *defaultInterface);
                    return createMetaAssociationConstIterator(src, target, &d);
                }, newMetaType, to);
            }
            if (createMetaAssociationIterator && !QMetaType::hasRegisteredMutableViewFunction(newMetaType, to)) {
                QMetaType _metaType1 = metaType1;
                QMetaType _metaType2 = metaType2;
                QMetaType::registerMutableViewFunction([_metaType1, hashFunction1,_metaType2, hashFunction2, defaultInterface, createMetaAssociationIterator](void *src, void *target) -> bool {
                    const QtMetaContainerPrivate::QMetaAssociationInterface &d = qtjambi_find_meta_association(_metaType1, hashFunction1, _metaType2, hashFunction2, *defaultInterface);
                    return createMetaAssociationIterator(src, target, &d);
                }, newMetaType, to);
            }
        }else if(pairAccessFactory){ // is QPair
            static const QMetaType to = QMetaType::fromType<QtMetaTypePrivate::QPairVariantInterfaceImpl>();
            if (!QMetaType::hasRegisteredConverterFunction(newMetaType, to)) {
                QMetaType _metaType1 = metaType1;
                QMetaType _metaType2 = metaType2;
                QMetaType::registerConverterFunction([_metaType1, _metaType2, pairAccessFactory](const void *src, void *target) -> bool {
                    return pairAccessFactory(src, target, _metaType1, _metaType2);
                }, newMetaType, to);
            }
        }
    }
    if(!hashFunctionByMetaType(newMetaType.iface())){
        insertHashFunctionByMetaType(newMetaType.iface(),
                                            [metaType1, hashFunction1,
                                             metaType2, hashFunction2, hashWrapper]
                                            (const void* ptr, size_t seed)->size_t{
                                                if(ptr){
                                                    MetaData oldMetaData1 = currentMetaData(0);
                                                    setCurrentMetaData(0, MetaData(metaType1, hashFunction1));
                                                    MetaData oldMetaData2 = currentMetaData(1);
                                                    setCurrentMetaData(1, MetaData(metaType2, hashFunction2));
                                                    size_t result = hashWrapper(ptr,  seed, hashFunction1, hashFunction2);
                                                    setCurrentMetaData(0, oldMetaData1);
                                                    setCurrentMetaData(1, oldMetaData2);
                                                    return result;
                                                }else{
                                                    return 0;
                                                }
                                            });
    }
    registerContainerAccess(newMetaType, access);
    return newMetaType;
}
#endif //defined(QTJAMBI_GENERIC_ACCESS)
