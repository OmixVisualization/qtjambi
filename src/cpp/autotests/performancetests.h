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

#ifndef PERFORMANCETESTS_H
#define PERFORMANCETESTS_H

#include <chrono>
#include <QtCore>

class PerformanceTests
{
private:
    PerformanceTests() = default;
public:
    class AbstractVirtualHost : public QObject{
    public:
        virtual void test() = 0;
        virtual void test(const QModelIndex& index) = 0;
        virtual void test(const QString& string) = 0;
        virtual void test(const QSize& size) = 0;
        virtual void test(const QSize& size1, const QSize& size2) = 0;
        virtual void test(const QSize& size1, const QSize& size2, const QSize& size3) = 0;
        virtual void test(const QSize& size1, const QSize& size2, const QSize& size3, const QSize& size4) = 0;
        virtual void test(const QSize& size1, const QSize& size2, const QSize& size3, const QSize& size4, const QSize& size5) = 0;
        virtual void test(int i) = 0;
    };
    class VirtualHost : public AbstractVirtualHost{
    public:
        void test() override;
        void test(const QModelIndex& index) override;
        void test(const QString& string) override;
        void test(const QSize& size) override;
        void test(const QSize& size1, const QSize& size2) override;
        void test(const QSize& size1, const QSize& size2, const QSize& size3) override;
        void test(const QSize& size1, const QSize& size2, const QSize& size3, const QSize& size4) override;
        void test(const QSize& size1, const QSize& size2, const QSize& size3, const QSize& size4, const QSize& size5) override;
        void test(int i) override;
    };

    static AbstractVirtualHost* createNativeHost();
    static std::chrono::nanoseconds testAcuireJniEnvironment(int count);
    static std::chrono::nanoseconds testAcuireJniEnvironmentExceptionHandler(int count);
    static std::chrono::nanoseconds testAcuireJniEnvironmentExceptionHandlerAndBlocker(int count);
    static std::chrono::nanoseconds testAcuireJniEnvironmentCalling(int count);
    static std::chrono::nanoseconds testAcuireJniEnvironmentExceptionHandlerCalling(int count);
    static std::chrono::nanoseconds testAcuireJniEnvironmentExceptionHandlerAndBlockerCalling(int count);
    static std::chrono::nanoseconds testAcuireJniEnvironmentCallingChecking(int count);
    static std::chrono::nanoseconds testAcuireJniEnvironmentExceptionHandlerCallingChecking(int count);
    static std::chrono::nanoseconds testAcuireJniEnvironmentExceptionHandlerAndBlockerCallingChecking(int count);
    static std::chrono::nanoseconds testAcuireJniEnvironmentCheckException1(int count);
    static std::chrono::nanoseconds testAcuireJniEnvironmentCheckException2(int count);
    static std::chrono::nanoseconds testConvertQModelIndex(int count);
    static std::chrono::nanoseconds testConvertQModelIndexInvalid(int count);
    static std::chrono::nanoseconds testConvertQModelIndexCopy(int count);
    static std::chrono::nanoseconds testConvertQSize(int count);
    static std::chrono::nanoseconds testConvertQString(int count);
    static std::chrono::nanoseconds testPlain(AbstractVirtualHost* host, int count);
    static std::chrono::nanoseconds testQModelIndex(AbstractVirtualHost* host, int count);
    static std::chrono::nanoseconds testQModelIndexInvalid(AbstractVirtualHost* host, int count);
    static std::chrono::nanoseconds testQString(AbstractVirtualHost* host, int count);
    static std::chrono::nanoseconds testQSize1(AbstractVirtualHost* host, int count);
    static std::chrono::nanoseconds testQSize2(AbstractVirtualHost* host, int count);
    static std::chrono::nanoseconds testQSize3(AbstractVirtualHost* host, int count);
    static std::chrono::nanoseconds testQSize4(AbstractVirtualHost* host, int count);
    static std::chrono::nanoseconds testQSize5(AbstractVirtualHost* host, int count);
    static std::chrono::nanoseconds testInt(AbstractVirtualHost* host, int count);
    static std::chrono::nanoseconds testRowCount(QAbstractItemModel* host, int count, const QModelIndex &parent = QModelIndex());
    static void acquireThreadLocal();
    static void acquireQThreadStorage();
    static std::chrono::nanoseconds testAcquireThreadLocal(int count);
    static std::chrono::nanoseconds testAcquireQThreadStorage(int count);
};

#endif // PERFORMANCETESTS_H
