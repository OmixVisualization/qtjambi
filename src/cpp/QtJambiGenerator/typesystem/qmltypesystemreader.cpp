/****************************************************************************
**
** Copyright (C) 2009-2023 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
**
** This file is part of QtJambi.
**
** $BEGIN_LICENSE$
**
** GNU Lesser General Public License Usage
** This file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Nokia gives you certain
** additional rights. These rights are described in the Nokia Qt LGPL
** Exception version 1.0, included in the file LGPL_EXCEPTION.txt in this
** package.
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

#include <QDebug>
#include <QFileInfo>
#include <QDir>
#include <QQmlEngine>
#include <QQmlComponent>

#include "qmltypesystemreader.h"
#include "typedatabase.h"
#include "../reporthandler.h"
#include "qml_loadtypesystem.h"
#include "qml_include.h"
#include "qml_injectcode.h"
#include "qml_modify.h"
#include "qml_suppressedwarning.h"
#include "qml_template.h"
#include "qml_types.h"
#include "qml_typesystem.h"

typedef TS::TypeEntry TypeEntry;
typedef TS::ComplexTypeEntry ComplexTypeEntry;
typedef TS::FunctionalTypeEntry FunctionalTypeEntry;
typedef TS::CodeSnip CodeSnip;
typedef TS::TemplateInstance TemplateInstance;
typedef TS::AbstractFunctionModification AbstractFunctionModification;
typedef TS::TemplateInstantiation TemplateInstantiation;
typedef TS::CustomFunction CustomFunction;
typedef TS::TypeDatabase TypeDatabase;
typedef TS::TypeSystemTypeEntry TypeSystemTypeEntry;
typedef TS::TemplateEntry TemplateEntry;
typedef TS::PrimitiveTypeEntry PrimitiveTypeEntry;
typedef TS::InterfaceTypeEntry InterfaceTypeEntry;
typedef TS::ObjectTypeEntry ObjectTypeEntry;
typedef TS::TemplateTypeEntry TemplateTypeEntry;
typedef TS::ArgumentModification ArgumentModification;
typedef TS::FieldModification FieldModification;
typedef TS::FunctionModification FunctionModification;
typedef TS::IteratorTypeEntry IteratorTypeEntry;
typedef TS::FlagsTypeEntry FlagsTypeEntry;
typedef TS::EnumTypeEntry EnumTypeEntry;
typedef TS::NamespaceTypeEntry NamespaceTypeEntry;
typedef TS::ValueTypeEntry ValueTypeEntry;
typedef TS::ImplementorTypeEntry ImplementorTypeEntry;
typedef TS::AliasTypeEntry AliasTypeEntry;

class QmlTypeSystemReaderPrivate {
    struct NameSpacePrefix{
        enum NamingPolicy{
            Cut,
            Preserve
        };

        QString prefix;
        QString _namespace;
        NamingPolicy namingPolicy;
    };

    QmlTypeSystemReaderPrivate(TS::TypeDatabase *database, bool generate, const QVersionNumber& qtVersion);
    void parse(const QString &filepath);
    bool checkQtVersion(AbstractObject* object);
    void parseTypeSystem(TypeSystem* typeSystem, const QString& fileName);
    void parseLoadTypeSystem(LoadTypeSystem* loadTypeSystem);
    void parseSuppressedWarning(SuppressedWarning* element);
    void parseInjectCode(InjectCode* element, ComplexTypeEntry* entry);
    void parseInjectCode(InjectCode* element, FunctionalTypeEntry* entry);
    void parseInjectCode(InjectCode* element,const QHash<CodeClass::Entries, TS::Language>& languageNames,
                         const QHash<Position::Entries, CodeSnip::Position>& positionNames,
                         const std::function<void(const QString&,const CodeSnip&)>& appendCodeSnip, bool argumentMapAllowed = false, bool allowPackage = false);
    void parseConversionRule(const QHash<CodeClass::Entries, TS::Language>& languageNames, ConversionRule* element, const std::function<void(const CodeSnip&)>& appendCodeSnip);
    void parseImportFile(ImportFile* element, const std::function<void(const QString&)>& consumeCode);
    void parseInsertTemplate(InsertTemplate* element, const std::function<void(TemplateInstance*)>& consumeTemplateInstance);
    void parseExtraIncludes(ExtraIncludes* element, TypeEntry* entry);
    TS::Include parseInclude(Include* element);
    void parseRejection(Rejection* element);
    void parseTemplate(Template* element);
    void parseTemplateArguments(TemplateArguments* element, ComplexTypeEntry* entry);
    void parsePrimitiveType(const QString& nameSpace, PrimitiveType* element);
    void parseObjectType(const QString& nameSpace, ObjectType* element);
    void parseAttributesOfComplexType(ComplexType* element, ComplexTypeEntry* entry);
    QList<AbstractObject*> parseChildrenOfComplexType(const QString& nameSpace, ComplexType* element, ComplexTypeEntry* entry);
    void parseTemplateType(const QString& nameSpace, TemplateType* element);
    void parseFunctionalType(const QString& nameSpace, FunctionalType* element);
    void parseIteratorType(const QString& nameSpace, IteratorType* element);
    void parseValueType(const QString& nameSpace, ValueType* element);
    void parseTypeAliasType(const QString& nameSpace, TypeAliasType* element);
    void parseInterfaceType(const QString& nameSpace, InterfaceType* element);
    void parseNamespaceType(const QString& nameSpace, NamespaceType* element);
    void parseModifyArgument(ModifyArgument* element, AbstractFunctionModification& mod);
    void parseDelegate(Delegate* element, AbstractFunctionModification& mod);
    void parseModifyFunction(ModifyFunction* element, TypeEntry* entry);
    TemplateInstantiation parseInstantiation(Instantiation* element);
    void parseModifyField(ModifyField* element, ComplexTypeEntry* entry);
    void parseEnumType(const QString& nameSpace, EnumType* element);
    CustomFunction parseCustomStructor(AbstractStructor* element);

    TypeDatabase *m_database;
    QVersionNumber m_qtVersion;
    QString m_defaultPackage;
    QString m_defaultPPCondition;
    QString m_defaultSuperclass;
    QStringList m_importInputDirectoryList;
    QStringList m_typesystemDirectoryList;
    TypeEntry::CodeGeneration m_generate;
    QList<NameSpacePrefix> m_namespacePrefixes;
    QMultiMap<TypeSystemTypeEntry*,QString> m_requiredModules;

    friend QmlTypeSystemReader;
};

QmlTypeSystemReader::QmlTypeSystemReader(TS::TypeDatabase *database, bool generate, const QVersionNumber& qtVersion)
        : d(new QmlTypeSystemReaderPrivate(database, generate, qtVersion)) {}

QmlTypeSystemReader::~QmlTypeSystemReader(){delete d;}

void QmlTypeSystemReader::setImportInputDirectoryList(const QStringList &importInputDirectoryList) {
    d->m_importInputDirectoryList = importInputDirectoryList;
}

void QmlTypeSystemReader::setTypesystemsDirectoryList(const QStringList &typesystemDirectoryList) {
    d->m_typesystemDirectoryList = typesystemDirectoryList;
}

void QmlTypeSystemReader::parse(const QString &filepath){
    d->parse(filepath);
}

void QmlTypeSystemReaderPrivate::parse(const QString &filepath){
    QQmlEngine engine;
    QUrl url;
    if(filepath.startsWith(":")){
        url = QUrl("qrc"+filepath);
    }else{
        QFileInfo file(filepath);
        url = QUrl::fromLocalFile(file.absoluteFilePath());
    }
    QQmlComponent component(&engine, url, QQmlComponent::PreferSynchronous);
    if(component.status()==QQmlComponent::Ready){
        QObject* obj = component.create();
        if(!obj->parent())
            obj->setParent(&engine);
        if(TypeSystem* typeSystem = qobject_cast<TypeSystem*>(obj)){
            parseTypeSystem(typeSystem, QFileInfo(filepath).fileName());
            for(TypeSystemTypeEntry* entry : m_requiredModules.keys()){
                for(const QString& libraryName : m_requiredModules.values(entry)){
                    if(TypeSystemTypeEntry* ts = TypeDatabase::instance()->findTypeSystem(libraryName)){
                        entry->addRequiredTypeSystem(ts);
                    }else{
                        TypesystemException::raise(QString("Unable to find required package %1 (%2)").arg(libraryName, url.toString()));
                    }
                }
            }
            return;
        }else if(obj){
            TypesystemException::raise(QString("QML Error: %1 defined in %2. Expected: TypeSystem. %2").arg(obj->metaObject()->className(), url.toString(), component.errorString()));
        }else{
            TypesystemException::raise(QString("QML Error: No TypeSystem defined in %1 %2").arg(url.toString(), component.errorString()));
        }
    }else{
        TypesystemException::raise(QString("QML Error: %1 in %2").arg(component.errorString(), url.toString()));
    }
}

QmlTypeSystemReaderPrivate::QmlTypeSystemReaderPrivate(TS::TypeDatabase *database, bool generate, const QVersionNumber& qtVersion)
        : m_database(database),
          m_qtVersion(qtVersion),
          m_generate(generate ? TypeEntry::GenerateAll : TypeEntry::GenerateForSubclass) {
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    static bool r1 = [](){return (QMetaType::registerConverter<int,AccessModifications>([](int i)->AccessModifications{return AccessModifications(i);}),
                                  QMetaType::registerConverter<AccessModifications,int>());}();
    Q_UNUSED(r1)
#endif
}

void QmlTypeSystemReaderPrivate::parseTypeSystem(TypeSystem* typeSystem, const QString& fileName){
    try{
        if (checkQtVersion(typeSystem)){
            QList<NameSpacePrefix> currentNamespacePrefixes = m_namespacePrefixes;
            m_namespacePrefixes.clear();
            m_defaultPackage = typeSystem->getPackageName();
            m_defaultSuperclass = typeSystem->getDefaultSuperClass();
            m_defaultPPCondition = typeSystem->getDefaultPPCondition();
            bool noExports = typeSystem->getNoExports();
            QString moduleName = typeSystem->getModule();
            QString description = typeSystem->getDescription();
            const QList<AbstractObject*>& childrenList = typeSystem->childrenList();
            for(int i=0; i<childrenList.size(); ++i){
                AbstractObject* item = childrenList[i];
                if(Module* mod = qobject_cast<Module*>(item)){
                    if (checkQtVersion(mod)){
                        if(!moduleName.isEmpty())
                            TypesystemException::raise(QStringLiteral("Duplicate module name specification"));
                        moduleName = mod->getName();
                    }
                }else if(Description* desc = qobject_cast<Description*>(item)){
                    if (checkQtVersion(desc)){
                        if(!description.isEmpty())
                            TypesystemException::raise(QStringLiteral("Duplicate description specification"));
                        description = desc->getText();
                    }
                }
            }
            TypeSystemTypeEntry* entry = new TypeSystemTypeEntry(m_defaultPackage,
                                                                 typeSystem->getQtLibrary(),
                                                                 moduleName);
            entry->setTargetName(typeSystem->getTargetName());
            entry->setNoExports(noExports);
            entry->setDescription(description);
            entry->setCodeGeneration(m_generate);
            //qDebug()<<"Adding element->entry (root)"<<element->entry->name();
            ReportHandler::debugTypes("Adding to TypeDatabase(3): " + entry->name());
            TypeDatabase::instance()->addType(entry);
            for(int i=0; i<childrenList.size(); ++i){
                AbstractObject* item = childrenList[i];
                if(Module* mod = qobject_cast<Module*>(item)){
                    // already handled above
                }else if(Description* desc = qobject_cast<Description*>(item)){
                    // already handled above
                }else if(RequiredLibrary* reqLib = qobject_cast<RequiredLibrary*>(item)){
                    if (checkQtVersion(reqLib)){
                        QString libraryName = reqLib->getName();
                        QStringList platforms = reqLib->getPlatforms();
                        if(libraryName.isEmpty())
                            ReportHandler::warning("required-library with no name specified");
                        entry->addRequiredQtLibrary(std::move(libraryName), TS::Dependency::Mode(reqLib->getMode()), std::move(platforms));
                    }
                }else if(RequiredPackage* reqPkg = qobject_cast<RequiredPackage*>(item)){
                    if (checkQtVersion(reqPkg)){
                        const QString& libraryName = reqPkg->getName();
                        if(libraryName.isEmpty())
                            ReportHandler::warning("required-module with no name specified");
                        if(TypeSystemTypeEntry* ts = TypeDatabase::instance()->findTypeSystem(libraryName)){
                            entry->addRequiredTypeSystem(ts);
                        }else{
                            m_requiredModules.insert(entry, libraryName);
                        }
                    }
                }else if(ForwardDeclaration* forwardDeclaration = qobject_cast<ForwardDeclaration*>(item)){
                    if (checkQtVersion(forwardDeclaration)){
                        QString className = forwardDeclaration->getClassName();
                        if(className.isEmpty())
                            ReportHandler::warning("forward-declaration with no class specified");
                        entry->addForwardDeclaration(className);
                    }
                }else if(NamespacePrefix* namespacePrefix = qobject_cast<NamespacePrefix*>(item)){
                    if (checkQtVersion(namespacePrefix)){
                        QString prefix = namespacePrefix->getPrefix();
                        QString _namespace = namespacePrefix->getNamespace();
                        if(!prefix.isEmpty() && !prefix.endsWith("::"))
                            prefix += "::";
                        if(!_namespace.isEmpty() && !_namespace.endsWith("::"))
                            _namespace += "::";
                        m_namespacePrefixes.append({prefix, _namespace, NameSpacePrefix::NamingPolicy(namespacePrefix->getNamingPolicy())});
                    }
                }else if(LoadTypeSystem* loadTypeSystem = qobject_cast<LoadTypeSystem*>(item)){
                    parseLoadTypeSystem(loadTypeSystem);
                }else if(Rejection* childElement = qobject_cast<Rejection*>(item)){
                    parseRejection(childElement);
                }else if(PrimitiveType* childElement = qobject_cast<PrimitiveType*>(item)){
                    parsePrimitiveType({}, childElement);
                }else if(ObjectType* childElement = qobject_cast<ObjectType*>(item)){
                    parseObjectType({}, childElement);
                }else if(TemplateType* childElement = qobject_cast<TemplateType*>(item)){
                    parseTemplateType({}, childElement);
                }else if(FunctionalType* childElement = qobject_cast<FunctionalType*>(item)){
                    parseFunctionalType({}, childElement);
                }else if(IteratorType* childElement = qobject_cast<IteratorType*>(item)){
                    parseIteratorType({}, childElement);
                }else if(ValueType* childElement = qobject_cast<ValueType*>(item)){
                    parseValueType({}, childElement);
                }else if(TypeAliasType* childElement = qobject_cast<TypeAliasType*>(item)){
                    parseTypeAliasType({}, childElement);
                }else if(InterfaceType* childElement = qobject_cast<InterfaceType*>(item)){
                    parseInterfaceType({}, childElement);
                }else if(NamespaceType* childElement = qobject_cast<NamespaceType*>(item)){
                    parseNamespaceType({}, childElement);
                }else if(EnumType* childElement = qobject_cast<EnumType*>(item)){
                    parseEnumType({}, childElement);
                }else if(ExtraIncludes* childElement = qobject_cast<ExtraIncludes*>(item)){
                    parseExtraIncludes(childElement, entry);
                }else if(InjectCode* childElement = qobject_cast<InjectCode*>(item)){
                    static const QHash<CodeClass::Entries, TS::Language> languageNames{
                        {CodeClass::Java, TS::TargetLangCode},
                        {CodeClass::Native, TS::NativeCode},
                        {CodeClass::ModuleInfo, TS::ModuleInfo},
                        {CodeClass::PackageInitializer, TS::PackageInitializer},
                        {CodeClass::MetaInfo, TS::MetaInfo},
                    };

                    static const QHash<Position::Entries, CodeSnip::Position> positionNames{
                        {Position::Beginning, CodeSnip::Beginning},
                        {Position::Position1, CodeSnip::Position1},
                        {Position::Position2, CodeSnip::Position2},
                        {Position::Position3, CodeSnip::Position3},
                        {Position::Position4, CodeSnip::Position4},
                        {Position::Position5, CodeSnip::Position5},
                        {Position::Comment, CodeSnip::Comment},
                        {Position::End, CodeSnip::End}
                    };
                    parseInjectCode(childElement, languageNames, positionNames, [entry](const QString& subTypeSystem, const CodeSnip &snip){
                        entry->addCodeSnip(subTypeSystem, snip);
                    }, false, true);
                }else if(SuppressedWarning* childElement = qobject_cast<SuppressedWarning*>(item)){
                    parseSuppressedWarning(childElement);
                }else if(Template* childElement = qobject_cast<Template*>(item)){
                    parseTemplate(childElement);
                }else if(ModifyFunction* childElement = qobject_cast<ModifyFunction*>(item)){
                    parseModifyFunction(childElement, entry);
                }else{
                    TypesystemException::raise(QStringLiteral("Unexpected element %1 as child of element %2").arg(item->metaObject()->className(), typeSystem->metaObject()->className()));
                }
            }
            m_namespacePrefixes = currentNamespacePrefixes;
        }
    }catch(const TypesystemException& exn){
        TypesystemException::raise(QString("%1 in file %2").arg(QLatin1String(exn.what()), fileName));
    }
}

bool QmlTypeSystemReaderPrivate::checkQtVersion(AbstractObject* object){
    if(object->since()<=m_qtVersion && object->until()>=m_qtVersion){
        return object->getUnless().isEmpty() || !TypeDatabase::instance()->defined(object->getUnless());
    }else{
        return false;
    }
}

void QmlTypeSystemReaderPrivate::parseSuppressedWarning(SuppressedWarning* element){
    if (checkQtVersion(element)){
        if (element->getText().isEmpty())
            ReportHandler::warning("Suppressed warning with no text specified");
        else
            m_database->addSuppressedWarning(element->getText());
        for(AbstractObject* item : element->childrenList()){
            TypesystemException::raise(QStringLiteral("Unexpected element %1 as child of %2").arg(item->metaObject()->className(), element->metaObject()->className()));
        }
    }
}

TS::Include QmlTypeSystemReaderPrivate::parseInclude(Include* element){
    TS::Include incl;
    if (checkQtVersion(element)){
        QString fileName = element->getFileName();
        QString ppCondition = element->getPpCondition();
        incl.inherited = element->getInherited();
        incl.suppressed = element->getSuppressed();
        incl.ckeckAvailability = element->getCkeckAvailability();
        for(const QString& feature : element->getRequiredFeatures()){
            if(!feature.isEmpty())
                incl.requiredFeatures[feature] = "";
        }

        static const QHash<Include::IncludeType, TS::Include::IncludeType> locationNames{
            {Include::Global, TS::Include::IncludePath},
            {Include::Local, TS::Include::LocalPath},
            {Include::Java, TS::Include::TargetLangImport}
        };

        if (!locationNames.contains(element->getLocation())) {
            TypesystemException::raise(QString("Location not recognized: '%1' referred by Include").arg(element->getLocation()));
        }
        incl.type = locationNames[element->getLocation()];
        incl.name = fileName;
        for(AbstractObject* item : element->childrenList()){
            TypesystemException::raise(QStringLiteral("Unexpected element %1 as child of %2").arg(item->metaObject()->className(), element->metaObject()->className()));
        }
    }
    return incl;
}

void QmlTypeSystemReaderPrivate::parseExtraIncludes(ExtraIncludes* element, TypeEntry* entry){
    if (checkQtVersion(element)){
        const QList<AbstractObject*>& childrenList = element->childrenList();
        for(int i=0; i<childrenList.size(); ++i){
            AbstractObject* item = childrenList[i];
            if(Include* childElement = qobject_cast<Include*>(item)){
                TS::Include inc = parseInclude(childElement);
                if(inc.isValid()){
                    if(entry->isTypeSystem()){
                        TypeSystemTypeEntry *ttype = static_cast<TypeSystemTypeEntry *>(entry);
                        ttype->addExtraInclude(inc);
                    }else if(entry->isFunctional()){
                        FunctionalTypeEntry *ctype = static_cast<FunctionalTypeEntry *>(entry);
                        ctype->addExtraInclude(inc);
                    }else{
                        ComplexTypeEntry *ctype = static_cast<ComplexTypeEntry *>(entry);
                        ctype->addExtraInclude(inc);
                        if(ctype->designatedInterface())
                            ctype->designatedInterface()->addExtraInclude(inc);
                    }
                }
            }else{
                TypesystemException::raise(QStringLiteral("Unexpected element %1 as child of %2").arg(item->metaObject()->className(), element->metaObject()->className()));
            }
        }
    }
}

void QmlTypeSystemReaderPrivate::parseTemplateArguments(TemplateArguments* element, ComplexTypeEntry* entry){
    if (checkQtVersion(element)){
        entry->addInstantiation(element->getArguments());
        for(AbstractObject* item : element->childrenList()){
            TypesystemException::raise(QStringLiteral("Unexpected element %1 as child of %2").arg(item->metaObject()->className(), element->metaObject()->className()));
        }
    }
}

void QmlTypeSystemReaderPrivate::parseTemplate(Template* element){
    if (checkQtVersion(element)){
        QString name = element->getName();
        std::unique_ptr<TemplateEntry> entry(new TemplateEntry(name));
        const QList<AbstractObject*>& childrenList = element->childrenList();
        for(int i=0; i<childrenList.size(); ++i){
            AbstractObject* item = childrenList[i];
            if(InsertTemplate* childElement = qobject_cast<InsertTemplate*>(item)){
                parseInsertTemplate(childElement, [&entry](TemplateInstance* templateInstance){entry->addTemplateInstance(templateInstance);});
            }else if(ImportFile* childElement = qobject_cast<ImportFile*>(item)){
                parseImportFile(childElement, [&entry](const QString& code){entry->addCode(code);});
            }else if(Text* childElement = qobject_cast<Text*>(item)){
                if (checkQtVersion(childElement)){
                    entry->addCode(childElement->getContent());
                    for(AbstractObject* item2 : item->childrenList()){
                        TypesystemException::raise(QStringLiteral("Unexpected element %1 as child of %2").arg(item2->metaObject()->className(), item->metaObject()->className()));
                    }
                }
            }else{
                TypesystemException::raise(QStringLiteral("Unexpected element %1 as child of Template").arg(item->metaObject()->className()));
            }
        }
        m_database->addTemplate(entry.release());
    }
}

void QmlTypeSystemReaderPrivate::parseInjectCode(InjectCode* element, ComplexTypeEntry* entry){
    static const QHash<CodeClass::Entries, TS::Language> languageNames{
        {CodeClass::Java, TS::TargetLangCode},
        {CodeClass::Native, TS::NativeCode},
        {CodeClass::ModuleInfo, TS::ModuleInfo},
        {CodeClass::PackageInitializer, TS::PackageInitializer},
        {CodeClass::MetaInfo, TS::MetaInfo},
        {CodeClass::Shell, TS::ShellCode},
        {CodeClass::ShellDeclaration, TS::ShellDeclaration},
        {CodeClass::DestructorFunction, TS::DestructorFunction},
        {CodeClass::DeleterFunction, TS::DeleterFunction},
        {CodeClass::Constructors, TS::Constructors},
        {CodeClass::Signal, TS::Signal}
    };

    static const QHash<Position::Entries, CodeSnip::Position> positionNames{
        {Position::Beginning, CodeSnip::Beginning},
        {Position::Position1, CodeSnip::Position1},
        {Position::Position2, CodeSnip::Position2},
        {Position::Position3, CodeSnip::Position3},
        {Position::Position4, CodeSnip::Position4},
        {Position::Position5, CodeSnip::Position5},
        {Position::Equals, CodeSnip::Equals},
        {Position::Compare, CodeSnip::Compare},
        {Position::HashCode, CodeSnip::HashCode},
        {Position::ToString, CodeSnip::ToString},
        {Position::Clone, CodeSnip::Clone},
        {Position::Comment, CodeSnip::Comment},
        {Position::End, CodeSnip::End}
    };
    if(entry->isInterface() || entry->designatedInterface()){
        static const QHash<CodeClass::Entries, TS::Language> _languageNames = [](const QHash<CodeClass::Entries, TS::Language>& names)->QHash<CodeClass::Entries, TS::Language>{
            QHash<CodeClass::Entries, TS::Language> result{{CodeClass::JavaInterface, TS::Interface}};
            result.insert(names);
            return result;
        }(languageNames);
        if(entry->designatedInterface()){
            return parseInjectCode(element, _languageNames, positionNames, [&entry](const QString&,const CodeSnip &snip){entry->addCodeSnip(snip); entry->designatedInterface()->addCodeSnip(snip);});
        }else{
            return parseInjectCode(element, _languageNames, positionNames, [&entry](const QString&,const CodeSnip &snip){entry->addCodeSnip(snip);});
        }
    }else{
        return parseInjectCode(element, languageNames, positionNames, [&entry](const QString&,const CodeSnip &snip){entry->addCodeSnip(snip);});
    }
}

void QmlTypeSystemReaderPrivate::parseInjectCode(InjectCode* element, FunctionalTypeEntry* entry){
    static const QHash<CodeClass::Entries, TS::Language> languageNames{
        {CodeClass::Java, TS::TargetLangCode},
        {CodeClass::Native, TS::NativeCode},
        {CodeClass::ModuleInfo, TS::ModuleInfo},
        {CodeClass::PackageInitializer, TS::PackageInitializer},
        {CodeClass::MetaInfo, TS::MetaInfo},
        {CodeClass::Shell, TS::ShellCode},
        {CodeClass::ShellDeclaration, TS::ShellDeclaration},
        {CodeClass::DestructorFunction, TS::DestructorFunction},
        {CodeClass::DeleterFunction, TS::DeleterFunction},
        {CodeClass::Constructors, TS::Constructors},
        {CodeClass::Signal, TS::Signal},
        {CodeClass::JavaInterface, TS::Interface}
    };

    static const QHash<Position::Entries, CodeSnip::Position> positionNames{
        {Position::Beginning, CodeSnip::Beginning},
        {Position::Position1, CodeSnip::Position1},
        {Position::Position2, CodeSnip::Position2},
        {Position::Position3, CodeSnip::Position3},
        {Position::Position4, CodeSnip::Position4},
        {Position::Position5, CodeSnip::Position5},
        {Position::Equals, CodeSnip::Equals},
        {Position::Compare, CodeSnip::Compare},
        {Position::HashCode, CodeSnip::HashCode},
        {Position::ToString, CodeSnip::ToString},
        {Position::Clone, CodeSnip::Clone},
        {Position::Comment, CodeSnip::Comment},
        {Position::End, CodeSnip::End}
    };
    return parseInjectCode(element, languageNames, positionNames, [&entry](const QString&,const CodeSnip &snip){entry->addCodeSnip(snip);});
}

void QmlTypeSystemReaderPrivate::parseInjectCode(InjectCode* element,const QHash<CodeClass::Entries, TS::Language>& languageNames,
                              const QHash<Position::Entries, CodeSnip::Position>& positionNames,
                              const std::function<void(const QString&,const CodeSnip&)>& appendCodeSnip, bool argumentMapAllowed, bool allowPackage){
    if (checkQtVersion(element)){
        QString subTypeSystem;
        if(allowPackage)
            subTypeSystem = element->getPackageName();
        auto className = element->getTarget();
        auto position = element->getPosition();
        CodeSnip snip;
        snip.language = languageNames[className];
        snip.position = positionNames[position];
        if((snip.position==CodeSnip::Equals
             || snip.position==CodeSnip::Compare
             || snip.position==CodeSnip::HashCode) && snip.language!=TS::TargetLangCode){
            QMetaEnum metaEnum1 = Position::staticMetaObject.enumerator(Position::staticMetaObject.indexOfEnumerator("Entries"));
            QMetaEnum metaEnum2 = CodeClass::staticMetaObject.enumerator(CodeClass::staticMetaObject.indexOfEnumerator("Entries"));
            TypesystemException::raise(QString("InjectCode{position: Position.%1; target: CodeClass.%2} invalid property combination for code injection").arg(metaEnum1.valueToKey(int(position)), metaEnum2.valueToKey(int(className))));
        }

        const QList<AbstractObject*>& childrenList = element->childrenList();
        for(int i=0; i<childrenList.size(); ++i){
            AbstractObject* item = childrenList[i];
            ArgumentMap* map;
            if(argumentMapAllowed && (map = qobject_cast<ArgumentMap*>(item))){
                if (checkQtVersion(map)){
                    int pos = map->getIndex();
                    if (pos < 0) {
                        TypesystemException::raise(QString("Argument index must be a positive number"));
                    }
                    QString meta_name = map->getMetaName();
                    if (meta_name.isEmpty()) {
                        TypesystemException::raise(QString("Argument meta-name must not be empty"));
                    }
                    snip.argumentMap[pos] = meta_name;
                    for(AbstractObject* item2 : item->childrenList()){
                        TypesystemException::raise(QStringLiteral("Unexpected element %1 as child of %2").arg(item2->metaObject()->className(), item->metaObject()->className()));
                    }
                }
            }else if(InsertTemplate* childElement = qobject_cast<InsertTemplate*>(item)){
                parseInsertTemplate(childElement, [&snip](TemplateInstance* templateInstance){snip.addTemplateInstance(templateInstance);});
            }else if(ImportFile* childElement = qobject_cast<ImportFile*>(item)){
                parseImportFile(childElement, [&snip](const QString& code){snip.addCode(code);});
            }else if(Text* childElement = qobject_cast<Text*>(item)){
                snip.addCode(childElement->getContent());
            }else{
                TypesystemException::raise(QString("Unexpected tag %2").arg(item->metaObject()->className()));
            }
        }
        appendCodeSnip(subTypeSystem, snip);
    }
}

void QmlTypeSystemReaderPrivate::parseInsertTemplate(InsertTemplate* element, const std::function<void(TemplateInstance*)>& consumeTemplateInstance){
    if (checkQtVersion(element)){
        QString name = element->getName();
        std::unique_ptr<TemplateInstance> templateInstance(new TemplateInstance(name));
        const QList<AbstractObject*>& childrenList = element->childrenList();
        for(int i=0; i<childrenList.size(); ++i){
            AbstractObject* item = childrenList[i];
            if(Replace* childElement = qobject_cast<Replace*>(item)){
                if (checkQtVersion(childElement)){
                    templateInstance->addReplaceRule(childElement->getFrom(), childElement->getTo());
                    for(AbstractObject* item2 : item->childrenList()){
                        TypesystemException::raise(QStringLiteral("Unexpected element %1 as child of %2").arg(item2->metaObject()->className(), item->metaObject()->className()));
                    }
                }
            }
        }
        consumeTemplateInstance(templateInstance.release());
    }
}

void QmlTypeSystemReaderPrivate::parseImportFile(ImportFile* element, const std::function<void(const QString&)>& consumeCode){
    if (checkQtVersion(element)){
        QString fileName = element->getName();
        if (fileName.isEmpty()) {
            TypesystemException::raise(QString("Required attribute 'name' missing for element ImportFile"));
        }

        QFile file(fileName);
        QFileInfo fileinfo(file);
        if (fileinfo.isRelative() && !m_importInputDirectoryList.isEmpty()) {
            // Resolve
            QString filepath = resolveFilePath(fileName, 0, m_importInputDirectoryList);
            if(filepath.isNull()) {
                TypesystemException::raise(QString("Could not resolve file path '%1'").arg(fileName));
            }
            file.setFileName(filepath);
        }
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
#if 0
            // This feature is disabled, better to use actual files
            file.setFileName(":/io/qt/generator/" + fileName);
            if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
#endif
                TypesystemException::raise(QString("Could not open file '%1'").arg(fileName));
#if 0
            }
#endif
        }

        QString quoteFrom = element->getQuoteAfterLine();
        bool foundFromOk = quoteFrom.isEmpty();
        bool from = quoteFrom.isEmpty();

        QString quoteTo = element->getQuoteBeforeLine();
        bool foundToOk = quoteTo.isEmpty();
        bool to = true;

        QTextStream in(&file);
        QStringList lines;
        while (!in.atEnd()) {
            QString line = in.readLine();
            if (from && to && line.contains(quoteTo)) {
                to = false;
                foundToOk = true;
                break;
            }
            if (from && to){
                lines << line;
            }
            if (!from && line.contains(quoteFrom)) {
                from = true;
                foundFromOk = true;
            }
        }
        while(!lines.isEmpty()){
            if(lines.last().trimmed().isEmpty()){
                lines.takeLast();
            }else{
                break;
            }
        }
        while(!lines.isEmpty()){
            if(lines.first().trimmed().isEmpty()){
                lines.takeFirst();
            }else{
                break;
            }
        }

        if(!lines.isEmpty()){
            int sp = 0;
            for(; sp<lines[0].length(); ++sp){
                if(lines[0][sp]!=QLatin1Char(' ')){
                    break;
                }
            }
            QString spaces;
            if(sp==0){
                sp = 0;
                for(; sp<lines[0].length(); ++sp){
                    if(lines[0][sp]!=QLatin1Char('\t')){
                        break;
                    }
                }
                spaces.fill(QLatin1Char('\t'), sp);
            }else{
                spaces.fill(QLatin1Char(' '), sp);
            }
            for(QString& line : lines){
                if(line.startsWith(spaces))
                    line = line.mid(sp);
            }
            consumeCode(lines.join("\n") + "\n");
        }
        if (!foundFromOk || !foundToOk) {
            QString fromError = QString("Could not find quote-after-line='%1' in file '%2'.").arg(quoteFrom).arg(fileName);
            QString toError = QString("Could not find quote-before-line='%1' closing quote-after-line='%2' in file '%3'.").arg(quoteTo).arg(quoteFrom).arg(fileName);

            if (!foundToOk)
                TypesystemException::raise(toError);
            if (!foundFromOk)
                TypesystemException::raise(fromError);
            if (!foundFromOk && !foundToOk)
                TypesystemException::raise(QString("%1 %2").arg(fromError, toError));

        }

    }
}

void QmlTypeSystemReaderPrivate::parseLoadTypeSystem(LoadTypeSystem* loadTypeSystem){
    if (checkQtVersion(loadTypeSystem)){
        for(AbstractObject* item : loadTypeSystem->childrenList()){
            TypesystemException::raise(QStringLiteral("Unexpected element %1 as child of %2").arg(item->metaObject()->className(), loadTypeSystem->metaObject()->className()));
        }
        QString name = loadTypeSystem->getName();
        if (name.isEmpty()) {
            TypesystemException::raise(QString("No typesystem name specified"));
        }
        bool generated = loadTypeSystem->getGenerate() && m_generate==TypeEntry::GenerateAll;
        bool optional = loadTypeSystem->getOptional();
        m_database->parseFile(name, m_importInputDirectoryList, m_typesystemDirectoryList, generated, optional);
    }
}

void QmlTypeSystemReaderPrivate::parseRejection(Rejection* element){
    if (checkQtVersion(element)){
        QString cls = element->getClassName();
        QString function = element->getFunctionName();
        QString field = element->getFieldName();
        QString enum_ = element->getEnumName();
        if (cls == "*" && function.isEmpty() && field.isEmpty() && enum_.isEmpty()) {
            TypesystemException::raise(QString("bad reject entry, neither 'className', 'functionName' nor 'fieldName'"));
        }
        m_database->addRejection(cls, function, field, enum_);
        for(AbstractObject* item2 : element->childrenList()){
            TypesystemException::raise(QStringLiteral("Unexpected element %1 as child of %2").arg(item2->metaObject()->className(), element->metaObject()->className()));
        }
    }
}

void QmlTypeSystemReaderPrivate::parsePrimitiveType(const QString& nameSpace, PrimitiveType* element){
    if (checkQtVersion(element)){
        QString name = element->getName();
        if(!nameSpace.isEmpty())
            name = nameSpace+"::"+name;
        try{
            if (name.isEmpty()) {
                TypesystemException::raise(QString("No 'name' attribute specified for element PrimitiveType"));
            }
            QString java_name = element->getJavaName();
            QString jni_name = element->getJniName();
            if (java_name.isEmpty())
                java_name = name;
            if (jni_name.isEmpty())
                jni_name = name;
            PrimitiveTypeEntry *entry = new PrimitiveTypeEntry(name, m_defaultPackage);
            entry->setCodeGeneration(m_generate);
            entry->setTargetLangName(java_name);
            entry->setJniName(jni_name);

            entry->setPreferredConversion(element->getPreferredConversion());
            entry->setPreferredTargetLangType(element->getPreferredJavaType());

            ReportHandler::debugTypes("Adding to TypeDatabase(2): " + entry->name());
            m_database->addType(entry);
            for(AbstractObject* item2 : element->childrenList()){
                TypesystemException::raise(QStringLiteral("Unexpected element %1 as child of %2").arg(item2->metaObject()->className(), element->metaObject()->className()));
            }
        }catch(const TypesystemException& exn){
            TypesystemException::raise(QString("%1 of type %2").arg(QLatin1String(exn.what()), name));
        }
    }
}

void QmlTypeSystemReaderPrivate::parseAttributesOfComplexType(ComplexType* element, ComplexTypeEntry* ctype){
    QString package = element->getPackageName().isEmpty() ? m_defaultPackage : element->getPackageName();
    QString ppCondition = element->getPpCondition().isEmpty() ? m_defaultPPCondition : element->getPpCondition();
    QString implements = element->getImplementing();
    QString _using = element->getUsing();
    QString javaName = element->getJavaName();
    QString defaultSuperclass = element->getDefaultSuperClass();
    QString extendType = element->getExtendType();

    if(defaultSuperclass.isEmpty())
        defaultSuperclass = m_defaultSuperclass;
    ctype->setPPCondition(ppCondition);
    if(!ppCondition.isEmpty()){
        TS::Include incl = ctype->include();
        if(incl.isValid()){
            incl.ckeckAvailability = true;
            ctype->setInclude(incl);
            if(ctype->designatedInterface())
                ctype->designatedInterface()->setInclude(incl);
        }
    }
    QString targetType = element->getTargetType();
    if (!targetType.isEmpty()){
        ctype->setTargetType(targetType);
    }
    ctype->setSkipMetaTypeRegistration(element->getNoMetaType());
    if (element->getDisableNativeIdUsage())
        ctype->disableNativeIdUsage();
    if (element->getForceAbstract())
        ctype->setForceAbstract();
    if (element->getForceFriendly())
        ctype->setForceFriendly();
    if (element->getDeprecated())
        ctype->setDeprecated();
    ctype->setNoImplicitConstructors(element->getNoImplicitConstructors());
    if(!ctype->isNamespace() && !ctype->isIterator()){
        QString threadAffine = element->getThreadAffinity();
        if (!threadAffine.trimmed().isEmpty()){
            ctype->setThreadAffine();
            ctype->setThreadAffinity(threadAffine);
        }
    }

    QVariant generate = element->getGenerate();

    if(element->getIsNativeInterface()){
        ctype->setGenericClass(element->getIsGeneric());
        ctype->setNativeInterface(true);
        if(m_generate==TypeEntry::GenerateAll){
            if((generate.userType()==QMetaType::QString && generate.value<QString>()=="no-shell") || (generate.userType()==QMetaType::Bool && generate.value<bool>())){
                ctype->setCodeGeneration(TypeEntry::GenerateNoShell | TypeEntry::GenerateAll);
            }else{
                ctype->setCodeGeneration(TypeEntry::GenerateForSubclass);
            }
        }else
            ctype->setCodeGeneration(m_generate);
    }else if(element->getTemplate()){
        ctype->setTemplate(true);
        ctype->setGenericClass(true);
        if(m_generate==TypeEntry::GenerateAll)
            ctype->setCodeGeneration(TypeEntry::GenerateForSubclass);
        else
            ctype->setCodeGeneration(m_generate);
    }else{
        ctype->setGenericClass(element->getIsGeneric());
        if(m_generate==TypeEntry::GenerateAll){
            if(generate.userType()==QMetaType::QString && generate.value<QString>()=="no-shell"){
                ctype->setCodeGeneration(TypeEntry::GenerateNoShell | TypeEntry::GenerateAll);
            }else if (generate.userType()==QMetaType::Bool && !generate.value<bool>())
                ctype->setCodeGeneration(TypeEntry::GenerateForSubclass);
            else
                ctype->setCodeGeneration(m_generate);
        }else
            ctype->setCodeGeneration(m_generate);
    }
    ctype->setTargetTypeSystem(m_defaultPackage);
    ctype->setTargetLangPackage(package);
    ctype->setDefaultSuperclass(defaultSuperclass);
    ctype->setImplements(implements);
    if(ctype->designatedInterface()){
        if (!targetType.isEmpty())
            ctype->designatedInterface()->setTargetType(targetType);
        ctype->designatedInterface()->setGenericClass(ctype->isGenericClass());
        ctype->designatedInterface()->setTargetTypeSystem(m_defaultPackage);
        ctype->designatedInterface()->setTargetLangPackage(package);
        ctype->designatedInterface()->setDefaultSuperclass(defaultSuperclass);
        ctype->designatedInterface()->setImplements(implements);
        if (ctype->isForceAbstract())
            ctype->designatedInterface()->setForceAbstract();
        if (ctype->isForceFriendly())
            ctype->designatedInterface()->setForceFriendly();
        if (ctype->isDeprecated())
            ctype->designatedInterface()->setDeprecated();
        if(ctype->isThreadAffine())
            ctype->designatedInterface()->setThreadAffine();
        ctype->designatedInterface()->setThreadAffinity(ctype->threadAffinity());
        ctype->designatedInterface()->setPPCondition(ctype->ppCondition());
        ctype->designatedInterface()->setNativeInterface(ctype->isNativeInterface());
        ctype->designatedInterface()->setCodeGeneration(ctype->codeGeneration());
    }

    if (!javaName.isEmpty()){
        if(ctype->designatedInterface()){
            ctype->designatedInterface()->setTargetLangName(javaName);
            ctype->setTargetLangName(InterfaceTypeEntry::implName(javaName).replace("::", "$"));
        }else{
            ctype->setTargetLangName(javaName);
        }
    }
    ctype->setExtendType(extendType);
    if(!extendType.isEmpty()){
        ctype->setCodeGeneration(TypeEntry::GenerateForSubclass);
    }
}

void QmlTypeSystemReaderPrivate::parseObjectType(const QString& nameSpace, ObjectType* element){
    if (checkQtVersion(element)){
        QString name = element->getName();
        if(!nameSpace.isEmpty())
            name = nameSpace+"::"+name;
        try{
            QString targetName;
            for(const NameSpacePrefix& prefix : qAsConst(m_namespacePrefixes)){
                if(name.startsWith(prefix.prefix)){
                    name = name.mid(prefix.prefix.size());
                    if(prefix.namingPolicy==NameSpacePrefix::Cut)
                        targetName = name;
                    name = prefix._namespace + name;
                    break;
                }
            }
//            if(!nameSpace.isEmpty()){
//                if(targetName.endsWith(nameSpace+"::"+element->getName())){
//                    targetName = targetName.mid(0, targetName.length()-element->getName().length()-2) + "$" + element->getName();
//                }else if(name.endsWith(nameSpace+"::"+element->getName())){
//                    targetName = name.mid(0, name.length()-element->getName().length()-2) + "$" + element->getName();
//                }
//            }
            std::unique_ptr<ObjectTypeEntry> entry(new ObjectTypeEntry(name));
            if(!targetName.isEmpty())
                entry->setTargetLangName(targetName.replace("::", "$"));
            entry->setIsValueOwner(element->getIsValueOwner());
            entry->setIsPolymorphicBase(element->getIsPolymorphicBase());
            entry->setPolymorphicIdValue(element->getPolymorphicIdExpression());
            parseAttributesOfComplexType(element, entry.get());
            QList<AbstractObject*> unhandledElements = parseChildrenOfComplexType(nameSpace, element, entry.get());
            for(AbstractObject* unhandledElement : unhandledElements){
                TypesystemException::raise(QString("Unexpected child element %1 in %2.").arg(unhandledElement->metaObject()->className(), element->metaObject()->className()));
            }
            if(name.endsWith(">")){
                auto idx = name.indexOf('<');
                QString templateName = name.mid(0, idx);
                QStringList templateArguments = name.mid(idx+1).chopped(1).split(",");
                ComplexTypeEntry* templateType = m_database->findComplexType(templateName);
                if(templateType){
                    if(templateType->isTemplate()){
                        if(templateType->instantiations().contains(templateArguments) && templateType->instantiations()[templateArguments]==nullptr){
                            entry->setTargetLangName(templateType->targetLangName());
                            templateType->addInstantiation(templateArguments, entry.release());
                        }else{
                            TypesystemException::raise(QString("Template %1<%2> already defined").arg(templateName).arg(templateArguments.join(",")));
                        }
                    }else{
                        TypesystemException::raise(QString("Type %1 not a template").arg(templateName));
                    }
                }else{
                    TypesystemException::raise(QString("Template %1 not found").arg(templateName));
                }
            }else{
                ReportHandler::debugTypes("Adding to TypeDatabase(2): " + entry->name());
                m_database->addType(entry.release());
            }
        }catch(const TypesystemException& exn){
            TypesystemException::raise(QString("%1 of type %2").arg(QLatin1String(exn.what()), name));
        }
    }
}

QList<AbstractObject*> QmlTypeSystemReaderPrivate::parseChildrenOfComplexType(const QString& nameSpace, ComplexType* element, ComplexTypeEntry* entry){
    QList<AbstractObject*> unhandledElements;
    const QList<AbstractObject*>& childrenList = element->childrenList();
    QString tmpDefaultPackage = entry->javaPackage();
    m_defaultPackage.swap(tmpDefaultPackage);
    auto s = qScopeGuard([&](){m_defaultPackage.swap(tmpDefaultPackage);});
    for(int i=0; i<childrenList.size(); ++i){
        AbstractObject* item = childrenList[i];
        if(NamespacePrefix* namespacePrefix = qobject_cast<NamespacePrefix*>(item)){
            if(entry->isNamespace()){
                if (checkQtVersion(namespacePrefix)){
                    QString prefix = entry->name();
                    if(!namespacePrefix->getPrefix().isEmpty())
                        prefix = namespacePrefix->getPrefix();
                    QString _namespace = entry->name();
                    if(!namespacePrefix->getNamespace().isEmpty())
                        _namespace = _namespace+"::"+namespacePrefix->getNamespace();
                    if(!prefix.isEmpty() && !prefix.endsWith("::"))
                        prefix += "::";
                    if(!_namespace.isEmpty() && !_namespace.endsWith("::"))
                        _namespace += "::";
                    m_namespacePrefixes.append({prefix, _namespace, NameSpacePrefix::NamingPolicy(namespacePrefix->getNamingPolicy())});
                }
            }else{
                unhandledElements << item;
            }
        }else if(Template* childElement = qobject_cast<Template*>(item)){
            parseTemplate(childElement);
        }else if(TemplateArguments* childElement = qobject_cast<TemplateArguments*>(item)){
            if(!entry->isTemplate())
                TypesystemException::raise(QString("Unexpected element Instantiation as child of non-template"));
            parseTemplateArguments(childElement, entry);
        }else if(ModifyFunction* childElement = qobject_cast<ModifyFunction*>(item)){
            parseModifyFunction(childElement, entry);
        }else if(ModifyField* childElement = qobject_cast<ModifyField*>(item)){
            parseModifyField(childElement, entry);
        }else if(InjectCode* childElement = qobject_cast<InjectCode*>(item)){
            parseInjectCode(childElement, entry);
        }else if(DelegateBaseClass* childElement = qobject_cast<DelegateBaseClass*>(item)){
            if (checkQtVersion(childElement)){
                if(!childElement->getBaseClass().isEmpty())
                    entry->addDelegatedBaseClass(childElement->getBaseClass(), childElement->getDelegate());
            }
        }else if(Implements* childElement = qobject_cast<Implements*>(item)){
            if (checkQtVersion(childElement)){
                entry->setImplements(childElement->getInterface());
            }
        }else if(Include* childElement = qobject_cast<Include*>(item)){
            TS::Include incl = parseInclude(childElement);
            if(incl.isValid()){
                if(!entry->ppCondition().isEmpty())
                    incl.ckeckAvailability = true;
                entry->setInclude(incl);
                if(entry->designatedInterface())
                    entry->designatedInterface()->setInclude(incl);
            }
        }else if(ExtraIncludes* childElement = qobject_cast<ExtraIncludes*>(item)){
            parseExtraIncludes(childElement, entry);
        }else if(Import* childElement = qobject_cast<Import*>(item)){
            if (checkQtVersion(childElement)){
                QString name = childElement->getTemplate();
                if(TemplateTypeEntry* templateEntry = m_database->findTemplateType(name)){
                    entry->addCodeSnips(templateEntry->codeSnips());
                    entry->addFunctionModifications(templateEntry->functionModifications());
                    entry->addFieldModifications(templateEntry->fieldModifications());
                    for(const TS::Include& incl : templateEntry->extraIncludes()){
                        entry->addExtraInclude(incl);
                    }
                    if(entry->designatedInterface()){
                        entry->designatedInterface()->addCodeSnips(templateEntry->codeSnips());
                        entry->designatedInterface()->addFunctionModifications(templateEntry->functionModifications());
                        entry->designatedInterface()->addFieldModifications(templateEntry->fieldModifications());
                        for(const TS::Include& incl : templateEntry->extraIncludes()){
                            entry->designatedInterface()->addExtraInclude(incl);
                        }
                    }
                }else{
                    TypesystemException::raise(QString("Unable to find template-type '%1'.").arg(name));
                }
            }
        }else if(InterfacePolymorphy* childElement = qobject_cast<InterfacePolymorphy*>(item)){
            if(entry->isValue() || entry->isObject() || entry->isInterface()){
                if (checkQtVersion(childElement)){
                    if(childElement->getInterfaceType().isEmpty())
                        TypesystemException::raise(QString("No interfaceType specified for InterfacePolymorphy"));
                    entry->addInterfacePolymorphicIdValue(childElement->getInterfaceType(), childElement->getCondition());
                    if(entry->designatedInterface())
                        entry->designatedInterface()->addInterfacePolymorphicIdValue(childElement->getInterfaceType(), childElement->getCondition());
                }
            }else{
                TypesystemException::raise(QString("Unexpected child element InterfacePolymorphy of %1.").arg(element->metaObject()->className()));
            }
        }else if(CustomConstructor* childElement = qobject_cast<CustomConstructor*>(item)){
            if(entry->isValue() || (entry->isInterface() && dynamic_cast<InterfaceTypeEntry*>(entry)->origin()->isValue())){
                if (checkQtVersion(childElement)){
                    CustomFunction func = parseCustomStructor(childElement);
                    func.name = childElement->getName();
                    if(func.name.isEmpty())
                        func.name = entry->name() + "_construct";
                    func.param_name = childElement->getParamName();
                    if(func.param_name.isEmpty())
                        func.param_name = "copy";
                    func.placement_name = childElement->getPlacementName();
                    if(func.placement_name.isEmpty())
                        func.placement_name = "placement";
                    QHash<CustomConstructor::ConstructorType,ComplexTypeEntry::ConstructorType> types{
                                {CustomConstructor::Legacy,ComplexTypeEntry::LegacyCopyConstructor},
                                {CustomConstructor::Default,ComplexTypeEntry::DefaultConstructor},
                                {CustomConstructor::Copy,ComplexTypeEntry::CopyConstructor},
                                {CustomConstructor::Move,ComplexTypeEntry::MoveConstructor}};
                    entry->setCustomConstructor(func, types[childElement->getType()]);
                    if(entry->designatedInterface())
                        entry->designatedInterface()->setCustomConstructor(func, types[childElement->getType()]);
                }
            }else{
                TypesystemException::raise(QString("Unexpected child element CustomConstructor of %1.").arg(element->metaObject()->className()));
            }
        }else if(CustomDestructor* childElement = qobject_cast<CustomDestructor*>(item)){
            if(entry->isValue() || (entry->isInterface() && dynamic_cast<InterfaceTypeEntry*>(entry)->origin()->isValue())){
                if (checkQtVersion(childElement)){
                    CustomFunction func = parseCustomStructor(childElement);
                    func.name = childElement->getName();
                    if(func.name.isEmpty())
                        func.name = entry->name() + "_destroy";
                    func.param_name = childElement->getParamName();
                    if(func.param_name.isEmpty())
                        func.param_name = "ptr";
                    entry->setCustomDestructor(func);
                    if(entry->designatedInterface())
                        entry->designatedInterface()->setCustomDestructor(func);
                }
            }else{
                TypesystemException::raise(QString("Unexpected child element CustomDestructor of %1.").arg(element->metaObject()->className()));
            }
        }else if(PrimitiveType* childElement = qobject_cast<PrimitiveType*>(item)){
            QString name = element->getName();
            if(!nameSpace.isEmpty())
                name = nameSpace+"::"+name;
            parsePrimitiveType(name, childElement);
        }else if(ObjectType* childElement = qobject_cast<ObjectType*>(item)){
            QString name = element->getName();
            if(!nameSpace.isEmpty())
                name = nameSpace+"::"+name;
            parseObjectType(name, childElement);
        }else if(TemplateType* childElement = qobject_cast<TemplateType*>(item)){
            QString name = element->getName();
            if(!nameSpace.isEmpty())
                name = nameSpace+"::"+name;
            parseTemplateType(name, childElement);
        }else if(FunctionalType* childElement = qobject_cast<FunctionalType*>(item)){
            QString name = element->getName();
            if(!nameSpace.isEmpty())
                name = nameSpace+"::"+name;
            parseFunctionalType(name, childElement);
        }else if(IteratorType* childElement = qobject_cast<IteratorType*>(item)){
            QString name = element->getName();
            if(!nameSpace.isEmpty())
                name = nameSpace+"::"+name;
            parseIteratorType(name, childElement);
        }else if(ValueType* childElement = qobject_cast<ValueType*>(item)){
            QString name = element->getName();
            if(!nameSpace.isEmpty())
                name = nameSpace+"::"+name;
            parseValueType(name, childElement);
        }else if(TypeAliasType* childElement = qobject_cast<TypeAliasType*>(item)){
            QString name = element->getName();
            if(!nameSpace.isEmpty())
                name = nameSpace+"::"+name;
            parseTypeAliasType(name, childElement);
        }else if(InterfaceType* childElement = qobject_cast<InterfaceType*>(item)){
            QString name = element->getName();
            if(!nameSpace.isEmpty())
                name = nameSpace+"::"+name;
            parseInterfaceType(name, childElement);
        }else if(NamespaceType* childElement = qobject_cast<NamespaceType*>(item)){
            QString name = element->getName();
            if(!nameSpace.isEmpty())
                name = nameSpace+"::"+name;
            parseNamespaceType(name, childElement);
        }else if(EnumType* childElement = qobject_cast<EnumType*>(item)){
            QString name = element->getName();
            if(!nameSpace.isEmpty())
                name = nameSpace+"::"+name;
            parseEnumType(name, childElement);
        }else if(ImplicitCast* childElement = qobject_cast<ImplicitCast*>(item)){
            if (checkQtVersion(childElement)){
                entry->addImplicitCast(childElement->getType());
                for(AbstractObject* item2 : childElement->childrenList()){
                    TypesystemException::raise(QStringLiteral("Unexpected element %1 as child of %2").arg(item2->metaObject()->className(), childElement->metaObject()->className()));
                }
            }
        }else if(Rejection* rejection = qobject_cast<Rejection*>(item)){
            if (checkQtVersion(element)){
                QString cls = rejection->getClassName();
                QString function = rejection->getFunctionName();
                QString field = rejection->getFieldName();
                QString enum_ = rejection->getEnumName();
                if (cls == "*" && function.isEmpty() && field.isEmpty() && enum_.isEmpty()) {
                    TypesystemException::raise(QString("bad reject entry, neither 'enumName', 'functionFame' nor 'fieldName'"));
                }
                cls = cls=="*" ? element->getName() : element->getName() + "::" + cls;
                if(!nameSpace.isEmpty())
                    cls = nameSpace+"::"+cls;
                m_database->addRejection(cls, function, field, enum_);
                for(AbstractObject* item2 : rejection->childrenList()){
                    TypesystemException::raise(QStringLiteral("Unexpected element %1 as child of %2").arg(item2->metaObject()->className(), childElement->metaObject()->className()));
                }
            }
        }else{
            unhandledElements << item;
        }
    }
    return unhandledElements;
}

CustomFunction QmlTypeSystemReaderPrivate::parseCustomStructor(AbstractStructor* element){
    CustomFunction func;
    const QList<AbstractObject*>& childrenList = element->childrenList();
    for(int i=0; i<childrenList.size(); ++i){
        AbstractObject* item = childrenList[i];
        if(InsertTemplate* childElement = qobject_cast<InsertTemplate*>(item)){
            parseInsertTemplate(childElement, [&func](TemplateInstance* templateInstance){func.addTemplateInstance(templateInstance);});
        }else if(ImportFile* childElement = qobject_cast<ImportFile*>(item)){
            parseImportFile(childElement, [&func](const QString& code){func.addCode(code);});
        }else if(Text* childElement = qobject_cast<Text*>(item)){
            if (checkQtVersion(childElement)){
                func.addCode(childElement->getContent());
                for(AbstractObject* item2 : item->childrenList()){
                    TypesystemException::raise(QStringLiteral("Unexpected element %1 as child of %2").arg(item2->metaObject()->className(), item->metaObject()->className()));
                }
            }
        }else{
            TypesystemException::raise(QString("Unexpected child element %1 in %2.").arg(item->metaObject()->className(), element->metaObject()->className()));
        }
    }
    return func;
}

void QmlTypeSystemReaderPrivate::parseTemplateType(const QString& nameSpace, TemplateType* element){
    if (checkQtVersion(element)){
        QString name = element->getName();
        if(!nameSpace.isEmpty())
            name = nameSpace+"::"+name;
        try{
            std::unique_ptr<TemplateTypeEntry> entry(new TemplateTypeEntry(name));
            const QList<AbstractObject*>& childrenList = element->childrenList();
            for(int i=0; i<childrenList.size(); ++i){
                AbstractObject* item = childrenList[i];
                if(Template* childElement = qobject_cast<Template*>(item)){
                    parseTemplate(childElement);
                }else if(TemplateArguments* childElement = qobject_cast<TemplateArguments*>(item)){
                    if(!entry->isTemplate())
                        TypesystemException::raise(QString("Unexpected element Instantiation as child of non-template"));
                    parseTemplateArguments(childElement, entry.get());
                }else if(ModifyFunction* childElement = qobject_cast<ModifyFunction*>(item)){
                    parseModifyFunction(childElement, entry.get());
                }else if(ModifyField* childElement = qobject_cast<ModifyField*>(item)){
                    parseModifyField(childElement, entry.get());
                }else if(InjectCode* childElement = qobject_cast<InjectCode*>(item)){
                    parseInjectCode(childElement, entry.get());
                }else if(DelegateBaseClass* childElement = qobject_cast<DelegateBaseClass*>(item)){
                    if (checkQtVersion(childElement)){
                        for(AbstractObject* item2 : item->childrenList()){
                            TypesystemException::raise(QStringLiteral("Unexpected element %1 as child of %2").arg(item2->metaObject()->className(), item->metaObject()->className()));
                        }
                        if(!childElement->getBaseClass().isEmpty())
                            entry->addDelegatedBaseClass(childElement->getBaseClass(), childElement->getDelegate());
                    }
                }else if(Implements* childElement = qobject_cast<Implements*>(item)){
                    if (checkQtVersion(childElement)){
                        for(AbstractObject* item2 : item->childrenList()){
                            TypesystemException::raise(QStringLiteral("Unexpected element %1 as child of %2").arg(item2->metaObject()->className(), item->metaObject()->className()));
                        }
                        entry->setImplements(childElement->getInterface());
                    }
                }else if(Include* childElement = qobject_cast<Include*>(item)){
                    TS::Include incl = parseInclude(childElement);
                    if(incl.isValid()){
                        if(!entry->ppCondition().isEmpty())
                            incl.ckeckAvailability = true;
                        entry->setInclude(incl);
                        if(entry->designatedInterface())
                            entry->designatedInterface()->setInclude(incl);
                    }
                }else if(ExtraIncludes* childElement = qobject_cast<ExtraIncludes*>(item)){
                    parseExtraIncludes(childElement, entry.get());
                }else if(Import* childElement = qobject_cast<Import*>(item)){
                    if (checkQtVersion(childElement)){
                        for(AbstractObject* item2 : item->childrenList()){
                            TypesystemException::raise(QStringLiteral("Unexpected element %1 as child of %2").arg(item2->metaObject()->className(), item->metaObject()->className()));
                        }
                        QString name = childElement->getTemplate();
                        if(TemplateTypeEntry* templateEntry = m_database->findTemplateType(name)){
                            entry->addCodeSnips(templateEntry->codeSnips());
                            entry->addFunctionModifications(templateEntry->functionModifications());
                            entry->addFieldModifications(templateEntry->fieldModifications());
                            for(const TS::Include& incl : templateEntry->extraIncludes()){
                                entry->addExtraInclude(incl);
                            }
                            if(entry->designatedInterface()){
                                entry->designatedInterface()->addCodeSnips(templateEntry->codeSnips());
                                entry->designatedInterface()->addFunctionModifications(templateEntry->functionModifications());
                                entry->designatedInterface()->addFieldModifications(templateEntry->fieldModifications());
                                for(const TS::Include& incl : templateEntry->extraIncludes()){
                                    entry->designatedInterface()->addExtraInclude(incl);
                                }
                            }
                        }else{
                            TypesystemException::raise(QString("Unable to find template-type '%1'.").arg(name));
                        }
                    }
                }else if(InterfacePolymorphy* childElement = qobject_cast<InterfacePolymorphy*>(item)){
                    if(entry->isValue() || entry->isObject() || entry->isInterface()){
                        if (checkQtVersion(childElement)){
                            for(AbstractObject* item2 : item->childrenList()){
                                TypesystemException::raise(QStringLiteral("Unexpected element %1 as child of %2").arg(item2->metaObject()->className(), item->metaObject()->className()));
                            }
                            if(childElement->getInterfaceType().isEmpty())
                                TypesystemException::raise(QString("No interfaceType specified for InterfacePolymorphy"));
                            entry->addInterfacePolymorphicIdValue(childElement->getInterfaceType(), childElement->getCondition());
                            if(entry->designatedInterface())
                                entry->designatedInterface()->addInterfacePolymorphicIdValue(childElement->getInterfaceType(), childElement->getCondition());
                        }
                    }else{
                        TypesystemException::raise(QString("Unexpected child element InterfacePolymorphy of %1.").arg(element->metaObject()->className()));
                    }
                }else if(CustomConstructor* childElement = qobject_cast<CustomConstructor*>(item)){
                    if(entry->isValue() || (entry->isInterface() && dynamic_cast<InterfaceTypeEntry*>(entry.get())->origin()->isValue())){
                        if (checkQtVersion(childElement)){
                            CustomFunction func = parseCustomStructor(childElement);
                            func.name = childElement->getName();
                            if(func.name.isEmpty())
                                func.name = entry->name() + "_construct";
                            func.param_name = childElement->getParamName();
                            if(func.param_name.isEmpty())
                                func.param_name = "copy";
                            func.placement_name = childElement->getPlacementName();
                            if(func.placement_name.isEmpty())
                                func.placement_name = "placement";
                            QHash<CustomConstructor::ConstructorType,ComplexTypeEntry::ConstructorType> types{
                                        {CustomConstructor::Legacy,ComplexTypeEntry::LegacyCopyConstructor},
                                        {CustomConstructor::Default,ComplexTypeEntry::DefaultConstructor},
                                        {CustomConstructor::Copy,ComplexTypeEntry::CopyConstructor},
                                        {CustomConstructor::Move,ComplexTypeEntry::MoveConstructor}};
                            entry->setCustomConstructor(func, types[childElement->getType()]);
                            if(entry->designatedInterface())
                                entry->designatedInterface()->setCustomConstructor(func, types[childElement->getType()]);
                        }
                    }else{
                        TypesystemException::raise(QString("Unexpected child element CustomConstructor of %1.").arg(element->metaObject()->className()));
                    }
                }else if(CustomDestructor* childElement = qobject_cast<CustomDestructor*>(item)){
                    if(entry->isValue() || (entry->isInterface() && dynamic_cast<InterfaceTypeEntry*>(entry.get())->origin()->isValue())){
                        if (checkQtVersion(childElement)){
                            CustomFunction func = parseCustomStructor(childElement);
                            func.name = childElement->getName();
                            if(func.name.isEmpty())
                                func.name = entry->name() + "_destroy";
                            func.param_name = childElement->getParamName();
                            if(func.param_name.isEmpty())
                                func.param_name = "ptr";
                            entry->setCustomDestructor(func);
                            if(entry->designatedInterface())
                                entry->designatedInterface()->setCustomDestructor(func);
                        }
                    }else{
                        TypesystemException::raise(QString("Unexpected child element CustomDestructor of %1.").arg(element->metaObject()->className()));
                    }
                }else{
                    TypesystemException::raise(QString("Unexpected child element %1 in %2.").arg(childElement->metaObject()->className(), element->metaObject()->className()));
                }
            }
            m_database->addTemplateType(entry.release());
        }catch(const TypesystemException& exn){
            TypesystemException::raise(QString("%1 of type %2").arg(QLatin1String(exn.what()), name));
        }
    }
}

void QmlTypeSystemReaderPrivate::parseDelegate(Delegate* element, AbstractFunctionModification& mod){
    if (checkQtVersion(element)){
        uint modifiers = 0;
        AccessModifications accesses = element->access();
        if(accesses.testFlag(Modification::Private)){
            modifiers |= TS::Modification::Private;
        }
        if(accesses.testFlag(Modification::Public)){
            modifiers |= TS::Modification::Public;
        }
        if(accesses.testFlag(Modification::Protected)){
            modifiers |= TS::Modification::Protected;
        }
        if(accesses.testFlag(Modification::Friendly)){
            modifiers |= TS::Modification::Friendly;
        }
        if(accesses.testFlag(Modification::Final)){
            modifiers |= TS::Modification::Final;
        }
        if(accesses.testFlag(Modification::DeclFinal)){
            modifiers |= TS::Modification::NativeDeclFinal;
        }
        if(accesses.testFlag(Modification::NonFinal)){
            modifiers |= TS::Modification::NonFinal;
        }
        TS::CodeSnipList snips;
        const QList<AbstractObject*>& childrenList = element->childrenList();
        for(int i=0; i<childrenList.size(); ++i){
            AbstractObject* item = childrenList[i];
            if(InjectCode* childElement = qobject_cast<InjectCode*>(item)){
                static const QHash<CodeClass::Entries, TS::Language> languageNames{
                    {CodeClass::Java, TS::TargetLangCode},
                    {CodeClass::JavaInterface, TS::Interface}
                };

                static const QHash<Position::Entries, CodeSnip::Position> positionNames{
                    {Position::Beginning, CodeSnip::Beginning},
                    {Position::Comment, CodeSnip::Comment},
                    {Position::End, CodeSnip::End}
                };
                parseInjectCode(childElement, languageNames, positionNames, [&snips](const QString&,const CodeSnip &snip){snips<<snip;});
            }else{
                TypesystemException::raise(QStringLiteral("Unexpected element %1 as child of %2").arg(item->metaObject()->className(), element->metaObject()->className()));
            }
        }
        mod.delegates << TS::Delegate{element->name(), element->isDeprecated(), element->isSelfReturning(), modifiers, snips};
    }
}

void QmlTypeSystemReaderPrivate::parseModifyArgument(ModifyArgument* element, AbstractFunctionModification& mod){
    if (checkQtVersion(element)){
        int index = 0;
        try{
            if (element->getIndex().userType()==QMetaType::QString && element->getIndex().value<QString>() == "return")
                index = 0;
            else if (element->getIndex().userType()==QMetaType::QString && element->getIndex().value<QString>() == "this")
                index = -1;
            else{
                bool ok = false;
                index = element->getIndex().toInt(&ok);
                if(!ok)
                    element->getIndex().value<QString>().toInt(&ok);
                if(!ok)
                    TypesystemException::raise(QString("Can't convert ModifyArgument property index to integer"));
            }
            ArgumentModification argumentModification(index);

            argumentModification.replace_value = element->getReplaceValue();
            if (!argumentModification.replace_value.isEmpty() && index != 0)
                TypesystemException::raise(QString("ModifyArgument.replaceValue is only supported for return values (index=0)"));
            if(element->getThreadAffinity().userType()==QMetaType::Bool){
                if(element->getThreadAffinity().toBool()){
                    argumentModification.thread_affine = TS::ThreadAffinity::Yes;
                }else{
                    argumentModification.thread_affine = TS::ThreadAffinity::None;
                }
            }else if(element->getThreadAffinity().userType()==QMetaType::QString){
                if(element->getThreadAffinity().toString().toLower()=="ui")
                    argumentModification.thread_affine = TS::ThreadAffinity::UI;
                else if(element->getThreadAffinity().toString().toLower()=="pixmap")
                    argumentModification.thread_affine = TS::ThreadAffinity::Pixmap;
            }else if(element->getThreadAffinity().canConvert<int>()){
                bool ok = false;
                int v = element->getThreadAffinity().toInt(&ok);
                if(ok){
                    switch(v){
                    case 0:
                        argumentModification.thread_affine = TS::ThreadAffinity::None;
                        break;
                    case 1:
                        argumentModification.thread_affine = TS::ThreadAffinity::Yes;
                        break;
                    case Affinity::UI:
                        argumentModification.thread_affine = TS::ThreadAffinity::UI;
                        break;
                    case Affinity::Pixmap:
                        argumentModification.thread_affine = TS::ThreadAffinity::Pixmap;
                        break;
                    default:
                        argumentModification.thread_affine = TS::ThreadAffinity::Yes;
                        break;
                    }
                }else{
                    argumentModification.thread_affine = TS::ThreadAffinity::Yes;
                }
            }
            argumentModification.modified_name = element->getRename();
            argumentModification.modified_type = element->getReplaceType();
            argumentModification.reset_after_use = element->getInvalidateAfterUse();
            argumentModification.value_as_pointer = element->getValueAsPointer();
            argumentModification.no_implicit_calls = element->getNoImplicitCalls();

            const QList<AbstractObject*>& childrenList = element->childrenList();
            for(int i=0; i<childrenList.size(); ++i){
                AbstractObject* item = childrenList[i];
                if(RemoveDefaultExpression* childElement = qobject_cast<RemoveDefaultExpression*>(item)){
                    if (checkQtVersion(childElement)){
                        for(AbstractObject* item2 : item->childrenList()){
                            TypesystemException::raise(QStringLiteral("Unexpected element %1 as child of %2").arg(item2->metaObject()->className(), item->metaObject()->className()));
                        }
                        argumentModification.removed_default_expression = true;
                    }
                }else if(ReplaceDefaultExpression* childElement = qobject_cast<ReplaceDefaultExpression*>(item)){
                    if (checkQtVersion(childElement)){
                        for(AbstractObject* item2 : item->childrenList()){
                            TypesystemException::raise(QStringLiteral("Unexpected element %1 as child of %2").arg(item2->metaObject()->className(), item->metaObject()->className()));
                        }
                        argumentModification.replaced_default_expression = childElement->getExpression();
                    }
                }else if(ReferenceCount* childElement = qobject_cast<ReferenceCount*>(item)){
                    if (checkQtVersion(childElement)){
                        for(AbstractObject* item2 : item->childrenList()){
                            TypesystemException::raise(QStringLiteral("Unexpected element %1 as child of %2").arg(item2->metaObject()->className(), item->metaObject()->className()));
                        }
                        TS::ReferenceCount rc;
                        rc.threadSafe = childElement->getIsThreadSafe();
                        static const QHash<ReferenceCount::Action, TS::ReferenceCount::Action> actions{
                            {ReferenceCount::Put, TS::ReferenceCount::Put},
                            {ReferenceCount::Add, TS::ReferenceCount::Add},
                            {ReferenceCount::AddAll, TS::ReferenceCount::AddAll},
                            {ReferenceCount::ClearAdd, TS::ReferenceCount::ClearAdd},
                            {ReferenceCount::ClearAddAll, TS::ReferenceCount::ClearAddAll},
                            {ReferenceCount::Take, TS::ReferenceCount::Remove},
                            {ReferenceCount::Set, TS::ReferenceCount::Set},
                            {ReferenceCount::Ignore, TS::ReferenceCount::Ignore}
                        };
                        rc.action = actions[childElement->getAction()];
                        rc.variableName = childElement->getVariableName();
                        if (rc.action != TS::ReferenceCount::Ignore && rc.variableName.isEmpty()) {
                            TypesystemException::raise(QString("ReferenceCount.variableName must be specified"));
                        }
                        rc.keyArgument = childElement->getKeyArgument();
                        if (rc.action == TS::ReferenceCount::Put && rc.keyArgument<=0) {
                            TypesystemException::raise(QString("ReferenceCount.keyArgument must be specified and greater than 0"));
                        }
                        rc.declareVariable = childElement->getDeclareVariable();
                        rc.condition = childElement->getCondition();
                        if(childElement->getAccess().testFlag(Modification::Private))
                            rc.access = TS::ReferenceCount::Private;
                        else if(childElement->getAccess().testFlag(Modification::Protected))
                            rc.access = TS::ReferenceCount::Protected;
                        else if(childElement->getAccess().testFlag(Modification::Public))
                            rc.access = TS::ReferenceCount::Public;
                        else if(childElement->getAccess().testFlag(Modification::Friendly))
                            rc.access = TS::ReferenceCount::Friendly;
                        else
                            rc.access = TS::ReferenceCount::Private;
                        argumentModification.referenceCounts.append(rc);
                    }
                }else if(AsBuffer* childElement = qobject_cast<AsBuffer*>(item)){
                    if (checkQtVersion(childElement)){
                        AsArray* asArray{nullptr};
                        for(AbstractObject* item2 : item->childrenList()){
                            if(AsArray* childElement2 = qobject_cast<AsArray*>(item2)){
                                if (checkQtVersion(childElement2)){
                                    asArray = childElement2;
                                    for(AbstractObject* item3 : asArray->childrenList()){
                                        TypesystemException::raise(QStringLiteral("Unexpected element %1 as child of %2").arg(item3->metaObject()->className(), asArray->metaObject()->className()));
                                    }
                                }
                            }else{
                                TypesystemException::raise(QStringLiteral("Unexpected element %1 as child of %2").arg(item2->metaObject()->className(), item->metaObject()->className()));
                            }
                        }
                        TS::AsBufferTypes asBufferType = TS::AsBufferType::Yes;
                        if(childElement->getDeref())
                            asBufferType |= TS::AsBufferType::Deref;
                        argumentModification.useAsBufferType = asBufferType;
                        if (childElement->getLengthParameter()>0) {
                            argumentModification.arrayLengthParameter = childElement->getLengthParameter();
                        }else{
                            argumentModification.arrayLengthParameter = 0;
                            if (childElement->getMinLength()>=0) {
                                argumentModification.minArrayLength = childElement->getMinLength();
                                if (childElement->getMaxLength()>=0) {
                                    argumentModification.maxArrayLength = childElement->getMaxLength();
                                }else{
                                    argumentModification.maxArrayLength = -1;
                                }
                            }else{
                                argumentModification.minArrayLength = -1;
                                argumentModification.maxArrayLength = -1;
                                argumentModification.arrayLengthExpression = childElement->getLengthExpression();
                            }
                        }
                        TS::AsArrayTypes asArrayType = TS::AsArrayType::No;
                        if(asArray){
                            asArrayType = TS::AsArrayType::Yes;
                            if(childElement->getDeref())
                                asArrayType |= TS::AsArrayType::Deref;
                            if(asArray->getVarargs()){
                                asArrayType |= TS::AsArrayType::VarArgs;
                                asArrayType |= TS::AsArrayType::NoOffset;
                            }
                        }
                        argumentModification.useAsArrayType = asArrayType;
                    }
                }else if(AsArray* childElement = qobject_cast<AsArray*>(item)){
                    if (checkQtVersion(childElement)){
                        for(AbstractObject* item2 : item->childrenList()){
                            TypesystemException::raise(QStringLiteral("Unexpected element %1 as child of %2").arg(item2->metaObject()->className(), item->metaObject()->className()));
                        }
                        TS::AsArrayTypes asArrayType = TS::AsArrayType::Yes;
                        if(childElement->getDeref())
                            asArrayType |= TS::AsArrayType::Deref;
                        if(childElement->getVarargs()){
                            asArrayType |= TS::AsArrayType::VarArgs;
                            asArrayType |= TS::AsArrayType::NoOffset;
                        }
                        if(childElement->getNoOffset())
                            asArrayType |= TS::AsArrayType::NoOffset;
                        if(childElement->getAddPlainDelegate())
                            asArrayType |= TS::AsArrayType::AddPlainDelegate;
                        argumentModification.useAsArrayType = asArrayType;
                        argumentModification.useAsBufferType = TS::AsBufferType::No;
                        if (childElement->getLengthParameter()>0) {
                            argumentModification.arrayLengthParameter = childElement->getLengthParameter();
                        }else{
                            argumentModification.arrayLengthParameter = 0;
                            if (childElement->getMinLength()>=0) {
                                argumentModification.minArrayLength = childElement->getMinLength();
                                if (childElement->getMaxLength()>=0) {
                                    argumentModification.maxArrayLength = childElement->getMaxLength();
                                }else{
                                    argumentModification.maxArrayLength = -1;
                                }
                            }else{
                                argumentModification.minArrayLength = -1;
                                argumentModification.maxArrayLength = -1;
                                argumentModification.arrayLengthExpression = childElement->getLengthExpression();
                            }
                        }
                        if(argumentModification.arrayLengthParameter<1
                            && argumentModification.minArrayLength<0
                            && argumentModification.arrayLengthExpression.isEmpty()){
                            TypesystemException::raise(QStringLiteral("%1 requires to specify either lengthParameter, lengthExpression or minLength").arg(item->metaObject()->className()));
                        }
                    }
                }else if(ReplaceType* childElement = qobject_cast<ReplaceType*>(item)){
                    if (checkQtVersion(childElement)){
                        for(AbstractObject* item2 : item->childrenList()){
                            TypesystemException::raise(QStringLiteral("Unexpected element %1 as child of %2").arg(item2->metaObject()->className(), item->metaObject()->className()));
                        }
                        if (childElement->getModifiedType().isEmpty()) {
                            TypesystemException::raise(QString("Missing required property ReplaceType.modifiedType"));
                        }
                        argumentModification.modified_type = childElement->getModifiedType();
                        argumentModification.modified_jni_type = childElement->getModifiedJniType();

                    }
                }else if(ConversionRule* childElement = qobject_cast<ConversionRule*>(item)){
                    static const QHash<CodeClass::Entries, TS::Language> languageNames{
                        {CodeClass::Native, TS::NativeCode},
                        {CodeClass::Shell, TS::ShellCode}
                    };
                    parseConversionRule(languageNames, childElement, [&argumentModification](const CodeSnip& snip){argumentModification.conversion_rules.append(snip);});
                }else if(NoNullPointer* childElement = qobject_cast<NoNullPointer*>(item)){
                    if (checkQtVersion(childElement)){
                        for(AbstractObject* item2 : item->childrenList()){
                            TypesystemException::raise(QStringLiteral("Unexpected element %1 as child of %2").arg(item2->metaObject()->className(), item->metaObject()->className()));
                        }
                        argumentModification.no_null_pointers = true;
                        if (argumentModification.index == 0) {
                            argumentModification.null_pointer_default_value = childElement->getDefaultValue();
                        } else if (!childElement->getDefaultValue().isEmpty()) {
                            ReportHandler::warning("default values for null pointer guards are only effective for return values");
                        }
                    }
                }else if(DefineOwnership* childElement = qobject_cast<DefineOwnership*>(item)){
                    if (checkQtVersion(childElement)){
                        for(AbstractObject* item2 : item->childrenList()){
                            TypesystemException::raise(QStringLiteral("Unexpected element %1 as child of %2").arg(item2->metaObject()->className(), item->metaObject()->className()));
                        }

                        static const QHash<Ownership::Entries, TS::Ownership> ownershipNames{
                            {Ownership::Java, TS::TargetLangOwnership},
                            {Ownership::Cpp, TS::CppOwnership},
                            {Ownership::Default, TS::DefaultOwnership},
                            {Ownership::Ignore, TS::IgnoreOwnership},
                            {Ownership::Invalidate, TS::Invalidate},
                            {Ownership::Dependent, TS::RegisterDependency},
//                            {Ownership::Invalid, TS::InvalidOwnership}
                        };

                        if(!ownershipNames.contains(childElement->getOwnership()))
                            TypesystemException::raise(QString("Unsupported ownership property"));

                        static const QHash<CodeClass::Entries, TS::Language> languageNames{
                            {CodeClass::Java, TS::TargetLangCode},
                            {CodeClass::Native, TS::NativeCode},
                            {CodeClass::Shell, TS::ShellCode}
                        };

                        QHash<CodeClass::Entries, TS::Language> _languageNames = languageNames;
                        if(childElement->getOwnership()==Ownership::Ignore)
                            _languageNames.insert(CodeClass::NoLanguage, TS::NoLanguage);
                        if (!_languageNames.contains(childElement->getCodeClass()))
                            TypesystemException::raise(QString("Unsupported codeClass property"));

                        argumentModification.ownerships[_languageNames[childElement->getCodeClass()]] = TS::OwnershipRule{ownershipNames[childElement->getOwnership()], childElement->getCondition()};
                    }
                }else if(RemoveArgument* childElement = qobject_cast<RemoveArgument*>(item)){
                    if (checkQtVersion(childElement)){
                        for(AbstractObject* item2 : item->childrenList()){
                            TypesystemException::raise(QStringLiteral("Unexpected element %1 as child of %2").arg(item2->metaObject()->className(), item->metaObject()->className()));
                        }
                        argumentModification.removed = true;
                    }
                }else if(AddImplicitCall* childElement = qobject_cast<AddImplicitCall*>(item)){
                    if (checkQtVersion(childElement)){
                        argumentModification.implicitCalls << childElement->getType();
                    }
                }else if(InhibitImplicitCall* childElement = qobject_cast<InhibitImplicitCall*>(item)){
                    if (checkQtVersion(childElement)){
                        argumentModification.inhibitedImplicitCalls << childElement->getType();
                    }
                }else{
                    TypesystemException::raise(QString("Unexpected child element %1 in %2.").arg(item->metaObject()->className(), element->metaObject()->className()));
                }
            }
            mod.argument_mods << argumentModification;
        }catch(const TypesystemException& exn){
            TypesystemException::raise(QString("%1 of argument %2").arg(QLatin1String(exn.what())).arg(index));
        }
    }
}

void QmlTypeSystemReaderPrivate::parseConversionRule(const QHash<CodeClass::Entries, TS::Language>& languageNames, ConversionRule* element, const std::function<void(const CodeSnip&)>& appendCodeSnip){
    if (checkQtVersion(element)){
        CodeSnip snip;
        if (!languageNames.contains(element->getCodeClass()))
            TypesystemException::raise(QString("Unsupported class attribute"));

        snip.language = languageNames[element->getCodeClass()];

        const QList<AbstractObject*>& childrenList = element->childrenList();
        for(int i=0; i<childrenList.size(); ++i){
            AbstractObject* item = childrenList[i];
            if(InsertTemplate* childElement = qobject_cast<InsertTemplate*>(item)){
                parseInsertTemplate(childElement, [&snip](TemplateInstance* templateInstance){snip.addTemplateInstance(templateInstance);});
            }else if(ImportFile* childElement = qobject_cast<ImportFile*>(item)){
                parseImportFile(childElement, [&snip](const QString& code){snip.addCode(code);});
            }else if(Text* childElement = qobject_cast<Text*>(item)){
                if (checkQtVersion(childElement)){
                    for(AbstractObject* item2 : item->childrenList()){
                        TypesystemException::raise(QStringLiteral("Unexpected element %1 as child of %2").arg(item2->metaObject()->className(), item->metaObject()->className()));
                    }
                    snip.addCode(childElement->getContent());
                }
            }else{
                TypesystemException::raise(QString("Unexpected child element %1 in %2.").arg(item->metaObject()->className(), element->metaObject()->className()));
            }
        }
        appendCodeSnip(snip);
    }
}

TemplateInstantiation QmlTypeSystemReaderPrivate::parseInstantiation(Instantiation* element){
    TemplateInstantiation mod;
    if (checkQtVersion(element)){
        AccessModifications accesses = element->getAccess();
        QString rename = element->getRename();
        mod.ppCondition = element->getPpCondition();
        mod.throws = element->getThrowing();
        mod.proxyCall = element->getProxyCall();
        mod.targetType = element->getTargetType();

        if (element->getDeprecated()) {
            mod.modifiers |= TS::Modification::Deprecated;
        }
        if (element->getNoExcept()) {
            mod.modifiers |= TS::Modification::NoExcept;
        }
        if (element->getBlockExceptions()) {
            mod.modifiers |= TS::Modification::BlockExcept;
        }
        if (element->getRethrowExceptions()) {
            mod.modifiers |= TS::Modification::RethrowExcept;
        }
        if (element->getVirtualSlot()) {
            mod.modifiers |= TS::Modification::VirtualSlot;
        }
        if (element->getAllowAsSlot()) {
            mod.modifiers |= TS::Modification::AllowAsSlot;
        }
        if (element->getPrivateSignal()) {
            mod.modifiers |= TS::Modification::PrivateSignal;
        }

        mod.association = element->getAssociatedTo();

        if(element->getThreadAffinity().userType()==QMetaType::Bool){
            if(element->getThreadAffinity().toBool()){
                mod.modifiers |= TS::Modification::ThreadAffine;
            }
        }else if(element->getThreadAffinity().userType()==QMetaType::QString){
            if(element->getThreadAffinity().toString().toLower()=="ui")
                mod.modifiers |= TS::Modification::UIThreadAffine;
            else if(element->getThreadAffinity().toString().toLower()=="pixmap")
                mod.modifiers |= TS::Modification::PixmapThreadAffine;
        }else if(element->getThreadAffinity().canConvert<int>()){
            bool ok = false;
            int v = element->getThreadAffinity().toInt(&ok);
            if(ok){
                switch(v){
                case 0:
                    break;
                case 1:
                    mod.modifiers |= TS::Modification::ThreadAffine;
                    break;
                case Affinity::UI:
                    mod.modifiers |= TS::Modification::UIThreadAffine;
                    break;
                case Affinity::Pixmap:
                    mod.modifiers |= TS::Modification::PixmapThreadAffine;
                    break;
                default:
                    mod.modifiers |= TS::Modification::ThreadAffine;
                    break;
                }
            }else{
                mod.modifiers |= TS::Modification::ThreadAffine;
            }
        }

        const QList<AbstractObject*>& childrenList = element->childrenList();
        for(int i=0; i<childrenList.size(); ++i){
            AbstractObject* item = childrenList[i];
            if(Argument* childElement = qobject_cast<Argument*>(item)){
                if (checkQtVersion(childElement)){
                    for(AbstractObject* item2 : item->childrenList()){
                        TypesystemException::raise(QStringLiteral("Unexpected element %1 as child of %2").arg(item2->metaObject()->className(), item->metaObject()->className()));
                    }
                    QString type = childElement->getType();
                    bool implicit = childElement->getIsImplicit();
                    if(type.isEmpty()){
                        QString parameter = childElement->getParameter();
                        QString extends = childElement->getExtending();
                        if(parameter.isEmpty()){
                            TypesystemException::raise(QString("Attribute 'type' or 'parameter' required"));
                        }
                        mod.arguments << TS::Parameter{type,parameter,extends,implicit};
                    }else{
                        mod.arguments << TS::Parameter{type,{},{},implicit};
                    }
                }
            }else if(ModifyArgument* childElement = qobject_cast<ModifyArgument*>(item)){
                parseModifyArgument(childElement, mod);
            }else if(Rename* childElement = qobject_cast<Rename*>(item)){
                if (checkQtVersion(childElement)){
                    rename = childElement->getTo();
                    for(AbstractObject* item2 : item->childrenList()){
                        TypesystemException::raise(QStringLiteral("Unexpected element %1 as child of %2").arg(item2->metaObject()->className(), item->metaObject()->className()));
                    }
                }
            }else if(Access* childElement = qobject_cast<Access*>(item)){
                if (checkQtVersion(childElement)){
                    if(int(accesses)!=0){
                        TypesystemException::raise(QString("ACCESS is already specified as property"));
                    }
                    accesses = childElement->getModifier();
                    for(AbstractObject* item2 : item->childrenList()){
                        TypesystemException::raise(QStringLiteral("Unexpected element %1 as child of %2").arg(item2->metaObject()->className(), item->metaObject()->className()));
                    }
                }
            }else if(AddArgument* childElement = qobject_cast<AddArgument*>(item)){
                int index = childElement->getIndex();
                ArgumentModification argumentModification = ArgumentModification(index, ArgumentModification::AdditionalArgument);
                argumentModification.replaced_default_expression = childElement->getDefaultExpression();
                argumentModification.modified_name = childElement->getName();
                argumentModification.modified_type = childElement->getType();
                argumentModification.modified_jni_type = childElement->getJniType();
                argumentModification.reset_after_use = childElement->getInvalidateAfterUse();
                mod.argument_mods.append(argumentModification);
                for(AbstractObject* item2 : item->childrenList()){
                    TypesystemException::raise(QStringLiteral("Unexpected element %1 as child of %2").arg(item2->metaObject()->className(), item->metaObject()->className()));
                }
            }else if(AddTypeParameter* childElement = qobject_cast<AddTypeParameter*>(item)){
                if (checkQtVersion(childElement)){
                    ArgumentModification argumentModification = ArgumentModification(ArgumentModification::TypeParameter);
                    argumentModification.modified_name = childElement->getName();
                    argumentModification.modified_type = childElement->getExtending();
                    mod.argument_mods.append(argumentModification);
                    for(AbstractObject* item2 : item->childrenList()){
                        TypesystemException::raise(QStringLiteral("Unexpected element %1 as child of %2").arg(item2->metaObject()->className(), item->metaObject()->className()));
                    }
                }
            }else if(InjectCode* childElement = qobject_cast<InjectCode*>(item)){
                static const QHash<CodeClass::Entries, TS::Language> languageNames{
                    {CodeClass::Java, TS::TargetLangCode},
                    {CodeClass::JavaInterface, TS::Interface},
                    {CodeClass::Native, TS::NativeCode},
                    {CodeClass::Shell, TS::ShellCode},
                    {CodeClass::Signal, TS::Signal}
                };

                static const QHash<Position::Entries, CodeSnip::Position> positionNames{
                    {Position::Beginning, CodeSnip::Beginning},
                    {Position::Position1, CodeSnip::Position1},
                    {Position::Position2, CodeSnip::Position2},
                    {Position::Position3, CodeSnip::Position3},
                    {Position::Position4, CodeSnip::Position4},
                    {Position::Position5, CodeSnip::Position5},
                    {Position::Comment, CodeSnip::Comment},
                    {Position::End, CodeSnip::End}
                };
                parseInjectCode(childElement,
                                    languageNames,
                                    positionNames,
                                    [&mod](const QString&,const CodeSnip &snip){mod.snips << snip;},
                                    true);
            }else{
                TypesystemException::raise(QStringLiteral("Unexpected element %1 as child of %2").arg(item->metaObject()->className(), element->metaObject()->className()));
            }
        }

        if(accesses.testFlag(Modification::Private)){
            mod.modifiers |= TS::Modification::Private;
        }
        if(accesses.testFlag(Modification::Public)){
            mod.modifiers |= TS::Modification::Public;
        }
        if(accesses.testFlag(Modification::Protected)){
            mod.modifiers |= TS::Modification::Protected;
        }
        if(accesses.testFlag(Modification::Friendly)){
            mod.modifiers |= TS::Modification::Friendly;
        }
        if(accesses.testFlag(Modification::Final)){
            mod.modifiers |= TS::Modification::Final;
        }
        if(accesses.testFlag(Modification::DeclFinal)){
            mod.modifiers |= TS::Modification::NativeDeclFinal;
        }
        if(accesses.testFlag(Modification::NonFinal)){
            mod.modifiers |= TS::Modification::NonFinal;
        }

        if (!rename.isEmpty()) {
            mod.renamedToName = rename;
            mod.modifiers |= TS::Modification::Rename;
        }
    }
    return mod;
}

void QmlTypeSystemReaderPrivate::parseModifyFunction(ModifyFunction* element, TypeEntry* entry){
    if (checkQtVersion(element)){
        QString signature = element->getSignature();
        try{
            for(const NameSpacePrefix& prefix : qAsConst(m_namespacePrefixes)){
                if(prefix._namespace!=prefix.prefix)
                    signature = signature.replace(prefix.prefix, prefix._namespace);
            }
            if (signature.isEmpty()) {
                TypesystemException::raise(QString("No signature"));
            }
            signature = QString::fromLatin1(QMetaObject::normalizedSignature(qPrintable(signature)));
            AccessModifications accesses = element->getAccess();
            QString rename = element->getRename();
            FunctionModification mod;
            mod.removal = TS::NoLanguage;
            mod.signature = signature;
            mod.ppCondition = element->getPpCondition();
            mod.throws = element->getThrowing();
            mod.proxyCall = element->getProxyCall();
            mod.targetType = element->getTargetType();

            if (element->noKotlinGetter()) {
                mod.modifiers |= TS::Modification::NoKotlinGetter;
            }
            if (element->getDeprecated()) {
                mod.modifiers |= TS::Modification::Deprecated;
            }
            if (element->getNoExcept()) {
                mod.modifiers |= TS::Modification::NoExcept;
            }
            if (element->getIsForcedExplicit()) {
                mod.modifiers |= TS::Modification::ForcedExplicit;
            }
            if (element->getIsForcedImplicit()) {
                mod.modifiers |= TS::Modification::ForcedImplicit;
            }
            if (element->getNoImplicitArguments()) {
                mod.modifiers |= TS::Modification::NoImplicitArguments;
            }
            if (element->getBlockExceptions()) {
                mod.modifiers |= TS::Modification::BlockExcept;
            }
            if (element->getRethrowExceptions()) {
                mod.modifiers |= TS::Modification::RethrowExcept;
            }
            if (element->getVirtualSlot()) {
                mod.modifiers |= TS::Modification::VirtualSlot;
            }
            if (element->getAllowAsSlot()) {
                mod.modifiers |= TS::Modification::AllowAsSlot;
            }
            if (element->getIsPaintMethod()) {
                mod.modifiers |= TS::Modification::IsPaintMethod;
            }
            if (element->getPrivateSignal()) {
                mod.modifiers |= TS::Modification::PrivateSignal;
            }

            switch(element->getRemove()){
            case RemoveFlag::All:
                mod.removal = TS::All;
                break;
            case RemoveFlag::JavaAndNative:
                mod.removal = TS::TargetLangAndNativeCode;
                break;
            case RemoveFlag::JavaOnly:
                mod.removal = TS::TargetLangCode;
                break;
            case RemoveFlag::None:
                mod.removal = TS::NoLanguage;
                break;
            default:
                TypesystemException::raise(QString("Unsupported remove attribute"));
                break;
            }
            mod.association = element->getAssociatedTo();

            if(element->getThreadAffinity().userType()==QMetaType::Bool){
                if(element->getThreadAffinity().toBool()){
                    mod.modifiers |= TS::Modification::ThreadAffine;
                }
            }else if(element->getThreadAffinity().userType()==QMetaType::QString){
                if(element->getThreadAffinity().toString().toLower()=="ui")
                    mod.modifiers |= TS::Modification::UIThreadAffine;
                else if(element->getThreadAffinity().toString().toLower()=="pixmap")
                    mod.modifiers |= TS::Modification::PixmapThreadAffine;
            }else if(element->getThreadAffinity().canConvert<int>()){
                bool ok = false;
                int v = element->getThreadAffinity().toInt(&ok);
                if(ok){
                    switch(v){
                    case 0:
                        break;
                    case 1:
                        mod.modifiers |= TS::Modification::ThreadAffine;
                        break;
                    case Affinity::UI:
                        mod.modifiers |= TS::Modification::UIThreadAffine;
                        break;
                    case Affinity::Pixmap:
                        mod.modifiers |= TS::Modification::PixmapThreadAffine;
                        break;
                    default:
                        mod.modifiers |= TS::Modification::ThreadAffine;
                        break;
                    }
                }else{
                    mod.modifiers |= TS::Modification::ThreadAffine;
                }
            }

            const QList<AbstractObject*>& childrenList = element->childrenList();
            for(int i=0; i<childrenList.size(); ++i){
                AbstractObject* item = childrenList[i];
                if(ModifyArgument* childElement = qobject_cast<ModifyArgument*>(item)){
                    parseModifyArgument(childElement, mod);
                }else if(Delegate* childElement = qobject_cast<Delegate*>(item)){
                    parseDelegate(childElement, mod);
                }else if(Instantiation* childElement = qobject_cast<Instantiation*>(item)){
                    TemplateInstantiation templateInstantiation = parseInstantiation(childElement);
                    if(!templateInstantiation.arguments.isEmpty()){
                        for(const ArgumentModification& amod : mod.argument_mods){
                            bool found = false;
                            for(const ArgumentModification& amod2 : templateInstantiation.argument_mods){
                                if(amod.index==amod2.index){
                                    found = true;
                                    break;
                                }
                            }
                            if(!found)
                                templateInstantiation.argument_mods << amod;
                        }
                        mod.template_instantiations << templateInstantiation;
                    }
                }else if(Remove* childElement = qobject_cast<Remove*>(item)){
                    if (checkQtVersion(childElement)){
                        if(element->getRemove()!=RemoveFlag::None){
                            TypesystemException::raise(QString("REMOVE is already specified as attribute"));
                        }
                        switch(childElement->getCodeClass()){
                        case RemoveFlag::All:
                            mod.removal = TS::All;
                            break;
                        case RemoveFlag::JavaAndNative:
                            mod.removal = TS::TargetLangAndNativeCode;
                            break;
                        case RemoveFlag::JavaOnly:
                            mod.removal = TS::TargetLangCode;
                            break;
                        case RemoveFlag::None:
                            mod.removal = TS::NoLanguage;
                            break;
                        default:
                            TypesystemException::raise(QString("Unsupported codeClass attribute"));
                            break;
                        }
                        for(AbstractObject* item2 : item->childrenList()){
                            TypesystemException::raise(QStringLiteral("Unexpected element %1 as child of %2").arg(item2->metaObject()->className(), item->metaObject()->className()));
                        }
                    }
                }else if(Rename* childElement = qobject_cast<Rename*>(item)){
                    if (checkQtVersion(childElement)){
                        rename = childElement->getTo();
                        for(AbstractObject* item2 : item->childrenList()){
                            TypesystemException::raise(QStringLiteral("Unexpected element %1 as child of %2").arg(item2->metaObject()->className(), item->metaObject()->className()));
                        }
                    }
                }else if(Access* childElement = qobject_cast<Access*>(item)){
                    if (checkQtVersion(childElement)){
                        if(int(accesses)!=0){
                            TypesystemException::raise(QString("ACCESS is already specified as property"));
                        }
                        accesses = childElement->getModifier();
                        for(AbstractObject* item2 : item->childrenList()){
                            TypesystemException::raise(QStringLiteral("Unexpected element %1 as child of %2").arg(item2->metaObject()->className(), item->metaObject()->className()));
                        }
                    }
                }else if(AddArgument* childElement = qobject_cast<AddArgument*>(item)){
                    if (checkQtVersion(childElement)){
                        int index = childElement->getIndex();
                        ArgumentModification argumentModification = ArgumentModification(index, ArgumentModification::AdditionalArgument);
                        argumentModification.replaced_default_expression = childElement->getDefaultExpression();
                        argumentModification.modified_name = childElement->getName();
                        argumentModification.modified_type = childElement->getType();
                        argumentModification.modified_jni_type = childElement->getJniType();
                        argumentModification.reset_after_use = childElement->getInvalidateAfterUse();
                        mod.argument_mods.append(argumentModification);
                        for(AbstractObject* item2 : item->childrenList()){
                            TypesystemException::raise(QStringLiteral("Unexpected element %1 as child of %2").arg(item2->metaObject()->className(), item->metaObject()->className()));
                        }
                    }
                }else if(AddTypeParameter* childElement = qobject_cast<AddTypeParameter*>(item)){
                    if (checkQtVersion(childElement)){
                        ArgumentModification argumentModification = ArgumentModification(ArgumentModification::TypeParameter);
                        argumentModification.modified_name = childElement->getName();
                        argumentModification.modified_type = childElement->getExtending();
                        mod.argument_mods.append(argumentModification);
                        for(AbstractObject* item2 : item->childrenList()){
                            TypesystemException::raise(QStringLiteral("Unexpected element %1 as child of %2").arg(item2->metaObject()->className(), item->metaObject()->className()));
                        }
                    }
                }else if(InjectCode* childElement = qobject_cast<InjectCode*>(item)){
                    static const QHash<CodeClass::Entries, TS::Language> languageNames{
                        {CodeClass::Java, TS::TargetLangCode},
                        {CodeClass::JavaInterface, TS::Interface},
                        {CodeClass::Native, TS::NativeCode},
                        {CodeClass::Shell, TS::ShellCode},
                        {CodeClass::Signal, TS::Signal}
                    };

                    static const QHash<Position::Entries, CodeSnip::Position> positionNames{
                        {Position::Beginning, CodeSnip::Beginning},
                        {Position::Position1, CodeSnip::Position1},
                        {Position::Position2, CodeSnip::Position2},
                        {Position::Position3, CodeSnip::Position3},
                        {Position::Position4, CodeSnip::Position4},
                        {Position::Position5, CodeSnip::Position5},
                        {Position::Comment, CodeSnip::Comment},
                        {Position::End, CodeSnip::End}
                    };
                    parseInjectCode(childElement,
                                        languageNames,
                                        positionNames,
                                        [&mod](const QString&,const CodeSnip &snip){mod.snips << snip;},
                                        true);
                }else{
                    TypesystemException::raise(QStringLiteral("Unexpected element %1 as child of %2").arg(item->metaObject()->className(), element->metaObject()->className()));
                }
            }

            if(accesses.testFlag(Modification::Private)){
                mod.modifiers |= TS::Modification::Private;
            }
            if(accesses.testFlag(Modification::Public)){
                mod.modifiers |= TS::Modification::Public;
            }
            if(accesses.testFlag(Modification::Protected)){
                mod.modifiers |= TS::Modification::Protected;
            }
            if(accesses.testFlag(Modification::Friendly)){
                mod.modifiers |= TS::Modification::Friendly;
            }
            if(accesses.testFlag(Modification::Final)){
                mod.modifiers |= TS::Modification::Final;
            }
            if(accesses.testFlag(Modification::DeclFinal)){
                mod.modifiers |= TS::Modification::NativeDeclFinal;
            }
            if(accesses.testFlag(Modification::NonFinal)){
                mod.modifiers |= TS::Modification::NonFinal;
            }

            if (!rename.isEmpty()) {
                mod.renamedToName = rename;
                mod.modifiers |= TS::Modification::Rename;
            }
            if(entry->isComplex()){
                if(!mod.targetType.isEmpty()){
                    TypesystemException::raise(QString("Unexpected atribute targetType"));
                }
                reinterpret_cast<ComplexTypeEntry*>(entry)->addFunctionModification(mod);
                if(entry->designatedInterface())
                    entry->designatedInterface()->addFunctionModification(mod);
            }else if(entry->isTypeSystem()){
                if(mod.targetType.isEmpty() && mod.removal != TS::All){
                    TypesystemException::raise(QString("Either define target-type or remove all for global functions"));
                }
                reinterpret_cast<TypeSystemTypeEntry*>(entry)->addFunctionModification(mod);
            }
        }catch(const TypesystemException& exn){
            TypesystemException::raise(QString("%1 of function '%2'").arg(QLatin1String(exn.what()), signature));
        }
    }
}

void QmlTypeSystemReaderPrivate::parseModifyField(ModifyField* element, ComplexTypeEntry* entry){
    if (checkQtVersion(element)){
        FieldModification fm;
        fm.name = element->getName();
        try{
            fm.modifiers = 0;
            if (element->getRead()) {
                fm.modifiers |= TS::Modification::Readable;
            }
            if (element->getWrite()) {
                fm.modifiers |= TS::Modification::Writable;
            }
            if (element->noKotlinGetter()) {
                fm.modifiers |= TS::Modification::NoKotlinGetter;
            }
            QString rename = element->getRename();
            AccessModifications accesses = element->getAccess();

            const QList<AbstractObject*>& childrenList = element->childrenList();
            for(int i=0; i<childrenList.size(); ++i){
                AbstractObject* item = childrenList[i];
                if(Rename* childElement = qobject_cast<Rename*>(item)){
                    if (checkQtVersion(childElement)){
                        rename = childElement->getTo();
                        for(AbstractObject* item2 : item->childrenList()){
                            TypesystemException::raise(QStringLiteral("Unexpected element %1 as child of %2").arg(item2->metaObject()->className(), item->metaObject()->className()));
                        }
                    }
                }else if(Access* childElement = qobject_cast<Access*>(item)){
                    if (checkQtVersion(childElement)){
                        if(int(accesses)!=0){
                            TypesystemException::raise(QString("ACCESS is already specified as property"));
                        }
                        accesses = childElement->getModifier();
                        for(AbstractObject* item2 : item->childrenList()){
                            TypesystemException::raise(QStringLiteral("Unexpected element %1 as child of %2").arg(item2->metaObject()->className(), item->metaObject()->className()));
                        }
                    }
                }else if(ReplaceType* childElement = qobject_cast<ReplaceType*>(item)){
                    if (checkQtVersion(childElement)){
                        for(AbstractObject* item2 : item->childrenList()){
                            TypesystemException::raise(QStringLiteral("Unexpected element %1 as child of %2").arg(item2->metaObject()->className(), item->metaObject()->className()));
                        }
                        if (childElement->getModifiedType().isEmpty()) {
                            TypesystemException::raise(QString("Missing required property ReplaceType.modifiedType"));
                        }
                        fm.modified_type = childElement->getModifiedType();
                        fm.modified_jni_type = childElement->getModifiedJniType();

                    }
                }else if(ConversionRule* childElement = qobject_cast<ConversionRule*>(item)){
                    static const QHash<CodeClass::Entries, TS::Language> languageNames{
                        {CodeClass::NativeGetter, TS::Language(TS::Getter | TS::NativeCode)},
                        {CodeClass::NativeSetter, TS::Language(TS::Setter | TS::NativeCode)}
                    };
                    parseConversionRule(languageNames, childElement, [&fm](const CodeSnip& snip){fm.conversion_rules.append(snip);});
                }else if(ReferenceCount* childElement = qobject_cast<ReferenceCount*>(item)){
                    if (checkQtVersion(childElement)){
                        for(AbstractObject* item2 : item->childrenList()){
                            TypesystemException::raise(QStringLiteral("Unexpected element %1 as child of %2").arg(item2->metaObject()->className(), item->metaObject()->className()));
                        }
                        TS::ReferenceCount rc;
                        rc.threadSafe = childElement->getIsThreadSafe();
                        static const QHash<ReferenceCount::Action, TS::ReferenceCount::Action> actions{
                            {ReferenceCount::Put, TS::ReferenceCount::Put},
                            {ReferenceCount::Add, TS::ReferenceCount::Add},
                            {ReferenceCount::AddAll, TS::ReferenceCount::AddAll},
                            {ReferenceCount::ClearAdd, TS::ReferenceCount::ClearAdd},
                            {ReferenceCount::ClearAddAll, TS::ReferenceCount::ClearAddAll},
                            {ReferenceCount::Take, TS::ReferenceCount::Remove},
                            {ReferenceCount::Set, TS::ReferenceCount::Set},
                            {ReferenceCount::Ignore, TS::ReferenceCount::Ignore}
                        };
                        rc.action = actions[childElement->getAction()];
                        rc.variableName = childElement->getVariableName();
                        if (rc.action != TS::ReferenceCount::Ignore && rc.variableName.isEmpty()) {
                            TypesystemException::raise(QString("ReferenceCount.variableName must be specified"));
                        }
                        bool ok = false;
                        uint ka = childElement->getKeyArgument();
                        rc.keyArgument = ok ? ka : 0;
                        if (rc.action == TS::ReferenceCount::Put && rc.keyArgument<=0) {
                            TypesystemException::raise(QString("ReferenceCount.keyArgument must be specified and greater than 0"));
                        }
                        rc.declareVariable = childElement->getDeclareVariable();
                        rc.condition = childElement->getCondition();
                        if(childElement->getAccess().testFlag(Modification::Private))
                            rc.access = TS::ReferenceCount::Private;
                        else if(childElement->getAccess().testFlag(Modification::Protected))
                            rc.access = TS::ReferenceCount::Protected;
                        else if(childElement->getAccess().testFlag(Modification::Public))
                            rc.access = TS::ReferenceCount::Public;
                        else if(childElement->getAccess().testFlag(Modification::Friendly))
                            rc.access = TS::ReferenceCount::Friendly;
                        else
                            rc.access = TS::ReferenceCount::Private;
                        fm.referenceCounts.append(rc);
                    }
                }else if(NoNullPointer* childElement = qobject_cast<NoNullPointer*>(item)){
                    if (checkQtVersion(childElement)){
                        for(AbstractObject* item2 : item->childrenList()){
                            TypesystemException::raise(QStringLiteral("Unexpected element %1 as child of %2").arg(item2->metaObject()->className(), item->metaObject()->className()));
                        }
                        fm.no_null_pointers = true;
                    }
                }else if(DefineOwnership* childElement = qobject_cast<DefineOwnership*>(item)){
                    if (checkQtVersion(childElement)){
                        for(AbstractObject* item2 : item->childrenList()){
                            TypesystemException::raise(QStringLiteral("Unexpected element %1 as child of %2").arg(item2->metaObject()->className(), item->metaObject()->className()));
                        }

                        static const QHash<Ownership::Entries, TS::Ownership> ownershipNames{
                            {Ownership::Java, TS::TargetLangOwnership},
                            {Ownership::Cpp, TS::CppOwnership},
                            {Ownership::Default, TS::DefaultOwnership},
                            {Ownership::Ignore, TS::IgnoreOwnership},
                            {Ownership::Dependent, TS::RegisterDependency},
                            {Ownership::Invalidate, TS::Invalidate},
//                            {Ownership::Invalid, TS::InvalidOwnership}
                        };

                        if(!ownershipNames.contains(childElement->getOwnership()))
                            TypesystemException::raise(QString("Unsupported ownership property"));

                        static const QHash<CodeClass::Entries, TS::Language> languageNames{
                            {CodeClass::Java, TS::TargetLangCode},
                            {CodeClass::Native, TS::NativeCode},
                            {CodeClass::Shell, TS::ShellCode}
                        };
                        QHash<CodeClass::Entries, TS::Language> _languageNames = languageNames;
                        if(childElement->getOwnership()==Ownership::Ignore)
                            _languageNames.insert(CodeClass::NoLanguage, TS::NoLanguage);
                        if (!_languageNames.contains(childElement->getCodeClass()))
                            TypesystemException::raise(QString("Unsupported codeClass property"));
                        fm.ownerships[_languageNames[childElement->getCodeClass()]] = TS::OwnershipRule{ownershipNames[childElement->getOwnership()], childElement->getCondition()};
                    }
                }else{
                    TypesystemException::raise(QStringLiteral("Unexpected element %1 as child of %2").arg(item->metaObject()->className(), element->metaObject()->className()));
                }
            }

            if(accesses.testFlag(Modification::Private)){
                fm.modifiers |= TS::Modification::Private;
            }
            if(accesses.testFlag(Modification::Public)){
                fm.modifiers |= TS::Modification::Public;
            }
            if(accesses.testFlag(Modification::Protected)){
                fm.modifiers |= TS::Modification::Protected;
            }
            if(accesses.testFlag(Modification::Friendly)){
                fm.modifiers |= TS::Modification::Friendly;
            }
            if(accesses.testFlag(Modification::Final)){
                fm.modifiers |= TS::Modification::Final;
            }
            if(accesses.testFlag(Modification::DeclFinal)){
                fm.modifiers |= TS::Modification::NativeDeclFinal;
            }
            if(accesses.testFlag(Modification::NonFinal)){
                fm.modifiers |= TS::Modification::NonFinal;
            }
            if (!rename.isEmpty()) {
                fm.renamedToName = rename;
                fm.modifiers |= TS::Modification::Rename;
            }
            entry->addFieldModification(fm);
            if(entry->designatedInterface())
                entry->designatedInterface()->addFieldModification(fm);
        }catch(const TypesystemException& exn){
            TypesystemException::raise(QString("%1 of field '%2'").arg(QLatin1String(exn.what()), fm.name));
        }
    }
}

void QmlTypeSystemReaderPrivate::parseFunctionalType(const QString& nameSpace, FunctionalType* element){
    if (checkQtVersion(element)){
        QString name = element->getName();
        if(!nameSpace.isEmpty())
            name = nameSpace+"::"+name;
        try{
            QString targetName;
            for(const NameSpacePrefix& prefix : qAsConst(m_namespacePrefixes)){
                if(name.startsWith(prefix.prefix)){
                    name = name.mid(prefix.prefix.size());
                    if(prefix.namingPolicy==NameSpacePrefix::Cut)
                        targetName = name;
                    name = prefix._namespace + name;
                    break;
                }
            }
//            if(!nameSpace.isEmpty()){
//                if(targetName.endsWith(nameSpace+"::"+element->getName())){
//                    targetName = targetName.mid(0, targetName.length()-element->getName().length()-2) + "$" + element->getName();
//                }else if(name.endsWith(nameSpace+"::"+element->getName())){
//                    targetName = name.mid(0, name.length()-element->getName().length()-2) + "$" + element->getName();
//                }
//            }
            QStringList names = name.split(QLatin1String("::"));

            std::unique_ptr<FunctionalTypeEntry> fentry;
            if (names.size() == 1) {
                fentry.reset(new FunctionalTypeEntry(QString(), name));
                if(!targetName.isEmpty())
                    fentry->setTargetLangName(targetName.replace("::", "$"));
            } else {
                fentry.reset(new FunctionalTypeEntry(QStringList(names.mid(0, names.size() - 1)).join("::"), names.last()));
            }
            QVariant generate = element->getGenerate();
            fentry->setFunctionName(element->getFunctionName());
            if(m_generate==TypeEntry::GenerateAll){
                if(generate.userType()==QMetaType::QString && generate.value<QString>()=="no-shell"){
                    fentry->setCodeGeneration(TypeEntry::GenerateNoShell | TypeEntry::GenerateAll);
                }else if (generate.userType()==QMetaType::Bool && !generate.value<bool>())
                    fentry->setCodeGeneration(TypeEntry::GenerateNothing);
                else
                    fentry->setCodeGeneration(m_generate);
            }else
                fentry->setCodeGeneration(m_generate);

            if (element->getDisableNativeIdUsage())
                fentry->disableNativeIdUsage();
            {
                TS::Modification mod;
                if (element->getNoExcept()) {
                    mod.modifiers |= TS::Modification::NoExcept;
                }
                if (element->getBlockExceptions()) {
                    mod.modifiers |= TS::Modification::BlockExcept;
                }
                if (element->getRethrowExceptions()) {
                    mod.modifiers |= TS::Modification::RethrowExcept;
                }
                if(mod.modifiers)
                    fentry->addModification(mod);
            }
            fentry->setTargetLangPackage(element->getPackageName().isEmpty() ? m_defaultPackage : element->getPackageName());
            fentry->setTargetTypeSystem(m_defaultPackage);
            fentry->setImplements(element->getImplementing());
            fentry->setPPCondition(element->getPpCondition());
            fentry->setUsing(element->getUsing());
            if(!element->getJavaName().isEmpty())
                fentry->setTargetLangName(element->getJavaName());
            fentry->setCount(element->getCount());

            const QList<AbstractObject*>& childrenList = element->childrenList();
            for(int i=0; i<childrenList.size(); ++i){
                AbstractObject* item = childrenList[i];
                if(ModifyArgument* childElement = qobject_cast<ModifyArgument*>(item)){
                    FunctionModification mod;
                    mod.removal = TS::NoLanguage;
                    parseModifyArgument(childElement, mod);
                    for(const ArgumentModification& am : mod.argument_mods){
                        fentry->addArgumentModification(am);
                    }
                }else if(InjectCode* childElement = qobject_cast<InjectCode*>(item)){
                    parseInjectCode(childElement, fentry.get());
                }else if(ExtraIncludes* childElement = qobject_cast<ExtraIncludes*>(item)){
                    parseExtraIncludes(childElement, fentry.get());
                }else{
                    TypesystemException::raise(QStringLiteral("Unexpected element %1 as child of %2").arg(item->metaObject()->className(), element->metaObject()->className()));
                }
            }
            m_database->addType(fentry.release());
        }catch(const TypesystemException& exn){
            TypesystemException::raise(QString("%1 of type %2").arg(QLatin1String(exn.what()), name));
        }
    }
}

void QmlTypeSystemReaderPrivate::parseIteratorType(const QString& nameSpace, IteratorType* element){
    if (checkQtVersion(element)){
        QString name = element->getName();
        if(!nameSpace.isEmpty())
            name = nameSpace+"::"+name;
        try{
            for(const NameSpacePrefix& prefix : qAsConst(m_namespacePrefixes)){
                if(name.startsWith(prefix.prefix)){
                    name = name.mid(prefix.prefix.size());
                    name = prefix._namespace + name;
                    break;
                }
            }
            ComplexTypeEntry* containerType = nullptr;
            QStringList spl = name.split("::");
            spl.removeLast();
            TypeEntry *tmp = m_database->findType(spl.join("::"));
            if(tmp && tmp->isComplex()){
                containerType = static_cast<ComplexTypeEntry*>(tmp);
            }
            std::unique_ptr<IteratorTypeEntry> entry(new IteratorTypeEntry(name, containerType));
            entry->setGenericClass(element->getGenericClass());
            entry->setIsConst(element->getIsConst());
            entry->setTargetTypeSystem(m_defaultPackage);
            ReportHandler::debugTypes("Adding to TypeDatabase(2): " + entry->name());
            m_database->addType(entry.release());
            for(AbstractObject* item2 : element->childrenList()){
                TypesystemException::raise(QStringLiteral("Unexpected element %1 as child of %2").arg(item2->metaObject()->className(), element->metaObject()->className()));
            }
        }catch(const TypesystemException& exn){
            TypesystemException::raise(QString("%1 of type %2").arg(QLatin1String(exn.what()), name));
        }
    }
}

void QmlTypeSystemReaderPrivate::parseValueType(const QString& nameSpace, ValueType* element){
    if (checkQtVersion(element)){
        QString name = element->getName();
        if(!nameSpace.isEmpty())
            name = nameSpace+"::"+name;
        try{
            QString targetName;
            for(const NameSpacePrefix& prefix : qAsConst(m_namespacePrefixes)){
                if(name.startsWith(prefix.prefix)){
                    name = name.mid(prefix.prefix.size());
                    if(prefix.namingPolicy==NameSpacePrefix::Cut)
                        targetName = name;
                    name = prefix._namespace + name;
                    break;
                }
            }
            std::unique_ptr<ValueTypeEntry> entry(new ValueTypeEntry(name));
//            if(!nameSpace.isEmpty()){
//                if(targetName.endsWith(nameSpace+"::"+element->getName())){
//                    targetName = targetName.mid(0, targetName.length()-element->getName().length()-2) + "$" + element->getName();
//                }else if(name.endsWith(nameSpace+"::"+element->getName())){
//                    targetName = name.mid(0, name.length()-element->getName().length()-2) + "$" + element->getName();
//                }
//            }
            if(!targetName.isEmpty())
                entry->setTargetLangName(targetName.replace("::", "$"));
            entry->setIsPolymorphicBase(element->getIsPolymorphicBase());
            entry->setPolymorphicIdValue(element->getPolymorphicIdExpression());
            parseAttributesOfComplexType(element, entry.get());
            QList<AbstractObject*> unhandledElements = parseChildrenOfComplexType(nameSpace, element, entry.get());
            for(AbstractObject* childElement : qAsConst(unhandledElements)){
                TypesystemException::raise(QStringLiteral("Unexpected element %1 as child of %2").arg(childElement->metaObject()->className(), element->metaObject()->className()));
            }
            if(name.endsWith(">")){
                auto idx = name.indexOf('<');
                QString templateName = name.mid(0, idx);
                QStringList templateArguments = name.mid(idx+1).chopped(1).split(",");
                ComplexTypeEntry* templateType = m_database->findComplexType(templateName);
                if(templateType){
                    if(templateType->isTemplate()){
                        if(templateType->instantiations().contains(templateArguments) && templateType->instantiations()[templateArguments]==nullptr){
                            entry->setTargetLangName(templateType->targetLangName());
                            templateType->addInstantiation(templateArguments, entry.release());
                        }else{
                            TypesystemException::raise(QString("Template %1<%2> already defined").arg(templateName).arg(templateArguments.join(",")));
                        }
                    }else{
                        TypesystemException::raise(QString("Type %1 not a template").arg(templateName));
                    }
                }else{
                    TypesystemException::raise(QString("Template %1 not found").arg(templateName));
                }
            }else{
                ReportHandler::debugTypes("Adding to TypeDatabase(2): " + name);
                m_database->addType(entry.release());
            }
        }catch(const TypesystemException& exn){
            TypesystemException::raise(QString("%1 of type %2").arg(QLatin1String(exn.what()), name));
        }
    }
}

void QmlTypeSystemReaderPrivate::parseTypeAliasType(const QString& nameSpace, TypeAliasType* element){
    if (checkQtVersion(element)){
        QString name = element->getName();
        if(!nameSpace.isEmpty())
            name = nameSpace+"::"+name;
        try{
            for(const NameSpacePrefix& prefix : qAsConst(m_namespacePrefixes)){
                if(name.startsWith(prefix.prefix)){
                    name = name.mid(prefix.prefix.size());
                    name = prefix._namespace + name;
                    break;
                }
            }
            std::unique_ptr<AliasTypeEntry> entry(new AliasTypeEntry(name, m_defaultPackage));
            entry->setTargetTypeSystem(m_defaultPackage);
            for(AbstractObject* item2 : element->childrenList()){
                TypesystemException::raise(QStringLiteral("Unexpected element %1 as child of %2").arg(item2->metaObject()->className(), element->metaObject()->className()));
            }
            if(name.endsWith(">")){
                auto idx = name.indexOf('<');
                QString templateName = name.mid(0, idx);
                QStringList templateArguments = name.mid(idx+1).chopped(1).split(",");
                ComplexTypeEntry* templateType = m_database->findComplexType(templateName);
                if(templateType){
                    if(templateType->isTemplate()){
                        if(templateType->instantiations().contains(templateArguments) && templateType->instantiations()[templateArguments]==nullptr){
                            entry->setTargetLangName(templateType->targetLangName());
                            templateType->addInstantiation(templateArguments, entry.release());
                        }else{
                            TypesystemException::raise(QString("Template %1<%2> already defined").arg(templateName).arg(templateArguments.join(",")));
                        }
                    }else{
                        TypesystemException::raise(QString("Type %1 not a template").arg(templateName));
                    }
                }else{
                    TypesystemException::raise(QString("Template %1 not found").arg(templateName));
                }
            }else{
                ReportHandler::debugTypes("Adding to TypeDatabase(2): " + entry->name());
                m_database->addType(entry.release());
            }
        }catch(const TypesystemException& exn){
            TypesystemException::raise(QString("%1 of type %2").arg(QLatin1String(exn.what()), name));
        }
    }
}

void QmlTypeSystemReaderPrivate::parseInterfaceType(const QString& nameSpace, InterfaceType* element){
    if (checkQtVersion(element)){
        QString name = element->getName();
        if(!nameSpace.isEmpty())
            name = nameSpace+"::"+name;
        try{
            QString implName = InterfaceTypeEntry::implName(name);
            QString targetName;
            for(const NameSpacePrefix& prefix : qAsConst(m_namespacePrefixes)){
                if(name.startsWith(prefix.prefix)){
                    name = name.mid(prefix.prefix.size());
                    if(prefix.namingPolicy==NameSpacePrefix::Cut){
                        targetName = name;
                        implName = InterfaceTypeEntry::implName(targetName);
                    }
                    name = prefix._namespace + name;
                    break;
                }
            }
            std::unique_ptr<ImplementorTypeEntry> otype;
            if(element->getIsValue()){
                otype.reset(new ValueTypeEntry(implName));
            }else
                otype.reset(new ObjectTypeEntry(implName));
            otype->setSkipMetaTypeRegistration(element->getNoMetaType());
            std::unique_ptr<InterfaceTypeEntry> itype(new InterfaceTypeEntry(name));
            if(!targetName.isEmpty())
                itype->setTargetLangName(targetName);
            otype->setDesignatedInterface(itype.get());
            itype->setOrigin(otype.get());
            itype->setNoImpl(element->getNoImpl());
            parseAttributesOfComplexType(element, otype.get());
            QList<AbstractObject*> unhandledElements = parseChildrenOfComplexType(nameSpace, element, otype.get());
            for(AbstractObject* childElement : qAsConst(unhandledElements)){
                TypesystemException::raise(QStringLiteral("Unexpected element %1 as child of %2").arg(childElement->metaObject()->className(), element->metaObject()->className()));
            }
            m_database->addType(otype.release());
            itype.release();
        }catch(const TypesystemException& exn){
            TypesystemException::raise(QString("%1 of type %2").arg(QLatin1String(exn.what()), name));
        }
    }
}

void QmlTypeSystemReaderPrivate::parseNamespaceType(const QString& nameSpace, NamespaceType* element){
    if (checkQtVersion(element)){
        QString name = element->getName();
        if(!nameSpace.isEmpty())
            name = nameSpace+"::"+name;
        try{
            QString targetName;
            for(const NameSpacePrefix& prefix : qAsConst(m_namespacePrefixes)){
                if(name.startsWith(prefix.prefix) && name!=prefix.prefix){
                    name = name.mid(prefix.prefix.size());
                    if(prefix.namingPolicy==NameSpacePrefix::Cut)
                        targetName = name;
                    name = prefix._namespace + name;
                    break;
                }
            }
//            if(!nameSpace.isEmpty()){
//                if(targetName.endsWith(nameSpace+"::"+element->getName())){
//                    targetName = targetName.mid(0, targetName.length()-element->getName().length()-2) + "$" + element->getName();
//                }else if(name.endsWith(nameSpace+"::"+element->getName())){
//                    targetName = name.mid(0, name.length()-element->getName().length()-2) + "$" + element->getName();
//                }
//            }
            std::unique_ptr<NamespaceTypeEntry> entry(new NamespaceTypeEntry(name, qobject_cast<HeaderType*>(element)!=nullptr, element->hasMetaObject()));
            if(!targetName.isEmpty())
                entry->setTargetLangName(targetName.replace("::", "$"));
            parseAttributesOfComplexType(element, entry.get());
            QList<AbstractObject*> unhandledElements = parseChildrenOfComplexType(nameSpace, element, entry.get());
            for(AbstractObject* childElement : qAsConst(unhandledElements)){
                TypesystemException::raise(QStringLiteral("Unexpected element %1 as child of %2").arg(childElement->metaObject()->className(), element->metaObject()->className()));
            }
            ReportHandler::debugTypes("Adding to TypeDatabase(2): " + entry->name());
            m_database->addType(entry.release());
        }catch(const TypesystemException& exn){
            TypesystemException::raise(QString("%1 of type %2").arg(QLatin1String(exn.what()), name));
        }
    }
}

void QmlTypeSystemReaderPrivate::parseEnumType(const QString& nameSpace, EnumType* element){
    if (checkQtVersion(element)){
        QString name = element->getName();
        if(!nameSpace.isEmpty())
            name = nameSpace+"::"+name;
        try{
            for(const NameSpacePrefix& prefix : qAsConst(m_namespacePrefixes)){
                if(name.startsWith(prefix.prefix)){
                    name = name.mid(prefix.prefix.size());
                    name = prefix._namespace + name;
                    break;
                }
            }
            QStringList names = name.split(QLatin1String("::"));
            std::unique_ptr<EnumTypeEntry> eentry;
            if (names.size() == 1) {
                eentry.reset(new EnumTypeEntry(QString(), name));
            } else {
                eentry.reset(new EnumTypeEntry(QStringList(names.mid(0, names.size() - 1)).join("::"), names.last()));
            }
            if(m_generate==TypeEntry::GenerateAll){
                if (!element->getGenerate())
                    eentry->setCodeGeneration(TypeEntry::GenerateNothing);
                else
                    eentry->setCodeGeneration(m_generate);
            }else
                eentry->setCodeGeneration(m_generate);
            eentry->setTargetLangPackage(element->getPackageName().isEmpty() ? m_defaultPackage : element->getPackageName());
            eentry->setTargetTypeSystem(m_defaultPackage);
            eentry->setUpperBound(element->getUpperBound());
            eentry->setLowerBound(element->getLowerBound());
            eentry->setJavaScope(element->getJavaScope());
            eentry->setImplements(element->getImplementing());
            eentry->setForceInteger(element->getForceInteger());
            eentry->setExtensible(element->getExtensible());
            eentry->setHiddenMetaObject(element->getHiddenMetaobject());
            eentry->setPPCondition(element->getPpCondition());

            const QList<AbstractObject*>& childrenList = element->childrenList();
            for(int i=0; i<childrenList.size(); ++i){
                AbstractObject* item = childrenList[i];
                if(RejectEnumValue* childElement = qobject_cast<RejectEnumValue*>(item)){
                    if (checkQtVersion(childElement)){
                        QString name = childElement->getName();
                        if (name.isEmpty()) {
                            TypesystemException::raise(QString("No 'name' attribute specified for RejectEnumValue"));
                        }
                        eentry->addEnumValueRejection(name, childElement->getRemove());
                    }
                }else if(RenameEnumValue* childElement = qobject_cast<RenameEnumValue*>(item)){
                    if (checkQtVersion(childElement)){
                        QString name = childElement->getName();
                        if (name.isEmpty()) {
                            TypesystemException::raise(QString("No 'name' attribute specified for RenameEnumValue"));
                        }
                        QString rename = childElement->getRename();
                        if (rename.isEmpty()) {
                            TypesystemException::raise(QString("No 'rename' attribute specified for RenameEnumValue"));
                        }
                        eentry->addRenamedEnumValue(name, rename);
                    }
                }else if(InjectCode* childElement = qobject_cast<InjectCode*>(item)){
                    static const QHash<CodeClass::Entries, TS::Language> languageNames{
                        {CodeClass::Java, TS::TargetLangCode},
                        {CodeClass::JavaInterface, TS::Interface},
                        {CodeClass::Native, TS::NativeCode},
                        {CodeClass::Shell, TS::ShellCode},
                        {CodeClass::Signal, TS::Signal}
                    };

                    static const QHash<Position::Entries, CodeSnip::Position> positionNames{
                        {Position::Beginning, CodeSnip::Beginning},
                        {Position::Position1, CodeSnip::Position1},
                        {Position::Position2, CodeSnip::Position2},
                        {Position::Position3, CodeSnip::Position3},
                        {Position::Position4, CodeSnip::Position4},
                        {Position::Position5, CodeSnip::Position5},
                        {Position::Comment, CodeSnip::Comment},
                        {Position::End, CodeSnip::End}
                    };
                    parseInjectCode(childElement,
                                        languageNames,
                                        positionNames,
                                        [&eentry](const QString&,const CodeSnip &snip){eentry->addCodeSnip(snip);},
                                        true);
                }
            }

            // put in the flags parallel...
            QString flags = element->getFlags();
            if (!flags.isEmpty() && flags.toLower() != "no") {
                for(const NameSpacePrefix& prefix : qAsConst(m_namespacePrefixes)){
                    if(flags.startsWith(prefix.prefix)){
                        flags = flags.mid(prefix.prefix.size());
                        flags = prefix._namespace + flags;
                        break;
                    }
                    break;
                }
                std::unique_ptr<FlagsTypeEntry> ftype(new FlagsTypeEntry(flags));
                ftype->setOriginator(eentry.get());
                ftype->setFlagsTemplate("QFlags<" + eentry->qualifiedCppName() + ">");
                ftype->setCodeGeneration(eentry->codeGeneration());
                QString n = flags;

                QStringList lst = n.split("::");
                if (QStringList(lst.mid(0, lst.size() - 1)).join("::") != QStringList(names.mid(0, names.size() - 1)).join("::")) {
                    ReportHandler::warning(QString("Enum %1 and flags %2 differ in qualifiers")
                                           .arg(eentry->qualifiedCppName()));
                }

                ftype->setFlagsName(lst.last());
                eentry->setFlags(ftype.get());

                m_database->addFlagsType(ftype.get());
                //qDebug()<<"Adding ftype"<<ftype->name();
                ReportHandler::debugTypes("Adding to TypeDatabase(1): " + ftype->name());
                m_database->addType(ftype.release());
            }else if(flags.toLower() == "no" || flags.toLower() == "false"){
                eentry->setForceNoFlags(true);
            }
            m_database->addType(eentry.release());
        }catch(const TypesystemException& exn){
            TypesystemException::raise(QString("%1 of type %2").arg(QLatin1String(exn.what()), name));
        }
    }
}
