/****************************************************************************
**
** Copyright (C) 2009-2024 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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

#include <QtCore/private/qcoreapplication_p.h>
#include <QtCore/private/qthread_p.h>
#include "exception.h"
#include "java_p.h"
#include "registryutil_p.h"
#include "guiapi.h"
#include "qtjambilink_p.h"
#include "utils_p.h"

struct UIInitialCheck{
    typedef void (*WidgetConstructorCheck)(JNIEnv *, const std::type_info&, const QObject*);
    typedef void (*WindowConstructorCheck)(JNIEnv *, const std::type_info&, const QObject*);
    typedef void (*ConstructorCheck)(JNIEnv *, const std::type_info&);
    typedef void (*UseCheck)(JNIEnv *, const std::type_info&);
    typedef void (*ArgumentCheck)(JNIEnv *, const char*, const std::type_info&);
    typedef void (*QObjectThreadCheck)(JNIEnv *, const QObject*);
    typedef void (*QObjectArgumentThreadCheck)(JNIEnv *, const char*, const QObject*);
    typedef void (*ValueArgumentThreadCheck)(JNIEnv *, const char*, const std::type_info&, const QObject*);
    typedef void (*ValueThreadCheck)(JNIEnv *, const std::type_info&, const QObject*);
    typedef void (*UIArgumentThreadCheck)(JNIEnv *, const char*, const std::type_info&);
    typedef void (*GeneralArgumentThreadCheck)(JNIEnv *, const char*, const std::type_info&, const void*);
    typedef void (*GeneralThreadCheck)(JNIEnv *, const std::type_info&, const void*);
    typedef bool (*EventNotify)(QObject *receiver, QEvent *event, bool* result);
    static WindowConstructorCheck windowConstructorCheck;
    static WidgetConstructorCheck widgetConstructorCheck;
    static ConstructorCheck uiConstructorCheck;
    static ConstructorCheck pixmapConstructorCheck;
    static UseCheck pixmapUseCheck;
    static UseCheck uiThreadCheck;
    static ArgumentCheck pixmapArgumentThreadCheck;
    static PtrOwnerFunction getPixmapOwner;
    static GuiAPI::ThreadedPixmapsChecker threadedPixmapsChecker;
    static QObjectThreadCheck objectThreadCheck;
    static QObjectArgumentThreadCheck objectArgumentThreadCheck;
    static ValueArgumentThreadCheck valueArgumentThreadCheck;
    static UIArgumentThreadCheck uiArgumentThreadCheck;
    static GeneralArgumentThreadCheck generalArgumentThreadCheck;
    static QObjectThreadCheck objectConstructorThreadCheck;
    static ValueThreadCheck valueConstructorThreadCheck;
    static GeneralThreadCheck generalConstructorThreadCheck;
    static EventNotify eventNotify;

    static void trivial(JNIEnv *, const std::type_info&);
    static void trivial(JNIEnv *, const std::type_info&, const QObject*);
    static void trivial(JNIEnv *, const char*, const std::type_info&);
    static void trivial(JNIEnv *, const char*, const std::type_info&, const void*);
    static void trivial(JNIEnv *, const std::type_info&, const void*);
    static void trivial(JNIEnv *, const QObject*);
    static void trivial(JNIEnv *, const char*, const QObject*);
    static void trivial(JNIEnv *env, const char*, const std::type_info&, const QObject*);
    static bool trivial(QObject *, QEvent *, bool*);
    static const QObject* trivial(const void * ptr);
    static void initialWindowConstructorCheck(JNIEnv *, const std::type_info&, const QObject*);
    static void initialWidgetConstructorCheck(JNIEnv *, const std::type_info&, const QObject*);
    static void initialPixmapConstructorCheck(JNIEnv *, const std::type_info&);
    static void initialUIConstructorCheck(JNIEnv *, const std::type_info&);
    static void initialPixmapUseCheck(JNIEnv *, const std::type_info&);
    static void initialPixmapArgumentCheck(JNIEnv *, const char*, const std::type_info&);
    static bool initialEventNotify(QObject *receiver, QEvent *event, bool* result);
    static bool enabledEventNotify(QObject *receiver, QEvent *event, bool* result);
    static void uiConstructorThreadCheck(JNIEnv *, const std::type_info&);
    static void windowConstructorThreadCheck(JNIEnv *, const std::type_info&, const QObject*);
    static void widgetConstructorThreadCheck(JNIEnv *, const std::type_info&, const QObject*);
    static void pixmapUseThreadCheck(JNIEnv *, const std::type_info&);
    static void enabledPixmapArgumentThreadCheck(JNIEnv *, const char*, const std::type_info&);
    static const QObject* initialGetPixmapOwner(const void * ptr);
    static void enabledQObjectThreadCheck(JNIEnv *env, const QObject* object);
    static void enabledQObjectConstructorThreadCheck(JNIEnv *env, const QObject*);
    static void enabledValueConstructorThreadCheck(JNIEnv *env, const std::type_info&, const QObject*);
    static void enabledGeneralConstructorThreadCheck(JNIEnv *env, const std::type_info&, const void*);
    static void enabledQObjectArgumentThreadCheck(JNIEnv *env, const char* argumentName, const QObject* argument);
    static void enabledValueArgumentThreadCheck(JNIEnv *env, const char* argumentName, const std::type_info& argumentType, const QObject* argumentOwner);
    static void enabledGeneralArgumentThreadCheck(JNIEnv *env, const char* argumentName, const std::type_info& argumentType, const void* ptr);
    static void enabledGeneralThreadCheck(JNIEnv *env, const std::type_info& argumentType, const void* ptr);
    static void enabledUIArgumentThreadCheck(JNIEnv *env, const char* argumentName, const std::type_info& argumentType);
    static void enabledUIThreadCheck(JNIEnv *, const std::type_info&);
};

UIInitialCheck::WindowConstructorCheck UIInitialCheck::windowConstructorCheck = &UIInitialCheck::trivial;
UIInitialCheck::WidgetConstructorCheck UIInitialCheck::widgetConstructorCheck = &UIInitialCheck::trivial;
UIInitialCheck::ConstructorCheck UIInitialCheck::pixmapConstructorCheck = &UIInitialCheck::trivial;
UIInitialCheck::ConstructorCheck UIInitialCheck::uiConstructorCheck = &UIInitialCheck::trivial;
UIInitialCheck::UseCheck UIInitialCheck::pixmapUseCheck = &UIInitialCheck::trivial;
UIInitialCheck::UseCheck UIInitialCheck::uiThreadCheck = &UIInitialCheck::trivial;
UIInitialCheck::ArgumentCheck UIInitialCheck::pixmapArgumentThreadCheck = &UIInitialCheck::trivial;
UIInitialCheck::QObjectThreadCheck UIInitialCheck::objectThreadCheck = &UIInitialCheck::trivial;
UIInitialCheck::QObjectArgumentThreadCheck UIInitialCheck::objectArgumentThreadCheck = &UIInitialCheck::trivial;
UIInitialCheck::ValueArgumentThreadCheck UIInitialCheck::valueArgumentThreadCheck = &UIInitialCheck::trivial;
UIInitialCheck::UIArgumentThreadCheck UIInitialCheck::uiArgumentThreadCheck = &UIInitialCheck::trivial;
UIInitialCheck::GeneralArgumentThreadCheck UIInitialCheck::generalArgumentThreadCheck = &UIInitialCheck::trivial;
UIInitialCheck::QObjectThreadCheck UIInitialCheck::objectConstructorThreadCheck = &UIInitialCheck::trivial;
UIInitialCheck::ValueThreadCheck UIInitialCheck::valueConstructorThreadCheck = &UIInitialCheck::trivial;
UIInitialCheck::GeneralThreadCheck UIInitialCheck::generalConstructorThreadCheck = &UIInitialCheck::trivial;

PtrOwnerFunction UIInitialCheck::getPixmapOwner = &UIInitialCheck::initialGetPixmapOwner;
GuiAPI::ThreadedPixmapsChecker UIInitialCheck::threadedPixmapsChecker = nullptr;
UIInitialCheck::EventNotify UIInitialCheck::eventNotify = &UIInitialCheck::initialEventNotify;

void GuiAPI::installThreadedPixmapsChecker(ThreadedPixmapsChecker threadedPixmapsChecker){
    UIInitialCheck::threadedPixmapsChecker = threadedPixmapsChecker;
}

void UIInitialCheck::trivial(JNIEnv *, const std::type_info&){}
void UIInitialCheck::trivial(JNIEnv *, const std::type_info&, const QObject*){}
void UIInitialCheck::trivial(JNIEnv *, const char*, const std::type_info&){}
void UIInitialCheck::trivial(JNIEnv *, const std::type_info&, const void *){}
void UIInitialCheck::trivial(JNIEnv *, const QObject*){}
const QObject* UIInitialCheck::trivial(const void *){return nullptr;}
void UIInitialCheck::trivial(JNIEnv *, const char*, const QObject*){}
void UIInitialCheck::trivial(JNIEnv *, const char*, const std::type_info&, const QObject*){}
void UIInitialCheck::trivial(JNIEnv *, const char*, const std::type_info&, const void*){}
bool UIInitialCheck::trivial(QObject *, QEvent *, bool*){return false;}

bool UIInitialCheck::initialEventNotify(QObject *receiver, QEvent *event, bool* result){
    QCoreApplication* instance = QCoreApplication::instance();
    if(instance) {
        if(dynamic_cast<QtJambiShellInterface*>(instance)){
            UIInitialCheck::eventNotify = &UIInitialCheck::trivial;
        }else{
            UIInitialCheck::eventNotify = &UIInitialCheck::enabledEventNotify;
            return enabledEventNotify(receiver, event, result);
        }
    }
    return false;
}

bool UIInitialCheck::enabledEventNotify(QObject *receiver, QEvent *event, bool* result){
    QObjectPrivate *d = QObjectPrivate::get(receiver);
    QThreadData *threadData = d->threadData;
    QScopedScopeLevelCounter scopeLevelCounter(threadData);
    QtJambiExceptionBlocker __qt_exceptionHandler;
    *result = QCoreApplication::instance()->notify(receiver, event);
    __qt_exceptionHandler.release(nullptr);
    return true;
}

void UIInitialCheck::uiConstructorThreadCheck(JNIEnv *env, const std::type_info& constructedType){
    QThread* mainThread = QCoreApplicationPrivate::theMainThread.loadRelaxed();
    QThread* currentThread = QThread::currentThread();
    if(currentThread!=mainThread){
        JavaException::raiseQThreadAffinityException(env, QStringLiteral("%1 created outside main thread").arg(getQtName(constructedType)) QTJAMBI_STACKTRACEINFO, nullptr,
                                                    currentThread, mainThread );
    }
}

void UIInitialCheck::pixmapUseThreadCheck(JNIEnv *env, const std::type_info& constructedType){
    QThread* mainThread = QCoreApplicationPrivate::theMainThread.loadRelaxed();
    QThread* currentThread = QThread::currentThread();
    if(currentThread!=mainThread){
        const char* typeName = getQtName(constructedType);
        if(QLatin1String("QPixmap")==typeName || QLatin1String("QBitmap")==typeName){
            JavaException::raiseQThreadAffinityException(env, QStringLiteral("%1 used from outside main thread").arg(typeName) QTJAMBI_STACKTRACEINFO, nullptr,
                                                        currentThread, mainThread );
        }else{
            JavaException::raiseQThreadAffinityException(env, QStringLiteral("QPixmap used in %1 from outside main thread").arg(typeName) QTJAMBI_STACKTRACEINFO, nullptr,
                                                        currentThread, mainThread );
        }
    }
}

void UIInitialCheck::enabledPixmapArgumentThreadCheck(JNIEnv *env, const char* argumentName, const std::type_info& argumentType){
    QThread* mainThread = QCoreApplicationPrivate::theMainThread.loadRelaxed();
    QThread* currentThread = QThread::currentThread();
    if(currentThread!=mainThread){
        JavaException::raiseQThreadAffinityException(env, QStringLiteral("%1 used as argument '%2' from outside main thread").arg(getQtName(argumentType)).arg(argumentName) QTJAMBI_STACKTRACEINFO ,
                                                     nullptr,
                                                     mainThread, currentThread);
    }
}

void UIInitialCheck::initialPixmapConstructorCheck(JNIEnv *env, const std::type_info& constructedType){
    QCoreApplication* instance = QCoreApplication::instance();
    if(!instance) {
        Java::Runtime::IllegalStateException::throwNew(env, QStringLiteral("Cannot create %1 before initializing QGuiApplication.").arg(getQtName(constructedType)) QTJAMBI_STACKTRACEINFO);
    }else{
        if(!instance->inherits("QGuiApplication"))
            Java::Runtime::IllegalStateException::throwNew(env, QStringLiteral("Cannot create %1 without QGuiApplication (instead of %2).").arg(getQtName(constructedType)).arg(instance->metaObject()->className()) QTJAMBI_STACKTRACEINFO);
        else{
            if(threadedPixmapsChecker()){
                pixmapConstructorCheck = &trivial;
            }else{
                pixmapConstructorCheck = &uiConstructorThreadCheck;
            }
            qAddPostRoutine([](){ UIInitialCheck::pixmapConstructorCheck = &UIInitialCheck::initialPixmapConstructorCheck; });
            pixmapConstructorCheck(env, constructedType);
        }
    }
}

const QObject* UIInitialCheck::initialGetPixmapOwner(const void * ptr){
    if(threadedPixmapsChecker()){
        getPixmapOwner = &trivial;
    }else{
        getPixmapOwner = &RegistryAPI::mainThreadOwner;
    }
    return getPixmapOwner(ptr);
}

void UIInitialCheck::initialUIConstructorCheck(JNIEnv *env, const std::type_info& constructedType){
    QCoreApplication* instance = QCoreApplication::instance();
    if(!instance) {
        Java::Runtime::IllegalStateException::throwNew(env, QStringLiteral("Cannot create %1 before initializing QGuiApplication.").arg(getQtName(constructedType)) QTJAMBI_STACKTRACEINFO);
    }else{
        if(!instance->inherits("QGuiApplication"))
            Java::Runtime::IllegalStateException::throwNew(env, QStringLiteral("Cannot create %1 without QGuiApplication (instead of %2).").arg(getQtName(constructedType)).arg(instance->metaObject()->className()) QTJAMBI_STACKTRACEINFO);
        else{
            uiConstructorCheck = &uiConstructorThreadCheck;
            qAddPostRoutine([](){ UIInitialCheck::uiConstructorCheck = &UIInitialCheck::initialUIConstructorCheck; });
            uiConstructorCheck(env, constructedType);
        }
    }
}

void UIInitialCheck::initialPixmapUseCheck(JNIEnv *env, const std::type_info& usedType){
    if(threadedPixmapsChecker()){
        pixmapUseCheck = &trivial;
    }else{
        pixmapUseCheck = &pixmapUseThreadCheck;
    }
    qAddPostRoutine([](){ UIInitialCheck::pixmapUseCheck = &UIInitialCheck::initialPixmapUseCheck; });
    pixmapUseCheck(env, usedType);
}

void UIInitialCheck::initialPixmapArgumentCheck(JNIEnv *env, const char* argumentName, const std::type_info& argumentType){
    if(threadedPixmapsChecker()){
        pixmapArgumentThreadCheck = &trivial;
    }else{
        pixmapArgumentThreadCheck = &enabledPixmapArgumentThreadCheck;
    }
    qAddPostRoutine([](){ UIInitialCheck::pixmapArgumentThreadCheck = &UIInitialCheck::initialPixmapArgumentCheck; });
    pixmapArgumentThreadCheck(env, argumentName, argumentType);
}

void UIInitialCheck::windowConstructorThreadCheck(JNIEnv *env, const std::type_info&, const QObject* parent){
    if(parent){
        QThread* currentThread = QThread::currentThread();
        QThread* objectThread = parent->thread();
        if (objectThread && objectThread != currentThread) {
            if(QThread* this_thread = qobject_cast<QThread*>(const_cast<QObject*>(parent))){
                objectThread = this_thread;
                if(objectThread == currentThread)
                    return;
            }
            JavaException::raiseQThreadAffinityException(env, "QObject used as parent from outside its own thread" QTJAMBI_STACKTRACEINFO ,
                                                         QtJambiAPI::convertQObjectToJavaObject(env, parent),
                                                         objectThread, currentThread);
        }
    }
}

void UIInitialCheck::widgetConstructorThreadCheck(JNIEnv *env, const std::type_info& constructedType, const QObject* parent){
    QThread* mainThread = QCoreApplicationPrivate::theMainThread.loadRelaxed();
    QThread* currentThread = QThread::currentThread();
    if(currentThread!=mainThread){
        JavaException::raiseQThreadAffinityException(env, QStringLiteral("%1 created outside main thread").arg(getQtName(constructedType)) QTJAMBI_STACKTRACEINFO, nullptr,
                                                    currentThread, mainThread );
    }else if(parent){
        QThread* objectThread = parent->thread();
        if (objectThread && objectThread != currentThread) {
            if(QThread* this_thread = qobject_cast<QThread*>(const_cast<QObject*>(parent))){
                objectThread = this_thread;
                if(objectThread == currentThread)
                    return;
            }
            JavaException::raiseQThreadAffinityException(env, "QObject used as parent from outside its own thread" QTJAMBI_STACKTRACEINFO ,
                                                         QtJambiAPI::convertQObjectToJavaObject(env, parent),
                                                         objectThread, currentThread);
        }
    }
}

void UIInitialCheck::initialWindowConstructorCheck(JNIEnv *env, const std::type_info& constructedType, const QObject* parent){
    QCoreApplication* instance = QCoreApplication::instance();
    if(!instance) {
        Java::Runtime::IllegalStateException::throwNew(env, QStringLiteral("Cannot create %1 before initializing QGuiApplication.").arg(getQtName(constructedType)) QTJAMBI_STACKTRACEINFO);
    }else{
        if(!instance->inherits("QGuiApplication"))
            Java::Runtime::IllegalStateException::throwNew(env, QStringLiteral("Cannot create %1 without QGuiApplication (instead of %2).").arg(getQtName(constructedType)).arg(instance->metaObject()->className()) QTJAMBI_STACKTRACEINFO);
        else{
            windowConstructorCheck = &windowConstructorThreadCheck;
            qAddPostRoutine([](){ UIInitialCheck::windowConstructorCheck = &UIInitialCheck::initialWindowConstructorCheck; });
            windowConstructorCheck(env, constructedType, parent);
        }
    }
}

void UIInitialCheck::initialWidgetConstructorCheck(JNIEnv *env, const std::type_info& constructedType, const QObject* parent){
    QCoreApplication* instance = QCoreApplication::instance();
    if(!instance) {
        Java::Runtime::IllegalStateException::throwNew(env, QStringLiteral("Cannot create %1 before initializing QApplication.").arg(getQtName(constructedType)) QTJAMBI_STACKTRACEINFO);
    }else{
        if(!instance->inherits("QApplication")){
            Java::Runtime::IllegalStateException::throwNew(env, QStringLiteral("Cannot create %1 without QApplication (instead of %2).").arg(getQtName(constructedType)).arg(instance->metaObject()->className()) QTJAMBI_STACKTRACEINFO);
        }else{
            widgetConstructorCheck = &widgetConstructorThreadCheck;
            qAddPostRoutine([](){ UIInitialCheck::widgetConstructorCheck = &UIInitialCheck::initialWidgetConstructorCheck; });
            widgetConstructorCheck(env, constructedType, parent);
        }
    }
}

void UIInitialCheck::enabledQObjectThreadCheck(JNIEnv *env, const QObject* object){
    if(object){
        QThread* objectThread = object->thread();
        QThread* currentThread = QThread::currentThread();
        if (objectThread && objectThread != currentThread) {
            if(QThread* this_thread = qobject_cast<QThread*>(const_cast<QObject*>(object))){
                objectThread = this_thread;
                if(objectThread == currentThread)
                    return;
            }
            JavaException::raiseQThreadAffinityException(env, "QObject used from outside its own thread" QTJAMBI_STACKTRACEINFO ,
                                                         QtJambiAPI::convertQObjectToJavaObject(env, object),
                                                         objectThread, currentThread);
        }
    }
}

void UIInitialCheck::enabledUIThreadCheck(JNIEnv *env, const std::type_info& typeId){
    QThread* objectThread = QCoreApplicationPrivate::theMainThread.loadRelaxed();
    QThread* currentThread = QThread::currentThread();
    if(currentThread!=objectThread){
        if(typeid_equals(typeId, typeid(void))){
            JavaException::raiseQThreadAffinityException(env, QStringLiteral("%1 used from outside main thread").arg(getQtName(typeId)) QTJAMBI_STACKTRACEINFO ,
                                                         nullptr,
                                                         objectThread, currentThread);
        }else{
            JavaException::raiseQThreadAffinityException(env, "Thread affinity breach" QTJAMBI_STACKTRACEINFO ,
                                                         nullptr,
                                                         objectThread, currentThread);
        }
    }
}

void UIInitialCheck::enabledQObjectArgumentThreadCheck(JNIEnv *env, const char* argumentName, const QObject* argument){
    if(argument){
        QThread* objectThread = argument->thread();
        QThread* currentThread = QThread::currentThread();
        if (objectThread && objectThread != currentThread) {
            if(QThread* this_thread = qobject_cast<QThread*>(const_cast<QObject*>(argument))){
                objectThread = this_thread;
                if(objectThread == currentThread)
                    return;
            }
            JavaException::raiseQThreadAffinityException(env, QStringLiteral("QObject used as argument '%1' from outside its own thread").arg(argumentName) QTJAMBI_STACKTRACEINFO ,
                                                         QtJambiAPI::convertQObjectToJavaObject(env, argument),
                                                         objectThread, currentThread);
        }
    }
}

void UIInitialCheck::enabledValueArgumentThreadCheck(JNIEnv *env, const char* argumentName, const std::type_info& argumentType, const QObject* argumentOwner){
    if(argumentOwner){
        QThread* objectThread = argumentOwner->thread();
        QThread* currentThread = QThread::currentThread();
        if (objectThread && objectThread != currentThread) {
            if(QThread* this_thread = qobject_cast<QThread*>(const_cast<QObject*>(argumentOwner))){
                objectThread = this_thread;
                if(objectThread == currentThread)
                    return;
            }
            JavaException::raiseQThreadAffinityException(env, QStringLiteral("%1 used as argument '%2' from outside its own thread").arg(getQtName(argumentType)).arg(argumentName) QTJAMBI_STACKTRACEINFO ,
                                                         nullptr,
                                                         objectThread, currentThread);
        }
    }
}

void UIInitialCheck::enabledGeneralArgumentThreadCheck(JNIEnv *env, const char* argumentName, const std::type_info& argumentType, const void* argument){
    if(argument){
        if(PtrOwnerFunction ownerFunction = registeredOwnerFunction(argumentType)){
            enabledValueArgumentThreadCheck(env, argumentName, argumentType, ownerFunction(argument));
        }
    }
}

void UIInitialCheck::enabledUIArgumentThreadCheck(JNIEnv *env, const char* argumentName, const std::type_info& argumentType){
    QThread* objectThread = QCoreApplicationPrivate::theMainThread.loadRelaxed();
    QThread* currentThread = QThread::currentThread();
    if(currentThread!=objectThread){
        JavaException::raiseQThreadAffinityException(env, QStringLiteral("%1 used as argument '%2' from outside main thread").arg(getQtName(argumentType)).arg(argumentName) QTJAMBI_STACKTRACEINFO ,
                                                     nullptr,
                                                     objectThread, currentThread);
    }
}

void UIInitialCheck::enabledQObjectConstructorThreadCheck(JNIEnv *env, const QObject* parent){
    if(parent){
        QThread* currentThread = QThread::currentThread();
        QThread* objectThread = parent->thread();
        if (objectThread && objectThread != currentThread) {
            if(QThread* this_thread = qobject_cast<QThread*>(const_cast<QObject*>(parent))){
                objectThread = this_thread;
                if(objectThread == currentThread)
                    return;
            }
            JavaException::raiseQThreadAffinityException(env, "QObject used as parent from outside its own thread" QTJAMBI_STACKTRACEINFO ,
                                                         QtJambiAPI::convertQObjectToJavaObject(env, parent),
                                                         objectThread, currentThread);
        }
    }
}

void UIInitialCheck::enabledValueConstructorThreadCheck(JNIEnv *env, const std::type_info& parentType, const QObject* parentOwner){
    if(parentOwner){
        QThread* objectThread = parentOwner->thread();
        QThread* currentThread = QThread::currentThread();
        if (objectThread && objectThread != currentThread) {
            if(QThread* this_thread = qobject_cast<QThread*>(const_cast<QObject*>(parentOwner))){
                objectThread = this_thread;
                if(objectThread == currentThread)
                    return;
            }
            JavaException::raiseQThreadAffinityException(env, QStringLiteral("%1 used as parent from outside its own thread").arg(getQtName(parentType)) QTJAMBI_STACKTRACEINFO ,
                                                         nullptr,
                                                         objectThread, currentThread);
        }
    }
}

void UIInitialCheck::enabledGeneralConstructorThreadCheck(JNIEnv *env, const std::type_info& parentType, const void* parent){
    if(parent){
        if(PtrOwnerFunction ownerFunction = registeredOwnerFunction(parentType)){
            enabledValueConstructorThreadCheck(env, parentType, ownerFunction(parent));
        }
    }
}

void QtJambiAPI::checkThread(JNIEnv *env, const QObject* object){
    UIInitialCheck::objectThreadCheck(env, object);
}

void QtJambiAPI::checkThreadQPixmap(JNIEnv *env, const std::type_info& typeId){
    UIInitialCheck::pixmapUseCheck(env, typeId);
}

void QtJambiAPI::checkThreadUI(JNIEnv *env, const std::type_info& typeId){
    UIInitialCheck::uiThreadCheck(env, typeId);
}

void QtJambiAPI::checkThreadOnArgument(JNIEnv *env, const char* argumentName, const QObject* argument){
    UIInitialCheck::objectArgumentThreadCheck(env, argumentName, argument);
}

void QtJambiAPI::checkThreadOnArgument(JNIEnv *env, const char* argumentName, const std::type_info& argumentType, const QObject* argumentOwner){
    UIInitialCheck::valueArgumentThreadCheck(env, argumentName, argumentType, argumentOwner);
}

void QtJambiAPI::checkThreadOnArgumentUI(JNIEnv *env, const char* argumentName, const std::type_info& argumentType){
    UIInitialCheck::uiArgumentThreadCheck(env, argumentName, argumentType);
}

void QtJambiAPI::checkThreadOnArgumentQPixmap(JNIEnv *env, const char* argumentName, const std::type_info& argumentType){
    UIInitialCheck::pixmapArgumentThreadCheck(env, argumentName, argumentType);
}

const QObject* QtJambiAPI::getPixmapOwner(const void * ptr){
    return UIInitialCheck::getPixmapOwner(ptr);
}

void QtJambiAPI::checkThreadOnArgument(JNIEnv *env, const char* argumentName, const std::type_info& argumentType, const void* argument){
    UIInitialCheck::generalArgumentThreadCheck(env, argumentName, argumentType, argument);
}

void QtJambiAPI::checkThreadOnParent(JNIEnv *env, const QObject* parent){
    UIInitialCheck::objectConstructorThreadCheck(env, parent);
}

void QtJambiAPI::checkThreadOnParent(JNIEnv *env, const std::type_info& parentType, const QObject* parentOwner){
    UIInitialCheck::valueConstructorThreadCheck(env, parentType, parentOwner);
}

void QtJambiAPI::checkThreadOnParent(JNIEnv *env, const std::type_info& parentType, const void* parent){
    UIInitialCheck::generalConstructorThreadCheck(env, parentType, parent);
}

void QtJambiAPI::checkThreadConstructingUI(JNIEnv *env, const std::type_info& constructedType){
    UIInitialCheck::uiConstructorCheck(env, constructedType);
}

void QtJambiAPI::checkThreadConstructingQPixmap(JNIEnv *env, const std::type_info& constructedType){
    UIInitialCheck::pixmapConstructorCheck(env, constructedType);
}

void QtJambiAPI::checkThreadConstructingQWidget(JNIEnv *env, const std::type_info& constructedType, const QObject* parent){
    UIInitialCheck::widgetConstructorCheck(env, constructedType, parent);
}

void QtJambiAPI::checkThreadConstructingQWindow(JNIEnv *env, const std::type_info& constructedType, const QObject* parent){
    UIInitialCheck::windowConstructorCheck(env, constructedType, parent);
}

void QtJambiAPI::checkThreadConstructingApplication(JNIEnv *env, const std::type_info& constructedType){
#ifdef Q_OS_DARWIN
    if (constructedType!=typeid(QCoreApplication) && !pthread_main_np()) {
        JavaException::raiseError(env, "QtJambi does not appear to be running on the main thread and will "
                 "most likely be unstable and crash. "
                 "Please make sure to launch your 'java' command with the "
                 "'-XstartOnFirstThread' command line option. For instance: "
                 "java -XstartOnFirstThread any.vendor.MainClass" QTJAMBI_STACKTRACEINFO );
    }
#endif
    if(Java::Runtime::Boolean::getBoolean(env, env->NewStringUTF("io.qt.enable-thread-affinity-check"))){
        QThread* mainThread = QCoreApplicationPrivate::theMainThread.loadRelaxed();
        QThread* currentThread = QThread::currentThread();
        if(currentThread!=mainThread){
            JavaException::raiseQThreadAffinityException(env, QStringLiteral("%1 created outside main thread").arg(getQtName(constructedType)) QTJAMBI_STACKTRACEINFO, nullptr,
                                                        currentThread, mainThread );
        }
#ifdef Q_OS_ANDROID
        if(UIInitialCheck::uiThreadCheck == UIInitialCheck::UseCheck(&UIInitialCheck::trivial)){
            enableThreadAffinity(true);
            if(Java::Runtime::Boolean::getBoolean(env, env->NewStringUTF("io.qt.enable-event-thread-affinity-check"))){
                QInternal::registerCallback(QInternal::EventNotifyCallback, &threadAffineEventNotify);
            }else{
                QInternal::registerCallback(QInternal::EventNotifyCallback, &simpleEventNotify);
            }
        }
    }else{
        if(UIInitialCheck::uiThreadCheck != UIInitialCheck::UseCheck(&UIInitialCheck::trivial)){
            enableThreadAffinity(false);
            if(Java::Runtime::Boolean::getBoolean(env, env->NewStringUTF("io.qt.enable-event-thread-affinity-check"))){
                QInternal::registerCallback(QInternal::EventNotifyCallback, &threadAffineEventNotify);
            }else{
                QInternal::registerCallback(QInternal::EventNotifyCallback, &simpleEventNotify);
            }
        }
#endif
    }
}

void enableThreadAffinity(bool enabled){
    if(enabled){
        UIInitialCheck::windowConstructorCheck = &UIInitialCheck::initialWindowConstructorCheck;
        UIInitialCheck::widgetConstructorCheck = &UIInitialCheck::initialWidgetConstructorCheck;
        UIInitialCheck::pixmapConstructorCheck = &UIInitialCheck::initialPixmapConstructorCheck;
        UIInitialCheck::uiConstructorCheck = &UIInitialCheck::initialUIConstructorCheck;
        UIInitialCheck::pixmapUseCheck = &UIInitialCheck::initialPixmapUseCheck;
        UIInitialCheck::uiThreadCheck = &UIInitialCheck::enabledUIThreadCheck;
        UIInitialCheck::pixmapArgumentThreadCheck = &UIInitialCheck::initialPixmapArgumentCheck;
        UIInitialCheck::objectThreadCheck = &UIInitialCheck::enabledQObjectThreadCheck;
        UIInitialCheck::objectArgumentThreadCheck = &UIInitialCheck::enabledQObjectArgumentThreadCheck;
        UIInitialCheck::valueArgumentThreadCheck = &UIInitialCheck::enabledValueArgumentThreadCheck;
        UIInitialCheck::uiArgumentThreadCheck = &UIInitialCheck::enabledUIArgumentThreadCheck;
        UIInitialCheck::generalArgumentThreadCheck = &UIInitialCheck::enabledGeneralArgumentThreadCheck;
        UIInitialCheck::objectConstructorThreadCheck = &UIInitialCheck::enabledQObjectConstructorThreadCheck;
        UIInitialCheck::valueConstructorThreadCheck = &UIInitialCheck::enabledValueConstructorThreadCheck;
        UIInitialCheck::generalConstructorThreadCheck = &UIInitialCheck::enabledGeneralConstructorThreadCheck;
    }else{
        UIInitialCheck::objectThreadCheck = &UIInitialCheck::trivial;
        UIInitialCheck::pixmapUseCheck = &UIInitialCheck::trivial;
        UIInitialCheck::uiThreadCheck = &UIInitialCheck::trivial;
        UIInitialCheck::pixmapArgumentThreadCheck = &UIInitialCheck::trivial;
        UIInitialCheck::objectArgumentThreadCheck = &UIInitialCheck::trivial;
        UIInitialCheck::valueArgumentThreadCheck = &UIInitialCheck::trivial;
        UIInitialCheck::uiArgumentThreadCheck = &UIInitialCheck::trivial;
        UIInitialCheck::generalArgumentThreadCheck = &UIInitialCheck::trivial;
        UIInitialCheck::objectConstructorThreadCheck = &UIInitialCheck::trivial;
        UIInitialCheck::valueConstructorThreadCheck = &UIInitialCheck::trivial;
        UIInitialCheck::generalConstructorThreadCheck = &UIInitialCheck::trivial;
        UIInitialCheck::uiConstructorCheck = &UIInitialCheck::trivial;
        UIInitialCheck::pixmapConstructorCheck = &UIInitialCheck::trivial;
        UIInitialCheck::widgetConstructorCheck = &UIInitialCheck::trivial;
        UIInitialCheck::windowConstructorCheck = &UIInitialCheck::trivial;
    }
}

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
void onDynamicPropertyChange(QObject *receiver, QDynamicPropertyChangeEvent* event);
#endif

bool eventNotifier(QObject *receiver, QEvent *event, bool* result)
{
    switch (event->type()) {
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    case QEvent::DynamicPropertyChange:
        onDynamicPropertyChange(receiver, dynamic_cast<QDynamicPropertyChangeEvent*>(event));
        break;
#endif
    case QEvent::ChildAdded:
    case QEvent::ChildRemoved:
        {
            QChildEvent *e = static_cast<QChildEvent *>(event);
            // We're not interested in objects that don't have a link as the GC won't
            // be interferring with them anyway.
            if (QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForQObject(e->child())) {
                if (link->qobject() || !link->isInitialized()) {
                    if(JniEnvironment env{200}){
                        if (e->added())
                           link->setCppOwnership(env);
                        else
                           link->setDefaultOwnership(env);
                    }
                } else if (event->type() == QEvent::ChildAdded) {
                    qCWarning(DebugAPI::internalCategory, "'%s' [%s] was garbage collected before it was reparented to '%s' [%s]",
                         qPrintable(e->child()->objectName()), e->child()->metaObject()->className(),
                         qPrintable(receiver->objectName()), receiver->metaObject()->className());
                }
            }
        }
        break;
    case QEvent::DeferredDelete:
        if (QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForQObject(receiver)) {
            if (link->qobject()) {
                if(JniEnvironment env{200}){
                    link->setCppOwnership(env);
                }
            }
        }
        break;

    default:
        break;
    }
    if (!QCoreApplicationPrivate::threadRequiresCoreApplication()){
        QObjectPrivate *d = QObjectPrivate::get(receiver);
        QThreadData *threadData = d->threadData;
        QScopedScopeLevelCounter scopeLevelCounter(threadData);
        if (receiver == nullptr) {                        // serious error
            qCWarning(DebugAPI::internalCategory, "QCoreApplication::notify: Unexpected null receiver");
            return true;
        }
#ifndef QT_NO_DEBUG
        QCoreApplicationPrivate::checkReceiverThread(receiver);
#endif
        QtJambiExceptionBlocker __qt_exceptionHandler;
        *result = receiver->isWidgetType() ? false : QCoreApplicationPrivate::notify_helper(receiver, event);
        __qt_exceptionHandler.release(nullptr);
        return true;
    }else{
        return UIInitialCheck::eventNotify(receiver, event, result);
    }
}

bool simpleEventNotify(void **data)
{
    if(QObject *receiver = reinterpret_cast<QObject *>(data[0])){
        QEvent *event = reinterpret_cast<QEvent *>(data[1]);
        bool* result = reinterpret_cast<bool*>(data[2]);
        return eventNotifier(receiver, event, result);
    }
    return false;
}

bool threadAffineEventNotify(void **data)
{
    if(QObject *receiver = reinterpret_cast<QObject *>(data[0])){
        QEvent *event = reinterpret_cast<QEvent *>(data[1]);
        {
            if(const QObjectPrivate* p = QObjectPrivate::get(receiver)){
                QThreadData *thr = p->threadData;
                QScopedScopeLevelCounter scopeLevelCounter(thr);
                QThreadData *currentThread = QThreadData::get2(QThread::currentThread());
                QScopedScopeLevelCounter cscopeLevelCounter(currentThread);
                if(thr && thr->threadId!=nullptr && currentThread != thr){
                    QMetaEnum enm = QMetaEnum::fromType<QEvent::Type>();
                    QString eventDescr;
                    if(enm.isValid()){
                        eventDescr = QLatin1String(enm.valueToKeys(event->type()));
                    }else{
                        eventDescr = QString::number(int(event->type()));
                    }
                    if(QCoreApplicationPrivate::is_app_closing)
                        return false;
                    if(JniEnvironment env{200}){
                        QtJambiExceptionHandler __exceptionHandler;
                        try{
                            qCWarning(DebugAPI::internalCategory, "Cannot send events to objects owned by a different thread (event type: %ls). "
                                                                  "Current thread 0x%p. Receiver '%ls' (of type '%s') was created in thread 0x%p",
                                      qUtf16Printable(eventDescr),
                                      currentThread, qUtf16Printable(receiver->objectName()),
                                      receiver->metaObject()->className(), thr);
                            if(QtJambiShellInterface* shellInterface = dynamic_cast<QtJambiShellInterface*>(receiver)){
                                JavaException::raiseQThreadAffinityException(env, QString::asprintf("Cannot send events to objects owned by a different thread (event type: %ls). "
                                                                                                               "Current thread 0x%p. Receiver '%ls' (of type '%s') was created in thread 0x%p",
                                                                                                               qUtf16Printable(eventDescr),
                                                                                                               currentThread, qUtf16Printable(receiver->objectName()),
                                                                                                               receiver->metaObject()->className(), thr) QTJAMBI_STACKTRACEINFO ,
                                                                             QtJambiShellInterface::getJavaObjectLocalRef(env, shellInterface),
                                                                             nullptr, nullptr);
                            }else if (QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForQObject(receiver)) {
                                JavaException::raiseQThreadAffinityException(env, QString::asprintf("Cannot send events to objects owned by a different thread (event type: %ls). "
                                                                                                    "Current thread 0x%p. Receiver '%ls' (of type '%s') was created in thread 0x%p",
                                                                                                    qUtf16Printable(eventDescr),
                                                                                                    currentThread, qUtf16Printable(receiver->objectName()),
                                                                                                    receiver->metaObject()->className(), thr) QTJAMBI_STACKTRACEINFO ,
                                                                             link->getJavaObjectLocalRef(env),
                                                                             nullptr, nullptr);
                            }else{
                                JavaException::raiseQThreadAffinityException(env, QString::asprintf("Cannot send events to objects owned by a different thread (event type: %ls). "
                                                  "Current thread 0x%p. Receiver '%ls' (of type '%s') was created in thread 0x%p",
                                                  qUtf16Printable(eventDescr),
                                                  currentThread, qUtf16Printable(receiver->objectName()),
                                                  receiver->metaObject()->className(), thr) QTJAMBI_STACKTRACEINFO ,
                                                  nullptr, nullptr, nullptr);
                            }
                        }catch(const JavaException& exn){
                            __exceptionHandler.handle(nullptr, exn, "QCoreApplication::sendEvent");
                        }
                        return true;
                    }
                }
            }
        }
        bool* result = reinterpret_cast<bool*>(data[2]);
        return eventNotifier(receiver, event, result);
    }
    return false;
}
