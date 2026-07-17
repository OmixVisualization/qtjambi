/****************************************************************************
**
** Copyright (C) 2009-2026 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
** Copyright (C) 1992-2009 Nokia. All rights reserved.
**
** This file is part of Qt Jambi.
**
** ** $BEGIN_LICENSE$
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

#include "global.h"

#include <QtCore/QDebug>
#include <QtCore/QtCore>
#include <QtGui/QtGui>
#if QT_CONFIG(vulkan) && __has_include(<vulkan/vulkan.h>)
#include <QtGui/QVulkanInstance>
#endif
#ifndef QTJAMBI_NO_QUICK
#include <QtQuick/QQuickItem>
#endif
#ifndef QTJAMBI_NO_WIDGETS
#include <QtWidgets/QtWidgets>
#endif
#include <QtJambi/QtJambiAPI>
#include <QtJambi/registryapi.h>
#include "general.h"
#include <QtJambi/Cast>
#include <QtJambi/ModelCast>
#include <QtJambi/FutureCast>
#include <QtJambi/ArrayCast>
#include <QtJambi/EnumCast>
#include <QtJambi/ArithmeticCast>
#include <QtJambi/SmartPointerCast>
#include <QtJambi/Template1Cast>
#include <QtJambi/Template2Cast>
#include <QtJambi/Template3Cast>
#include <QtJambi/Template4Cast>
#include <QtJambi/Template5Cast>
#include <QtJambi/ContainerCast>
#include <QtJambi/StringAPI>
#include <QtJambi/BufferAPI>
#include <QtJambiQml/Cast>
#include <QtJambiCore/Cast>
#include <QtJambi/QList>
#include <QtJambi/QVariantList>
#include <QtJambi/QObjectList>
#include <QtJambi/QStringList>
#include <QtJambi/QByteArrayList>
#include <QtJambi/QPair>
#include <QtJambi/QSet>
#include <QtJambi/QMap>
#include <QtJambi/QHash>
#include <QtJambi/QMultiMap>
#include <QtJambi/QMultiHash>
#include <cstdlib>
#ifdef Q_OS_MAC
#include <signal.h>
#include <unistd.h>
#endif

QT_WARNING_DISABLE_DEPRECATED
QT_WARNING_DISABLE_GCC("-Wdeprecated-declarations")

class UnknownKey{
public:
    UnknownKey(int){}
};

//bool operator <(const UnknownKey&,const UnknownKey&){return false;}
//bool operator ==(const UnknownKey&,const UnknownKey&){return false;}
uint qHash(const UnknownKey&){return 0;}

class UnknownClass{
public:
    UnknownClass(int){}
};
bool operator ==(const UnknownClass&,const UnknownClass&){return false;}
//bool operator <(const UnknownClass&,const UnknownClass&){return false;}
uint qHash(const UnknownClass&){return 0;}
#if QT_VERSION < QT_VERSION_CHECK(6, 8, 0)
uint qHash(const QMap<QString,int>&){return 0;}
#endif

#ifndef QTJAMBI_NO_WIDGETS
class CalendarWidgetAccessor: public QCalendarWidget {
public:
    void paintCellAccess(QPainter *p) {
        paintCell(p, QRect(), QDate::currentDate());
    }
};
void General::callPaintCell(QCalendarWidget *w, QPainter *painter) {
    QPainter localPainter;
    if (!painter)
        painter = &localPainter;

    static_cast<CalendarWidgetAccessor *>(w)->paintCellAccess(painter);
}

void General::callPaintCellNull(QCalendarWidget *w) {
    static_cast<CalendarWidgetAccessor *>(w)->paintCellAccess(nullptr);
}
#endif

bool General::canVulkan(){
#if QT_CONFIG(vulkan)
    return true;
#else
    return false;
#endif
}

bool General::hasVulkanInstance(QWindow* window){
#if QT_CONFIG(vulkan)
    return window->vulkanInstance();
#else
    Q_UNUSED(window)
    return false;
#endif
}

bool General::canCreateVulkanInstance(){
#if QT_CONFIG(vulkan) && __has_include(<vulkan/vulkan.h>)
    QVulkanInstance inst;
    return inst.create();
#else
    return false;
#endif
}

class Runner : public QThread{
public: void runMe(){run();}
};

#ifdef Q_OS_MAC
QByteArray StringSysctlByName(const char* name);
#endif

QString General::stringSysctlByName(const char* name){
    Q_UNUSED(name)
#ifdef Q_OS_MAC
    return QString::fromUtf8(StringSysctlByName(name));
#else
    return {};
#endif
}

void General::run(QThread* runnable){
    if(runnable)static_cast<Runner*>(runnable)->runMe();
}

void General::run(QRunnable* runnable){
    if(runnable)runnable->run();
}

void General::std_terminate(){
    std::terminate();
}

void General::std_exit(int i){
    std::exit(i);
}

void General::std_abort(){
    std::abort();
}

void General::c_kill(){
#ifdef Q_OS_MAC
    kill(getpid(), SIGKILL);
#endif
}

void General::c_exit(int i){
    exit(i);
}

void General::c_abort(){
    abort();
}

void General::badAccess(){
    qintptr p = 0x00CAFEBABE00;
    QMutex* mutex = reinterpret_cast<QMutex*>(p);
    mutex->lock();
    mutex->unlock();
}

void General::uncaughtException(){
    throw "General::uncaughtException";
}

void General::badVirtual(){
    qintptr p = 0x00CAFEBABE00;
    QObject* object = reinterpret_cast<QObject*>(p);
    QEvent event(QEvent::Enter);
    object->event(&event);
}

void General::qtjambi_jni_test(JNIEnv * env, jobject object){
    jclass cls = env->FindClass("Ljava/lang/String;");
    jobject null = nullptr;
    bool isInstance1 = env->IsInstanceOf(null, cls);
    bool isInstance2 = env->IsInstanceOf(object, cls);
    jfieldID mtd = env->GetFieldID(cls, "value", "[B");
    jobject l = env->GetObjectField(object, mtd);
    printf("isInstance1=%i\n", isInstance1);
    printf("isInstance2=%i\n", isInstance2);
    printf("result=%p\n", l);
}

void fun0(){
    qInfo() << "Hello World!";
}

void fun1(int i){
    qInfo() << "arg: " << i;
}

void fun2(bool b){
    qInfo() << "arg: " << b;
}

void fun3(double d){
    qInfo() << "arg: " << d;
}

void fun4(float f){
    qInfo() << "arg: " << f;
}

void fun5(QChar c){
    qInfo() << "arg: " << c;
}

void fun6(QString strg){
    qInfo() << "arg: " << strg;
}

void fun7(const QString& strg){
    qInfo() << "arg: " << strg;
}

void fun8(QObject* obj){
    qInfo() << "arg: " << (obj ? obj->objectName() : "null");
}

void fun9(std::nullptr_t n){
    qInfo() << "arg: " << (n==nullptr ? "null" : "not null");
}

void fun10(JNIEnv* env){
    qInfo() << "arg: " << quint64(env);
}

void fun11(char, int i){
    qInfo() << "arg: " << i;
}

void fun12(char, bool b){
    qInfo() << "arg: " << b;
}

void fun13(int, double d){
    qInfo() << "arg: " << d;
}

void fun14(long long, float f){
    qInfo() << "arg: " << f;
}

void fun15(char, QChar c){
    qInfo() << "arg: " << c;
}

void fun16(char, QString strg){
    qInfo() << "arg: " << strg;
}

void fun17(char, const QString& strg){
    qInfo() << "arg: " << strg;
}

void fun18(char, QObject* obj){
    qInfo() << "arg: " << (obj ? obj->objectName() : "null");
}

void fun19(char, std::nullptr_t n){
    qInfo() << "arg: " << (n==nullptr ? "null" : "not null");
}

void fun20(char, JNIEnv* env){
    qInfo() << "arg: " << quint64(env);
}

int fun21(int i){
    return i;
}

bool fun22(bool b){
    return b;
}

double fun23(double d){
    return d;
}

float fun24(float f){
    return f;
}

QChar fun25(QChar c){
    return c;
}

QString fun26(QString strg){
    return strg;
}

QString fun27(const QString& strg){
    return strg;
}

QObject* fun28(QObject* obj){
    return obj;
}

QColor fun29(){
    return QColor(Qt::darkBlue);
}

QList<QVariant> fun30(QString s, QObject* o, qint64 j, qint16 i16, double d, const QColor& color, const QStringList& list, QRectF rect, QFont font){
    return {QVariant::fromValue(s),
                QVariant::fromValue(o),
                QVariant::fromValue(j),
                QVariant::fromValue(i16),
                QVariant::fromValue(d),
                QVariant::fromValue(color),
                QVariant::fromValue(list),
                QVariant::fromValue(rect),
                QVariant::fromValue(font)};
}

Qt::AlignmentFlag fun31(Qt::AlignmentFlag flag){
    return flag;
}

Qt::Alignment fun32(Qt::Alignment flag){
    return flag;
}

QDir fun33(const QDir& dir){
    return dir;
}

QVariant fun34(const QVariant& value){
    return value;
}

char16_t fun35(char16_t value){
    return value;
}

wchar_t fun36(wchar_t value){
    return value;
}

void fun37(char c, short s, int i, long long j, Qt::GlobalColor col, Qt::Alignment flag){
    qInfo() << c << " " << s << " " << i << " " << j << " " << col << " " << flag;
}

void fun38(const QDir& arg){
    qInfo() << arg;
}

void fun39(const QColor& arg){
    qInfo() << arg;
}

void fun40(QColor& arg){
    qInfo() << arg;
    arg = QColor(Qt::darkMagenta);
}

void fun41(const QRect& arg){
    qInfo() << arg;
}

void fun42(QRect arg){
    qInfo() << arg;
}

void fun43(QDir arg){
    qInfo() << arg;
}

QRect fun44(){
    qInfo() << "here I am";
    return QRect(8,2,4,9);
}

Qt::Alignment fun45(){
    qInfo() << "here I am";
    return Qt::Alignment(Qt::AlignVCenter | Qt::AlignJustify);
}

void fun46(QMap<QString,QRect> arg){
    qInfo() << arg;
}

void fun47(const QMap<QString,QRect>& arg){
    qInfo() << arg;
}

void fun48(QString& strg){
    qInfo() << strg;
    strg = QLatin1String("Test");
}

void fun49(int* array, int size){
    for(int i=0; i<size; ++i){
        qInfo() << "array[" << i << "] = " << array[i];
        array[i] = i;
    }
}

void fun50(QRectF* array, int size){
    for(int i=0; i<size; ++i){
        qInfo() << "array[" << i << "] = " << array[i];
        array[i] = QRectF(0,0,i+1,i*2+1);
    }
}

void fun51(const char* format,...){
    va_list args;
    va_start(args, format);
    vprintf(format, args);
    va_end(args);
    fflush(stdout);
}

typedef void (*Runnable)();

Runnable fun52(Runnable runnable){
    if(runnable)
        runnable();
    return runnable;
}

typedef void (*UnknownFunction1)(int);

void fun53(UnknownFunction1 function){
    if(function)
        function(987654321);
}

typedef qlonglong(*UnknownFunction2)(double, int, const QColor&, Qt::Alignment);

qlonglong fun54(UnknownFunction2 function){
    if(function)
        return function(9.12345, 10, Qt::blue, Qt::AlignLeft | Qt::AlignTop);
    return 0;
}

typedef UnknownFunction1(*UnknownFunction3)(UnknownFunction2 f2);

UnknownFunction1 fun55(UnknownFunction3 function){
    if(function)
        return function([](double d, int i, const QColor& c, Qt::Alignment a) -> qlonglong{
            return qlonglong((i + int(c.rgba()) + int(a)) * d);
        });
    return nullptr;
}

QFunctionPointer FunctionalTest::getFunction(int id){
    static QFunctionPointer functionPointers[] = {
        reinterpret_cast<QFunctionPointer>(&fun0),
        reinterpret_cast<QFunctionPointer>(&fun1),
        reinterpret_cast<QFunctionPointer>(&fun2),
        reinterpret_cast<QFunctionPointer>(&fun3),
        reinterpret_cast<QFunctionPointer>(&fun4),
        reinterpret_cast<QFunctionPointer>(&fun5),
        reinterpret_cast<QFunctionPointer>(&fun6),
        reinterpret_cast<QFunctionPointer>(&fun7),
        reinterpret_cast<QFunctionPointer>(&fun8),
        reinterpret_cast<QFunctionPointer>(&fun9),
        reinterpret_cast<QFunctionPointer>(&fun10),
        reinterpret_cast<QFunctionPointer>(&fun11),
        reinterpret_cast<QFunctionPointer>(&fun12),
        reinterpret_cast<QFunctionPointer>(&fun13),
        reinterpret_cast<QFunctionPointer>(&fun14),
        reinterpret_cast<QFunctionPointer>(&fun15),
        reinterpret_cast<QFunctionPointer>(&fun16),
        reinterpret_cast<QFunctionPointer>(&fun17),
        reinterpret_cast<QFunctionPointer>(&fun18),
        reinterpret_cast<QFunctionPointer>(&fun19),
        reinterpret_cast<QFunctionPointer>(&fun20),
        reinterpret_cast<QFunctionPointer>(&fun21),
        reinterpret_cast<QFunctionPointer>(&fun22),
        reinterpret_cast<QFunctionPointer>(&fun23),
        reinterpret_cast<QFunctionPointer>(&fun24),
        reinterpret_cast<QFunctionPointer>(&fun25),
        reinterpret_cast<QFunctionPointer>(&fun26),
        reinterpret_cast<QFunctionPointer>(&fun27),
        reinterpret_cast<QFunctionPointer>(&fun28),
        reinterpret_cast<QFunctionPointer>(&fun29),
        reinterpret_cast<QFunctionPointer>(&fun30),
        reinterpret_cast<QFunctionPointer>(&fun31),
        reinterpret_cast<QFunctionPointer>(&fun32),
        reinterpret_cast<QFunctionPointer>(&fun33),
        reinterpret_cast<QFunctionPointer>(&fun34),
        reinterpret_cast<QFunctionPointer>(&fun35),
        reinterpret_cast<QFunctionPointer>(&fun36),
        reinterpret_cast<QFunctionPointer>(&fun37),
        reinterpret_cast<QFunctionPointer>(&fun38),
        reinterpret_cast<QFunctionPointer>(&fun39),
        reinterpret_cast<QFunctionPointer>(&fun40),
        reinterpret_cast<QFunctionPointer>(&fun41),
        reinterpret_cast<QFunctionPointer>(&fun42),
        reinterpret_cast<QFunctionPointer>(&fun43),
        reinterpret_cast<QFunctionPointer>(&fun44),
        reinterpret_cast<QFunctionPointer>(&fun45),
        reinterpret_cast<QFunctionPointer>(&fun46),
        reinterpret_cast<QFunctionPointer>(&fun47),
        reinterpret_cast<QFunctionPointer>(&fun48),
        reinterpret_cast<QFunctionPointer>(&fun49),
        reinterpret_cast<QFunctionPointer>(&fun50),
        reinterpret_cast<QFunctionPointer>(&fun51),
        reinterpret_cast<QFunctionPointer>(&fun52),
        reinterpret_cast<QFunctionPointer>(&fun53),
        reinterpret_cast<QFunctionPointer>(&fun54),
        reinterpret_cast<QFunctionPointer>(&fun55)
    };
    if(id>=0 && id<int(sizeof(functionPointers))){
        return functionPointers[id];
    }
    return nullptr;
}
