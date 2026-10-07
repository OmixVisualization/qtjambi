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
    packageName: "io.qt.coap"
    defaultSuperClass: "QtObject"
    qtLibrary: "QtCoap"
    module: "qtjambi.coap"
    description: "Constrained Application Protocol (CoAP) is a machine-to-machine (M2M) web transfer protocol for use with constrained nodes and constrained networks in the Internet of Things (IoT). It is designed to easily interface with HTTP for integration with the Web, while meeting specialized requirements such as multicast support, very low overhead, and simplicity for constrained environments."
    NamespaceType{
        name: "QtCoap"
        EnumType{
            name: "Error"
        }
        EnumType{
            name: "Method"
        }
        EnumType{
            name: "MulticastGroup"
        }
        EnumType{
            name: "Port"
        }
        EnumType{
            name: "ResponseCode"
        }
        EnumType{
            name: "SecurityMode"
        }
    }
    ObjectType{
        name: "QCoapClient"
        ModifyFunction{
            signature: "disconnect()"
            rename: "disconnectClient"
        }
        ModifyFunction{
            signature: "deleteResource(const QCoapRequest &)"
            ModifyArgument{
                index: 0
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Java
                }
            }
        }
        ModifyFunction{
            signature: "deleteResource(const QUrl &)"
            ModifyArgument{
                index: 0
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Java
                }
            }
        }
        ModifyFunction{
            signature: "discover(const QUrl &, const QString &)"
            ModifyArgument{
                index: 0
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Java
                }
            }
        }
        ModifyFunction{
            signature: "discover(QtCoap::MulticastGroup, int, const QString &)"
            ModifyArgument{
                index: 0
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Java
                }
            }
        }
        ModifyFunction{
            signature: "get(const QCoapRequest &)"
            ModifyArgument{
                index: 0
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Java
                }
            }
        }
        ModifyFunction{
            signature: "get(const QUrl &)"
            ModifyArgument{
                index: 0
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Java
                }
            }
        }
        ModifyFunction{
            signature: "observe(const QCoapRequest &)"
            ModifyArgument{
                index: 0
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Java
                }
            }
        }
        ModifyFunction{
            signature: "observe(const QUrl &)"
            ModifyArgument{
                index: 0
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Java
                }
            }
        }
        ModifyFunction{
            signature: "post(const QCoapRequest &, const QByteArray &)"
            ModifyArgument{
                index: 0
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Java
                }
            }
        }
        ModifyFunction{
            signature: "post(const QCoapRequest &, QIODevice *)"
            ModifyArgument{
                index: 0
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Java
                }
            }
        }
        ModifyFunction{
            signature: "post(const QUrl &, const QByteArray &)"
            ModifyArgument{
                index: 0
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Java
                }
            }
        }
        ModifyFunction{
            signature: "put(const QCoapRequest &, const QByteArray &)"
            ModifyArgument{
                index: 0
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Java
                }
            }
        }
        ModifyFunction{
            signature: "put(const QCoapRequest &, QIODevice *)"
            ModifyArgument{
                index: 0
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Java
                }
            }
        }
        ModifyFunction{
            signature: "put(const QUrl &, const QByteArray &)"
            ModifyArgument{
                index: 0
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Java
                }
            }
        }
    }
    ValueType{
        name: "QCoapMessage"
        EnumType{
            name: "Type"
        }
        ModifyFunction{
            signature: "addOption(QCoapOption)"
            ModifyArgument{
                index: 1
                noImplicitCalls: true
            }
        }
    }
    ValueType{
        name: "QCoapOption"
        EnumType{
            name: "OptionName"
        }
    }
    ValueType{
        name: "QCoapPrivateKey"
    }
    ObjectType{
        name: "QCoapReply"
    }
    ValueType{
        name: "QCoapRequest"
        ModifyFunction{
            signature: "QCoapRequest(QUrl,QCoapMessage::Type,QUrl)"
            ModifyArgument{
                index: 1
                noImplicitCalls: true
            }
        }
    }
    ValueType{
        name: "QCoapResource"
        ModifyFunction{
            signature: "interface()const"
            rename: "iface"
        }
    }
    ObjectType{
        name: "QCoapResourceDiscoveryReply"
    }
    ValueType{
        name: "QCoapSecurityConfiguration"
    }
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: Class 'QCoap*' has equals operators but no qHash() function. Hashcode of objects will consistently be 0."}
    SuppressedWarning{text: "WARNING(JavaGenerator) :: No ==/!= operator found for value type QCoap*."}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: Final class 'QCoapReply' set to non-final, as it is extended by other classes"}
}
