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
    packageName: "io.qt.grpc"
    defaultSuperClass: "QtObject"
    qtLibrary: "QtGrpc"
    module: "qtjambi.grpc"
    description: "Provides an ability to generate Qt-based classes from protobuf specifications used to communicate with gRPC services."
    ObjectType{
        name: "QAbstractGrpcChannel"
    }
    ObjectType{
        name: "QAbstractGrpcClient"
        ExtraIncludes{
            Include{
                fileName: "utils_p.h"
                location: Include.Local
                since: 6
            }
        }
    }
    ObjectType{
        name: "QAbstractGrpcCredentials"
    }
    ObjectType{
        name: "QGrpcCallCredentials"
    }
    ObjectType{
        name: "QGrpcChannelOptions"
        since: 6.6
    }
    ObjectType{
        name: "QGrpcCallOptions"
        since: 6.6
    }
    ObjectType{
        name: "QGrpcCallReply"
    }
    ObjectType{
        name: "QGrpcChannelCredentials"
    }
    ObjectType{
        name: "QGrpcHttp2Channel"
    }
    ObjectType{
        name: "QGrpcChannelOptions"
    }
    ObjectType{
        name: "QGrpcCallOptions"
    }
    ObjectType{
        name: "QGrpcInsecureCallCredentials"
    }
    ObjectType{
        name: "QGrpcInsecureChannelCredentials"
    }
    ObjectType{
        name: "QGrpcOperation"
        ExtraIncludes{
            Include{
                fileName: "utils_p.h"
                location: Include.Local
                since: 6
            }
        }
        ModifyFunction{
            signature: "read<T>()const"
            Instantiation{
                proxyCall: "qtjambi_QGrpcOperation_read"
                Argument{
                    type: "jobject"
                }
                ModifyArgument{
                    index: 0
                    ReplaceType{
                        modifiedType: "T"
                        modifiedJniType: "jobject"
                    }
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "%out = %in;"}
                    }
                }
                AddTypeParameter{
                    name: "T"
                }
                AddArgument{
                    name: "clazz"
                    type: "java.lang.Class<T>"
                }
                AddArgument{
                    name: "instantiations"
                    type: "io.qt.core.QMetaType..."
                }
            }
        }
    }
    ObjectType{
        name: "QGrpcSslCredentials"
    }
    ValueType{
        name: "QGrpcStatus"
    }
    ObjectType{
        name: "QGrpcStream"
        FunctionalType{
            name: "StreamHandler"
        }
    }
    ObjectType{
        name: "QGrpcUserPasswordCredentials"
    }
}
