/****************************************************************************
**
** Copyright (C) 2009-2023 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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

#include <QtJambi/QtJambiAPI>
#include <QtScxml/QScxmlStateMachine>
#include <QtJambi/JavaAPI>
#include <QtJambi/JObjectWrapper>
#include <QtJambi/qtjambi_cast.h>

// QScxmlStateMachine::connectToState(const QString &scxmlStateName, Functor functor, Qt::ConnectionType type = Qt::AutoConnection)
extern "C" Q_DECL_EXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_scxml_QScxmlStateMachine_connectToState)
(JNIEnv *__jni_env,
 jobject __this,
 QtJambiNativeID __this_nativeId,
 jstring scxmlStateName0,
 jobject slot1,
 jint type2)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QScxmlStateMachine::connectToState(const QString &scxmlStateName, Functor functor, Qt::ConnectionType type = Qt::AutoConnection)")
    Q_UNUSED(__this)
    jobject _result{nullptr};
    QTJAMBI_TRY{
        QScxmlStateMachine *__qt_this = QtJambiAPI::objectFromNativeId<QScxmlStateMachine>(__this_nativeId);
        QtJambiAPI::checkPointer(__jni_env, __qt_this);
        const QString&  __qt_scxmlStateName0 = qtjambi_cast<QString>(__jni_env, scxmlStateName0);
        JObjectWrapper pointer(__jni_env, slot1);
        QMetaObject::Connection connection;
        if(Java::QtCore::QMetaObject$Slot1::isInstanceOf(__jni_env, slot1)){
            connection = __qt_this->connectToState(__qt_scxmlStateName0, [pointer](bool isEnteringState){
                if(JniEnvironment env{300}){
                    Java::QtCore::QMetaObject$Slot1::invoke(env, pointer.object(), QtJambiAPI::toJavaBooleanObject(env, isEnteringState));
                }
            }, Qt::ConnectionType(type2));
        }else{
            connection = __qt_this->connectToState(__qt_scxmlStateName0, [pointer](bool){
                if(JniEnvironment env{300}){
                    Java::QtCore::QMetaObject$Slot0::invoke(env, pointer.object());
                }
            }, Qt::ConnectionType(type2));
        }
        _result = qtjambi_cast<jobject>(__jni_env, connection);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
    return _result;
}

// QScxmlStateMachine::connectToEvent(const QString &scxmlStateName, Functor functor, Qt::ConnectionType type = Qt::AutoConnection)
extern "C" Q_DECL_EXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_scxml_QScxmlStateMachine_connectToEvent)
(JNIEnv *__jni_env,
 jobject __this,
 QtJambiNativeID __this_nativeId,
 jstring scxmlStateName0,
 jobject slot1,
 jint type2)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QScxmlStateMachine::connectToEvent(const QString &scxmlStateName, Functor functor, Qt::ConnectionType type = Qt::AutoConnection)")
    Q_UNUSED(__this)
    jobject _result{nullptr};
    QTJAMBI_TRY{
        QScxmlStateMachine *__qt_this = QtJambiAPI::objectFromNativeId<QScxmlStateMachine>(__this_nativeId);
        QtJambiAPI::checkPointer(__jni_env, __qt_this);
        const QString&  __qt_scxmlStateName0 = qtjambi_cast<QString>(__jni_env, scxmlStateName0);
        JObjectWrapper pointer(__jni_env, slot1);
        QMetaObject::Connection connection;
        if(Java::QtCore::QMetaObject$Slot1::isInstanceOf(__jni_env, slot1)){
            connection = __qt_this->connectToEvent(__qt_scxmlStateName0, [pointer](const QScxmlEvent &event){
                if(JniEnvironment env{300}){
                    Java::QtCore::QMetaObject$Slot1::invoke(env, pointer.object(), qtjambi_cast<jobject>(env, event));
                }
            }, Qt::ConnectionType(type2));
        }else{
            connection = __qt_this->connectToEvent(__qt_scxmlStateName0, [pointer](const QScxmlEvent &){
                if(JniEnvironment env{300}){
                    Java::QtCore::QMetaObject$Slot0::invoke(env, pointer.object());
                }
            }, Qt::ConnectionType(type2));
        }

        _result = qtjambi_cast<jobject>(__jni_env, connection);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
    return _result;
}

#if QT_VERSION < QT_VERSION_CHECK(5,12,0)
#include <qscxmldatamodel.h>

// QScxmlDataModel::ForeachLoopBody::run(bool * ok)
extern "C" Q_DECL_EXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_scxml_QScxmlDataModel_00024ForeachLoopBody_run)
(JNIEnv *__jni_env,
 jobject,
 QtJambiNativeID __this_nativeId,
 jbooleanArray ok0)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QScxmlDataModel::ForeachLoopBody::run(bool * ok)")
    QTJAMBI_TRY{
        QScxmlDataModel::ForeachLoopBody *__qt_this = QtJambiAPI::objectFromNativeId<QScxmlDataModel::ForeachLoopBody>(__this_nativeId);
        QtJambiAPI::checkPointer(__jni_env, __qt_this);
        JBooleanArrayPointer boolPointer(__jni_env, ok0);
        __qt_this->run(boolPointer.pointer());
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
}

void deleter_ForeachLoopBody(void* ptr){
    delete reinterpret_cast<QScxmlDataModel::ForeachLoopBody*>(ptr);
}

void initialize_meta_info_ForeachLoopBody(){
    using namespace RegistryAPI;
    const std::type_info& typeId = registerUnspecificTypeInfo<QScxmlDataModel::ForeachLoopBody>("QScxmlDataModel::ForeachLoopBody", "io/qt/scxml/QScxmlDataModel$ForeachLoopBody");
    registerDeleter(typeId, &deleter_ForeachLoopBody);
}
#endif
