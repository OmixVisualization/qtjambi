/****************************************************************************
**
** Copyright (C) 2009-2026 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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

#include <QtQuickTest/QtQuickTest>

#include <QtJambi/QtJambiAPI>
#include <QtJambi/qtjambi_cast.h>
#include <QtCore/private/qcoreapplication_p.h>

extern "C" JNIEXPORT jint JNICALL Java_io_qt_quick_test_QQuickTest_quickTestMainWithSetup
(JNIEnv *env, jclass, jobjectArray args, jstring name, jstring sourceDir, QtJambiNativeID setup)
{
    J2CStringBuffer _name(env, name);
    J2CStringBuffer _sourceDir(env, sourceDir);
    int argc;
    char** argv;
    QByteArray appName;
    QVector<QByteArray> argsBuffers;
    QCoreApplication* app = QCoreApplication::instance();
    if (QCoreApplicationPrivate* pvt = app ? dynamic_cast<QCoreApplicationPrivate*>(QCoreApplicationPrivate::get(app)) : nullptr){
        argc = pvt->argc;
        argv = pvt->argv;
    }else{
        argc = int(env->GetArrayLength(args));
        argv = new char*[size_t(argc+1)];
        appName = qAppName().toUtf8();
        if(appName.isEmpty())
            appName = _name.data();
        argv[0] = appName.data();
        argsBuffers.resize(argc);
        for(jsize i=0; i<argc; ++i){
            jstring strg = jstring(env->GetObjectArrayElement(args, i));
            const char* data = strg && env->GetStringUTFLength(strg)>0 ? env->GetStringUTFChars(strg, nullptr) : nullptr;
            argsBuffers[i] = data;
            argv[i+1] = argsBuffers[i].data();
            if(data)
                env->ReleaseStringUTFChars(strg, data);
        }
    }
    QObject* _setup = QtJambiAPI::objectFromNativeId<QObject>(setup);
    return quick_test_main_with_setup(argc, argv, _name.data(), _sourceDir.data(), _setup);
}
