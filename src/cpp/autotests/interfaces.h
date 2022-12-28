/****************************************************************************
**
** Copyright (C) 1992-2009 Nokia. All rights reserved.
**
** This file is part of Qt Jambi.
**
** ** $BEGIN_LICENSE$
** Commercial Usage
** Licensees holding valid Qt Commercial licenses may use this file in
** accordance with the Qt Commercial License Agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Nokia.
** 
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
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
** If you are unsure which license is appropriate for your use, please
** contact the sales department at qt-sales@nokia.com.
** $END_LICENSE$

**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

#ifndef INTERFACES_H
#define INTERFACES_H

#ifndef QTJAMBI_GENERATOR_RUNNING
#include <QtJambi/global.h>
#endif
#include <QtGui/QtGui>
#ifndef QTJAMBI_NO_WIDGETS
#include <QtWidgets/QtWidgets>
#endif

class TestAbstractClass
{
public:
    explicit TestAbstractClass();
    virtual ~TestAbstractClass();
    virtual bool setReferenceCountTest1(QObject* object) = 0;
    virtual QString method1() const;
    virtual QString method2() const = 0;
    virtual QString method3() const = 0;
    virtual QString method4() const;
    QString method5() const;
    static QString callMethod1(TestAbstractClass* cls);
    static QString callMethod2(TestAbstractClass* cls);
    static QString callMethod3(TestAbstractClass* cls);
    static QString callMethod4(TestAbstractClass* cls);
    static QString callMethod5(TestAbstractClass* cls);
    static QObject* createInterfaceImplementingQObject(QObject* parent = nullptr);
    static class TestInterface* createQObjectInheritingInterface(QObject* parent = nullptr);
};

class TestInterface
{
public:
    explicit TestInterface(const QString &);
    virtual ~TestInterface();
    virtual bool setReferenceCountTest1(QObject* object) = 0;
    virtual bool setReferenceCountTest2(QObject* object);
    bool setReferenceCountTest3(QObject* object);
    QString method1() const;
    QString method2() const;
    virtual QString method3() const;
    virtual QString method4() const = 0;
    virtual QString method5() const = 0;
    static QString callMethod1(TestInterface* cls);
    static QString callMethod2(TestInterface* cls);
    static QString callMethod3(TestInterface* cls);
    static QString callMethod4(TestInterface* cls);
    static QString callMethod5(TestInterface* cls);
    virtual void virtualInterfaceMethod();
    virtual void virtualImplementedInterfaceMethod();
protected:
    void protectedNonVirtualInterfaceMethod();
    virtual void protectedVirtualInterfaceMethod();
    virtual void protectedImplementedVirtualInterfaceMethod();
};

class TestInterfaceObject : public QObject, public TestInterface
{
    Q_OBJECT
public:
    explicit TestInterfaceObject(const QString & s);
    virtual ~TestInterfaceObject() override;
    virtual bool setReferenceCountTest1(QObject* object) override;
    void nonVirtualMethod();
    virtual void virtualMethod();
    virtual void virtualImplementedMethod();
    static TestInterfaceObject* create(const QString & s);
protected:
    virtual void protectedVirtualMethod();
};

class TestInterfaceObject2 : public TestInterfaceObject{
    Q_OBJECT
public:
    explicit TestInterfaceObject2(const QString & s);
    virtual ~TestInterfaceObject2() override;
    void virtualImplementedMethod() override;
    void virtualImplementedInterfaceMethod() override;
    void protectedImplementedVirtualInterfaceMethod() override;
    QString method4() const override;
    QString method5() const override;
};

class TestInterfaceImpl : public TestInterface
{
public:
    explicit TestInterfaceImpl(const QString & s);
    virtual ~TestInterfaceImpl() override;
    virtual bool setReferenceCountTest1(QObject* object) override;
};

class TestPrivateInterface
{
public:
    virtual void interfaceVirtual() = 0;
    virtual ~TestPrivateInterface();
private:
    TestPrivateInterface();
};

class SetupLayout
{
public:
    static void setupLayout(QLayout *layout);
    static QLayout* createLayout();
};

class FunctionManager : public QObject{
    Q_OBJECT
public:
    FunctionManager(QObject* parent = nullptr);
    virtual ~FunctionManager() override;

    typedef std::function<QString(int)> StringSupplier;
    typedef std::function<void(const QString&)> StringConsumer;

    void reset();
    void setStringSupplier(StringSupplier stringSupplier);
    void setStringConsumer(StringConsumer stringConsumer);
    bool callStringSupplier();
    bool callStringConsumer();
    QString text() const;
private:
    QString m_text;
    StringSupplier m_stringSupplier;
    StringConsumer m_stringConsumer;
};

class MoccedObject : public QObject{
    Q_OBJECT
public:
    MoccedObject(QObject* parent = nullptr) : QObject(parent) {}
};

class UnMoccedObject : public MoccedObject{
public:
    UnMoccedObject(QObject* parent = nullptr) : MoccedObject(parent) {}

    static QObject* create(int type);
};

class MoccedSub1Object : public MoccedObject{
public:
    MoccedSub1Object(QObject* parent = nullptr) : MoccedObject(parent) {}
};

class UnMoccedSub1Object : public UnMoccedObject{
public:
    UnMoccedSub1Object(QObject* parent = nullptr) : UnMoccedObject(parent) {}
};

class MoccedSub2Object : public MoccedObject{
public:
    MoccedSub2Object(QObject* parent = nullptr) : MoccedObject(parent) {}
};

class UnMoccedSub2Object : public UnMoccedObject{
public:
    UnMoccedSub2Object(QObject* parent = nullptr) : UnMoccedObject(parent) {}
};

#endif

