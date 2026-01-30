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

#include "propertyandmethodcalltest.h"
#include <QtJambi/CoreAPI>
#include <QtJambi/TestAPI>
#include <QtJambi/QtJambiAPI>

#define QTJAMBI_ARG(Type, MetaType, arg) QArgument<Type >(#MetaType, arg)
#define QTJAMBI_RETURN_ARG(Type, MetaType, arg) QReturnArgument<Type >(#MetaType, arg)
#define TEST_METHOD(Type, MetaType, Method) "test"#Method"("#MetaType")"

#if QT_VERSION >= QT_VERSION_CHECK(6, 5, 0)
#undef Q_ARG
#undef Q_RETURN_ARG
#define Q_ARG(Type, data)           QArgument<Type>(QT_STRINGIFY(Type), data)
#define Q_RETURN_ARG(Type, data)    QReturnArgument<Type>(QT_STRINGIFY(Type), data)
#endif

#define PROPERTY_TEST(Type, MetaType, Property)\
    bool result = false;\
    if(qobj && qobj->metaObject()){\
        QString name(#Property);\
        name[0] = name[0].toLower();\
        QVariant variant = qobj->property(name.toLatin1());\
        if(variant.metaType()==QMetaType::fromName(#MetaType)){\
            const QMetaMethod testmethod = qobj->metaObject()->method(qobj->metaObject()->indexOfMethod(TEST_METHOD(Type, MetaType, Property)));\
            if(testmethod.isValid())\
                testmethod.invoke(qobj, Qt::AutoConnection, Q_RETURN_ARG(bool, result), QGenericArgument(#MetaType, variant.constData()));\
        }\
    }\
    return result;

#define GETMETHOD_TEST(Type, MetaType, Method)\
    bool result = false;\
    if(qobj){\
    QVariant variant(QMetaType::fromName(#MetaType));\
    const QMetaMethod qmethod = qobj->metaObject()->method(qobj->metaObject()->indexOfMethod("get"#Method"()"));\
    const QMetaMethod testmethod = qobj->metaObject()->method(qobj->metaObject()->indexOfMethod(TEST_METHOD(Type, MetaType, Method)));\
    if(qmethod.isValid())\
        qmethod.invoke(qobj, QGenericReturnArgument(#MetaType, variant.data()));\
    if(testmethod.isValid())\
        testmethod.invoke(qobj, Qt::AutoConnection, Q_RETURN_ARG(bool, result), QGenericArgument(#MetaType, variant.constData()));\
    }\
    return result;

#define PTR_PROPERTY_TEST(Type, MetaType, Property)\
    bool result = false;\
    const QMetaObject* metaobject = qobj->metaObject();\
    if(qobj && metaobject){\
        QString name(#Property);\
        name[0] = name[0].toLower();\
        QVariant variant = qobj->property(name.toLatin1());\
        void** data = reinterpret_cast<void**>(variant.data());\
        if(data){\
            Type arg = reinterpret_cast<Type>(*data);\
            const QMetaMethod testmethod = metaobject->method(metaobject->indexOfMethod(TEST_METHOD(Type, MetaType, Property)));\
            if(testmethod.isValid())\
                testmethod.invoke(qobj, Qt::AutoConnection, Q_RETURN_ARG(bool, result), QTJAMBI_ARG(Type, MetaType, arg));\
        }\
    }\
    return result;

PropertyAndMethodCallTest::PropertyAndMethodCallTest(QObject *parent) :
    QObject(parent),
    m_receivedEnum(),
    m_receivedColor(),
    m_receivedColorPtr(nullptr),
    m_receivedQtEnum(),
    m_receivedQtFlags(),
    m_receivedList(),
    m_receivedNumber(),
    m_receivedCustomQtValue(nullptr),
    m_receivedCustomQtInterfaceValue(nullptr),
    m_receivedCustomJavaType(),
    m_receivedDerivedQObject(nullptr)
{
}

bool PropertyAndMethodCallTest::connectSignals(QObject* sender, bool useAnnotatedType){
    bool connected = true;
    connected &= bool(QtJambiAPI::connect(sender, SIGNAL(customEnumChanged(io::qt::autotests::TestPropertyAndMethodCall::TestQObject::CustomEnum)), this, SLOT(receiveCustomEnum(JObjectWrapper))));
    connected &= bool(QtJambiAPI::connect(sender, SIGNAL(customQtEnumChanged(io::qt::autotests::TestPropertyAndMethodCall::TestQObject::CustomQtEnum)), this, SLOT(receiveCustomQtEnum(JObjectWrapper))));
    connected &= bool(QtJambiAPI::connect(sender, SIGNAL(customQtFlagsChanged(io::qt::autotests::TestPropertyAndMethodCall::TestQObject::CustomQtFlags)), this, SLOT(receiveQtFlags(JObjectWrapper))));
    connected &= bool(QObject::connect(sender, SIGNAL(customColorChanged(QColor)), this, SLOT(receiveColor(QColor))));
    if(useAnnotatedType)
        connected &= bool(QObject::connect(sender, SIGNAL(customColorPtrChanged(QColor*)), this, SLOT(receiveColorPtr(QColor*))));
    connected &= bool(QtJambiAPI::connect(sender, SIGNAL(customQtValueChanged(io::qt::autotests::TestPropertyAndMethodCall::TestQObject::CustomQtValue*)), this, SLOT(receiveCustomQtValue(QGraphicsItem*))));
    connected &= bool(QtJambiAPI::connect(sender, SIGNAL(customQtInterfaceValueChanged(io::qt::autotests::TestPropertyAndMethodCall::TestQObject::CustomQtInterfaceValue*)), this, SLOT(receiveCustomQtInterfaceValue(QGraphicsItem*))));
    connected &= bool(QtJambiAPI::connect(sender, SIGNAL(customJavaTypeChanged(JObjectWrapper<io::qt::autotests::TestPropertyAndMethodCall::TestQObject::CustomJavaType>)), this, SLOT(receiveCustomJavaType(JObjectWrapper))));
    connected &= bool(QtJambiAPI::connect(sender, SIGNAL(derivedQObjectChanged(io::qt::autotests::TestPropertyAndMethodCall::TestQObject::DerivedQObject*)), this, SLOT(receiveDerivedQObject(QObject*))));
    connected &= bool(QtJambiAPI::connect(sender, SIGNAL(extendedColorChanged(io::qt::autotests::TestPropertyAndMethodCall::TestQObject::ExtendedColor)), this, SLOT(receiveExtendedColor(JObjectWrapper))));
    return connected;
}

void PropertyAndMethodCallTest::receiveCustomEnum(JObjectWrapper value){
    m_receivedEnum = value;
}

void PropertyAndMethodCallTest::receiveColor(QColor value){
    m_receivedColor = value;
}

void PropertyAndMethodCallTest::receiveColorPtr(QColor* value){
    m_receivedColorPtr = value;
}

void PropertyAndMethodCallTest::receiveCustomQtEnum(JObjectWrapper value){
    m_receivedQtEnum = value;
}

void PropertyAndMethodCallTest::receiveQtFlags(JObjectWrapper value){
    m_receivedQtFlags = value;
}

void PropertyAndMethodCallTest::receiveList(JObjectWrapper value){
    m_receivedList = value;
}

void PropertyAndMethodCallTest::receiveNumber(JObjectWrapper value){
    m_receivedNumber = value;
}

void PropertyAndMethodCallTest::receiveCustomQtValue(QGraphicsItem* value){
    m_receivedCustomQtValue = value;
}

void PropertyAndMethodCallTest::receiveCustomQtInterfaceValue(QGraphicsItem* value){
    m_receivedCustomQtInterfaceValue = value;
}

void PropertyAndMethodCallTest::receiveCustomJavaType(JObjectWrapper value){
    m_receivedCustomJavaType = value;
}

void PropertyAndMethodCallTest::receiveExtendedColor(JObjectWrapper value){
    m_receivedExtendedColor = value;
}

void PropertyAndMethodCallTest::receiveDerivedQObject(QObject* value){
    m_receivedDerivedQObject = value;
}

JObjectWrapper PropertyAndMethodCallTest::receivedCustomEnum(){
    return m_receivedEnum;
}

QColor PropertyAndMethodCallTest::receivedColor(){
    return m_receivedColor;
}

QColor* PropertyAndMethodCallTest::receivedColorPtr(){
    return m_receivedColorPtr;
}

JObjectWrapper PropertyAndMethodCallTest::receivedCustomQtEnum(){
    return m_receivedQtEnum;
}

JObjectWrapper PropertyAndMethodCallTest::receivedList(){
    return m_receivedList;
}

JObjectWrapper PropertyAndMethodCallTest::receivedNumber(){
    return m_receivedNumber;
}

QGraphicsItem* PropertyAndMethodCallTest::receivedCustomQtValue(){
    return m_receivedCustomQtValue;
}

QGraphicsItem* PropertyAndMethodCallTest::receivedCustomQtInterfaceValue(){
    return m_receivedCustomQtInterfaceValue;
}

JObjectWrapper PropertyAndMethodCallTest::receivedCustomJavaType(){
    return m_receivedCustomJavaType;
}

QObject* PropertyAndMethodCallTest::receivedDerivedQObject(){
    return m_receivedDerivedQObject;
}

JObjectWrapper PropertyAndMethodCallTest::receivedExtendedColor(){
    return m_receivedExtendedColor;
}

JObjectWrapper PropertyAndMethodCallTest::receivedQtFlags(){
    return m_receivedQtFlags;
}

bool PropertyAndMethodCallTest::testMethodCallNumber(QObject* qobj){
    GETMETHOD_TEST(JObjectWrapper, JObjectWrapper<java::lang::Number>, Number)
}

bool PropertyAndMethodCallTest::testMethodCallEnum(QObject* qobj){
    GETMETHOD_TEST(int, io::qt::autotests::TestPropertyAndMethodCall::TestQObject::CustomEnum, Enum)
}

bool PropertyAndMethodCallTest::testMethodCallColor(QObject* qobj){
    GETMETHOD_TEST(QColor, QColor, Color)
}

bool PropertyAndMethodCallTest::testMethodCallColorPtr(QObject* qobj){
    GETMETHOD_TEST(QColor*, QColor*, ColorPtr)
}

bool PropertyAndMethodCallTest::testMethodCallQtEnum(QObject* qobj){
    GETMETHOD_TEST(Qt::AspectRatioMode, Qt::AspectRatioMode, QtEnum)
}

bool PropertyAndMethodCallTest::testMethodCallDerivedQObject(QObject* qobj){
    GETMETHOD_TEST(QObject*, io::qt::autotests::TestPropertyAndMethodCall::TestQObject::DerivedQObject*, DerivedQObject)

}

bool PropertyAndMethodCallTest::testMethodCallCustomQtEnum(QObject* qobj){
    GETMETHOD_TEST(int, io::qt::autotests::TestPropertyAndMethodCall::TestQObject::CustomQtEnum, CustomQtEnum)
}

bool PropertyAndMethodCallTest::testMethodCallQtFlags(QObject* qobj){
    GETMETHOD_TEST(Qt::Orientations, QFlags<Qt::Orientation>, QtFlags)
}

bool PropertyAndMethodCallTest::testMethodCallCustomQtValue(QObject* qobj){
    GETMETHOD_TEST(QGraphicsItem*, io::qt::autotests::TestPropertyAndMethodCall::TestQObject::CustomQtValue*, CustomQtValue)
}

bool PropertyAndMethodCallTest::testMethodCallCustomQtInterfaceValue(QObject* qobj){
    GETMETHOD_TEST(QGraphicsItem*, io::qt::autotests::TestPropertyAndMethodCall::TestQObject::CustomQtInterfaceValue*, CustomQtInterfaceValue)
}

bool PropertyAndMethodCallTest::testMethodCallCustomJavaType(QObject* qobj){
    GETMETHOD_TEST(JObjectWrapper, JObjectWrapper<io::qt::autotests::TestPropertyAndMethodCall::TestQObject::CustomJavaType>, CustomJavaType)
}

bool PropertyAndMethodCallTest::testMethodCallCustomQtEnum2(QObject* qobj){
    GETMETHOD_TEST(int, io::qt::autotests::TestPropertyAndMethodCall::TestQObject::CustomQtEnum, CustomQtEnum2)
}

bool PropertyAndMethodCallTest::testMethodCallExtendedColor(QObject* qobj){
    GETMETHOD_TEST(JObjectWrapper, io::qt::autotests::TestPropertyAndMethodCall::TestQObject::ExtendedColor, ExtendedColor)
}

bool PropertyAndMethodCallTest::testFetchPropertyNumber(QObject* qobj){
    PROPERTY_TEST(JObjectWrapper, JObjectWrapper<java::lang::Number>, Number)
}

bool PropertyAndMethodCallTest::testFetchPropertyEnum(QObject* qobj){
    PROPERTY_TEST(int, io::qt::autotests::TestPropertyAndMethodCall::TestQObject::CustomEnum, CustomEnum)
}

bool PropertyAndMethodCallTest::testFetchPropertyColor(QObject* qobj){
    PROPERTY_TEST(QColor, QColor, Color)
}

bool PropertyAndMethodCallTest::testFetchPropertyColorPtr(QObject* qobj){
    PROPERTY_TEST(QColor*, QColor*, ColorPtr)
}

bool PropertyAndMethodCallTest::testFetchPropertyQtEnum(QObject* qobj){
    PROPERTY_TEST(Qt::AspectRatioMode, Qt::AspectRatioMode, QtEnum)
}

bool PropertyAndMethodCallTest::testFetchPropertyDerivedQObject(QObject* qobj){
    PROPERTY_TEST(QObject*, io::qt::autotests::TestPropertyAndMethodCall::TestQObject::DerivedQObject*, DerivedQObject)
}

bool PropertyAndMethodCallTest::testFetchPropertyCustomQtEnum(QObject* qobj){
    PROPERTY_TEST(int, io::qt::autotests::TestPropertyAndMethodCall::TestQObject::CustomQtEnum, CustomQtEnum)
}

bool PropertyAndMethodCallTest::testFetchPropertyQtFlags(QObject* qobj){
    bool result = false;
    if(qobj){
        QString name("QtFlags");
        name[0] = name[0].toLower();
        Qt::Orientations arg = qobj->property(name.toLatin1()).value<Qt::Orientations>();
        const QMetaMethod testmethod = qobj->metaObject()->method(qobj->metaObject()->indexOfMethod("testQtFlags(Qt::Orientations)"));
        testmethod.invoke(qobj, Q_RETURN_ARG(bool, result), Q_ARG(QFlags<Qt::Orientation>, arg));
    }
    return result;
}

bool PropertyAndMethodCallTest::testFetchPropertyCustomQtValue(QObject* qobj){
    PROPERTY_TEST(QGraphicsItem*, io::qt::autotests::TestPropertyAndMethodCall::TestQObject::CustomQtValue*, CustomQtValue)
}

bool PropertyAndMethodCallTest::testFetchPropertyExtendedColor(QObject* qobj){
    PROPERTY_TEST(JObjectWrapper, io::qt::autotests::TestPropertyAndMethodCall::TestQObject::ExtendedColor, ExtendedColor)
}

bool PropertyAndMethodCallTest::testFetchPropertyCustomQtInterfaceValue(QObject* qobj){
    PROPERTY_TEST(QGraphicsItem*, io::qt::autotests::TestPropertyAndMethodCall::TestQObject::CustomQtInterfaceValue*, CustomQtInterfaceValue)
}

bool PropertyAndMethodCallTest::testFetchPropertyCustomJavaType(QObject* qobj){
    PROPERTY_TEST(JObjectWrapper, JObjectWrapper<io::qt::autotests::TestPropertyAndMethodCall::TestQObject::CustomJavaType>, CustomJavaType)
}

bool PropertyAndMethodCallTest::testFetchPropertyCustomQtEnum2(QObject* qobj){
    PROPERTY_TEST(int, io::qt::autotests::TestPropertyAndMethodCall::TestQObject::CustomQtEnum, CustomQtEnum2)
}

#if QT_VERSION >= QT_VERSION_CHECK(6, 6, 0)
QVariant PropertyAndMethodCallTest::instantiateInPlace(const QMetaMethod& constructor, std::initializer_list<QVariant> args){
    if(JniEnvironment env{128}){
        const QMetaObject* metaObject = constructor.enclosingMetaObject();
        QMetaType metaType = metaObject->metaType();
        if(constructor.parameterCount()!=int(args.size()))
            Java::Runtime::IllegalArgumentException::throwNew(env, "Mismatching number of arguments." QTJAMBI_STACKTRACEINFO);
        int index = -1;
        for(int i=0; i<metaObject->constructorCount(); ++i){
            if(metaObject->constructor(i)==constructor){
                index = i;
                break;
            }
        }
        if(index>=0){
            QList<QVariant> _args(args);
            QScopedArrayPointer<void*> a(new void*[_args.size()+1]);
            for(int i=0, l=_args.size(); i<l; ++i){
                _args[i].convert(constructor.parameterMetaType(i));
                a[i+1] = _args[i].data();
            }
            if(!metaType.isValid() || (metaType.flags() & QMetaType::IsPointer)){
                QPair<size_t,size_t> sizeAlign = TestAPI::sizeAndAlignOf(env, metaObject);
                if(sizeAlign.first>0){
                    if(metaObject->inherits(&QObject::staticMetaObject)){
                        void* placement;
                        if (sizeAlign.second > __STDCPP_DEFAULT_NEW_ALIGNMENT__) {
                            placement = operator new(sizeAlign.first, std::align_val_t(sizeAlign.second));
                        }else{
                            placement = operator new(sizeAlign.first);
                        }
                        memset(placement, 0, sizeAlign.first);
                        a[0] = placement;
                        QObject* ptr = reinterpret_cast<QObject*>(a[0]);
                        if(metaObject->static_metacall(QMetaObject::ConstructInPlace, index, a.data())>=0){
                            ptr = nullptr;
                            if (sizeAlign.second > __STDCPP_DEFAULT_NEW_ALIGNMENT__) {
#ifdef __cpp_sized_deallocation
                                operator delete(placement, sizeAlign.first, std::align_val_t(sizeAlign.second));
#else
                                operator delete(placement, std::align_val_t(sizeAlign.second));
#endif
                            } else {
#ifdef __cpp_sized_deallocation
                                operator delete(placement, sizeAlign.first);
#else
                                operator delete(placement);
#endif
                            }
                        }
                        return QVariant::fromValue(ptr);
                    }else if(Java::QtJambi::QtObjectInterface::isAssignableFrom(env, CoreAPI::getMetaObjectJavaType(env, metaObject))){
                        void* placement;
                        if (sizeAlign.second > __STDCPP_DEFAULT_NEW_ALIGNMENT__) {
                            placement = operator new(sizeAlign.first, std::align_val_t(sizeAlign.second));
                        }else{
                            placement = operator new(sizeAlign.first);
                        }
                        memset(placement, 0, sizeAlign.first);
                        a[0] = placement;
                        if(metaObject->static_metacall(QMetaObject::ConstructInPlace, index, a.data())<0){
                            jobject obj = QtJambiAPI::findObject(env, placement);
                            return QVariant::fromValue(JObjectWrapper(env, obj));
                        }else{
                            if (sizeAlign.second > __STDCPP_DEFAULT_NEW_ALIGNMENT__) {
#ifdef __cpp_sized_deallocation
                                operator delete(placement, sizeAlign.first, std::align_val_t(sizeAlign.second));
#else
                                operator delete(placement, std::align_val_t(sizeAlign.second));
#endif
                            } else {
#ifdef __cpp_sized_deallocation
                                operator delete(placement, sizeAlign.first);
#else
                                operator delete(placement);
#endif
                            }
                        }
                    }
                }
            }else{
                QVariant result(metaType, nullptr);
                a[0] = result.data();
                metaType.destruct(a[0]);
                if(metaObject->static_metacall(QMetaObject::ConstructInPlace, index, a.data())<0){
                    return result;
                }
            }
        }
    }
    return QVariant{};
}
#endif

void PropertyAndMethodCallTest::dumpMetaObject(const QMetaObject* metaObject){
    if(metaObject){
        printf("class %s{\n", qPrintable(metaObject->className()));
        printf("\tProperty Count: %d\n", metaObject->propertyCount());
        for(int i=0; i<metaObject->propertyCount(); i++){
            QMetaProperty property = metaObject->property(i);
            printf("\t\t%s %s\n", qPrintable(property.typeName()), qPrintable(property.name()));
        }

        printf("\tMethod Count: %d\n", metaObject->methodCount());
        for(int i=0; i<metaObject->methodCount(); i++){
            QMetaMethod method = metaObject->method(i);
            printf("\t\t%s %s\n", qPrintable(method.typeName()), qPrintable(method.methodSignature()));
        }

        printf("\tConstructor Count: %d\n", metaObject->constructorCount());
        for(int i=0; i<metaObject->constructorCount(); i++){
            QMetaMethod method = metaObject->constructor(i);
            printf("\t\t%s\n", qPrintable(method.methodSignature()));
        }

        printf("\tEnumerator Count: %d\n", metaObject->enumeratorCount());
        for(int i=0; i<metaObject->enumeratorCount(); i++){
            QMetaEnum enumerator = metaObject->enumerator(i);
            printf("\t\t%s::%s{\n", qPrintable(enumerator.scope()), qPrintable(enumerator.name()));
            for(int j=0; j<enumerator.keyCount(); j++){
                printf("\t\t\t%s = 0x%0x;\n", qPrintable(enumerator.key(j)), enumerator.value(j));
            }
            printf("\t\t}\n");
        }
        printf("}\n");
    }
}

