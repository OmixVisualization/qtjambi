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

#include "performancetests.h"
#include <iostream>
#include <thread>
#include <QtGui>

#include <QtJambi/QtJambiAPI>
#include <QtJambi/JavaAPI>
#include <QtJambi/Cast>

namespace Java{
namespace Runtime{
QTJAMBI_REPOSITORY_DECLARE_CLASS(Map,
                                 QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(put)
                                 QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(get))
QTJAMBI_REPOSITORY_DEFINE_CLASS(java/util,Map,
                                QTJAMBI_REPOSITORY_DEFINE_METHOD(put,(Ljava/lang/Object;Ljava/lang/Object;)Ljava/lang/Object;)
                                QTJAMBI_REPOSITORY_DEFINE_METHOD(get,(Ljava/lang/Object;)Ljava/lang/Object;)
                                )
}
namespace Autotests{
QTJAMBI_REPOSITORY_DECLARE_CLASS(PerformanceTests,
                                 QTJAMBI_REPOSITORY_DECLARE_STATIC_VOID_METHOD(empty)
                                public: static inline jmethodID empty_method(JNIEnv* env){
                                    auto _this = __qt_get_this(env);
                                    return _this.__empty;
                                }
                                 )
QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/autotests/generated,PerformanceTests,
                                QTJAMBI_REPOSITORY_DEFINE_STATIC_METHOD(empty,()V)
                                )
}
}

void PerformanceTests::VirtualHost::test(){}
void PerformanceTests::VirtualHost::test(const QModelIndex&){}
void PerformanceTests::VirtualHost::test(const QString&){}
void PerformanceTests::VirtualHost::test(const QSize&){}
void PerformanceTests::VirtualHost::test(const QSize&,const QSize&){}
void PerformanceTests::VirtualHost::test(const QSize&,const QSize&,const QSize&){}
void PerformanceTests::VirtualHost::test(const QSize&,const QSize&,const QSize&,const QSize&){}
void PerformanceTests::VirtualHost::test(const QSize&,const QSize&,const QSize&,const QSize&,const QSize&){}
void PerformanceTests::VirtualHost::test(int){}

PerformanceTests::AbstractVirtualHost* PerformanceTests::createNativeHost() { return new VirtualHost(); }

std::chrono::nanoseconds PerformanceTests::testAcuireJniEnvironment(int count){
    auto start = std::chrono::high_resolution_clock::now();
    for(int i=0; i<count; ++i){
        if(JniEnvironment env{500}){
            QtJambiExceptionHandler h;
        }
        //std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
    auto end = std::chrono::high_resolution_clock::now();
    return std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
}

std::chrono::nanoseconds PerformanceTests::testAcuireJniEnvironmentExceptionHandler(int count){
    auto start = std::chrono::high_resolution_clock::now();
    for(int i=0; i<count; ++i){
        if(JniEnvironmentExceptionHandler env{500}){
        }
        //std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
    auto end = std::chrono::high_resolution_clock::now();
    return std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
}

std::chrono::nanoseconds PerformanceTests::testAcuireJniEnvironmentExceptionHandlerAndBlocker(int count){
    auto start = std::chrono::high_resolution_clock::now();
    for(int i=0; i<count; ++i){
        if(JniEnvironmentExceptionHandlerAndBlocker env{500}){
            env.releaseException();
        }
        //std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
    auto end = std::chrono::high_resolution_clock::now();
    return std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
}

std::chrono::nanoseconds PerformanceTests::testAcuireJniEnvironmentCalling(int count){
    Java::Autotests::PerformanceTests::getClass(JniEnvironment{100});
    auto start = std::chrono::high_resolution_clock::now();
    for(int i=0; i<count; ++i){
        if(JniEnvironment env{500}){
            QtJambiExceptionHandler h;
            QTJAMBI_TRY {
                Java::Autotests::PerformanceTests::empty(env);
            } QTJAMBI_CATCH(const JavaException& exn){
                h.handle(env, exn, "");
            } QTJAMBI_TRY_END
        }
        //std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
    auto end = std::chrono::high_resolution_clock::now();
    return std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
}

std::chrono::nanoseconds PerformanceTests::testAcuireJniEnvironmentExceptionHandlerCalling(int count){
    Java::Autotests::PerformanceTests::getClass(JniEnvironment{100});
    auto start = std::chrono::high_resolution_clock::now();
    for(int i=0; i<count; ++i){
        if(JniEnvironmentExceptionHandler env{500}){
            QTJAMBI_TRY {
                Java::Autotests::PerformanceTests::empty(env);
            } QTJAMBI_CATCH(const JavaException& exn){
                env.handleException(exn, "");
            } QTJAMBI_TRY_END
        }
        //std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
    auto end = std::chrono::high_resolution_clock::now();
    return std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
}

std::chrono::nanoseconds PerformanceTests::testAcuireJniEnvironmentExceptionHandlerAndBlockerCalling(int count){
    Java::Autotests::PerformanceTests::getClass(JniEnvironment{100});
    auto start = std::chrono::high_resolution_clock::now();
    for(int i=0; i<count; ++i){
        if(JniEnvironmentExceptionHandlerAndBlocker env{500}){
            QTJAMBI_TRY {
                Java::Autotests::PerformanceTests::empty(env);
            } QTJAMBI_CATCH(const JavaException& exn){
                env.handleException(exn, "");
            } QTJAMBI_TRY_END
                env.releaseException();
        }
        //std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
    auto end = std::chrono::high_resolution_clock::now();
    return std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
}

std::chrono::nanoseconds PerformanceTests::testAcuireJniEnvironmentCallingChecking(int count){
    jmethodID empty_method;
    jclass cls;
    {
        JniEnvironment env{100};
        empty_method = Java::Autotests::PerformanceTests::empty_method(env);
        cls = jclass(env->NewGlobalRef(Java::Autotests::PerformanceTests::getClass(env)));
    }
    auto start = std::chrono::high_resolution_clock::now();
    for(int i=0; i<count; ++i){
        if(JniEnvironment env{500}){
            QtJambiExceptionHandler h;
            QTJAMBI_TRY {
                env->CallStaticVoidMethod(cls, empty_method);
                env.checkException();
            Java::Autotests::PerformanceTests::empty(env);
            } QTJAMBI_CATCH(const JavaException& exn){
                h.handle(env, exn, "");
            } QTJAMBI_TRY_END
        }
        //std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
    auto end = std::chrono::high_resolution_clock::now();
    {
        JniEnvironment env{100};
        env->DeleteGlobalRef(cls);
    }
    return std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
}

std::chrono::nanoseconds PerformanceTests::testAcuireJniEnvironmentExceptionHandlerCallingChecking(int count){
    jmethodID empty_method;
    jclass cls;
    {
        JniEnvironment env{100};
        empty_method = Java::Autotests::PerformanceTests::empty_method(env);
        cls = jclass(env->NewGlobalRef(Java::Autotests::PerformanceTests::getClass(env)));
    }
    auto start = std::chrono::high_resolution_clock::now();
    for(int i=0; i<count; ++i){
        if(JniEnvironmentExceptionHandler env{500}){
            QTJAMBI_TRY {
                env->CallStaticVoidMethod(cls, empty_method);
                env.checkException();
            } QTJAMBI_CATCH(const JavaException& exn){
                env.handleException(exn, "");
            } QTJAMBI_TRY_END
        }
        //std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
    auto end = std::chrono::high_resolution_clock::now();
    {
        JniEnvironment env{100};
        env->DeleteGlobalRef(cls);
    }
    return std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
}

std::chrono::nanoseconds PerformanceTests::testAcuireJniEnvironmentExceptionHandlerAndBlockerCallingChecking(int count){
    jmethodID empty_method;
    jclass cls;
    {
        JniEnvironment env{100};
        empty_method = Java::Autotests::PerformanceTests::empty_method(env);
        cls = jclass(env->NewGlobalRef(Java::Autotests::PerformanceTests::getClass(env)));
    }
    auto start = std::chrono::high_resolution_clock::now();
    for(int i=0; i<count; ++i){
        if(JniEnvironmentExceptionHandlerAndBlocker env{500}){
            QTJAMBI_TRY {
                env->CallStaticVoidMethod(cls, empty_method);
                env.checkException();
            } QTJAMBI_CATCH(const JavaException& exn){
                env.handleException(exn, "");
            } QTJAMBI_TRY_END
            env.releaseException();
        }
        //std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
    auto end = std::chrono::high_resolution_clock::now();
    {
        JniEnvironment env{100};
        env->DeleteGlobalRef(cls);
    }
    return std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
}

std::chrono::nanoseconds PerformanceTests::testAcuireJniEnvironmentCheckException1(int count){
    auto start = std::chrono::high_resolution_clock::now();
    for(int i=0; i<count; ++i){
        if(JniEnvironment env{500}){
            env.checkException();
        }
        //std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
    auto end = std::chrono::high_resolution_clock::now();
    return std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
}

std::chrono::nanoseconds PerformanceTests::testAcuireJniEnvironmentCheckException2(int count){
    auto start = std::chrono::high_resolution_clock::now();
    for(int i=0; i<count; ++i){
        if(JniEnvironment env{500}){
            JavaException::check(env);
        }
        //std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
    auto end = std::chrono::high_resolution_clock::now();
    return std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
}

std::chrono::nanoseconds PerformanceTests::testConvertQModelIndexInvalid(int count){
    JniEnvironment env{500};
    QModelIndex index;
    auto start = std::chrono::high_resolution_clock::now();
    for(int i=0; i<count; ++i){
        QtJambiScope scope;
        QtJambiAPI::convertModelIndexToEphemeralJavaObject(env, scope, &index);
        //std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
    auto end = std::chrono::high_resolution_clock::now();
    return std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
}

std::chrono::nanoseconds PerformanceTests::testConvertQModelIndexCopy(int count){
    JniEnvironment env{500};
    QStandardItemModel model;
    model.setColumnCount(1);
    model.setRowCount(1);
    QModelIndex index = model.index(0, 0);
    auto start = std::chrono::high_resolution_clock::now();
    for(int i=0; i<count; ++i){
        QtJambiScope scope;
        qtjambi_cast<jobject>(env, index);
        //std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
    auto end = std::chrono::high_resolution_clock::now();
    return std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
}

std::chrono::nanoseconds PerformanceTests::testConvertQModelIndex(int count){
    JniEnvironment env{500};
    QStandardItemModel model;
    model.setColumnCount(1);
    model.setRowCount(1);
    QModelIndex index = model.index(0, 0);
    auto start = std::chrono::high_resolution_clock::now();
    for(int i=0; i<count; ++i){
        QtJambiScope scope;
        QtJambiAPI::convertModelIndexToEphemeralJavaObject(env, scope, &index);
        //std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
    auto end = std::chrono::high_resolution_clock::now();
    return std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
}

std::chrono::nanoseconds PerformanceTests::testClassCall(int count, const QByteArray& className, const QByteArray& methodName){
    JniEnvironment env{500};
    jmethodID mtd;
    {
        JniLocalFrame f(env, 24);
        jclass cls = env->FindClass(className.constData());
        JavaException::check(env);
        mtd = env->GetStaticMethodID(cls, methodName.constData(), "()V");
        JavaException::check(env);
    }
    auto start = std::chrono::high_resolution_clock::now();
    for(int i=0; i<count; ++i){
        JniLocalFrame f(env, 24);
        jclass cls = env->FindClass(className.data());
        JavaException::check(env);
        env->CallStaticVoidMethod(cls, mtd);
        JavaException::check(env);
    }
    auto end = std::chrono::high_resolution_clock::now();
    return std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
}

std::chrono::nanoseconds PerformanceTests::testClassCallBuffer(int count, const QByteArray& className, const QByteArray& methodName, jobject map){
    JniEnvironment env{500};
    jmethodID mtd;
    {
        JniLocalFrame f(env, 24);
        jclass cls = env->FindClass(className.constData());
        JavaException::check(env);
        QtJambiAPI::putJavaMap(env, map, env->NewDirectByteBuffer(const_cast<char*>(className.constData()), className.capacity()), cls);
        mtd = env->GetStaticMethodID(cls, methodName.constData(), "()V");
        JavaException::check(env);
    }
    auto start = std::chrono::high_resolution_clock::now();
    for(int i=0; i<count; ++i){
        JniLocalFrame f(env, 24);
        jclass cls = jclass(Java::Runtime::Map::get(env, map, env->NewDirectByteBuffer(const_cast<char*>(className.constData()), className.capacity())));
        JavaException::check(env);
        env->CallStaticVoidMethod(cls, mtd);
        JavaException::check(env);
    }
    auto end = std::chrono::high_resolution_clock::now();
    return std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
}

std::chrono::nanoseconds PerformanceTests::testClassCallString(int count, const QByteArray& className, const QByteArray& methodName, jobject map){
    JniEnvironment env{500};
    jmethodID mtd;
    {
        JniLocalFrame f(env, 24);
        jclass cls = env->FindClass(className.constData());
        JavaException::check(env);
        QtJambiAPI::putJavaMap(env, map, env->NewStringUTF(className.constData()), cls);
        mtd = env->GetStaticMethodID(cls, methodName.constData(), "()V");
        JavaException::check(env);
    }
    auto start = std::chrono::high_resolution_clock::now();
    for(int i=0; i<count; ++i){
        JniLocalFrame f(env, 24);
        jclass cls = jclass(Java::Runtime::Map::get(env, map, env->NewStringUTF(className.constData())));
        JavaException::check(env);
        env->CallStaticVoidMethod(cls, mtd);
        JavaException::check(env);
    }
    auto end = std::chrono::high_resolution_clock::now();
    return std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
}

std::chrono::nanoseconds PerformanceTests::testClassCallHash(int count, const QByteArray& className, const QByteArray& methodName, jobject map){
    JniEnvironment env{500};
    jmethodID mtd;
    {
        JniLocalFrame f(env, 24);
        jclass cls = env->FindClass(className.constData());
        JavaException::check(env);

        QtJambiAPI::putJavaMap(env, map, Java::Runtime::Long::valueOf(env, qHash(className)), cls);
        mtd = env->GetStaticMethodID(cls, methodName.constData(), "()V");
        JavaException::check(env);
    }
    auto start = std::chrono::high_resolution_clock::now();
    for(int i=0; i<count; ++i){
        JniLocalFrame f(env, 24);
        jclass cls = jclass(Java::Runtime::Map::get(env, map, Java::Runtime::Long::valueOf(env, qHash(className))));
        JavaException::check(env);
        env->CallStaticVoidMethod(cls, mtd);
        JavaException::check(env);
    }
    auto end = std::chrono::high_resolution_clock::now();
    return std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
}

std::chrono::nanoseconds PerformanceTests::testClassCallGlobalRef(int count, const QByteArray& className, const QByteArray& methodName){
    JniEnvironment env{500};
    jmethodID mtd;
    jclass cls;
    {
        JniLocalFrame f(env, 24);
        cls = env->FindClass(className.constData());
        JavaException::check(env);
        mtd = env->GetStaticMethodID(cls, methodName.constData(), "()V");
        JavaException::check(env);
        cls = jclass(env->NewGlobalRef(cls));
    }
    auto start = std::chrono::high_resolution_clock::now();
    for(int i=0; i<count; ++i){
        JniLocalFrame f(env, 24);
        env->CallStaticVoidMethod(cls, mtd);
        JavaException::check(env);
    }
    auto end = std::chrono::high_resolution_clock::now();
    return std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
}

std::chrono::nanoseconds PerformanceTests::testClassCallLocalRef(int count, const QByteArray& className, const QByteArray& methodName){
    JniEnvironment env{500};
    jmethodID mtd;
    jclass cls;
    {
        cls = env->FindClass(className.constData());
        JavaException::check(env);
        mtd = env->GetStaticMethodID(cls, methodName.constData(), "()V");
        JavaException::check(env);
    }
    auto start = std::chrono::high_resolution_clock::now();
    for(int i=0; i<count; ++i){
        JniLocalFrame f(env, 24);
        env->CallStaticVoidMethod(cls, mtd);
        JavaException::check(env);
    }
    auto end = std::chrono::high_resolution_clock::now();
    return std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
}

std::chrono::nanoseconds PerformanceTests::testClassCallLocalFromGlobalRef(int count, const QByteArray& className, const QByteArray& methodName){
    JniEnvironment env{500};
    jmethodID mtd;
    jclass cls;
    {
        JniLocalFrame f(env, 24);
        cls = env->FindClass(className.constData());
        JavaException::check(env);
        mtd = env->GetStaticMethodID(cls, methodName.constData(), "()V");
        JavaException::check(env);
        cls = jclass(env->NewGlobalRef(cls));
    }
    auto start = std::chrono::high_resolution_clock::now();
    for(int i=0; i<count; ++i){
        JniLocalFrame f(env, 24);
        env->CallStaticVoidMethod(jclass(env->NewLocalRef(cls)), mtd);
        JavaException::check(env);
    }
    auto end = std::chrono::high_resolution_clock::now();
    return std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
}

std::chrono::nanoseconds PerformanceTests::testConvertQSize(int count){
    JniEnvironment env{500};
    QSize sz;
    const QSize& crSz = sz;
    auto start = std::chrono::high_resolution_clock::now();
    for(int i=0; i<count; ++i){
        qtjambi_cast<jobject>(env, crSz);
        //std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
    auto end = std::chrono::high_resolution_clock::now();
    return std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
}

std::chrono::nanoseconds PerformanceTests::testConvertQString(int count){
    JniEnvironment env{500};
    QString strg;
    strg.fill(u'#', 512);
    const QString& crStrg = strg;
    auto start = std::chrono::high_resolution_clock::now();
    for(int i=0; i<count; ++i){
        qtjambi_cast<jstring>(env, crStrg);
        //std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
    auto end = std::chrono::high_resolution_clock::now();
    return std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
}

std::chrono::nanoseconds PerformanceTests::testRowCount(QAbstractItemModel* host, int count, const QModelIndex &parent) {
    auto start = std::chrono::high_resolution_clock::now();
    for(int i=0; i<count; ++i){
        host->rowCount(parent);
        //std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
    auto end = std::chrono::high_resolution_clock::now();
    return std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
}

std::chrono::nanoseconds PerformanceTests::testPlain(AbstractVirtualHost* host, int count) {
    auto start = std::chrono::high_resolution_clock::now();
    for(int i=0; i<count; ++i){
        host->test();
        //std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
    auto end = std::chrono::high_resolution_clock::now();
    return std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
}

std::chrono::nanoseconds PerformanceTests::testQModelIndex(AbstractVirtualHost* host, int count) {
    QStandardItemModel model;
    model.setColumnCount(1);
    model.setRowCount(1);
    QModelIndex index = model.index(0, 0);
    auto start = std::chrono::high_resolution_clock::now();
    for(int i=0; i<count; ++i){
        host->test(index);
        //std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
    auto end = std::chrono::high_resolution_clock::now();
    return std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
}

std::chrono::nanoseconds PerformanceTests::testQModelIndexInvalid(AbstractVirtualHost* host, int count) {
    QModelIndex index;
    auto start = std::chrono::high_resolution_clock::now();
    for(int i=0; i<count; ++i){
        host->test(index);
        //std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
    auto end = std::chrono::high_resolution_clock::now();
    return std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
}

std::chrono::nanoseconds PerformanceTests::testQString(AbstractVirtualHost* host, int count) {
    QString string;
    string.fill(u'#', 512);
    auto start = std::chrono::high_resolution_clock::now();
    for(int i=0; i<count; ++i){
        host->test(string);
        //std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
    auto end = std::chrono::high_resolution_clock::now();
    return std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
}

std::chrono::nanoseconds PerformanceTests::testQSize1(AbstractVirtualHost* host, int count) {
    QSize size{512,512};
    auto start = std::chrono::high_resolution_clock::now();
    for(int i=0; i<count; ++i){
        host->test(size);
        //std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
    auto end = std::chrono::high_resolution_clock::now();
    return std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
}

std::chrono::nanoseconds PerformanceTests::testQSize2(AbstractVirtualHost* host, int count) {
    QSize size1{512,512};
    QSize size2{12,512};
    auto start = std::chrono::high_resolution_clock::now();
    for(int i=0; i<count; ++i){
        host->test(size1, size2);
        //std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
    auto end = std::chrono::high_resolution_clock::now();
    return std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
}

std::chrono::nanoseconds PerformanceTests::testQSize3(AbstractVirtualHost* host, int count) {
    QSize size1{512,512};
    QSize size2{12,512};
    QSize size3{2,512};
    auto start = std::chrono::high_resolution_clock::now();
    for(int i=0; i<count; ++i){
        host->test(size1, size2, size3);
        //std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
    auto end = std::chrono::high_resolution_clock::now();
    return std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
}

std::chrono::nanoseconds PerformanceTests::testQSize4(AbstractVirtualHost* host, int count) {
    QSize size1{512,512};
    QSize size2{12,512};
    QSize size3{2,512};
    QSize size4{512,12};
    auto start = std::chrono::high_resolution_clock::now();
    for(int i=0; i<count; ++i){
        host->test(size1, size2, size3, size4);
        //std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
    auto end = std::chrono::high_resolution_clock::now();
    return std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
}

std::chrono::nanoseconds PerformanceTests::testQSize5(AbstractVirtualHost* host, int count) {
    QSize size1{512,512};
    QSize size2{12,512};
    QSize size3{2,512};
    QSize size4{512,12};
    QSize size5{512,2};
    auto start = std::chrono::high_resolution_clock::now();
    for(int i=0; i<count; ++i){
        host->test(size1, size2, size3, size4, size5);
        //std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
    auto end = std::chrono::high_resolution_clock::now();
    return std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
}

std::chrono::nanoseconds PerformanceTests::testInt(AbstractVirtualHost* host, int count) {
    int integer = 512;
    auto start = std::chrono::high_resolution_clock::now();
    for(int i=0; i<count; ++i){
        host->test(integer);
        //std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
    auto end = std::chrono::high_resolution_clock::now();
    return std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
}

struct ThreadLocalValue{
    ThreadLocalValue() : m_thread_id(std::this_thread::get_id()){
#ifdef TEST_PRINT_THREAD_LOCAL_LIFETINE
        QThread* currentThread = QThread::currentThread();
        quintptr currentThreadId = quintptr(QThread::currentThreadId());
        std::cout << "ThreadLocalValue created in thread " << m_thread_id << "; current thread: " << currentThread << "; current thread ID: " << currentThreadId << std::endl;
        std::flush(std::cout);
#endif
    }
    ~ThreadLocalValue(){
#ifdef TEST_PRINT_THREAD_LOCAL_LIFETINE
        QThread* currentThread = QThread::currentThread();
        quintptr currentThreadId = quintptr(QThread::currentThreadId());
        std::thread::id this_id = std::this_thread::get_id();
        if(this_id!=m_thread_id)
            std::cout << "ThreadLocalValue associated to thread " << m_thread_id << " destroyed in thread " << this_id << "; current thread: " << currentThread << "; current thread ID: " << currentThreadId << std::endl;
        else
            std::cout << "ThreadLocalValue destroyed in thread " << this_id << "; current thread: " << currentThread << "; current thread ID: " << currentThreadId << std::endl;
        std::flush(std::cout);
#endif
    }
    void hello(){}
    std::thread::id m_thread_id;
    static thread_local ThreadLocalValue value;
};
thread_local ThreadLocalValue ThreadLocalValue::value;

struct ThreadStorageValue{
    ThreadStorageValue() : m_thread_id(std::this_thread::get_id()) {
#ifdef TEST_PRINT_THREAD_LOCAL_LIFETINE
        QThread* currentThread = QThread::currentThread();
        quintptr currentThreadId = quintptr(QThread::currentThreadId());
        std::cout << "ThreadStorageValue created in thread " << m_thread_id << "; current thread: " << currentThread << "; current thread ID: " << currentThreadId << std::endl;
        std::flush(std::cout);
#endif
    }
    ~ThreadStorageValue(){
#ifdef TEST_PRINT_THREAD_LOCAL_LIFETINE
        QThread* currentThread = QThread::currentThread();
        std::thread::id this_id = std::this_thread::get_id();
        quintptr currentThreadId = quintptr(QThread::currentThreadId());
        if(this_id!=m_thread_id)
            std::cout << "ThreadStorageValue associated to thread " << m_thread_id << " destroyed in thread " << this_id << "; current thread: " << currentThread << "; current thread ID: " << currentThreadId << std::endl;
        else
            std::cout << "ThreadStorageValue destroyed in thread " << this_id << "; current thread: " << currentThread << "; current thread ID: " << currentThreadId << std::endl;
        std::flush(std::cout);
#endif
    }
    void hello(){}
    std::thread::id m_thread_id;
    static QThreadStorage<ThreadStorageValue> value;
};
QThreadStorage<ThreadStorageValue> ThreadStorageValue::value;

void PerformanceTests::acquireThreadLocal(){
    ThreadLocalValue::value.hello();
}

void PerformanceTests::acquireQThreadStorage(){
    ThreadStorageValue::value.localData().hello();
}

std::chrono::nanoseconds PerformanceTests::testAcquireThreadLocal(int count){
    auto start = std::chrono::high_resolution_clock::now();
    for(int i=0; i<count; ++i){
        ThreadLocalValue::value.hello();
    }
    auto end = std::chrono::high_resolution_clock::now();
    return std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
}

std::chrono::nanoseconds PerformanceTests::testAcquireQThreadStorage(int count){
    auto start = std::chrono::high_resolution_clock::now();
    for(int i=0; i<count; ++i){
        ThreadStorageValue::value.localData().hello();
    }
    auto end = std::chrono::high_resolution_clock::now();
    return std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
}
