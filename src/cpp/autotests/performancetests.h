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
