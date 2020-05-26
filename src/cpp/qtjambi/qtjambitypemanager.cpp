/****************************************************************************
**
** Copyright (C) 1992-2009 Nokia. All rights reserved.
** Copyright (C) 2009-2020 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
**
** This file is part of Qt Jambi.
**
** ** $BEGIN_LICENSE$
**
** GNU Lesser General Public License Usage
** This file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Nokia gives you certain
** additional rights. These rights are described in the Nokia Qt LGPL
** Exception version 1.0, included in the file LGPL_EXCEPTION.txt in this
** package.
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

#include "qtjambi_repository_p.h"
#include "qtjambi_jobjectwrapper.h"
#include "qtjambitypemanager_p.h"
#include "qtjambilink_p.h"
#include "qtjambi_core.h"
#include "qtjambi_cast.h"
#include "qtjambi_registry_p.h"
#include "qtjambi_interfaces.h"
#include "qtjambi_functionpointer.h"

#include <cstring>
#include <QThread>
#include <QtCore>

#define CONVERT_MULTIMAP 0

QString qtjambi_to_qstring(JNIEnv *env, jobject object);
uint hashSum(std::initializer_list<uint> list);
const QMetaObject * findMetaObject(const char* name);
const char* getInterface(const char*qt_interface);
jobject qtjambi_from_object_shared_pointer(JNIEnv *env, const char *className,
                            void* ptr_shared_pointer, PointerDeleter sharedPointerDeleter, PointerGetter sharedPointerGetter);

Q_GLOBAL_STATIC_WITH_ARGS(QReadWriteLock, gCacheLock, (QReadWriteLock::Recursive))
typedef QHash<uint, InternalToExternalConverter> InternalToExternalConverterHash;
typedef QHash<uint, ExternalToInternalConverter> ExternalToInternalConverterHash;
Q_GLOBAL_STATIC(InternalToExternalConverterHash, gInternalToExternalConverters)
Q_GLOBAL_STATIC(ExternalToInternalConverterHash, gExternalToInternalConverters)
typedef QMap<int, QHashFunction> HashFunctionHash;
typedef QMap<int, QLessFunction> LessFunctionHash;
typedef QMap<int, QEqualFunction> EqualFunctionHash;
typedef QMap<int, QMetaEnum> MetaEnumHash;
Q_GLOBAL_STATIC(HashFunctionHash, gHashFunctionByMetaTypeHash)
Q_GLOBAL_STATIC(LessFunctionHash, gLessFunctionByMetaTypeHash)
Q_GLOBAL_STATIC(EqualFunctionHash, gEqualFunctionByMetaTypeHash)
Q_GLOBAL_STATIC(MetaEnumHash, gMetaEnumByMetaTypeHash)
typedef QMap<QString, size_t> SizeByTypeHash;
Q_GLOBAL_STATIC(SizeByTypeHash, gSizeByTypeHash)
Q_GLOBAL_STATIC(SizeByTypeHash, gAlignmentByTypeHash)

void reduceMapInstantiations(QStringList& instantiations){
    if(instantiations.size()>2){
        QStringList parameterTypes;
        QString tmp;
        while(!instantiations.isEmpty()){
            if(tmp.isEmpty()){
                QString i1 = instantiations.takeFirst();
                int opener = i1.count('<');
                int closer = i1.count('>');
                if(opener == closer){
                    parameterTypes << i1;
                }else{
                    tmp = i1;
                }
            }else{
                tmp += QLatin1String(",") + instantiations.takeFirst();
                int opener = tmp.count('<');
                int closer = tmp.count('>');
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

void insertHashFunctionByMetaType(int type, QHashFunction fct){
    gHashFunctionByMetaTypeHash->insert(type, fct);
}

void insertLessFunctionByMetaType(int type, QLessFunction fct){
    gLessFunctionByMetaTypeHash->insert(type, fct);
}

void insertEqualFunctionByMetaType(int type, QEqualFunction fct){
    gEqualFunctionByMetaTypeHash->insert(type, fct);
}

uint pointerHashFunction(const void* ptr){ return !ptr ? 0 : qHash(quintptr(*reinterpret_cast<void*const*const>(ptr)));}
bool pointerLessFunction(const void* ptr1, const void* ptr2){ return !ptr1 || !ptr2 ? false : quintptr(*reinterpret_cast<void*const*const>(ptr1)) < quintptr(*reinterpret_cast<void*const*const>(ptr2));}
bool pointerEqualFunction(const void* ptr1, const void* ptr2){ return !ptr1 || !ptr2 ? false : quintptr(*reinterpret_cast<void*const*const>(ptr1)) == quintptr(*reinterpret_cast<void*const*const>(ptr2));}

QString QtJambiTypeManager::complexTypeOf(TypePattern type) {
    switch (type) {
        case Integer: return QLatin1String("java/lang/Integer");
        case Long: return QLatin1String("java/lang/Long");
        case Boolean: return QLatin1String("java/lang/Boolean");
        case Short: return QLatin1String("java/lang/Short");
        case Char: return QLatin1String("java/lang/Character");
        case Byte: return QLatin1String("java/lang/Byte");
        case Float: return QLatin1String("java/lang/Float");
        case Double: return QLatin1String("java/lang/Double");
        default:
            break;
    }

    qWarning("Cannot find complex type of type id '%d'", int(type));
    return QString();
}

QString QtJambiTypeManager::primitiveTypeOf(TypePattern type) {
    switch (type) {
        case Integer: return QLatin1String("int");
        case Long: return QLatin1String("long");
        case Boolean: return QLatin1String("boolean");
        case Short: return QLatin1String("short");
        case Char: return QLatin1String("char");
        case Byte: return QLatin1String("byte");
        case Float: return QLatin1String("float");
        case Double: return QLatin1String("double");
        default:
            break;
    }

    qWarning("Cannot find complex type of type id '%d'", int(type));
    return QString();
}

QString QtJambiTypeManager::mangle(const QString &_typeName) {
    int indirections = _typeName.count("[]");
    QString typeName = _typeName.left(_typeName.length() - indirections * 2);
    QString strIndirections(indirections, QLatin1Char('['));

    static const QHash<QString, QString> mangleHash{
        {"int","I"},
        {"long","J"},
        {"boolean","Z"},
        {"float","F"},
        {"double","D"},
        {"char","C"},
        {"short","S"},
        {"byte","B"},
        {"void","V"}
    };
    QString returned;
    {
        returned = mangleHash.value(typeName, QString());
    }
    if (returned.isEmpty()) {
        return strIndirections + QLatin1Char('L') + QString(typeName).replace(QLatin1Char('.'),
                QLatin1Char('/')) + QLatin1Char(';');
    } else {
        return strIndirections + returned;
    }
}

// Converts a JVM compatible type specification (J, N, I, Z, B, etc) to a
// type name.
QString QtJambiTypeManager::demangle(const QString &_typeName) {
    int indirections = _typeName.count("[");
    QString typeName = _typeName.right(_typeName.length() - indirections);
    QString strIndirections;
    QString brackets = QLatin1String("[]");
    for (int i = 0; i < indirections; ++i)
        strIndirections += brackets;

    switch (typeName.at(0).toLatin1()) {
        case 'I': return QLatin1String("int") + strIndirections;
        case 'Z': return QLatin1String("boolean") + strIndirections;
        case 'B': return QLatin1String("byte") + strIndirections;
        case 'J': return QLatin1String("long") + strIndirections;
        case 'C': return QLatin1String("char") + strIndirections;
        case 'D': return QLatin1String("double") + strIndirections;
        case 'F': return QLatin1String("float") + strIndirections;
        case 'S': return QLatin1String("short") + strIndirections;
        case 'V': return QLatin1String("void") + strIndirections;
        case 'L':
            return typeName.mid(1, typeName.length() - 2)
                   .replace(QLatin1Char('/'), QLatin1Char('.')) + strIndirections;
        default:
            qWarning("demangle: Unrecognized java type specification: %s",
                     qPrintable(typeName));
            break ;
    }

    return QString();
}

bool QtJambiTypeManager::isQtSubClass(JNIEnv *env, const QString &className, jobject classLoader) {
    return !closestQtSuperclass(env, className, classLoader).isEmpty();
}

bool QtJambiTypeManager::isQtClass(JNIEnv *env, const QString &className, jobject classLoader) {
    jclass clazz = resolveClass(env, qPrintable(className), classLoader);
    return (clazz != nullptr && Java::Private::QtJambi::QtJambiInternal.isGeneratedClass(env, clazz));
}

bool QtJambiTypeManager::isQObjectSubclass(JNIEnv *env, const QString &className, jobject classLoader) {
    jclass clazz = resolveClass(env, qPrintable(className), classLoader);
    return (clazz != nullptr && Java::Private::QtCore::QObject.isAssignableFrom(env, clazz));
}

QString QtJambiTypeManager::closestQtSuperclass(JNIEnv *env, const QString &className, jobject classLoader) {
    jclass clazz = resolveClass(env, qPrintable(className), classLoader);
    const SuperTypeInfos& infos = getSuperTypeInfos(env, clazz);
    if (!infos.isEmpty())
        return qtjambi_class_name(env, infos.first().javaClass).replace(QLatin1Char('.'), QLatin1Char('/'));
    else
        return QString();
}

bool lessThan(JNIEnv *env, jobject k1, jobject k2){
    QVariant v1(qtjambi_to_qvariant(env, k1));
    QVariant v2(qtjambi_to_qvariant(env, k2));
    if(v1.userType()==v2.userType()){
        if(QMetaType::hasRegisteredComparators(v1.userType())){
            return v1 < v2;
        }
    }
    if(JNIEnv *env = qtjambi_current_environment()){
        if(Java::Private::QtJambi::QtObjectInterface.isInstanceOf(env, k1) && Java::Private::QtJambi::QtObjectInterface.isInstanceOf(env, k2)){
            if(const QSharedPointer<QtJambiLink>& l1 = QtJambiLink::findLinkForJavaObject(env, k1)){
                if(const QSharedPointer<QtJambiLink>& l2 = QtJambiLink::findLinkForJavaObject(env, k2)){
                    return quintptr(l1->pointer())
                            < quintptr(l2->pointer());
                }
            }
        }
        jint h1 = Java::Private::Runtime::System.identityHashCode(env, k1);
        jint h2 = Java::Private::Runtime::System.identityHashCode(env, k2);
        return h1<h2;
    }
    return false;
}

/*!
    \internal
*/
QString QtJambiTypeManager::toJNISignature(const QString &signature, QString *name) {
    QString tmp(signature);

    // Remove generic parameters from signature
    int genericIdx = tmp.indexOf(QLatin1Char('<'));
    while (genericIdx >= 0) {
        int paramNum = 1;
        while (paramNum > 0 && genericIdx < tmp.length()) {
            tmp.remove(genericIdx, 1);
            paramNum += int(tmp.at(genericIdx) == QLatin1Char('<'));
            paramNum -= int(tmp.at(genericIdx) == QLatin1Char('>'));
        }

        tmp.replace(genericIdx, 1, QLatin1Char(' '));
        genericIdx = tmp.indexOf('<');
    }

    tmp = tmp.trimmed();

    int endName = tmp.indexOf(QLatin1Char('('), 1) - 1;
    int firstParen = tmp.indexOf(QLatin1Char(' '), 1);
    int startName = 0;
    QString returnType(QLatin1String("V"));
    if (firstParen >= 0 && firstParen < endName) {
        returnType = mangle(tmp.left(firstParen).trimmed());
        startName = firstParen + 1;
    }

    if (name != nullptr)
        *name = tmp.mid(startName, endName - startName + 1);

    int end = tmp.indexOf(QLatin1Char(')'), endName + 1);
    QString outSignature(QLatin1String("("));
    int pos = endName + 2;
    while (pos < end) {
        int comma = tmp.indexOf(QLatin1Char(','), pos + 1);
        if (comma <= 0)
            comma = end;
        outSignature += mangle(tmp.mid(pos, comma - pos).trimmed());
        pos = comma + 1;
    }
    outSignature += QLatin1String(")") + returnType;

    return outSignature;
}

QHashFunction hashFunctionByMetaType(int metaType)
{
    if(metaType!=QMetaType::UnknownType){
        return gHashFunctionByMetaTypeHash->value(metaType, QHashFunction());
    }
    return QHashFunction();
}

QLessFunction lessFunctionByMetaType(int metaType)
{
    if(metaType!=QMetaType::UnknownType){
        return gLessFunctionByMetaTypeHash->value(metaType, QLessFunction());
    }
    return QLessFunction();
}

QEqualFunction equalFunctionByMetaType(int metaType)
{
    if(metaType!=QMetaType::UnknownType){
        return gEqualFunctionByMetaTypeHash->value(metaType, QEqualFunction());
    }
    return QEqualFunction();
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

QString QtJambiTypeManager::processInternalTypeName(const QString &_internalTypeName,
        uint &_indirections, PointerType &_pointerType) {
    if(_internalTypeName.startsWith("QSharedPointer<") && _internalTypeName.endsWith(">")){
        _pointerType = QtJambiTypeManager::SharedPointer;
        QString internalTypeName = _internalTypeName.mid(15, _internalTypeName.length() - 16)+"*";
        int indirections = internalTypeName.count(QLatin1Char('*'));
        _indirections = indirections<0 ? 0 : uint(indirections);
        return internalTypeName.left(internalTypeName.length() - indirections);
    }else if(_internalTypeName.startsWith("QWeakPointer<") && _internalTypeName.endsWith(">")){
        _pointerType = QtJambiTypeManager::WeakPointer;
        QString internalTypeName = _internalTypeName.mid(13, _internalTypeName.length() - 14)+"*";
        int indirections = internalTypeName.count(QLatin1Char('*'));
        _indirections = indirections<0 ? 0 : uint(indirections);
        return internalTypeName.left(internalTypeName.length() - indirections);
    }else if(_internalTypeName.startsWith("QScopedPointer<") && _internalTypeName.endsWith(">")){
        _pointerType = QtJambiTypeManager::ScopedPointer;
        QString internalTypeName = _internalTypeName.mid(15, _internalTypeName.length() - 15)+"*";
        int indirections = internalTypeName.count(QLatin1Char('*'));
        _indirections = indirections<0 ? 0 : uint(indirections);
        return internalTypeName.left(internalTypeName.length() - indirections);
    }else if(_internalTypeName.startsWith("std::shared_ptr<") && _internalTypeName.endsWith(">")){
        _pointerType = QtJambiTypeManager::shared_ptr;
        QString internalTypeName = _internalTypeName.mid(16, _internalTypeName.length() - 17)+"*";
        int indirections = internalTypeName.count(QLatin1Char('*'));
        _indirections = indirections<0 ? 0 : uint(indirections);
        return internalTypeName.left(internalTypeName.length() - indirections);
    }else if(_internalTypeName.startsWith("std::weak_ptr<") && _internalTypeName.endsWith(">")){
        _pointerType = QtJambiTypeManager::weak_ptr;
        QString internalTypeName = _internalTypeName.mid(15, _internalTypeName.length() - 16)+"*";
        int indirections = internalTypeName.count(QLatin1Char('*'));
        _indirections = indirections<0 ? 0 : uint(indirections);
        return internalTypeName.left(internalTypeName.length() - indirections);
    }else if(_internalTypeName.startsWith("std::unique_ptr<") && _internalTypeName.endsWith(">")){
        _pointerType = QtJambiTypeManager::unique_ptr;
        QString internalTypeName = _internalTypeName.mid(16, _internalTypeName.length() - 17)+"*";
        int indirections = internalTypeName.count(QLatin1Char('*'));
        _indirections = indirections<0 ? 0 : uint(indirections);
        return internalTypeName.left(internalTypeName.length() - indirections);
    }else if(_internalTypeName.startsWith("std::initializer_list<") && _internalTypeName.endsWith(">")){
        QString internalTypeName = _internalTypeName.mid(22, _internalTypeName.length() - 23);
        _pointerType = QtJambiTypeManager::initializer_list;
        int indirections = internalTypeName.count(QLatin1Char('*'));
        _indirections = indirections<0 ? 0 : uint(indirections);
        return internalTypeName.left(internalTypeName.length() - indirections);
    }else if(_internalTypeName.endsWith("*")){
        QString internalTypeName = _internalTypeName;
        _indirections = 0;
        while(internalTypeName.endsWith("*")){
            _indirections++;
            internalTypeName.chop(1);
        }
        return internalTypeName;
    }
    return _internalTypeName;
}

QtJambiTypeManager::TypePattern QtJambiTypeManager::typeIdOfInternal(JNIEnv *env, const QString &_internalTypeName) {
    // For "void" we always return None.
    if (_internalTypeName == QLatin1String("void"))
        return None;
    if (_internalTypeName == QLatin1String("QVariant"))
        return VariantPattern;

    uint indirections = 0;
    QtJambiTypeManager::PointerType pointerType = QtJambiTypeManager::NoPointer;
    QString internalTypeName = processInternalTypeName(_internalTypeName, indirections, pointerType);
    int metaType = QMetaType::type(internalTypeName.toLatin1().constData());

    // If we have more than one indirection we always use the QNativePointer pattern
    if (indirections > 1){
        if(pointerType==QtJambiTypeManager::SharedPointer){
            return TypePattern(NativePointer | QtJambiTypeManager::SharedPointerWrapped);
        }else if(pointerType==QtJambiTypeManager::WeakPointer){
            return TypePattern(NativePointer | QtJambiTypeManager::WeakPointerWrapped);
        }else if(pointerType==QtJambiTypeManager::ScopedPointer){
            return TypePattern(NativePointer | QtJambiTypeManager::ScopedPointerWrapped);
        }else if(pointerType==QtJambiTypeManager::weak_ptr){
            return TypePattern(NativePointer | QtJambiTypeManager::WeakPtrWrapped);
        }else if(pointerType==QtJambiTypeManager::shared_ptr){
            return TypePattern(NativePointer | QtJambiTypeManager::SharedPtrWrapped);
        }else if(pointerType==QtJambiTypeManager::unique_ptr){
            return TypePattern(NativePointer | QtJambiTypeManager::UniquePtrWrapped);
        }else if(pointerType==QtJambiTypeManager::initializer_list){
            return TypePattern(NativePointer | QtJambiTypeManager::initializerListWrapper);
        }else
            return NativePointer;
    }
    const std::type_info* typeId = getTypeByQtName(internalTypeName);
    QString javaName;
    if(typeId){
        javaName = getJavaName(*typeId);
    }
    return typeIdOfInternal(env, javaName, pointerType, indirections, internalTypeName, metaType, nullptr);
}

QtJambiTypeManager::TypePattern QtJambiTypeManager::typeIdOfInternal(JNIEnv *env, const QString &javaName, const QString &internalTypeName, int metaType, jobject classLoader) {
    // For "void" we always return None.
    if (internalTypeName == QLatin1String("void"))
        return None;
    if (internalTypeName == QLatin1String("QVariant"))
        return VariantPattern;

    uint indirections = 0;
    QtJambiTypeManager::PointerType pointerType = QtJambiTypeManager::NoPointer;
    QString _internalTypeName = processInternalTypeName(internalTypeName, indirections, pointerType);
    if(_internalTypeName!=internalTypeName){
        int _metaType = QMetaType::type(_internalTypeName.toLatin1().constData());
        if(_metaType!=QMetaType::UnknownType){
            metaType = _metaType;
        }
    }
    return typeIdOfInternal(env, javaName, pointerType, indirections, _internalTypeName, metaType, classLoader);
}

QtJambiTypeManager::TypePattern QtJambiTypeManager::typeIdOfInternal(JNIEnv *env, const QString &javaName, QtJambiTypeManager::PointerType pointerType, uint indirections, const QString &internalTypeName, int metaType, jobject) {
    // Or we need to resolve the type
    quint64 type = 0;
    quint64 value_type_pattern = valueTypePattern(javaName);
    if(pointerType==QtJambiTypeManager::SharedPointer){
        type |= QtJambiTypeManager::SharedPointerWrapped;
    }else if(pointerType==QtJambiTypeManager::WeakPointer){
        type |= QtJambiTypeManager::WeakPointerWrapped;
    }else if(pointerType==QtJambiTypeManager::ScopedPointer){
        type |= QtJambiTypeManager::ScopedPointerWrapped;
    }else if(pointerType==QtJambiTypeManager::weak_ptr){
        type |= QtJambiTypeManager::WeakPtrWrapped;
    }else if(pointerType==QtJambiTypeManager::shared_ptr){
        type |= QtJambiTypeManager::SharedPtrWrapped;
    }else if(pointerType==QtJambiTypeManager::unique_ptr){
        type |= QtJambiTypeManager::UniquePtrWrapped;
    }else if(pointerType==QtJambiTypeManager::initializer_list){
        type |= QtJambiTypeManager::initializerListWrapper;
    }
    if ((value_type_pattern & Primitive) == 0) {
        if (isQtClass(env, javaName))
            type |= QtClass | QtSubClass;
        else if (isQtSubClass(env, javaName))
            type |= QtSubClass;
    }
    if(internalTypeName.endsWith("&") && !internalTypeName.endsWith("const ")){
        type |= Reference;
    }

    if(metaType==qMetaTypeId<QVariant>())
        type |= VariantPattern;
#if QT_VERSION < 0x050000
    if (metaType != QMetaType::Void) {
#else
    if (metaType != QMetaType::UnknownType) {
#endif
        QMetaType qMetaType(metaType);
        if(qMetaType.flags() & QMetaType::IsEnumeration && javaName.isEmpty()){
            type = Value | Primitive | Integer;
        } else if (isQObjectSubclass(env, javaName)){
            if(indirections > 0
                    || pointerType==QtJambiTypeManager::SharedPointer
                    || pointerType==QtJambiTypeManager::WeakPointer){
                type &= ~Value;
                type |= Object;
            }
            type |= QObjectSubclass;
        }else{
            if(value_type_pattern & TypeMask){
                value_type_pattern |= Primitive;
            }
            type |= value_type_pattern;
        }
    } else {
        type |= Object;

        if (isQObjectSubclass(env, javaName))
            type |= QObjectSubclass;
        else if(type==Object){
            if(internalTypeName.contains("::")){
                QStringList splitted = internalTypeName.split("::");
                QString enclosedTypeName = splitted.takeLast();
                QString enclosingTypeName = splitted.join("::");
                int enclosingMetaType = QMetaType::type(qPrintable(enclosingTypeName));
                const QMetaObject* metaObject = nullptr;
                if(enclosingMetaType != QMetaType::UnknownType){
                    metaObject = QMetaType::metaObjectForType(enclosingMetaType);
                }
                if(!metaObject){
                    metaObject = findMetaObject(qPrintable(enclosingTypeName));
                }
                if(metaObject){
                    QMetaEnum enumerator = metaObject->enumerator(metaObject->indexOfEnumerator(qPrintable(enclosedTypeName)));
                    if(enumerator.isValid()){
                        type = Value | Primitive | Integer;
                    }
                }
            }
        }
    }

    if (internalTypeName == QLatin1String("void") && indirections > 0)
        type = NativePointer;

    // Pointers to value types are native pointers
    if ((type & Value) && indirections > 0)
        type = NativePointer;

    return TypePattern(type);
}

bool QtJambiTypeManager::isFlagsType(JNIEnv* env, jclass clazz) {
    Q_ASSERT(clazz != nullptr);
    return Java::Private::QtJambi::QFlags.isAssignableFrom(env, clazz);
}

QtJambiTypeManager::EnumType QtJambiTypeManager::getEnumType(JNIEnv* env, const QString &className, jobject classLoader) {
    jclass clazz = resolveClass(env, qPrintable(className), classLoader);
    if (clazz != nullptr)
        return getEnumType(env, clazz);
    else
        return NoEnum;
}

QtJambiTypeManager::EnumType QtJambiTypeManager::getEnumType(JNIEnv* env, jclass clazz) {
    Q_ASSERT(clazz != nullptr);
    if(Java::Private::QtJambi::QtEnumerator.isAssignableFrom(env, clazz)){
        return QtJambiTypeManager::Enum32;
    }else if(Java::Private::QtJambi::QtShortEnumerator.isAssignableFrom(env, clazz)){
        return QtJambiTypeManager::Enum16;
    }else if(Java::Private::QtJambi::QtByteEnumerator.isAssignableFrom(env, clazz)){
        return QtJambiTypeManager::Enum8;
    }else if(Java::Private::QtJambi::QtLongEnumerator.isAssignableFrom(env, clazz)){
        return QtJambiTypeManager::Enum64;
    }else if(Java::Private::Runtime::Enum.isAssignableFrom(env, clazz)){
        return QtJambiTypeManager::Enum32;
    }else{
        return QtJambiTypeManager::NoEnum;
    }
}

bool QtJambiTypeManager::isFlagsType(JNIEnv* env, const QString &className, jobject classLoader) {
    jclass clazz = resolveClass(env, qPrintable(className), classLoader);
    if (clazz != nullptr)
        return isFlagsType(env, clazz);
    else
        return false;
}

/*!
    Parses a signature and returns a vector containing the names of the return and argument types
    in the signature. The first entry in the returned QVector will contain the name of the
    return type, the subsequent entries will contain the names of the argument types in order.
    If \a name is different from null, the string it points to will contain the name of the
    function after when the call to parseSignature() returns.
*/
QVector<QString> QtJambiTypeManager::parseSignature(const QString &signature,
        QString *name) {
    QString tmp(signature.trimmed());
    tmp.replace(QLatin1Char('.'), QLatin1Char('/'));

    QString returnType = QLatin1String("void");
    int idxFirstSpace = tmp.indexOf(QLatin1Char(' '));
    int idxFirstParen = tmp.indexOf(QLatin1Char('('));

    // Does it have a return type?
    if (idxFirstSpace >= 0 && idxFirstSpace < idxFirstParen) {
        returnType = tmp.left(idxFirstSpace).trimmed();
        tmp = tmp.remove(0, idxFirstSpace + 1).trimmed();
    }

    QVector<QString> returned;
    returned.append(returnType);

    idxFirstParen = tmp.indexOf(QLatin1Char('('));
    if (name != nullptr)
        *name = tmp.left(idxFirstParen).trimmed();

    tmp = tmp.remove(0, idxFirstParen + 1).trimmed();
    int idxComma = tmp.indexOf(QLatin1Char(','));
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


/*!
    \reimp

    Returns the C++ equivalent of a Java type.

    The translation is as follows:
      * Primitive types:
        long -> qint64
        boolean -> bool
        char -> QChar
        byte -> char
        ... otherwise the same type name is used
      * Box types:
        java/lang/Long -> qint64
        ... etc. (equivalent to primitive types)
      * Reference types, subclasses of QObject:
        The simple class name with an asterisk appended to it
      * Reference types, non-QObjects in the Qt package (value types):
        The simple class name by itself
      * Reference types, regular Java Objects
        jobjects
*/
QString QtJambiTypeManager::getInternalTypeName(JNIEnv* env, const QString &externalTypeName, jobject classLoader) {
    EntryTypes getEntryType(const std::type_info& typeId);
    // First we try a simple look up. This will handle any type in the type system,
    // so: primitives, boxed primitives and direct mappings of Qt types
    if(externalTypeName=="void")
        return externalTypeName;
    if(externalTypeName=="io/qt/core/QNativePointer")
        return QLatin1String(QMetaType::typeName(QMetaType::VoidStar));
    const std::type_info* typeId = getTypeByJavaName(externalTypeName);
    if(typeId){
        int metaTypeID = registeredMetaTypeID(*typeId);
        if(metaTypeID!=QMetaType::UnknownType){
            return QMetaType::typeName(metaTypeID);
        }else{
            QString qtName = getQtName(*typeId);
            switch(getEntryType(*typeId)){
            case EntryTypes::ObjectTypeInfo:
            case EntryTypes::QObjectTypeInfo:
            case EntryTypes::InterfaceTypeInfo:
                return qtName + "*";
            default:
                return qtName;
            }
        }
    }else{
        QString internalTypeName = QString(externalTypeName).replace("/", "::");
        if(QMetaType::type(qPrintable(internalTypeName))!=QMetaType::UnknownType){
            return internalTypeName;
        }
        internalTypeName += "*";
        if(QMetaType::type(qPrintable(internalTypeName))!=QMetaType::UnknownType){
            return internalTypeName;
        }
        jclass cls = nullptr;
        try{
            cls = resolveClass(env, qPrintable(externalTypeName), classLoader);
        }catch(...){}
        if(cls){
            if(Java::Private::Runtime::Class.isArray(env,cls)){
                return QLatin1String("JObjectWrapper");
            }else if(Java::Runtime::Integer.isSameClass(env, cls)){
                return QLatin1String("int");
            }else if(Java::Runtime::Long.isSameClass(env, cls)){
                return QLatin1String(QMetaType::typeName(qMetaTypeId<qint64>()));
            }else if(Java::Runtime::Short.isSameClass(env, cls)){
                return QLatin1String(QMetaType::typeName(qMetaTypeId<qint16>()));
            }else if(Java::Runtime::Byte.isSameClass(env, cls)){
                return QLatin1String(QMetaType::typeName(qMetaTypeId<qint8>()));
            }else if(Java::Runtime::Boolean.isSameClass(env, cls)){
                return QLatin1String("bool");
            }else if(Java::Runtime::Character.isSameClass(env, cls)){
                return QLatin1String(QMetaType::typeName(qMetaTypeId<QChar>()));
            }else if(Java::Runtime::Float.isSameClass(env, cls)){
                return QLatin1String("float");
            }else if(Java::Runtime::Double.isSameClass(env, cls)){
                return QLatin1String("double");
            }else if(Java::Private::QtCore::QMetaObject$Connection.isAssignableFrom(env, cls)){
                return QLatin1String("QMetaObject::Connection");
            }else {
                const SuperTypeInfos& superType = getSuperTypeInfos(env, cls);
                if(!superType.isEmpty()){
                    QString qtName = QLatin1String(superType.first().qtName);
                    switch(getEntryType(superType.first().typeId())){
                    case EntryTypes::ObjectTypeInfo:
                    case EntryTypes::QObjectTypeInfo:
                    case EntryTypes::InterfaceTypeInfo:
                        return qtName + "*";
                    default:
                        return qtName;
                    }
                }else if(Java::Private::Runtime::Map.isAssignableFrom(env, cls)){
                    return QLatin1String("JMapWrapper");
                }else if(Java::Private::Runtime::Collection.isAssignableFrom(env, cls)){
                    return QLatin1String("JCollectionWrapper");
                }else if(Java::Private::Runtime::Iterator.isAssignableFrom(env, cls)){
                    return QLatin1String("JIteratorWrapper");
                }else if(Java::Private::Runtime::Enum.isAssignableFrom(env, cls)
                         || Java::Private::QtJambi::QtEnumerator.isAssignableFrom(env, cls)
                         || Java::Private::QtJambi::QtLongEnumerator.isAssignableFrom(env, cls)
                         || Java::Private::QtJambi::QtShortEnumerator.isAssignableFrom(env, cls)
                         || Java::Private::QtJambi::QtByteEnumerator.isAssignableFrom(env, cls)){
                    return QLatin1String("JEnumWrapper");
                }else{
                    return QLatin1String("JObjectWrapper");
                }
            }
        }else{
            return QString();
        }
    }
}

/*!
    \reimp

    Returns the Java equivalent of C++ type \a internalTypeName.

    The translation is as follows:
        qint64 -> java/lang/Long
        bool -> java/lang/Boolean
        QChar -> java/lang/Character
        char -> java/lang/Byte
        .. etc for other types that map to Java primitive types. All these types are boxed.
      * Reference types with equivalents in Qt package:
        Translated to equivalent java class, fully qualified.
*/
QString QtJambiTypeManager::getExternalTypeName(JNIEnv *env, const QString &internalTypeName, VariableContext ctx, int metaType) {
    QString result = getExternalTypeNameNoWarn(env, internalTypeName, ctx, metaType);
    if(result.isEmpty()){
        if(internalTypeName.isEmpty()){
            if(metaType==QMetaType::UnknownType){
                qWarning("QtJambiTypeManager::getExternalTypeName: Couldn't convert unknown type to Java type");
            }else{
                qWarning("QtJambiTypeManager::getExternalTypeName: Couldn't convert '%s' to Java type",
                         QMetaType::typeName(metaType));
            }
        }else{
            qWarning("QtJambiTypeManager::getExternalTypeName: Couldn't convert '%s' to Java type",
                     qPrintable(internalTypeName));
        }
    }
    return result;
}

QString QtJambiTypeManager::getExternalTypeName(JNIEnv *env, const QString &internalTypeName, const QMetaObject * metaObject, VariableContext ctx, int metaType){
    QString result = getExternalTypeNameNoWarn(env, internalTypeName, metaObject, ctx, metaType);
    if(result.isEmpty()){
        if(internalTypeName.isEmpty()){
            if(metaType==QMetaType::UnknownType){
                qWarning("QtJambiTypeManager::getExternalTypeName: Couldn't convert unknown type to Java type");
            }else{
                qWarning("QtJambiTypeManager::getExternalTypeName: Couldn't convert '%s' to Java type",
                         QMetaType::typeName(metaType));
            }
        }else{
            qWarning("QtJambiTypeManager::getExternalTypeName: Couldn't convert '%s' to Java type",
                     qPrintable(internalTypeName));
        }
    }
    return result;
}

QString QtJambiTypeManager::getExternalTypeNameNoWarn(JNIEnv *env, const QString &internalTypeName, const QMetaObject * metaObject, VariableContext ctx, int metaType){
    QString result = getExternalTypeNameNoWarn(env, internalTypeName, ctx, metaType);
    if(result.isEmpty()){
        const QMetaObject * _metaObject = metaObject;
        while(_metaObject){
            result = getExternalTypeNameNoWarn(env, QLatin1String(_metaObject->className())+QLatin1String("::")+internalTypeName, ctx, metaType);
            if(!result.isEmpty()){
                break;
            }
            _metaObject = _metaObject->superClass();
        }
    }
    return result;
}

QString QtJambiTypeManager::getExternalTypeNameNoWarn(JNIEnv* environment, const QString &internalTypeName, VariableContext ctx, int metaType) {
    // Handle types in the type system, so: primitives and direct mappings.
    // Primitives need to be converted to objects for return types, so we
    // have to do some more work for them.
    uint indirections = 0;
    PointerType pointerType = QtJambiTypeManager::NoPointer;
    QString _internalTypeName = processInternalTypeName(internalTypeName, indirections, pointerType);
    QStringList instantiations;
    {
        int idx = _internalTypeName.indexOf("<");
        if(idx>0 && _internalTypeName.endsWith(">") && !_internalTypeName.startsWith("QFlags<")){
            QString instantiationTypes = _internalTypeName.mid(idx+1);
            instantiationTypes.chop(1);
            instantiations = instantiationTypes.split(",");
            _internalTypeName = _internalTypeName.left(idx);
        }
    }
    {
        if(_internalTypeName.startsWith("const ")){
            _internalTypeName = _internalTypeName.mid(6);
        }
        if(_internalTypeName.endsWith("&")){
            _internalTypeName.chop(1);
        }
    }
    const std::type_info* typeId = getTypeByQtName(_internalTypeName);
    if(!typeId && !instantiations.isEmpty()){
        typeId = getTypeByQtName(QString(_internalTypeName)+"<"+instantiations.join(",")+">");
    }
    /*if(!typeId && metaType!=QMetaType::UnknownType){
        typeId = getTypeByMetaType(metaType);
    }*/
    QString javaName;
    if(typeId){
        javaName = getJavaName(*typeId);
    }
    if(javaName.isEmpty())
        javaName = QLatin1String(getInterface(qPrintable(_internalTypeName)));// TODO adapt!!
    if(javaName.isEmpty()){
        if(const std::type_info* typeId = getTypeByQtName(_internalTypeName)){
            javaName = getJavaName(*typeId);
        }
    }
    if(javaName.isEmpty() && !instantiations.isEmpty()){
        if(const std::type_info* typeId = getTypeByQtName(QString(_internalTypeName)+"<"+instantiations.join(",")+">")){
            javaName = QLatin1String(getJavaName(*typeId));
        }
    }
    if(javaName.isEmpty()){
        int metaType = QMetaType::type(qPrintable(_internalTypeName));
        if(metaType!=QMetaType::UnknownType
                && metaType!=QMetaType::Nullptr
                && metaType!=QMetaType::Void){
            QVariant variant(metaType, nullptr);
            if(variant.canConvert(QMetaType::QVariantList)){
                javaName = "java/util/ArrayList";
            }else if(variant.canConvert(QMetaType::QVariantHash)){
                javaName = "java/util/HashMap";
            }else if(variant.canConvert(QMetaType::QVariantMap)){
                javaName = "java/util/TreeMap";
            }
        }
    }
    if(!javaName.isEmpty() && pointerType==QtJambiTypeManager::initializer_list){
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
    if(!instantiations.isEmpty()){
        switch(ctx){
        case ReturnType:
            if(javaName=="io/qt/core/QHash"){
                javaName = "java/util/Map";
            }else if(javaName=="io/qt/core/QMultiHash"){
                javaName = "java/util/Map";
            }else if(javaName=="io/qt/core/QMultiMap"){
                javaName = "java/util/NavigableMap";
            }else if(javaName=="io/qt/core/QMap"){
                javaName = "java/util/NavigableMap";
            }else if(javaName=="io/qt/core/QLinkedList"){
                javaName = "java/util/Collection";
            }else if(javaName=="io/qt/core/QList"){
                javaName = "java/util/List";
            }else if(javaName=="io/qt/core/QQueue"){
                javaName = "java/util/Deque";
            }else if(javaName=="io/qt/core/QVector"){
                javaName = "java/util/List";
            }else if(javaName=="io/qt/core/QStack"){
                javaName = "java/util/Deque";
            }else if(javaName=="io/qt/core/QSet"){
                javaName = "java/util/Set";
            }
            break;
        case ArgumentType:
            if(javaName=="io/qt/core/QHash"){
                javaName = "java/util/Map";
            }else if(javaName=="io/qt/core/QMultiHash"){
                javaName = "java/util/Map";
            }else if(javaName=="io/qt/core/QMultiMap"){
                javaName = "java/util/Map";
            }else if(javaName=="io/qt/core/QMap"){
                javaName = "java/util/Map";
            }else if(javaName=="io/qt/core/QLinkedList"){
                javaName = "java/util/Collection";
            }else if(javaName=="io/qt/core/QList"){
                javaName = "java/util/Collection";
            }else if(javaName=="io/qt/core/QQueue"){
                javaName = "java/util/Collection";
            }else if(javaName=="io/qt/core/QVector"){
                javaName = "java/util/Collection";
            }else if(javaName=="io/qt/core/QStack"){
                javaName = "java/util/Collection";
            }else if(javaName=="io/qt/core/QSet"){
                javaName = "java/util/Collection";
            }
            break;
        case InstantiationType:
            if(javaName=="io/qt/core/QHash"){
                javaName = "java/util/HashMap";
            }else if(javaName=="io/qt/core/QMultiHash"){
                javaName = "java/util/HashMap";
            }else if(javaName=="io/qt/core/QMultiMap"){
                javaName = "java/util/TreeMap";
            }else if(javaName=="io/qt/core/QMap"){
                javaName = "java/util/TreeMap";
            }else if(javaName=="io/qt/core/QLinkedList"){
                javaName = "java/util/ArrayList";
            }else if(javaName=="io/qt/core/QList"){
                javaName = "java/util/ArrayList";
            }else if(javaName=="io/qt/core/QQueue"){
                javaName = "java/util/ArrayDeque";
            }else if(javaName=="io/qt/core/QVector"){
                javaName = "java/util/ArrayList";
            }else if(javaName=="io/qt/core/QStack"){
                javaName = "java/util/ArrayDeque";
            }else if(javaName=="io/qt/core/QSet"){
                javaName = "java/util/HashSet";
            }
            //javaName += "|" + instantiations.join("|");
            break;
        }
    }
    if (ctx != ReturnType && !javaName.isEmpty())
        return javaName;

    if(metaType == QMetaType::UnknownType){
        if(_internalTypeName.contains("::") && javaName.isEmpty()){
            QStringList splitted = _internalTypeName.split("::");
            QString enclosedTypeName = splitted.takeLast();
            QString enclosingTypeName = splitted.join("::");
            int enclosingMetaType = QMetaType::type(qPrintable(enclosingTypeName));
            const QMetaObject *metaObject = nullptr;
            if(enclosingMetaType != QMetaType::UnknownType){
                metaObject = QMetaType::metaObjectForType(enclosingMetaType);
            }
            if(!metaObject){
                metaObject = findMetaObject(qPrintable(enclosingTypeName));
            }
            if(metaObject){
                QMetaEnum enumerator = metaObject->enumerator(metaObject->indexOfEnumerator(qPrintable(enclosedTypeName)));
                if(enumerator.isValid()){
                    return "int";
                }
            }
        }
    }else if(gMetaEnumByMetaTypeHash->value(metaType).isValid()){
        QMetaEnum enumerator = gMetaEnumByMetaTypeHash->value(metaType);
        if(enumerator.isFlag()){
            return "io/qt/core/QMetaType$GenericFlags";
        }else{
            switch(QMetaType::sizeOf(metaType)){
            case 1:  return "io/qt/core/QMetaType$GenericByteEnumerator";
            case 2:  return "io/qt/core/QMetaType$GenericShortEnumerator";
            case 8:  return "io/qt/core/QMetaType$GenericLongEnumerator";
            default: return "io/qt/core/QMetaType$GenericEnumerator";
            }
        }
    }else{
        QMetaType qMetaType(metaType);
        if(!qMetaType.metaObject() && _internalTypeName.contains("::")){
            QStringList splitted = _internalTypeName.split("::");
            QString enclosedTypeName = splitted.takeLast();
            QString enclosingTypeName = splitted.join("::");
            int enclosingMetaType = QMetaType::type(qPrintable(enclosingTypeName));
            const QMetaObject *metaObject = nullptr;
            if(enclosingMetaType != QMetaType::UnknownType){
                metaObject = QMetaType::metaObjectForType(enclosingMetaType);
            }
            if(!metaObject){
                metaObject = findMetaObject(qPrintable(enclosingTypeName));
            }
            if(metaObject){
                QMetaEnum enumerator = metaObject->enumerator(metaObject->indexOfEnumerator(qPrintable(enclosedTypeName)));
                if(enumerator.isValid()){
                    gMetaEnumByMetaTypeHash->insert(metaType, enumerator);
                    if(enumerator.isFlag()){
                        return "io/qt/core/QMetaType$GenericFlags";
                    }else{
                        switch(qMetaType.sizeOf()){
                        case 1:  return "io/qt/core/QMetaType$GenericByteEnumerator";
                        case 2:  return "io/qt/core/QMetaType$GenericShortEnumerator";
                        case 8:  return "io/qt/core/QMetaType$GenericLongEnumerator";
                        default: return "io/qt/core/QMetaType$GenericEnumerator";
                        }
                    }
                }
            }
        }
        if(const QMetaObject *metaObjectForParameter = qMetaType.metaObject()){
            if(metaObjectForParameter->className()==_internalTypeName){
                while(metaObjectForParameter){
                    QString externalTypeName = getExternalTypeNameNoWarn(environment, QString("%1%2").arg(QLatin1String(metaObjectForParameter->className())).arg(QString(int(indirections), QLatin1Char('*'))), ctx);
                    if(!externalTypeName.isEmpty())
                        return externalTypeName;
                    metaObjectForParameter = metaObjectForParameter->superClass();
                }
            }else if(_internalTypeName.contains("::")){
                QMetaEnum enumerator = metaObjectForParameter->enumerator(metaObjectForParameter->indexOfEnumerator(qPrintable(_internalTypeName.split("::").last())));
                if(enumerator.isValid()){
                    gMetaEnumByMetaTypeHash->insert(metaType, enumerator);
                    if(enumerator.isFlag()){
                        return "io/qt/core/QMetaType$GenericFlags";
                    }else{
                        switch(qMetaType.sizeOf()){
                        case 1:  return "io/qt/core/QMetaType$GenericByteEnumerator";
                        case 2:  return "io/qt/core/QMetaType$GenericShortEnumerator";
                        case 8:  return "io/qt/core/QMetaType$GenericLongEnumerator";
                        default: return "io/qt/core/QMetaType$GenericEnumerator";
                        }
                    }
                }
            }
        }else{
            QString metaTypeName(QMetaType::typeName(metaType));
            if(metaTypeName!=internalTypeName){
                return getExternalTypeNameNoWarn(environment, metaTypeName, ctx, metaType);
            }
        }
        if(qMetaType.flags() & QMetaType::IsEnumeration){
            switch(qMetaType.sizeOf()){
            case 1:  return "io/qt/core/QMetaType$GenericByteEnumerator";
            case 2:  return "io/qt/core/QMetaType$GenericShortEnumerator";
            case 8:  return "io/qt/core/QMetaType$GenericLongEnumerator";
            default: return "io/qt/core/QMetaType$GenericEnumerator";
            }
        }
    }

    TypePattern type = typeIdOfInternal(environment, internalTypeName);

    // If it's a primitive, then we're doing a return type and we convert to an object
    if (!(type & Enum) && type & TypeMask)
        return complexTypeOf(TypePattern(type & TypeMask));
    else if (type & NativePointer)
        return QLatin1String("io/qt/QNativePointer");
    else if (type & Object){
        if(!javaName.isEmpty())
            return javaName;
    }
    if (type == None)
        return QLatin1String("void");
    else if (internalTypeName=="QVariant")
        return QLatin1String("java/lang/Object");
    else if (internalTypeName=="JMapWrapper")
        return QLatin1String("java/util/Map");
    else if (internalTypeName=="JCollectionWrapper")
        return QLatin1String("java/util/Collection");
    else if (internalTypeName=="JIteratorWrapper")
        return QLatin1String("java/util/Iterator");
    else if (internalTypeName=="JObjectWrapper")
        return QLatin1String("java/lang/Object");
    else if (internalTypeName=="JEnumWrapper")
        return QLatin1String("java/lang/Enum");
    else if (internalTypeName=="JQFlagsWrapper")
        return QLatin1String("io/qt/QFlags");
    else if (internalTypeName.startsWith("QPair<") && internalTypeName.endsWith(">"))
        return QLatin1String("io/qt/core/QPair");
    else if (internalTypeName.startsWith("std::pair<") && internalTypeName.endsWith(">"))
        return QLatin1String("io/qt/core/QPair");

    return QString();
}

bool isPolymorphicBase(const char *lookup);

void checkClassName(QObject* qobject, QString className, QString &_externalClassName, QString &_externalClassPackage){
    const QMetaObject *mo = qtjambi_find_first_static_metaobject(qobject->metaObject());
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
            QtJambiTypeManager::splitClassName(javaClassName, javaPackageName, javaName);

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

const InternalToExternalConverter& QtJambiTypeManager::getInternalToExternalConverter(
                               JNIEnv* env,
                               const QString &internalTypeName,
                               TypePattern internalType,
                               int internalMetaType,
                               jclass externalClass,
                               bool allowValuePointers){
    uint key = hashSum({qHash(internalTypeName), qHash(internalType), uint(externalClass ? Java::Private::Runtime::Object.hashCode(env, externalClass) : 0), qHash(allowValuePointers)});
    {
        QReadLocker locker(gCacheLock());
        Q_UNUSED(locker)
        if(gInternalToExternalConverters()->contains(key))
            return (*gInternalToExternalConverters())[key];
    }
    InternalToExternalConverter converter = getInternalToExternalConverterImpl(
                env,
                internalTypeName,
                internalType,
                internalMetaType,
                externalClass,
                allowValuePointers
            );
    Q_ASSERT(converter);
    QWriteLocker locker(gCacheLock());
    Q_UNUSED(locker)
    gInternalToExternalConverters()->insert(key, converter);
    return (*gInternalToExternalConverters())[key];
}

InternalToExternalConverter QtJambiTypeManager::getInternalToExternalConverterImpl(
                               JNIEnv* _env,
                               const QString &internalTypeName,
                               TypePattern internalType,
                               int internalMetaType,
                               jclass externalClass,
                               bool allowValuePointers){
    if (internalMetaType == qMetaTypeId<QVariant>()) {
        return [](JNIEnv* env, QtJambiScope*, const void* in, jvalue* p, bool)->bool{
            p->l = qtjambi_from_qvariant(env, *reinterpret_cast<const QVariant*>(in));
            return true;
        };
    }else if ( internalMetaType == QMetaType::Nullptr
               || internalMetaType == QMetaType::Void
               || _env->IsSameObject(externalClass, nullptr)
               || Java::Runtime::Void.isSameClass(_env, externalClass)
               || Java::Runtime::Void.isPrimitiveType(_env, externalClass)) {
        return [](JNIEnv*, QtJambiScope*, const void*, jvalue* p, bool)->bool{
            p->l = nullptr;
            return true;
        };
    }
    bool isArrayClass = externalClass ? Java::Private::Runtime::Class.isArray(_env, externalClass) : false;

    if(isArrayClass && ((internalType & QtJambiTypeManager::initializer_list) || internalTypeName.startsWith("initializer_list<"))){
        jclass componentClass = Java::Private::Runtime::Class.getComponentType(_env, externalClass);
        QString javaTypeName = qtjambi_class_name(_env, jclass(componentClass)).replace(".", "/");
        QString _internalTypeName = internalTypeName;
        if((internalType & QtJambiTypeManager::initializer_list)==0){
            _internalTypeName = internalTypeName.mid(22, internalTypeName.length() - 23);
        }
        if(const std::type_info* typeId = getTypeByQtName(_internalTypeName)){
            int metaType = QMetaType::type(qPrintable(_internalTypeName));
            if(metaType==QMetaType::UnknownType){
                metaType = registeredMetaTypeID(*typeId);
            }
            size_t size = getValueSize(*typeId);
            if(size>0){
                QtJambiTypeManager::TypePattern internalTypePattern = QtJambiTypeManager::typeIdOfInternal(_env, javaTypeName, _internalTypeName, metaType);
                const InternalToExternalConverter& internalToExternalComponentConverter = QtJambiTypeManager::getInternalToExternalConverter(
                                                                _env,
                                                                _internalTypeName,
                                                                internalTypePattern,
                                                                metaType,
                                                                componentClass,
                                                                true);
                if(Java::Runtime::Byte.isPrimitiveType(_env,componentClass)){
                    return [internalToExternalComponentConverter, size](JNIEnv* env, QtJambiScope* scope, const void* in, jvalue* p, bool forceBoxedType)->bool{
                        const std::initializer_list<char>* il = reinterpret_cast<const std::initializer_list<char>*>(in);
                        const char* begin = il->begin();
                        bool result = true;
                        jbyteArray array = env->NewByteArray(jsize(il->size()/size));
                        jbyte* elements = env->GetByteArrayElements(array, nullptr);
                        jsize j=0;
                        for (size_t i=0; i<il->size(); i+=size) {
                            jvalue el;
                            el.l = nullptr;
                            result &= internalToExternalComponentConverter(env, scope, &begin[i], &el, forceBoxedType);
                            elements[j++] = el.b;
                        }
                        env->ReleaseByteArrayElements(array, elements, 0);
                        p->l = array;
                        return result;
                    };
                }else if(Java::Runtime::Short.isPrimitiveType(_env,componentClass)){
                    return [internalToExternalComponentConverter, size](JNIEnv* env, QtJambiScope* scope, const void* in, jvalue* p, bool forceBoxedType)->bool{
                        const std::initializer_list<char>* il = reinterpret_cast<const std::initializer_list<char>*>(in);
                        const char* begin = il->begin();
                        bool result = true;
                        jshortArray array = env->NewShortArray(jsize(il->size()/size));
                        jshort* elements = env->GetShortArrayElements(array, nullptr);
                        jsize j=0;
                        for (size_t i=0; i<il->size(); i+=size) {
                            jvalue el;
                            el.l = nullptr;
                            result &= internalToExternalComponentConverter(env, scope, &begin[i], &el, forceBoxedType);
                            elements[j++] = el.s;
                        }
                        env->ReleaseShortArrayElements(array, elements, 0);
                        p->l = array;
                        return result;
                    };
                }else if(Java::Runtime::Integer.isPrimitiveType(_env,componentClass)){
                    return [internalToExternalComponentConverter, size](JNIEnv* env, QtJambiScope* scope, const void* in, jvalue* p, bool forceBoxedType)->bool{
                        const std::initializer_list<char>* il = reinterpret_cast<const std::initializer_list<char>*>(in);
                        const char* begin = il->begin();
                        bool result = true;
                        jintArray array = env->NewIntArray(jsize(il->size()/size));
                        jint* elements = env->GetIntArrayElements(array, nullptr);
                        jsize j=0;
                        for (size_t i=0; i<il->size(); i+=size) {
                            jvalue el;
                            el.l = nullptr;
                            result &= internalToExternalComponentConverter(env, scope, &begin[i], &el, forceBoxedType);
                            elements[j++] = el.i;
                        }
                        env->ReleaseIntArrayElements(array, elements, 0);
                        p->l = array;
                        return result;
                    };
                }else if(Java::Runtime::Long.isPrimitiveType(_env,componentClass)){
                    return [internalToExternalComponentConverter, size](JNIEnv* env, QtJambiScope* scope, const void* in, jvalue* p, bool forceBoxedType)->bool{
                        const std::initializer_list<char>* il = reinterpret_cast<const std::initializer_list<char>*>(in);
                        const char* begin = il->begin();
                        bool result = true;
                        jlongArray array = env->NewLongArray(jsize(il->size()/size));
                        jlong* elements = env->GetLongArrayElements(array, nullptr);
                        jsize j=0;
                        for (size_t i=0; i<il->size(); i+=size) {
                            jvalue el;
                            el.l = nullptr;
                            result &= internalToExternalComponentConverter(env, scope, &begin[i], &el, forceBoxedType);
                            elements[j++] = el.j;
                        }
                        env->ReleaseLongArrayElements(array, elements, 0);
                        p->l = array;
                        return result;
                    };
                }else if(Java::Runtime::Float.isPrimitiveType(_env,componentClass)){
                    return [internalToExternalComponentConverter, size](JNIEnv* env, QtJambiScope* scope, const void* in, jvalue* p, bool forceBoxedType)->bool{
                        const std::initializer_list<char>* il = reinterpret_cast<const std::initializer_list<char>*>(in);
                        const char* begin = il->begin();
                        bool result = true;
                        jfloatArray array = env->NewFloatArray(jsize(il->size()/size));
                        jfloat* elements = env->GetFloatArrayElements(array, nullptr);
                        jsize j=0;
                        for (size_t i=0; i<il->size(); i+=size) {
                            jvalue el;
                            el.l = nullptr;
                            result &= internalToExternalComponentConverter(env, scope, &begin[i], &el, forceBoxedType);
                            elements[j++] = el.f;
                        }
                        env->ReleaseFloatArrayElements(array, elements, 0);
                        p->l = array;
                        return result;
                    };
                }else if(Java::Runtime::Double.isPrimitiveType(_env,componentClass)){
                    return [internalToExternalComponentConverter, size](JNIEnv* env, QtJambiScope* scope, const void* in, jvalue* p, bool forceBoxedType)->bool{
                        const std::initializer_list<char>* il = reinterpret_cast<const std::initializer_list<char>*>(in);
                        const char* begin = il->begin();
                        bool result = true;
                        jdoubleArray array = env->NewDoubleArray(jsize(il->size()/size));
                        jdouble* elements = env->GetDoubleArrayElements(array, nullptr);
                        jsize j=0;
                        for (size_t i=0; i<il->size(); i+=size) {
                            jvalue el;
                            el.l = nullptr;
                            result &= internalToExternalComponentConverter(env, scope, &begin[i], &el, forceBoxedType);
                            elements[j++] = el.d;
                        }
                        env->ReleaseDoubleArrayElements(array, elements, 0);
                        p->l = array;
                        return result;
                    };
                }else if(Java::Runtime::Character.isPrimitiveType(_env,componentClass)){
                    return [internalToExternalComponentConverter, size](JNIEnv* env, QtJambiScope* scope, const void* in, jvalue* p, bool forceBoxedType)->bool{
                        const std::initializer_list<char>* il = reinterpret_cast<const std::initializer_list<char>*>(in);
                        const char* begin = il->begin();
                        bool result = true;
                        jcharArray array = env->NewCharArray(jsize(il->size()/size));
                        jchar* elements = env->GetCharArrayElements(array, nullptr);
                        jsize j=0;
                        for (size_t i=0; i<il->size(); i+=size) {
                            jvalue el;
                            el.l = nullptr;
                            result &= internalToExternalComponentConverter(env, scope, &begin[i], &el, forceBoxedType);
                            elements[j++] = el.c;
                        }
                        env->ReleaseCharArrayElements(array, elements, 0);
                        p->l = array;
                        return result;
                    };
                }else if(Java::Runtime::Boolean.isPrimitiveType(_env,componentClass)){
                    return [internalToExternalComponentConverter, size](JNIEnv* env, QtJambiScope* scope, const void* in, jvalue* p, bool forceBoxedType)->bool{
                        const std::initializer_list<char>* il = reinterpret_cast<const std::initializer_list<char>*>(in);
                        const char* begin = il->begin();
                        bool result = true;
                        jbooleanArray array = env->NewBooleanArray(jsize(il->size()/size));
                        jboolean* elements = env->GetBooleanArrayElements(array, nullptr);
                        jsize j=0;
                        for (size_t i=0; i<il->size(); i+=size) {
                            jvalue el;
                            el.l = nullptr;
                            result &= internalToExternalComponentConverter(env, scope, &begin[i], &el, forceBoxedType);
                            elements[j++] = el.z;
                        }
                        env->ReleaseBooleanArrayElements(array, elements, 0);
                        p->l = array;
                        return result;
                    };
                }else{
                    JObjectWrapper classWrapper(_env, componentClass);
                    return [internalToExternalComponentConverter, size, classWrapper](JNIEnv* env, QtJambiScope* scope, const void* in, jvalue* p, bool forceBoxedType)->bool{
                        const std::initializer_list<char>* il = reinterpret_cast<const std::initializer_list<char>*>(in);
                        const char* begin = il->begin();
                        bool result = true;
                        jobjectArray array = env->NewObjectArray(jsize(il->size()/size), jclass(classWrapper.object()), nullptr);
                        jsize j=0;
                        for (size_t i=0; i<il->size(); i+=size) {
                            jvalue el;
                            el.l = nullptr;
                            result &= internalToExternalComponentConverter(env, scope, &begin[i], &el, forceBoxedType);
                            env->SetObjectArrayElement(array, j++, el.l);
                        }
                        p->l = array;
                        return result;
                    };
                }
            }
        }
    }else if(Java::Runtime::Integer.isPrimitiveType(_env,externalClass) || Java::Runtime::Integer.isSameClass(_env,externalClass)){
        return [](JNIEnv* env, QtJambiScope*, const void* in, jvalue* p, bool forceBoxedType)->bool{
            memcpy(&p->i, in, 4);
            if(forceBoxedType){
                p->l = qtjambi_from_int(env, p->i);
            }
            return true;
        };
    }else if(Java::Runtime::Long.isPrimitiveType(_env,externalClass) || Java::Runtime::Long.isSameClass(_env,externalClass)){
        return [](JNIEnv* env, QtJambiScope*, const void* in, jvalue* p, bool forceBoxedType)->bool{
            memcpy(&p->j, in, 8);
            if(forceBoxedType){
                p->l = qtjambi_from_long(env, p->j);
            }
            return true;
        };
    }else if(Java::Runtime::Short.isPrimitiveType(_env,externalClass) || Java::Runtime::Short.isSameClass(_env,externalClass)){
        return [](JNIEnv* env, QtJambiScope*, const void* in, jvalue* p, bool forceBoxedType)->bool{
            memcpy(&p->s, in, 2);
            if(forceBoxedType){
                p->l = qtjambi_from_short(env, p->s);
            }
            return true;
        };
    }else if(Java::Runtime::Byte.isPrimitiveType(_env,externalClass) || Java::Runtime::Byte.isSameClass(_env,externalClass)){
        return [](JNIEnv* env, QtJambiScope*, const void* in, jvalue* p, bool forceBoxedType)->bool{
            memcpy(&p->b, in, 1);
            if(forceBoxedType){
                p->l = qtjambi_from_byte(env, p->b);
            }
            return true;
        };
    }else if(Java::Runtime::Boolean.isPrimitiveType(_env,externalClass) || Java::Runtime::Boolean.isSameClass(_env,externalClass)){
        return [](JNIEnv* env, QtJambiScope*, const void* in, jvalue* p, bool forceBoxedType)->bool{
            memcpy(&p->z, in, 1);
            if(forceBoxedType){
                p->l = qtjambi_from_boolean(env, p->z);
            }
            return true;
        };
    }else if(Java::Runtime::Character.isPrimitiveType(_env,externalClass) || Java::Runtime::Character.isSameClass(_env,externalClass)){
        if(internalMetaType==QMetaType::QChar){
            return [](JNIEnv* env, QtJambiScope*, const void* in, jvalue* p, bool forceBoxedType)->bool{
                const QChar* qchr = reinterpret_cast<const QChar*>(in);
                p->c = qchr->unicode();
                if(forceBoxedType){
                    p->l = qtjambi_from_char(env, p->c);
                }
                return true;
            };
        }else{
            return [](JNIEnv* env, QtJambiScope*, const void* in, jvalue* p, bool forceBoxedType)->bool{
                memcpy(&p->c, in, 2);
                if(forceBoxedType){
                    p->l = qtjambi_from_char(env, p->c);
                }
                return true;
            };
        }
    }else if(Java::Runtime::Float.isPrimitiveType(_env,externalClass) || Java::Runtime::Float.isSameClass(_env,externalClass)){
        return [](JNIEnv* env, QtJambiScope*, const void* in, jvalue* p, bool forceBoxedType)->bool{
            memcpy(&p->f, in, 4);
            if(forceBoxedType){
                p->l = qtjambi_from_float(env, p->f);
            }
            return true;
        };
    }else if(Java::Runtime::Double.isPrimitiveType(_env,externalClass) || Java::Runtime::Double.isSameClass(_env,externalClass)){
        return [](JNIEnv* env, QtJambiScope*, const void* in, jvalue* p, bool forceBoxedType)->bool{
            memcpy(&p->d, in, 8);
            if(forceBoxedType){
                p->l = qtjambi_from_double(env, p->d);
            }
            return true;
        };
    }else if(Java::Runtime::String.isSameClass(_env,externalClass)){
        return [](JNIEnv* env, QtJambiScope*, const void* in, jvalue* p, bool)->bool{
            const QString *strp = reinterpret_cast<const QString *>(in);
            p->l = qtjambi_from_qstring(env, *strp);
            return true;
        };
    }else if(Java::Private::QtJambi::QNativePointer.isSameClass(_env,externalClass)){
        return [](JNIEnv* env, QtJambiScope*, const void* in, jvalue* p, bool)->bool{
            const void * const*in_p = reinterpret_cast<const void * const*>(in);
            p->l = qtjambi_from_cpointer(env, *in_p, QNativePointer::Type::Pointer, 1);
            return true;
        };
    } else if (Java::Private::QtJambi::QFlags.isAssignableFrom(_env,externalClass)) {
        if (internalMetaType == qMetaTypeId<JObjectWrapper>()) {
            return [](JNIEnv* env, QtJambiScope*, const void* in, jvalue* p, bool)->bool{
                p->l = env->NewLocalRef(reinterpret_cast<const JObjectWrapper *>(in)->object());
                return true;
            };
        }else if (Java::Private::QtCore::QMetaType$GenericFlags.isSameClass(_env,externalClass)) {
            return [internalMetaType](JNIEnv* env, QtJambiScope*, const void* in, jvalue* p, bool)->bool{
                jint value = *reinterpret_cast<const qint32*>(in);
                p->l = Java::Private::QtCore::QMetaType$GenericFlags.newInstance(env, jint(internalMetaType), value);
                return true;
            };
        }else{
            JObjectWrapper external_class(_env, externalClass);
            return [external_class](JNIEnv* env, QtJambiScope*, const void* in, jvalue* p, bool)->bool{
                p->l = qtjambi_from_flags(env, *reinterpret_cast<const int *>(in), jclass(external_class.object()));
                return p->l!=nullptr;
            };
        }
    } else if(Java::Private::Runtime::Enum.isAssignableFrom(_env,externalClass)
              || Java::Private::QtJambi::QtEnumerator.isAssignableFrom(_env,externalClass)
              || Java::Private::QtJambi::QtLongEnumerator.isAssignableFrom(_env,externalClass)
              || Java::Private::QtJambi::QtShortEnumerator.isAssignableFrom(_env,externalClass)
              || Java::Private::QtJambi::QtByteEnumerator.isAssignableFrom(_env,externalClass)){
        if (internalMetaType == qMetaTypeId<JObjectWrapper>() || internalMetaType == qMetaTypeId<JEnumWrapper>()) {
            return [](JNIEnv* env, QtJambiScope*, const void* in, jvalue* p, bool)->bool{
                p->l = env->NewLocalRef(reinterpret_cast<const JObjectWrapper *>(in)->object());
                return true;
            };
        }else if (Java::Private::QtCore::QMetaType$GenericEnumerator.isSameClass(_env,externalClass)) {
            return [internalMetaType](JNIEnv* env, QtJambiScope*, const void* in, jvalue* p, bool)->bool{
                jint index = -1;
                jint value = *reinterpret_cast<const int*>(in);
                jstring name = nullptr;
                QMetaEnum enm = gMetaEnumByMetaTypeHash->value(internalMetaType);
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
                p->l = Java::Private::QtCore::QMetaType$GenericEnumerator.newInstance(env, jint(internalMetaType), index, value, name);
                return true;
            };
        } else if (Java::Private::QtCore::QMetaType$GenericShortEnumerator.isSameClass(_env,externalClass)) {
            return [internalMetaType](JNIEnv* env, QtJambiScope*, const void* in, jvalue* p, bool)->bool{
                jint index = -1;
                jshort value = *reinterpret_cast<const qint16*>(in);
                jstring name = nullptr;
                QMetaEnum enm = gMetaEnumByMetaTypeHash->value(internalMetaType);
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
                p->l = Java::Private::QtCore::QMetaType$GenericShortEnumerator.newInstance(env, jint(internalMetaType), index, value, name);
                return true;
            };
        } else if (Java::Private::QtCore::QMetaType$GenericByteEnumerator.isSameClass(_env,externalClass)) {
            return [internalMetaType](JNIEnv* env, QtJambiScope*, const void* in, jvalue* p, bool)->bool{
                jint index = -1;
                jbyte value = *reinterpret_cast<const qint8*>(in);
                jstring name = nullptr;
                QMetaEnum enm = gMetaEnumByMetaTypeHash->value(internalMetaType);
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
                p->l = Java::Private::QtCore::QMetaType$GenericByteEnumerator.newInstance(env, jint(internalMetaType), index, value, name);
                return true;
            };
        } else if (Java::Private::QtCore::QMetaType$GenericLongEnumerator.isSameClass(_env,externalClass)) {
            return [internalMetaType](JNIEnv* env, QtJambiScope*, const void* in, jvalue* p, bool)->bool{
                jint index = -1;
                jlong value = *reinterpret_cast<const qint64*>(in);
                jstring name = nullptr;
                QMetaEnum enm = gMetaEnumByMetaTypeHash->value(internalMetaType);
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
                p->l = Java::Private::QtCore::QMetaType$GenericLongEnumerator.newInstance(env, jint(internalMetaType), index, value, name);
                return true;
            };
        }else{
            JObjectWrapper external_class(_env, externalClass);
            Q_ASSERT(getEnumType(_env, externalClass));

            if (Java::Private::QtJambi::QtEnumerator.isAssignableFrom(_env,externalClass)) {
                return [external_class](JNIEnv* env, QtJambiScope*, const void* in, jvalue* p, bool)->bool{
                    p->l = qtjambi_from_enum(env, *reinterpret_cast<const qint32 *>(in), jclass(external_class.object()));
                    return p->l!=nullptr;
                };
            }else if (Java::Private::QtJambi::QtShortEnumerator.isAssignableFrom(_env,externalClass)) {
                return [external_class](JNIEnv* env, QtJambiScope*, const void* in, jvalue* p, bool)->bool{
                    p->l = qtjambi_from_enum(env, *reinterpret_cast<const qint16 *>(in), jclass(external_class.object()));
                    return p->l!=nullptr;
                };
            }else if (Java::Private::QtJambi::QtByteEnumerator.isAssignableFrom(_env,externalClass)) {
                return [external_class](JNIEnv* env, QtJambiScope*, const void* in, jvalue* p, bool)->bool{
                    p->l = qtjambi_from_enum(env, *reinterpret_cast<const qint8 *>(in), jclass(external_class.object()));
                    return p->l!=nullptr;
                };
            }else if (Java::Private::QtJambi::QtLongEnumerator.isAssignableFrom(_env,externalClass)) {
                return [external_class](JNIEnv* env, QtJambiScope*, const void* in, jvalue* p, bool)->bool{
                    p->l = qtjambi_from_enum(env, *reinterpret_cast<const qint64 *>(in), jclass(external_class.object()));
                    return p->l!=nullptr;
                };
            }else /*if (Java::Private::Runtime::Enum.isAssignableFrom(_env,externalClass))*/ {
                return [external_class](JNIEnv* env, QtJambiScope*, const void* in, jvalue* p, bool)->bool{
                    jobjectArray enumConstants = Java::Private::Runtime::Class.getEnumConstants(env, external_class.object());
                    p->l = env->GetObjectArrayElement(enumConstants, *reinterpret_cast<const qint32 *>(in));
                    return p->l!=nullptr;
                };
            }
        }
    } else if (Java::Private::QtCore::QMetaType$GenericValue.isSameClass(_env,externalClass)) {
        return [internalMetaType](JNIEnv* env, QtJambiScope*, const void* in, jvalue* p, bool)->bool{
            p->l = qtjambi_from_object(env, QMetaType::create(internalMetaType, in), Java::Private::QtCore::QMetaType$GenericValue.getClass(env), false);
            if(p->l){
                Java::Private::QtCore::QMetaType$GenericValue.set_type(env, p->l, internalMetaType);
            }
            return true;
        };
    } else if (Java::Private::QtCore::QMetaType$GenericGadget.isSameClass(_env,externalClass)) {
        return [internalMetaType](JNIEnv* env, QtJambiScope*, const void* in, jvalue* p, bool)->bool{
            if(QMetaType(internalMetaType).flags() & QMetaType::PointerToGadget){
                p->l = qtjambi_from_object(env, in, Java::Private::QtCore::QMetaType$GenericGadget.getClass(env), false);
            }else{
                p->l = qtjambi_from_object(env, QMetaType::create(internalMetaType, in), Java::Private::QtCore::QMetaType$GenericGadget.getClass(env), false);
            }
            if(p->l){
                Java::Private::QtCore::QMetaType$GenericGadget.set_type(env, p->l, internalMetaType);
                jobject mo = qtjambi_from_QMetaObject(env, QMetaType::metaObjectForType(internalMetaType));
                Java::Private::QtJambi::QtGadget.set_staticMetaObject(env, p->l, mo);
            }
            return true;
        };
    }else if (Java::Private::QtCore::QMetaEnum.isAssignableFrom(_env,externalClass)) {
        if(internalType & Pointer){
            return [](JNIEnv* env, QtJambiScope*, const void* in, jvalue* p, bool)->bool{
                if(in){
                    p->l = qtjambi_from_QMetaEnum(env, **reinterpret_cast<QMetaEnum const* const*>(in));
                }
                return true;
            };
        }else{
            return [](JNIEnv* env, QtJambiScope*, const void* in, jvalue* p, bool)->bool{
                if(in){
                    p->l = qtjambi_from_QMetaEnum(env, *reinterpret_cast<QMetaEnum const*>(in));
                }
                return true;
            };
        }
    }else if (Java::Private::QtCore::QMetaMethod.isAssignableFrom(_env,externalClass)) {
        if(internalType & Pointer){
            return [](JNIEnv* env, QtJambiScope*, const void* in, jvalue* p, bool)->bool{
                if(in){
                    p->l = qtjambi_from_QMetaMethod(env, **reinterpret_cast<QMetaMethod const* const*>(in));
                }
                return true;
            };
        }else{
            return [](JNIEnv* env, QtJambiScope*, const void* in, jvalue* p, bool)->bool{
                if(in){
                    p->l = qtjambi_from_QMetaMethod(env, *reinterpret_cast<QMetaMethod const*>(in));
                }
                return true;
            };
        }
    }else if (Java::Private::QtCore::QMetaObject.isAssignableFrom(_env,externalClass)) {
        if (allowValuePointers && (internalType & Pointer || internalTypeName.contains(QLatin1Char('*')))){
            return [](JNIEnv* env, QtJambiScope*, const void* in, jvalue* p, bool)->bool{
                if(in){
                    p->l = qtjambi_from_QMetaObject(env, *reinterpret_cast<QMetaObject const* const*>(in));
                }
                return true;
            };
        }else{
            return [](JNIEnv* env, QtJambiScope*, const void* in, jvalue* p, bool)->bool{
                p->l = qtjambi_from_QMetaObject(env, reinterpret_cast<QMetaObject const*>(in));
                return true;
            };
        }
    }else if (Java::Private::QtCore::QMetaObject$Connection.isAssignableFrom(_env,externalClass)) {
        return [](JNIEnv* env, QtJambiScope*, const void* in, jvalue* p, bool)->bool{
            const QtJambiTypeEntry* typeEntry = QtJambiTypeEntry::getTypeEntry(env, typeid(QMetaObject::Connection));
            if(in && typeEntry){
                return typeEntry->convertToJava(env, in, true, false, p, jValueType::l);
            }
            return false;
        };
    }else if (Java::Private::QtCore::QMetaProperty.isAssignableFrom(_env,externalClass)) {
        if(internalType & Pointer){
            return [](JNIEnv* env, QtJambiScope*, const void* in, jvalue* p, bool)->bool{
                if(in){
                    p->l = qtjambi_from_QMetaProperty(env, **reinterpret_cast<QMetaProperty const* const*>(in));
                }
                return true;
            };
        }else{
            return [](JNIEnv* env, QtJambiScope*, const void* in, jvalue* p, bool)->bool{
                if(in){
                    p->l = qtjambi_from_QMetaProperty(env, *reinterpret_cast<QMetaProperty const*>(in));
                }
                return true;
            };
        }
    }else if (Java::Private::QtCore::QModelIndex.isAssignableFrom(_env,externalClass)) {
        if(internalType & Pointer){
            return [](JNIEnv* env, QtJambiScope*, const void* in, jvalue* p, bool)->bool{
                if(in){
                    p->l = qtjambi_from_QModelIndex(env, **reinterpret_cast<QModelIndex const* const*>(in));
                }
                return true;
            };
        }else{
            return [](JNIEnv* env, QtJambiScope*, const void* in, jvalue* p, bool)->bool{
                if(in){
                    p->l = qtjambi_from_QModelIndex(env, *reinterpret_cast<QModelIndex const*>(in));
                }
                return true;
            };
        }
    }else if( (Java::Private::Runtime::Object.isSameClass(_env,externalClass)
               || Java::Private::Runtime::Collection.isSameClass(_env,externalClass)
               || Java::Private::Runtime::ArrayList.isSameClass(_env,externalClass)
               || Java::Private::Runtime::ArrayDeque.isSameClass(_env,externalClass)
               || Java::Private::Runtime::HashSet.isSameClass(_env,externalClass)
               || Java::Private::Runtime::List.isSameClass(_env,externalClass)
               || Java::Private::Runtime::Deque.isSameClass(_env,externalClass)
               || Java::Private::Runtime::Set.isSameClass(_env,externalClass))
              && internalMetaType!=QMetaType::UnknownType
              && (internalMetaType == QMetaType::QVariantList
                  || internalMetaType == QMetaType::QStringList
                  || internalMetaType == QMetaType::QByteArrayList
                  || QMetaType::hasRegisteredConverterFunction(internalMetaType,
                        qMetaTypeId<QtMetaTypePrivate::QSequentialIterableImpl>())) ){
        ContainerType containerType;
        if(internalTypeName.startsWith("QStack<")
                || internalTypeName.startsWith("QQueue<")
                || Java::Private::Runtime::Deque.isAssignableFrom(_env,externalClass)){
            containerType = ContainerType::Deque;
        }else if(internalTypeName.startsWith("QLinkedList<")
                 || Java::Private::Runtime::LinkedList.isSameClass(_env,externalClass)){
            containerType = ContainerType::LinkedList;
        }else if(internalTypeName.startsWith("QSet<")
                 || Java::Private::Runtime::Set.isAssignableFrom(_env,externalClass)){
            containerType = ContainerType::Set;
        }else{
            containerType = ContainerType::List;
        }
        return [internalMetaType,containerType](JNIEnv* env, QtJambiScope*, const void* in, jvalue* p, bool)->bool{
            if(in){
                QtMetaTypePrivate::QSequentialIterableImpl iter;
                QMetaType::convert(in, internalMetaType, &iter, qMetaTypeId<QtMetaTypePrivate::QSequentialIterableImpl>());
                if(!iter._iterable || !iter._at || !iter._size || !iter._moveToBegin || !iter._destroyIter)
                    return false;
                switch(containerType){
                case ContainerType::Deque:
                    p->l = Java::Private::Runtime::ArrayDeque.newInstance(env);
                    break;
                case ContainerType::List:
                    p->l = Java::Private::Runtime::ArrayList.newInstance(env);
                    break;
                case ContainerType::Set:
                    p->l = Java::Private::Runtime::HashSet.newInstance(env);
                    break;
                default:
                    p->l = Java::Private::Runtime::LinkedList.newInstance(env);
                    break;
                }
                QtMetaTypePrivate::QSequentialIterableImpl end(iter);
                iter.moveToBegin();
                end.moveToEnd();
                InternalToExternalConverter converter;
                int internalMetaType = QMetaType::UnknownType;
                for ( ; !iter.equal(end); iter.advance(1)) {
                    const QtMetaTypePrivate::VariantData d = iter.getCurrent();
                    if(internalMetaType!=d.metaTypeId || !converter){
                        internalMetaType = d.metaTypeId;
                        QString qtType = QMetaType::typeName(d.metaTypeId);
                        QString javaType = QtJambiTypeManager::getExternalTypeName(env, qtType, QtJambiTypeManager::InstantiationType, d.metaTypeId);
                        if(javaType.isEmpty() && d.metaTypeId!=QMetaType::UnknownType && d.metaTypeId!=QMetaType::Void && d.metaTypeId!=QMetaType::Nullptr){
                            if(qtType.endsWith("*")){
                                javaType = QLatin1String("io/qt/QNativePointer");
                                if(QMetaType(d.metaTypeId).flags() & QMetaType::PointerToQObject
                                        || QMetaType(d.metaTypeId).flags() & QMetaType::SharedPointerToQObject
                                        || QMetaType(d.metaTypeId).flags() & QMetaType::WeakPointerToQObject){
                                    javaType = QLatin1String("io/qt/core/QObject");
                                }else if(QMetaType(d.metaTypeId).flags() & QMetaType::PointerToGadget){
                                     javaType = QLatin1String("io/qt/core/QMetaType$GenericGadget");
                                }
                            }else{
                                if(QMetaType(d.metaTypeId).flags() & QMetaType::IsGadget){
                                     javaType = QLatin1String("io/qt/core/QMetaType$GenericGadget");
                                }else{
                                    javaType = QLatin1String("io/qt/core/QMetaType$GenericValue");
                                }
                            }
                        }
                        // Find usage pattern
                        QtJambiTypeManager::TypePattern internalType = QtJambiTypeManager::typeIdOfInternal(env, javaType, qtType, d.metaTypeId, nullptr);
                        converter = QtJambiTypeManager::getInternalToExternalConverter(
                                                                env,
                                                                qtType,
                                                                internalType,
                                                                d.metaTypeId,
                                                                resolveClass(env, qPrintable(javaType)),
                                                                internalType==QtJambiTypeManager::NativePointer);
                    }
                    jvalue val;
                    val.l = nullptr;
                    if(!converter || !converter(env, nullptr, d.data, &val, true)){
                        qWarning("Unable to convert qVariant to jobject");
                    }
                    Java::Private::Runtime::Collection.add(env, p->l, val.l);
                }
                if(iter._iterator){
                    iter.destroyIter();
                }
            }
            return true;
        };
    }else if( (Java::Private::Runtime::Object.isSameClass(_env,externalClass)
               || Java::Private::Runtime::Map.isSameClass(_env,externalClass)
               || Java::Private::Runtime::NavigableMap.isSameClass(_env,externalClass)
               || Java::Private::Runtime::HashMap.isSameClass(_env,externalClass)
               || Java::Private::Runtime::TreeMap.isSameClass(_env,externalClass))
              && internalMetaType!=QMetaType::UnknownType
              && (internalMetaType == QMetaType::QVariantMap
                  || internalMetaType == QMetaType::QVariantHash
                  || QMetaType::hasRegisteredConverterFunction(internalMetaType,
                        qMetaTypeId<QtMetaTypePrivate::QAssociativeIterableImpl>())) ){

        MapType mapType = MapType::HashMap;
        if(internalTypeName.startsWith("QMap<")
                || internalTypeName.startsWith("QMultiMap<")
                || Java::Private::Runtime::NavigableMap.isAssignableFrom(_env,externalClass)){
            mapType = MapType::TreeMap;
        }
        bool isMulti = internalTypeName.startsWith("QMultiHash<") || internalTypeName.startsWith("QMultiMap<");

        return [internalMetaType, mapType, isMulti](JNIEnv* env, QtJambiScope*, const void* in, jvalue* p, bool)->bool{
            if(in){
                QtMetaTypePrivate::QAssociativeIterableImpl iter;
                QMetaType::convert(in, internalMetaType, &iter, qMetaTypeId<QtMetaTypePrivate::QAssociativeIterableImpl>());
                if(!iter._iterable || !iter._getKey || !iter._getValue || !iter._size || !iter._begin || !iter._destroyIter)
                    return false;
                QtMetaTypePrivate::QAssociativeIterableImpl end(iter);
                iter.begin();
                end.end();
                InternalToExternalConverter keyConverter;
                int keyMetaType = QMetaType::UnknownType;
                InternalToExternalConverter valueConverter;
                int valueMetaType = QMetaType::UnknownType;
                QList<QPair<jobject,jobject>> elements;
                bool areComparable = true;
                uint enumType = 0;
                for ( ; !iter.equal(end); iter.advance(1)) {
                    const QtMetaTypePrivate::VariantData key = iter.getCurrentKey();
                    if(keyMetaType!=key.metaTypeId || !keyConverter){
                        keyMetaType = key.metaTypeId;
                        QString qtType = QLatin1String(QMetaType::typeName(key.metaTypeId));
                        QString javaType = QtJambiTypeManager::getExternalTypeName(env, qtType, QtJambiTypeManager::InstantiationType, key.metaTypeId);
                        if(javaType.isEmpty() && key.metaTypeId!=QMetaType::UnknownType && key.metaTypeId!=QMetaType::Void && key.metaTypeId!=QMetaType::Nullptr){
                            if(qtType.endsWith("*")){
                                javaType = QLatin1String("io/qt/QNativePointer");
                                if(QMetaType(key.metaTypeId).flags() & QMetaType::PointerToQObject
                                        || QMetaType(key.metaTypeId).flags() & QMetaType::SharedPointerToQObject
                                        || QMetaType(key.metaTypeId).flags() & QMetaType::WeakPointerToQObject){
                                    javaType = QLatin1String("io/qt/core/QObject");
                                }else if(QMetaType(key.metaTypeId).flags() & QMetaType::PointerToGadget){
                                     javaType = ("io/qt/core/QMetaType$GenericGadget");
                                }
                            }else{
                                if(QMetaType(key.metaTypeId).flags() & QMetaType::IsGadget){
                                     javaType = QLatin1String("io/qt/core/QMetaType$GenericGadget");
                                }else{
                                    javaType = QLatin1String("io/qt/core/QMetaType$GenericValue");
                                }
                            }
                        }
                        // Find usage pattern
                        QtJambiTypeManager::TypePattern internalType = QtJambiTypeManager::typeIdOfInternal(env, javaType, qtType, key.metaTypeId, nullptr);
                        jclass javaClass = resolveClass(env, qPrintable(javaType));

                        keyConverter = QtJambiTypeManager::getInternalToExternalConverter(
                                                                env,
                                                                qtType,
                                                                internalType,
                                                                key.metaTypeId,
                                                                javaClass,
                                                                internalType==QtJambiTypeManager::NativePointer);
                        if(mapType==MapType::TreeMap){
                            if(Java::Private::QtJambi::QtEnumerator.isAssignableFrom(env, javaClass))
                                enumType = 4;
                            else if(Java::Private::QtJambi::QtShortEnumerator.isAssignableFrom(env, javaClass))
                                enumType = 2;
                            else if(Java::Private::QtJambi::QtByteEnumerator.isAssignableFrom(env, javaClass))
                                enumType = 1;
                            else if(Java::Private::QtJambi::QtLongEnumerator.isAssignableFrom(env, javaClass))
                                enumType = 2;
                        }
                    }
                    const QtMetaTypePrivate::VariantData value = iter.getCurrentValue();
                    if(valueMetaType!=value.metaTypeId || !valueConverter){
                        valueMetaType = value.metaTypeId;
                        QString qtType = QMetaType::typeName(value.metaTypeId);
                        QString javaType = QtJambiTypeManager::getExternalTypeName(env, qtType, QtJambiTypeManager::InstantiationType, value.metaTypeId);
                        if(javaType.isEmpty() && value.metaTypeId!=QMetaType::UnknownType && value.metaTypeId!=QMetaType::Void && value.metaTypeId!=QMetaType::Nullptr){
                            if(qtType.endsWith("*")){
                                javaType = QLatin1String("io/qt/QNativePointer");
                                if(QMetaType(value.metaTypeId).flags() & QMetaType::PointerToQObject
                                        || QMetaType(value.metaTypeId).flags() & QMetaType::SharedPointerToQObject
                                        || QMetaType(value.metaTypeId).flags() & QMetaType::WeakPointerToQObject){
                                    javaType = QLatin1String("io/qt/core/QObject");
                                }else if(QMetaType(value.metaTypeId).flags() & QMetaType::PointerToGadget){
                                     javaType = QLatin1String("io/qt/core/QMetaType$GenericGadget");
                                }
                            }else{
                                if(QMetaType(value.metaTypeId).flags() & QMetaType::IsGadget){
                                     javaType = QLatin1String("io/qt/core/QMetaType$GenericGadget");
                                }else{
                                    javaType = QLatin1String("io/qt/core/QMetaType$GenericValue");
                                }
                            }
                        }
                        // Find usage pattern
                        QtJambiTypeManager::TypePattern internalType = QtJambiTypeManager::typeIdOfInternal(env, javaType, qtType, value.metaTypeId, nullptr);

                        valueConverter = QtJambiTypeManager::getInternalToExternalConverter(
                                                                env,
                                                                qtType,
                                                                internalType,
                                                                value.metaTypeId,
                                                                resolveClass(env, qPrintable(javaType)),
                                                                internalType==QtJambiTypeManager::NativePointer);
                    }
                    jvalue kval;
                    kval.l = nullptr;
                    if(!keyConverter || !keyConverter(env, nullptr, key.data, &kval, true)){
                        qWarning("Unable to convert qVariant to jobject");
                    }
                    jvalue val;
                    val.l = nullptr;
                    if(!valueConverter || !valueConverter(env, nullptr, value.data, &val, true)){
                        qWarning("Unable to convert qVariant to jobject");
                    }
                    elements << QPair<jobject,jobject>(kval.l, val.l);
                    if(mapType==MapType::TreeMap && enumType==0){
                        areComparable &= bool(Java::Private::Runtime::Comparable.isInstanceOf(env, kval.l));
                    }
                }

                switch(mapType){
                case MapType::TreeMap:{
                        jobject comparator = nullptr;
                        switch(enumType){
                        case 1:
                            comparator = Java::Private::QtJambi::QtByteEnumerator.comparator(env);
                            break;
                        case 2:
                            comparator = Java::Private::QtJambi::QtShortEnumerator.comparator(env);
                            break;
                        case 4:
                            comparator = Java::Private::QtJambi::QtEnumerator.comparator(env);
                            break;
                        case 8:
                            comparator = Java::Private::QtJambi::QtLongEnumerator.comparator(env);
                            break;
                        default:
                            if(!areComparable){
                                comparator = Java::Private::QtJambi::QtJambiInternal.createComparator(env, jlong(&lessThan));
                            }
                        }
                        if(comparator){
                            p->l = Java::Private::Runtime::TreeMap.newInstance2(env, comparator);
                        }else{
                            p->l = Java::Private::Runtime::TreeMap.newInstance(env);
                        }
                    }
                    break;
                default:
                    p->l = Java::Private::Runtime::HashMap.newInstance(env,jint(iter.size()));
                }

                if(isMulti){
                    for(const QPair<jobject,jobject>& pair : elements){
                        qtjambi_multimap_put(env, p->l, pair.first, pair.second);
                    }
                }else{
                    for(const QPair<jobject,jobject>& pair : elements){
                        qtjambi_map_put(env, p->l, pair.first, pair.second);
                    }
                }

                if(iter._iterator){
                    iter.destroyIter();
                }
                if(end._iterator){
                    end.destroyIter();
                }
            }
            return true;
        };
    }else if( (Java::Private::Runtime::Object.isSameClass(_env,externalClass)
               || Java::Private::QtCore::QPair.isSameClass(_env,externalClass))
              && internalMetaType!=QMetaType::UnknownType
              && (internalMetaType == QMetaType::QVariantMap
                  || internalMetaType == QMetaType::QVariantHash
                  || QMetaType::hasRegisteredConverterFunction(internalMetaType,
                        qMetaTypeId<QtMetaTypePrivate::QPairVariantInterfaceImpl>())) ){
        return [internalMetaType](JNIEnv* env, QtJambiScope*, const void* in, jvalue* p, bool)->bool{
            if(in){
                QtMetaTypePrivate::QPairVariantInterfaceImpl pair;
                if(!QMetaType::convert(in, internalMetaType, &pair, qMetaTypeId<QtMetaTypePrivate::QPairVariantInterfaceImpl>()))
                    return false;
                const QtMetaTypePrivate::VariantData firstData = pair.first();
                QString firstQtType = QLatin1String(QMetaType::typeName(firstData.metaTypeId));
                QString firstJavaType = QtJambiTypeManager::getExternalTypeName(env, firstQtType, QtJambiTypeManager::InstantiationType, firstData.metaTypeId);
                if(firstJavaType.isEmpty() && firstData.metaTypeId!=QMetaType::UnknownType && firstData.metaTypeId!=QMetaType::Void && firstData.metaTypeId!=QMetaType::Nullptr){
                    if(firstQtType.endsWith("*")){
                        firstJavaType = QLatin1String("io/qt/QNativePointer");
                        if(QMetaType(firstData.metaTypeId).flags() & QMetaType::PointerToQObject
                                || QMetaType(firstData.metaTypeId).flags() & QMetaType::SharedPointerToQObject
                                || QMetaType(firstData.metaTypeId).flags() & QMetaType::WeakPointerToQObject){
                            firstJavaType = QLatin1String("io/qt/core/QObject");
                        }else if(QMetaType(firstData.metaTypeId).flags() & QMetaType::PointerToGadget){
                             firstJavaType = ("io/qt/core/QMetaType$GenericGadget");
                        }
                    }else{
                        if(QMetaType(firstData.metaTypeId).flags() & QMetaType::IsGadget){
                             firstJavaType = QLatin1String("io/qt/core/QMetaType$GenericGadget");
                        }else{
                            firstJavaType = QLatin1String("io/qt/core/QMetaType$GenericValue");
                        }
                    }
                }
                // Find usage pattern
                QtJambiTypeManager::TypePattern firstInternalType = QtJambiTypeManager::typeIdOfInternal(env, firstJavaType, firstQtType, firstData.metaTypeId, nullptr);
                InternalToExternalConverter firstConverter = QtJambiTypeManager::getInternalToExternalConverter(
                                                        env,
                                                        firstQtType,
                                                        firstInternalType,
                                                        firstData.metaTypeId,
                                                        resolveClass(env, qPrintable(firstJavaType)),
                                                        firstInternalType==QtJambiTypeManager::NativePointer);

                const QtMetaTypePrivate::VariantData secondData = pair.second();
                QString secondQtType = QMetaType::typeName(secondData.metaTypeId);
                QString secondJavaType = QtJambiTypeManager::getExternalTypeName(env, secondQtType, QtJambiTypeManager::InstantiationType, secondData.metaTypeId);
                if(secondJavaType.isEmpty() && secondData.metaTypeId!=QMetaType::UnknownType && secondData.metaTypeId!=QMetaType::Void && secondData.metaTypeId!=QMetaType::Nullptr){
                    if(secondQtType.endsWith("*")){
                        secondJavaType = QLatin1String("io/qt/QNativePointer");
                        if(QMetaType(secondData.metaTypeId).flags() & QMetaType::PointerToQObject
                                || QMetaType(secondData.metaTypeId).flags() & QMetaType::SharedPointerToQObject
                                || QMetaType(secondData.metaTypeId).flags() & QMetaType::WeakPointerToQObject){
                            secondJavaType = QLatin1String("io/qt/core/QObject");
                        }else if(QMetaType(secondData.metaTypeId).flags() & QMetaType::PointerToGadget){
                             secondJavaType = QLatin1String("io/qt/core/QMetaType$GenericGadget");
                        }
                    }else{
                        if(QMetaType(secondData.metaTypeId).flags() & QMetaType::IsGadget){
                             secondJavaType = QLatin1String("io/qt/core/QMetaType$GenericGadget");
                        }else{
                            secondJavaType = QLatin1String("io/qt/core/QMetaType$GenericValue");
                        }
                    }
                }
                // Find usage pattern
                QtJambiTypeManager::TypePattern secondInternalType = QtJambiTypeManager::typeIdOfInternal(env, firstJavaType, secondQtType, secondData.metaTypeId, nullptr);
                InternalToExternalConverter secondConverter = QtJambiTypeManager::getInternalToExternalConverter(
                                                        env,
                                                        secondQtType,
                                                        secondInternalType,
                                                        secondData.metaTypeId,
                                                        resolveClass(env, qPrintable(secondJavaType)),
                                                        firstInternalType==QtJambiTypeManager::NativePointer);
                jvalue fval;
                fval.l = nullptr;
                if(!firstConverter || !firstConverter(env, nullptr, firstData.data, &fval, true)){
                    qWarning("Unable to convert qVariant to jobject");
                }
                jvalue sval;
                sval.l = nullptr;
                if(!secondConverter || !secondConverter(env, nullptr, secondData.data, &sval, true)){
                    qWarning("Unable to convert qVariant to jobject");
                }
                p->l = Java::Private::QtCore::QPair.newInstance(env, fval.l, sval.l);
            }
            return true;
        };
    }else if( ( (
                    (internalTypeName.startsWith("QVector<")
                     || internalTypeName.startsWith("QList<"))
                    &&
                    ( Java::Private::Runtime::ArrayList.isSameClass(_env,externalClass)
                      || Java::Private::Runtime::List.isSameClass(_env,externalClass)
                      || Java::Private::Runtime::Collection.isSameClass(_env,externalClass)
                      || Java::Private::Runtime::Object.isSameClass(_env,externalClass))
                )
                ||
                (
                    (internalTypeName.startsWith("QStack<")
                     || internalTypeName.startsWith("QQueue<"))
                    &&
                    (Java::Private::Runtime::ArrayDeque.isSameClass(_env,externalClass)
                     || Java::Private::Runtime::Deque.isSameClass(_env,externalClass)
                     || Java::Private::Runtime::Collection.isSameClass(_env,externalClass)
                     || Java::Private::Runtime::Object.isSameClass(_env,externalClass))
                )
                ||
                (
                    internalTypeName.startsWith("QLinkedList<")
                    &&
                    (Java::Private::Runtime::LinkedList.isSameClass(_env,externalClass)
                     || Java::Private::Runtime::Collection.isSameClass(_env,externalClass)
                     || Java::Private::Runtime::Object.isSameClass(_env,externalClass))
                )
                ||
                (
                    internalTypeName.startsWith("QSet<")
                    &&
                    (Java::Private::Runtime::HashSet.isSameClass(_env,externalClass)
                     || Java::Private::Runtime::Set.isSameClass(_env,externalClass)
                     || Java::Private::Runtime::Collection.isSameClass(_env,externalClass)
                     || Java::Private::Runtime::Object.isSameClass(_env,externalClass))
                )
             ) && internalTypeName.endsWith(">")){
        int idx = internalTypeName.indexOf("<");
        ContainerType containerType;
        if(internalTypeName.startsWith("QStack<")
                || internalTypeName.startsWith("QQueue<")){
            containerType = ContainerType::Deque;
        }else if(internalTypeName.startsWith("QLinkedList<")){
            containerType = ContainerType::LinkedList;
        }else if(internalTypeName.startsWith("QSet<")){
            containerType = ContainerType::Set;
        }else{
            containerType = ContainerType::List;
        }
        QString container = internalTypeName.mid(0, idx);
        QString instantiation = internalTypeName.mid(idx+1).chopped(1).trimmed();
        const std::type_info* t;
        bool isPointer = instantiation.endsWith("*");
        if(isPointer){
            t = getTypeByQtName(instantiation.chopped(1));
        }else{
            t = getTypeByQtName(instantiation);
        }
        QByteArray clazz;
        size_t size = 0;
        size_t align = 0;
        int memberMetaType = QMetaType::UnknownType;
        bool isStaticType = true;
        if(t){
            clazz = getJavaName(*t);
            if(isPointer){
                memberMetaType = QMetaType::type(qPrintable(instantiation));
                size = sizeof(void*);
            }else{
                memberMetaType = registeredMetaTypeID(*t);
                if(const QtJambiTypeInfo* typeInfo = getQTypeInfo(*t)){
                    isStaticType = typeInfo->isStatic;
                }
                size = getValueSize(*t);
                align = getValueAlignment(*t);
            }
        }else{
            memberMetaType = QMetaType::type(qPrintable(instantiation));
            isStaticType = QtJambiTypeManager::isStaticType(instantiation);
            if(memberMetaType != QMetaType::UnknownType){
                int sz = QMetaType::sizeOf(memberMetaType);
                size = sz<0 ? 0 : size_t(sz);
            }
            if(size==0){
                size = getInternalSize(instantiation);
            }
            align = getInternalAlignment(instantiation);
            if(memberMetaType==QMetaType::UnknownType){
                clazz = getExternalTypeName(_env, instantiation, ArgumentType, memberMetaType).toUtf8();
            }else{
                clazz = getExternalTypeName(_env, QMetaType::typeName(memberMetaType), ArgumentType, memberMetaType).toUtf8();
            }
        }

        if(clazz.isEmpty() && memberMetaType!=QMetaType::UnknownType && memberMetaType!=QMetaType::Void && memberMetaType!=QMetaType::Nullptr){
            if(isPointer){
                clazz = "io/qt/QNativePointer";
                if(QMetaType(memberMetaType).flags() & QMetaType::PointerToQObject
                        || QMetaType(memberMetaType).flags() & QMetaType::SharedPointerToQObject
                        || QMetaType(memberMetaType).flags() & QMetaType::WeakPointerToQObject){
                    clazz = "io/qt/core/QObject";
                }else if(QMetaType(memberMetaType).flags() & QMetaType::PointerToGadget){
                     clazz = "io/qt/core/QMetaType$GenericGadget";
                }
            }else{
                if(QMetaType(memberMetaType).flags() & QMetaType::IsGadget){
                     clazz = "io/qt/core/QMetaType$GenericGadget";
                }else{
                     clazz = "io/qt/core/QMetaType$GenericValue";
                }
            }
        }

        if(!clazz.isEmpty() && size>0){
            jclass javaClass = resolveClass(_env, clazz);
            QtJambiTypeManager::TypePattern internalTypePattern = typeIdOfInternal(_env, QString::fromUtf8(clazz), instantiation, memberMetaType, nullptr);
            const InternalToExternalConverter& memberConverter = QtJambiTypeManager::getInternalToExternalConverter(
                                                            _env,
                                                            instantiation,
                                                            internalTypePattern,
                                                            memberMetaType,
                                                            javaClass);
            if(memberMetaType == QMetaType::UnknownType){
                memberMetaType = QMetaType::type(qPrintable(instantiation));
                isStaticType = QtJambiTypeManager::isStaticType(instantiation);
                if(memberMetaType != QMetaType::UnknownType){
                    int sz = QMetaType::sizeOf(memberMetaType);
                    size = sz<0 ? 0 : size_t(sz);
                }
                if(size==0){
                    size = getInternalSize(instantiation);
                }
                align = getInternalAlignment(instantiation);
            }
            if(container=="QVector"){
                return ContainerConverter::getInternalToExternalConverter_QVector(internalTypeName, memberConverter, isPointer, isStaticType, align, size, memberMetaType, containerType);
            }else if(container=="QStack"){
                return ContainerConverter::getInternalToExternalConverter_QStack(internalTypeName, memberConverter, isPointer, isStaticType, align, size, memberMetaType, containerType);
            }else if(container=="QList"){
                return ContainerConverter::getInternalToExternalConverter_QList(internalTypeName, memberConverter, isPointer, isStaticType, align, size, memberMetaType, containerType);
            }else if(container=="QQueue"){
                return ContainerConverter::getInternalToExternalConverter_QQueue(internalTypeName, memberConverter, isPointer, isStaticType, align, size, memberMetaType, containerType);
            }else if(container=="QLinkedList"){
                return ContainerConverter::getInternalToExternalConverter_QLinkedList(internalTypeName, memberConverter, isPointer, isStaticType, align, size, memberMetaType, containerType);
            }else if(container=="QSet"){
                return ContainerConverter::getInternalToExternalConverter_QSet(internalTypeName, memberConverter, isPointer, isStaticType, align, size, memberMetaType, containerType);
            }
        }
    }else if( ( (
                    (internalTypeName.startsWith("QMap<")
                     || internalTypeName.startsWith("QMultiMap<"))
                    &&
                    ( Java::Private::Runtime::TreeMap.isSameClass(_env,externalClass)
                      || Java::Private::Runtime::NavigableMap.isSameClass(_env,externalClass)
                      || Java::Private::Runtime::Map.isSameClass(_env,externalClass)
                      || Java::Private::Runtime::Object.isSameClass(_env,externalClass))
                )
                ||
                (
                    (internalTypeName.startsWith("QHash<")
                     || internalTypeName.startsWith("QMultiHash<"))
                    &&
                    (Java::Private::Runtime::HashMap.isSameClass(_env,externalClass)
                     || Java::Private::Runtime::Map.isSameClass(_env,externalClass)
                     || Java::Private::Runtime::Object.isSameClass(_env,externalClass))
                )
             ) && internalTypeName.endsWith(">")){
        int idx = internalTypeName.indexOf("<");
        MapType mapType;
        if(internalTypeName.startsWith("QMap<")
                || internalTypeName.startsWith("QMultiMap<")){
            mapType = MapType::TreeMap;
        }else{
            mapType = MapType::HashMap;
        }
        QString container = internalTypeName.mid(0, idx);
        QStringList instantiations = internalTypeName.mid(idx+1).chopped(1).trimmed().split(",");
        reduceMapInstantiations(instantiations);
        if(instantiations.size()==2){
            const std::type_info* t1;
            bool isPointer1 = instantiations[0].endsWith("*");
            if(isPointer1){
                t1 = getTypeByQtName(instantiations[0].chopped(1));
            }else{
                t1 = getTypeByQtName(instantiations[0]);
            }
            QByteArray clazz1;
            size_t size1 = 0;
            size_t align1 = 0;
            bool isStaticType1 = true;
            int memberMetaType1 = QMetaType::UnknownType;
            if(t1){
                clazz1 = getJavaName(*t1);
                if(isPointer1){
                    size1 = sizeof(void*);
                    memberMetaType1 = QMetaType::type(qPrintable(instantiations[0]));
                }else{
                    size1 = getValueSize(*t1);
                    align1 = getValueAlignment(*t1);
                    memberMetaType1 = registeredMetaTypeID(*t1);
                    if(const QtJambiTypeInfo* typeInfo = getQTypeInfo(*t1)){
                        isStaticType1 = typeInfo->isStatic;
                    }
                }
            }else{
                memberMetaType1 = QMetaType::type(qPrintable(instantiations[0]));
                isStaticType1 = QtJambiTypeManager::isStaticType(instantiations[0]);
                if(memberMetaType1 != QMetaType::UnknownType){
                    int sz = QMetaType::sizeOf(memberMetaType1);
                    size1 = sz<0 ? 0 : size_t(sz);
                }
                if(size1==0){
                    size1 = getInternalSize(instantiations[0]);
                }
                align1 = getInternalAlignment(instantiations[0]);
                if(memberMetaType1==QMetaType::UnknownType){
                    clazz1 = getExternalTypeName(_env, instantiations[0], ArgumentType, memberMetaType1).toUtf8();
                }else{
                    clazz1 = getExternalTypeName(_env, QMetaType::typeName(memberMetaType1), ArgumentType, memberMetaType1).toUtf8();
                }
            }
            if(clazz1.isEmpty() && memberMetaType1!=QMetaType::UnknownType && memberMetaType1!=QMetaType::Void && memberMetaType1!=QMetaType::Nullptr){
                if(isPointer1){
                    clazz1 = "io/qt/QNativePointer";
                    if(QMetaType(memberMetaType1).flags() & QMetaType::PointerToQObject
                            || QMetaType(memberMetaType1).flags() & QMetaType::SharedPointerToQObject
                            || QMetaType(memberMetaType1).flags() & QMetaType::WeakPointerToQObject){
                         clazz1 = "io/qt/core/QObject";
                    }else if(QMetaType(memberMetaType1).flags() & QMetaType::PointerToGadget){
                         clazz1 = "io/qt/core/QMetaType$GenericGadget";
                    }
                }else{
                    if(QMetaType(memberMetaType1).flags() & QMetaType::IsGadget){
                         clazz1 = "io/qt/core/QMetaType$GenericGadget";
                    }else{
                         clazz1 = "io/qt/core/QMetaType$GenericValue";
                    }
                }
            }
            const std::type_info* t2;
            bool isPointer2 = instantiations[1].endsWith("*");
            if(isPointer2){
                t2 = getTypeByQtName(instantiations[1].chopped(1));
            }else{
                t2 = getTypeByQtName(instantiations[1]);
            }
            QByteArray clazz2;
            size_t size2 = 0;
            size_t align2 = 0;
            bool isStaticType2 = true;
            int memberMetaType2 = QMetaType::UnknownType;
            if(t2){
                clazz2 = getJavaName(*t2);
                if(isPointer2){
                    size2 = sizeof(void*);
                    memberMetaType2 = QMetaType::type(qPrintable(instantiations[1]));
                }else{
                    size2 = getValueSize(*t2);
                    align2 = getValueAlignment(*t2);
                    memberMetaType2 = registeredMetaTypeID(*t2);
                    if(const QtJambiTypeInfo* typeInfo = getQTypeInfo(*t2)){
                        isStaticType2 = typeInfo->isStatic;
                    }
                }
            }else{
                memberMetaType2 = QMetaType::type(qPrintable(instantiations[1]));
                isStaticType2 = QtJambiTypeManager::isStaticType(instantiations[1]);
                if(memberMetaType2 != QMetaType::UnknownType){
                    int sz = QMetaType::sizeOf(memberMetaType2);
                    size2 = sz<0 ? 0 : size_t(sz);
                }
                if(size2==0){
                    size2 = getInternalSize(instantiations[1]);
                }
                align2 = getInternalAlignment(instantiations[1]);
                if(memberMetaType2==QMetaType::UnknownType){
                    clazz2 = getExternalTypeName(_env, instantiations[1], ArgumentType, memberMetaType2).toUtf8();
                }else{
                    clazz2 = getExternalTypeName(_env, QMetaType::typeName(memberMetaType2), ArgumentType, memberMetaType2).toUtf8();
                }
            }
            if(clazz2.isEmpty() && memberMetaType2!=QMetaType::UnknownType && memberMetaType2!=QMetaType::Void && memberMetaType2!=QMetaType::Nullptr){
                if(isPointer2){
                    clazz2 = "io/qt/QNativePointer";
                    if(QMetaType(memberMetaType2).flags() & QMetaType::PointerToQObject
                            || QMetaType(memberMetaType2).flags() & QMetaType::SharedPointerToQObject
                            || QMetaType(memberMetaType2).flags() & QMetaType::WeakPointerToQObject){
                         clazz2 = "io/qt/core/QObject";
                    }else if(QMetaType(memberMetaType2).flags() & QMetaType::PointerToGadget){
                         clazz2 = "io/qt/core/QMetaType$GenericGadget";
                    }
                }else{
                    if(QMetaType(memberMetaType2).flags() & QMetaType::IsGadget){
                         clazz2 = "io/qt/core/QMetaType$GenericGadget";
                    }else{
                         clazz2 = "io/qt/core/QMetaType$GenericValue";
                    }
                }
            }

            if(!clazz1.isEmpty() && size1>0 && !clazz2.isEmpty() && size2>0){
                jclass javaClass1 = resolveClass(_env, clazz1);
                QtJambiTypeManager::TypePattern internalTypePattern1 = typeIdOfInternal(_env, QString::fromUtf8(clazz1), instantiations[0], memberMetaType1, nullptr);
                const InternalToExternalConverter& memberConverter1 = QtJambiTypeManager::getInternalToExternalConverter(
                                                                _env,
                                                                instantiations[0],
                                                                internalTypePattern1,
                                                                memberMetaType1,
                                                                javaClass1);
                if(memberMetaType1 == QMetaType::UnknownType){
                    memberMetaType1 = QMetaType::type(qPrintable(instantiations[0]));
                    isStaticType1 = QtJambiTypeManager::isStaticType(instantiations[0]);
                    if(memberMetaType1 != QMetaType::UnknownType){
                        int sz = QMetaType::sizeOf(memberMetaType1);
                        size1 = sz<0 ? 0 : size_t(sz);
                    }
                    if(size1==0){
                        size1 = getInternalSize(instantiations[0]);
                    }
                    align1 = getInternalAlignment(instantiations[0]);
                }
                jclass javaClass2 = resolveClass(_env, clazz2);
                QtJambiTypeManager::TypePattern internalTypePattern2 = typeIdOfInternal(_env, QString::fromUtf8(clazz2), instantiations[1], memberMetaType2, nullptr);
                const InternalToExternalConverter& memberConverter2 = QtJambiTypeManager::getInternalToExternalConverter(
                                                                _env,
                                                                instantiations[1],
                                                                internalTypePattern2,
                                                                memberMetaType2,
                                                                javaClass2);
                if(memberMetaType2 == QMetaType::UnknownType){
                    memberMetaType2 = QMetaType::type(qPrintable(instantiations[1]));
                    isStaticType2 = QtJambiTypeManager::isStaticType(instantiations[1]);
                    if(memberMetaType2 != QMetaType::UnknownType){
                        int sz = QMetaType::sizeOf(memberMetaType2);
                        size2 = sz<0 ? 0 : size_t(sz);
                    }
                    if(size2==0){
                        size2 = getInternalSize(instantiations[1]);
                    }
                    align2 = getInternalAlignment(instantiations[1]);
                }
                if(container=="QMap"){
                    return ContainerConverter::getInternalToExternalConverter_QMap(internalTypeName, memberConverter1, isPointer1, isStaticType1, align1, size1, memberMetaType1, memberConverter2, isPointer2, isStaticType2, align2, size2, memberMetaType2, mapType);
                }else if(container=="QMultiMap"){
                    return ContainerConverter::getInternalToExternalConverter_QMultiMap(internalTypeName, memberConverter1, isPointer1, isStaticType1, align1, size1, memberMetaType1, memberConverter2, isPointer2, isStaticType2, align2, size2, memberMetaType2, mapType);
                }else if(container=="QHash"){
                    return ContainerConverter::getInternalToExternalConverter_QHash(internalTypeName, memberConverter1, isPointer1, isStaticType1, align1, size1, memberMetaType1, memberConverter2, isPointer2, isStaticType2, align2, size2, memberMetaType2, mapType);
                }else if(container=="QMultiHash"){
                    return ContainerConverter::getInternalToExternalConverter_QMultiHash(internalTypeName, memberConverter1, isPointer1, isStaticType1, align1, size1, memberMetaType1, memberConverter2, isPointer2, isStaticType2, align2, size2, memberMetaType2, mapType);
                }
            }
        }
    }else if( ( (
                    (internalTypeName.startsWith("QPair<") || internalTypeName.startsWith("std::pair<"))
                    &&
                    ( Java::Private::QtCore::QPair.isAssignableFrom(_env,externalClass)
                      || Java::Private::Runtime::Object.isSameClass(_env,externalClass))
                )
             ) && internalTypeName.endsWith(">")){
        int idx = internalTypeName.indexOf("<");
        QString container = internalTypeName.mid(0, idx);
        QStringList instantiations = internalTypeName.mid(idx+1).chopped(1).trimmed().split(",");
        reduceMapInstantiations(instantiations);
        if(instantiations.size()==2){
            instantiations[0] = instantiations[0].trimmed();
            instantiations[1] = instantiations[1].trimmed();
            const std::type_info* t1;
            bool isPointer1 = instantiations[0].endsWith("*");
            bool isPointer2 = instantiations[1].endsWith("*");
            if(isPointer1){
                t1 = getTypeByQtName(instantiations[0].chopped(1));
            }else{
                t1 = getTypeByQtName(instantiations[0]);
            }
            const std::type_info* t2;
            if(isPointer2){
                t2 = getTypeByQtName(instantiations[1].chopped(1));
            }else{
                t2 = getTypeByQtName(instantiations[1]);
            }
            QByteArray class1;
            QByteArray class2;
            size_t size1 = 0;
            size_t size2 = 0;
            size_t align1 = 0;
            size_t align2 = 0;
            int metaType1 = QMetaType::UnknownType;
            int metaType2 = QMetaType::UnknownType;
            bool isStaticType1 = true;
            bool isStaticType2 = true;
            if(t1){
                class1 = getJavaName(*t1);
                if(isPointer1){
                    size1 = sizeof(void*);
                    metaType1 = QMetaType::type(qPrintable(instantiations[0]));
                }else{
                    size1 = getValueSize(*t1);
                    align1 = getValueAlignment(*t1);
                    metaType1 = registeredMetaTypeID(*t1);
                    if(const QtJambiTypeInfo* typeInfo = getQTypeInfo(*t1)){
                        isStaticType1 = typeInfo->isStatic;
                    }
                }
            }else{
                metaType1 = QMetaType::type(qPrintable(instantiations[0]));
                isStaticType1 = QtJambiTypeManager::isStaticType(instantiations[0]);
                if(metaType1 != QMetaType::UnknownType){
                    int sz = QMetaType::sizeOf(metaType1);
                    size1 = sz<0 ? 0 : size_t(sz);
                }
                if(size1==0){
                    size1 = getInternalSize(instantiations[0]);
                }
                align1 = getInternalAlignment(instantiations[0]);
                if(metaType1==QMetaType::UnknownType){
                    class1 = getExternalTypeName(_env, instantiations[0], ArgumentType, metaType1).toUtf8();
                }else{
                    class1 = getExternalTypeName(_env, QMetaType::typeName(metaType1), ArgumentType, metaType1).toUtf8();
                }
            }
            if(t2){
                class2 = getJavaName(*t2);
                if(isPointer2){
                    size2 = sizeof(void*);
                    metaType2 = QMetaType::type(qPrintable(instantiations[1]));
                }else{
                    size2 = getValueSize(*t2);
                    align2 = getValueAlignment(*t2);
                    metaType2 = registeredMetaTypeID(*t2);
                    if(const QtJambiTypeInfo* typeInfo = getQTypeInfo(*t2)){
                        isStaticType2 = typeInfo->isStatic;
                    }
                }
            }else{
                metaType2 = QMetaType::type(qPrintable(instantiations[1]));
                isStaticType2 = QtJambiTypeManager::isStaticType(instantiations[1]);
                if(metaType2 != QMetaType::UnknownType){
                    int sz = QMetaType::sizeOf(metaType2);
                    size2 = sz<0 ? 0 : size_t(sz);
                }
                if(size2==0){
                    size2 = getInternalSize(instantiations[1]);
                }
                align2 = getInternalAlignment(instantiations[1]);
                if(metaType2==QMetaType::UnknownType){
                    class2 = getExternalTypeName(_env, instantiations[1], ArgumentType, metaType2).toUtf8();
                    if(QMetaType(metaType1).flags() & QMetaType::PointerToQObject
                            || QMetaType(metaType1).flags() & QMetaType::SharedPointerToQObject
                            || QMetaType(metaType1).flags() & QMetaType::WeakPointerToQObject){
                        class1 = "io/qt/core/QObject";
                    }else if(QMetaType(metaType1).flags() & QMetaType::PointerToGadget){
                         class1 = "io/qt/core/QMetaType$GenericGadget";
                    }
                }else{
                    class2 = getExternalTypeName(_env, QMetaType::typeName(metaType2), ArgumentType, metaType2).toUtf8();
                }
            }

            if(class1.isEmpty() && metaType1!=QMetaType::UnknownType && metaType1!=QMetaType::Void && metaType1!=QMetaType::Nullptr){
                if(isPointer1){
                    class1 = "io/qt/QNativePointer";
                    if(QMetaType(metaType1).flags() & QMetaType::PointerToQObject
                            || QMetaType(metaType1).flags() & QMetaType::SharedPointerToQObject
                            || QMetaType(metaType1).flags() & QMetaType::WeakPointerToQObject){
                         class1 = "io/qt/core/QObject";
                    }else if(QMetaType(metaType1).flags() & QMetaType::PointerToGadget){
                         class1 = "io/qt/core/QMetaType$GenericGadget";
                    }
                }else{
                    if(QMetaType(metaType2).flags() & QMetaType::IsGadget){
                         class1 = "io/qt/core/QMetaType$GenericGadget";
                    }else{
                         class1 = "io/qt/core/QMetaType$GenericValue";
                    }
                }
            }

            if(class2.isEmpty() && metaType2!=QMetaType::UnknownType && metaType2!=QMetaType::Void && metaType2!=QMetaType::Nullptr){
                if(isPointer2){
                    class2 = "io/qt/QNativePointer";
                    if(QMetaType(metaType2).flags() & QMetaType::PointerToQObject
                            || QMetaType(metaType2).flags() & QMetaType::SharedPointerToQObject
                            || QMetaType(metaType2).flags() & QMetaType::WeakPointerToQObject){
                         class2 = "io/qt/core/QObject";
                    }else if(QMetaType(metaType2).flags() & QMetaType::PointerToGadget){
                         class2 = "io/qt/core/QMetaType$GenericGadget";
                    }
                }else{
                    if(QMetaType(metaType2).flags() & QMetaType::IsGadget){
                         class2 = "io/qt/core/QMetaType$GenericGadget";
                    }else{
                         class2 = "io/qt/core/QMetaType$GenericValue";
                    }
                }
            }

            if(!class1.isEmpty() && !class2.isEmpty() && size1>0 && size2>0){
                jclass javaClass1 = resolveClass(_env, class1);
                jclass javaClass2 = resolveClass(_env, class2);
                QtJambiTypeManager::TypePattern internalTypePattern1 = typeIdOfInternal(_env, QString::fromUtf8(class1), instantiations[0], metaType1, nullptr);
                QtJambiTypeManager::TypePattern internalTypePattern2 = typeIdOfInternal(_env, QString::fromUtf8(class2), instantiations[1], metaType2, nullptr);
                const InternalToExternalConverter& internalToExternalConverter1 = getInternalToExternalConverter(
                                                                _env,
                                                                instantiations[0],
                                                                internalTypePattern1,
                                                                metaType1,
                                                                javaClass1,true);
                if(metaType1 == QMetaType::UnknownType){
                    metaType1 = QMetaType::type(qPrintable(instantiations[0]));
                    isStaticType1 = QtJambiTypeManager::isStaticType(instantiations[0]);
                    if(metaType1 != QMetaType::UnknownType){
                        int sz = QMetaType::sizeOf(metaType1);
                        size1 = sz<0 ? 0 : size_t(sz);
                    }
                    if(size1==0){
                        size1 = getInternalSize(instantiations[0]);
                    }
                    align1 = getInternalAlignment(instantiations[0]);
                }
                const InternalToExternalConverter& internalToExternalConverter2 = getInternalToExternalConverter(
                                                                _env,
                                                                instantiations[1],
                                                                internalTypePattern2,
                                                                metaType2,
                                                                javaClass2,true);
                if(metaType2 == QMetaType::UnknownType){
                    metaType2 = QMetaType::type(qPrintable(instantiations[1]));
                    isStaticType2 = QtJambiTypeManager::isStaticType(instantiations[1]);
                    if(metaType2 != QMetaType::UnknownType){
                        int sz = QMetaType::sizeOf(metaType2);
                        size2 = sz<0 ? 0 : size_t(sz);
                    }
                    if(size2==0){
                        size2 = getInternalSize(instantiations[1]);
                    }
                    align2 = getInternalAlignment(instantiations[1]);
                }
                if(container=="QPair"){
                    return ContainerConverter::getInternalToExternalConverter_QPair(internalTypeName, internalToExternalConverter1, isPointer1, isStaticType1, align1, size1, metaType1, internalToExternalConverter2, isPointer2, isStaticType2, align2, size2, metaType2);
                }else if(container=="std::pair"){
                    return ContainerConverter::getInternalToExternalConverter_std_pair(internalTypeName, internalToExternalConverter1, isPointer1, isStaticType1, align1, size1, metaType1, internalToExternalConverter2, isPointer2, isStaticType2, align2, size2, metaType2);
                }
            }
        }
    } else if ([&]() -> bool {
               bool result = Java::Private::QtJambi::QtJambiInternal.isGeneratedClass(_env, externalClass);
               if(!result){
                   result = !getSuperTypeInfos(_env, externalClass).isEmpty();
               }
               return result;
          }()) {
        // If we're dealing with a QObject, then we try to find the original java instance
        const std::type_info* typeId = nullptr;
        if(internalMetaType!=QMetaType::UnknownType){
            typeId = getTypeByMetaType(internalMetaType);
        }
        if(!typeId){
            QString _internalTypeName = internalTypeName;
            _internalTypeName = _internalTypeName.replace("*", "");
            _internalTypeName = _internalTypeName.replace("&", "");
            _internalTypeName = _internalTypeName.replace("const", "").trimmed();
            typeId = getTypeByQtName(_internalTypeName);
        }
        if(!typeId){
            typeId = getTypeByJavaName(qtjambi_class_name(_env, externalClass).replace(".", "/"));
        }
        if(typeId){
            if (Java::Private::QtCore::QObject.isAssignableFrom(_env,externalClass)) {
                if(internalType & QtJambiTypeManager::SharedPointerWrapped){
                    return [typeId](JNIEnv* env, QtJambiScope*, const void* in, jvalue* p, bool)->bool{
                        const QSharedPointer<QObject>* ptr = reinterpret_cast<const QSharedPointer<QObject>*>(in);
                        if(ptr){
                            p->l = qtjambi_from_qobject_shared_pointer(env, *typeId,
                                                                       new QSharedPointer<QObject>(*ptr),
                                                                       &deletePointer<QSharedPointer, QObject>,
                                                                       &getFromPointer<QSharedPointer, QObject>);
                        }
                        return true;
                    };
                }else if(internalType & QtJambiTypeManager::WeakPointerWrapped){
                    return [typeId](JNIEnv* env, QtJambiScope*, const void* in, jvalue* p, bool)->bool{
                        const QWeakPointer<QObject>* ptr = reinterpret_cast<const QWeakPointer<QObject>*>(in);
                        if(ptr){
                            p->l = qtjambi_from_qobject_shared_pointer(env, *typeId,
                                                                       new QSharedPointer<QObject>(*ptr),
                                                                       &deletePointer<QSharedPointer, QObject>,
                                                                       &getFromPointer<QSharedPointer, QObject>);
                        }
                        return true;
                    };
                }else if(internalType & QtJambiTypeManager::SharedPtrWrapped){
                    return [typeId](JNIEnv* env, QtJambiScope*, const void* in, jvalue* p, bool)->bool{
                        const std::shared_ptr<QObject>* ptr = reinterpret_cast<const std::shared_ptr<QObject>*>(in);
                        if(ptr){
                            p->l = qtjambi_from_qobject_shared_pointer(env, *typeId,
                                                                       new std::shared_ptr<QObject>(*ptr),
                                                                       &deletePointer<std::shared_ptr, QObject>,
                                                                       &getFromPointer<std::shared_ptr, QObject>);
                        }
                        return true;
                    };
                }else if(internalType & QtJambiTypeManager::WeakPtrWrapped){
                    return [typeId](JNIEnv* env, QtJambiScope*, const void* in, jvalue* p, bool)->bool{
                        const std::weak_ptr<QObject>* ptr = reinterpret_cast<const std::weak_ptr<QObject>*>(in);
                        if(ptr){
                            p->l = qtjambi_from_qobject_shared_pointer(env, *typeId,
                                                                       new std::shared_ptr<QObject>(*ptr),
                                                                       &deletePointer<std::shared_ptr, QObject>,
                                                                       &getFromPointer<std::shared_ptr, QObject>);
                        }
                        return true;
                    };
                }else if(internalType & QtJambiTypeManager::ScopedPointerWrapped){
                    return [typeId](JNIEnv* env, QtJambiScope* scope, const void* in, jvalue* p, bool)->bool{
                        const QScopedPointer<QObject>* ptr = reinterpret_cast<const QScopedPointer<QObject>*>(in);
                        if(ptr){
                            p->l = qtjambi_from_qobject(env, ptr->data(), *typeId);
                            if(scope){
                                JObjectWrapper object(env, p->l);
                                scope->addFinalAction([object](){
                                    if(JNIEnv* env = qtjambi_current_environment()){
                                        qtjambi_invalidate_object(env, object.object(), false);
                                    }
                                });
                            }
                        }
                        return true;
                    };
                }else if(internalType & QtJambiTypeManager::UniquePtrWrapped){
                    return [typeId](JNIEnv* env, QtJambiScope* scope, const void* in, jvalue* p, bool)->bool{
                        const std::unique_ptr<QObject>* ptr = reinterpret_cast<const std::unique_ptr<QObject>*>(in);
                        if(ptr){
                            p->l = qtjambi_from_qobject(env, ptr->get(), *typeId);
                            if(scope){
                                JObjectWrapper object(env, p->l);
                                scope->addFinalAction([object](){
                                    if(JNIEnv* env = qtjambi_current_environment()){
                                        qtjambi_invalidate_object(env, object.object(), false);
                                    }
                                });
                            }
                        }
                        return true;
                    };
                }else{
                    return [typeId](JNIEnv* env, QtJambiScope*, const void* in, jvalue* p, bool)->bool{
                        QObject * const *ptr = reinterpret_cast<QObject * const *>(in);
                        if(ptr){
                            QObject * qobject = *ptr;
                            p->l = qtjambi_from_qobject(env, qobject, *typeId);
                        }
                        return true;
                    };
                }
            } else if(isInterface(*typeId)){
                if(internalType & QtJambiTypeManager::SharedPointerWrapped){
                    return [typeId](JNIEnv* env, QtJambiScope*, const void* in, jvalue* p, bool)->bool{
                        const QSharedPointer<void*>* ptr = reinterpret_cast<const QSharedPointer<void*>*>(in);
                        if(ptr){
                            p->l = qtjambi_from_interface_shared_pointer(env,
                                                                      *typeId,
                                                                      new QSharedPointer<void*>(*ptr),
                                                                      &deletePointer<QSharedPointer, void*>,
                                                                      &getFromPointer<QSharedPointer, void*>);
                        }
                        return true;
                    };
                }else if(internalType & QtJambiTypeManager::WeakPointerWrapped){
                    return [typeId](JNIEnv* env, QtJambiScope*, const void* in, jvalue* p, bool)->bool{
                        const QWeakPointer<void*>* ptr = reinterpret_cast<const QWeakPointer<void*>*>(in);
                        if(ptr){
                            p->l = qtjambi_from_interface_shared_pointer(env,
                                                                      *typeId,
                                                                      new QSharedPointer<void*>(*ptr),
                                                                      &deletePointer<QSharedPointer, void*>,
                                                                      &getFromPointer<QSharedPointer, void*>);
                        }
                        return true;
                    };
                }else if(internalType & QtJambiTypeManager::SharedPtrWrapped){
                    return [typeId](JNIEnv* env, QtJambiScope*, const void* in, jvalue* p, bool)->bool{
                        const std::shared_ptr<void*>* ptr = reinterpret_cast<const std::shared_ptr<void*>*>(in);
                        if(ptr){
                            p->l = qtjambi_from_interface_shared_pointer(env, *typeId,
                                                                       new std::shared_ptr<void*>(*ptr),
                                                                       &deletePointer<std::shared_ptr, void*>,
                                                                       &getFromPointer<std::shared_ptr, void*>);
                        }
                        return true;
                    };
                }else if(internalType & QtJambiTypeManager::WeakPtrWrapped){
                    return [typeId](JNIEnv* env, QtJambiScope*, const void* in, jvalue* p, bool)->bool{
                        const std::weak_ptr<void*>* ptr = reinterpret_cast<const std::weak_ptr<void*>*>(in);
                        if(ptr){
                            p->l = qtjambi_from_interface_shared_pointer(env, *typeId,
                                                                       new std::shared_ptr<void*>(*ptr),
                                                                       &deletePointer<std::shared_ptr, void*>,
                                                                       &getFromPointer<std::shared_ptr, void*>);
                        }
                        return true;
                    };
                }else if(internalType & QtJambiTypeManager::ScopedPointerWrapped){
                    return [typeId](JNIEnv* env, QtJambiScope* scope, const void* in, jvalue* p, bool)->bool{
                        const QScopedPointer<void*>* ptr = reinterpret_cast<const QScopedPointer<void*>*>(in);
                        if(ptr){
                            p->l = qtjambi_from_interface(env, ptr->data(), *typeId, false);
                            if(scope){
                                JObjectWrapper object(env, p->l);
                                scope->addFinalAction([object](){
                                    if(JNIEnv* env = qtjambi_current_environment()){
                                        qtjambi_invalidate_object(env, object.object(), false);
                                    }
                                });
                            }
                        }
                        return true;
                    };
                }else if(internalType & QtJambiTypeManager::UniquePtrWrapped){
                    return [typeId](JNIEnv* env, QtJambiScope* scope, const void* in, jvalue* p, bool)->bool{
                        const std::unique_ptr<void*>* ptr = reinterpret_cast<const std::unique_ptr<void*>*>(in);
                        if(ptr){
                            p->l = qtjambi_from_interface(env, ptr->get(), *typeId, false);
                            if(scope){
                                JObjectWrapper object(env, p->l);
                                scope->addFinalAction([object](){
                                    if(JNIEnv* env = qtjambi_current_environment()){
                                        qtjambi_invalidate_object(env, object.object(), false);
                                    }
                                });
                            }
                        }
                        return true;
                    };
                }else{
                    if ((allowValuePointers || (internalType & Object)) && internalTypeName.contains(QLatin1Char('*'))){
                        return [typeId](JNIEnv* env, QtJambiScope*, const void* in, jvalue* p, bool)->bool{
                            void * const *ptr = reinterpret_cast<void * const *>(in);
                            if(ptr){
                                // If we found a link for the object, we use the java object
                                // from the link.
                                if (QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForPointer(*ptr)) {
                                    jobject obj = link->getJavaObjectLocalRef(env);
                                    if(!obj && link->ownership()==QtJambiLink::Ownership::Split){
                                        link->invalidate(env);
                                        p->l = qtjambi_from_interface(env, *ptr, *typeId, false);
                                    }else{
                                        p->l = obj;
                                    }
                                }else{
                                    p->l = qtjambi_from_interface(env, *ptr, *typeId, false);
                                }
                            }
                            return true;
                        };
                    }else{
                        return [typeId](JNIEnv* env, QtJambiScope*, const void* in, jvalue* p, bool)->bool{
                            void * const *ptr = reinterpret_cast<void * const *>(in);
                            if(ptr){
                                p->l = qtjambi_from_interface(env, ptr, *typeId, true);
                            }
                            return true;
                        };
                    }
                }
            }else if(isPolymorphicBase(*typeId)){
                if(internalType & QtJambiTypeManager::SharedPointerWrapped){
                    return [typeId](JNIEnv* env, QtJambiScope*, const void* in, jvalue* p, bool)->bool{
                        const QSharedPointer<void*>* ptr = reinterpret_cast<const QSharedPointer<void*>*>(in);
                        if(ptr){
                            p->l = qtjambi_from_polymorphic_object_shared_pointer(env,
                                                                      *typeId,
                                                                      *typeId,
                                                                      new QSharedPointer<void*>(*ptr),
                                                                      &deletePointer<QSharedPointer, void*>,
                                                                      &getFromPointer<QSharedPointer, void*>);
                        }
                        return true;
                    };
                }else if(internalType & QtJambiTypeManager::WeakPointerWrapped){
                    return [typeId](JNIEnv* env, QtJambiScope*, const void* in, jvalue* p, bool)->bool{
                        const QWeakPointer<void*>* ptr = reinterpret_cast<const QWeakPointer<void*>*>(in);
                        if(ptr){
                            p->l = qtjambi_from_polymorphic_object_shared_pointer(env,
                                                                      *typeId,
                                                                      *typeId,
                                                                      new QSharedPointer<void*>(*ptr),
                                                                      &deletePointer<QSharedPointer, void*>,
                                                                      &getFromPointer<QSharedPointer, void*>);
                        }
                        return true;
                    };
                }else if(internalType & QtJambiTypeManager::SharedPtrWrapped){
                    return [typeId](JNIEnv* env, QtJambiScope*, const void* in, jvalue* p, bool)->bool{
                        const std::shared_ptr<void*>* ptr = reinterpret_cast<const std::shared_ptr<void*>*>(in);
                        if(ptr){
                            p->l = qtjambi_from_polymorphic_object_shared_pointer(env,
                                                                                  *typeId,
                                                                                  *typeId,
                                                                                   new std::shared_ptr<void*>(*ptr),
                                                                                   &deletePointer<std::shared_ptr, void*>,
                                                                                   &getFromPointer<std::shared_ptr, void*>);
                        }
                        return true;
                    };
                }else if(internalType & QtJambiTypeManager::WeakPtrWrapped){
                    return [typeId](JNIEnv* env, QtJambiScope*, const void* in, jvalue* p, bool)->bool{
                        const std::weak_ptr<void*>* ptr = reinterpret_cast<const std::weak_ptr<void*>*>(in);
                        if(ptr){
                            p->l = qtjambi_from_polymorphic_object_shared_pointer(env,
                                                                                  *typeId,
                                                                                  *typeId,
                                                                                   new std::shared_ptr<void*>(*ptr),
                                                                                   &deletePointer<std::shared_ptr, void*>,
                                                                                   &getFromPointer<std::shared_ptr, void*>);
                        }
                        return true;
                    };
                }else if(internalType & QtJambiTypeManager::ScopedPointerWrapped){
                    return [typeId](JNIEnv* env, QtJambiScope* scope, const void* in, jvalue* p, bool)->bool{
                        const QScopedPointer<void*>* ptr = reinterpret_cast<const QScopedPointer<void*>*>(in);
                        if(ptr){
                            p->l = qtjambi_from_polymorphic_object(env, ptr->data(), *typeId, *typeId, false);
                            if(scope){
                                JObjectWrapper object(env, p->l);
                                scope->addFinalAction([object](){
                                    if(JNIEnv* env = qtjambi_current_environment()){
                                        qtjambi_invalidate_object(env, object.object(), false);
                                    }
                                });
                            }
                        }
                        return true;
                    };
                }else if(internalType & QtJambiTypeManager::UniquePtrWrapped){
                    return [typeId](JNIEnv* env, QtJambiScope* scope, const void* in, jvalue* p, bool)->bool{
                        const std::unique_ptr<void*>* ptr = reinterpret_cast<const std::unique_ptr<void*>*>(in);
                        if(ptr){
                            p->l = qtjambi_from_polymorphic_object(env, ptr->get(), *typeId, *typeId, false);
                            if(scope){
                                JObjectWrapper object(env, p->l);
                                scope->addFinalAction([object](){
                                    if(JNIEnv* env = qtjambi_current_environment()){
                                        qtjambi_invalidate_object(env, object.object(), false);
                                    }
                                });
                            }
                        }
                        return true;
                    };
                }else{
                    if ((allowValuePointers || (internalType & Object)) && internalTypeName.contains(QLatin1Char('*'))){
                        return [typeId](JNIEnv* env, QtJambiScope* scope, const void* in, jvalue* p, bool)->bool{
                            void * const *ptr = reinterpret_cast<void * const *>(in);
                            if(ptr){
                                // If we found a link for the object, we use the java object
                                // from the link.
                                if (QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForPointer(*ptr)) {
                                    jobject obj = link->getJavaObjectLocalRef(env);
                                    if(!obj && link->ownership()==QtJambiLink::Ownership::Split){
                                        link->invalidate(env);
                                        p->l = qtjambi_from_polymorphic_object(env,
                                                               *ptr, *typeId, *typeId, false);
                                        if(scope){
                                            JObjectWrapper object(env, p->l);
                                            scope->addFinalAction([object](){
                                                if(JNIEnv* env = qtjambi_current_environment()){
                                                    qtjambi_invalidate_object(env, object.object());
                                                }
                                            });
                                        }
                                    }else{
                                        p->l = obj;
                                    }
                                }else{
                                    p->l = qtjambi_from_polymorphic_object(env,
                                                           *ptr, *typeId, *typeId, false);
                                    if(scope){
                                        JObjectWrapper object(env, p->l);
                                        scope->addFinalAction([object](){
                                            if(JNIEnv* env = qtjambi_current_environment()){
                                                qtjambi_invalidate_object(env, object.object());
                                            }
                                        });
                                    }
                                }
                            }
                            return true;
                        };
                    }else{
                        return [typeId](JNIEnv* env, QtJambiScope*, const void* in, jvalue* p, bool)->bool{
                            if(in){
                                p->l = qtjambi_from_polymorphic_object(env,
                                                       in, *typeId, *typeId, true);
                            }
                            return true;
                        };
                    }
                }
            }else if(const std::type_info* polymorphicBaseTypeId = getPolymorphicBase(*typeId)){
                if(internalType & QtJambiTypeManager::SharedPointerWrapped){
                    return [typeId, polymorphicBaseTypeId](JNIEnv* env, QtJambiScope*, const void* in, jvalue* p, bool)->bool{
                        const QSharedPointer<void*>* ptr = reinterpret_cast<const QSharedPointer<void*>*>(in);
                        if(ptr){
                            p->l = qtjambi_from_polymorphic_object_shared_pointer(env,
                                                                      *typeId,
                                                                      *polymorphicBaseTypeId,
                                                                      new QSharedPointer<void*>(*ptr),
                                                                      &deletePointer<QSharedPointer, void*>,
                                                                      &getFromPointer<QSharedPointer, void*>);
                        }
                        return true;
                    };
                }else if(internalType & QtJambiTypeManager::WeakPointerWrapped){
                    return [typeId, polymorphicBaseTypeId](JNIEnv* env, QtJambiScope*, const void* in, jvalue* p, bool)->bool{
                        const QWeakPointer<void*>* ptr = reinterpret_cast<const QWeakPointer<void*>*>(in);
                        if(ptr){
                            p->l = qtjambi_from_polymorphic_object_shared_pointer(env,
                                                                      *typeId,
                                                                      *polymorphicBaseTypeId,
                                                                      new QSharedPointer<void*>(*ptr),
                                                                      &deletePointer<QSharedPointer, void*>,
                                                                      &getFromPointer<QSharedPointer, void*>);
                        }
                        return true;
                    };
                }else if(internalType & QtJambiTypeManager::SharedPtrWrapped){
                    return [typeId, polymorphicBaseTypeId](JNIEnv* env, QtJambiScope*, const void* in, jvalue* p, bool)->bool{
                        const std::shared_ptr<void*>* ptr = reinterpret_cast<const std::shared_ptr<void*>*>(in);
                        if(ptr){
                            p->l = qtjambi_from_polymorphic_object_shared_pointer(env,
                                                                                  *typeId,
                                                                                  *polymorphicBaseTypeId,
                                                                                   new std::shared_ptr<void*>(*ptr),
                                                                                   &deletePointer<std::shared_ptr, void*>,
                                                                                   &getFromPointer<std::shared_ptr, void*>);
                        }
                        return true;
                    };
                }else if(internalType & QtJambiTypeManager::WeakPtrWrapped){
                    return [typeId, polymorphicBaseTypeId](JNIEnv* env, QtJambiScope*, const void* in, jvalue* p, bool)->bool{
                        const std::weak_ptr<void*>* ptr = reinterpret_cast<const std::weak_ptr<void*>*>(in);
                        if(ptr){
                            p->l = qtjambi_from_polymorphic_object_shared_pointer(env,
                                                                                  *typeId,
                                                                                  *polymorphicBaseTypeId,
                                                                                   new std::shared_ptr<void*>(*ptr),
                                                                                   &deletePointer<std::shared_ptr, void*>,
                                                                                   &getFromPointer<std::shared_ptr, void*>);
                        }
                        return true;
                    };
                }else if(internalType & QtJambiTypeManager::ScopedPointerWrapped){
                    return [typeId, polymorphicBaseTypeId](JNIEnv* env, QtJambiScope* scope, const void* in, jvalue* p, bool)->bool{
                        const QScopedPointer<void*>* ptr = reinterpret_cast<const QScopedPointer<void*>*>(in);
                        if(ptr){
                            p->l = qtjambi_from_polymorphic_object(env, ptr->data(), *typeId, *polymorphicBaseTypeId, false);
                            if(scope){
                                JObjectWrapper object(env, p->l);
                                scope->addFinalAction([object](){
                                    if(JNIEnv* env = qtjambi_current_environment()){
                                        qtjambi_invalidate_object(env, object.object(), false);
                                    }
                                });
                            }
                        }
                        return true;
                    };
                }else if(internalType & QtJambiTypeManager::UniquePtrWrapped){
                    return [typeId, polymorphicBaseTypeId](JNIEnv* env, QtJambiScope* scope, const void* in, jvalue* p, bool)->bool{
                        const std::unique_ptr<void*>* ptr = reinterpret_cast<const std::unique_ptr<void*>*>(in);
                        if(ptr){
                            p->l = qtjambi_from_polymorphic_object(env, ptr->get(), *typeId, *polymorphicBaseTypeId, false);
                            if(scope){
                                JObjectWrapper object(env, p->l);
                                scope->addFinalAction([object](){
                                    if(JNIEnv* env = qtjambi_current_environment()){
                                        qtjambi_invalidate_object(env, object.object(), false);
                                    }
                                });
                            }
                        }
                        return true;
                    };
                }else{
                    if ((allowValuePointers || (internalType & Object)) && internalTypeName.contains(QLatin1Char('*'))){
                        return [typeId, polymorphicBaseTypeId](JNIEnv* env, QtJambiScope* scope, const void* in, jvalue* p, bool)->bool{
                            void * const *ptr = reinterpret_cast<void * const *>(in);
                            if(ptr){
                                // If we found a link for the object, we use the java object
                                // from the link.
                                if (QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForPointer(*ptr)) {
                                    jobject obj = link->getJavaObjectLocalRef(env);
                                    if(!obj && link->ownership()==QtJambiLink::Ownership::Split){
                                        link->invalidate(env);
                                        p->l = qtjambi_from_polymorphic_object(env,
                                                               *ptr, *typeId, *polymorphicBaseTypeId, false);
                                        if(scope){
                                            JObjectWrapper object(env, p->l);
                                            scope->addFinalAction([object](){
                                                if(JNIEnv* env = qtjambi_current_environment()){
                                                    qtjambi_invalidate_object(env, object.object());
                                                }
                                            });
                                        }
                                    }else{
                                        p->l = obj;
                                    }
                                }else{
                                    p->l = qtjambi_from_polymorphic_object(env,
                                                           *ptr, *typeId, *polymorphicBaseTypeId, false);
                                    if(scope){
                                        JObjectWrapper object(env, p->l);
                                        scope->addFinalAction([object](){
                                            if(JNIEnv* env = qtjambi_current_environment()){
                                                qtjambi_invalidate_object(env, object.object());
                                            }
                                        });
                                    }
                                }
                            }
                            return true;
                        };
                    }else{
                        return [typeId, polymorphicBaseTypeId](JNIEnv* env, QtJambiScope*, const void* in, jvalue* p, bool)->bool{
                            if(in){
                                p->l = qtjambi_from_polymorphic_object(env,
                                                       in, *typeId, *polymorphicBaseTypeId, true);
                            }
                            return true;
                        };
                    }
                }
            }else {
                if(internalType & QtJambiTypeManager::SharedPointerWrapped){
                    return [typeId](JNIEnv* env, QtJambiScope*, const void* in, jvalue* p, bool)->bool{
                        const QSharedPointer<void*>* ptr = reinterpret_cast<const QSharedPointer<void*>*>(in);
                        if(ptr){
                            p->l = qtjambi_from_object_shared_pointer(env,
                                                                      *typeId,
                                                                      new QSharedPointer<void*>(*ptr),
                                                                      &deletePointer<QSharedPointer, void*>,
                                                                      &getFromPointer<QSharedPointer, void*>);
                        }
                        return true;
                    };
                }else if(internalType & QtJambiTypeManager::WeakPointerWrapped){
                    return [typeId](JNIEnv* env, QtJambiScope*, const void* in, jvalue* p, bool)->bool{
                        const QWeakPointer<void*>* ptr = reinterpret_cast<const QWeakPointer<void*>*>(in);
                        if(ptr){
                            p->l = qtjambi_from_object_shared_pointer(env,
                                                                      *typeId,
                                                                      new QSharedPointer<void*>(*ptr),
                                                                      &deletePointer<QSharedPointer, void*>,
                                                                      &getFromPointer<QSharedPointer, void*>);
                        }
                        return true;
                    };
                }else if(internalType & QtJambiTypeManager::SharedPtrWrapped){
                    return [typeId](JNIEnv* env, QtJambiScope*, const void* in, jvalue* p, bool)->bool{
                        const std::shared_ptr<void*>* ptr = reinterpret_cast<const std::shared_ptr<void*>*>(in);
                        if(ptr){
                            p->l = qtjambi_from_object_shared_pointer(env, *typeId,
                                                                       new std::shared_ptr<void*>(*ptr),
                                                                       &deletePointer<std::shared_ptr, void*>,
                                                                       &getFromPointer<std::shared_ptr, void*>);
                        }
                        return true;
                    };
                }else if(internalType & QtJambiTypeManager::WeakPtrWrapped){
                    return [typeId](JNIEnv* env, QtJambiScope*, const void* in, jvalue* p, bool)->bool{
                        const std::weak_ptr<void*>* ptr = reinterpret_cast<const std::weak_ptr<void*>*>(in);
                        if(ptr){
                            p->l = qtjambi_from_object_shared_pointer(env, *typeId,
                                                                       new std::shared_ptr<void*>(*ptr),
                                                                       &deletePointer<std::shared_ptr, void*>,
                                                                       &getFromPointer<std::shared_ptr, void*>);
                        }
                        return true;
                    };
                }else if(internalType & QtJambiTypeManager::ScopedPointerWrapped){
                    return [typeId](JNIEnv* env, QtJambiScope* scope, const void* in, jvalue* p, bool)->bool{
                        const QScopedPointer<void*>* ptr = reinterpret_cast<const QScopedPointer<void*>*>(in);
                        if(ptr){
                            p->l = qtjambi_from_object(env, ptr->data(), *typeId, false);
                            if(scope){
                                JObjectWrapper object(env, p->l);
                                scope->addFinalAction([object](){
                                    if(JNIEnv* env = qtjambi_current_environment()){
                                        qtjambi_invalidate_object(env, object.object(), false);
                                    }
                                });
                            }
                        }
                        return true;
                    };
                }else if(internalType & QtJambiTypeManager::UniquePtrWrapped){
                    return [typeId](JNIEnv* env, QtJambiScope* scope, const void* in, jvalue* p, bool)->bool{
                        const std::unique_ptr<void*>* ptr = reinterpret_cast<const std::unique_ptr<void*>*>(in);
                        if(ptr){
                            p->l = qtjambi_from_object(env, ptr->get(), *typeId, false);
                            if(scope){
                                JObjectWrapper object(env, p->l);
                                scope->addFinalAction([object](){
                                    if(JNIEnv* env = qtjambi_current_environment()){
                                        qtjambi_invalidate_object(env, object.object(), false);
                                    }
                                });
                            }
                        }
                        return true;
                    };
                }else{
                    if ((allowValuePointers || (internalType & Object)) && internalTypeName.contains(QLatin1Char('*'))){
                        return [typeId](JNIEnv* env, QtJambiScope* scope, const void* in, jvalue* p, bool)->bool{
                            void * const *ptr = reinterpret_cast<void * const *>(in);
                            if(ptr){
                                // If we found a link for the object, we use the java object
                                // from the link.
                                if (QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForPointer(*ptr)) {
                                    jobject obj = link->getJavaObjectLocalRef(env);
                                    if(!obj && link->ownership()==QtJambiLink::Ownership::Split){
                                        link->invalidate(env);
                                        p->l = qtjambi_from_object(env, in, *typeId, false);
                                        if(scope){
                                            JObjectWrapper object(env, p->l);
                                            scope->addFinalAction([object](){
                                                if(JNIEnv* env = qtjambi_current_environment()){
                                                    qtjambi_invalidate_object(env, object.object());
                                                }
                                            });
                                        }
                                    }else{
                                        p->l = obj;
                                    }
                                }else{
                                    p->l = qtjambi_from_object(env, *ptr, *typeId, false);
                                    if(scope){
                                        JObjectWrapper object(env, p->l);
                                        scope->addFinalAction([object](){
                                            if(JNIEnv* env = qtjambi_current_environment()){
                                                qtjambi_invalidate_object(env, object.object());
                                            }
                                        });
                                    }
                                }
                            }
                            return true;
                        };
                    }else{
                        if(internalType & Reference){
                            return [typeId](JNIEnv* env, QtJambiScope* scope, const void* in, jvalue* p, bool)->bool{
                                if(in){
                                    // If we found a link for the object, we use the java object
                                    // from the link.
                                    if (QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForPointer(in)) {
                                        jobject obj = link->getJavaObjectLocalRef(env);
                                        if(!obj && link->ownership()==QtJambiLink::Ownership::Split){
                                            link->invalidate(env);
                                            p->l = qtjambi_from_object(env, in, *typeId, false);
                                            if(scope){
                                                JObjectWrapper object(env, p->l);
                                                scope->addFinalAction([object](){
                                                    if(JNIEnv* env = qtjambi_current_environment()){
                                                        qtjambi_invalidate_object(env, object.object());
                                                    }
                                                });
                                            }
                                        }else{
                                            p->l = obj;
                                        }
                                    }else{
                                        p->l = qtjambi_from_object(env, in, *typeId, false);
                                        if(scope){
                                            JObjectWrapper object(env, p->l);
                                            scope->addFinalAction([object](){
                                                if(JNIEnv* env = qtjambi_current_environment()){
                                                    qtjambi_invalidate_object(env, object.object());
                                                }
                                            });
                                        }
                                    }
                                }
                                return true;
                            };
                        }else{
                            return [typeId](JNIEnv* env, QtJambiScope*, const void* in, jvalue* p, bool)->bool{
                                if(in){
                                    p->l = qtjambi_from_object(env, in, *typeId, true);
                                }
                                return true;
                            };
                        }
                    }
                }
            }
        }else{
            QString className = qtjambi_class_name(_env, externalClass).replace(".", "/");
            if (Java::Private::QtCore::QObject.isAssignableFrom(_env,externalClass)) {
                if(internalType & QtJambiTypeManager::SharedPointerWrapped){
                    return [className](JNIEnv* env, QtJambiScope*, const void* in, jvalue* p, bool)->bool{
                        const QSharedPointer<QObject>* ptr = reinterpret_cast<const QSharedPointer<QObject>*>(in);
                        if(ptr){
                            p->l = qtjambi_from_qobject_shared_pointer(env, qPrintable(className),
                                                                       new QSharedPointer<QObject>(*ptr),
                                                                       &deletePointer<QSharedPointer, QObject>,
                                                                       &getFromPointer<QSharedPointer, QObject>);
                        }
                        return true;
                    };
                }else if(internalType & QtJambiTypeManager::WeakPointerWrapped){
                    return [className](JNIEnv* env, QtJambiScope*, const void* in, jvalue* p, bool)->bool{
                        const QWeakPointer<QObject>* ptr = reinterpret_cast<const QWeakPointer<QObject>*>(in);
                        if(ptr){
                            p->l = qtjambi_from_qobject_shared_pointer(env, qPrintable(className),
                                                                       new QSharedPointer<QObject>(*ptr),
                                                                       &deletePointer<QSharedPointer, QObject>,
                                                                       &getFromPointer<QSharedPointer, QObject>);
                        }
                        return true;
                    };
                }else if(internalType & QtJambiTypeManager::SharedPtrWrapped){
                    return [className](JNIEnv* env, QtJambiScope*, const void* in, jvalue* p, bool)->bool{
                        const std::shared_ptr<QObject>* ptr = reinterpret_cast<const std::shared_ptr<QObject>*>(in);
                        if(ptr){
                            p->l = qtjambi_from_qobject_shared_pointer(env, qPrintable(className),
                                                                       new std::shared_ptr<QObject>(*ptr),
                                                                       &deletePointer<std::shared_ptr, QObject>,
                                                                       &getFromPointer<std::shared_ptr, QObject>);
                        }
                        return true;
                    };
                }else if(internalType & QtJambiTypeManager::WeakPtrWrapped){
                    return [className](JNIEnv* env, QtJambiScope*, const void* in, jvalue* p, bool)->bool{
                        const std::weak_ptr<QObject>* ptr = reinterpret_cast<const std::weak_ptr<QObject>*>(in);
                        if(ptr){
                            p->l = qtjambi_from_qobject_shared_pointer(env, qPrintable(className),
                                                                       new std::shared_ptr<QObject>(*ptr),
                                                                       &deletePointer<std::shared_ptr, QObject>,
                                                                       &getFromPointer<std::shared_ptr, QObject>);
                        }
                        return true;
                    };
                }else if(internalType & QtJambiTypeManager::ScopedPointerWrapped){
                    return [className](JNIEnv* env, QtJambiScope* scope, const void* in, jvalue* p, bool)->bool{
                        const QScopedPointer<QObject>* ptr = reinterpret_cast<const QScopedPointer<QObject>*>(in);
                        if(ptr){
                            p->l = qtjambi_from_qobject(env, ptr->data(), qPrintable(className));
                            if(scope){
                                JObjectWrapper object(env, p->l);
                                scope->addFinalAction([object](){
                                    if(JNIEnv* env = qtjambi_current_environment()){
                                        qtjambi_invalidate_object(env, object.object(), false);
                                    }
                                });
                            }
                        }
                        return true;
                    };
                }else if(internalType & QtJambiTypeManager::UniquePtrWrapped){
                    return [className](JNIEnv* env, QtJambiScope* scope, const void* in, jvalue* p, bool)->bool{
                        const std::unique_ptr<QObject>* ptr = reinterpret_cast<const std::unique_ptr<QObject>*>(in);
                        if(ptr){
                            p->l = qtjambi_from_qobject(env, ptr->get(), qPrintable(className));
                            if(scope){
                                JObjectWrapper object(env, p->l);
                                scope->addFinalAction([object](){
                                    if(JNIEnv* env = qtjambi_current_environment()){
                                        qtjambi_invalidate_object(env, object.object(), false);
                                    }
                                });
                            }
                        }
                        return true;
                    };
                }else{
                    JObjectWrapper external_class(_env, externalClass);
                    return [external_class](JNIEnv* env, QtJambiScope*, const void* in, jvalue* p, bool)->bool{
                        QObject * const *ptr = reinterpret_cast<QObject * const *>(in);
                        if(ptr){
                            QObject * qobject = *ptr;
                            p->l = qtjambi_from_qobject(env, qobject, jclass(external_class.object()));
                        }
                        return true;
                    };
                }
            } else if(internalType & QtJambiTypeManager::SharedPointerWrapped){
                return [className](JNIEnv* env, QtJambiScope*, const void* in, jvalue* p, bool)->bool{
                    const QSharedPointer<void*>* ptr = reinterpret_cast<const QSharedPointer<void*>*>(in);
                    if(ptr){
                        p->l = qtjambi_from_object_shared_pointer(env,
                                                                  qPrintable(className),
                                                                  new QSharedPointer<void*>(*ptr),
                                                                  &deletePointer<QSharedPointer, void*>,
                                                                  &getFromPointer<QSharedPointer, void*>);
                    }
                    return true;
                };
            }else if(internalType & QtJambiTypeManager::WeakPointerWrapped){
                return [className](JNIEnv* env, QtJambiScope*, const void* in, jvalue* p, bool)->bool{
                    const QWeakPointer<void*>* ptr = reinterpret_cast<const QWeakPointer<void*>*>(in);
                    if(ptr){
                        p->l = qtjambi_from_object_shared_pointer(env,
                                                                  qPrintable(className),
                                                                  new QSharedPointer<void*>(*ptr),
                                                                  &deletePointer<QSharedPointer, void*>,
                                                                  &getFromPointer<QSharedPointer, void*>);
                    }
                    return true;
                };
            }else if(internalType & QtJambiTypeManager::SharedPtrWrapped){
                return [className](JNIEnv* env, QtJambiScope*, const void* in, jvalue* p, bool)->bool{
                    const std::shared_ptr<void*>* ptr = reinterpret_cast<const std::shared_ptr<void*>*>(in);
                    if(ptr){
                        p->l = qtjambi_from_object_shared_pointer(env, qPrintable(className),
                                                                   new std::shared_ptr<void*>(*ptr),
                                                                   &deletePointer<std::shared_ptr, void*>,
                                                                   &getFromPointer<std::shared_ptr, void*>);
                    }
                    return true;
                };
            }else if(internalType & QtJambiTypeManager::WeakPtrWrapped){
                return [className](JNIEnv* env, QtJambiScope*, const void* in, jvalue* p, bool)->bool{
                    const std::weak_ptr<void*>* ptr = reinterpret_cast<const std::weak_ptr<void*>*>(in);
                    if(ptr){
                        p->l = qtjambi_from_object_shared_pointer(env, qPrintable(className),
                                                                   new std::shared_ptr<void*>(*ptr),
                                                                   &deletePointer<std::shared_ptr, void*>,
                                                                   &getFromPointer<std::shared_ptr, void*>);
                    }
                    return true;
                };
            }else if(internalType & QtJambiTypeManager::ScopedPointerWrapped){
                return [className](JNIEnv* env, QtJambiScope* scope, const void* in, jvalue* p, bool)->bool{
                    const QScopedPointer<void*>* ptr = reinterpret_cast<const QScopedPointer<void*>*>(in);
                    if(ptr){
                        p->l = qtjambi_from_object(env, ptr->data(), qPrintable(className), false);
                        if(scope){
                            JObjectWrapper object(env, p->l);
                            scope->addFinalAction([object](){
                                if(JNIEnv* env = qtjambi_current_environment()){
                                    qtjambi_invalidate_object(env, object.object(), false);
                                }
                            });
                        }
                    }
                    return true;
                };
            }else if(internalType & QtJambiTypeManager::UniquePtrWrapped){
                return [className](JNIEnv* env, QtJambiScope* scope, const void* in, jvalue* p, bool)->bool{
                    const std::unique_ptr<void*>* ptr = reinterpret_cast<const std::unique_ptr<void*>*>(in);
                    if(ptr){
                        p->l = qtjambi_from_object(env, ptr->get(), qPrintable(className), false);
                        if(scope){
                            JObjectWrapper object(env, p->l);
                            scope->addFinalAction([object](){
                                if(JNIEnv* env = qtjambi_current_environment()){
                                    qtjambi_invalidate_object(env, object.object(), false);
                                }
                            });
                        }
                    }
                    return true;
                };
            }else{
                JObjectWrapper external_class(_env, externalClass);
                if ((allowValuePointers || (internalType & Object)) && internalTypeName.contains(QLatin1Char('*'))){
                    return [external_class](JNIEnv* env, QtJambiScope* scope, const void* in, jvalue* p, bool)->bool{
                        void * const *ptr = reinterpret_cast<void * const *>(in);
                        if(ptr){
                            // If we found a link for the object, we use the java object
                            // from the link.
                            if (QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForPointer(*ptr)) {
                                jobject obj = link->getJavaObjectLocalRef(env);
                                if(!obj && link->ownership()==QtJambiLink::Ownership::Split){
                                    link->invalidate(env);
                                    p->l = qtjambi_from_object(env, *ptr, jclass(external_class.object()), false);
                                    if(scope){
                                        JObjectWrapper object(env, p->l);
                                        scope->addFinalAction([object](){
                                            if(JNIEnv* env = qtjambi_current_environment()){
                                                qtjambi_invalidate_object(env, object.object());
                                            }
                                        });
                                    }
                                }else{
                                    p->l = obj;
                                }
                            }else{
                                p->l = qtjambi_from_object(env, *ptr, jclass(external_class.object()), false);
                                if(scope){
                                    JObjectWrapper object(env, p->l);
                                    scope->addFinalAction([object](){
                                        if(JNIEnv* env = qtjambi_current_environment()){
                                            qtjambi_invalidate_object(env, object.object());
                                        }
                                    });
                                }
                            }
                        }
                        return true;
                    };
                }else{
                    return [external_class](JNIEnv* env, QtJambiScope*, const void* in, jvalue* p, bool)->bool{
                        if(in){
                            p->l = qtjambi_from_object(env, in, jclass(external_class.object()), true);
                        }
                        return true;
                    };
                }
            }
        }
    } else if (internalMetaType == qMetaTypeId<JObjectWrapper>()) {
        return [](JNIEnv* env, QtJambiScope*, const void* in, jvalue* p, bool)->bool{
            p->l = env->NewLocalRef(reinterpret_cast<const JObjectWrapper *>(in)->object());
            return true;
        };
    }else if (internalMetaType == qMetaTypeId<JCollectionWrapper>()) {
        return [](JNIEnv* env, QtJambiScope*, const void* in, jvalue* p, bool)->bool{
            p->l = env->NewLocalRef(reinterpret_cast<const JCollectionWrapper *>(in)->object());
            return true;
        };
    }else if (internalMetaType == qMetaTypeId<JMapWrapper>()) {
        return [](JNIEnv* env, QtJambiScope*, const void* in, jvalue* p, bool)->bool{
            p->l = env->NewLocalRef(reinterpret_cast<const JMapWrapper *>(in)->object());
            return true;
        };
    }else if (internalMetaType == qMetaTypeId<JIteratorWrapper>()) {
        return [](JNIEnv* env, QtJambiScope*, const void* in, jvalue* p, bool)->bool{
            p->l = env->NewLocalRef(reinterpret_cast<const JIteratorWrapper *>(in)->object());
            return true;
        };
    }else if (internalMetaType == qMetaTypeId<QVariant>()) {
        return [](JNIEnv* env, QtJambiScope*, const void* in, jvalue* p, bool)->bool{
            const QVariant & variant = *reinterpret_cast<const QVariant*>(in);
            p->l = qtjambi_from_qvariant(env, variant);
            return true;
        };
    }else if(const QMetaObject* metaObject = QMetaType::metaObjectForType(internalMetaType)){
        if(metaObject->inherits(&QObject::staticMetaObject)){
            return [](JNIEnv* env, QtJambiScope*, const void* in, jvalue* p, bool)->bool{
                const QVariant & variant = *reinterpret_cast<const QVariant*>(in);
                p->l = qtjambi_from_QObject(env, variant.value<QObject*>());
                return true;
            };
        }else if(const std::type_info* interfaceTypeId = getTypeByMetaType(internalMetaType)){
            return [interfaceTypeId](JNIEnv* env, QtJambiScope*, const void* in, jvalue* p, bool)->bool{
                p->l = qtjambi_from_object(env, in, *interfaceTypeId, true, false);
                return true;
            };
        }else if(const std::type_info* interfaceTypeId = getTypeByQtName(metaObject->className())){
            return [interfaceTypeId](JNIEnv* env, QtJambiScope*, const void* in, jvalue* p, bool)->bool{
                p->l = qtjambi_from_object(env, in, *interfaceTypeId, true, false);
                return true;
            };
        }
    }

    qWarning("Cannot convert to type '%s' from '%s'.",
             qPrintable(qtjambi_class_name(_env, externalClass).replace("$", ".")), qPrintable(internalTypeName));

    return ParameterTypeInfo::default_internalToExternalConverter();
}

QVariant int_for_QtEnumerator_or_QFlags(JNIEnv* env, jobject enum_value) {
    if (env->IsSameObject(enum_value, nullptr))
        return 0;

    QVariant result;
    if (Java::Private::QtJambi::QFlags.isInstanceOf(env, enum_value)) {
        result = QVariant::fromValue<qint32>(Java::Private::QtJambi::QFlags.value(env,enum_value));
    }else if (Java::Private::QtJambi::QtEnumerator.isInstanceOf(env, enum_value)) {
        result = QVariant::fromValue<qint32>(Java::Private::QtJambi::QtEnumerator.value(env,enum_value));
    }else if (Java::Private::QtJambi::QtShortEnumerator.isInstanceOf(env, enum_value)) {
        result = QVariant::fromValue<qint16>(Java::Private::QtJambi::QtShortEnumerator.value(env,enum_value));
    }else if (Java::Private::QtJambi::QtByteEnumerator.isInstanceOf(env, enum_value)) {
        result = QVariant::fromValue<qint8>(Java::Private::QtJambi::QtByteEnumerator.value(env,enum_value));
    }else if (Java::Private::QtJambi::QtLongEnumerator.isInstanceOf(env, enum_value)) {
        result = QVariant::fromValue<qint64>(Java::Private::QtJambi::QtLongEnumerator.value(env,enum_value));
    }else if(Java::Runtime::Long.isInstanceOf(env, enum_value)){
        result = QVariant::fromValue<qint64>(Java::Runtime::Number.longValue(env, enum_value));
    }else if(Java::Runtime::Integer.isInstanceOf(env, enum_value)){
        result = QVariant::fromValue<qint32>(Java::Runtime::Number.intValue(env, enum_value));
    }else if(Java::Runtime::Short.isInstanceOf(env, enum_value)){
        result = QVariant::fromValue<qint16>(Java::Runtime::Number.shortValue(env, enum_value));
    }else if(Java::Runtime::Byte.isInstanceOf(env, enum_value)){
        result = QVariant::fromValue<qint8>(Java::Runtime::Number.byteValue(env, enum_value));
    }else  if(Java::Private::Runtime::Enum.isInstanceOf(env, enum_value)){
        result = QVariant::fromValue<qint32>(Java::Private::Runtime::Enum.ordinal(env,enum_value));
    }
    return result;
}

const ExternalToInternalConverter& QtJambiTypeManager::getExternalToInternalConverter(JNIEnv* env, jclass externalClass, TypePattern internalType, const QString &internalTypeName, int internalMetaType) {
    uint key = hashSum({uint(Java::Private::Runtime::Object.hashCode(env,externalClass)), qHash(internalTypeName), uint(internalMetaType)});
    {
        QReadLocker locker(gCacheLock());
        Q_UNUSED(locker)
        if(gExternalToInternalConverters()->contains(key))
            return (*gExternalToInternalConverters())[key];
    }
    ExternalToInternalConverter converter = getExternalToInternalConverterImpl(env, externalClass, internalType, internalTypeName, internalMetaType);
    Q_ASSERT(converter);
    QWriteLocker locker(gCacheLock());
    Q_UNUSED(locker)
    gExternalToInternalConverters()->insert(key, converter);
    return (*gExternalToInternalConverters())[key];
}

ExternalToInternalConverter QtJambiTypeManager::getExternalToInternalConverterImpl(JNIEnv* _env, jclass externalClass, TypePattern internalType, const QString &internalTypeName, int internalMetaType) {
    bool isArrayClass = Java::Private::Runtime::Class.isArray(_env, externalClass);

    if (internalMetaType == qMetaTypeId<QVariant>()) {
        return [](JNIEnv* env, QtJambiScope* scope, const jvalue& val, void* &out, jValueType valueType) -> ConvertResponse{
            if(scope && !out){
                QVariant* ptr;
                out = ptr = new QVariant();
                scope->addFinalAction([ptr](){
                    delete ptr;
                });
            }
            if(!out)
                return ConvertResponse::NotSuccessful;
            switch(valueType){
            case jValueType::l:
                *reinterpret_cast<QVariant*>(out) = qtjambi_to_qvariant(env, val.l);
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
            return ConvertResponse::Successful;
        };
    }

    if(internalMetaType==qMetaTypeId<JObjectWrapper>()
            || internalMetaType==qMetaTypeId<JEnumWrapper>()
            || internalMetaType==qMetaTypeId<JCollectionWrapper>()
            || internalMetaType==qMetaTypeId<JMapWrapper>()
            || internalMetaType==qMetaTypeId<JIteratorWrapper>()){
        return [](JNIEnv* env, QtJambiScope* scope, const jvalue& val, void* &out, jValueType) -> ConvertResponse{
            if(scope && !out){
                JObjectWrapper* ptr;
                out = ptr = new JObjectWrapper;
                scope->addFinalAction([ptr](){
                    delete ptr;
                });
            }
            if(!out)
                return ConvertResponse::NotSuccessful;
            *reinterpret_cast<JObjectWrapper*>(out) = JObjectWrapper(env, val.l);
            return ConvertResponse::Successful;
        };
    }else if(Java::Runtime::Void.isPrimitiveType(_env,externalClass) || Java::Runtime::Void.isSameClass(_env,externalClass) || internalMetaType==QMetaType::Nullptr){
        return [](JNIEnv*, QtJambiScope*, const jvalue&, void* &, jValueType) -> ConvertResponse{
            return ConvertResponse::Successful;
        };
    }else if(Java::Runtime::Integer.isPrimitiveType(_env,externalClass) || Java::Runtime::Integer.isSameClass(_env,externalClass)){
        if(internalMetaType==QMetaType::UnknownType || internalMetaType==qMetaTypeId<qint32>()){
            return [](JNIEnv* env, QtJambiScope* scope, const jvalue& val, void* &out, jValueType valueType) -> ConvertResponse{
                jint v;
                switch(valueType){
                case jValueType::l:
                    v = qtjambi_to_int(env, val.l);
                    break;
                default:
                    v = val.i;
                    break;
                }
                if(scope && !out){
                    qint32 * ptr;
                    out = ptr = new qint32;
                    scope->addFinalAction([ptr](){
                        delete ptr;
                    });
                }
                if(!out)
                    return ConvertResponse::NotSuccessful;
                memcpy(out, &v, 4);
                return ConvertResponse::Successful;
            };
        }else{
            return [internalMetaType](JNIEnv* env, QtJambiScope* scope, const jvalue& val, void* &out, jValueType valueType) -> ConvertResponse{
                jint v;
                switch(valueType){
                case jValueType::l:
                    v = qtjambi_to_int(env, val.l);
                    break;
                default:
                    v = val.i;
                    break;
                }
                if(scope && !out){
                    void* ptr;
                    out = ptr = QMetaType::create(internalMetaType, &v);
                    scope->addFinalAction([internalMetaType,ptr](){
                        QMetaType::destroy(internalMetaType, ptr);
                    });
                }else{
                    if(!out)
                        return ConvertResponse::NotSuccessful;
                    QMetaType::destruct(internalMetaType, out);
                    QByteArray ba;
                    bool saved;
                    /* write the copy to the stream */ {
                        QDataStream stream(&ba, QIODevice::WriteOnly);
                        saved = QMetaType::save(stream, qMetaTypeId<qint32>(), &v);
                    }

                    /* read it back into the destination */
                    if(saved){
                        QDataStream stream(&ba, QIODevice::ReadOnly);
                        QMetaType::load(stream, internalMetaType, out);
                    }else if(QMetaType::construct(internalMetaType, out, &v)!=out){
                        return ConvertResponse::NotSuccessful;
                    }
                }
                return ConvertResponse::Successful;
            };
        }
    }else if(Java::Runtime::Long.isPrimitiveType(_env,externalClass) || Java::Runtime::Long.isSameClass(_env,externalClass)){
        if(internalMetaType==QMetaType::UnknownType || internalMetaType==qMetaTypeId<qint64>()){
            return [](JNIEnv* env, QtJambiScope* scope, const jvalue& val, void* &out, jValueType valueType) -> ConvertResponse{
                jlong v;
                switch(valueType){
                case jValueType::l:
                    v = qtjambi_to_long(env, val.l);
                    break;
                default:
                    v = val.j;
                    break;
                }
                if(scope && !out){
                    qint64 *ptr;
                    out = ptr = new qint64(v);
                    scope->addFinalAction([ptr](){
                        delete ptr;
                    });
                }
                if(!out)
                    return ConvertResponse::NotSuccessful;
                memcpy(out, &v, 8);
                return ConvertResponse::Successful;
            };
        }else{
            return [internalMetaType](JNIEnv* env, QtJambiScope* scope, const jvalue& val, void* &out, jValueType valueType) -> ConvertResponse{
                jlong v;
                switch(valueType){
                case jValueType::l:
                    v = qtjambi_to_long(env, val.l);
                    break;
                default:
                    v = val.j;
                    break;
                }
                if(scope && !out){
                    void* ptr;
                    out = ptr = QMetaType::create(internalMetaType, &v);
                    scope->addFinalAction([internalMetaType,ptr](){
                        QMetaType::destroy(internalMetaType, ptr);
                    });
                }else{
                    if(!out)
                        return ConvertResponse::NotSuccessful;
                    QMetaType::destruct(internalMetaType, out);
                    QByteArray ba;
                    bool saved;
                    /* write the copy to the stream */ {
                        QDataStream stream(&ba, QIODevice::WriteOnly);
                        saved = QMetaType::save(stream, qMetaTypeId<qint64>(), &v);
                    }

                    /* read it back into the destination */
                    if(saved){
                        QDataStream stream(&ba, QIODevice::ReadOnly);
                        QMetaType::load(stream, internalMetaType, out);
                    }else if(QMetaType::construct(internalMetaType, out, &v)!=out){
                        return ConvertResponse::NotSuccessful;
                    }
                }
                return ConvertResponse::Successful;
            };
        }
    }else if(Java::Runtime::Short.isPrimitiveType(_env,externalClass) || Java::Runtime::Short.isSameClass(_env,externalClass)){
        if(internalMetaType==QMetaType::UnknownType || internalMetaType==qMetaTypeId<qint16>()){
            return [](JNIEnv* env, QtJambiScope* scope, const jvalue& val, void* &out, jValueType valueType) -> ConvertResponse{
                jshort v;
                switch(valueType){
                case jValueType::l:
                    v = qtjambi_to_short(env, val.l);
                    break;
                default:
                    v = val.s;
                    break;
                }
                if(scope && !out){
                    qint16* ptr;
                    out = ptr = new qint16(v);
                    scope->addFinalAction([ptr](){
                        delete ptr;
                    });
                }
                if(!out)
                    return ConvertResponse::NotSuccessful;
                memcpy(out, &v, 2);
                return ConvertResponse::Successful;
            };
        }else{
            return [internalMetaType](JNIEnv* env, QtJambiScope* scope, const jvalue& val, void* &out, jValueType valueType) -> ConvertResponse{
                jshort v;
                switch(valueType){
                case jValueType::l:
                    v = qtjambi_to_short(env, val.l);
                    break;
                default:
                    v = val.s;
                    break;
                }
                if(scope && !out){
                    void* ptr;
                    out = ptr = QMetaType::create(internalMetaType, &v);
                    scope->addFinalAction([internalMetaType,ptr](){
                        QMetaType::destroy(internalMetaType, ptr);
                    });
                }else{
                    if(!out)
                        return ConvertResponse::NotSuccessful;
                    QMetaType::destruct(internalMetaType, out);
                    QByteArray ba;
                    bool saved;
                    /* write the copy to the stream */ {
                        QDataStream stream(&ba, QIODevice::WriteOnly);
                        saved = QMetaType::save(stream, qMetaTypeId<qint16>(), &v);
                    }

                    /* read it back into the destination */
                    if(saved){
                        QDataStream stream(&ba, QIODevice::ReadOnly);
                        QMetaType::load(stream, internalMetaType, out);
                    }else if(QMetaType::construct(internalMetaType, out, &v)!=out){
                        return ConvertResponse::NotSuccessful;
                    }
                }
                return ConvertResponse::Successful;
            };
        }
    }else if(Java::Runtime::Byte.isPrimitiveType(_env,externalClass) || Java::Runtime::Byte.isSameClass(_env,externalClass)){
        if(internalMetaType==QMetaType::UnknownType || internalMetaType==qMetaTypeId<qint8>()){
            return [](JNIEnv* env, QtJambiScope* scope, const jvalue& val, void* &out, jValueType valueType) -> ConvertResponse{
                jbyte v;
                switch(valueType){
                case jValueType::l:
                    v = qtjambi_to_byte(env, val.l);
                    break;
                default:
                    v = val.b;
                    break;
                }
                if(scope && !out){
                    qint8* ptr;
                    out = ptr = new qint8(v);
                    scope->addFinalAction([ptr](){
                        delete ptr;
                    });
                }
                if(!out)
                    return ConvertResponse::NotSuccessful;
                memcpy(out, &v, 1);
                return ConvertResponse::Successful;
            };
        }else{
            return [internalMetaType](JNIEnv* env, QtJambiScope* scope, const jvalue& val, void* &out, jValueType valueType) -> ConvertResponse{
                jbyte v;
                switch(valueType){
                case jValueType::l:
                    v = qtjambi_to_byte(env, val.l);
                    break;
                default:
                    v = val.b;
                    break;
                }
                if(scope && !out){
                    void* ptr;
                    out = ptr = QMetaType::create(internalMetaType, &v);
                    scope->addFinalAction([internalMetaType,ptr](){
                        QMetaType::destroy(internalMetaType, ptr);
                    });
                }else{
                    if(!out)
                        return ConvertResponse::NotSuccessful;
                    QMetaType::destruct(internalMetaType, out);
                    QByteArray ba;
                    bool saved;
                    /* write the copy to the stream */ {
                        QDataStream stream(&ba, QIODevice::WriteOnly);
                        saved = QMetaType::save(stream, qMetaTypeId<qint8>(), &v);
                    }

                    /* read it back into the destination */
                    if(saved){
                        QDataStream stream(&ba, QIODevice::ReadOnly);
                        QMetaType::load(stream, internalMetaType, out);
                    }else if(QMetaType::construct(internalMetaType, out, &v)!=out){
                        return ConvertResponse::NotSuccessful;
                    }
                }
                return ConvertResponse::Successful;
            };
        }
    }else if(Java::Runtime::Boolean.isPrimitiveType(_env,externalClass) || Java::Runtime::Boolean.isSameClass(_env,externalClass)){
        if(internalMetaType==QMetaType::UnknownType || internalMetaType==qMetaTypeId<bool>()){
            return [](JNIEnv* env, QtJambiScope* scope, const jvalue& val, void* &out, jValueType valueType) -> ConvertResponse{
                jboolean v;
                switch(valueType){
                case jValueType::l:
                    v = qtjambi_to_boolean(env, val.l);
                    break;
                default:
                    v = val.z;
                    break;
                }
                if(scope && !out){
                    bool* ptr;
                    out = ptr = new bool(v);
                    scope->addFinalAction([ptr](){
                        delete ptr;
                    });
                }
                if(!out)
                    return ConvertResponse::NotSuccessful;
                *reinterpret_cast<bool*>(out) = v;
                return ConvertResponse::Successful;
            };
        }else{
            return [internalMetaType](JNIEnv* env, QtJambiScope* scope, const jvalue& val, void* &out, jValueType valueType) -> ConvertResponse{
                jboolean v;
                switch(valueType){
                case jValueType::l:
                    v = qtjambi_to_boolean(env, val.l);
                    break;
                default:
                    v = val.z;
                    break;
                }
                if(scope && !out){
                    void* ptr;
                    out = ptr = QMetaType::create(internalMetaType, &v);
                    scope->addFinalAction([internalMetaType,ptr](){
                        QMetaType::destroy(internalMetaType, ptr);
                    });
                }else{
                    if(!out)
                        return ConvertResponse::NotSuccessful;
                    QMetaType::destruct(internalMetaType, out);
                    QByteArray ba;
                    bool saved;
                    /* write the copy to the stream */ {
                        QDataStream stream(&ba, QIODevice::WriteOnly);
                        saved = QMetaType::save(stream, qMetaTypeId<bool>(), &v);
                    }

                    /* read it back into the destination */
                    if(saved){
                        QDataStream stream(&ba, QIODevice::ReadOnly);
                        QMetaType::load(stream, internalMetaType, out);
                    }else if(QMetaType::construct(internalMetaType, out, &v)!=out){
                        return ConvertResponse::NotSuccessful;
                    }
                }
                return ConvertResponse::Successful;
            };
        }
    }else if(Java::Runtime::Character.isPrimitiveType(_env,externalClass) || Java::Runtime::Character.isSameClass(_env,externalClass)){
        if(internalMetaType==qMetaTypeId<QChar>()){
            return [](JNIEnv* env, QtJambiScope* scope, const jvalue& val, void* &out, jValueType valueType) -> ConvertResponse{
                jchar v;
                switch(valueType){
                case jValueType::l:
                    v = qtjambi_to_char(env, val.l);
                    break;
                default:
                    v = val.c;
                    break;
                }
                if(scope && !out){
                    QChar* ptr;
                    out = ptr = new QChar(v);
                    scope->addFinalAction([ptr](){
                        delete ptr;
                    });
                }
                if(!out)
                    return ConvertResponse::NotSuccessful;
                *reinterpret_cast<QChar*>(out) = v;
                return ConvertResponse::Successful;
            };
        }else if(internalMetaType==QMetaType::UnknownType || internalMetaType==qMetaTypeId<unsigned short>()){
            return [](JNIEnv* env, QtJambiScope* scope, const jvalue& val, void* &out, jValueType valueType) -> ConvertResponse{
                jchar v;
                switch(valueType){
                case jValueType::l:
                    v = qtjambi_to_char(env, val.l);
                    break;
                default:
                    v = val.c;
                    break;
                }
                if(scope && !out){
                    unsigned short* ptr;
                    out = ptr = new unsigned short(v);
                    scope->addFinalAction([ptr](){
                        delete ptr;
                    });
                }
                if(!out)
                    return ConvertResponse::NotSuccessful;
                *reinterpret_cast<unsigned short*>(out) = v;
                return ConvertResponse::Successful;
            };
        }else{
            return [internalMetaType](JNIEnv* env, QtJambiScope* scope, const jvalue& val, void* &out, jValueType valueType) -> ConvertResponse{
                jchar v;
                switch(valueType){
                case jValueType::l:
                    v = qtjambi_to_char(env, val.l);
                    break;
                default:
                    v = val.c;
                    break;
                }
                if(scope && !out){
                    void* ptr;
                    out = ptr = QMetaType::create(internalMetaType, &v);
                    scope->addFinalAction([internalMetaType,ptr](){
                        QMetaType::destroy(internalMetaType, ptr);
                    });
                }else{
                    if(!out)
                        return ConvertResponse::NotSuccessful;
                    QMetaType::destruct(internalMetaType, out);
                    QByteArray ba;
                    bool saved;
                    /* write the copy to the stream */ {
                        QDataStream stream(&ba, QIODevice::WriteOnly);
                        saved = QMetaType::save(stream, qMetaTypeId<unsigned short>(), &v);
                    }

                    /* read it back into the destination */
                    if(saved){
                        QDataStream stream(&ba, QIODevice::ReadOnly);
                        QMetaType::load(stream, internalMetaType, out);
                    }else if(QMetaType::construct(internalMetaType, out, &v)!=out){
                        return ConvertResponse::NotSuccessful;
                    }
                }
                return ConvertResponse::Successful;
            };
        }
    }else if(Java::Runtime::Float.isPrimitiveType(_env,externalClass) || Java::Runtime::Float.isSameClass(_env,externalClass)){
        if(internalMetaType==QMetaType::UnknownType || internalMetaType==qMetaTypeId<float>()){
            return [](JNIEnv* env, QtJambiScope* scope, const jvalue& val, void* &out, jValueType valueType) -> ConvertResponse{
                jfloat v;
                switch(valueType){
                case jValueType::l:
                    v = qtjambi_to_float(env, val.l);
                    break;
                default:
                    v = val.f;
                    break;
                }
                if(scope && !out){
                    float* ptr;
                    out = ptr = new float(v);
                    scope->addFinalAction([ptr](){
                        delete ptr;
                    });
                }
                if(!out)
                    return ConvertResponse::NotSuccessful;
                *reinterpret_cast<float*>(out) = v;
                return ConvertResponse::Successful;
            };
        }else{
            return [internalMetaType](JNIEnv* env, QtJambiScope* scope, const jvalue& val, void* &out, jValueType valueType) -> ConvertResponse{
                jfloat v;
                switch(valueType){
                case jValueType::l:
                    v = qtjambi_to_float(env, val.l);
                    break;
                default:
                    v = val.f;
                    break;
                }
                if(scope && !out){
                    void* ptr;
                    out = ptr = QMetaType::create(internalMetaType, &v);
                    scope->addFinalAction([internalMetaType,ptr](){
                        QMetaType::destroy(internalMetaType, ptr);
                    });
                }else{
                    if(!out)
                        return ConvertResponse::NotSuccessful;
                    QMetaType::destruct(internalMetaType, out);
                    QByteArray ba;
                    bool saved;
                    /* write the copy to the stream */ {
                        QDataStream stream(&ba, QIODevice::WriteOnly);
                        saved = QMetaType::save(stream, qMetaTypeId<float>(), &v);
                    }

                    /* read it back into the destination */
                    if(saved){
                        QDataStream stream(&ba, QIODevice::ReadOnly);
                        QMetaType::load(stream, internalMetaType, out);
                    }else if(QMetaType::construct(internalMetaType, out, &v)!=out){
                        return ConvertResponse::NotSuccessful;
                    }
                }
                return ConvertResponse::Successful;
            };
        }
    }else if(Java::Runtime::Double.isPrimitiveType(_env,externalClass) || Java::Runtime::Double.isSameClass(_env,externalClass)){
        if(internalMetaType==QMetaType::UnknownType || internalMetaType==qMetaTypeId<double>()){
            return [](JNIEnv* env, QtJambiScope* scope, const jvalue& val, void* &out, jValueType valueType) -> ConvertResponse{
                jdouble v;
                switch(valueType){
                case jValueType::l:
                    v = qtjambi_to_double(env, val.l);
                    break;
                default:
                    v = val.d;
                    break;
                }
                if(scope && !out){
                    double* ptr;
                    out = ptr = new double(v);
                    scope->addFinalAction([ptr](){
                        delete ptr;
                    });
                }
                if(!out)
                    return ConvertResponse::NotSuccessful;
                *reinterpret_cast<double*>(out) = v;
                return ConvertResponse::Successful;
            };
        }else{
            return [internalMetaType](JNIEnv* env, QtJambiScope* scope, const jvalue& val, void* &out, jValueType valueType) -> ConvertResponse{
                jdouble v;
                switch(valueType){
                case jValueType::l:
                    v = qtjambi_to_double(env, val.l);
                    break;
                default:
                    v = val.d;
                    break;
                }
                if(scope && !out){
                    void* ptr;
                    out = ptr = QMetaType::create(internalMetaType, &v);
                    scope->addFinalAction([internalMetaType,ptr](){
                        QMetaType::destroy(internalMetaType, ptr);
                    });
                }else{
                    if(!out)
                        return ConvertResponse::NotSuccessful;
                    QMetaType::destruct(internalMetaType, out);
                    QByteArray ba;
                    bool saved;
                    /* write the copy to the stream */ {
                        QDataStream stream(&ba, QIODevice::WriteOnly);
                        saved = QMetaType::save(stream, qMetaTypeId<double>(), &v);
                    }

                    /* read it back into the destination */
                    if(saved){
                        QDataStream stream(&ba, QIODevice::ReadOnly);
                        QMetaType::load(stream, internalMetaType, out);
                    }else if(QMetaType::construct(internalMetaType, out, &v)!=out){
                        return ConvertResponse::NotSuccessful;
                    }
                }
                return ConvertResponse::Successful;
            };
        }
    }else if(Java::Runtime::String.isSameClass(_env,externalClass)){
        if(internalMetaType==QMetaType::UnknownType || internalMetaType==qMetaTypeId<QString>()){
            return [](JNIEnv* env, QtJambiScope* scope, const jvalue& val, void* &out, jValueType) -> ConvertResponse{
                if(scope && !out){
                    QString* ptr;
                    out = ptr = new QString;
                    scope->addFinalAction([ptr](){
                        delete ptr;
                    });
                }
                if(!out)
                    return ConvertResponse::NotSuccessful;
                jstring strg;
                if(!val.l || Java::Runtime::String.isInstanceOf(env, val.l)){
                    strg = reinterpret_cast<jstring>(val.l);
                }else{
                    strg = Java::Private::Runtime::Object.toString(env,val.l);
                }
                *reinterpret_cast<QString*>(out) = qtjambi_to_qstring(env, strg);
                return ConvertResponse::Successful;
            };
        }else{
            return [internalMetaType](JNIEnv* env, QtJambiScope* scope, const jvalue& val, void* &out, jValueType) -> ConvertResponse{
                jstring strg;
                if(!val.l || Java::Runtime::String.isInstanceOf(env, val.l)){
                    strg = reinterpret_cast<jstring>(val.l);
                }else{
                    strg = Java::Private::Runtime::Object.toString(env,val.l);
                }
                QString s(qtjambi_to_qstring(env, strg));
                if(scope && !out){
                    void* ptr;
                    out = ptr = QMetaType::create(internalMetaType, &s);
                    scope->addFinalAction([internalMetaType,ptr](){
                        QMetaType::destroy(internalMetaType, ptr);
                    });
                }else{
                    if(!out)
                        return ConvertResponse::NotSuccessful;
                    QMetaType::destruct(internalMetaType, out);
                    QByteArray ba;
                    bool saved;
                    /* write the copy to the stream */ {
                        QDataStream stream(&ba, QIODevice::WriteOnly);
                        saved = QMetaType::save(stream, qMetaTypeId<QString>(), &s);
                    }

                    /* read it back into the destination */
                    if(saved){
                        QDataStream stream(&ba, QIODevice::ReadOnly);
                        QMetaType::load(stream, internalMetaType, out);
                    }else if(QMetaType::construct(internalMetaType, out, &s)!=out){
                        return ConvertResponse::NotSuccessful;
                    }
                }
                return ConvertResponse::Successful;
            };
        }
    }else if(Java::Private::QtJambi::QNativePointer.isSameClass(_env,externalClass)){
        if(internalMetaType==QMetaType::UnknownType){
            return [](JNIEnv* env, QtJambiScope* scope, const jvalue& val, void* &out, jValueType) -> ConvertResponse{
                if(scope && !out){
                    void** ptr;
                    out = ptr = new void*(qtjambi_to_cpointer(env, val.l, 1));
                    scope->addFinalAction([ptr](){
                            delete ptr;
                        });
                    return ConvertResponse::Successful;
                }else if(out){
                    *reinterpret_cast<void**>(out) = qtjambi_to_cpointer(env, val.l, 1);
                    return ConvertResponse::Successful;
                }
                return ConvertResponse::NotSuccessful;
            };
        }else{
            return [internalMetaType](JNIEnv* env, QtJambiScope* scope, const jvalue& val, void* &out, jValueType) -> ConvertResponse{
                void* nptr = qtjambi_to_cpointer(env, val.l, 1);
                if(scope && !out){
                    void* ptr;
                    out = ptr = QMetaType::create(internalMetaType, nptr);
                    scope->addFinalAction([internalMetaType,ptr](){
                        QMetaType::destroy(internalMetaType, ptr);
                    });
                }else if(out){
                    QMetaType::destruct(internalMetaType, out);
                    if(QMetaType::construct(internalMetaType, out, nptr)!=out){
                        QByteArray ba;
                        bool saved;
                        /* write the copy to the stream */ {
                            QDataStream stream(&ba, QIODevice::WriteOnly);
                            saved = QMetaType::save(stream, internalMetaType, nptr);
                        }

                        /* read it back into the destination */
                        if(saved){
                            QDataStream stream(&ba, QIODevice::ReadOnly);
                            QMetaType::load(stream, internalMetaType, out);
                        }else{
                            return ConvertResponse::NotSuccessful;
                        }
                    }
                }else{
                    out = nptr;
                }
                return ConvertResponse::Successful;
            };
        }
    } else if (Java::Private::QtCore::QMetaType$GenericValue.isSameClass(_env,externalClass)) {
        if(internalType & Pointer){
            return [internalMetaType](JNIEnv* env, QtJambiScope*, const jvalue& val, void* &out, jValueType) -> ConvertResponse{
                if(val.l){
                    int given = Java::Private::QtCore::QMetaType$GenericTypeInterface.metaType(env, val.l);
                    if(internalMetaType!=given){
                        JavaException::raiseIllegalArgumentException(env, qPrintable(QString("Wrong argument given: %1, expected: %2").arg(QMetaType::typeName(given)).arg(QMetaType::typeName(internalMetaType))) QTJAMBI_STACKTRACEINFO );
                    }
                }
                if(const QSharedPointer<QtJambiLink>& link = QtJambiLink::findLinkForJavaObject(env, val.l)){
                    out = link->pointer();
                    return ConvertResponse::Successful;
                }else{
                    out = nullptr;
                    return ConvertResponse::JavaObjectNotLinked;
                }
            };
        }else{
            return [internalMetaType](JNIEnv* env, QtJambiScope* scope, const jvalue& val, void* &out, jValueType) -> ConvertResponse{
                if(val.l){
                    int given = Java::Private::QtCore::QMetaType$GenericTypeInterface.metaType(env, val.l);
                    if(internalMetaType!=given){
                        JavaException::raiseIllegalArgumentException(env, qPrintable(QString("Wrong argument given: %1, expected: %2").arg(QMetaType::typeName(given)).arg(QMetaType::typeName(internalMetaType))) QTJAMBI_STACKTRACEINFO );
                    }
                }
                if(scope && !out){
                    void** ptr;
                    out = ptr = new void*;
                    scope->addFinalAction([ptr](){
                        delete ptr;
                    });
                }
                if(!out)
                    return ConvertResponse::NotSuccessful;
                if(const QSharedPointer<QtJambiLink>& link = QtJambiLink::findLinkForJavaObject(env, val.l)){
                    *reinterpret_cast<void**>(out) = link->pointer();
                    return ConvertResponse::Successful;
                }else{
                    *reinterpret_cast<void**>(out) = nullptr;
                    return ConvertResponse::JavaObjectNotLinked;
                }
            };
        }
    } else if (Java::Private::QtCore::QMetaType$GenericGadget.isSameClass(_env,externalClass)) {
        if(internalType & Reference){
            return [internalMetaType](JNIEnv* env, QtJambiScope*, const jvalue& val, void* &out, jValueType) -> ConvertResponse{
                if(val.l){
                    int given = Java::Private::QtCore::QMetaType$GenericTypeInterface.metaType(env, val.l);
                    if(internalMetaType!=given){
                        JavaException::raiseIllegalArgumentException(env, qPrintable(QString("Wrong argument given: %1, expected: %2").arg(QMetaType::typeName(given)).arg(QMetaType::typeName(internalMetaType))) QTJAMBI_STACKTRACEINFO );
                    }
                }
                if(const QSharedPointer<QtJambiLink>& link = QtJambiLink::findLinkForJavaObject(env, val.l)){
                    out = link->pointer();
                    return ConvertResponse::Successful;
                }else{
                    out = nullptr;
                    return ConvertResponse::JavaObjectNotLinked;
                }
            };
        }else{
            return [internalMetaType](JNIEnv* env, QtJambiScope* scope, const jvalue& val, void* &out, jValueType) -> ConvertResponse{
                if(val.l){
                    int given = Java::Private::QtCore::QMetaType$GenericTypeInterface.metaType(env, val.l);
                    if(internalMetaType!=given){
                        JavaException::raiseIllegalArgumentException(env, qPrintable(QString("Wrong argument given: %1, expected: %2").arg(QMetaType::typeName(given)).arg(QMetaType::typeName(internalMetaType))) QTJAMBI_STACKTRACEINFO );
                    }
                }
                if(scope && !out){
                    void** ptr;
                    out = ptr = new void*;
                    scope->addFinalAction([ptr](){
                        delete ptr;
                    });
                }
                if(!out)
                    return ConvertResponse::NotSuccessful;
                if(const QSharedPointer<QtJambiLink>& link = QtJambiLink::findLinkForJavaObject(env, val.l)){
                    *reinterpret_cast<void**>(out) = link->pointer();
                    return ConvertResponse::Successful;
                }else{
                    *reinterpret_cast<void**>(out) = nullptr;
                    return ConvertResponse::JavaObjectNotLinked;
                }
            };
        }
    }else if(Java::Private::QtJambi::QFlags.isAssignableFrom(_env,externalClass)){
        if(internalMetaType==qMetaTypeId<JObjectWrapper>()){
            return [](JNIEnv* env, QtJambiScope* scope, const jvalue& val, void* &out, jValueType valueType) -> ConvertResponse{
                jobject v;
                switch(valueType){
                case jValueType::l:
                    v = val.l;
                    break;
                default:
                    v = qtjambi_from_int(env, val.i);
                    break;
                }
                if(scope && !out){
                    JObjectWrapper* ptr;
                    out = ptr = new JObjectWrapper;
                    scope->addFinalAction([ptr](){
                        delete ptr;
                    });
                }
                if(!out)
                    return ConvertResponse::NotSuccessful;
                *reinterpret_cast<JObjectWrapper*>(out) = JObjectWrapper(env, v);
                return ConvertResponse::Successful;
            };
        }else {
            return [internalMetaType](JNIEnv* env, QtJambiScope* scope, const jvalue& val, void* &out, jValueType valueType) -> ConvertResponse{
                switch(valueType){
                case jValueType::l:
                    if(Java::Private::QtCore::QMetaType$GenericTypeInterface.isInstanceOf(env,val.l)){
                        int given = Java::Private::QtCore::QMetaType$GenericTypeInterface.metaType(env, val.l);
                        if(internalMetaType!=given){
                            JavaException::raiseIllegalArgumentException(env, qPrintable(QString("Wrong argument given: %1, expected: %2").arg(QMetaType::typeName(given)).arg(QMetaType::typeName(internalMetaType))) QTJAMBI_STACKTRACEINFO );
                        }
                    }
                    if(scope && !out){
                        qint32* ptr;
                        out = ptr = new qint32;
                        scope->addFinalAction([ptr](){
                            delete ptr;
                        });
                    }
                    if(!out)
                        return ConvertResponse::NotSuccessful;
                    *reinterpret_cast<qint32*>(out) = int_for_QtEnumerator_or_QFlags(env, val.l).value<qint32>();
                    break;
                default:
                    if(scope && !out){
                        qint32* ptr;
                        out = ptr = new qint32;
                        scope->addFinalAction([ptr](){
                            delete ptr;
                        });
                    }
                    if(!out)
                        return ConvertResponse::NotSuccessful;
                    *reinterpret_cast<qint32*>(out) = val.i;
                    break;
                }
                return ConvertResponse::Successful;
            };
        }
    }else if(Java::Private::QtJambi::QtEnumerator.isAssignableFrom(_env,externalClass)
             || Java::Private::QtJambi::QtLongEnumerator.isAssignableFrom(_env,externalClass)
             || Java::Private::QtJambi::QtShortEnumerator.isAssignableFrom(_env,externalClass)
             || Java::Private::QtJambi::QtByteEnumerator.isAssignableFrom(_env,externalClass)
             || Java::Private::Runtime::Enum.isAssignableFrom(_env,externalClass)){
        if(internalMetaType==qMetaTypeId<JEnumWrapper>() || internalMetaType==qMetaTypeId<JObjectWrapper>()){
            return [](JNIEnv* env, QtJambiScope* scope, const jvalue& val, void* &out, jValueType valueType) -> ConvertResponse{
                jobject v;
                switch(valueType){
                case jValueType::l:
                    v = val.l;
                    break;
                default:
                    v = qtjambi_from_int(env, val.i);
                    break;
                }
                if(scope && !out){
                    JEnumWrapper* ptr;
                    out = ptr = new JEnumWrapper;
                    scope->addFinalAction([ptr](){
                        delete ptr;
                    });
                }
                if(!out)
                    return ConvertResponse::NotSuccessful;
                *reinterpret_cast<JEnumWrapper*>(out) = JEnumWrapper(env, v);
                return ConvertResponse::Successful;
            };
        }else if(internalMetaType!=QMetaType::UnknownType){
            return [internalMetaType](JNIEnv* env, QtJambiScope* scope, const jvalue& val, void* &out, jValueType valueType) -> ConvertResponse{
                QVariant value;
                switch(valueType){
                case jValueType::l:{
                    if(Java::Private::QtCore::QMetaType$GenericTypeInterface.isInstanceOf(env,val.l)){
                        int given = Java::Private::QtCore::QMetaType$GenericTypeInterface.metaType(env, val.l);
                        if(internalMetaType!=given){
                            JavaException::raiseIllegalArgumentException(env, qPrintable(QString("Wrong argument given: %1, expected: %2").arg(QMetaType::typeName(given)).arg(QMetaType::typeName(internalMetaType))) QTJAMBI_STACKTRACEINFO );
                        }
                    }
                    value = int_for_QtEnumerator_or_QFlags(env, val.l);
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
                    out = QMetaType::create(internalMetaType, value.data());
                    void* ptr = out;
                    scope->addFinalAction([internalMetaType, ptr](){
                        QMetaType::destroy(internalMetaType, ptr);
                    });
                }else{
                    if(!out)
                        return ConvertResponse::NotSuccessful;
                    QMetaType::destruct(internalMetaType, out);
                    if(QMetaType::construct(internalMetaType, out, value.data())!=out){
                        QByteArray ba;
                        bool saved;
                        /* write the copy to the stream */ {
                            QDataStream stream(&ba, QIODevice::WriteOnly);
                            saved = QMetaType::save(stream, internalMetaType, value.data());
                        }

                        /* read it back into the destination */
                        if(saved){
                            QDataStream stream(&ba, QIODevice::ReadOnly);
                            QMetaType::load(stream, internalMetaType, out);
                        }else{
                            return ConvertResponse::NotSuccessful;
                        }
                    }
                }
                return ConvertResponse::Successful;
            };
        }else if(Java::Private::QtJambi::QtEnumerator.isAssignableFrom(_env,externalClass)
                 || Java::Private::Runtime::Enum.isAssignableFrom(_env,externalClass)){
            return [](JNIEnv* env, QtJambiScope* scope, const jvalue& val, void* &out, jValueType valueType) -> ConvertResponse{
                switch(valueType){
                case jValueType::l:
                    if(scope && !out){
                        qint32* ptr;
                        out = ptr = new qint32;
                        scope->addFinalAction([ptr](){
                            delete ptr;
                        });
                    }
                    if(!out)
                        return ConvertResponse::NotSuccessful;
                    *reinterpret_cast<qint32*>(out) = int_for_QtEnumerator_or_QFlags(env, val.l).value<qint32>();
                    break;
                default:
                    if(scope && !out){
                        qint32* ptr;
                        out = ptr = new qint32;
                        scope->addFinalAction([ptr](){
                            delete ptr;
                        });
                    }
                    if(!out)
                        return ConvertResponse::NotSuccessful;
                    *reinterpret_cast<qint32*>(out) = val.i;
                    break;
                }
                return ConvertResponse::Successful;
            };
        }else if (Java::Private::QtJambi::QtLongEnumerator.isAssignableFrom(_env,externalClass)) {
            return [](JNIEnv* env, QtJambiScope* scope, const jvalue& val, void* &out, jValueType valueType) -> ConvertResponse{
                switch(valueType){
                case jValueType::l:
                    if(scope && !out){
                        qint64* ptr;
                        out = ptr = new qint64;
                        scope->addFinalAction([ptr](){
                            delete ptr;
                        });
                    }
                    if(!out)
                        return ConvertResponse::NotSuccessful;
                    *reinterpret_cast<qint64*>(out) = int_for_QtEnumerator_or_QFlags(env, val.l).value<qint64>();
                    break;
                default:
                    if(scope && !out){
                        qint64* ptr;
                        out = ptr = new qint64;
                        scope->addFinalAction([ptr](){
                            delete ptr;
                        });
                    }
                    if(!out)
                        return ConvertResponse::NotSuccessful;
                    *reinterpret_cast<qint64*>(out) = val.j;
                    break;
                }
                return ConvertResponse::Successful;
            };
        }else if (Java::Private::QtJambi::QtShortEnumerator.isAssignableFrom(_env,externalClass)) {
            return [](JNIEnv* env, QtJambiScope* scope, const jvalue& val, void* &out, jValueType valueType) -> ConvertResponse{
                switch(valueType){
                    case jValueType::l:
                    if(scope && !out){
                        qint16* ptr;
                        out = ptr = new qint16;
                        scope->addFinalAction([ptr](){
                            delete ptr;
                        });
                    }
                    if(!out)
                        return ConvertResponse::NotSuccessful;
                    *reinterpret_cast<qint16*>(out) = int_for_QtEnumerator_or_QFlags(env, val.l).value<qint16>();
                    break;
                default:
                    if(scope && !out){
                        qint16* ptr;
                        out = ptr = new qint16;
                        scope->addFinalAction([ptr](){
                            delete ptr;
                        });
                    }
                    if(!out)
                        return ConvertResponse::NotSuccessful;
                    *reinterpret_cast<qint16*>(out) = val.s;
                    break;
                }
                return ConvertResponse::Successful;
            };
        }else if (Java::Private::QtJambi::QtByteEnumerator.isAssignableFrom(_env,externalClass)) {
            return [](JNIEnv* env, QtJambiScope* scope, const jvalue& val, void* &out, jValueType valueType) -> ConvertResponse{
                switch(valueType){
                    case jValueType::l:
                    if(scope && !out){
                        qint8* ptr;
                        out = ptr = new qint8;
                        scope->addFinalAction([ptr](){
                            delete ptr;
                        });
                    }
                    if(!out)
                        return ConvertResponse::NotSuccessful;
                    *reinterpret_cast<qint8*>(out) = int_for_QtEnumerator_or_QFlags(env, val.l).value<qint8>();
                    break;
                default:
                    if(scope && !out){
                        qint16* ptr;
                        out = ptr = new qint16;
                        scope->addFinalAction([ptr](){
                            delete ptr;
                        });
                    }
                    if(!out)
                        return ConvertResponse::NotSuccessful;
                    *reinterpret_cast<qint16*>(out) = val.b;
                    break;
                }
                return ConvertResponse::Successful;
            };
        }
    }else if (internalMetaType==QMetaType::QStringList) {
        if (Java::Private::Runtime::Collection.isAssignableFrom(_env,externalClass)) {
            if (Java::Private::QtJambi::QtJambiListObject.isAssignableFrom(_env,externalClass)) {
                return [](JNIEnv* env, QtJambiScope* scope, const jvalue& val, void* &out, jValueType t) -> ConvertResponse{
                    jclass elementType = Java::Private::QtJambi::QtJambiCollectionObject.elementType(env,val.l);
                    if(t!=jValueType::l)
                        return ConvertResponse::NotSuccessful;
                    if(Java::Runtime::String.isSameClass(env,elementType)){
                        if(void* ptr = qtjambi_to_object(env, val.l)){
                            if(out)
                                *reinterpret_cast<QStringList*>(out) = *reinterpret_cast<QStringList*>(ptr);
                            else out = ptr;
                        }
                    }else{
                        if(scope && !out){
                            QStringList* ptr;
                            out = ptr = new QStringList;
                            scope->addFinalAction([ptr](){
                                delete ptr;
                            });
                        }
                        if(!out)
                            return ConvertResponse::NotSuccessful;
                        jobject iter = qtjambi_collection_iterator(env, val.l);
                        QStringList* list = reinterpret_cast<QStringList*>(out);
                        while(qtjambi_iterator_has_next(env, iter)){
                            jobject next = qtjambi_iterator_next(env, iter);
                            list->append(qtjambi_to_qstring(env, next));
                        }
                    }
                    return ConvertResponse::Successful;
                };
            }else{
                return [](JNIEnv* env, QtJambiScope* scope, const jvalue& val, void* &out, jValueType t) -> ConvertResponse{
                    if(scope && !out){
                        QStringList* ptr;
                        out = ptr = new QStringList;
                        scope->addFinalAction([ptr](){
                            delete ptr;
                        });
                    }
                    if(!out || t!=jValueType::l)
                        return ConvertResponse::NotSuccessful;
                    jobject iter = qtjambi_collection_iterator(env, val.l);
                    QStringList* list = reinterpret_cast<QStringList*>(out);
                    while(qtjambi_iterator_has_next(env, iter)){
                        jobject next = qtjambi_iterator_next(env, iter);
                        list->append(qtjambi_to_qstring(env, next));
                    }
                    return ConvertResponse::Successful;
                };
            }
        }
    }else if (internalMetaType==QMetaType::QByteArrayList) {
        if (Java::Private::Runtime::Collection.isAssignableFrom(_env,externalClass)) {
            if (Java::Private::QtJambi::QtJambiListObject.isAssignableFrom(_env,externalClass)) {
                return [](JNIEnv* env, QtJambiScope* scope, const jvalue& val, void* &out, jValueType t) -> ConvertResponse{
                    jclass elementType = Java::Private::QtJambi::QtJambiCollectionObject.elementType(env,val.l);
                    if(t!=jValueType::l)
                        return ConvertResponse::NotSuccessful;
                    if(Java::Private::QtCore::QByteArray.isSameClass(env,elementType)){
                        if(void* ptr = qtjambi_to_object(env, val.l)){
                            if(out)
                                *reinterpret_cast<QByteArrayList*>(out) = *reinterpret_cast<QByteArrayList*>(ptr);
                            else out = ptr;
                        }
                    }else{
                        if(scope && !out){
                            QByteArrayList* ptr;
                            out = ptr = new QByteArrayList;
                            scope->addFinalAction([ptr](){
                                delete ptr;
                            });
                        }
                        if(!out)
                            return ConvertResponse::NotSuccessful;
                        jobject iter = qtjambi_collection_iterator(env, val.l);
                        QByteArrayList* list = reinterpret_cast<QByteArrayList*>(out);
                        while(qtjambi_iterator_has_next(env, iter)){
                            jobject next = qtjambi_iterator_next(env, iter);
                            list->append(qtjambi_cast<QByteArray>(env, next));
                        }
                    }
                    return ConvertResponse::Successful;
                };
            }else{
                return [](JNIEnv* env, QtJambiScope* scope, const jvalue& val, void* &out, jValueType t) -> ConvertResponse{
                    if(scope && !out){
                        QStringList* ptr;
                        out = ptr = new QStringList;
                        scope->addFinalAction([ptr](){
                            delete ptr;
                        });
                    }
                    if(!out || t!=jValueType::l)
                        return ConvertResponse::NotSuccessful;
                    jobject iter = qtjambi_collection_iterator(env, val.l);
                    QByteArrayList* list = reinterpret_cast<QByteArrayList*>(out);
                    while(qtjambi_iterator_has_next(env, iter)){
                        jobject next = qtjambi_iterator_next(env, iter);
                        list->append(qtjambi_cast<QByteArray>(env, next));
                    }
                    return ConvertResponse::Successful;
                };
            }
        }
    }else if (internalMetaType==QMetaType::QVariantList) {
        if (Java::Private::Runtime::Collection.isAssignableFrom(_env,externalClass)) {
            if (Java::Private::QtJambi::QtJambiListObject.isAssignableFrom(_env,externalClass)) {
                return [](JNIEnv* env, QtJambiScope* scope, const jvalue& val, void* &out, jValueType t) -> ConvertResponse{
                    jclass elementType = Java::Private::QtJambi::QtJambiCollectionObject.elementType(env,val.l);
                    if(t!=jValueType::l)
                        return ConvertResponse::NotSuccessful;
                    if(Java::Private::Runtime::Object.isSameClass(env,elementType)){
                        if(void* ptr = qtjambi_to_object(env, val.l)){
                            if(out)
                                *reinterpret_cast<QList<QVariant>*>(out) = *reinterpret_cast<QList<QVariant>*>(ptr);
                            else out = ptr;
                        }
                    }else{
                        if(scope && !out){
                            QList<QVariant>* ptr;
                            out = ptr = new QList<QVariant>;
                            scope->addFinalAction([ptr](){
                                delete ptr;
                            });
                        }
                        if(!out)
                            return ConvertResponse::NotSuccessful;
                        jobject iter = qtjambi_collection_iterator(env, val.l);
                        QList<QVariant>* list = reinterpret_cast<QList<QVariant>*>(out);
                        while(qtjambi_iterator_has_next(env, iter)){
                            jobject next = qtjambi_iterator_next(env, iter);
                            list->append(qtjambi_to_qvariant(env, next));
                        }
                    }
                    return ConvertResponse::Successful;
                };
            }else{
                return [](JNIEnv* env, QtJambiScope* scope, const jvalue& val, void* &out, jValueType t) -> ConvertResponse{
                    if(scope && !out){
                        QList<QVariant>* ptr;
                        out = ptr = new QList<QVariant>;
                        scope->addFinalAction([ptr](){
                            delete ptr;
                        });
                    }
                    if(!out || t!=jValueType::l)
                        return ConvertResponse::NotSuccessful;
                    jobject iter = qtjambi_collection_iterator(env, val.l);
                    QList<QVariant>* list = reinterpret_cast<QList<QVariant>*>(out);
                    while(qtjambi_iterator_has_next(env, iter)){
                        jobject next = qtjambi_iterator_next(env, iter);
                        list->append(qtjambi_to_qvariant(env, next));
                    }
                    return ConvertResponse::Successful;
                };
            }
        }
    }else if (internalMetaType==QMetaType::QVariantMap) {
        if (Java::Private::Runtime::Map.isAssignableFrom(_env,externalClass)) {
            if (Java::Private::QtJambi::QtJambiMapObject.isAssignableFrom(_env,externalClass)) {
                return [](JNIEnv* env, QtJambiScope* scope, const jvalue& val, void* &out, jValueType t) -> ConvertResponse{
                    jclass keyType = Java::Private::QtJambi::QtJambiAbstractMapObject.keyType(env,val.l);
                    jclass valueType = Java::Private::QtJambi::QtJambiAbstractMapObject.valueType(env,val.l);
                    if(t!=jValueType::l)
                        return ConvertResponse::NotSuccessful;
                    if(Java::Runtime::String.isSameClass(env, keyType) && Java::Private::Runtime::Object.isSameClass(env,valueType)){
                        if(void* ptr = qtjambi_to_object(env, val.l)){
                            if(out)
                                *reinterpret_cast<QMap<QString,QVariant>*>(out) = *reinterpret_cast<QMap<QString,QVariant>*>(ptr);
                            else out = ptr;
                        }
                    }else{
                        if(scope && !out){
                            QMap<QString,QVariant>* ptr;
                            out = ptr = new QMap<QString,QVariant>;
                            scope->addFinalAction([ptr](){
                                delete ptr;
                            });
                        }
                        if(!out)
                            return ConvertResponse::NotSuccessful;
                        jobject iter = qtjambi_map_entryset_iterator(env, val.l);
                        QMap<QString,QVariant>* map = reinterpret_cast<QMap<QString,QVariant>*>(out);
                        while(qtjambi_iterator_has_next(env, iter)){
                            jobject next = qtjambi_iterator_next(env, iter);
                            jobject key = qtjambi_map$entry_key(env, next);
                            jobject value = qtjambi_map$entry_value(env, next);
                            map->insert(qtjambi_to_qstring(env, key), qtjambi_to_qvariant(env, value));
                        }
                    }
                    return ConvertResponse::Successful;
                };
            }else{
                return [](JNIEnv* env, QtJambiScope* scope, const jvalue& val, void* &out, jValueType t) -> ConvertResponse{
                    if(scope && !out){
                        QMap<QString,QVariant>* ptr;
                        out = ptr = new QMap<QString,QVariant>;
                        scope->addFinalAction([ptr](){
                            delete ptr;
                        });
                    }
                    if(!out || t!=jValueType::l)
                        return ConvertResponse::NotSuccessful;
                    jobject iter = qtjambi_map_entryset_iterator(env, val.l);
                    QMap<QString,QVariant>* map = reinterpret_cast<QMap<QString,QVariant>*>(out);
                    while(qtjambi_iterator_has_next(env, iter)){
                        jobject next = qtjambi_iterator_next(env, iter);
                        jobject key = qtjambi_map$entry_key(env, next);
                        jobject value = qtjambi_map$entry_value(env, next);
                        map->insert(qtjambi_to_qstring(env, key), qtjambi_to_qvariant(env, value));
                    }
                    return ConvertResponse::Successful;
                };
            }
        }
    }else if (internalMetaType==QMetaType::QVariantHash) {
        if (Java::Private::Runtime::Map.isAssignableFrom(_env,externalClass)) {
            if (Java::Private::QtJambi::QtJambiMapObject.isAssignableFrom(_env,externalClass)) {
                return [](JNIEnv* env, QtJambiScope* scope, const jvalue& val, void* &out, jValueType t) -> ConvertResponse{
                    jclass keyType = Java::Private::QtJambi::QtJambiAbstractMapObject.keyType(env,val.l);
                    jclass valueType = Java::Private::QtJambi::QtJambiAbstractMapObject.valueType(env,val.l);
                    if(t!=jValueType::l)
                        return ConvertResponse::NotSuccessful;
                    if(Java::Runtime::String.isSameClass(env, keyType) && Java::Private::Runtime::Object.isSameClass(env,valueType)){
                        if(void* ptr = qtjambi_to_object(env, val.l)){
                            if(out)
                                *reinterpret_cast<QMap<QString,QVariant>*>(out) = *reinterpret_cast<QMap<QString,QVariant>*>(ptr);
                            else out = ptr;
                        }
                    }else{
                        if(scope && !out){
                            QHash<QString,QVariant>* ptr;
                            out = ptr = new QHash<QString,QVariant>;
                            scope->addFinalAction([ptr](){
                                delete ptr;
                            });
                        }
                        if(!out)
                            return ConvertResponse::NotSuccessful;
                        jobject iter = qtjambi_map_entryset_iterator(env, val.l);
                        QHash<QString,QVariant>* map = reinterpret_cast<QHash<QString,QVariant>*>(out);
                        while(qtjambi_iterator_has_next(env, iter)){
                            jobject next = qtjambi_iterator_next(env, iter);
                            jobject key = qtjambi_map$entry_key(env, next);
                            jobject value = qtjambi_map$entry_value(env, next);
                            map->insert(qtjambi_to_qstring(env, key), qtjambi_to_qvariant(env, value));
                        }
                    }
                    return ConvertResponse::Successful;
                };
            }else{
                return [](JNIEnv* env, QtJambiScope* scope, const jvalue& val, void* &out, jValueType t) -> ConvertResponse{
                    if(scope && !out){
                        QHash<QString,QVariant>* ptr;
                        out = ptr = new QHash<QString,QVariant>;
                        scope->addFinalAction([ptr](){
                            delete ptr;
                        });
                    }
                    if(!out || t!=jValueType::l)
                        return ConvertResponse::NotSuccessful;
                    jobject iter = qtjambi_map_entryset_iterator(env, val.l);
                    QHash<QString,QVariant>* map = reinterpret_cast<QHash<QString,QVariant>*>(out);
                    while(qtjambi_iterator_has_next(env, iter)){
                        jobject next = qtjambi_iterator_next(env, iter);
                        jobject key = qtjambi_map$entry_key(env, next);
                        jobject value = qtjambi_map$entry_value(env, next);
                        map->insert(qtjambi_to_qstring(env, key), qtjambi_to_qvariant(env, value));
                    }
                    return ConvertResponse::Successful;
                };
            }
        }
    }else if (Java::Private::QtCore::QMetaEnum.isAssignableFrom(_env,externalClass) && (internalMetaType==QMetaType::UnknownType || internalMetaType==QMetaType::type("QMetaEnum"))) {
        return [](JNIEnv* env, QtJambiScope* scope, const jvalue& val, void* &out, jValueType) -> ConvertResponse{
            if(scope && !out){
                QMetaEnum* ptr;
                out = ptr = new QMetaEnum;
                scope->addFinalAction([ptr](){
                    delete ptr;
                });
            }
            if(!out)
                return ConvertResponse::NotSuccessful;
            *reinterpret_cast<QMetaEnum*>(out) = qtjambi_to_QMetaEnum(env, val.l);
            return ConvertResponse::Successful;
        };
    }else if (Java::Private::QtCore::QMetaMethod.isAssignableFrom(_env,externalClass) && (internalMetaType==QMetaType::UnknownType || internalMetaType==QMetaType::type("QMetaMethod"))) {
        return [](JNIEnv* env, QtJambiScope* scope, const jvalue& val, void* &out, jValueType) -> ConvertResponse{
            if(scope && !out){
                QMetaMethod* ptr;
                out = ptr = new QMetaMethod;
                scope->addFinalAction([ptr](){
                    delete ptr;
                });
            }
            if(!out)
                return ConvertResponse::NotSuccessful;
            *reinterpret_cast<QMetaMethod*>(out) = qtjambi_to_QMetaMethod(env, val.l);
            return ConvertResponse::Successful;
        };
    }else if (Java::Private::QtCore::QMetaObject.isAssignableFrom(_env,externalClass) && (internalMetaType==QMetaType::UnknownType || internalMetaType==QMetaType::type("QMetaObject*") || internalMetaType==QMetaType::type("const QMetaObject*"))) {
        return [](JNIEnv* env, QtJambiScope* scope, const jvalue& val, void* &out, jValueType) -> ConvertResponse{
            if(scope && !out){
                QMetaObject** ptr;
                out = ptr = new QMetaObject*;
                scope->addFinalAction([ptr](){
                    delete ptr;
                });
            }
            if(!out)
                return ConvertResponse::NotSuccessful;
            *reinterpret_cast<const QMetaObject**>(out) = qtjambi_to_QMetaObject(env, val.l);
            return ConvertResponse::Successful;
        };
    }else if (Java::Private::QtCore::QMetaObject$Connection.isAssignableFrom(_env,externalClass) && (internalMetaType==QMetaType::UnknownType || internalMetaType==QMetaType::type("QMetaObject::Connection"))) {
        return [](JNIEnv* env, QtJambiScope* scope, const jvalue& val, void* &out, jValueType) -> ConvertResponse{
            if(scope && !out){
                QMetaObject::Connection* ptr;
                out = ptr = new QMetaObject::Connection;
                scope->addFinalAction([ptr](){
                    delete ptr;
                });
            }
            if(!out)
                return ConvertResponse::NotSuccessful;
            if(qtjambi_convert_to_native(env, typeid(QMetaObject::Connection), val.l, out))
                return ConvertResponse::Successful;
            else return ConvertResponse::NotSuccessful;
        };
    }else if (Java::Private::QtCore::QMetaProperty.isAssignableFrom(_env,externalClass) && (internalMetaType==QMetaType::UnknownType || internalMetaType==QMetaType::type("QMetaProperty"))) {
        return [](JNIEnv* env, QtJambiScope* scope, const jvalue& val, void* &out, jValueType) -> ConvertResponse{
            if(scope && !out){
                QMetaProperty* ptr;
                out = ptr = new QMetaProperty;
                scope->addFinalAction([ptr](){
                    delete ptr;
                });
            }
            if(!out)
                return ConvertResponse::NotSuccessful;
            *reinterpret_cast<QMetaProperty*>(out) = qtjambi_to_QMetaProperty(env, val.l);
            return ConvertResponse::Successful;
        };
    }else if (Java::Private::QtCore::QModelIndex.isAssignableFrom(_env,externalClass) && (internalMetaType==QMetaType::UnknownType || internalMetaType==QMetaType::type("QModelIndex"))) {
        return [](JNIEnv* env, QtJambiScope* scope, const jvalue& val, void* &out, jValueType) -> ConvertResponse{
            if(scope && !out){
                QModelIndex* ptr;
                out = ptr = new QModelIndex;
                scope->addFinalAction([ptr](){
                    delete ptr;
                });
            }
            if(!out)
                return ConvertResponse::NotSuccessful;
            *reinterpret_cast<QModelIndex*>(out) = qtjambi_to_QModelIndex(env, val.l);
            return ConvertResponse::Successful;
        };
    }else if([&]() -> bool {
             bool result = Java::Private::QtJambi::QtJambiInternal.isGeneratedClass(_env, externalClass);
             if(!result){
                 result = !getSuperTypeInfos(_env, externalClass).isEmpty();
             }
             return result;
        }()){
        if(internalMetaType==QMetaType::UnknownType || QString(QMetaType::typeName(internalMetaType)).endsWith("*")){
            if(internalType & Reference){
                return [internalMetaType](JNIEnv* env, QtJambiScope*, const jvalue& val, void* &out, jValueType) -> ConvertResponse{
                    if(internalMetaType!=QMetaType::UnknownType && Java::Private::QtCore::QMetaType$GenericTypeInterface.isInstanceOf(env,val.l)){
                        int given = Java::Private::QtCore::QMetaType$GenericTypeInterface.metaType(env, val.l);
                        if(internalMetaType!=given){
                            JavaException::raiseIllegalArgumentException(env, qPrintable(QString("Wrong argument given: %1, expected: %2").arg(QMetaType::typeName(given)).arg(QMetaType::typeName(internalMetaType))) QTJAMBI_STACKTRACEINFO );
                        }
                    }
                    if(const QSharedPointer<QtJambiLink>& link = QtJambiLink::findLinkForJavaObject(env, val.l)){
                        out = link->pointer();
                        return ConvertResponse::Successful;
                    }else{
                        out = nullptr;
                        return ConvertResponse::JavaObjectNotLinked;
                    }
                };
            }else{
                return [internalMetaType](JNIEnv* env, QtJambiScope* scope, const jvalue& val, void* &out, jValueType) -> ConvertResponse{
                    if(internalMetaType!=QMetaType::UnknownType && Java::Private::QtCore::QMetaType$GenericTypeInterface.isInstanceOf(env,val.l)){
                        int given = Java::Private::QtCore::QMetaType$GenericTypeInterface.metaType(env, val.l);
                        if(internalMetaType!=given){
                            JavaException::raiseIllegalArgumentException(env, qPrintable(QString("Wrong argument given: %1, expected: %2").arg(QMetaType::typeName(given)).arg(QMetaType::typeName(internalMetaType))) QTJAMBI_STACKTRACEINFO );
                        }
                    }
                    if(scope && !out){
                        void** ptr;
                        out = ptr = new void*;
                        scope->addFinalAction([ptr](){
                            delete ptr;
                        });
                    }
                    if(!out)
                        return ConvertResponse::NotSuccessful;
                    if(const QSharedPointer<QtJambiLink>& link = QtJambiLink::findLinkForJavaObject(env, val.l)){
                        *reinterpret_cast<void**>(out) = link->pointer();
                        return ConvertResponse::Successful;
                    }else{
                        *reinterpret_cast<void**>(out) = nullptr;
                        return ConvertResponse::JavaObjectNotLinked;
                    }
                };
            }
        }else{
            return [internalMetaType](JNIEnv* env, QtJambiScope* scope, const jvalue& val, void* &out, jValueType) -> ConvertResponse{
                if(Java::Private::QtCore::QMetaType$GenericTypeInterface.isInstanceOf(env,val.l)){
                    int given = Java::Private::QtCore::QMetaType$GenericTypeInterface.metaType(env, val.l);
                    if(internalMetaType!=given){
                        JavaException::raiseIllegalArgumentException(env, qPrintable(QString("Wrong argument given: %1, expected: %2").arg(QMetaType::typeName(given)).arg(QMetaType::typeName(internalMetaType))) QTJAMBI_STACKTRACEINFO );
                    }
                }
                if(const QSharedPointer<QtJambiLink>& link = QtJambiLink::findLinkForJavaObject(env, val.l)){
                    if(scope && !out){
                        out = QMetaType::create(internalMetaType, link->pointer());
                        void* ptr = out;
                        scope->addFinalAction([internalMetaType,ptr](){
                            QMetaType::destroy(internalMetaType, ptr);
                        });
                    }else{
                        if(!out)
                            return ConvertResponse::NotSuccessful;
                        QMetaType::destruct(internalMetaType, out);
                        if(QMetaType::construct(internalMetaType, out, link->pointer())!=out){
                            QByteArray ba;
                            bool saved;
                            /* write the copy to the stream */ {
                                QDataStream stream(&ba, QIODevice::WriteOnly);
                                saved = QMetaType::save(stream, internalMetaType, link->pointer());
                            }

                            /* read it back into the destination */
                            if(saved){
                                QDataStream stream(&ba, QIODevice::ReadOnly);
                                QMetaType::load(stream, internalMetaType, out);
                            }else{
                                return ConvertResponse::NotSuccessful;
                            }
                        }
                    }
                    return ConvertResponse::Successful;
                }else{
                    if(scope && !out){
                        void** ptr;
                        out = ptr = new void*;
                        scope->addFinalAction([ptr](){
                            delete ptr;
                        });
                    }
                    *reinterpret_cast<void**>(out) = nullptr;
                    return ConvertResponse::JavaObjectNotLinked;
                }
            };
        }
    }else if(isArrayClass && ((internalType & QtJambiTypeManager::initializer_list) || internalTypeName.startsWith("initializer_list<"))){
        jclass componentClass = Java::Private::Runtime::Class.getComponentType(_env,externalClass);
        QString javaTypeName = qtjambi_class_name(_env, jclass(componentClass)).replace(".", "/");
        QString _internalTypeName = internalTypeName;
        if((internalType & QtJambiTypeManager::initializer_list)==0){
            _internalTypeName = internalTypeName.mid(22, internalTypeName.length() - 23);
        }
        if(const std::type_info* typeId = getTypeByQtName(_internalTypeName)){
            int metaType = QMetaType::type(qPrintable(_internalTypeName));
            if(metaType==QMetaType::UnknownType){
                metaType = registeredMetaTypeID(*typeId);
            }
            size_t size = getValueSize(*typeId);
            if(size>0){
                if(Java::Runtime::Byte.isPrimitiveType(_env,componentClass)){
                    return [](JNIEnv* env, QtJambiScope* scope, const jvalue& in, void* &out, jValueType) -> ConvertResponse {
                                if(scope){
                                    if(!out){
                                        std::initializer_list<jbyte>* _out = new std::initializer_list<jbyte>;
                                        out = _out;
                                        scope->addFinalAction([_out](){delete _out;});
                                    }
                                    jsize arrayLength = env->GetArrayLength(jarray(in.l));
                                    jbyte* data = new jbyte[size_t(arrayLength)];
                                    env->GetByteArrayRegion(jbyteArray(in.l), 0, arrayLength, data);
                                    scope->addFinalAction([data](){delete[] data;});
#ifdef Q_OS_WIN32
                                    *reinterpret_cast<std::initializer_list<jbyte>*>(out) = std::initializer_list<jbyte>(data, data + size_t(arrayLength));
#else
                                    struct jbyte_initializer_list_access{
                                        jbyte* _M_array;
                                        size_t _M_len;
                                    };
                                    reinterpret_cast<jbyte_initializer_list_access*>(out)->_M_array = data;
                                    reinterpret_cast<jbyte_initializer_list_access*>(out)->_M_len = size_t(arrayLength);
#endif
                                }else{
                                    return ConvertResponse::NotSuccessful;
                                }
                                return ConvertResponse::Successful;
                            };
                }else if(Java::Runtime::Short.isPrimitiveType(_env,componentClass)){
                    return [](JNIEnv* env, QtJambiScope* scope, const jvalue& in, void* &out, jValueType) -> ConvertResponse {
                                if(scope){
                                    if(!out){
                                        std::initializer_list<jshort>* _out = new std::initializer_list<jshort>;
                                        out = _out;
                                        scope->addFinalAction([_out](){delete _out;});
                                    }
                                    jsize arrayLength = env->GetArrayLength(jarray(in.l));
                                    jshort* data = new jshort[size_t(arrayLength)];
                                    env->GetShortArrayRegion(jshortArray(in.l), 0, arrayLength, data);
                                    scope->addFinalAction([data](){delete[] data;});
#ifdef Q_OS_WIN32
                                    *reinterpret_cast<std::initializer_list<jshort>*>(out) = std::initializer_list<jshort>(data, data + size_t(arrayLength));
#else
                                    struct jshort_initializer_list_access{
                                        jshort* _M_array;
                                        size_t _M_len;
                                    };
                                    reinterpret_cast<jshort_initializer_list_access*>(out)->_M_array = data;
                                    reinterpret_cast<jshort_initializer_list_access*>(out)->_M_len = size_t(arrayLength);
#endif
                                }else{
                                    return ConvertResponse::NotSuccessful;
                                }
                                return ConvertResponse::Successful;
                            };
                }else if(Java::Runtime::Integer.isPrimitiveType(_env,componentClass)){
                    return [](JNIEnv* env, QtJambiScope* scope, const jvalue& in, void* &out, jValueType) -> ConvertResponse {
                                if(scope){
                                    if(!out){
                                        std::initializer_list<jint>* _out = new std::initializer_list<jint>;
                                        out = _out;
                                        scope->addFinalAction([_out](){delete _out;});
                                    }
                                    jsize arrayLength = env->GetArrayLength(jarray(in.l));
                                    jint* data = new jint[size_t(arrayLength)];
                                    env->GetIntArrayRegion(jintArray(in.l), 0, arrayLength, data);
                                    scope->addFinalAction([data](){delete[] data;});
#ifdef Q_OS_WIN32
                                    *reinterpret_cast<std::initializer_list<jint>*>(out) = std::initializer_list<jint>(data, data + size_t(arrayLength));
#else
                                    struct jint_initializer_list_access{
                                        jint* _M_array;
                                        size_t _M_len;
                                    };
                                    reinterpret_cast<jint_initializer_list_access*>(out)->_M_array = data;
                                    reinterpret_cast<jint_initializer_list_access*>(out)->_M_len = size_t(arrayLength);
#endif
                                }else{
                                    return ConvertResponse::NotSuccessful;
                                }
                                return ConvertResponse::Successful;
                            };
                }else if(Java::Runtime::Long.isPrimitiveType(_env,componentClass)){
                    return [](JNIEnv* env, QtJambiScope* scope, const jvalue& in, void* &out, jValueType) -> ConvertResponse {
                                if(scope){
                                    if(!out){
                                        std::initializer_list<jlong>* _out = new std::initializer_list<jlong>;
                                        out = _out;
                                        scope->addFinalAction([_out](){delete _out;});
                                    }
                                    jsize arrayLength = env->GetArrayLength(jarray(in.l));
                                    jlong* data = new jlong[size_t(arrayLength)];
                                    env->GetLongArrayRegion(jlongArray(in.l), 0, arrayLength, data);
                                    scope->addFinalAction([data](){delete[] data;});
#ifdef Q_OS_WIN32
                                    *reinterpret_cast<std::initializer_list<jlong>*>(out) = std::initializer_list<jlong>(data, data + size_t(arrayLength));
#else
                                    struct jlong_initializer_list_access{
                                        jlong* _M_array;
                                        size_t _M_len;
                                    };
                                    reinterpret_cast<jlong_initializer_list_access*>(out)->_M_array = data;
                                    reinterpret_cast<jlong_initializer_list_access*>(out)->_M_len = size_t(arrayLength);
#endif
                                }else{
                                    return ConvertResponse::NotSuccessful;
                                }
                                return ConvertResponse::Successful;
                            };
                }else if(Java::Runtime::Float.isPrimitiveType(_env,componentClass)){
                    return [](JNIEnv* env, QtJambiScope* scope, const jvalue& in, void* &out, jValueType) -> ConvertResponse {
                                if(scope){
                                    if(!out){
                                        std::initializer_list<jfloat>* _out = new std::initializer_list<jfloat>;
                                        out = _out;
                                        scope->addFinalAction([_out](){delete _out;});
                                    }
                                    jsize arrayLength = env->GetArrayLength(jarray(in.l));
                                    jfloat* data = new jfloat[size_t(arrayLength)];
                                    env->GetFloatArrayRegion(jfloatArray(in.l), 0, arrayLength, data);
                                    scope->addFinalAction([data](){delete[] data;});
#ifdef Q_OS_WIN32
                                    *reinterpret_cast<std::initializer_list<jfloat>*>(out) = std::initializer_list<jfloat>(data, data + size_t(arrayLength));
#else
                                    struct jfloat_initializer_list_access{
                                        jfloat* _M_array;
                                        size_t _M_len;
                                    };
                                    reinterpret_cast<jfloat_initializer_list_access*>(out)->_M_array = data;
                                    reinterpret_cast<jfloat_initializer_list_access*>(out)->_M_len = size_t(arrayLength);
#endif
                                }else{
                                    return ConvertResponse::NotSuccessful;
                                }
                                return ConvertResponse::Successful;
                            };
                }else if(Java::Runtime::Double.isPrimitiveType(_env,componentClass)){
                    return [](JNIEnv* env, QtJambiScope* scope, const jvalue& in, void* &out, jValueType) -> ConvertResponse {
                                if(scope){
                                    if(!out){
                                        std::initializer_list<jdouble>* _out = new std::initializer_list<jdouble>;
                                        out = _out;
                                        scope->addFinalAction([_out](){delete _out;});
                                    }
                                    jsize arrayLength = env->GetArrayLength(jarray(in.l));
                                    jdouble* data = new jdouble[size_t(arrayLength)];
                                    env->GetDoubleArrayRegion(jdoubleArray(in.l), 0, arrayLength, data);
                                    scope->addFinalAction([data](){delete[] data;});
#ifdef Q_OS_WIN32
                                    *reinterpret_cast<std::initializer_list<jdouble>*>(out) = std::initializer_list<jdouble>(data, data + size_t(arrayLength));
#else
                                    struct jdouble_initializer_list_access{
                                        jdouble* _M_array;
                                        size_t _M_len;
                                    };
                                    reinterpret_cast<jdouble_initializer_list_access*>(out)->_M_array = data;
                                    reinterpret_cast<jdouble_initializer_list_access*>(out)->_M_len = size_t(arrayLength);
#endif
                                }else{
                                    return ConvertResponse::NotSuccessful;
                                }
                                return ConvertResponse::Successful;
                            };
                }else if(Java::Runtime::Character.isPrimitiveType(_env,componentClass)){
                    return [](JNIEnv* env, QtJambiScope* scope, const jvalue& in, void* &out, jValueType) -> ConvertResponse {
                                if(scope){
                                    if(!out){
                                        std::initializer_list<jchar>* _out = new std::initializer_list<jchar>;
                                        out = _out;
                                        scope->addFinalAction([_out](){delete _out;});
                                    }
                                    jsize arrayLength = env->GetArrayLength(jarray(in.l));
                                    jchar* data = new jchar[size_t(arrayLength)];
                                    env->GetCharArrayRegion(jcharArray(in.l), 0, arrayLength, data);
                                    scope->addFinalAction([data](){delete[] data;});
#ifdef Q_OS_WIN32
                                    *reinterpret_cast<std::initializer_list<jchar>*>(out) = std::initializer_list<jchar>(data, data + size_t(arrayLength));
#else
                                    struct jchar_initializer_list_access{
                                        jchar* _M_array;
                                        size_t _M_len;
                                    };
                                    reinterpret_cast<jchar_initializer_list_access*>(out)->_M_array = data;
                                    reinterpret_cast<jchar_initializer_list_access*>(out)->_M_len = size_t(arrayLength);
#endif
                                }else{
                                    return ConvertResponse::NotSuccessful;
                                }
                                return ConvertResponse::Successful;
                            };
                }else if(Java::Runtime::Boolean.isPrimitiveType(_env,componentClass)){
                    return [](JNIEnv* env, QtJambiScope* scope, const jvalue& in, void* &out, jValueType) -> ConvertResponse {
                                if(scope){
                                    if(!out){
                                        std::initializer_list<jboolean>* _out = new std::initializer_list<jboolean>;
                                        out = _out;
                                        scope->addFinalAction([_out](){delete _out;});
                                    }
                                    jsize arrayLength = env->GetArrayLength(jarray(in.l));
                                    bool* data = new bool[size_t(arrayLength)];
                                    jboolean* _data = new jboolean[size_t(arrayLength)];
                                    env->GetBooleanArrayRegion(jbooleanArray(in.l), 0, arrayLength, _data);
                                    for (jsize i=0; i<arrayLength; ++i) {
                                        data[i] = _data[i];
                                    }
                                    delete[] _data;
                                    scope->addFinalAction([data](){delete[] data;});
#ifdef Q_OS_WIN32
                                    *reinterpret_cast<std::initializer_list<bool>*>(out) = std::initializer_list<bool>(data, data + size_t(arrayLength));
#else
                                    struct bool_initializer_list_access{
                                        bool* _M_array;
                                        size_t _M_len;
                                    };
                                    reinterpret_cast<bool_initializer_list_access*>(out)->_M_array = data;
                                    reinterpret_cast<bool_initializer_list_access*>(out)->_M_len = size_t(arrayLength);
#endif
                                }else{
                                    return ConvertResponse::NotSuccessful;
                                }
                                return ConvertResponse::Successful;
                            };
                }else{
                    QtJambiTypeManager::TypePattern internalTypePattern = QtJambiTypeManager::typeIdOfInternal(_env, javaTypeName, _internalTypeName, metaType);
                    const ExternalToInternalConverter& externalToInternalComponentConverter = QtJambiTypeManager::getExternalToInternalConverter(
                                                                    _env,
                                                                    componentClass,
                                                                    internalTypePattern,
                                                                    _internalTypeName,
                                                                    metaType);
                    return [size,externalToInternalComponentConverter](JNIEnv* env, QtJambiScope* scope, const jvalue& in, void* &out, jValueType) -> ConvertResponse {
                                if(scope){
                                    if(!out){
                                        std::initializer_list<char>* _out = new std::initializer_list<char>;
                                        out = _out;
                                        scope->addFinalAction([_out](){delete _out;});
                                    }
                                    jsize arrayLength = env->GetArrayLength(jarray(in.l));
                                    char* data = new char[size * size_t(arrayLength)];
                                    for (jsize i=0; i<arrayLength; ++i) {
                                        void* ptr = &data[size * size_t(i)];
                                        jvalue val;
                                        val.l = env->GetObjectArrayElement(jobjectArray(in.l), i);
                                        externalToInternalComponentConverter(env, scope, val, ptr, jValueType::l);
                                    }
                                    scope->addFinalAction([data](){delete[] data;});
#ifdef Q_OS_WIN32
                                    *reinterpret_cast<std::initializer_list<char>*>(out) = std::initializer_list<char>(data, data + size * size_t(arrayLength));
#else
                                    struct initializer_list_access{
                                        char* _M_array;
                                        size_t _M_len;
                                    };
                                    reinterpret_cast<initializer_list_access*>(out)->_M_array = data;
                                    reinterpret_cast<initializer_list_access*>(out)->_M_len = size * size_t(arrayLength);
#endif
                                }else{
                                    return ConvertResponse::NotSuccessful;
                                }
                                return ConvertResponse::Successful;
                            };
                }
            }
        }
    }else{
        if(const QMetaObject* metaObject = QMetaType::metaObjectForType(internalMetaType)){
            if(metaObject->inherits(&QObject::staticMetaObject)){
                return [metaObject,internalMetaType](JNIEnv* env, QtJambiScope* scope, const jvalue& val, void* &out, jValueType) -> ConvertResponse{
                    if(internalMetaType!=QMetaType::UnknownType && Java::Private::QtCore::QMetaType$GenericTypeInterface.isInstanceOf(env,val.l)){
                        int given = Java::Private::QtCore::QMetaType$GenericTypeInterface.metaType(env, val.l);
                        if(internalMetaType!=given){
                            JavaException::raiseIllegalArgumentException(env, qPrintable(QString("Wrong argument given: %1, expected: %2").arg(QMetaType::typeName(given)).arg(QMetaType::typeName(internalMetaType))) QTJAMBI_STACKTRACEINFO );
                        }
                    }
                    QObject* object = qtjambi_to_qobject(env, val.l);
                    object = metaObject->cast(object);
                    if(scope && !out){
                        QObject** ptr;
                        out = ptr = new QObject*;
                        scope->addFinalAction([ptr](){
                            delete ptr;
                        });
                    }
                    if(!out)
                        return ConvertResponse::NotSuccessful;
                    *reinterpret_cast<QObject**>(out) = object;
                    return ConvertResponse::Successful;
                };
            }
        }
        if((internalTypeName.startsWith("QPair<") || internalTypeName.startsWith("std::pair<"))
                && internalTypeName.endsWith(">")
                && (Java::Private::QtCore::QPair.isAssignableFrom(_env,externalClass)
                    || Java::Private::Runtime::Object.isSameClass(_env,externalClass)
                    )){
            int idx = internalTypeName.indexOf("<");
            QString container = internalTypeName.mid(0, idx);
            QStringList instantiations = internalTypeName.mid(idx+1).chopped(1).trimmed().split(",");
            reduceMapInstantiations(instantiations);
            if(instantiations.size()==2){
                instantiations[0] = instantiations[0].trimmed();
                instantiations[1] = instantiations[1].trimmed();
                const std::type_info* t1;
                bool isPointer1 = instantiations[0].endsWith("*");
                bool isPointer2 = instantiations[1].endsWith("*");
                if(isPointer1){
                    t1 = getTypeByQtName(instantiations[0].chopped(1));
                }else{
                    t1 = getTypeByQtName(instantiations[0]);
                }
                const std::type_info* t2;
                if(isPointer2){
                    t2 = getTypeByQtName(instantiations[1].chopped(1));
                }else{
                    t2 = getTypeByQtName(instantiations[1]);
                }
                QByteArray class1;
                QByteArray class2;
                size_t size1 = 0;
                size_t size2 = 0;
                size_t align1 = 0;
                size_t align2 = 0;
                int metaType1 = QMetaType::UnknownType;
                int metaType2 = QMetaType::UnknownType;
                bool isStaticType1 = true;
                bool isStaticType2 = true;
                if(t1){
                    class1 = getJavaName(*t1);
                    if(isPointer1){
                        size1 = sizeof(void*);
                        metaType1 = QMetaType::type(qPrintable(instantiations[0]));
                    }else{
                        size1 = getValueSize(*t1);
                        align1 = getValueAlignment(*t1);
                        metaType1 = registeredMetaTypeID(*t1);
                        if(const QtJambiTypeInfo* typeInfo = getQTypeInfo(*t1)){
                            isStaticType1 = typeInfo->isStatic;
                        }
                    }
                }else{
                    metaType1 = QMetaType::type(qPrintable(instantiations[0]));
                    isStaticType1 = QtJambiTypeManager::isStaticType(instantiations[0]);
                    if(metaType1 != QMetaType::UnknownType){
                        int sz = QMetaType::sizeOf(metaType1);
                        size1 = sz<0 ? 0 : size_t(sz);
                    }
                    if(size1==0){
                        size1 = getInternalSize(instantiations[0]);
                    }
                    align1 = getInternalAlignment(instantiations[0]);
                    if(metaType1==QMetaType::UnknownType){
                        class1 = getExternalTypeName(_env, instantiations[0], ArgumentType, metaType1).toUtf8();
                    }else{
                        class1 = getExternalTypeName(_env, QMetaType::typeName(metaType1), ArgumentType, metaType1).toUtf8();
                    }
                }
                if(t2){
                    class2 = getJavaName(*t2);
                    if(isPointer2){
                        size2 = sizeof(void*);
                        metaType2 = QMetaType::type(qPrintable(instantiations[1]));
                    }else{
                        size2 = getValueSize(*t2);
                        align2 = getValueAlignment(*t2);
                        metaType2 = registeredMetaTypeID(*t2);
                        if(const QtJambiTypeInfo* typeInfo = getQTypeInfo(*t2)){
                            isStaticType2 = typeInfo->isStatic;
                        }
                    }
                }else{
                    metaType2 = QMetaType::type(qPrintable(instantiations[1]));
                    isStaticType2 = QtJambiTypeManager::isStaticType(instantiations[1]);
                    if(metaType2 != QMetaType::UnknownType){
                        int sz = QMetaType::sizeOf(metaType2);
                        size2 = sz<0 ? 0 : size_t(sz);
                    }
                    if(size2==0){
                        size2 = getInternalSize(instantiations[1]);
                    }
                    align2 = getInternalAlignment(instantiations[1]);
                    if(metaType2==QMetaType::UnknownType){
                        class2 = getExternalTypeName(_env, instantiations[1], ArgumentType, metaType2).toUtf8();
                    }else{
                        class2 = getExternalTypeName(_env, QMetaType::typeName(metaType2), ArgumentType, metaType2).toUtf8();
                    }
                }
                if(!class1.isEmpty() && !class2.isEmpty() && size1>0 && size2>0){
                    jclass javaClass1 = resolveClass(_env, class1);
                    jclass javaClass2 = resolveClass(_env, class2);
                    QtJambiTypeManager::TypePattern internalTypePattern1 = typeIdOfInternal(_env, QString::fromUtf8(class1), instantiations[0], metaType1, nullptr);
                    QtJambiTypeManager::TypePattern internalTypePattern2 = typeIdOfInternal(_env, QString::fromUtf8(class2), instantiations[1], metaType2, nullptr);
                    const ExternalToInternalConverter& externalToInternalConverter1 = getExternalToInternalConverter(
                                                                    _env,
                                                                    javaClass1,
                                                                    internalTypePattern1,
                                                                    instantiations[0],
                                                                    metaType1);
                    if(metaType1 == QMetaType::UnknownType){
                        // it is possible that getExternalToInternalConverter has registered the meta type
                        metaType1 = QMetaType::type(qPrintable(instantiations[0]));
                    }
                    const ExternalToInternalConverter& externalToInternalConverter2 = getExternalToInternalConverter(
                                                                    _env,
                                                                    javaClass2,
                                                                    internalTypePattern2,
                                                                    instantiations[1],
                                                                    metaType2);
                    if(metaType2 == QMetaType::UnknownType){
                        // it is possible that getExternalToInternalConverter has registered the meta type
                        metaType2 = QMetaType::type(qPrintable(instantiations[1]));
                    }
                    if(container=="QPair"){
                        return ContainerConverter::getExternalToInternalConverter_QPair(internalTypeName, externalToInternalConverter1, isPointer1, isStaticType1, align1, size1, metaType1, externalToInternalConverter2, isPointer2, isStaticType2, align2, size2, metaType2);
                    }else if(container=="std::pair"){
                        return ContainerConverter::getExternalToInternalConverter_std_pair(internalTypeName, externalToInternalConverter1, isPointer1, isStaticType1, align1, size1, metaType1, externalToInternalConverter2, isPointer2, isStaticType2, align2, size2, metaType2);
                    }
                }
            }
            return [](JNIEnv*, QtJambiScope*, const jvalue&, void* &, jValueType) -> ConvertResponse{
                return ConvertResponse::NotSuccessful;
            };
        }
        if( (internalTypeName.startsWith("QVector<")
             || internalTypeName.startsWith("QStack<")
             || internalTypeName.startsWith("QList<")
             || internalTypeName.startsWith("QQueue<")
             || internalTypeName.startsWith("QSet<")
             || internalTypeName.startsWith("QLinkedList<"))
                && internalTypeName.endsWith(">")
                && (Java::Private::Runtime::Collection.isAssignableFrom(_env,externalClass) || Java::Private::Runtime::Object.isSameClass(_env,externalClass)) ){
            int idx = internalTypeName.indexOf("<");
            QString container = internalTypeName.mid(0, idx);
            QString instantiation = internalTypeName.mid(idx+1).chopped(1).trimmed();
            const std::type_info* t;
            bool isPointer = instantiation.endsWith("*");
            if(isPointer){
                t = getTypeByQtName(instantiation.chopped(1));
            }else{
                t = getTypeByQtName(instantiation);
            }
            QByteArray clazz;
            size_t size = 0;
            size_t align = 0;
            bool isStaticType = true;
            int memberMetaType = QMetaType::UnknownType;
            if(t){
                clazz = getJavaName(*t);
                if(isPointer){
                    size = sizeof(void*);
                    memberMetaType = QMetaType::type(qPrintable(instantiation));
                }else{
                    size = getValueSize(*t);
                    align = getValueAlignment(*t);
                    memberMetaType = registeredMetaTypeID(*t);
                    if(const QtJambiTypeInfo* typeInfo = getQTypeInfo(*t)){
                        isStaticType = typeInfo->isStatic;
                    }
                }
            }else{
                memberMetaType = QMetaType::type(qPrintable(instantiation));
                isStaticType = QtJambiTypeManager::isStaticType(instantiation);
                if(memberMetaType != QMetaType::UnknownType){
                    int sz = QMetaType::sizeOf(memberMetaType);
                    size = sz<0 ? 0 : size_t(sz);
                }
                if(size==0){
                    size = getInternalSize(instantiation);
                }
                align = getInternalAlignment(instantiation);
                if(memberMetaType == QMetaType::UnknownType){
                    clazz = getExternalTypeName(_env, qPrintable(instantiation), ArgumentType, memberMetaType).toUtf8();
                }else{
                    clazz = getExternalTypeName(_env, QMetaType::typeName(memberMetaType), ArgumentType, memberMetaType).toUtf8();
                }
            }
            if(clazz.isEmpty() && memberMetaType!=QMetaType::UnknownType && memberMetaType!=QMetaType::Void && memberMetaType!=QMetaType::Nullptr){
                if(isPointer){
                    clazz = "io/qt/QNativePointer";
                    if(QMetaType(memberMetaType).flags() & QMetaType::PointerToQObject
                            || QMetaType(memberMetaType).flags() & QMetaType::SharedPointerToQObject
                            || QMetaType(memberMetaType).flags() & QMetaType::WeakPointerToQObject){
                         clazz = "io/qt/core/QObject";
                    }else if(QMetaType(memberMetaType).flags() & QMetaType::PointerToGadget){
                         clazz = "io/qt/core/QMetaType$GenericGadget";
                    }
                }else{
                    if(QMetaType(memberMetaType).flags() & QMetaType::IsGadget){
                         clazz = "io/qt/core/QMetaType$GenericGadget";
                    }else{
                         clazz = "io/qt/core/QMetaType$GenericValue";
                    }
                }
            }
            if(!clazz.isEmpty() && size>0){
                jclass javaClass = resolveClass(_env, clazz);
                QtJambiTypeManager::TypePattern internalTypePattern = typeIdOfInternal(_env, QString::fromUtf8(clazz), instantiation, memberMetaType, nullptr);
                const ExternalToInternalConverter& memberConverter = QtJambiTypeManager::getExternalToInternalConverter(
                                                                _env,
                                                                javaClass,
                                                                internalTypePattern,
                                                                instantiation,
                                                                memberMetaType);
                if(memberMetaType == QMetaType::UnknownType){
                    // it is possible that getExternalToInternalConverter has registered the meta type
                    memberMetaType = QMetaType::type(qPrintable(instantiation));
                }
                if(container=="QVector"){
                    return ContainerConverter::getExternalToInternalConverter_QVector(internalTypeName, memberConverter, isPointer, isStaticType, align, size, memberMetaType);
                }else if(container=="QStack"){
                    return ContainerConverter::getExternalToInternalConverter_QStack(internalTypeName, memberConverter, isPointer, isStaticType, align, size, memberMetaType);
                }else if(container=="QList"){
                    return ContainerConverter::getExternalToInternalConverter_QList(internalTypeName, memberConverter, isPointer, isStaticType, align, size, memberMetaType);
                }else if(container=="QQueue"){
                    return ContainerConverter::getExternalToInternalConverter_QQueue(internalTypeName, memberConverter, isPointer, isStaticType, align, size, memberMetaType);
                }else if(container=="QLinkedList"){
                    return ContainerConverter::getExternalToInternalConverter_QLinkedList(internalTypeName, memberConverter, isPointer, isStaticType, align, size, memberMetaType);
                }else if(container=="QSet"){
                    return ContainerConverter::getExternalToInternalConverter_QSet(internalTypeName, memberConverter, isPointer, isStaticType, align, size, memberMetaType);
                }
            }
        }else if( (internalTypeName.startsWith("QMap<")
                   || internalTypeName.startsWith("QMultiMap<")
                   || internalTypeName.startsWith("QHash<")
                   || internalTypeName.startsWith("QMultiHash<"))
                      && internalTypeName.endsWith(">")
                      && (Java::Private::Runtime::Map.isAssignableFrom(_env,externalClass) || Java::Private::Runtime::Object.isSameClass(_env,externalClass)) ){
              int idx = internalTypeName.indexOf("<");
              QString container = internalTypeName.mid(0, idx);
              QStringList instantiations = internalTypeName.mid(idx+1).chopped(1).trimmed().split(",");
              reduceMapInstantiations(instantiations);
              if(instantiations.size()==2){
                  const std::type_info* t1;
                  bool isPointer1 = instantiations[0].endsWith("*");
                  if(isPointer1){
                      t1 = getTypeByQtName(instantiations[0].chopped(1));
                  }else{
                      t1 = getTypeByQtName(instantiations[0]);
                      if(!t1){

                      }
                  }
                  const std::type_info* t2;
                  bool isPointer2 = instantiations[1].endsWith("*");
                  if(isPointer2){
                      t2 = getTypeByQtName(instantiations[1].chopped(1));
                  }else{
                      t2 = getTypeByQtName(instantiations[1]);
                  }
                  QByteArray clazz1;
                  QByteArray clazz2;
                  size_t size1 = 0;
                  size_t size2 = 0;
                  size_t align1 = 0;
                  size_t align2 = 0;
                  bool isStaticType1 = true;
                  bool isStaticType2 = true;
                  int memberMetaType1 = QMetaType::UnknownType;
                  int memberMetaType2 = QMetaType::UnknownType;
                  if(t1){
                      clazz1 = getJavaName(*t1);
                      if(isPointer1){
                          size1 = sizeof(void*);
                          memberMetaType1 = QMetaType::type(qPrintable(instantiations[0]));
                      }else{
                          size1 = getValueSize(*t1);
                          align1 = getValueAlignment(*t1);
                          memberMetaType1 = registeredMetaTypeID(*t1);
                          if(const QtJambiTypeInfo* typeInfo = getQTypeInfo(*t1)){
                              isStaticType1 = typeInfo->isStatic;
                          }
                      }
                  }else{
                      memberMetaType1 = QMetaType::type(qPrintable(instantiations[0]));
                      isStaticType1 = isStaticType(instantiations[0]);
                      if(memberMetaType1 != QMetaType::UnknownType){
                          int sz = QMetaType::sizeOf(memberMetaType1);
                          size1 = sz<0 ? 0 : size_t(sz);
                      }
                      if(size1==0){
                          size1 = getInternalSize(instantiations[0]);
                      }
                      align1 = getInternalAlignment(instantiations[0]);
                      if(memberMetaType1 == QMetaType::UnknownType){
                          clazz1 = getExternalTypeName(_env, qPrintable(instantiations[0]), ArgumentType, memberMetaType1).toUtf8();
                      }else{
                          clazz1 = getExternalTypeName(_env, QMetaType::typeName(memberMetaType1), ArgumentType, memberMetaType1).toUtf8();
                      }
                  }
                  if(t2){
                      clazz2 = getJavaName(*t2);
                      if(isPointer2){
                          size2 = sizeof(void*);
                          memberMetaType2 = QMetaType::type(qPrintable(instantiations[1]));
                      }else{
                          size2 = getValueSize(*t2);
                          align2 = getValueAlignment(*t2);
                          memberMetaType2 = registeredMetaTypeID(*t2);
                          if(const QtJambiTypeInfo* typeInfo = getQTypeInfo(*t2)){
                              isStaticType2 = typeInfo->isStatic;
                          }
                      }
                  }else{
                      memberMetaType2 = QMetaType::type(qPrintable(instantiations[1]));
                      isStaticType2 = isStaticType(instantiations[1]);
                      if(memberMetaType2 != QMetaType::UnknownType){
                          int sz = QMetaType::sizeOf(memberMetaType2);
                          size2 = sz<0 ? 0 : size_t(sz);
                      }
                      if(size2==0){
                          size2 = getInternalSize(instantiations[1]);
                      }
                      align2 = getInternalAlignment(instantiations[1]);
                      if(memberMetaType2 == QMetaType::UnknownType){
                          clazz2 = getExternalTypeName(_env, qPrintable(instantiations[1]), ArgumentType, memberMetaType2).toUtf8();
                      }else{
                          clazz2 = getExternalTypeName(_env, QMetaType::typeName(memberMetaType2), ArgumentType, memberMetaType2).toUtf8();
                      }
                  }
                  if(clazz1.isEmpty() && memberMetaType1!=QMetaType::UnknownType && memberMetaType1!=QMetaType::Void && memberMetaType1!=QMetaType::Nullptr){
                      if(isPointer1){
                          clazz1 = "io/qt/QNativePointer";
                          if(QMetaType(memberMetaType1).flags() & QMetaType::PointerToQObject
                                  || QMetaType(memberMetaType1).flags() & QMetaType::SharedPointerToQObject
                                  || QMetaType(memberMetaType1).flags() & QMetaType::WeakPointerToQObject){
                               clazz1 = "io/qt/core/QObject";
                          }else if(QMetaType(memberMetaType1).flags() & QMetaType::PointerToGadget){
                               clazz1 = "io/qt/core/QMetaType$GenericGadget";
                          }
                      }else{
                          if(QMetaType(memberMetaType1).flags() & QMetaType::IsGadget){
                               clazz1 = "io/qt/core/QMetaType$GenericGadget";
                          }else{
                               clazz1 = "io/qt/core/QMetaType$GenericValue";
                          }
                      }
                  }
                  if(clazz2.isEmpty() && memberMetaType2!=QMetaType::UnknownType && memberMetaType2!=QMetaType::Void && memberMetaType2!=QMetaType::Nullptr){
                      if(isPointer2){
                          clazz2 = "io/qt/QNativePointer";
                          if(QMetaType(memberMetaType2).flags() & QMetaType::PointerToQObject
                                  || QMetaType(memberMetaType2).flags() & QMetaType::SharedPointerToQObject
                                  || QMetaType(memberMetaType2).flags() & QMetaType::WeakPointerToQObject){
                              clazz2 = "io/qt/core/QObject";
                          }else if(QMetaType(memberMetaType2).flags() & QMetaType::PointerToGadget){
                               clazz2 = "io/qt/core/QMetaType$GenericGadget";
                          }
                      }else{
                          if(QMetaType(memberMetaType2).flags() & QMetaType::IsGadget){
                               clazz2 = "io/qt/core/QMetaType$GenericGadget";
                          }else{
                              clazz2 = "io/qt/core/QMetaType$GenericValue";
                          }
                      }
                  }
                  if(!clazz1.isEmpty() && size1>0 && !clazz2.isEmpty() && size2>0){
                      jclass javaClass1 = resolveClass(_env, clazz1);
                      jclass javaClass2 = resolveClass(_env, clazz2);
                      QtJambiTypeManager::TypePattern internalTypePattern1 = typeIdOfInternal(_env, QString::fromUtf8(clazz1), instantiations[0], memberMetaType1, nullptr);
                      QtJambiTypeManager::TypePattern internalTypePattern2 = typeIdOfInternal(_env, QString::fromUtf8(clazz2), instantiations[1], memberMetaType2, nullptr);
                      const ExternalToInternalConverter& memberConverter1 = QtJambiTypeManager::getExternalToInternalConverter(
                                                                      _env,
                                                                      javaClass1,
                                                                      internalTypePattern1,
                                                                      instantiations[0],
                                                                      memberMetaType1);
                      if(memberMetaType1 == QMetaType::UnknownType){
                          // it is possible that getExternalToInternalConverter has registered the meta type
                          memberMetaType1 = QMetaType::type(qPrintable(instantiations[0]));
                      }
                      const ExternalToInternalConverter& memberConverter2 = QtJambiTypeManager::getExternalToInternalConverter(
                                                                      _env,
                                                                      javaClass2,
                                                                      internalTypePattern2,
                                                                      instantiations[1],
                                                                      memberMetaType2);
                      if(memberMetaType2 == QMetaType::UnknownType){
                          // it is possible that getExternalToInternalConverter has registered the meta type
                          memberMetaType2 = QMetaType::type(qPrintable(instantiations[1]));
                      }
                      if(container=="QMap"){
                          return ContainerConverter::getExternalToInternalConverter_QMap(internalTypeName, memberConverter1, isPointer1, isStaticType1, align1, size1, memberMetaType1, memberConverter2, isPointer2, isStaticType2, align2, size2, memberMetaType2);
                      }else if(container=="QMultiMap"){
                          return ContainerConverter::getExternalToInternalConverter_QMultiMap(internalTypeName, memberConverter1, isPointer1, isStaticType1, align1, size1, memberMetaType1, memberConverter2, isPointer2, isStaticType2, align2, size2, memberMetaType2);
                      }else if(container=="QHash"){
                          return ContainerConverter::getExternalToInternalConverter_QHash(internalTypeName, memberConverter1, isPointer1, isStaticType1, align1, size1, memberMetaType1, memberConverter2, isPointer2, isStaticType2, align2, size2, memberMetaType2);
                      }else if(container=="QMultiHash"){
                          return ContainerConverter::getExternalToInternalConverter_QMultiHash(internalTypeName, memberConverter1, isPointer1, isStaticType1, align1, size1, memberMetaType1, memberConverter2, isPointer2, isStaticType2, align2, size2, memberMetaType2);
                      }
                  }
              }
          }
    }

    qWarning("Cannot convert from type '%s' to '%s'.",
             qPrintable(qtjambi_class_name(_env, externalClass).replace("$", ".")), qPrintable(internalTypeName));
    return ParameterTypeInfo::default_externalToInternalConverter();
}

bool QtJambiTypeManager::isStaticType(const QString &internalTypeName){
    if(internalTypeName.endsWith("*")){
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
    }else if(internalTypeName.startsWith("QPair<")){
        int idx = internalTypeName.indexOf("<");
        QString container = internalTypeName.mid(0, idx);
        QStringList instantiations = internalTypeName.mid(idx+1).chopped(1).trimmed().split(",");
        reduceMapInstantiations(instantiations);
        if(instantiations.size()==2){
            const std::type_info* t1;
            bool isPointer1 = instantiations[0].endsWith("*");
            if(isPointer1){
                t1 = getTypeByQtName(instantiations[0].chopped(1));
            }else{
                t1 = getTypeByQtName(instantiations[0]);
                if(!t1){

                }
            }
            const std::type_info* t2;
            bool isPointer2 = instantiations[1].endsWith("*");
            if(isPointer2){
                t2 = getTypeByQtName(instantiations[1].chopped(1));
            }else{
                t2 = getTypeByQtName(instantiations[1]);
            }
            bool isStaticType1 = true;
            bool isStaticType2 = true;
            if(t1){
                if(isPointer1){
                    isStaticType1 = false;
                }else{
                    if(const QtJambiTypeInfo* typeInfo = getQTypeInfo(*t1)){
                        isStaticType1 = typeInfo->isStatic;
                    }
                }
            }else{
                isStaticType1 = isStaticType(instantiations[0]);
            }
            if(t2){
                if(isPointer2){
                    isStaticType2 = false;
                }else{
                    if(const QtJambiTypeInfo* typeInfo = getQTypeInfo(*t2)){
                        isStaticType2 = typeInfo->isStatic;
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

void QtJambiTypeManager::registerInternalSize(const QString &internalTypeName, size_t size)
{
    if(!gSizeByTypeHash->contains(internalTypeName))
        gSizeByTypeHash->insert(internalTypeName, size);
}

void QtJambiTypeManager::registerInternalAlignment(const QString &internalTypeName, size_t align)
{
    if(!gAlignmentByTypeHash->contains(internalTypeName))
        gAlignmentByTypeHash->insert(internalTypeName, align);
}

size_t QtJambiTypeManager::getInternalAlignment(const QString &internalTypeName)
{
    if(internalTypeName.endsWith("*")){
        return Q_ALIGNOF(void*);
    }
    size_t align = gAlignmentByTypeHash->value(internalTypeName, 0);
    if(align>0)
        return align;
    if( (internalTypeName.startsWith("QVector<")
                || internalTypeName.startsWith("QStack<")
                || internalTypeName.startsWith("QList<")
                || internalTypeName.startsWith("QQueue<")
                || internalTypeName.startsWith("QSet<")
                || internalTypeName.startsWith("QMap<")
                || internalTypeName.startsWith("QHash<")
                || internalTypeName.startsWith("QMultiMap<")
                || internalTypeName.startsWith("QMultiHash<")
                || internalTypeName.startsWith("QLinkedList<")) && internalTypeName.endsWith(">") ){
        return 8;
    }
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
    return align;
}

size_t QtJambiTypeManager::getInternalSize(const QString &internalTypeName){
    if(internalTypeName.endsWith("*")){
        return sizeof(void*);
    }
    if(gSizeByTypeHash->contains(internalTypeName))
        return gSizeByTypeHash->value(internalTypeName, 0);
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
            if(instantiations[0].endsWith("*")){
                t1 = getTypeByQtName(instantiations[0].chopped(1));
            }else{
                t1 = getTypeByQtName(instantiations[0]);
            }
            const std::type_info* t2;
            if(instantiations[1].endsWith("*")){
                t2 = getTypeByQtName(instantiations[1].chopped(1));
            }else{
                t2 = getTypeByQtName(instantiations[1]);
            }
            size_t size1 = 0;
            size_t size2 = 0;
            if(t1){
                size1 = instantiations[0].endsWith("*") ? sizeof(void*) : getValueSize(*t1);
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
                size2 = instantiations[1].endsWith("*") ? sizeof(void*) : getValueSize(*t2);
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
            || internalTypeName.startsWith("QLinkedList<")) && internalTypeName.endsWith(">") ){
        return 8;
    }
    return 0;
}

struct MetaData{
    MetaData(int _metaType = QMetaType::UnknownType,
            QHashFunction _hashFunction = nullptr,
            QLessFunction _lessFunction = nullptr,
            QEqualFunction _equalFunction = nullptr)
        : metaType(_metaType),
          hashFunction(_hashFunction),
          lessFunction(_lessFunction),
          equalFunction(_equalFunction)
    {}
    MetaData(const MetaData& d)
        : metaType(d.metaType),
          hashFunction(d.hashFunction),
          lessFunction(d.lessFunction),
          equalFunction(d.equalFunction)
    {}

    int metaType = QMetaType::UnknownType;
    QHashFunction hashFunction;
    QLessFunction lessFunction;
    QEqualFunction equalFunction;
};

struct Storages{
    QThreadStorage<MetaData> storage[2];
};

Q_GLOBAL_STATIC(Storages, gCurrentMetaData)

const MetaData& currentMetaData(uint i)
{
    return gCurrentMetaData->storage[i].localData();
}

void setCurrentMetaData(uint i, const MetaData& metaData)
{
    gCurrentMetaData->storage[i].setLocalData(metaData);
}

void constructContainerElement(uint i, void* ptr, const void* copy)
{
    const MetaData& metaData = currentMetaData(i);
    Q_ASSERT(metaData.metaType!=QMetaType::UnknownType);
//    if(copy)
//        fprintf(stderr, "copy %s from %p at %p\n", QMetaType::typeName(metaData.metaType), copy, ptr);
//    else
//        fprintf(stderr, "creating %s at %p\n", QMetaType::typeName(metaData.metaType), ptr);
    QMetaType::construct(metaData.metaType, ptr, copy);
}
void destructContainerElement(uint i, void* ptr)
{
    const MetaData& metaData = currentMetaData(i);
    Q_ASSERT(metaData.metaType!=QMetaType::UnknownType);
//    fprintf(stderr, "delete %s at %p\n", QMetaType::typeName(metaData.metaType), ptr);
    QMetaType::destruct(metaData.metaType, ptr);
}

uint containerElementHash(uint i, const void* ptr)
{
    const MetaData& metaData = currentMetaData(i);
    Q_ASSERT(metaData.metaType!=QMetaType::UnknownType);
    if(metaData.hashFunction){
        return metaData.hashFunction(ptr);
    }
    return 0;
}

bool containerElementEqual(uint i, const void* ptr, const void* ptr2)
{
    const MetaData& metaData = currentMetaData(i);
    Q_ASSERT(metaData.metaType!=QMetaType::UnknownType);
    if(metaData.equalFunction){
        return metaData.equalFunction(ptr, ptr2);
    }
    return false;
}

bool containerElementLess(uint i, const void* ptr, const void* ptr2)
{
    const MetaData& metaData = currentMetaData(i);
    Q_ASSERT(metaData.metaType!=QMetaType::UnknownType);
    if(metaData.lessFunction){
        return metaData.lessFunction(ptr, ptr2);
    }
    return false;
}

void destruct(uint index,
              int metaType,
              QHashFunction hashFunction,
              QLessFunction lessFunction,
              QEqualFunction equalFunction,
              QMetaType::Destructor destructor,
              void * ptr){
    MetaData oldMetaData = currentMetaData(index);
    setCurrentMetaData(index, MetaData(metaType, hashFunction, lessFunction, equalFunction));
    destructor(ptr);
    setCurrentMetaData(index, oldMetaData);
}

QMetaType::Destructor ContainerConverter::wrapDestructor(int metaType,
                                                        QHashFunction hashFunction,
                                                        QLessFunction lessFunction,
                                                        QEqualFunction equalFunction,
                                                        uint index,
                                                        QMetaType::Destructor destructor,
                                                        uint hash){
    return qtjambi_function_pointer<16 /* = 1024 options */,void(void*)>([index, metaType, hashFunction, lessFunction, equalFunction, destructor](void * ptr){
        destruct(index, metaType, hashFunction, lessFunction, equalFunction, destructor, ptr);
    }, hash);
}

void* construct(uint index,
               int metaType,
               QHashFunction hashFunction,
               QLessFunction lessFunction,
               QEqualFunction equalFunction,
               QMetaType::Constructor constructor, void *where, const void * copy){
    MetaData oldMetaData = currentMetaData(index);
    setCurrentMetaData(index, MetaData(metaType, hashFunction, lessFunction, equalFunction));
    void * result = constructor(where, copy);
    setCurrentMetaData(index, oldMetaData);
    return result;
}

QMetaType::Constructor ContainerConverter::wrapConstructor(int metaType,
                                                          QHashFunction hashFunction,
                                                          QLessFunction lessFunction,
                                                          QEqualFunction equalFunction,
                                                          uint index,
                                                          QMetaType::Constructor constructor, uint hash){
    return qtjambi_function_pointer<16 /* = 1024 options */,void*(void*, const void *)>([index, metaType, hashFunction, lessFunction, equalFunction, constructor](void *where, const void * copy)->void *{
        return construct(index, metaType, hashFunction, lessFunction, equalFunction, constructor, where, copy);
    }, hash);
}

void destruct(int metaType1,
              QHashFunction hashFunction1,
              QLessFunction lessFunction1,
              QEqualFunction equalFunction1,
              int metaType2,
              QHashFunction hashFunction2,
              QLessFunction lessFunction2,
              QEqualFunction equalFunction2,
              QMetaType::Destructor destructor,
              void * ptr){
    MetaData oldMetaData1 = currentMetaData(0);
    setCurrentMetaData(0, MetaData(metaType1, hashFunction1, lessFunction1, equalFunction1));
    MetaData oldMetaData2 = currentMetaData(1);
    setCurrentMetaData(1, MetaData(metaType2, hashFunction2, lessFunction2, equalFunction2));
    destructor(ptr);
    setCurrentMetaData(0, oldMetaData1);
    setCurrentMetaData(1, oldMetaData2);
}

QMetaType::Destructor ContainerConverter::wrapDestructor(int metaType1,
                                                        QHashFunction hashFunction1,
                                                        QLessFunction lessFunction1,
                                                        QEqualFunction equalFunction1,
                                                        int metaType2,
                                                        QHashFunction hashFunction2,
                                                        QLessFunction lessFunction2,
                                                        QEqualFunction equalFunction2,
                                                        QMetaType::Destructor destructor,
                                                        uint hash){
    return qtjambi_function_pointer<16 /* = 1024 options */,void(void*)>([metaType1, hashFunction1, lessFunction1, equalFunction1,
                                                                          metaType2, hashFunction2, lessFunction2, equalFunction2,
                                                                          destructor](void * ptr){
        destruct(metaType1, hashFunction1, lessFunction1, equalFunction1,
                 metaType2, hashFunction2, lessFunction2, equalFunction2,
                 destructor, ptr);
    }, hash);
}

void* construct(int metaType1,
                QHashFunction hashFunction1,
                QLessFunction lessFunction1,
                QEqualFunction equalFunction1,
                int metaType2,
                QHashFunction hashFunction2,
                QLessFunction lessFunction2,
                QEqualFunction equalFunction2,
                QMetaType::Constructor constructor, void *where, const void * copy){
    MetaData oldMetaData1 = currentMetaData(0);
    setCurrentMetaData(0, MetaData(metaType1, hashFunction1, lessFunction1, equalFunction1));
    MetaData oldMetaData2 = currentMetaData(1);
    setCurrentMetaData(1, MetaData(metaType2, hashFunction2, lessFunction2, equalFunction2));
    void * result = constructor(where, copy);
    setCurrentMetaData(0, oldMetaData1);
    setCurrentMetaData(1, oldMetaData2);
    return result;
}

QMetaType::Constructor ContainerConverter::wrapConstructor(int metaType1,
                                                          QHashFunction hashFunction1,
                                                          QLessFunction lessFunction1,
                                                          QEqualFunction equalFunction1,
                                                          int metaType2,
                                                          QHashFunction hashFunction2,
                                                          QLessFunction lessFunction2,
                                                          QEqualFunction equalFunction2,
                                                          QMetaType::Constructor constructor, uint hash){
    return qtjambi_function_pointer<16 /* = 1024 options */,void*(void*, const void *)>([metaType1, hashFunction1, lessFunction1, equalFunction1,
                                                                                         metaType2, hashFunction2, lessFunction2, equalFunction2,
                                                                                         constructor](void *where, const void * copy)->void *{
        return construct(metaType1, hashFunction1, lessFunction1, equalFunction1,
                         metaType2, hashFunction2, lessFunction2, equalFunction2,
                         constructor, where, copy);
    }, hash);
}

int ContainerConverter::registerContainerType(const QString& typeName, size_t containerSize, bool isPointer, int metaType,
                                      QMetaType::Constructor constructor,
                                      QMetaType::Destructor destructor,
                                      HashWrapper hashWrapper,
                                      QEqualFunction equalWrapper)
{
    QHashFunction hashFunction = nullptr;
    QLessFunction lessFunction = nullptr;
    QEqualFunction equalFunction = nullptr;
    findFunction(hashFunction, lessFunction, equalFunction, isPointer, metaType);
    if(!isPointer){
        destructor = wrapDestructor(metaType, hashFunction, lessFunction, equalFunction, 0, destructor, qHash(typeName));
        constructor = wrapConstructor(metaType, hashFunction, lessFunction, equalFunction, 0, constructor, qHash(typeName));
    }
    int newMetaType = QMetaType::registerType(qPrintable(typeName),
                            nullptr,
                            nullptr,
                            destructor,
                            constructor,
                            int(containerSize),
                            QMetaType::MovableType | QMetaType::NeedsDestruction | QMetaType::NeedsConstruction,
                            nullptr
                        );
    if(!hashFunctionByMetaType(newMetaType)){
        insertHashFunctionByMetaType(newMetaType,
                                            [metaType, hashFunction, lessFunction, equalFunction, hashWrapper]
                                            (const void* ptr)->uint{
                                                if(ptr){
                                                    MetaData oldMetaData = currentMetaData(0);
                                                    setCurrentMetaData(0, MetaData(metaType, hashFunction, lessFunction, equalFunction));
                                                    uint result = hashWrapper(ptr, hashFunction);
                                                    setCurrentMetaData(0, oldMetaData);
                                                    return result;
                                                }else{
                                                    return 0;
                                                }
                                            });
    }

    if(!equalFunctionByMetaType(newMetaType)){
        insertEqualFunctionByMetaType(newMetaType,
                                            [metaType, hashFunction, lessFunction, equalFunction, equalWrapper]
                                            (const void* ptr1, const void* ptr2)->bool{
                                                if(ptr1 && ptr2){
                                                    MetaData oldMetaData = currentMetaData(0);
                                                    setCurrentMetaData(0, MetaData(metaType, hashFunction, lessFunction, equalFunction));
                                                    bool result = equalWrapper(ptr1, ptr2);
                                                    setCurrentMetaData(0, oldMetaData);
                                                    return result;
                                                }
                                                return false;
                                            });
    }
    return newMetaType;
}

int ContainerConverter::registerBiContainerType(const QString& typeName,
                                                size_t containerSize,
                                                bool isPointer1, int metaType1,
                                                bool isPointer2, int metaType2,
                                                QMetaType::Constructor constructor,
                                                QMetaType::Destructor destructor,
                                                HashWrapper2 hashWrapper,
                                                QEqualFunction equalWrapper)
{
    QHashFunction hashFunction1 = nullptr;
    QLessFunction lessFunction1 = nullptr;
    QEqualFunction equalFunction1 = nullptr;
    QHashFunction hashFunction2 = nullptr;
    QLessFunction lessFunction2 = nullptr;
    QEqualFunction equalFunction2 = nullptr;
    findFunction(hashFunction1, lessFunction1, equalFunction1, isPointer1, metaType1);
    findFunction(hashFunction2, lessFunction2, equalFunction2, isPointer2, metaType2);
    if(!isPointer1 && !isPointer2){
        destructor = wrapDestructor(metaType1, hashFunction1, lessFunction1, equalFunction1,
                                    metaType2, hashFunction2, lessFunction2, equalFunction2,
                                    destructor, qHash(typeName));
        constructor = wrapConstructor(metaType1, hashFunction1, lessFunction1, equalFunction1,
                                      metaType2, hashFunction2, lessFunction2, equalFunction2,
                                      constructor, qHash(typeName));
    }else if(!isPointer1){
        destructor = wrapDestructor(metaType1, hashFunction1, lessFunction1, equalFunction1, 0, destructor, qHash(typeName));
        constructor = wrapConstructor(metaType1, hashFunction1, lessFunction1, equalFunction1, 0, constructor, qHash(typeName));
    }else if(!isPointer2){
        destructor = wrapDestructor(metaType2, hashFunction2, lessFunction2, equalFunction2, 1, destructor, qHash(typeName));
        constructor = wrapConstructor(metaType2, hashFunction2, lessFunction2, equalFunction2, 1, constructor, qHash(typeName));
    }
    int newMetaType = QMetaType::registerType(qPrintable(typeName),
                            nullptr,
                            nullptr,
                            destructor,
                            constructor,
                            int(containerSize),
                            QMetaType::MovableType | QMetaType::NeedsDestruction | QMetaType::NeedsConstruction,
                            nullptr
                        );
    if(!hashFunctionByMetaType(newMetaType)){
        insertHashFunctionByMetaType(newMetaType,
                                            [metaType1, hashFunction1, lessFunction1, equalFunction1,
                                             metaType2, hashFunction2, lessFunction2, equalFunction2, hashWrapper]
                                            (const void* ptr)->uint{
                                                if(ptr){
                                                    MetaData oldMetaData1 = currentMetaData(0);
                                                    setCurrentMetaData(0, MetaData(metaType1, hashFunction1, lessFunction1, equalFunction1));
                                                    MetaData oldMetaData2 = currentMetaData(1);
                                                    setCurrentMetaData(1, MetaData(metaType2, hashFunction2, lessFunction2, equalFunction2));
                                                    uint result = hashWrapper(ptr, hashFunction1, hashFunction2);
                                                    setCurrentMetaData(0, oldMetaData1);
                                                    setCurrentMetaData(1, oldMetaData2);
                                                    return result;
                                                }else{
                                                    return 0;
                                                }
                                            });
    }

    if(!equalFunctionByMetaType(newMetaType)){
        insertEqualFunctionByMetaType(newMetaType,
                                            [metaType1, hashFunction1, lessFunction1, equalFunction1,
                                             metaType2, hashFunction2, lessFunction2, equalFunction2, equalWrapper]
                                            (const void* ptr1, const void* ptr2)->bool{
                                                if(ptr1 && ptr2){
                                                    MetaData oldMetaData1 = currentMetaData(0);
                                                    setCurrentMetaData(0, MetaData(metaType1, hashFunction1, lessFunction1, equalFunction1));
                                                    MetaData oldMetaData2 = currentMetaData(1);
                                                    setCurrentMetaData(1, MetaData(metaType2, hashFunction2, lessFunction2, equalFunction2));
                                                    bool result = equalWrapper(ptr1, ptr2);
                                                    setCurrentMetaData(0, oldMetaData1);
                                                    setCurrentMetaData(1, oldMetaData2);
                                                    return result;
                                                }
                                                return false;
                                            });
    }
    return newMetaType;
}

ExternalToInternalConverter ContainerConverter::getExternalToInternalConverter(
                                                                  const ExternalToInternalConverter& memberConverter1, int metaType1, size_t size1,
                                                                  const ExternalToInternalConverter& memberConverter2, int metaType2, size_t size2,
                                                                  QMetaType::Creator creator, QMetaType::Deleter deleter, MapInsertFunction insertFunction)
{
    QHashFunction hashFunction1 = nullptr;
    QLessFunction lessFunction1 = nullptr;
    QEqualFunction equalFunction1 = nullptr;
    QHashFunction hashFunction2 = nullptr;
    QLessFunction lessFunction2 = nullptr;
    QEqualFunction equalFunction2 = nullptr;
    findFunction(hashFunction1, lessFunction1, equalFunction1, size1, metaType1);
    findFunction(hashFunction2, lessFunction2, equalFunction2, size2, metaType2);
    return [metaType1, hashFunction1, lessFunction1, equalFunction1, memberConverter1,
            metaType2, hashFunction2, lessFunction2, equalFunction2, memberConverter2,
            creator, deleter, insertFunction]
           (JNIEnv* env, QtJambiScope* scope, const jvalue& val, void* &out, jValueType) -> ConvertResponse{
        MetaData oldMetaData1 = currentMetaData(0);
        setCurrentMetaData(0, MetaData(metaType1, hashFunction1, lessFunction1, equalFunction1));
        MetaData oldMetaData2 = currentMetaData(1);
        setCurrentMetaData(1, MetaData(metaType2, hashFunction2, lessFunction2, equalFunction2));
        if(scope && !out){
            out = creator(nullptr);
            scope->addFinalAction([out, metaType1, hashFunction1, lessFunction1, equalFunction1,
                                        metaType2, hashFunction2, lessFunction2, equalFunction2, deleter](){
                MetaData oldMetaData1 = currentMetaData(0);
                setCurrentMetaData(0, MetaData(metaType1, hashFunction1, lessFunction1, equalFunction1));
                MetaData oldMetaData2 = currentMetaData(1);
                setCurrentMetaData(1, MetaData(metaType2, hashFunction2, lessFunction2, equalFunction2));
                deleter(out);
                setCurrentMetaData(0, oldMetaData1);
                setCurrentMetaData(1, oldMetaData2);
            });
        }
        ConvertResponse response = ConvertResponse::NotSuccessful;
        if(out){
            response = ConvertResponse::Successful;
            jobject iter = qtjambi_map_entryset_iterator(env, val.l);
            while(qtjambi_iterator_has_next(env, iter)){
                jobject entry = qtjambi_iterator_next(env, iter);
                jobject _key = qtjambi_map$entry_key(env, entry);
                jobject _val = qtjambi_map$entry_value(env, entry);
                response = insertFunction(out, env, scope, _key, _val, memberConverter1, memberConverter2);
                if(response!=ConvertResponse::Successful){
                    break;
                }
            }
        }
        setCurrentMetaData(0, oldMetaData1);
        setCurrentMetaData(1, oldMetaData2);
        return response;
    };
}

ExternalToInternalConverter ContainerConverter::getExternalToInternalConverter_QPair(
                                                                  const ExternalToInternalConverter& memberConverter1, int metaType1, size_t size1,
                                                                  const ExternalToInternalConverter& memberConverter2, int metaType2, size_t size2,
                                                                  QMetaType::Creator creator, QMetaType::Deleter deleter, MapInsertFunction insertFunction)
{
    QHashFunction hashFunction1 = nullptr;
    QLessFunction lessFunction1 = nullptr;
    QEqualFunction equalFunction1 = nullptr;
    QHashFunction hashFunction2 = nullptr;
    QLessFunction lessFunction2 = nullptr;
    QEqualFunction equalFunction2 = nullptr;
    findFunction(hashFunction1, lessFunction1, equalFunction1, size1, metaType1);
    findFunction(hashFunction2, lessFunction2, equalFunction2, size2, metaType2);
    return [metaType1, hashFunction1, lessFunction1, equalFunction1, memberConverter1,
            metaType2, hashFunction2, lessFunction2, equalFunction2, memberConverter2,
            creator, deleter, insertFunction]
           (JNIEnv* env, QtJambiScope* scope, const jvalue& val, void* &out, jValueType) -> ConvertResponse{
        MetaData oldMetaData1 = currentMetaData(0);
        setCurrentMetaData(0, MetaData(metaType1, hashFunction1, lessFunction1, equalFunction1));
        MetaData oldMetaData2 = currentMetaData(1);
        setCurrentMetaData(1, MetaData(metaType2, hashFunction2, lessFunction2, equalFunction2));
        if(scope && !out){
            out = creator(nullptr);
            scope->addFinalAction([out, metaType1, hashFunction1, lessFunction1, equalFunction1,
                                        metaType2, hashFunction2, lessFunction2, equalFunction2, deleter](){
                MetaData oldMetaData1 = currentMetaData(0);
                setCurrentMetaData(0, MetaData(metaType1, hashFunction1, lessFunction1, equalFunction1));
                MetaData oldMetaData2 = currentMetaData(1);
                setCurrentMetaData(1, MetaData(metaType2, hashFunction2, lessFunction2, equalFunction2));
                deleter(out);
                setCurrentMetaData(0, oldMetaData1);
                setCurrentMetaData(1, oldMetaData2);
            });
        }
        ConvertResponse response = ConvertResponse::NotSuccessful;
        if(out){
            jobject key = Java::Private::QtCore::QPair.first(env, val.l);
            jobject snd = Java::Private::QtCore::QPair.second(env, val.l);
            response = insertFunction(out, env, scope, key, snd, memberConverter1, memberConverter2);
        }
        setCurrentMetaData(0, oldMetaData1);
        setCurrentMetaData(1, oldMetaData2);
        return response;
    };
}

ExternalToInternalConverter ContainerConverter::getExternalToInternalConverter(const ExternalToInternalConverter& memberConverter, int metaType, size_t size,
                                                                               QMetaType::Creator creator, QMetaType::Deleter deleter, ContainerInsertFunction insertFunction)
{
    QHashFunction hashFunction = nullptr;
    QLessFunction lessFunction = nullptr;
    QEqualFunction equalFunction = nullptr;
    findFunction(hashFunction, lessFunction, equalFunction, size, metaType);
    return [memberConverter, metaType, hashFunction, lessFunction, equalFunction, creator, deleter, insertFunction]
           (JNIEnv* env, QtJambiScope* scope, const jvalue& val, void* &out, jValueType) -> ConvertResponse{
        MetaData oldMetaData = currentMetaData(0);
        setCurrentMetaData(0, MetaData(metaType, hashFunction, lessFunction, equalFunction));
        if(scope && !out){
            out = creator(nullptr);
            scope->addFinalAction([metaType, hashFunction, lessFunction, equalFunction, deleter, out](){
                MetaData oldMetaData = currentMetaData(0);
                setCurrentMetaData(0, MetaData(metaType, hashFunction, lessFunction, equalFunction));
                deleter(out);
                setCurrentMetaData(0, oldMetaData);
            });
        }
        if(!out){
            setCurrentMetaData(0, oldMetaData);
            return ConvertResponse::NotSuccessful;
        }
        jobject iter = qtjambi_collection_iterator(env, val.l);
        while(qtjambi_iterator_has_next(env, iter)){
            ConvertResponse response = insertFunction(out, env, scope, qtjambi_iterator_next(env, iter), memberConverter);
            if(response!=ConvertResponse::Successful){
                setCurrentMetaData(0, oldMetaData);
                return response;
            }
        }
        setCurrentMetaData(0, oldMetaData);
        return ConvertResponse::Successful;
    };
}

InternalToExternalConverter ContainerConverter::getInternalToExternalConverter(const InternalToExternalConverter& memberConverter,
                                                                               int metaType, size_t size, ContainerType containerType,
                                                                               FillJavaCollection fillJavaCollection)
{
    QHashFunction hashFunction = nullptr;
    QLessFunction lessFunction = nullptr;
    QEqualFunction equalFunction = nullptr;
    findFunction(hashFunction, lessFunction, equalFunction, size, metaType);
    return [memberConverter, containerType, metaType, hashFunction, lessFunction, equalFunction, fillJavaCollection]
            (JNIEnv* env, QtJambiScope* scope, const void* in, jvalue* p, bool)->bool{
        if(in){
            MetaData oldMetaData = currentMetaData(0);
            setCurrentMetaData(0, MetaData(metaType, hashFunction, lessFunction, equalFunction));
            switch(containerType){
            case ContainerType::Deque:
                p->l = Java::Private::Runtime::ArrayDeque.newInstance(env);
                break;
            case ContainerType::List:
                p->l = Java::Private::Runtime::ArrayList.newInstance(env);
                break;
            case ContainerType::Set:
                p->l = Java::Private::Runtime::HashSet.newInstance(env);
                break;
            default:
                p->l = Java::Private::Runtime::LinkedList.newInstance(env);
                break;
            }
            bool result = fillJavaCollection(env, scope, in, p->l, memberConverter);
            setCurrentMetaData(0, oldMetaData);
            return result;
        }
        return true;
    };
}

bool convertKeyValue(JNIEnv* env, QtJambiScope* scope,
        QList<QPair<jobject,jobject>>& elements,
        const void* _key, const void* _value,
        const InternalToExternalConverter& memberConverter1,
        const InternalToExternalConverter& memberConverter2,
        bool* areComparable, int* enumType)
{
    jvalue key;
    key.l = nullptr;
    if(!memberConverter1(env, scope, _key, &key, true)){
        return false;
    }
    jvalue val;
    val.l = nullptr;
    if(!memberConverter2(env, scope, _value, &val, true)){
        return false;
    }
    elements << QPair<jobject,jobject>(key.l, val.l);
    if(areComparable)
        *areComparable &= bool(Java::Private::Runtime::Comparable.isInstanceOf(env, key.l));
    if(enumType){
        if(*enumType>=0){
            if(Java::Private::QtJambi::QtEnumerator.isInstanceOf(env, key.l))
                *enumType = 4;
            else if(Java::Private::QtJambi::QtShortEnumerator.isInstanceOf(env, key.l))
                *enumType = 2;
            else if(Java::Private::QtJambi::QtByteEnumerator.isInstanceOf(env, key.l))
                *enumType = 1;
            else if(Java::Private::QtJambi::QtLongEnumerator.isInstanceOf(env, key.l))
                *enumType = 2;
        }else if(*enumType==1){
            if(!Java::Private::QtJambi::QtByteEnumerator.isInstanceOf(env, key.l))
                *enumType = -1;
        }else if(*enumType==2){
            if(!Java::Private::QtJambi::QtShortEnumerator.isInstanceOf(env, key.l))
                *enumType = -1;
        }else if(*enumType==4){
            if(!Java::Private::QtJambi::QtEnumerator.isInstanceOf(env, key.l))
                *enumType = -1;
        }else if(*enumType==8){
            if(!Java::Private::QtJambi::QtLongEnumerator.isInstanceOf(env, key.l))
                *enumType = -1;
        }
    }
    return true;
}

bool lessThan(JNIEnv *env, jobject k1, jobject k2);

InternalToExternalConverter ContainerConverter::getInternalToExternalConverter(const InternalToExternalConverter& memberConverter1,
                                                                      int metaType1, size_t size1,
                                                                      const InternalToExternalConverter& memberConverter2,
                                                                      int metaType2, size_t size2,
                                                                      MapType mapType, MapPutMethod mapPutMethod, IterateMap iterateMap)
{
    QHashFunction hashFunction1 = nullptr;
    QLessFunction lessFunction1 = nullptr;
    QEqualFunction equalFunction1 = nullptr;
    QHashFunction hashFunction2 = nullptr;
    QLessFunction lessFunction2 = nullptr;
    QEqualFunction equalFunction2 = nullptr;
    findFunction(hashFunction1, lessFunction1, equalFunction1, size1, metaType1);
    findFunction(hashFunction2, lessFunction2, equalFunction2, size2, metaType2);
    return [metaType1, hashFunction1, lessFunction1, equalFunction1, memberConverter1,
            metaType2, hashFunction2, lessFunction2, equalFunction2, memberConverter2,
            mapType, mapPutMethod, iterateMap]
            (JNIEnv* env, QtJambiScope* scope, const void* in, jvalue* p, bool)->bool{
        MetaData oldMetaData1 = currentMetaData(0);
        setCurrentMetaData(0, MetaData(metaType1, hashFunction1, lessFunction1, equalFunction1));
        MetaData oldMetaData2 = currentMetaData(1);
        setCurrentMetaData(1, MetaData(metaType2, hashFunction2, lessFunction2, equalFunction2));
        bool result = true;
        if(in){
            jint size = 0;
            QList<QPair<jobject,jobject>> elements;
            bool areComparable = true;
            int enumType = 0;
            result = iterateMap(env, scope, elements, size,
                                mapType==MapType::TreeMap ? &areComparable : nullptr,
                                mapType==MapType::TreeMap ? &enumType : nullptr,
                                in, memberConverter1, memberConverter2, &convertKeyValue);

            if(result){
                switch(mapType){
                case MapType::TreeMap:{
                        jobject comparator = nullptr;
                        switch(enumType){
                        case 1:
                            comparator = Java::Private::QtJambi::QtByteEnumerator.comparator(env);
                            break;
                        case 2:
                            comparator = Java::Private::QtJambi::QtShortEnumerator.comparator(env);
                            break;
                        case 4:
                            comparator = Java::Private::QtJambi::QtEnumerator.comparator(env);
                            break;
                        case 8:
                            comparator = Java::Private::QtJambi::QtLongEnumerator.comparator(env);
                            break;
                        default:
                            if(!areComparable){
                                comparator = Java::Private::QtJambi::QtJambiInternal.createComparator(env, jlong(&lessThan));
                            }
                        }
                        if(comparator){
                            p->l = Java::Private::Runtime::TreeMap.newInstance2(env, comparator);
                        }else{
                            p->l = Java::Private::Runtime::TreeMap.newInstance(env);
                        }
                    }
                    break;
                default:
                    p->l = Java::Private::Runtime::HashMap.newInstance(env,size);
                }
                for(const QPair<jobject,jobject>& pair : elements){
                    mapPutMethod(env, p->l, pair.first, pair.second);
                }
            }
        }
        setCurrentMetaData(0, oldMetaData1);
        setCurrentMetaData(1, oldMetaData2);
        return true;
    };
}

InternalToExternalConverter ContainerConverter::getInternalToExternalConverter_QPair(const InternalToExternalConverter& memberConverter1,
                                                                      int metaType1, size_t size1,
                                                                      const InternalToExternalConverter& memberConverter2,
                                                                      int metaType2, size_t size2,
                                                                      GetElements getElements)
{
    QHashFunction hashFunction1 = nullptr;
    QLessFunction lessFunction1 = nullptr;
    QEqualFunction equalFunction1 = nullptr;
    QHashFunction hashFunction2 = nullptr;
    QLessFunction lessFunction2 = nullptr;
    QEqualFunction equalFunction2 = nullptr;
    findFunction(hashFunction1, lessFunction1, equalFunction1, size1, metaType1);
    findFunction(hashFunction2, lessFunction2, equalFunction2, size2, metaType2);
    return [metaType1, hashFunction1, lessFunction1, equalFunction1, memberConverter1,
            metaType2, hashFunction2, lessFunction2, equalFunction2, memberConverter2,
            getElements]
            (JNIEnv* env, QtJambiScope* scope, const void* in, jvalue* p, bool)->bool{
        MetaData oldMetaData1 = currentMetaData(0);
        setCurrentMetaData(0, MetaData(metaType1, hashFunction1, lessFunction1, equalFunction1));
        MetaData oldMetaData2 = currentMetaData(1);
        setCurrentMetaData(1, MetaData(metaType2, hashFunction2, lessFunction2, equalFunction2));
        bool result = true;
        if(in){
            const void* _first = nullptr;
            const void* _second = nullptr;
            getElements(in, _first, _second);
            jvalue first;
            first.l = nullptr;
            if(memberConverter1(env, scope, _first, &first, true)){
                jvalue second;
                second.l = nullptr;
                if(memberConverter2(env, scope, _second, &second, true)){
                    p->l = Java::Private::QtCore::QPair.newInstance(env, first.l, second.l);
                }else{
                    result = false;
                }
            }else{
                result = false;
            }
        }
        setCurrentMetaData(0, oldMetaData1);
        setCurrentMetaData(1, oldMetaData2);
        return result;
    };
}

void ContainerConverter::findFunction(QHashFunction &hashFunction, QLessFunction &lessFunction, QEqualFunction &equalFunction, bool isPointer, int metaType)
{
    if(isPointer){
        hashFunction = &pointerHashFunction;
        lessFunction = &pointerLessFunction;
        equalFunction = &pointerEqualFunction;
    }else{
        if(const std::type_info* typeId = getTypeByMetaType(metaType)){
            hashFunction = registeredHashFunction(*typeId);
            lessFunction = registeredLessFunction(*typeId);
            equalFunction = registeredEqualFunction(*typeId);
        }
        if(!hashFunction)
            hashFunction = hashFunctionByMetaType(metaType);
        if(!lessFunction)
            lessFunction = lessFunctionByMetaType(metaType);
        if(!equalFunction)
            equalFunction = equalFunctionByMetaType(metaType);
    }
}
