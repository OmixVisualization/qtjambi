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
    packageName: "io.qt.webchannelquick"
    defaultSuperClass: "QtObject"
    qtLibrary: "QtWebChannelQuick"
    module: "qtjambi.webchannelquick"
    description: "Provides access to QObject or QML objects from HTML clients for seamless integration of Qt applications with HTML/JavaScript clients."
    ObjectType{
        name: "QQmlWebChannel"
        Rejection{
            functionName: "qmlAttachedProperties"
        }
        ExtraIncludes{
            Include{
                fileName: "QtJambiQml/hashes.h"
                location: Include.Global
            }
        }
        ModifyFunction{
            signature: "registerObjects(const QMap<QString, QVariant> &)"
            ModifyArgument{
                index: 1
                ReplaceType{
                    modifiedType: "java.util.@Nullable SortedMap<java.lang.@NonNull String, java.lang.@Nullable Object>"
                }
            }
        }
    }
}
