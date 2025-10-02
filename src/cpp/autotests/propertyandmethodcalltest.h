/****************************************************************************
**
** Copyright (C) 2009-2025 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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

#ifndef PROPERTYANDMETHODCALLTEST_H
#define PROPERTYANDMETHODCALLTEST_H

#ifndef QTJAMBI_GENERATOR_RUNNING
#include <QtJambi/global.h>
#endif
#include <QtCore/QtCore>
#include <QtGui/QtGui>
#ifndef QTJAMBI_NO_WIDGETS
#include <QtWidgets/QtWidgets>
#endif
#ifndef QTJAMBI_GENERATOR_RUNNING
#include <QtJambi/QtJambiAPI>
#include <QtJambi/jobjectwrapper.h>
#endif

class PropertyAndMethodCallTest : public QObject
{
    Q_OBJECT
public:
    explicit PropertyAndMethodCallTest(QObject *parent = nullptr);
    
    bool connectSignals(QObject* sender, bool useAnnotatedType);
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
    JObjectWrapper receivedExtendedColor();

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
    static bool testMethodCallExtendedColor(QObject* qobj);

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
    static bool testFetchPropertyExtendedColor(QObject* qobj);
#if QT_VERSION >= QT_VERSION_CHECK(6, 6, 0)
    static QVariant instantiateInPlace(const QMetaMethod& constructor, std::initializer_list<QVariant> args);
#endif
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
    void receiveExtendedColor(JObjectWrapper value);

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
    JObjectWrapper m_receivedExtendedColor;
    QObject* m_receivedDerivedQObject;
};
Q_DECLARE_METATYPE(QColor*)

#endif // PROPERTYANDMETHODCALLTEST_H
