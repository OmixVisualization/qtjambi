/****************************************************************************
**
** Copyright (C) 2009-2026 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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
    packageName: "io.qt.mqtt"
    defaultSuperClass: "QtObject"
    qtLibrary: "QtMqtt"
    module: "qtjambi.mqtt"
    description: "MQTT is a machine-to-machine (M2M) protocol utilizing the publish-and-subscribe paradigm. Its purpose is to provide a channel with minimal communication overhead."
    NamespaceType{
        name: "QMqtt"
        EnumType{
            name: "MessageStatus"
        }
        EnumType{
            name: "PayloadFormatIndicator"
        }
        EnumType{
            name: "ReasonCode"
        }
    }
    ObjectType{
        name: "QMqttAuthenticationProperties"
    }
    ObjectType{
        name: "QMqttClient"
        EnumType{
            name: "ClientError"
        }
        EnumType{
            name: "ClientState"
        }
        EnumType{
            name: "ProtocolVersion"
        }
        EnumType{
            name: "TransportType"
        }
        ModifyFunction{
            signature: "setTransport(QIODevice *, QMqttClient::TransportType)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    variableName: "__rcTransport"
                    action: ReferenceCount.Set
                }
            }
        }
    }
    ObjectType{
        name: "QMqttConnectionProperties"
    }
    ObjectType{
        name: "QMqttLastWillProperties"
    }
    ObjectType{
        name: "QMqttMessageStatusProperties"
    }
    ValueType{
        name: "QMqttMessage"
    }
    ObjectType{
        name: "QMqttPublishProperties"
        EnumType{
            name: "PublishPropertyDetail"
        }
    }
    ObjectType{
        name: "QMqttServerConnectionProperties"
        EnumType{
            name: "ServerPropertyDetail"
        }
    }
    ValueType{
        name: "QMqttStringPair"
    }
    ObjectType{
        name: "QMqttSubscription"
        EnumType{
            name: "SubscriptionState"
        }
    }
    ObjectType{
        name: "QMqttSubscriptionProperties"
    }
    ValueType{
        name: "QMqttTopicFilter"
        ModifyFunction{
            signature: "QMqttTopicFilter(QLatin1String)"
            remove: RemoveFlag.All
        }
        EnumType{
            name: "MatchOption"
        }
    }
    ValueType{
        name: "QMqttTopicName"
        ModifyFunction{
            signature: "QMqttTopicName(QLatin1String)"
            remove: RemoveFlag.All
        }
    }
    ObjectType{
        name: "QMqttUnsubscriptionProperties"
    }
    ValueType{
        name: "QMqttUserProperties"
    }
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: duplicate enum values: QMqtt::ReasonCode, *"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: Class 'QMqtt*' has equals operators but no qHash() function. Hashcode of objects will consistently be 0."}
}
