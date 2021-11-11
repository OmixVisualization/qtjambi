#include "interfaces.h"

TestAbstractClass::TestAbstractClass(){}
TestAbstractClass::~TestAbstractClass(){}
QString TestAbstractClass::method1() const { return QString("TestAbstractClass::method1"); }
QString TestAbstractClass::method4() const { return QString("TestAbstractClass::method4"); }
QString TestAbstractClass::method5() const { return QString("TestAbstractClass::method5"); }
QString TestAbstractClass::callMethod1(TestAbstractClass* cls) { return cls->method1(); }
QString TestAbstractClass::callMethod2(TestAbstractClass* cls) { return cls->method2(); }
QString TestAbstractClass::callMethod3(TestAbstractClass* cls) { return cls->method3(); }
QString TestAbstractClass::callMethod4(TestAbstractClass* cls) { return cls->method4(); }
QString TestAbstractClass::callMethod5(TestAbstractClass* cls) { return cls->method5(); }

TestInterface::TestInterface(const QString &){}
TestInterface::~TestInterface(){}
bool TestInterface::setReferenceCountTest2(QObject* object) { Q_UNUSED(object) return false; }
bool TestInterface::setReferenceCountTest3(QObject* object) { Q_UNUSED(object) return false; }
QString TestInterface::method1() const { return QString("TestInterface::method1"); }
QString TestInterface::method2() const { return QString("TestInterface::method2"); }
QString TestInterface::method3() const { return QString("TestInterface::method3"); }
QString TestInterface::callMethod1(TestInterface* cls) { return cls->method1(); }
QString TestInterface::callMethod2(TestInterface* cls) { return cls->method2(); }
QString TestInterface::callMethod3(TestInterface* cls) { return cls->method3(); }
QString TestInterface::callMethod4(TestInterface* cls) { return cls->method4(); }
QString TestInterface::callMethod5(TestInterface* cls) { return cls->method5(); }
void TestInterface::protectedNonVirtualInterfaceMethod(){}
void TestInterface::protectedVirtualInterfaceMethod(){}
void TestInterface::virtualInterfaceMethod(){}
void TestInterface::virtualImplementedInterfaceMethod(){}
void TestInterface::protectedImplementedVirtualInterfaceMethod(){}

TestInterfaceObject::TestInterfaceObject(const QString & s) : QObject(), TestInterface(s) {}
TestInterfaceObject::~TestInterfaceObject(){}
bool TestInterfaceObject::setReferenceCountTest1(QObject* object) { Q_UNUSED(object) return false; }
void TestInterfaceObject::protectedVirtualMethod(){}
TestInterfaceObject* TestInterfaceObject::create(const QString & s) {
    return new TestInterfaceObject2(s);
}
void TestInterfaceObject::nonVirtualMethod(){}
void TestInterfaceObject::virtualMethod(){}
void TestInterfaceObject::virtualImplementedMethod(){}

TestInterfaceObject2::TestInterfaceObject2(const QString & s) : TestInterfaceObject(s) {}
TestInterfaceObject2::~TestInterfaceObject2(){}
void TestInterfaceObject2::virtualImplementedInterfaceMethod(){}
void TestInterfaceObject2::protectedImplementedVirtualInterfaceMethod(){}
void TestInterfaceObject2::virtualImplementedMethod(){}
QString TestInterfaceObject2::method4() const { return QString("TestInterfaceObject2::method4"); }
QString TestInterfaceObject2::method5() const { return QString("TestInterfaceObject2::method5"); }

TestInterfaceImpl::TestInterfaceImpl(const QString & s) : TestInterface(s) {}
TestInterfaceImpl::~TestInterfaceImpl(){}
bool TestInterfaceImpl::setReferenceCountTest1(QObject* object) { Q_UNUSED(object) return false; }

TestPrivateInterface::~TestPrivateInterface(){}
TestPrivateInterface::TestPrivateInterface(){}

void SetupLayout::setupLayout(QLayout *layout)
{
    QPushButton *button1 = new QPushButton("Test", layout->parentWidget());
    QPushButton *button2 = new QPushButton("Test2", layout->parentWidget());
    QSpacerItem *spacer = new QSpacerItem(10, 10);

    layout->addWidget(button1);
    layout->addItem(spacer);
    layout->addWidget(button2);

    {
        QHBoxLayout *other_layout = new QHBoxLayout;
        QPushButton *button3 = new QPushButton("Test3", layout->parentWidget());
        QSpacerItem *spacer2 = new QSpacerItem(5, 5);
        QPushButton *button4 = new QPushButton("Test4", layout->parentWidget());
        other_layout->addWidget(button3);
        other_layout->addItem(spacer2);
        other_layout->addWidget(button4);

        layout->addItem(other_layout);
    }
}

QLayout* SetupLayout::createLayout(){
    return new QHBoxLayout;
}

FunctionManager::FunctionManager(QObject* parent) : QObject(parent), m_text(), m_stringSupplier(), m_stringConsumer() {}
FunctionManager::~FunctionManager(){}
void FunctionManager::reset() { m_text = QString(); }
void FunctionManager::setStringSupplier(StringSupplier stringSupplier){ m_stringSupplier = stringSupplier; }
void FunctionManager::setStringConsumer(StringConsumer stringConsumer){ m_stringConsumer = stringConsumer; }
bool FunctionManager::callStringSupplier(){
    if(m_stringSupplier)
        m_text = m_stringSupplier(0);
    return bool(m_stringSupplier);
}
bool FunctionManager::callStringConsumer(){
    if(m_stringConsumer)
        m_stringConsumer(QLatin1String("StringConsumerTest"));
    return bool(m_stringConsumer);
}
QString FunctionManager::text() const {return m_text;}

class MoccedInternalSub1Object : public MoccedSub1Object{
};

class UnMoccedInternalSub1Object : public UnMoccedSub1Object{
};

class MoccedInternalSub2Object : public MoccedSub2Object{
};

class UnMoccedInternalSub2Object : public UnMoccedSub2Object{
};

class MoccedInternalObject : public MoccedObject{
};

class UnMoccedInternalObject : public UnMoccedObject{
};

QObject* UnMoccedObject::create(int type){
    switch(type){
    case 1:
        return new MoccedObject;
    case 2:
        return new MoccedSub1Object;
    case 3:
        return new MoccedSub2Object;
    case 4:
        return new MoccedInternalSub1Object;
    case 5:
        return new MoccedInternalSub2Object;
    case -1:
        return new UnMoccedObject;
    case -2:
        return new UnMoccedSub1Object;
    case -3:
        return new UnMoccedSub2Object;
    case -4:
        return new UnMoccedInternalSub1Object;
    case -5:
        return new UnMoccedInternalSub2Object;
    }
    return nullptr;
}
