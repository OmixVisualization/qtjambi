/****************************************************************************
**
** Copyright (C) 2009-2026 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
**
** This file is part of Qt Jambi.
**
** ** $BEGIN_LICENSE$
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
**
** $END_LICENSE$
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

import QtJambiGenerator 1.0

TypeSystem{
    packageName: "io.qt.autotests.generated"
    targetName: "autotests"
    defaultSuperClass: "io.qt.QtObject"

    LoadTypeSystem{name: "QtCore"}
    LoadTypeSystem{name: "QtGui"}
    LoadTypeSystem{name: "QtWidgets"}
    LoadTypeSystem{name: "QtXml"}
    LoadTypeSystem{name: "QtNetwork"}
    LoadTypeSystem{name: "QtSql"; optional: true}
    RequiredLibrary{name: "QtCore"}
    RequiredLibrary{name: "QtGui"}
    RequiredLibrary{name: "QtWidgets"}
    RequiredLibrary{name: "QtXml"}
    RequiredLibrary{name: "QtNetwork"}
    RequiredLibrary{name: "QtQuick"}
    RequiredLibrary{name: "QtConcurrent"}
    RequiredLibrary{name: "QtSql"}
    
    Rejection{
        className: "SignalsAndSlots"
        functionName: "connectNotify"
    }
    
    Rejection{
        className: "SignalsAndSlots"
        functionName: "disconnectNotify"
    }
    
    ValueType{
        name: "Tulip"
        Include{
            fileName: "tulip.h"
            location: Include.Local
        }
    
		FunctionalType{
			name: "TestStdFunction"
		}
		
		
        InjectCode{
            target: CodeClass.Native
            position: Position.Beginning
            Text{content: String.raw`
namespace QHashPrivate {
template <>
constexpr inline bool HasQHashSingleArgOverload<QMap<QString,QRunnable *>> = false;
template <>
constexpr inline bool HasQHashSingleArgOverload<QMap<QString,QObject *>> = false;
template <>
constexpr inline bool HasQHashSingleArgOverload<QMap<QString,QPoint>> = false;
}`
            }
            since: 6.8
        }
    }
    
    ValueType{
        name: "Variants"
        Include{
            fileName: "variants.h"
            location: Include.Local
        }
        ModifyFunction{
            signature: "createInternalObject(QObject*)"
            ModifyArgument{
                index: 0
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Java
                }
            }
        }
    }
    
    EnumType{
        name: "TheGlobalEnum"
    }
    
    EnumType{
        name: "TheGlobalShortEnum"
    }
    
    EnumType{
        name: "TheGlobalFlag"
    }
    
    EnumType{
        name: "TheGlobalEnumClass"
        RejectEnumValue{
            name: "Repeat"
        }
    }
    
    EnumType{
        name: "TheGlobalCharEnumStruct"
    }
    
    EnumType{
        name: "TheGlobalLongEnumStruct"
    }
    
    EnumType{
        name: "TheGlobalShortEnumClass"
    }
    
    EnumType{
        name: "TheExtensibleGlobalLongEnumClass"
        extensible: true
    }
    
    EnumType{
        name: "TheGlobalIntegerEnum"
        forceInteger: true
    }
    
    EnumType{
        name: "TheGlobalLongIntegerEnumClass"
        forceInteger: true
    }
    
    NamespaceType{
        name: "EnumNamespace"
		EnumType{
			name: "NSFlag"
		}
    
		EnumType{
			name: "NSEnumClass"
			RejectEnumValue{
				name: "Repeat1"
			}
			RejectEnumValue{
				name: "Repeat2"
			}
			RejectEnumValue{
				name: "Repeat3"
			}
		}
		
		EnumType{
			name: "NSClassFlag"
		}
    }
    
    ObjectType{
        name: "GlobalEnumTest"
    
		EnumType{
			name: "ScopedEnum"
		}
		
		EnumType{
			name: "ScopedEnumClass"
		}
        ModifyFunction{
            signature: "testGlobalFlags(TheGlobalFlags)"
            ModifyArgument{
                index: 1
                ReplaceDefaultExpression{
                    expression: "io.qt.autotests.generated.TheGlobalFlag.Flag2, io.qt.autotests.generated.TheGlobalFlag.Flag3"
                }
            }
        }
    }
    
    ObjectType{
        name: "General"
        InjectCode{
            position: Position.End
            Text{content: "public final static io.qt.InternalAccess internalAccess = QtJambi_LibraryUtilities.internal;"}
        }
        ModifyFunction{
            signature: "run(QRunnable*)"
            rethrowExceptions: true
        }
        ModifyFunction{
            signature: "run(QThread*)"
            rethrowExceptions: true
        }
    }
    
    ObjectType{
        name: "MoccedSub1Object"
    }
    
    ObjectType{
        name: "UnMoccedSub1Object"
    }
    
    ObjectType{
        name: "MoccedSub2Object"
    }
    
    ObjectType{
        name: "UnMoccedSub2Object"
    }
    
    ObjectType{
        name: "MoccedObject"
    }
    
    ObjectType{
        name: "UnMoccedObject"
        ModifyFunction{
            signature: "create(int)"
            ModifyArgument{
                index: 0
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Java
                }
            }
        }
    }
    
    ObjectType{
        name: "SignalsAndSlots"
        Include{
            fileName: "signalsandslots.h"
            location: Include.Local
        }
        ModifyFunction{
            signature: "connectToDestroyedSignal(QObject *, QRunnable *)"
            ModifyArgument{
                index: 2
                DefineOwnership{
                    codeClass: CodeClass.Java
                    ownership: Ownership.Cpp
                }
            }
        }
    }

    ObjectType{
        name: "SignalReceiver"
    }
    
    ObjectType{
        name: "BindableOwner"
    }
    
    ObjectType{
        name: "SenderQObject"
    }
    
    ObjectType{
        name: "StyledItemDelegateSubclass"
        ModifyFunction{
            signature: "initStyleOptionInStyledDelegate(QStyleOptionViewItem *)"
            ModifyArgument{
                index: 1
                invalidateAfterUse: true
                ReplaceType{
                    modifiedType: "io.qt.widgets.QStyleOptionViewItem"
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "QStyleOptionViewItem* %out = qtjambi_cast<QStyleOptionViewItem*>(%env, %in);"}
                }
                ConversionRule{
                    codeClass: CodeClass.Shell
                    Text{content: "jobject %out = qtjambi_cast<jobject>(%env, %in);"}
                }
            }
        }
    }
    
    ObjectType{
        name: "GraphicsWidgetSubclass"
        ExtraIncludes{
            Include{
                fileName: "QtJambiWidgets/hashes.h"
                location: Include.Global
            }
        }
    }
    
    ObjectType{
        name: "AbstractSubclass"
    }
    
    ObjectType{
        name: "AbstractClass"
        Include{
            fileName: "abstractclass.h"
            location: Include.Local
        }
        ModifyFunction{
            signature: "setReferenceCountTest(QObject*)"
            ModifyArgument{
                index: 1
                invalidateAfterUse: true
                ReferenceCount{
                    variableName: "__rcReferenceCountTest"
                    action: ReferenceCount.Set
                }
            }
        }
    }
    
    ObjectType{
        name: "CustomPaintEngine"
        ModifyFunction{
            signature: "callDrawLineFs(QLineF,int)"
            noImplicitArguments: true
        }
        ModifyFunction{
            signature: "callDrawLines(QLine,int)"
            noImplicitArguments: true
        }
        ModifyFunction{
            signature: "callDrawPointFs(QPointF,int)"
            noImplicitArguments: true
        }
        ModifyFunction{
            signature: "callDrawPoints(QPoint,int)"
            noImplicitArguments: true
        }
        ModifyFunction{
            signature: "callDrawPolygonFs(QPointF,int)"
            noImplicitArguments: true
        }
        ModifyFunction{
            signature: "callDrawPolygons(QPoint,int)"
            noImplicitArguments: true
        }
        ModifyFunction{
            signature: "callDrawRectFs(QRectF,int)"
            noImplicitArguments: true
        }
        ModifyFunction{
            signature: "callDrawRects(QRect,int)"
            noImplicitArguments: true
        }
    }
    
    ObjectType{
        name: "OtherCustomPaintEngine"
        ExtraIncludes{
            Include{
                fileName: "QVarLengthArray"
                location: Include.Global
            }
        }
    }
    
    ObjectType{
        name: "AnotherNonAbstractSubclass"
        Include{
            fileName: "abstractclass.h"
            location: Include.Local
        }
        ModifyFunction{
            signature: "setReferenceCountTest(QObject*)"
            ModifyArgument{
                index: 1
                invalidateAfterUse: true
                ReferenceCount{
                    variableName: "__rcReferenceCountTest"
                    declareVariable: "AbstractClass"
                    action: ReferenceCount.Set
                }
            }
        }
    }
    
    ObjectType{
        name: "NativePointerTester"
        Include{
            fileName: "nativepointertester.h"
            location: Include.Local
        }
    }
    
    ObjectType{
        name: "OrdinarySuperclass"
    }
    
    ObjectType{
        name: "OrdinaryDestroyed"
        ModifyFunction{
            signature: "getObjectJavaOwnership(DestroyCounter*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
            ModifyArgument{
                index: "return"
                DefineOwnership{
                    codeClass: CodeClass.Java
                    ownership: Ownership.Java
                }
            }
        }
        ModifyFunction{
            signature: "getObjectCppOwnership(DestroyCounter*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
            ModifyArgument{
                index: "return"
                DefineOwnership{
                    codeClass: CodeClass.Java
                    ownership: Ownership.Cpp
                }
            }
        }
        ModifyFunction{
            signature: "setDefaultOwnership(OrdinaryDestroyed *)"
            ModifyArgument{
                index: 1
                DefineOwnership{
                    codeClass: CodeClass.Java
                    ownership: Ownership.Default
                }
            }
        }
        ModifyFunction{
            signature: "virtualGetObjectJavaOwnership(DestroyCounter*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
            ModifyArgument{
                index: "return"
                DefineOwnership{
                    codeClass: CodeClass.Shell
                    ownership: Ownership.Java
                }
            }
        }
        ModifyFunction{
            signature: "virtualGetObjectCppOwnership(DestroyCounter*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
            ModifyArgument{
                index: "return"
                DefineOwnership{
                    codeClass: CodeClass.Shell
                    ownership: Ownership.Cpp
                }
            }
        }
        ModifyFunction{
            signature: "virtualSetDefaultOwnership(OrdinaryDestroyed *)"
            ModifyArgument{
                index: 1
                DefineOwnership{
                    codeClass: CodeClass.Shell
                    ownership: Ownership.Default
                }
            }
        }
    }
    
    ObjectType{
        name: "DestroyCounter"
    }
    
    ObjectType{
        name: "QObjectDestroyed"
    }
    
    ObjectType{
        name: "XmlEntityResolverSubclass"
        deprecated: true
        ExtraIncludes{
            Include{
                fileName: "QtJambi/JavaAPI"
                location: Include.Global
            }
        }
    }
    
    ObjectType{
        name: "SpinBoxHandler"
    }
    
    ObjectType{
        name: "SpinBoxSubclass"
        ExtraIncludes{
            Include{
                fileName: "QtJambi/JavaAPI"
                location: Include.Global
            }
        }
    }
    
    ObjectType{
        name: "GraphicsSceneSubclass"
        ExtraIncludes{
            Include{
                fileName: "QtJambiWidgets/hashes.h"
                location: Include.Global
            }
            Include{
                fileName: "QtJambiCore/hashes.h"
                location: Include.Global
            }
        }
    }
    
    ObjectType{
        name: "IODeviceSubclass"
        ModifyField{
            name: "buffer_length"
            read: false
            write: false
        }
        ModifyField{
            name: "buffer"
            read: true
            write: false
            ReplaceType{
                modifiedType: "java.nio.ByteBuffer"
            }
            ConversionRule{
                codeClass: CodeClass.NativeGetter
                Text{content: "%out = %in ? %env->NewDirectByteBuffer(%in, jlong(__qt_this->buffer_length)) : nullptr;\n"+
                              "JavaException::check(%env QTJAMBI_STACKTRACEINFO );"}
            }
        }
    }
    
    ObjectType{
        name: "XmlReaderSubclass"
        deprecated: true
    }
    
    ObjectType{
        name: "TextCodecSubclass"
        ModifyFunction{
            signature: "TextCodecSubclass()"
            ModifyArgument{
                index: -1
                DefineOwnership{
                    codeClass: CodeClass.Java
                    ownership: Ownership.Cpp
                }
            }
        }
    }
    
    ValueType{
        name: "PictureSubclass"
    }
    
    ObjectType{
        name: "SetupLayout"
        ModifyFunction{
            signature: "createLayout()"
            ModifyArgument{
                index: "return"
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Java
                }
            }
        }
    }
    
    ObjectType{
        name: "GraphicsItemSubclass"
        ExtraIncludes{
            Include{
                fileName: "QtJambiWidgets/hashes.h"
                location: Include.Global
            }
        }
    }
    
    ObjectType{
        name: "ImageIOHandlerSubclass"
        ModifyFunction{
            signature: "callRead(QImage*)"
            ModifyArgument{
                index: 1
                invalidateAfterUse: true
            }
        }
    }
    
    ObjectType{
        name: "AccessibleInterfaceSubclass"
    }
    
    ObjectType{
        name: "SqlTableModelSubclass"
    }
    
    ObjectType{
        name: "PolymorphicType"
        ModifyFunction{
            signature: "getButtonStyleOption()"
            ModifyArgument{
                index: "return"
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Java
                }
            }
        }
        ModifyFunction{
            signature: "getCustomStyleOption(int)"
            ModifyArgument{
                index: "return"
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Java
                }
            }
        }
        ModifyFunction{
            signature: "getUnmappedCustomStyleOption()"
            ModifyArgument{
                index: "return"
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Java
                }
            }
        }
    }

    ObjectType{
        name: "PerformanceTests"
        ObjectType{
            name: "AbstractVirtualHost"
        }
        ObjectType{
            name: "VirtualHost"
        }
        ModifyFunction{
            signature: "createNativeHost()"
            ModifyArgument{
                index: "return"
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Java
                }
            }
        }
        InjectCode{
            position: Position.End
            Text{content: "public static void empty(){}"}
        }
    }

    ObjectType{
        name: "SpanTest"
        ValueType{
            name: "AlignedStruct"
        }
        since: 6.7
    }
    
    ObjectType{
        name: "CustomEvent"
    }
    
    ObjectType{
        name: "ValidatorSubclass"
        ExtraIncludes{
            Include{
                fileName: "QtJambi/JavaAPI"
                location: Include.Global
            }
        }
        ModifyFunction{
            signature: "callValidate(QString *, int *)"
            ModifyArgument{
                index: 2
                AsArray{
                    minLength: 0
                }
            }
        }
    }
    
    ObjectType{
        name: "SomeQObject"
    }
    
    ValueType{
        name: "CustomStyleOption"
        polymorphicIdExpression: "%1->type == QStyleOption::SO_CustomBase + 1"
    }
    
    ObjectType{
        name: "TestDialog"
    }
    
    ObjectType{
        name: "MessageHandler"
    }
    
    ObjectType{
        name: "AccessibleTextInterfaceSubclass"
        ModifyFunction{
            signature: "callAttributes(AccessibleTextInterfaceSubclass *, int, int *, int *)"
            ModifyArgument{
                index: 3
                ReplaceType{
                    modifiedType: "int[]"
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "jsize %out_size = 1;\n"+
                                  "int* %out = qtjambi_cast<int*>(%env, %scope, jintArray(%in), %out_size);"}
                }
                ConversionRule{
                    codeClass: CodeClass.Shell
                    Text{content: "jintArray %out = qtjambi_cast<jintArray>(%env, %scope, %in, %in ? 1 : 0);"}
                }
            }
            ModifyArgument{
                index: 4
                ReplaceType{
                    modifiedType: "int[]"
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "jsize %out_size = 1;\n"+
                                  "int* %out = qtjambi_cast<int*>(%env, %scope, jintArray(%in), %out_size);"}
                }
                ConversionRule{
                    codeClass: CodeClass.Shell
                    Text{content: "jintArray %out = qtjambi_cast<jintArray>(%env, %scope, %in, %in ? 1 : 0);"}
                }
            }
        }
        ModifyFunction{
            signature: "callSelection(AccessibleTextInterfaceSubclass *, int, int *, int *)"
            ModifyArgument{
                index: 3
                ReplaceType{
                    modifiedType: "int[]"
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "jsize %out_size = 1;\n"+
                                  "int* %out = qtjambi_cast<int*>(%env, %scope, jintArray(%in), %out_size);"}
                }
                ConversionRule{
                    codeClass: CodeClass.Shell
                    Text{content: "jintArray %out = qtjambi_cast<jintArray>(%env, %scope, %in, %in ? 1 : 0);"}
                }
            }
            ModifyArgument{
                index: 4
                ReplaceType{
                    modifiedType: "int[]"
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "jsize %out_size = 1;\n"+
                                  "int* %out = qtjambi_cast<int*>(%env, %scope, jintArray(%in), %out_size);"}
                }
                ConversionRule{
                    codeClass: CodeClass.Shell
                    Text{content: "jintArray %out = qtjambi_cast<jintArray>(%env, %scope, %in, %in ? 1 : 0);"}
                }
            }
        }
        ModifyFunction{
            signature: "callTextAfterOffset(AccessibleTextInterfaceSubclass *, int, QAccessible::TextBoundaryType, int *, int *)"
            ModifyArgument{
                index: 4
                ReplaceType{
                    modifiedType: "int[]"
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "jsize %out_size = 1;\n"+
                                  "int* %out = qtjambi_cast<int*>(%env, %scope, jintArray(%in), %out_size);"}
                }
                ConversionRule{
                    codeClass: CodeClass.Shell
                    Text{content: "jintArray %out = qtjambi_cast<jintArray>(%env, %scope, %in, %in ? 1 : 0);"}
                }
            }
            ModifyArgument{
                index: 5
                ReplaceType{
                    modifiedType: "int[]"
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "jsize %out_size = 1;\n"+
                                  "int* %out = qtjambi_cast<int*>(%env, %scope, jintArray(%in), %out_size);"}
                }
                ConversionRule{
                    codeClass: CodeClass.Shell
                    Text{content: "jintArray %out = qtjambi_cast<jintArray>(%env, %scope, %in, %in ? 1 : 0);"}
                }
            }
        }
        ModifyFunction{
            signature: "callTextAtOffset(AccessibleTextInterfaceSubclass *, int, QAccessible::TextBoundaryType, int *, int *)"
            ModifyArgument{
                index: 4
                ReplaceType{
                    modifiedType: "int[]"
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "jsize %out_size = 1;\n"+
                                  "int* %out = qtjambi_cast<int*>(%env, %scope, jintArray(%in), %out_size);"}
                }
                ConversionRule{
                    codeClass: CodeClass.Shell
                    Text{content: "jintArray %out = qtjambi_cast<jintArray>(%env, %scope, %in, %in ? 1 : 0);"}
                }
            }
            ModifyArgument{
                index: 5
                ReplaceType{
                    modifiedType: "int[]"
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "jsize %out_size = 1;\n"+
                                  "int* %out = qtjambi_cast<int*>(%env, %scope, jintArray(%in), %out_size);"}
                }
                ConversionRule{
                    codeClass: CodeClass.Shell
                    Text{content: "jintArray %out = qtjambi_cast<jintArray>(%env, %scope, %in, %in ? 1 : 0);"}
                }
            }
        }
        ModifyFunction{
            signature: "callTextBeforeOffset(AccessibleTextInterfaceSubclass *, int, QAccessible::TextBoundaryType, int *, int *)"
            ModifyArgument{
                index: 4
                ReplaceType{
                    modifiedType: "int[]"
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "jsize %out_size = 1;\n"+
                                  "int* %out = qtjambi_cast<int*>(%env, %scope, jintArray(%in), %out_size);"}
                }
                ConversionRule{
                    codeClass: CodeClass.Shell
                    Text{content: "jintArray %out = qtjambi_cast<jintArray>(%env, %scope, %in, %in ? 1 : 0);"}
                }
            }
            ModifyArgument{
                index: 5
                ReplaceType{
                    modifiedType: "int[]"
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "jsize %out_size = 1;\n"+
                                  "int* %out = qtjambi_cast<int*>(%env, %scope, jintArray(%in), %out_size);"}
                }
                ConversionRule{
                    codeClass: CodeClass.Shell
                    Text{content: "jintArray %out = qtjambi_cast<jintArray>(%env, %scope, %in, %in ? 1 : 0);"}
                }
            }
        }
    }
    
    NamespaceType{
        name: "ContainerTest"
        Include{
            fileName: "containers.h"
            location: Include.Local
        }
    }
    
    NamespaceType{
        name: "NameSpace"
        Include{
            fileName: "namespace.h"
            location: Include.Local
        }
    
		NamespaceType{
			name: "NameSpace2"
    
			NamespaceType{
				name: "NameSpace3"
    
				ObjectType{
					name: "ObjectC"
				}
				
				ObjectType{
					name: "ObjectD"
				}
				
				ValueType{
					name: "ValueB"
				}
				
				InterfaceType{
					name: "InterfaceA"
				}
			}
		}
    
		ObjectType{
			name: "ObjectA"
			Include{
				fileName: "namespace.h"
				location: Include.Local
			}
		}
		
		ObjectType{
			name: "ObjectB"
			Include{
				fileName: "namespace.h"
				location: Include.Local
			}
		}
		
		ValueType{
			name: "ValueA"
			Include{
				fileName: "namespace.h"
				location: Include.Local
			}
			ModifyFunction{
				signature: "operator=(NameSpace::ValueA)"
				remove: RemoveFlag.All
			}
		}
    }
    
    ObjectType{
        name: "AbstractSocketSubclass"
        ModifyFunction{
            signature: "emitProxyAuthenticationRequired(QAbstractSocket *, const QNetworkProxy &, QAuthenticator *)"
            ModifyArgument{
                index: 3
                invalidateAfterUse: true
            }
        }
        ModifyFunction{
            signature: "emitSignalAccessor(const QNetworkProxy &, QAuthenticator *)"
            ModifyArgument{
                index: 2
                invalidateAfterUse: true
            }
        }
    }
    
    ValueType{
        name: "QHash_int"
    }
    
    ValueType{
        name: "QLinkedList_int"
    }
    
    ValueType{
        name: "QList_int"
    }
    
    ValueType{
        name: "QList_String"
    }
    
    ValueType{
        name: "QMap_int"
    }
    
    ValueType{
        name: "QMap_int_String"
    }
    
    ValueType{
        name: "QMap_String_int"
    }
    
    ValueType{
        name: "QMap_String"
    }
    
    ValueType{
        name: "QQueue_int"
    }
    
    ValueType{
        name: "QQueue_String"
    }
    
    ValueType{
        name: "QSet_int"
    }
    
    ValueType{
        name: "QSet_String"
    }
    
    ValueType{
        name: "QStack_int"
    }
    
    ValueType{
        name: "QStack_String"
    }
    
    ValueType{
        name: "QVector_int"
    }
    
    ValueType{
        name: "QVector_String"
    }
    
    ValueType{
        name: "QMultiMap_int"
    }
    
    ValueType{
        name: "QMultiMap_String"
    }
    
    ValueType{
        name: "QMultiMap_String_int"
    }
    
    ValueType{
        name: "QMultiMap_int_String"
    }
    
    ValueType{
        name: "QMultiHash_int"
    }
    
    ValueType{
        name: "QMultiHash_String"
    }
    
    ValueType{
        name: "QMultiHash_String_int"
    }
    
    ValueType{
        name: "QMultiHash_int_String"
    }
    
    ObjectType{
        name: "PolymorphicObjectType"
    }
    
    ObjectType{
        name: "PolymorphicOwnedObjectType"
        threadAffinity: "%1->owner()"
    }
    
    ObjectType{
        name: "NonPolymorphicObjectType"
    }
    
    ObjectType{
        name: "NonPolymorphicOwnedObjectType"
        threadAffinity: "%1->owner()"
    }
    
    ObjectType{
        name: "QObjectType"
        ModifyFunction{
            signature: "QObjectType()"
            InjectCode{
                position: Position.End
				Text{content: String.raw`
if(java.util.logging.Logger.getLogger("io.qt.autotests").isLoggable(java.util.logging.Level.FINEST)) {
	Class<?> cls = this.getClass();
	int hash = System.identityHashCode(this);
	destroyed.connect(()->{
		String className = cls.getName();
		String shortClassName = className;
		int i = shortClassName.lastIndexOf('.');
		if(i > 0)
			shortClassName = shortClassName.substring(i + 1);
		java.util.logging.Logger.getLogger("io.qt.autotests").log(java.util.logging.Level.FINEST, shortClassName + ".dtor(" + className + "@" + hash + ") thread=" + io.qt.autotests.ApplicationInitializer.currentThreadToString());
	});
}
				`}
            }
        }
    }
    
    ValueType{
        name: "ValueType"
		InjectCode{
			position: Position.Beginning
			target: CodeClass.Native
            Text{content: "void qtjambi_increase_destructorFunctionCalledCount(const char*className);"}
		}
        ModifyFunction{
            signature: "newInstance()"
            ModifyArgument{
                index: 0
                ReplaceType{
                    modifiedType: "io.qt.autotests.generated.ValueType"
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "%out = qtjambi_cast<jobject>(%env, %in);"}
                }
            }
        }
        CustomDestructor{
            Text{content: String.raw`
                qtjambi_increase_destructorFunctionCalledCount("ValueType");
                QtPrivate::QMetaTypeForType<ValueType>::getDtor()(metaTypeInterface, ptr);`}
        }
    }
    
    ObjectType{
        name: "InvalidatorPolymorphicOwnedObjectType"
        ModifyFunction{
            signature: "overrideMe(PolymorphicOwnedObjectType*)"
            ModifyArgument{
                index: 1
                invalidateAfterUse: true
            }
        }
    }
    
    ObjectType{
        name: "InvalidatorNonPolymorphicOwnedObjectType"
        ModifyFunction{
            signature: "overrideMe(NonPolymorphicOwnedObjectType*)"
            ModifyArgument{
                index: 1
                invalidateAfterUse: true
            }
        }
    }
    
    ObjectType{
        name: "InvalidatorPolymorphicObjectType"
        ModifyFunction{
            signature: "overrideMe(PolymorphicObjectType*)"
            ModifyArgument{
                index: 1
                invalidateAfterUse: true
            }
        }
    }
    
    ObjectType{
        name: "InvalidatorNonPolymorphicObjectType"
        ModifyFunction{
            signature: "overrideMe(NonPolymorphicObjectType*)"
            ModifyArgument{
                index: 1
                invalidateAfterUse: true
            }
        }
    }
    
    ObjectType{
        name: "InvalidatorValueType"
        ModifyFunction{
            signature: "invalidateObject(ValueType*)"
            ModifyArgument{
                index: 1
                ReplaceType{
                    modifiedType: "io.qt.autotests.generated.ValueType"
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "ValueType* %out = qtjambi_cast<ValueType*>(%env, %in);"}
                }
            }
        }
        ModifyFunction{
            signature: "overrideMe(ValueType*)"
            ModifyArgument{
                index: 1
                invalidateAfterUse: true
                ReplaceType{
                    modifiedType: "io.qt.autotests.generated.ValueType"
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "ValueType* %out = qtjambi_cast<ValueType*>(%env, %in);"}
                }
                ConversionRule{
                    codeClass: CodeClass.Shell
                    Text{content: "jobject %out = qtjambi_cast<jobject>(%env, %in);"}
                }
            }
        }
    }
    
    ObjectType{
        name: "InvalidatorQObjectType"
        ModifyFunction{
            signature: "overrideMe(QObjectType*)"
            ModifyArgument{
                index: 1
                invalidateAfterUse: true
            }
        }
    }
    
    ObjectType{
        name: "FlagsAndEnumTest"
    }
    
    ObjectType{
        name: "PropertyAndMethodCallTest"
        ExtraIncludes{
            Include{
                fileName: "QtJambiWidgets/hashes.h"
                location: Include.Global
            }
        }
        //Rejection{functionName: "dumpMetaObject"}
        ModifyFunction{
            signature: "instantiateInPlace(const QMetaMethod&, std::initializer_list<QVariant>)"
            ModifyArgument{
                index: 0
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Java
                }
            }
            since: 6.8
        }
    }
    
    ObjectType{
        name: "MetaObjectQtMetaCast"
        Include{
            fileName: "metaobjectqtmetacast.h"
            location: Include.Local
        }
    }
    
    ObjectType{
        name: "SmartPointerTest"
        ExtraIncludes{
            Include{
                fileName: "QtJambiWidgets/hashes.h"
                location: Include.Global
            }
            Include{
                fileName: "QtJambi/JObjectWrapper"
                location: Include.Global
            }
        }
		FunctionalType{
			name: "StringSupplier"
		}
    }
    
    ObjectType{
        name: "SettingsTest"
    }
    
    ObjectType{
        name: "MultiSignalTest"
    }
    
    ObjectType{
        name: "TestAbstractClass"
        ModifyFunction{
            signature: "createInterfaceImplementingQObject(QObject*)"
            ModifyArgument{
                index: 0
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Java
                }
            }
        }
        ModifyFunction{
            signature: "createQObjectInheritingInterface(QObject*)"
            ModifyArgument{
                index: 0
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Java
                }
            }
        }
        ModifyFunction{
            signature: "setReferenceCountTest1(QObject*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    variableName: "__rcReferenceCountTest1"
                    condition: "__qt_return_value"
                    action: ReferenceCount.Set
                }
            }
        }
    }
    
    ObjectType{
        name: "TestInterfaceImpl"
        ModifyFunction{
            signature: "setReferenceCountTest1(QObject*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    variableName: "__rcReferenceCountTest1"
                    condition: "__qt_return_value"
                    action: ReferenceCount.Set
                }
            }
        }
    }
    
    ObjectType{
        name: "TestInterfaceObject"
        ModifyFunction{
            signature: "setReferenceCountTest1(QObject*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    variableName: "__rcReferenceCountTest1"
                    condition: "__qt_return_value"
                    action: ReferenceCount.Set
                }
            }
        }
        ModifyFunction{
            signature: "create(QString)"
            ModifyArgument{
                index: "return"
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Java
                }
            }
        }
    }
    
    InterfaceType{
        name: "TestInterface"
        ModifyFunction{
            signature: "setReferenceCountTest1(QObject*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    variableName: "__rcReferenceCountTest1"
                    condition: "__qt_return_value"
                    action: ReferenceCount.Set
                }
            }
        }
        ModifyFunction{
            signature: "setReferenceCountTest2(QObject*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    variableName: "__rcReferenceCountTest2"
                    condition: "__qt_return_value"
                    action: ReferenceCount.Set
                }
            }
        }
        ModifyFunction{
            signature: "setReferenceCountTest3(QObject*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    variableName: "__rcReferenceCountTest3"
                    condition: "__qt_return_value"
                    action: ReferenceCount.Set
                }
            }
        }
    }
    
    InterfaceType{
        name: "TestPrivateInterface"
    }
    
    ObjectType{
        name: "FunctionManager"
    
		FunctionalType{
			name: "StringSupplier"
		}
		
		FunctionalType{
			name: "StringConsumer"
		}
    }
    
    ObjectType{
        name: "FutureHandler"
        ExtraIncludes{
            Include{
                fileName: "QtCore/QPointer"
                location: Include.Global
            }
        }
        ModifyFunction{
            signature: "checkString(QFutureWatcher<QString>*)"
            ModifyArgument{
                index: 1
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "QFutureWatcher<QString>* %out = qtjambi_cast<QFutureWatcher<QString>*>(%env, %scope, %in);"}
                }
            }
        }
        ModifyFunction{
            signature: "checkObject(QFutureWatcher<QObject*>*)"
            ModifyArgument{
                index: 1
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "QFutureWatcher<QObject*>* %out = qtjambi_cast<QFutureWatcher<QObject*>*>(%env, %scope, %in);"}
                }
            }
        }
        ModifyFunction{
            signature: "checkInt(QFutureWatcher<int>*)"
            ModifyArgument{
                index: 1
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "QFutureWatcher<int>* %out = qtjambi_cast<QFutureWatcher<int>*>(%env, %scope, %in);"}
                }
            }
        }
        ModifyFunction{
            signature: "checkVoid(QFutureWatcher<void>*)"
            ModifyArgument{
                index: 1
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "QFutureWatcher<void>* %out = qtjambi_cast<QFutureWatcher<void>*>(%env, %scope, %in);"}
                }
            }
        }
    }
    
    ObjectType{
        name: "ExceptionTest"
    }
    
    ObjectType{
        name: "FunctionalTest"
    
		FunctionalType{
			name: "TestFunction1"
		}
		FunctionalType{
			name: "TestFunction2"
		}
        FunctionalType{
            name: "TestFunction3"
        }
        FunctionalType{
            name: "TestFunction4"
        }
        FunctionalType{
            name: "TestFunction5"
        }
        ModifyFunction{
            signature: "takeTestFunction2(FunctionalTest::TestFunction2)"
            ModifyArgument{
                index: 1
                ReplaceType{
                    modifiedType: "io.qt.autotests.generated.FunctionalTest$TestFunction2"
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "FunctionalTest::TestFunction2 %out = qtjambi_cast<FunctionalTest::TestFunction2>(%env, %in);"}
                }
            }
        }
        ModifyFunction{
            signature: "takeTestFunction3(FunctionalTest::TestFunction3)"
            ModifyArgument{
                index: 1
                ReplaceType{
                    modifiedType: "io.qt.autotests.generated.FunctionalTest$TestFunction3"
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "FunctionalTest::TestFunction3 %out = qtjambi_cast<FunctionalTest::TestFunction3>(%env, %in);"}
                }
            }
        }
        ModifyFunction{
            signature: "takeTestFunction4(FunctionalTest::TestFunction4)"
            ModifyArgument{
                index: 1
                ReplaceType{
                    modifiedType: "io.qt.autotests.generated.FunctionalTest$TestFunction4"
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "FunctionalTest::TestFunction4 %out = qtjambi_cast<FunctionalTest::TestFunction4>(%env, %in);"}
                }
            }
        }
        ModifyFunction{
            signature: "takeTestFunction5(FunctionalTest::TestFunction5)"
            ModifyArgument{
                index: 1
                ReplaceType{
                    modifiedType: "io.qt.autotests.generated.FunctionalTest$TestFunction5"
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "FunctionalTest::TestFunction5 %out = qtjambi_cast<FunctionalTest::TestFunction5>(%env, %in);"}
                }
            }
        }
    }

    ObjectType{
        name: "ThreadFactory"
        ModifyFunction{
            signature: "createQLoopThread()"
            ModifyArgument{
                index: "return"
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Java
                }
            }
        }
        ModifyFunction{
            signature: "createQWorkerThread(QRunnable*)"
            ModifyArgument{
                index: "return"
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Java
                }
            }
            ModifyArgument{
                index: 1
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Cpp
                }
            }
        }
        ModifyFunction{
            signature: "startQWorkerThread(QRunnable*)"
            ModifyArgument{
                index: 1
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Cpp
                }
            }
        }
        ModifyFunction{
            signature: "startNativeWorkerThread(QRunnable*)"
            ModifyArgument{
                index: 1
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Cpp
                }
            }
        }
    }
    
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: private virtual function 'abstractFunction(const QString*)' in 'AnotherNonAbstractSubclass'"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: enum 'Variants::UnknownEnum' does not have a type entry or is not an enum"}
}
