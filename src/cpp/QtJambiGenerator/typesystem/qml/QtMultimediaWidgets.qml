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
    packageName: "io.qt.multimedia.widgets"
    defaultSuperClass: "QtObject"
    qtLibrary: "QtMultimediaWidgets"
    module: "qtjambi.multimediawidgets"
    description: "Widget classes for audio, video, radio and camera functionality."
    InjectCode{
        position: Position.Position4
        until: 5
        Text{content: "if(QtUtilities.isAvailableQtLibrary(\"OpenGL\"))\n"+
                      "QtUtilities.loadQtLibrary(\"OpenGL\");"}
    }
    
    RequiredLibrary{
        name: "QtOpenGL"
        mode: RequiredLibrary.ProvideOnly
        until: 5
    }
    
    ObjectType{
        name: "QGraphicsVideoItem"
        ModifyFunction{
            signature: "setMediaObject(QMediaObject *)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    variableName: "__rcMediaObject"
                    action: ReferenceCount.Set
                }
            }
            until: 5
        }
    }
    
    ObjectType{
        name: "QVideoWidget"
        ModifyFunction{
            signature: "setMediaObject(QMediaObject *)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    variableName: "__rcMediaObject"
                    action: ReferenceCount.Set
                }
            }
            until: 5
        }
    }
    
    ObjectType{
        name: "QCameraViewfinder"
        ModifyFunction{
            signature: "setMediaObject(QMediaObject *)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    variableName: "__rcMediaObject"
                    action: ReferenceCount.Set
                }
            }
            until: 5
        }
    }
    
    ObjectType{
        name: "QVideoWidgetControl"
    }
}
