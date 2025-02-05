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

#include <QtCore/QCoreApplication>
#include <QtCore/QFileInfo>
#include <QtCore/QDir>
#include "qtjambiapi.h"
#include "coreapi.h"
#include "java_p.h"
#include "threadutils_p.h"
#include "qtjambi_cast.h"

void checkThreadConstructingApplication(JNIEnv *env, const std::type_info& constructedType);

std::unique_ptr<ApplicationData> ApplicationData::initialize(JNIEnv *env, jobjectArray array, const std::type_info& constructedType, bool checkMainThread){
#ifdef Q_OS_DARWIN
    if (checkMainThread && !pthread_main_np()) {
        JavaException::raiseError(env, "QtJambi does not appear to be running on the main thread and will "
                                       "most likely be unstable and crash. "
                                       "Please make sure to launch your 'java' command with the "
                                       "'-XstartOnFirstThread' command line option. For instance: "
                                       "java -XstartOnFirstThread any.vendor.MainClass" QTJAMBI_STACKTRACEINFO );
    }
#else
    Q_UNUSED(checkMainThread)
#endif
    ThreadPrivate::initializeMainThread(env);
    checkThreadConstructingApplication(env, constructedType);
    int size = array ? int(env->GetArrayLength(array)) : 0;
    char** chars = size>0 ? new char*[size_t(size)] : nullptr;
    for(jsize i=0; i<size; ++i){
        jstring strg = jstring(env->GetObjectArrayElement(array, i));
        if(strg){
            jsize length = env->GetStringUTFLength(strg);
            if(length>=0){
                chars[i] = new char[size_t(length+1)];
                chars[i][length] = 0;
                env->GetStringUTFRegion(strg, 0, length, chars[i]);
            }else{
                chars[i] = nullptr;
            }
        }else{
            chars[i] = new char[1];
            chars[i][0] = 0;
        }
    }
    return std::unique_ptr<ApplicationData>(new ApplicationData(size, chars));
}

ApplicationData::ApplicationData(int size, char** chars)
    : QtJambiObjectData(), m_size(size), m_chars(chars)
{
}

ApplicationData::~ApplicationData(){
    for(jsize i=0; i<m_size; ++i){
        delete[] m_chars[i];
    }
    delete[] m_chars;
}

char** ApplicationData::chars(){
    return m_chars;
}

int& ApplicationData::size(){
    return m_size;
}

void ApplicationData::update(JNIEnv *env){
    if(m_size>0 && (!m_chars[0] || m_chars[0][0]==0)){
        QDir sunBootLibraryPath(qtjambi_cast<QString>(env, Java::Runtime::System::getProperty(env, env->NewStringUTF("sun.boot.library.path"), nullptr)));
        if(sunBootLibraryPath.exists() && sunBootLibraryPath.cdUp()){
            if(QFileInfo(QCoreApplication::applicationFilePath()).dir().canonicalPath().startsWith(sunBootLibraryPath.canonicalPath())){
                return;
            }
        }
        QByteArray path = QCoreApplication::applicationFilePath().toLocal8Bit();
        char* newArg0 = new char[path.size()+1];
        newArg0[path.size()] = 0;
        memcpy(newArg0, path.data(), path.size());
        std::swap(m_chars[0], newArg0);
        delete[] newArg0;
    }
}

QTJAMBI_OBJECTUSERDATA_ID_IMPL(,ApplicationData::)
