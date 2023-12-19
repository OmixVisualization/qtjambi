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
    defaultSuperClass: "QtObject"
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

        EnumType{
            name: "CallMode"
        }
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
            signature: "call<Args...>(QString,Args&&)"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "call<Args...>(QDBus::CallMode,QString,Args&&)"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "asyncCall<Args...>(QString,Args&&)"
            remove: RemoveFlag.All
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
            signature: "callWithCallback(const QString &,const QList<QVariant> &,QObject *, const char *)"
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
            Text{content: "@QtUninvokable\n"+
                          "public final io.qt.dbus.QDBusPendingCall asyncCall(java.lang.String method, java.lang.Object... args){\n"+
                          "    return asyncCallWithArgumentList(method, java.util.Arrays.asList(args));\n"+
                          "}\n"+
                          "\n"+
                          "@QtUninvokable\n"+
                          "public final io.qt.dbus.QDBusMessage call(io.qt.dbus.QDBus.CallMode mode, java.lang.String method, java.lang.Object... args){\n"+
                          "    return callWithArgumentList(mode, method, java.util.Arrays.asList(args));\n"+
                          "}\n"+
                          "\n"+
                          "@QtUninvokable\n"+
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
        EnumType{
            name: "ElementType"
        }
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
            signature: "operator<<(qulonglong)"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "operator<<(ushort)"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "operator>>(ushort &)const"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "operator>>(uint &)const"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "operator>>(qulonglong &)const"
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
        }
        ModifyFunction{
            signature: "operator<<(qlonglong)"
            rename: "append"
            InjectCode{
                target: CodeClass.Native
                position: Position.Beginning
                Text{content: "qtjambi_dbus_check_write_argument(%env, __qt_this);"}
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
        }
        ModifyFunction{
            signature: "operator<<(uchar)"
            rename: "append"
            InjectCode{
                target: CodeClass.Native
                position: Position.Beginning
                Text{content: "qtjambi_dbus_check_write_argument(%env, __qt_this);"}
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
        }
        ModifyFunction{
            signature: "operator<<(double)"
            rename: "append"
            InjectCode{
                target: CodeClass.Native
                position: Position.Beginning
                Text{content: "qtjambi_dbus_check_write_argument(%env, __qt_this);"}
            }
        }
        ModifyFunction{
            signature: "operator<<(const QString &)"
            rename: "append"
            ModifyArgument{
                index: 1
                ReplaceType{
                    modifiedType: "java.lang.@Nullable CharSequence"
                }
            }
            InjectCode{
                target: CodeClass.Native
                position: Position.Beginning
                Text{content: "qtjambi_dbus_check_write_argument(%env, __qt_this);"}
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
        }
        ModifyFunction{
            signature: "operator<<(const QStringList &)"
            rename: "append"
            ModifyArgument{
                index: 1
                replaceType: "java.util.@NonNull List<java.lang.@NonNull String>"
            }
            InjectCode{
                target: CodeClass.Native
                position: Position.Beginning
                Text{content: "qtjambi_dbus_check_write_argument(%env, __qt_this);"}
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
        }
        ModifyFunction{
            signature: "operator<<(const QDBusObjectPath &)"
            rename: "append"
            InjectCode{
                target: CodeClass.Native
                position: Position.Beginning
                Text{content: "qtjambi_dbus_check_write_argument(%env, __qt_this);"}
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
        }
        ModifyFunction{
            signature: "operator<<(const QDBusUnixFileDescriptor &)"
            rename: "append"
            InjectCode{
                target: CodeClass.Native
                position: Position.Beginning
                Text{content: "qtjambi_dbus_check_write_argument(%env, __qt_this);"}
            }
        }
        ModifyFunction{
            signature: "operator>>(uchar &)const"
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
                                  "uchar& %out = *reinterpret_cast<uchar*>(&__java_return_value);"}
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
            signature: "operator>>(qlonglong &)const"
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
            signature: "operator>>(uint &)const"
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
            signature: "operator>>(QString &)const"
            rename: "extractQString"
            InjectCode{
                target: CodeClass.Native
                position: Position.Beginning
                Text{content: "qtjambi_dbus_check_read_argument(%env, __qt_this);"}
            }
            ModifyArgument{
                index: 0
                ReplaceType{
                    modifiedType: "io.qt.core.QString"
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "Q_UNUSED(__qt_return_value)\n"+
                                  "%out = QtJambiAPI::convertQStringToJavaObject(%env, __qt_%1);"}
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
                    modifiedType: "io.qt.dbus.@NonNull QDBusVariant"
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
                    modifiedType: "io.qt.dbus.@NonNull QDBusObjectPath"
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
                    modifiedType: "io.qt.dbus.@NonNull QDBusObjectPath"
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
                    modifiedType: "io.qt.dbus.@NonNull QDBusUnixFileDescriptor"
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
                    modifiedType: "io.qt.core.@NonNull QStringList"
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
                    modifiedType: "io.qt.core.@NonNull QByteArray"
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
        ModifyFunction{
            signature: "operator<<(QDBusArgument&,QMap<QString,QVariant>)"
            ModifyArgument{
                index: 2
                replaceType: "java.util.@NonNull NavigableMap<java.lang.@NonNull String, ? extends java.lang.Object>"
            }
            InjectCode{
                target: CodeClass.Native
                position: Position.Beginning
                Text{content: "qtjambi_dbus_check_write_argument(%env, __qt_this);"}
            }
            since: 6
        }
        ModifyFunction{
            signature: "operator>>(QDBusArgument,QDate &)"
            rename: "extractDate"
            InjectCode{
                target: CodeClass.Native
                position: Position.Beginning
                Text{content: "qtjambi_dbus_check_read_argument(%env, __qt_this);"}
            }
            ModifyArgument{
                index: 0
                ReplaceType{
                    modifiedType: "io.qt.core.@NonNull QDate"
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
                    Text{content: "QDate %out;"}
                }
            }
        }
        ModifyFunction{
            signature: "operator>>(QDBusArgument,QDateTime &)"
            rename: "extractDateTime"
            InjectCode{
                target: CodeClass.Native
                position: Position.Beginning
                Text{content: "qtjambi_dbus_check_read_argument(%env, __qt_this);"}
            }
            ModifyArgument{
                index: 0
                ReplaceType{
                    modifiedType: "io.qt.core.@NonNull QDateTime"
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
                    Text{content: "QDateTime %out;"}
                }
            }
        }
        ModifyFunction{
            signature: "operator>>(QDBusArgument,QLine &)"
            rename: "extractLine"
            InjectCode{
                target: CodeClass.Native
                position: Position.Beginning
                Text{content: "qtjambi_dbus_check_read_argument(%env, __qt_this);"}
            }
            ModifyArgument{
                index: 0
                ReplaceType{
                    modifiedType: "io.qt.core.@NonNull QLine"
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
                    Text{content: "QLine %out;"}
                }
            }
        }
        ModifyFunction{
            signature: "operator>>(QDBusArgument,QLineF &)"
            rename: "extractLineF"
            InjectCode{
                target: CodeClass.Native
                position: Position.Beginning
                Text{content: "qtjambi_dbus_check_read_argument(%env, __qt_this);"}
            }
            ModifyArgument{
                index: 0
                ReplaceType{
                    modifiedType: "io.qt.core.@NonNull QLineF"
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
                    Text{content: "QLineF %out;"}
                }
            }
        }
        ModifyFunction{
            signature: "operator>>(QDBusArgument,QPoint &)"
            rename: "extractPoint"
            InjectCode{
                target: CodeClass.Native
                position: Position.Beginning
                Text{content: "qtjambi_dbus_check_read_argument(%env, __qt_this);"}
            }
            ModifyArgument{
                index: 0
                ReplaceType{
                    modifiedType: "io.qt.core.@NonNull QPoint"
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
                    Text{content: "QPoint %out;"}
                }
            }
        }
        ModifyFunction{
            signature: "operator>>(QDBusArgument,QPointF &)"
            rename: "extractPointF"
            InjectCode{
                target: CodeClass.Native
                position: Position.Beginning
                Text{content: "qtjambi_dbus_check_read_argument(%env, __qt_this);"}
            }
            ModifyArgument{
                index: 0
                ReplaceType{
                    modifiedType: "io.qt.core.@NonNull QPointF"
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
                    Text{content: "QPointF %out;"}
                }
            }
        }
        ModifyFunction{
            signature: "operator>>(QDBusArgument,QRect &)"
            rename: "extractRect"
            InjectCode{
                target: CodeClass.Native
                position: Position.Beginning
                Text{content: "qtjambi_dbus_check_read_argument(%env, __qt_this);"}
            }
            ModifyArgument{
                index: 0
                ReplaceType{
                    modifiedType: "io.qt.core.@NonNull QRect"
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
                    Text{content: "QRect %out;"}
                }
            }
        }
        ModifyFunction{
            signature: "operator>>(QDBusArgument,QRectF &)"
            rename: "extractRectF"
            InjectCode{
                target: CodeClass.Native
                position: Position.Beginning
                Text{content: "qtjambi_dbus_check_read_argument(%env, __qt_this);"}
            }
            ModifyArgument{
                index: 0
                ReplaceType{
                    modifiedType: "io.qt.core.@NonNull QRectF"
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
                    Text{content: "QRectF %out;"}
                }
            }
        }
        ModifyFunction{
            signature: "operator>>(QDBusArgument,QSize &)"
            rename: "extractSize"
            InjectCode{
                target: CodeClass.Native
                position: Position.Beginning
                Text{content: "qtjambi_dbus_check_read_argument(%env, __qt_this);"}
            }
            ModifyArgument{
                index: 0
                ReplaceType{
                    modifiedType: "io.qt.core.@NonNull QSize"
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
                    Text{content: "QSize %out;"}
                }
            }
        }
        ModifyFunction{
            signature: "operator>>(QDBusArgument,QSizeF &)"
            rename: "extractSizeF"
            InjectCode{
                target: CodeClass.Native
                position: Position.Beginning
                Text{content: "qtjambi_dbus_check_read_argument(%env, __qt_this);"}
            }
            ModifyArgument{
                index: 0
                ReplaceType{
                    modifiedType: "io.qt.core.@NonNull QSizeF"
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
                    Text{content: "QSizeF %out;"}
                }
            }
        }
        ModifyFunction{
            signature: "operator>>(QDBusArgument,QTime &)"
            rename: "extractTime"
            InjectCode{
                target: CodeClass.Native
                position: Position.Beginning
                Text{content: "qtjambi_dbus_check_read_argument(%env, __qt_this);"}
            }
            ModifyArgument{
                index: 0
                ReplaceType{
                    modifiedType: "io.qt.core.@NonNull QTime"
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
                    Text{content: "QTime %out;"}
                }
            }
        }
        ModifyFunction{
            signature: "operator>>(QDBusArgument,QVariant &)"
            rename: "extractObject"
            InjectCode{
                target: CodeClass.Native
                position: Position.Beginning
                Text{content: "qtjambi_dbus_check_read_argument(%env, __qt_this);"}
            }
            ModifyArgument{
                index: 0
                ReplaceType{
                    modifiedType: "io.qt.core.@NonNull QVariant"
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
                    Text{content: "QVariant %out;"}
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

        Rejection{functionName: "registerVirtualObject"}

        EnumType{
            name: "VirtualObjectRegisterOption"
        }

        EnumType{
            name: "RegisterOption"
            RejectEnumValue{
                name: "ExportAllSignal"
            }
        }

        EnumType{
            name: "UnregisterMode"
        }

        EnumType{
            name: "BusType"
        }


        EnumType{
            name: "ConnectionCapability"
        }


        EnumType{
            name: "RegisterServiceReply"
        }


        EnumType{
            name: "ServiceQueueOptions"
        }

        EnumType{
            name: "ServiceReplacementOption"
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
            signature: "callWithCallback(const QDBusMessage &, QObject *, const char *, const char *, int) const"
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

        EnumType{
            name: "ServiceQueueOptions"
        }

        EnumType{
            name: "RegisterServiceReply"
        }

        EnumType{
            name: "ServiceReplacementOptions"
        }
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
        noImplicitConstructors: true
        EnumType{
            name: "ErrorType"
            RejectEnumValue{
                name: "LastErrorType"
            }
        }
    }
    
    ObjectType{
        name: "QDBusInterface"
    }
    
    ValueType{
        name: "QDBusMessage"
        EnumType{
            name: "MessageType"
        }
        ModifyFunction{
            signature: "interface() const"
            rename: "interfaceName"
        }
        ModifyFunction{
            signature: "operator<<(const QVariant &)"
            rename: "append"
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
            signature: "QDBusPendingCall(const QDBusPendingCall &)"
            ModifyArgument{
                index: 1
                NoNullPointer{
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
        EnumType{
            name: "WatchModeFlag"
        }
        ModifyFunction{
            signature: "connection() const"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "QDBusServiceWatcher(const QString &,const QDBusConnection &,QDBusServiceWatcher::WatchMode,QObject *)"
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
                index: 2
                NoNullPointer{
                }
                AsArray{
                    lengthParameter: 1
                    noOffset: true
                }
            }
        }
        InjectCode{
            Text{content: "@QtUninvokable\n"+
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
                index: 2
                NoNullPointer{
                }
                AsArray{
                    lengthParameter: 1
                    noOffset: true
                }
            }
        }
        since: 6
    }
    
    ObjectType{
        name: "QDBusVariant"
        ModifyFunction{
            signature: "QDBusVariant(QVariant&&)"
            remove: RemoveFlag.All
            since: 6.6
        }
    }

    GlobalFunction{
        signature: "operator<<<T1,T2>(QDBusArgument&,std::pair<T1,T2>)"
        remove: RemoveFlag.All
    }

    GlobalFunction{
        signature: "operator>><T1,T2>(QDBusArgument,std::pair<T1,T2>&)"
        remove: RemoveFlag.All
    }

    GlobalFunction{
        signature: "operator<<<T1,T2>(QDBusArgument&,QPair<T1,T2>)"
        remove: RemoveFlag.All
        until: 5
    }

    GlobalFunction{
        signature: "operator>><T1,T2>(QDBusArgument,QPair<T1,T2>&)"
        remove: RemoveFlag.All
        until: 5
    }

    GlobalFunction{
        signature: "operator>><Key,T>(QDBusArgument,QHash<Key,T>&)"
        remove: RemoveFlag.All
        until: 5
    }

    GlobalFunction{
        signature: "operator>><T>(QDBusArgument,QList<T>&)"
        remove: RemoveFlag.All
        until: 5
    }

    GlobalFunction{
        signature: "operator>><Key,T>(QDBusArgument,QMap<Key,T>&)"
        remove: RemoveFlag.All
        until: 5
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
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: Class 'QDBusVariant' has equals operators but no qHash() function. Hashcode of objects will consistently be 0."}
}
