#include "propertyandmethodcalltest.h"

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

JObjectWrapper PropertyAndMethodCallTest::receivedQtFlags(){
    return m_receivedQtFlags;
}

bool PropertyAndMethodCallTest::testMethodCallNumber(QObject* qobj){
    GETMETHOD_TEST(JObjectWrapper, Number)
}

bool PropertyAndMethodCallTest::testMethodCallEnum(QObject* qobj){
    GETMETHOD_TEST(JEnumWrapper, Enum)
}

bool PropertyAndMethodCallTest::testMethodCallColor(QObject* qobj){
    GETMETHOD_TEST(QColor, Color)
}

bool PropertyAndMethodCallTest::testMethodCallColorPtr(QObject* qobj){
    GETMETHOD_TEST(QColor*, ColorPtr)
}

bool PropertyAndMethodCallTest::testMethodCallQtEnum(QObject* qobj){
    GETMETHOD_TEST(Qt::AspectRatioMode, QtEnum)
}

bool PropertyAndMethodCallTest::testMethodCallDerivedQObject(QObject* qobj){
    GETMETHOD_TEST(QObject*, DerivedQObject)

}

bool PropertyAndMethodCallTest::testMethodCallCustomQtEnum(QObject* qobj){
    GETMETHOD_TEST(JEnumWrapper, CustomQtEnum)
}

bool PropertyAndMethodCallTest::testMethodCallQtFlags(QObject* qobj){
    GETMETHOD_TEST(Qt::Orientations, QtFlags)
}

bool PropertyAndMethodCallTest::testMethodCallCustomQtValue(QObject* qobj){
    GETMETHOD_TEST(QGraphicsItem*, CustomQtValue)
}

bool PropertyAndMethodCallTest::testMethodCallCustomQtInterfaceValue(QObject* qobj){
    GETMETHOD_TEST(QGraphicsItem*, CustomQtInterfaceValue)
}

bool PropertyAndMethodCallTest::testMethodCallCustomJavaType(QObject* qobj){
    GETMETHOD_TEST(JObjectWrapper, CustomJavaType)
}

bool PropertyAndMethodCallTest::testMethodCallCustomQtEnum2(QObject* qobj){
    GETMETHOD_TEST(JEnumWrapper, CustomQtEnum2)
}

bool PropertyAndMethodCallTest::testFetchPropertyNumber(QObject* qobj){
    PROPERTY_TEST(JObjectWrapper, Number)
}

bool PropertyAndMethodCallTest::testFetchPropertyEnum(QObject* qobj){
    PROPERTY_TEST(JEnumWrapper, CustomEnum)
}

bool PropertyAndMethodCallTest::testFetchPropertyColor(QObject* qobj){
    PROPERTY_TEST(QColor, Color)
}

bool PropertyAndMethodCallTest::testFetchPropertyColorPtr(QObject* qobj){
    PROPERTY_TEST(QColor*, ColorPtr)
}

bool PropertyAndMethodCallTest::testFetchPropertyQtEnum(QObject* qobj){
    PROPERTY_TEST(Qt::AspectRatioMode, QtEnum)
}

bool PropertyAndMethodCallTest::testFetchPropertyDerivedQObject(QObject* qobj){
    PROPERTY_TEST3(QObject*, DerivedQObject)
}

bool PropertyAndMethodCallTest::testFetchPropertyCustomQtEnum(QObject* qobj){
    PROPERTY_TEST(JEnumWrapper, CustomQtEnum)
}

bool PropertyAndMethodCallTest::testFetchPropertyQtFlags(QObject* qobj){
    bool result = false;
    if(qobj){
        QString name("QtFlags");
        name[0] = name[0].toLower();
        Qt::Orientations arg = qobj->property(name.toLatin1()).value<Qt::Orientations>();
        const QMetaMethod testmethod = qobj->metaObject()->method(qobj->metaObject()->indexOfMethod("testQtFlags(Qt::Orientations)"));
        testmethod.invoke(qobj, Q_RETURN_ARG(bool, result), Q_ARG(Qt::Orientations, arg));
    }
    return result;
}

bool PropertyAndMethodCallTest::testFetchPropertyCustomQtValue(QObject* qobj){
    PROPERTY_TEST3(QGraphicsItem*, CustomQtValue)
}

bool PropertyAndMethodCallTest::testFetchPropertyCustomQtInterfaceValue(QObject* qobj){
    PROPERTY_TEST3(QGraphicsItem*, CustomQtInterfaceValue)
}

bool PropertyAndMethodCallTest::testFetchPropertyCustomJavaType(QObject* qobj){
    PROPERTY_TEST(JObjectWrapper, CustomJavaType)
}

bool PropertyAndMethodCallTest::testFetchPropertyCustomQtEnum2(QObject* qobj){
    PROPERTY_TEST(JEnumWrapper, CustomQtEnum2)
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

