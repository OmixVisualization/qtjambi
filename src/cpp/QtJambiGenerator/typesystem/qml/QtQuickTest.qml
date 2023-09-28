/****************************************************************************
**
** Copyright (C) 2009-2023 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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
    packageName: "io.qt.quick.test"
    defaultSuperClass: "QtObject"
    qtLibrary: "QtQuickTest"
    module: "qtjambi.quicktest"
    description: "A unit test framework for QML applications, where the test cases are written as JavaScript functions."
    LoadTypeSystem{name: "QtQuick"; unless: "QTJAMBI_NO_QUICK"}

    RequiredLibrary{
        name: "QtQuick"
    }
    
    RequiredLibrary{
        name: "QtWidgets"
    }
    
    NamespaceType{
        name: "QQuickTest"
        ExtraIncludes{
            Include{
                fileName: "qquickitem.h"
                suppressed: true
                location: Include.Global
            }
            Include{
                fileName: "QtJambi/CoreAPI"
                location: Include.Global
            }
        }
        InjectCode{
            ImportFile{
                name: ":/io/qtjambi/generator/typesystem/QtJambiQuickTest.java"
                quoteAfterLine: "class QQuickTest___"
                quoteBeforeLine: "}// class"
            }
        }
    }
    
    GlobalFunction{
        signature: "quick_test_main(int, char**, const char*, const char*)"
        targetType: "QQuickTest"
        ModifyArgument{
            index: 1
            RemoveArgument{
            }
        }
        ModifyArgument{
            index: 2
            ReplaceType{
                modifiedType: "java.lang.@Nullable String @Nullable[]"
            }
            ConversionRule{
                codeClass: CodeClass.Native
                Text{content: "std::unique_ptr<ApplicationData> applicationData(new ApplicationData(%env, jobjectArray(%in)));\n"+
                              "char** %out = applicationData->chars();\n"+
                              "int %1 = applicationData->size();"}
            }
        }
    }
    
    GlobalFunction{
        signature: "quick_test_main_with_setup(int, char**, const char*, const char*, QObject*)"
        targetType: "QQuickTest"
        ModifyArgument{
            index: 1
            RemoveArgument{
            }
        }
        ModifyArgument{
            index: 2
            ReplaceType{
                modifiedType: "java.lang.@Nullable String @Nullable[]"
            }
            ConversionRule{
                codeClass: CodeClass.Native
                Text{content: "std::unique_ptr<ApplicationData> applicationData(new ApplicationData(%env, jobjectArray(%in)));\n"+
                              "char** %out = applicationData->chars();\n"+
                              "int %1 = applicationData->size();"}
            }
        }
    }
}
