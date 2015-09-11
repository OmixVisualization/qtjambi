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

#include <QtCore/QObject>
#include <QtCore/QVariant>
#include <QtCore/QMetaProperty>
#include <QtCore/QStringList>

#include <qtjambi/qtjambi_core.h>
#include <QDebug>

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

    Q_PROPERTY(QByteArray cppProperty READ cppProperty WRITE setCppProperty RESET resetCppProperty);
public:
    SignalsAndSlots()
    {
        slot1_1_called = 0;
        slot1_2_called = 0;
        slot1_3_called = 0;
        slot2_called = 0;
        slot3_called = 0;
    }

    void disconnectSignals(SignalsAndSlots *obj)
    {
        disconnect(this, SIGNAL(signal1()), obj, SLOT(slot1_1()));
        disconnect(this, SIGNAL(signal3(const QString &)), obj, SLOT(slot3(const QString &)));
    }

    void setupSignals(SignalsAndSlots *obj, int i)
    {
        connect(this, SIGNAL(signal1()), obj, SLOT(slot1_1()));
        if (i > 0)
            connect(this, SIGNAL(signal1()), obj, SLOT(slot1_2()));
        if (i > 1)
            connect(this, SIGNAL(signal1()), obj, SLOT(slot1_3()));

        connect(this, SIGNAL(signal2(int)), obj, SLOT(slot2(int)));
        connect(this, SIGNAL(signal3(const QString &)), obj, SLOT(slot3(const QString &)));
    }

    void emit_signal_1() { emit signal1(); }
    void emit_signal_2(int i) { emit signal2(i); }
    void emit_signal_3(const QString &str) { emit signal3(str); }

    bool connectSignal1ToSlot4() {
        return connect(this, SIGNAL(signal1()), this, SLOT(slot4()));
    }

    bool connectSignal4ToSlot1_1() {
        return connect(this, SIGNAL(signal4), this, SLOT(slot1_1()));
    }

    bool connectSignal4ToSlot4() {
        return connect(this, SIGNAL(signal4), this, SLOT(slot4()));
    }

    bool connectSignal5ToSlot3()
    {
        return connect(this, SIGNAL(signal5), this, SLOT(slot3(const QString &)));
    }

    void connectSignal1ToSlot1_1()
    {
        connectSignal1ToSlot1_1In(this);
    }

    void connectSignal1ToSlot1_1In(QObject *other)
    {
        connect(this, SIGNAL(signal1()), other, SLOT(slot1_1()));
    }

    void disconnectSignal1FromSlot1_1()
    {
        disconnect(this, SIGNAL(signal1()), this, SLOT(slot1_1()));
    }

    void connectSignal2ToSlot2()
    {
        connect(this, SIGNAL(signal2(int)), this, SLOT(slot2(int)));
    }

    void connectSignal6ToUnnormalizedSignature()
    {
        connect(this, SIGNAL(  signal6( const QString &, int   ) ), this, SLOT(          unnormalized_signature(   String,   int   )));
    }

    void javaSignalToCppSlot()
    {
        connect(this, SIGNAL(aJavaSignal(const QString &, const QByteArray &)), this, SLOT(aCppSlot(const QString &, const QByteArray &)));
    }

    void disconnectAllFromObject()
    {
        disconnect();
    }

    void disconnectAllFromSignal1()
    {
        disconnect(SIGNAL(signal1()));
    }

    void disconnectReceiverFromSignal1(QObject *receiver)
    {
        disconnect(SIGNAL(signal1()), receiver);
    }

    void disconnectAllFromReceiver(QObject *receiver)
    {
        disconnect(0, receiver);
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

    void resetProperty(const QString &propertyName) {
        QMetaProperty prop = metaObject()->property(metaObject()->indexOfProperty(propertyName.toLatin1()));
        prop.reset(this);
    }

    QString classNameFromMetaObject() {
        return metaObject()->className();
    }

    QString classNameOfSuperClassFromMetaObject() {
        return metaObject()->superClass()->className();
    }

    QStringList propertyNamesFromMetaObject() {
        QStringList list;
        for (int i=0; i<metaObject()->propertyCount(); ++i)
            list.append(QLatin1String(metaObject()->property(i).name()));

        return list;
    }

    int propertyCountFromMetaObject() {
        return metaObject()->propertyCount();
    }

    int propertyCountOfSuperClassFromMetaObject() {
        return metaObject()->superClass()->propertyCount();
    }

    QByteArray cppProperty() const { return m_cppProperty; }

    void setCppProperty(const QByteArray &ba) { m_cppProperty = ba; }

    void resetCppProperty() { m_cppProperty = "it was the darkest and stormiest night evar"; }

    int numberOfEnumTypes()
    {
        return metaObject()->enumeratorCount();
    }

    QStringList namesOfEnumType(const QString &name)
    {
        QStringList returned;

        QMetaEnum metaEnum = metaObject()->enumerator(metaObject()->indexOfEnumerator(name.toUtf8().constData()));
        int count = metaEnum.keyCount();
        for (int i=0; i<count; ++i) {
            returned.append(QString::fromLatin1(metaEnum.key(i)));
        }

        return returned;
    }

    QList<int> valuesOfEnumType(const QString &name)
    {
        QList<int> returned;

        QMetaEnum metaEnum = metaObject()->enumerator(metaObject()->indexOfEnumerator(name.toUtf8().constData()));
        int count = metaEnum.keyCount();
        for (int i=0; i<count; ++i) {
            returned.append(metaEnum.value(i));
        }

        return returned;
    }

    bool isFlagsType(const QString &name)
    {
        QMetaEnum metaEnum = metaObject()->enumerator(metaObject()->indexOfEnumerator(name.toUtf8().constData()));
        return metaEnum.isFlag();
    }

    bool isValidEnum(const QString &name)
    {
        QMetaEnum metaEnum = metaObject()->enumerator(metaObject()->indexOfEnumerator(name.toUtf8().constData()));
        return metaEnum.isValid();
    }

    static QString metaObjectMethodSignature(QObject *obj, const QString &name)
    {
        for (int i=0; i<obj->metaObject()->methodCount(); ++i) {
            if (QString(obj->metaObject()->method(i).methodSignature()).contains(name)) {
                return obj->metaObject()->method(i).methodSignature();
            }
        }

        return "";
    }

    static bool invokeMethod(QObject *obj, const QString &name, jobject arg)
    {
        JNIEnv *env = qtjambi_current_environment();
        return QMetaObject::invokeMethod(obj, name.toLatin1(), Qt::DirectConnection, Q_ARG(JObjectWrapper, JObjectWrapper(env, arg)));
    }

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
    void slot1_1() { slot1_1_called++; }
    virtual void slot1_2() { slot1_2_called++; }
    QByteArray slot1_3() { slot1_3_called++; return QByteArray(); }
    virtual void slot2(int i) { slot2_called += i; }
    void slot3(const QString &str) { slot3_called += str.toInt(); }

private slots:
    void aCppSlot(const QString &str, const QByteArray &ba) { received_string = str; received_bytearray = ba; }

private:
    QByteArray m_cppProperty;

protected:
    virtual void connectNotify(const QMetaMethod & signal){
        //QObjectPrivate* objectPrivate = QObjectPrivate::get(this);

        qDebug() << QByteArray("connecting to signal "+signal.name());
    }

    virtual void disconnectNotify(const QMetaMethod & signal){
        qDebug() << QByteArray(signal.isValid() ? "disconnecting signal "+signal.name() : "diconnectAll()");
    }
};

#endif // SIGNALSANDSLOTS_H
