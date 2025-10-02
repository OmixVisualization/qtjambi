/****************************************************************************
**
** Copyright (C) 2009-2025 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
**
** This file is part of QtJambi.
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

import QtJambiGenerator 1.0

TypeSystem{
    packageName: "io.qt.webview"
    defaultSuperClass: "QtObject"
    qtLibrary: "QtWebView"
    module: "qtjambi.webview"
    description: "Displays web content in a QML application by using APIs native to the platform, without the need to include a full web browser stack."
    RequiredLibrary{
        name: "QtWebEngineQuick"
        mode: RequiredLibrary.Supressed
        since: 6.8
    }
    RequiredLibrary{
        name: "QtWebEngineQuick"
        mode: RequiredLibrary.ProvideOnly
        since: 6
    }
    RequiredLibrary{
        name: "QtWebEngine"
        mode: RequiredLibrary.ProvideOnly
        until: 5
    }
    RequiredLibrary{
        name: "QtQuick"
        mode: RequiredLibrary.Mandatory
        until: 5
    }
    
    NamespaceType{
        name: "QtWebView"
        ExtraIncludes{
            Include{
                fileName: "QtCore/QLibrary"
                location: Include.Global
                ppCondition: "defined(Q_OS_ANDROID)"
            }
            since: 6.8
        }
        ModifyFunction{
            signature: "initialize()"
            InjectCode{
                target: CodeClass.Native
                position: Position.End
                Text{content: String.raw`
#ifdef Q_OS_ANDROID
        QLibrary library("libplugins_webview_qtwebview_android");
        if(library.load()){
            typedef jint (*JNI_OnLoadFn)(JavaVM*,void*);
            JNI_OnLoadFn onLoad = JNI_OnLoadFn(library.resolve("JNI_OnLoad"));
            if(onLoad){
                onLoad(nullptr,nullptr);
            }
        }else{
            qWarning() << library.errorString();
        }
#endif
`}
            }
            since: 6.8
        }
    }
}
