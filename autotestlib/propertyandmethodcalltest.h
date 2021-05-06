#ifndef PROPERTYANDMETHODCALLTEST_H
#define PROPERTYANDMETHODCALLTEST_H

#ifndef QT_JAMBI_RUN
#include <qtjambi/qtjambi_global.h>
#endif
#include <QtCore/QtCore>
#include <QtGui/QtGui>
#ifndef QTJAMBI_NO_WIDGETS
#include <QtWidgets/QtWidgets>
#endif
#ifndef QT_JAMBI_RUN
#include <qtjambi/qtjambi_core.h>
#include <qtjambi/qtjambi_jobjectwrapper.h>
#endif

#define GETMETHOD_TEST(Type, Method)\
    bool result = false;\
    if(qobj){\
    Type arg;\
    const QMetaMethod qmethod = qobj->metaObject()->method(qobj->metaObject()->indexOfMethod("get"#Method"()"));\
    const QMetaMethod testmethod = qobj->metaObject()->method(qobj->metaObject()->indexOfMethod("test"#Method"("#Type")"));\
    qmethod.invoke(qobj, Q_RETURN_ARG(Type, arg));\
    if(testmethod.isValid())\
    testmethod.invoke(qobj, Q_RETURN_ARG(bool, result), Q_ARG(Type, arg));\
    }\
    return result;

#define PROPERTY_TEST(Type, Property)\
    bool result = false;\
    if(qobj && qobj->metaObject()){\
    QString name(#Property);\
    name[0] = name[0].toLower();\
    Type arg = qobj->property(name.toLatin1()).value<Type>();\
    const QMetaMethod testmethod = qobj->metaObject()->method(qobj->metaObject()->indexOfMethod("test"#Property"("#Type")"));\
    if(testmethod.isValid())\
    testmethod.invoke(qobj, Q_RETURN_ARG(bool, result), Q_ARG(Type, arg));\
    }\
    return result;

#define PROPERTY_TEST2(Type, Property)\
    bool result = false;\
    if(qobj && qobj->metaObject()){\
    QString name(#Property);\
    name[0] = name[0].toLower();\
    Type arg = qobj->property(name.toLatin1()).value<Type>();\
    const QMetaMethod testmethod = qobj->metaObject()->method(qobj->metaObject()->indexOfMethod("test"#Property"("#Type")"));\
    if(testmethod.isValid())\
    testmethod.invoke(qobj, Q_RETURN_ARG(bool, result), Q_ARG(Type, arg));\
    }\
    return result;

#define PROPERTY_TEST3(Type, Property)\
    bool result = false;\
    const QMetaObject* metaobject = qobj->metaObject();\
    if(qobj && metaobject){\
        QString name(#Property);\
        name[0] = name[0].toLower();\
        QVariant variant = qobj->property(name.toLatin1());\
        void** data = reinterpret_cast<void**>(variant.data());\
        if(data){\
            Type arg = reinterpret_cast<Type>(*data);\
            const QMetaMethod testmethod = metaobject->method(metaobject->indexOfMethod("test"#Property"("#Type")"));\
            if(testmethod.isValid())\
            testmethod.invoke(qobj, Q_RETURN_ARG(bool, result), Q_ARG(Type, arg));\
        }\
    }\
    return result;

class PropertyAndMethodCallTest : public QObject
{
    Q_OBJECT
public:
    explicit PropertyAndMethodCallTest(QObject *parent = nullptr);
    
    bool connectSignals(QObject* sender);
    static void dumpMetaObject(const QMetaObject* metaObject);

    JEnumWrapper receivedCustomEnum();
    QColor receivedColor();
    QColor* receivedColorPtr();
    JEnumWrapper receivedCustomQtEnum();
    JObjectWrapper receivedQtFlags();
    JObjectWrapper receivedList();
    JObjectWrapper receivedNumber();
    QGraphicsItem* receivedCustomQtValue();
    QGraphicsItem* receivedCustomQtInterfaceValue();
    JObjectWrapper receivedCustomJavaType();
    QObject* receivedDerivedQObject();

    static bool testMethodCallNumber(QObject* qobj);
    static bool testMethodCallEnum(QObject* qobj);
    static bool testMethodCallColor(QObject* qobj);
    static bool testMethodCallColorPtr(QObject* qobj);
    static bool testMethodCallQtEnum(QObject* qobj);
    static bool testMethodCallDerivedQObject(QObject* qobj);
    static bool testMethodCallCustomQtEnum(QObject* qobj);
    static bool testMethodCallQtFlags(QObject* qobj);
    static bool testMethodCallCustomQtValue(QObject* qobj);
    static bool testMethodCallCustomQtInterfaceValue(QObject* qobj);
    static bool testMethodCallCustomJavaType(QObject* qobj);
    static bool testMethodCallCustomQtEnum2(QObject* qobj);

    static bool testFetchPropertyNumber(QObject* qobj);
    static bool testFetchPropertyEnum(QObject* qobj);
    static bool testFetchPropertyColor(QObject* qobj);
    static bool testFetchPropertyColorPtr(QObject* qobj);
    static bool testFetchPropertyQtEnum(QObject* qobj);
    static bool testFetchPropertyDerivedQObject(QObject* qobj);
    static bool testFetchPropertyCustomQtEnum(QObject* qobj);
    static bool testFetchPropertyQtFlags(QObject* qobj);
    static bool testFetchPropertyCustomQtValue(QObject* qobj);
    static bool testFetchPropertyCustomQtInterfaceValue(QObject* qobj);
    static bool testFetchPropertyCustomJavaType(QObject* qobj);
    static bool testFetchPropertyCustomQtEnum2(QObject* qobj);

signals:
    
public slots:

private slots:
    void receiveCustomEnum(JEnumWrapper value);
    void receiveColor(QColor value);
    void receiveColorPtr(QColor* value);
    void receiveCustomQtEnum(JEnumWrapper value);
    void receiveQtFlags(JObjectWrapper value);
    void receiveList(JObjectWrapper value);
    void receiveNumber(JObjectWrapper value);
    void receiveCustomQtValue(QGraphicsItem* value);
    void receiveCustomQtInterfaceValue(QGraphicsItem* value);
    void receiveCustomJavaType(JObjectWrapper value);
    void receiveDerivedQObject(QObject* value);

private:
    JEnumWrapper m_receivedEnum;
    QColor m_receivedColor;
    QColor* m_receivedColorPtr;
    JEnumWrapper m_receivedQtEnum;
    JObjectWrapper m_receivedQtFlags;
    JObjectWrapper m_receivedList;
    JObjectWrapper m_receivedNumber;
    QGraphicsItem* m_receivedCustomQtValue;
    QGraphicsItem* m_receivedCustomQtInterfaceValue;
    JObjectWrapper m_receivedCustomJavaType;
    QObject* m_receivedDerivedQObject;
};
Q_DECLARE_METATYPE(QColor*)

#endif // PROPERTYANDMETHODCALLTEST_H
