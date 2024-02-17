#include "propertyandmethodcalltest.h"

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
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
#else
#define QTJAMBI_ARG(Type, MetaType, arg) Q_ARG(Type, arg)
#define QTJAMBI_RETURN_ARG(Type, MetaType, arg) Q_RETURN_ARG(Type, arg)
#define TEST_METHOD(Type, MetaType, Method) "test"#Method"("#Type")"
#define PROPERTY_TEST(Type, MetaType, Property)\
    bool result = false;\
    if(qobj && qobj->metaObject()){\
        QString name(#Property);\
        name[0] = name[0].toLower();\
        QVariant pValue = qobj->property(name.toLatin1());\
        pValue.convert(QMetaType::fromType<Type>().id());\
        Type arg = pValue.value<Type>();\
        const QMetaMethod testmethod = qobj->metaObject()->method(qobj->metaObject()->indexOfMethod(TEST_METHOD(Type, MetaType, Property)));\
        if(testmethod.isValid())\
            testmethod.invoke(qobj, Qt::AutoConnection, Q_RETURN_ARG(bool, result), Q_ARG(Type, arg));\
    }\
    return result;

#define GETMETHOD_TEST(Type, MetaType, Method)\
    bool result = false;\
    if(qobj){\
    Type arg;\
    const QMetaMethod qmethod = qobj->metaObject()->method(qobj->metaObject()->indexOfMethod("get"#Method"()"));\
    const QMetaMethod testmethod = qobj->metaObject()->method(qobj->metaObject()->indexOfMethod(TEST_METHOD(Type, MetaType, Method)));\
    if(qmethod.isValid())\
        qmethod.invoke(qobj, Qt::AutoConnection, QTJAMBI_RETURN_ARG(Type, MetaType, arg));\
    if(testmethod.isValid())\
        testmethod.invoke(qobj, Qt::AutoConnection, Q_RETURN_ARG(bool, result), QTJAMBI_ARG(Type, MetaType, arg));\
    }\
    return result;

#endif

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
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    connected &= bool(QtJambiAPI::connect(sender, SIGNAL(customEnumChanged(io::qt::autotests::TestPropertyAndMethodCall::TestQObject::CustomEnum)), this, SLOT(receiveCustomEnum(JEnumWrapper))));
    connected &= bool(QtJambiAPI::connect(sender, SIGNAL(customQtEnumChanged(io::qt::autotests::TestPropertyAndMethodCall::TestQObject::CustomQtEnum)), this, SLOT(receiveCustomQtEnum(JEnumWrapper))));
    connected &= bool(QtJambiAPI::connect(sender, SIGNAL(customQtFlagsChanged(io::qt::autotests::TestPropertyAndMethodCall::TestQObject::CustomQtFlags)), this, SLOT(receiveQtFlags(JObjectWrapper))));
    connected &= bool(QObject::connect(sender, SIGNAL(customColorChanged(QColor)), this, SLOT(receiveColor(QColor))));
    if(useAnnotatedType)
        connected &= bool(QObject::connect(sender, SIGNAL(customColorPtrChanged(QColor*)), this, SLOT(receiveColorPtr(QColor*))));
    connected &= bool(QtJambiAPI::connect(sender, SIGNAL(customQtValueChanged(io::qt::autotests::TestPropertyAndMethodCall::TestQObject::CustomQtValue*)), this, SLOT(receiveCustomQtValue(QGraphicsItem*))));
    connected &= bool(QtJambiAPI::connect(sender, SIGNAL(customQtInterfaceValueChanged(io::qt::autotests::TestPropertyAndMethodCall::TestQObject::CustomQtInterfaceValue*)), this, SLOT(receiveCustomQtInterfaceValue(QGraphicsItem*))));
    connected &= bool(QtJambiAPI::connect(sender, SIGNAL(customJavaTypeChanged(JObjectWrapper<io::qt::autotests::TestPropertyAndMethodCall::TestQObject::CustomJavaType>)), this, SLOT(receiveCustomJavaType(JObjectWrapper))));
    connected &= bool(QtJambiAPI::connect(sender, SIGNAL(derivedQObjectChanged(io::qt::autotests::TestPropertyAndMethodCall::TestQObject::DerivedQObject*)), this, SLOT(receiveDerivedQObject(QObject*))));
    connected &= bool(QtJambiAPI::connect(sender, SIGNAL(extendedColorChanged(io::qt::autotests::TestPropertyAndMethodCall::TestQObject::ExtendedColor)), this, SLOT(receiveExtendedColor(JObjectWrapper))));
#else
    connected &= bool(QObject::connect(sender, SIGNAL(customEnumChanged(JEnumWrapper)), this, SLOT(receiveCustomEnum(JEnumWrapper))));
    connected &= bool(QObject::connect(sender, SIGNAL(customQtEnumChanged(JEnumWrapper)), this, SLOT(receiveCustomQtEnum(JEnumWrapper))));
    connected &= bool(QObject::connect(sender, SIGNAL(customQtFlagsChanged(JObjectWrapper)), this, SLOT(receiveQtFlags(JObjectWrapper))));
    connected &= bool(QObject::connect(sender, SIGNAL(customColorChanged(QColor)), this, SLOT(receiveColor(QColor))));
    if(useAnnotatedType)
        connected &= bool(QObject::connect(sender, SIGNAL(customColorPtrChanged(QColor*)), this, SLOT(receiveColorPtr(QColor*))));
    connected &= bool(QObject::connect(sender, SIGNAL(customQtValueChanged(QGraphicsItem*)), this, SLOT(receiveCustomQtValue(QGraphicsItem*))));
    connected &= bool(QObject::connect(sender, SIGNAL(customQtInterfaceValueChanged(QGraphicsItem*)), this, SLOT(receiveCustomQtInterfaceValue(QGraphicsItem*))));
    connected &= bool(QObject::connect(sender, SIGNAL(customJavaTypeChanged(JObjectWrapper)), this, SLOT(receiveCustomJavaType(JObjectWrapper))));
    connected &= bool(QObject::connect(sender, SIGNAL(derivedQObjectChanged(QObject*)), this, SLOT(receiveDerivedQObject(QObject*))));
    //connected &= bool(QObject::connect(sender, SIGNAL(extendedColorChanged(QColor)), this, SLOT(receiveExtendedColor(JObjectWrapper))));
#endif
    return connected;
}

void PropertyAndMethodCallTest::receiveCustomEnum(JEnumWrapper value){
    m_receivedEnum = value;
}

void PropertyAndMethodCallTest::receiveColor(QColor value){
    m_receivedColor = value;
}

void PropertyAndMethodCallTest::receiveColorPtr(QColor* value){
    m_receivedColorPtr = value;
}

void PropertyAndMethodCallTest::receiveCustomQtEnum(JEnumWrapper value){
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

JEnumWrapper PropertyAndMethodCallTest::receivedCustomEnum(){
    return m_receivedEnum;
}

QColor PropertyAndMethodCallTest::receivedColor(){
    return m_receivedColor;
}

QColor* PropertyAndMethodCallTest::receivedColorPtr(){
    return m_receivedColorPtr;
}

JEnumWrapper PropertyAndMethodCallTest::receivedCustomQtEnum(){
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
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    GETMETHOD_TEST(int, io::qt::autotests::TestPropertyAndMethodCall::TestQObject::CustomEnum, Enum)
#else
    GETMETHOD_TEST(JEnumWrapper, io::qt::autotests::TestPropertyAndMethodCall::TestQObject::CustomEnum, Enum)
#endif
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
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    GETMETHOD_TEST(int, io::qt::autotests::TestPropertyAndMethodCall::TestQObject::CustomQtEnum, CustomQtEnum)
#else
    GETMETHOD_TEST(JEnumWrapper, io::qt::autotests::TestPropertyAndMethodCall::TestQObject::CustomQtEnum, CustomQtEnum)
#endif
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
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    GETMETHOD_TEST(int, io::qt::autotests::TestPropertyAndMethodCall::TestQObject::CustomQtEnum, CustomQtEnum2)
#else
    GETMETHOD_TEST(JEnumWrapper, io::qt::autotests::TestPropertyAndMethodCall::TestQObject::CustomQtEnum, CustomQtEnum2)
#endif
}

bool PropertyAndMethodCallTest::testMethodCallExtendedColor(QObject* qobj){
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    GETMETHOD_TEST(JObjectWrapper, io::qt::autotests::TestPropertyAndMethodCall::TestQObject::ExtendedColor, ExtendedColor)
#else
    GETMETHOD_TEST(QColor, io::qt::autotests::TestPropertyAndMethodCall::TestQObject::ExtendedColor, ExtendedColor)
#endif
}

bool PropertyAndMethodCallTest::testFetchPropertyNumber(QObject* qobj){
    PROPERTY_TEST(JObjectWrapper, JObjectWrapper<java::lang::Number>, Number)
}

bool PropertyAndMethodCallTest::testFetchPropertyEnum(QObject* qobj){
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    PROPERTY_TEST(int, io::qt::autotests::TestPropertyAndMethodCall::TestQObject::CustomEnum, CustomEnum)
#else
    PROPERTY_TEST(JEnumWrapper, io::qt::autotests::TestPropertyAndMethodCall::TestQObject::CustomEnum, CustomEnum)
#endif
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
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    PROPERTY_TEST(int, io::qt::autotests::TestPropertyAndMethodCall::TestQObject::CustomQtEnum, CustomQtEnum)
#else
    PROPERTY_TEST(JEnumWrapper, io::qt::autotests::TestPropertyAndMethodCall::TestQObject::CustomQtEnum, CustomQtEnum)
#endif
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
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    PROPERTY_TEST(JObjectWrapper, io::qt::autotests::TestPropertyAndMethodCall::TestQObject::ExtendedColor, ExtendedColor)
#else
    PROPERTY_TEST(QColor, io::qt::autotests::TestPropertyAndMethodCall::TestQObject::ExtendedColor, ExtendedColor)
#endif
}

bool PropertyAndMethodCallTest::testFetchPropertyCustomQtInterfaceValue(QObject* qobj){
    PROPERTY_TEST(QGraphicsItem*, io::qt::autotests::TestPropertyAndMethodCall::TestQObject::CustomQtInterfaceValue*, CustomQtInterfaceValue)
}

bool PropertyAndMethodCallTest::testFetchPropertyCustomJavaType(QObject* qobj){
    PROPERTY_TEST(JObjectWrapper, JObjectWrapper<io::qt::autotests::TestPropertyAndMethodCall::TestQObject::CustomJavaType>, CustomJavaType)
}

bool PropertyAndMethodCallTest::testFetchPropertyCustomQtEnum2(QObject* qobj){
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    PROPERTY_TEST(int, io::qt::autotests::TestPropertyAndMethodCall::TestQObject::CustomQtEnum, CustomQtEnum2)
#else
    PROPERTY_TEST(JEnumWrapper, io::qt::autotests::TestPropertyAndMethodCall::TestQObject::CustomQtEnum, CustomQtEnum2)
#endif
}

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

