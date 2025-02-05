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

#include <QtCore/private/qcoreapplication_p.h>
#include <QtCore/private/qthread_p.h>
#include "exception.h"
#include "java_p.h"
#include "registryutil_p.h"
#include "guiapi.h"
#include "qtjambilink_p.h"
#include "qtjambishell_p.h"
#include "utils_p.h"
#if QT_VERSION < QT_VERSION_CHECK(6, 8, 0)
#include "threadutils_p.h"
#endif

#ifdef QTJAMBI_NO_METHOD_TRACE
#define QTJAMBI_DEBUG_EVENT_PRINT(...)
#define QTJAMBI_DEBUG_METHOD_PRINT_PLAIN(...)
#else
#define QTJAMBI_DEBUG_EVENT_PRINT(receiver, event)\
DebugAPI::EventPrint __debug_method_print(__FILE__, __LINE__, __FUNCTION__, receiver, event);
#define QTJAMBI_DEBUG_METHOD_PRINT_PLAIN(msg)\
DebugAPI::MethodPrint __debug_method_print(DebugAPI::MethodPrint::Internal, msg, __FILE__, __LINE__, __FUNCTION__);
#endif

struct UIInitialCheck{
    typedef void (*WidgetConstructorCheck)(JNIEnv *, const std::type_info&, const QObject*);
    typedef void (*WindowConstructorCheck)(JNIEnv *, const std::type_info&, const QObject*);
    typedef void (*ConstructorCheck)(JNIEnv *, const std::type_info&);
    typedef void (*UseCheck)(JNIEnv *, const std::type_info&);
    typedef void (*ArgumentCheck)(JNIEnv *, const char*, const std::type_info&);
    typedef void (*JObjectThreadCheck)(JNIEnv *, jobject);
    typedef void (*OwnerThreadCheck)(JNIEnv *, PtrOwnerFunction, const void *, jclass);
    typedef void (*QObjectThreadCheck)(JNIEnv *, const QObject*);
    typedef void (*QObjectArgumentThreadCheck)(JNIEnv *, const char*, const QObject*);
    typedef void (*ValueArgumentThreadCheck)(JNIEnv *, const char*, const std::type_info&, const QObject*);
    typedef void (*ValueThreadCheck)(JNIEnv *, const std::type_info&, const QObject*);
    typedef void (*UIArgumentThreadCheck)(JNIEnv *, const char*, const std::type_info&);
    typedef void (*GeneralArgumentThreadCheck)(JNIEnv *, const char*, const std::type_info&, const void*);
    typedef void (*GeneralThreadCheck)(JNIEnv *, const std::type_info&, const void*);
    typedef bool (*EventNotify)(QObject *receiver, QEvent *event, bool* result);
    typedef bool (*IsObjectsThread)(QObject *object);
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
    static JObjectThreadCheck jobjectThreadCheck;
    static OwnerThreadCheck ownerThreadCheck;
    static QObjectArgumentThreadCheck objectArgumentThreadCheck;
    static ValueArgumentThreadCheck valueArgumentThreadCheck;
    static UIArgumentThreadCheck uiArgumentThreadCheck;
    static GeneralArgumentThreadCheck generalArgumentThreadCheck;
    static QObjectThreadCheck objectConstructorThreadCheck;
    static ValueThreadCheck valueConstructorThreadCheck;
    static GeneralThreadCheck generalConstructorThreadCheck;
    static GeneralThreadCheck generalThreadCheck;
    static EventNotify eventNotify;
    static IsObjectsThread signalEmitThreadCheck;

    static void trivial(JNIEnv *, PtrOwnerFunction, const void *, jclass);
    static void trivial(JNIEnv *, const std::type_info&);
    static void trivial(JNIEnv *, const std::type_info&, const QObject*);
    static void trivial(JNIEnv *, const char*, const std::type_info&);
    static void trivial(JNIEnv *, const char*, const std::type_info&, const void*);
    static void trivial(JNIEnv *, const std::type_info&, const void*);
    static void trivial(JNIEnv *, const QObject*);
    static void trivial(JNIEnv *, jobject);
    static void trivial(JNIEnv *, const char*, const QObject*);
    static void trivial(JNIEnv *env, const char*, const std::type_info&, const QObject*);
    static bool trivial(QObject *, QEvent *, bool*);
    static bool trivial(QObject *);
    static const QObject* trivial(const void * ptr);
    template<WindowConstructorCheck replacement>
    static void initialWindowConstructorCheck(JNIEnv *, const std::type_info&, const QObject*);
    template<WidgetConstructorCheck replacement>
    static void initialWidgetConstructorCheck(JNIEnv *, const std::type_info&, const QObject*);
    template<ConstructorCheck replacement>
    static void initialPixmapConstructorCheck(JNIEnv *, const std::type_info&);
    template<ConstructorCheck replacement>
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
    static void enabledQObjectThreadCheck(JNIEnv *env, PtrOwnerFunction owner_function, const void *qt_object, jclass cls);
    static void enabledQObjectThreadCheck(JNIEnv *env, jobject object);
    static void enabledQObjectConstructorThreadCheck(JNIEnv *env, const QObject*);
    static void enabledValueConstructorThreadCheck(JNIEnv *env, const std::type_info&, const QObject*);
    static void enabledGeneralConstructorThreadCheck(JNIEnv *env, const std::type_info&, const void*);
    static void enabledQObjectArgumentThreadCheck(JNIEnv *env, const char* argumentName, const QObject* argument);
    static void enabledValueArgumentThreadCheck(JNIEnv *env, const char* argumentName, const std::type_info& argumentType, const QObject* argumentOwner);
    static void enabledGeneralArgumentThreadCheck(JNIEnv *env, const char* argumentName, const std::type_info& argumentType, const void* ptr);
    static void enabledGeneralThreadCheck(JNIEnv *env, const std::type_info& argumentType, const void* ptr);
    static void enabledUIArgumentThreadCheck(JNIEnv *env, const char* argumentName, const std::type_info& argumentType);
    static void enabledUIThreadCheck(JNIEnv *, const std::type_info&);
    static bool enabledSignalEmitThreadCheck(QObject *);
};

UIInitialCheck::WindowConstructorCheck UIInitialCheck::windowConstructorCheck = &UIInitialCheck::initialWindowConstructorCheck<&UIInitialCheck::trivial>;
UIInitialCheck::WidgetConstructorCheck UIInitialCheck::widgetConstructorCheck = &UIInitialCheck::initialWidgetConstructorCheck<&UIInitialCheck::trivial>;
UIInitialCheck::ConstructorCheck UIInitialCheck::pixmapConstructorCheck = &UIInitialCheck::trivial;
UIInitialCheck::ConstructorCheck UIInitialCheck::uiConstructorCheck = &UIInitialCheck::trivial;
UIInitialCheck::UseCheck UIInitialCheck::pixmapUseCheck = &UIInitialCheck::trivial;
UIInitialCheck::UseCheck UIInitialCheck::uiThreadCheck = &UIInitialCheck::trivial;
UIInitialCheck::ArgumentCheck UIInitialCheck::pixmapArgumentThreadCheck = &UIInitialCheck::trivial;
UIInitialCheck::QObjectThreadCheck UIInitialCheck::objectThreadCheck = &UIInitialCheck::trivial;
UIInitialCheck::JObjectThreadCheck UIInitialCheck::jobjectThreadCheck = &UIInitialCheck::trivial;
UIInitialCheck::OwnerThreadCheck UIInitialCheck::ownerThreadCheck = &UIInitialCheck::trivial;
UIInitialCheck::QObjectArgumentThreadCheck UIInitialCheck::objectArgumentThreadCheck = &UIInitialCheck::trivial;
UIInitialCheck::ValueArgumentThreadCheck UIInitialCheck::valueArgumentThreadCheck = &UIInitialCheck::trivial;
UIInitialCheck::UIArgumentThreadCheck UIInitialCheck::uiArgumentThreadCheck = &UIInitialCheck::trivial;
UIInitialCheck::GeneralArgumentThreadCheck UIInitialCheck::generalArgumentThreadCheck = &UIInitialCheck::trivial;
UIInitialCheck::QObjectThreadCheck UIInitialCheck::objectConstructorThreadCheck = &UIInitialCheck::trivial;
UIInitialCheck::ValueThreadCheck UIInitialCheck::valueConstructorThreadCheck = &UIInitialCheck::trivial;
UIInitialCheck::GeneralThreadCheck UIInitialCheck::generalConstructorThreadCheck = &UIInitialCheck::trivial;
UIInitialCheck::GeneralThreadCheck UIInitialCheck::generalThreadCheck = &UIInitialCheck::trivial;
UIInitialCheck::IsObjectsThread UIInitialCheck::signalEmitThreadCheck = &UIInitialCheck::trivial;

PtrOwnerFunction UIInitialCheck::getPixmapOwner = &UIInitialCheck::initialGetPixmapOwner;
GuiAPI::ThreadedPixmapsChecker UIInitialCheck::threadedPixmapsChecker = nullptr;
UIInitialCheck::EventNotify UIInitialCheck::eventNotify = &UIInitialCheck::initialEventNotify;

void GuiAPI::installThreadedPixmapsChecker(ThreadedPixmapsChecker threadedPixmapsChecker){
    UIInitialCheck::threadedPixmapsChecker = threadedPixmapsChecker;
}

void UIInitialCheck::trivial(JNIEnv *, PtrOwnerFunction, const void *, jclass){}
void UIInitialCheck::trivial(JNIEnv *, const std::type_info&){}
void UIInitialCheck::trivial(JNIEnv *, const std::type_info&, const QObject*){}
void UIInitialCheck::trivial(JNIEnv *, const char*, const std::type_info&){}
void UIInitialCheck::trivial(JNIEnv *, const std::type_info&, const void *){}
void UIInitialCheck::trivial(JNIEnv *, const QObject*){}
const QObject* UIInitialCheck::trivial(const void *){return nullptr;}
void UIInitialCheck::trivial(JNIEnv *, jobject){}
void UIInitialCheck::trivial(JNIEnv *, const char*, const QObject*){}
void UIInitialCheck::trivial(JNIEnv *, const char*, const std::type_info&, const QObject*){}
void UIInitialCheck::trivial(JNIEnv *, const char*, const std::type_info&, const void*){}
bool UIInitialCheck::trivial(QObject *, QEvent *, bool*){return false;}
bool UIInitialCheck::trivial(QObject *){return true;}

bool UIInitialCheck::initialEventNotify(QObject *receiver, QEvent *event, bool* result){
    QCoreApplication* instance = QCoreApplication::instance();
    if(Q_LIKELY(instance)) {
        if(QtJambiShellInterface* shellInterface = dynamic_cast<QtJambiShellInterface*>(instance)){
            QtJambiShellImpl* shellImpl = QtJambiShellImpl::get(shellInterface);
            if(!shellImpl->hasEmptyVTable()){
                UIInitialCheck::eventNotify = &UIInitialCheck::trivial;
            }else{
                UIInitialCheck::eventNotify = &UIInitialCheck::enabledEventNotify;
                return enabledEventNotify(receiver, event, result);
            }
        }else{
            UIInitialCheck::eventNotify = &UIInitialCheck::enabledEventNotify;
            return enabledEventNotify(receiver, event, result);
        }
    }
    return false;
}

class ScopedScopeLevelCounter
{
    int& scopeLevel;
public:
    inline ScopedScopeLevelCounter(QThreadData *threadData)
        : scopeLevel(threadData->scopeLevel)
    { ++scopeLevel; }
    inline ~ScopedScopeLevelCounter()
    { --scopeLevel; }
};

bool UIInitialCheck::enabledEventNotify(QObject *receiver, QEvent *event, bool* result){
    QObjectPrivate *d = QObjectPrivate::get(receiver);
    QThreadData *threadData = d->threadData;
    ScopedScopeLevelCounter scopeLevelCounter(threadData);
    QtJambiExceptionUnraiser __qt_unraiser;
    QtJambiExceptionBlocker __qt_exceptionHandler;
    *result = QCoreApplication::instance()->notify(receiver, event);
    __qt_exceptionHandler.release(nullptr);
    return true;
}

void UIInitialCheck::uiConstructorThreadCheck(JNIEnv *env, const std::type_info& constructedType){
#if QT_VERSION >= QT_VERSION_CHECK(6, 8, 0)
    auto mainThreadId = QCoreApplicationPrivate::theMainThreadId.loadRelaxed();
#else
    auto mainThreadId = QThreadUserData::theMainThreadId.loadRelaxed();
#endif
    if(mainThreadId!=QThread::currentThreadId()){
        JavaException::raiseQThreadAffinityException(env, QStringLiteral("%1 created outside main thread").arg(getQtName(constructedType)) QTJAMBI_STACKTRACEINFO, nullptr,
                                                     QThread::currentThread(), QCoreApplicationPrivate::theMainThread.loadRelaxed() );
    }
}

void UIInitialCheck::pixmapUseThreadCheck(JNIEnv *env, const std::type_info& constructedType){
#if QT_VERSION >= QT_VERSION_CHECK(6, 8, 0)
    auto mainThreadId = QCoreApplicationPrivate::theMainThreadId.loadRelaxed();
#else
    auto mainThreadId = QThreadUserData::theMainThreadId.loadRelaxed();
#endif
    if(mainThreadId!=QThread::currentThreadId()){
        const char* typeName = getQtName(constructedType);
        if(QLatin1String("QPixmap")==typeName || QLatin1String("QBitmap")==typeName){
            JavaException::raiseQThreadAffinityException(env, QStringLiteral("%1 used from outside main thread").arg(typeName) QTJAMBI_STACKTRACEINFO, nullptr,
                                                         QThread::currentThread(), QCoreApplicationPrivate::theMainThread.loadRelaxed() );
        }else{
            JavaException::raiseQThreadAffinityException(env, QStringLiteral("QPixmap used in %1 from outside main thread").arg(typeName) QTJAMBI_STACKTRACEINFO, nullptr,
                                                         QThread::currentThread(), QCoreApplicationPrivate::theMainThread.loadRelaxed() );
        }
    }
}

void UIInitialCheck::enabledPixmapArgumentThreadCheck(JNIEnv *env, const char* argumentName, const std::type_info& argumentType){
#if QT_VERSION >= QT_VERSION_CHECK(6, 8, 0)
    auto mainThreadId = QCoreApplicationPrivate::theMainThreadId.loadRelaxed();
#else
    auto mainThreadId = QThreadUserData::theMainThreadId.loadRelaxed();
#endif
    if(mainThreadId!=QThread::currentThreadId()){
        JavaException::raiseQThreadAffinityException(env, QStringLiteral("%1 used as argument '%2' from outside main thread").arg(getQtName(argumentType), argumentName) QTJAMBI_STACKTRACEINFO ,
                                                     nullptr,
                                                     QThread::currentThread(), QCoreApplicationPrivate::theMainThread.loadRelaxed() );
    }
}

template<UIInitialCheck::ConstructorCheck replacement>
void UIInitialCheck::initialPixmapConstructorCheck(JNIEnv *env, const std::type_info& constructedType){
    QCoreApplication* instance = QCoreApplication::instance();
    if(!instance) {
        Java::Runtime::IllegalStateException::throwNew(env, QStringLiteral("Cannot create %1 before initializing QGuiApplication.").arg(getQtName(constructedType)) QTJAMBI_STACKTRACEINFO);
    }else{
        if(!instance->inherits("QGuiApplication"))
            Java::Runtime::IllegalStateException::throwNew(env, QStringLiteral("Cannot create %1 without QGuiApplication (instead of %2).").arg(getQtName(constructedType), instance->metaObject()->className()) QTJAMBI_STACKTRACEINFO);
        else{
            if(threadedPixmapsChecker()){
                pixmapConstructorCheck = &trivial;
            }else{
                pixmapConstructorCheck = replacement;
            }
            qAddPostRoutine([](){ UIInitialCheck::pixmapConstructorCheck = &UIInitialCheck::initialPixmapConstructorCheck<replacement>; });
            pixmapConstructorCheck(env, constructedType);
        }
    }
}

const QObject* UIInitialCheck::initialGetPixmapOwner(const void * ptr){
    if(threadedPixmapsChecker()){
        getPixmapOwner = &trivial;
    }else{
        getPixmapOwner = &QtJambiAPI::mainThreadOwner;
    }
    return getPixmapOwner(ptr);
}

const QObject* QtJambiAPI::mainThreadOwner(const void *)
{
    if(QCoreApplicationPrivate::is_app_closing){
        return nullptr;
    }
    return QCoreApplication::instance();
}

template<UIInitialCheck::ConstructorCheck replacement>
void UIInitialCheck::initialUIConstructorCheck(JNIEnv *env, const std::type_info& constructedType){
    QCoreApplication* instance = QCoreApplication::instance();
    if(!instance) {
        Java::Runtime::IllegalStateException::throwNew(env, QStringLiteral("Cannot create %1 before initializing QGuiApplication.").arg(getQtName(constructedType)) QTJAMBI_STACKTRACEINFO);
    }else{
        if(!instance->inherits("QGuiApplication"))
            Java::Runtime::IllegalStateException::throwNew(env, QStringLiteral("Cannot create %1 without QGuiApplication (instead of %2).").arg(getQtName(constructedType), instance->metaObject()->className()) QTJAMBI_STACKTRACEINFO);
        else{
            uiConstructorCheck = replacement;
            qAddPostRoutine([](){ UIInitialCheck::uiConstructorCheck = &UIInitialCheck::initialUIConstructorCheck<replacement>; });
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
#if QT_VERSION >= QT_VERSION_CHECK(6, 8, 0)
    auto mainThreadId = QCoreApplicationPrivate::theMainThreadId.loadRelaxed();
#else
    auto mainThreadId = QThreadUserData::theMainThreadId.loadRelaxed();
#endif
    Qt::HANDLE currentThreadId = QThread::currentThreadId();
    if(mainThreadId!=currentThreadId){
        JavaException::raiseQThreadAffinityException(env, QStringLiteral("%1 created outside main thread").arg(getQtName(constructedType)) QTJAMBI_STACKTRACEINFO, nullptr,
                                                     QThread::currentThread(), QCoreApplicationPrivate::theMainThread.loadRelaxed() );
    }
    else if(parent){
        QThreadData* objectThreadData = QObjectPrivate::get(parent)->threadData.loadRelaxed();
        Qt::HANDLE currentThreadId = QThread::currentThreadId();
        if (objectThreadData && objectThreadData->threadId.loadRelaxed() != currentThreadId && objectThreadData->thread.loadRelaxed() != nullptr) {
            if(QThread* this_thread = qobject_cast<QThread*>(const_cast<QObject*>(parent))){
                if(QThreadData::get2(this_thread)->threadId.loadRelaxed() == currentThreadId)
                    return;
            }
            JavaException::raiseQThreadAffinityException(env, "QObject used as parent from outside its own thread" QTJAMBI_STACKTRACEINFO ,
                                                         QtJambiAPI::convertQObjectToJavaObject(env, parent),
                                                         objectThreadData->thread.loadRelaxed(),
                                                         QThread::currentThread()
                );
        }
    }
}

template<UIInitialCheck::WindowConstructorCheck replacement>
void UIInitialCheck::initialWindowConstructorCheck(JNIEnv *env, const std::type_info& constructedType, const QObject* parent){
    QCoreApplication* instance = QCoreApplication::instance();
    if(!instance) {
        Java::Runtime::IllegalStateException::throwNew(env, QStringLiteral("Cannot create %1 before initializing QGuiApplication.").arg(getQtName(constructedType)) QTJAMBI_STACKTRACEINFO);
    }else{
        if(!instance->inherits("QGuiApplication"))
            Java::Runtime::IllegalStateException::throwNew(env, QStringLiteral("Cannot create %1 without QGuiApplication (instead of %2).").arg(getQtName(constructedType), instance->metaObject()->className()) QTJAMBI_STACKTRACEINFO);
        else{
            windowConstructorCheck = replacement;
            qAddPostRoutine([](){ UIInitialCheck::windowConstructorCheck = &UIInitialCheck::initialWindowConstructorCheck<replacement>; });
            windowConstructorCheck(env, constructedType, parent);
        }
    }
}

template<UIInitialCheck::WidgetConstructorCheck replacement>
void UIInitialCheck::initialWidgetConstructorCheck(JNIEnv *env, const std::type_info& constructedType, const QObject* parent){
    QCoreApplication* instance = QCoreApplication::instance();
    if(!instance) {
        Java::Runtime::IllegalStateException::throwNew(env, QStringLiteral("Cannot create %1 before initializing QApplication.").arg(getQtName(constructedType)) QTJAMBI_STACKTRACEINFO);
    }else{
        if(!instance->inherits("QApplication")){
            Java::Runtime::IllegalStateException::throwNew(env, QStringLiteral("Cannot create %1 without QApplication (instead of %2).").arg(getQtName(constructedType), instance->metaObject()->className()) QTJAMBI_STACKTRACEINFO);
        }else{
            widgetConstructorCheck = replacement;
            qAddPostRoutine([](){ UIInitialCheck::widgetConstructorCheck = &UIInitialCheck::initialWidgetConstructorCheck<replacement>; });
            widgetConstructorCheck(env, constructedType, parent);
        }
    }
}

void UIInitialCheck::enabledQObjectThreadCheck(JNIEnv *env, const QObject* object){
    if(object){
        QThreadData* objectThreadData = QObjectPrivate::get(object)->threadData.loadRelaxed();
        Qt::HANDLE currentThreadId = QThread::currentThreadId();
        if (objectThreadData && objectThreadData->threadId.loadRelaxed() != currentThreadId && objectThreadData->thread.loadRelaxed() != nullptr) {
            if(QThread* this_thread = qobject_cast<QThread*>(const_cast<QObject*>(object))){
                if(QThreadData::get2(this_thread)->threadId.loadRelaxed() == currentThreadId)
                    return;
            }
            JavaException::raiseQThreadAffinityException(env, "QObject used from outside its own thread" QTJAMBI_STACKTRACEINFO ,
                                                         QtJambiAPI::convertQObjectToJavaObject(env, object),
                                                         objectThreadData->thread.loadRelaxed(), QThread::currentThread());
        }
    }
}

void UIInitialCheck::enabledQObjectThreadCheck(JNIEnv *env, jobject object){
    enabledQObjectThreadCheck(env, QtJambiAPI::convertJavaObjectToQObject(env, object));
}

void UIInitialCheck::enabledQObjectThreadCheck(JNIEnv *env, PtrOwnerFunction owner_function, const void *qt_object, jclass cls){
    if(owner_function){
        if(const QObject* object = owner_function(qt_object)){
            QThreadData* objectThreadData = QObjectPrivate::get(object)->threadData.loadRelaxed();
            Qt::HANDLE currentThreadId = QThread::currentThreadId();
            if (objectThreadData && objectThreadData->threadId.loadRelaxed() != currentThreadId && objectThreadData->thread.loadRelaxed() != nullptr) {
                if(QThread* this_thread = qobject_cast<QThread*>(const_cast<QObject*>(object))){
                    if(QThreadData::get2(this_thread)->threadId.loadRelaxed() == currentThreadId)
                        return;
                }
                JavaException::raiseQThreadAffinityException(env, QStringLiteral("QObject used from outside its own thread when creating %1").arg(QtJambiAPI::getClassName(env, cls).replace('/', '.').replace('/', '$')) QTJAMBI_STACKTRACEINFO ,
                                                             QtJambiAPI::convertQObjectToJavaObject(env, object),
                                                             objectThreadData->thread.loadRelaxed(), QThread::currentThread());
            }
        }
    }
}

bool UIInitialCheck::enabledSignalEmitThreadCheck(QObject *object){
    if(object){
        QThreadData* objectThreadData = QObjectPrivate::get(object)->threadData.loadRelaxed();
        return !objectThreadData || objectThreadData->threadId.loadRelaxed() == QThread::currentThreadId();
    }
    return true;
}

void UIInitialCheck::enabledUIThreadCheck(JNIEnv *env, const std::type_info& typeId){
#if QT_VERSION >= QT_VERSION_CHECK(6, 8, 0)
    auto mainThreadId = QCoreApplicationPrivate::theMainThreadId.loadRelaxed();
#else
    auto mainThreadId = QThreadUserData::theMainThreadId.loadRelaxed();
#endif
    if(mainThreadId!=QThread::currentThreadId()){
        if(typeid_equals(typeId, typeid(void))){
            JavaException::raiseQThreadAffinityException(env, QStringLiteral("%1 used from outside main thread").arg(getQtName(typeId)) QTJAMBI_STACKTRACEINFO ,
                                                         nullptr,
                                                         QThread::currentThread(), QCoreApplicationPrivate::theMainThread.loadRelaxed() );
        }else{
            JavaException::raiseQThreadAffinityException(env, "Thread affinity breach" QTJAMBI_STACKTRACEINFO ,
                                                         nullptr,
                                                         QThread::currentThread(), QCoreApplicationPrivate::theMainThread.loadRelaxed() );
        }
    }
}

void UIInitialCheck::enabledQObjectArgumentThreadCheck(JNIEnv *env, const char* argumentName, const QObject* argument){
    if(argument){
        QThreadData* objectThreadData = QObjectPrivate::get(argument)->threadData.loadRelaxed();
        Qt::HANDLE currentThreadId = QThread::currentThreadId();
        if (objectThreadData && objectThreadData->threadId.loadRelaxed() != currentThreadId && objectThreadData->thread.loadRelaxed() != nullptr) {
            if(QThread* this_thread = qobject_cast<QThread*>(const_cast<QObject*>(argument))){
                if(QThreadData::get2(this_thread)->threadId.loadRelaxed() == currentThreadId)
                    return;
            }
            JavaException::raiseQThreadAffinityException(env, QStringLiteral("QObject used as argument '%1' from outside its own thread").arg(argumentName) QTJAMBI_STACKTRACEINFO ,
                                                         QtJambiAPI::convertQObjectToJavaObject(env, argument),
                                                         objectThreadData->thread.loadRelaxed(), QThread::currentThread());
        }
    }
}

void UIInitialCheck::enabledValueArgumentThreadCheck(JNIEnv *env, const char* argumentName, const std::type_info& argumentType, const QObject* argumentOwner){
    if(argumentOwner){
        QThreadData* objectThreadData = QObjectPrivate::get(argumentOwner)->threadData.loadRelaxed();
        Qt::HANDLE currentThreadId = QThread::currentThreadId();
        if (objectThreadData && objectThreadData->threadId.loadRelaxed() != currentThreadId && objectThreadData->thread.loadRelaxed() != nullptr) {
            if(QThread* this_thread = qobject_cast<QThread*>(const_cast<QObject*>(argumentOwner))){
                if(QThreadData::get2(this_thread)->threadId.loadRelaxed() == currentThreadId)
                    return;
            }
            JavaException::raiseQThreadAffinityException(env, QStringLiteral("%1 used as argument '%2' from outside its own thread").arg(getQtName(argumentType), argumentName) QTJAMBI_STACKTRACEINFO ,
                                                         nullptr,
                                                         objectThreadData->thread.loadRelaxed(), QThread::currentThread());
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
#if QT_VERSION >= QT_VERSION_CHECK(6, 8, 0)
    auto mainThreadId = QCoreApplicationPrivate::theMainThreadId.loadRelaxed();
#else
    auto mainThreadId = QThreadUserData::theMainThreadId.loadRelaxed();
#endif
    if(mainThreadId!=QThread::currentThreadId()){
        JavaException::raiseQThreadAffinityException(env, QStringLiteral("%1 used as argument '%2' from outside main thread").arg(getQtName(argumentType), argumentName) QTJAMBI_STACKTRACEINFO ,
                                                     nullptr,
                                                     QThread::currentThread(), QCoreApplicationPrivate::theMainThread.loadRelaxed() );
    }
}

void UIInitialCheck::enabledQObjectConstructorThreadCheck(JNIEnv *env, const QObject* parent){
    if(parent){
        QThreadData* objectThreadData = QObjectPrivate::get(parent)->threadData.loadRelaxed();
        Qt::HANDLE currentThreadId = QThread::currentThreadId();
        if (objectThreadData && objectThreadData->threadId.loadRelaxed() != currentThreadId && objectThreadData->thread.loadRelaxed() != nullptr) {
            if(QThread* this_thread = qobject_cast<QThread*>(const_cast<QObject*>(parent))){
                if(QThreadData::get2(this_thread)->threadId.loadRelaxed() == currentThreadId)
                    return;
            }
            JavaException::raiseQThreadAffinityException(env, "QObject used as parent from outside its own thread" QTJAMBI_STACKTRACEINFO ,
                                                         QtJambiAPI::convertQObjectToJavaObject(env, parent),
                                                         objectThreadData->thread.loadRelaxed(), QThread::currentThread());
        }
    }
}

void UIInitialCheck::enabledValueConstructorThreadCheck(JNIEnv *env, const std::type_info& parentType, const QObject* parentOwner){
    if(parentOwner){
        QThreadData* objectThreadData = QObjectPrivate::get(parentOwner)->threadData.loadRelaxed();
        Qt::HANDLE currentThreadId = QThread::currentThreadId();
        if (objectThreadData && objectThreadData->threadId.loadRelaxed() != currentThreadId && objectThreadData->thread.loadRelaxed() != nullptr) {
            if(QThread* this_thread = qobject_cast<QThread*>(const_cast<QObject*>(parentOwner))){
                if(QThreadData::get2(this_thread)->threadId.loadRelaxed() == currentThreadId)
                    return;
            }
            JavaException::raiseQThreadAffinityException(env, QStringLiteral("%1 used as parent from outside its own thread").arg(getQtName(parentType)) QTJAMBI_STACKTRACEINFO ,
                                                         nullptr,
                                                         objectThreadData->thread.loadRelaxed(), QThread::currentThread());
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

void UIInitialCheck::enabledGeneralThreadCheck(JNIEnv *env, const std::type_info& pointerType, const void* qt_object){
    if(qt_object){
        if(PtrOwnerFunction ownerFunction = registeredOwnerFunction(pointerType)){
            enabledQObjectThreadCheck(env, ownerFunction(qt_object));
        }
    }
}

void checkThreadOnQObject(JNIEnv *env, PtrOwnerFunction owner_function, const void *qt_object, jclass cls){
    UIInitialCheck::ownerThreadCheck(env, owner_function, qt_object, cls);
}

void checkThreadOnQObject(JNIEnv *env, jobject object){
    UIInitialCheck::jobjectThreadCheck(env, object);
}

void QtJambiAPI::checkThread(JNIEnv *env, const QObject* object){
    UIInitialCheck::objectThreadCheck(env, object);
}

void QtJambiAPI::checkThread(JNIEnv *env, const std::type_info& argumentType, const void* object){
    UIInitialCheck::generalThreadCheck(env, argumentType, object);
}

bool checkThreadOnSignalEmit(QObject* object){
    return UIInitialCheck::signalEmitThreadCheck(object);
}

void QtJambiAPI::checkMainThread(JNIEnv *env, const std::type_info& typeId){
    UIInitialCheck::uiThreadCheck(env, typeId);
}

void QtJambiAPI::checkThreadOnArgument(JNIEnv *env, const char* argumentName, const QObject* argument){
    UIInitialCheck::objectArgumentThreadCheck(env, argumentName, argument);
}

void QtJambiAPI::checkThreadOnArgument(JNIEnv *env, const char* argumentName, const std::type_info& argumentType, const QObject* argumentOwner){
    UIInitialCheck::valueArgumentThreadCheck(env, argumentName, argumentType, argumentOwner);
}

void QtJambiAPI::checkMainThreadOnArgument(JNIEnv *env, const char* argumentName, const std::type_info& argumentType){
    UIInitialCheck::uiArgumentThreadCheck(env, argumentName, argumentType);
}

void QtJambiAPI::checkThreadOnArgumentQPixmap(JNIEnv *env, const char* argumentName, const std::type_info& argumentType){
    UIInitialCheck::pixmapArgumentThreadCheck(env, argumentName, argumentType);
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

void QtJambiAPI::checkMainThreadConstructing(JNIEnv *env, const std::type_info& constructedType){
    UIInitialCheck::uiConstructorCheck(env, constructedType);
}

const QObject* QtJambiAPI::getPixmapOwner(const void * ptr){
    return UIInitialCheck::getPixmapOwner(ptr);
}

void QtJambiAPI::checkThreadQPixmap(JNIEnv *env, const std::type_info& typeId){
    UIInitialCheck::pixmapUseCheck(env, typeId);
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

void checkThreadConstructingApplication(JNIEnv *env, const std::type_info& constructedType){
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
        UIInitialCheck::windowConstructorCheck = &UIInitialCheck::initialWindowConstructorCheck<&UIInitialCheck::windowConstructorThreadCheck>;
        UIInitialCheck::widgetConstructorCheck = &UIInitialCheck::initialWidgetConstructorCheck<&UIInitialCheck::widgetConstructorThreadCheck>;
        UIInitialCheck::pixmapConstructorCheck = &UIInitialCheck::initialPixmapConstructorCheck<&UIInitialCheck::uiConstructorThreadCheck>;
        UIInitialCheck::uiConstructorCheck = &UIInitialCheck::initialUIConstructorCheck<&UIInitialCheck::uiConstructorThreadCheck>;
        UIInitialCheck::pixmapUseCheck = &UIInitialCheck::initialPixmapUseCheck;
        UIInitialCheck::uiThreadCheck = &UIInitialCheck::enabledUIThreadCheck;
        UIInitialCheck::pixmapArgumentThreadCheck = &UIInitialCheck::initialPixmapArgumentCheck;
        UIInitialCheck::objectThreadCheck = &UIInitialCheck::enabledQObjectThreadCheck;
        UIInitialCheck::jobjectThreadCheck = &UIInitialCheck::enabledQObjectThreadCheck;
        UIInitialCheck::ownerThreadCheck = &UIInitialCheck::enabledQObjectThreadCheck;
        UIInitialCheck::objectArgumentThreadCheck = &UIInitialCheck::enabledQObjectArgumentThreadCheck;
        UIInitialCheck::valueArgumentThreadCheck = &UIInitialCheck::enabledValueArgumentThreadCheck;
        UIInitialCheck::uiArgumentThreadCheck = &UIInitialCheck::enabledUIArgumentThreadCheck;
        UIInitialCheck::generalArgumentThreadCheck = &UIInitialCheck::enabledGeneralArgumentThreadCheck;
        UIInitialCheck::objectConstructorThreadCheck = &UIInitialCheck::enabledQObjectConstructorThreadCheck;
        UIInitialCheck::valueConstructorThreadCheck = &UIInitialCheck::enabledValueConstructorThreadCheck;
        UIInitialCheck::generalConstructorThreadCheck = &UIInitialCheck::enabledGeneralConstructorThreadCheck;
        UIInitialCheck::generalThreadCheck = &UIInitialCheck::enabledGeneralThreadCheck;
    }else{
        UIInitialCheck::windowConstructorCheck = &UIInitialCheck::initialWindowConstructorCheck<&UIInitialCheck::trivial>;
        UIInitialCheck::widgetConstructorCheck = &UIInitialCheck::initialWidgetConstructorCheck<&UIInitialCheck::trivial>;
        UIInitialCheck::pixmapConstructorCheck = &UIInitialCheck::initialPixmapConstructorCheck<&UIInitialCheck::trivial>;
        UIInitialCheck::uiConstructorCheck = &UIInitialCheck::initialUIConstructorCheck<&UIInitialCheck::trivial>;
        UIInitialCheck::objectThreadCheck = &UIInitialCheck::trivial;
        UIInitialCheck::jobjectThreadCheck = &UIInitialCheck::trivial;
        UIInitialCheck::ownerThreadCheck = &UIInitialCheck::trivial;
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
        UIInitialCheck::generalThreadCheck = &UIInitialCheck::trivial;
    }
}

void enableSignalEmitThreadCheck(bool enabled){
    if(enabled){
        UIInitialCheck::signalEmitThreadCheck = &UIInitialCheck::enabledSignalEmitThreadCheck;
    }else{
        UIInitialCheck::signalEmitThreadCheck = &UIInitialCheck::trivial;
    }
}

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
void onDynamicPropertyChange(QObject *receiver, QDynamicPropertyChangeEvent* event);
#endif

bool eventNotifier(QObject *receiver, QEvent *event, bool* result)
{
    QTJAMBI_DEBUG_EVENT_PRINT(receiver, event);
    if (receiver == nullptr) {                        // serious error
        qCCritical(DebugAPI::internalCategory, "QCoreApplication::notify: Unexpected null receiver");
        return true;
    }
    QObjectPrivate *d = QObjectPrivate::get(receiver);
    if(!d || d->wasDeleted){
        if(JniEnvironmentExceptionHandler env{200}){
            try{
                Java::QtJambi::QDanglingPointerException::throwNew(env, QString::asprintf("Dangling pointer to QObject: %p", receiver) QTJAMBI_STACKTRACEINFO );
            }catch(const JavaException& exn){
                env.handleException(exn, "QCoreApplication::sendEvent");
            }
        }else{
            qCCritical(DebugAPI::internalCategory, "QCoreApplication::notify: Receiver has been already deleted");
        }
        return true;
    }
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
    QThreadData *threadData = d->threadData;
    if (!(threadData && threadData->requiresCoreApplication)){
        ScopedScopeLevelCounter scopeLevelCounter(threadData);
#ifndef QT_NO_DEBUG
        QCoreApplicationPrivate::checkReceiverThread(receiver);
#endif
        QtJambiExceptionUnraiser __qt_unraiser;
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
        if(const QObjectPrivate* p = QObjectPrivate::get(receiver)){
            if(p->wasDeleted){
                if(QThreadData *thr = p->threadData){
                    ScopedScopeLevelCounter scopeLevelCounter(thr);
                    QThreadData *currentThread = QThreadData::get2(QThread::currentThread());
                    ScopedScopeLevelCounter cscopeLevelCounter(currentThread);
                    if(thr->threadId!=nullptr && currentThread != thr){
                        QMetaEnum enm = QMetaEnum::fromType<QEvent::Type>();
                        QString eventDescr;
                        if(enm.isValid()){
                            eventDescr = QLatin1String(enm.valueToKeys(event->type()));
                        }else{
                            eventDescr = QString::number(int(event->type()));
                        }
                        if(QCoreApplicationPrivate::is_app_closing)
                            return false;
                        if(JniEnvironmentExceptionHandler env{200}){
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
                                env.handleException(exn, "QCoreApplication::sendEvent");
                            }
                            return true;
                        }
                    }
                }
            }
        }
        bool* result = reinterpret_cast<bool*>(data[2]);
        return eventNotifier(receiver, event, result);
    }
    return false;
}
