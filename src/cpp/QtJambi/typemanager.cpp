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

#include <QtCore/qcompilerdetection.h>
QT_WARNING_DISABLE_DEPRECATED
QT_WARNING_DISABLE_GCC("-Wignored-qualifiers")
QT_WARNING_DISABLE_CLANG("-Wignored-qualifiers")
QT_WARNING_DISABLE_GCC("-Winit-list-lifetime")
#include "functionpointer.h"
#include <QtCore/QThread>
#include <QtCore/QThreadStorage>
#include <QtCore/QIODevice>
#include <QtCore/QMetaType>
#include <QtCore/QHash>
#include <QtCore/QList>
#include <QtCore/QSet>
#include <QtCore/QMap>
#include <QtCore/QMultiHash>
#include <QtCore/QMultiMap>
#include <QtCore/QLoggingCategory>

#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
#include <QtCore/QLinkedList>
#include <QtCore/QVector>
#include "coreapi.h"
#endif

#include "qtjambiapi.h"
#include "functionalbase.h"
#include "java_p.h"
#include "jobjectwrapper.h"
#include "typemanager_p.h"
#include "qtjambilink_p.h"
#include "registryutil_p.h"
#include "containeraccess.h"
#include "containeraccess_p.h"
#include "supertypeinfo_p.h"
#include "qtjambimetaobject_p.h"
#include "utils_p.h"

#include <cstring>
#include "qtjambi_cast.h"

Q_LOGGING_CATEGORY(CATEGORY, "io.qtjambi.typemanager", QtWarningMsg)

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
#define QtJambiMetaType QMetaType
#define METATYPE_ID(metaType) metaType.id()
#else
#define METATYPE_ID(metaTypeId) metaTypeId
class QtJambiMetaType{
public:
    inline QtJambiMetaType() : m_metaType(new QMetaType()) {}
    inline QtJambiMetaType(const QtJambiMetaType& other) : m_metaType(other.m_metaType) {}
    inline QtJambiMetaType(int metaType) : m_metaType(new QMetaType(metaType)) {}
    inline QtJambiMetaType(const QMetaType& metaType) : m_metaType(new QMetaType(metaType.id())) {}
    inline QtJambiMetaType& operator=(int metaType){
        if(m_metaType->id()!=metaType)
            m_metaType.reset(new QMetaType(metaType));
        return *this;
    }
    inline QtJambiMetaType& operator=(const QMetaType& metaType){
        if(m_metaType->id()!=metaType.id())
            m_metaType.reset(new QMetaType(metaType.id()));
        return *this;
    }
    inline bool isValid() const { return m_metaType->isValid(); }
    inline QByteArray name() const { return m_metaType->name(); }
    inline int id() const { return m_metaType->id(); }
    inline operator int() const { return m_metaType->id(); }
    inline QMetaType::TypeFlags flags() const { return m_metaType->flags(); }
    inline int sizeOf() const { return m_metaType->sizeOf(); }
    operator const QMetaType&() const { return *m_metaType; }
private:
    QSharedPointer<QMetaType> m_metaType;
};
#endif

const char* getInterface(const char*qt_interface);

Q_GLOBAL_STATIC_WITH_ARGS(QReadWriteLock, gCacheLock, (QReadWriteLock::Recursive))
typedef QHash<hash_type, InternalToExternalConverter> InternalToExternalConverterHash;
typedef QHash<hash_type, ExternalToInternalConverter> ExternalToInternalConverterHash;
Q_GLOBAL_STATIC(InternalToExternalConverterHash, gInternalToExternalConverters)
Q_GLOBAL_STATIC(ExternalToInternalConverterHash, gExternalToInternalConverters)
typedef QMap<int, QHashFunction> HashFunctionHash;
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
typedef QHash<hash_type, QtMetaContainerPrivate::QMetaAssociationInterface> MetaAssociationHash;
typedef QHash<hash_type, QtMetaContainerPrivate::QMetaSequenceInterface> MetaSequenceHash;
Q_GLOBAL_STATIC(MetaAssociationHash, gMetaAssociationHash)
Q_GLOBAL_STATIC(MetaSequenceHash, gMetaSequenceHash)
#endif
typedef QMap<int, QMetaEnum> MetaEnumHash;
Q_GLOBAL_STATIC(HashFunctionHash, gHashFunctionByMetaTypeHash)
Q_GLOBAL_STATIC(MetaEnumHash, gMetaEnumByMetaTypeHash)
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
typedef QMap<QByteArray, size_t> SizeByTypeHash;
Q_GLOBAL_STATIC(SizeByTypeHash, gAlignmentByTypeHash)
#endif

jobject internal_convertQObjectSmartPointerToJavaObject(JNIEnv *env, const char *className, void* ptr_shared_pointer, SmartPointerDeleter shared_pointer_deleter, SmartPointerGetter pointerGetter);
jobject internal_convertQObjectSmartPointerToJavaObject(JNIEnv *env, const std::type_info& typeId, void* ptr_shared_pointer, SmartPointerDeleter shared_pointer_deleter, SmartPointerGetter pointerGetter);

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

void insertHashFunctionByMetaType(int type, const QHashFunction& fct){
    gHashFunctionByMetaTypeHash->insert(type, fct);
}

hash_type ContainerAccessAPI::pointerHashFunction(const void* ptr, hash_type seed){ return !ptr ? 0 : ::qHash(*reinterpret_cast<QHashDummyValue*const*>(ptr), seed);}
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
bool ContainerAccessAPI::pointerLessFunction(const void* ptr1, const void* ptr2){ return !ptr1 || !ptr2 ? false : quintptr(*reinterpret_cast<void*const*>(ptr1)) < quintptr(*reinterpret_cast<void*const*>(ptr2));}
bool ContainerAccessAPI::pointerEqualFunction(const void* ptr1, const void* ptr2){ return !ptr1 || !ptr2 ? false : quintptr(*reinterpret_cast<void*const*>(ptr1)) == quintptr(*reinterpret_cast<void*const*>(ptr2));}
#endif

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

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
bool QtJambiTypeManager::hasRegisteredComparators(int id){
    return QMetaType::hasRegisteredComparators(id) ||
            id==QMetaType::Bool ||
            id==QMetaType::Double ||
            id==QMetaType::Float ||
            id==QMetaType::Char ||
            id==QMetaType::SChar ||
            id==QMetaType::UChar ||
            id==QMetaType::Short ||
            id==QMetaType::UShort ||
            id==QMetaType::Int ||
            id==QMetaType::UInt ||
            id==QMetaType::Long ||
            id==QMetaType::ULong ||
            id==QMetaType::LongLong ||
            id==QMetaType::ULongLong ||
            id==QMetaType::QDate ||
            id==QMetaType::QDateTime ||
            id==QMetaType::QTime ||
            id==QMetaType::QString ||
            id==QMetaType::QStringList ||
            registeredComparator(id);
}
#endif

QHashFunction hashFunctionByMetaType(int metaType)
{
    if(metaType!=QMetaType::UnknownType){
        return gHashFunctionByMetaTypeHash->value(metaType, QHashFunction());
    }
    return QHashFunction();
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
    }else if(_internalTypeName.startsWith("QScopedPointer<") && _internalTypeName.endsWith(">")){
        _pointerType = PointerType::ScopedPointer;
        return _internalTypeName.mid(15, _internalTypeName.length() - 15)+"*";
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

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
QList<int> registeredCustomMetaTypesForJavaClass(const QByteArray& javaClass);
#else
QList<QMetaType> registeredCustomMetaTypesForJavaClass(const QByteArray& javaClass);
#endif

EntryTypes getEntryType(const std::type_info& typeId);

QString QtJambiTypeManager::getInternalTypeName(JNIEnv* env, jclass externalClass, bool useNextSuperclass) {
    // First we try a simple look up. This will handle any type in the type system,
    // so: primitives, boxed primitives and direct mappings of Qt types
    if(Java::Runtime::Void::isPrimitiveType(env, externalClass))
        return QLatin1String("void");
    else if(Java::Runtime::Object::isSameClass(env, externalClass))
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
        const std::type_info* typeId = getTypeByJavaName(externalTypeName);
        if(typeId){
            int metaTypeID = registeredMetaTypeID(*typeId);
            if(metaTypeID!=QMetaType::UnknownType){
                QString qtName = QMetaType::typeName(metaTypeID);
                switch(getEntryType(*typeId)){
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
                QString qtName = getQtName(*typeId);
                switch(getEntryType(*typeId)){
                case EntryTypes::ObjectTypeInfo:
                case EntryTypes::QObjectTypeInfo:
                case EntryTypes::InterfaceTypeInfo:
                    qtName += QLatin1String("*");
                    break;
                default:break;
                }
                return qtName;
            }
        }else{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
            QList<int> metaTypeIDs = registeredCustomMetaTypesForJavaClass(externalTypeName.toLatin1());
            if(metaTypeIDs.size()==1 && metaTypeIDs[0]!=0){
                return QLatin1String(QMetaType(metaTypeIDs[0]).name());
            }
#else
            QList<QMetaType> metaTypeIDs = registeredCustomMetaTypesForJavaClass(externalTypeName.toLatin1());
            if(metaTypeIDs.size()==1 && metaTypeIDs[0].isValid()){
                return QLatin1String(metaTypeIDs[0].name());
            }
#endif
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
            const SuperTypeInfos& superType = SuperTypeInfos::fromClass(env, externalClass);
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
            return {};
        }
    }
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

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
jclass registeredJavaClassForCustomMetaType(QMetaType metaType);
#else
jclass registeredJavaClassForCustomMetaType(const QMetaType& metaType);
#endif

QString QtJambiTypeManager::getExternalTypeName(JNIEnv* environment, const QString &_internalTypeName, const QMetaType& metaType) {
    switch(metaType.id()){
    case QMetaType::Void: return "void";
    case QMetaType::SChar:
    case QMetaType::Char:
    case QMetaType::UChar: return "byte";
    case QMetaType::UShort:
    case QMetaType::Short: return "short";
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    case QMetaType::Char32:
#endif
    case QMetaType::Int:
    case QMetaType::UInt: return "int";
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    case QMetaType::Char16:
#endif
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
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    case QMetaType::QVariantPair: return "io/qt/core/QPair";
#endif
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
    else if (_internalTypeName=="JIteratorWrapper")
        return QLatin1String("java/util/Iterator");
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
    else if (_internalTypeName=="JEnumWrapper")
        return QLatin1String("java/lang/Enum");
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
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
        return QLatin1String("java/util/LinkedList");
#else
        return QLatin1String("io/qt/core/QLinkedList");
#endif
    else if (_internalTypeName.startsWith("QList<") && _internalTypeName.endsWith(">"))
        return QLatin1String("io/qt/core/QList");
    else if (_internalTypeName.startsWith("QSet<") && _internalTypeName.endsWith(">"))
        return QLatin1String("io/qt/core/QSet");
    else if (_internalTypeName.startsWith("QQueue<") && _internalTypeName.endsWith(">"))
        return QLatin1String("io/qt/core/QQueue");
    else if (_internalTypeName.startsWith("QVector<") && _internalTypeName.endsWith(">"))
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
        return QLatin1String("io/qt/core/QList");
#else
        return QLatin1String("io/qt/core/QVector");
#endif
    else if (_internalTypeName.startsWith("QStack<") && _internalTypeName.endsWith(">"))
        return QLatin1String("io/qt/core/QStack");
    else if (_internalTypeName.startsWith("std::vector<") && _internalTypeName.endsWith(">"))
        return QLatin1String("java/util/ArrayList");
    else if (_internalTypeName.startsWith("std::map<") && _internalTypeName.endsWith(">"))
        return QLatin1String("java/util/HashMap");
    else if (_internalTypeName.startsWith("std::set<") && _internalTypeName.endsWith(">"))
        return QLatin1String("java/util/HashSet");
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
    }
    if(javaName.isEmpty())
        javaName = QLatin1String(getInterface(qPrintable(internalTypeName)));// TODO adapt!!
    if(javaName.isEmpty()){
        if(!metaType.isValid()){
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
            QMetaType _metaType(QMetaType::type(qPrintable(_internalTypeName)));
#else
            QMetaType _metaType = QMetaType::fromName(internalTypeName.toLatin1());
#endif //QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
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
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
            QVariant variant(metaType.id(), nullptr);
            if(variant.canConvert(QMetaType::QVariantList)){
                javaName = "io/qt/core/QList";
            }else if(variant.canConvert(QMetaType::QVariantMap)){
                javaName = "io/qt/core/QMap";
            }else if(variant.canConvert(QMetaType::QVariantHash)){
                javaName = "io/qt/core/QHash";
            }
#else
            if(QMetaType::canConvert(QMetaType(metaType), QMetaType(QMetaType::QVariantList))){
                javaName = "io/qt/core/QList";
            }else if(QMetaType::canConvert(QMetaType(metaType), QMetaType(QMetaType::QVariantMap))){
                javaName = "io/qt/core/QMap";
            }else if(QMetaType::canConvert(QMetaType(metaType), QMetaType(QMetaType::QVariantHash))){
                javaName = "io/qt/core/QHash";
            }
#endif
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
    }else if(gMetaEnumByMetaTypeHash->value(metaType.id()).isValid()){
        QMetaEnum enumerator = gMetaEnumByMetaTypeHash->value(metaType.id());
        if(enumerator.isFlag()){
            return "io/qt/core/QMetaType$GenericFlags";
        }else{
            switch(metaType.sizeOf()){
            case 1:  return "io/qt/core/QMetaType$GenericByteEnumerator";
            case 2:  return "io/qt/core/QMetaType$GenericShortEnumerator";
            case 8:  return "io/qt/core/QMetaType$GenericLongEnumerator";
            default: return "io/qt/core/QMetaType$GenericEnumerator";
            }
        }
    }else{
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
                    gMetaEnumByMetaTypeHash->insert(metaType.id(), enumerator);
                    if(enumerator.isFlag()){
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
                    gMetaEnumByMetaTypeHash->insert(metaType.id(), enumerator);
                    if(enumerator.isFlag()){
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
            if(metaTypeName!=_internalTypeName){
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
            }else if(isPointerType(_internalTypeName)){
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
hash_type qHash(const QMetaType& metaType, hash_type seed = 0){
    return qHash(metaType.id(), seed);
}
#endif

hash_type computeHash(JNIEnv* env,
                   const QString &internalTypeName,
                   const QMetaType& internalMetaType,
                   jclass externalClass,
                   bool allowValuePointers){
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    if(internalMetaType.isValid())
        return qHashMulti(0, internalMetaType, externalClass ? Java::Runtime::Object::hashCode(env, externalClass) : 0, allowValuePointers);
    else
        return qHashMulti(0, internalTypeName, externalClass ? Java::Runtime::Object::hashCode(env, externalClass) : 0, allowValuePointers);
#else
    QList<hash_type> range;
    if(internalMetaType.isValid())
        range << qHash(internalMetaType);
    else
        range << qHash(internalTypeName);
    range << qHash(externalClass ? Java::Runtime::Object::hashCode(env, externalClass) : 0);
    range << qHash(allowValuePointers);
    return qHashRange(range.begin(), range.end());
#endif
}

hash_type computeHash(JNIEnv* env, jclass externalClass, const QString &internalTypeName, const QMetaType& internalMetaType) {
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    if(internalMetaType.isValid())
        return qHashMulti(0, internalMetaType, externalClass ? Java::Runtime::Object::hashCode(env, externalClass) : 0);
    else
        return qHashMulti(0, internalTypeName, externalClass ? Java::Runtime::Object::hashCode(env, externalClass) : 0);
#else
    QList<hash_type> range;
    if(internalMetaType.isValid())
        range << qHash(internalMetaType);
    else
        range << qHash(internalTypeName);
    range << qHash(externalClass ? Java::Runtime::Object::hashCode(env, externalClass) : 0);
    return qHashRange(range.begin(), range.end());
#endif
}

InternalToExternalConverter QtJambiTypeManager::tryGetInternalToExternalConverter(
        JNIEnv* env,
        const QString &internalTypeName,
        const QMetaType& internalMetaType,
        jclass externalClass,
        bool allowValuePointers){
    hash_type key = computeHash(env, internalTypeName, internalMetaType, externalClass, allowValuePointers);
    {
        QReadLocker locker(gCacheLock());
        Q_UNUSED(locker)
        if(gInternalToExternalConverters()->contains(key))
            return (*gInternalToExternalConverters())[key];
    }
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    if(internalMetaType.isValid() && !QMetaType(internalMetaType).iface()->metaObjectFn){
        InternalToExternalConverter converter = getInternalToExternalConverterImpl(
                    env,
                    internalTypeName,
                    internalMetaType,
                    externalClass,
                    allowValuePointers
                );
        Q_ASSERT(converter);
        QWriteLocker locker(gCacheLock());
        Q_UNUSED(locker)
        if(gInternalToExternalConverters()->contains(key))
            return (*gInternalToExternalConverters())[key];
        (*gInternalToExternalConverters())[key] = converter;
        return converter;
    }
#endif
    return InternalToExternalConverter();
}

InternalToExternalConverter QtJambiTypeManager::getInternalToExternalConverter(
                               JNIEnv* env,
                               const QString &internalTypeName,
                               const QMetaType& internalMetaType,
                               jclass externalClass,
                               bool allowValuePointers){
    hash_type key = computeHash(env, internalTypeName, internalMetaType, externalClass, allowValuePointers);
    {
        QReadLocker locker(gCacheLock());
        Q_UNUSED(locker)
        if(gInternalToExternalConverters()->contains(key))
            return (*gInternalToExternalConverters())[key];
    }
    InternalToExternalConverter converter = getInternalToExternalConverterImpl(
                env,
                internalTypeName,
                internalMetaType,
                externalClass,
                allowValuePointers
            );
    Q_ASSERT(converter);
    QWriteLocker locker(gCacheLock());
    Q_UNUSED(locker)
    if(gInternalToExternalConverters()->contains(key))
        return (*gInternalToExternalConverters())[key];
    (*gInternalToExternalConverters())[key] = converter;
    return converter;
}

InternalToExternalConverter QtJambiTypeManager::getInternalToExternalConverterImpl(
                               JNIEnv* _env,
                               QString internalTypeName,
                               const QMetaType& internalMetaType,
                               jclass externalClass,
                               bool allowValuePointers){
    PointerType pointerType = PointerType::NoPointer;
    internalTypeName = processInternalTypeName(internalTypeName, pointerType);

    if (internalMetaType.id() == QMetaType::QVariant) {
        return [](JNIEnv* env, QtJambiScope*, const void* in, jvalue& p, bool)->bool{
            p.l = QtJambiAPI::convertQVariantToJavaObject(env, *reinterpret_cast<const QVariant*>(in));
            return true;
        };
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
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    }else if(JObjectValueWrapper::isValueType(internalMetaType)){
        return [](JNIEnv* env, QtJambiScope*, const void* in, jvalue& p, bool)->bool{
            p.l = reinterpret_cast<const JObjectValueWrapper*>(in)->value(env);
            return true;
        };
    }else if(isJObjectWrappedMetaType(internalMetaType)){
        return [](JNIEnv* env, QtJambiScope*, const void* in, jvalue& p, bool)->bool{
            p.l = env->NewLocalRef(reinterpret_cast<const JObjectWrapper*>(in)->object());
            return true;
        };
#endif
    }
    bool isArrayClass = externalClass ? Java::Runtime::Class::isArray(_env, externalClass) : false;

    if(isArrayClass && (pointerType==PointerType::initializer_list || internalTypeName.startsWith("initializer_list<"))){
        jclass componentClass = Java::Runtime::Class::getComponentType(_env, externalClass);
        QString javaTypeName = QtJambiAPI::getClassName(_env, jclass(componentClass)).replace(".", "/");
        QString _internalTypeName = internalTypeName;
        if(_internalTypeName.startsWith("initializer_list<"))
            _internalTypeName = _internalTypeName.mid(22, _internalTypeName.length() - 23);
        if(const std::type_info* typeId = getTypeByQtName(_internalTypeName)){
            int metaType = QMetaType::type(qPrintable(_internalTypeName));
            if(metaType==QMetaType::UnknownType){
                metaType = registeredMetaTypeID(*typeId);
            }
            size_t size = getValueSize(*typeId);
            if(size>0){
                QMetaType _metaType(metaType);
                InternalToExternalConverter internalToExternalComponentConverter = getInternalToExternalConverter(
                                                                _env,
                                                                _internalTypeName,
                                                                _metaType,
                                                                componentClass,
                                                                true);
                if(Java::Runtime::Byte::isPrimitiveType(_env,componentClass)){
                    return [internalToExternalComponentConverter, size](JNIEnv* env, QtJambiScope* scope, const void* in, jvalue& p, bool forceBoxedType)->bool{
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
                    return [internalToExternalComponentConverter, size](JNIEnv* env, QtJambiScope* scope, const void* in, jvalue& p, bool forceBoxedType)->bool{
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
                    return [internalToExternalComponentConverter, size](JNIEnv* env, QtJambiScope* scope, const void* in, jvalue& p, bool forceBoxedType)->bool{
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
                    return [internalToExternalComponentConverter, size](JNIEnv* env, QtJambiScope* scope, const void* in, jvalue& p, bool forceBoxedType)->bool{
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
                    return [internalToExternalComponentConverter, size](JNIEnv* env, QtJambiScope* scope, const void* in, jvalue& p, bool forceBoxedType)->bool{
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
                    return [internalToExternalComponentConverter, size](JNIEnv* env, QtJambiScope* scope, const void* in, jvalue& p, bool forceBoxedType)->bool{
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
                    return [internalToExternalComponentConverter, size](JNIEnv* env, QtJambiScope* scope, const void* in, jvalue& p, bool forceBoxedType)->bool{
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
                    return [internalToExternalComponentConverter, size](JNIEnv* env, QtJambiScope* scope, const void* in, jvalue& p, bool forceBoxedType)->bool{
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
                    return [internalToExternalComponentConverter, size, componentClass](JNIEnv* env, QtJambiScope* scope, const void* in, jvalue& p, bool forceBoxedType)->bool{
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
        return [](JNIEnv* env, QtJambiScope*, const void* in, jvalue& p, bool forceBoxedType)->bool{
            memcpy(&p.i, in, 4);
            if(forceBoxedType){
                p.l = QtJambiAPI::toJavaIntegerObject(env, p.i);
            }
            return true;
        };
    }else if(Java::Runtime::Long::isPrimitiveType(_env,externalClass) || Java::Runtime::Long::isSameClass(_env,externalClass)){
        return [](JNIEnv* env, QtJambiScope*, const void* in, jvalue& p, bool forceBoxedType)->bool{
            memcpy(&p.j, in, 8);
            if(forceBoxedType){
                p.l = QtJambiAPI::toJavaLongObject(env, p.j);
            }
            return true;
        };
    }else if(Java::Runtime::Short::isPrimitiveType(_env,externalClass) || Java::Runtime::Short::isSameClass(_env,externalClass)){
        return [](JNIEnv* env, QtJambiScope*, const void* in, jvalue& p, bool forceBoxedType)->bool{
            memcpy(&p.s, in, 2);
            if(forceBoxedType){
                p.l = QtJambiAPI::toJavaShortObject(env, p.s);
            }
            return true;
        };
    }else if(Java::Runtime::Byte::isPrimitiveType(_env,externalClass) || Java::Runtime::Byte::isSameClass(_env,externalClass)){
        return [](JNIEnv* env, QtJambiScope*, const void* in, jvalue& p, bool forceBoxedType)->bool{
            memcpy(&p.b, in, 1);
            if(forceBoxedType){
                p.l = QtJambiAPI::toJavaByteObject(env, p.b);
            }
            return true;
        };
    }else if(Java::Runtime::Boolean::isPrimitiveType(_env,externalClass) || Java::Runtime::Boolean::isSameClass(_env,externalClass)){
        return [](JNIEnv* env, QtJambiScope*, const void* in, jvalue& p, bool forceBoxedType)->bool{
            memcpy(&p.z, in, 1);
            if(forceBoxedType){
                p.l = QtJambiAPI::toJavaBooleanObject(env, p.z);
            }
            return true;
        };
    }else if(Java::Runtime::Character::isPrimitiveType(_env,externalClass) || Java::Runtime::Character::isSameClass(_env,externalClass)){
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
    }else if(Java::Runtime::Float::isPrimitiveType(_env,externalClass) || Java::Runtime::Float::isSameClass(_env,externalClass)){
        return [](JNIEnv* env, QtJambiScope*, const void* in, jvalue& p, bool forceBoxedType)->bool{
            memcpy(&p.f, in, 4);
            if(forceBoxedType){
                p.l = QtJambiAPI::toJavaFloatObject(env, p.f);
            }
            return true;
        };
    }else if(Java::Runtime::Double::isPrimitiveType(_env,externalClass) || Java::Runtime::Double::isSameClass(_env,externalClass)){
        return [](JNIEnv* env, QtJambiScope*, const void* in, jvalue& p, bool forceBoxedType)->bool{
            memcpy(&p.d, in, 8);
            if(forceBoxedType){
                p.l = QtJambiAPI::toJavaDoubleObject(env, p.d);
            }
            return true;
        };
    }else if(Java::Runtime::String::isSameClass(_env,externalClass)){
        return [](JNIEnv* env, QtJambiScope*, const void* in, jvalue& p, bool)->bool{
            const QString *strp = reinterpret_cast<const QString *>(in);
            p.l = qtjambi_cast<jstring>(env, *strp);
            return true;
        };
    }else if(Java::Runtime::CharSequence::isSameClass(_env,externalClass) || Java::QtCore::QString::isSameClass(_env,externalClass)){
        return [](JNIEnv* env, QtJambiScope*, const void* in, jvalue& p, bool)->bool{
            const QString *strp = reinterpret_cast<const QString *>(in);
            p.l = QtJambiAPI::convertQStringToJavaObject(env, *strp);
            return true;
        };
    }else if(Java::QtJambi::QNativePointer::isSameClass(_env,externalClass)){
        return [](JNIEnv* env, QtJambiScope*, const void* in, jvalue& p, bool)->bool{
            const void * const*in_p = reinterpret_cast<const void * const*>(in);
            p.l = QtJambiAPI::convertNativeToQNativePointer(env, *in_p, QNativePointer::Type::Pointer, 1);
            return true;
        };
    } else if (Java::QtJambi::QFlags::isAssignableFrom(_env,externalClass)) {
        if (internalMetaType.id() == registeredMetaTypeID(typeid(JObjectWrapper))) {
            return [](JNIEnv* env, QtJambiScope*, const void* in, jvalue& p, bool)->bool{
                p.l = env->NewLocalRef(reinterpret_cast<const JObjectWrapper *>(in)->object());
                return true;
            };
        }else if (Java::QtCore::QMetaType$GenericFlags::isSameClass(_env,externalClass)) {
            jint _internalMetaType = internalMetaType.id();
            return [_internalMetaType](JNIEnv* env, QtJambiScope*, const void* in, jvalue& p, bool)->bool{
                jint value = *reinterpret_cast<const qint32*>(in);
                p.l = Java::QtCore::QMetaType$GenericFlags::newInstance(env, _internalMetaType, value);
                return true;
            };
        }else{
            externalClass = getGlobalClassRef(_env, externalClass);
            return [externalClass](JNIEnv* env, QtJambiScope*, const void* in, jvalue& p, bool)->bool{
                p.l = QtJambiAPI::convertQFlagsToJavaObject(env, *reinterpret_cast<const int *>(in), externalClass);
                return p.l!=nullptr;
            };
        }
    } else if(Java::Runtime::Enum::isAssignableFrom(_env,externalClass)
              || Java::QtJambi::QtEnumerator::isAssignableFrom(_env,externalClass)
              || Java::QtJambi::QtLongEnumerator::isAssignableFrom(_env,externalClass)
              || Java::QtJambi::QtShortEnumerator::isAssignableFrom(_env,externalClass)
              || Java::QtJambi::QtByteEnumerator::isAssignableFrom(_env,externalClass)){
        if (internalMetaType.id() == registeredMetaTypeID(typeid(JObjectWrapper)) || internalMetaType.id() == registeredMetaTypeID(typeid(JEnumWrapper))) {
            return [](JNIEnv* env, QtJambiScope*, const void* in, jvalue& p, bool)->bool{
                p.l = env->NewLocalRef(reinterpret_cast<const JObjectWrapper *>(in)->object());
                return true;
            };
        }else if (Java::QtCore::QMetaType$GenericEnumerator::isSameClass(_env,externalClass)) {
            jint _internalMetaType = internalMetaType.id();
            return [_internalMetaType](JNIEnv* env, QtJambiScope*, const void* in, jvalue& p, bool)->bool{
                jint index = -1;
                jint value = *reinterpret_cast<const int*>(in);
                jstring name = nullptr;
                QMetaEnum enm = gMetaEnumByMetaTypeHash->value(_internalMetaType);
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
                p.l = Java::QtCore::QMetaType$GenericEnumerator::newInstance(env, _internalMetaType, index, value, name);
                return true;
            };
        } else if (Java::QtCore::QMetaType$GenericShortEnumerator::isSameClass(_env,externalClass)) {
            jint _internalMetaType = internalMetaType.id();
            return [_internalMetaType](JNIEnv* env, QtJambiScope*, const void* in, jvalue& p, bool)->bool{
                jint index = -1;
                jshort value = *reinterpret_cast<const qint16*>(in);
                jstring name = nullptr;
                QMetaEnum enm = gMetaEnumByMetaTypeHash->value(_internalMetaType);
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
                p.l = Java::QtCore::QMetaType$GenericShortEnumerator::newInstance(env, _internalMetaType, index, value, name);
                return true;
            };
        } else if (Java::QtCore::QMetaType$GenericByteEnumerator::isSameClass(_env,externalClass)) {
            jint _internalMetaType = internalMetaType.id();
            return [_internalMetaType](JNIEnv* env, QtJambiScope*, const void* in, jvalue& p, bool)->bool{
                jint index = -1;
                jbyte value = *reinterpret_cast<const qint8*>(in);
                jstring name = nullptr;
                QMetaEnum enm = gMetaEnumByMetaTypeHash->value(_internalMetaType);
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
                p.l = Java::QtCore::QMetaType$GenericByteEnumerator::newInstance(env, _internalMetaType, index, value, name);
                return true;
            };
        } else if (Java::QtCore::QMetaType$GenericLongEnumerator::isSameClass(_env,externalClass)) {
            jint _internalMetaType = internalMetaType.id();
            return [_internalMetaType](JNIEnv* env, QtJambiScope*, const void* in, jvalue& p, bool)->bool{
                jint index = -1;
                jlong value = *reinterpret_cast<const qint64*>(in);
                jstring name = nullptr;
                QMetaEnum enm = gMetaEnumByMetaTypeHash->value(_internalMetaType);
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
                p.l = Java::QtCore::QMetaType$GenericLongEnumerator::newInstance(env, _internalMetaType, index, value, name);
                return true;
            };
        }else{
            JObjectWrapper external_class(_env, externalClass);
            if (Java::QtJambi::QtEnumerator::isAssignableFrom(_env,externalClass)) {
                return [external_class](JNIEnv* env, QtJambiScope*, const void* in, jvalue& p, bool)->bool{
                    p.l = QtJambiAPI::convertEnumToJavaObject(env, *reinterpret_cast<const qint32 *>(in), jclass(external_class.object()));
                    return p.l!=nullptr;
                };
            }else if (Java::QtJambi::QtShortEnumerator::isAssignableFrom(_env,externalClass)) {
                return [external_class](JNIEnv* env, QtJambiScope*, const void* in, jvalue& p, bool)->bool{
                    p.l = QtJambiAPI::convertEnumToJavaObject(env, *reinterpret_cast<const qint16 *>(in), jclass(external_class.object()));
                    return p.l!=nullptr;
                };
            }else if (Java::QtJambi::QtByteEnumerator::isAssignableFrom(_env,externalClass)) {
                return [external_class](JNIEnv* env, QtJambiScope*, const void* in, jvalue& p, bool)->bool{
                    p.l = QtJambiAPI::convertEnumToJavaObject(env, *reinterpret_cast<const qint8 *>(in), jclass(external_class.object()));
                    return p.l!=nullptr;
                };
            }else if (Java::QtJambi::QtLongEnumerator::isAssignableFrom(_env,externalClass)) {
                return [external_class](JNIEnv* env, QtJambiScope*, const void* in, jvalue& p, bool)->bool{
                    p.l = QtJambiAPI::convertEnumToJavaObject(env, *reinterpret_cast<const qint64 *>(in), jclass(external_class.object()));
                    return p.l!=nullptr;
                };
            }else /*if (Java::Runtime::Enum::isAssignableFrom(_env,externalClass))*/ {
                return [external_class](JNIEnv* env, QtJambiScope*, const void* in, jvalue& p, bool)->bool{
                    jobjectArray enumConstants = Java::Runtime::Class::getEnumConstants(env, external_class.object());
                    p.l = env->GetObjectArrayElement(enumConstants, *reinterpret_cast<const qint32 *>(in));
                    return p.l!=nullptr;
                };
            }
        }
    } else if (Java::QtCore::QMetaType$GenericValue::isSameClass(_env,externalClass)) {
        jint _internalMetaType = internalMetaType.id();
        return [_internalMetaType](JNIEnv* env, QtJambiScope*, const void* in, jvalue& p, bool)->bool{
            p.l = QtJambiAPI::convertNativeToJavaOwnedObjectAsWrapper(env, QMetaType::create(_internalMetaType, in), Java::QtCore::QMetaType$GenericValue::getClass(env));
            if(p.l){
                Java::QtCore::QMetaType$GenericValue::set_type(env, p.l, _internalMetaType);
            }
            return true;
        };
    } else if (Java::QtCore::QMetaType$GenericObject::isSameClass(_env,externalClass)) {
        jint _internalMetaType = internalMetaType.id();
        return [_internalMetaType](JNIEnv* env, QtJambiScope* scope, const void* in, jvalue& p, bool)->bool{
            p.l = QtJambiAPI::convertNativeToJavaObjectAsWrapper(env, in, Java::QtCore::QMetaType$GenericObject::getClass(env));
            if(p.l){
                Java::QtCore::QMetaType$GenericObject::set_type(env, p.l, _internalMetaType);
                if(scope)
                    scope->addObjectInvalidation(env, p.l, true, true);
            }
            return true;
        };
    } else if (Java::QtCore::QMetaType$GenericGadget::isSameClass(_env,externalClass)) {
        jint _internalMetaType = internalMetaType.id();
        if(internalMetaType.flags() & QMetaType::PointerToGadget){
            return [_internalMetaType](JNIEnv* env, QtJambiScope* scope, const void* in, jvalue& p, bool)->bool{
                p.l = QtJambiAPI::convertNativeToJavaObjectAsWrapper(env, in, Java::QtCore::QMetaType$GenericGadget::getClass(env));
                if(p.l){
                    Java::QtCore::QMetaType$GenericGadget::set_type(env, p.l, _internalMetaType);
                    jobject mo = qtjambi_cast<jobject>(env, QMetaType::metaObjectForType(_internalMetaType));
                    Java::QtJambi::QtGadget::set_staticMetaObject(env, p.l, mo);
                    if(scope)
                        scope->addObjectInvalidation(env, p.l, true, true);
                }
                return true;
            };
        }else{
            return [_internalMetaType](JNIEnv* env, QtJambiScope*, const void* in, jvalue& p, bool)->bool{
                p.l = QtJambiAPI::convertNativeToJavaObjectAsWrapper(env, QMetaType::create(_internalMetaType, in), Java::QtCore::QMetaType$GenericGadget::getClass(env));
                if(p.l){
                    Java::QtCore::QMetaType$GenericGadget::set_type(env, p.l, _internalMetaType);
                    jobject mo = qtjambi_cast<jobject>(env, QMetaType::metaObjectForType(_internalMetaType));
                    Java::QtJambi::QtGadget::set_staticMetaObject(env, p.l, mo);
                }
                return true;
            };
        }
    }else if (Java::QtCore::QMetaObject::isAssignableFrom(_env,externalClass)) {
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
        return [](JNIEnv* env, QtJambiScope*, const void* in, jvalue& p, bool)->bool{
            if(in){
                p.l = qtjambi_cast<jobject>(env, *reinterpret_cast<const QMetaObject::Connection*>(in));
            }
            return true;
        };
    }else if((internalMetaType.id()==QMetaType::QStringList
              || internalTypeName=="QStringList")
             &&
             ( Java::QtCore::QList::isAssignableFrom(_env,externalClass)
               || Java::Runtime::List::isSameClass(_env,externalClass)
               || Java::Runtime::Collection::isSameClass(_env,externalClass)
               || Java::Runtime::Object::isSameClass(_env,externalClass))
             ){
        return [](JNIEnv* env, QtJambiScope*, const void* in, jvalue& p, bool)->bool{
            if(in){
                p.l = qtjambi_cast<jobject>(env, *reinterpret_cast<const QStringList*>(in));
            }
            return true;
        };
    }else if((internalMetaType.id()==QMetaType::QByteArrayList
              || internalTypeName=="QByteArrayList")
             &&
             ( Java::QtCore::QList::isAssignableFrom(_env,externalClass)
               || Java::Runtime::List::isSameClass(_env,externalClass)
               || Java::Runtime::Collection::isSameClass(_env,externalClass)
               || Java::Runtime::Object::isSameClass(_env,externalClass))
             ){
        return [](JNIEnv* env, QtJambiScope*, const void* in, jvalue& p, bool)->bool{
            if(in){
                p.l = qtjambi_cast<jobject>(env, *reinterpret_cast<const QByteArrayList*>(in));
            }
            return true;
        };
    }else if((internalMetaType.id()==QMetaType::QVariantList
              || internalTypeName=="QVariantList")
             &&
             ( Java::QtCore::QList::isAssignableFrom(_env,externalClass)
               || Java::Runtime::List::isSameClass(_env,externalClass)
               || Java::Runtime::Collection::isSameClass(_env,externalClass)
               || Java::Runtime::Object::isSameClass(_env,externalClass))
             ){
        return [](JNIEnv* env, QtJambiScope*, const void* in, jvalue& p, bool)->bool{
            if(in){
                p.l = qtjambi_cast<jobject>(env, *reinterpret_cast<const QVariantList*>(in));
            }
            return true;
        };
    }else if( ( (
                    internalTypeName.startsWith("QList<")
                    &&
                    ( Java::QtCore::QList::isAssignableFrom(_env,externalClass)
                      || Java::Runtime::List::isSameClass(_env,externalClass)
                      || Java::Runtime::Collection::isSameClass(_env,externalClass)
                      || Java::Runtime::Object::isSameClass(_env,externalClass))
                )
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
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
                     || Java::QtCore::QVector::isSameClass(_env,externalClass)
#else
                     || Java::QtCore::QList::isSameClass(_env,externalClass)
#endif
                     || Java::Runtime::Deque::isSameClass(_env,externalClass)
                     || Java::Runtime::Collection::isSameClass(_env,externalClass)
                     || Java::Runtime::Object::isSameClass(_env,externalClass))
                )
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
                ||
                (
                    internalTypeName.startsWith("QVector<")
                    &&
                    ( Java::QtCore::QVector::isAssignableFrom(_env,externalClass)
                      || Java::Runtime::List::isSameClass(_env,externalClass)
                      || Java::Runtime::Collection::isSameClass(_env,externalClass)
                      || Java::Runtime::Object::isSameClass(_env,externalClass))
                )
                ||
                (
                    internalTypeName.startsWith("QLinkedList<")
                    &&
                    ( Java::QtCore::QLinkedList::isAssignableFrom(_env,externalClass)
                     || Java::Runtime::Collection::isSameClass(_env,externalClass)
                     || Java::Runtime::Object::isSameClass(_env,externalClass))
                )
#endif
                ||
                (
                    internalTypeName.startsWith("QSet<")
                    &&
                    ( Java::QtCore::QSet::isAssignableFrom(_env,externalClass)
                     || Java::Runtime::Set::isSameClass(_env,externalClass)
                     || Java::Runtime::Collection::isSameClass(_env,externalClass)
                     || Java::Runtime::Object::isSameClass(_env,externalClass))
                )
             ) && internalTypeName.endsWith(">")){
        auto idx = internalTypeName.indexOf("<");
        SequentialContainerType containerType;
        if(internalTypeName.startsWith("QStack<")){
            containerType = SequentialContainerType::QStack;
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
        }else if(internalTypeName.startsWith("QVector<")){
            containerType = SequentialContainerType::QVector;
        }else if(internalTypeName.startsWith("QLinkedList<")){
            containerType = SequentialContainerType::QLinkedList;
#endif
        }else if(internalTypeName.startsWith("QQueue<")){
            containerType = SequentialContainerType::QQueue;
        }else if(internalTypeName.startsWith("QSet<")){
            containerType = SequentialContainerType::QSet;
        }else{
            containerType = SequentialContainerType::QList;
        }
        QString container = internalTypeName.mid(0, idx);
        QString instantiation = internalTypeName.mid(idx+1).chopped(1).trimmed();
        QByteArray clazz;
        QtJambiMetaType memberMetaType(QMetaType::UnknownType);
        bool isPointer = isPointerType(instantiation);
        size_t size = 0;
        size_t align = 0;
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
        bool isStaticType = false;
#endif
        const std::type_info* t;
        if(isPointer){
            t = getTypeByQtName(instantiation.chopped(1));
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
            memberMetaType = QMetaType::type(qPrintable(instantiation));
#else
            memberMetaType = QMetaType::fromName(qPrintable(instantiation));
#endif
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
                memberMetaType = QMetaType(registeredMetaTypeID(*t));
                if(memberMetaType.isValid()){
                    auto sz = memberMetaType.sizeOf();
                    size = sz<0 ? 0 : size_t(sz);
                }else{
                    size = getValueSize(*t);
                }
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
                align = getValueAlignment(*t);
                if(OptionalBool op = isRegisteredAsStaticType(*t))
                    isStaticType = op.value();
                if(isFunctional(*t)){
                    QString typeName = QtJambiAPI::typeName(*t);
                    if(!typeName.startsWith("std::function") && (typeName.contains("(*)"))){
                        isPointer = true;
                    }
                }
                if(align==0 && !isPointer)
                    align = QtJambiTypeManager::getInternalAlignment(instantiation);
#else
                if(memberMetaType.isValid()){
                    auto al = memberMetaType.alignOf();
                    align = size_t(al);
                }else{
                    align = getValueAlignment(*t);
                }
#endif
            }else{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
                memberMetaType = QMetaType::type(qPrintable(instantiation));
                isStaticType = QtJambiTypeManager::isStaticType(instantiation);
#else
                memberMetaType = QMetaType::fromName(qPrintable(instantiation));
#endif
                if(memberMetaType.isValid()){
                    auto sz = memberMetaType.sizeOf();
                    size = sz<0 ? 0 : size_t(sz);
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
                    if(memberMetaType.alignOf()!=0)
                        align = size_t(memberMetaType.alignOf());
#endif
                }
                if(size==0){
                    size = getInternalSize(instantiation);
                }
                if(align==0){
                    align = getInternalAlignment(instantiation);
                }
                if(!memberMetaType.isValid()){
                    clazz = getExternalTypeName(_env, instantiation, memberMetaType).toUtf8();
                }else{
                    clazz = getExternalTypeName(_env, memberMetaType.name(), memberMetaType).toUtf8();
                }
            }
        }

        if(!isPointer && !memberMetaType.isValid()){
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
            memberMetaType = QMetaType::type(qPrintable(instantiation));
            isStaticType = QtJambiTypeManager::isStaticType(instantiation);
#else
            memberMetaType = QMetaType::fromName(qPrintable(instantiation));
#endif
            if(memberMetaType.isValid()){
                auto sz = memberMetaType.sizeOf();
                size = sz<0 ? 0 : size_t(sz);
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
                if(memberMetaType.alignOf()!=0)
                    align = size_t(memberMetaType.alignOf());
#endif
            }
            if(size==0){
                size = getInternalSize(instantiation);
            }
            if(align==0){
                align = getInternalAlignment(instantiation);
            }
        }
        AbstractContainerAccess* containerAccess = AbstractContainerAccess::create(_env, containerType, memberMetaType);

        if(!clazz.isEmpty()){
            jclass elementClass = JavaAPI::resolveClass(_env, clazz);
            if(!containerAccess){
                InternalToExternalConverter memberConverter = getInternalToExternalConverter(
                                                                _env,
                                                                instantiation,
                                                                memberMetaType,
                                                                elementClass);
                ExternalToInternalConverter memberReConverter = getExternalToInternalConverter(
                                                                _env,
                                                                elementClass,
                                                                instantiation, memberMetaType);
                if(!isPointer && !memberMetaType.isValid()){
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
                    memberMetaType = QMetaType::type(qPrintable(instantiation));
                    align = getInternalAlignment(instantiation);
#else
                    memberMetaType = QMetaType::fromName(qPrintable(instantiation));
                    align = memberMetaType.alignOf();
#endif
                    size = memberMetaType.sizeOf();
                    containerAccess = AbstractContainerAccess::create(_env, containerType, memberMetaType);
                }

                if(!containerAccess && (size==0 || memberMetaType.isValid())){
                    QHashFunction hashFunction = QtJambiTypeManager::findHashFunction(isPointer, memberMetaType.id());
                    if(containerType == SequentialContainerType::QSet
                            && !hashFunction){
                        if(clazz.startsWith("io/qt/core/QMetaType$Generic")){
                            hashFunction = [](const void*, hash_type seed) -> hash_type{ return seed; };
                        }else{
                            Java::Runtime::IllegalArgumentException::throwNew(_env, QStringLiteral("Unable to use %1 in QSet.").arg(QLatin1String(clazz.replace('/', '.').replace('$', '.'))) QTJAMBI_STACKTRACEINFO );
                        }
                    }
                    containerAccess = AbstractContainerAccess::create(_env, containerType, memberMetaType,
                                                                      align, size,
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
                                                                      isStaticType,
#endif
                                                                      isPointer,
                                                                      hashFunction,
                                                                      memberConverter,
                                                                      memberReConverter);
                }
            }
            if(containerAccess){
                switch(containerType){
                case SequentialContainerType::QStack:
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
                case SequentialContainerType::QVector:
                    containerAccess = checkContainerAccess(_env, dynamic_cast<AbstractVectorAccess*>(containerAccess));
                    break;
                case SequentialContainerType::QLinkedList:
                    containerAccess = checkContainerAccess(_env, dynamic_cast<AbstractLinkedListAccess*>(containerAccess));
                    break;
#endif
                case SequentialContainerType::QQueue:
                case SequentialContainerType::QList:
                    containerAccess = checkContainerAccess(_env, dynamic_cast<AbstractListAccess*>(containerAccess));
                    break;
                case SequentialContainerType::QSet:
                    containerAccess = checkContainerAccess(_env, dynamic_cast<AbstractSetAccess*>(containerAccess));
                    break;
                }
                QSharedPointer<AbstractContainerAccess> sharedAccess(containerAccess, &containerDisposer);
                int containerMetaType = containerAccess->registerContainer(internalTypeName.toLatin1());
                return [containerMetaType, containerType, sharedAccess]
                        (JNIEnv* env, QtJambiScope*, const void* in, jvalue& p, bool)->bool{
                    if(in){
                        AbstractContainerAccess* access = sharedAccess->clone();
                        switch(containerType){
                        case SequentialContainerType::QStack:
                            p.l = Java::QtCore::QStack::newInstance(env, nullptr);
                            break;
                        case SequentialContainerType::QQueue:
                            p.l = Java::QtCore::QQueue::newInstance(env, nullptr);
                            break;
                        case SequentialContainerType::QSet:
                            p.l = Java::QtCore::QSet::newInstance(env, nullptr);
                            break;
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
                        case SequentialContainerType::QLinkedList:
                            p.l = Java::QtCore::QLinkedList::newInstance(env, nullptr);
                            break;
                        case SequentialContainerType::QVector:
                            p.l = Java::QtCore::QVector::newInstance(env, nullptr);
                            break;
#endif
                        default:
                            p.l = Java::QtCore::QList::newInstance(env, nullptr);
                            break;
                        }
                        void* ptr = access->createContainer(in);
                        QMetaType _containerMetaType(containerMetaType);
                        QSharedPointer<QtJambiLink> link = QtJambiLink::createLinkForContainer(env, p.l, ptr, _containerMetaType,
                                                                                            false, true, access);
                        if (!link) {
                            p.l = nullptr;
                            if(access)
                                access->dispose();
                        }
                        return !!link;
                    }
                    return true;
                };
            }
        }
        return [](JNIEnv*, QtJambiScope*, const void*, jvalue&, bool)->bool{
            return false;
        };
    }else if((internalMetaType.id()==QMetaType::QVariantMap
              || internalTypeName=="QVariantMap")
             &&
             ( Java::QtCore::QMap::isAssignableFrom(_env,externalClass)
               || Java::Runtime::NavigableMap::isSameClass(_env,externalClass)
               || Java::Runtime::Map::isSameClass(_env,externalClass)
               || Java::Runtime::Object::isSameClass(_env,externalClass))
             ){
        return [](JNIEnv* env, QtJambiScope*, const void* in, jvalue& p, bool)->bool{
            if(in){
                p.l = qtjambi_cast<jobject>(env, *reinterpret_cast<const QVariantMap*>(in));
            }
            return true;
        };
    }else if((internalMetaType.id()==QMetaType::QVariantHash
              || internalTypeName=="QVariantHash")
             &&
             ( Java::QtCore::QHash::isAssignableFrom(_env,externalClass)
               || Java::Runtime::Map::isSameClass(_env,externalClass)
               || Java::Runtime::Object::isSameClass(_env,externalClass))
             ){
        return [](JNIEnv* env, QtJambiScope*, const void* in, jvalue& p, bool)->bool{
            if(in){
                p.l = qtjambi_cast<jobject>(env, *reinterpret_cast<const QVariantHash*>(in));
            }
            return true;
        };
#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
    }else if((internalMetaType.id()==QMetaType::QVariantPair
              || internalTypeName=="QVariantPair")
             &&
             ( Java::QtCore::QPair::isSameClass(_env,externalClass)
               || Java::Runtime::Object::isSameClass(_env,externalClass))
             ){
        return []
                (JNIEnv* env, QtJambiScope*, const void* in, jvalue& p, bool)->bool{
            if(in){
                p.l = qtjambi_cast<jobject>(env, *reinterpret_cast<const QVariantPair*>(in));
            }
            return true;
        };
#endif
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
             ) && internalTypeName.endsWith(">")){
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
            size_t size1 = 0;
            size_t align1 = 0;
            QtJambiMetaType memberMetaType1(QMetaType::UnknownType);
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
                    memberMetaType1 = QMetaType(registeredMetaTypeID(*t1));
                    if(memberMetaType1.isValid()){
                        auto sz = memberMetaType1.sizeOf();
                        size1 = sz<0 ? 0 : size_t(sz);
                    }else{
                        size1 = getValueSize(*t1);
                    }
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
                    align1 = getValueAlignment(*t1);
                    if(isFunctional(*t1)){
                        QString typeName = QtJambiAPI::typeName(*t1);
                        if(!typeName.startsWith("std::function") && (typeName.contains("(*)"))){
                            isPointer1 = true;
                        }
                    }
                    if(align1==0 && !isPointer1)
                        align1 = QtJambiTypeManager::getInternalAlignment(instantiations[0]);
#else
                    if(memberMetaType1.isValid()){
                        auto al = memberMetaType1.alignOf();
                        align1 = size_t(al);
                    }else{
                        align1 = getValueAlignment(*t1);
                    }
#endif
                }
                if(!memberMetaType1.isValid()){
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
                    memberMetaType1 = QMetaType::type(qPrintable(instantiations[0]));
#else
                    memberMetaType1 = QMetaType::fromName(qPrintable(instantiations[0]));
#endif
                    if(memberMetaType1.isValid()){
                        auto sz = memberMetaType1.sizeOf();
                        size1 = sz<0 ? 0 : size_t(sz);
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
                        if(memberMetaType1.alignOf()!=0)
                            align1 = size_t(memberMetaType1.alignOf());
#endif
                    }
                    if(size1==0){
                        size1 = getInternalSize(instantiations[0]);
                    }
                    if(align1==0){
                        align1 = getInternalAlignment(instantiations[0]);
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
            size_t size2 = 0;
            size_t align2 = 0;
            QtJambiMetaType memberMetaType2(QMetaType::UnknownType);
            if(isPointer2){
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
                memberMetaType2 = QMetaType::type(qPrintable(instantiations[1]));
#else
                memberMetaType2 = QMetaType::fromName(qPrintable(instantiations[1]));
#endif
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
                    size2 = getValueSize(*t2);
                    memberMetaType2 = QMetaType(registeredMetaTypeID(*t2));
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
                    align2 = getValueAlignment(*t2);
                    if(isFunctional(*t2)){
                        QString typeName = QtJambiAPI::typeName(*t2);
                        if(!typeName.startsWith("std::function") && (typeName.contains("(*)"))){
                            isPointer2 = true;
                        }
                    }
                    if(align2==0 && !isPointer2)
                        align2 = QtJambiTypeManager::getInternalAlignment(instantiations[1]);
#else
                    if(memberMetaType2.isValid()){
                        auto al = memberMetaType2.alignOf();
                        align2 = size_t(al);
                    }else{
                        align2 = getValueAlignment(*t2);
                    }
#endif
                }else{
                    memberMetaType2 = QMetaType(QMetaType::type(qPrintable(instantiations[1])));
                    if(memberMetaType2.isValid()){
                        auto sz = memberMetaType2.sizeOf();
                        size2 = sz<0 ? 0 : size_t(sz);
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
                        if(memberMetaType2.alignOf()!=0)
                            align2 = size_t(memberMetaType2.alignOf());
#endif
                    }
                    if(size2==0){
                        size2 = getInternalSize(instantiations[1]);
                    }
                    if(align2==0){
                        align2 = getInternalAlignment(instantiations[1]);
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
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
                    memberMetaType1 = QMetaType::type(qPrintable(instantiations[0]));
#else
                    memberMetaType1 = QMetaType::fromName(qPrintable(instantiations[0]));
#endif
                    if(memberMetaType1.isValid()){
                        auto sz = memberMetaType1.sizeOf();
                        size1 = sz<0 ? 0 : size_t(sz);
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
                        if(memberMetaType1.alignOf()!=0)
                            align1 = size_t(memberMetaType1.alignOf());
#endif
                    }
                    if(size1==0){
                        size1 = getInternalSize(instantiations[0]);
                    }
                    if(align1==0){
                        align1 = getInternalAlignment(instantiations[0]);
                    }
                }
                jclass valueClass = JavaAPI::resolveClass(_env, clazz2);
                if(!isPointer2 && !memberMetaType2.isValid()){
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
                    memberMetaType2 = QMetaType::type(qPrintable(instantiations[1]));
#else
                    memberMetaType2 = QMetaType::fromName(qPrintable(instantiations[1]));
#endif
                    if(memberMetaType2.isValid()){
                        auto sz = memberMetaType2.sizeOf();
                        size2 = sz<0 ? 0 : size_t(sz);
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
                        if(memberMetaType2.alignOf()!=0)
                            align2 = size_t(memberMetaType2.alignOf());
#endif
                    }
                    if(size2==0){
                        size2 = getInternalSize(instantiations[1]);
                    }
                    if(align2==0){
                        align2 = getInternalAlignment(instantiations[1]);
                    }
                }
                AbstractContainerAccess* containerAccess = AbstractContainerAccess::create(_env, mapType, memberMetaType1, memberMetaType2);
                if(!containerAccess){
                    InternalToExternalConverter memberConverter1 = getInternalToExternalConverter(
                                                                    _env,
                                                                    instantiations[0],
                                                                    memberMetaType1,
                                                                    keyClass);
                    ExternalToInternalConverter memberReConverter1 = getExternalToInternalConverter(
                                                                    _env,
                                                                    keyClass,
                                                                    instantiations[0], memberMetaType1);
                    InternalToExternalConverter memberConverter2 = getInternalToExternalConverter(
                                                                    _env,
                                                                    instantiations[1],
                                                                    memberMetaType2,
                                                                    valueClass);
                    ExternalToInternalConverter memberReConverter2 = getExternalToInternalConverter(
                                                                    _env,
                                                                    valueClass,
                                                                    instantiations[1], memberMetaType2);
                    bool tryAgain = false;
                    if(!isPointer1 && !memberMetaType1.isValid()){
                        tryAgain = true;
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
                        memberMetaType1 = QMetaType::type(qPrintable(instantiations[0]));
                        align1 = getInternalAlignment(instantiations[1]);
#else
                        memberMetaType1 = QMetaType::fromName(qPrintable(instantiations[0]));
                        align1 = memberMetaType1.alignOf();
#endif
                        size1 = memberMetaType1.sizeOf();
                    }
                    if(!isPointer2 && !memberMetaType2.isValid()){
                        tryAgain = true;
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
                        memberMetaType2 = QMetaType::type(qPrintable(instantiations[1]));
                        align2 = getInternalAlignment(instantiations[1]);
#else
                        memberMetaType2 = QMetaType::fromName(qPrintable(instantiations[1]));
                        align2 = memberMetaType2.alignOf();
#endif
                        size2 = memberMetaType2.sizeOf();
                    }
                    if(tryAgain)
                        containerAccess = AbstractContainerAccess::create(_env, mapType, memberMetaType1, memberMetaType2);
                    if(!containerAccess && (size1==0 || memberMetaType1.isValid()) && (size2==0 || memberMetaType2.isValid())){
                        QHashFunction hashFunction1 = findHashFunction(isPointer1, memberMetaType1.id());
                        QHashFunction hashFunction2 = findHashFunction(isPointer2, memberMetaType2.id());
                        if(mapType == AssociativeContainerType::QHash
                                && !hashFunction1){
                            if(clazz1.startsWith("io/qt/core/QMetaType$Generic"))
                                hashFunction1 = [](const void*, hash_type seed) -> hash_type{ return seed; };
                            else
                                Java::Runtime::IllegalArgumentException::throwNew(_env, QStringLiteral("Unable to use %1 as hash key.").arg(QLatin1String(clazz1.replace('/', '.').replace('$', '.'))) QTJAMBI_STACKTRACEINFO );
                        }
                        containerAccess = AbstractContainerAccess::create(_env, mapType,
                                                                          memberMetaType1,
                                                                          align1, size1,
                                                                          isPointer1,
                                                                          hashFunction1,
                                                                          memberConverter1,
                                                                          memberReConverter1,
                                                                          memberMetaType2,
                                                                          align2, size2,
                                                                          isPointer2,
                                                                          hashFunction2,
                                                                          memberConverter2,
                                                                          memberReConverter2);
                    }
                }
                if(containerAccess){
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
                    QSharedPointer<AbstractContainerAccess> sharedAccess(containerAccess, &containerDisposer);
                    int containerMetaType = sharedAccess->registerContainer(internalTypeName.toLatin1());
                    if(mapType == AssociativeContainerType::QPair){
                        QSharedPointer<AbstractPairAccess> pairAccess(sharedAccess.dynamicCast<AbstractPairAccess>());
                        return [pairAccess]
                                (JNIEnv* env, QtJambiScope*, const void* in, jvalue& p, bool)->bool{
                            if(in){
                                jobject first = pairAccess->first(env, in);
                                jobject second = pairAccess->second(env, in);
                                p.l = Java::QtCore::QPair::newInstance(env, first, second);
                            }
                            return true;
                        };
                    }else{
                        return [containerMetaType, mapType, sharedAccess]
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
                                QMetaType _containerMetaType(containerMetaType);
                                QSharedPointer<QtJambiLink> link = QtJambiLink::createLinkForNativeObject(env, p.l, ptr, _containerMetaType,
                                                                                                    false, true, access, QtJambiLink::Ownership::None);
                                if (!link) {
                                    p.l = nullptr;
                                    if(access)
                                        access->dispose();
                                }
                                return !!link;
                            }
                            return true;
                        };
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
            const SuperTypeInfos& superTypeInfos = SuperTypeInfos::fromClass(_env, externalClass);
            if(!superTypeInfos.isEmpty()){
                typeId = &superTypeInfos[0].typeId();
            }
        }
        if(typeId){
            if (Java::QtCore::QObject::isAssignableFrom(_env,externalClass)) {
                if(pointerType == PointerType::SharedPointer){
                    return [typeId](JNIEnv* env, QtJambiScope*, const void* in, jvalue& p, bool)->bool{
                        const QSharedPointer<QObject>* ptr = reinterpret_cast<const QSharedPointer<QObject>*>(in);
                        if(ptr){
                            p.l = internal_convertQObjectSmartPointerToJavaObject(env, *typeId,
                                                                       new QSharedPointer<QObject>(*ptr),
                                                                       &QtJambiPrivate::SmartPointerHelper<QSharedPointer, QObject>::deletePointer,
                                                                       &QtJambiPrivate::SmartPointerHelper<QSharedPointer, void*>::getFromPointer);
                        }
                        return true;
                    };
                }else if(pointerType == PointerType::WeakPointer){
                    return [typeId](JNIEnv* env, QtJambiScope*, const void* in, jvalue& p, bool)->bool{
                        const QWeakPointer<QObject>* ptr = reinterpret_cast<const QWeakPointer<QObject>*>(in);
                        if(ptr){
                            p.l = internal_convertQObjectSmartPointerToJavaObject(env, *typeId,
                                                                       new QSharedPointer<QObject>(*ptr),
                                                                       &QtJambiPrivate::SmartPointerHelper<QSharedPointer, QObject>::deletePointer,
                                                                       &QtJambiPrivate::SmartPointerHelper<QSharedPointer, void*>::getFromPointer);
                        }
                        return true;
                    };
                }else if(pointerType == PointerType::shared_ptr){
                    return [typeId](JNIEnv* env, QtJambiScope*, const void* in, jvalue& p, bool)->bool{
                        const std::shared_ptr<QObject>* ptr = reinterpret_cast<const std::shared_ptr<QObject>*>(in);
                        if(ptr){
                            p.l = internal_convertQObjectSmartPointerToJavaObject(env, *typeId,
                                                                       new std::shared_ptr<QObject>(*ptr),
                                                                       &QtJambiPrivate::SmartPointerHelper<std::shared_ptr, QObject>::deletePointer,
                                                                       &QtJambiPrivate::SmartPointerHelper<std::shared_ptr, void*>::getFromPointer);
                        }
                        return true;
                    };
                }else if(pointerType == PointerType::weak_ptr){
                    return [typeId](JNIEnv* env, QtJambiScope*, const void* in, jvalue& p, bool)->bool{
                        const std::weak_ptr<QObject>* ptr = reinterpret_cast<const std::weak_ptr<QObject>*>(in);
                        if(ptr){
                            p.l = internal_convertQObjectSmartPointerToJavaObject(env, *typeId,
                                                                       new std::shared_ptr<QObject>(*ptr),
                                                                       &QtJambiPrivate::SmartPointerHelper<std::shared_ptr, QObject>::deletePointer,
                                                                       &QtJambiPrivate::SmartPointerHelper<std::shared_ptr, void*>::getFromPointer);
                        }
                        return true;
                    };
                }else if(pointerType == PointerType::ScopedPointer){
                    return [typeId](JNIEnv* env, QtJambiScope* scope, const void* in, jvalue& p, bool)->bool{
                        const QScopedPointer<QObject>* ptr = reinterpret_cast<const QScopedPointer<QObject>*>(in);
                        if(ptr){
                            p.l = QtJambiAPI::convertQObjectToJavaObject(env, ptr->data(), *typeId);
                            if(scope)
                                scope->addObjectInvalidation(env, p.l, false, true);
                        }
                        return true;
                    };
                }else if(pointerType == PointerType::unique_ptr){
                    return [typeId](JNIEnv* env, QtJambiScope* scope, const void* in, jvalue& p, bool)->bool{
                        const std::unique_ptr<QObject>* ptr = reinterpret_cast<const std::unique_ptr<QObject>*>(in);
                        if(ptr){
                            p.l = QtJambiAPI::convertQObjectToJavaObject(env, ptr->get(), *typeId);
                            if(scope)
                                scope->addObjectInvalidation(env, p.l, false, true);
                        }
                        return true;
                    };
                }else{
                    return [typeId](JNIEnv* env, QtJambiScope*, const void* in, jvalue& p, bool)->bool{
                        QObject * const *ptr = reinterpret_cast<QObject * const *>(in);
                        if(ptr){
                            QObject * qobject = *ptr;
                            p.l = QtJambiAPI::convertQObjectToJavaObject(env, qobject, *typeId);
                        }
                        return true;
                    };
                }
            } else if(isInterface(*typeId)){
                if(pointerType == PointerType::SharedPointer){
                    return [typeId](JNIEnv* env, QtJambiScope*, const void* in, jvalue& p, bool)->bool{
                        const QSharedPointer<void*>* ptr = reinterpret_cast<const QSharedPointer<void*>*>(in);
                        if(ptr){
                            p.l = internal_convertSmartPointerToJavaInterface(env,
                                                                      *typeId,
                                                                      new QSharedPointer<void*>(*ptr),
                                                                      &QtJambiPrivate::SmartPointerHelper<QSharedPointer, void*>::deletePointer,
                                                                      &QtJambiPrivate::SmartPointerHelper<QSharedPointer, void*>::getFromPointer);
                        }
                        return true;
                    };
                }else if(pointerType == PointerType::WeakPointer){
                    return [typeId](JNIEnv* env, QtJambiScope*, const void* in, jvalue& p, bool)->bool{
                        const QWeakPointer<void*>* ptr = reinterpret_cast<const QWeakPointer<void*>*>(in);
                        if(ptr){
                            p.l = internal_convertSmartPointerToJavaInterface(env,
                                                                      *typeId,
                                                                      new QSharedPointer<void*>(*ptr),
                                                                      &QtJambiPrivate::SmartPointerHelper<QSharedPointer, void*>::deletePointer,
                                                                      &QtJambiPrivate::SmartPointerHelper<QSharedPointer, void*>::getFromPointer);
                        }
                        return true;
                    };
                }else if(pointerType == PointerType::shared_ptr){
                    return [typeId](JNIEnv* env, QtJambiScope*, const void* in, jvalue& p, bool)->bool{
                        const std::shared_ptr<void*>* ptr = reinterpret_cast<const std::shared_ptr<void*>*>(in);
                        if(ptr){
                            p.l = internal_convertSmartPointerToJavaInterface(env, *typeId,
                                                                       new std::shared_ptr<void*>(*ptr),
                                                                       &QtJambiPrivate::SmartPointerHelper<std::shared_ptr, void*>::deletePointer,
                                                                       &QtJambiPrivate::SmartPointerHelper<std::shared_ptr, void*>::getFromPointer);
                        }
                        return true;
                    };
                }else if(pointerType == PointerType::weak_ptr){
                    return [typeId](JNIEnv* env, QtJambiScope*, const void* in, jvalue& p, bool)->bool{
                        const std::weak_ptr<void*>* ptr = reinterpret_cast<const std::weak_ptr<void*>*>(in);
                        if(ptr){
                            p.l = internal_convertSmartPointerToJavaInterface(env, *typeId,
                                                                       new std::shared_ptr<void*>(*ptr),
                                                                       &QtJambiPrivate::SmartPointerHelper<std::shared_ptr, void*>::deletePointer,
                                                                       &QtJambiPrivate::SmartPointerHelper<std::shared_ptr, void*>::getFromPointer);
                        }
                        return true;
                    };
                }else if(pointerType == PointerType::ScopedPointer){
                    return [typeId](JNIEnv* env, QtJambiScope* scope, const void* in, jvalue& p, bool)->bool{
                        const QScopedPointer<void*>* ptr = reinterpret_cast<const QScopedPointer<void*>*>(in);
                        if(ptr){
                            p.l = QtJambiAPI::convertNativeToJavaObjectAsWrapper(env, ptr->data(), *typeId);
                            if(scope)
                                scope->addObjectInvalidation(env, p.l, false, true);
                        }
                        return true;
                    };
                }else if(pointerType == PointerType::unique_ptr){
                    return [typeId](JNIEnv* env, QtJambiScope* scope, const void* in, jvalue& p, bool)->bool{
                        const std::unique_ptr<void*>* ptr = reinterpret_cast<const std::unique_ptr<void*>*>(in);
                        if(ptr){
                            p.l = QtJambiAPI::convertNativeToJavaObjectAsWrapper(env, ptr->get(), *typeId);
                            if(scope)
                                scope->addObjectInvalidation(env, p.l, false, true);
                        }
                        return true;
                    };
                }else{
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
                                        scope->addObjectInvalidation(env, p.l, true, true);
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
                }
            }else {
                if(pointerType == PointerType::SharedPointer){
                    return [typeId](JNIEnv* env, QtJambiScope*, const void* in, jvalue& p, bool)->bool{
                        const QSharedPointer<void*>* ptr = reinterpret_cast<const QSharedPointer<void*>*>(in);
                        if(ptr){
                            p.l = QtJambiAPI::convertSmartPointerToJavaObject(env,
                                                                      *typeId,
                                                                      new QSharedPointer<void*>(*ptr),
                                                                      &QtJambiPrivate::SmartPointerHelper<QSharedPointer, void*>::deletePointer,
                                                                      &QtJambiPrivate::SmartPointerHelper<QSharedPointer, void*>::getFromPointer);
                        }
                        return true;
                    };
                }else if(pointerType == PointerType::WeakPointer){
                    return [typeId](JNIEnv* env, QtJambiScope*, const void* in, jvalue& p, bool)->bool{
                        const QWeakPointer<void*>* ptr = reinterpret_cast<const QWeakPointer<void*>*>(in);
                        if(ptr){
                            p.l = QtJambiAPI::convertSmartPointerToJavaObject(env,
                                                                      *typeId,
                                                                      new QSharedPointer<void*>(*ptr),
                                                                      &QtJambiPrivate::SmartPointerHelper<QSharedPointer, void*>::deletePointer,
                                                                      &QtJambiPrivate::SmartPointerHelper<QSharedPointer, void*>::getFromPointer);
                        }
                        return true;
                    };
                }else if(pointerType == PointerType::shared_ptr){
                    return [typeId](JNIEnv* env, QtJambiScope*, const void* in, jvalue& p, bool)->bool{
                        const std::shared_ptr<void*>* ptr = reinterpret_cast<const std::shared_ptr<void*>*>(in);
                        if(ptr){
                            p.l = QtJambiAPI::convertSmartPointerToJavaObject(env, *typeId,
                                                                       new std::shared_ptr<void*>(*ptr),
                                                                       &QtJambiPrivate::SmartPointerHelper<std::shared_ptr, void*>::deletePointer,
                                                                       &QtJambiPrivate::SmartPointerHelper<std::shared_ptr, void*>::getFromPointer);
                        }
                        return true;
                    };
                }else if(pointerType == PointerType::weak_ptr){
                    return [typeId](JNIEnv* env, QtJambiScope*, const void* in, jvalue& p, bool)->bool{
                        const std::weak_ptr<void*>* ptr = reinterpret_cast<const std::weak_ptr<void*>*>(in);
                        if(ptr){
                            p.l = QtJambiAPI::convertSmartPointerToJavaObject(env, *typeId,
                                                                       new std::shared_ptr<void*>(*ptr),
                                                                       &QtJambiPrivate::SmartPointerHelper<std::shared_ptr, void*>::deletePointer,
                                                                       &QtJambiPrivate::SmartPointerHelper<std::shared_ptr, void*>::getFromPointer);
                        }
                        return true;
                    };
                }else if(pointerType == PointerType::ScopedPointer){
                    return [typeId](JNIEnv* env, QtJambiScope* scope, const void* in, jvalue& p, bool)->bool{
                        const QScopedPointer<void*>* ptr = reinterpret_cast<const QScopedPointer<void*>*>(in);
                        if(ptr){
                            p.l = QtJambiAPI::convertNativeToJavaObjectAsWrapper(env, ptr->data(), *typeId);
                            if(scope)
                                scope->addObjectInvalidation(env, p.l, false, true);
                        }
                        return true;
                    };
                }else if(pointerType == PointerType::unique_ptr){
                    return [typeId](JNIEnv* env, QtJambiScope* scope, const void* in, jvalue& p, bool)->bool{
                        const std::unique_ptr<void*>* ptr = reinterpret_cast<const std::unique_ptr<void*>*>(in);
                        if(ptr){
                            p.l = QtJambiAPI::convertNativeToJavaObjectAsWrapper(env, ptr->get(), *typeId);
                            if(scope)
                                scope->addObjectInvalidation(env, p.l, false, true);
                        }
                        return true;
                    };
                }else{
                    if (internalTypeName.endsWith(QLatin1Char('*')) || (isFunctional(*typeId) &&
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
                                [&]()->bool{QString typeName = QtJambiAPI::typeName(*typeId);
                                            return !typeName.startsWith("std::function") && typeName.contains("(*)");}()
#else
                                (internalMetaType.flags() & QMetaType::IsPointer)
#endif

                            )){
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
                                            scope->addObjectInvalidation(env, p.l, true, true);
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
                                        scope->addObjectInvalidation(env, p.l, true, true);
                                }
                            }
                            return true;
                        };
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
                                                scope->addObjectInvalidation(env, p.l, true, true);
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
                                            scope->addObjectInvalidation(env, p.l, true, true);
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
                    }
                }
            }
        }else{
            QByteArray className = QtJambiAPI::getClassName(_env, externalClass).replace(".", "/").toLatin1();
            externalClass = getGlobalClassRef(_env, externalClass, className);
            if (Java::QtCore::QObject::isAssignableFrom(_env,externalClass)) {
                if(pointerType == PointerType::SharedPointer){
                    return [className](JNIEnv* env, QtJambiScope*, const void* in, jvalue& p, bool)->bool{
                        const QSharedPointer<QObject>* ptr = reinterpret_cast<const QSharedPointer<QObject>*>(in);
                        if(ptr){
                            p.l = internal_convertQObjectSmartPointerToJavaObject(env, className,
                                                                       new QSharedPointer<QObject>(*ptr),
                                                                       &QtJambiPrivate::SmartPointerHelper<QSharedPointer, QObject>::deletePointer,
                                                                       &QtJambiPrivate::SmartPointerHelper<QSharedPointer, void*>::getFromPointer);
                        }
                        return true;
                    };
                }else if(pointerType == PointerType::WeakPointer){
                    return [className](JNIEnv* env, QtJambiScope*, const void* in, jvalue& p, bool)->bool{
                        const QWeakPointer<QObject>* ptr = reinterpret_cast<const QWeakPointer<QObject>*>(in);
                        if(ptr){
                            p.l = internal_convertQObjectSmartPointerToJavaObject(env, className,
                                                                       new QSharedPointer<QObject>(*ptr),
                                                                       &QtJambiPrivate::SmartPointerHelper<QSharedPointer, QObject>::deletePointer,
                                                                       &QtJambiPrivate::SmartPointerHelper<QSharedPointer, void*>::getFromPointer);
                        }
                        return true;
                    };
                }else if(pointerType == PointerType::shared_ptr){
                    return [className](JNIEnv* env, QtJambiScope*, const void* in, jvalue& p, bool)->bool{
                        const std::shared_ptr<QObject>* ptr = reinterpret_cast<const std::shared_ptr<QObject>*>(in);
                        if(ptr){
                            p.l = internal_convertQObjectSmartPointerToJavaObject(env, className,
                                                                       new std::shared_ptr<QObject>(*ptr),
                                                                       &QtJambiPrivate::SmartPointerHelper<std::shared_ptr, QObject>::deletePointer,
                                                                       &QtJambiPrivate::SmartPointerHelper<std::shared_ptr, void*>::getFromPointer);
                        }
                        return true;
                    };
                }else if(pointerType == PointerType::weak_ptr){
                    return [className](JNIEnv* env, QtJambiScope*, const void* in, jvalue& p, bool)->bool{
                        const std::weak_ptr<QObject>* ptr = reinterpret_cast<const std::weak_ptr<QObject>*>(in);
                        if(ptr){
                            p.l = internal_convertQObjectSmartPointerToJavaObject(env, className,
                                                                       new std::shared_ptr<QObject>(*ptr),
                                                                       &QtJambiPrivate::SmartPointerHelper<std::shared_ptr, QObject>::deletePointer,
                                                                       &QtJambiPrivate::SmartPointerHelper<std::shared_ptr, void*>::getFromPointer);
                        }
                        return true;
                    };
                }else if(pointerType == PointerType::ScopedPointer){
                    return [externalClass](JNIEnv* env, QtJambiScope* scope, const void* in, jvalue& p, bool)->bool{
                        const QScopedPointer<QObject>* ptr = reinterpret_cast<const QScopedPointer<QObject>*>(in);
                        if(ptr){
                            p.l = QtJambiAPI::convertQObjectToJavaObject(env, ptr->data(), externalClass);
                            if(scope)
                                scope->addObjectInvalidation(env, p.l, false, true);
                        }
                        return true;
                    };
                }else if(pointerType == PointerType::unique_ptr){
                    return [externalClass](JNIEnv* env, QtJambiScope* scope, const void* in, jvalue& p, bool)->bool{
                        const std::unique_ptr<QObject>* ptr = reinterpret_cast<const std::unique_ptr<QObject>*>(in);
                        if(ptr){
                            p.l = QtJambiAPI::convertQObjectToJavaObject(env, ptr->get(), externalClass);
                            if(scope)
                                scope->addObjectInvalidation(env, p.l, false, true);
                        }
                        return true;
                    };
                }else{
                    return [externalClass](JNIEnv* env, QtJambiScope*, const void* in, jvalue& p, bool)->bool{
                        QObject * const *ptr = reinterpret_cast<QObject * const *>(in);
                        if(ptr){
                            QObject * qobject = *ptr;
                            p.l = QtJambiAPI::convertQObjectToJavaObject(env, qobject, externalClass);
                        }
                        return true;
                    };
                }
            } else if(pointerType == PointerType::SharedPointer){
                return [className](JNIEnv* env, QtJambiScope*, const void* in, jvalue& p, bool)->bool{
                    const QSharedPointer<void*>* ptr = reinterpret_cast<const QSharedPointer<void*>*>(in);
                    if(ptr){
                        p.l = internal_convertSmartPointerToJavaObject(env,
                                                                  className,
                                                                  new QSharedPointer<void*>(*ptr),
                                                                  &QtJambiPrivate::SmartPointerHelper<QSharedPointer, void*>::deletePointer,
                                                                  &QtJambiPrivate::SmartPointerHelper<QSharedPointer, void*>::getFromPointer);
                    }
                    return true;
                };
            }else if(pointerType == PointerType::WeakPointer){
                return [className](JNIEnv* env, QtJambiScope*, const void* in, jvalue& p, bool)->bool{
                    const QWeakPointer<void*>* ptr = reinterpret_cast<const QWeakPointer<void*>*>(in);
                    if(ptr){
                        p.l = internal_convertSmartPointerToJavaObject(env,
                                                                  className,
                                                                  new QSharedPointer<void*>(*ptr),
                                                                  &QtJambiPrivate::SmartPointerHelper<QSharedPointer, void*>::deletePointer,
                                                                  &QtJambiPrivate::SmartPointerHelper<QSharedPointer, void*>::getFromPointer);
                    }
                    return true;
                };
            }else if(pointerType == PointerType::shared_ptr){
                return [className](JNIEnv* env, QtJambiScope*, const void* in, jvalue& p, bool)->bool{
                    const std::shared_ptr<void*>* ptr = reinterpret_cast<const std::shared_ptr<void*>*>(in);
                    if(ptr){
                        p.l = internal_convertSmartPointerToJavaObject(env, className,
                                                                   new std::shared_ptr<void*>(*ptr),
                                                                   &QtJambiPrivate::SmartPointerHelper<std::shared_ptr, void*>::deletePointer,
                                                                   &QtJambiPrivate::SmartPointerHelper<std::shared_ptr, void*>::getFromPointer);
                    }
                    return true;
                };
            }else if(pointerType == PointerType::weak_ptr){
                return [className](JNIEnv* env, QtJambiScope*, const void* in, jvalue& p, bool)->bool{
                    const std::weak_ptr<void*>* ptr = reinterpret_cast<const std::weak_ptr<void*>*>(in);
                    if(ptr){
                        p.l = internal_convertSmartPointerToJavaObject(env, className,
                                                                   new std::shared_ptr<void*>(*ptr),
                                                                   &QtJambiPrivate::SmartPointerHelper<std::shared_ptr, void*>::deletePointer,
                                                                   &QtJambiPrivate::SmartPointerHelper<std::shared_ptr, void*>::getFromPointer);
                    }
                    return true;
                };
            }else if(pointerType == PointerType::ScopedPointer){
                return [externalClass](JNIEnv* env, QtJambiScope* scope, const void* in, jvalue& p, bool)->bool{
                    const QScopedPointer<void*>* ptr = reinterpret_cast<const QScopedPointer<void*>*>(in);
                    if(ptr){
                        p.l = QtJambiAPI::convertNativeToJavaObjectAsWrapper(env, ptr->data(), externalClass);
                        if(scope)
                            scope->addObjectInvalidation(env, p.l, false, true);
                    }
                    return true;
                };
            }else if(pointerType == PointerType::unique_ptr){
                return [externalClass](JNIEnv* env, QtJambiScope* scope, const void* in, jvalue& p, bool)->bool{
                    const std::unique_ptr<void*>* ptr = reinterpret_cast<const std::unique_ptr<void*>*>(in);
                    if(ptr){
                        p.l = QtJambiAPI::convertNativeToJavaObjectAsWrapper(env, ptr->get(), externalClass);
                        if(scope)
                            scope->addObjectInvalidation(env, p.l, false, true);
                    }
                    return true;
                };
            }else{
                const SuperTypeInfos& superTypes = SuperTypeInfos::fromClass(_env, externalClass);
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
                                        scope->addObjectInvalidation(env, p.l, true, true);
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
                                    scope->addObjectInvalidation(env, p.l, true, true);
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
                                        scope->addObjectInvalidation(env, p.l, true, true);
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
                                    scope->addObjectInvalidation(env, p.l, true, true);
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
            }
        }
    } else if (internalMetaType.id() == registeredMetaTypeID(typeid(JObjectWrapper))
               || internalMetaType.id() == registeredMetaTypeID(typeid(JCollectionWrapper))
               || internalMetaType.id() == registeredMetaTypeID(typeid(JMapWrapper))
               || internalMetaType.id() == registeredMetaTypeID(typeid(JIteratorWrapper))
               || internalMetaType.id() == registeredMetaTypeID(typeid(JObjectArrayWrapper))
               || internalMetaType.id() == registeredMetaTypeID(typeid(JIntArrayWrapper))
               || internalMetaType.id() == registeredMetaTypeID(typeid(JLongArrayWrapper))
               || internalMetaType.id() == registeredMetaTypeID(typeid(JShortArrayWrapper))
               || internalMetaType.id() == registeredMetaTypeID(typeid(JByteArrayWrapper))
               || internalMetaType.id() == registeredMetaTypeID(typeid(JBooleanArrayWrapper))
               || internalMetaType.id() == registeredMetaTypeID(typeid(JCharArrayWrapper))
               || internalMetaType.id() == registeredMetaTypeID(typeid(JFloatArrayWrapper))
               || internalMetaType.id() == registeredMetaTypeID(typeid(JDoubleArrayWrapper))) {
        return [](JNIEnv* env, QtJambiScope*, const void* in, jvalue& p, bool)->bool{
            p.l = env->NewLocalRef(reinterpret_cast<const JObjectWrapper *>(in)->object());
            return true;
        };
    }else if (internalMetaType.id() == QMetaType::QVariant) {
        return [](JNIEnv* env, QtJambiScope*, const void* in, jvalue& p, bool)->bool{
            const QVariant & variant = *reinterpret_cast<const QVariant*>(in);
            p.l = QtJambiAPI::convertQVariantToJavaObject(env, variant);
            return true;
        };
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
                    scope->addObjectInvalidation(env, p.l, true, true);
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
    if (Java::QtJambi::QFlags::isInstanceOf(env, enum_value)) {
        result = QVariant::fromValue<qint32>(Java::QtJambi::QFlags::value(env,enum_value));
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

ExternalToInternalConverter QtJambiTypeManager::tryGetExternalToInternalConverter(JNIEnv* env, jclass externalClass, const QString &internalTypeName, const QMetaType& internalMetaType) {
    hash_type key = computeHash(env, externalClass, internalTypeName, internalMetaType);
    {
        QReadLocker locker(gCacheLock());
        Q_UNUSED(locker)
        if(gExternalToInternalConverters()->contains(key))
            return (*gExternalToInternalConverters())[key];
    }
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    if(internalMetaType.isValid() && !QMetaType(internalMetaType).iface()->metaObjectFn){
        ExternalToInternalConverter converter = getExternalToInternalConverterImpl(env, externalClass, internalTypeName, internalMetaType);
        Q_ASSERT(converter);
        QWriteLocker locker(gCacheLock());
        Q_UNUSED(locker)
        if(gExternalToInternalConverters()->contains(key))
            return (*gExternalToInternalConverters())[key];
        (*gExternalToInternalConverters())[key] = converter;
        return converter;
    }
#endif
    return ExternalToInternalConverter();
}

ExternalToInternalConverter QtJambiTypeManager::getExternalToInternalConverter(JNIEnv* env, jclass externalClass, const QString &internalTypeName, const QMetaType& internalMetaType) {
    hash_type key = computeHash(env, externalClass, internalTypeName, internalMetaType);
    {
        QReadLocker locker(gCacheLock());
        Q_UNUSED(locker)
        if(gExternalToInternalConverters()->contains(key))
            return (*gExternalToInternalConverters())[key];
    }
    ExternalToInternalConverter converter = getExternalToInternalConverterImpl(env, externalClass, internalTypeName, internalMetaType);
    Q_ASSERT(converter);
    QWriteLocker locker(gCacheLock());
    Q_UNUSED(locker)
    if(gExternalToInternalConverters()->contains(key))
        return (*gExternalToInternalConverters())[key];
    (*gExternalToInternalConverters())[key] = converter;
    return converter;
}

ExternalToInternalConverter QtJambiTypeManager::getExternalToInternalConverterImpl(JNIEnv* _env, jclass externalClass, QString internalTypeName, const QMetaType& internalMetaType) {
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
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    }else if(JObjectValueWrapper::isValueType(internalMetaType)){
        QMetaType _internalMetaType = internalMetaType;
        return [_internalMetaType, externalClass](JNIEnv* env, QtJambiScope* scope, jvalue val, void* &out, jValueType) -> bool{
            if(val.l && !env->IsInstanceOf(val.l, externalClass))
                Java::Runtime::IllegalArgumentException::throwNew(env, QString("Wrong argument given: %1, expected: %2").arg(QtJambiAPI::getObjectClassName(env, val.l).replace("$", ".")).arg(QtJambiAPI::getClassName(env, externalClass).replace("$", ".")) QTJAMBI_STACKTRACEINFO );
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
                Java::Runtime::IllegalArgumentException::throwNew(env, QString("Wrong argument given: %1, expected: %2").arg(QtJambiAPI::getObjectClassName(env, val.l).replace("$", ".")).arg(QtJambiAPI::getClassName(env, externalClass).replace("$", ".")) QTJAMBI_STACKTRACEINFO );
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
#endif
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

    if(internalMetaType.id() == registeredMetaTypeID(typeid(JObjectWrapper))
    || internalMetaType.id() == registeredMetaTypeID(typeid(JEnumWrapper))
    || internalMetaType.id() == registeredMetaTypeID(typeid(JCollectionWrapper))
    || internalMetaType.id() == registeredMetaTypeID(typeid(JMapWrapper))
    || internalMetaType.id() == registeredMetaTypeID(typeid(JIteratorWrapper))
    || internalMetaType.id() == registeredMetaTypeID(typeid(JObjectArrayWrapper))
    || internalMetaType.id() == registeredMetaTypeID(typeid(JIntArrayWrapper))
    || internalMetaType.id() == registeredMetaTypeID(typeid(JLongArrayWrapper))
    || internalMetaType.id() == registeredMetaTypeID(typeid(JShortArrayWrapper))
    || internalMetaType.id() == registeredMetaTypeID(typeid(JByteArrayWrapper))
    || internalMetaType.id() == registeredMetaTypeID(typeid(JBooleanArrayWrapper))
    || internalMetaType.id() == registeredMetaTypeID(typeid(JCharArrayWrapper))
    || internalMetaType.id() == registeredMetaTypeID(typeid(JFloatArrayWrapper))
    || internalMetaType.id() == registeredMetaTypeID(typeid(JDoubleArrayWrapper))){
        int _internalMetaType = internalMetaType.id();
        return [_internalMetaType, externalClass](JNIEnv* env, QtJambiScope* scope, jvalue val, void* &out, jValueType) -> bool{
            if(val.l && !env->IsInstanceOf(val.l, externalClass))
                Java::Runtime::IllegalArgumentException::throwNew(env, QString("Wrong argument given: %1, expected: %2").arg(QtJambiAPI::getObjectClassName(env, val.l).replace("$", ".")).arg(QtJambiAPI::getClassName(env, externalClass).replace("$", ".")) QTJAMBI_STACKTRACEINFO );
            if(scope && !out){
                JObjectWrapper* ptr;
                if(_internalMetaType == registeredMetaTypeID(typeid(JEnumWrapper))){
                    out = ptr = new JEnumWrapper;
                }else if(_internalMetaType == registeredMetaTypeID(typeid(JCollectionWrapper))){
                    out = ptr = new JCollectionWrapper;
                }else if(_internalMetaType == registeredMetaTypeID(typeid(JMapWrapper))){
                    out = ptr = new JMapWrapper;
                }else if(_internalMetaType == registeredMetaTypeID(typeid(JIteratorWrapper))){
                    out = ptr = new JIteratorWrapper;
                }else if(_internalMetaType == registeredMetaTypeID(typeid(JObjectArrayWrapper))){
                    out = ptr = new JObjectArrayWrapper;
                }else if(_internalMetaType == registeredMetaTypeID(typeid(JIntArrayWrapper))){
                    out = ptr = new JIntArrayWrapper;
                }else if(_internalMetaType == registeredMetaTypeID(typeid(JLongArrayWrapper))){
                    out = ptr = new JLongArrayWrapper;
                }else if(_internalMetaType == registeredMetaTypeID(typeid(JShortArrayWrapper))){
                    out = ptr = new JShortArrayWrapper;
                }else if(_internalMetaType == registeredMetaTypeID(typeid(JByteArrayWrapper))){
                    out = ptr = new JByteArrayWrapper;
                }else if(_internalMetaType == registeredMetaTypeID(typeid(JBooleanArrayWrapper))){
                    out = ptr = new JBooleanArrayWrapper;
                }else if(_internalMetaType == registeredMetaTypeID(typeid(JCharArrayWrapper))){
                    out = ptr = new JCharArrayWrapper;
                }else if(_internalMetaType == registeredMetaTypeID(typeid(JFloatArrayWrapper))){
                    out = ptr = new JFloatArrayWrapper;
                }else if(_internalMetaType == registeredMetaTypeID(typeid(JDoubleArrayWrapper))){
                    out = ptr = new JDoubleArrayWrapper;
                }else{
                    out = ptr = new JObjectWrapper;
                }
                scope->addDeletion(ptr);
            }
            if(!out)
                return false;
            *reinterpret_cast<JObjectWrapper*>(out) = JObjectWrapper(env, val.l);
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
            int _internalMetaType = internalMetaType.id();
            return [_internalMetaType](JNIEnv* env, QtJambiScope* scope, jvalue val, void* &out, jValueType valueType) -> bool{
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
                    out = ptr = QMetaType::create(_internalMetaType, &v);
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
                    scope->addDeletion(_internalMetaType, ptr);
#else
                    scope->addDeletion(QMetaType(_internalMetaType), ptr);
#endif
                }else{
                    if(!out)
                        return false;
                    QMetaType::destruct(_internalMetaType, out);
                    QByteArray ba;
                    bool saved;
                    /* write the copy to the stream */ {
                        QDataStream stream(&ba, QIODevice::WriteOnly);
                        saved = QMetaType::save(stream, qMetaTypeId<qint32>(), &v);
                    }

                    /* read it back into the destination */
                    if(saved){
                        QDataStream stream(&ba, QIODevice::ReadOnly);
                        QMetaType::load(stream, _internalMetaType, out);
                    }else if(QMetaType::construct(_internalMetaType, out, &v)!=out){
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
            int _internalMetaType = internalMetaType.id();
            return [_internalMetaType](JNIEnv* env, QtJambiScope* scope, jvalue val, void* &out, jValueType valueType) -> bool{
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
                    out = ptr = QMetaType::create(_internalMetaType, &v);
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
                    scope->addDeletion(_internalMetaType, ptr);
#else
                    scope->addDeletion(QMetaType(_internalMetaType), ptr);
#endif
                }else{
                    if(!out)
                        return false;
                    QMetaType::destruct(_internalMetaType, out);
                    QByteArray ba;
                    bool saved;
                    /* write the copy to the stream */ {
                        QDataStream stream(&ba, QIODevice::WriteOnly);
                        saved = QMetaType::save(stream, qMetaTypeId<qint64>(), &v);
                    }

                    /* read it back into the destination */
                    if(saved){
                        QDataStream stream(&ba, QIODevice::ReadOnly);
                        QMetaType::load(stream, _internalMetaType, out);
                    }else if(QMetaType::construct(_internalMetaType, out, &v)!=out){
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
            int _internalMetaType = internalMetaType.id();
            return [_internalMetaType](JNIEnv* env, QtJambiScope* scope, jvalue val, void* &out, jValueType valueType) -> bool{
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
                    out = ptr = QMetaType::create(_internalMetaType, &v);
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
                    scope->addDeletion(_internalMetaType, ptr);
#else
                    scope->addDeletion(QMetaType(_internalMetaType), ptr);
#endif
                }else{
                    if(!out)
                        return false;
                    QMetaType::destruct(_internalMetaType, out);
                    QByteArray ba;
                    bool saved;
                    /* write the copy to the stream */ {
                        QDataStream stream(&ba, QIODevice::WriteOnly);
                        saved = QMetaType::save(stream, qMetaTypeId<qint16>(), &v);
                    }

                    /* read it back into the destination */
                    if(saved){
                        QDataStream stream(&ba, QIODevice::ReadOnly);
                        QMetaType::load(stream, _internalMetaType, out);
                    }else if(QMetaType::construct(_internalMetaType, out, &v)!=out){
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
            int _internalMetaType = internalMetaType.id();
            return [_internalMetaType](JNIEnv* env, QtJambiScope* scope, jvalue val, void* &out, jValueType valueType) -> bool{
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
                    out = ptr = QMetaType::create(_internalMetaType, &v);
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
                    scope->addDeletion(_internalMetaType, ptr);
#else
                    scope->addDeletion(QMetaType(_internalMetaType), ptr);
#endif
                }else{
                    if(!out)
                        return false;
                    QMetaType::destruct(_internalMetaType, out);
                    QByteArray ba;
                    bool saved;
                    /* write the copy to the stream */ {
                        QDataStream stream(&ba, QIODevice::WriteOnly);
                        saved = QMetaType::save(stream, qMetaTypeId<qint8>(), &v);
                    }

                    /* read it back into the destination */
                    if(saved){
                        QDataStream stream(&ba, QIODevice::ReadOnly);
                        QMetaType::load(stream, _internalMetaType, out);
                    }else if(QMetaType::construct(_internalMetaType, out, &v)!=out){
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
            int _internalMetaType = internalMetaType.id();
            return [_internalMetaType](JNIEnv* env, QtJambiScope* scope, jvalue val, void* &out, jValueType valueType) -> bool{
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
                    out = ptr = QMetaType::create(_internalMetaType, &v);
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
                    scope->addDeletion(_internalMetaType, ptr);
#else
                    scope->addDeletion(QMetaType(_internalMetaType), ptr);
#endif
                }else{
                    if(!out)
                        return false;
                    QMetaType::destruct(_internalMetaType, out);
                    QByteArray ba;
                    bool saved;
                    /* write the copy to the stream */ {
                        QDataStream stream(&ba, QIODevice::WriteOnly);
                        saved = QMetaType::save(stream, qMetaTypeId<bool>(), &v);
                    }

                    /* read it back into the destination */
                    if(saved){
                        QDataStream stream(&ba, QIODevice::ReadOnly);
                        QMetaType::load(stream, _internalMetaType, out);
                    }else if(QMetaType::construct(_internalMetaType, out, &v)!=out){
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
            int _internalMetaType = internalMetaType.id();
            return [_internalMetaType](JNIEnv* env, QtJambiScope* scope, jvalue val, void* &out, jValueType valueType) -> bool{
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
                    out = ptr = QMetaType::create(_internalMetaType, &v);
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
                    scope->addDeletion(_internalMetaType, ptr);
#else
                    scope->addDeletion(QMetaType(_internalMetaType), ptr);
#endif
                }else{
                    if(!out)
                        return false;
                    QMetaType::destruct(_internalMetaType, out);
                    QByteArray ba;
                    bool saved;
                    /* write the copy to the stream */ {
                        QDataStream stream(&ba, QIODevice::WriteOnly);
                        saved = QMetaType::save(stream, qMetaTypeId<unsigned short>(), &v);
                    }

                    /* read it back into the destination */
                    if(saved){
                        QDataStream stream(&ba, QIODevice::ReadOnly);
                        QMetaType::load(stream, _internalMetaType, out);
                    }else if(QMetaType::construct(_internalMetaType, out, &v)!=out){
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
            int _internalMetaType = internalMetaType.id();
            return [_internalMetaType](JNIEnv* env, QtJambiScope* scope, jvalue val, void* &out, jValueType valueType) -> bool{
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
                    out = ptr = QMetaType::create(_internalMetaType, &v);
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
                    scope->addDeletion(_internalMetaType, ptr);
#else
                    scope->addDeletion(QMetaType(_internalMetaType), ptr);
#endif
                }else{
                    if(!out)
                        return false;
                    QMetaType::destruct(_internalMetaType, out);
                    QByteArray ba;
                    bool saved;
                    /* write the copy to the stream */ {
                        QDataStream stream(&ba, QIODevice::WriteOnly);
                        saved = QMetaType::save(stream, qMetaTypeId<float>(), &v);
                    }

                    /* read it back into the destination */
                    if(saved){
                        QDataStream stream(&ba, QIODevice::ReadOnly);
                        QMetaType::load(stream, _internalMetaType, out);
                    }else if(QMetaType::construct(_internalMetaType, out, &v)!=out){
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
            int _internalMetaType = internalMetaType.id();
            return [_internalMetaType](JNIEnv* env, QtJambiScope* scope, jvalue val, void* &out, jValueType valueType) -> bool{
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
                    out = ptr = QMetaType::create(_internalMetaType, &v);
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
                    scope->addDeletion(_internalMetaType, ptr);
#else
                    scope->addDeletion(QMetaType(_internalMetaType), ptr);
#endif
                }else{
                    if(!out)
                        return false;
                    QMetaType::destruct(_internalMetaType, out);
                    QByteArray ba;
                    bool saved;
                    /* write the copy to the stream */ {
                        QDataStream stream(&ba, QIODevice::WriteOnly);
                        saved = QMetaType::save(stream, qMetaTypeId<double>(), &v);
                    }

                    /* read it back into the destination */
                    if(saved){
                        QDataStream stream(&ba, QIODevice::ReadOnly);
                        QMetaType::load(stream, _internalMetaType, out);
                    }else if(QMetaType::construct(_internalMetaType, out, &v)!=out){
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
            int _internalMetaType = internalMetaType.id();
            return [_internalMetaType](JNIEnv* env, QtJambiScope* scope, jvalue val, void* &out, jValueType) -> bool{
                if(val.l && !Java::Runtime::String::isInstanceOf(env, val.l))
                    Java::Runtime::IllegalArgumentException::throwNew(env, QString("Wrong argument given: %1, expected: java.lang.String").arg(QtJambiAPI::getObjectClassName(env, val.l).replace("$", ".")) QTJAMBI_STACKTRACEINFO );
                jstring strg = reinterpret_cast<jstring>(val.l);
                QString s(qtjambi_cast<QString>(env, strg));
                if(scope && !out){
                    void* ptr;
                    out = ptr = QMetaType::create(_internalMetaType, &s);
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
                    scope->addDeletion(_internalMetaType, ptr);
#else
                    scope->addDeletion(QMetaType(_internalMetaType), ptr);
#endif
                }else{
                    if(!out)
                        return false;
                    QMetaType::destruct(_internalMetaType, out);
                    QByteArray ba;
                    bool saved;
                    /* write the copy to the stream */ {
                        QDataStream stream(&ba, QIODevice::WriteOnly);
                        saved = QMetaType::save(stream, qMetaTypeId<QString>(), &s);
                    }

                    /* read it back into the destination */
                    if(saved){
                        QDataStream stream(&ba, QIODevice::ReadOnly);
                        QMetaType::load(stream, _internalMetaType, out);
                    }else if(QMetaType::construct(_internalMetaType, out, &s)!=out){
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
            int _internalMetaType = internalMetaType.id();
            return [_internalMetaType](JNIEnv* env, QtJambiScope* scope, jvalue val, void* &out, jValueType) -> bool{
                if(val.l && !Java::Runtime::CharSequence::isInstanceOf(env, val.l))
                    Java::Runtime::IllegalArgumentException::throwNew(env, QString("Wrong argument given: %1, expected: java.lang.String").arg(QtJambiAPI::getObjectClassName(env, val.l).replace("$", ".")) QTJAMBI_STACKTRACEINFO );
                QString s(qtjambi_cast<QString>(env, val.l));
                if(scope && !out){
                    void* ptr;
                    out = ptr = QMetaType::create(_internalMetaType, &s);
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
                    scope->addDeletion(_internalMetaType, ptr);
#else
                    scope->addDeletion(QMetaType(_internalMetaType), ptr);
#endif
                }else{
                    if(!out)
                        return false;
                    QMetaType::destruct(_internalMetaType, out);
                    QByteArray ba;
                    bool saved;
                    /* write the copy to the stream */ {
                        QDataStream stream(&ba, QIODevice::WriteOnly);
                        saved = QMetaType::save(stream, qMetaTypeId<QString>(), &s);
                    }

                    /* read it back into the destination */
                    if(saved){
                        QDataStream stream(&ba, QIODevice::ReadOnly);
                        QMetaType::load(stream, _internalMetaType, out);
                    }else if(QMetaType::construct(_internalMetaType, out, &s)!=out){
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
                    out = ptr = new void*(!val.l ? nullptr : QtJambiAPI::convertQNativePointerToNative(env, val.l, 1));
                    scope->addDeletion(ptr);
                    return true;
                }else if(out){
                    *reinterpret_cast<void**>(out) = !val.l ? nullptr : QtJambiAPI::convertQNativePointerToNative(env, val.l, 1);
                    return true;
                }
                return false;
            };
        }else{
            int _internalMetaType = internalMetaType.id();
            return [_internalMetaType](JNIEnv* env, QtJambiScope* scope, jvalue val, void* &out, jValueType) -> bool{
                if(val.l && !Java::QtJambi::QNativePointer::isInstanceOf(env, val.l))
                    Java::Runtime::IllegalArgumentException::throwNew(env, QString("Wrong argument given: %1, expected: QNativePointer").arg(QtJambiAPI::getObjectClassName(env, val.l).replace("$", ".")) QTJAMBI_STACKTRACEINFO );
                void* nptr = !val.l ? nullptr : QtJambiAPI::convertQNativePointerToNative(env, val.l, 1);
                if(scope && !out){
                    void* ptr;
                    out = ptr = QMetaType::create(_internalMetaType, nptr);
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
                    scope->addDeletion(_internalMetaType, ptr);
#else
                    scope->addDeletion(QMetaType(_internalMetaType), ptr);
#endif
                }else if(out){
                    QMetaType::destruct(_internalMetaType, out);
                    if(QMetaType::construct(_internalMetaType, out, nptr)!=out){
                        QByteArray ba;
                        bool saved;
                        /* write the copy to the stream */ {
                            QDataStream stream(&ba, QIODevice::WriteOnly);
                            saved = QMetaType::save(stream, _internalMetaType, nptr);
                        }

                        /* read it back into the destination */
                        if(saved){
                            QDataStream stream(&ba, QIODevice::ReadOnly);
                            QMetaType::load(stream, _internalMetaType, out);
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
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
        int _internalMetaType = internalMetaType.id();
        if(isPointerType(internalTypeName)){
#else
        QMetaType _internalMetaType = internalMetaType;
        if(internalMetaType.flags() & QMetaType::IsPointer){
#endif
            return [_internalMetaType](JNIEnv* env, QtJambiScope* scope, jvalue val, void* &out, jValueType) -> bool{
                void* ptr = nullptr;
                if(val.l){
                    if(!Java::QtCore::QMetaType$GenericValue::isInstanceOf(env, val.l)){
                        QString className = QtJambiAPI::getObjectClassName(env, val.l);
                        bool matches = false;
                        if(const std::type_info* typeId = getTypeByJavaName(className)){
                            if(registeredMetaTypeID(*typeId)==METATYPE_ID(_internalMetaType))
                                matches = true;
                        }
                        if(!matches){
                            for(auto mtype : registeredCustomMetaTypesForJavaClass(className.toLatin1())){
                                if(mtype==_internalMetaType)
                                    matches = true;
                            }
                        }
                        if(!matches)
                            Java::Runtime::IllegalArgumentException::throwNew(env, QString("Wrong argument given: %1, expected: %2").arg(QtJambiAPI::getObjectClassName(env, val.l).replace("$", ".")).arg(QtJambiAPI::getClassName(env, Java::QtCore::QMetaType$GenericValue::getClass(env)).replace("$", ".")) QTJAMBI_STACKTRACEINFO );
                    }else{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
                        int given = qtjambi_cast<const QMetaType&>(env, Java::QtCore::QMetaType$GenericTypeInterface::metaType(env, val.l)).id();
                        if(_internalMetaType!=given){
                            Java::Runtime::IllegalArgumentException::throwNew(env, QStringLiteral("Wrong argument given: %1, expected: %2").arg(QMetaType::typeName(given)).arg(QMetaType::typeName(_internalMetaType)) QTJAMBI_STACKTRACEINFO );
                        }
#else
                        const QMetaType& given = qtjambi_cast<const QMetaType&>(env, Java::QtCore::QMetaType$GenericTypeInterface::metaType(env, val.l));
                        if(_internalMetaType!=given){
                            Java::Runtime::IllegalArgumentException::throwNew(env, QStringLiteral("Wrong argument given: %1, expected: %2").arg(given.name()).arg(_internalMetaType.name()) QTJAMBI_STACKTRACEINFO );
                        }
#endif
                    }
                    if(const QSharedPointer<QtJambiLink>& link = QtJambiLink::findLinkForJavaObject(env, val.l)){
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
            return [_internalMetaType](JNIEnv* env, QtJambiScope* scope, jvalue val, void* &out, jValueType) -> bool{
                if(val.l){
                    if(!Java::QtCore::QMetaType$GenericValue::isInstanceOf(env, val.l)){
                        QString className = QtJambiAPI::getObjectClassName(env, val.l);
                        bool matches = false;
                        if(const std::type_info* typeId = getTypeByJavaName(className)){
                            if(registeredMetaTypeID(*typeId)==METATYPE_ID(_internalMetaType))
                                matches = true;
                        }
                        if(!matches){
                            for(auto mtype : registeredCustomMetaTypesForJavaClass(className.toLatin1())){
                                if(mtype==_internalMetaType)
                                    matches = true;
                            }
                        }
                        if(!matches)
                            Java::Runtime::IllegalArgumentException::throwNew(env, QString("Wrong argument given: %1, expected: %2").arg(QtJambiAPI::getObjectClassName(env, val.l).replace("$", ".")).arg(QtJambiAPI::getClassName(env, Java::QtCore::QMetaType$GenericValue::getClass(env)).replace("$", ".")) QTJAMBI_STACKTRACEINFO );
                    }else{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
                        int given = qtjambi_cast<const QMetaType&>(env, Java::QtCore::QMetaType$GenericTypeInterface::metaType(env, val.l)).id();
                        if(_internalMetaType!=given){
                            Java::Runtime::IllegalArgumentException::throwNew(env, QStringLiteral("Wrong argument given: %1, expected: %2").arg(QMetaType::typeName(given)).arg(QMetaType::typeName(_internalMetaType)) QTJAMBI_STACKTRACEINFO );
                        }
#else
                        const QMetaType& given = qtjambi_cast<const QMetaType&>(env, Java::QtCore::QMetaType$GenericTypeInterface::metaType(env, val.l));
                        if(_internalMetaType!=given){
                            Java::Runtime::IllegalArgumentException::throwNew(env, QStringLiteral("Wrong argument given: %1, expected: %2").arg(given.name()).arg(_internalMetaType.name()) QTJAMBI_STACKTRACEINFO );
                        }
#endif
                    }
                    if(const QSharedPointer<QtJambiLink>& link = QtJambiLink::findLinkForJavaObject(env, val.l)){
                        void* nptr = link->pointer();
                        if(scope && !out){
                            void* ptr;
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
                            out = ptr = QMetaType::create(_internalMetaType, nptr);
                            scope->addDeletion(_internalMetaType, ptr);
#else
                            out = ptr = _internalMetaType.create(nptr);
                            scope->addDeletion(_internalMetaType, ptr);
#endif
                        }else if(out){
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
                            QMetaType::destruct(_internalMetaType, out);
                            if(QMetaType::construct(_internalMetaType, out, nptr)!=out){
#else
                            _internalMetaType.destruct(out);
                            if(_internalMetaType.construct(out, nptr)!=out){
#endif
                                QByteArray ba;
                                bool saved;
                                /* write the copy to the stream */ {
                                    QDataStream stream(&ba, QIODevice::WriteOnly);
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
                                    saved = QMetaType::save(stream, _internalMetaType, nptr);
#else
                                    saved = _internalMetaType.save(stream, nptr);
#endif
                                }

                                /* read it back into the destination */
                                if(saved){
                                    QDataStream stream(&ba, QIODevice::ReadOnly);
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
                                    QMetaType::load(stream, _internalMetaType, out);
#else
                                    _internalMetaType.load(stream, out);
#endif
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
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
                        out = ptr = QMetaType::create(_internalMetaType);
                        scope->addDeletion(_internalMetaType, ptr);
#else
                        out = ptr = _internalMetaType.create();
                        scope->addDeletion(_internalMetaType, ptr);
#endif
                    }
                    return out;
                }
            };
        }
    } else if (Java::QtCore::QMetaType$GenericObject::isSameClass(_env,externalClass)) {
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
        int _internalMetaType = internalMetaType.id();
        if(isPointerType(internalTypeName)){
#else
        QMetaType _internalMetaType = internalMetaType;
        if(internalMetaType.flags() & QMetaType::IsPointer){
#endif
            return [_internalMetaType](JNIEnv* env, QtJambiScope* scope, jvalue val, void* &out, jValueType) -> bool{
                void* ptr = nullptr;
                if(val.l){
                    if(!Java::QtCore::QMetaType$GenericObject::isInstanceOf(env, val.l)){
                        QString className = QtJambiAPI::getObjectClassName(env, val.l);
                        bool matches = false;
                        if(const std::type_info* typeId = getTypeByJavaName(className)){
                            if(registeredMetaTypeID(*typeId)==METATYPE_ID(_internalMetaType))
                                matches = true;
                        }
                        if(!matches){
                            for(auto mtype : registeredCustomMetaTypesForJavaClass(className.toLatin1())){
                                if(mtype==_internalMetaType)
                                    matches = true;
                            }
                        }
                        if(!matches)
                            Java::Runtime::IllegalArgumentException::throwNew(env, QString("Wrong argument given: %1, expected: %2").arg(QtJambiAPI::getObjectClassName(env, val.l).replace("$", ".")).arg(QtJambiAPI::getClassName(env, Java::QtCore::QMetaType$GenericObject::getClass(env)).replace("$", ".")) QTJAMBI_STACKTRACEINFO );
                    }else{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
                        int given = qtjambi_cast<const QMetaType&>(env, Java::QtCore::QMetaType$GenericTypeInterface::metaType(env, val.l)).id();
                        if(_internalMetaType!=given){
                            Java::Runtime::IllegalArgumentException::throwNew(env, QStringLiteral("Wrong argument given: %1, expected: %2").arg(QMetaType::typeName(given)).arg(QMetaType::typeName(_internalMetaType)) QTJAMBI_STACKTRACEINFO );
                        }
#else
                        const QMetaType& given = qtjambi_cast<const QMetaType&>(env, Java::QtCore::QMetaType$GenericTypeInterface::metaType(env, val.l));
                        if(_internalMetaType!=given){
                            Java::Runtime::IllegalArgumentException::throwNew(env, QStringLiteral("Wrong argument given: %1, expected: %2").arg(given.name()).arg(_internalMetaType.name()) QTJAMBI_STACKTRACEINFO );
                        }
#endif
                    }
                    if(const QSharedPointer<QtJambiLink>& link = QtJambiLink::findLinkForJavaObject(env, val.l)){
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
            return [_internalMetaType](JNIEnv* env, QtJambiScope* scope, jvalue val, void* &out, jValueType) -> bool{
                if(val.l){
                    if(!Java::QtCore::QMetaType$GenericObject::isInstanceOf(env, val.l)){
                        QString className = QtJambiAPI::getObjectClassName(env, val.l);
                        bool matches = false;
                        if(const std::type_info* typeId = getTypeByJavaName(className)){
                            if(registeredMetaTypeID(*typeId)==METATYPE_ID(_internalMetaType))
                                matches = true;
                        }
                        if(!matches){
                            for(auto mtype : registeredCustomMetaTypesForJavaClass(className.toLatin1())){
                                if(mtype==_internalMetaType)
                                    matches = true;
                            }
                        }
                        if(!matches)
                            Java::Runtime::IllegalArgumentException::throwNew(env, QString("Wrong argument given: %1, expected: %2").arg(QtJambiAPI::getObjectClassName(env, val.l).replace("$", ".")).arg(QtJambiAPI::getClassName(env, Java::QtCore::QMetaType$GenericObject::getClass(env)).replace("$", ".")) QTJAMBI_STACKTRACEINFO );
                    }else{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
                        int given = qtjambi_cast<const QMetaType&>(env, Java::QtCore::QMetaType$GenericTypeInterface::metaType(env, val.l)).id();
                        if(_internalMetaType!=given){
                            Java::Runtime::IllegalArgumentException::throwNew(env, QStringLiteral("Wrong argument given: %1, expected: %2").arg(QMetaType::typeName(given)).arg(QMetaType::typeName(_internalMetaType)) QTJAMBI_STACKTRACEINFO );
                        }
#else
                        const QMetaType& given = qtjambi_cast<const QMetaType&>(env, Java::QtCore::QMetaType$GenericTypeInterface::metaType(env, val.l));
                        if(_internalMetaType!=given){
                            Java::Runtime::IllegalArgumentException::throwNew(env, QStringLiteral("Wrong argument given: %1, expected: %2").arg(given.name()).arg(_internalMetaType.name()) QTJAMBI_STACKTRACEINFO );
                        }
#endif
                    }
                    if(const QSharedPointer<QtJambiLink>& link = QtJambiLink::findLinkForJavaObject(env, val.l)){
                        void* nptr = link->pointer();
                        if(scope && !out){
                            void* ptr;
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
                            out = ptr = QMetaType::create(_internalMetaType, nptr);
                            scope->addDeletion(_internalMetaType, ptr);
#else
                            out = ptr = _internalMetaType.create(nptr);
                            scope->addDeletion(_internalMetaType, ptr);
#endif
                        }else if(out){
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
                            QMetaType::destruct(_internalMetaType, out);
                            if(QMetaType::construct(_internalMetaType, out, nptr)!=out){
#else
                            _internalMetaType.destruct(out);
                            if(_internalMetaType.construct(out, nptr)!=out){
#endif
                                QByteArray ba;
                                bool saved;
                                /* write the copy to the stream */ {
                                    QDataStream stream(&ba, QIODevice::WriteOnly);
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
                                    saved = QMetaType::save(stream, _internalMetaType, nptr);
#else
                                    saved = _internalMetaType.save(stream, nptr);
#endif
                                }

                                /* read it back into the destination */
                                if(saved){
                                    QDataStream stream(&ba, QIODevice::ReadOnly);
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
                                    QMetaType::load(stream, _internalMetaType, out);
#else
                                    _internalMetaType.load(stream, out);
#endif
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
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
                        out = ptr = QMetaType::create(_internalMetaType);
                        scope->addDeletion(_internalMetaType, ptr);
#else
                        out = ptr = _internalMetaType.create();
                        scope->addDeletion(_internalMetaType, ptr);
#endif
                    }
                    return out;
                }
            };
        }
    } else if (Java::QtCore::QMetaType$GenericGadget::isSameClass(_env,externalClass)) {
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
        int _internalMetaType = internalMetaType.id();
#else
        QMetaType _internalMetaType = internalMetaType;
#endif
        if(internalTypeName.endsWith("&")){
            return [_internalMetaType](JNIEnv* env, QtJambiScope*, jvalue val, void* &out, jValueType) -> bool{
                if(val.l){
                    if(!Java::QtCore::QMetaType$GenericGadget::isInstanceOf(env, val.l)){
                        QString className = QtJambiAPI::getObjectClassName(env, val.l);
                        bool matches = false;
                        if(const std::type_info* typeId = getTypeByJavaName(className)){
                            if(registeredMetaTypeID(*typeId)==METATYPE_ID(_internalMetaType))
                                matches = true;
                        }
                        if(!matches){
                            for(auto mtype : registeredCustomMetaTypesForJavaClass(className.toLatin1())){
                                if(mtype==_internalMetaType)
                                    matches = true;
                            }
                        }
                        if(!matches)
                            Java::Runtime::IllegalArgumentException::throwNew(env, QString("Wrong argument given: %1, expected: %2").arg(QtJambiAPI::getObjectClassName(env, val.l).replace("$", ".")).arg(QtJambiAPI::getClassName(env, Java::QtCore::QMetaType$GenericGadget::getClass(env)).replace("$", ".")) QTJAMBI_STACKTRACEINFO );
                    }else{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
                        int given = qtjambi_cast<const QMetaType&>(env, Java::QtCore::QMetaType$GenericTypeInterface::metaType(env, val.l)).id();
                        if(_internalMetaType!=given){
                            Java::Runtime::IllegalArgumentException::throwNew(env, QStringLiteral("Wrong argument given: %1, expected: %2").arg(QMetaType::typeName(given)).arg(QMetaType::typeName(_internalMetaType)) QTJAMBI_STACKTRACEINFO );
                        }
#else
                        const QMetaType& given = qtjambi_cast<const QMetaType&>(env, Java::QtCore::QMetaType$GenericTypeInterface::metaType(env, val.l));
                        if(_internalMetaType!=given){
                            Java::Runtime::IllegalArgumentException::throwNew(env, QStringLiteral("Wrong argument given: %1, expected: %2").arg(given.name()).arg(_internalMetaType.name()) QTJAMBI_STACKTRACEINFO );
                        }
#endif
                    }
                    if(const QSharedPointer<QtJambiLink>& link = QtJambiLink::findLinkForJavaObject(env, val.l)){
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
            return [_internalMetaType](JNIEnv* env, QtJambiScope* scope, jvalue val, void* &out, jValueType) -> bool{
                void* ptr = nullptr;
                if(val.l){
                    if(!Java::QtCore::QMetaType$GenericGadget::isInstanceOf(env, val.l)){
                        QString className = QtJambiAPI::getObjectClassName(env, val.l);
                        bool matches = false;
                        if(const std::type_info* typeId = getTypeByJavaName(className)){
                            if(registeredMetaTypeID(*typeId)==METATYPE_ID(_internalMetaType))
                                matches = true;
                        }
                        if(!matches){
                            for(auto mtype : registeredCustomMetaTypesForJavaClass(className.toLatin1())){
                                if(mtype==_internalMetaType)
                                    matches = true;
                            }
                        }
                        if(!matches)
                            Java::Runtime::IllegalArgumentException::throwNew(env, QString("Wrong argument given: %1, expected: %2").arg(QtJambiAPI::getObjectClassName(env, val.l).replace("$", ".")).arg(QtJambiAPI::getClassName(env, Java::QtCore::QMetaType$GenericGadget::getClass(env)).replace("$", ".")) QTJAMBI_STACKTRACEINFO );
                    }else{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
                        int given = qtjambi_cast<const QMetaType&>(env, Java::QtCore::QMetaType$GenericTypeInterface::metaType(env, val.l)).id();
                        if(_internalMetaType!=given){
                            Java::Runtime::IllegalArgumentException::throwNew(env, QStringLiteral("Wrong argument given: %1, expected: %2").arg(QMetaType::typeName(given)).arg(QMetaType::typeName(_internalMetaType)) QTJAMBI_STACKTRACEINFO );
                        }
#else
                        const QMetaType& given = qtjambi_cast<const QMetaType&>(env, Java::QtCore::QMetaType$GenericTypeInterface::metaType(env, val.l));
                        if(_internalMetaType!=given){
                            Java::Runtime::IllegalArgumentException::throwNew(env, QStringLiteral("Wrong argument given: %1, expected: %2").arg(given.name()).arg(_internalMetaType.name()) QTJAMBI_STACKTRACEINFO );
                        }
#endif
                    }
                    if(const QSharedPointer<QtJambiLink>& link = QtJambiLink::findLinkForJavaObject(env, val.l)){
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
                    if(!env->IsInstanceOf(val.l, externalClass))
                        Java::Runtime::IllegalArgumentException::throwNew(env, QString("Wrong argument given: %1, expected: %2").arg(QtJambiAPI::getObjectClassName(env, val.l).replace("$", ".")).arg(QtJambiAPI::getClassName(env, externalClass).replace("$", ".")) QTJAMBI_STACKTRACEINFO );
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
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
            int _internalMetaType = internalMetaType.id();
#else
            QMetaType _internalMetaType = internalMetaType;
#endif
            return [_internalMetaType](JNIEnv* env, QtJambiScope* scope, jvalue val, void* &out, jValueType valueType) -> bool{
                switch(valueType){
                case jValueType::l:
                    if(Java::QtCore::QMetaType$GenericFlags::isInstanceOf(env,val.l)){
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
                        int given = qtjambi_cast<const QMetaType&>(env, Java::QtCore::QMetaType$GenericTypeInterface::metaType(env, val.l)).id();
                        if(_internalMetaType!=given){
                            Java::Runtime::IllegalArgumentException::throwNew(env, QStringLiteral("Wrong argument given: %1, expected: %2").arg(QMetaType::typeName(given)).arg(QMetaType::typeName(_internalMetaType)) QTJAMBI_STACKTRACEINFO );
                        }
#else
                        const QMetaType& given = qtjambi_cast<const QMetaType&>(env, Java::QtCore::QMetaType$GenericTypeInterface::metaType(env, val.l));
                        if(_internalMetaType!=given){
                            Java::Runtime::IllegalArgumentException::throwNew(env, QStringLiteral("Wrong argument given: %1, expected: %2").arg(given.name()).arg(_internalMetaType.name()) QTJAMBI_STACKTRACEINFO );
                        }
#endif
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
                                if(registeredMetaTypeID(*typeId)==METATYPE_ID(_internalMetaType))
                                    matches = true;
                            }
                            if(!matches){
                                for(auto mtype : registeredCustomMetaTypesForJavaClass(className.toLatin1())){
                                    if(mtype==_internalMetaType)
                                        matches = true;
                                }
                            }
                            if(!matches && !env->IsInstanceOf(val.l, Java::Runtime::Number::getClass(env)))
                                Java::Runtime::IllegalArgumentException::throwNew(env, QString("Wrong argument given: %1, expected: %2").arg(QtJambiAPI::getObjectClassName(env, val.l).replace("$", ".")).arg("any number") QTJAMBI_STACKTRACEINFO );
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
        }else {
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
            int _internalMetaType = internalMetaType.id();
#else
            QMetaType _internalMetaType = internalMetaType;
#endif
            return [_internalMetaType, externalClass](JNIEnv* env, QtJambiScope* scope, jvalue val, void* &out, jValueType valueType) -> bool{
                switch(valueType){
                case jValueType::l:
                    if(Java::QtCore::QMetaType$GenericTypeInterface::isInstanceOf(env,val.l)){
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
                        int given = qtjambi_cast<const QMetaType&>(env, Java::QtCore::QMetaType$GenericTypeInterface::metaType(env, val.l)).id();
                        if(_internalMetaType!=given){
                            Java::Runtime::IllegalArgumentException::throwNew(env, QStringLiteral("Wrong argument given: %1, expected: %2").arg(QMetaType::typeName(given)).arg(QMetaType::typeName(_internalMetaType)) QTJAMBI_STACKTRACEINFO );
                        }
#else
                        const QMetaType& given = qtjambi_cast<const QMetaType&>(env, Java::QtCore::QMetaType$GenericTypeInterface::metaType(env, val.l));
                        if(_internalMetaType!=given){
                            Java::Runtime::IllegalArgumentException::throwNew(env, QStringLiteral("Wrong argument given: %1, expected: %2").arg(given.name()).arg(_internalMetaType.name()) QTJAMBI_STACKTRACEINFO );
                        }
#endif
                        if(scope && !out){
                            qint32* ptr;
                            out = ptr = new qint32;
                            scope->addDeletion(ptr);
                        }
                        if(!out)
                            return false;
                        *reinterpret_cast<qint32*>(out) = int_for_QtEnumerator_or_QFlags(env, val.l).value<qint32>();
                    }else{
                        if(!env->IsInstanceOf(val.l, externalClass))
                            Java::Runtime::IllegalArgumentException::throwNew(env, QString("Wrong argument given: %1, expected: %2").arg(QtJambiAPI::getObjectClassName(env, val.l).replace("$", ".")).arg(QtJambiAPI::getClassName(env, externalClass).replace("$", ".")) QTJAMBI_STACKTRACEINFO );
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
    }else if(Java::QtJambi::QtEnumerator::isAssignableFrom(_env,externalClass)
             || Java::QtJambi::QtLongEnumerator::isAssignableFrom(_env,externalClass)
             || Java::QtJambi::QtShortEnumerator::isAssignableFrom(_env,externalClass)
             || Java::QtJambi::QtByteEnumerator::isAssignableFrom(_env,externalClass)
             || Java::Runtime::Enum::isAssignableFrom(_env,externalClass)){
        if(internalMetaType==QMetaType::fromType<JEnumWrapper>() || internalMetaType==QMetaType::fromType<JObjectWrapper>()){
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
                    JEnumWrapper* ptr;
                    out = ptr = new JEnumWrapper;
                    scope->addDeletion(ptr);
                }
                if(!out)
                    return false;
                *reinterpret_cast<JEnumWrapper*>(out) = JEnumWrapper(env, v);
                return true;
            };
        }else if(internalMetaType.isValid()){
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
            int _internalMetaType = internalMetaType.id();
#else
            QMetaType _internalMetaType = internalMetaType;
#endif
            if(Java::QtCore::QMetaType$GenericEnumerator::isSameClass(_env, externalClass)
                    || Java::QtCore::QMetaType$GenericByteEnumerator::isSameClass(_env, externalClass)
                    || Java::QtCore::QMetaType$GenericShortEnumerator::isSameClass(_env, externalClass)
                    || Java::QtCore::QMetaType$GenericLongEnumerator::isSameClass(_env, externalClass)){
                return [_internalMetaType](JNIEnv* env, QtJambiScope* scope, jvalue val, void* &out, jValueType valueType) -> bool{
                    QVariant value;
                    switch(valueType){
                    case jValueType::l:{
                        if(Java::QtCore::QMetaType$GenericTypeInterface::isInstanceOf(env,val.l)){
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
                        int given = qtjambi_cast<const QMetaType&>(env, Java::QtCore::QMetaType$GenericTypeInterface::metaType(env, val.l)).id();
                        if(_internalMetaType!=given){
                            Java::Runtime::IllegalArgumentException::throwNew(env, QStringLiteral("Wrong argument given: %1, expected: %2").arg(QMetaType::typeName(given)).arg(QMetaType::typeName(_internalMetaType)) QTJAMBI_STACKTRACEINFO );
                        }
#else
                        const QMetaType& given = qtjambi_cast<const QMetaType&>(env, Java::QtCore::QMetaType$GenericTypeInterface::metaType(env, val.l));
                        if(_internalMetaType!=given){
                            Java::Runtime::IllegalArgumentException::throwNew(env, QStringLiteral("Wrong argument given: %1, expected: %2").arg(given.name()).arg(_internalMetaType.name()) QTJAMBI_STACKTRACEINFO );
                        }
#endif
                            value = int_for_QtEnumerator_or_QFlags(env, val.l);
                        }else{
                            if(val.l){
                                QString className = QtJambiAPI::getObjectClassName(env, val.l);
                                bool matches = false;
                                if(const std::type_info* typeId = getTypeByJavaName(className)){
                                    if(registeredMetaTypeID(*typeId)==METATYPE_ID(_internalMetaType))
                                        matches = true;
                                }
                                if(!matches){
                                    for(auto mtype : registeredCustomMetaTypesForJavaClass(className.toLatin1())){
                                        if(mtype==_internalMetaType)
                                            matches = true;
                                    }
                                }
                                if(!matches
                                        && !env->IsInstanceOf(val.l, Java::Runtime::Number::getClass(env))
                                        && !env->IsInstanceOf(val.l, Java::Runtime::Enum::getClass(env)))
                                    Java::Runtime::IllegalArgumentException::throwNew(env, QString("Wrong argument given: %1, expected: %2").arg(QtJambiAPI::getObjectClassName(env, val.l).replace("$", ".")).arg("any number or enum") QTJAMBI_STACKTRACEINFO );
                                value = int_for_QtEnumerator_or_QFlags(env, val.l);
                            }else{
                                value = QVariant::fromValue(0);
                            }
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
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
                        out = QMetaType::create(_internalMetaType, value.data());
                        void* ptr = out;
                        scope->addDeletion(_internalMetaType, ptr);
#else
                        out = _internalMetaType.create(value.data());
                        void* ptr = out;
                        scope->addDeletion(QMetaType(_internalMetaType), ptr);
#endif
                    }else{
                        if(!out)
                            return false;
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
                        QMetaType::destruct(_internalMetaType, out);
                        if(QMetaType::construct(_internalMetaType, out, value.data())!=out){
#else
                        _internalMetaType.destruct(out);
                        if(_internalMetaType.construct(out, value.data())!=out){
#endif
                            QByteArray ba;
                            bool saved;
                            /* write the copy to the stream */ {
                                QDataStream stream(&ba, QIODevice::WriteOnly);
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
                                saved = QMetaType::save(stream, _internalMetaType, value.data());
#else
                                saved = _internalMetaType.save(stream, value.data());
#endif
                            }

                            /* read it back into the destination */
                            if(saved){
                                QDataStream stream(&ba, QIODevice::ReadOnly);
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
                                QMetaType::load(stream, _internalMetaType, out);
#else
                                _internalMetaType.load(stream, out);
#endif
                            }else{
                                return false;
                            }
                        }
                    }
                    return true;
                };
            }else{
                return [_internalMetaType, externalClass](JNIEnv* env, QtJambiScope* scope, jvalue val, void* &out, jValueType valueType) -> bool{
                    QVariant value;
                    switch(valueType){
                    case jValueType::l:{
                        if(Java::QtCore::QMetaType$GenericTypeInterface::isInstanceOf(env,val.l)){
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
                        int given = qtjambi_cast<const QMetaType&>(env, Java::QtCore::QMetaType$GenericTypeInterface::metaType(env, val.l)).id();
                        if(_internalMetaType!=given){
                            Java::Runtime::IllegalArgumentException::throwNew(env, QStringLiteral("Wrong argument given: %1, expected: %2").arg(QMetaType::typeName(given)).arg(QMetaType::typeName(_internalMetaType)) QTJAMBI_STACKTRACEINFO );
                        }
#else
                        const QMetaType& given = qtjambi_cast<const QMetaType&>(env, Java::QtCore::QMetaType$GenericTypeInterface::metaType(env, val.l));
                        if(_internalMetaType!=given){
                            Java::Runtime::IllegalArgumentException::throwNew(env, QStringLiteral("Wrong argument given: %1, expected: %2").arg(given.name()).arg(_internalMetaType.name()) QTJAMBI_STACKTRACEINFO );
                        }
#endif
                            value = int_for_QtEnumerator_or_QFlags(env, val.l);
                        }else{
                            if(!env->IsInstanceOf(val.l, externalClass))
                                Java::Runtime::IllegalArgumentException::throwNew(env, QString("Wrong argument given: %1, expected: %2").arg(QtJambiAPI::getObjectClassName(env, val.l).replace("$", ".")).arg(QtJambiAPI::getClassName(env, externalClass).replace("$", ".")) QTJAMBI_STACKTRACEINFO );
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
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
                        out = QMetaType::create(_internalMetaType, value.data());
                        void* ptr = out;
                        scope->addDeletion(_internalMetaType, ptr);
#else
                        out = _internalMetaType.create(value.data());
                        void* ptr = out;
                        scope->addDeletion(QMetaType(_internalMetaType), ptr);
#endif
                    }else{
                        if(!out)
                            return false;
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
                        QMetaType::destruct(_internalMetaType, out);
                        if(QMetaType::construct(_internalMetaType, out, value.data())!=out){
#else
                        _internalMetaType.destruct(out);
                        if(_internalMetaType.construct(out, value.data())!=out){
#endif
                            QByteArray ba;
                            bool saved;
                            /* write the copy to the stream */ {
                                QDataStream stream(&ba, QIODevice::WriteOnly);
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
                                saved = QMetaType::save(stream, _internalMetaType, value.data());
#else
                                saved = _internalMetaType.save(stream, value.data());
#endif
                            }

                            /* read it back into the destination */
                            if(saved){
                                QDataStream stream(&ba, QIODevice::ReadOnly);
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
                                QMetaType::load(stream, _internalMetaType, out);
#else
                                _internalMetaType.load(stream, out);
#endif
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
                    if(!env->IsInstanceOf(val.l, externalClass))
                        Java::Runtime::IllegalArgumentException::throwNew(env, QString("Wrong argument given: %1, expected: %2").arg(QtJambiAPI::getObjectClassName(env, val.l).replace("$", ".")).arg(QtJambiAPI::getClassName(env, externalClass).replace("$", ".")) QTJAMBI_STACKTRACEINFO );
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
                    if(!env->IsInstanceOf(val.l, externalClass))
                        Java::Runtime::IllegalArgumentException::throwNew(env, QString("Wrong argument given: %1, expected: %2").arg(QtJambiAPI::getObjectClassName(env, val.l).replace("$", ".")).arg(QtJambiAPI::getClassName(env, externalClass).replace("$", ".")) QTJAMBI_STACKTRACEINFO );
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
                    if(!env->IsInstanceOf(val.l, externalClass))
                        Java::Runtime::IllegalArgumentException::throwNew(env, QString("Wrong argument given: %1, expected: %2").arg(QtJambiAPI::getObjectClassName(env, val.l).replace("$", ".")).arg(QtJambiAPI::getClassName(env, externalClass).replace("$", ".")) QTJAMBI_STACKTRACEINFO );
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
                    if(!env->IsInstanceOf(val.l, externalClass))
                        Java::Runtime::IllegalArgumentException::throwNew(env, QString("Wrong argument given: %1, expected: %2").arg(QtJambiAPI::getObjectClassName(env, val.l).replace("$", ".")).arg(QtJambiAPI::getClassName(env, externalClass).replace("$", ".")) QTJAMBI_STACKTRACEINFO );
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
    }else if (internalMetaType.id()==QMetaType::QStringList
              || internalTypeName=="QStringList") {
        return [](JNIEnv* env, QtJambiScope* scope, jvalue val, void* &out, jValueType t) -> bool{
            if(t!=jValueType::l)
                return false;
            if(val.l && !env->IsInstanceOf(val.l, Java::Runtime::Collection::getClass(env)))
                Java::Runtime::IllegalArgumentException::throwNew(env, QString("Wrong argument given: %1, expected: %2").arg(QtJambiAPI::getObjectClassName(env, val.l).replace("$", ".")).arg(QtJambiAPI::getClassName(env, Java::Runtime::Collection::getClass(env))) QTJAMBI_STACKTRACEINFO );
            if(env->IsInstanceOf(val.l, Java::QtCore::QList::getClass(env))){
                if(QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForJavaObject(env, val.l)){
                    if(auto containerAccess = dynamic_cast<AbstractListAccess*>(link->containerAccess())){
                        if(containerAccess->elementMetaType().id()==QMetaType::QString){
                            if(out){
                                containerAccess->assign(out, link->pointer());
                                return true;
                            }else if(scope){
                                out = containerAccess->createContainer(reinterpret_cast<const void*>(link->pointer()));
                                auto _containerAccess = containerAccess->clone();
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
                jobject iter = QtJambiAPI::iteratorOfJavaCollection(env, val.l);
                while(QtJambiAPI::hasJavaIteratorNext(env, iter)){
                    jobject next = QtJambiAPI::nextOfJavaIterator(env, iter);
                    if(next && !Java::Runtime::String::isInstanceOf(env, next))
                        Java::Runtime::IllegalArgumentException::throwNew(env, QString("Wrong collection content given: %1, expected: %2").arg(QtJambiAPI::getObjectClassName(env, next).replace("$", ".")).arg(QtJambiAPI::getClassName(env, Java::Runtime::String::getClass(env))) QTJAMBI_STACKTRACEINFO );
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
                for(jstring next : content){
                    list->append(qtjambi_cast<QString>(env, next));
                }
            }
            return true;
        };
    }else if (internalMetaType.id()==QMetaType::QByteArrayList
              || internalTypeName=="QByteArrayList") {
        return [](JNIEnv* env, QtJambiScope* scope, jvalue val, void* &out, jValueType t) -> bool{
            if(t!=jValueType::l)
                return false;
            if(val.l && !env->IsInstanceOf(val.l, Java::Runtime::Collection::getClass(env)))
                Java::Runtime::IllegalArgumentException::throwNew(env, QString("Wrong argument given: %1, expected: %2").arg(QtJambiAPI::getObjectClassName(env, val.l).replace("$", ".")).arg(QtJambiAPI::getClassName(env, Java::Runtime::Collection::getClass(env))) QTJAMBI_STACKTRACEINFO );
            if(env->IsInstanceOf(val.l, Java::QtCore::QList::getClass(env))){
                if(QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForJavaObject(env, val.l)){
                    if(auto containerAccess = dynamic_cast<AbstractListAccess*>(link->containerAccess())){
                        if(containerAccess->elementMetaType().id()==QMetaType::QByteArray){
                            if(out){
                                containerAccess->assign(out, link->pointer());
                                return true;
                            }else if(scope){
                                out = containerAccess->createContainer(reinterpret_cast<const void*>(link->pointer()));
                                auto _containerAccess = containerAccess->clone();
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
                jobject iter = QtJambiAPI::iteratorOfJavaCollection(env, val.l);
                while(QtJambiAPI::hasJavaIteratorNext(env, iter)){
                    jobject next = QtJambiAPI::nextOfJavaIterator(env, iter);
                    if(next && !env->IsInstanceOf(next, Java::QtCore::QByteArray::getClass(env)))
                        Java::Runtime::IllegalArgumentException::throwNew(env, QString("Wrong collection content given: %1, expected: %2").arg(QtJambiAPI::getObjectClassName(env, next).replace("$", ".")).arg(QtJambiAPI::getClassName(env, Java::QtCore::QByteArray::getClass(env))) QTJAMBI_STACKTRACEINFO );
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
                for(jobject next : content){
                    list->append(qtjambi_cast<QByteArray>(env, next));
                }
            }
            return true;
        };
    }else if (internalMetaType.id()==QMetaType::QVariantList
              || internalTypeName=="QVariantList") {
        return [](JNIEnv* env, QtJambiScope* scope, jvalue val, void* &out, jValueType t) -> bool{
            if(t!=jValueType::l)
                return false;
            if(val.l && !env->IsInstanceOf(val.l, Java::Runtime::Collection::getClass(env)))
                Java::Runtime::IllegalArgumentException::throwNew(env, QString("Wrong argument given: %1, expected: %2").arg(QtJambiAPI::getObjectClassName(env, val.l).replace("$", ".")).arg(QtJambiAPI::getClassName(env, Java::Runtime::Collection::getClass(env))) QTJAMBI_STACKTRACEINFO );
            if(env->IsInstanceOf(val.l, Java::QtCore::QList::getClass(env))){
                if(QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForJavaObject(env, val.l)){
                    if(auto containerAccess = dynamic_cast<AbstractListAccess*>(link->containerAccess())){
                        if(containerAccess->elementMetaType().id()==QMetaType::QVariant){
                            if(out){
                                containerAccess->assign(out, link->pointer());
                                return true;
                            }else if(scope){
                                out = containerAccess->createContainer(reinterpret_cast<const void*>(link->pointer()));
                                auto _containerAccess = containerAccess->clone();
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
                jobject iter = QtJambiAPI::iteratorOfJavaCollection(env, val.l);
                while(QtJambiAPI::hasJavaIteratorNext(env, iter)){
                    jobject next = QtJambiAPI::nextOfJavaIterator(env, iter);
                    list->append(qtjambi_cast<QVariant>(env, next));
                }
            }
            return true;
        };
    }else if (internalMetaType.id()==QMetaType::QVariantMap
              || internalTypeName=="QVariantMap") {
        return [](JNIEnv* env, QtJambiScope* scope, jvalue val, void* &out, jValueType t) -> bool{
            if(t!=jValueType::l)
                return false;
            if(val.l && !env->IsInstanceOf(val.l, Java::Runtime::Map::getClass(env)))
                Java::Runtime::IllegalArgumentException::throwNew(env, QString("Wrong argument given: %1, expected: %2").arg(QtJambiAPI::getObjectClassName(env, val.l).replace("$", ".")).arg(QtJambiAPI::getClassName(env, Java::Runtime::Map::getClass(env))) QTJAMBI_STACKTRACEINFO );
            if(env->IsInstanceOf(val.l, Java::QtCore::QMap::getClass(env))){
                if(QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForJavaObject(env, val.l)){
                    if(auto containerAccess = dynamic_cast<AbstractMapAccess*>(link->containerAccess())){
                        if(containerAccess->keyMetaType().id()==QMetaType::QString){
                            if(containerAccess->valueMetaType().id()==QMetaType::QVariant){
                                if(out){
                                    containerAccess->assign(out, link->pointer());
                                    return true;
                                }else if(scope){
                                    out = containerAccess->createContainer(reinterpret_cast<const void*>(link->pointer()));
                                    auto _containerAccess = containerAccess->clone();
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
                        Java::Runtime::IllegalArgumentException::throwNew(env, QString("Wrong map key given: %1, expected: %2").arg(QtJambiAPI::getObjectClassName(env, key).replace("$", ".")).arg(QtJambiAPI::getClassName(env, Java::Runtime::String::getClass(env))) QTJAMBI_STACKTRACEINFO );
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
                for(jobject next : content){
                    jobject key = QtJambiAPI::keyOfJavaMapEntry(env, next);
                    jobject value = QtJambiAPI::valueOfJavaMapEntry(env, next);
                    map->insert(qtjambi_cast<QString>(env, key), QtJambiAPI::convertJavaObjectToQVariant(env, value));
                }
            }
            return true;
        };
    }else if (internalMetaType.id()==QMetaType::QVariantHash
              || internalTypeName=="QVariantHash") {
        return [](JNIEnv* env, QtJambiScope* scope, jvalue val, void* &out, jValueType t) -> bool{
            if(t!=jValueType::l)
                return false;
            if(val.l && !env->IsInstanceOf(val.l, Java::Runtime::Map::getClass(env)))
                Java::Runtime::IllegalArgumentException::throwNew(env, QString("Wrong argument given: %1, expected: %2").arg(QtJambiAPI::getObjectClassName(env, val.l).replace("$", ".")).arg(QtJambiAPI::getClassName(env, Java::Runtime::Map::getClass(env))) QTJAMBI_STACKTRACEINFO );
            if(env->IsInstanceOf(val.l, Java::QtCore::QHash::getClass(env))){
                if(QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForJavaObject(env, val.l)){
                    if(auto containerAccess = dynamic_cast<AbstractMapAccess*>(link->containerAccess())){
                        if(containerAccess->keyMetaType().id()==QMetaType::QString){
                            if(containerAccess->valueMetaType().id()==QMetaType::QVariant){
                                if(out){
                                    containerAccess->assign(out, link->pointer());
                                    return true;
                                }else if(scope){
                                    out = containerAccess->createContainer(reinterpret_cast<const void*>(link->pointer()));
                                    auto _containerAccess = containerAccess->clone();
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
                        Java::Runtime::IllegalArgumentException::throwNew(env, QString("Wrong map key given: %1, expected: %2").arg(QtJambiAPI::getObjectClassName(env, key).replace("$", ".")).arg(QtJambiAPI::getClassName(env, Java::Runtime::String::getClass(env))) QTJAMBI_STACKTRACEINFO );
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
                for(jobject next : content){
                    jobject key = QtJambiAPI::keyOfJavaMapEntry(env, next);
                    jobject value = QtJambiAPI::valueOfJavaMapEntry(env, next);
                    map->insert(qtjambi_cast<QString>(env, key), QtJambiAPI::convertJavaObjectToQVariant(env, value));
                }
            }
            return true;
        };
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    }else if(internalMetaType.id()==QMetaType::QVariantPair
              || internalTypeName=="QVariantPair"){
        return [](JNIEnv* env, QtJambiScope* scope, jvalue in, void* &out, jValueType valueType) -> bool{
            bool response = false;
            if(valueType==jValueType::l){
                if(in.l && !env->IsInstanceOf(in.l, Java::QtCore::QPair::getClass(env)))
                    Java::Runtime::IllegalArgumentException::throwNew(env, QString("Wrong argument given: %1, expected: %2").arg(QtJambiAPI::getObjectClassName(env, in.l).replace("$", ".")).arg(QtJambiAPI::getClassName(env, Java::QtCore::QPair::getClass(env)).replace("$", ".")) QTJAMBI_STACKTRACEINFO );
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
#endif
    }else if (Java::QtCore::QMetaObject::isAssignableFrom(_env,externalClass) && (!internalMetaType.isValid() || QByteArray("QMetaObject*")==internalMetaType.name() || QByteArray("const QMetaObject*")==internalMetaType.name())) {
        return [](JNIEnv* env, QtJambiScope* scope, jvalue val, void* &out, jValueType) -> bool{
            if(val.l && !env->IsInstanceOf(val.l, Java::QtCore::QMetaObject::getClass(env)))
                Java::Runtime::IllegalArgumentException::throwNew(env, QString("Wrong argument given: %1, expected: %2").arg(QtJambiAPI::getObjectClassName(env, val.l).replace("$", ".")).arg(QtJambiAPI::getClassName(env, Java::QtCore::QMetaObject::getClass(env))) QTJAMBI_STACKTRACEINFO );
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
                Java::Runtime::IllegalArgumentException::throwNew(env, QString("Wrong argument given: %1, expected: %2").arg(QtJambiAPI::getObjectClassName(env, val.l).replace("$", ".")).arg(QtJambiAPI::getClassName(env, Java::QtCore::QMetaObject$Connection::getClass(env)).replace("$", ".")) QTJAMBI_STACKTRACEINFO );
            if(scope && !out){
                QMetaObject::Connection* ptr;
                out = ptr = new QMetaObject::Connection;
                scope->addDeletion(ptr);
            }
            if(!out)
                return false;
            if(QtJambiAPI::convertJavaToNative(env, typeid(QMetaObject::Connection), val.l, out, scope))
                return true;
            else return false;
        };
    }else if(isArrayClass && (pointerType==PointerType::initializer_list || internalTypeName.startsWith("initializer_list<"))){
        jclass componentClass = Java::Runtime::Class::getComponentType(_env,externalClass);
        QString javaTypeName = QtJambiAPI::getClassName(_env, jclass(componentClass)).replace(".", "/");
        QString _internalTypeName = internalTypeName;
        if(_internalTypeName.startsWith("initializer_list<"))
            _internalTypeName = _internalTypeName.mid(22, _internalTypeName.length() - 23);
        if(const std::type_info* typeId = getTypeByQtName(_internalTypeName)){
            int metaType = QMetaType::type(qPrintable(_internalTypeName));
            if(metaType==QMetaType::UnknownType){
                metaType = registeredMetaTypeID(*typeId);
            }
            size_t size = getValueSize(*typeId);
            if(size>0){
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
                    QMetaType _metaType(metaType);
                    componentClass = getGlobalClassRef(_env, componentClass);
                    ExternalToInternalConverter externalToInternalComponentConverter = getExternalToInternalConverter(
                                                                    _env,
                                                                    componentClass,
                                                                    _internalTypeName,
                                                                    _metaType);
                    return [size, externalToInternalComponentConverter, componentClass](JNIEnv* env, QtJambiScope* scope, jvalue in, void* &out, jValueType) -> bool {
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
                int _internalMetaType = internalMetaType.id();
                return [metaObject,_internalMetaType, externalClass](JNIEnv* env, QtJambiScope* scope, jvalue val, void* &out, jValueType) -> bool{
                    if(Java::QtCore::QMetaType$GenericTypeInterface::isInstanceOf(env,val.l)){
                        int given = qtjambi_cast<const QMetaType&>(env, Java::QtCore::QMetaType$GenericTypeInterface::metaType(env, val.l)).id();
                        if(_internalMetaType!=given){
                            Java::Runtime::IllegalArgumentException::throwNew(env, QStringLiteral("Wrong argument given: %1, expected: %2").arg(QMetaType::typeName(given)).arg(QMetaType::typeName(_internalMetaType)) QTJAMBI_STACKTRACEINFO );
                        }
                    }else{
                        if(!env->IsInstanceOf(val.l, externalClass))
                            Java::Runtime::IllegalArgumentException::throwNew(env, QString("Wrong argument given: %1, expected: %2").arg(QtJambiAPI::getObjectClassName(env, val.l).replace("$", ".")).arg(QtJambiAPI::getClassName(env, externalClass).replace("$", ".")) QTJAMBI_STACKTRACEINFO );
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
                            JavaException::raiseIllegalArgumentException(env, qPrintable(QString("Wrong argument given: %1, expected: %2").arg(object->metaObject()->className()).arg(metaObject->className())) QTJAMBI_STACKTRACEINFO );
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
             || internalTypeName.startsWith("QSet<")
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
             || internalTypeName.startsWith("QLinkedList<")
#endif
             )
                && internalTypeName.endsWith(">")
                && (Java::Runtime::Collection::isAssignableFrom(_env,externalClass) || Java::Runtime::Object::isSameClass(_env,externalClass)) ){
            auto idx = internalTypeName.indexOf("<");
            SequentialContainerType containerType;
            if(internalTypeName.startsWith("QStack<")){
                containerType = SequentialContainerType::QStack;
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
            }else if(internalTypeName.startsWith("QLinkedList<")){
                containerType = SequentialContainerType::QLinkedList;
            }else if(internalTypeName.startsWith("QVector<")){
                containerType = SequentialContainerType::QVector;
#endif
            }else if(internalTypeName.startsWith("QQueue<")){
                containerType = SequentialContainerType::QQueue;
            }else if(internalTypeName.startsWith("QSet<")){
                containerType = SequentialContainerType::QSet;
            }else{
                containerType = SequentialContainerType::QList;
            }
            QString container = internalTypeName.mid(0, idx);
            QString instantiation = internalTypeName.mid(idx+1).chopped(1).trimmed();
            bool isPointer = isPointerType(instantiation);
            QByteArray clazz;
            size_t size = 0;
            size_t align = 0;
            QtJambiMetaType memberMetaType(QMetaType::UnknownType);
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
            bool isStaticType = false;
#endif
            const std::type_info* t;
            if(isPointer){
                t = getTypeByQtName(instantiation.chopped(1));
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
                memberMetaType = QMetaType::type(qPrintable(instantiation));
#else
                memberMetaType = QMetaType::fromName(qPrintable(instantiation));
#endif
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
                    memberMetaType = QMetaType(registeredMetaTypeID(*t));
                    if(memberMetaType.isValid()){
                        auto sz = memberMetaType.sizeOf();
                        size = sz<0 ? 0 : size_t(sz);
                    }else{
                        size = getValueSize(*t);
                    }
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
                    align = getValueAlignment(*t);
                    if(OptionalBool op = isRegisteredAsStaticType(*t))
                        isStaticType = op.value();
                    if(isFunctional(*t)){
                        QString typeName = QtJambiAPI::typeName(*t);
                        if(!typeName.startsWith("std::function") && (typeName.contains("(*)"))){
                            isPointer = true;
                        }
                    }
                    if(align==0 && !isPointer)
                        align = QtJambiTypeManager::getInternalAlignment(instantiation);
#else
                    if(memberMetaType.isValid()){
                        auto al = memberMetaType.alignOf();
                        align = size_t(al);
                    }else{
                        align = getValueAlignment(*t);
                    }
#endif
                }else{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
                    memberMetaType = QMetaType::type(qPrintable(instantiation));
                    isStaticType = QtJambiTypeManager::isStaticType(instantiation);
#else
                    memberMetaType = QMetaType::fromName(qPrintable(instantiation));
#endif
                    if(memberMetaType.isValid()){
                        auto sz = memberMetaType.sizeOf();
                        size = sz<0 ? 0 : size_t(sz);
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
                        if(memberMetaType.alignOf()!=0)
                            align = size_t(memberMetaType.alignOf());
#endif
                    }
                    if(size==0){
                        size = getInternalSize(instantiation);
                    }
                    if(align==0){
                        align = getInternalAlignment(instantiation);
                    }
                    if(!memberMetaType.isValid()){
                        clazz = getExternalTypeName(_env, instantiation, memberMetaType).toUtf8();
                    }else{
                        clazz = getExternalTypeName(_env, memberMetaType.name(), memberMetaType).toUtf8();
                    }
                }
            }

            if(!isPointer && !memberMetaType.isValid()){
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
                memberMetaType = QMetaType::type(qPrintable(instantiation));
                isStaticType = QtJambiTypeManager::isStaticType(instantiation);
#else
                memberMetaType = QMetaType::fromName(qPrintable(instantiation));
#endif
                if(memberMetaType.isValid()){
                    auto sz = memberMetaType.sizeOf();
                    size = sz<0 ? 0 : size_t(sz);
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
                    if(memberMetaType.alignOf()!=0)
                        align = size_t(memberMetaType.alignOf());
#endif
                }
                if(size==0){
                    size = getInternalSize(instantiation);
                }
                if(align==0){
                    align = getInternalAlignment(instantiation);
                }
            }
            AbstractContainerAccess* containerAccess = AbstractContainerAccess::create(_env, containerType, memberMetaType);

            if(!clazz.isEmpty()){
                jclass elementClass = JavaAPI::resolveClass(_env, clazz);
                typedef bool (*IsContainerFunction)(JNIEnv *, jobject, const QMetaType&, void*& pointer);
                typedef void (*ContainerAppendFunction)(AbstractContainerAccess*, JNIEnv *, void*, jobject);

                IsContainerFunction isContainerFunction = nullptr;
                ContainerAppendFunction containerAppendFunction = nullptr;

                switch(containerType){
                case SequentialContainerType::QSet:
                    isContainerFunction = ContainerAPI::getAsQSet;
                    containerAppendFunction = [](AbstractContainerAccess* access, JNIEnv * env, void* container, jobject value){
                        if(AbstractSetAccess* _access = dynamic_cast<AbstractSetAccess*>(access))
                            _access->insert(env, container, value);
                    };
                    break;
                case SequentialContainerType::QStack:
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
                case SequentialContainerType::QVector:
                    isContainerFunction = ContainerAPI::getAsQVector;
                    containerAppendFunction = [](AbstractContainerAccess* access, JNIEnv * env, void* container, jobject value){
                        if(AbstractVectorAccess* _access = dynamic_cast<AbstractVectorAccess*>(access))
                            _access->insert(env, container, _access->size(env, container), 1, value);
                    };
                    break;
                case SequentialContainerType::QLinkedList:
                    isContainerFunction = ContainerAPI::getAsQLinkedList;
                    containerAppendFunction = [](AbstractContainerAccess* access, JNIEnv * env, void* container, jobject value){
                        if(AbstractLinkedListAccess* _access = dynamic_cast<AbstractLinkedListAccess*>(access))
                            _access->append(env, container, value);
                    };
                    break;
#endif
                case SequentialContainerType::QQueue:
                case SequentialContainerType::QList:
                    isContainerFunction = ContainerAPI::getAsQList;
                    containerAppendFunction = [](AbstractContainerAccess* access, JNIEnv * env, void* container, jobject value){
                        if(AbstractListAccess* _access = dynamic_cast<AbstractListAccess*>(access))
                            _access->insert(env, container, _access->size(env, container), 1, value);
                    };
                    break;
                }

                if(!containerAccess){
                    InternalToExternalConverter memberConverter = getInternalToExternalConverter(
                                                                    _env,
                                                                    instantiation,
                                                                    memberMetaType,
                                                                    elementClass);
                    ExternalToInternalConverter memberReConverter = getExternalToInternalConverter(
                                                                    _env,
                                                                    elementClass,
                                                                    instantiation, memberMetaType);
                    if(!isPointer && !memberMetaType.isValid()){
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
                        memberMetaType = QMetaType::type(qPrintable(instantiation));
                        align = getInternalAlignment(instantiation);
#else
                        memberMetaType = QMetaType::fromName(qPrintable(instantiation));
                        align = memberMetaType.alignOf();
#endif
                        size = memberMetaType.sizeOf();
                        containerAccess = AbstractContainerAccess::create(_env, containerType, memberMetaType);
                    }
                    if(!containerAccess && (size==0 || memberMetaType.isValid())){
                        QHashFunction hashFunction = QtJambiTypeManager::findHashFunction(isPointer, memberMetaType.id());
                        containerAccess = AbstractContainerAccess::create(_env, containerType,
                                                                          memberMetaType,
                                                                          align, size,
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
                                                                          isStaticType,
#endif
                                                                          isPointer,
                                                                          hashFunction,
                                                                          memberConverter,
                                                                          memberReConverter);
                    }
                }
                if(containerAccess){
                    containerAccess->registerContainer(internalTypeName.toLatin1());
                    switch(containerType){
                    case SequentialContainerType::QStack:
    #if QT_VERSION < QT_VERSION_CHECK(6,0,0)
                    case SequentialContainerType::QVector:
                        containerAccess = checkContainerAccess(_env, dynamic_cast<AbstractVectorAccess*>(containerAccess));
                        break;
                    case SequentialContainerType::QLinkedList:
                        containerAccess = checkContainerAccess(_env, dynamic_cast<AbstractLinkedListAccess*>(containerAccess));
                        break;
    #endif
                    case SequentialContainerType::QQueue:
                    case SequentialContainerType::QList:
                        containerAccess = checkContainerAccess(_env, dynamic_cast<AbstractListAccess*>(containerAccess));
                        break;
                    case SequentialContainerType::QSet:
                        containerAccess = checkContainerAccess(_env, dynamic_cast<AbstractSetAccess*>(containerAccess));
                        break;
                    }
                    QSharedPointer<AbstractContainerAccess> sharedAccess(containerAccess, &containerDisposer);
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
                    return [sharedAccess,
                            containerAppendFunction,
                            elementClass,
                            memberMetaType,
                            isContainerFunction](JNIEnv* env, QtJambiScope* scope, jvalue in, void* &out, jValueType valueType) -> bool{
                        bool response = false;
                        if(valueType==jValueType::l){
                            if(in.l && !env->IsInstanceOf(in.l, Java::Runtime::Collection::getClass(env)))
                                Java::Runtime::IllegalArgumentException::throwNew(env, QString("Wrong argument given: %1, expected: %2").arg(QtJambiAPI::getObjectClassName(env, in.l).replace("$", ".")).arg(QtJambiAPI::getClassName(env, Java::Runtime::Collection::getClass(env)).replace("$", ".")) QTJAMBI_STACKTRACEINFO );
                            bool deleteAccess = true;
                            Q_ASSERT(env->GetObjectRefType(elementClass)==JNIGlobalRefType);
                            AbstractContainerAccess* access = sharedAccess->clone();
                            void* pointer{nullptr};
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
                            }else if(isContainerFunction(env, in.l, memberMetaType, pointer)){
                                if(out){
                                    access->assign(out, pointer);
                                }else if(scope){
                                    out = access->createContainer(reinterpret_cast<const void*>(pointer));
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
                                    jobject iterator = QtJambiAPI::iteratorOfJavaCollection(env, in.l);
                                    while(QtJambiAPI::hasJavaIteratorNext(env, iterator)) {
                                        jobject element = QtJambiAPI::nextOfJavaIterator(env, iterator);
                                        if(element && !env->IsInstanceOf(element, elementClass))
                                            Java::Runtime::IllegalArgumentException::throwNew(env, QString("Wrong collection content given: %1, expected: %2").arg(QtJambiAPI::getObjectClassName(env, element).replace("$", ".")).arg(QtJambiAPI::getClassName(env, elementClass).replace("$", ".")) QTJAMBI_STACKTRACEINFO );
                                        content << element;
                                    }
                                    for(const QSharedPointer<QtJambiLink>& link : QtJambiLink::findLinksForPointer(out)){
                                        if(auto _access = link->containerAccess()){
                                            response = true;
                                            for(jobject element : content) {
                                                containerAppendFunction(_access, env, out, element);
                                            }
                                            break;
                                        }
                                    }
                                    if(!response){
                                        response = true;
                                        for(jobject element : content) {
                                            containerAppendFunction(access, env, out, element);
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
            QStringList instantiations = internalTypeName.mid(idx+1).chopped(1).trimmed().split(",");
            reduceMapInstantiations(instantiations);
            if(instantiations.size()==2){
                bool isPointer1 = isPointerType(instantiations[0]);
                QByteArray clazz1;
                size_t size1 = 0;
                size_t align1 = 0;
                QtJambiMetaType memberMetaType1(QMetaType::UnknownType);
                const std::type_info* t1(nullptr);
                if(isPointer1){
                    t1 = getTypeByQtName(instantiations[0].chopped(1));
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
                    memberMetaType1 = QMetaType::type(qPrintable(instantiations[0]));
#else
                    memberMetaType1 = QMetaType::fromName(qPrintable(instantiations[0]));
#endif
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
                        memberMetaType1 = QMetaType(registeredMetaTypeID(*t1));
                        if(memberMetaType1.isValid()){
                            auto sz = memberMetaType1.sizeOf();
                            size1 = sz<0 ? 0 : size_t(sz);
                        }else{
                            size1 = getValueSize(*t1);
                        }
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
                        align1 = getValueAlignment(*t1);
                        if(isFunctional(*t1)){
                            QString typeName = QtJambiAPI::typeName(*t1);
                            if(!typeName.startsWith("std::function") && (typeName.contains("(*)"))){
                                isPointer1 = true;
                            }
                        }
                        if(align1==0 && !isPointer1)
                            align1 = QtJambiTypeManager::getInternalAlignment(instantiations[0]);
#else
                        if(memberMetaType1.isValid()){
                            auto al = memberMetaType1.alignOf();
                            align1 = size_t(al);
                        }else{
                            align1 = getValueAlignment(*t1);
                        }
#endif
                    }
                    if(!memberMetaType1.isValid()){
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
                        memberMetaType1 = QMetaType::type(qPrintable(instantiations[0]));
#else
                        memberMetaType1 = QMetaType::fromName(qPrintable(instantiations[0]));
#endif
                        if(memberMetaType1.isValid()){
                            auto sz = memberMetaType1.sizeOf();
                            size1 = sz<0 ? 0 : size_t(sz);
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
                            if(memberMetaType1.alignOf()!=0)
                                align1 = size_t(memberMetaType1.alignOf());
#endif
                        }
                        if(size1==0){
                            size1 = getInternalSize(instantiations[0]);
                        }
                        if(align1==0){
                            align1 = getInternalAlignment(instantiations[0]);
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
                size_t size2 = 0;
                size_t align2 = 0;
                QtJambiMetaType memberMetaType2(QMetaType::UnknownType);
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
                        size2 = getValueSize(*t2);
                        memberMetaType2 = QMetaType(registeredMetaTypeID(*t2));
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
                        align2 = getValueAlignment(*t2);
                        if(isFunctional(*t2)){
                            QString typeName = QtJambiAPI::typeName(*t2);
                            if(!typeName.startsWith("std::function") && (typeName.contains("(*)"))){
                                isPointer2 = true;
                            }
                        }
                        if(align2==0 && !isPointer2)
                            align2 = QtJambiTypeManager::getInternalAlignment(instantiations[1]);
#else
                        if(memberMetaType2.isValid()){
                            auto al = memberMetaType2.alignOf();
                            align2 = size_t(al);
                        }else{
                            align2 = getValueAlignment(*t2);
                        }
#endif
                    }else{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
                        memberMetaType2 = QMetaType::type(qPrintable(instantiations[1]));
#else
                        memberMetaType2 = QMetaType::fromName(qPrintable(instantiations[1]));
#endif
                        if(memberMetaType2.isValid()){
                            auto sz = memberMetaType2.sizeOf();
                            size2 = sz<0 ? 0 : size_t(sz);
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
                            if(memberMetaType2.alignOf()!=0)
                                align2 = size_t(memberMetaType2.alignOf());
#endif
                        }
                        if(size2==0){
                            size2 = getInternalSize(instantiations[1]);
                        }
                        if(align2==0){
                            align2 = getInternalAlignment(instantiations[1]);
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
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
                        memberMetaType1 = QMetaType::type(qPrintable(instantiations[0]));
#else
                        memberMetaType1 = QMetaType::fromName(qPrintable(instantiations[0]));
#endif
                        if(memberMetaType1.isValid()){
                            auto sz = memberMetaType1.sizeOf();
                            size1 = sz<0 ? 0 : size_t(sz);
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
                            if(memberMetaType1.alignOf()!=0)
                                align1 = size_t(memberMetaType1.alignOf());
#endif
                        }
                        if(size1==0){
                            size1 = getInternalSize(instantiations[0]);
                        }
                        if(align1==0){
                            align1 = getInternalAlignment(instantiations[0]);
                        }
                    }
                    jclass valueClass = JavaAPI::resolveClass(_env, clazz2);
                    if(!isPointer2 && !memberMetaType2.isValid()){
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
                        memberMetaType2 = QMetaType::type(qPrintable(instantiations[1]));
#else
                        memberMetaType2 = QMetaType::fromName(qPrintable(instantiations[1]));
#endif
                        if(memberMetaType2.isValid()){
                            auto sz = memberMetaType2.sizeOf();
                            size2 = sz<0 ? 0 : size_t(sz);
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
                            if(memberMetaType2.alignOf()!=0)
                                align2 = size_t(memberMetaType2.alignOf());
#endif
                        }
                        if(size2==0){
                            size2 = getInternalSize(instantiations[1]);
                        }
                        if(align2==0){
                            align2 = getInternalAlignment(instantiations[1]);
                        }
                    }
                    AbstractContainerAccess* containerAccess = AbstractContainerAccess::create(_env, mapType, memberMetaType1, memberMetaType2);
                    if(!containerAccess){
                        InternalToExternalConverter memberConverter1 = getInternalToExternalConverter(
                                                                        _env,
                                                                        instantiations[0],
                                                                        memberMetaType1,
                                                                        keyClass);
                        ExternalToInternalConverter memberReConverter1 = getExternalToInternalConverter(
                                                                        _env,
                                                                        keyClass,
                                                                        instantiations[0], memberMetaType1);
                        InternalToExternalConverter memberConverter2 = getInternalToExternalConverter(
                                                                        _env,
                                                                        instantiations[1],
                                                                        memberMetaType2,
                                                                        valueClass);
                        ExternalToInternalConverter memberReConverter2 = getExternalToInternalConverter(
                                                                        _env,
                                                                        valueClass,
                                                                        instantiations[1], memberMetaType2);
                        bool tryAgain = false;
                        if(!isPointer1 && !memberMetaType1.isValid()){
                            tryAgain = true;
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
                            memberMetaType1 = QMetaType::type(qPrintable(instantiations[0]));
                            align1 = getInternalAlignment(instantiations[1]);
#else
                            memberMetaType1 = QMetaType::fromName(qPrintable(instantiations[0]));
                            align1 = memberMetaType1.alignOf();
#endif
                            size1 = memberMetaType1.sizeOf();
                        }
                        if(!isPointer2 && !memberMetaType2.isValid()){
                            tryAgain = true;
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
                            memberMetaType2 = QMetaType::type(qPrintable(instantiations[1]));
                            align2 = getInternalAlignment(instantiations[1]);
#else
                            memberMetaType2 = QMetaType::fromName(qPrintable(instantiations[1]));
                            align2 = memberMetaType2.alignOf();
#endif
                            size2 = memberMetaType2.sizeOf();
                        }
                        if(tryAgain)
                            containerAccess = AbstractContainerAccess::create(_env, mapType, memberMetaType1, memberMetaType2);
                        if(!containerAccess && (size1==0 || memberMetaType1.isValid()) && (size2==0 || memberMetaType2.isValid())){
                            QHashFunction hashFunction1 = findHashFunction(isPointer1, memberMetaType1.id());
                            QHashFunction hashFunction2 = findHashFunction(isPointer2, memberMetaType2.id());
                            containerAccess = AbstractContainerAccess::create(_env, mapType,
                                                                              memberMetaType1,
                                                                              align1, size1,
                                                                              isPointer1,
                                                                              hashFunction1,
                                                                              memberConverter1,
                                                                              memberReConverter1,
                                                                              memberMetaType2,
                                                                              align2, size2,
                                                                              isPointer2,
                                                                              hashFunction2,
                                                                              memberConverter2,
                                                                              memberReConverter2);
                        }
                    }
                    if(containerAccess){
                        int containerMetaType = containerAccess->registerContainer(internalTypeName.toLatin1());
                        Q_UNUSED(containerMetaType)
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
                                            Java::Runtime::IllegalArgumentException::throwNew(env, QString("Wrong argument given: %1, expected: QPair<%2,%3>").arg(QtJambiAPI::getObjectClassName(env, in.l).replace("$", ".")).arg(QtJambiAPI::getClassName(env, keyClass).replace("$", ".")).arg(QtJambiAPI::getClassName(env, valueClass).replace("$", ".")) QTJAMBI_STACKTRACEINFO );
                                        first = Java::QtCore::QPair::first(env, in.l);
                                        second = Java::QtCore::QPair::second(env, in.l);
                                        if(first && !env->IsInstanceOf(first, keyClass))
                                            Java::Runtime::IllegalArgumentException::throwNew(env, QString("Wrong QPair.first given: %1, expected: %2").arg(QtJambiAPI::getObjectClassName(env, first).replace("$", ".")).arg(QtJambiAPI::getClassName(env, keyClass).replace("$", ".")) QTJAMBI_STACKTRACEINFO );
                                        if(second && !env->IsInstanceOf(second, valueClass))
                                            Java::Runtime::IllegalArgumentException::throwNew(env, QString("Wrong QPair.second given: %1, expected: %2").arg(QtJambiAPI::getObjectClassName(env, second).replace("$", ".")).arg(QtJambiAPI::getClassName(env, valueClass).replace("$", ".")) QTJAMBI_STACKTRACEINFO );
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
                            typedef bool (*IsBiContainerFunction)(JNIEnv *, jobject, const QMetaType&, const QMetaType&, void*& pointer);
                            typedef void (*ContainerInsertFunction)(AbstractContainerAccess*, JNIEnv *, void*, jobject, jobject);

                            IsBiContainerFunction isContainerFunction = ContainerAPI::getAsQMap;
                            ContainerInsertFunction containerInsertFunction = [](AbstractContainerAccess* access, JNIEnv * env, void* container, jobject key, jobject value){
                                if(AbstractMapAccess* _access = dynamic_cast<AbstractMapAccess*>(access))
                                    _access->insert(env, container, key, value);
                            };

                            switch(mapType){
                            case AssociativeContainerType::QMultiMap:
                                isContainerFunction = ContainerAPI::getAsQMultiMap;
                                containerInsertFunction = [](AbstractContainerAccess* access, JNIEnv * env, void* container, jobject key, jobject value){
                                    jobject iterator = QtJambiAPI::iteratorOfJavaCollection(env, value);
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
                                containerInsertFunction = [](AbstractContainerAccess* access, JNIEnv * env, void* container, jobject key, jobject value){
                                    jobject iterator = QtJambiAPI::iteratorOfJavaCollection(env, value);
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
                                containerInsertFunction = [](AbstractContainerAccess* access, JNIEnv * env, void* container, jobject key, jobject value){
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
                                void* pointer{nullptr};
                                if(valueType==jValueType::l){
                                    if(in.l && !env->IsInstanceOf(in.l, Java::Runtime::Map::getClass(env)))
                                        Java::Runtime::IllegalArgumentException::throwNew(env, QString("Wrong argument given: %1, expected: %2").arg(QtJambiAPI::getObjectClassName(env, in.l).replace("$", ".")).arg(QtJambiAPI::getClassName(env, Java::Runtime::Map::getClass(env)).replace("$", ".")) QTJAMBI_STACKTRACEINFO );
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
                                    }else if(isContainerFunction(env, in.l, memberMetaType1, memberMetaType2, pointer)){
                                        if(out){
                                            access->assign(out, pointer);
                                        }else if(scope){
                                            out = access->createContainer(reinterpret_cast<const void*>(pointer));
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
                                                        Java::Runtime::IllegalArgumentException::throwNew(env, QString("Wrong map key given: %1, expected: %2").arg(QtJambiAPI::getObjectClassName(env, key).replace("$", ".")).arg(QtJambiAPI::getClassName(env, keyClass).replace("$", ".")) QTJAMBI_STACKTRACEINFO );
                                                    if(value && !env->IsInstanceOf(value, valueClass))
                                                        Java::Runtime::IllegalArgumentException::throwNew(env, QString("Wrong map value given: %1, expected: %2").arg(QtJambiAPI::getObjectClassName(env, value).replace("$", ".")).arg(QtJambiAPI::getClassName(env, valueClass).replace("$", ".")) QTJAMBI_STACKTRACEINFO );
                                                    content << next;
                                                }
                                            }
                                            for(const QSharedPointer<QtJambiLink>& link : QtJambiLink::findLinksForPointer(out)){
                                                if(auto _access = link->containerAccess()){
                                                    response = true;
                                                    for(jobject entry : content) {
                                                        jobject key = QtJambiAPI::keyOfJavaMapEntry(env, entry);
                                                        jobject val = QtJambiAPI::valueOfJavaMapEntry(env, entry);
                                                        containerInsertFunction(_access, env, out, key, val);
                                                    }
                                                    break;
                                                }
                                            }
                                            if(!response){
                                                response = true;
                                                for(jobject entry : content) {
                                                    jobject key = QtJambiAPI::keyOfJavaMapEntry(env, entry);
                                                    jobject val = QtJambiAPI::valueOfJavaMapEntry(env, entry);
                                                    containerInsertFunction(access, env, out, key, val);
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
                const SuperTypeInfos& superTypeInfos = SuperTypeInfos::fromClass(_env, externalClass);
                if(!superTypeInfos.isEmpty()){
                    typeId = &superTypeInfos[0].typeId();
                }
            }
            if(typeId){
                bool isInterface = false;
                bool isPointer = false;
                bool isValue = false;
                bool isFunctional = false;
                EntryTypes entryType = getEntryType(*typeId);
                switch(entryType){
                case EntryTypes::FunctionalTypeInfo:
                    isFunctional = true;
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
                int _internalMetaType = internalMetaType.id();
                if(isQmlList && !internalMetaType.isValid()){
                    _internalMetaType = registeredMetaTypeID(*typeId);
                }
                if(internalTypeName.endsWith("&") || !(isValue || isPointer || internalTypeName.endsWith("*") || isQmlList)){
                    if(isInterface){
                        return [typeId, externalClass](JNIEnv* env, QtJambiScope*, jvalue val, void* &out, jValueType) -> bool{
                            if(val.l){
                                if(!env->IsInstanceOf(val.l, externalClass))
                                    Java::Runtime::IllegalArgumentException::throwNew(env, QString("Wrong argument given: %1, expected: %2").arg(QtJambiAPI::getObjectClassName(env, val.l).replace("$", ".")).arg(QtJambiAPI::getClassName(env, externalClass).replace("$", ".")) QTJAMBI_STACKTRACEINFO );
                                out = QtJambiLink::findPointerForJavaInterface(env, val.l, *typeId);
                            }else{
                                out = nullptr;
                            }
                            return true;
                        };
                    }else if(isFunctional){
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
                        QString typeName = QtJambiAPI::typeName(*typeId);
                        bool is_std_function = typeName.startsWith("std::function") || !typeName.contains("(*)");
#else
                        bool is_std_function = !(internalMetaType.flags() & QMetaType::IsPointer);
#endif
                        if(is_std_function){
                            int metaTypeId = internalMetaType.id();
                            return [typeId, externalClass, metaTypeId](JNIEnv* env, QtJambiScope* scope, jvalue val, void* &out, jValueType) -> bool{
                                if(val.l){
                                    QMetaType metaType(metaTypeId);
                                    if(!env->IsInstanceOf(val.l, externalClass))
                                        Java::Runtime::IllegalArgumentException::throwNew(env, QString("Wrong argument given: %1, expected: %2").arg(QtJambiAPI::getObjectClassName(env, val.l).replace("$", ".")).arg(QtJambiAPI::getClassName(env, externalClass).replace("$", ".")) QTJAMBI_STACKTRACEINFO );
                                    if(const QSharedPointer<QtJambiLink>& link = QtJambiLink::findLinkForJavaInterface(env, val.l)){
                                        if(!out){
                                            if(scope){
                                                out = metaType.create();
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
                        }else{
                            return [typeId, externalClass](JNIEnv* env, QtJambiScope* scope, jvalue val, void* &out, jValueType) -> bool{
                                if(val.l){
                                    if(!env->IsInstanceOf(val.l, externalClass))
                                        Java::Runtime::IllegalArgumentException::throwNew(env, QString("Wrong argument given: %1, expected: %2").arg(QtJambiAPI::getObjectClassName(env, val.l).replace("$", ".")).arg(QtJambiAPI::getClassName(env, externalClass).replace("$", ".")) QTJAMBI_STACKTRACEINFO );
                                    if(const QSharedPointer<QtJambiLink>& link = QtJambiLink::findLinkForJavaInterface(env, val.l)){
                                        if(!out){
                                            if(scope){
                                                out = new void*();
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
                                    }else{
                                        Java::QtJambi::QNoNativeResourcesException::throwNew(env, QStringLiteral("Incomplete object of type: %1").arg(QtJambiAPI::getObjectClassName(env, val.l).replace("$", ".")) QTJAMBI_STACKTRACEINFO );
                                    }
                                }else{
                                    out = nullptr;
                                }
                                return true;
                            };
                        }
                    }else{
                        return [externalClass](JNIEnv* env, QtJambiScope*, jvalue val, void* &out, jValueType) -> bool{
                            if(val.l){
                                if(!env->IsInstanceOf(val.l, externalClass))
                                    Java::Runtime::IllegalArgumentException::throwNew(env, QString("Wrong argument given: %1, expected: %2").arg(QtJambiAPI::getObjectClassName(env, val.l).replace("$", ".")).arg(QtJambiAPI::getClassName(env, externalClass).replace("$", ".")) QTJAMBI_STACKTRACEINFO );
                                if(const QSharedPointer<QtJambiLink>& link = QtJambiLink::findLinkForJavaObject(env, val.l)){
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
                                    int given = qtjambi_cast<const QMetaType&>(env, Java::QtCore::QMetaType$GenericTypeInterface::metaType(env, val.l)).id();
                                    if(_internalMetaType!=given){
                                        Java::Runtime::IllegalArgumentException::throwNew(env, QStringLiteral("Wrong argument given: %1, expected: %2").arg(QMetaType::typeName(given)).arg(QMetaType::typeName(_internalMetaType)) QTJAMBI_STACKTRACEINFO );
                                    }
                                }else{
                                    if(!env->IsInstanceOf(val.l, externalClass))
                                        Java::Runtime::IllegalArgumentException::throwNew(env, QStringLiteral("Wrong argument given: %1, expected: %2").arg(QtJambiAPI::getObjectClassName(env, val.l).replace("$", ".")).arg(QtJambiAPI::getClassName(env, externalClass).replace("$", ".")) QTJAMBI_STACKTRACEINFO );
                                }
                                ptr = QtJambiLink::findPointerForJavaInterface(env, val.l, *typeId);
                            }
                            if(scope && !out){
                                out = QMetaType::create(_internalMetaType, ptr);
                                void* pptr = out;
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
                                scope->addDeletion(_internalMetaType, pptr);
#else
                                scope->addDeletion(QMetaType(_internalMetaType), pptr);
#endif
                            }else if(out){
                                QMetaType::destruct(_internalMetaType, out);
                                if(QMetaType::construct(_internalMetaType, out, ptr)!=out){
                                    if(ptr){
                                        QByteArray ba;
                                        bool saved;
                                        /* write the copy to the stream */ {
                                            QDataStream stream(&ba, QIODevice::WriteOnly);
                                            saved = QMetaType::save(stream, _internalMetaType, ptr);
                                        }

                                        /* read it back into the destination */
                                        if(saved){
                                            QDataStream stream(&ba, QIODevice::ReadOnly);
                                            QMetaType::load(stream, _internalMetaType, out);
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
                                    int given = qtjambi_cast<const QMetaType&>(env, Java::QtCore::QMetaType$GenericTypeInterface::metaType(env, val.l)).id();
                                    if(_internalMetaType!=given){
                                        Java::Runtime::IllegalArgumentException::throwNew(env, QStringLiteral("Wrong argument given: %1, expected: %2").arg(QMetaType::typeName(given)).arg(QMetaType::typeName(_internalMetaType)) QTJAMBI_STACKTRACEINFO );
                                    }
                                    if(const QSharedPointer<QtJambiLink>& link = QtJambiLink::findLinkForJavaObject(env, val.l)){
                                        ptr = link->pointer();
                                    }else{
                                        Java::QtJambi::QNoNativeResourcesException::throwNew(env, QStringLiteral("Incomplete object of type: %1").arg(QtJambiAPI::getObjectClassName(env, val.l).replace("$", ".")) QTJAMBI_STACKTRACEINFO );
                                    }
                                }else{
                                    if(!env->IsInstanceOf(val.l, externalClass))
                                        Java::Runtime::IllegalArgumentException::throwNew(env, QStringLiteral("Wrong argument given: %1, expected: %2").arg(QtJambiAPI::getObjectClassName(env, val.l).replace("$", ".")).arg(QtJambiAPI::getClassName(env, externalClass).replace("$", ".")) QTJAMBI_STACKTRACEINFO );
                                    if(const QSharedPointer<QtJambiLink>& link = QtJambiLink::findLinkForJavaObject(env, val.l)){
                                        ptr = link->pointer();
                                    }else{
                                        Java::QtJambi::QNoNativeResourcesException::throwNew(env, QStringLiteral("Incomplete object of type: %1").arg(QtJambiAPI::getObjectClassName(env, val.l).replace("$", ".")) QTJAMBI_STACKTRACEINFO );
                                    }
                                }
                            }
                            if(scope && !out){
                                out = QMetaType::create(_internalMetaType, ptr);
                                void* pptr = out;
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
                                scope->addDeletion(_internalMetaType, pptr);
#else
                                scope->addDeletion(QMetaType(_internalMetaType), pptr);
#endif
                            }else if(out){
                                QMetaType::destruct(_internalMetaType, out);
                                if(QMetaType::construct(_internalMetaType, out, ptr)!=out){
                                    if(ptr){
                                        QByteArray ba;
                                        bool saved;
                                        /* write the copy to the stream */ {
                                            QDataStream stream(&ba, QIODevice::WriteOnly);
                                            saved = QMetaType::save(stream, _internalMetaType, ptr);
                                        }

                                        /* read it back into the destination */
                                        if(saved){
                                            QDataStream stream(&ba, QIODevice::ReadOnly);
                                            QMetaType::load(stream, _internalMetaType, out);
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
                                if(!env->IsInstanceOf(val.l, externalClass))
                                    Java::Runtime::IllegalArgumentException::throwNew(env, QString("Wrong argument given: %1, expected: %2").arg(QtJambiAPI::getObjectClassName(env, val.l).replace("$", ".")).arg(QtJambiAPI::getClassName(env, externalClass).replace("$", ".")) QTJAMBI_STACKTRACEINFO );
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
                                if(!env->IsInstanceOf(val.l, externalClass))
                                    Java::Runtime::IllegalArgumentException::throwNew(env, QString("Wrong argument given: %1, expected: %2").arg(QtJambiAPI::getObjectClassName(env, val.l).replace("$", ".")).arg(QtJambiAPI::getClassName(env, externalClass).replace("$", ".")) QTJAMBI_STACKTRACEINFO );
                                if(const QSharedPointer<QtJambiLink>& link = QtJambiLink::findLinkForJavaObject(env, val.l)){
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

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
bool QtJambiTypeManager::isStaticType(const QString &internalTypeName){
    if(isPointerType(internalTypeName)){
        return false;
    }
    if(internalTypeName.startsWith("QVector<")
            || internalTypeName.startsWith("QList<")
            || internalTypeName.startsWith("QQueue<")
            || internalTypeName.startsWith("QStack<")
            || internalTypeName.startsWith("QSet<")){
        return false;
    }else if(internalTypeName.startsWith("QMap<")
             || internalTypeName.startsWith("QMultiMap<")
             || internalTypeName.startsWith("QHash<")
             || internalTypeName.startsWith("QMultiHash<")){
         return (QT_VERSION < QT_VERSION_CHECK(6, 0, 0));
    }else if(internalTypeName.startsWith("QPair<") || internalTypeName.startsWith("std::pair<")){
        auto idx = internalTypeName.indexOf("<");
        QString container = internalTypeName.mid(0, idx);
        QStringList instantiations = internalTypeName.mid(idx+1).chopped(1).trimmed().split(",");
        reduceMapInstantiations(instantiations);
        if(instantiations.size()==2){
            const std::type_info* t1;
            bool isPointer1 = isPointerType(instantiations[0]);
            if(isPointer1){
                t1 = getTypeByQtName(instantiations[0].chopped(1));
                if(!t1)
                    t1 = getTypeByQtName(instantiations[0]);
            }else{
                t1 = getTypeByQtName(instantiations[0]);
            }
            const std::type_info* t2;
            bool isPointer2 = isPointerType(instantiations[1]);
            if(isPointer2){
                t2 = getTypeByQtName(instantiations[1].chopped(1));
                if(!t2)
                    t2 = getTypeByQtName(instantiations[1]);
            }else{
                t2 = getTypeByQtName(instantiations[1]);
            }
            bool isStaticType1 = true;
            bool isStaticType2 = true;
            if(t1){
                if(isPointer1){
                    isStaticType1 = false;
                }else{
                    if(OptionalBool op = isRegisteredAsStaticType(*t1)){
                        isStaticType1 = op.value();
                    }
                }
            }else{
                isStaticType1 = isStaticType(instantiations[0]);
            }
            if(t2){
                if(isPointer2){
                    isStaticType2 = false;
                }else{
                    if(OptionalBool op = isRegisteredAsStaticType(*t2)){
                        isStaticType2 = op.value();
                    }
                }
            }else{
                isStaticType2 = isStaticType(instantiations[1]);
            }
            return isStaticType1 || isStaticType2;
        }
    }
    return true;
}

void QtJambiTypeManager::registerInternalAlignment(const QByteArray &internalTypeName, size_t align)
{
    if(!gAlignmentByTypeHash->contains(internalTypeName))
        gAlignmentByTypeHash->insert(internalTypeName, align);
}
#endif

size_t QtJambiTypeManager::getInternalAlignment(const QString &internalTypeName)
{
    if(isPointerType(internalTypeName)){
        return Q_ALIGNOF(void*);
    }
    size_t align;
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    align = size_t(QMetaType::fromName(internalTypeName.toLatin1()).alignOf());
#else
    align = gAlignmentByTypeHash->value(internalTypeName.toLatin1(), 0);
#endif
    if(align<=0 && (internalTypeName.startsWith("QVector<")
                || internalTypeName.startsWith("QStack<")
                || internalTypeName.startsWith("QList<")
                || internalTypeName.startsWith("QQueue<")
                || internalTypeName.startsWith("QSet<")
                || internalTypeName.startsWith("QMap<")
                || internalTypeName.startsWith("QHash<")
                || internalTypeName.startsWith("QMultiMap<")
                || internalTypeName.startsWith("QMultiHash<")
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
                || internalTypeName.startsWith("QLinkedList<")
#endif
         ) && internalTypeName.endsWith(">") ){
        align = alignof(QList<void*>);
    }
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    int metaType = QMetaType::type(qPrintable(internalTypeName));
    if(metaType!=QMetaType::UnknownType){
        QMetaType::TypeFlags typeFlags = QMetaType::typeFlags(metaType);
        if(typeFlags.testFlag(QMetaType::IsEnumeration)){
            int size = QMetaType::sizeOf(metaType);
            switch(size){
            case 1: align = 1; break;
            case 2: align = 2; break;
            case 4: align = 4; break;
            case 8: align = 8; break;
            }
        }
    }
#endif
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
        size_t sz = getValueSize(*t);
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
                size1 = isPointerType(instantiations[0]) ? sizeof(void*) : getValueSize(*t1);
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
                size2 = isPointerType(instantiations[1]) ? sizeof(void*) : getValueSize(*t2);
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
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
            || internalTypeName.startsWith("QLinkedList<")
#endif
        ) && internalTypeName.endsWith(">") ){
        return sizeof(QList<void*>);
    }
    return 0;
}

struct MetaData{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    MetaData(int _metaType,
             const QHashFunction& _hashFunction);
#endif
    MetaData(const QMetaType& _metaType,
             const QHashFunction& _hashFunction);
    MetaData(QMetaType&& _metaType = QMetaType(),
             QHashFunction&& _hashFunction = QHashFunction());
    MetaData(const MetaData& d);
    MetaData(MetaData&& d);
    QMetaType metaType;
    QHashFunction hashFunction;
};

struct Storages{
    QThreadStorage<MetaData> storage[2];
};

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
MetaData::MetaData(int _metaType,
         const QHashFunction& _hashFunction)
    : metaType(_metaType),
      hashFunction(_hashFunction)
{}
#endif

MetaData::MetaData(const QMetaType& _metaType,
         const QHashFunction& _hashFunction
        )
    : metaType(
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
          _metaType.id()
#else
          _metaType
#endif
        ),
      hashFunction(_hashFunction)
{}

MetaData::MetaData(QMetaType&& _metaType,
         QHashFunction&& _hashFunction)
    : metaType(
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
          _metaType.id()
#else
          std::move(_metaType)
#endif
        ),
      hashFunction(std::move(_hashFunction))
{}

MetaData::MetaData(const MetaData& d)
    : metaType(
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
            d.metaType.id()
#else
            d.metaType
#endif
        ),
      hashFunction(d.hashFunction)
{}

MetaData::MetaData(MetaData&& d)
    : metaType(
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
            d.metaType.id()
#else
            std::move(d.metaType)
#endif
        ),
      hashFunction(std::move(d.hashFunction))
{}

Q_GLOBAL_STATIC(Storages, gCurrentMetaData)

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

AbstractMetaTypeInfoLocker::AbstractMetaTypeInfoLocker(int index, const QMetaType& _metaType, const QHashFunction& _hashFunction)
    : m_index(index)
{
    const MetaData& md = currentMetaData(index);
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    m_metaType = md.metaType.id();
#else
    m_metaType = md.metaType;
#endif
    m_hashFunction = md.hashFunction;
    setCurrentMetaData(m_index, MetaData(_metaType, _hashFunction));
}

AbstractMetaTypeInfoLocker::~AbstractMetaTypeInfoLocker(){
    setCurrentMetaData(m_index, MetaData(m_metaType, m_hashFunction));
}

}//namespace ContainerAccessAPI

void ContainerAccessAPI::constructContainerElement(uint i, void* ptr, const void* copy)
{
    const MetaData& metaData = currentMetaData(i);
    Q_ASSERT(metaData.metaType.isValid());
//    if(copy)
//        fprintf(stderr, "copy %s from %p at %p\n", metaData.metaType.name().data(), copy, ptr);
//    else
//        fprintf(stderr, "creating %s at %p\n", metaData.metaType.name().data(), ptr);
    metaData.metaType.construct(ptr, copy);
}

void ContainerAccessAPI::destructContainerElement(uint i, void* ptr)
{
    const MetaData& metaData = currentMetaData(i);
    Q_ASSERT(metaData.metaType.isValid());
//    fprintf(stderr, "delete %s at %p\n", QMetaType::typeName(metaData.metaType), ptr);
    metaData.metaType.destruct(ptr);
}

QDebug ContainerAccessAPI::containerElementDebugStream(QDebug debug, uint i, const void* ptr)
{
    const MetaData& metaData = currentMetaData(i);
    Q_ASSERT(metaData.metaType.isValid());
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    CoreAPI::appendToDebugStream(debug, metaData.metaType.id(), ptr);
#else
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
#endif
    return debug;
}

QDataStream & ContainerAccessAPI::containerElementDataStreamOut(QDataStream & stream, uint i, const void* ptr)
{
    const MetaData& metaData = currentMetaData(i);
    Q_ASSERT(metaData.metaType.isValid());
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    if(!QMetaType::save(stream, metaData.metaType.id(), ptr)){
#else
    if(!metaData.metaType.save(stream, ptr)){
#endif
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

QDataStream & ContainerAccessAPI::containerElementDataStreamIn(QDataStream & stream, uint i, void* ptr)
{
    const MetaData& metaData = currentMetaData(i);
    Q_ASSERT(metaData.metaType.isValid());
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    if(!QMetaType::load(stream, metaData.metaType.id(), ptr)){
#else
    if(!metaData.metaType.load(stream, ptr)){
#endif
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

hash_type ContainerAccessAPI::containerElementHash(uint i, const void* ptr, hash_type seed)
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

bool ContainerAccessAPI::containerElementEqual(uint i, const void* ptr, const void* ptr2)
{
    const MetaData& metaData = currentMetaData(i);
    Q_ASSERT(metaData.metaType.isValid());
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    return isEquals(metaData.metaType, ptr, ptr2);
#else
    return metaData.metaType.equals(ptr, ptr2);
#endif
}

bool ContainerAccessAPI::containerElementLess(uint i, const void* ptr, const void* ptr2)
{
    const MetaData& metaData = currentMetaData(i);
    Q_ASSERT(metaData.metaType.isValid());
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    return isLessThan(metaData.metaType, ptr, ptr2);
#else
    QPartialOrdering po = metaData.metaType.compare(ptr, ptr2);
    return po == QPartialOrdering::Less;
#endif
}

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
void destruct(uint index,
              int metaType,
              QHashFunction hashFunction,
              QMetaType::Destructor destructor,
              void * ptr){
    MetaData oldMetaData = currentMetaData(index);
    setCurrentMetaData(index, MetaData(metaType, hashFunction));
    destructor(ptr);
    setCurrentMetaData(index, oldMetaData);
}

QMetaType::Destructor wrapDestructor(int metaType,
                                    QHashFunction hashFunction,
                                    uint index,
                                    QMetaType::Destructor destructor,
                                    uint hash){
    return qtjambi_function_pointer<16,void(void*)>([index, metaType, hashFunction, destructor](void * ptr){
        destruct(index, metaType, hashFunction, destructor, ptr);
    }, hash);
}

void* construct(uint index,
               int metaType,
               QHashFunction hashFunction,
               QMetaType::Constructor constructor, void *where, const void * copy){
    MetaData oldMetaData = currentMetaData(index);
    setCurrentMetaData(index, MetaData(metaType, hashFunction));
    void * result = constructor(where, copy);
    setCurrentMetaData(index, oldMetaData);
    return result;
}

QMetaType::Constructor wrapConstructor(int metaType,
                                      QHashFunction hashFunction,
                                      uint index,
                                      QMetaType::Constructor constructor, uint hash){
    return qtjambi_function_pointer<16,void*(void*, const void *)>([index, metaType, hashFunction, constructor](void *where, const void * copy)->void *{
        return construct(index, metaType, hashFunction, constructor, where, copy);
    }, hash);
}

void _saveOperator(uint index,
               int metaType,
               QHashFunction hashFunction,
               QMetaType::SaveOperator saveOperator, QDataStream & stream, const void * data){
    MetaData oldMetaData = currentMetaData(index);
    setCurrentMetaData(index, MetaData(metaType, hashFunction));
    saveOperator(stream, data);
    setCurrentMetaData(index, oldMetaData);
}

QMetaType::SaveOperator wrapSaveOperator(int metaType,
                                      QHashFunction hashFunction,
                                      uint index,
                                      QMetaType::SaveOperator saveOperator, uint hash){
    return qtjambi_function_pointer<16,void(QDataStream &, const void *)>([index, metaType, hashFunction, saveOperator](QDataStream & stream, const void * data){
        _saveOperator(index, metaType, hashFunction, saveOperator, stream, data);
    }, hash);
}

void _loadOperator(uint index,
               int metaType,
               QHashFunction hashFunction,
               QMetaType::LoadOperator loadOperator, QDataStream & stream, void * data){
    MetaData oldMetaData = currentMetaData(index);
    setCurrentMetaData(index, MetaData(metaType, hashFunction));
    loadOperator(stream, data);
    setCurrentMetaData(index, oldMetaData);
}

QMetaType::LoadOperator wrapLoadOperator(int metaType,
                                      QHashFunction hashFunction,
                                      uint index,
                                      QMetaType::LoadOperator saveOperator, uint hash){
    return qtjambi_function_pointer<16,void(QDataStream &, void *)>([index, metaType, hashFunction, saveOperator](QDataStream & stream, void * data){
        _loadOperator(index, metaType, hashFunction, saveOperator, stream, data);
    }, hash);
}

void destruct(int metaType1,
              QHashFunction hashFunction1,
              int metaType2,
              QHashFunction hashFunction2,
              QMetaType::Destructor destructor,
              void * ptr){
    MetaData oldMetaData1 = currentMetaData(0);
    setCurrentMetaData(0, MetaData(metaType1, hashFunction1));
    MetaData oldMetaData2 = currentMetaData(1);
    setCurrentMetaData(1, MetaData(metaType2, hashFunction2));
    destructor(ptr);
    setCurrentMetaData(0, oldMetaData1);
    setCurrentMetaData(1, oldMetaData2);
}

QMetaType::Destructor wrapDestructor(int metaType1,
                                    QHashFunction hashFunction1,
                                    int metaType2,
                                    QHashFunction hashFunction2,
                                    QMetaType::Destructor destructor,
                                    uint hash){
    return qtjambi_function_pointer<16,void(void*)>([metaType1, hashFunction1,
                                                                          metaType2, hashFunction2,
                                                                          destructor](void * ptr){
        destruct(metaType1, hashFunction1,
                 metaType2, hashFunction2,
                 destructor, ptr);
    }, hash);
}

void* construct(int metaType1,
                QHashFunction hashFunction1,
                int metaType2,
                QHashFunction hashFunction2,
                QMetaType::Constructor constructor, void *where, const void * copy){
    MetaData oldMetaData1 = currentMetaData(0);
    setCurrentMetaData(0, MetaData(metaType1, hashFunction1));
    MetaData oldMetaData2 = currentMetaData(1);
    setCurrentMetaData(1, MetaData(metaType2, hashFunction2));
    void * result = constructor(where, copy);
    setCurrentMetaData(0, oldMetaData1);
    setCurrentMetaData(1, oldMetaData2);
    return result;
}

QMetaType::Constructor wrapConstructor(int metaType1,
                                      QHashFunction hashFunction1,
                                      int metaType2,
                                      QHashFunction hashFunction2,
                                      QMetaType::Constructor constructor, uint hash){
    return qtjambi_function_pointer<16,void*(void*, const void *)>([metaType1, hashFunction1,
                                                                                         metaType2, hashFunction2,
                                                                                         constructor](void *where, const void * copy)->void *{
        return construct(metaType1, hashFunction1,
                         metaType2, hashFunction2,
                         constructor, where, copy);
    }, hash);
}

void _saveOperator(int metaType1,
                   QHashFunction hashFunction1,
                   int metaType2,
                   QHashFunction hashFunction2,
                   QMetaType::SaveOperator saveOperator, QDataStream & stream, const void * data){
    MetaData oldMetaData1 = currentMetaData(0);
    setCurrentMetaData(0, MetaData(metaType1, hashFunction1));
    MetaData oldMetaData2 = currentMetaData(1);
    setCurrentMetaData(1, MetaData(metaType2, hashFunction2));
    saveOperator(stream, data);
    setCurrentMetaData(0, oldMetaData1);
    setCurrentMetaData(1, oldMetaData2);
}

QMetaType::SaveOperator wrapSaveOperator(int metaType1,
                                         QHashFunction hashFunction1,
                                         int metaType2,
                                         QHashFunction hashFunction2,
                                         QMetaType::SaveOperator saveOperator, uint hash){
    return qtjambi_function_pointer<16,void(QDataStream &, const void *)>([metaType1, hashFunction1, metaType2, hashFunction2, saveOperator](QDataStream & stream, const void * data){
        _saveOperator(metaType1, hashFunction1, metaType2, hashFunction2, saveOperator, stream, data);
    }, hash);
}

void _loadOperator(int metaType1,
                   QHashFunction hashFunction1,
                   int metaType2,
                   QHashFunction hashFunction2,
                   QMetaType::LoadOperator loadOperator, QDataStream & stream, void * data){
    MetaData oldMetaData1 = currentMetaData(0);
    setCurrentMetaData(0, MetaData(metaType1, hashFunction1));
    MetaData oldMetaData2 = currentMetaData(1);
    setCurrentMetaData(1, MetaData(metaType2, hashFunction2));
    loadOperator(stream, data);
    setCurrentMetaData(0, oldMetaData1);
    setCurrentMetaData(1, oldMetaData2);
}

QMetaType::LoadOperator wrapLoadOperator(int metaType1,
                                         QHashFunction hashFunction1,
                                         int metaType2,
                                         QHashFunction hashFunction2,
                                         QMetaType::LoadOperator saveOperator, uint hash){
    return qtjambi_function_pointer<16,void(QDataStream &, void *)>([metaType1, hashFunction1, metaType2, hashFunction2, saveOperator](QDataStream & stream, void * data){
        _loadOperator(metaType1, hashFunction1, metaType2, hashFunction2, saveOperator, stream, data);
    }, hash);
}

int QtJambiPrivate::registerSequentialContainerType(const QByteArray& typeName, size_t containerSize, size_t containerAlign, bool isPointer, const QMetaType& metaType,
                                      QMetaType::Destructor destructor,
                                      QMetaType::Constructor constructor,
                                      QMetaType::SaveOperator saveOperator,
                                      QMetaType::LoadOperator loadOperator,
                                      const QtPrivate::AbstractDebugStreamFunction * debugStreamFunction,
                                      const QtPrivate::AbstractComparatorFunction * comparator,
                                      ConverterFunctionFactory1 createConverterFunction,
                                    HashWrapper hashWrapper)
{
    QtJambiTypeManager::registerInternalAlignment(typeName, containerAlign);
    QHashFunction hashFunction = QtJambiTypeManager::findHashFunction(isPointer, metaType.id());
    if(!isPointer){
        destructor = wrapDestructor(metaType.id(), hashFunction, 0, destructor, qHash(typeName));
        constructor = wrapConstructor(metaType.id(), hashFunction, 0, constructor, qHash(typeName));
        saveOperator = wrapSaveOperator(metaType.id(), hashFunction, 0, saveOperator, qHash(typeName));
        loadOperator = wrapLoadOperator(metaType.id(), hashFunction, 0, loadOperator, qHash(typeName));
    }else{
        saveOperator = nullptr;
        loadOperator = nullptr;
    }
    int newMetaType = QMetaType::registerType(typeName,
                            nullptr,
                            nullptr,
                            destructor,
                            constructor,
                            int(containerSize),
                            QMetaType::MovableType | QMetaType::NeedsDestruction | QMetaType::NeedsConstruction,
                            nullptr
                        );
    if(QtJambiTypeManager::hasRegisteredComparators(metaType.id()))
        RegistryAPI::registerComparator(comparator, newMetaType);
    if(saveOperator && loadOperator)
        QMetaType::registerStreamOperators(newMetaType, saveOperator, loadOperator);
    if(!QMetaType::hasRegisteredConverterFunction(newMetaType, qMetaTypeId<QtMetaTypePrivate::QSequentialIterableImpl>()))
        registerConverter(createConverterFunction(metaType.id()), newMetaType, qMetaTypeId<QtMetaTypePrivate::QSequentialIterableImpl>());
    if(debugStreamFunction && debugStreamFunction->stream){
        struct DebugStreamFunction : QtPrivate::AbstractDebugStreamFunction{
            DebugStreamFunction(int _metaType, const QHashFunction& _hashFunction, const QtPrivate::AbstractDebugStreamFunction* _debugFunction)
                : QtPrivate::AbstractDebugStreamFunction(&debugStream, &destroy), metaType(_metaType), hashFunction(_hashFunction), debugFunction(_debugFunction) {}

            int metaType;
            QHashFunction hashFunction;
            const QtPrivate::AbstractDebugStreamFunction* debugFunction;

            static void debugStream(const QtPrivate::AbstractDebugStreamFunction *f, QDebug& d, const void * ptr){
                const DebugStreamFunction* df = static_cast<const DebugStreamFunction*>(f);
                MetaData oldMetaData = currentMetaData(0);
                setCurrentMetaData(0, MetaData(df->metaType, df->hashFunction));
                df->debugFunction->stream(df->debugFunction, d, ptr);
                setCurrentMetaData(0, oldMetaData);
            }

            static void destroy(QtPrivate::AbstractDebugStreamFunction *f){
                DebugStreamFunction* df = static_cast<DebugStreamFunction*>(f);
                if(df->debugFunction->destroy){
                    df->debugFunction->destroy(const_cast<QtPrivate::AbstractDebugStreamFunction *>(df->debugFunction));
                }
                delete df;
            }
        };
        RegistryAPI::registerDebugStreamOperator(new DebugStreamFunction(metaType.id(), hashFunction, debugStreamFunction), newMetaType);
    }
    if(!hashFunctionByMetaType(newMetaType)){
        int _metaType = metaType.id();
        insertHashFunctionByMetaType(newMetaType,
                                            [_metaType, hashFunction, hashWrapper]
                                            (const void* ptr, hash_type seed)->hash_type{
                                                if(ptr){
                                                    MetaData oldMetaData = currentMetaData(0);
                                                    setCurrentMetaData(0, MetaData(_metaType, hashFunction));
                                                    hash_type result = hashWrapper(ptr, seed, hashFunction);
                                                    setCurrentMetaData(0, oldMetaData);
                                                    return result;
                                                }else{
                                                    return 0;
                                                }
                                            });
    }
    return newMetaType;
}

int QtJambiPrivate::registerAssociativeContainerType(const QByteArray& typeName,
                                      size_t containerSize,
                                      size_t containerAlign,
                                      bool isPointer1, const QMetaType& metaType1,
                                      bool isPointer2, const QMetaType& metaType2,
                                      QMetaType::Destructor destructor,
                                      QMetaType::Constructor constructor,
                                      QMetaType::SaveOperator saveOperator,
                                      QMetaType::LoadOperator loadOperator,
                                      const QtPrivate::AbstractDebugStreamFunction * debugStreamFunction,
                                      const QtPrivate::AbstractComparatorFunction * comparator,
                                      bool isPair,
                                      ConverterFunctionFactory2 createConverterFunction,
                                      HashWrapper2 hashWrapper)
{
    QtJambiTypeManager::registerInternalAlignment(typeName, containerAlign);
    QHashFunction hashFunction1 = QtJambiTypeManager::findHashFunction(isPointer1, metaType1.id());
    QHashFunction hashFunction2 = QtJambiTypeManager::findHashFunction(isPointer2, metaType2.id());
    if(!isPointer1 && !isPointer2){
        destructor = wrapDestructor(metaType1.id(), hashFunction1,
                                    metaType2.id(), hashFunction2,
                                    destructor, qHash(typeName));
        constructor = wrapConstructor(metaType1.id(), hashFunction1,
                                      metaType2.id(), hashFunction2,
                                      constructor, qHash(typeName));
        saveOperator = wrapSaveOperator(metaType1.id(), hashFunction1,
                                        metaType2.id(), hashFunction2,
                                        saveOperator, qHash(typeName));
        loadOperator = wrapLoadOperator(metaType1.id(), hashFunction1,
                                        metaType2.id(), hashFunction2,
                                        loadOperator, qHash(typeName));
    }else{
        if(!isPointer1){
            destructor = wrapDestructor(metaType1.id(), hashFunction1, 0, destructor, qHash(typeName));
            constructor = wrapConstructor(metaType1.id(), hashFunction1, 0, constructor, qHash(typeName));
        }else if(!isPointer2){
            destructor = wrapDestructor(metaType2.id(), hashFunction2, 1, destructor, qHash(typeName));
            constructor = wrapConstructor(metaType2.id(), hashFunction2, 1, constructor, qHash(typeName));
        }
        saveOperator = nullptr;
        loadOperator = nullptr;
    }
    int newMetaType = QMetaType::registerType(typeName,
                            nullptr,
                            nullptr,
                            destructor,
                            constructor,
                            int(containerSize),
                            QMetaType::MovableType | QMetaType::NeedsDestruction | QMetaType::NeedsConstruction,
                            nullptr
                        );
    if(QtJambiTypeManager::hasRegisteredComparators(metaType1.id()) && QtJambiTypeManager::hasRegisteredComparators(metaType2.id()))
        RegistryAPI::registerComparator(comparator, newMetaType);
    if(!QMetaType::hasRegisteredConverterFunction(newMetaType, isPair ? qMetaTypeId<QtMetaTypePrivate::QPairVariantInterfaceImpl>() : qMetaTypeId<QtMetaTypePrivate::QAssociativeIterableImpl>()))
        registerConverter(createConverterFunction(metaType1.id(), metaType2.id()), newMetaType, isPair ? qMetaTypeId<QtMetaTypePrivate::QPairVariantInterfaceImpl>() : qMetaTypeId<QtMetaTypePrivate::QAssociativeIterableImpl>());
    if(saveOperator && loadOperator)
        QMetaType::registerStreamOperators(newMetaType, saveOperator, loadOperator);
    if(debugStreamFunction && debugStreamFunction->stream){
        struct DebugStreamFunction : QtPrivate::AbstractDebugStreamFunction{
            DebugStreamFunction(int _metaType1, const QHashFunction& _hashFunction1,
                                int _metaType2, const QHashFunction& _hashFunction2, const QtPrivate::AbstractDebugStreamFunction* _debugFunction)
                : QtPrivate::AbstractDebugStreamFunction(&debugStream, &destroy),
                  metaType1(_metaType1), hashFunction1(_hashFunction1),
                  metaType2(_metaType2), hashFunction2(_hashFunction2),
                  debugFunction(_debugFunction) {}

            int metaType1;
            QHashFunction hashFunction1;
            int metaType2;
            QHashFunction hashFunction2;
            const QtPrivate::AbstractDebugStreamFunction* debugFunction;

            static void debugStream(const QtPrivate::AbstractDebugStreamFunction *f, QDebug& d, const void * ptr){
                const DebugStreamFunction* df = static_cast<const DebugStreamFunction*>(f);
                MetaData oldMetaData1 = currentMetaData(0);
                MetaData oldMetaData2 = currentMetaData(1);
                setCurrentMetaData(0, MetaData(df->metaType1, df->hashFunction1));
                setCurrentMetaData(1, MetaData(df->metaType2, df->hashFunction2));
                df->debugFunction->stream(df->debugFunction, d, ptr);
                setCurrentMetaData(0, oldMetaData1);
                setCurrentMetaData(1, oldMetaData2);
            }

            static void destroy(QtPrivate::AbstractDebugStreamFunction *f){
                DebugStreamFunction* df = static_cast<DebugStreamFunction*>(f);
                if(df->debugFunction->destroy){
                    df->debugFunction->destroy(const_cast<QtPrivate::AbstractDebugStreamFunction *>(df->debugFunction));
                }
                delete df;
            }
        };
        RegistryAPI::registerDebugStreamOperator(new DebugStreamFunction(metaType1.id(), hashFunction1, metaType2.id(), hashFunction2, debugStreamFunction), newMetaType);
    }
    if(!hashFunctionByMetaType(newMetaType)){
        int _metaType1 = metaType1.id();
        int _metaType2 = metaType2.id();
        insertHashFunctionByMetaType(newMetaType,
                                            [_metaType1, hashFunction1,
                                             _metaType2, hashFunction2, hashWrapper]
                                            (const void* ptr, hash_type seed)->hash_type{
                                                if(ptr){
                                                    MetaData oldMetaData1 = currentMetaData(0);
                                                    setCurrentMetaData(0, MetaData(_metaType1, hashFunction1));
                                                    MetaData oldMetaData2 = currentMetaData(1);
                                                    setCurrentMetaData(1, MetaData(_metaType2, hashFunction2));
                                                    hash_type result = hashWrapper(ptr, seed, hashFunction1, hashFunction2);
                                                    setCurrentMetaData(0, oldMetaData1);
                                                    setCurrentMetaData(1, oldMetaData2);
                                                    return result;
                                                }else{
                                                    return 0;
                                                }
                                            });
    }
    return newMetaType;
}


#else

struct ContainerInfo{
    bool isPointer;
    QMetaType metaType;
    QHashFunction hashFunction;
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

struct BiContainerInfo{
    bool isPointer1;
    QMetaType metaType1;
    QHashFunction hashFunction1;
    bool isPointer2;
    QMetaType metaType2;
    QHashFunction hashFunction2;
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

typedef QHash<const QtPrivate::QMetaTypeInterface*,ContainerInfo> ContainerInfoHash;
typedef QHash<const QtPrivate::QMetaTypeInterface*,BiContainerInfo> BiContainerInfoHash;
Q_GLOBAL_STATIC(ContainerInfoHash, gContainerInfos)
Q_GLOBAL_STATIC(BiContainerInfoHash, gBiContainerInfos)

void qtjambi_container_destruct(const QtPrivate::QMetaTypeInterface * metaTypeInterface, void* ptr){
    if(gContainerInfos->contains(metaTypeInterface)){
        const ContainerInfo& info = (*gContainerInfos)[metaTypeInterface];
        if(info.isPointer){
            info.dtor(metaTypeInterface, ptr);
        }else{
            MetaData oldMetaData = currentMetaData(0);
            setCurrentMetaData(0, MetaData(info.metaType, info.hashFunction));
            info.dtor(metaTypeInterface, ptr);
            setCurrentMetaData(0, oldMetaData);
        }
    }else if(gBiContainerInfos->contains(metaTypeInterface)){
        const BiContainerInfo& info = (*gBiContainerInfos)[metaTypeInterface];
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

void container_default_construct(const QtPrivate::QMetaTypeInterface * metaTypeInterface, void *where){
    if(gContainerInfos->contains(metaTypeInterface)){
        const ContainerInfo& info = (*gContainerInfos)[metaTypeInterface];
        if(info.isPointer){
            info.defaultCtr(metaTypeInterface, where);
        }else{
            const ContainerInfo& info = (*gContainerInfos)[metaTypeInterface];
            MetaData oldMetaData = currentMetaData(0);
            setCurrentMetaData(0, MetaData(info.metaType, info.hashFunction));
            info.defaultCtr(metaTypeInterface, where);
            setCurrentMetaData(0, oldMetaData);
        }
    }else if(gBiContainerInfos->contains(metaTypeInterface)){
        const BiContainerInfo& info = (*gBiContainerInfos)[metaTypeInterface];
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

void qtjambi_container_copy_construct(const QtPrivate::QMetaTypeInterface * metaTypeInterface, void *where, const void * copy){
    if(gContainerInfos->contains(metaTypeInterface)){
        const ContainerInfo& info = (*gContainerInfos)[metaTypeInterface];
        if(info.isPointer){
            info.copyCtr(metaTypeInterface, where, copy);
        }else{
            MetaData oldMetaData = currentMetaData(0);
            setCurrentMetaData(0, MetaData(info.metaType, info.hashFunction));
            info.copyCtr(metaTypeInterface, where, copy);
            setCurrentMetaData(0, oldMetaData);
        }
    }else if(gBiContainerInfos->contains(metaTypeInterface)){
        const BiContainerInfo& info = (*gBiContainerInfos)[metaTypeInterface];
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

void qtjambi_container_move_construct(const QtPrivate::QMetaTypeInterface * metaTypeInterface, void *where, void * copy){
    if(gContainerInfos->contains(metaTypeInterface)){
        const ContainerInfo& info = (*gContainerInfos)[metaTypeInterface];
        if(info.isPointer){
            info.moveCtr(metaTypeInterface, where, copy);
        }else{
            MetaData oldMetaData = currentMetaData(0);
            setCurrentMetaData(0, MetaData(info.metaType, info.hashFunction));
            info.moveCtr(metaTypeInterface, where, copy);
            setCurrentMetaData(0, oldMetaData);
        }
    }else if(gBiContainerInfos->contains(metaTypeInterface)){
        const BiContainerInfo& info = (*gBiContainerInfos)[metaTypeInterface];
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

bool qtjambi_container_equals(const QtPrivate::QMetaTypeInterface * metaTypeInterface, const void *container1, const void *container2){
    if(gContainerInfos->contains(metaTypeInterface)){
        const ContainerInfo& info = (*gContainerInfos)[metaTypeInterface];
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
    }else if(gBiContainerInfos->contains(metaTypeInterface)){
        const BiContainerInfo& info = (*gBiContainerInfos)[metaTypeInterface];
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
    return false;
}

bool container_lessThan(const QtPrivate::QMetaTypeInterface * metaTypeInterface, const void *container1, const void *container2){
    if(gContainerInfos->contains(metaTypeInterface)){
        const ContainerInfo& info = (*gContainerInfos)[metaTypeInterface];
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
    }else if(gBiContainerInfos->contains(metaTypeInterface)){
        const BiContainerInfo& info = (*gBiContainerInfos)[metaTypeInterface];
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
    return false;
}

void qtjambi_container_debugStream(const QtPrivate::QMetaTypeInterface * metaTypeInterface, QDebug & debug, const void *container){
    if(gContainerInfos->contains(metaTypeInterface)){
        const ContainerInfo& info = (*gContainerInfos)[metaTypeInterface];
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
    }else if(gBiContainerInfos->contains(metaTypeInterface)){
        const BiContainerInfo& info = (*gBiContainerInfos)[metaTypeInterface];
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

void qtjambi_container_dataStreamOut(const QtPrivate::QMetaTypeInterface * metaTypeInterface, QDataStream & dataStream, const void *container){
    if(gContainerInfos->contains(metaTypeInterface)){
        const ContainerInfo& info = (*gContainerInfos)[metaTypeInterface];
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
    }else if(gBiContainerInfos->contains(metaTypeInterface)){
        const BiContainerInfo& info = (*gBiContainerInfos)[metaTypeInterface];
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

void qtjambi_container_dataStreamIn(const QtPrivate::QMetaTypeInterface * metaTypeInterface, QDataStream & dataStream, void *container){
    if(gContainerInfos->contains(metaTypeInterface)){
        const ContainerInfo& info = (*gContainerInfos)[metaTypeInterface];
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
    }else if(gBiContainerInfos->contains(metaTypeInterface)){
        const BiContainerInfo& info = (*gBiContainerInfos)[metaTypeInterface];
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

const char* registerMetaTypeName(const QByteArray& typeName);

hash_type computeHash(const QtMetaContainerPrivate::QMetaSequenceInterface& defaultInterface, const QMetaType& elementMetaType){
    QList<quintptr> range{quintptr(&defaultInterface),quintptr(elementMetaType.id())};
    return qHashRange(range.begin(), range.end());
}

hash_type computeHash(const QtMetaContainerPrivate::QMetaAssociationInterface& defaultInterface, const QMetaType& keyMetaType, const QMetaType& valueMetaType){
    QList<quintptr> range{quintptr(&defaultInterface),quintptr(keyMetaType.id()),quintptr(valueMetaType.id())};
    return qHashRange(range.begin(), range.end());
}

const QtMetaContainerPrivate::QMetaSequenceInterface& qtjambi_find_meta_sequence(QMetaType elementMetaType, const QtMetaContainerPrivate::QMetaSequenceInterface& defaultInterface)
{
    hash_type hash = computeHash(defaultInterface, elementMetaType);
    {
        QReadLocker locker(gCacheLock());
        Q_UNUSED(locker)
        if(gMetaSequenceHash->contains(hash))
           return (*gMetaSequenceHash)[hash];
    }
    {
        QWriteLocker locker(gCacheLock());
        Q_UNUSED(locker)
        if(gMetaSequenceHash->contains(hash))
           return (*gMetaSequenceHash)[hash];
        QtMetaContainerPrivate::QMetaSequenceInterface d(defaultInterface);
        d.valueMetaType = elementMetaType.iface();
        gMetaSequenceHash->insert(hash, d);
        return (*gMetaSequenceHash)[hash];
    }
}

const QtMetaContainerPrivate::QMetaAssociationInterface& qtjambi_find_meta_association(QMetaType metaType1, QHashFunction hashFunction1, QMetaType metaType2, QHashFunction hashFunction2, const QtMetaContainerPrivate::QMetaAssociationInterface& defaultInterface)
{
    hash_type hash = computeHash(defaultInterface, metaType1, metaType2);
    {
        QReadLocker locker(gCacheLock());
        Q_UNUSED(locker)
        if(gMetaAssociationHash->contains(hash))
           return (*gMetaAssociationHash)[hash];
    }
    {
        QWriteLocker locker(gCacheLock());
        Q_UNUSED(locker)
        if(gMetaAssociationHash->contains(hash))
           return (*gMetaAssociationHash)[hash];
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

        gMetaAssociationHash->insert(hash, d);
        return (*gMetaAssociationHash)[hash];
    }
}

int QtJambiPrivate::registerSequentialContainerType(const QByteArray& typeName, size_t containerSize, size_t containerAlign, bool isPointer, const QMetaType& metaType,
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
                                      HashWrapper hashWrapper)
{
    QHashFunction hashFunction = QtJambiTypeManager::findHashFunction(isPointer, metaType.id());
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
    QMetaType _newMetaType = createMetaType(typeName,
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
                                            /*.flags=*/ QMetaType::TypeFlags(QMetaType::MovableType | QMetaType::NeedsDestruction | QMetaType::NeedsConstruction),
                                            /*.metaObject=*/ nullptr,
                                            /*.metaObject=*/ nullptr
                                    );
    gContainerInfos->insert(_newMetaType.iface(), {isPointer,
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
    int newMetaType = _newMetaType.id();
    if(defaultInterface){
        const QMetaType to = QMetaType::fromType<QIterable<QMetaSequence>>();
        if (createMetaSequenceConstIterator && !QMetaType::hasRegisteredConverterFunction(_newMetaType, to)) {
            QMetaType _metaType = metaType;
            QMetaType::registerConverterFunction([_metaType, defaultInterface, createMetaSequenceConstIterator](const void *src, void *target) -> bool {
                const QtMetaContainerPrivate::QMetaSequenceInterface &d = qtjambi_find_meta_sequence(_metaType, *defaultInterface);
                return createMetaSequenceConstIterator(src, target, &d);
            }, _newMetaType, to);
        }
        if (createMetaSequenceIterator && !QMetaType::hasRegisteredMutableViewFunction(_newMetaType, to)) {
            QMetaType _metaType = metaType;
            QMetaType::registerMutableViewFunction([_metaType, defaultInterface, createMetaSequenceIterator](void *src, void *target) -> bool {
                const QtMetaContainerPrivate::QMetaSequenceInterface &d = qtjambi_find_meta_sequence(_metaType, *defaultInterface);
                return createMetaSequenceIterator(src, target, &d);
            }, _newMetaType, to);
        }
    }
    if(!hashFunctionByMetaType(newMetaType)){
        insertHashFunctionByMetaType(newMetaType,
                                            [metaType, hashFunction, hashWrapper]
                                            (const void* ptr, hash_type seed)->hash_type{
                                                if(ptr){
                                                    MetaData oldMetaData = currentMetaData(0);
                                                    setCurrentMetaData(0, MetaData(metaType, hashFunction));
                                                    hash_type result = hashWrapper(ptr, seed, hashFunction);
                                                    setCurrentMetaData(0, oldMetaData);
                                                    return result;
                                                }else{
                                                    return 0;
                                                }
                                            });
    }
    return newMetaType;
}

int QtJambiPrivate::registerAssociativeContainerType(const QByteArray& typeName,
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
                                      HashWrapper2 hashWrapper)
{
    QHashFunction hashFunction1 = QtJambiTypeManager::findHashFunction(isPointer1, metaType1.id());
    QHashFunction hashFunction2 = QtJambiTypeManager::findHashFunction(isPointer2, metaType2.id());
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
    QMetaType _newMetaType = createMetaType(typeName,
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
                                            /*.flags=*/ QMetaType::TypeFlags(QMetaType::MovableType | QMetaType::NeedsDestruction | QMetaType::NeedsConstruction),
                                            /*.metaObject=*/ nullptr,
                                            /*.metaObject=*/ nullptr);
    gBiContainerInfos->insert(_newMetaType.iface(), {isPointer1,
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
    int newMetaType = _newMetaType.id();
    if(createMetaAssociationIterator){
        if(defaultInterface){
            static const QMetaType to = QMetaType::fromType<QIterable<QMetaAssociation>>();
            if (createMetaAssociationConstIterator && !QMetaType::hasRegisteredConverterFunction(_newMetaType, to)) {
                QMetaType _metaType1 = metaType1;
                QMetaType _metaType2 = metaType2;
                QMetaType::registerConverterFunction([_metaType1, hashFunction1, _metaType2, hashFunction2, defaultInterface, createMetaAssociationConstIterator](const void *src, void *target) -> bool {
                    const QtMetaContainerPrivate::QMetaAssociationInterface &d = qtjambi_find_meta_association(_metaType1, hashFunction1, _metaType2, hashFunction2, *defaultInterface);
                    return createMetaAssociationConstIterator(src, target, &d);
                }, _newMetaType, to);
            }
            if (createMetaAssociationIterator && !QMetaType::hasRegisteredMutableViewFunction(_newMetaType, to)) {
                QMetaType _metaType1 = metaType1;
                QMetaType _metaType2 = metaType2;
                QMetaType::registerMutableViewFunction([_metaType1, hashFunction1,_metaType2, hashFunction2, defaultInterface, createMetaAssociationIterator](void *src, void *target) -> bool {
                    const QtMetaContainerPrivate::QMetaAssociationInterface &d = qtjambi_find_meta_association(_metaType1, hashFunction1, _metaType2, hashFunction2, *defaultInterface);
                    return createMetaAssociationIterator(src, target, &d);
                }, _newMetaType, to);
            }
        }else if(pairAccessFactory){ // is QPair
            static const QMetaType to = QMetaType::fromType<QtMetaTypePrivate::QPairVariantInterfaceImpl>();
            if (!QMetaType::hasRegisteredConverterFunction(_newMetaType, to)) {
                QMetaType _metaType1 = metaType1;
                QMetaType _metaType2 = metaType2;
                QMetaType::registerConverterFunction([_metaType1, _metaType2, pairAccessFactory](const void *src, void *target) -> bool {
                    return pairAccessFactory(src, target, _metaType1, _metaType2);
                }, _newMetaType, to);
            }
        }
    }
    if(!hashFunctionByMetaType(newMetaType)){
        insertHashFunctionByMetaType(newMetaType,
                                            [metaType1, hashFunction1,
                                             metaType2, hashFunction2, hashWrapper]
                                            (const void* ptr, hash_type seed)->hash_type{
                                                if(ptr){
                                                    MetaData oldMetaData1 = currentMetaData(0);
                                                    setCurrentMetaData(0, MetaData(metaType1, hashFunction1));
                                                    MetaData oldMetaData2 = currentMetaData(1);
                                                    setCurrentMetaData(1, MetaData(metaType2, hashFunction2));
                                                    hash_type result = hashWrapper(ptr,  seed, hashFunction1, hashFunction2);
                                                    setCurrentMetaData(0, oldMetaData1);
                                                    setCurrentMetaData(1, oldMetaData2);
                                                    return result;
                                                }else{
                                                    return 0;
                                                }
                                            });
    }
    return newMetaType;
}
#endif

bool lessThan(JNIEnv *env, jobject k1, jobject k2);

QHashFunction QtJambiTypeManager::findHashFunction(bool isPointer, int metaType)
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
        QHashFunction f;
        if(typeId)
            f = registeredHashFunction(*typeId);
        if(!f)
            f = hashFunctionByMetaType(metaType);
        if(!f){
            QMetaType::TypeFlags typeFlags = QMetaType::typeFlags(metaType);
            if(typeFlags & QMetaType::IsEnumeration){
                switch(QMetaType::sizeOf(metaType)){
                case sizeof(qint8): f = [](const void* ptr, hash_type seed) -> hash_type{ return !ptr ? 0 : qHash(*reinterpret_cast<const qint8*>(ptr), seed);}; break;
                case sizeof(qint16): f = [](const void* ptr, hash_type seed) -> hash_type{ return !ptr ? 0 : qHash(*reinterpret_cast<const qint16*>(ptr), seed);}; break;
                case sizeof(qint32): f = [](const void* ptr, hash_type seed) -> hash_type{ return !ptr ? 0 : qHash(*reinterpret_cast<const qint32*>(ptr), seed);}; break;
                case sizeof(qint64): f = [](const void* ptr, hash_type seed) -> hash_type{ return !ptr ? 0 : qHash(*reinterpret_cast<const qint64*>(ptr), seed);}; break;
                }
            }
        }
        if(!f && metaTypeName.startsWith("QFlags<"))
            f = [](const void* ptr, hash_type seed) -> hash_type{ return !ptr ? 0 : qHash(*reinterpret_cast<const qint32*>(ptr), seed);};
        return f;
    }
}
