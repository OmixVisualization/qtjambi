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
#include "utils_p.h"
#if QT_VERSION >= QT_VERSION_CHECK(6, 6, 0)
#include <QtJambi/JObjectWrapper>
#include <QtTextToSpeech/QTextToSpeech>
#include <QtJambi/Cast>
#endif

namespace Java{
    namespace QtTextToSpeech{
        QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/texttospeech,QTextToSpeechPlugin$CreateException,
            QTJAMBI_REPOSITORY_DEFINE_CONSTRUCTOR(Ljava/lang/String;)
            QTJAMBI_REPOSITORY_DEFINE_METHOD(getMessage,()Ljava/lang/String;)
        )
    }
    namespace QtMultimedia{
        QTJAMBI_REPOSITORY_DECLARE_CLASS(QAudioFormat,)
        QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/multimedia,QAudioFormat,)
        QTJAMBI_REPOSITORY_DECLARE_CLASS(QAudioBuffer,)
        QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/multimedia,QAudioBuffer,)
    }
}

#if QT_VERSION >= QT_VERSION_CHECK(6, 6, 0)

struct Prototype1{
    Prototype1(JNIEnv *env, jobject func) : wrapper(env, func){}
    void operator()(const QAudioBuffer& arg1){
        if(JniEnvironment env{200}){
            Java::QtCore::QMetaObject$Slot1::invoke(env, wrapper.object(), QtJambiAPI::convertNativeToJavaObjectAsCopy(env, &arg1, Java::QtMultimedia::QAudioBuffer::getClass(env)));
        }
    }
    JObjectWrapper wrapper;
};

struct Prototype2{
    Prototype2(JNIEnv *env, jobject func) : wrapper(env, func){}
    void operator()(const QAudioFormat& arg1, const QByteArray& arg2){
        if(JniEnvironment env{200}){
            Java::QtCore::QMetaObject$Slot2::invoke(env, wrapper.object(), QtJambiAPI::convertNativeToJavaObjectAsCopy(env, &arg1, Java::QtMultimedia::QAudioFormat::getClass(env)), qtjambi_cast<jobject>(env, arg2));
        }
    }
    JObjectWrapper wrapper;
};

void qtjambi_synthesize1(JNIEnv *env, QTextToSpeech *_this, jobject _context, const QString&  text, jobject func){
    QObject* context = qtjambi_cast<QObject*>(env, _context);
    auto out = [wrapper = JObjectWrapper(env, func)](const QAudioBuffer& arg1){
        if(JniEnvironment env{200}){
            Java::QtCore::QMetaObject$Slot1::invoke(env, wrapper.object(), QtJambiAPI::convertNativeToJavaObjectAsCopy(env, &arg1, Java::QtMultimedia::QAudioBuffer::getClass(env)));
        }
    };
    _this->synthesize(text, context, std::move(out));
}

void qtjambi_synthesize2(JNIEnv *env, QTextToSpeech *_this, jobject _context, const QString&  text, jobject func){
    QObject* context = qtjambi_cast<QObject*>(env, _context);
    auto out = [wrapper = JObjectWrapper(env, func)](const QAudioFormat& arg1, const QByteArray& arg2){
        if(JniEnvironment env{200}){
            Java::QtCore::QMetaObject$Slot2::invoke(env, wrapper.object(), QtJambiAPI::convertNativeToJavaObjectAsCopy(env, &arg1, Java::QtMultimedia::QAudioFormat::getClass(env)), qtjambi_cast<jobject>(env, arg2));
        }
    };
    _this->synthesize(text, context, std::move(out));
}
#endif
