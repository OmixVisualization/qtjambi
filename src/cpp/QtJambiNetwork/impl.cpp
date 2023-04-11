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

#include <QtCore/QMutex>
#include <QtNetwork>
#include <QtJambi/Global>
#include <QtJambi/QtJambiAPI>
#include <QtJambi/JObjectWrapper>
#include <QtJambi/JavaAPI>
#include <QtJambi/qtjambi_cast.h>
#include "utils_p.h"

namespace Java{
namespace QtNetwork{
QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/network,QTimeoutException,
                                QTJAMBI_REPOSITORY_DEFINE_CONSTRUCTOR(Ljava/lang/String;)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/network,QHostAddress$HostInfo,
                                QTJAMBI_REPOSITORY_DEFINE_FIELD(address,Lio/qt/network/QHostAddress;)
                                QTJAMBI_REPOSITORY_DEFINE_FIELD(port,S)
)
}
}

extern "C" Q_DECL_EXPORT bool JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_network_QSsl_isAvailable)
(JNIEnv *, jclass)
{
#if defined(QT_NO_SSL)
    return false;
#else
    return true;
#endif
}

// QHostInfo::lookupHost(const QString & name, Functor functor)
extern "C" Q_DECL_EXPORT jint JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_network_QHostInfo_lookupHost)
(JNIEnv *__jni_env,
 jclass,
 jstring name0,
 QtJambiNativeID context1,
 jobject slot)
{
    jint _result{0};
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QHostInfo::lookupHost(const QString & name, Functor functor)")
    QTJAMBI_TRY{
        if(!slot)
            JavaException::raiseNullPointerException(__jni_env, "slot must not be null." QTJAMBI_STACKTRACEINFO );
        const QString&  __qt_name0 = qtjambi_cast<QString>(__jni_env, name0);
        QObject*  __qt_context1 = QtJambiAPI::objectFromNativeId<QObject>(context1);
        JObjectWrapper objectWrapper(__jni_env, slot);
        _result = QHostInfo::lookupHost(__qt_name0, __qt_context1, [objectWrapper](const QHostInfo& info){
            if(JniEnvironment env{200}){
                jobject result = qtjambi_cast<jobject>(env, info);
                Java::QtCore::QMetaObject$Slot1::invoke(env, objectWrapper.object(), result);
            }
        });
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
    return _result;
}

class QHttpMultiPartIODevice{
public:
    static const void* d(const QHttpPart &value){
        return value.d.data();
    }
};

size_t qHash(const QHttpPart &value, size_t seed){
    return qHash(qintptr(QHttpMultiPartIODevice::d(value)), seed);
}
