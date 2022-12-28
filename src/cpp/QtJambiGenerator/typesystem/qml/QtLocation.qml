/****************************************************************************
**
** Copyright (C) 2009-2022 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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
    defaultSuperClass: "io.qt.QtObject"
    qtLibrary: "QtLocation"
    module: "qtjambi.location"
    description: "Displays map, navigation, and place content in a QML application."

    Rejection{
        className: "QGeoServiceProviderFactory"
        functionName: "createMappingManagerEngine"
    }
    
    Rejection{
        className: "QGeoServiceProviderFactoryV2"
        functionName: "createNavigationManagerEngine"
    }
    
    NamespaceType{
        name: "QLocation"
        targetType: "class"
        ExtraIncludes{
            Include{
                fileName: "QtLocation/QLocation"
                location: Include.Global
            }
        }
    }
    
    EnumType{
        name: "QLocation::Visibility"
        flags: "QLocation::VisibilityScope"
    }
    
    EnumType{
        name: "QGeoCodeReply::Error"
    }
    
    ObjectType{
        name: "QGeoCodeReply"
    }
    
    EnumType{
        name: "QGeoRouteReply::Error"
    }
    
    ObjectType{
        name: "QGeoRouteReply"
    }
    
    EnumType{
        name: "QGeoServiceProvider::Error"
    }
    
    EnumType{
        name: "QGeoServiceProvider::GeocodingFeature"
        flags: "QGeoServiceProvider::GeocodingFeatures"
    }
    
    EnumType{
        name: "QGeoServiceProvider::MappingFeature"
        flags: "QGeoServiceProvider::MappingFeatures"
    }
    
    EnumType{
        name: "QGeoServiceProvider::NavigationFeature"
        flags: "QGeoServiceProvider::NavigationFeatures"
    }
    
    EnumType{
        name: "QGeoServiceProvider::PlacesFeature"
        flags: "QGeoServiceProvider::PlacesFeatures"
    }
    
    EnumType{
        name: "QGeoServiceProvider::RoutingFeature"
        flags: "QGeoServiceProvider::RoutingFeatures"
    }
    
    ObjectType{
        name: "QGeoServiceProvider"
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
    
    EnumType{
        name: "QPlaceReply::Error"
    }
    
    EnumType{
        name: "QPlaceReply::Type"
    }
    
    EnumType{
        name: "QPlaceIdReply::OperationType"
    }
    
    ObjectType{
        name: "QPlaceReply"
    }
    
    ObjectType{
        name: "QPlaceIdReply"
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
            signature: "createGeocodingManagerEngine(const QMap<QString,QVariant> &,QGeoServiceProvider::Error *,QString *) const"
            ModifyArgument{
                index: 0
                ReplaceType{
                    modifiedType: "io.qt.location.QGeoServiceProviderFactory$Result<QGeoCodingManagerEngine>"
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
                    Text{content: "jobject __java_engine = qtjambi_cast<jobject>(%env, %in);\n"+
                                  "QtJambiAPI::setJavaOwnershipForTopLevelObject(%env, %in);\n"+
                                  "jobject __java_%2 = qtjambi_cast<jobject>(%env, %2);\n"+
                                  "jstring __java_%3 = qtjambi_cast<jstring>(%env, %3);\n"+
                                  "jobject %out = Java::QtLocation::QGeoServiceProviderFactory$Result::newInstance(\n"+
                                  "%env,\n"+
                                  "__java_engine,\n"+
                                  "__java_%2,\n"+
                                  "__java_%3);"}
                }
                ConversionRule{
                    codeClass: CodeClass.Shell
                    Text{content: "QGeoCodingManagerEngine*  %out = nullptr;\n"+
                                  "if(%in){\n"+
                                  "jobject __java_engine = Java::QtLocation::QGeoServiceProviderFactory$Result::engine(%env, %in);\n"+
                                  "%out = qtjambi_cast<QGeoCodingManagerEngine*>(%env, __java_engine);\n"+
                                  "if(%2){\n"+
                                  "jobject __java_%2 = Java::QtLocation::QGeoServiceProviderFactory$Result::error(%env, %in);\n"+
                                  "*%2 = qtjambi_cast<QGeoServiceProvider::Error>(%env, __java_%2);\n"+
                                  "}\n"+
                                  "if(%3){\n"+
                                  "jobject __java_%3 = Java::QtLocation::QGeoServiceProviderFactory$Result::errorString(%env, %in);\n"+
                                  "*%3 = qtjambi_cast<QString>(%env, __java_%3);\n"+
                                  "}\n"+
                                  "}else{\n"+
                                  "if(%2){\n"+
                                  "*%2 = QGeoServiceProvider::LoaderError;\n"+
                                  "}\n"+
                                  "if(%3){\n"+
                                  "*%3 = QString(\"No result returned.\");\n"+
                                  "}\n"+
                                  "}"}
                }
            }
            ModifyArgument{
                index: 2
                RemoveArgument{
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "QGeoServiceProvider::Error %in = QGeoServiceProvider::NoError;\n"+
                                  "QGeoServiceProvider::Error* %out = &%in;"}
                }
            }
            ModifyArgument{
                index: 3
                RemoveArgument{
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "QString %in;\n"+
                                  "QString* %out = &%in;"}
                }
            }
        }
        ModifyFunction{
            signature: "createRoutingManagerEngine(const QMap<QString,QVariant> &,QGeoServiceProvider::Error *,QString *) const"
            ModifyArgument{
                index: 0
                ReplaceType{
                    modifiedType: "io.qt.location.QGeoServiceProviderFactory.Result<QGeoRoutingManagerEngine>"
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
                    Text{content: "jobject __java_engine = qtjambi_cast<jobject>(%env, %in);\n"+
                                  "QtJambiAPI::setJavaOwnershipForTopLevelObject(%env, %in);\n"+
                                  "jobject __java_%2 = qtjambi_cast<jobject>(%env, %2);\n"+
                                  "jstring __java_%3 = qtjambi_cast<jstring>(%env, %3);\n"+
                                  "jobject %out = Java::QtLocation::QGeoServiceProviderFactory$Result::newInstance(\n"+
                                  "%env,\n"+
                                  "__java_engine,\n"+
                                  "__java_%2,\n"+
                                  "__java_%3);"}
                }
                ConversionRule{
                    codeClass: CodeClass.Shell
                    Text{content: "QGeoRoutingManagerEngine*  %out = nullptr;\n"+
                                  "if(%in){\n"+
                                  "jobject __java_engine = Java::QtLocation::QGeoServiceProviderFactory$Result::engine(%env, %in);\n"+
                                  "%out = qtjambi_cast<QGeoRoutingManagerEngine*>(%env, __java_engine);\n"+
                                  "if(%2){\n"+
                                  "jobject __java_%2 = Java::QtLocation::QGeoServiceProviderFactory$Result::error(%env, %in);\n"+
                                  "*%2 = qtjambi_cast<QGeoServiceProvider::Error>(%env, __java_%2);\n"+
                                  "}\n"+
                                  "if(%3){\n"+
                                  "jobject __java_%3 = Java::QtLocation::QGeoServiceProviderFactory$Result::errorString(%env, %in);\n"+
                                  "*%3 = qtjambi_cast<QString>(%env, __java_%3);\n"+
                                  "}\n"+
                                  "}else{\n"+
                                  "if(%2){\n"+
                                  "*%2 = QGeoServiceProvider::LoaderError;\n"+
                                  "}\n"+
                                  "if(%3){\n"+
                                  "*%3 = QString(\"No result returned.\");\n"+
                                  "}\n"+
                                  "}"}
                }
            }
            ModifyArgument{
                index: 2
                RemoveArgument{
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "QGeoServiceProvider::Error %in = QGeoServiceProvider::NoError;\n"+
                                  "QGeoServiceProvider::Error* %out = &%in;"}
                }
            }
            ModifyArgument{
                index: 3
                RemoveArgument{
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "QString %in;\n"+
                                  "QString* %out = &%in;"}
                }
            }
        }
        ModifyFunction{
            signature: "createPlaceManagerEngine(const QMap<QString,QVariant> &,QGeoServiceProvider::Error *,QString *) const"
            ModifyArgument{
                index: 0
                ReplaceType{
                    modifiedType: "io.qt.location.QGeoServiceProviderFactory.Result<QPlaceManagerEngine>"
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
                    Text{content: "jobject __java_engine = qtjambi_cast<jobject>(%env, %in);\n"+
                                  "QtJambiAPI::setJavaOwnershipForTopLevelObject(%env, %in);\n"+
                                  "jobject __java_%2 = qtjambi_cast<jobject>(%env, %2);\n"+
                                  "jstring __java_%3 = qtjambi_cast<jstring>(%env, %3);\n"+
                                  "jobject %out = Java::QtLocation::QGeoServiceProviderFactory$Result::newInstance(\n"+
                                  "%env,\n"+
                                  "__java_engine,\n"+
                                  "__java_%2,\n"+
                                  "__java_%3);"}
                }
                ConversionRule{
                    codeClass: CodeClass.Shell
                    Text{content: "QPlaceManagerEngine*  %out = nullptr;\n"+
                                  "if(%in){\n"+
                                  "jobject __java_engine = Java::QtLocation::QGeoServiceProviderFactory$Result::engine(%env, %in);\n"+
                                  "%out = qtjambi_cast<QPlaceManagerEngine*>(%env, __java_engine);\n"+
                                  "if(%2){\n"+
                                  "jobject __java_%2 = Java::QtLocation::QGeoServiceProviderFactory$Result::error(%env, %in);\n"+
                                  "*%2 = qtjambi_cast<QGeoServiceProvider::Error>(%env, __java_%2);\n"+
                                  "}\n"+
                                  "if(%3){\n"+
                                  "jobject __java_%3 = Java::QtLocation::QGeoServiceProviderFactory$Result::errorString(%env, %in);\n"+
                                  "*%3 = qtjambi_cast<QString>(%env, __java_%3);\n"+
                                  "}\n"+
                                  "}else{\n"+
                                  "if(%2){\n"+
                                  "*%2 = QGeoServiceProvider::LoaderError;\n"+
                                  "}\n"+
                                  "if(%3){\n"+
                                  "*%3 = QString(\"No result returned.\");\n"+
                                  "}\n"+
                                  "}"}
                }
            }
            ModifyArgument{
                index: 2
                RemoveArgument{
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "QGeoServiceProvider::Error %in = QGeoServiceProvider::NoError;\n"+
                                  "QGeoServiceProvider::Error* %out = &%in;"}
                }
            }
            ModifyArgument{
                index: 3
                RemoveArgument{
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "QString %in;\n"+
                                  "QString* %out = &%in;"}
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
        ModifyFunction{
            signature: "operator=(const QGeoRoute &)"
            remove: RemoveFlag.All
        }
    }
    
    ValueType{
        name: "QGeoRouteLeg"
        ModifyFunction{
            signature: "operator=(const QGeoRouteLeg &)"
            remove: RemoveFlag.All
            since: [5, 15]
        }
    }
    
    ValueType{
        name: "QPlace"
        ModifyFunction{
            signature: "operator=(const QPlace &)"
            remove: RemoveFlag.All
        }
    }
    
    ValueType{
        name: "QPlaceIcon"
        ModifyFunction{
            signature: "operator=(const QPlaceIcon &)"
            remove: RemoveFlag.All
        }
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
    
    EnumType{
        name: "QGeoManeuver::InstructionDirection"
    }
    
    ValueType{
        name: "QGeoManeuver"
        ModifyFunction{
            signature: "operator=(const QGeoManeuver &)"
            remove: RemoveFlag.All
        }
    }
    
    ValueType{
        name: "QPlaceAttribute"
        ModifyFunction{
            signature: "operator=(const QPlaceAttribute &)"
            remove: RemoveFlag.All
        }
    }
    
    ValueType{
        name: "QPlaceCategory"
        ModifyFunction{
            signature: "operator=(const QPlaceCategory &)"
            remove: RemoveFlag.All
        }
    }
    
    ValueType{
        name: "QPlaceContactDetail"
        ModifyFunction{
            signature: "operator=(const QPlaceContactDetail &)"
            remove: RemoveFlag.All
        }
    }
    
    ValueType{
        name: "QPlaceMatchRequest"
        ModifyFunction{
            signature: "operator=(const QPlaceMatchRequest &)"
            remove: RemoveFlag.All
        }
    }
    
    EnumType{
        name: "QPlaceSearchResult::SearchResultType"
    }
    
    ValueType{
        name: "QPlaceSearchResult"
        ModifyFunction{
            signature: "operator=(const QPlaceSearchResult &)"
            remove: RemoveFlag.All
        }
    }
    
    ValueType{
        name: "QPlaceResult"
    }
    
    EnumType{
        name: "QPlaceContent::Type"
    }
    
    ValueType{
        name: "QPlaceContent"
        ModifyFunction{
            signature: "operator=(const QPlaceContent &)"
            remove: RemoveFlag.All
        }
    }
    
    ValueType{
        name: "QPlaceEditorial"
    }
    
    ValueType{
        name: "QPlaceImage"
    }
    
    ValueType{
        name: "QPlaceReview"
    }
    
    ValueType{
        name: "QPlaceContentRequest"
        ModifyFunction{
            signature: "operator=(const QPlaceContentRequest &)"
            remove: RemoveFlag.All
        }
    }
    
    ValueType{
        name: "QPlaceProposedSearchResult"
    }
    
    ValueType{
        name: "QPlaceRatings"
        ModifyFunction{
            signature: "operator=(const QPlaceRatings &)"
            remove: RemoveFlag.All
        }
    }
    
    EnumType{
        name: "QPlaceSearchRequest::RelevanceHint"
    }
    
    ValueType{
        name: "QPlaceSearchRequest"
        ModifyFunction{
            signature: "operator=(const QPlaceSearchRequest &)"
            remove: RemoveFlag.All
        }
    }
    
    ValueType{
        name: "QPlaceSupplier"
        ModifyFunction{
            signature: "operator=(const QPlaceSupplier &)"
            remove: RemoveFlag.All
        }
    }
    
    ValueType{
        name: "QPlaceUser"
        ModifyFunction{
            signature: "operator=(const QPlaceUser &)"
            remove: RemoveFlag.All
        }
    }
    
    EnumType{
        name: "QGeoRouteRequest::FeatureType"
        flags: "QGeoRouteRequest::FeatureTypes"
    }
    
    EnumType{
        name: "QGeoRouteRequest::FeatureWeight"
        flags: "QGeoRouteRequest::FeatureWeights"
    }
    
    EnumType{
        name: "QGeoRouteRequest::ManeuverDetail"
        flags: "QGeoRouteRequest::ManeuverDetails"
    }
    
    EnumType{
        name: "QGeoRouteRequest::RouteOptimization"
        flags: "QGeoRouteRequest::RouteOptimizations"
    }
    
    EnumType{
        name: "QGeoRouteRequest::SegmentDetail"
        flags: "QGeoRouteRequest::SegmentDetails"
    }
    
    EnumType{
        name: "QGeoRouteRequest::TravelMode"
        flags: "QGeoRouteRequest::TravelModes"
    }
    
    ValueType{
        name: "QGeoRouteRequest"
        ModifyFunction{
            signature: "operator=(const QGeoRouteRequest &)"
            remove: RemoveFlag.All
        }
    }
    
    ValueType{
        name: "QGeoRouteSegment"
        ModifyFunction{
            signature: "operator=(const QGeoRouteSegment &)"
            remove: RemoveFlag.All
        }
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
