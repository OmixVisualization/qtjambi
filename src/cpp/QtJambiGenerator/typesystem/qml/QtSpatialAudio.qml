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
    packageName: "io.qt.spatialaudio"
    defaultSuperClass: "QtObject"
    qtLibrary: "QtSpatialAudio"
    module: "qtjambi.spatialaudio"
    description: "Qt Spatial Audio is an add-on module that provides a rich set of QML types and C++ classes to implement sound fields in 3D space."

    EnumType{
        name: "QAudioEngine::OutputMode"
    }
    
    EnumType{
        name: "QAmbientSound::Loops"
    }
    
    EnumType{
        name: "QAudioRoom::Material"
    }
    
    EnumType{
        name: "QAudioRoom::Wall"
    }
    
    EnumType{
        name: "QSpatialSound::Loops"
    }
    
    EnumType{
        name: "QSpatialSound::DistanceModel"
    }
    
    ObjectType{
        name: "QAudioEngine"
        ModifyField{
            name: "DistanceScaleCentimeter"
            read: false
        }
        ModifyField{
            name: "DistanceScaleMeter"
            read: false
        }
        InjectCode{
            target: CodeClass.Java
            Text{content: "public static final float DistanceScaleCentimeter = 1.f;\n"+
                          "public static final float DistanceScaleMeter = 100.f;"}
        }
    }
    
    ObjectType{
        name: "QAmbientSound"
    }
    
    ObjectType{
        name: "QAudioListener"
    }
    
    ObjectType{
        name: "QAudioRoom"
    }
    
    ObjectType{
        name: "QSpatialSound"
    }
}
