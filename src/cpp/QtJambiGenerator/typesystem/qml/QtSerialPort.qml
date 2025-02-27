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
    packageName: "io.qt.serialport"
    defaultSuperClass: "QtObject"
    qtLibrary: "QtSerialPort"
    module: "qtjambi.serialport"
    description: "Provides access to hardware and virtual serial ports."
    
    ObjectType{
        name: "QSerialPort"
        EnumType{
            name: "DataErrorPolicy"
        }

        EnumType{
            name: "BaudRate"
        }

        EnumType{
            name: "DataBits"
        }

        EnumType{
            name: "Direction"
        }

        EnumType{
            name: "FlowControl"
        }

        EnumType{
            name: "Parity"
        }

        EnumType{
            name: "PinoutSignal"
        }

        EnumType{
            name: "SerialPortError"
        }

        EnumType{
            name: "StopBits"
        }
        ModifyFunction{
            signature: "handle()const"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "sendBreak(int)"
            remove: RemoveFlag.All
            until: 5
        }
        ModifyFunction{
            signature: "bindableStopBits(Qt::Disambiguated_t)"
            ModifyArgument{
                index: 1
                RemoveArgument{}
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "constexpr Qt::Disambiguated_t %out = Qt::Disambiguated;"}
                }
            }
            since: 6.7
        }
    }
    
    ValueType{
        name: "QSerialPortInfo"
    }
    
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: skipping *unmatched *type 'QSerialPort::DataErrorPolicy'"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: skipping *unmatched *type 'const QSerialPortInfoPrivate&'"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: Unable to decide type of property: 'DataErrorPolicy' in class 'QSerialPort'"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: enum 'QSerialPort::DataErrorPolicy' does not have a type entry or is not an enum"}
    SuppressedWarning{text: "WARNING(JavaGenerator) :: No ==/!= operator found for value type QSerialPortInfo."}
}
