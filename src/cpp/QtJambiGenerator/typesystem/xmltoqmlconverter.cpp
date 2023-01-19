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
#include <QtXml/QtXml>

#include "xmltoqmlconverter.h"
#include "typedatabase.h"
#include "../reporthandler.h"
#include "abstractgenerator.h"
#include "typedatabase.h"

using namespace TS;

class XmlToQmlConverterPrivate{
    XmlToQmlConverterPrivate(TypeDatabase *database) : m_database(database) {}

    void parse(const QString &filepath);

    QString escapeQuote(QString text, int offset);
    QString convertBoolean(const QString &, const QString &, bool);

    QString attributeValue(const QDomNode& domNode, const QString & defaultValue = QString());

    QString textArgument(QTextStream& s, QDomNamedNodeMap& attributes, const QString& argument, const QString& qmlArgumentName = {});
    void variantArgument(QTextStream& s, QDomNamedNodeMap& attributes, const QString& argument, const QString& qmlArgumentName = {});
    void textListArgument(QTextStream& s, QDomNamedNodeMap& attributes, const QString& argument, const QString& qmlArgumentName = {});
    void boolArgument(QTextStream& s, QDomNamedNodeMap& attributes, const QString& argument, const QString& qmlArgumentName = {});
    void numberArgument(QTextStream& s, QDomNamedNodeMap& attributes, const QString& argument, const QString& qmlArgumentName = {});
    void checkQtVersion(QTextStream& s, QDomNamedNodeMap& attributes);
    void parseTypeSystem(QTextStream& s, const QDomElement &typeSystemElement, const QString& fileName);
    void parseLoadTypeSystem(QTextStream& s, const QDomElement &element);
    void parseSuppressedWarning(QTextStream& s, const QDomElement &element);
    void parseInjectCode(QTextStream& s, const QDomElement &element, bool argumentMapAllowed = false, bool allowPackage = false);
    void parseConversionRule(QTextStream& s, const QDomElement &element);
    void parseImportFile(QTextStream& s, const QDomElement &element);
    void parseInsertTemplate(QTextStream& s, const QDomElement &element);
    void parseExtraIncludes(QTextStream& s, const QDomElement &element);
    void parseInclude(QTextStream& s, const QDomElement &element);
    void parseRejection(QTextStream& s, const QDomElement &element);
    void parseTemplate(QTextStream& s, const QDomElement &element);
    void parseTemplateInstantiation(QTextStream& s, const QDomElement &element);
    void parsePrimitiveType(QTextStream& s, const QDomElement &element);
    void parseObjectType(QTextStream& s, const QDomElement &element);
    void parseAttributesOfComplexType(QTextStream& s, const QDomElement &element, QDomNamedNodeMap& attributes);
    QList<QDomElement> parseChildrenOfComplexType(QTextStream& s, const QDomElement &element);
    void parseTemplateType(QTextStream& s, const QDomElement &element);
    void parseFunctionalType(QTextStream& s, const QDomElement &element);
    void parseIteratorType(QTextStream& s, const QDomElement &element);
    void parseValueType(QTextStream& s, const QDomElement &element);
    void parseTypeAliasType(QTextStream& s, const QDomElement &element);
    void parseInterfaceType(QTextStream& s, const QDomElement &element);
    void parseNamespaceType(QTextStream& s, const QDomElement &element);
    void parseModifyArgument(QTextStream& s, const QDomElement &element);
    void parseModifyFunction(QTextStream& s, const QDomElement &element);
    void parseInstantiation(QTextStream& s, const QDomElement &element);
    void parseModifyField(QTextStream& s, const QDomElement &element);
    void parseEnumType(QTextStream& s, const QDomElement &element);
    void parseCustomStructor(QTextStream& s, const QDomElement &element);
    void ensureNoChildren(const QDomElement &element);
    TypeDatabase *m_database;
    QStringList m_importInputDirectoryList;
    QStringList m_typesystemDirectoryList;
    Indentor INDENT;

    friend XmlToQmlConverter;
};

XmlToQmlConverter::XmlToQmlConverter(TypeDatabase *database, bool, const QVersionNumber&)
    : d(new XmlToQmlConverterPrivate(database)) {}

XmlToQmlConverter::~XmlToQmlConverter(){ delete d; }

void XmlToQmlConverter::parse(const QString &filepath){
    d->parse(filepath);
}

void XmlToQmlConverter::setImportInputDirectoryList(const QStringList &importInputDirectoryList) {
    d->m_importInputDirectoryList = importInputDirectoryList;
}

void XmlToQmlConverter::setTypesystemsDirectoryList(const QStringList &typesystemDirectoryList) {
    d->m_typesystemDirectoryList = typesystemDirectoryList;
}

void XmlToQmlConverterPrivate::parse(const QString &filepath){
    QFile file(filepath);
    QFile file_out(QString(filepath).replace(".xml", ".qml"));
    if(!file.exists())return;
    QDomDocument document;
    QString errorMsg;
    int errorLine = -1;
    int errorColumn = -1;
    try{
        if(file.open(QIODevice::ReadOnly)
                && document.setContent(&file, true, &errorMsg, &errorLine, &errorColumn)
                && file_out.open(QIODevice::WriteOnly)){
            file.close();
            QTextStream s(&file_out);
            QDomElement documentElement = document.documentElement();
            if(documentElement.localName()=="typesystem"){
                s << INDENT << "import QtJambiGenerator 1.0" << Qt::endl << Qt::endl
                  << INDENT << "TypeSystem{" << Qt::endl;
                {
                    Indentation indent(INDENT);
                    parseTypeSystem(s, documentElement, QFileInfo(filepath).fileName());
                }
                s << INDENT << "}" << Qt::endl;
            }else{
                TypesystemException::raise(QString("Unexpected tag: <%1>. Expected: <typesystem>  (%2:%3:%4)").arg(documentElement.localName(), filepath).arg(errorLine).arg(errorColumn));
            }
        }else{
            TypesystemException::raise(QString("XML Error: %1 (%2:%3:%4)").arg(errorMsg, filepath).arg(errorLine).arg(errorColumn));
        }
    }catch(...){
        file.close();
        file_out.close();
        throw;
    }
}

QString XmlToQmlConverterPrivate::escapeQuote(QString text, int offset){
    if(offset>1){
        QStringList lines = text.trimmed().replace("\\", "\\\\").replace("\"", "\\\"").remove("\r").split("\n");
        if(lines.size()>0){
            QString result;
            QTextStream s(&result);
            bool firstLine = true;
            for(int i=0; i<lines.size(); ++i){
                if(firstLine){
                    firstLine = false;
                }else{
                    s << Qt::endl << INDENT << QString(offset, ' ');
                }
                s << "\"" << lines[i].trimmed();
                if(i<lines.size()-1)
                    s << "\\n";
                s << "\"";
                if(i<lines.size()-1)
                    s << "+";
            }
            return result;
        }else{
            return "\"" + lines[0].trimmed() + "\"";
        }
    }else{
        return "\"" + text.trimmed().replace("\\", "\\\\")
                .replace("\n", "\\n")
                .replace("\r", "\\r")
                .replace("\"", "\\\"")
                .replace("\t", "    ") + "\"";
    }
}

void XmlToQmlConverterPrivate::parseTypeSystem(QTextStream& s, const QDomElement &element, const QString& fileName){
    try{
        QDomNamedNodeMap attributes = element.attributes();
        {
            textArgument(s, attributes, "package", "packageName");
            textArgument(s, attributes, "default-superclass", "defaultSuperClass");
            textArgument(s, attributes, "qt-library", "qtLibrary");
            boolArgument(s, attributes, "no-exports", "noExports");
            QString moduleName = textArgument(s, attributes, "module");
            QString description = textArgument(s, attributes, "description");
            if(element.hasChildNodes()){
                QDomNodeList childNodes = element.childNodes();
                for(int i=0; i<childNodes.length(); ++i){
                    QDomNode item = childNodes.item(i);
                    switch(item.nodeType()){
                    case QDomNode::ElementNode:
                    {
                        QDomElement childElement = item.toElement();
                        if(childElement.localName()=="module"){
                            s << INDENT << "Module{" << Qt::endl;
                            {
                                Indentation indent(INDENT);
                                QDomNamedNodeMap attributes = childElement.attributes();
                                {
                                    if(!moduleName.isEmpty())
                                        TypesystemException::raise(QString("Duplicate module name specification in line %1").arg(childElement.lineNumber()));
                                    textArgument(s, attributes, "name");
                                    checkQtVersion(s, attributes);
                                    if(attributes.count()){
                                        TypesystemException::raise(QString("Unexpected attribute '%2' of tag <%1> in line %3").arg(childElement.localName()).arg(attributes.item(0).localName()).arg(element.lineNumber()));
                                    }
                                }
                            }
                            s << INDENT << "}" << Qt::endl << Qt::endl;
                        }else if(childElement.localName()=="description"){
                            QDomNamedNodeMap attributes = childElement.attributes();
                            {
                                if(!description.isEmpty())
                                    TypesystemException::raise(QString("Duplicate description specification in line %1").arg(childElement.lineNumber()));
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
                                if(!attributes.count()){
                                    s << INDENT << "description: " << escapeQuote(description, 13) << Qt::endl;
                                }else{
                                    s << INDENT << "Description{" << Qt::endl;
                                    {
                                        Indentation indent(INDENT);
                                        s << INDENT << "text: " << escapeQuote(description, 6) << Qt::endl;
                                        checkQtVersion(s, attributes);
                                        if(attributes.count()){
                                            TypesystemException::raise(QString("Unexpected attribute '%2' of tag <%1> in line %3").arg(childElement.localName()).arg(attributes.item(0).localName()).arg(element.lineNumber()));
                                        }
                                    }
                                    s << INDENT << "}" << Qt::endl << Qt::endl;
                                }
                                description.clear();
                            }
                        }
                    }
                        break;
                    default: break;
                    }
                }
            }
            checkQtVersion(s, attributes);
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
                            s << INDENT << "RequiredLibrary{" << Qt::endl;
                            {
                                Indentation indent(INDENT);
                                QDomNamedNodeMap attributes = childElement.attributes();
                                {
                                    textArgument(s, attributes, "name");
                                    QDomNode node = attributes.removeNamedItem("mode");
                                    if(node.isAttr()){
                                        static const QMap<QString, QString> locationNames{
                                            {"mandatory", "RequiredLibrary.Mandatory"},
                                            {"optional", "RequiredLibrary.Optional"},
                                            {"provide-only", "RequiredLibrary.ProvideOnly"}
                                        };
                                        if(locationNames.contains(node.toAttr().value())){
                                            s << INDENT << "mode: " << locationNames[node.toAttr().value()] << Qt::endl;
                                        }else{
                                            bool ok = false;
                                            int value = node.toAttr().value().toInt(&ok);
                                            if(ok){
                                                s << INDENT << "location: " << value << Qt::endl;
                                            }else{
                                                TypesystemException::raise(QString("Unexpected value %4 of attribute '%2' of tag <%1> in line %3").arg(element.localName()).arg(node.localName()).arg(element.lineNumber()).arg(node.toAttr().value()));
                                            }
                                        }
                                    }
                                    textListArgument(s, attributes, "platforms");
                                    checkQtVersion(s, attributes);
                                    if(attributes.count()){
                                        TypesystemException::raise(QString("Unexpected attribute '%2' of tag <%1> in line %3").arg(childElement.localName()).arg(attributes.item(0).localName()).arg(element.lineNumber()));
                                    }
                                }
                            }
                            s << INDENT << "}" << Qt::endl;
                        }else if(childElement.localName()=="required-package"){
                            s << INDENT << "RequiredPackage{" << Qt::endl;
                            {
                                Indentation indent(INDENT);
                                QDomNamedNodeMap attributes = childElement.attributes();
                                {
                                    textArgument(s, attributes, "name");
                                    checkQtVersion(s, attributes);
                                }
                            }
                            s << INDENT << "}" << Qt::endl;
                        }else if(childElement.localName()=="forward-declaration"){
                            s << INDENT << "ForwardDeclaration{" << Qt::endl;
                            {
                                Indentation indent(INDENT);
                                QDomNamedNodeMap attributes = childElement.attributes();
                                {
                                    textArgument(s, attributes, "class", "className");
                                    checkQtVersion(s, attributes);
                                }
                            }
                            s << INDENT << "}" << Qt::endl;
                        }else if(childElement.localName()=="namespace-prefix"){
                            s << INDENT << "NamespacePrefix{" << Qt::endl;
                            {
                                Indentation indent(INDENT);
                                QDomNamedNodeMap attributes = childElement.attributes();
                                {
                                    textArgument(s, attributes, "prefix");
                                    textArgument(s, attributes, "namespace");
                                    QDomNode node = attributes.removeNamedItem("naming-policy");
                                    if(node.isAttr()){
                                        static const QMap<QString, QString> namingPolicies{
                                            {"cut", "NamespacePrefix.Cut"},
                                            {"preserve", "NamespacePrefix.Preserve"}
                                        };
                                        if(namingPolicies.contains(node.toAttr().value())){
                                            s << INDENT << "namingPolicy: " << namingPolicies[node.toAttr().value()] << Qt::endl;
                                        }else{
                                            bool ok = false;
                                            int value = node.toAttr().value().toInt(&ok);
                                            if(ok){
                                                s << INDENT << "namingPolicy: " << value << Qt::endl;
                                            }else{
                                                TypesystemException::raise(QString("Unexpected value %4 of attribute '%2' of tag <%1> in line %3").arg(element.localName()).arg(node.localName()).arg(element.lineNumber()).arg(node.toAttr().value()));
                                            }
                                        }
                                    }
                                    checkQtVersion(s, attributes);
                                }
                            }
                            s << INDENT << "}" << Qt::endl;
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
                        }else if(childElement.localName()=="load-typesystem"){
                            parseLoadTypeSystem(s, childElement);
                        }else if(childElement.localName()=="rejection"){
                            parseRejection(s, childElement);
                        }else if(childElement.localName()=="primitive-type"){
                            parsePrimitiveType(s, childElement);
                        }else if(childElement.localName()=="object-type"){
                            parseObjectType(s, childElement);
                        }else if(childElement.localName()=="template-type"){
                            parseTemplateType(s, childElement);
                        }else if(childElement.localName()=="functional-type"){
                            parseFunctionalType(s, childElement);
                        }else if(childElement.localName()=="iterator-type"){
                            parseIteratorType(s, childElement);
                        }else if(childElement.localName()=="value-type"){
                            parseValueType(s, childElement);
                        }else if(childElement.localName()=="typealias-type"){
                            parseTypeAliasType(s, childElement);
                        }else if(childElement.localName()=="interface-type"){
                            parseInterfaceType(s, childElement);
                        }else if(childElement.localName()=="namespace-type" || childElement.localName()=="header-type"){
                            parseNamespaceType(s, childElement);
                        }else if(childElement.localName()=="enum-type"){
                            parseEnumType(s, childElement);
                        }else if(childElement.localName()=="extra-includes"){
                            parseExtraIncludes(s, childElement);
                        }else if(childElement.localName()=="inject-code"){
                            parseInjectCode(s, childElement, false, true);
                        }else if(childElement.localName()=="suppress-warning"){
                            parseSuppressedWarning(s, childElement);
                            continue;
                        }else if(childElement.localName()=="template"){
                            parseTemplate(s, childElement);
                        }else if(childElement.localName()=="global-function"){
                            parseModifyFunction(s, childElement);
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
                    if(i<childNodes.length()-1){
                        s << INDENT << Qt::endl;
                    }
                }
            }
        }
    }catch(const TypesystemException& exn){
        TypesystemException::raise(QString("%1 in file %2").arg(QLatin1String(exn.what()), fileName));
    }
}

void XmlToQmlConverterPrivate::checkQtVersion(QTextStream& s, QDomNamedNodeMap& attributes){
    QDomNode node = attributes.removeNamedItem("since");
    if(node.isAttr()){
        QString value = node.toAttr().value();
        if(value.endsWith(".*"))
            value.chop(2);
        bool ok = false;
        value.toUInt(&ok);
        if(ok){
            s << INDENT << "since: " << value << Qt::endl;
        }else{
            QVersionNumber number = QVersionNumber::fromString(value);
            QVector<int> segments = number.segments();
            while(segments.endsWith(0)){
                segments.removeLast();
            }
            if(segments.size()==1){
                s << INDENT << "since: " << segments[0] << Qt::endl;
            }else{
                s << INDENT << "since: [";
                for(int i=0; i<segments.size(); ++i){
                    s << segments[i];
                    if(i<segments.size()-1)
                        s << ", ";
                }
                s << "]" << Qt::endl;
            }
        }
    }
    node = attributes.removeNamedItem("until");
    if(node.isAttr()){
        QString value = node.toAttr().value();
        if(value.endsWith(".*"))
            value.chop(2);
        bool ok = false;
        value.toUInt(&ok);
        if(ok){
            s << INDENT << "until: " << value << Qt::endl;
        }else{
            QVersionNumber number = QVersionNumber::fromString(value);
            QVector<int> segments = number.segments();
            if(segments.size()==1){
                s << INDENT << "until: " << segments[0] << Qt::endl;
            }else{
                s << INDENT << "until: [";
                for(int i=0; i<segments.size(); ++i){
                    s << segments[i];
                    if(i<segments.size()-1)
                        s << ", ";
                }
                s << "]" << Qt::endl;
            }
        }
    }
    textArgument(s, attributes, "unless");
}

QString XmlToQmlConverterPrivate::textArgument(QTextStream& s, QDomNamedNodeMap& attributes, const QString& argument, const QString& qmlArgumentName){
    QDomNode node = attributes.removeNamedItem(argument);
    QString result;
    if(node.isAttr())
        s << INDENT << (qmlArgumentName.isEmpty() ? argument : qmlArgumentName)
          << ": "
          << escapeQuote(result = node.toAttr().value(), ((qmlArgumentName.isEmpty() ? argument : qmlArgumentName)).size()+2)
          << Qt::endl;
    return result;
}

void XmlToQmlConverterPrivate::textListArgument(QTextStream& s, QDomNamedNodeMap& attributes, const QString& argument, const QString& qmlArgumentName){
    QDomNode node = attributes.removeNamedItem(argument);
    if(node.isAttr()){
        QStringList list = node.toAttr().value().split(",");
        for(int i=0, length=list.size(); i<length; ++i){
            list[i] = escapeQuote(list[i].trimmed(), -1);
        }
        s << INDENT << (qmlArgumentName.isEmpty() ? argument : qmlArgumentName) << ": [" << list.join(", ") << "]" << Qt::endl;
    }
}

void XmlToQmlConverterPrivate::variantArgument(QTextStream& s, QDomNamedNodeMap& attributes, const QString& argument, const QString& qmlArgumentName){
    QDomNode node = attributes.removeNamedItem(argument);
    if(node.isAttr()){
        if(node.toAttr().value().toLower()=="yes" || node.toAttr().value().toLower()=="true")
            s << INDENT << (qmlArgumentName.isEmpty() ? argument : qmlArgumentName) << ": true" << Qt::endl;
        else if(node.toAttr().value().toLower()=="no" || node.toAttr().value().toLower()=="false")
            s << INDENT << (qmlArgumentName.isEmpty() ? argument : qmlArgumentName) << ": false" << Qt::endl;
        else{
            bool ok = false;
            node.toAttr().value().toInt(&ok);
            if(!ok)
                node.toAttr().value().toDouble(&ok);
            if(ok)
                s << INDENT << (qmlArgumentName.isEmpty() ? argument : qmlArgumentName) << ": " << node.toAttr().value() << Qt::endl;
            else
                s << INDENT << (qmlArgumentName.isEmpty() ? argument : qmlArgumentName)
                  << ": "
                  << escapeQuote(node.toAttr().value(), ((qmlArgumentName.isEmpty() ? argument : qmlArgumentName)).size()+2)
                  << Qt::endl;
        }
    }
}

void XmlToQmlConverterPrivate::numberArgument(QTextStream& s, QDomNamedNodeMap& attributes, const QString& argument, const QString& qmlArgumentName){
    QDomNode node = attributes.removeNamedItem(argument);
    if(node.isAttr())
        s << INDENT << (qmlArgumentName.isEmpty() ? argument : qmlArgumentName) << ": " << node.toAttr().value() << Qt::endl;
}

void XmlToQmlConverterPrivate::boolArgument(QTextStream& s, QDomNamedNodeMap& attributes, const QString& argument, const QString& qmlArgumentName){
    QDomNode node = attributes.removeNamedItem(argument);
    if(node.isAttr())
        s << INDENT << (qmlArgumentName.isEmpty() ? argument : qmlArgumentName) << ": " << convertBoolean(node.toAttr().value(), argument, false) << Qt::endl;
}

QString XmlToQmlConverterPrivate::convertBoolean(const QString &_value, const QString &attributeName, bool defaultValue) {
    QString value = _value.toLower();
    if (value == "true" || value == "yes") {
        return "true";
    } else if (value == "false" || value == "no") {
        return "false";
    } else {
        QString warn = QString("Boolean value '%1' not supported in attribute '%2'. Use 'yes' or 'no'. Defaulting to '%3'.")
                       .arg(value, attributeName, defaultValue ? "yes" : "no");

        ReportHandler::warning(warn);
        return defaultValue ? "true" : "false";
    }
}

QString XmlToQmlConverterPrivate::attributeValue(const QDomNode& domNode, const QString & defaultValue){
    if(domNode.isAttr()){
        return domNode.toAttr().value();
    }else{
        return defaultValue;
    }
}

void XmlToQmlConverterPrivate::parseSuppressedWarning(QTextStream& s, const QDomElement &element){
    QDomNamedNodeMap attributes = element.attributes();
    if(attributes.count()==1 && attributes.item(0).nodeName()=="text"){
        s << INDENT << "SuppressedWarning{text: ";
        QDomNode node = attributes.item(0);
        s << escapeQuote(node.toAttr().value(), 24) << "}" << Qt::endl;
    }else{
        s << INDENT << "SuppressedWarning{" << Qt::endl;
        {
            Indentation indent(INDENT);
            textArgument(s, attributes, "text");
            checkQtVersion(s, attributes);
        }
        s << INDENT << "}" << Qt::endl;
    }
}

void XmlToQmlConverterPrivate::parseInclude(QTextStream& s, const QDomElement &element){
    s << INDENT << "Include{" << Qt::endl;
    {
        Indentation indent(INDENT);
        QDomNamedNodeMap attributes = element.attributes();
        textArgument(s, attributes, "file-name", "fileName");
        textArgument(s, attributes, "pp-condition", "ppCondition");
        textListArgument(s, attributes, "required-features", "requiredFeatures");
        boolArgument(s, attributes, "inherited");
        boolArgument(s, attributes, "suppressed");
        QDomNode node = attributes.removeNamedItem("location");
        if(node.isAttr()){
            static const QMap<QString, QString> locationNames{
                {"global", "Include.Global"},
                {"local", "Include.Local"},
                {"java", "Include.Java"}
            };
            if(locationNames.contains(node.toAttr().value())){
                s << INDENT << "location: " << locationNames[node.toAttr().value()] << Qt::endl;
            }else{
                bool ok = false;
                int value = node.toAttr().value().toInt(&ok);
                if(ok){
                    s << INDENT << "location: " << value << Qt::endl;
                }else{
                    TypesystemException::raise(QString("Unexpected value %4 of attribute '%2' of tag <%1> in line %3").arg(element.localName()).arg(node.localName()).arg(element.lineNumber()).arg(node.toAttr().value()));
                }
            }
        }
        checkQtVersion(s, attributes);
    }
    s << INDENT << "}" << Qt::endl;
}

void XmlToQmlConverterPrivate::parseExtraIncludes(QTextStream& s, const QDomElement &element){
    s << INDENT << "ExtraIncludes{" << Qt::endl;
    {
        Indentation indent(INDENT);
        QDomNamedNodeMap attributes = element.attributes();
        {
            QDomNodeList childNodes = element.childNodes();
            for(int i=0; i<childNodes.length(); ++i){
                QDomNode item = childNodes.item(i);
                switch(item.nodeType()){
                case QDomNode::ElementNode:
                {
                    QDomElement childElement = item.toElement();
                    if(childElement.localName()=="include"){
                        parseInclude(s, childElement);
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
        checkQtVersion(s, attributes);
    }
    s << INDENT << "}" << Qt::endl;
}

void XmlToQmlConverterPrivate::parseTemplateInstantiation(QTextStream& s, const QDomElement &element){
    s << INDENT << "TemplateArguments{" << Qt::endl;
    {
        Indentation indent(INDENT);
        QDomNamedNodeMap attributes = element.attributes();
        {
            QString arguments = attributeValue(attributes.removeNamedItem("arguments"));
            QStringList inst;
            if(!arguments.isEmpty()){
                inst = arguments.split(",");
                for(QString& s : inst){
                    s = escapeQuote(s, -1);
                }
            }else{
                QDomNodeList childNodes = element.childNodes();
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
                                    inst << escapeQuote(item.toText().data().trimmed(), -1);
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
            }
            checkQtVersion(s, attributes);
            if(attributes.count()){
                TypesystemException::raise(QString("Unexpected attribute '%2' of tag <%1> in line %3").arg(element.localName()).arg(attributes.item(0).localName()).arg(element.lineNumber()));
            }
            s << INDENT << "arguments: [" << inst.join(", ") << "]" << Qt::endl;
        }
    }
    s << INDENT << "}" << Qt::endl;
}

void XmlToQmlConverterPrivate::parseTemplate(QTextStream& s, const QDomElement &element){
    s << INDENT << "Template{" << Qt::endl;
    {
        Indentation indent(INDENT);
        QDomNamedNodeMap attributes = element.attributes();
        textArgument(s, attributes, "name");
        checkQtVersion(s, attributes);
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
                    parseInsertTemplate(s, childElement);
                }else if(childElement.localName()=="import-file"){
                    parseImportFile(s, childElement);
                }else{
                    TypesystemException::raise(QString("Unexpected tag <%2> as child of tag <%1> in line %3").arg(element.localName()).arg(childElement.localName()).arg(childElement.lineNumber()));
                }
            }
                break;
            case QDomNode::TextNode:
                s << INDENT << "Text{content: " << escapeQuote(item.toText().data(), 14) << "}" << Qt::endl;
                break;
            case QDomNode::CharacterDataNode:
                s << INDENT << "Text{content: " << escapeQuote(item.toCharacterData().data(), 14) << "}" << Qt::endl;
                break;
            case QDomNode::CDATASectionNode:
                s << INDENT << "Text{content: " << escapeQuote(item.toCDATASection().data(), 14) << "}" << Qt::endl;
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
    s << INDENT << "}" << Qt::endl;
}

void XmlToQmlConverterPrivate::parseInjectCode(QTextStream& s, const QDomElement &element, bool argumentMapAllowed, bool allowPackage){
    s << INDENT << "InjectCode{" << Qt::endl;
    {
        Indentation indent(INDENT);
        QDomNamedNodeMap attributes = element.attributes();
        {
            if(allowPackage)
                textArgument(s, attributes, "package", "packageName");
            QDomNode node = attributes.removeNamedItem("class");
            if(node.isAttr()){
                static const QMap<QString, QString> languageNames{
                    {"java", "CodeClass.Java"},
                    {"interface", "CodeClass.JavaInterface"},
                    {"native", "CodeClass.Native"},
                    {"shell", "CodeClass.Shell"},
                    {"shell-declaration", "CodeClass.ShellDeclaration"},
                    {"library-initializer", "CodeClass.PackageInitializer"},
                    {"destructor-function", "CodeClass.DestructorFunction"},
                    {"java-interface-default", "CodeClass.JavaInterface"},
                    {"metainfo", "CodeClass.MetaInfo"},
                    {"constructors", "CodeClass.Constructors"},
                    {"signal", "CodeClass.Signal"},
                    {"module-info", "CodeClass.ModuleInfo"},
                    {"getter", "CodeClass.Getter"},
                    {"setter", "CodeClass.Setter"}
                };
                if(languageNames.contains(node.toAttr().value())){
                    s << INDENT << "target: " << languageNames[node.toAttr().value()] << Qt::endl;
                }else{
                    bool ok = false;
                    int value = node.toAttr().value().toInt(&ok);
                    if(ok){
                        s << INDENT << "target: " << value << Qt::endl;
                    }else{
                        TypesystemException::raise(QString("Unexpected value %4 of attribute '%2' of tag <%1> in line %3").arg(element.localName()).arg(node.localName()).arg(element.lineNumber()).arg(node.toAttr().value()));
                    }
                }
            }
            node = attributes.removeNamedItem("position");
            if(node.isAttr()){
                static const QMap<QString, QString> positions{
                    {"begin", "Position.Beginning"},
                    {"beginning", "Position.Beginning"},
                    {"position1", "Position.Position1"},
                    {"position2", "Position.Position2"},
                    {"position3", "Position.Position3"},
                    {"position4", "Position.Position4"},
                    {"position5", "Position.Position5"},
                    {"1", "Position.Position1"},
                    {"2", "Position.Position2"},
                    {"3", "Position.Position3"},
                    {"4", "Position.Position4"},
                    {"5", "Position.Position5"},
                    {"equals", "Position.Equals"},
                    {"compare", "Position.Compare"},
                    {"hashcode", "Position.HashCode"},
                    {"tostring", "Position.ToString"},
                    {"end", "Position.End"}
                };
                if(positions.contains(node.toAttr().value())){
                    s << INDENT << "position: " << positions[node.toAttr().value()] << Qt::endl;
                }else{
                    bool ok = false;
                    int value = node.toAttr().value().toInt(&ok);
                    if(ok){
                        s << INDENT << "position: " << value << Qt::endl;
                    }else{
                        TypesystemException::raise(QString("Unexpected value %4 of attribute '%2' of tag <%1> in line %3").arg(element.localName()).arg(node.localName()).arg(element.lineNumber()).arg(node.toAttr().value()));
                    }
                }
            }
            checkQtVersion(s, attributes);
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
                    if(argumentMapAllowed && childElement.localName()=="argument-map"){
                        s << INDENT << "ArgumentMap{" << Qt::endl;
                        {
                            Indentation indent(INDENT);
                            QDomNamedNodeMap attributes = childElement.attributes();
                            {
                                numberArgument(s, attributes, "index");
                                textArgument(s, attributes, "meta-name", "metaName");
                                checkQtVersion(s, attributes);
                            }
                        }
                        s << INDENT << "}" << Qt::endl;
                    }else if(childElement.localName()=="insert-template"){
                        parseInsertTemplate(s, childElement);
                    }else if(childElement.localName()=="import-file"){
                        parseImportFile(s, childElement);
                    }else{
                        TypesystemException::raise(QString("Unexpected tag <%2> as child of tag <%1> in line %3").arg(element.localName()).arg(childElement.localName()).arg(childElement.lineNumber()));
                    }
                }
                    break;
                case QDomNode::TextNode:
                    s << INDENT << "Text{content: " << escapeQuote(item.toText().data(), 14) << "}" << Qt::endl;
                    break;
                case QDomNode::CharacterDataNode:
                    s << INDENT << "Text{content: " << escapeQuote(item.toCharacterData().data(), 14) << "}" << Qt::endl;
                    break;
                case QDomNode::CDATASectionNode:
                    s << INDENT << "Text{content: " << escapeQuote(item.toCDATASection().data(), 14) << "}" << Qt::endl;
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
    s << INDENT << "}" << Qt::endl;
}

void XmlToQmlConverterPrivate::parseInsertTemplate(QTextStream& s, const QDomElement &element){
    s << INDENT << "InsertTemplate{" << Qt::endl;
    {
        Indentation indent(INDENT);
        QDomNamedNodeMap attributes = element.attributes();
        {
            textArgument(s, attributes, "name");
            checkQtVersion(s, attributes);
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
                    if(childElement.localName()=="replace"){
                        s << INDENT << "Replace{" << Qt::endl;
                        {
                            Indentation indent(INDENT);
                            QDomNamedNodeMap attributes = childElement.attributes();
                            {
                                textArgument(s, attributes, "from");
                                textArgument(s, attributes, "to");
                                checkQtVersion(s, attributes);
                                if(attributes.count()){
                                    TypesystemException::raise(QString("Unexpected attribute of tag <%1>: %2").arg(childElement.localName()).arg(attributes.item(0).localName()));
                                }
                            }
                        }
                        s << INDENT << "}" << Qt::endl;
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
    s << INDENT << "}" << Qt::endl;
}

void XmlToQmlConverterPrivate::parseImportFile(QTextStream& s, const QDomElement &element){
    s << INDENT << "ImportFile{" << Qt::endl;
    {
        Indentation indent(INDENT);
        QDomNamedNodeMap attributes = element.attributes();
        {
            QString fileName = textArgument(s, attributes, "name");
            if (fileName.isEmpty()) {
                TypesystemException::raise(QString("Required attribute 'name' missing for tag <%1>: line %2").arg(element.localName()).arg(element.lineNumber()));
            }
            textArgument(s, attributes, "quote-after-line", "quoteAfterLine");
            textArgument(s, attributes, "quote-before-line", "quoteBeforeLine");
            checkQtVersion(s, attributes);
        }
    }
    s << INDENT << "}" << Qt::endl;
}

void XmlToQmlConverterPrivate::parseLoadTypeSystem(QTextStream& s, const QDomElement &element){
    s << INDENT << "LoadTypeSystem{" << Qt::endl;
    {
        Indentation indent(INDENT);
        QDomNamedNodeMap attributes = element.attributes();
        {
            QString name = textArgument(s, attributes, "name");
            if (name.isEmpty()) {
                TypesystemException::raise(QString("No typesystem name specified"));
            }
            boolArgument(s, attributes, "generate");
            boolArgument(s, attributes, "optional");
            checkQtVersion(s, attributes);
            if(attributes.count()){
                TypesystemException::raise(QString("Unexpected attribute '%2' of tag <%1> in line %3").arg(element.localName()).arg(attributes.item(0).localName()).arg(element.lineNumber()));
            }
            ensureNoChildren(element);
            m_database->parseFile("typesystem_"+name+".xml", m_importInputDirectoryList, m_typesystemDirectoryList, true, false);
        }
    }
    s << INDENT << "}" << Qt::endl;
}

void XmlToQmlConverterPrivate::parseRejection(QTextStream& s, const QDomElement &element){
    s << INDENT << "Rejection{" << Qt::endl;
    {
        Indentation indent(INDENT);
        QDomNamedNodeMap attributes = element.attributes();
        {
            textArgument(s, attributes, "class", "className");
            textArgument(s, attributes, "function-name", "functionName");
            textArgument(s, attributes, "field-name", "fieldName");
            textArgument(s, attributes, "enum-name", "enumName");
            checkQtVersion(s, attributes);
            if(attributes.count()){
                TypesystemException::raise(QString("Unexpected attribute '%2' of tag <%1> in line %3").arg(element.localName()).arg(attributes.item(0).localName()).arg(element.lineNumber()));
            }
            ensureNoChildren(element);
        }
    }
    s << INDENT << "}" << Qt::endl;
}

void XmlToQmlConverterPrivate::parsePrimitiveType(QTextStream& s, const QDomElement &element){
    s << INDENT << "PrimitiveType{" << Qt::endl;
    {
        Indentation indent(INDENT);
        QDomNamedNodeMap attributes = element.attributes();
        {
            textArgument(s, attributes, "name");
            textArgument(s, attributes, "java-name", "javaName");
            textArgument(s, attributes, "jni-name", "jniName");
            boolArgument(s, attributes, "preferred-conversion", "preferredConversion");
            boolArgument(s, attributes, "preferred-java-type", "preferredJavaType");
            checkQtVersion(s, attributes);
            if(attributes.count()){
                TypesystemException::raise(QString("Unexpected attribute '%2' of tag <%1> in line %3").arg(element.localName()).arg(attributes.item(0).localName()).arg(element.lineNumber()));
            }
            ensureNoChildren(element);
        }
    }
    s << INDENT << "}" << Qt::endl;
}

void XmlToQmlConverterPrivate::parseAttributesOfComplexType(QTextStream& s, const QDomElement &, QDomNamedNodeMap& attributes){
    textArgument(s, attributes, "package", "packageName");
    textArgument(s, attributes, "implements", "implementing");
    textArgument(s, attributes, "using");
    textArgument(s, attributes, "java-name", "javaName");
    textArgument(s, attributes, "default-superclass", "defaultSuperClass");
    boolArgument(s, attributes, "generic-class", "isGeneric");
    textArgument(s, attributes, "pp-condition", "ppCondition");
    textArgument(s, attributes, "target-type", "targetType");
    boolArgument(s, attributes, "disable-native-id-usage", "disableNativeIdUsage");
    boolArgument(s, attributes, "force-abstract", "forceAbstract");
    boolArgument(s, attributes, "force-friendly", "forceFriendly");
    boolArgument(s, attributes, "deprecated");
    textArgument(s, attributes, "thread-affine", "threadAffinity");
    boolArgument(s, attributes, "is-native-interface", "isNativeInterface");
    boolArgument(s, attributes, "template");
    variantArgument(s, attributes, "generate");
    textArgument(s, attributes, "extend-type", "extendType");
    attributes.removeNamedItem("expense-limit");
    attributes.removeNamedItem("expense-cost");
}

void XmlToQmlConverterPrivate::parseObjectType(QTextStream& s, const QDomElement &element){
    s << INDENT << "ObjectType{" << Qt::endl;
    {
        Indentation indent(INDENT);
        QDomNamedNodeMap attributes = element.attributes();
        {
            QString name = textArgument(s, attributes, "name");
            boolArgument(s, attributes, "is-value-owner", "isValueOwner");
            boolArgument(s, attributes, "polymorphic-base", "isPolymorphicBase");
            textArgument(s, attributes, "polymorphic-id-expression", "polymorphicIdExpression");
            parseAttributesOfComplexType(s, element, attributes);
            QList<QDomElement> unhandledElements = parseChildrenOfComplexType(s, element);
            for(const QDomElement& childElement : unhandledElements){
                TypesystemException::raise(QString("Unexpected tag <%2> as child of tag <%1> in line %3").arg(element.localName()).arg(childElement.toElement().localName()).arg(childElement.lineNumber()));
            }
            checkQtVersion(s, attributes);
            if(attributes.count()){
                TypesystemException::raise(QString("Unexpected attribute '%2' of tag <%1> in line %3").arg(element.localName()).arg(attributes.item(0).localName()).arg(element.lineNumber()));
            }
        }
    }
    s << INDENT << "}" << Qt::endl;
}

QList<QDomElement> XmlToQmlConverterPrivate::parseChildrenOfComplexType(QTextStream& s, const QDomElement &element){
    QList<QDomElement> unhandledElements;
    QDomNodeList childNodes = element.childNodes();
    for(int i=0; i<childNodes.length(); ++i){
        QDomNode item = childNodes.item(i);
        switch(item.nodeType()){
        case QDomNode::ElementNode:
        {
            QDomElement childElement = item.toElement();
            if(childElement.localName()=="template"){
                parseTemplate(s, childElement);
            }else if(childElement.localName()=="instantiation"){
                parseTemplateInstantiation(s, childElement);
            }else if(childElement.localName()=="modify-function"){
                parseModifyFunction(s, childElement);
            }else if(childElement.localName()=="modify-field"){
                parseModifyField(s, childElement);
            }else if(childElement.localName()=="inject-code"){
                parseInjectCode(s, childElement);
            }else if(childElement.localName()=="delegate-base-class"){
                s << INDENT << "DelegateBaseClass{" << Qt::endl;
                {
                    Indentation indent(INDENT);
                    QDomNamedNodeMap attributes = childElement.attributes();
                    {
                        textArgument(s, attributes, "base-class", "baseClass");
                        textArgument(s, attributes, "delegate");
                        checkQtVersion(s, attributes);
                        if(attributes.count()){
                            TypesystemException::raise(QString("Unexpected attribute '%2' of tag <%1> in line %3").arg(childElement.localName()).arg(attributes.item(0).localName()).arg(childElement.lineNumber()));
                        }
                    }
                }
                s << INDENT << "}" << Qt::endl;
            }else if(childElement.localName()=="implements"){
                s << INDENT << "Implements{" << Qt::endl;
                {
                    Indentation indent(INDENT);
                    QDomNamedNodeMap attributes = childElement.attributes();
                    {
                        s << INDENT << "interface: \"" << childElement.text() << "\"" << Qt::endl;
                        checkQtVersion(s, attributes);
                    }
                }
                s << INDENT << "}" << Qt::endl;
            }else if(childElement.localName()=="include"){
                parseInclude(s, childElement);
            }else if(childElement.localName()=="extra-includes"){
                parseExtraIncludes(s, childElement);
            }else if(childElement.localName()=="import"){
                s << INDENT << "Import{" << Qt::endl;
                {
                    Indentation indent(INDENT);
                    QDomNamedNodeMap attributes = childElement.attributes();
                    {
                        textArgument(s, attributes, "template");
                        checkQtVersion(s, attributes);
                    }
                }
                s << INDENT << "}" << Qt::endl;
            }else if(childElement.localName()=="interface-polymorphy"){
                s << INDENT << "InterfacePolymorphy{" << Qt::endl;
                {
                    Indentation indent(INDENT);
                    QDomNamedNodeMap attributes = childElement.attributes();
                    {
                        textArgument(s, attributes, "interface", "interfaceType");
                        textArgument(s, attributes, "condition");
                        checkQtVersion(s, attributes);
                    }
                }
                s << INDENT << "}" << Qt::endl;
            }else if(childElement.localName()=="custom-constructor"){
                s << INDENT << "CustomConstructor{" << Qt::endl;
                {
                    Indentation indent(INDENT);
                    QDomNamedNodeMap attributes = childElement.attributes();
                    {
                        textArgument(s, attributes, "name");
                        textArgument(s, attributes, "param-name", "paramName");
                        textArgument(s, attributes, "placement-name", "placementName");
                        QDomNode node = attributes.removeNamedItem("type");
                        if(node.isAttr()){
                            static const QMap<QString, QString> types{
                                {"default", "CustomConstructor.Default"},
                                {"move", "CustomConstructor.Move"},
                                {"copy", "CustomConstructor.Copy"},
                                {"legacy", "CustomConstructor.Legacy"},
                                {"", "CustomConstructor.Legacy"}
                            };
                            if(types.contains(node.toAttr().value())){
                                s << INDENT << "type: " << types[node.toAttr().value()] << Qt::endl;
                            }else{
                                bool ok = false;
                                int value = node.toAttr().value().toInt(&ok);
                                if(ok){
                                    s << INDENT << "type: " << value << Qt::endl;
                                }else{
                                    TypesystemException::raise(QString("Unexpected value %4 of attribute '%2' of tag <%1> in line %3").arg(element.localName()).arg(node.localName()).arg(element.lineNumber()).arg(node.toAttr().value()));
                                }
                            }
                        }
                        checkQtVersion(s, attributes);
                        if(attributes.count()){
                            TypesystemException::raise(QString("Unexpected attribute '%2' of tag <%1> in line %3").arg(childElement.localName()).arg(attributes.item(0).localName()).arg(childElement.lineNumber()));
                        }
                        parseCustomStructor(s, childElement);
                    }
                }
                s << INDENT << "}" << Qt::endl;
            }else if(childElement.localName()=="custom-destructor"){
                s << INDENT << "CustomDestructor{" << Qt::endl;
                {
                    Indentation indent(INDENT);
                    QDomNamedNodeMap attributes = childElement.attributes();
                    {
                        textArgument(s, attributes, "name");
                        textArgument(s, attributes, "param-name", "paramName");
                        checkQtVersion(s, attributes);
                        parseCustomStructor(s, childElement);
                    }
                }
                s << INDENT << "}" << Qt::endl;
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

void XmlToQmlConverterPrivate::parseCustomStructor(QTextStream& s, const QDomElement &element){
    QDomNodeList childNodes = element.childNodes();
    for(int i=0; i<childNodes.length(); ++i){
        QDomNode item = childNodes.item(i);
        switch(item.nodeType()){
        case QDomNode::ElementNode:
        {
            QDomElement childElement = item.toElement();
            if(childElement.localName()=="insert-template"){
                parseInsertTemplate(s, childElement);
            }else if(childElement.localName()=="import-file"){
                parseImportFile(s, childElement);
            }else{
                TypesystemException::raise(QString("Unexpected tag <%2> as child of tag <%1> in line %3").arg(element.localName()).arg(childElement.localName()).arg(childElement.lineNumber()));
            }
        }
            break;
        case QDomNode::TextNode:
            s << INDENT << "Text{content: " << escapeQuote(item.toText().data(), 14) << "}" << Qt::endl;
            break;
        case QDomNode::CharacterDataNode:
            s << INDENT << "Text{content: " << escapeQuote(item.toCharacterData().data(), 14) << "}" << Qt::endl;
            break;
        case QDomNode::CDATASectionNode:
            s << INDENT << "Text{content: " << escapeQuote(item.toCDATASection().data(), 14) << "}" << Qt::endl;
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

void XmlToQmlConverterPrivate::parseTemplateType(QTextStream& s, const QDomElement &element){
    s << INDENT << "TemplateType{" << Qt::endl;
    {
        Indentation indent(INDENT);
        QDomNamedNodeMap attributes = element.attributes();
        {
            textArgument(s, attributes, "name");
            checkQtVersion(s, attributes);
            if(attributes.count()){
                TypesystemException::raise(QString("Unexpected attribute '%2' of tag <%1> in line %3").arg(element.localName()).arg(attributes.item(0).localName()).arg(element.lineNumber()));
            }
            QList<QDomElement> unhandledElements = parseChildrenOfComplexType(s, element);
            for(const QDomElement& childElement : unhandledElements){
                TypesystemException::raise(QString("Unexpected tag <%2> as child of tag <%1> in line %3").arg(element.localName()).arg(childElement.toElement().localName()).arg(childElement.lineNumber()));
            }
        }
    }
    s << INDENT << "}" << Qt::endl;
}

void XmlToQmlConverterPrivate::parseModifyArgument(QTextStream& s, const QDomElement &element){
    s << INDENT << "ModifyArgument{" << Qt::endl;
    {
        Indentation indent(INDENT);
        QDomNamedNodeMap attributes = element.attributes();
        {
            variantArgument(s, attributes, "index");
            textArgument(s, attributes, "replace-value", "replaceValue");
            QDomNode node = attributes.removeNamedItem("thread-affine");
            if(node.isAttr()){
                static const QMap<QString, QString> affinities{
                    {"ui", "Affinity.UI"},
                    {"pixmap", "Affinity.Pixmap"},
                    {"yes", "true"},
                    {"true", "true"},
                    {"no", "false"},
                    {"false", "false"}
                };
                if(affinities.contains(node.toAttr().value().toLower())){
                    s << INDENT << "threadAffinity: " << affinities[node.toAttr().value().toLower()] << Qt::endl;
                }else{
                    s << INDENT << "threadAffinity: " << escapeQuote(node.toAttr().value(), 16) << Qt::endl;
                }
            }
            textArgument(s, attributes, "rename");
            textArgument(s, attributes, "replace-type", "replaceType");
            boolArgument(s, attributes, "invalidate-after-use", "invalidateAfterUse");
            boolArgument(s, attributes, "value-as-pointer", "valueAsPointer");
            checkQtVersion(s, attributes);
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
                        s << INDENT << "RemoveDefaultExpression{" << Qt::endl;
                        {
                            Indentation indent(INDENT);
                            QDomNamedNodeMap attributes = childElement.attributes();
                            checkQtVersion(s, attributes);
                        }
                        s << INDENT << "}" << Qt::endl;
                    }else if(childElement.localName()=="replace-default-expression"){
                        s << INDENT << "ReplaceDefaultExpression{" << Qt::endl;
                        {
                            Indentation indent(INDENT);
                            QDomNamedNodeMap attributes = childElement.attributes();
                            textArgument(s, attributes, "with", "expression");
                            checkQtVersion(s, attributes);
                        }
                        s << INDENT << "}" << Qt::endl;
                    }else if(childElement.localName()=="reference-count"){
                        s << INDENT << "ReferenceCount{" << Qt::endl;
                        {
                            Indentation indent(INDENT);
                            QDomNamedNodeMap attributes = childElement.attributes();
                            {
                                boolArgument(s, attributes, "thread-safe", "isThreadSafe");
                                textArgument(s, attributes, "variable-name", "variableName");
                                numberArgument(s, attributes, "key-argument", "keyArgument");
                                textArgument(s, attributes, "declare-variable", "declareVariable");
                                textArgument(s, attributes, "condition");
                                QDomNode node = attributes.removeNamedItem("action");
                                if(node.isAttr()){
                                    static const QMap<QString, QString> actions{
                                        {"put", "ReferenceCount.Put"},
                                        {"add", "ReferenceCount.Add"},
                                        {"add-all", "ReferenceCount.AddAll"},
                                        {"clear-add", "ReferenceCount.ClearAdd"},
                                        {"clear-add-all", "ReferenceCount.ClearAddAll"},
                                        {"remove", "ReferenceCount.Take"},
                                        {"set", "ReferenceCount.Set"},
                                        {"ignore", "ReferenceCount.Ignore"}
                                    };
                                    if(actions.contains(node.toAttr().value())){
                                        s << INDENT << "action: " << actions[node.toAttr().value()] << Qt::endl;
                                    }else{
                                        bool ok = false;
                                        int value = node.toAttr().value().toInt(&ok);
                                        if(ok){
                                            s << INDENT << "action: " << value << Qt::endl;
                                        }else{
                                            TypesystemException::raise(QString("Unexpected value %4 of attribute '%2' of tag <%1> in line %3").arg(element.localName()).arg(node.localName()).arg(element.lineNumber()).arg(node.toAttr().value()));
                                        }
                                    }
                                }
                                node = attributes.removeNamedItem("access");
                                if(node.isAttr()){
                                    static const QMap<QString, QString> accesses{
                                        {"private", "Modification.Private"},
                                        {"public", "Modification.Public"},
                                        {"protected", "Modification.Protected"},
                                        {"friendly", "Modification.Friendly"},
                                        {"final", "Modification.Final"},
                                        {"declared-final", "Modification.DeclFinal"},
                                        {"non-final", "Modification.NonFinal"}
                                    };
                                    QStringList accs = node.toAttr().value().split(",");
                                    for(QString& a : accs){
                                        a = a.trimmed();
                                        if(accesses.contains(a)){
                                            a = accesses[a];
                                        }else{
                                            bool ok = false;
                                            a.toInt(&ok);
                                            if(!ok){
                                                TypesystemException::raise(QString("Unexpected value %4 of attribute '%2' of tag <%1> in line %3").arg(element.localName()).arg(node.localName()).arg(element.lineNumber()).arg(node.toAttr().value()));
                                            }
                                        }
                                    }
                                    s << INDENT << "access: " << accs.join(" | ") << Qt::endl;
                                }
                                checkQtVersion(s, attributes);
                                if(attributes.count()){
                                    TypesystemException::raise(QString("Unexpected attribute '%2' of tag <%1> in line %3").arg(childElement.localName()).arg(attributes.item(0).localName()).arg(childElement.lineNumber()));
                                }
                                ensureNoChildren(childElement);
                            }
                        }
                        s << INDENT << "}" << Qt::endl;
                    }else if(childElement.localName()=="array-type"){
                        s << INDENT << "ArrayType{" << Qt::endl;
                        {
                            Indentation indent(INDENT);
                            QDomNamedNodeMap attributes = childElement.attributes();
                            {
                                boolArgument(s, attributes, "deref");
                                boolArgument(s, attributes, "varargs");
                                boolArgument(s, attributes, "as-buffer", "asBuffer");
                                numberArgument(s, attributes, "length-parameter", "lengthParameter");
                                numberArgument(s, attributes, "min-length", "minLength");
                                numberArgument(s, attributes, "max-length", "maxLength");
                                checkQtVersion(s, attributes);
                                if(attributes.count()){
                                    TypesystemException::raise(QString("Unexpected attribute '%2' of tag <%1> in line %3").arg(childElement.localName()).arg(attributes.item(0).localName()).arg(childElement.lineNumber()));
                                }
                                ensureNoChildren(childElement);
                            }
                        }
                        s << INDENT << "}" << Qt::endl;
                    }else if(childElement.localName()=="replace-type"){
                        s << INDENT << "ReplaceType{" << Qt::endl;
                        {
                            Indentation indent(INDENT);
                            QDomNamedNodeMap attributes = childElement.attributes();
                            {
                                textArgument(s, attributes, "modified-type", "modifiedType");
                                textArgument(s, attributes, "modified-jni-type", "modifiedJniType");
                                checkQtVersion(s, attributes);
                                if(attributes.count()){
                                    TypesystemException::raise(QString("Unexpected attribute '%2' of tag <%1> in line %3").arg(childElement.localName()).arg(attributes.item(0).localName()).arg(childElement.lineNumber()));
                                }
                                ensureNoChildren(childElement);
                            }
                        }
                        s << INDENT << "}" << Qt::endl;
                    }else if(childElement.localName()=="conversion-rule"){
                        parseConversionRule(s, childElement);
                    }else if(childElement.localName()=="no-null-pointer"){
                        s << INDENT << "NoNullPointer{" << Qt::endl;
                        {
                            Indentation indent(INDENT);
                            QDomNamedNodeMap attributes = childElement.attributes();
                            {
                                textArgument(s, attributes, "default-value", "defaultValue");
                                checkQtVersion(s, attributes);
                                if(attributes.count()){
                                    TypesystemException::raise(QString("Unexpected attribute '%2' of tag <%1> in line %3").arg(childElement.localName()).arg(attributes.item(0).localName()).arg(childElement.lineNumber()));
                                }
                                ensureNoChildren(childElement);
                            }
                        }
                        s << INDENT << "}" << Qt::endl;
                    }else if(childElement.localName()=="define-ownership"){
                        s << INDENT << "DefineOwnership{" << Qt::endl;
                        {
                            Indentation indent(INDENT);
                            QDomNamedNodeMap attributes = childElement.attributes();
                            {
                                QDomNode node = attributes.removeNamedItem("class");
                                if(node.isAttr()){
                                    static const QMap<QString, QString> languageNames{
                                        {"java", "CodeClass.Java"},
                                        {"native", "CodeClass.Native"},
                                        {"shell", "CodeClass.Shell"}
                                    };
                                    if(languageNames.contains(node.toAttr().value())){
                                        s << INDENT << "codeClass: " << languageNames[node.toAttr().value()] << Qt::endl;
                                    }else{
                                        bool ok = false;
                                        int value = node.toAttr().value().toInt(&ok);
                                        if(ok){
                                            s << INDENT << "codeClass: " << value << Qt::endl;
                                        }else{
                                            TypesystemException::raise(QString("Unexpected value %4 of attribute '%2' of tag <%1> in line %3").arg(element.localName()).arg(node.localName()).arg(element.lineNumber()).arg(node.toAttr().value()));
                                        }
                                    }
                                }
                                node = attributes.removeNamedItem("owner");
                                if(node.isAttr()){
                                    static const QMap<QString, QString> ownershipNames{
                                        {"java", "Ownership.Java"},
                                        {"cplusplus", "Ownership.Cpp"},
                                        {"cpp", "Ownership.Cpp"},
                                        {"default", "Ownership.Default"},
                                        {"ignore", "Ownership.Ignore"},
                                        {"invalidate", "Ownership.Invalidate"}
                                    };
                                    if(ownershipNames.contains(node.toAttr().value())){
                                        s << INDENT << "ownership: " << ownershipNames[node.toAttr().value()] << Qt::endl;
                                    }else{
                                        bool ok = false;
                                        int value = node.toAttr().value().toInt(&ok);
                                        if(ok){
                                            s << INDENT << "ownership: " << value << Qt::endl;
                                        }else{
                                            TypesystemException::raise(QString("Unexpected value %4 of attribute '%2' of tag <%1> in line %3").arg(element.localName()).arg(node.localName()).arg(element.lineNumber()).arg(node.toAttr().value()));
                                        }
                                    }
                                }
                                textArgument(s, attributes, "condition");
                                checkQtVersion(s, attributes);
                                if(attributes.count()){
                                    TypesystemException::raise(QString("Unexpected attribute '%2' of tag <%1> in line %3").arg(childElement.localName()).arg(attributes.item(0).localName()).arg(childElement.lineNumber()));
                                }
                                ensureNoChildren(childElement);
                            }
                        }
                        s << INDENT << "}" << Qt::endl;
                    }else if(childElement.localName()=="remove-argument"){
                        s << INDENT << "RemoveArgument{" << Qt::endl;
                        {
                            Indentation indent(INDENT);
                            QDomNamedNodeMap attributes = childElement.attributes();
                            {
                                checkQtVersion(s, attributes);
                                if(attributes.count()){
                                    TypesystemException::raise(QString("Unexpected attribute '%2' of tag <%1> in line %3").arg(childElement.localName()).arg(attributes.item(0).localName()).arg(childElement.lineNumber()));
                                }
                                ensureNoChildren(childElement);
                            }
                        }
                        s << INDENT << "}" << Qt::endl;
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
        }
    }
    s << INDENT << "}" << Qt::endl;
}

void XmlToQmlConverterPrivate::parseConversionRule(QTextStream& s, const QDomElement &element){
    s << INDENT << "ConversionRule{" << Qt::endl;
    {
        Indentation indent(INDENT);
        QDomNamedNodeMap attributes = element.attributes();
        {
            QDomNode node = attributes.removeNamedItem("class");
            if(node.isAttr()){
                static const QMap<QString, QString> languageNames{
                    {"java", "CodeClass.Java"},
                    {"interface", "CodeClass.JavaInterface"},
                    {"native", "CodeClass.Native"},
                    {"shell", "CodeClass.Shell"},
                    {"shell-declaration", "CodeClass.ShellDeclaration"},
                    {"library-initializer", "CodeClass.PackageInitializer"},
                    {"destructor-function", "CodeClass.DestructorFunction"},
                    {"java-interface-default", "CodeClass.JavaInterface"},
                    {"metainfo", "CodeClass.MetaInfo"},
                    {"constructors", "CodeClass.Constructors"},
                    {"signal", "CodeClass.Signal"},
                    {"module-info", "CodeClass.ModuleInfo"},
                    {"getter", "CodeClass.Getter"},
                    {"setter", "CodeClass.Setter"},
                    {"native-getter", "CodeClass.NativeGetter"},
                    {"native-setter", "CodeClass.NativeSetter"}
                };
                if(languageNames.contains(node.toAttr().value())){
                    s << INDENT << "codeClass: " << languageNames[node.toAttr().value()] << Qt::endl;
                }else{
                    bool ok = false;
                    int value = node.toAttr().value().toInt(&ok);
                    if(ok){
                        s << INDENT << "codeClass: " << value << Qt::endl;
                    }else{
                        TypesystemException::raise(QString("Unexpected value %4 of attribute '%2' of tag <%1> in line %3").arg(element.localName()).arg(node.localName()).arg(element.lineNumber()).arg(node.toAttr().value()));
                    }
                }
            }
            QDomNodeList childNodes = element.childNodes();
            for(int i=0; i<childNodes.length(); ++i){
                QDomNode item = childNodes.item(i);
                switch(item.nodeType()){
                case QDomNode::ElementNode:
                {
                    QDomElement childElement = item.toElement();
                    if(childElement.localName()=="insert-template"){
                        parseInsertTemplate(s, childElement);
                    }else if(childElement.localName()=="import-file"){
                        parseImportFile(s, childElement);
                    }else{
                        TypesystemException::raise(QString("Unexpected tag <%2> as child of tag <%1> in line %3").arg(element.localName()).arg(childElement.localName()).arg(childElement.lineNumber()));
                    }
                }
                    break;
                case QDomNode::TextNode:
                    s << INDENT << "Text{content: " << escapeQuote(item.toText().data(), 14) << "}" << Qt::endl;
                    break;
                case QDomNode::CharacterDataNode:
                    s << INDENT << "Text{content: " << escapeQuote(item.toCharacterData().data(), 14) << "}" << Qt::endl;
                    break;
                case QDomNode::CDATASectionNode:
                    s << INDENT << "Text{content: " << escapeQuote(item.toCDATASection().data(), 14) << "}" << Qt::endl;
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

            checkQtVersion(s, attributes);
            if(attributes.count()){
                TypesystemException::raise(QString("Unexpected attribute '%2' of tag <%1> in line %3").arg(element.localName()).arg(attributes.item(0).localName()).arg(element.lineNumber()));
            }
        }
    }
    s << INDENT << "}" << Qt::endl;
}

void XmlToQmlConverterPrivate::parseInstantiation(QTextStream& s, const QDomElement &element){
    s << INDENT << "Instantiation{" << Qt::endl;
    {
        Indentation indent(INDENT);
        QDomNamedNodeMap attributes = element.attributes();
        {
            auto node = attributes.removeNamedItem("access");
            if(node.isAttr()){
                static const QMap<QString, QString> accesses{
                    {"private", "Modification.Private"},
                    {"public", "Modification.Public"},
                    {"protected", "Modification.Protected"},
                    {"friendly", "Modification.Friendly"},
                    {"final", "Modification.Final"},
                    {"declared-final", "Modification.DeclFinal"},
                    {"non-final", "Modification.NonFinal"}
                };
                QStringList accs = node.toAttr().value().split(",");
                for(QString& a : accs){
                    a = a.trimmed();
                    if(accesses.contains(a)){
                        a = accesses[a];
                    }else{
                        bool ok = false;
                        a.toInt(&ok);
                        if(!ok){
                            TypesystemException::raise(QString("Unexpected value %4 of attribute '%2' of tag <%1> in line %3").arg(element.localName()).arg(node.localName()).arg(element.lineNumber()).arg(node.toAttr().value()));
                        }
                    }
                }
                s << INDENT << "access: " << accs.join(" | ") << Qt::endl;
            }
            textArgument(s, attributes, "rename");
            boolArgument(s, attributes, "no-except", "noExcept");
            boolArgument(s, attributes, "block-exceptions", "blockExceptions");
            boolArgument(s, attributes, "rethrow-exceptions", "rethrowExceptions");
            boolArgument(s, attributes, "deprecated");
            textArgument(s, attributes, "associated-to", "associatedTo");
            boolArgument(s, attributes, "virtual-slot", "virtualSlot");
            boolArgument(s, attributes, "allow-as-slot", "allowAsSlot");
            boolArgument(s, attributes, "private-signal", "privateSignal");
            node = attributes.removeNamedItem("thread-affine");
            if(node.isAttr()){
                static const QMap<QString, QString> affinities{
                    {"ui", "Affinity.UI"},
                    {"pixmap", "Affinity.Pixmap"},
                    {"yes", "true"},
                    {"true", "true"},
                    {"no", "false"},
                    {"false", "false"}
                };
                if(affinities.contains(node.toAttr().value().toLower())){
                    s << INDENT << "threadAffinity: " << affinities[node.toAttr().value().toLower()] << Qt::endl;
                }else{
                    s << INDENT << "threadAffinity: " << escapeQuote(node.toAttr().value(), 16) << Qt::endl;
                }
            }
            textArgument(s, attributes, "pp-condition", "ppCondition");
            textArgument(s, attributes, "throws", "throwing");
            textArgument(s, attributes, "proxy-call", "proxyCall");
            textArgument(s, attributes, "target-type", "targetType");
            checkQtVersion(s, attributes);

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
                    if(childElement.localName()=="argument"){
                        s << INDENT << "Argument{" << Qt::endl;
                        {
                            Indentation indent(INDENT);
                            QDomNamedNodeMap attributes = childElement.attributes();
                            {
                                textArgument(s, attributes, "type");
                                boolArgument(s, attributes, "implicit", "isImplicit");
                                textArgument(s, attributes, "parameter");
                                textArgument(s, attributes, "extends", "extending");
                                checkQtVersion(s, attributes);
                                if(attributes.count()){
                                    TypesystemException::raise(QString("Unexpected attribute '%2' of tag <%1> in line %3").arg(childElement.localName()).arg(attributes.item(0).localName()).arg(childElement.lineNumber()));
                                }
                            }
                        }
                        s << INDENT << "}" << Qt::endl;
                    }else if(childElement.localName()=="modify-argument"){
                        parseModifyArgument(s, childElement);
                    }else if(childElement.localName()=="rename"){
                        s << INDENT << "Rename{" << Qt::endl;
                        {
                            Indentation indent(INDENT);
                            QDomNamedNodeMap attributes = childElement.attributes();
                            {
                                textArgument(s, attributes, "to");
                                checkQtVersion(s, attributes);
                                if(attributes.count()){
                                    TypesystemException::raise(QString("Unexpected attribute '%2' of tag <%1> in line %3").arg(childElement.localName()).arg(attributes.item(0).localName()).arg(childElement.lineNumber()));
                                }
                                ensureNoChildren(childElement);
                            }
                        }
                        s << INDENT << "}" << Qt::endl;
                    }else if(childElement.localName()=="access"){
                        s << INDENT << "Access{" << Qt::endl;
                        {
                            Indentation indent(INDENT);
                            QDomNamedNodeMap attributes = childElement.attributes();
                            {
                                auto node = attributes.removeNamedItem("modifier");
                                if(node.isAttr()){
                                    static const QMap<QString, QString> accesses{
                                        {"private", "Modification.Private"},
                                        {"public", "Modification.Public"},
                                        {"protected", "Modification.Protected"},
                                        {"friendly", "Modification.Friendly"},
                                        {"final", "Modification.Final"},
                                        {"declared-final", "Modification.DeclFinal"},
                                        {"non-final", "Modification.NonFinal"}
                                    };
                                    QStringList accs = node.toAttr().value().split(",");
                                    for(QString& a : accs){
                                        a = a.trimmed();
                                        if(accesses.contains(a)){
                                            a = accesses[a];
                                        }else{
                                            bool ok = false;
                                            a.toInt(&ok);
                                            if(!ok){
                                                TypesystemException::raise(QString("Unexpected value %4 of attribute '%2' of tag <%1> in line %3").arg(element.localName()).arg(node.localName()).arg(element.lineNumber()).arg(node.toAttr().value()));
                                            }
                                        }
                                    }
                                    s << INDENT << "modifiers: " << accs.join(" | ") << Qt::endl;
                                }
                                checkQtVersion(s, attributes);
                                if(attributes.count()){
                                    TypesystemException::raise(QString("Unexpected attribute '%2' of tag <%1> in line %3").arg(childElement.localName()).arg(attributes.item(0).localName()).arg(childElement.lineNumber()));
                                }
                                ensureNoChildren(childElement);
                            }
                        }
                        s << INDENT << "}" << Qt::endl;
                    }else if(childElement.localName()=="add-argument"){
                        s << INDENT << "AddArgument{" << Qt::endl;
                        {
                            Indentation indent(INDENT);
                            QDomNamedNodeMap attributes = childElement.attributes();
                            {
                                numberArgument(s, attributes, "index");
                                textArgument(s, attributes, "default", "defaultExpression");
                                textArgument(s, attributes, "name");
                                textArgument(s, attributes, "type");
                                textArgument(s, attributes, "jni-type", "jniType");
                                boolArgument(s, attributes, "invalidate-after-use", "invalidateAfterUse");
                                checkQtVersion(s, attributes);
                                if(attributes.count()){
                                    TypesystemException::raise(QString("Unexpected attribute '%2' of tag <%1> in line %3").arg(childElement.localName()).arg(attributes.item(0).localName()).arg(childElement.lineNumber()));
                                }
                                ensureNoChildren(childElement);
                            }
                        }
                        s << INDENT << "}" << Qt::endl;
                    }else if(childElement.localName()=="add-type-parameter"){
                        s << INDENT << "AddTypeParameter{" << Qt::endl;
                        {
                            Indentation indent(INDENT);
                            QDomNamedNodeMap attributes = childElement.attributes();
                            {
                                textArgument(s, attributes, "name");
                                textArgument(s, attributes, "extends", "extending");
                                checkQtVersion(s, attributes);
                                if(attributes.count()){
                                    TypesystemException::raise(QString("Unexpected attribute '%2' of tag <%1> in line %3").arg(childElement.localName()).arg(attributes.item(0).localName()).arg(childElement.lineNumber()));
                                }
                                ensureNoChildren(childElement);
                            }
                        }
                        s << INDENT << "}" << Qt::endl;
                    }else if(childElement.localName()=="inject-code"){
                        parseInjectCode(s, childElement, true);
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
            if(attributes.count()){
                TypesystemException::raise(QString("Unexpected attribute '%2' of tag <%1> in line %3").arg(element.localName()).arg(attributes.item(0).localName()).arg(element.lineNumber()));
            }
        }
    }
    s << INDENT << "}" << Qt::endl;
}

void XmlToQmlConverterPrivate::parseModifyFunction(QTextStream& s, const QDomElement &element){
    s << INDENT << (element.localName()=="global-function" ? "GlobalFunction{" : "ModifyFunction{") << Qt::endl;
    {
        Indentation indent(INDENT);
        QDomNamedNodeMap attributes = element.attributes();
        {
            textArgument(s, attributes, "signature");
            boolArgument(s, attributes, "no-except", "noExcept");
            boolArgument(s, attributes, "block-exceptions", "blockExceptions");
            boolArgument(s, attributes, "rethrow-exceptions", "rethrowExceptions");
            textArgument(s, attributes, "rename");

            static const QMap<QString, QString> removes{
                {"all", "RemoveFlag.All"},
                {"java", "RemoveFlag.JavaAndNative"},
                {"non-native", "RemoveFlag.JavaOnly"},
                {"none", "RemoveFlag.None"}
            };
            QDomNode node = attributes.removeNamedItem("remove");
            if(node.isAttr()){
                if(removes.contains(node.toAttr().value())){
                    s << INDENT << "remove: " << removes[node.toAttr().value()] << Qt::endl;
                }else{
                    bool ok = false;
                    int value = node.toAttr().value().toInt(&ok);
                    if(ok){
                        s << INDENT << "remove: " << value << Qt::endl;
                    }else{
                        TypesystemException::raise(QString("Unexpected value %4 of attribute '%2' of tag <%1> in line %3").arg(element.localName()).arg(node.localName()).arg(element.lineNumber()).arg(node.toAttr().value()));
                    }
                }
            }
            node = attributes.removeNamedItem("access");
            if(node.isAttr()){
                static const QMap<QString, QString> accesses{
                    {"private", "Modification.Private"},
                    {"public", "Modification.Public"},
                    {"protected", "Modification.Protected"},
                    {"friendly", "Modification.Friendly"},
                    {"final", "Modification.Final"},
                    {"declared-final", "Modification.DeclFinal"},
                    {"non-final", "Modification.NonFinal"}
                };
                QStringList accs = node.toAttr().value().split(",");
                for(QString& a : accs){
                    a = a.trimmed();
                    if(accesses.contains(a)){
                        a = accesses[a];
                    }else{
                        bool ok = false;
                        a.toInt(&ok);
                        if(!ok){
                            TypesystemException::raise(QString("Unexpected value %4 of attribute '%2' of tag <%1> in line %3").arg(element.localName()).arg(node.localName()).arg(element.lineNumber()).arg(node.toAttr().value()));
                        }
                    }
                }
                s << INDENT << "access: " << accs.join(" | ") << Qt::endl;
            }
            boolArgument(s, attributes, "deprecated");
            textArgument(s, attributes, "associated-to", "associatedTo");
            boolArgument(s, attributes, "virtual-slot", "virtualSlot");
            boolArgument(s, attributes, "allow-as-slot", "allowAsSlot");
            boolArgument(s, attributes, "private-signal", "privateSignal");
            node = attributes.removeNamedItem("thread-affine");
            if(node.isAttr()){
                static const QMap<QString, QString> affinities{
                    {"ui", "Affinity.UI"},
                    {"pixmap", "Affinity.Pixmap"},
                    {"yes", "true"},
                    {"true", "true"},
                    {"no", "false"},
                    {"false", "false"}
                };
                if(affinities.contains(node.toAttr().value().toLower())){
                    s << INDENT << "threadAffinity: " << affinities[node.toAttr().value().toLower()] << Qt::endl;
                }else{
                    s << INDENT << "threadAffinity: " << escapeQuote(node.toAttr().value(), 16) << Qt::endl;
                }
            }
            textArgument(s, attributes, "pp-condition", "ppCondition");
            textArgument(s, attributes, "throws", "throwing");
            textArgument(s, attributes, "proxy-call", "proxyCall");
            textArgument(s, attributes, "target-type", "targetType");
            QDomNodeList childNodes = element.childNodes();
            for(int i=0; i<childNodes.length(); ++i){
                QDomNode item = childNodes.item(i);
                switch(item.nodeType()){
                case QDomNode::ElementNode:
                {
                    QDomElement childElement = item.toElement();
                    if(childElement.localName()=="argument"){
                        s << INDENT << "Argument{" << Qt::endl;
                        {
                            Indentation indent(INDENT);
                            QDomNamedNodeMap attributes = childElement.attributes();
                            {
                                textArgument(s, attributes, "type");
                                boolArgument(s, attributes, "implicit");
                                textArgument(s, attributes, "parameter");
                                textArgument(s, attributes, "extends", "extending");
                                checkQtVersion(s, attributes);
                                if(attributes.count()){
                                    TypesystemException::raise(QString("Unexpected attribute '%2' of tag <%1> in line %3").arg(childElement.localName()).arg(attributes.item(0).localName()).arg(childElement.lineNumber()));
                                }
                            }
                        }
                        s << INDENT << "}" << Qt::endl;
                    }else if(childElement.localName()=="modify-argument"){
                        parseModifyArgument(s, childElement);
                    }else if(childElement.localName()=="instantiation"){
                        parseInstantiation(s, childElement);
                    }else if(childElement.localName()=="rename"){
                        s << INDENT << "Rename{" << Qt::endl;
                        {
                            Indentation indent(INDENT);
                            QDomNamedNodeMap attributes = childElement.attributes();
                            {
                                textArgument(s, attributes, "to");
                                checkQtVersion(s, attributes);
                                if(attributes.count()){
                                    TypesystemException::raise(QString("Unexpected attribute '%2' of tag <%1> in line %3").arg(childElement.localName()).arg(attributes.item(0).localName()).arg(childElement.lineNumber()));
                                }
                                ensureNoChildren(childElement);
                            }
                        }
                        s << INDENT << "}" << Qt::endl;
                    }else if(childElement.localName()=="remove"){
                        s << INDENT << "Remove{" << Qt::endl;
                        {
                            Indentation indent(INDENT);
                            QDomNamedNodeMap attributes = childElement.attributes();
                            {
                                QDomNode node = attributes.removeNamedItem("class");
                                if(node.isAttr()){
                                    if(removes.contains(node.toAttr().value())){
                                        s << INDENT << "codeClass: " << removes[node.toAttr().value()] << Qt::endl;
                                    }else{
                                        bool ok = false;
                                        int value = node.toAttr().value().toInt(&ok);
                                        if(ok){
                                            s << INDENT << "codeClass: " << value << Qt::endl;
                                        }else{
                                            TypesystemException::raise(QString("Unexpected value %4 of attribute '%2' of tag <%1> in line %3").arg(element.localName()).arg(node.localName()).arg(element.lineNumber()).arg(node.toAttr().value()));
                                        }
                                    }
                                }
                                checkQtVersion(s, attributes);
                                if(attributes.count()){
                                    TypesystemException::raise(QString("Unexpected attribute '%2' of tag <%1> in line %3").arg(childElement.localName()).arg(attributes.item(0).localName()).arg(childElement.lineNumber()));
                                }
                                ensureNoChildren(childElement);
                            }
                        }
                        s << INDENT << "}" << Qt::endl;
                    }else if(childElement.localName()=="access"){
                        s << INDENT << "Access{" << Qt::endl;
                        {
                            Indentation indent(INDENT);
                            QDomNamedNodeMap attributes = childElement.attributes();
                            {
                                auto node = attributes.removeNamedItem("modifier");
                                if(node.isAttr()){
                                    static const QMap<QString, QString> accesses{
                                        {"private", "Modification.Private"},
                                        {"public", "Modification.Public"},
                                        {"protected", "Modification.Protected"},
                                        {"friendly", "Modification.Friendly"},
                                        {"final", "Modification.Final"},
                                        {"declared-final", "Modification.DeclFinal"},
                                        {"non-final", "Modification.NonFinal"}
                                    };
                                    QStringList accs = node.toAttr().value().split(",");
                                    for(QString& a : accs){
                                        a = a.trimmed();
                                        if(accesses.contains(a)){
                                            a = accesses[a];
                                        }else{
                                            bool ok = false;
                                            a.toInt(&ok);
                                            if(!ok){
                                                TypesystemException::raise(QString("Unexpected value %4 of attribute '%2' of tag <%1> in line %3").arg(element.localName()).arg(node.localName()).arg(element.lineNumber()).arg(node.toAttr().value()));
                                            }
                                        }
                                    }
                                    s << INDENT << "modifiers: " << accs.join(" | ") << Qt::endl;
                                }
                                checkQtVersion(s, attributes);
                                if(attributes.count()){
                                    TypesystemException::raise(QString("Unexpected attribute '%2' of tag <%1> in line %3").arg(childElement.localName()).arg(attributes.item(0).localName()).arg(childElement.lineNumber()));
                                }
                                ensureNoChildren(childElement);
                            }
                        }
                        s << INDENT << "}" << Qt::endl;
                    }else if(childElement.localName()=="add-argument"){
                        s << INDENT << "AddArgument{" << Qt::endl;
                        {
                            Indentation indent(INDENT);
                            QDomNamedNodeMap attributes = childElement.attributes();
                            {
                                numberArgument(s, attributes, "index");
                                textArgument(s, attributes, "default", "defaultExpression");
                                textArgument(s, attributes, "name");
                                textArgument(s, attributes, "type");
                                textArgument(s, attributes, "jni-type", "jniType");
                                boolArgument(s, attributes, "invalidate-after-use", "invalidateAfterUse");
                                checkQtVersion(s, attributes);
                                if(attributes.count()){
                                    TypesystemException::raise(QString("Unexpected attribute '%2' of tag <%1> in line %3").arg(childElement.localName()).arg(attributes.item(0).localName()).arg(childElement.lineNumber()));
                                }
                                ensureNoChildren(childElement);
                            }
                        }
                        s << INDENT << "}" << Qt::endl;
                    }else if(childElement.localName()=="add-type-parameter"){
                        s << INDENT << "AddTypeParameter{" << Qt::endl;
                        {
                            Indentation indent(INDENT);
                            QDomNamedNodeMap attributes = childElement.attributes();
                            {
                                textArgument(s, attributes, "name");
                                textArgument(s, attributes, "extends", "extending");
                                checkQtVersion(s, attributes);
                                if(attributes.count()){
                                    TypesystemException::raise(QString("Unexpected attribute '%2' of tag <%1> in line %3").arg(childElement.localName()).arg(attributes.item(0).localName()).arg(childElement.lineNumber()));
                                }
                                ensureNoChildren(childElement);
                            }
                        }
                        s << INDENT << "}" << Qt::endl;
                    }else if(childElement.localName()=="inject-code"){
                        parseInjectCode(s, childElement, true);
                    }else if(element.localName()=="global-function" && childElement.localName()=="instantiation"){
                        parseInstantiation(s, childElement);
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
            checkQtVersion(s, attributes);
        }
    }
    s << INDENT << "}" << Qt::endl;
}

void XmlToQmlConverterPrivate::parseModifyField(QTextStream& s, const QDomElement &element){
    s << INDENT << "ModifyField{" << Qt::endl;
    {
        Indentation indent(INDENT);
        QDomNamedNodeMap attributes = element.attributes();
        {
            textArgument(s, attributes, "name");
            boolArgument(s, attributes, "read");
            boolArgument(s, attributes, "write");
            auto node = attributes.removeNamedItem("access");
            if(node.isAttr()){
                static const QMap<QString, QString> accesses{
                    {"private", "Modification.Private"},
                    {"public", "Modification.Public"},
                    {"protected", "Modification.Protected"},
                    {"friendly", "Modification.Friendly"},
                    {"final", "Modification.Final"},
                    {"declared-final", "Modification.DeclFinal"},
                    {"non-final", "Modification.NonFinal"}
                };
                QStringList accs = node.toAttr().value().split(",");
                for(QString& a : accs){
                    a = a.trimmed();
                    if(accesses.contains(a)){
                        a = accesses[a];
                    }else{
                        bool ok = false;
                        a.toInt(&ok);
                        if(!ok){
                            TypesystemException::raise(QString("Unexpected value %4 of attribute '%2' of tag <%1> in line %3").arg(element.localName()).arg(node.localName()).arg(element.lineNumber()).arg(node.toAttr().value()));
                        }
                    }
                }
                s << INDENT << "access: " << accs.join(" | ") << Qt::endl;
            }
            textArgument(s, attributes, "rename");
            QDomNodeList childNodes = element.childNodes();
            for(int i=0; i<childNodes.length(); ++i){
                QDomNode item = childNodes.item(i);
                switch(item.nodeType()){
                case QDomNode::ElementNode:
                {
                    QDomElement childElement = item.toElement();
                    if(childElement.localName()=="rename"){
                        s << INDENT << "Rename{" << Qt::endl;
                        {
                            Indentation indent(INDENT);
                            QDomNamedNodeMap attributes = childElement.attributes();
                            {
                                textArgument(s, attributes, "to");
                                checkQtVersion(s, attributes);
                                if(attributes.count()){
                                    TypesystemException::raise(QString("Unexpected attribute '%2' of tag <%1> in line %3").arg(childElement.localName()).arg(attributes.item(0).localName()).arg(childElement.lineNumber()));
                                }
                                ensureNoChildren(childElement);
                            }
                        }
                        s << INDENT << "}" << Qt::endl;
                    }else if(childElement.localName()=="access"){
                        s << INDENT << "Access{" << Qt::endl;
                        {
                            Indentation indent(INDENT);
                            QDomNamedNodeMap attributes = childElement.attributes();
                            {
                                auto node = attributes.removeNamedItem("modifier");
                                if(node.isAttr()){
                                    static const QMap<QString, QString> accesses{
                                        {"private", "Modification.Private"},
                                        {"public", "Modification.Public"},
                                        {"protected", "Modification.Protected"},
                                        {"friendly", "Modification.Friendly"},
                                        {"final", "Modification.Final"},
                                        {"declared-final", "Modification.DeclFinal"},
                                        {"non-final", "Modification.NonFinal"}
                                    };
                                    QStringList accs = node.toAttr().value().split(",");
                                    for(QString& a : accs){
                                        a = a.trimmed();
                                        if(accesses.contains(a)){
                                            a = accesses[a];
                                        }else{
                                            bool ok = false;
                                            a.toInt(&ok);
                                            if(!ok){
                                                TypesystemException::raise(QString("Unexpected value %4 of attribute '%2' of tag <%1> in line %3").arg(element.localName()).arg(node.localName()).arg(element.lineNumber()).arg(node.toAttr().value()));
                                            }
                                        }
                                    }
                                    s << INDENT << "access: " << accs.join(" | ") << Qt::endl;
                                }
                                checkQtVersion(s, attributes);
                                if(attributes.count()){
                                    TypesystemException::raise(QString("Unexpected attribute '%2' of tag <%1> in line %3").arg(childElement.localName()).arg(attributes.item(0).localName()).arg(childElement.lineNumber()));
                                }
                                ensureNoChildren(childElement);
                            }
                        }
                        s << INDENT << "}" << Qt::endl;
                    }else if(childElement.localName()=="replace-type"){
                        s << INDENT << "ReplaceType{" << Qt::endl;
                        {
                            Indentation indent(INDENT);
                            QDomNamedNodeMap attributes = childElement.attributes();
                            {
                                textArgument(s, attributes, "modified-type", "modifiedType");
                                textArgument(s, attributes, "modified-jni-type", "modifiedJniType");
                                checkQtVersion(s, attributes);
                                if(attributes.count()){
                                    TypesystemException::raise(QString("Unexpected attribute '%2' of tag <%1> in line %3").arg(childElement.localName()).arg(attributes.item(0).localName()).arg(childElement.lineNumber()));
                                }
                                ensureNoChildren(childElement);
                            }
                        }
                        s << INDENT << "}" << Qt::endl;
                    }else if(childElement.localName()=="conversion-rule"){
                        parseConversionRule(s, childElement);
                    }else if(childElement.localName()=="reference-count"){
                        s << INDENT << "ReferenceCount{" << Qt::endl;
                        {
                            Indentation indent(INDENT);
                            QDomNamedNodeMap attributes = childElement.attributes();
                            {
                                boolArgument(s, attributes, "thread-safe", "threadSafe");
                                QDomNode node = attributes.removeNamedItem("action");
                                if(node.isAttr()){
                                    static const QMap<QString, QString> actions{
                                        {"put", "ReferenceCount.Put"},
                                        {"add", "ReferenceCount.Add"},
                                        {"add-all", "ReferenceCount.AddAll"},
                                        {"clear-add", "ReferenceCount.ClearAdd"},
                                        {"clear-add-all", "ReferenceCount.ClearAddAll"},
                                        {"remove", "ReferenceCount.Take"},
                                        {"set", "ReferenceCount.Set"},
                                        {"ignore", "ReferenceCount.Ignore"}
                                    };
                                    if(actions.contains(node.toAttr().value())){
                                        s << INDENT << "action: " << actions[node.toAttr().value()] << Qt::endl;
                                    }else{
                                        bool ok = false;
                                        int value = node.toAttr().value().toInt(&ok);
                                        if(ok){
                                            s << INDENT << "action: " << value << Qt::endl;
                                        }else{
                                            TypesystemException::raise(QString("Unexpected value %4 of attribute '%2' of tag <%1> in line %3").arg(element.localName()).arg(node.localName()).arg(element.lineNumber()).arg(node.toAttr().value()));
                                        }
                                    }
                                }
                                textArgument(s, attributes, "variable-name", "variableName");
                                numberArgument(s, attributes, "key-argument", "keyArgument");
                                textArgument(s, attributes, "declare-variable", "declareVariable");
                                textArgument(s, attributes, "condition");
                                textArgument(s, attributes, "access");
                                checkQtVersion(s, attributes);
                                if(attributes.count()){
                                    TypesystemException::raise(QString("Unexpected attribute '%2' of tag <%1> in line %3").arg(childElement.localName()).arg(attributes.item(0).localName()).arg(childElement.lineNumber()));
                                }
                                ensureNoChildren(childElement);
                            }
                        }
                        s << INDENT << "}" << Qt::endl;
                    }else if(childElement.localName()=="no-null-pointer"){
                        s << INDENT << "NoNullPointer{" << Qt::endl;
                        {
                            Indentation indent(INDENT);
                            QDomNamedNodeMap attributes = childElement.attributes();
                            {
                                textArgument(s, attributes, "default-value", "defaultValue");
                                checkQtVersion(s, attributes);
                                if(attributes.count()){
                                    TypesystemException::raise(QString("Unexpected attribute '%2' of tag <%1> in line %3").arg(childElement.localName()).arg(attributes.item(0).localName()).arg(childElement.lineNumber()));
                                }
                                ensureNoChildren(childElement);
                            }
                        }
                        s << INDENT << "}" << Qt::endl;
                    }else if(childElement.localName()=="define-ownership"){
                        s << INDENT << "DefineOwnership{" << Qt::endl;
                        {
                            Indentation indent(INDENT);
                            QDomNamedNodeMap attributes = childElement.attributes();
                            {
                                QDomNode node = attributes.removeNamedItem("class");
                                if(node.isAttr()){
                                    static const QMap<QString, QString> languageNames{
                                        {"java", "CodeClass.Java"},
                                        {"native", "CodeClass.Native"},
                                        {"shell", "CodeClass.Shell"}
                                    };
                                    if(languageNames.contains(node.toAttr().value())){
                                        s << INDENT << "codeClass: " << languageNames[node.toAttr().value()] << Qt::endl;
                                    }else{
                                        bool ok = false;
                                        int value = node.toAttr().value().toInt(&ok);
                                        if(ok){
                                            s << INDENT << "codeClass: " << value << Qt::endl;
                                        }else{
                                            TypesystemException::raise(QString("Unexpected value %4 of attribute '%2' of tag <%1> in line %3").arg(element.localName()).arg(node.localName()).arg(element.lineNumber()).arg(node.toAttr().value()));
                                        }
                                    }
                                }
                                node = attributes.removeNamedItem("owner");
                                if(node.isAttr()){
                                    static const QMap<QString, QString> ownershipNames{
                                        {"java", "Ownership.Java"},
                                        {"cplusplus", "Ownership.Cpp"},
                                        {"cpp", "Ownership.Cpp"},
                                        {"default", "Ownership.Default"},
                                        {"ignore", "Ownership.Ignore"},
                                        {"invalidate", "Ownership.Invalidate"}
                                    };
                                    if(ownershipNames.contains(node.toAttr().value())){
                                        s << INDENT << "ownership: " << ownershipNames[node.toAttr().value()] << Qt::endl;
                                    }else{
                                        bool ok = false;
                                        int value = node.toAttr().value().toInt(&ok);
                                        if(ok){
                                            s << INDENT << "ownership: " << value << Qt::endl;
                                        }else{
                                            TypesystemException::raise(QString("Unexpected value %4 of attribute '%2' of tag <%1> in line %3").arg(element.localName()).arg(node.localName()).arg(element.lineNumber()).arg(node.toAttr().value()));
                                        }
                                    }
                                }
                                textArgument(s, attributes, "condition");
                                checkQtVersion(s, attributes);
                                if(attributes.count()){
                                    TypesystemException::raise(QString("Unexpected attribute '%2' of tag <%1> in line %3").arg(childElement.localName()).arg(attributes.item(0).localName()).arg(childElement.lineNumber()));
                                }
                                ensureNoChildren(childElement);
                            }
                        }
                        s << INDENT << "}" << Qt::endl;
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
            checkQtVersion(s, attributes);
        }
    }
    s << INDENT << "}" << Qt::endl;
}

void XmlToQmlConverterPrivate::ensureNoChildren(const QDomElement &element){
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

void XmlToQmlConverterPrivate::parseFunctionalType(QTextStream& s, const QDomElement &element){
    s << INDENT << "FunctionalType{" << Qt::endl;
    {
        Indentation indent(INDENT);
        QDomNamedNodeMap attributes = element.attributes();
        {
            textArgument(s, attributes, "name");
            variantArgument(s, attributes, "generate");
            textArgument(s, attributes, "package", "packageName");
            textArgument(s, attributes, "implements", "implementing");
            textArgument(s, attributes, "pp-condition", "ppCondition");
            textArgument(s, attributes, "using");
            textArgument(s, attributes, "java-name", "javaName");
            numberArgument(s, attributes, "count");
            boolArgument(s, attributes, "no-except", "noExcept");
            boolArgument(s, attributes, "block-exceptions", "blockExceptions");
            boolArgument(s, attributes, "rethrow-exceptions", "rethrowExceptions");
            textArgument(s, attributes, "function-name", "functionName");
            boolArgument(s, attributes, "disable-native-id-usage", "disableNativeIdUsage");

            QDomNodeList childNodes = element.childNodes();
            for(int i=0; i<childNodes.length(); ++i){
                QDomNode item = childNodes.item(i);
                switch(item.nodeType()){
                case QDomNode::ElementNode:
                {
                    QDomElement childElement = item.toElement();
                    if(childElement.localName()=="modify-argument"){
                        parseModifyArgument(s, childElement);
                    }else if(childElement.localName()=="inject-code"){
                        parseInjectCode(s, childElement);
                    }else if(childElement.localName()=="include"){
                        parseInclude(s, childElement);
                    }else if(childElement.localName()=="extra-includes"){
                        parseExtraIncludes(s, childElement);
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
            checkQtVersion(s, attributes);
            if(attributes.count()){
                TypesystemException::raise(QString("Unexpected attribute '%2' of tag <%1> in line %3").arg(element.localName()).arg(attributes.item(0).localName()).arg(element.lineNumber()));
            }
        }
    }
    s << INDENT << "}" << Qt::endl;
}

void XmlToQmlConverterPrivate::parseIteratorType(QTextStream& s, const QDomElement &element){
    s << INDENT << "IteratorType{" << Qt::endl;
    {
        Indentation indent(INDENT);
        QDomNamedNodeMap attributes = element.attributes();
        {
            textArgument(s, attributes, "name");
            boolArgument(s, attributes, "generic-class", "genericClass");
            checkQtVersion(s, attributes);
        }
    }
    s << INDENT << "}" << Qt::endl;
}

void XmlToQmlConverterPrivate::parseValueType(QTextStream& s, const QDomElement &element){
    s << INDENT << "ValueType{" << Qt::endl;
    {
        Indentation indent(INDENT);
        QDomNamedNodeMap attributes = element.attributes();
        {
            textArgument(s, attributes, "name");
            boolArgument(s, attributes, "no-meta-type", "noMetaType");
            boolArgument(s, attributes, "polymorphic-base", "isPolymorphicBase");
            textArgument(s, attributes, "polymorphic-id-expression", "polymorphicIdExpression");
            parseAttributesOfComplexType(s, element, attributes);
            QList<QDomElement> unhandledElements = parseChildrenOfComplexType(s, element);
            for(const QDomElement& childElement : unhandledElements){
                TypesystemException::raise(QString("Unexpected tag <%2> as child of tag <%1> in line %3").arg(element.localName()).arg(childElement.toElement().localName()).arg(childElement.lineNumber()));
            }
            checkQtVersion(s, attributes);
            if(attributes.count()){
                TypesystemException::raise(QString("Unexpected attribute '%2' of tag <%1> in line %3").arg(element.localName()).arg(attributes.item(0).localName()).arg(element.lineNumber()));
            }
        }
    }
    s << INDENT << "}" << Qt::endl;
}

void XmlToQmlConverterPrivate::parseTypeAliasType(QTextStream& s, const QDomElement &element){
    s << INDENT << "TypeAliasType{" << Qt::endl;
    {
        Indentation indent(INDENT);
        QDomNamedNodeMap attributes = element.attributes();
        {
            textArgument(s, attributes, "name");
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
            checkQtVersion(s, attributes);
            if(attributes.count()){
                TypesystemException::raise(QString("Unexpected attribute '%2' of tag <%1> in line %3").arg(element.localName()).arg(attributes.item(0).localName()).arg(element.lineNumber()));
            }
        }
    }
    s << INDENT << "}" << Qt::endl;
}

void XmlToQmlConverterPrivate::parseInterfaceType(QTextStream& s, const QDomElement &element){
    s << INDENT << "InterfaceType{" << Qt::endl;
    {
        Indentation indent(INDENT);
        QDomNamedNodeMap attributes = element.attributes();
        {
            textArgument(s, attributes, "name");
            boolArgument(s, attributes, "is-value", "isValue");
            boolArgument(s, attributes, "no-meta-type", "noMetaType");
            boolArgument(s, attributes, "no-impl", "noImpl");
            parseAttributesOfComplexType(s, element, attributes);
            QList<QDomElement> unhandledElements = parseChildrenOfComplexType(s, element);
            for(const QDomElement& childElement : unhandledElements){
                TypesystemException::raise(QString("Unexpected tag <%2> as child of tag <%1> in line %3").arg(element.localName()).arg(childElement.toElement().localName()).arg(childElement.lineNumber()));
            }
            checkQtVersion(s, attributes);
            if(attributes.count()){
                TypesystemException::raise(QString("Unexpected attribute '%2' of tag <%1> in line %3").arg(element.localName()).arg(attributes.item(0).localName()).arg(element.lineNumber()));
            }
        }
    }
    s << INDENT << "}" << Qt::endl;
}

void XmlToQmlConverterPrivate::parseNamespaceType(QTextStream& s, const QDomElement &element){
    s << INDENT << (element.localName()=="header-type" ? "HeaderType{" : "NamespaceType{") << Qt::endl;
    {
        Indentation indent(INDENT);
        QDomNamedNodeMap attributes = element.attributes();
        {
             textArgument(s, attributes, "name");
             parseAttributesOfComplexType(s, element, attributes);
             QList<QDomElement> unhandledElements = parseChildrenOfComplexType(s, element);
             for(const QDomElement& childElement : unhandledElements){
                 TypesystemException::raise(QString("Unexpected tag <%2> as child of tag <%1> in line %3").arg(element.localName()).arg(childElement.toElement().localName()).arg(childElement.lineNumber()));
             }
             checkQtVersion(s, attributes);
             if(attributes.count()){
                 TypesystemException::raise(QString("Unexpected attribute '%2' of tag <%1> in line %3").arg(element.localName()).arg(attributes.item(0).localName()).arg(element.lineNumber()));
             }
        }
    }
    s << INDENT << "}" << Qt::endl;
}

void XmlToQmlConverterPrivate::parseEnumType(QTextStream& s, const QDomElement &element){
    s << INDENT << "EnumType{" << Qt::endl;
    {
        Indentation indent(INDENT);
        QDomNamedNodeMap attributes = element.attributes();
        {
            textArgument(s, attributes, "name");
            textArgument(s, attributes, "package", "packageName");
            textArgument(s, attributes, "flags");
            boolArgument(s, attributes, "generate");
            textArgument(s, attributes, "pp-condition", "ppCondition");
            textArgument(s, attributes, "upper-bound", "upperBound");
            textArgument(s, attributes, "lower-bound", "lowerBound");
            textArgument(s, attributes, "java-scope", "javaScope");
            textArgument(s, attributes, "implements", "implementing");
            boolArgument(s, attributes, "force-integer", "forceInteger");
            boolArgument(s, attributes, "extensible");
            boolArgument(s, attributes, "hidden-metaobject", "hiddenMetaobject");

            QDomNodeList childNodes = element.childNodes();
            for(int i=0; i<childNodes.length(); ++i){
                QDomNode item = childNodes.item(i);
                switch(item.nodeType()){
                case QDomNode::ElementNode:
                {
                    QDomElement childElement = item.toElement();
                    if(childElement.localName()=="reject-enum-value"){
                        s << INDENT << "RejectEnumValue{" << Qt::endl;
                        {
                            Indentation indent(INDENT);
                            QDomNamedNodeMap attributes = childElement.attributes();
                            {
                                textArgument(s, attributes, "name");
                                boolArgument(s, attributes, "remove");
                                checkQtVersion(s, attributes);
                                if(attributes.count()){
                                    TypesystemException::raise(QString("Unexpected attribute of tag <%1>: %2").arg(childElement.localName()).arg(attributes.item(0).localName()));
                                }
                            }
                        }
                        s << INDENT << "}" << Qt::endl;
                    }else if(childElement.localName()=="rename-enum-value"){
                        s << INDENT << "RenameEnumValue{" << Qt::endl;
                        {
                            Indentation indent(INDENT);
                            QDomNamedNodeMap attributes = childElement.attributes();
                            {
                                textArgument(s, attributes, "name");
                                textArgument(s, attributes, "rename");
                                checkQtVersion(s, attributes);
                                if(attributes.count()){
                                    TypesystemException::raise(QString("Unexpected attribute of tag <%1>: %2").arg(childElement.localName()).arg(attributes.item(0).localName()));
                                }
                            }
                        }
                        s << INDENT << "}" << Qt::endl;
                    }else if(childElement.localName()=="inject-code"){
                        parseInjectCode(s, childElement);
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
            checkQtVersion(s, attributes);
            if(attributes.count()){
                TypesystemException::raise(QString("Unexpected attribute '%2' of tag <%1> in line %3").arg(element.localName()).arg(attributes.item(0).localName()).arg(element.lineNumber()));
            }
        }
    }
    s << INDENT << "}" << Qt::endl;
}
