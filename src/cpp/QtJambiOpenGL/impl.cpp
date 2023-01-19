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

#include <QtOpenGL/QOpenGLVersionFunctionsFactory>
#include <QtOpenGL/qopenglversionfunctions.h>
#if defined(QT_OPENGL_ES_2)
#include <QtOpenGL/QOpenGLFunctions_ES2>
#endif
#include <QtJambi/qtjambi_cast.h>

QAbstractOpenGLFunctions* qtjambi_QOpenGLVersionFunctionsFactory_get(JNIEnv *__jni_env, jclass type, QOpenGLContext* context){
    //QAbstractOpenGLFunctions* __qt_return_value = nullptr;
    QString className = QtJambiAPI::getClassName(__jni_env, type);
    if(className == "io.qt.opengl.QOpenGLFunctions_ES2"){
#if defined(QT_OPENGL_ES_2)
        return QOpenGLVersionFunctionsFactory::get<QOpenGLFunctions_ES2>(context);
#else
    }else if(className.startsWith("io.qt.opengl.QOpenGLFunctions_")){
        className = className.mid(30);
        auto idx = className.indexOf('_');
        if(idx>0){
            bool ok = false;
            int majorVersion = QStringView(className).left(idx).toInt(&ok);
            if(ok){
                className = className.mid(idx+1);
                QSurfaceFormat::OpenGLContextProfile profile = QSurfaceFormat::NoProfile;
                if(className.endsWith("_Core")){
                    className = className.chopped(5);
                    profile = QSurfaceFormat::CoreProfile;
                }else if(className.endsWith("_Compatibility")){
                    className = className.chopped(14);
                    profile = QSurfaceFormat::CompatibilityProfile;
                }
                ok = false;
                int minorVersion = className.toInt(&ok);
                if(ok){
                    QOpenGLVersionProfile version;
                    version.setProfile(profile);
                    version.setVersion(majorVersion, minorVersion);
                    return QOpenGLVersionFunctionsFactory::get(version, context);
                }
            }
        }
#endif
    }
    return nullptr;
}

