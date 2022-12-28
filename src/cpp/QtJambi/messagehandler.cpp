/****************************************************************************
**
** Copyright (C) 2009-2022 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
**
** This file is part of Qt Jambi.
**
** $BEGIN_LICENSE$
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
** $END_LICENSE$
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

#include <QtCore/QReadWriteLock>
#include "qtjambiapi.h"
#include "jobjectwrapper.h"
#include "java_p.h"
#include "qtjambilink_p.h"

#include "qtjambi_cast.h"

Q_GLOBAL_STATIC(QReadWriteLock, gMessageHandlerLock)
Q_GLOBAL_STATIC(QSet<QtMsgType>, gEnabledMessages)
Q_GLOBAL_STATIC(JObjectWrapper, gMessageHandler)
static bool messageHandlerInstalled = false;

void qtjambi_messagehandler_proxy(QtMsgType type, const QMessageLogContext & context, const QString & message)
{
    if(!gMessageHandlerLock.isDestroyed()){
        QReadLocker locker(gMessageHandlerLock());
        if(gEnabledMessages->contains(type)){
            if(JniEnvironment env{500}){
                QtJambiExceptionInhibitor __exnHandler;
                try{
                    jobject msgType = qtjambi_cast<jobject>(env, type);
                    jobject _context = qtjambi_cast<jobject>(env, &context);
                    jobject msg = qtjambi_cast<jstring>(env, message);
                    Java::QtCore::QtMessageHandler::accept(env, gMessageHandler->object(), msgType, _context, msg);
                    InvalidateAfterUse::invalidate(env, _context);
                }catch(const JavaException& exn){
                    __exnHandler.handle(env, exn, nullptr);
                }
            }
        }
    }else{
        switch(type){
        case QtMsgType::QtCriticalMsg:
            printf("CRITICAL: %s\n", qPrintable(message));
            break;
        case QtMsgType::QtDebugMsg:
            printf("DEBUG: %s\n", qPrintable(message));
            break;
        case QtMsgType::QtInfoMsg:
            printf("INFO: %s\n", qPrintable(message));
            break;
        case QtMsgType::QtWarningMsg:
            printf("WARNING: %s\n", qPrintable(message));
            break;
        case QtMsgType::QtFatalMsg:
            printf("FATAL: %s\n", qPrintable(message));
            break;
        }
    }
}

jobject CoreAPI::installMessageHandler(JNIEnv *env, jobject supportedMessageTypes, jobject handler){
    QWriteLocker locker(gMessageHandlerLock());
    gEnabledMessages->clear();
    if(supportedMessageTypes){
        jobject iter = QtJambiAPI::iteratorOfJavaCollection(env, supportedMessageTypes);
        while(QtJambiAPI::hasJavaIteratorNext(env, iter)){
            jobject supportedMessageType = QtJambiAPI::nextOfJavaIterator(env, iter);
            gEnabledMessages->insert(qtjambi_cast<QtMsgType>(env, supportedMessageType));
        }
    }
    messageHandlerInstalled = handler!=nullptr;
    QtMessageHandler oldHandler;
    jobject result;
    if(gEnabledMessages->size()==5){
        QtMessageHandler messageHandler = qtjambi_cast<QtMessageHandler>(env, handler, "QtMessageHandler");
        oldHandler = qInstallMessageHandler(messageHandler);
        if(oldHandler==messageHandler)
            return handler;
        if (QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForJavaInterface(env, handler)) {
            if(link->isShell() && link->ownership()==QtJambiLink::Ownership::Java)
                link->setCppOwnership(env);
        }
        if(oldHandler==&qtjambi_messagehandler_proxy){
            result = env->NewLocalRef(gMessageHandler->object());
            *gMessageHandler = JObjectWrapper();
        }else{
            result = qtjambi_cast<jobject>(env, oldHandler);
        }
    }else{
        *gMessageHandler = JObjectWrapper(env, handler);
        oldHandler = qInstallMessageHandler(&qtjambi_messagehandler_proxy);
        if(oldHandler==&qtjambi_messagehandler_proxy){
            result = env->NewLocalRef(gMessageHandler->object());
        }else{
            result = qtjambi_cast<jobject>(env, oldHandler);
        }
        *gMessageHandler = JObjectWrapper(env, handler);
    }
    if (QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForJavaInterface(env, result)) {
        if(link->isShell() && link->ownership()!=QtJambiLink::Ownership::Java)
            link->setJavaOwnership(env);
    }
    return result;
}


void clearMessageHandlerAtShutdown(JNIEnv *env){
    QWriteLocker locker(gMessageHandlerLock());
    if(messageHandlerInstalled){
        gEnabledMessages->clear();
        QtMessageHandler oldHandler = qInstallMessageHandler(nullptr);
        if(oldHandler==&qtjambi_messagehandler_proxy){
            jobject result = env->NewLocalRef(gMessageHandler->object());
            *gMessageHandler = JObjectWrapper();
            messageHandlerInstalled = false;
            if (QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForJavaInterface(env, result)) {
                if(link->isShell() && link->ownership()!=QtJambiLink::Ownership::Java)
                    link->setJavaOwnership(env);
            }
        }
    }

}
