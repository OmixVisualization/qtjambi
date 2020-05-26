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

#ifndef QTJAMBITYPEMANAGER_H
#define QTJAMBITYPEMANAGER_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Qt Jambi API.
// This header file may change from version to version without notice,
// or even be removed.
//
// We mean it.
//
//

#include "qtjambi_core.h"
#include <QtCore/QSet>
#include <QtCore/QHash>
#include <QtCore/QString>
#include <QtCore/QMetaType>
#include <QtCore/QReadWriteLock>
#include <QtCore/QVector>
#include <functional>
#include "qtjambi_repository_p.h"
#include "qtjambi_registry_p.h"
#include "qtjambi_templates.h"

#define ELEMENT_STATICSIZE_SWITCH2(isStaticType,size)\
    case size:\
    if(isStaticType){\
        ELEMENT_STATICSIZE_CASEACTION(true,size)\
    }else{\
        ELEMENT_STATICSIZE_CASEACTION(false,size)\
    }

#define ELEMENT_STATICSIZE_SWITCH(isStaticType,size)\
    switch(size){\
    case 0: ELEMENT_STATICSIZE_CASEACTION(false,0)\
    ELEMENT_STATICSIZE_SWITCH2(isStaticType,1)\
    ELEMENT_STATICSIZE_SWITCH2(isStaticType,2)\
    ELEMENT_STATICSIZE_SWITCH2(isStaticType,4)\
    ELEMENT_STATICSIZE_SWITCH2(isStaticType,6)\
    ELEMENT_STATICSIZE_SWITCH2(isStaticType,8)\
    ELEMENT_STATICSIZE_SWITCH2(isStaticType,10)\
    ELEMENT_STATICSIZE_SWITCH2(isStaticType,12)\
    ELEMENT_STATICSIZE_SWITCH2(isStaticType,14)\
    ELEMENT_STATICSIZE_SWITCH2(isStaticType,16)\
    ELEMENT_STATICSIZE_SWITCH2(isStaticType,24)\
    ELEMENT_STATICSIZE_SWITCH2(isStaticType,32)\
    ELEMENT_STATICSIZE_SWITCH2(isStaticType,48)\
    ELEMENT_STATICSIZE_SWITCH2(isStaticType,56)\
    ELEMENT_STATICSIZE_SWITCH2(isStaticType,64)\
    }

#define ELEMENT_ALIGNSIZE_CASE(align,size)\
    case align: ELEMENT_ALIGNSIZE_CASEACTION(align,size)

#define ELEMENT_ALIGNSIZE_SWITCH2(align,size)\
    case size:\
    switch(align){\
    ELEMENT_ALIGNSIZE_CASE(1,size)\
    ELEMENT_ALIGNSIZE_CASE(2,size)\
    ELEMENT_ALIGNSIZE_CASE(4,size)\
    ELEMENT_ALIGNSIZE_CASE(8,size)\
    ELEMENT_ALIGNSIZE_CASE(16,size)\
    }\
    break

#define ELEMENT_ALIGNSIZE_SWITCH(align,size)\
    switch(size){\
    case 0: ELEMENT_ALIGNSIZE_CASEACTION(0,0)\
    ELEMENT_ALIGNSIZE_SWITCH2(align,1);\
    ELEMENT_ALIGNSIZE_SWITCH2(align,2);\
    ELEMENT_ALIGNSIZE_SWITCH2(align,4);\
    ELEMENT_ALIGNSIZE_SWITCH2(align,6);\
    ELEMENT_ALIGNSIZE_SWITCH2(align,8);\
    ELEMENT_ALIGNSIZE_SWITCH2(align,10);\
    ELEMENT_ALIGNSIZE_SWITCH2(align,12);\
    ELEMENT_ALIGNSIZE_SWITCH2(align,14);\
    ELEMENT_ALIGNSIZE_SWITCH2(align,16);\
    ELEMENT_ALIGNSIZE_SWITCH2(align,24);\
    ELEMENT_ALIGNSIZE_SWITCH2(align,32);\
    ELEMENT_ALIGNSIZE_SWITCH2(align,48);\
    ELEMENT_ALIGNSIZE_SWITCH2(align,56);\
    ELEMENT_ALIGNSIZE_SWITCH2(align,64);\
    }

#define ELEMENT_SIZE_CASE(size)\
    case size: ELEMENT_SIZE_CASEACTION(size)

#define ELEMENT_SIZE_SWITCH(size)\
    switch(size){\
    ELEMENT_SIZE_CASE(0)\
    ELEMENT_SIZE_CASE(1)\
    ELEMENT_SIZE_CASE(2)\
    ELEMENT_SIZE_CASE(4)\
    ELEMENT_SIZE_CASE(6)\
    ELEMENT_SIZE_CASE(8)\
    ELEMENT_SIZE_CASE(10)\
    ELEMENT_SIZE_CASE(12)\
    ELEMENT_SIZE_CASE(14)\
    ELEMENT_SIZE_CASE(16)\
    ELEMENT_SIZE_CASE(24)\
    ELEMENT_SIZE_CASE(32)\
    ELEMENT_SIZE_CASE(48)\
    ELEMENT_SIZE_CASE(56)\
    ELEMENT_SIZE_CASE(64)\
    ELEMENT_SIZE_CASE(72)\
    ELEMENT_SIZE_CASE(96)\
    ELEMENT_SIZE_CASE(128)\
    }

#define ELEMENT_ALIGNSIZE2_CASE(align, size, align2, size2)\
    case align2: ELEMENT_ALIGNSIZE2_CASEACTION(align, size, align2, size2)

#define ELEMENT_ALIGNSIZE2_INNER_ALIGNSWITCH(align, size, align2, size2)\
    case size2:\
    switch(align2){\
    ELEMENT_ALIGNSIZE2_CASE(align,size,1,size2)\
    ELEMENT_ALIGNSIZE2_CASE(align,size,2,size2)\
    ELEMENT_ALIGNSIZE2_CASE(align,size,4,size2)\
    ELEMENT_ALIGNSIZE2_CASE(align,size,8,size2)\
    ELEMENT_ALIGNSIZE2_CASE(align,size,16,size2)\
    }\
    break

#define ELEMENT_ALIGNSIZE2_INNER_SWITCH(align,size,align2,size2)\
    case align:\
    switch(size2){\
        ELEMENT_ALIGNSIZE2_CASE(align,size,0,0)\
        break;\
    ELEMENT_ALIGNSIZE2_INNER_ALIGNSWITCH(align,size,align2,1);\
    ELEMENT_ALIGNSIZE2_INNER_ALIGNSWITCH(align,size,align2,2);\
    ELEMENT_ALIGNSIZE2_INNER_ALIGNSWITCH(align,size,align2,4);\
    ELEMENT_ALIGNSIZE2_INNER_ALIGNSWITCH(align,size,align2,8);\
    ELEMENT_ALIGNSIZE2_INNER_ALIGNSWITCH(align,size,align2,16);\
    ELEMENT_ALIGNSIZE2_INNER_ALIGNSWITCH(align,size,align2,32);\
    ELEMENT_ALIGNSIZE2_INNER_ALIGNSWITCH(align,size,align2,64);\
    }\
    break

#define ELEMENT_ALIGNSIZE2_ALIGNSWITCH(align, size, align2, size2)\
    case size:\
    switch(align){\
    ELEMENT_ALIGNSIZE2_INNER_SWITCH(1,size,align2,size2);\
    ELEMENT_ALIGNSIZE2_INNER_SWITCH(2,size,align2,size2);\
    ELEMENT_ALIGNSIZE2_INNER_SWITCH(4,size,align2,size2);\
    ELEMENT_ALIGNSIZE2_INNER_SWITCH(8,size,align2,size2);\
    ELEMENT_ALIGNSIZE2_INNER_SWITCH(16,size,align2,size2);\
    }\
    break

#define ELEMENT_ALIGNSIZE2_SWITCH(align,size,align2,size2)\
    switch(size){\
    case 0:\
        switch(size2){\
            ELEMENT_ALIGNSIZE2_CASE(0,0,0,0);\
            break;\
        ELEMENT_ALIGNSIZE2_INNER_ALIGNSWITCH(0,0,align2,1);\
        ELEMENT_ALIGNSIZE2_INNER_ALIGNSWITCH(0,0,align2,2);\
        ELEMENT_ALIGNSIZE2_INNER_ALIGNSWITCH(0,0,align2,4);\
        ELEMENT_ALIGNSIZE2_INNER_ALIGNSWITCH(0,0,align2,8);\
        ELEMENT_ALIGNSIZE2_INNER_ALIGNSWITCH(0,0,align2,16);\
        ELEMENT_ALIGNSIZE2_INNER_ALIGNSWITCH(0,0,align2,32);\
        ELEMENT_ALIGNSIZE2_INNER_ALIGNSWITCH(0,0,align2,64);\
        }\
    break;\
    ELEMENT_ALIGNSIZE2_ALIGNSWITCH(align,1,align2,size2);\
    ELEMENT_ALIGNSIZE2_ALIGNSWITCH(align,2,align2,size2);\
    ELEMENT_ALIGNSIZE2_ALIGNSWITCH(align,4,align2,size2);\
    ELEMENT_ALIGNSIZE2_ALIGNSWITCH(align,8,align2,size2);\
    ELEMENT_ALIGNSIZE2_ALIGNSWITCH(align,16,align2,size2);\
    ELEMENT_ALIGNSIZE2_ALIGNSWITCH(align,32,align2,size2);\
    }

class QtJambiTypeManager {
    public:
        enum TypePattern : quint64 {
            None = 0,
#if QT_VERSION >= 0x050000
            Void            = 0x00100000,
#endif
            Primitive       = 0x00000001,
            Integer         = 0x00000002,
            Long            = 0x00000004,
            Boolean         = 0x00000008,
            Float           = 0x00000010,
            Double          = 0x00000020,
            Short           = 0x00000040,
            Byte            = 0x00000080,
            Char            = 0x00000100,

            QObjectSubclass = 0x00000200,
            Object          = 0x00000400,

            NativePointer   = 0x00000800,

            Value           = 0x00001000,
            String          = 0x00002000,
            QtClass         = 0x00004000,
            Enum            = 0x00008000,
            Flags           = 0x00010000,
            Array           = 0x00020000,
            QtSubClass      = 0x00040000,
            SharedPointerWrapped = 0x00080000,
            WeakPointerWrapped   = 0x00200000,
            initializerListWrapper = 0x00400000,
            VariantPattern  = 0x00800000,
            Reference       = 0x01000000,
            Pointer         = 0x02000000,
            SharedPtrWrapped = 0x04000000,
            WeakPtrWrapped   = 0x08000000,
            UniquePtrWrapped = 0x10000000,
            ScopedPointerWrapped = 0x20000000,

            TypeMask = Integer + Long + Boolean + Float + Double + Short + Byte + Char
        };

        enum VariableContext {
            ReturnType,
            ArgumentType,
            InstantiationType
        };

        static TypePattern typeIdOfInternal(JNIEnv *env, const QString &internalType);
        static TypePattern typeIdOfInternal(JNIEnv *env, const QString &className, const QString &internalType, int metaType, jobject classLoader = nullptr);
        inline static void splitClassName(QString& className, QString& package, const QString &qualifiedName, QChar separator = QLatin1Char('/'));

        static size_t getInternalSize(const QString &internalTypeName);
        static size_t getInternalAlignment(const QString &internalTypeName);
        static bool isStaticType(const QString &internalTypeName);
        static QString getExternalTypeName(JNIEnv* env, const QString &internalTypeName, VariableContext ctx, int metaType = QMetaType::UnknownType);
        static QString getExternalTypeName(JNIEnv* env, const QString &internalTypeName, const QMetaObject * metaObject, VariableContext ctx, int metaType = QMetaType::UnknownType);
        static QString getExternalTypeNameNoWarn(JNIEnv* env, const QString &internalTypeName, const QMetaObject * metaObject, VariableContext ctx, int metaType = QMetaType::UnknownType);
        static QString getInternalTypeName(JNIEnv* env, const QString &externalTypeName, jobject classLoader = nullptr);
        static const InternalToExternalConverter& getInternalToExternalConverter(
                                       JNIEnv* env,
                                       const QString &internalTypeName,
                                       TypePattern internalType,
                                       int internalMetaType,
                                       jclass externalClass,
                                       bool allowValuePointers = false);
        static const ExternalToInternalConverter& getExternalToInternalConverter(JNIEnv* env, jclass externalClass, TypePattern externalType, const QString &internalTypeName, int internalMetaType);

        static QVector<QString> parseSignature(const QString &signature, QString *name = nullptr);

        enum EnumType{
            NoEnum,
            Enum32,
            Enum8,
            Enum16,
            Enum64
        };

private:
        enum PointerType{
            NoPointer,
            SharedPointer,
            WeakPointer,
            ScopedPointer,
            unique_ptr,
            shared_ptr,
            weak_ptr,
            initializer_list
        };

        static TypePattern typeIdOfInternal(JNIEnv *env, const QString &javaName, QtJambiTypeManager::PointerType pointerType, uint indirections, const QString &internalType, int metaType, jobject classLoader = nullptr);

        static EnumType getEnumType(JNIEnv* env, const QString &className, jobject classLoader = nullptr);
        static EnumType getEnumType(JNIEnv* env, jclass clazz);
        static bool isFlagsType(JNIEnv* env, const QString &className, jobject classLoader = nullptr);
        static bool isFlagsType(JNIEnv* env, jclass clazz);

        static QString complexTypeOf(TypePattern type);
        static QString primitiveTypeOf(TypePattern type);
        static QString toJNISignature(const QString &signature, QString *name);
        static QString demangle(const QString &mangledName);
        static QString mangle(const QString &demangledName);
        static TypePattern valueTypePattern(const QString &javaName);
        static QString closestQtSuperclass(JNIEnv *env, const QString &className, jobject classLoader = nullptr);
        static bool isQObjectSubclass(JNIEnv *env, const QString &className, jobject classLoader = nullptr);
        static bool isQtClass(JNIEnv *env, const QString &className, jobject classLoader = nullptr);
        static bool isQtSubClass(JNIEnv *env, const QString &className, jobject classLoader = nullptr);

        static QString getExternalTypeNameNoWarn(JNIEnv* env, const QString &internalTypeName, VariableContext ctx, int metaType = QMetaType::UnknownType);
        static InternalToExternalConverter getInternalToExternalConverterImpl(
                                       JNIEnv* env,
                                       const QString &internalTypeName,
                                       TypePattern internalType,
                                       int internalMetaType,
                                       jclass externalClass,
                                       bool allowValuePointers = false);
        static ExternalToInternalConverter getExternalToInternalConverterImpl(JNIEnv* env, jclass externalClass, TypePattern externalType, const QString &internalTypeName, int internalMetaType);
        static QString processInternalTypeName(const QString &typeName, uint &indirections, PointerType &pointerType);
        static void registerInternalSize(const QString &internalTypeName, size_t size);
        static void registerInternalAlignment(const QString &internalTypeName, size_t align);
        friend struct ContainerConverter;
};

// *********** Implementations ***********
inline void QtJambiTypeManager::splitClassName(QString& className, QString& package, const QString &qualifiedName, QChar separator){
    int idx = qualifiedName.lastIndexOf(separator);
    if (idx >= 0){
        className = qualifiedName.mid(idx + 1);
        package = qualifiedName.left(idx + 1);
    }else{
        className = qualifiedName;
        package = QString();
    }
}

void constructContainerElement(uint i, void* ptr, const void* copy = nullptr);
void destructContainerElement(uint i, void* ptr);
uint containerElementHash(uint i, const void* ptr);
bool containerElementEqual(uint i, const void* ptr, const void* ptr2);
bool containerElementLess(uint i, const void* ptr, const void* ptr2);
QHashFunction hashFunctionByMetaType(int metaType);
QLessFunction lessFunctionByMetaType(int metaType);
QEqualFunction equalFunctionByMetaType(int metaType);
uint pointerHashFunction(const void* ptr);
bool pointerLessFunction(const void* ptr1, const void* ptr2);
bool pointerEqualFunction(const void* ptr1, const void* ptr2);
QHashFunction hashFunctionByMetaType(int type);
QLessFunction lessFunctionByMetaType(int type);
QEqualFunction equalFunctionByMetaType(int type);
void insertHashFunctionByMetaType(int type, QHashFunction fct);
void insertLessFunctionByMetaType(int type, QLessFunction fct);
void insertEqualFunctionByMetaType(int type, QEqualFunction fct);

template<size_t size, uint i, bool isStatic, size_t align = 0, bool useAlignment = (align>0) >
struct ContainerElement{
};

template<size_t size, uint i, bool isStatic>
struct ContainerElement<size, i, isStatic, 0, false>{
    ContainerElement(nullptr_t = nullptr){
        constructContainerElement(i, this);
    }
    ContainerElement(const ContainerElement& copy){
        constructContainerElement(i, this, &copy);
    }
    ~ContainerElement(){
        destructContainerElement(i, this);
    }
private:
    qint8 data[size];
};

template<uint i, bool isStatic>
struct ContainerElement<1, i, isStatic, 0, false>{
    ContainerElement(nullptr_t = nullptr){
        constructContainerElement(i, this);
    }
    ContainerElement(const ContainerElement& copy){
        constructContainerElement(i, this, &copy);
    }
    ~ContainerElement(){
        destructContainerElement(i, this);
    }
private:
    qint8 data;
};

template<uint i, bool isStatic>
struct ContainerElement<2, i, isStatic, 0, false>{
    ContainerElement(nullptr_t = nullptr){
        constructContainerElement(i, this);
    }
    ContainerElement(const ContainerElement& copy){
        constructContainerElement(i, this, &copy);
    }
    ~ContainerElement(){
        destructContainerElement(i, this);
    }
private:
    qint16 data;
};

template<uint i, bool isStatic>
struct ContainerElement<4, i, isStatic, 0, false>{
    ContainerElement(nullptr_t = nullptr){
        constructContainerElement(i, this);
    }
    ContainerElement(const ContainerElement& copy){
        constructContainerElement(i, this, &copy);
    }
    ~ContainerElement(){
        destructContainerElement(i, this);
    }
private:
    qint32 data;
};

template<uint i, bool isStatic>
struct ContainerElement<8, i, isStatic, 0, false>{
    ContainerElement(nullptr_t = nullptr){
        constructContainerElement(i, this);
    }
    ContainerElement(const ContainerElement& copy){
        constructContainerElement(i, this, &copy);
    }
    ~ContainerElement(){
        destructContainerElement(i, this);
    }
private:
    qint32 data[2];
};

template<uint i, bool isStatic>
struct ContainerElement<16, i, isStatic, 0, false>{
    ContainerElement(nullptr_t = nullptr){
        constructContainerElement(i, this);
    }
    ContainerElement(const ContainerElement& copy){
        constructContainerElement(i, this, &copy);
    }
    ~ContainerElement(){
        destructContainerElement(i, this);
    }
private:
    qint32 data[4];
};

template<size_t size, uint i, bool isStatic, size_t align>
struct Q_DECL_ALIGN(align) ContainerElement<size, i, isStatic, align, true>{
    ContainerElement(nullptr_t = nullptr){
        constructContainerElement(i, this);
    }
    ContainerElement(const ContainerElement& copy){
        constructContainerElement(i, this, &copy);
    }
    ~ContainerElement(){
        destructContainerElement(i, this);
    }
private:
    qint8 data[size];
};

template<uint i, bool isStatic, size_t align>
struct Q_DECL_ALIGN(align) ContainerElement<1, i, isStatic, align, true>{
    ContainerElement(nullptr_t = nullptr){
        constructContainerElement(i, this);
    }
    ContainerElement(const ContainerElement& copy){
        constructContainerElement(i, this, &copy);
    }
    ~ContainerElement(){
        destructContainerElement(i, this);
    }
private:
    qint8 data;
};

template<size_t size, uint i, size_t align, bool useAlignment>
class QTypeInfo<ContainerElement<size, i, false, align, useAlignment>>
{
public:
    enum {
        isSpecialized = false,
        isPointer = false,
        isIntegral = false,
        isComplex = true,
        isStatic = false,
        isRelocatable = qIsRelocatable<ContainerElement<size,i,true, align, useAlignment>>(),
        isLarge = (size>sizeof(void*)),
        isDummy = false, //### Qt6: remove
        sizeOf = size
    };
};

template<size_t size, uint i, size_t align, bool useAlignment>
class QTypeInfo<ContainerElement<size, i, true, align, useAlignment>>
{
public:
    enum {
        isSpecialized = false,
        isPointer = false,
        isIntegral = false,
        isComplex = true,
        isStatic = true,
        isRelocatable = qIsRelocatable<ContainerElement<size,i,true, align, useAlignment>>(),
        isLarge = (size>sizeof(void*)),
        isDummy = false, //### Qt6: remove
        sizeOf = size
    };
};

template<size_t size, uint i, bool isStatic, size_t align, bool useAlignment>
uint qHash(const ContainerElement<size, i, isStatic, align, useAlignment>& value){
    return containerElementHash(i, &value);
}

template<size_t size, uint i, bool isStatic, size_t align, bool useAlignment>
bool operator ==(const ContainerElement<size, i, isStatic, align, useAlignment>& value1, const ContainerElement<size, i, isStatic, align, useAlignment>& value2){
    return containerElementEqual(i, &value1, &value2);
}

template<size_t size, uint i, bool isStatic, size_t align, bool useAlignment>
bool operator <(const ContainerElement<size, i, isStatic, align, useAlignment>& value1, const ContainerElement<size, i, isStatic, align, useAlignment>& value2){
    return containerElementLess(i, &value1, &value2);
}

enum class ContainerType{
    List,
    Deque,
    LinkedList,
    Set
};

enum class MapType{
    HashMap,
    TreeMap
};

#if !defined(Q_NO_TEMPLATE_FRIENDS) && !defined(Q_CC_MSVC)
namespace QtPrivate {
class DummyType{};
template<>
struct MetaTypeSmartPointerHelper<DummyType,DummyType>
{
    static bool registerConverterFunction(const AbstractConverterFunction *f, int from, int to)
    {
        return QMetaType::registerConverterFunction(f, from, to);
    }
};
}
#endif

struct ContainerConverter {
public:
    static ExternalToInternalConverter getExternalToInternalConverter_QVector(const QString& typeName, const ExternalToInternalConverter& memberConverter, bool isPointer, bool isStaticType, size_t align, size_t size, int metaType);
    static ExternalToInternalConverter getExternalToInternalConverter_QStack(const QString& typeName, const ExternalToInternalConverter& memberConverter, bool isPointer, bool isStaticType, size_t align, size_t size, int metaType);
    static ExternalToInternalConverter getExternalToInternalConverter_QList(const QString& typeName, const ExternalToInternalConverter& memberConverter, bool isPointer, bool isStaticType, size_t align, size_t size, int metaType);
    static ExternalToInternalConverter getExternalToInternalConverter_QQueue(const QString& typeName, const ExternalToInternalConverter& memberConverter, bool isPointer, bool isStaticType, size_t align, size_t size, int metaType);
    static ExternalToInternalConverter getExternalToInternalConverter_QLinkedList(const QString& typeName, const ExternalToInternalConverter& memberConverter, bool isPointer, bool isStaticType, size_t align, size_t size, int metaType);
    static ExternalToInternalConverter getExternalToInternalConverter_QSet(const QString& typeName, const ExternalToInternalConverter& memberConverter, bool isPointer, bool isStaticType, size_t align, size_t size, int metaType);
    static ExternalToInternalConverter getExternalToInternalConverter_QMap(const QString& typeName, const ExternalToInternalConverter& memberConverter1, bool isPointer1, bool isStaticType1, size_t align1, size_t size1, int metaType1, const ExternalToInternalConverter& memberConverter2, bool isPointer2, bool isStaticType2, size_t align2, size_t size2, int metaType2);
    static ExternalToInternalConverter getExternalToInternalConverter_QMultiMap(const QString& typeName, const ExternalToInternalConverter& memberConverter1, bool isPointer1, bool isStaticType1, size_t align1, size_t size1, int metaType1, const ExternalToInternalConverter& memberConverter2, bool isPointer2, bool isStaticType2, size_t align2, size_t size2, int metaType2);
    static ExternalToInternalConverter getExternalToInternalConverter_QHash(const QString& typeName, const ExternalToInternalConverter& memberConverter1, bool isPointer1, bool isStaticType1, size_t align1, size_t size1, int metaType1, const ExternalToInternalConverter& memberConverter2, bool isPointer2, bool isStaticType2, size_t align2, size_t size2, int metaType2);
    static ExternalToInternalConverter getExternalToInternalConverter_QMultiHash(const QString& typeName, const ExternalToInternalConverter& memberConverter1, bool isPointer1, bool isStaticType1, size_t align1, size_t size1, int metaType1, const ExternalToInternalConverter& memberConverter2, bool isPointer2, bool isStaticType2, size_t align2, size_t size2, int metaType2);
    static ExternalToInternalConverter getExternalToInternalConverter_QPair(const QString& typeName, const ExternalToInternalConverter& memberConverter1, bool isPointer1, bool isStaticType1, size_t align1, size_t size1, int metaType1, const ExternalToInternalConverter& memberConverter2, bool isPointer2, bool isStaticType2, size_t align2, size_t size2, int metaType2);
    static ExternalToInternalConverter getExternalToInternalConverter_std_pair(const QString& typeName, const ExternalToInternalConverter& memberConverter1, bool isPointer1, bool isStaticType1, size_t align1, size_t size1, int metaType1, const ExternalToInternalConverter& memberConverter2, bool isPointer2, bool isStaticType2, size_t align2, size_t size2, int metaType2);
    static InternalToExternalConverter getInternalToExternalConverter_QVector(const QString& typeName, const InternalToExternalConverter& memberConverter, bool isPointer, bool isStaticType, size_t align, size_t size, int metaType, ContainerType containerType);
    static InternalToExternalConverter getInternalToExternalConverter_QStack(const QString& typeName, const InternalToExternalConverter& memberConverter, bool isPointer, bool isStaticType, size_t align, size_t size, int metaType, ContainerType containerType);
    static InternalToExternalConverter getInternalToExternalConverter_QList(const QString& typeName, const InternalToExternalConverter& memberConverter, bool isPointer, bool isStaticType, size_t align, size_t size, int metaType, ContainerType containerType);
    static InternalToExternalConverter getInternalToExternalConverter_QQueue(const QString& typeName, const InternalToExternalConverter& memberConverter, bool isPointer, bool isStaticType, size_t align, size_t size, int metaType, ContainerType containerType);
    static InternalToExternalConverter getInternalToExternalConverter_QLinkedList(const QString& typeName, const InternalToExternalConverter& memberConverter, bool isPointer, bool isStaticType, size_t align, size_t size, int metaType, ContainerType containerType);
    static InternalToExternalConverter getInternalToExternalConverter_QSet(const QString& typeName, const InternalToExternalConverter& memberConverter, bool isPointer, bool isStaticType, size_t align, size_t size, int metaType, ContainerType containerType);
    static InternalToExternalConverter getInternalToExternalConverter_QMap(const QString& typeName, const InternalToExternalConverter& memberConverter1, bool isPointer1, bool isStaticType1, size_t align1, size_t size1, int metaType1, const InternalToExternalConverter& memberConverter2, bool isPointer2, bool isStaticType2, size_t align2, size_t size2, int metaType2, MapType mapType);
    static InternalToExternalConverter getInternalToExternalConverter_QMultiMap(const QString& typeName, const InternalToExternalConverter& memberConverter1, bool isPointer1, bool isStaticType1, size_t align1, size_t size1, int metaType1, const InternalToExternalConverter& memberConverter2, bool isPointer2, bool isStaticType2, size_t align2, size_t size2, int metaType2, MapType mapType);
    static InternalToExternalConverter getInternalToExternalConverter_QHash(const QString& typeName, const InternalToExternalConverter& memberConverter1, bool isPointer1, bool isStaticType1, size_t align1, size_t size1, int metaType1, const InternalToExternalConverter& memberConverter2, bool isPointer2, bool isStaticType2, size_t align2, size_t size2, int metaType2, MapType mapType);
    static InternalToExternalConverter getInternalToExternalConverter_QMultiHash(const QString& typeName, const InternalToExternalConverter& memberConverter1, bool isPointer1, bool isStaticType1, size_t align1, size_t size1, int metaType1, const InternalToExternalConverter& memberConverter2, bool isPointer2, bool isStaticType2, size_t align2, size_t size2, int metaType2, MapType mapType);
    static InternalToExternalConverter getInternalToExternalConverter_QPair(const QString& typeName, const InternalToExternalConverter& memberConverter1, bool isPointer1, bool isStaticType1, size_t align1, size_t size1, int metaType1, const InternalToExternalConverter& memberConverter2, bool isPointer2, bool isStaticType2, size_t align2, size_t size2, int metaType2);
    static InternalToExternalConverter getInternalToExternalConverter_std_pair(const QString& typeName, const InternalToExternalConverter& memberConverter1, bool isPointer1, bool isStaticType1, size_t align1, size_t size1, int metaType1, const InternalToExternalConverter& memberConverter2, bool isPointer2, bool isStaticType2, size_t align2, size_t size2, int metaType2);
private:
    static QMetaType::Destructor wrapDestructor(int metaType,
                                               QHashFunction hashFunction,
                                               QLessFunction lessFunction,
                                               QEqualFunction equalFunction,
                                               uint index,
                                               QMetaType::Destructor destructor,
                                               uint hash);
    static QMetaType::Constructor wrapConstructor(int metaType,
                                                 QHashFunction hashFunction,
                                                 QLessFunction lessFunction,
                                                 QEqualFunction equalFunction,
                                                 uint index,
                                                 QMetaType::Constructor constructor,
                                                 uint hash);
    static QMetaType::Destructor wrapDestructor(int metaType1,
                                               QHashFunction hashFunction1,
                                               QLessFunction lessFunction1,
                                               QEqualFunction equalFunction1,
                                               int metaType2,
                                               QHashFunction hashFunction2,
                                               QLessFunction lessFunction2,
                                               QEqualFunction equalFunction2,
                                               QMetaType::Destructor destructor,
                                               uint hash);
    static QMetaType::Constructor wrapConstructor(int metaType1,
                                                 QHashFunction hashFunction1,
                                                 QLessFunction lessFunction1,
                                                 QEqualFunction equalFunction1,
                                                 int metaType2,
                                                 QHashFunction hashFunction2,
                                                 QLessFunction lessFunction2,
                                                 QEqualFunction equalFunction2,
                                                 QMetaType::Constructor constructor, uint hash);

    typedef uint(*HashWrapper)(const void* ptr, QHashFunction hashFunction);
    typedef uint(*HashWrapper2)(const void* ptr, QHashFunction hashFunction1, QHashFunction hashFunction2);

    static int registerContainerType(const QString& typeName, size_t containerSize, bool isPointer, int metaType,
                                      QMetaType::Constructor constructor,
                                      QMetaType::Destructor destructor,
                                      HashWrapper hashWrapper,
                                      QEqualFunction equalWrapper);
    static int registerBiContainerType(const QString& typeName, size_t containerSize,
                                       bool isPointer1, int metaType1,
                                       bool isPointer2, int metaType2,
                                       QMetaType::Constructor constructor,
                                       QMetaType::Destructor destructor,
                                       HashWrapper2 hashWrapper,
                                       QEqualFunction equalWrapper);
    typedef ConvertResponse(*MapInsertFunction)(void* containerPtr, JNIEnv* env, QtJambiScope* scope, jobject key, jobject val, const ExternalToInternalConverter& memberConverter1, const ExternalToInternalConverter& memberConverter2);

    static ExternalToInternalConverter getExternalToInternalConverter(const ExternalToInternalConverter& memberConverter1, int metaType1, size_t size1,
                                                                      const ExternalToInternalConverter& memberConverter2, int metaType2, size_t size2,
                                                                      QMetaType::Creator creator, QMetaType::Deleter deleter, MapInsertFunction accessFunction);

    static ExternalToInternalConverter getExternalToInternalConverter_QPair(const ExternalToInternalConverter& memberConverter1, int metaType1, size_t size1,
                                                                      const ExternalToInternalConverter& memberConverter2, int metaType2, size_t size2,
                                                                      QMetaType::Creator creator, QMetaType::Deleter deleter, MapInsertFunction accessFunction);

    typedef ConvertResponse(*ContainerInsertFunction)(void* containerPtr, JNIEnv* env, QtJambiScope* scope, jobject val, const ExternalToInternalConverter& memberConverter);

    static ExternalToInternalConverter getExternalToInternalConverter(const ExternalToInternalConverter& memberConverter, int metaType, size_t size,
                                                                      QMetaType::Creator creator, QMetaType::Deleter deleter, ContainerInsertFunction insertFunction);

    typedef bool (*FillJavaCollection)(JNIEnv* env, QtJambiScope* scope, const void* in, jobject out, const InternalToExternalConverter& memberConverter);

    static InternalToExternalConverter getInternalToExternalConverter(const InternalToExternalConverter& memberConverter,
                                                                      int metaType, size_t size, ContainerType containerType,
                                                                      FillJavaCollection fillJavaCollection);

    typedef void(*MapPutMethod)(JNIEnv *env, jobject map, jobject key, jobject val);
    typedef bool (*ConvertKeyValue)(JNIEnv* env, QtJambiScope* scope,
                                QList<QPair<jobject,jobject>>& elements,
                                const void* key, const void* value,
                                const InternalToExternalConverter& memberConverter1,
                                const InternalToExternalConverter& memberConverter2, bool* areComparable, int* enumType);
    typedef bool (*IterateMap)(JNIEnv* env, QtJambiScope* scope,
                                QList<QPair<jobject,jobject>>& elements,
                                jint& size,
                                bool* areComparable,
                                int* enumType,
                                const void* in,
                                const InternalToExternalConverter& memberConverter1,
                                const InternalToExternalConverter& memberConverter2,
                                ConvertKeyValue convertKeyValue);
    typedef void (*GetElements)(const void* in, const void*& first, const void*& second);

    static InternalToExternalConverter getInternalToExternalConverter(const InternalToExternalConverter& memberConverter1,
                                                                      int metaType1, size_t size1,
                                                                      const InternalToExternalConverter& memberConverter2,
                                                                      int metaType2, size_t size2,
                                                                      MapType mapType, MapPutMethod mapPutMethod, IterateMap iterateMap);
    static InternalToExternalConverter getInternalToExternalConverter_QPair(const InternalToExternalConverter& memberConverter1,
                                                                      int metaType1, size_t size1,
                                                                      const InternalToExternalConverter& memberConverter2,
                                                                      int metaType2, size_t size2,
                                                                      GetElements getElements);

    static void findFunction(QHashFunction &hashFunction, QLessFunction &lessFunction, QEqualFunction &equalFunction, bool isPointer, int metaType);

    template <typename T>
    static bool genericEqual(const void *ptr1, const void *ptr2)
    {
        const T& t1 = *reinterpret_cast<const T*>(ptr1);
        const T& t2 = *reinterpret_cast<const T*>(ptr2);
        return t1==t2;
    }

    template<typename Container>
    static uint hashWrapper(const void* ptr, QHashFunction hashFunction)
    {
        const Container& container = *reinterpret_cast<const Container*>(ptr);
        uint prime = 31;
        uint result = 1;
        result = prime * result + qHash(container.size());
        for(typename Container::const_iterator iter = container.begin();
            iter != container.end(); ++iter){
            result = prime * result + hashFunction(& (*iter));
        }
        return result;
    }

    template<typename Container, size_t size>
    static void registerContainerType(const QString& typeName, int metaType){
        QtJambiTypeManager::registerInternalSize(typeName, sizeof(Container));
        QtJambiTypeManager::registerInternalAlignment(typeName, Q_ALIGNOF(Container));
        if(QMetaType::type(qPrintable(typeName))==QMetaType::UnknownType){
            int newMetaType = registerContainerType(typeName, sizeof(Container), size==0, metaType,
                                                    qtjambiGenericConstructor<Container>,
                                                    qtjambiGenericDestructor<Container>,
                                                    hashWrapper<Container>,
                                                    genericEqual<Container>);

            struct Converter : QtPrivate::AbstractConverterFunction
            {
                explicit Converter(int metaType)
                    : QtPrivate::AbstractConverterFunction(convert),
                      m_metaType(metaType) {}
                static bool convert(const QtPrivate::AbstractConverterFunction *_this, const void *in, void *out)
                {
                    QtMetaTypePrivate::QSequentialIterableImpl *t = static_cast<QtMetaTypePrivate::QSequentialIterableImpl *>(out);
                    t->_iterable = in;
                    t->_iterator = nullptr;
                    t->_metaType_id = static_cast<const Converter *>(_this)->m_metaType;
                    t->_metaType_flags = size==0;
                    t->_iteratorCapabilities = QtMetaTypePrivate::ForwardCapability;
                    t->_size = QtMetaTypePrivate::QSequentialIterableImpl::sizeImpl<Container>;
                    t->_at = QtMetaTypePrivate::QSequentialIterableImpl::atImpl<Container>;
                    t->_moveToBegin = QtMetaTypePrivate::QSequentialIterableImpl::moveToBeginImpl<Container>;
                    t->_moveToEnd = QtMetaTypePrivate::QSequentialIterableImpl::moveToEndImpl<Container>;
                    t->_advance = QtMetaTypePrivate::IteratorOwner<typename Container::const_iterator>::advance;
                    t->_get = QtMetaTypePrivate::QSequentialIterableImpl::getImpl<Container>;
                    t->_destroyIter = QtMetaTypePrivate::IteratorOwner<typename Container::const_iterator>::destroy;
                    t->_equalIter = QtMetaTypePrivate::IteratorOwner<typename Container::const_iterator>::equal;
                    t->_copyIter = QtMetaTypePrivate::IteratorOwner<typename Container::const_iterator>::assign;
                    return true;
                }
                int m_metaType;
            };
#if !defined(Q_NO_TEMPLATE_FRIENDS) && !defined(Q_CC_MSVC)
            QtPrivate::MetaTypeSmartPointerHelper<QtPrivate::DummyType,QtPrivate::DummyType>::registerConverterFunction(new Converter(metaType), newMetaType, qMetaTypeId<QtMetaTypePrivate::QSequentialIterableImpl>());
#else
            QMetaType::registerConverterFunction(new Converter(metaType), newMetaType, qMetaTypeId<QtMetaTypePrivate::QSequentialIterableImpl>());
#endif
        }
    }

    template<typename Container>
    static uint hashWrapper2(const void* ptr, QHashFunction hashFunction1, QHashFunction hashFunction2)
    {
        const Container& container = *reinterpret_cast<const Container*>(ptr);
        uint prime = 31;
        uint result = 1;
        result = prime * result + qHash(container.size());
        for(typename Container::const_iterator iter = container.begin();
            iter != container.end(); ++iter){
            result = prime * result + hashFunction1(&iter.key());
            result = prime * result + hashFunction2(&iter.value());
        }
        return result;
    }

    template<typename Container, size_t size1, size_t size2>
    static void registerBiContainerType(const QString& typeName, int metaType1, int metaType2){
        QtJambiTypeManager::registerInternalSize(typeName, sizeof(Container));
        QtJambiTypeManager::registerInternalAlignment(typeName, Q_ALIGNOF(Container));
        if(QMetaType::type(qPrintable(typeName))==QMetaType::UnknownType){
            int newMetaType = registerBiContainerType(typeName, sizeof(Container), size1==0, metaType1, size2==0, metaType2,
                                                      qtjambiGenericConstructor<Container>,
                                                      qtjambiGenericDestructor<Container>,
                                                      hashWrapper2<Container>,
                                                      genericEqual<Container>);

            struct Converter : QtPrivate::AbstractConverterFunction
            {
                explicit Converter(int metaType_key, int metaType_value)
                    : QtPrivate::AbstractConverterFunction(convert),
                      m_metaType_key(metaType_key), m_metaType_value(metaType_value) {}
                static bool convert(const QtPrivate::AbstractConverterFunction *_this, const void *in, void *out)
                {
                    QtMetaTypePrivate::QAssociativeIterableImpl *t = static_cast<QtMetaTypePrivate::QAssociativeIterableImpl *>(out);
                    t->_iterable = in;
                    t->_iterator = nullptr;
                    t->_metaType_id_key = static_cast<const Converter *>(_this)->m_metaType_key;
                    t->_metaType_flags_key = size1==0;
                    t->_metaType_id_value = static_cast<const Converter *>(_this)->m_metaType_value;
                    t->_metaType_flags_value = size2==0;
                    t->_size = QtMetaTypePrivate::QAssociativeIterableImpl::sizeImpl<Container>;
                    t->_find = QtMetaTypePrivate::QAssociativeIterableImpl::findImpl<Container>;
                    t->_begin = QtMetaTypePrivate::QAssociativeIterableImpl::beginImpl<Container>;
                    t->_end = QtMetaTypePrivate::QAssociativeIterableImpl::endImpl<Container>;
                    t->_advance = QtMetaTypePrivate::QAssociativeIterableImpl::advanceImpl<Container>;
                    t->_getKey = QtMetaTypePrivate::QAssociativeIterableImpl::getKeyImpl<Container>;
                    t->_getValue = QtMetaTypePrivate::QAssociativeIterableImpl::getValueImpl<Container>;
                    t->_destroyIter = QtMetaTypePrivate::IteratorOwner<typename Container::const_iterator>::destroy;
                    t->_equalIter = QtMetaTypePrivate::IteratorOwner<typename Container::const_iterator>::equal;
                    t->_copyIter = QtMetaTypePrivate::IteratorOwner<typename Container::const_iterator>::assign;
                    return true;
                }
                int m_metaType_key;
                int m_metaType_value;
            };
#if !defined(Q_NO_TEMPLATE_FRIENDS) && !defined(Q_CC_MSVC)
            QtPrivate::MetaTypeSmartPointerHelper<QtPrivate::DummyType,QtPrivate::DummyType>::registerConverterFunction(new Converter(metaType1, metaType2), newMetaType, qMetaTypeId<QtMetaTypePrivate::QAssociativeIterableImpl>());
#else
            QMetaType::registerConverterFunction(new Converter(metaType1, metaType2), newMetaType, qMetaTypeId<QtMetaTypePrivate::QAssociativeIterableImpl>());
#endif
        }
    }

    template<typename Container>
    static QtMetaTypePrivate::VariantData getFirstImpl(const void * const *pair, int metaTypeId, uint flags)
    {
        return QtMetaTypePrivate::VariantData(metaTypeId, &static_cast<const Container*>(*pair)->first, flags);
    }

    template<typename Container>
    static QtMetaTypePrivate::VariantData getSecondImpl(const void * const *pair, int metaTypeId, uint flags)
    {
        return QtMetaTypePrivate::VariantData(metaTypeId, &static_cast<const Container*>(*pair)->second, flags);
    }

    template<typename Container>
    static uint pairHashWrapper(const void* ptr, QHashFunction hashFunction1, QHashFunction hashFunction2){
        const Container& container = *reinterpret_cast<const Container*>(ptr);
        uint prime = 31;
        uint result = 1;
        result = prime * result + hashFunction1(&container.first);
        result = prime * result + hashFunction2(&container.second);
        return result;
    }

    template<typename Container, size_t size1, size_t size2>
    static void registerQPairType(const QString& typeName, int metaType1, int metaType2){
        QtJambiTypeManager::registerInternalSize(typeName, sizeof(Container));
        QtJambiTypeManager::registerInternalAlignment(typeName, Q_ALIGNOF(Container));
        if(QMetaType::type(qPrintable(typeName))==QMetaType::UnknownType){
            int newMetaType = registerBiContainerType(typeName, sizeof(Container), size1==0, metaType1, size2==0, metaType2,
                                                      qtjambiGenericConstructor<Container>,
                                                      qtjambiGenericDestructor<Container>,
                                                      pairHashWrapper<Container>,
                                                      genericEqual<Container>);

            struct Converter : QtPrivate::AbstractConverterFunction
            {
                explicit Converter(int metaType_first, int metaType_second)
                    : QtPrivate::AbstractConverterFunction(convert),
                      m_metaType_first(metaType_first), m_metaType_second(metaType_second) {}
                static bool convert(const QtPrivate::AbstractConverterFunction *_this, const void *in, void *out)
                {
                    struct _QPairVariantInterfaceImpl
                    {
                        const void *_pair;
                        int _metaType_id_first;
                        uint _metaType_flags_first;
                        int _metaType_id_second;
                        uint _metaType_flags_second;
                        QtMetaTypePrivate::VariantData (*_getFirst)(const void * const *p, int metaTypeId, uint flags);
                        QtMetaTypePrivate::VariantData (*_getSecond)(const void * const *p, int metaTypeId, uint flags);
                    };

                    _QPairVariantInterfaceImpl *t = static_cast<_QPairVariantInterfaceImpl *>(out);
                    t->_pair = in;
                    t->_metaType_id_first = static_cast<const Converter *>(_this)->m_metaType_first;
                    t->_metaType_flags_first = size1==0;
                    t->_metaType_id_second = static_cast<const Converter *>(_this)->m_metaType_second;
                    t->_metaType_flags_second = size2==0;
                    t->_getFirst = getFirstImpl<Container>;
                    t->_getSecond = getSecondImpl<Container>;
                    return true;
                }
                int m_metaType_first;
                int m_metaType_second;
            };
#if !defined(Q_NO_TEMPLATE_FRIENDS) && !defined(Q_CC_MSVC)
            QtPrivate::MetaTypeSmartPointerHelper<QtPrivate::DummyType,QtPrivate::DummyType>::registerConverterFunction(new Converter(metaType1, metaType2), newMetaType, qMetaTypeId<QtMetaTypePrivate::QPairVariantInterfaceImpl>());
#else
            QMetaType::registerConverterFunction(new Converter(metaType1, metaType2), newMetaType, qMetaTypeId<QtMetaTypePrivate::QPairVariantInterfaceImpl>());
#endif
        }
    }

    template<template <typename Key,typename Value> class T, typename Key, typename Value>
    struct MapUtil{
        static constexpr MapPutMethod put = qtjambi_map_put;

        static ConvertResponse insertValue(T<Key,Value>& map, const Key& key, JNIEnv* env, QtJambiScope* scope, jobject entry_value, const ExternalToInternalConverter& memberConverter2){
            jvalue jval;
            jval.l = entry_value;
            Value value(nullptr);
            void * ptr2 = &value;
            ConvertResponse response2 = memberConverter2(env, scope, jval, ptr2, jValueType::l);
            if(response2==ConvertResponse::Successful){
                map.insert(key, value);
            }
            return response2;
        }
    };

    template<typename Key, typename Value>
    struct MapUtil<QMultiMap, Key, Value>{
        static constexpr MapPutMethod put = qtjambi_multimap_put;

        static ConvertResponse insertValue(QMultiMap<Key,Value>& map, const Key& key, JNIEnv* env, QtJambiScope* scope, jobject entry_value, const ExternalToInternalConverter& memberConverter2){
            ConvertResponse response2 = ConvertResponse::Successful;
            jobject iter = qtjambi_collection_iterator(env, entry_value);
            while(qtjambi_iterator_has_next(env, iter)){
                jvalue jval;
                jval.l = qtjambi_iterator_next(env, iter);
                Value value(nullptr);
                void * ptr2 = &value;
                response2 = memberConverter2(env, scope, jval, ptr2, jValueType::l);
                if(response2==ConvertResponse::Successful){
                    map.insert(key, value);
                }else{
                    break;
                }
            }
            return response2;
        }
    };

    template<typename Key, typename Value>
    struct MapUtil<QMultiHash, Key, Value>{
        static constexpr MapPutMethod put = qtjambi_multimap_put;

        static ConvertResponse insertValue(QMultiHash<Key,Value>& map, const Key& key, JNIEnv* env, QtJambiScope* scope, jobject entry_value, const ExternalToInternalConverter& memberConverter2){
            ConvertResponse response2 = ConvertResponse::Successful;
            jobject iter = qtjambi_collection_iterator(env, entry_value);
            while(qtjambi_iterator_has_next(env, iter)){
                jvalue jval;
                jval.l = qtjambi_iterator_next(env, iter);
                Value value(nullptr);
                void * ptr2 = &value;
                response2 = memberConverter2(env, scope, jval, ptr2, jValueType::l);
                if(response2==ConvertResponse::Successful){
                    map.insert(key, value);
                }else{
                    break;
                }
            }
            return response2;
        }
    };

    template<template <typename> class T, typename Element>
    static ConvertResponse insertIntoContainer(void* containerPtr,
                                                   JNIEnv* env, QtJambiScope* scope, jobject _val,
                                                   const ExternalToInternalConverter& memberConverter)
    {
        T<Element>& vec = *reinterpret_cast<T<Element>*>(containerPtr);
        jvalue jval;
        jval.l = _val;
        Element element(nullptr);
        void* ptr = &element;
        ConvertResponse response = memberConverter(env, scope, jval, ptr, jValueType::l);
        if(response==ConvertResponse::Successful){
            vec << std::move(element);
        }
        return response;
    }

    template<template <typename> class T, size_t align, size_t size, bool isStatic>
    static ExternalToInternalConverter getExternalToInternalConverter(const QString& typeName, const ExternalToInternalConverter& memberConverter, int metaType){
        typedef typename std::conditional<size==0, void*, ContainerElement<size, 0, isStatic, align>>::type Element;
        typedef T<Element> Container;
        registerContainerType<Container, size>(typeName, metaType);
        return getExternalToInternalConverter(memberConverter, metaType, size,
                                       qtjambiGenericCreator<Container>, qtjambiGenericDeleter<Container>,
                                       insertIntoContainer<T, Element>);
    }

    template<template <typename,typename> class T, typename Key, typename Value>
    static ConvertResponse insertKeyValue(void* containerPtr, JNIEnv* env, QtJambiScope* scope, jobject _key, jobject _val,
                                             const ExternalToInternalConverter& memberConverter1, const ExternalToInternalConverter& memberConverter2)
    {
        T<Key,Value>& map = *reinterpret_cast<T<Key,Value>*>(containerPtr);
        jvalue jval;
        jval.l = _key;
        void* ptr = scope ? nullptr : new Key(nullptr);
        ConvertResponse response = memberConverter1(env, scope, jval, ptr, jValueType::l);
        if(response==ConvertResponse::Successful){
            Q_ASSERT(ptr);
            response = MapUtil<T,Key,Value>::insertValue(map, *reinterpret_cast<Key *>(ptr), env, scope, _val, memberConverter2);
        }
        if(!scope && ptr)
            delete reinterpret_cast<Key *>(ptr);
        return response;
    }

    template<template <typename,typename> class T, size_t align1, size_t size1, bool isStatic1, size_t align2, size_t size2, bool isStatic2>
    static ExternalToInternalConverter getExternalToInternalConverter(const QString& typeName, const ExternalToInternalConverter& memberConverter1, int metaType1, const ExternalToInternalConverter& memberConverter2, int metaType2){
        typedef typename std::conditional<size1==0, void*, ContainerElement<size1, 0, isStatic1, align1>>::type Key;
        typedef typename std::conditional<size2==0, void*, ContainerElement<size2, 1, isStatic2, align2>>::type Value;
        typedef T<Key,Value> Container;
        registerBiContainerType<Container, size1, size2>(typeName, metaType1, metaType2);
        return getExternalToInternalConverter(memberConverter1, metaType1, size1,
                                       memberConverter2, metaType2, size2,
                                       qtjambiGenericCreator<Container>, qtjambiGenericDeleter<Container>, insertKeyValue<T,Key,Value>);
    }

    template<typename Container>
    static ConvertResponse fillPair(void* containerPtr, JNIEnv* env, QtJambiScope* scope, jobject _key, jobject _val,
                                             const ExternalToInternalConverter& memberConverter1, const ExternalToInternalConverter& memberConverter2)
    {
        Container& pair = *reinterpret_cast<Container*>(containerPtr);
        jvalue jval;
        jval.l = _key;
        void * ptr = &pair.first;
        ConvertResponse response = memberConverter1(env, scope, jval, ptr, jValueType::l);
        if(response==ConvertResponse::Successful){
            jval.l = _val;
            void * ptr = &pair.second;
            response = memberConverter2(env, scope, jval, ptr, jValueType::l);
        }
        return response;
    }

    template<template <typename,typename> class T, size_t align1, size_t size1, bool isStatic1, size_t align2, size_t size2, bool isStatic2>
    static ExternalToInternalConverter getExternalToInternalConverter_QPair(const QString& typeName, const ExternalToInternalConverter& memberConverter1, int metaType1, const ExternalToInternalConverter& memberConverter2, int metaType2){
        typedef typename std::conditional<size1==0, void*, ContainerElement<size1, 0, isStatic1, align1>>::type Key;
        typedef typename std::conditional<size2==0, void*, ContainerElement<size2, 1, isStatic2, align2>>::type Value;
        typedef T<Key,Value> Container;
        registerQPairType<Container, size1, size2>(typeName, metaType1, metaType2);
        return getExternalToInternalConverter_QPair(memberConverter1, metaType1, size1,
                                       memberConverter2, metaType2, size2,
                                       qtjambiGenericCreator<Container>, qtjambiGenericDeleter<Container>, fillPair<Container>);
    }

    template<typename Container>
    static bool fillJavaCollection(JNIEnv* env, QtJambiScope* scope,
                                   const void* in, jobject out,
                                   const InternalToExternalConverter& memberConverter){
        const Container& container = *reinterpret_cast<const Container*>(in);
        for(typename Container::const_iterator iter = container.begin();
            iter != container.end(); ++iter){
            jvalue val;
            val.l = nullptr;
            const void* ptr = & (*iter);
            if(!memberConverter(env, scope, ptr, &val, true)){
                return false;
            }
            qtjambi_collection_add(env, out, val.l);
        }
        return true;
    }

    template<template <typename> class T, size_t align, size_t size, bool isStatic>
    static InternalToExternalConverter getInternalToExternalConverter(const QString& typeName, const InternalToExternalConverter& memberConverter, int metaType, ContainerType containerType){
        typedef typename std::conditional<size==0, void*, ContainerElement<size, 0, isStatic, align>>::type Element;
        typedef T<Element> Container;
        registerContainerType<Container, size>(typeName, metaType);
        return getInternalToExternalConverter(memberConverter, metaType, size, containerType, fillJavaCollection<Container>);
    }

    template<typename Container>
    static bool iterateMap(JNIEnv* env, QtJambiScope* scope,
                           QList<QPair<jobject,jobject>>& elements,
                           jint& size,
                           bool* areComparable,
                           int* enumType,
                           const void* in,
                           const InternalToExternalConverter& memberConverter1,
                           const InternalToExternalConverter& memberConverter2,
                           ConvertKeyValue convertKeyValue)
    {
        const Container& container = *reinterpret_cast<const Container*>(in);
        size = jint(container.size());
        for(typename Container::const_iterator iter = container.begin();
            iter != container.end(); ++iter){
            bool result = convertKeyValue(env, scope, elements,
                            &iter.key(), &iter.value(),
                            memberConverter1, memberConverter2, areComparable, enumType);
            if(!result)
                return false;
        }
        return true;
    }

    template<template <typename,typename> class T, size_t align1, size_t size1, bool isStatic1, size_t align2, size_t size2, bool isStatic2>
    static InternalToExternalConverter getInternalToExternalConverter(const QString& typeName, const InternalToExternalConverter& memberConverter1, int metaType1, const InternalToExternalConverter& memberConverter2, int metaType2, MapType mapType){
        typedef typename std::conditional<size1==0, void*, ContainerElement<size1, 0, isStatic1, align1>>::type Key;
        typedef typename std::conditional<size2==0, void*, ContainerElement<size2, 1, isStatic2, align2>>::type Value;
        typedef T<Key,Value> Container;
        registerBiContainerType<Container, size1, size2>(typeName, metaType1, metaType2);
        return getInternalToExternalConverter(memberConverter1, metaType1, size1,
                                              memberConverter2, metaType2, size2, mapType, MapUtil<T,Key,Value>::put, iterateMap<Container>);
    }

    template<typename Container>
    static void getElements(const void* in, const void*& first, const void*& second) {
        const Container& container = *reinterpret_cast<const Container*>(in);
        first = &container.first;
        second = &container.second;
    }

    template<template <typename,typename> class T, size_t align1, size_t size1, bool isStatic1, size_t align2, size_t size2, bool isStatic2>
    static InternalToExternalConverter getInternalToExternalConverter_QPair(const QString& typeName, const InternalToExternalConverter& memberConverter1, int metaType1, const InternalToExternalConverter& memberConverter2, int metaType2){
        typedef typename std::conditional<size1==0, void*, ContainerElement<size1, 0, isStatic1, align1>>::type Key;
        typedef typename std::conditional<size2==0, void*, ContainerElement<size2, 1, isStatic2, align2>>::type Value;
        typedef T<Key,Value> Container;
        registerQPairType<Container, size1, size2>(typeName, metaType1, metaType2);
        return getInternalToExternalConverter_QPair(memberConverter1, metaType1, size1,
                                              memberConverter2, metaType2, size2, getElements<Container>);
    }
};

#endif
