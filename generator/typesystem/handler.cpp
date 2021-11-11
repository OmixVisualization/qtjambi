/****************************************************************************
**
** Copyright (C) 2013-2015 Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
** (in parts)
**
** This file is part of Qt Jambi.
**
** ** $BEGIN_LICENSE$
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

#include "handler.h"
#include "typedatabase.h"
#include "wrapper.h"			/* for isTargetPlatformArmCpu */
#include "../reporthandler.h"
#include "../main.h"

int ArgumentModification::ADDED_ARGUMENT = -12345;

void Handler::parse(const QString &filepath){
    QFile file(filepath);
    Q_ASSERT(file.exists());
    QDomDocument document;
    QString errorMsg;
    int errorLine = -1;
    int errorColumn = -1;
    try{
        if(file.open(QIODevice::ReadOnly) && document.setContent(&file, true, &errorMsg, &errorLine, &errorColumn)){
            QDomElement documentElement = document.documentElement();
            if(documentElement.localName()=="typesystem"){
                parseTypeSystem(documentElement);
                for(TypeSystemTypeEntry* entry : m_requiredModules.keys()){
                    for(const QString& libraryName : m_requiredModules.values(entry)){
                        if(TypeSystemTypeEntry* ts = TypeDatabase::instance()->findTypeSystem(libraryName)){
                            entry->addRequiredTypeSystem(ts);
                        }else{
                            TypesystemException::raise(QString("Unable to find required package %1").arg(libraryName));
                        }
                    }
                }
            }else{
                TypesystemException::raise(QString("Unexpected tag: <%1>. Expected: <typesystem>").arg(documentElement.localName()));
            }
        }else{
            TypesystemException::raise(QString("XML Error: %1 (%2:%3)").arg(errorMsg).arg(errorLine).arg(errorColumn));
        }
    }catch(...){
        file.close();
        throw;
    }
}

void Handler::parseTypeSystem(const QDomElement &element){
    QDomNamedNodeMap attributes = element.attributes();
    if (checkQtVersion(attributes)){
        QList<NamespacePrefix> currentNamespacePrefixes = m_namespacePrefixes;
        m_namespacePrefixes.clear();
        m_defaultPackage = attributeValue(attributes.removeNamedItem("package"));
        m_defaultSuperclass = attributeValue(attributes.removeNamedItem("default-superclass"));
        QString moduleName = attributeValue(attributes.removeNamedItem("module"));
        QString description = attributeValue(attributes.removeNamedItem("description"));
        if(element.hasChildNodes()){
            QDomNodeList childNodes = element.childNodes();
            for(int i=0; i<childNodes.length(); ++i){
                QDomNode item = childNodes.item(i);
                switch(item.nodeType()){
                case QDomNode::ElementNode:
                {
                    QDomElement childElement = item.toElement();
                    if(childElement.localName()=="module"){
                        QDomNamedNodeMap attributes = childElement.attributes();
                        if (checkQtVersion(attributes)){
                            if(!moduleName.isEmpty())
                                TypesystemException::raise(QString("Duplicate module name specification in line %1").arg(childElement.lineNumber()));
                            moduleName = attributeValue(attributes.removeNamedItem("name"));
                            if(attributes.count()){
                                TypesystemException::raise(QString("Unexpected attribute '%2' of tag <%1> in line %3").arg(childElement.localName()).arg(attributes.item(0).localName()).arg(element.lineNumber()));
                            }
                        }
                    }else if(childElement.localName()=="description"){
                        QDomNamedNodeMap attributes = childElement.attributes();
                        if (checkQtVersion(attributes)){
                            if(!description.isEmpty())
                                TypesystemException::raise(QString("Duplicate description specification in line %1").arg(childElement.lineNumber()));
                            if(attributes.count()){
                                TypesystemException::raise(QString("Unexpected attribute '%2' of tag <%1> in line %3").arg(childElement.localName()).arg(attributes.item(0).localName()).arg(element.lineNumber()));
                            }
                            QDomNodeList childNodes = childElement.childNodes();
                            for(int i=0; i<childNodes.length(); ++i){
                                QDomNode item = childNodes.item(i);
                                switch(item.nodeType()){
                                case QDomNode::TextNode:
                                    description = item.toText().data();
                                    break;
                                case QDomNode::CDATASectionNode:
                                    description = item.toCDATASection().data();
                                    break;
                                case QDomNode::CharacterDataNode:
                                    description = item.toCharacterData().data();
                                    break;
                                default:
                                    TypesystemException::raise(QString("Unexpected content in tag <%1> in line %2").arg(element.localName()).arg(item.lineNumber()));
                                    break;
                                }
                            }
                        }
                    }
                }
                    break;
                default: break;
                }
            }
        }
        TypeSystemTypeEntry* entry = new TypeSystemTypeEntry(m_defaultPackage,
                                                             attributeValue(attributes.removeNamedItem("qt-library")),
                                                             moduleName);
        entry->setDescription(description);
        entry->setCodeGeneration(m_generate);
        //qDebug()<<"Adding element->entry (root)"<<element->entry->name();
        ReportHandler::debugTypes("Adding to TypeDatabase(3): " + entry->name());
        TypeDatabase::instance()->addType(entry);
        if(attributes.count()){
            TypesystemException::raise(QString("Unexpected attribute '%2' of tag <%1> in line %3").arg(element.localName()).arg(attributes.item(0).localName()).arg(element.lineNumber()));
        }
        if(element.hasChildNodes()){
            QDomNodeList childNodes = element.childNodes();
            for(int i=0; i<childNodes.length(); ++i){
                QDomNode item = childNodes.item(i);
                switch(item.nodeType()){
                case QDomNode::ElementNode:
                {
                    QDomElement childElement = item.toElement();
                    if(childElement.localName()=="module" || childElement.localName()=="description"){
                        // already handled above
                    }else if(childElement.localName()=="required-library"){
                        QDomNamedNodeMap attributes = childElement.attributes();
                        if (checkQtVersion(attributes)){
                            QString libraryName = attributeValue(attributes.removeNamedItem("name"));
                            bool optional = convertBoolean(attributeValue(attributes.removeNamedItem("optional"), "false"), "optional", false);
                            if(attributes.count()){
                                TypesystemException::raise(QString("Unexpected attribute '%2' of tag <%1> in line %3").arg(childElement.localName()).arg(attributes.item(0).localName()).arg(element.lineNumber()));
                            }
                            if(libraryName.isEmpty())
                                ReportHandler::warning("required-library with no name specified");
                            entry->addRequiredQtLibrary(libraryName, optional);
                        }
                    }else if(childElement.localName()=="required-package"){
                        QDomNamedNodeMap attributes = childElement.attributes();
                        if (checkQtVersion(attributes)){
                            QString libraryName = attributeValue(attributes.removeNamedItem("name"));
                            if(attributes.count()){
                                TypesystemException::raise(QString("Unexpected attribute '%2' of tag <%1> in line %3").arg(childElement.localName()).arg(attributes.item(0).localName()).arg(element.lineNumber()));
                            }
                            if(libraryName.isEmpty())
                                ReportHandler::warning("required-module with no name specified");
                            if(TypeSystemTypeEntry* ts = TypeDatabase::instance()->findTypeSystem(libraryName)){
                                entry->addRequiredTypeSystem(ts);
                            }else{
                                m_requiredModules.insert(entry, libraryName);
                            }
                        }
                    }else if(childElement.localName()=="forward-declaration"){
                        QDomNamedNodeMap attributes = childElement.attributes();
                        if (checkQtVersion(attributes)){
                            QString className = attributeValue(attributes.removeNamedItem("class"));
                            if(attributes.count()){
                                TypesystemException::raise(QString("Unexpected attribute '%2' of tag <%1> in line %3").arg(childElement.localName()).arg(attributes.item(0).localName()).arg(element.lineNumber()));
                            }
                            if(className.isEmpty())
                                ReportHandler::warning("forward-declaration with no class specified");
                            entry->addForwardDeclaration(className);
                        }
                    }else if(childElement.localName()=="namespace-prefix"){
                        QDomNamedNodeMap attributes = childElement.attributes();
                        if (checkQtVersion(attributes)){
                            QString prefix = attributeValue(attributes.removeNamedItem("prefix"));
                            QString _namespace = attributeValue(attributes.removeNamedItem("namespace"));
                            NamespacePrefix::NamingPolicy namingPolicy = NamespacePrefix::Preserve;
                            QString _namingPolicy = attributeValue(attributes.removeNamedItem("naming-policy"));
                            if(!_namingPolicy.isEmpty()){
                                static const QHash<QString, NamespacePrefix::NamingPolicy> namingPolicies{
                                    {"preserve", NamespacePrefix::Preserve},
                                    {"cut", NamespacePrefix::Cut}};
                                if (!namingPolicies.contains(_namingPolicy)) {
                                    TypesystemException::raise(QString("Invalid java-naming-policy specifier '%1' of tag <%2> in line %3").arg(_namingPolicy).arg(childElement.localName()).arg(childElement.lineNumber()));
                                }
                                namingPolicy = namingPolicies[_namingPolicy];
                            }
                            if(!prefix.isEmpty() && !prefix.endsWith("::"))
                                prefix += "::";
                            if(!_namespace.isEmpty() && !_namespace.endsWith("::"))
                                _namespace += "::";
                            m_namespacePrefixes.append({prefix, _namespace, namingPolicy});
                            QDomNodeList _childNodes = childElement.childNodes();
                            for(int i=0; i<_childNodes.length(); ++i){
                                QDomNode item = _childNodes.item(i);
                                switch(item.nodeType()){
                                case QDomNode::ElementNode:
                                    TypesystemException::raise(QString("Unexpected tag <%1> as child of tag <%2> in line %3").arg(item.toElement().localName()).arg(childElement.localName()).arg(item.lineNumber()));
                                    break;
                                case QDomNode::TextNode:
                                    TypesystemException::raise(QString("Unexpected text content as child of tag <%1> in line %2").arg(childElement.localName()).arg(item.lineNumber()));
                                    break;
                                case QDomNode::CharacterDataNode:
                                case QDomNode::CDATASectionNode:
                                    TypesystemException::raise(QString("Unexpected CDATA as child of tag <%1> in line %2").arg(childElement.localName()).arg(item.lineNumber()));
                                    break;
                                case QDomNode::EntityNode:
                                    TypesystemException::raise(QString("Unexpected entity as child of tag <%1> in line %2").arg(childElement.localName()).arg(item.lineNumber()));
                                    break;
                                case QDomNode::NotationNode:
                                    TypesystemException::raise(QString("Unexpected notation as child of tag <%1> in line %2").arg(childElement.localName()).arg(item.lineNumber()));
                                    break;
                                default: break;
                                }
                            }
                        }
                    }else if(childElement.localName()=="load-typesystem"){
                        parseLoadTypeSystem(childElement);
                    }else if(childElement.localName()=="rejection"){
                        parseRejection(childElement);
                    }else if(childElement.localName()=="primitive-type"){
                        parsePrimitiveType(childElement);
                    }else if(childElement.localName()=="object-type"){
                        parseObjectType(childElement);
                    }else if(childElement.localName()=="template-type"){
                        parseTemplateType(childElement);
                    }else if(childElement.localName()=="functional-type"){
                        parseFunctionalType(childElement);
                    }else if(childElement.localName()=="iterator-type"){
                        parseIteratorType(childElement);
                    }else if(childElement.localName()=="value-type"){
                        parseValueType(childElement);
                    }else if(childElement.localName()=="typealias-type"){
                        parseTypeAliasType(childElement);
                    }else if(childElement.localName()=="interface-type"){
                        parseInterfaceType(childElement);
                    }else if(childElement.localName()=="namespace-type"){
                        parseNamespaceType(childElement);
                    }else if(childElement.localName()=="enum-type"){
                        parseEnumType(childElement);
                    }else if(childElement.localName()=="extra-includes"){
                        parseExtraIncludes(childElement, entry);
                    }else if(childElement.localName()=="inject-code"){
                        static const QHash<QString, TypeSystem::Language> languageNames{
                            {"java", TypeSystem::TargetLangCode},
                            {"native", TypeSystem::NativeCode},
                            {"module-info", TypeSystem::ModuleInfo},
                //            {"shell", TypeSystem::ShellCode},
                //            {"shell-declaration", TypeSystem::ShellDeclaration},
                            {"library-initializer", TypeSystem::PackageInitializer},
                //            {"destructor-function", TypeSystem::DestructorFunction},
                            {"metainfo", TypeSystem::MetaInfo},
                //            {"constructors", TypeSystem::Constructors},
                //            {"interface", TypeSystem::Interface},
                //            {"signal", TypeSystem::Signal}
                        };

                        static const QHash<QString, CodeSnip::Position> positionNames{
                            {"begin", CodeSnip::Beginning},
                            {"beginning", CodeSnip::Beginning},
                            {"position1", CodeSnip::Position1},
                            {"position2", CodeSnip::Position2},
                            {"position3", CodeSnip::Position3},
                            {"position4", CodeSnip::Position4},
                            {"1", CodeSnip::Position1},
                            {"2", CodeSnip::Position2},
                            {"3", CodeSnip::Position3},
                            {"4", CodeSnip::Position4},
                            {"end", CodeSnip::End}
                        };
                        parseInjectCode(childElement, languageNames, positionNames, [&entry](const CodeSnip &snip){entry->addCodeSnip(snip);});
                    }else if(childElement.localName()=="suppress-warning"){
                        parseSuppressedWarning(childElement);
                    }else if(childElement.localName()=="template"){
                        parseTemplate(childElement);
                    }else{
                        TypesystemException::raise(QString("Unexpected tag <%2> as child of tag <%1> in line %3").arg(element.localName()).arg(childElement.localName()).arg(childElement.lineNumber()));
                    }
                }
                    break;
                case QDomNode::TextNode:
                    TypesystemException::raise(QString("Unexpected text content as child of tag <%1> in line %2").arg(element.localName()).arg(item.lineNumber()));
                    break;
                case QDomNode::CharacterDataNode:
                case QDomNode::CDATASectionNode:
                    TypesystemException::raise(QString("Unexpected CDATA as child of tag <%1> in line %2").arg(element.localName()).arg(item.lineNumber()));
                    break;
                case QDomNode::EntityNode:
                    TypesystemException::raise(QString("Unexpected entity as child of tag <%1> in line %2").arg(element.localName()).arg(item.lineNumber()));
                    break;
                case QDomNode::NotationNode:
                    TypesystemException::raise(QString("Unexpected notation as child of tag <%1> in line %2").arg(element.localName()).arg(item.lineNumber()));
                    break;
                default: break;
                }
            }
        }
        m_namespacePrefixes = currentNamespacePrefixes;
    }
}

bool Handler::checkQtVersion(QDomNamedNodeMap& attributes){
    uint since = parseQtVersion(attributeValue(attributes.removeNamedItem("since")), 0x000000);
    uint until = parseQtVersion(attributeValue(attributes.removeNamedItem("until")), 0xffffff);
    return since<=m_qtVersion && until>=m_qtVersion;
}

bool Handler::convertBoolean(const QString &_value, const QString &attributeName, bool defaultValue) {
    QString value = _value.toLower();
    if (value == "true" || value == "yes") {
        return true;
    } else if (value == "false" || value == "no") {
        return false;
    } else {
        QString warn = QString("Boolean value '%1' not supported in attribute '%2'. Use 'yes' or 'no'. Defaulting to '%3'.")
                       .arg(value).arg(attributeName).arg(defaultValue ? "yes" : "no");

        ReportHandler::warning(warn);
        return defaultValue;
    }
}

uint Handler::parseQtVersion(QString strg, uint defaultValue){
    if(!strg.isNull() && !strg.isEmpty()){
        bool ok = false;
        if(strg.startsWith("Qt")){
            strg.remove(0, 2);
            strg = strg.trimmed();
        }
        QStringList split = strg.split(QChar('.'));
        if(split.size()==1){
            uint value = split[0].toUInt(&ok);
            if(ok){
                if(value<0x00ffff){
                    return (value << 16) | (defaultValue & 0x00ff00) | (defaultValue & 0x0000ff);
                }else{
                    return value;
                }
            }
        }else{
            uint major = split[0].toUInt(&ok);
            if(ok){
                major <<= 16;
            }else{
                major = defaultValue & 0xff0000;
            }
            uint minor = split[1].toUInt(&ok);
            if(ok){
                minor <<= 8;
            }else{
                minor = defaultValue & 0x00ff00;
            }
            uint patch;
            if(split.size()>2){
                patch = split[2].toUInt(&ok);
                if(!ok){
                    patch = defaultValue & 0x0000ff;
                }
            }else{
                patch = defaultValue & 0x0000ff;
            }
            return major | minor | patch;
        }
    }
    return defaultValue;
}

QString Handler::attributeValue(const QDomNode& domNode, const QString & defaultValue){
    if(domNode.isAttr()){
        return domNode.toAttr().value();
    }else{
        return defaultValue;
    }
}

void Handler::parseSuppressedWarning(const QDomElement &element){
    QDomNamedNodeMap attributes = element.attributes();
    if (checkQtVersion(attributes)){
        QString text = attributeValue(attributes.removeNamedItem("text"));
        if(attributes.count()){
            TypesystemException::raise(QString("Unexpected attribute '%2' of tag <%1> in line %3").arg(element.localName()).arg(attributes.item(0).localName()).arg(element.lineNumber()));
        }
        ensureNoChildren(element);
        if (text.isEmpty())
            ReportHandler::warning("Suppressed warning with no text specified");
        else
            m_database->addSuppressedWarning(text);
    }
}

Include Handler::parseInclude(const QDomElement &element){
    Include incl;
    QDomNamedNodeMap attributes = element.attributes();
    if (checkQtVersion(attributes)){
        QString location = attributeValue(attributes.removeNamedItem("location")).toLower();
        QString fileName = attributeValue(attributes.removeNamedItem("file-name"));
        incl.inherited = convertBoolean(attributeValue(attributes.removeNamedItem("inherited"), "false"), "inherited", false);
        incl.suppressed = convertBoolean(attributeValue(attributes.removeNamedItem("suppressed"), "no"), "suppressed", false);
        if(attributes.count()){
            TypesystemException::raise(QString("Unexpected attribute '%2' of tag <%1> in line %3").arg(element.localName()).arg(attributes.item(0).localName()).arg(element.lineNumber()));
        }
        QMap<QString,QString> requiredFeatures;
        for(const QString& feature : attributeValue(attributes.removeNamedItem("required-features")).split(",")){
            if(!feature.isEmpty())
                requiredFeatures[feature] = "";
        }

        static const QHash<QString, Include::IncludeType> locationNames{
            {"global", Include::IncludePath},
            {"local", Include::LocalPath},
            {"java", Include::TargetLangImport}
        };

        if (!locationNames.contains(location)) {
            TypesystemException::raise(QString("Location not recognized: '%2' referred by <%1> in line %3").arg(element.localName()).arg(location).arg(element.lineNumber()));
        }
        incl.type = locationNames[location];
        incl.name = fileName;
    }
    return incl;
}

void Handler::parseExtraIncludes(const QDomElement &element, TypeEntry* entry){
    QDomNamedNodeMap attributes = element.attributes();
    if (checkQtVersion(attributes)){
        if(attributes.count()){
            TypesystemException::raise(QString("Unexpected attribute '%2' of tag <%1> in line %3").arg(element.localName()).arg(attributes.item(0).localName()).arg(element.lineNumber()));
        }
        QDomNodeList childNodes = element.childNodes();
        for(int i=0; i<childNodes.length(); ++i){
            QDomNode item = childNodes.item(i);
            switch(item.nodeType()){
            case QDomNode::ElementNode:
            {
                QDomElement childElement = item.toElement();
                if(childElement.localName()=="include"){
                    Include inc = parseInclude(childElement);
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
                    TypesystemException::raise(QString("Unexpected tag <%2> as child of tag <%1> in line %3").arg(element.localName()).arg(childElement.localName()).arg(childElement.lineNumber()));
                }
            }
                break;
            case QDomNode::TextNode:
                TypesystemException::raise(QString("Unexpected text content as child of tag <%1> in line %2").arg(element.localName()).arg(item.lineNumber()));
                break;
            case QDomNode::CharacterDataNode:
            case QDomNode::CDATASectionNode:
                TypesystemException::raise(QString("Unexpected CDATA as child of tag <%1> in line %2").arg(element.localName()).arg(item.lineNumber()));
                break;
            case QDomNode::EntityNode:
                TypesystemException::raise(QString("Unexpected entity as child of tag <%1> in line %2").arg(element.localName()).arg(item.lineNumber()));
                break;
            case QDomNode::NotationNode:
                TypesystemException::raise(QString("Unexpected notation as child of tag <%1> in line %2").arg(element.localName()).arg(item.lineNumber()));
                break;
            default: break;
            }
        }
    }
}

void Handler::parseInstantiation(const QDomElement &element, ComplexTypeEntry* entry){
    QDomNamedNodeMap attributes = element.attributes();
    if (checkQtVersion(attributes)){
        QString arguments = attributeValue(attributes.removeNamedItem("arguments"));
        if(!arguments.isEmpty()){
            QStringList inst = arguments.split(",");
            for(QString& s : inst){
                s = s.trimmed();
            }
            entry->addInstantiation(inst);
        }else{
            QDomNodeList childNodes = element.childNodes();
            QStringList inst;
            for(int i=0; i<childNodes.length(); ++i){
                QDomNode item = childNodes.item(i);
                switch(item.nodeType()){
                case QDomNode::ElementNode:
                {
                    QDomElement childElement = item.toElement();
                    if(childElement.localName()=="argument"){
                        QDomNodeList childNodes = childElement.childNodes();
                        for(int i=0; i<childNodes.length(); ++i){
                            QDomNode item = childNodes.item(i);
                            switch(item.nodeType()){
                            case QDomNode::TextNode:
                                inst << item.toText().data().trimmed();
                                break;
                            case QDomNode::ElementNode:
                                TypesystemException::raise(QString("Unexpected tag <%2> as child of tag <%1> in line %3").arg(childElement.localName()).arg(item.toElement().localName()).arg(item.lineNumber()));
                                break;
                            case QDomNode::CharacterDataNode:
                            case QDomNode::CDATASectionNode:
                                TypesystemException::raise(QString("Unexpected CDATA as child of tag <%1> in line %2").arg(childElement.localName()).arg(item.lineNumber()));
                                break;
                            case QDomNode::EntityNode:
                                TypesystemException::raise(QString("Unexpected entity as child of tag <%1> in line %2").arg(childElement.localName()).arg(item.lineNumber()));
                                break;
                            case QDomNode::NotationNode:
                                TypesystemException::raise(QString("Unexpected notation as child of tag <%1> in line %2").arg(childElement.localName()).arg(item.lineNumber()));
                                break;
                            default: break;
                            }
                        }
                    }
                }
                    break;
                case QDomNode::TextNode:
                    TypesystemException::raise(QString("Unexpected text as child of tag <%1> in line %2").arg(element.localName()).arg(item.lineNumber()));
                    break;
                case QDomNode::CharacterDataNode:
                case QDomNode::CDATASectionNode:
                    TypesystemException::raise(QString("Unexpected CDATA as child of tag <%1> in line %2").arg(element.localName()).arg(item.lineNumber()));
                    break;
                case QDomNode::EntityNode:
                    TypesystemException::raise(QString("Unexpected entity as child of tag <%1> in line %2").arg(element.localName()).arg(item.lineNumber()));
                    break;
                case QDomNode::NotationNode:
                    TypesystemException::raise(QString("Unexpected notation as child of tag <%1> in line %2").arg(element.localName()).arg(item.lineNumber()));
                    break;
                default: break;
                }
            }
            entry->addInstantiation(inst);
        }
        if(attributes.count()){
            TypesystemException::raise(QString("Unexpected attribute '%2' of tag <%1> in line %3").arg(element.localName()).arg(attributes.item(0).localName()).arg(element.lineNumber()));
        }
    }
}

void Handler::parseTemplate(const QDomElement &element){
    QDomNamedNodeMap attributes = element.attributes();
    if (checkQtVersion(attributes)){
        QString name = attributeValue(attributes.removeNamedItem("name"));
        std::unique_ptr<TemplateEntry> entry(new TemplateEntry(name));
        if(attributes.count()){
            TypesystemException::raise(QString("Unexpected attribute '%2' of tag <%1> in line %3").arg(element.localName()).arg(attributes.item(0).localName()).arg(element.lineNumber()));

        }
        QDomNodeList childNodes = element.childNodes();
        for(int i=0; i<childNodes.length(); ++i){
            QDomNode item = childNodes.item(i);
            switch(item.nodeType()){
            case QDomNode::ElementNode:
            {
                QDomElement childElement = item.toElement();
                if(childElement.localName()=="insert-template"){
                    parseInsertTemplate(childElement, [&entry](TemplateInstance* templateInstance){entry->addTemplateInstance(templateInstance);});
                }else if(childElement.localName()=="import-file"){
                    parseImportFile(childElement, [&entry](const QString& code){entry->addCode(code);});
                }else{
                    TypesystemException::raise(QString("Unexpected tag <%2> as child of tag <%1> in line %3").arg(element.localName()).arg(childElement.localName()).arg(childElement.lineNumber()));
                }
            }
                break;
            case QDomNode::TextNode:
                entry->addCode(item.toText().data());
                break;
            case QDomNode::CharacterDataNode:
                entry->addCode(item.toCharacterData().data());
                break;
            case QDomNode::CDATASectionNode:
                entry->addCode(item.toCDATASection().data());
                break;
            case QDomNode::EntityNode:
                TypesystemException::raise(QString("Unexpected entity as child of tag <%1> in line %2").arg(element.localName()).arg(item.lineNumber()));
                break;
            case QDomNode::NotationNode:
                TypesystemException::raise(QString("Unexpected notation as child of tag <%1> in line %2").arg(element.localName()).arg(item.lineNumber()));
                break;
            default: break;
            }
        }
        m_database->addTemplate(entry.release());
    }
}

void Handler::parseInjectCode(const QDomElement &element, ComplexTypeEntry* entry){
    static const QHash<QString, TypeSystem::Language> languageNames{
        {"java", TypeSystem::TargetLangCode},
        {"native", TypeSystem::NativeCode},
        {"shell", TypeSystem::ShellCode},
        {"shell-declaration", TypeSystem::ShellDeclaration},
        {"library-initializer", TypeSystem::PackageInitializer},
        {"destructor-function", TypeSystem::DestructorFunction},
        {"metainfo", TypeSystem::MetaInfo},
        {"constructors", TypeSystem::Constructors},
        {"signal", TypeSystem::Signal}
    };

    static const QHash<QString, CodeSnip::Position> positionNames{
        {"begin", CodeSnip::Beginning},
        {"beginning", CodeSnip::Beginning},
        {"position1", CodeSnip::Position1},
        {"position2", CodeSnip::Position2},
        {"position3", CodeSnip::Position3},
        {"position4", CodeSnip::Position4},
        {"1", CodeSnip::Position1},
        {"2", CodeSnip::Position2},
        {"3", CodeSnip::Position3},
        {"4", CodeSnip::Position4},
        {"equals", CodeSnip::Equals},
        {"compare", CodeSnip::Compare},
        {"hashcode", CodeSnip::HashCode},
        {"tostring", CodeSnip::ToString},
        {"end", CodeSnip::End}
    };
    if(entry->isInterface() || entry->designatedInterface()){
        static const QHash<QString, TypeSystem::Language> _languageNames = [](const QHash<QString, TypeSystem::Language>& names)->QHash<QString, TypeSystem::Language>{
            QHash<QString, TypeSystem::Language> result{{"interface", TypeSystem::Interface}};
            result.insert(names);
            return result;
        }(languageNames);
        if(entry->designatedInterface()){
            return parseInjectCode(element, _languageNames, positionNames, [&entry](const CodeSnip &snip){entry->addCodeSnip(snip); entry->designatedInterface()->addCodeSnip(snip);});
        }else{
            return parseInjectCode(element, _languageNames, positionNames, [&entry](const CodeSnip &snip){entry->addCodeSnip(snip);});
        }
    }else{
        return parseInjectCode(element, languageNames, positionNames, [&entry](const CodeSnip &snip){entry->addCodeSnip(snip);});
    }
}

void Handler::parseInjectCode(const QDomElement &element, FunctionalTypeEntry* entry){
    static const QHash<QString, TypeSystem::Language> languageNames{
        {"java", TypeSystem::TargetLangCode},
        {"interface", TypeSystem::Interface},
        {"native", TypeSystem::NativeCode},
        {"shell", TypeSystem::ShellCode},
        {"shell-declaration", TypeSystem::ShellDeclaration},
        {"library-initializer", TypeSystem::PackageInitializer},
        {"destructor-function", TypeSystem::DestructorFunction},
        {"metainfo", TypeSystem::MetaInfo},
        {"constructors", TypeSystem::Constructors},
        {"signal", TypeSystem::Signal}
    };

    static const QHash<QString, CodeSnip::Position> positionNames{
        {"begin", CodeSnip::Beginning},
        {"beginning", CodeSnip::Beginning},
        {"position1", CodeSnip::Position1},
        {"position2", CodeSnip::Position2},
        {"position3", CodeSnip::Position3},
        {"position4", CodeSnip::Position4},
        {"1", CodeSnip::Position1},
        {"2", CodeSnip::Position2},
        {"3", CodeSnip::Position3},
        {"4", CodeSnip::Position4},
        {"equals", CodeSnip::Equals},
        {"compare", CodeSnip::Compare},
        {"hashcode", CodeSnip::HashCode},
        {"tostring", CodeSnip::ToString},
        {"end", CodeSnip::End}
    };
    return parseInjectCode(element, languageNames, positionNames, [&entry](const CodeSnip &snip){entry->addCodeSnip(snip);});
}

void Handler::parseInjectCode(const QDomElement &element,const QHash<QString, TypeSystem::Language>& languageNames,
                              const QHash<QString, CodeSnip::Position>& positionNames,
                              const std::function<void(const CodeSnip&)>& appendCodeSnip, bool argumentMapAllowed){
    QDomNamedNodeMap attributes = element.attributes();
    if (checkQtVersion(attributes)){
        QString className = attributeValue(attributes.removeNamedItem("class"), "java").toLower();
        if (!languageNames.contains(className)) {
            TypesystemException::raise(QString("Invalid class specifier '%1' of tag <%2> in line %3").arg(className).arg(element.localName()).arg(element.lineNumber()));
        }

        QString position = attributeValue(attributes.removeNamedItem("position"), "beginning").toLower();
        if (!positionNames.contains(position)) {
            TypesystemException::raise(QString("Invalid position '%1' of tag <%2> in line %3").arg(position).arg(element.localName()).arg(element.lineNumber()));
        }
        if(attributes.count()){
            TypesystemException::raise(QString("Unexpected attribute '%2' of tag <%1> in line %3").arg(element.localName()).arg(attributes.item(0).localName()).arg(element.lineNumber()));

        }

        CodeSnip snip;
        snip.language = languageNames[className];
        snip.position = positionNames[position];
        if((snip.position==CodeSnip::Equals
            || snip.position==CodeSnip::Compare
            || snip.position==CodeSnip::HashCode
            || snip.position==CodeSnip::ToString) && snip.language!=TypeSystem::TargetLangCode){
            TypesystemException::raise(QString("Invalid position '%1' for language '%2' of tag <%3> in line %4").arg(position).arg(className).arg(element.localName()).arg(element.lineNumber()));
        }

        QDomNodeList childNodes = element.childNodes();
        for(int i=0; i<childNodes.length(); ++i){
            QDomNode item = childNodes.item(i);
            switch(item.nodeType()){
            case QDomNode::ElementNode:
            {
                QDomElement childElement = item.toElement();
                if(argumentMapAllowed && childElement.localName()=="argument-map"){
                    QDomNamedNodeMap attributes = childElement.attributes();
                    if (checkQtVersion(attributes)){
                        bool ok;
                        QString idx = attributeValue(attributes.removeNamedItem("index"), "1");
                        int pos = idx.toInt(&ok);
                        if (!ok) {
                            TypesystemException::raise(QString("Can't convert attribute index '%2' to integer in tag <%1> in line %3").arg(childElement.localName()).arg(idx).arg(childElement.lineNumber()));
                        }

                        if (pos < 0) {
                            TypesystemException::raise(QString("Argument index %2 must be a positive number in tag <%1> in line %3").arg(childElement.localName()).arg(idx).arg(childElement.lineNumber()));
                        }

                        QString meta_name = attributeValue(attributes.removeNamedItem("meta-name"));
                        if (meta_name.isEmpty()) {
                            TypesystemException::raise(QString("Argument meta-name must not be empty in tag <%1> in line %2").arg(childElement.localName()).arg(childElement.lineNumber()));
                        }
                        snip.argumentMap[pos] = meta_name;
                    }
                }else if(childElement.localName()=="insert-template"){
                    parseInsertTemplate(childElement, [&snip](TemplateInstance* templateInstance){snip.addTemplateInstance(templateInstance);});
                }else if(childElement.localName()=="import-file"){
                    parseImportFile(childElement, [&snip](const QString& code){snip.addCode(code);});
                }else{
                    TypesystemException::raise(QString("Unexpected tag <%2> as child of tag <%1> in line %3").arg(element.localName()).arg(childElement.localName()).arg(childElement.lineNumber()));
                }
            }
                break;
            case QDomNode::TextNode:
                snip.addCode(item.toText().data());
                break;
            case QDomNode::CharacterDataNode:
                snip.addCode(item.toCharacterData().data());
                break;
            case QDomNode::CDATASectionNode:
                snip.addCode(item.toCDATASection().data());
                break;
            case QDomNode::EntityNode:
                TypesystemException::raise(QString("Unexpected entity as child of tag <%1> in line %2").arg(element.localName()).arg(item.lineNumber()));
                break;
            case QDomNode::NotationNode:
                TypesystemException::raise(QString("Unexpected notation as child of tag <%1> in line %2").arg(element.localName()).arg(item.lineNumber()));
                break;
            default: break;
            }
        }
        appendCodeSnip(snip);
    }
}

void Handler::parseInsertTemplate(const QDomElement &element, const std::function<void(TemplateInstance*)>& consumeTemplateInstance){
    QDomNamedNodeMap attributes = element.attributes();
    if (checkQtVersion(attributes)){
        QString name = attributeValue(attributes.removeNamedItem("name"));
        if(attributes.count()){
            TypesystemException::raise(QString("Unexpected attribute '%2' of tag <%1> in line %3").arg(element.localName()).arg(attributes.item(0).localName()).arg(element.lineNumber()));
        }
        std::unique_ptr<TemplateInstance> templateInstance(new TemplateInstance(name));
        QDomNodeList childNodes = element.childNodes();
        for(int i=0; i<childNodes.length(); ++i){
            QDomNode item = childNodes.item(i);
            switch(item.nodeType()){
            case QDomNode::ElementNode:
            {
                QDomElement childElement = item.toElement();
                if(childElement.localName()=="replace"){
                    QDomNamedNodeMap attributes = childElement.attributes();
                    if (checkQtVersion(attributes)){
                        QString from = attributeValue(attributes.removeNamedItem("from"));
                        QString to = attributeValue(attributes.removeNamedItem("to"));
                        if(attributes.count()){
                            TypesystemException::raise(QString("Unexpected attribute of tag <%1>: %2").arg(childElement.localName()).arg(attributes.item(0).localName()));
                        }
                        templateInstance->addReplaceRule(from, to);
                    }
                }else{
                    TypesystemException::raise(QString("Unexpected tag <%2> as child of tag <%1> in line %3").arg(element.localName()).arg(childElement.localName()).arg(childElement.lineNumber()));
                }
            }
                break;
            case QDomNode::TextNode:
                TypesystemException::raise(QString("Unexpected text content as child of tag <%1> in line %2").arg(element.localName()).arg(item.lineNumber()));
                break;
            case QDomNode::CharacterDataNode:
            case QDomNode::CDATASectionNode:
                TypesystemException::raise(QString("Unexpected CDATA as child of tag <%1> in line %2").arg(element.localName()).arg(item.lineNumber()));
                break;
            case QDomNode::EntityNode:
                TypesystemException::raise(QString("Unexpected entity as child of tag <%1> in line %2").arg(element.localName()).arg(item.lineNumber()));
                break;
            case QDomNode::NotationNode:
                TypesystemException::raise(QString("Unexpected notation as child of tag <%1> in line %2").arg(element.localName()).arg(item.lineNumber()));
                break;
            default: break;
            }
        }
        consumeTemplateInstance(templateInstance.release());
    }
}

void Handler::parseImportFile(const QDomElement &element, const std::function<void(const QString&)>& consumeCode){
    QDomNamedNodeMap attributes = element.attributes();
    if (checkQtVersion(attributes)){
        QString fileName = attributeValue(attributes.removeNamedItem("name"));
        if (fileName.isEmpty()) {
            TypesystemException::raise(QString("Required attribute 'name' missing for tag <%1>: line %2").arg(element.localName()).arg(element.lineNumber()));
        }

        QFile file(fileName);
        QFileInfo fileinfo(file);
        if (fileinfo.isRelative() && !m_importInputDirectoryList.isEmpty()) {
            // Resolve
            QString filepath = resolveFilePath(fileName, 0, m_importInputDirectoryList);
            if(filepath.isNull()) {
                TypesystemException::raise(QString("Could not resolve file path '%1' specified in tag <%2> in line %3").arg(fileName).arg(element.localName()).arg(element.lineNumber()));
            }
            file.setFileName(filepath);
        }
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
#if 0
            // This feature is disabled, better to use actual files
            file.setFileName(":/io/qt/generator/" + fileName);
            if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
#endif
                TypesystemException::raise(QString("Could not open file '%1' specified in tag <%2> in line %3").arg(fileName).arg(element.localName()).arg(element.lineNumber()));

#if 0
            }
#endif
        }

        QString quoteFrom = attributeValue(attributes.removeNamedItem("quote-after-line"));
        bool foundFromOk = quoteFrom.isEmpty();
        bool from = quoteFrom.isEmpty();

        QString quoteTo = attributeValue(attributes.removeNamedItem("quote-before-line"));
        bool foundToOk = quoteTo.isEmpty();
        bool to = true;

        if(attributes.count()){
            TypesystemException::raise(QString("Unexpected attribute '%2' of tag <%1> in line %3").arg(element.localName()).arg(attributes.item(0).localName()).arg(element.lineNumber()));

        }

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
                TypesystemException::raise(QString("%1 See <%2> in line %3").arg(toError).arg(element.localName()).arg(element.lineNumber()));
            if (!foundFromOk)
                TypesystemException::raise(QString("%1 See <%2> in line %3").arg(fromError).arg(element.localName()).arg(element.lineNumber()));
            if (!foundFromOk && !foundToOk)
                TypesystemException::raise(QString("%1 %2 See <%3> in line %4").arg(fromError).arg(toError).arg(element.localName()).arg(element.lineNumber()));

        }
        ensureNoChildren(element);

    }
}

void Handler::parseLoadTypeSystem(const QDomElement &element){
    QDomNamedNodeMap attributes = element.attributes();
    if (checkQtVersion(attributes)){
        QString unless = attributeValue(attributes.removeNamedItem("unless"));
        bool skip = !unless.isEmpty() && TypeDatabase::instance()->defined(unless);
        if(!skip){
            QString name = attributeValue(attributes.removeNamedItem("name"));
            if (name.isEmpty()) {
                TypesystemException::raise(QString("No typesystem name specified"));
            }
            bool generated = convertBoolean(attributeValue(attributes.removeNamedItem("generate"), "yes"), "generate", true);
            bool optional = convertBoolean(attributeValue(attributes.removeNamedItem("optional"), "no"), "optional", false);
            if(attributes.count()){
                TypesystemException::raise(QString("Unexpected attribute '%2' of tag <%1> in line %3").arg(element.localName()).arg(attributes.item(0).localName()).arg(element.lineNumber()));
            }
            ensureNoChildren(element);
            m_database->parseFile(name, m_importInputDirectoryList, generated, optional);
        }
    }
}

void Handler::parseRejection(const QDomElement &element){
    QDomNamedNodeMap attributes = element.attributes();
    if (checkQtVersion(attributes)){
        QString cls = attributeValue(attributes.removeNamedItem("class"), "*");
        QString function = attributeValue(attributes.removeNamedItem("function-name"), "*");
        QString field = attributeValue(attributes.removeNamedItem("field-name"), "*");
        QString enum_ = attributeValue(attributes.removeNamedItem("enum-name"), "*");
        if(attributes.count()){
            TypesystemException::raise(QString("Unexpected attribute '%2' of tag <%1> in line %3").arg(element.localName()).arg(attributes.item(0).localName()).arg(element.lineNumber()));
        }
        ensureNoChildren(element);

        if (cls == "*" && function == "*" && field == "*" && enum_ == "*") {
            TypesystemException::raise(QString("bad reject entry, neither 'class', 'function-name' nor "
                      "'field' specified in line %1").arg(element.lineNumber()));
        }
        m_database->addRejection(cls, function, field, enum_);
    }
}

void Handler::parsePrimitiveType(const QDomElement &element){
    QDomNamedNodeMap attributes = element.attributes();
    if (checkQtVersion(attributes)){
        QString name = attributeValue(attributes.removeNamedItem("name"));
        if (name.isEmpty()) {
            TypesystemException::raise(QString("No 'name' attribute specified for tag <%1> in line %2").arg(element.localName()).arg(element.lineNumber()));
        }
        QString java_name = attributeValue(attributes.removeNamedItem("java-name"));
        QString jni_name = attributeValue(attributes.removeNamedItem("jni-name"));
        QString preferred_conversion = attributeValue(attributes.removeNamedItem("preferred-conversion"), "yes").toLower();
        QString preferred_java_type = attributeValue(attributes.removeNamedItem("preferred-java-type"), "yes").toLower();

        if(attributes.count()){
            TypesystemException::raise(QString("Unexpected attribute '%2' of tag <%1> in line %3").arg(element.localName()).arg(attributes.item(0).localName()).arg(element.lineNumber()));
        }
        ensureNoChildren(element);

        if (java_name.isEmpty())
            java_name = name;
        if (jni_name.isEmpty())
            jni_name = name;

        PrimitiveTypeEntry *entry = new PrimitiveTypeEntry(name, m_defaultPackage);
        entry->setCodeGeneration(m_generate);
        entry->setTargetLangName(java_name);
        entry->setJniName(jni_name);

        entry->setPreferredConversion(convertBoolean(preferred_conversion, "preferred-conversion", true));
        entry->setPreferredTargetLangType(convertBoolean(preferred_java_type, "preferred-java-type", true));

        ReportHandler::debugTypes("Adding to TypeDatabase(2): " + entry->name());
        m_database->addType(entry);
    }
}

void Handler::parseAttributesOfComplexType(const QDomElement &element, QDomNamedNodeMap& attributes, ComplexTypeEntry* ctype){
    QString package = attributeValue(attributes.removeNamedItem("package"), m_defaultPackage);
    QString implements = attributeValue(attributes.removeNamedItem("implements"));
    QString _using = attributeValue(attributes.removeNamedItem("using"));
    QString javaName = attributeValue(attributes.removeNamedItem("java-name"));
    QString defaultSuperclass = attributeValue(attributes.removeNamedItem("default-superclass"));
    QString genericClass = attributeValue(attributes.removeNamedItem("generic-class"), "no");
    QString limit = attributeValue(attributes.removeNamedItem("expense-limit"), "none");
    if (!limit.isEmpty() && limit != "none") {
        ExpensePolicy ep;
        bool ok = false;
        ep.limit = limit.toLongLong(&ok);
        if(!ok)
            TypesystemException::raise(QString("Can't convert attribute expense-limit='%2' to integer in tag <%1> in line %3").arg(element.localName()).arg(limit).arg(element.lineNumber()));
        ep.cost = attributeValue(attributes.removeNamedItem("expense-cost"), "1");
        ctype->setExpensePolicy(ep);
        if(ctype->designatedInterface())
            ctype->designatedInterface()->setExpensePolicy(ep);
    }
    ctype->setPPCondition(attributeValue(attributes.removeNamedItem("pp-condition")));
    QString targetType = attributeValue(attributes.removeNamedItem("target-type"));
    if (!targetType.isEmpty()){
        ctype->setTargetType(targetType);
    }
    if (convertBoolean(attributeValue(attributes.removeNamedItem("disable-native-id-usage"), "no"), "disable-native-id-usage", false))
        ctype->disableNativeIdUsage();
    if (convertBoolean(attributeValue(attributes.removeNamedItem("force-abstract"), "no"), "force-abstract", false))
        ctype->setTypeFlags(ctype->typeFlags() | ComplexTypeEntry::ForceAbstract);
    if (convertBoolean(attributeValue(attributes.removeNamedItem("force-friendly"), "no"), "force-friendly", false))
        ctype->setTypeFlags(ctype->typeFlags() | ComplexTypeEntry::ForceFriendly);
    if (convertBoolean(attributeValue(attributes.removeNamedItem("deprecated"), "no"), "deprecated", false))
        ctype->setTypeFlags(ctype->typeFlags() | ComplexTypeEntry::Deprecated);
    if(!ctype->isNamespace() && !ctype->isIterator()){
        QString threadAffine = attributeValue(attributes.removeNamedItem("thread-affine"), "");
        if (!threadAffine.trimmed().isEmpty()){
            ctype->setTypeFlags(ctype->typeFlags() | ComplexTypeEntry::ThreadAffine);
            ctype->setThreadAffinity(threadAffine);
        }
    }

    if(convertBoolean(attributeValue(attributes.removeNamedItem("template"), "no"), "template", false)){
        ctype->setTemplate(true);
        ctype->setGenericClass(true);
        ctype->setCodeGeneration(TypeEntry::GenerateForSubclass);
    }else{
        ctype->setGenericClass(convertBoolean(genericClass, "generic-class", false));
        QString generate = attributeValue(attributes.removeNamedItem("generate"), "yes");
        if(generate=="no-shell"){
            ctype->setCodeGeneration(TypeEntry::GenerateNoShell | TypeEntry::GenerateAll);
        }else if (!convertBoolean(generate, "generate", true))
            ctype->setCodeGeneration(TypeEntry::GenerateForSubclass);
        else
            ctype->setCodeGeneration(m_generate);
    }
    if(attributes.count()){
        TypesystemException::raise(QString("Unexpected attribute '%2' of tag <%1> in line %3").arg(element.localName()).arg(attributes.item(0).localName()).arg(element.lineNumber()));
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
        ctype->designatedInterface()->setTypeFlags(ctype->typeFlags());
        ctype->designatedInterface()->setThreadAffinity(ctype->threadAffinity());
        ctype->designatedInterface()->setPPCondition(ctype->ppCondition());
    }

    if(ctype->designatedInterface()){
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
}

void Handler::parseObjectType(const QDomElement &element){
    QDomNamedNodeMap attributes = element.attributes();
    if (checkQtVersion(attributes)){
        QString name = parseTypeName(attributes, element);
        QString targetName;
        for(const NamespacePrefix& prefix : m_namespacePrefixes){
            if(name.startsWith(prefix.prefix)){
                name = name.mid(prefix.prefix.size());
                if(prefix.namingPolicy==NamespacePrefix::Cut)
                    targetName = name;
                name = prefix._namespace + name;
                break;
            }
        }
        std::unique_ptr<ObjectTypeEntry> entry(new ObjectTypeEntry(name));
        if(!targetName.isEmpty())
            entry->setTargetLangName(targetName);
        entry->setIsValueOwner(convertBoolean(attributeValue(attributes.removeNamedItem("is-value-owner"), "no"), "is-value-owner", false));
        entry->setIsPolymorphicBase(convertBoolean(attributeValue(attributes.removeNamedItem("polymorphic-base"), "no"), "polymorphic-base", false));
        entry->setPolymorphicIdValue(attributeValue(attributes.removeNamedItem("polymorphic-id-expression")));
        parseAttributesOfComplexType(element, attributes, entry.get());
        QList<QDomElement> unhandledElements = parseChildrenOfComplexType(element, entry.get());
        for(const QDomElement& childElement : unhandledElements){
            TypesystemException::raise(QString("Unexpected tag <%2> as child of tag <%1> in line %3").arg(element.localName()).arg(childElement.toElement().localName()).arg(childElement.lineNumber()));
        }
        if(entry->name().endsWith(">")){
            auto idx = entry->name().indexOf('<');
            QString templateName = entry->name().mid(0, idx);
            QStringList templateArguments = entry->name().mid(idx+1).chopped(1).split(",");
            ComplexTypeEntry* templateType = m_database->findComplexType(templateName);
            if(templateType){
                if(templateType->isTemplate()){
                    if(templateType->instantiations().contains(templateArguments) && templateType->instantiations()[templateArguments]==nullptr){
                        if(entry->targetLangName()==entry->name()){
                            entry->setTargetLangName(templateName);
                        }
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
    }
}

QList<QDomElement> Handler::parseChildrenOfComplexType(const QDomElement &element, ComplexTypeEntry* entry){
    QList<QDomElement> unhandledElements;
    QDomNodeList childNodes = element.childNodes();
    for(int i=0; i<childNodes.length(); ++i){
        QDomNode item = childNodes.item(i);
        switch(item.nodeType()){
        case QDomNode::ElementNode:
        {
            QDomElement childElement = item.toElement();
            if(childElement.localName()=="template"){
                parseTemplate(childElement);
            }else if(childElement.localName()=="instantiation"){
                if(!entry->isTemplate())
                    TypesystemException::raise(QString("Unexpected tag <%2> as child of non-template <%1> in line %3").arg(element.localName()).arg(childElement.localName()).arg(childElement.lineNumber()));
                parseInstantiation(childElement, entry);
            }else if(childElement.localName()=="modify-function"){
                parseModifyFunction(childElement, entry);
            }else if(childElement.localName()=="modify-field"){
                parseModifyField(childElement, entry);
            }else if(childElement.localName()=="inject-code"){
                parseInjectCode(childElement, entry);
            }else if(childElement.localName()=="delegate-base-class"){
                QDomNamedNodeMap attributes = childElement.attributes();
                if (checkQtVersion(attributes)){
                    QString baseClass = attributeValue(attributes.removeNamedItem("base-class"));
                    QString delegate = attributeValue(attributes.removeNamedItem("delegate"));
                    if(attributes.count()){
                        TypesystemException::raise(QString("Unexpected attribute '%2' of tag <%1> in line %3").arg(childElement.localName()).arg(attributes.item(0).localName()).arg(childElement.lineNumber()));
                    }
                    if(!baseClass.isEmpty())
                        entry->addDelegatedBaseClass(baseClass, delegate);
                }
            }else if(childElement.localName()=="implements"){
                QDomNamedNodeMap attributes = childElement.attributes();
                if (checkQtVersion(attributes)){
                    entry->setImplements(childElement.text());
                }
            }else if(childElement.localName()=="include"){
                Include incl = parseInclude(childElement);
                if(incl.isValid()){
                    entry->setInclude(incl);
                    if(entry->designatedInterface())
                        entry->designatedInterface()->setInclude(incl);
                }
            }else if(childElement.localName()=="extra-includes"){
                parseExtraIncludes(childElement, entry);
            }else if(childElement.localName()=="import"){
                QDomNamedNodeMap attributes = childElement.attributes();
                if (checkQtVersion(attributes)){
                    QString name = attributeValue(attributes.removeNamedItem("template"));
                    if(TemplateTypeEntry* templateEntry = m_database->findTemplateType(name)){
                        if(attributes.count()){
                            TypesystemException::raise(QString("Unexpected attribute '%2' of tag <%1> in line %3").arg(childElement.localName()).arg(attributes.item(0).localName()).arg(childElement.lineNumber()));
                        }
                        entry->addCodeSnips(templateEntry->codeSnips());
                        entry->addFunctionModifications(templateEntry->functionModifications());
                        entry->addFieldModifications(templateEntry->fieldModifications());
                        for(const Include& incl : templateEntry->extraIncludes()){
                            entry->addExtraInclude(incl);
                        }
                        if(entry->designatedInterface()){
                            entry->designatedInterface()->addCodeSnips(templateEntry->codeSnips());
                            entry->designatedInterface()->addFunctionModifications(templateEntry->functionModifications());
                            entry->designatedInterface()->addFieldModifications(templateEntry->fieldModifications());
                            for(const Include& incl : templateEntry->extraIncludes()){
                                entry->designatedInterface()->addExtraInclude(incl);
                            }
                        }
                    }else{
                        TypesystemException::raise(QString("Unable to find template-type '%1'. Referred in line %2").arg(name).arg(childElement.lineNumber()));
                    }
                }
            }else if((entry->isValue() || entry->isObject() || entry->isInterface()) && childElement.localName()=="interface-polymorphy"){
                QDomNamedNodeMap attributes = childElement.attributes();
                if (checkQtVersion(attributes)){
                    QString interface = attributeValue(attributes.removeNamedItem("interface"));
                    QString condition = attributeValue(attributes.removeNamedItem("condition"));
                    if(interface.isEmpty()){
                        TypesystemException::raise(QString("No interface specified for tag <%1> in line %2").arg(element.localName()).arg(element.lineNumber()));
                    }
                    if(attributes.count()){
                        TypesystemException::raise(QString("Unexpected attribute '%2' of tag <%1> in line %3").arg(childElement.localName()).arg(attributes.item(0).localName()).arg(childElement.lineNumber()));
                    }
                    entry->addInterfacePolymorphicIdValue(interface, condition);
                    if(entry->designatedInterface())
                        entry->designatedInterface()->addInterfacePolymorphicIdValue(interface, condition);
                }
            }else if((entry->isValue() || (entry->isInterface() && dynamic_cast<InterfaceTypeEntry*>(entry)->origin()->isValue()))
                     && childElement.localName()=="custom-constructor"){
                QDomNamedNodeMap attributes = childElement.attributes();
                if (checkQtVersion(attributes)){
                    CustomFunction func = parseCustomStructor(childElement);
                    func.name = attributeValue(attributes.removeNamedItem("name"), entry->name() + "_construct");
                    func.param_name = attributeValue(attributes.removeNamedItem("param-name"), "copy");
                    func.placement_name = attributeValue(attributes.removeNamedItem("placement-name"), "placement");
                    QString _type = attributeValue(attributes.removeNamedItem("type"));
                    QHash<QString,TypeEntry::ConstructorType> types{{"",TypeEntry::LegacyCopyConstructor},
                                {"default",TypeEntry::DefaultConstructor},
                                {"copy",TypeEntry::CopyConstructor},
                                {"move",TypeEntry::MoveConstructor}};
                    if(attributes.count()){
                        TypesystemException::raise(QString("Unexpected attribute '%2' of tag <%1> in line %3").arg(childElement.localName()).arg(attributes.item(0).localName()).arg(childElement.lineNumber()));
                    }
                    if(!types.contains(_type)){
                        TypesystemException::raise(QString("Invalid type specifier '%1' of tag <%2> in line %3").arg(_type).arg(element.localName()).arg(element.lineNumber()));
                    }
                    entry->setCustomConstructor(func, types[_type]);
                    if(entry->designatedInterface())
                        entry->designatedInterface()->setCustomConstructor(func, types[_type]);
                }
            }else if((entry->isValue() || (entry->isInterface() && dynamic_cast<InterfaceTypeEntry*>(entry)->origin()->isValue()))
                     && childElement.localName()=="custom-destructor"){
                QDomNamedNodeMap attributes = childElement.attributes();
                if (checkQtVersion(attributes)){
                    CustomFunction func = parseCustomStructor(childElement);
                    func.name = attributeValue(attributes.removeNamedItem("name"), entry->name() + "_destroy");
                    func.param_name = attributeValue(attributes.removeNamedItem("param-name"), "copy");
                    if(attributes.count()){
                        TypesystemException::raise(QString("Unexpected attribute '%2' of tag <%1> in line %3").arg(childElement.localName()).arg(attributes.item(0).localName()).arg(childElement.lineNumber()));
                    }
                    entry->setCustomDestructor(func);
                    if(entry->designatedInterface())
                        entry->designatedInterface()->setCustomDestructor(func);
                }
            }else{
                unhandledElements << childElement;
            }
        }
            break;
        case QDomNode::TextNode:
            TypesystemException::raise(QString("Unexpected text content as child of tag <%1> in line %2").arg(element.localName()).arg(item.lineNumber()));
            break;
        case QDomNode::CharacterDataNode:
        case QDomNode::CDATASectionNode:
            TypesystemException::raise(QString("Unexpected CDATA as child of tag <%1> in line %2").arg(element.localName()).arg(item.lineNumber()));
            break;
        case QDomNode::EntityNode:
            TypesystemException::raise(QString("Unexpected entity as child of tag <%1> in line %2").arg(element.localName()).arg(item.lineNumber()));
            break;
        case QDomNode::NotationNode:
            TypesystemException::raise(QString("Unexpected notation as child of tag <%1> in line %2").arg(element.localName()).arg(item.lineNumber()));
            break;
        default: break;
        }
    }
    return unhandledElements;
}

CustomFunction Handler::parseCustomStructor(const QDomElement &element){
    CustomFunction func;
    QDomNodeList childNodes = element.childNodes();
    for(int i=0; i<childNodes.length(); ++i){
        QDomNode item = childNodes.item(i);
        switch(item.nodeType()){
        case QDomNode::ElementNode:
        {
            QDomElement childElement = item.toElement();
            if(childElement.localName()=="insert-template"){
                parseInsertTemplate(childElement, [&func](TemplateInstance* templateInstance){func.addTemplateInstance(templateInstance);});
            }else if(childElement.localName()=="import-file"){
                parseImportFile(childElement, [&func](const QString& code){func.addCode(code);});
            }else{
                TypesystemException::raise(QString("Unexpected tag <%2> as child of tag <%1> in line %3").arg(element.localName()).arg(childElement.localName()).arg(childElement.lineNumber()));
            }
        }
            break;
        case QDomNode::TextNode:
            func.addCode(item.toText().data());
            break;
        case QDomNode::CharacterDataNode:
            func.addCode(item.toCharacterData().data());
            break;
        case QDomNode::CDATASectionNode:
            func.addCode(item.toCDATASection().data());
            break;
        case QDomNode::EntityNode:
            TypesystemException::raise(QString("Unexpected entity as child of tag <%1> in line %2").arg(element.localName()).arg(item.lineNumber()));
            break;
        case QDomNode::NotationNode:
            TypesystemException::raise(QString("Unexpected notation as child of tag <%1> in line %2").arg(element.localName()).arg(item.lineNumber()));
            break;
        default: break;
        }
    }
    return func;
}

void Handler::parseTemplateType(const QDomElement &element){
    QDomNamedNodeMap attributes = element.attributes();
    if (checkQtVersion(attributes)){
        QString name = parseTypeName(attributes, element);
        std::unique_ptr<TemplateTypeEntry> entry(new TemplateTypeEntry(name));
        if(attributes.count()){
            TypesystemException::raise(QString("Unexpected attribute '%2' of tag <%1> in line %3").arg(element.localName()).arg(attributes.item(0).localName()).arg(element.lineNumber()));
        }
        QList<QDomElement> unhandledElements = parseChildrenOfComplexType(element, entry.get());
        for(const QDomElement& childElement : unhandledElements){
            TypesystemException::raise(QString("Unexpected tag <%2> as child of tag <%1> in line %3").arg(element.localName()).arg(childElement.toElement().localName()).arg(childElement.lineNumber()));
        }
        m_database->addTemplateType(entry.release());
    }
}

void Handler::parseModifyArgument(const QDomElement &element, FunctionModification& mod){
    QDomNamedNodeMap attributes = element.attributes();
    if (checkQtVersion(attributes)){
        QString index = attributeValue(attributes.removeNamedItem("index"));
        if (index == "return")
            index = "0";
        else if (index == "this")
            index = "-1";
        bool ok = false;
        int idx = index.toInt(&ok);
        if (!ok) {
            TypesystemException::raise(QString("Can't convert attribute index='%2' to integer in tag <%1> in line %3").arg(element.localName()).arg(index).arg(element.lineNumber()));
        }
        ArgumentModification argumentModification(idx);

        argumentModification.replace_value = attributeValue(attributes.removeNamedItem("replace-value"));
        if (!argumentModification.replace_value.isEmpty() && idx != 0) {
            TypesystemException::raise(QString("replace-value is only supported for return values (index=0) in tag <%1> in line %2").arg(element.localName()).arg(element.lineNumber()));
        }
        QString threadAffine = attributeValue(attributes.removeNamedItem("thread-affine"), "no").trimmed();
        if(threadAffine=="ui")
            argumentModification.thread_affine = ThreadAffinity::UI;
        else if(threadAffine=="pixmap")
            argumentModification.thread_affine = ThreadAffinity::Pixmap;
        else if(!threadAffine.isEmpty() && convertBoolean(threadAffine, "thread-affine", false))
            argumentModification.thread_affine = ThreadAffinity::Yes;
        argumentModification.modified_name = attributeValue(attributes.removeNamedItem("rename"));
        argumentModification.modified_type = attributeValue(attributes.removeNamedItem("replace-type"));
        argumentModification.reset_after_use = convertBoolean(attributeValue(attributes.removeNamedItem("invalidate-after-use"), "no"), "invalidate-after-use", false);

        if(attributes.count()){
            TypesystemException::raise(QString("Unexpected attribute '%2' of tag <%1> in line %3").arg(element.localName()).arg(attributes.item(0).localName()).arg(element.lineNumber()));
        }

        QDomNodeList childNodes = element.childNodes();
        for(int i=0; i<childNodes.length(); ++i){
            QDomNode item = childNodes.item(i);
            switch(item.nodeType()){
            case QDomNode::ElementNode:
            {
                QDomElement childElement = item.toElement();
                if(childElement.localName()=="remove-default-expression"){
                    QDomNamedNodeMap attributes = childElement.attributes();
                    if (checkQtVersion(attributes)){
                        argumentModification.removed_default_expression = true;
                        if(attributes.count()){
                            TypesystemException::raise(QString("Unexpected attribute '%2' of tag <%1> in line %3").arg(childElement.localName()).arg(attributes.item(0).localName()).arg(childElement.lineNumber()));
                        }
                        ensureNoChildren(childElement);
                    }
                }else if(childElement.localName()=="replace-default-expression"){
                    QDomNamedNodeMap attributes = childElement.attributes();
                    if (checkQtVersion(attributes)){
                        argumentModification.replaced_default_expression = attributeValue(attributes.removeNamedItem("with"));
                        if(attributes.count()){
                            TypesystemException::raise(QString("Unexpected attribute '%2' of tag <%1> in line %3").arg(childElement.localName()).arg(attributes.item(0).localName()).arg(childElement.lineNumber()));
                        }
                        ensureNoChildren(childElement);
                    }
                }else if(childElement.localName()=="reference-count"){
                    QDomNamedNodeMap attributes = childElement.attributes();
                    if (checkQtVersion(attributes)){
                        ReferenceCount rc;
                        rc.threadSafe = convertBoolean(attributeValue(attributes.removeNamedItem("thread-safe"), "no"), "thread-safe", false);
                        static const QHash<QString, ReferenceCount::Action> actions{
                            {"put", ReferenceCount::Put},
                            {"add", ReferenceCount::Add},
                            {"add-all", ReferenceCount::AddAll},
                            {"clear-add", ReferenceCount::ClearAdd},
                            {"clear-add-all", ReferenceCount::ClearAddAll},
                            {"remove", ReferenceCount::Remove},
                            {"set", ReferenceCount::Set},
                            {"ignore", ReferenceCount::Ignore}
                        };
                        QString action = attributeValue(attributes.removeNamedItem("action"));
                        if (!actions.contains(action)) {
                            TypesystemException::raise(QString("unrecognized value '%2' for action attribute of tag <%1> in line %3").arg(childElement.localName()).arg(action).arg(childElement.lineNumber()));
                        }
                        rc.action = actions.value(action.toLower(), ReferenceCount::Invalid);
                        rc.variableName = attributeValue(attributes.removeNamedItem("variable-name"));
                        if (rc.action != ReferenceCount::Ignore && rc.variableName.isEmpty()) {
                            TypesystemException::raise(QString("variable-name attribute must be specified in tag <%1> in line %2").arg(childElement.localName()).arg(childElement.lineNumber()));
                        }
                        bool ok = false;
                        uint ka = attributeValue(attributes.removeNamedItem("key-argument")).toUInt(&ok);
                        rc.keyArgument = ok ? ka : 0;
                        if (rc.action == ReferenceCount::Put && rc.keyArgument<=0) {
                            TypesystemException::raise(QString("key-argument attribute must be specified and greater than 0 in tag <%1> in line %2").arg(childElement.localName()).arg(childElement.lineNumber()));
                        }
                        rc.declareVariable = attributeValue(attributes.removeNamedItem("declare-variable"));
                        rc.condition = attributeValue(attributes.removeNamedItem("condition"));

                        static const QHash<QString, uint> accessRights{
                            {"private", ReferenceCount::Private},
                            {"public", ReferenceCount::Public},
                            {"protected", ReferenceCount::Protected},
                            {"friendly", ReferenceCount::Friendly}
                        };
                        QString access = attributeValue(attributes.removeNamedItem("access"), "private");
                        if (!accessRights.contains(access)) {
                            TypesystemException::raise(QString("unrecognized access value '%2' of tag <%1> in line %3").arg(childElement.localName()).arg(access).arg(childElement.lineNumber()));
                        }
                        rc.access = accessRights.value(access.toLower(), 0);

                        if(attributes.count()){
                            TypesystemException::raise(QString("Unexpected attribute '%2' of tag <%1> in line %3").arg(childElement.localName()).arg(attributes.item(0).localName()).arg(childElement.lineNumber()));
                        }
                        ensureNoChildren(childElement);

                        argumentModification.referenceCounts.append(rc);
                    }
                }else if(childElement.localName()=="array-type"){
                    QDomNamedNodeMap attributes = childElement.attributes();
                    if (checkQtVersion(attributes)){
                        AsArrayTypes asArrayType = AsArrayType::Yes;

                        if(convertBoolean(attributeValue(attributes.removeNamedItem("deref"), "no"), "deref", false))
                            asArrayType |= AsArrayType::Deref;
                        if(convertBoolean(attributeValue(attributes.removeNamedItem("varargs"), "no"), "varargs", false))
                            asArrayType |= AsArrayType::VarArgs;
                        if(convertBoolean(attributeValue(attributes.removeNamedItem("as-buffer"), "no"), "as-buffer", false))
                            asArrayType |= AsArrayType::Buffer;
                        argumentModification.useAsArrayType = asArrayType;
                        QString lengthParameter = attributeValue(attributes.removeNamedItem("length-parameter"));
                        if (!lengthParameter.isEmpty()) {
                            bool ok = false;
                            argumentModification.arrayLengthParameter = int(lengthParameter.toUInt(&ok));
                            if (!ok) {
                                TypesystemException::raise(QString("Cannot convert length-parameter '%3' to unsigned integer in tag <%1> in line %2").arg(childElement.localName()).arg(childElement.lineNumber()).arg(lengthParameter));
                            }
                        }else{
                            argumentModification.arrayLengthParameter = 0;
                            QString minLength = attributeValue(attributes.removeNamedItem("min-length"));
                            if (!minLength.isEmpty()) {
                                bool ok = false;
                                argumentModification.minArrayLength = int(minLength.toUInt(&ok));
                                if (!ok) {
                                    TypesystemException::raise(QString("Cannot convert min-length '%3' to unsigned integer in tag <%1> in line %2").arg(childElement.localName()).arg(childElement.lineNumber()).arg(minLength));
                                }
                                QString maxLength = attributeValue(attributes.removeNamedItem("max-length"));
                                if (!maxLength.isEmpty()) {
                                    ok = false;
                                    argumentModification.maxArrayLength = int(maxLength.toUInt(&ok));
                                    if (!ok) {
                                        TypesystemException::raise(QString("Cannot convert max-length '%3' to unsigned integer in tag <%1> in line %2").arg(childElement.localName()).arg(childElement.lineNumber()).arg(maxLength));
                                    }
                                }else{
                                    argumentModification.maxArrayLength = -1;
                                }
                            }else{
                                argumentModification.minArrayLength = -1;
                                argumentModification.maxArrayLength = -1;
                            }
                        }

                        if( (asArrayType & AsArrayType::Buffer) == 0 ){
                            if(argumentModification.arrayLengthParameter<1
                                    && argumentModification.minArrayLength<1){
                                TypesystemException::raise(QString("Both, length-parameter and min-length must not be < 1 in tag <%1> in line %2").arg(childElement.localName()).arg(childElement.lineNumber()));
                            }
                        }
                        if(attributes.count()){
                            TypesystemException::raise(QString("Unexpected attribute '%2' of tag <%1> in line %3").arg(childElement.localName()).arg(attributes.item(0).localName()).arg(childElement.lineNumber()));
                        }
                        ensureNoChildren(childElement);

                    }
                }else if(childElement.localName()=="replace-type"){
                    QDomNamedNodeMap attributes = childElement.attributes();
                    if (checkQtVersion(attributes)){
                        QString modifiedType = attributeValue(attributes.removeNamedItem("modified-type"));
                        if (modifiedType.isEmpty()) {
                            TypesystemException::raise(QString("Missing required 'modified-type' attribute in tag <%1> in line %2").arg(childElement.localName()).arg(childElement.lineNumber()));
                        }
                        argumentModification.modified_type = modifiedType;
                        argumentModification.modified_jni_type = attributeValue(attributes.removeNamedItem("modified-jni-type"));
                        if(attributes.count()){
                            TypesystemException::raise(QString("Unexpected attribute '%2' of tag <%1> in line %3").arg(childElement.localName()).arg(attributes.item(0).localName()).arg(childElement.lineNumber()));
                        }
                        ensureNoChildren(childElement);

                    }
                }else if(childElement.localName()=="conversion-rule"){
                    parseConversionRule(childElement, [&argumentModification](const CodeSnip& snip){argumentModification.conversion_rules.append(snip);});
                }else if(childElement.localName()=="no-null-pointer"){
                    QDomNamedNodeMap attributes = childElement.attributes();
                    if (checkQtVersion(attributes)){
                        argumentModification.no_null_pointers = true;
                        QString defaultValue = attributeValue(attributes.removeNamedItem("default-value"));
                        if (argumentModification.index == 0) {
                            argumentModification.null_pointer_default_value = defaultValue;
                        } else if (!defaultValue.isEmpty()) {
                            ReportHandler::warning("default values for null pointer guards are only effective for return values");
                        }
                        if(attributes.count()){
                            TypesystemException::raise(QString("Unexpected attribute '%2' of tag <%1> in line %3").arg(childElement.localName()).arg(attributes.item(0).localName()).arg(childElement.lineNumber()));
                        }
                        ensureNoChildren(childElement);

                    }
                }else if(childElement.localName()=="define-ownership"){
                    QDomNamedNodeMap attributes = childElement.attributes();
                    if (checkQtVersion(attributes)){
                        static const QHash<QString, TypeSystem::Language> languageNames{
                            {"java", TypeSystem::TargetLangCode},
                            {"native", TypeSystem::NativeCode},
                            {"shell", TypeSystem::ShellCode}
                        };

                        QString classAttribute = attributeValue(attributes.removeNamedItem("class"), "java").toLower();
                        TypeSystem::Language lang = languageNames.value(classAttribute, TypeSystem::NoLanguage);
                        if (lang == TypeSystem::NoLanguage) {
                            TypesystemException::raise(QString("Unsupported class attribute '%2' of tag <%1> in line %3").arg(childElement.localName()).arg(classAttribute).arg(element.lineNumber()));
                        }

                        static const QHash<QString, TypeSystem::Ownership> ownershipNames{
                            {"java", TypeSystem::TargetLangOwnership},
                            {"cplusplus", TypeSystem::CppOwnership},
                            {"cpp", TypeSystem::CppOwnership},
                            {"default", TypeSystem::DefaultOwnership},
                            {"ignore", TypeSystem::IgnoreOwnership},
                            {"invalidate", TypeSystem::Invalidate}
                        };

                        QString ownershipAttribute = attributeValue(attributes.removeNamedItem("owner")).toLower();
                        TypeSystem::Ownership owner = ownershipNames.value(ownershipAttribute, TypeSystem::InvalidOwnership);
                        if (owner == TypeSystem::InvalidOwnership) {
                            TypesystemException::raise(QString("Unsupported owner attribute '%2' of tag <%1> in line %3").arg(childElement.localName()).arg(ownershipAttribute).arg(element.lineNumber()));
                        }
                        QString condition = attributeValue(attributes.removeNamedItem("condition"));
                        argumentModification.ownerships[lang] = OwnershipRule{owner, condition};
                        if(attributes.count()){
                            TypesystemException::raise(QString("Unexpected attribute '%2' of tag <%1> in line %3").arg(childElement.localName()).arg(attributes.item(0).localName()).arg(childElement.lineNumber()));
                        }
                        ensureNoChildren(childElement);
                    }
                }else if(childElement.localName()=="remove-argument"){
                    QDomNamedNodeMap attributes = childElement.attributes();
                    if (checkQtVersion(attributes)){
                        argumentModification.removed = true;
                        if(attributes.count()){
                            TypesystemException::raise(QString("Unexpected attribute '%2' of tag <%1> in line %3").arg(childElement.localName()).arg(attributes.item(0).localName()).arg(childElement.lineNumber()));
                        }
                        ensureNoChildren(childElement);
                    }
                }else{
                    TypesystemException::raise(QString("Unexpected tag <%2> as child of tag <%1> in line %3").arg(element.localName()).arg(item.toElement().localName()).arg(item.lineNumber()));
                }
            }
                break;
            case QDomNode::TextNode:
                TypesystemException::raise(QString("Unexpected text content as child of tag <%1> in line %2").arg(element.localName()).arg(item.lineNumber()));
                break;
            case QDomNode::CharacterDataNode:
            case QDomNode::CDATASectionNode:
                TypesystemException::raise(QString("Unexpected CDATA as child of tag <%1> in line %2").arg(element.localName()).arg(item.lineNumber()));
                break;
            case QDomNode::EntityNode:
                TypesystemException::raise(QString("Unexpected entity as child of tag <%1> in line %2").arg(element.localName()).arg(item.lineNumber()));
                break;
            case QDomNode::NotationNode:
                TypesystemException::raise(QString("Unexpected notation as child of tag <%1> in line %2").arg(element.localName()).arg(item.lineNumber()));
                break;
            default: break;
            }
        }
        mod.argument_mods << argumentModification;
    }
}

void Handler::parseConversionRule(const QDomElement &element, const std::function<void(const CodeSnip&)>& appendCodeSnip){
    QDomNamedNodeMap attributes = element.attributes();
    if (checkQtVersion(attributes)){
        static const QHash<QString, TypeSystem::Language> languageNames{
            {"native", TypeSystem::NativeCode},
            {"shell", TypeSystem::ShellCode}
        };

        CodeSnip snip;
        QString languageAttribute = attributeValue(attributes.removeNamedItem("class")).toLower();
        TypeSystem::Language lang = languageNames.value(languageAttribute, TypeSystem::NoLanguage);
        if (lang == TypeSystem::NoLanguage) {
            TypesystemException::raise(QString("Unsupported class attribute '%2' of tag <%1> in line %3").arg(element.localName()).arg(languageAttribute).arg(element.lineNumber()));
        }

        snip.language = lang;

        QDomNodeList childNodes = element.childNodes();
        for(int i=0; i<childNodes.length(); ++i){
            QDomNode item = childNodes.item(i);
            switch(item.nodeType()){
            case QDomNode::ElementNode:
            {
                QDomElement childElement = item.toElement();
                if(childElement.localName()=="insert-template"){
                    parseInsertTemplate(childElement, [&snip](TemplateInstance* templateInstance){snip.addTemplateInstance(templateInstance);});
                }else if(childElement.localName()=="import-file"){
                    parseImportFile(childElement, [&snip](const QString& code){snip.addCode(code);});
                }else{
                    TypesystemException::raise(QString("Unexpected tag <%2> as child of tag <%1> in line %3").arg(element.localName()).arg(childElement.localName()).arg(childElement.lineNumber()));
                }
            }
                break;
            case QDomNode::TextNode:
                snip.addCode(item.toText().data());
                break;
            case QDomNode::CharacterDataNode:
                snip.addCode(item.toCharacterData().data());
                break;
            case QDomNode::CDATASectionNode:
                snip.addCode(item.toCDATASection().data());
                break;
            case QDomNode::EntityNode:
                TypesystemException::raise(QString("Unexpected entity as child of tag <%1> in line %2").arg(element.localName()).arg(item.lineNumber()));
                break;
            case QDomNode::NotationNode:
                TypesystemException::raise(QString("Unexpected notation as child of tag <%1> in line %2").arg(element.localName()).arg(item.lineNumber()));
                break;
            default: break;
            }
        }

        if(attributes.count()){
            TypesystemException::raise(QString("Unexpected attribute '%2' of tag <%1> in line %3").arg(element.localName()).arg(attributes.item(0).localName()).arg(element.lineNumber()));
        }
        appendCodeSnip(snip);
    }
}

void Handler::parseModifyFunction(const QDomElement &element, ComplexTypeEntry* entry){
    QDomNamedNodeMap attributes = element.attributes();
    if (checkQtVersion(attributes)){
        QString signature = attributeValue(attributes.removeNamedItem("signature")).replace(QLatin1String("qreal"), QLatin1String("qtjambireal"));
        for(const NamespacePrefix& prefix : m_namespacePrefixes){
            if(prefix._namespace!=prefix.prefix)
                signature = signature.replace(prefix.prefix, prefix._namespace);
        }
        signature = QString::fromLatin1(QMetaObject::normalizedSignature(qPrintable(signature)));
        if (signature.isEmpty()) {
            TypesystemException::raise(QString("No signature for <%1> in line %2").arg(element.localName()).arg(element.lineNumber()));
        }
        QStringList accesses = attributeValue(attributes.removeNamedItem("access")).trimmed().toLower().split(QLatin1String(","));
        accesses.removeAll(QLatin1String(""));
        QString noExcept = attributeValue(attributes.removeNamedItem("no-except"), "no");
        QString blockExcept = attributeValue(attributes.removeNamedItem("block-exceptions"), "no");
        QString rethrowExcept = attributeValue(attributes.removeNamedItem("rethrow-exceptions"), "no");
        QString rename = attributeValue(attributes.removeNamedItem("rename")).trimmed();
        QString deprecated = attributeValue(attributes.removeNamedItem("deprecated"), "no");
        QString association = attributeValue(attributes.removeNamedItem("associated-to"));
        QString virtualSlot = attributeValue(attributes.removeNamedItem("virtual-slot"), "no");
        QString allowAsSlot = attributeValue(attributes.removeNamedItem("allow-as-slot"), "no");
        QString privateSignal = attributeValue(attributes.removeNamedItem("private-signal"), "no");
        QString threadAffine = attributeValue(attributes.removeNamedItem("thread-affine"), "no");
        FunctionModification mod;
        mod.signature = signature;
        mod.ppCondition = attributeValue(attributes.removeNamedItem("pp-condition"));
        mod.throws = attributeValue(attributes.removeNamedItem("throws")).trimmed();

        static const QHash<QString, FunctionModification::Modifiers> modifierNames{
            {"private", Modification::Private},
            {"public", Modification::Public},
            {"protected", Modification::Protected},
            {"friendly", Modification::Friendly},
            {"final", Modification::Final},
            {"declared-final", Modification::NativeDeclFinal},
            {"non-final", Modification::NonFinal}
        };
        if (!accesses.isEmpty()) {
            for(const QString& access : accesses){
                if(!modifierNames.contains(access)){
                    TypesystemException::raise(QString("Bad access type '%3' in <%1> in line %2").arg(element.localName()).arg(element.lineNumber()).arg(access));
                }
                mod.modifiers |= modifierNames[access];
            }
        }

        if (convertBoolean(deprecated, "deprecated", false)) {
            mod.modifiers |= Modification::Deprecated;
        }
        if (convertBoolean(noExcept, "no-except", false)) {
            mod.modifiers |= Modification::NoExcept;
        }
        if (convertBoolean(blockExcept, "block-exceptions", false)) {
            mod.modifiers |= Modification::BlockExcept;
        }
        if (convertBoolean(rethrowExcept, "rethrow-exceptions", false)) {
            mod.modifiers |= Modification::RethrowExcept;
        }
        if (convertBoolean(virtualSlot, "virtual-slot", false)) {
            mod.modifiers |= Modification::VirtualSlot;
        }
        if (convertBoolean(allowAsSlot, "allow-as-slot", false)) {
            mod.modifiers |= Modification::AllowAsSlot;
        }
        if (convertBoolean(privateSignal, "private-signal", false)) {
            mod.modifiers |= Modification::PrivateSignal;
        }

        QString remove = attributeValue(attributes.removeNamedItem("remove")).toLower();
        if (!remove.isEmpty()) {
            if (remove == QLatin1String("all"))
                mod.removal = TypeSystem::All;
            else if (remove == QLatin1String("java"))
                mod.removal = TypeSystem::TargetLangAndNativeCode;
            else if (remove == QLatin1String("non-native"))
                mod.removal = TypeSystem::TargetLangCode;
            else if (remove == QLatin1String("none"))
                mod.removal = TypeSystem::NoLanguage;
            else {
                TypesystemException::raise(QString("Bad removal type '%3' in <%1> in line %2").arg(element.localName()).arg(element.lineNumber()).arg(remove));
            }
        }

        if (!association.isEmpty()) {
            association = association.replace(QLatin1String("qreal"), QLatin1String("qtjambireal"));
            mod.association = association;
        }

        if(threadAffine=="ui"){
            mod.modifiers |= Modification::UIThreadAffine;
        }else if(threadAffine=="pixmap"){
            mod.modifiers |= Modification::PixmapThreadAffine;
        }else if(!threadAffine.isEmpty() && convertBoolean(threadAffine, "thread-affine", false)){
            mod.modifiers |= Modification::ThreadAffine;
        }

        if(attributes.count()){
            TypesystemException::raise(QString("Unexpected attribute '%2' of tag <%1> in line %3").arg(element.localName()).arg(attributes.item(0).localName()).arg(element.lineNumber()));
        }

        QDomNodeList childNodes = element.childNodes();
        for(int i=0; i<childNodes.length(); ++i){
            QDomNode item = childNodes.item(i);
            switch(item.nodeType()){
            case QDomNode::ElementNode:
            {
                QDomElement childElement = item.toElement();
                if(childElement.localName()=="modify-argument"){
                    parseModifyArgument(childElement, mod);
                }else if(childElement.localName()=="remove"){
                    QDomNamedNodeMap attributes = childElement.attributes();
                    if (checkQtVersion(attributes)){
                        if(!remove.isEmpty()){
                            TypesystemException::raise(QString("REMOVE is already specified as attribute in tag <%1> in line %2").arg(element.localName()).arg(element.lineNumber()));
                        }
                        static QHash<QString, TypeSystem::Language> languageNames{
                            {"java", TypeSystem::TargetLangAndNativeCode},
                            {"all", TypeSystem::All}
                        };

                        QString languageAttribute = attributeValue(attributes.removeNamedItem("class"), "all").toLower();
                        TypeSystem::Language lang = languageNames.value(languageAttribute, TypeSystem::NoLanguage);
                        if (lang == TypeSystem::NoLanguage) {
                            TypesystemException::raise(QString("Unsupported class attribute '%3' in tag <%1> in line %2").arg(childElement.localName()).arg(childElement.lineNumber()).arg(languageAttribute));
                        }
                        if(attributes.count()){
                            TypesystemException::raise(QString("Unexpected attribute '%2' of tag <%1> in line %3").arg(childElement.localName()).arg(attributes.item(0).localName()).arg(childElement.lineNumber()));
                        }
                        ensureNoChildren(childElement);
                        mod.removal = lang;
                    }
                }else if(childElement.localName()=="rename"){
                    QDomNamedNodeMap attributes = childElement.attributes();
                    if (checkQtVersion(attributes)){
                        rename = attributeValue(attributes.removeNamedItem("to"));
                        if(attributes.count()){
                            TypesystemException::raise(QString("Unexpected attribute '%2' of tag <%1> in line %3").arg(childElement.localName()).arg(attributes.item(0).localName()).arg(childElement.lineNumber()));
                        }
                        ensureNoChildren(childElement);
                    }
                }else if(childElement.localName()=="access"){
                    QDomNamedNodeMap attributes = childElement.attributes();
                    if (checkQtVersion(attributes)){
                        if(!accesses.isEmpty()){
                            TypesystemException::raise(QString("ACCESS is already specified as attribute in tag <%1> in line %2").arg(element.localName()).arg(element.lineNumber()));
                        }
                        QString access = attributeValue(attributes.removeNamedItem("modifier"));
                        if (!access.isEmpty()) {
                            if(!modifierNames.contains(access)){
                                TypesystemException::raise(QString("Bad access type '%3' in <%1> in line %2").arg(element.localName()).arg(element.lineNumber()).arg(access));
                            }
                            mod.modifiers |= modifierNames[access];
                        }
                        if(attributes.count()){
                            TypesystemException::raise(QString("Unexpected attribute '%2' of tag <%1> in line %3").arg(childElement.localName()).arg(attributes.item(0).localName()).arg(childElement.lineNumber()));
                        }
                        ensureNoChildren(childElement);
                    }
                }else if(childElement.localName()=="add-argument"){
                    QDomNamedNodeMap attributes = childElement.attributes();
                    if (checkQtVersion(attributes)){
                        ArgumentModification argumentModification = ArgumentModification(ArgumentModification::ADDED_ARGUMENT);
                        argumentModification.modified_name = attributeValue(attributes.removeNamedItem("name"));
                        argumentModification.modified_type = attributeValue(attributes.removeNamedItem("type"));
                        argumentModification.modified_jni_type = attributeValue(attributes.removeNamedItem("jni-type"));
                        argumentModification.reset_after_use = convertBoolean(attributeValue(attributes.removeNamedItem("invalidate-after-use"), "no"), "invalidate-after-use", false);
                        if(attributes.count()){
                            TypesystemException::raise(QString("Unexpected attribute '%2' of tag <%1> in line %3").arg(childElement.localName()).arg(attributes.item(0).localName()).arg(childElement.lineNumber()));
                        }
                        ensureNoChildren(childElement);
                        mod.argument_mods.append(argumentModification);
                    }
                }else if(childElement.localName()=="inject-code"){
                    static const QHash<QString, TypeSystem::Language> languageNames{
                        {"java", TypeSystem::TargetLangCode},
                        {"java-interface-default", TypeSystem::Interface},
                        {"native", TypeSystem::NativeCode},
                        {"shell", TypeSystem::ShellCode},
                        {"signal", TypeSystem::Signal}
                    };

                    static const QHash<QString, CodeSnip::Position> positionNames{
                        {"begin", CodeSnip::Beginning},
                        {"beginning", CodeSnip::Beginning},
                        {"position1", CodeSnip::Position1},
                        {"position2", CodeSnip::Position2},
                        {"position3", CodeSnip::Position3},
                        {"position4", CodeSnip::Position4},
                        {"1", CodeSnip::Position1},
                        {"2", CodeSnip::Position2},
                        {"3", CodeSnip::Position3},
                        {"4", CodeSnip::Position4},
                        {"end", CodeSnip::End}
                    };
                    parseInjectCode(childElement,
                                        languageNames,
                                        positionNames,
                                        [&mod](const CodeSnip &snip){mod.snips << snip;},
                                        true);
                }else{
                    TypesystemException::raise(QString("Unexpected tag <%2> as child of tag <%1> in line %3").arg(element.localName()).arg(item.toElement().localName()).arg(item.lineNumber()));
                }
            }
                break;
            case QDomNode::TextNode:
                TypesystemException::raise(QString("Unexpected text content as child of tag <%1> in line %2").arg(element.localName()).arg(item.lineNumber()));
                break;
            case QDomNode::CharacterDataNode:
            case QDomNode::CDATASectionNode:
                TypesystemException::raise(QString("Unexpected CDATA as child of tag <%1> in line %2").arg(element.localName()).arg(item.lineNumber()));
                break;
            case QDomNode::EntityNode:
                TypesystemException::raise(QString("Unexpected entity as child of tag <%1> in line %2").arg(element.localName()).arg(item.lineNumber()));
                break;
            case QDomNode::NotationNode:
                TypesystemException::raise(QString("Unexpected notation as child of tag <%1> in line %2").arg(element.localName()).arg(item.lineNumber()));
                break;
            default: break;
            }
        }

        if (!rename.isEmpty()) {
            mod.renamedToName = rename;
            mod.modifiers |= Modification::Rename;
        }
        entry->addFunctionModification(mod);
        if(entry->designatedInterface())
            entry->designatedInterface()->addFunctionModification(mod);
    }
}

void Handler::parseModifyField(const QDomElement &element, ComplexTypeEntry* entry){
    QDomNamedNodeMap attributes = element.attributes();
    if (checkQtVersion(attributes)){
        FieldModification fm;
        fm.name = attributeValue(attributes.removeNamedItem("name"));
        fm.modifiers = 0;
        if (convertBoolean(attributeValue(attributes.removeNamedItem("read"), "true"), "read", false)) {
            fm.modifiers |= FieldModification::Readable;
        }
        if (convertBoolean(attributeValue(attributes.removeNamedItem("write"), "true"), "write", false)) {
            fm.modifiers |= FieldModification::Writable;
        }
        QString rename = attributeValue(attributes.removeNamedItem("rename"));
        if (!rename.isEmpty()){
            fm.setRenamedTo(rename);
        }
        QStringList accesses = attributeValue(attributes.removeNamedItem("access")).trimmed().toLower().split(QLatin1String(","));
        accesses.removeAll(QLatin1String(""));
        if(attributes.count()){
            TypesystemException::raise(QString("Unexpected attribute '%2' of tag <%1> in line %3").arg(element.localName()).arg(attributes.item(0).localName()).arg(element.lineNumber()));
        }

        static const QHash<QString, FunctionModification::Modifiers> modifierNames{
            {"private", Modification::Private},
            {"public", Modification::Public},
            {"protected", Modification::Protected},
            {"friendly", Modification::Friendly},
            {"rename", Modification::Rename},
            {"final", Modification::Final},
            {"declared-final", Modification::NativeDeclFinal},
            {"non-final", Modification::NonFinal}
        };

        if (!accesses.isEmpty() && !accesses[0].isEmpty()) {
            for(const QString& access : accesses){
                if(!modifierNames.contains(access)){
                    TypesystemException::raise(QString("Bad access type '%3' in <%1> in line %2").arg(element.localName()).arg(element.lineNumber()).arg(access));
                }
                fm.modifiers |= modifierNames[access];
            }
        }

        QDomNodeList childNodes = element.childNodes();
        for(int i=0; i<childNodes.length(); ++i){
            QDomNode item = childNodes.item(i);
            switch(item.nodeType()){
            case QDomNode::ElementNode:
            {
                QDomElement childElement = item.toElement();
                if(childElement.localName()=="rename"){
                    QDomNamedNodeMap attributes = childElement.attributes();
                    if (checkQtVersion(attributes)){
                        rename = attributeValue(attributes.removeNamedItem("to"));
                        if(attributes.count()){
                            TypesystemException::raise(QString("Unexpected attribute '%2' of tag <%1> in line %3").arg(childElement.localName()).arg(attributes.item(0).localName()).arg(childElement.lineNumber()));
                        }
                        ensureNoChildren(childElement);
                    }
                }else if(childElement.localName()=="access"){
                    QDomNamedNodeMap attributes = childElement.attributes();
                    if (checkQtVersion(attributes)){
                        if(!accesses.isEmpty()){
                            TypesystemException::raise(QString("ACCESS is already specified as attribute in tag <%1> in line %2").arg(element.localName()).arg(element.lineNumber()));
                        }
                        QString access = attributeValue(attributes.removeNamedItem("modifier"));
                        if (!access.isEmpty()) {
                            if(!modifierNames.contains(access)){
                                TypesystemException::raise(QString("Bad access type '%3' in <%1> in line %2").arg(element.localName()).arg(element.lineNumber()).arg(access));
                            }
                            fm.modifiers |= modifierNames[access];
                        }
                        if(attributes.count()){
                            TypesystemException::raise(QString("Unexpected attribute '%2' of tag <%1> in line %3").arg(childElement.localName()).arg(attributes.item(0).localName()).arg(childElement.lineNumber()));
                        }
                        ensureNoChildren(childElement);
                    }
                }else if(childElement.localName()=="replace-type"){
                    QDomNamedNodeMap attributes = childElement.attributes();
                    if (checkQtVersion(attributes)){
                        QString modifiedType = attributeValue(attributes.removeNamedItem("modified-type"));
                        if (modifiedType.isEmpty()) {
                            TypesystemException::raise(QString("Missing required 'modified-type' attribute in tag <%1> in line %2").arg(childElement.localName()).arg(childElement.lineNumber()));
                        }
                        fm.modified_type = modifiedType;
                        fm.modified_jni_type = attributeValue(attributes.removeNamedItem("modified-jni-type"));
                        if(attributes.count()){
                            TypesystemException::raise(QString("Unexpected attribute '%2' of tag <%1> in line %3").arg(childElement.localName()).arg(attributes.item(0).localName()).arg(childElement.lineNumber()));
                        }
                        ensureNoChildren(childElement);
                    }
                }else if(childElement.localName()=="conversion-rule"){
                    parseConversionRule(childElement, [&fm](const CodeSnip& snip){fm.conversion_rules.append(snip);});
                }else if(childElement.localName()=="reference-count"){
                    QDomNamedNodeMap attributes = childElement.attributes();
                    if (checkQtVersion(attributes)){
                        ReferenceCount rc;
                        rc.threadSafe = convertBoolean(attributeValue(attributes.removeNamedItem("thread-safe"), "no"), "thread-safe", false);
                        static const QHash<QString, ReferenceCount::Action> actions{
                            {"put", ReferenceCount::Put},
                            {"add", ReferenceCount::Add},
                            {"add-all", ReferenceCount::AddAll},
                            {"clear-add", ReferenceCount::ClearAdd},
                            {"clear-add-all", ReferenceCount::ClearAddAll},
                            {"remove", ReferenceCount::Remove},
                            {"set", ReferenceCount::Set},
                            {"ignore", ReferenceCount::Ignore}
                        };
                        QString action = attributeValue(attributes.removeNamedItem("action"));
                        if (!actions.contains(action)) {
                            TypesystemException::raise(QString("unrecognized value '%2' for action attribute of tag <%1> in line %3").arg(childElement.localName()).arg(action).arg(childElement.lineNumber()));
                        }
                        rc.action = actions.value(action.toLower(), ReferenceCount::Invalid);
                        rc.variableName = attributeValue(attributes.removeNamedItem("variable-name"));
                        if (rc.action != ReferenceCount::Ignore && rc.variableName.isEmpty()) {
                            TypesystemException::raise(QString("variable-name attribute must be specified in tag <%1> in line %2").arg(childElement.localName()).arg(childElement.lineNumber()));
                        }
                        bool ok = false;
                        uint ka = attributeValue(attributes.removeNamedItem("key-argument")).toUInt(&ok);
                        rc.keyArgument = ok ? ka : 0;
                        if (rc.action == ReferenceCount::Put && rc.keyArgument<=0) {
                            TypesystemException::raise(QString("key-argument attribute must be specified and greater than 0 in tag <%1> in line %2").arg(childElement.localName()).arg(childElement.lineNumber()));
                        }
                        rc.declareVariable = attributeValue(attributes.removeNamedItem("declare-variable"));
                        rc.condition = attributeValue(attributes.removeNamedItem("condition"));

                        static const QHash<QString, uint> accessRights{
                            {"private", ReferenceCount::Private},
                            {"public", ReferenceCount::Public},
                            {"protected", ReferenceCount::Protected},
                            {"friendly", ReferenceCount::Friendly}
                        };
                        QString access = attributeValue(attributes.removeNamedItem("access"), "private");
                        if (!accessRights.contains(access)) {
                            TypesystemException::raise(QString("unrecognized access value '%2' of tag <%1> in line %3").arg(childElement.localName()).arg(access).arg(childElement.lineNumber()));
                        }
                        rc.access = accessRights.value(access.toLower(), 0);

                        if(attributes.count()){
                            TypesystemException::raise(QString("Unexpected attribute '%2' of tag <%1> in line %3").arg(childElement.localName()).arg(attributes.item(0).localName()).arg(childElement.lineNumber()));
                        }
                        ensureNoChildren(childElement);
                        fm.referenceCounts.append(rc);
                    }
                }else if(childElement.localName()=="no-null-pointer"){
                    QDomNamedNodeMap attributes = childElement.attributes();
                    if (checkQtVersion(attributes)){
                        fm.no_null_pointers = true;
                        if(attributes.count()){
                            TypesystemException::raise(QString("Unexpected attribute '%2' of tag <%1> in line %3").arg(childElement.localName()).arg(attributes.item(0).localName()).arg(childElement.lineNumber()));
                        }
                        ensureNoChildren(childElement);
                    }
                }else if(childElement.localName()=="define-ownership"){
                    QDomNamedNodeMap attributes = childElement.attributes();
                    if (checkQtVersion(attributes)){
                        static const QHash<QString, TypeSystem::Language> languageNames{
                            {"java", TypeSystem::TargetLangCode},
                            {"native", TypeSystem::NativeCode},
                            {"shell", TypeSystem::ShellCode}
                        };

                        QString classAttribute = attributeValue(attributes.removeNamedItem("class"), "java").toLower();
                        TypeSystem::Language lang = languageNames.value(classAttribute, TypeSystem::NoLanguage);
                        if (lang == TypeSystem::NoLanguage) {
                            TypesystemException::raise(QString("Unsupported class attribute '%2' of tag <%1> in line %3").arg(childElement.localName()).arg(classAttribute).arg(element.lineNumber()));
                        }

                        static const QHash<QString, TypeSystem::Ownership> ownershipNames{
                            {"java", TypeSystem::TargetLangOwnership},
                            {"cplusplus", TypeSystem::CppOwnership},
                            {"cpp", TypeSystem::CppOwnership},
                            {"default", TypeSystem::DefaultOwnership},
                            {"ignore", TypeSystem::IgnoreOwnership},
                            {"invalidate", TypeSystem::Invalidate}
                        };

                        QString ownershipAttribute = attributeValue(attributes.removeNamedItem("owner")).toLower();
                        TypeSystem::Ownership owner = ownershipNames.value(ownershipAttribute, TypeSystem::InvalidOwnership);
                        if (owner == TypeSystem::InvalidOwnership) {
                            TypesystemException::raise(QString("Unsupported owner attribute '%2' of tag <%1> in line %3").arg(childElement.localName()).arg(ownershipAttribute).arg(element.lineNumber()));
                        }
                        QString condition = attributeValue(attributes.removeNamedItem("condition"));
                        fm.ownerships[lang] = OwnershipRule{owner, condition};
                        if(attributes.count()){
                            TypesystemException::raise(QString("Unexpected attribute '%2' of tag <%1> in line %3").arg(childElement.localName()).arg(attributes.item(0).localName()).arg(childElement.lineNumber()));
                        }
                        ensureNoChildren(childElement);
                    }
                }
            }
                break;
            case QDomNode::TextNode:
                TypesystemException::raise(QString("Unexpected text content as child of tag <%1> in line %2").arg(element.localName()).arg(item.lineNumber()));
                break;
            case QDomNode::CDATASectionNode:
                TypesystemException::raise(QString("Unexpected CDATA as child of tag <%1> in line %2").arg(element.localName()).arg(item.lineNumber()));
                break;
            case QDomNode::EntityNode:
                TypesystemException::raise(QString("Unexpected entity as child of tag <%1> in line %2").arg(element.localName()).arg(item.lineNumber()));
                break;
            case QDomNode::NotationNode:
                TypesystemException::raise(QString("Unexpected notation as child of tag <%1> in line %2").arg(element.localName()).arg(item.lineNumber()));
                break;
            default: break;
            }
        }
        if (!rename.isEmpty()) {
            fm.renamedToName = rename;
            fm.modifiers |= Modification::Rename;
        }
        entry->addFieldModification(fm);
        if(entry->designatedInterface())
            entry->designatedInterface()->addFieldModification(fm);
    }
}

void Handler::ensureNoChildren(const QDomElement &element){
    QDomNodeList childNodes = element.childNodes();
    for(int i=0; i<childNodes.length(); ++i){
        QDomNode item = childNodes.item(i);
        switch(item.nodeType()){
        case QDomNode::ElementNode:
            TypesystemException::raise(QString("Unexpected tag <%2> as child of tag <%1> in line %3").arg(element.localName()).arg(item.toElement().localName()).arg(item.lineNumber()));
            break;
        case QDomNode::TextNode:
            if(!item.toText().data().trimmed().isEmpty()){
                TypesystemException::raise(QString("Unexpected text content as child of tag <%1> in line %2").arg(element.localName()).arg(item.lineNumber()));
            }
            break;
        case QDomNode::CharacterDataNode:
        case QDomNode::CDATASectionNode:
            TypesystemException::raise(QString("Unexpected CDATA as child of tag <%1> in line %2").arg(element.localName()).arg(item.lineNumber()));
            break;
        case QDomNode::EntityNode:
            TypesystemException::raise(QString("Unexpected entity as child of tag <%1> in line %2").arg(element.localName()).arg(item.lineNumber()));
            break;
        case QDomNode::NotationNode:
            TypesystemException::raise(QString("Unexpected notation as child of tag <%1> in line %2").arg(element.localName()).arg(item.lineNumber()));
            break;
        default: break;
        }
    }
}

void Handler::parseFunctionalType(const QDomElement &element){
    QDomNamedNodeMap attributes = element.attributes();
    if (checkQtVersion(attributes)){
        QString name = parseTypeName(attributes, element);
        QString targetName;
        for(const NamespacePrefix& prefix : m_namespacePrefixes){
            if(name.startsWith(prefix.prefix)){
                name = name.mid(prefix.prefix.size());
                if(prefix.namingPolicy==NamespacePrefix::Cut)
                    targetName = name;
                name = prefix._namespace + name;
                break;
            }
        }
        QStringList names = name.split(QLatin1String("::"));
        QString generate = attributeValue(attributes.removeNamedItem("generate"), "yes");
        QString package = attributeValue(attributes.removeNamedItem("package"), m_defaultPackage);
        QString implements = attributeValue(attributes.removeNamedItem("implements"));
        QString ppCondition = attributeValue(attributes.removeNamedItem("pp-condition"));
        QString _using = attributeValue(attributes.removeNamedItem("using"));
        QString javaName = attributeValue(attributes.removeNamedItem("java-name"));
        QString _count = attributeValue(attributes.removeNamedItem("count"), "0");
        QString noExcept = attributeValue(attributes.removeNamedItem("no-except"), "false");
        QString blockExcept = attributeValue(attributes.removeNamedItem("block-exceptions"), "no");
        QString rethrowExcept = attributeValue(attributes.removeNamedItem("rethrow-exceptions"), "no");
        QString functionName = attributeValue(attributes.removeNamedItem("function-name"), "");
        QString disableNativeIdUsage = attributeValue(attributes.removeNamedItem("disable-native-id-usage"), "no");
        if(attributes.count()){
            TypesystemException::raise(QString("Unexpected attribute '%2' of tag <%1> in line %3").arg(element.localName()).arg(attributes.item(0).localName()).arg(element.lineNumber()));
        }

        std::unique_ptr<FunctionalTypeEntry> fentry;
        if (names.size() == 1) {
            fentry.reset(new FunctionalTypeEntry(QString(), name));
            if(!targetName.isEmpty())
                fentry->setTargetLangName(targetName);
        } else {
            fentry.reset(new FunctionalTypeEntry(QStringList(names.mid(0, names.size() - 1)).join("::"), names.last()));
        }
        fentry->setFunctionName(functionName);
        if(generate=="no-shell"){
            fentry->setCodeGeneration(TypeEntry::GenerateNoShell | TypeEntry::GenerateAll);
        }else if (!convertBoolean(generate, "generate", true))
            fentry->setCodeGeneration(TypeEntry::GenerateNothing);
        else
            fentry->setCodeGeneration(m_generate);
        if (convertBoolean(disableNativeIdUsage, "disable-native-id-usage", false))
            fentry->disableNativeIdUsage();
        {
            Modification mod;
            if (convertBoolean(noExcept, "no-except", false)) {
                mod.modifiers |= Modification::NoExcept;
            }
            if (convertBoolean(blockExcept, "block-exceptions", false)) {
                mod.modifiers |= Modification::BlockExcept;
            }
            if (convertBoolean(rethrowExcept, "rethrow-exceptions", false)) {
                mod.modifiers |= Modification::RethrowExcept;
            }
            if(mod.modifiers)
                fentry->addModification(mod);
        }
        fentry->setTargetLangPackage(package);
        fentry->setTargetTypeSystem(m_defaultPackage);
        fentry->setImplements(implements);
        fentry->setPPCondition(ppCondition);
        fentry->setUsing(_using);
        if(!javaName.isEmpty())
            fentry->setTargetLangName(javaName);

        bool ok = false;
        uint count = _count.toUInt(&ok);
        if (!ok) {
            TypesystemException::raise(QString("Cannot convert count='%1' to integer in tag <%2> in line %3").arg(_count).arg(element.localName()).arg(element.lineNumber()));
        }
        fentry->setCount(count);
        QDomNodeList childNodes = element.childNodes();
        for(int i=0; i<childNodes.length(); ++i){
            QDomNode item = childNodes.item(i);
            switch(item.nodeType()){
            case QDomNode::ElementNode:
            {
                QDomElement childElement = item.toElement();
                if(childElement.localName()=="modify-argument"){
                    FunctionModification mod;
                    parseModifyArgument(childElement, mod);
                    for(const ArgumentModification& am : mod.argument_mods){
                        fentry->addArgumentModification(am);
                    }
                }else if(childElement.localName()=="inject-code"){
                    parseInjectCode(childElement, fentry.get());
                }else if(childElement.localName()=="include"){
                    Include incl = parseInclude(childElement);
                    if(incl.isValid())
                        fentry->setInclude(incl);
                }else if(childElement.localName()=="extra-includes"){
                    parseExtraIncludes(childElement, fentry.get());
                }else{
                    TypesystemException::raise(QString("Unexpected tag <%2> as child of tag <%1> in line %3").arg(element.localName()).arg(item.toElement().localName()).arg(item.lineNumber()));
                }
            }
                break;
            case QDomNode::TextNode:
                TypesystemException::raise(QString("Unexpected text content as child of tag <%1> in line %2").arg(element.localName()).arg(item.lineNumber()));
                break;
            case QDomNode::CharacterDataNode:
            case QDomNode::CDATASectionNode:
                TypesystemException::raise(QString("Unexpected CDATA as child of tag <%1> in line %2").arg(element.localName()).arg(item.lineNumber()));
                break;
            case QDomNode::EntityNode:
                TypesystemException::raise(QString("Unexpected entity as child of tag <%1> in line %2").arg(element.localName()).arg(item.lineNumber()));
                break;
            case QDomNode::NotationNode:
                TypesystemException::raise(QString("Unexpected notation as child of tag <%1> in line %2").arg(element.localName()).arg(item.lineNumber()));
                break;
            default: break;
            }
        }

        m_database->addType(fentry.release());
    }
}

void Handler::parseIteratorType(const QDomElement &element){
    QDomNamedNodeMap attributes = element.attributes();
    if (checkQtVersion(attributes)){
        QString name = parseTypeName(attributes, element);
        for(const NamespacePrefix& prefix : m_namespacePrefixes){
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
        QString genericClass = attributeValue(attributes.removeNamedItem("generic-class"), "no");
        entry->setGenericClass(convertBoolean(genericClass, "generic-class", false));
        if(attributes.count()){
            TypesystemException::raise(QString("Unexpected attribute '%2' of tag <%1> in line %3").arg(element.localName()).arg(attributes.item(0).localName()).arg(element.lineNumber()));
        }
        ensureNoChildren(element);
        ReportHandler::debugTypes("Adding to TypeDatabase(2): " + entry->name());
        m_database->addType(entry.release());
    }
}

void Handler::parseValueType(const QDomElement &element){
    QDomNamedNodeMap attributes = element.attributes();
    if (checkQtVersion(attributes)){
        QString name = parseTypeName(attributes, element);
        QString targetName;
        for(const NamespacePrefix& prefix : m_namespacePrefixes){
            if(name.startsWith(prefix.prefix)){
                name = name.mid(prefix.prefix.size());
                if(prefix.namingPolicy==NamespacePrefix::Cut)
                    targetName = name;
                name = prefix._namespace + name;
                break;
            }
        }
        std::unique_ptr<ValueTypeEntry> entry(new ValueTypeEntry(name));
        if(!targetName.isEmpty())
            entry->setTargetLangName(targetName);
        entry->setIsPolymorphicBase(convertBoolean(attributeValue(attributes.removeNamedItem("polymorphic-base"), "no"), "polymorphic-base", false));
        entry->setPolymorphicIdValue(attributeValue(attributes.removeNamedItem("polymorphic-id-expression")));
        parseAttributesOfComplexType(element, attributes, entry.get());
        QList<QDomElement> unhandledElements = parseChildrenOfComplexType(element, entry.get());
        for(const QDomElement& childElement : unhandledElements){
            TypesystemException::raise(QString("Unexpected tag <%2> as child of tag <%1> in line %3").arg(element.localName()).arg(childElement.toElement().localName()).arg(childElement.lineNumber()));
        }
        if(entry->name().endsWith(">")){
            auto idx = entry->name().indexOf('<');
            QString templateName = entry->name().mid(0, idx);
            QStringList templateArguments = entry->name().mid(idx+1).chopped(1).split(",");
            ComplexTypeEntry* templateType = m_database->findComplexType(templateName);
            if(templateType){
                if(templateType->isTemplate()){
                    if(templateType->instantiations().contains(templateArguments) && templateType->instantiations()[templateArguments]==nullptr){
                        if(entry->targetLangName()==entry->name()){
                            entry->setTargetLangName(templateName);
                        }
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
    }
}

QString Handler::parseTypeName(QDomNamedNodeMap& attributes, const QDomElement &element){
    QString name = attributeValue(attributes.removeNamedItem("name"));
    if (name.isEmpty()) {
        TypesystemException::raise(QString("No 'name' attribute specified for tag <%1> in line %2").arg(element.localName()).arg(element.lineNumber()));
    }
    TypeEntry *tmp = m_database->findType(name);
    if (tmp) {
        ReportHandler::warning(QString("Duplicate type entry: '%1'").arg(name));
    }
    return name;
}

void Handler::parseTypeAliasType(const QDomElement &element){
    QDomNamedNodeMap attributes = element.attributes();
    if (checkQtVersion(attributes)){
        QString name = parseTypeName(attributes, element);
        for(const NamespacePrefix& prefix : m_namespacePrefixes){
            if(name.startsWith(prefix.prefix)){
                name = name.mid(prefix.prefix.size());
                name = prefix._namespace + name;
                break;
            }
        }
        if(attributes.count()){
            TypesystemException::raise(QString("Unexpected attribute '%2' of tag <%1> in line %3").arg(element.localName()).arg(attributes.item(0).localName()).arg(element.lineNumber()));
        }
        std::unique_ptr<AliasTypeEntry> entry(new AliasTypeEntry(name, m_defaultPackage));
        QDomNodeList childNodes = element.childNodes();
        for(int i=0; i<childNodes.length(); ++i){
            QDomNode item = childNodes.item(i);
            switch(item.nodeType()){
            case QDomNode::ElementNode:
                TypesystemException::raise(QString("Unexpected tag <%2> as child of tag <%1> in line %3").arg(element.localName()).arg(item.toElement().localName()).arg(item.lineNumber()));
                break;
            case QDomNode::TextNode:
                TypesystemException::raise(QString("Unexpected text content as child of tag <%1> in line %2").arg(element.localName()).arg(item.lineNumber()));
                break;
            case QDomNode::CharacterDataNode:
            case QDomNode::CDATASectionNode:
                TypesystemException::raise(QString("Unexpected CDATA as child of tag <%1> in line %2").arg(element.localName()).arg(item.lineNumber()));
                break;
            case QDomNode::EntityNode:
                TypesystemException::raise(QString("Unexpected entity as child of tag <%1> in line %2").arg(element.localName()).arg(item.lineNumber()));
                break;
            case QDomNode::NotationNode:
                TypesystemException::raise(QString("Unexpected notation as child of tag <%1> in line %2").arg(element.localName()).arg(item.lineNumber()));
                break;
            default: break;
            }
        }
        if(entry->name().endsWith(">")){
            auto idx = entry->name().indexOf('<');
            QString templateName = entry->name().mid(0, idx);
            QStringList templateArguments = entry->name().mid(idx+1).chopped(1).split(",");
            ComplexTypeEntry* templateType = m_database->findComplexType(templateName);
            if(templateType){
                if(templateType->isTemplate()){
                    if(templateType->instantiations().contains(templateArguments) && templateType->instantiations()[templateArguments]==nullptr){
                        if(entry->targetLangName()==entry->name()){
                            entry->setTargetLangName(templateName);
                        }
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
    }
}

void Handler::parseInterfaceType(const QDomElement &element){
    QDomNamedNodeMap attributes = element.attributes();
    if (checkQtVersion(attributes)){
        QString name = parseTypeName(attributes, element);
        QString targetName;
        for(const NamespacePrefix& prefix : m_namespacePrefixes){
            if(name.startsWith(prefix.prefix)){
                name = name.mid(prefix.prefix.size());
                if(prefix.namingPolicy==NamespacePrefix::Cut)
                    targetName = name;
                name = prefix._namespace + name;
                break;
            }
        }
        std::unique_ptr<ImplementorTypeEntry> otype;
        if(convertBoolean(attributeValue(attributes.removeNamedItem("is-value"), "no"), "is-value", false))
            otype.reset(new ValueTypeEntry(InterfaceTypeEntry::implName(name)));
        else
            otype.reset(new ObjectTypeEntry(InterfaceTypeEntry::implName(name)));
        if(!targetName.isEmpty())
            otype->setTargetLangName(targetName);
        std::unique_ptr<InterfaceTypeEntry> itype(new InterfaceTypeEntry(name));
        otype->setDesignatedInterface(itype.get());
        itype->setOrigin(otype.get());
        itype->setNoImpl(convertBoolean(attributeValue(attributes.removeNamedItem("no-impl"), "no"), "no-impl", false));
        parseAttributesOfComplexType(element, attributes, otype.get());
        QList<QDomElement> unhandledElements = parseChildrenOfComplexType(element, otype.get());
        for(const QDomElement& childElement : unhandledElements){
            TypesystemException::raise(QString("Unexpected tag <%2> as child of tag <%1> in line %3").arg(element.localName()).arg(childElement.toElement().localName()).arg(childElement.lineNumber()));
        }
        m_database->addType(otype.release());
        itype.release();
    }
}

void Handler::parseNamespaceType(const QDomElement &element){
    QDomNamedNodeMap attributes = element.attributes();
    if (checkQtVersion(attributes)){
        QString name = parseTypeName(attributes, element);
        QString targetName;
        for(const NamespacePrefix& prefix : m_namespacePrefixes){
            if(name.startsWith(prefix.prefix)){
                name = name.mid(prefix.prefix.size());
                if(prefix.namingPolicy==NamespacePrefix::Cut)
                    targetName = name;
                name = prefix._namespace + name;
                break;
            }
        }
        std::unique_ptr<NamespaceTypeEntry> entry(new NamespaceTypeEntry(name));
        if(!targetName.isEmpty())
            entry->setTargetLangName(targetName);
        parseAttributesOfComplexType(element, attributes, entry.get());
        QList<QDomElement> unhandledElements = parseChildrenOfComplexType(element, entry.get());
        for(const QDomElement& childElement : unhandledElements){
            TypesystemException::raise(QString("Unexpected tag <%2> as child of tag <%1> in line %3").arg(element.localName()).arg(childElement.toElement().localName()).arg(childElement.lineNumber()));
        }
        ReportHandler::debugTypes("Adding to TypeDatabase(2): " + entry->name());
        m_database->addType(entry.release());
    }
}

void Handler::parseEnumType(const QDomElement &element){
    QDomNamedNodeMap attributes = element.attributes();
    if (checkQtVersion(attributes)){
        QString name = parseTypeName(attributes, element);
        QString flags = attributeValue(attributes.removeNamedItem("flags"));
        for(const NamespacePrefix& prefix : m_namespacePrefixes){
            if(name.startsWith(prefix.prefix)){
                name = name.mid(prefix.prefix.size());
                name = prefix._namespace + name;
                if(flags.startsWith(prefix.prefix)){
                    flags = flags.mid(prefix.prefix.size());
                    flags = prefix._namespace + flags;
                    break;
                }
                break;
            }
        }
        QStringList names = name.split(QLatin1String("::"));
        QString generate = attributeValue(attributes.removeNamedItem("generate"), "yes");
        QString package = attributeValue(attributes.removeNamedItem("package"), m_defaultPackage);
        QString upperBound = attributeValue(attributes.removeNamedItem("upper-bound"));
        QString lowerBound = attributeValue(attributes.removeNamedItem("lower-bound"));
        QString cppName = attributeValue(attributes.removeNamedItem("cpp-name"));
        QString implements = attributeValue(attributes.removeNamedItem("implements"));
        QString forceInteger = attributeValue(attributes.removeNamedItem("force-integer"), "no");
        QString extensible = attributeValue(attributes.removeNamedItem("extensible"), "no");
        QString hiddenMetaobject = attributeValue(attributes.removeNamedItem("hidden-metaobject"), "no");
        if(attributes.count()){
            TypesystemException::raise(QString("Unexpected attribute '%2' of tag <%1> in line %3").arg(element.localName()).arg(attributes.item(0).localName()).arg(element.lineNumber()));
        }

        std::unique_ptr<EnumTypeEntry> eentry;
        if (names.size() == 1) {
            eentry.reset(new EnumTypeEntry(QString(), name));
        } else {
            eentry.reset(new EnumTypeEntry(QStringList(names.mid(0, names.size() - 1)).join("::"), names.last()));
        }
        if(generate=="no-shell"){
            eentry->setCodeGeneration(TypeEntry::GenerateNoShell | TypeEntry::GenerateAll);
        }else if (!convertBoolean(generate, "generate", true))
            eentry->setCodeGeneration(TypeEntry::GenerateNothing);
        else
            eentry->setCodeGeneration(m_generate);
        eentry->setTargetLangPackage(package);
        eentry->setTargetTypeSystem(m_defaultPackage);
        eentry->setUpperBound(upperBound);
        eentry->setLowerBound(lowerBound);
        if(!cppName.isEmpty()){
            eentry->setCppName(cppName);
        }
        eentry->setImplements(implements);
        eentry->setForceInteger(convertBoolean(forceInteger, "force-integer", false));
        eentry->setExtensible(convertBoolean(extensible, "extensible", false));
        eentry->setHiddenMetaObject(convertBoolean(hiddenMetaobject, "hidden-metaobject", false));

        QDomNodeList childNodes = element.childNodes();
        for(int i=0; i<childNodes.length(); ++i){
            QDomNode item = childNodes.item(i);
            switch(item.nodeType()){
            case QDomNode::ElementNode:
            {
                QDomElement childElement = item.toElement();
                if(childElement.localName()=="reject-enum-value"){
                    QDomNamedNodeMap attributes = childElement.attributes();
                    if (checkQtVersion(attributes)){
                        QString name = attributeValue(attributes.removeNamedItem("name"));
                        if (name.isEmpty()) {
                            TypesystemException::raise(QString("No 'name' attribute specified for tag <%1> in line %2").arg(childElement.localName()).arg(childElement.lineNumber()));
                        }
                        eentry->addEnumValueRejection(name, convertBoolean(attributeValue(attributes.removeNamedItem("remove"), "no"), "remove", false));
                        if(attributes.count()){
                            TypesystemException::raise(QString("Unexpected attribute of tag <%1>: %2").arg(childElement.localName()).arg(attributes.item(0).localName()));
                        }
                    }
                }else if(childElement.localName()=="rename-enum-value"){
                    QDomNamedNodeMap attributes = childElement.attributes();
                    if (checkQtVersion(attributes)){
                        QString name = attributeValue(attributes.removeNamedItem("name"));
                        if (name.isEmpty()) {
                            TypesystemException::raise(QString("No 'name' attribute specified for tag <%1> in line %2").arg(childElement.localName()).arg(childElement.lineNumber()));
                        }
                        QString rename = attributeValue(attributes.removeNamedItem("rename"));
                        if (rename.isEmpty()) {
                            TypesystemException::raise(QString("No 'rename' attribute specified for tag <%1> in line %2").arg(childElement.localName()).arg(childElement.lineNumber()));
                        }
                        eentry->addRenamedEnumValue(name, rename);
                        if(attributes.count()){
                            TypesystemException::raise(QString("Unexpected attribute of tag <%1>: %2").arg(childElement.localName()).arg(attributes.item(0).localName()));
                        }
                    }
                }else if(childElement.localName()=="inject-code"){
                    static const QHash<QString, TypeSystem::Language> languageNames{
                        {"java", TypeSystem::TargetLangCode},
                    };

                    static const QHash<QString, CodeSnip::Position> positionNames{
                        {"begin", CodeSnip::Beginning},
                        {"beginning", CodeSnip::Beginning},
                        {"position1", CodeSnip::Position1},
                        {"position2", CodeSnip::Position2},
                        {"position3", CodeSnip::Position3},
                        {"position4", CodeSnip::Position4},
                        {"1", CodeSnip::Position1},
                        {"2", CodeSnip::Position2},
                        {"3", CodeSnip::Position3},
                        {"4", CodeSnip::Position4},
                        {"end", CodeSnip::End}
                    };
                    parseInjectCode(childElement, languageNames, positionNames, [&eentry](const CodeSnip &snip){eentry->addCodeSnip(snip);});
                }else{
                    TypesystemException::raise(QString("Unexpected tag <%2> as child of tag <%1> in line %3").arg(element.localName()).arg(childElement.localName()).arg(childElement.lineNumber()));
                }
            }
                break;
            case QDomNode::TextNode:
                TypesystemException::raise(QString("Unexpected text content as child of tag <%1> in line %2").arg(element.localName()).arg(item.lineNumber()));
                break;
            case QDomNode::CharacterDataNode:
            case QDomNode::CDATASectionNode:
                TypesystemException::raise(QString("Unexpected CDATA as child of tag <%1> in line %2").arg(element.localName()).arg(item.lineNumber()));
                break;
            case QDomNode::EntityNode:
                TypesystemException::raise(QString("Unexpected entity as child of tag <%1> in line %2").arg(element.localName()).arg(item.lineNumber()));
                break;
            case QDomNode::NotationNode:
                TypesystemException::raise(QString("Unexpected notation as child of tag <%1> in line %2").arg(element.localName()).arg(item.lineNumber()));
                break;
            default: break;
            }
        }

        // put in the flags parallel...
        if (!flags.isEmpty() && flags.toLower() != "no") {
            std::unique_ptr<FlagsTypeEntry> ftype(new FlagsTypeEntry("QFlags<" + eentry->qualifiedCppName() + ">"));
            ftype->setOriginator(eentry.get());
            QStringList flagsNames = flags.split(QLatin1String("::"));
            if(!cppName.isEmpty()){
                QStringList scope = cppName.split(QLatin1String("::"));
                Q_ASSERT(scope.size());
                Q_ASSERT(flags.size());
                scope.removeLast();
                scope.append(flagsNames.last());
                ftype->setCppName(scope.join("::"));
            }
            ftype->setOriginalName(flags);
            ftype->setCodeGeneration(eentry->codeGeneration());
            QString n = ftype->originalName();

            QStringList lst = n.split("::");
            if (QStringList(lst.mid(0, lst.size() - 1)).join("::") != QStringList(names.mid(0, names.size() - 1)).join("::")) {
                ReportHandler::warning(QString("Enum %1 and flags %2 differ in qualifiers in line %3")
                                       .arg(eentry->qualifiedCppName())
                                       .arg(n).arg(element.lineNumber()));
            }

            ftype->setFlagsName(lst.last());
            eentry->setFlags(ftype.get());

            m_database->addFlagsType(ftype.get());
            //qDebug()<<"Adding ftype"<<ftype->name();
            ReportHandler::debugTypes("Adding to TypeDatabase(1): " + ftype->name());
            m_database->addType(ftype.release());
        }
        m_database->addType(eentry.release());
    }
}

TypesystemException::TypesystemException(const QString& message) : QException(), m_message(message.toUtf8())
{
}

char const* TypesystemException::what() const noexcept{
    return m_message;
}

void TypesystemException::raise(const QString& message) {
    TypesystemException e(message);
    throw e;
}
