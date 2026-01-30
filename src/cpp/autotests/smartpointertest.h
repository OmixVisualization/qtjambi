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

#ifndef SMARTPOINTERTEST_H
#define SMARTPOINTERTEST_H

#ifndef QTJAMBI_GENERATOR_RUNNING
#include <QtJambi/global.h>
#endif
#include <QtCore/QtCore>
#ifndef QTJAMBI_NO_WIDGETS
#include <QtWidgets/QtWidgets>
#endif

class JObjectWrapper;

class SmartPointerTest : public QObject
{
    Q_OBJECT
public:
    explicit SmartPointerTest(QObject *parent = nullptr);
    ~SmartPointerTest();

    QSharedPointer<QObject> createSharedObject1();
    const QSharedPointer<QGraphicsItem> createSharedObject2();
    QSharedPointer<QEvent> createSharedObject3();
    const QSharedPointer<QLayoutItem> createSharedObject4();
    const QSharedPointer<QLayoutItem> createSharedObject5();
    const QSharedPointer<QLayoutItem> createSharedObject6();
    QSharedPointer<QTemporaryFile> createSharedObject7();
    QSharedPointer<QList<QString> > createSharedObject8();
    static QSharedPointer<QObject> asSharedPointer(QSharedPointer<QObject> object);
    std::shared_ptr<QObject> createStdSharedObject1();
    const std::shared_ptr<QGraphicsItem> createStdSharedObject2();
    std::shared_ptr<QEvent> createStdSharedObject3();
    const std::shared_ptr<QLayoutItem> createStdSharedObject4();
    const std::shared_ptr<QLayoutItem> createStdSharedObject5();
    const std::shared_ptr<QLayoutItem> createStdSharedObject6();
    std::shared_ptr<QTemporaryFile> createStdSharedObject7();
    std::shared_ptr<QList<QString> > createStdSharedObject8();
    static std::shared_ptr<QObject> asStdSharedPointer(std::shared_ptr<QObject> object);
    static std::unique_ptr<QObject> asUniquePointer(std::unique_ptr<QObject> object);
    const QString& deletedSharedObjectName();
    void resetSharedObjectName();
    virtual std::unique_ptr<QObject> createUniqueObject();
    void useUniqueObject();
    virtual std::unique_ptr<QEvent> createUniqueEvent();
    virtual void analyze(QObject* obj);
    virtual void analyze(QEvent* evt);
    void useUniqueEvent();
    virtual const char* virtualString();
    QByteArray returnVirtualString();
    typedef std::function<const char*()> StringSupplier;
    QByteArray returnString(StringSupplier supplier);
    static QScopedPointer<QObject> asScopedPointer(QScopedPointer<QObject> object);
    virtual QScopedPointer<QObject> createScopedObject();
    void useScopedObject();
    virtual QScopedPointer<QEvent> createScopedEvent();
    void useScopedEvent();
signals:

public slots:
    void onDestroyed(QObject* object);
private:
    QString m_deletedSharedObjectName;
};

#endif // SMARTPOINTERTEST_H
