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

#ifndef SIGNALSANDSLOTS_H
#define SIGNALSANDSLOTS_H

#ifndef QTJAMBI_GENERATOR_RUNNING
#include <QtJambi/global.h>
#endif
#ifndef QTJAMBI_NO_WIDGETS
#include <QtWidgets/QtWidgets>
#endif

#include <QtCore/QObject>
#include <QtCore/QVariant>
#include <QtCore/QMetaProperty>
#include <QtCore/QStringList>
#include <QtCore/QDebug>

#ifndef QTJAMBI_GENERATOR_RUNNING
#include <QtJambi/QtJambiAPI>
#include <QtJambi/jobjectwrapper.h>
#endif

#ifndef SIGNAL
#  define SIGNAL(A) #A
#endif

#ifndef SLOT
#  define SLOT(A) #A
#endif

#ifndef emit
#  define emit
#endif

class SignalsAndSlots: public QObject
{
    Q_OBJECT

    Q_PROPERTY(QByteArray cppProperty READ cppProperty WRITE setCppProperty RESET resetCppProperty)
public:
    SignalsAndSlots()
    {
        slot1_1_called = 0;
        slot1_2_called = 0;
        slot1_3_called = 0;
        slot2_called = 0;
        slot3_called = 0;
    }

    static QMetaObject::Connection connectToDestroyedSignal(QObject * object, QRunnable *runnable){
        QObject* receiver = new QObject();
        receiver->moveToThread(nullptr);
        return QObject::connect(object, &QObject::destroyed, receiver, [runnable,receiver](){
            runnable->run();
            delete runnable;
            delete receiver;
        }, Qt::DirectConnection);
    }

    QList<bool> disconnectSignals(SignalsAndSlots *obj)
    {
        QList<bool> result;
        result << disconnect(this, SIGNAL(signal1()), obj, SLOT(slot1_1()));
        result << disconnect(this, SIGNAL(signal3(const QString &)), obj, SLOT(slot3(const QString &)));
        return result;
    }

    QList<bool> setupSignals(SignalsAndSlots *obj, int i)
    {
        QList<bool> result;
        result << connect(this, SIGNAL(signal1()), obj, SLOT(slot1_1()));
        if (i > 0)
            result << connect(this, SIGNAL(signal1()), obj, SLOT(slot1_2()));
        if (i > 1)
            result << connect(this, SIGNAL(signal1()), obj, SLOT(slot1_3()));

        result << connect(this, SIGNAL(signal2(int)), obj, SLOT(slot2(int)));
        result << connect(this, SIGNAL(signal3(const QString &)), obj, SLOT(slot3(const QString &)));
        return result;
    }

    QList<bool> setupSignalsFP(SignalsAndSlots *obj, int i)
    {
        QList<bool> result;
        result << connect(this, &SignalsAndSlots::signal1, obj, &SignalsAndSlots::slot1_1);
        if (i > 0)
            result << connect(this, &SignalsAndSlots::signal1, obj, &SignalsAndSlots::slot1_2);
        if (i > 1)
            result << connect(this, &SignalsAndSlots::signal1, obj, &SignalsAndSlots::slot1_3);

        result << connect(this, &SignalsAndSlots::signal2, obj, &SignalsAndSlots::slot2);
        result << connect(this, &SignalsAndSlots::signal3, obj, &SignalsAndSlots::slot3);
        return result;
    }

    void emit_signal_1() { emit signal1(); }
    void emit_signal_2(int i) { emit signal2(i); }
    void emit_signal_3(const QString &str) { emit signal3(str); }

    bool connectSignal1ToSlot4() {
        return connect(this, SIGNAL(signal1()), this, SLOT(slot4()));
    }

    bool connectSignal4ToSlot1_1() {
        return connect(this, SIGNAL(signal4()), this, SLOT(slot1_1()));
    }

    bool connectSignal4ToSlot4() {
        return connect(this, SIGNAL(signal4()), this, SLOT(slot4()));
    }

    bool connectSignal5ToSlot3()
    {
        return connect(this, SIGNAL(signal5(const QString &)), this, SLOT(slot3(const QString &)));
    }

    bool connectSignal1ToSlot1_1()
    {
        return connectSignal1ToSlot1_1In(this);
    }

    bool connectSignal1ToSlot1_1In(QObject *other)
    {
        return connect(this, SIGNAL(signal1()), other, SLOT(slot1_1()));
    }

    bool disconnectSignal1FromSlot1_1()
    {
        return disconnect(this, SIGNAL(signal1()), this, SLOT(slot1_1()));
    }

    bool connectSignal2ToSlot2()
    {
        return connect(this, SIGNAL(signal2(int)), this, SLOT(slot2(int)));
    }

    bool connectSignal1ToSlot1_1FP()
    {
        return connectSignal1ToSlot1_1InFP(this);
    }

    bool connectSignal1ToSlot1_1InFP(SignalsAndSlots *other)
    {
        return connect(this, &SignalsAndSlots::signal1, other, &SignalsAndSlots::slot1_1);
    }

    bool disconnectSignal1FromSlot1_1FP()
    {
        return disconnect(this, &SignalsAndSlots::signal1, this, &SignalsAndSlots::slot1_1);
    }

    bool connectSignal2ToSlot2FP()
    {
        return connect(this, &SignalsAndSlots::signal2, this, &SignalsAndSlots::slot2);
    }

    bool connectSignal6ToUnnormalizedSignature()
    {
        return connect(this, SIGNAL(  signal6( const QString &, int   ) ), this, SLOT(          unnormalized_signature(   String,   int   )));
    }

    bool javaSignalToCppSlot()
    {
        return connect(this, SIGNAL(aJavaSignal(const QString &, const QByteArray &)), this, SLOT(aCppSlot(const QString &, const QByteArray &)));
    }

    bool disconnectAllFromObject()
    {
        return disconnect();
    }

    bool disconnectAllFromSignal1()
    {
        return disconnect(SIGNAL(signal1()));
    }

    bool disconnectAllFromSignal1FP()
    {
        return QObject::disconnect(this, &SignalsAndSlots::signal1, nullptr, nullptr);
    }

    bool disconnectReceiverFromSignal1(QObject *receiver)
    {
        return disconnect(SIGNAL(signal1()), receiver);
    }

    bool disconnectAllFromReceiver(QObject *receiver)
    {
        return disconnect(nullptr, receiver);
    }

    static SignalsAndSlots *createConnectedObject()
    {
        SignalsAndSlots *sas = new SignalsAndSlots;
        QObject::connect(sas, SIGNAL(signal1()), sas, SLOT(slot1_1()));
        return sas;
    }

    bool setByteArrayProperty(const QString &propertyName, const QByteArray &byteArray) {
        return setProperty(propertyName.toLatin1(), QVariant(byteArray));
    }

    QByteArray byteArrayProperty(const QString &propertyName) {
        return property(propertyName.toLatin1()).toByteArray();
    }

    QByteArray cppProperty() const { return m_cppProperty; }

    void setCppProperty(const QByteArray &ba) { m_cppProperty = ba; }

    void resetCppProperty() { m_cppProperty = "it was the darkest and stormiest night evar"; }

    int slot1_1_called;
    int slot1_2_called;
    int slot1_3_called;
    int slot2_called;
    int slot3_called;

    QString received_string;
    QByteArray received_bytearray;

signals:
    void signal1();
    void signal2(int);
    void signal3(const QString &);
    void signal6(const QString &, int);

public slots:
    void slot1_1() { ++slot1_1_called; }
    virtual void slot1_2() { ++slot1_2_called; }
    QByteArray slot1_3() { ++slot1_3_called; return QByteArray(); }
    virtual void slot2(int i) { slot2_called += i; }
    void slot3(const QString &str) { slot3_called += str.toInt(); }

private slots:
    void aCppSlot(const QString &str, const QByteArray &ba) { received_string = str; received_bytearray = ba; }

private:
    QByteArray m_cppProperty;

protected:
    virtual void connectNotify(const QMetaMethod & signal) override{
        qDebug() << "connecting signal " << signal.name();
    }

    virtual void disconnectNotify(const QMetaMethod & signal) override{
        if(signal.isValid())
            qDebug() << "disconnecting signal " << signal.name();
        else
            qDebug() << "diconnectAll()";
    }
};

#endif // SIGNALSANDSLOTS_H
