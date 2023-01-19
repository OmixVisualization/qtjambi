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
    packageName: "io.qt.dbus"
    defaultSuperClass: "io.qt.QtObject"
    qtLibrary: "QtDBus"
    module: "qtjambi.dbus"
    description: "Classes for inter-process communication over the D-Bus protocol."
    InjectCode{
        target: CodeClass.MetaInfo
        position: Position.Position1
        Text{content: "void initialize_meta_info_QtDBus();"}
    }
    
    InjectCode{
        target: CodeClass.MetaInfo
        Text{content: "initialize_meta_info_QtDBus();"}
    }
    
    Rejection{
        className: "QDBusPendingReply"
    }
    
    Rejection{
        className: "QDBusPendingReply::Select"
    }
    
    Rejection{
        className: "QtJambiDBusPendingReply"
    }
    
    Rejection{
        className: "QDBusAbstractInterfacePrivate"
    }
    
    Rejection{
        className: "DBusError"
    }
    
    Rejection{
        className: "QDBusConnection"
        functionName: "registerVirtualObject"
    }
    
    Rejection{
        className: ""
        functionName: "qDBusReplyFill"
    }
    
    Rejection{
        className: ""
        functionName: "qDBusRegisterMetaType"
    }
    
    Rejection{
        className: ""
        functionName: "qdbus_cast"
    }
    
    Rejection{
        className: ""
        functionName: "qDBusDemarshallHelper"
    }
    
    Rejection{
        className: ""
        functionName: "qDBusMarshallHelper"
    }
    
    NamespaceType{
        name: "QDBus"
        ExtraIncludes{
            Include{
                fileName: "QtDBus/QtDBus"
                location: Include.Global
            }
        }
    }
    
    EnumType{
        name: "QDBus::CallMode"
    }
    
    EnumType{
        name: "QDBusConnectionInterface::ServiceQueueOptions"
    }
    
    EnumType{
        name: "QDBusConnectionInterface::RegisterServiceReply"
    }
    
    EnumType{
        name: "QDBusConnectionInterface::ServiceReplacementOptions"
    }
    
    EnumType{
        name: "QDBusConnection::VirtualObjectRegisterOption"
    }
    
    EnumType{
        name: "QDBusConnection::RegisterOption"
        flags: "QDBusConnection::RegisterOptions"
        RejectEnumValue{
            name: "ExportAllSignal"
        }
    }
    
    EnumType{
        name: "QDBusConnection::UnregisterMode"
    }
    
    
    EnumType{
        name: "QDBusArgument::ElementType"
    }
    
    
    EnumType{
        name: "QDBusConnection::BusType"
    }
    
    
    EnumType{
        name: "QDBusConnection::ConnectionCapability"
        flags: "QDBusConnection::ConnectionCapabilities"
    }
    
    
    EnumType{
        name: "QDBusConnection::RegisterServiceReply"
    }
    
    
    EnumType{
        name: "QDBusConnection::ServiceQueueOptions"
    }
    
    
    EnumType{
        name: "QDBusConnection::ServiceReplacementOption"
        flags: "QDBusConnection::ServiceReplacementOptions"
    }
    
    
    EnumType{
        name: "QDBusError::ErrorType"
        RejectEnumValue{
            name: "LastErrorType"
        }
    }
    
    EnumType{
        name: "QDBusMessage::MessageType"
    }
    
    EnumType{
        name: "QDBusServiceWatcher::WatchModeFlag"
        flags: "QDBusServiceWatcher::WatchMode"
    }
    
    
    ObjectType{
        name: "QDBusServer"
    }
    
    ObjectType{
        name: "QDBusVirtualObject"
    }
    
    ObjectType{
        name: "QDBusAbstractAdaptor"
    }
    
    
    ObjectType{
        name: "QDBusAbstractInterface"
        ModifyFunction{
            signature: "interface() const"
            rename: "interfaceName"
        }
        ModifyFunction{
            signature: "asyncCall(const QString &, const QVariant &, const QVariant &, const QVariant &, const QVariant &, const QVariant &, const QVariant &, const QVariant &, const QVariant &)"
            remove: RemoveFlag.All
            until: 5
        }
        ModifyFunction{
            signature: "call(const QString &, const QVariant &, const QVariant &, const QVariant &, const QVariant &, const QVariant &, const QVariant &, const QVariant &, const QVariant &)"
            remove: RemoveFlag.All
            until: 5
        }
        ModifyFunction{
            signature: "call(const QString &)"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "call(QDBus::CallMode, const QString &, const QVariant &, const QVariant &, const QVariant &, const QVariant &, const QVariant &, const QVariant &, const QVariant &, const QVariant &)"
            remove: RemoveFlag.All
            until: 5
        }
        ModifyFunction{
            signature: "call(QDBus::CallMode, const QString &)"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "asyncCallWithArgumentList(const QString &, const QList<QVariant > &)"
            ModifyArgument{
                index: "return"
                ReplaceType{
                    modifiedType: "io.qt.dbus.QDBusPendingCall"
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "%out = qtjambi_cast<jobject>(%env, %in);"}
                }
            }
        }
        ModifyFunction{
            signature: "QDBusAbstractInterface(const QString &, const QString &, const char *, const QDBusConnection &, QObject *)"
            ModifyArgument{
                index: 3
                ReplaceType{
                    modifiedType: "java.lang.String"
                }
            }
        }
        ModifyFunction{
            signature: "internalPropSet(const char *,const QVariant &)"
            ModifyArgument{
                index: 1
                ReplaceType{
                    modifiedType: "java.lang.String"
                }
            }
        }
        ModifyFunction{
            signature: "internalPropGet(const char *)const"
            ModifyArgument{
                index: 1
                ReplaceType{
                    modifiedType: "java.lang.String"
                }
            }
        }
        ModifyFunction{
            signature: "callWithCallback(const QString &,const QList<QVariant> &,QObject *, const char *)"
            ModifyArgument{
                index: 4
                ReplaceType{
                    modifiedType: "java.lang.String"
                }
            }
            InjectCode{
                target: CodeClass.Java
                position: Position.Beginning
                ArgumentMap{
                    index: 3
                    metaName: "object"
                }
                ArgumentMap{
                    index: 4
                    metaName: "member"
                }
                Text{content: "if(member!=null && !member.startsWith(\"1\") && !member.startsWith(\"2\")) {\n"+
                              "    io.qt.core.QMetaMethod _method = object.metaObject().method(member);\n"+
                              "    if(_method!=null && _method.isValid()) {\n"+
                              "        if(_method.methodType()==io.qt.core.QMetaMethod.MethodType.Signal)\n"+
                              "            member = \"2\" + _method.cppMethodSignature();\n"+
                              "        else\n"+
                              "            member = \"1\" + _method.cppMethodSignature();\n"+
                              "    }\n"+
                              "}"}
            }
        }
        ModifyFunction{
            signature: "callWithCallback(const QString &,const QList<QVariant> &,QObject *, const char *, const char *)"
            ModifyArgument{
                index: 4
                ReplaceType{
                    modifiedType: "java.lang.String"
                }
            }
            ModifyArgument{
                index: 5
                ReplaceType{
                    modifiedType: "java.lang.String"
                }
            }
            InjectCode{
                target: CodeClass.Java
                position: Position.Beginning
                ArgumentMap{
                    index: 3
                    metaName: "object"
                }
                ArgumentMap{
                    index: 4
                    metaName: "member"
                }
                ArgumentMap{
                    index: 5
                    metaName: "errorSlot"
                }
                Text{content: "if(member!=null && !member.startsWith(\"1\") && !member.startsWith(\"2\")) {\n"+
                              "    io.qt.core.QMetaMethod _method = object.metaObject().method(member);\n"+
                              "    if(_method!=null && _method.isValid()) {\n"+
                              "        if(_method.methodType()==io.qt.core.QMetaMethod.MethodType.Signal)\n"+
                              "            member = \"2\" + _method.cppMethodSignature();\n"+
                              "        else\n"+
                              "            member = \"1\" + _method.cppMethodSignature();\n"+
                              "    }\n"+
                              "}\n"+
                              "if(errorSlot!=null && !errorSlot.startsWith(\"1\") && !errorSlot.startsWith(\"2\")) {\n"+
                              "    io.qt.core.QMetaMethod _method = object.metaObject().method(errorSlot);\n"+
                              "    if(_method!=null && _method.isValid()) {\n"+
                              "        if(_method.methodType()==io.qt.core.QMetaMethod.MethodType.Signal)\n"+
                              "            errorSlot = \"2\" + _method.cppMethodSignature();\n"+
                              "        else\n"+
                              "            errorSlot = \"1\" + _method.cppMethodSignature();\n"+
                              "    }\n"+
                              "}"}
            }
        }
        InjectCode{
            Text{content: "@io.qt.QtUninvokable\n"+
                          "public final io.qt.dbus.QDBusPendingCall asyncCall(java.lang.String method, java.lang.Object... args){\n"+
                          "    return asyncCallWithArgumentList(method, java.util.Arrays.asList(args));\n"+
                          "}\n"+
                          "\n"+
                          "@io.qt.QtUninvokable\n"+
                          "public final io.qt.dbus.QDBusMessage call(io.qt.dbus.QDBus.CallMode mode, java.lang.String method, java.lang.Object... args){\n"+
                          "    return callWithArgumentList(mode, method, java.util.Arrays.asList(args));\n"+
                          "}\n"+
                          "\n"+
                          "@io.qt.QtUninvokable\n"+
                          "public final io.qt.dbus.QDBusMessage call(java.lang.String method, java.lang.Object... args){\n"+
                          "    return callWithArgumentList(io.qt.dbus.QDBus.CallMode.AutoDetect, method, java.util.Arrays.asList(args));\n"+
                          "}"}
        }
    }
    
    ObjectType{
        name: "QDBusAbstractInterfaceBase"
        ModifyFunction{
            signature: "qt_metacall(QMetaObject::Call,int,void**)"
            remove: RemoveFlag.All
        }
    }
    
    ValueType{
        name: "QDBusArgument"
        ModifyFunction{
            signature: "beginMapEntry() const"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "beginStructure() const"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "endStructure() const"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "endArray() const"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "endMap() const"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "endMapEntry() const"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "operator<<(uint)"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "operator<<(unsigned long long)"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "operator<<(unsigned short)"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "operator=(const QDBusArgument &)"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "operator>>(unsigned short &)const"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "operator>>(uint &)const"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "operator>>(unsigned long long &)const"
            remove: RemoveFlag.All
        }
        InjectCode{
            target: CodeClass.Native
            position: Position.Beginning
            Text{content: "void qtjambi_dbus_check_write_argument(JNIEnv *,const QDBusArgument *);\n"+
                          "void qtjambi_dbus_check_read_argument(JNIEnv *,const QDBusArgument *);\n"+
                          "bool qtjambi_dbus_is_read_argument(const QDBusArgument *);"}
        }
        ModifyFunction{
            signature: "beginMapEntry()"
            InjectCode{
                target: CodeClass.Native
                position: Position.Beginning
                Text{content: "if(qtjambi_dbus_is_read_argument(__qt_this)){\n"+
                              "    const_cast<const QDBusArgument *>(__qt_this)->beginMapEntry();\n"+
                              "    return;\n"+
                              "}"}
            }
        }
        ModifyFunction{
            signature: "beginStructure()"
            InjectCode{
                target: CodeClass.Native
                position: Position.Beginning
                Text{content: "if(qtjambi_dbus_is_read_argument(__qt_this)){\n"+
                              "    const_cast<const QDBusArgument *>(__qt_this)->beginStructure();\n"+
                              "    return;\n"+
                              "}"}
            }
        }
        ModifyFunction{
            signature: "endStructure()"
            InjectCode{
                target: CodeClass.Native
                position: Position.Beginning
                Text{content: "if(qtjambi_dbus_is_read_argument(__qt_this)){\n"+
                              "    const_cast<const QDBusArgument *>(__qt_this)->endStructure();\n"+
                              "    return;\n"+
                              "}"}
            }
        }
        ModifyFunction{
            signature: "endArray()"
            InjectCode{
                target: CodeClass.Native
                position: Position.Beginning
                Text{content: "if(qtjambi_dbus_is_read_argument(__qt_this)){\n"+
                              "    const_cast<const QDBusArgument *>(__qt_this)->endArray();\n"+
                              "    return;\n"+
                              "}"}
            }
        }
        ModifyFunction{
            signature: "endMap()"
            InjectCode{
                target: CodeClass.Native
                position: Position.Beginning
                Text{content: "if(qtjambi_dbus_is_read_argument(__qt_this)){\n"+
                              "    const_cast<const QDBusArgument *>(__qt_this)->endMap();\n"+
                              "    return;\n"+
                              "}"}
            }
        }
        ModifyFunction{
            signature: "endMapEntry()"
            InjectCode{
                target: CodeClass.Native
                position: Position.Beginning
                Text{content: "if(qtjambi_dbus_is_read_argument(__qt_this)){\n"+
                              "    const_cast<const QDBusArgument *>(__qt_this)->endMapEntry();\n"+
                              "    return;\n"+
                              "}"}
            }
        }
        ModifyFunction{
            signature: "asVariant() const"
            InjectCode{
                target: CodeClass.Native
                position: Position.Beginning
                Text{content: "qtjambi_dbus_check_read_argument(%env, __qt_this);"}
            }
        }
        ModifyFunction{
            signature: "atEnd() const"
            InjectCode{
                target: CodeClass.Native
                position: Position.Beginning
                Text{content: "qtjambi_dbus_check_read_argument(%env, __qt_this);"}
            }
        }
        ModifyFunction{
            signature: "appendVariant(QVariant)"
            InjectCode{
                target: CodeClass.Native
                position: Position.Beginning
                Text{content: "qtjambi_dbus_check_write_argument(%env, __qt_this);"}
            }
        }
        ModifyFunction{
            signature: "beginArray(int)"
            InjectCode{
                target: CodeClass.Native
                position: Position.Beginning
                Text{content: "qtjambi_dbus_check_write_argument(%env, __qt_this);"}
            }
        }
        ModifyFunction{
            signature: "beginArray(QMetaType)"
            InjectCode{
                target: CodeClass.Native
                position: Position.Beginning
                Text{content: "qtjambi_dbus_check_write_argument(%env, __qt_this);"}
            }
            since: 6
        }
        ModifyFunction{
            signature: "beginMap(int,int)"
            InjectCode{
                target: CodeClass.Native
                position: Position.Beginning
                Text{content: "qtjambi_dbus_check_write_argument(%env, __qt_this);"}
            }
        }
        ModifyFunction{
            signature: "beginMap(QMetaType,QMetaType)"
            InjectCode{
                target: CodeClass.Native
                position: Position.Beginning
                Text{content: "qtjambi_dbus_check_write_argument(%env, __qt_this);"}
            }
            since: 6
        }
        ModifyFunction{
            signature: "operator<<(int)"
            rename: "append"
            InjectCode{
                target: CodeClass.Native
                position: Position.Beginning
                Text{content: "qtjambi_dbus_check_write_argument(%env, __qt_this);"}
            }
            ModifyArgument{
                index: 0
                replaceValue: "this"
            }
        }
        ModifyFunction{
            signature: "operator<<(long long)"
            rename: "append"
            InjectCode{
                target: CodeClass.Native
                position: Position.Beginning
                Text{content: "qtjambi_dbus_check_write_argument(%env, __qt_this);"}
            }
            ModifyArgument{
                index: 0
                replaceValue: "this"
            }
        }
        ModifyFunction{
            signature: "operator<<(short)"
            rename: "append"
            InjectCode{
                target: CodeClass.Native
                position: Position.Beginning
                Text{content: "qtjambi_dbus_check_write_argument(%env, __qt_this);"}
            }
            ModifyArgument{
                index: 0
                replaceValue: "this"
            }
        }
        ModifyFunction{
            signature: "operator<<(unsigned char)"
            rename: "append"
            InjectCode{
                target: CodeClass.Native
                position: Position.Beginning
                Text{content: "qtjambi_dbus_check_write_argument(%env, __qt_this);"}
            }
            ModifyArgument{
                index: 0
                replaceValue: "this"
            }
        }
        ModifyFunction{
            signature: "operator<<(bool)"
            rename: "append"
            InjectCode{
                target: CodeClass.Native
                position: Position.Beginning
                Text{content: "qtjambi_dbus_check_write_argument(%env, __qt_this);"}
            }
            ModifyArgument{
                index: 0
                replaceValue: "this"
            }
        }
        ModifyFunction{
            signature: "operator<<(double)"
            rename: "append"
            InjectCode{
                target: CodeClass.Native
                position: Position.Beginning
                Text{content: "qtjambi_dbus_check_write_argument(%env, __qt_this);"}
            }
            ModifyArgument{
                index: 0
                replaceValue: "this"
            }
        }
        ModifyFunction{
            signature: "operator<<(const QString &)"
            rename: "append"
            InjectCode{
                target: CodeClass.Native
                position: Position.Beginning
                Text{content: "qtjambi_dbus_check_write_argument(%env, __qt_this);"}
            }
            ModifyArgument{
                index: 0
                replaceValue: "this"
            }
        }
        ModifyFunction{
            signature: "operator<<(const QDBusVariant &)"
            rename: "append"
            InjectCode{
                target: CodeClass.Native
                position: Position.Beginning
                Text{content: "qtjambi_dbus_check_write_argument(%env, __qt_this);"}
            }
            ModifyArgument{
                index: 0
                replaceValue: "this"
            }
        }
        ModifyFunction{
            signature: "operator<<(const QStringList &)"
            rename: "append"
            InjectCode{
                target: CodeClass.Native
                position: Position.Beginning
                Text{content: "qtjambi_dbus_check_write_argument(%env, __qt_this);"}
            }
            ModifyArgument{
                index: 0
                replaceValue: "this"
            }
        }
        ModifyFunction{
            signature: "operator<<(const QByteArray &)"
            rename: "append"
            InjectCode{
                target: CodeClass.Native
                position: Position.Beginning
                Text{content: "qtjambi_dbus_check_write_argument(%env, __qt_this);"}
            }
            ModifyArgument{
                index: 0
                replaceValue: "this"
            }
        }
        ModifyFunction{
            signature: "operator<<(const QDBusObjectPath &)"
            rename: "append"
            InjectCode{
                target: CodeClass.Native
                position: Position.Beginning
                Text{content: "qtjambi_dbus_check_write_argument(%env, __qt_this);"}
            }
            ModifyArgument{
                index: 0
                replaceValue: "this"
            }
        }
        ModifyFunction{
            signature: "operator<<(const QDBusSignature &)"
            rename: "append"
            InjectCode{
                target: CodeClass.Native
                position: Position.Beginning
                Text{content: "qtjambi_dbus_check_write_argument(%env, __qt_this);"}
            }
            ModifyArgument{
                index: 0
                replaceValue: "this"
            }
        }
        ModifyFunction{
            signature: "operator<<(const QDBusUnixFileDescriptor &)"
            rename: "append"
            InjectCode{
                target: CodeClass.Native
                position: Position.Beginning
                Text{content: "qtjambi_dbus_check_write_argument(%env, __qt_this);"}
            }
            ModifyArgument{
                index: 0
                replaceValue: "this"
            }
        }
        ModifyFunction{
            signature: "operator>>(unsigned char &)const"
            rename: "extractByte"
            InjectCode{
                target: CodeClass.Native
                position: Position.Beginning
                Text{content: "qtjambi_dbus_check_read_argument(%env, __qt_this);"}
            }
            ModifyArgument{
                index: 0
                ReplaceType{
                    modifiedType: "byte"
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "Q_UNUSED(__qt_return_value)"}
                }
            }
            ModifyArgument{
                index: 1
                RemoveArgument{
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "jbyte __java_return_value;\n"+
                                  "unsigned char& %out = *reinterpret_cast<unsigned char*>(&__java_return_value);"}
                }
            }
        }
        ModifyFunction{
            signature: "operator>>(bool &)const"
            rename: "extractBoolean"
            InjectCode{
                target: CodeClass.Native
                position: Position.Beginning
                Text{content: "qtjambi_dbus_check_read_argument(%env, __qt_this);"}
            }
            ModifyArgument{
                index: 0
                ReplaceType{
                    modifiedType: "boolean"
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "Q_UNUSED(__qt_return_value)"}
                }
            }
            ModifyArgument{
                index: 1
                RemoveArgument{
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "bool __java_return_value;\n"+
                                  "bool& %out = __java_return_value;"}
                }
            }
        }
        ModifyFunction{
            signature: "operator>>(short &)const"
            rename: "extractShort"
            InjectCode{
                target: CodeClass.Native
                position: Position.Beginning
                Text{content: "qtjambi_dbus_check_read_argument(%env, __qt_this);"}
            }
            ModifyArgument{
                index: 0
                ReplaceType{
                    modifiedType: "short"
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "Q_UNUSED(__qt_return_value)"}
                }
            }
            ModifyArgument{
                index: 1
                RemoveArgument{
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "jshort __java_return_value;\n"+
                                  "short& %out = *reinterpret_cast<short*>(&__java_return_value);"}
                }
            }
        }
        ModifyFunction{
            signature: "operator>>(int &)const"
            rename: "extractInt"
            InjectCode{
                target: CodeClass.Native
                position: Position.Beginning
                Text{content: "qtjambi_dbus_check_read_argument(%env, __qt_this);"}
            }
            ModifyArgument{
                index: 0
                ReplaceType{
                    modifiedType: "int"
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "Q_UNUSED(__qt_return_value)"}
                }
            }
            ModifyArgument{
                index: 1
                RemoveArgument{
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "int __java_return_value;\n"+
                                  "int& %out = __java_return_value;"}
                }
            }
        }
        ModifyFunction{
            signature: "operator>>(long long &)const"
            rename: "extractLong"
            InjectCode{
                target: CodeClass.Native
                position: Position.Beginning
                Text{content: "qtjambi_dbus_check_read_argument(%env, __qt_this);"}
            }
            ModifyArgument{
                index: 0
                ReplaceType{
                    modifiedType: "long"
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "Q_UNUSED(__qt_return_value)"}
                }
            }
            ModifyArgument{
                index: 1
                RemoveArgument{
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "jlong __java_return_value;\n"+
                                  "long long& %out = *reinterpret_cast<long long*>(&__java_return_value);"}
                }
            }
        }
        ModifyFunction{
            signature: "operator>>(double &)const"
            rename: "extractDouble"
            InjectCode{
                target: CodeClass.Native
                position: Position.Beginning
                Text{content: "qtjambi_dbus_check_read_argument(%env, __qt_this);"}
            }
            ModifyArgument{
                index: 0
                ReplaceType{
                    modifiedType: "double"
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "Q_UNUSED(__qt_return_value)"}
                }
            }
            ModifyArgument{
                index: 1
                RemoveArgument{
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "jdouble __java_return_value;\n"+
                                  "double& %out = __java_return_value;"}
                }
            }
        }
        ModifyFunction{
            signature: "operator>>(QString &)const"
            rename: "extractString"
            InjectCode{
                target: CodeClass.Native
                position: Position.Beginning
                Text{content: "qtjambi_dbus_check_read_argument(%env, __qt_this);"}
            }
            ModifyArgument{
                index: 0
                ReplaceType{
                    modifiedType: "java.lang.String"
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "Q_UNUSED(__qt_return_value)\n"+
                                  "%out = qtjambi_cast<jstring>(%env, __qt_%1);"}
                }
            }
            ModifyArgument{
                index: 1
                RemoveArgument{
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "QString %out;"}
                }
            }
        }
        ModifyFunction{
            signature: "operator>>(QDBusVariant &)const"
            rename: "extractVariant"
            InjectCode{
                target: CodeClass.Native
                position: Position.Beginning
                Text{content: "qtjambi_dbus_check_read_argument(%env, __qt_this);"}
            }
            ModifyArgument{
                index: 0
                ReplaceType{
                    modifiedType: "io.qt.dbus.QDBusVariant"
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "Q_UNUSED(__qt_return_value)\n"+
                                  "%out = qtjambi_cast<jobject>(%env, __qt_%1);"}
                }
            }
            ModifyArgument{
                index: 1
                RemoveArgument{
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "QDBusVariant %out;"}
                }
            }
        }
        ModifyFunction{
            signature: "operator>>(QDBusObjectPath &)const"
            rename: "extractObjectPath"
            InjectCode{
                target: CodeClass.Native
                position: Position.Beginning
                Text{content: "qtjambi_dbus_check_read_argument(%env, __qt_this);"}
            }
            ModifyArgument{
                index: 0
                ReplaceType{
                    modifiedType: "io.qt.dbus.QDBusObjectPath"
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "Q_UNUSED(__qt_return_value)\n"+
                                  "%out = qtjambi_cast<jobject>(%env, __qt_%1);"}
                }
            }
            ModifyArgument{
                index: 1
                RemoveArgument{
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "QDBusObjectPath %out;"}
                }
            }
        }
        ModifyFunction{
            signature: "operator>>(QDBusSignature &)const"
            rename: "extractSignature"
            InjectCode{
                target: CodeClass.Native
                position: Position.Beginning
                Text{content: "qtjambi_dbus_check_read_argument(%env, __qt_this);"}
            }
            ModifyArgument{
                index: 0
                ReplaceType{
                    modifiedType: "io.qt.dbus.QDBusObjectPath"
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "Q_UNUSED(__qt_return_value)\n"+
                                  "%out = qtjambi_cast<jobject>(%env, __qt_%1);"}
                }
            }
            ModifyArgument{
                index: 1
                RemoveArgument{
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "QDBusObjectPath %out;"}
                }
            }
        }
        ModifyFunction{
            signature: "operator>>(QDBusUnixFileDescriptor &)const"
            rename: "extractUnixFileDescriptor"
            InjectCode{
                target: CodeClass.Native
                position: Position.Beginning
                Text{content: "qtjambi_dbus_check_read_argument(%env, __qt_this);"}
            }
            ModifyArgument{
                index: 0
                ReplaceType{
                    modifiedType: "io.qt.dbus.QDBusUnixFileDescriptor"
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "Q_UNUSED(__qt_return_value)\n"+
                                  "%out = qtjambi_cast<jobject>(%env, __qt_%1);"}
                }
            }
            ModifyArgument{
                index: 1
                RemoveArgument{
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "QDBusUnixFileDescriptor %out;"}
                }
            }
        }
        ModifyFunction{
            signature: "operator>>(QStringList &)const"
            rename: "extractStrings"
            InjectCode{
                target: CodeClass.Native
                position: Position.Beginning
                Text{content: "qtjambi_dbus_check_read_argument(%env, __qt_this);"}
            }
            ModifyArgument{
                index: 0
                ReplaceType{
                    modifiedType: "java.util.List<java.lang.String>"
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "Q_UNUSED(__qt_return_value)\n"+
                                  "%out = qtjambi_cast<jobject>(%env, __qt_%1);"}
                }
            }
            ModifyArgument{
                index: 1
                RemoveArgument{
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "QStringList %out;"}
                }
            }
        }
        ModifyFunction{
            signature: "operator>>(QByteArray &)const"
            rename: "extractBytes"
            InjectCode{
                target: CodeClass.Native
                position: Position.Beginning
                Text{content: "qtjambi_dbus_check_read_argument(%env, __qt_this);"}
            }
            ModifyArgument{
                index: 0
                ReplaceType{
                    modifiedType: "io.qt.core.QByteArray"
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "Q_UNUSED(__qt_return_value)\n"+
                                  "%out = qtjambi_cast<jobject>(%env, __qt_%1);"}
                }
            }
            ModifyArgument{
                index: 1
                RemoveArgument{
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "QByteArray %out;"}
                }
            }
        }
    }
    
    ValueType{
        name: "QDBusConnection"
        ExtraIncludes{
            Include{
                fileName: "QDBusConnection"
                location: Include.Global
            }
        }
        CustomConstructor{
            Text{content: "if(copy){\n"+
                          "    return new(placement) QDBusConnection(*copy);\n"+
                          "}else{\n"+
                          "    return new(placement) QDBusConnection(\"\");\n"+
                          "}"}
        }
        CustomConstructor{
            type: CustomConstructor.Default
            Text{content: "new(placement) QDBusConnection(\"\");"}
        }
        ModifyFunction{
            signature: "interface() const"
            rename: "connectionInterface"
        }
        ModifyFunction{
            signature: "internalPointer() const"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "operator=(const QDBusConnection &)"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "asyncCall(const QDBusMessage &, int) const"
            ModifyArgument{
                index: "return"
                ReplaceType{
                    modifiedType: "io.qt.dbus.QDBusPendingCall"
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "%out = qtjambi_cast<jobject>(%env, %in);"}
                }
            }
        }
        ModifyFunction{
            signature: "callWithCallback(const QDBusMessage &, QObject *, const char *, const char *, int) const"
            ModifyArgument{
                index: 3
                ReplaceType{
                    modifiedType: "java.lang.String"
                }
            }
            ModifyArgument{
                index: 4
                ReplaceType{
                    modifiedType: "java.lang.String"
                }
            }
            InjectCode{
                target: CodeClass.Java
                position: Position.Beginning
                ArgumentMap{
                    index: 2
                    metaName: "object"
                }
                ArgumentMap{
                    index: 3
                    metaName: "member"
                }
                ArgumentMap{
                    index: 4
                    metaName: "errorSlot"
                }
                Text{content: "if(member!=null && !member.startsWith(\"1\") && !member.startsWith(\"2\")) {\n"+
                              "    io.qt.core.QMetaMethod method = object.metaObject().method(member);\n"+
                              "    if(method!=null && method.isValid()) {\n"+
                              "        if(method.methodType()==io.qt.core.QMetaMethod.MethodType.Signal)\n"+
                              "            member = \"2\" + method.cppMethodSignature();\n"+
                              "        else\n"+
                              "            member = \"1\" + method.cppMethodSignature();\n"+
                              "    }\n"+
                              "}\n"+
                              "if(errorSlot!=null && !errorSlot.startsWith(\"1\") && !errorSlot.startsWith(\"2\")) {\n"+
                              "    io.qt.core.QMetaMethod method = object.metaObject().method(errorSlot);\n"+
                              "    if(method!=null && method.isValid()) {\n"+
                              "        if(method.methodType()==io.qt.core.QMetaMethod.MethodType.Signal)\n"+
                              "            errorSlot = \"2\" + method.cppMethodSignature();\n"+
                              "        else\n"+
                              "            errorSlot = \"1\" + method.cppMethodSignature();\n"+
                              "    }\n"+
                              "}"}
            }
        }
        ModifyFunction{
            signature: "callWithCallback(const QDBusMessage &, QObject *, const char *, int) const"
            ModifyArgument{
                index: 3
                ReplaceType{
                    modifiedType: "java.lang.String"
                }
            }
            InjectCode{
                target: CodeClass.Java
                position: Position.Beginning
                ArgumentMap{
                    index: 2
                    metaName: "object"
                }
                ArgumentMap{
                    index: 3
                    metaName: "slot"
                }
                Text{content: "if(slot!=null && !slot.startsWith(\"1\") && !slot.startsWith(\"2\")) {\n"+
                              "    io.qt.core.QMetaMethod method = object.metaObject().method(slot);\n"+
                              "    if(method!=null && method.isValid()) {\n"+
                              "        if(method.methodType()==io.qt.core.QMetaMethod.MethodType.Signal)\n"+
                              "            slot = \"2\" + method.cppMethodSignature();\n"+
                              "        else\n"+
                              "            slot = \"1\" + method.cppMethodSignature();\n"+
                              "    }\n"+
                              "}"}
            }
        }
        ModifyFunction{
            signature: "connect(const QString &, const QString &, const QString &, const QString &, QObject *, const char *)"
            ModifyArgument{
                index: 6
                ReplaceType{
                    modifiedType: "java.lang.String"
                }
            }
            InjectCode{
                target: CodeClass.Java
                position: Position.Beginning
                ArgumentMap{
                    index: 5
                    metaName: "object"
                }
                ArgumentMap{
                    index: 6
                    metaName: "slot"
                }
                Text{content: "if(slot!=null && !slot.startsWith(\"1\") && !slot.startsWith(\"2\")) {\n"+
                              "    io.qt.core.QMetaMethod method = object.metaObject().method(slot);\n"+
                              "    if(method!=null && method.isValid()) {\n"+
                              "        if(method.methodType()==io.qt.core.QMetaMethod.MethodType.Signal)\n"+
                              "            slot = \"2\" + method.cppMethodSignature();\n"+
                              "        else\n"+
                              "            slot = \"1\" + method.cppMethodSignature();\n"+
                              "    }\n"+
                              "}"}
            }
        }
        ModifyFunction{
            signature: "connect(const QString &, const QString &, const QString &, const QString &, const QString &, QObject *, const char *)"
            ModifyArgument{
                index: 7
                ReplaceType{
                    modifiedType: "java.lang.String"
                }
            }
            InjectCode{
                target: CodeClass.Java
                position: Position.Beginning
                ArgumentMap{
                    index: 6
                    metaName: "object"
                }
                ArgumentMap{
                    index: 7
                    metaName: "slot"
                }
                Text{content: "if(slot!=null && !slot.startsWith(\"1\") && !slot.startsWith(\"2\")) {\n"+
                              "    io.qt.core.QMetaMethod method = object.metaObject().method(slot);\n"+
                              "    if(method!=null && method.isValid()) {\n"+
                              "        if(method.methodType()==io.qt.core.QMetaMethod.MethodType.Signal)\n"+
                              "            slot = \"2\" + method.cppMethodSignature();\n"+
                              "        else\n"+
                              "            slot = \"1\" + method.cppMethodSignature();\n"+
                              "    }\n"+
                              "}"}
            }
        }
        ModifyFunction{
            signature: "connect(const QString &, const QString &, const QString &, const QString &, const QStringList &, const QString &, QObject *, const char *)"
            ModifyArgument{
                index: 8
                ReplaceType{
                    modifiedType: "java.lang.String"
                }
            }
            InjectCode{
                target: CodeClass.Java
                position: Position.Beginning
                ArgumentMap{
                    index: 7
                    metaName: "object"
                }
                ArgumentMap{
                    index: 8
                    metaName: "slot"
                }
                Text{content: "if(slot!=null && !slot.startsWith(\"1\") && !slot.startsWith(\"2\")) {\n"+
                              "    io.qt.core.QMetaMethod method = object.metaObject().method(slot);\n"+
                              "    if(method!=null && method.isValid()) {\n"+
                              "        if(method.methodType()==io.qt.core.QMetaMethod.MethodType.Signal)\n"+
                              "            slot = \"2\" + method.cppMethodSignature();\n"+
                              "        else\n"+
                              "            slot = \"1\" + method.cppMethodSignature();\n"+
                              "    }\n"+
                              "}"}
            }
        }
        ModifyFunction{
            signature: "disconnect(const QString &, const QString &, const QString &, const QString &, QObject *, const char *)"
            ModifyArgument{
                index: 6
                ReplaceType{
                    modifiedType: "java.lang.String"
                }
            }
            InjectCode{
                target: CodeClass.Java
                position: Position.Beginning
                ArgumentMap{
                    index: 5
                    metaName: "object"
                }
                ArgumentMap{
                    index: 6
                    metaName: "slot"
                }
                Text{content: "if(slot!=null && !slot.startsWith(\"1\") && !slot.startsWith(\"2\")) {\n"+
                              "    io.qt.core.QMetaMethod method = object.metaObject().method(slot);\n"+
                              "    if(method!=null && method.isValid()) {\n"+
                              "        if(method.methodType()==io.qt.core.QMetaMethod.MethodType.Signal)\n"+
                              "            slot = \"2\" + method.cppMethodSignature();\n"+
                              "        else\n"+
                              "            slot = \"1\" + method.cppMethodSignature();\n"+
                              "    }\n"+
                              "}"}
            }
        }
        ModifyFunction{
            signature: "disconnect(const QString &, const QString &, const QString &, const QString &, const QString &, QObject *, const char *)"
            ModifyArgument{
                index: 7
                ReplaceType{
                    modifiedType: "java.lang.String"
                }
            }
            InjectCode{
                target: CodeClass.Java
                position: Position.Beginning
                ArgumentMap{
                    index: 6
                    metaName: "object"
                }
                ArgumentMap{
                    index: 7
                    metaName: "slot"
                }
                Text{content: "if(slot!=null && !slot.startsWith(\"1\") && !slot.startsWith(\"2\")) {\n"+
                              "    io.qt.core.QMetaMethod method = object.metaObject().method(slot);\n"+
                              "    if(method!=null && method.isValid()) {\n"+
                              "        if(method.methodType()==io.qt.core.QMetaMethod.MethodType.Signal)\n"+
                              "            slot = \"2\" + method.cppMethodSignature();\n"+
                              "        else\n"+
                              "            slot = \"1\" + method.cppMethodSignature();\n"+
                              "    }\n"+
                              "}"}
            }
        }
        ModifyFunction{
            signature: "disconnect(const QString &, const QString &, const QString &, const QString &, const QStringList &, const QString &, QObject *, const char *)"
            ModifyArgument{
                index: 8
                ReplaceType{
                    modifiedType: "java.lang.String"
                }
            }
            InjectCode{
                target: CodeClass.Java
                position: Position.Beginning
                ArgumentMap{
                    index: 7
                    metaName: "object"
                }
                ArgumentMap{
                    index: 8
                    metaName: "slot"
                }
                Text{content: "if(slot!=null && !slot.startsWith(\"1\") && !slot.startsWith(\"2\")) {\n"+
                              "    io.qt.core.QMetaMethod method = object.metaObject().method(slot);\n"+
                              "    if(method!=null && method.isValid()) {\n"+
                              "        if(method.methodType()==io.qt.core.QMetaMethod.MethodType.Signal)\n"+
                              "            slot = \"2\" + method.cppMethodSignature();\n"+
                              "        else\n"+
                              "            slot = \"1\" + method.cppMethodSignature();\n"+
                              "    }\n"+
                              "}"}
            }
        }
    }
    
    ObjectType{
        name: "QDBusConnectionInterface"
    }
    
    InterfaceType{
        name: "QDBusContext"
        InjectCode{
            target: CodeClass.Java
            position: Position.Beginning
            Text{content: "private static void checkCalledFromDBus(QDBusContext instance){\n"+
                          "    if(!instance.calledFromDBus())\n"+
                          "        throw new IllegalStateException(\"QDBusContext has not ben called from DBus.\");\n"+
                          "}"}
        }
        ModifyFunction{
            signature: "connection() const"
            InjectCode{
                target: CodeClass.Java
                position: Position.Beginning
                Text{content: "QDBusContext.Impl.checkCalledFromDBus(%this);"}
            }
        }
        ModifyFunction{
            signature: "message() const"
            InjectCode{
                target: CodeClass.Java
                position: Position.Beginning
                Text{content: "QDBusContext.Impl.checkCalledFromDBus(%this);"}
            }
        }
        ModifyFunction{
            signature: "isDelayedReply() const"
            InjectCode{
                target: CodeClass.Java
                position: Position.Beginning
                Text{content: "QDBusContext.Impl.checkCalledFromDBus(%this);"}
            }
        }
        ModifyFunction{
            signature: "setDelayedReply(bool)const"
            InjectCode{
                target: CodeClass.Java
                position: Position.Beginning
                Text{content: "QDBusContext.Impl.checkCalledFromDBus(%this);"}
            }
        }
        ModifyFunction{
            signature: "sendErrorReply(const QString &, const QString &) const"
            InjectCode{
                target: CodeClass.Java
                position: Position.Beginning
                Text{content: "QDBusContext.Impl.checkCalledFromDBus(%this);"}
            }
        }
        ModifyFunction{
            signature: "sendErrorReply(QDBusError::ErrorType, const QString &) const"
            InjectCode{
                target: CodeClass.Java
                position: Position.Beginning
                Text{content: "QDBusContext.Impl.checkCalledFromDBus(%this);"}
            }
        }
    }
    
    
    ValueType{
        name: "QDBusError"
        ModifyFunction{
            signature: "operator=(const QDBusError &)"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "operator=(const QDBusMessage &)"
            remove: RemoveFlag.All
        }
    }
    
    ObjectType{
        name: "QDBusInterface"
    }
    
    ValueType{
        name: "QDBusMessage"
        ModifyFunction{
            signature: "interface() const"
            rename: "interfaceName"
        }
        ModifyFunction{
            signature: "operator=(const QDBusMessage &)"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "operator<<(const QVariant &)"
            remove: RemoveFlag.All
        }
    }
    
    ValueType{
        name: "QDBusObjectPath"
        ModifyFunction{
            signature: "QDBusObjectPath(QLatin1StringView)"
            remove: RemoveFlag.All
            since: [6, 4]
        }
        ModifyFunction{
            signature: "QDBusObjectPath(QLatin1String)"
            remove: RemoveFlag.All
            until: [6, 3]
        }
        ModifyFunction{
            signature: "QDBusObjectPath(const char *)"
            remove: RemoveFlag.All
        }
    }
    
    InterfaceType{
        name: "QDBusPendingCall"
        isValue: true
        CustomConstructor{
            Text{content: "if(copy)\n"+
                          "    return new(placement) QDBusPendingCall(*copy);\n"+
                          "else\n"+
                          "    return new(placement) QDBusPendingCall(QDBusPendingCall::fromCompletedCall(QDBusMessage()));"}
        }
        CustomConstructor{
            type: CustomConstructor.Default
            Text{content: "new(placement) QDBusPendingCall(QDBusPendingCall::fromCompletedCall(QDBusMessage()));"}
        }
        ModifyFunction{
            signature: "operator=(QDBusPendingCall &&)"
            remove: RemoveFlag.All
            since: [5, 14]
        }
        ModifyFunction{
            signature: "operator=(const QDBusPendingCall &)"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "QDBusPendingCall(const QDBusPendingCall &)"
            ModifyArgument{
                index: 1
                NoNullPointer{
                }
            }
        }
        ModifyFunction{
            signature: "swap(QDBusPendingCall &)"
            ModifyArgument{
                index: 1
                ReplaceType{
                    modifiedType: "io.qt.dbus.QDBusPendingCall"
                }
                NoNullPointer{
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "QDBusPendingCall& %out = QtJambiAPI::convertJavaInterfaceToNativeReference<QDBusPendingCall>(%env, %in, \"io/qt/dbus/QDBusPendingCall\");"}
                }
            }
        }
    }
    
    ObjectType{
        name: "QDBusPendingCallWatcher"
        InterfacePolymorphy{
            interfaceType: "QDBusPendingCall"
            condition: "false"
        }
        ModifyFunction{
            signature: "QDBusPendingCallWatcher(const QDBusPendingCall &, QObject*)"
            ModifyArgument{
                index: 1
                NoNullPointer{
                }
            }
        }
        ModifyFunction{
            signature: "fromCompletedCall(const QDBusMessage &)"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "fromError(const QDBusError &)"
            remove: RemoveFlag.All
        }
    }
    
    ObjectType{
        name: "QDBusServiceWatcher"
        ModifyFunction{
            signature: "connection() const"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "QDBusServiceWatcher(const QString &,const QDBusConnection &,QFlags<QDBusServiceWatcher::WatchModeFlag>,QObject *)"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "setConnection(const QDBusConnection &)"
            remove: RemoveFlag.All
        }
    }
    
    ValueType{
        name: "QDBusSignature"
        ModifyFunction{
            signature: "QDBusSignature(QLatin1StringView)"
            remove: RemoveFlag.All
            since: [6, 4]
        }
        ModifyFunction{
            signature: "QDBusSignature(QLatin1String)"
            remove: RemoveFlag.All
            until: [6, 3]
        }
        ModifyFunction{
            signature: "QDBusSignature(const char*)"
            remove: RemoveFlag.All
        }
    }
    
    ValueType{
        name: "QDBusUnixFileDescriptor"
        ModifyFunction{
            signature: "operator=(const QDBusUnixFileDescriptor &)"
            remove: RemoveFlag.All
        }
    }
    
    Rejection{
        className: "QDBusMetaType"
    }
    
    ObjectType{
        name: "QDBusPendingReplyData"
        javaName: "QDBusPendingReplyBase"
        forceFriendly: true
        InterfacePolymorphy{
            interfaceType: "QDBusPendingCall"
            condition: "false"
        }
        ModifyFunction{
            signature: "setMetaTypes(int, const int *)"
            access: Modification.Private
            ModifyArgument{
                index: 1
                RemoveArgument{
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "int __qt_%in(0);\n"+
                                  "int& %in = __qt_%in;"}
                }
            }
            ModifyArgument{
                index: 2
                NoNullPointer{
                }
                ArrayType{
                    lengthParameter: 1
                }
            }
        }
        InjectCode{
            Text{content: "@io.qt.QtUninvokable\n"+
                          "final void setMetaTypes(io.qt.core.QMetaType[] metaTypes){\n"+
                          "    int[] metaTypeIDs = new int[metaTypes.length];\n"+
                          "    for(int i=0; i<metaTypes.length; ++i){\n"+
                          "        metaTypeIDs[i] = metaTypes[i].id();\n"+
                          "    }\n"+
                          "    setMetaTypes(metaTypeIDs);\n"+
                          "}"}
        }
        until: 5
    }
    
    ObjectType{
        name: "QDBusPendingReplyBase"
        forceFriendly: true
        InterfacePolymorphy{
            interfaceType: "QDBusPendingCall"
            condition: "false"
        }
        ModifyFunction{
            signature: "setMetaTypes(int, const QMetaType *)"
            access: Modification.Friendly
            ModifyArgument{
                index: 1
                RemoveArgument{
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "int __qt_%in(0);\n"+
                                  "int& %in = __qt_%in;"}
                }
            }
            ModifyArgument{
                index: 2
                NoNullPointer{
                }
                ArrayType{
                    lengthParameter: 1
                }
            }
        }
        since: 6
    }
    
    ObjectType{
        name: "QDBusVariant"
    }
    
    
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: Object type 'QDBusPendingCall' passed as value. Resulting code will not compile.*"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: skipping function 'QDBusError::QDBusError', unmatched parameter type '*DBusError*'"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: Final class 'QDBusAbstractInterfaceBase' set to non-final, as it is extended by other classes"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: signature 'qt_metacall(QMetaObject::Call,int,void**)' for function modification in 'QDBusInterface' not found. Possible candidates:*"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: skipping function 'QtJambiDBusPendingReply::QtJambiDBusPendingReply', unmatched parameter type '*'"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: unknown operator 'Type'"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: horribly broken type ''"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: skipping function 'QDBusReply::operator Type', unmatched return type ''"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: Operator type unknown: QDBusReply::operator Type()const"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: skipping function 'QDBusReply::*', unmatched *type '*'"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: template baseclass '*' of 'QDBusPendingReplyBase' is not known"}
    SuppressedWarning{text: "WARNING(JavaGenerator) :: No ==/!= operator found for value type QDBus*."}
}
