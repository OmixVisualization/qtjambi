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
    packageName: "io.qt.location"
    defaultSuperClass: "QtObject"
    qtLibrary: "QtLocation"
    module: "qtjambi.location"
    description: "Displays map, navigation, and place content in a QML application."
    RequiredLibrary{
        name: "QtQuickShapes"
        mode: RequiredLibrary.ProvideOnly
    }
    RequiredLibrary{
        name: "QtNetwork"
        mode: RequiredLibrary.Optional
    }
    RequiredLibrary{
        name: "QtDBus"
        mode: RequiredLibrary.Optional
    }

    Rejection{
        className: "QGeoRoute"
        functionName: "d"
    }
    Rejection{
        className: "QGeoRoute"
        functionName: "const_d"
    }

    Rejection{
        className: "QGeoServiceProviderFactory"
        functionName: "createMappingManagerEngine"
    }
    
    Rejection{
        className: "QGeoServiceProviderFactoryV2"
        functionName: "createNavigationManagerEngine"
    }

    Rejection{
        className: "QGeoServiceProvider"
        functionName: "mappingManager"
    }
    
    NamespaceType{
        name: "QLocation"
        ExtraIncludes{
            Include{
                fileName: "QtLocation/QLocation"
                location: Include.Global
            }
        }

        EnumType{
            name: "Visibility"
        }
    }
    
    ObjectType{
        name: "QGeoCodeReply"

        EnumType{
            name: "Error"
        }
    }
    
    ObjectType{
        name: "QGeoRouteReply"
        EnumType{
            name: "Error"
        }
    }
    
    ObjectType{
        name: "QGeoServiceProvider"

        EnumType{
            name: "Error"
        }

        EnumType{
            name: "GeocodingFeature"
        }

        EnumType{
            name: "MappingFeature"
        }

        EnumType{
            name: "NavigationFeature"
        }

        EnumType{
            name: "PlacesFeature"
        }

        EnumType{
            name: "RoutingFeature"
        }
        ModifyFunction{
            signature: "setQmlEngine(QQmlEngine *)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    variableName: "__rcQmlEngine"
                    action: ReferenceCount.Set
                }
            }
        }
    }
    
    ObjectType{
        name: "QPlaceReply"

        EnumType{
            name: "Error"
        }

        EnumType{
            name: "Type"
        }
    }
    
    ObjectType{
        name: "QPlaceIdReply"

        EnumType{
            name: "OperationType"
        }
    }
    
    ObjectType{
        name: "QPlaceMatchReply"
    }
    
    ObjectType{
        name: "QPlaceContentReply"
    }
    
    ObjectType{
        name: "QPlaceDetailsReply"
    }
    
    ObjectType{
        name: "QPlaceSearchReply"
    }
    
    ObjectType{
        name: "QPlaceManager"
    }
    
    ObjectType{
        name: "QGeoJson"
    }
    
    ObjectType{
        name: "QGeoCodingManager"
    }
    
    ObjectType{
        name: "QGeoCodingManagerEngine"
    }
    
    ObjectType{
        name: "QGeoRoutingManager"
    }
    
    ObjectType{
        name: "QGeoRoutingManagerEngine"
    }
    
    InterfaceType{
        name: "QGeoServiceProviderFactory"
        InjectCode{
            target: CodeClass.JavaInterface
            ImportFile{
                name: ":/io/qtjambi/generator/typesystem/QtJambiLocation.java"
                quoteAfterLine: "class QGeoServiceProviderFactory___"
                quoteBeforeLine: "}// class"
            }
        }
        ExtraIncludes{
            Include{
                fileName: "utils_p.h"
                location: Include.Local
            }
        }
        ModifyFunction{
            signature: "setQmlEngine(QQmlEngine*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    variableName: "__rcEngine"
                    action: ReferenceCount.Set
                }
            }
        }
        ModifyFunction{
            signature: "createGeocodingManagerEngine(const QMap<QString,QVariant> &,QGeoServiceProvider::Error *,QString *) const"
            ModifyArgument{
                index: 0
                ReplaceType{
                    modifiedType: "io.qt.location.QGeoServiceProviderFactory$@NonNull Result<@Nullable QGeoCodingManagerEngine>"
                }
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Java
                }
                DefineOwnership{
                    codeClass: CodeClass.Shell
                    ownership: Ownership.Cpp
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: String.raw`
jobject __java_engine = qtjambi_cast<jobject>(%env, %in);
QtJambiAPI::setJavaOwnershipForTopLevelObject(%env, %in);
jobject __java_%2 = qtjambi_cast<jobject>(%env, %2);
jstring __java_%3 = qtjambi_cast<jstring>(%env, %3);
jobject %out = Java::QtLocation::QGeoServiceProviderFactory$Result::newInstance(%env, __java_engine, __java_%2, __java_%3);`}
                }
                ConversionRule{
                    codeClass: CodeClass.Shell
                    Text{content: String.raw`
%out = nullptr;
if(%in){
    %out = qtjambi_cast<QGeoCodingManagerEngine*>(%env, Java::QtLocation::QGeoServiceProviderFactory$Result::engine(%env, %in));
    if(%2)
        *%2 = qtjambi_cast<QGeoServiceProvider::Error>(%env, Java::QtLocation::QGeoServiceProviderFactory$Result::error(%env, %in));
    if(%3)
        *%3 = qtjambi_cast<QString>(%env, Java::QtLocation::QGeoServiceProviderFactory$Result::errorString(%env, %in));
}else{
    if(%2)
        *%2 = QGeoServiceProvider::LoaderError;
    if(%3)
        *%3 = QStringLiteral("No result returned.");
}`}
                }
            }
            ModifyArgument{
                index: 2
                RemoveArgument{
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: String.raw`
                        QGeoServiceProvider::Error %in = QGeoServiceProvider::NoError;
                        QGeoServiceProvider::Error* %out = &%in;`}
                }
            }
            ModifyArgument{
                index: 3
                RemoveArgument{
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: String.raw`
                            QString %in;
                            QString* %out = &%in;`}
                }
            }
        }
        ModifyFunction{
            signature: "createRoutingManagerEngine(const QMap<QString,QVariant> &,QGeoServiceProvider::Error *,QString *) const"
            ModifyArgument{
                index: 0
                ReplaceType{
                    modifiedType: "io.qt.location.QGeoServiceProviderFactory$@NonNull Result<@Nullable QGeoRoutingManagerEngine>"
                }
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Java
                }
                DefineOwnership{
                    codeClass: CodeClass.Shell
                    ownership: Ownership.Cpp
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: String.raw`
jobject __java_engine = qtjambi_cast<jobject>(%env, %in);
QtJambiAPI::setJavaOwnershipForTopLevelObject(%env, %in);
jobject __java_%2 = qtjambi_cast<jobject>(%env, %2);
jstring __java_%3 = qtjambi_cast<jstring>(%env, %3);
jobject %out = Java::QtLocation::QGeoServiceProviderFactory$Result::newInstance(%env, __java_engine, __java_%2, __java_%3);`}
                }
                ConversionRule{
                    codeClass: CodeClass.Shell
                    Text{content: String.raw`
%out = nullptr;
if(%in){
    %out = qtjambi_cast<QGeoRoutingManagerEngine*>(%env, Java::QtLocation::QGeoServiceProviderFactory$Result::engine(%env, %in));
    if(%2)
        *%2 = qtjambi_cast<QGeoServiceProvider::Error>(%env, Java::QtLocation::QGeoServiceProviderFactory$Result::error(%env, %in));
    if(%3)
        *%3 = qtjambi_cast<QString>(%env, Java::QtLocation::QGeoServiceProviderFactory$Result::errorString(%env, %in));
}else{
    if(%2)
        *%2 = QGeoServiceProvider::LoaderError;
    if(%3)
        *%3 = QStringLiteral("No result returned.");
}`}
                }
            }
            ModifyArgument{
                index: 2
                RemoveArgument{
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: String.raw`
                            QGeoServiceProvider::Error %in = QGeoServiceProvider::NoError;
                            QGeoServiceProvider::Error* %out = &%in;`}
                }
            }
            ModifyArgument{
                index: 3
                RemoveArgument{
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: String.raw`
                        QString %in;
                        QString* %out = &%in;`}
                }
            }
        }
        ModifyFunction{
            signature: "createPlaceManagerEngine(const QMap<QString,QVariant> &,QGeoServiceProvider::Error *,QString *) const"
            ModifyArgument{
                index: 0
                ReplaceType{
                    modifiedType: "io.qt.location.QGeoServiceProviderFactory$@NonNull Result<@Nullable QPlaceManagerEngine>"
                }
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Java
                }
                DefineOwnership{
                    codeClass: CodeClass.Shell
                    ownership: Ownership.Cpp
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: String.raw`
jobject __java_engine = qtjambi_cast<jobject>(%env, %in);
QtJambiAPI::setJavaOwnershipForTopLevelObject(%env, %in);
jobject __java_%2 = qtjambi_cast<jobject>(%env, %2);
jstring __java_%3 = qtjambi_cast<jstring>(%env, %3);
jobject %out = Java::QtLocation::QGeoServiceProviderFactory$Result::newInstance(%env, __java_engine, __java_%2, __java_%3);`}
                }
                ConversionRule{
                    codeClass: CodeClass.Shell
                    Text{content: String.raw`
%out = nullptr;
if(%in){
    %out = qtjambi_cast<QPlaceManagerEngine*>(%env, Java::QtLocation::QGeoServiceProviderFactory$Result::engine(%env, %in));
    if(%2)
        *%2 = qtjambi_cast<QGeoServiceProvider::Error>(%env, Java::QtLocation::QGeoServiceProviderFactory$Result::error(%env, %in));
    if(%3)
        *%3 = qtjambi_cast<QString>(%env, Java::QtLocation::QGeoServiceProviderFactory$Result::errorString(%env, %in));
}else{
    if(%2)
        *%2 = QGeoServiceProvider::LoaderError;
    if(%3)
        *%3 = QStringLiteral("No result returned.");
}`}
                }
            }
            ModifyArgument{
                index: 2
                RemoveArgument{
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: String.raw`
                            QGeoServiceProvider::Error %in = QGeoServiceProvider::NoError;
                            QGeoServiceProvider::Error* %out = &%in;`}
                }
            }
            ModifyArgument{
                index: 3
                RemoveArgument{
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: String.raw`
                        QString %in;
                        QString* %out = &%in;`}
                }
            }
        }
    }
    
    InterfaceType{
        name: "QGeoServiceProviderFactoryV2"
        ExtraIncludes{
            Include{
                fileName: "utils_p.h"
                location: Include.Local
            }
        }
    }
    
    InterfaceType{
        name: "QGeoServiceProviderFactoryV3"
        ModifyFunction{
            signature: "setQmlEngine(QQmlEngine *)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    variableName: "__rcQmlEngine"
                    action: ReferenceCount.Set
                }
            }
        }
        ExtraIncludes{
            Include{
                fileName: "utils_p.h"
                location: Include.Local
            }
        }
    }
    
    ObjectType{
        name: "QPlaceManagerEngine"
    }
    
    ObjectType{
        name: "QPlaceSearchSuggestionReply"
    }
    
    ValueType{
        name: "QGeoRoute"
    }
    
    ValueType{
        name: "QGeoRouteLeg"
    }
    
    ValueType{
        name: "QPlace"
    }
    
    ValueType{
        name: "QPlaceIcon"
        ModifyFunction{
            signature: "setManager(QPlaceManager *)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    variableName: "__rcManager"
                    action: ReferenceCount.Set
                }
            }
        }
    }
    
    ValueType{
        name: "QGeoManeuver"

        EnumType{
            name: "InstructionDirection"
        }
    }
    
    ValueType{
        name: "QPlaceAttribute"
    }
    
    ValueType{
        name: "QPlaceCategory"
    }
    
    ValueType{
        name: "QPlaceContactDetail"
    }
    
    ValueType{
        name: "QPlaceMatchRequest"
    }
    
    ValueType{
        name: "QPlaceSearchResult"

        EnumType{
            name: "SearchResultType"
        }
    }
    
    ValueType{
        name: "QPlaceResult"
        noImplicitConstructors: true
    }
    
    ValueType{
        name: "QPlaceContent"

        EnumType{
            name: "Type"
        }

        EnumType{
            name: "DataTag"
            since: [6,5]
        }
    }
    
    ValueType{
        name: "QPlaceEditorial"
        noImplicitConstructors: true
    }
    
    ValueType{
        name: "QPlaceImage"
        noImplicitConstructors: true
    }
    
    ValueType{
        name: "QPlaceReview"
        noImplicitConstructors: true
    }
    
    ValueType{
        name: "QPlaceContentRequest"
    }
    
    ValueType{
        name: "QPlaceProposedSearchResult"
        noImplicitConstructors: true
    }
    
    ValueType{
        name: "QPlaceRatings"
    }
    
    ValueType{
        name: "QPlaceSearchRequest"

        EnumType{
            name: "RelevanceHint"
        }
    }
    
    ValueType{
        name: "QPlaceSupplier"
    }
    
    ValueType{
        name: "QPlaceUser"
    }
    
    ValueType{
        name: "QGeoRouteRequest"

        EnumType{
            name: "FeatureType"
        }

        EnumType{
            name: "FeatureWeight"
        }

        EnumType{
            name: "ManeuverDetail"
        }

        EnumType{
            name: "RouteOptimization"
        }

        EnumType{
            name: "SegmentDetail"
        }

        EnumType{
            name: "TravelMode"
        }
    }
    
    ValueType{
        name: "QGeoRouteSegment"
    }
    
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: skipping function 'QGeoRoute::d', unmatched return type '*QExplicitlySharedDataPointer*'"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: skipping function 'QGeoRoute::const_d', unmatched return type '*QExplicitlySharedDataPointer*'"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: skipping function 'QGeoRoute::QGeoRoute', unmatched parameter type '*QExplicitlySharedDataPointer*'"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: skipping function 'QGeoRouteSegment::d', unmatched return type '*QExplicitlySharedDataPointer*'"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: skipping function 'QGeoRouteSegment::QGeoRouteSegment', unmatched parameter type '*QExplicitlySharedDataPointer*'"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: skipping function 'QGeoRouteLeg::QGeoRouteLeg', unmatched parameter type '*QExplicitlySharedDataPointer*'"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: skipping function 'QPlace::d', unmatched return type '*QSharedDataPointer*'"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: skipping function 'QPlace::QPlace', unmatched parameter type '*QSharedDataPointer*'"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: skipping function 'QGeoManeuver::QGeoManeuver', unmatched parameter type '*QSharedDataPointer*'"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: skipping *, unmatched *type '*QGeoMappingManager*'"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: skipping *, unmatched *type '*QNavigationManager*'"}
}
