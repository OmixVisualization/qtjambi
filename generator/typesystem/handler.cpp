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

bool Handler::error(const QXmlParseException &e) {
    qWarning("Error: line=%d, column=%d, message=%s\n",
             e.lineNumber(), e.columnNumber(), qPrintable(e.message()));
    return false;
}

bool Handler::fatalError(const QXmlParseException &e) {
    qWarning("Fatal error: line=%d, column=%d, message=%s\n",
             e.lineNumber(), e.columnNumber(), qPrintable(e.message()));

    return false;
}

bool Handler::warning(const QXmlParseException &e) {
    qWarning("Warning: line=%d, column=%d, message=%s\n",
             e.lineNumber(), e.columnNumber(), qPrintable(e.message()));

    return false;
}

void Handler::fetchAttributeValues(const QString &name, const QXmlAttributes &atts,
                                   QHash<QString, QString> *acceptedAttributes) {
    Q_ASSERT(acceptedAttributes);

    for (int i = 0; i < atts.length(); ++i) {
        QString key = atts.localName(i).toLower();
        const QString val = atts.value(i);

        if (!acceptedAttributes->contains(key)) {
            const QString qname = atts.qName(i);
            if (qname.indexOf(':') < 0){	// FIXME remove this once namespace work is done
                if(current && current->entry){
                    ReportHandler::warning(QString("Unknown attribute in '%1' for '%2': '%3'").arg(current->entry->name()).arg(name).arg(key));
                }else{
                    ReportHandler::warning(QString("Unknown attribute for '%1': '%2'").arg(name).arg(key));
                }
            }
        } else {
            (*acceptedAttributes)[key] = val;
        }
    }
}

bool Handler::endElement(const QString &, const QString &localName, const QString &) {
    QString tagName = localName.toLower();
    if (tagName == "import-file")
        return true;

    if (!current)
        return true;

    switch (current->type) {
        case StackElement::FunctionalTypeEntry:{
            FunctionalTypeEntry* fentry = static_cast<FunctionalTypeEntry *>(current->entry);
            for(const FunctionModification& mod : m_function_mods){
                for(const ArgumentModification & amod : mod.argument_mods){
                    fentry->addArgumentModification(amod);
                }
            }
        }
        break;
        case StackElement::IteratorTypeEntry:
        case StackElement::ObjectTypeEntry:
        case StackElement::ValueTypeEntry:
        case StackElement::InterfaceTypeEntry:
        case StackElement::NamespaceTypeEntry: {
            ComplexTypeEntry *centry = static_cast<ComplexTypeEntry *>(current->entry);
            centry->setFunctionModifications(m_function_mods);
            centry->setFieldModifications(m_field_mods);
            centry->setCodeSnips(m_code_snips);

            if (centry->designatedInterface()) {
                centry->designatedInterface()->setCodeSnips(m_code_snips);
                centry->designatedInterface()->setFunctionModifications(m_function_mods);
            }
            m_code_snips = CodeSnipList();
            m_function_mods = FunctionModificationList();
            m_field_mods = FieldModificationList();
        }
        break;
        case StackElement::EnumTypeEntry: {
            EnumTypeEntry *centry = static_cast<EnumTypeEntry *>(current->entry);
            centry->setCodeSnips(m_code_snips);
            m_code_snips = CodeSnipList();
        }
        break;
        case StackElement::TemplateTypeEntry: {
            TemplateTypeEntry *centry = static_cast<TemplateTypeEntry *>(current->entry);
            centry->setFunctionModifications(m_function_mods);
            centry->setFieldModifications(m_field_mods);
            centry->setCodeSnips(m_code_snips);
            m_code_snips = CodeSnipList();
            m_function_mods = FunctionModificationList();
            m_field_mods = FieldModificationList();
            m_database->addTemplateType(centry);
        }
        break;
        case StackElement::CustomMetaConstructor: {
            current->entry->setCustomConstructor(*current->value.customFunction);
            delete current->value.customFunction;
        }
        break;
        case StackElement::CustomMetaDestructor: {
            current->entry->setCustomDestructor(*current->value.customFunction);
            delete current->value.customFunction;
        }
        break;
        case StackElement::Template:
            m_database->addTemplate(current->value.templateEntry);
            break;
        case StackElement::ImportTemplate:
            {
                Q_ASSERT(current->entry && current->entry->isComplex());
                ComplexTypeEntry* templateType = static_cast<ComplexTypeEntry*>(current->entry);
                ComplexTypeEntry* centry = static_cast<ComplexTypeEntry*>(current->parent->entry);
                m_code_snips << templateType->codeSnips();
                m_function_mods << templateType->functionModifications();
                m_field_mods << templateType->fieldModifications();
                for(const Include& incl : templateType->extraIncludes()){
                    centry->addExtraInclude(incl);
                }
            }
            break;
        case StackElement::TemplateInstanceEnum:
            if (current->parent->type == StackElement::InjectCode) {
                m_code_snips.last().addTemplateInstance(current->value.templateInstance);
            } else if (current->parent->type == StackElement::Template) {
                current->parent->value.templateEntry->addTemplateInstance(current->value.templateInstance);
            } else if (current->parent->type == StackElement::CustomMetaConstructor || current->parent->type == StackElement::CustomMetaConstructor) {
                current->parent->value.customFunction->addTemplateInstance(current->value.templateInstance);
            } else if (current->parent->type == StackElement::ConversionRule) {
                m_function_mods.last().argument_mods.last().conversion_rules.last().addTemplateInstance(current->value.templateInstance);
            } else if (current->parent->type == StackElement::InjectCodeInFunction) {
                m_function_mods.last().snips.last().addTemplateInstance(current->value.templateInstance);
            }
            break;
        default:
            break;
    }

    StackElement *child = current;
    current = current->parent;
    delete(child);

    return true;
}

bool Handler::characters(const QString &ch) {
    if(ch.trimmed().isEmpty())
        return true;
    if (current->type == StackElement::Template) {
        current->value.templateEntry->addCode(ch);
        return true;
    }

    if (current->type == StackElement::CustomMetaConstructor || current->type == StackElement::CustomMetaDestructor) {
        current->value.customFunction->addCode(ch);
        return true;
    }

    if (current->type == StackElement::ConversionRule) {
        if(current->parent && current->parent->type == StackElement::ModifyField){
            m_field_mods.last().conversion_rules.last().addCode(ch);
        }else if(!m_function_mods.isEmpty()
                 && !m_function_mods.last().argument_mods.isEmpty()
                 && !m_function_mods.last().argument_mods.last().conversion_rules.isEmpty()){
            m_function_mods.last().argument_mods.last().conversion_rules.last().addCode(ch);
        }
        return true;
    }

    if (current->parent) {
        if ((current->type & StackElement::CodeSnipMask) != 0) {
            switch (current->parent->type) {
                case StackElement::Root:
                    static_cast<TypeSystemTypeEntry *>(current->parent->entry)->snips.last().addCode(ch);
                    break;
                case StackElement::ModifyFunction:
                    m_function_mods.last().snips.last().addCode(ch);
                    break;
                case StackElement::NamespaceTypeEntry:
                case StackElement::TemplateTypeEntry:
                case StackElement::ObjectTypeEntry:
                case StackElement::ValueTypeEntry:
                case StackElement::InterfaceTypeEntry:
                case StackElement::IteratorTypeEntry:
                case StackElement::EnumTypeEntry:
                    m_code_snips.last().addCode(ch);
                    break;
                default:
                    Q_ASSERT(false);
            }
            return true;
        }
    }

    return true;
}

bool Handler::importFileElement(const QXmlAttributes &atts) {
    QString fileName = atts.value("name");
    if (fileName.isEmpty()) {
        m_error = "Required attribute 'name' missing for include-file tag.";
        return false;
    }

    QFile file(fileName);
    QFileInfo fileinfo(file);
    if (fileinfo.isRelative() && !m_importInputDirectoryList.isEmpty()) {
        // Resolve
        QString filepath = resolveFilePath(fileName, 0, m_importInputDirectoryList);
        if(filepath.isNull()) {
            m_error = QString("Could not open file: '%1'").arg(fileName);
            return false;
        }
        file.setFileName(filepath);
    }
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
#if 0
        // This feature is disabled, better to use actual files
        file.setFileName(":/io/qt/generator/" + fileName);
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
#endif
            m_error = QString("Could not open file: '%1'").arg(fileName);
            return false;
#if 0
        }
#endif
    }

    QString quoteFrom = atts.value("quote-after-line");
    bool foundFromOk = quoteFrom.isEmpty();
    bool from = quoteFrom.isEmpty();

    QString quoteTo = atts.value("quote-before-line");
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
        //characters("\n");
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
        characters(lines.join("\n") + "\n");
    }
    if (!foundFromOk || !foundToOk) {
        QString fromError = QString("Could not find quote-after-line='%1' in file '%2'.").arg(quoteFrom).arg(fileName);
        QString toError = QString("Could not find quote-before-line='%1' in file '%2'.").arg(quoteTo).arg(fileName);

        if (!foundToOk)
            m_error = toError;
        if (!foundFromOk)
            m_error = fromError;
        if (!foundFromOk && !foundToOk)
            m_error = fromError + " " + toError;
        return false;
    }

    return true;
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

QHash<QString, QString> Handler::setStackElementAttributes(StackElement::ElementType type) {
    QHash<QString, QString> attributes;
    attributes["name"] = QString();
    attributes["since"] = QString();
    attributes["until"] = QString();

    switch (type) {
        case StackElement::FunctionalTypeEntry:
            attributes["java-name"] = QString();
            attributes["using"] = QString();
            attributes["deprecated"] = QString("no");
            attributes["implements"] = QString();
            attributes["count"] = QString("0");
            attributes["pp-condition"] = QString();
            attributes["package"] = m_defaultPackage;
            attributes["generate"] = QString("yes");
            break;
        case StackElement::PrimitiveTypeEntry:
            attributes["java-name"] = QString();
            attributes["jni-name"] = QString();
            attributes["preferred-conversion"] = "yes";
            attributes["preferred-java-type"] = "yes";
            break;
        case StackElement::EnumTypeEntry:
            attributes["flags"] = "no";
            attributes["upper-bound"] = QString();
            attributes["lower-bound"] = QString();
            attributes["cpp-name"] = QString();
            attributes["force-integer"] = "no";
            attributes["extensible"] = "no";
            attributes["implements"] = QString();
            attributes["hidden-metaobject"] = "no";
            attributes["package"] = m_defaultPackage;
            attributes["generate"] = QString("yes");
            break;
        case StackElement::IteratorTypeEntry:
            attributes["pointer"] = QString("no");
            Q_FALLTHROUGH();
        case StackElement::ObjectTypeEntry:
            if(type==StackElement::ObjectTypeEntry)
                attributes["is-value-owner"] = QString("no");
            Q_FALLTHROUGH();
        case StackElement::ValueTypeEntry:
            attributes["force-abstract"] = QString("no");
            attributes["force-friendly"] = QString("no");
            attributes["deprecated"] = QString("no");
            attributes["implements"] = QString();
            attributes["polymorphic-base"] = QString("no");
            attributes["polymorphic-id-expression"] = QString();
            // fall throooough
            Q_FALLTHROUGH();
        case StackElement::InterfaceTypeEntry:
            attributes["default-superclass"] = m_defaultSuperclass;
            attributes["delete-in-main-thread"] = QString("no");
            attributes["delete-in-owner-thread"] = QString("no");
            attributes["owner"] = QString("");
            attributes["implements"] = QString();
            if(type==StackElement::InterfaceTypeEntry)
                attributes["is-value"] = QString("no");
            // fall through
            Q_FALLTHROUGH();
        case StackElement::NamespaceTypeEntry:
            attributes["java-name"] = QString();
            attributes["package"] = m_defaultPackage;
            attributes["expense-cost"] = "1";
            attributes["expense-limit"] = "none";
            attributes["generate"] = QString("yes");
            attributes["target-type"] = QString();
            attributes["generic-class"] = QString("no");
            attributes["pp-condition"] = QString();
            break;
        case StackElement::TemplateTypeEntry:
            break;
        default:
            ; // nada
    }

    return attributes;
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
                    return QT_VERSION_CHECK(value, 0, 0);
                }else{
                    return value;
                }
            }
        }else{
            uint major = split[0].toUInt(&ok);
            if(ok){
                uint minor = split[1].toUInt(&ok);
                if(ok){
                    uint patch;
                    if(split.size()>2){
                        patch = split[2].toUInt(&ok);
                    }else{
                        patch = defaultValue & 0x0000ff;
                    }
                    if(ok){
                        return QT_VERSION_CHECK(major, minor, patch);
                    }
                }
            }
        }
    }
    return defaultValue;
}

bool Handler::startElement(const QString &, const QString &n,
                           const QString &, const QXmlAttributes &atts) {
    QString tagName = n.toLower();
    if (tagName == "import-file") {
        return importFileElement(atts);
    }

    StackElement *element = new StackElement(current);

    if (!tagNames.contains(tagName)) {
        m_error = QString("Unknown tag name: '%1'").arg(tagName);
        return false;
    }

    element->type = tagNames[tagName];
    if (element->type & StackElement::TypeEntryMask) {
        if (current->type != StackElement::Root) {
            m_error = "Nested types not supported";
            return false;
        }

        QHash<QString, QString> attributes;

        attributes = setStackElementAttributes(element->type);
        fetchAttributeValues(tagName, atts, &attributes);

        QString name = attributes["name"];

        uint since = parseQtVersion(attributes["since"], 0x000000);
        uint until = parseQtVersion(attributes["until"], 0xffffff);

        if ((since<=QT_VERSION && until>=QT_VERSION) && (!current || current->type != StackElement::None)){
            /*
            We need to be able to have duplicate primitive type entries, or it's not possible to
            cover all primitive java types (which we need to do in order to support fake
            meta objects)
            */
            if (element->type != StackElement::PrimitiveTypeEntry) {
                TypeEntry *tmp = m_database->findType(name);
                if (tmp) {
                    ReportHandler::warning(QString("Duplicate type entry: '%1'").arg(name));
                }
            }

            if (name.isEmpty()) {
                m_error = "no 'name' attribute specified";
                return false;
            }

            switch (element->type) {
                case StackElement::PrimitiveTypeEntry: {
                    QString java_name = attributes["java-name"];
                    QString jni_name = attributes["jni-name"];
                    QString preferred_conversion = attributes["preferred-conversion"].toLower();
                    QString preferred_java_type = attributes["preferred-java-type"].toLower();

                    if (java_name.isEmpty())
                        java_name = name;
                    if (jni_name.isEmpty())
                        jni_name = name;

                    PrimitiveTypeEntry *type = new PrimitiveTypeEntry(name, m_defaultPackage);
                    type->setCodeGeneration(m_generate);
                    type->setTargetLangName(java_name);
                    type->setJniName(jni_name);

                    type->setPreferredConversion(convertBoolean(preferred_conversion, "preferred-conversion", true));
                    type->setPreferredTargetLangType(convertBoolean(preferred_java_type, "preferred-java-type", true));

                    element->entry = type;
                }
                break;
            case StackElement::FunctionalTypeEntry: {
                QStringList names = name.split(QLatin1String("::"));

                FunctionalTypeEntry *fentry;
                if (names.size() == 1) {
                    fentry = new FunctionalTypeEntry(QString(), name);
                } else {
                    fentry = new FunctionalTypeEntry(QStringList(names.mid(0, names.size() - 1)).join("::"), names.last());
                }
                m_function_mods.append(FunctionModification());

                element->entry = fentry;
                if(attributes["generate"]=="no-shell"){
                    fentry->setCodeGeneration(TypeEntry::GenerateNoShell | TypeEntry::GenerateAll);
                }else if (!convertBoolean(attributes["generate"], "generate", true))
                    fentry->setCodeGeneration(TypeEntry::GenerateNothing);
                else
                    fentry->setCodeGeneration(m_generate);
                fentry->setTargetLangPackage(attributes["package"]);
                fentry->setTargetTypeSystem(m_defaultPackage);
                fentry->setImplements(attributes["implements"]);
                fentry->setPPCondition(attributes["pp-condition"]);
                fentry->setUsing(attributes["using"]);
                if(!attributes["java-name"].isEmpty())
                    fentry->setTargetLangName(attributes["java-name"]);

                QString _count = attributes["count"];
                bool ok = false;
                uint count = _count.toUInt(&ok);
                if (!ok) {
                    m_error = QString("Cannot convert '%1' to integer").arg(_count);
                    return false;
                }
                fentry->setCount(count);
            }
            break;
                case StackElement::EnumTypeEntry: {
                    QStringList names = name.split(QLatin1String("::"));

                    EnumTypeEntry *eentry;
                    if (names.size() == 1) {
                        eentry = new EnumTypeEntry(QString(), name);
                    } else {
                        eentry =
                            new EnumTypeEntry(QStringList(names.mid(0, names.size() - 1)).join("::"),
                                              names.last());
                    }
                    element->entry = eentry;
                    if(attributes["generate"]=="no-shell"){
                        eentry->setCodeGeneration(TypeEntry::GenerateNoShell | TypeEntry::GenerateAll);
                    }else if (!convertBoolean(attributes["generate"], "generate", true))
                        eentry->setCodeGeneration(TypeEntry::GenerateNothing);
                    else
                        eentry->setCodeGeneration(m_generate);
                    eentry->setTargetLangPackage(attributes["package"]);
                    eentry->setTargetTypeSystem(m_defaultPackage);
                    eentry->setUpperBound(attributes["upper-bound"]);
                    eentry->setLowerBound(attributes["lower-bound"]);
                    if(!attributes["cpp-name"].isEmpty()){
                        eentry->setCppName(attributes["cpp-name"]);
                    }
                    eentry->setImplements(attributes["implements"]);
                    eentry->setForceInteger(convertBoolean(attributes["force-integer"], "force-integer", false));
                    eentry->setExtensible(convertBoolean(attributes["extensible"], "extensible", false));
                    eentry->setHiddenMetaObject(convertBoolean(attributes["hidden-metaobject"], "hidden-metaobject", false));

                    // put in the flags parallel...
                    if (!attributes["flags"].isEmpty() && attributes["flags"].toLower() != "no") {
                        FlagsTypeEntry *ftype = new FlagsTypeEntry("QFlags<" + eentry->qualifiedCppName() + ">");
                        ftype->setOriginator(eentry);
                        QStringList flags = attributes["flags"].split(QLatin1String("::"));
                        if(!attributes["cpp-name"].isEmpty()){
                            QStringList scope = attributes["cpp-name"].split(QLatin1String("::"));
                            Q_ASSERT(scope.size());
                            Q_ASSERT(flags.size());
                            scope.removeLast();
                            scope.append(flags.last());
                            ftype->setCppName(scope.join("::"));
                        }
                        ftype->setOriginalName(attributes["flags"]);
                        ftype->setCodeGeneration(eentry->codeGeneration());
                        QString n = ftype->originalName();

                        QStringList lst = n.split("::");
                        if (QStringList(lst.mid(0, lst.size() - 1)).join("::") != QStringList(names.mid(0, names.size() - 1)).join("::")) {
                            ReportHandler::warning(QString("enum %1 and flags %2 differ in qualifiers")
                                                   .arg(eentry->qualifiedCppName())
                                                   .arg(n));
                        }

                        ftype->setFlagsName(lst.last());
                        eentry->setFlags(ftype);

                        m_database->addFlagsType(ftype);
                        //qDebug()<<"Adding ftype"<<ftype->name();
                        ReportHandler::debugTypes("Adding to TypeDatabase(1): " + ftype->name());
                        m_database->addType(ftype);
                    }
                }
                break;

                case StackElement::InterfaceTypeEntry: {
                    ImplementorTypeEntry *otype;
                    if(convertBoolean(attributes["is-value"], "is-value", false))
                        otype = new ValueTypeEntry(InterfaceTypeEntry::implName(name));
                    else
                        otype = new ObjectTypeEntry(InterfaceTypeEntry::implName(name));
                    InterfaceTypeEntry *itype = new InterfaceTypeEntry(name);
                    if(attributes["generate"]=="no-shell"){
                        itype->setCodeGeneration(TypeEntry::GenerateNoShell | TypeEntry::GenerateAll);
                    }else if (!convertBoolean(attributes["generate"], "generate", true))
                        itype->setCodeGeneration(TypeEntry::GenerateForSubclass);
                    else
                        itype->setCodeGeneration(m_generate);
                    otype->setDesignatedInterface(itype);
                    itype->setOrigin(otype);
                    itype->setImplements(attributes["implements"]);
                    element->entry = otype;
                }
                // fall through
                Q_FALLTHROUGH();
                case StackElement::NamespaceTypeEntry:
                    if (!element->entry) {
                        element->entry = new NamespaceTypeEntry(name);
                    }
                    // fall through
                    Q_FALLTHROUGH();
                case StackElement::IteratorTypeEntry:
                    if (!element->entry) {
                        ComplexTypeEntry* containerType = nullptr;
                        QStringList spl = name.split("::");
                        spl.removeLast();
                        TypeEntry *tmp = m_database->findType(spl.join("::"));
                        if(tmp && tmp->isComplex()){
                            containerType = static_cast<ComplexTypeEntry*>(tmp);
                        }
                        element->entry = new IteratorTypeEntry(name, containerType);
                    }
                    // fall through
                    Q_FALLTHROUGH();
                case StackElement::ObjectTypeEntry:
                    if (!element->entry) {
                        ObjectTypeEntry* entry = new ObjectTypeEntry(name);
                        entry->setIsValueOwner(convertBoolean(attributes["is-value-owner"], "is-value-owner", false));
                        element->entry = entry;
                    }
                    // fall through
                    Q_FALLTHROUGH();
                case StackElement::ValueTypeEntry: {
                    if (!element->entry) {
                        element->entry = new ValueTypeEntry(name);
                    }

                    ComplexTypeEntry *ctype = static_cast<ComplexTypeEntry *>(element->entry);
                    ctype->setTargetTypeSystem(m_defaultPackage);
                    ctype->setTargetLangPackage(attributes["package"]);
                    ctype->setDefaultSuperclass(attributes["default-superclass"]);
                    ctype->setGenericClass(convertBoolean(attributes["generic-class"], "generic-class", false));
                    ctype->setImplements(attributes["implements"]);

                    if(attributes["generate"]=="no-shell"){
                        element->entry->setCodeGeneration(TypeEntry::GenerateNoShell | TypeEntry::GenerateAll);
                    }else if (!convertBoolean(attributes["generate"], "generate", true))
                        element->entry->setCodeGeneration(TypeEntry::GenerateForSubclass);
                    else
                        element->entry->setCodeGeneration(m_generate);

                    QString javaName = attributes["java-name"];
                    if (!javaName.isEmpty()){
                        if(ctype->designatedInterface()){
                            ctype->designatedInterface()->setTargetLangName(javaName);
                            ctype->setTargetLangName(InterfaceTypeEntry::implName(javaName).replace("::", "$"));
                        }else{
                            ctype->setTargetLangName(javaName);
                        }
                    }

                    // The expense policy
                    QString limit = attributes["expense-limit"];
                    if (!limit.isEmpty() && limit != "none") {
                        ExpensePolicy ep;
                        ep.limit = limit.toInt();
                        ep.cost = attributes["expense-cost"];
                        ctype->setExpensePolicy(ep);
                    }

                    if (element->type != StackElement::InterfaceTypeEntry
                            && element->type != StackElement::NamespaceTypeEntry) {
                        ctype->setIsPolymorphicBase(convertBoolean(attributes["polymorphic-base"], "polymorphic-base", false));
                        ctype->setPolymorphicIdValue(attributes["polymorphic-id-expression"]);
                    }

                    if (element->type == StackElement::ObjectTypeEntry ||
                            element->type == StackElement::ValueTypeEntry ||
                            element->type == StackElement::InterfaceTypeEntry ||
                            element->type == StackElement::IteratorTypeEntry) {

                        if (element->type == StackElement::IteratorTypeEntry){
                            IteratorTypeEntry *ittype = static_cast<IteratorTypeEntry *>(element->entry);
                            ittype->setIsPointer(convertBoolean(attributes["pointer"], "pointer", false));
                        }

                        if (element->type != StackElement::InterfaceTypeEntry) { // ObjectTypeEntry or ValueTypeEntry
                            if (convertBoolean(attributes["force-abstract"], "force-abstract", false))
                                ctype->setTypeFlags(ctype->typeFlags() | ComplexTypeEntry::ForceAbstract);
                            if (convertBoolean(attributes["force-friendly"], "force-friendly", false))
                                ctype->setTypeFlags(ctype->typeFlags() | ComplexTypeEntry::ForceFriendly);

                            if (convertBoolean(attributes["deprecated"], "deprecated", false))
                                ctype->setTypeFlags(ctype->typeFlags() | ComplexTypeEntry::Deprecated);
                        }

                        ctype->setPPCondition(attributes["pp-condition"]);

                        if (convertBoolean(attributes["delete-in-main-thread"], "delete-in-main-thread", false))
                            ctype->setTypeFlags(ctype->typeFlags() | ComplexTypeEntry::DeleteInMainThread);

                        if (convertBoolean(attributes["delete-in-owner-thread"], "delete-in-owner-thread", false)){
                            ctype->setTypeFlags(ctype->typeFlags() | ComplexTypeEntry::DeleteInOwnerThread);

                            ctype->setOwner(attributes["owner"]);
                            if(ctype->owner().isEmpty()){
                                m_error = "Attribute 'delete-in-owner-thread' requires 'owner' attribute";
                                return false;
                            }
                        }
                    }

                    QString targetType = attributes["target-type"];
                    if (!targetType.isEmpty())
                        ctype->setTargetType(targetType);

                    // ctype->setInclude(Include(Include::IncludePath, ctype->name()));
                    ctype = ctype->designatedInterface();
                    if (ctype){
                        ctype->setTargetLangPackage(attributes["package"]);
                        ctype->setTargetTypeSystem(m_defaultPackage);
                    }
                }
                break;
            case StackElement::TemplateTypeEntry:
                element->entry = new TemplateTypeEntry(name);
                current = element;
                return true;
            default:
                    Q_ASSERT(false);
            }

            if (element->entry) {
                //qDebug()<<"Adding element->entry"<<element->entry->name();
                ReportHandler::debugTypes("Adding to TypeDatabase(2): " + element->entry->name());
                m_database->addType(element->entry);
            } else
                ReportHandler::warning(QString("Type: %1 was rejected by typesystem").arg(name));
        }else{
            element->type = StackElement::None;
        }
    } else if (element->type != StackElement::None) {
        bool topLevel = element->type == StackElement::Root
                        || element->type == StackElement::SuppressedWarning
                        || element->type == StackElement::Rejection
                        || element->type == StackElement::LoadTypesystem
                        || element->type == StackElement::InjectCode
                        || element->type == StackElement::ExtraIncludes
                        || element->type == StackElement::Template;

        if (!topLevel && current->type == StackElement::Root) {
            m_error = QString("Tag requires parent: '%1'").arg(tagName);
            return false;
        }

        StackElement topElement = !current ? StackElement(nullptr) : *current;
        element->entry = topElement.entry;

        QHash<QString, QString> attributes;
        attributes["since"] = QString();
        attributes["until"] = QString();
        switch (element->type) {
            case StackElement::Root:
                attributes["package"] = QString();
                attributes["qt-library"] = QString();
                attributes["default-superclass"] = QString();
                break;
            case StackElement::LoadTypesystem:
                attributes["name"] = QString();
                attributes["unless"] = "";
                attributes["generate"] = "yes";
                attributes["optional"] = "no";
                break;
            case StackElement::NoNullPointers:
                attributes["default-value"] = QString();
                break;
            case StackElement::SuppressedWarning:
                attributes["text"] = QString();
                break;
            case StackElement::ReplaceDefaultExpression:
                attributes["with"] = QString();
                break;
            case StackElement::DefineOwnership:
                attributes["class"] = "java";
                attributes["owner"] = "";
                attributes["condition"] = "";
                break;
            case StackElement::InstantiateTemplate:
                attributes["argument"] = QString();
                attributes["java-name"] = QString();
                break;
            case StackElement::ModifyFunction:
                attributes["signature"] = QString();
                attributes["throws"] = QString();
                attributes["access"] = QString();
                attributes["no-except"] = QString("no");
                attributes["remove"] = QString();
                attributes["rename"] = QString();
                attributes["deprecated"] = QString("no");
                attributes["associated-to"] = QString();
                attributes["virtual-slot"] = QString("no");
                attributes["allow-as-slot"] = QString("no");
                attributes["private-signal"] = QString("no");
                attributes["thread-affine"] = QString("no");
                attributes["pp-condition"] = QString();
                break;
            case StackElement::ModifyArgument:
                attributes["rename"] = QString();
                attributes["index"] = QString();
                attributes["replace-value"] = QString();
                attributes["invalidate-after-use"] = QString("no");
                break;
            case StackElement::AddArgument:
                attributes["name"] = QString();
                attributes["type"] = QString();
                attributes["jni-type"] = QString();
                attributes["invalidate-after-use"] = QString("no");
                break;
            case StackElement::ModifyField:
                attributes["name"] = QString();
                attributes["write"] = "true";
                attributes["read"] = "true";
                attributes["rename"] = QString();
                attributes["access"] = QString();
                break;
            case StackElement::Access:
                attributes["modifier"] = QString();
                break;
            case StackElement::Include:
                attributes["file-name"] = QString();
                attributes["required-features"] = QString();
                attributes["location"] = QString();
                attributes["suppressed"] = QString("no");
                attributes["inherited"] = QString("false");
                break;
            case StackElement::CustomMetaConstructor:
                attributes["name"] = topElement.entry->name().toLower() + "_construct";
                attributes["placement-name"] = "placement";
                attributes["param-name"] = "copy";
                break;
            case StackElement::CustomMetaDestructor:
                attributes["name"] = topElement.entry->name().toLower() + "_destroy";
                attributes["param-name"] = "copy";
                break;
            case StackElement::ReplaceType:
                attributes["modified-type"] = QString();
                attributes["modified-jni-type"] = QString();
                break;
            case StackElement::ArrayType:
                attributes["as-buffer"] = QString("no");
                attributes["length-parameter"] = QString();
                attributes["min-length"] = QString();
                attributes["max-length"] = QString();
                attributes["deref"] = QString("no");
                attributes["varargs"] = QString("no");
                break;
            case StackElement::InjectCode:
                attributes["class"] = "java";
                attributes["position"] = "beginning";
                break;
            case StackElement::InterfacePolymorphicId:
                attributes["interface"] = "";
                attributes["condition"] = "";
                break;
            case StackElement::ConversionRule:
                attributes["class"] = "";
                break;
            case StackElement::RejectEnumValue:
                attributes["name"] = "";
                attributes["remove"] = "no";
                break;
            case StackElement::RenameEnumValue:
                attributes["name"] = "";
                attributes["rename"] = "";
                break;
            case StackElement::ArgumentMap:
                attributes["index"] = "1";
                attributes["meta-name"] = QString();
                break;
            case StackElement::Rename:
                attributes["to"] = QString();
                break;
            case StackElement::Rejection:
                attributes["class"] = "*";
                attributes["function-name"] = "*";
                attributes["field-name"] = "*";
                attributes["enum-name"] = "*";
                break;
            case StackElement::Removal:
                attributes["class"] = "all";
                break;
            case StackElement::Template:
                attributes["name"] = QString();
                break;
            case StackElement::ImportTemplate:
                attributes["template"] = QString();
                break;
            case StackElement::TemplateInstanceEnum:
                attributes["name"] = QString();
                break;
            case StackElement::Replace:
                attributes["from"] = QString();
                attributes["to"] = QString();
                break;
            case StackElement::ReferenceCount:
                attributes["action"] = QString();
                attributes["variable-name"] = QString();
                attributes["thread-safe"] = QString("no");
                attributes["declare-variable"] = QString();
                attributes["access"] = QString("private");
                attributes["condition"] = QString("");
                attributes["key-argument"] = QString();
                break;
            default:
                ; // nada
        }

        if (attributes.count() > 0)
            fetchAttributeValues(tagName, atts, &attributes);

        uint since = parseQtVersion(attributes["since"], 0x000000);
        uint until = parseQtVersion(attributes["until"], 0xffffff);

        if ((since<=QT_VERSION && until>=QT_VERSION) && (!current || current->type != StackElement::None)){
            switch (element->type) {
                case StackElement::Root:{
                    uint since = parseQtVersion(attributes["since"], 0x000000);
                    uint until = parseQtVersion(attributes["until"], 0xffffff);

                    if (since<=QT_VERSION && until>=QT_VERSION){
                        m_defaultPackage = attributes["package"];
                        m_defaultSuperclass = attributes["default-superclass"];
                        element->type = StackElement::Root;
                        element->entry = new TypeSystemTypeEntry(m_defaultPackage, attributes["qt-library"]);
                        //qDebug()<<"Adding element->entry (root)"<<element->entry->name();
                        ReportHandler::debugTypes("Adding to TypeDatabase(3): " + element->entry->name());
                        TypeDatabase::instance()->addType(element->entry);
                    }else{
                        return true;
                    }
                }
                break;
                case StackElement::LoadTypesystem: {
                    QString name = attributes["name"];
                    if (name.isEmpty()) {
                        m_error = "No typesystem name specified";
                        return false;
                    }
                    bool skip = !attributes["unless"].isEmpty() && TypeDatabase::instance()->defined(attributes["unless"]);

                    uint since = parseQtVersion(attributes["since"], 0x000000);
                    uint until = parseQtVersion(attributes["until"], 0xffffff);

                    if (!skip && since<=QT_VERSION && until>=QT_VERSION){
                        if (!m_database->parseFile(name, m_importInputDirectoryList, convertBoolean(attributes["generate"], "generate", true), convertBoolean(attributes["optional"], "optional", false))) {
                            m_error = QString("Failed to parse: '%1'").arg(name);
                            return false;
                        }
                    }
                }
                break;
                case StackElement::RejectEnumValue: {
                    if (current->type != StackElement::EnumTypeEntry) {
                        m_error = "<reject-enum-value> node must be used inside a <enum-type> node";
                        return false;
                    }
                    QString name = attributes["name"];

                    if (!name.isEmpty()) {
                        EnumTypeEntry *eentry = static_cast<EnumTypeEntry*>(current->entry);
                        eentry->addEnumValueRejection(name, convertBoolean(attributes["remove"], "remove", false));
                    }

                }
                break;
                case StackElement::RenameEnumValue: {
                    if (current->type != StackElement::EnumTypeEntry) {
                        m_error = "<rename-enum-value> node must be used inside a <enum-type> node";
                        return false;
                    }
                    QString name = attributes["name"];
                    QString rename = attributes["rename"];

                    if (!name.isEmpty() && !rename.isEmpty()) {
                        EnumTypeEntry *eentry = static_cast<EnumTypeEntry*>(current->entry);
                        eentry->addRenamedEnumValue(name, rename);
                    }

                }
                break;
                case StackElement::ArrayType: {
                    if (topElement.type != StackElement::ModifyArgument) {
                        m_error = "Type replacement can only be specified for argument modifications";
                        return false;
                    }

                    AsArrayTypes asArrayType = AsArrayType::Yes;

                    if(convertBoolean(attributes["deref"], "deref", false))
                        asArrayType |= AsArrayType::Deref;
                    if(convertBoolean(attributes["varargs"], "varargs", false))
                        asArrayType |= AsArrayType::VarArgs;
                    if(convertBoolean(attributes["as-buffer"], "as-buffer", false))
                        asArrayType |= AsArrayType::Buffer;
                    m_function_mods.last().argument_mods.last().useAsArrayType = asArrayType;
                    if (!attributes["length-parameter"].isEmpty()) {
                        bool ok = false;
                        m_function_mods.last().argument_mods.last().arrayLengthParameter = int(attributes["length-parameter"].toUInt(&ok));
                        if (!ok) {
                            m_error = QString("Cannot convert length-parameter '%1' to unsigned integer").arg(attributes["length-parameter"]);
                            return false;
                        }
                    }else{
                        m_function_mods.last().argument_mods.last().arrayLengthParameter = 0;
                        if (!attributes["min-length"].isEmpty()) {
                            bool ok = false;
                            m_function_mods.last().argument_mods.last().minArrayLength = int(attributes["min-length"].toUInt(&ok));
                            if (!ok) {
                                m_error = QString("Cannot convert min-length '%1' to unsigned integer").arg(attributes["min-length"]);
                                return false;
                            }
                            if (!attributes["max-length"].isEmpty()) {
                                ok = false;
                                m_function_mods.last().argument_mods.last().maxArrayLength = int(attributes["max-length"].toUInt(&ok));
                                if (!ok) {
                                    m_error = QString("Cannot convert max-length '%1' to unsigned integer").arg(attributes["max-length"]);
                                    return false;
                                }
                            }else{
                                m_function_mods.last().argument_mods.last().maxArrayLength = -1;
                            }
                        }else{
                            m_function_mods.last().argument_mods.last().minArrayLength = -1;
                            m_function_mods.last().argument_mods.last().maxArrayLength = -1;
                        }
                    }

                    if( (asArrayType & AsArrayType::Buffer) == 0 ){
                        if(m_function_mods.last().argument_mods.last().arrayLengthParameter<1
                                && m_function_mods.last().argument_mods.last().minArrayLength<1){
                            m_error = QString("both length-parameter and min-length must not be <1");
                            return false;
                        }
                    }
                }
                break;
                case StackElement::ReplaceType: {
                    if (topElement.type != StackElement::ModifyArgument
                            && topElement.type != StackElement::ModifyField) {
                        m_error = "Type replacement can only be specified for argument or field modifications";
                        return false;
                    }

                    if (attributes["modified-type"].isEmpty()) {
                        m_error = "Type replacement requires 'modified-type' attribute";
                        return false;
                    }
                    if(topElement.type == StackElement::ModifyField){
                        m_field_mods.last().modified_type = attributes["modified-type"];
                        m_field_mods.last().modified_jni_type = attributes["modified-jni-type"];
                    }else{
                        m_function_mods.last().argument_mods.last().modified_type = attributes["modified-type"];
                        m_function_mods.last().argument_mods.last().modified_jni_type = attributes["modified-jni-type"];
                    }
                }
                break;
                case StackElement::ConversionRule: {
                    if (topElement.type != StackElement::ModifyArgument
                            && topElement.type != StackElement::ModifyField) {
                        m_error = "Conversion rules can only be specified for argument or field modification";
                        return false;
                    }

                    static const QHash<QString, TypeSystem::Language> languageNames{
                        {"native", TypeSystem::NativeCode},
                        {"shell", TypeSystem::ShellCode}
                    };

                    CodeSnip snip;
                    QString languageAttribute = attributes["class"].toLower();
                    TypeSystem::Language lang = languageNames.value(languageAttribute, TypeSystem::NoLanguage);
                    if (lang == TypeSystem::NoLanguage) {
                        m_error = QString("unsupported class attribute: '%1'").arg(languageAttribute);
                        return false;
                    }

                    snip.language = lang;
                    if(topElement.type == StackElement::ModifyField){
                        m_field_mods.last().conversion_rules.append(snip);
                    }else{
                        m_function_mods.last().argument_mods.last().conversion_rules.append(snip);
                    }
                }

                break;
                case StackElement::ModifyArgument: {
                    if (topElement.type != StackElement::ModifyFunction
                            && topElement.type != StackElement::FunctionalTypeEntry) {
                        m_error = QString::fromLatin1("argument modification requires function"
                                                      " modification or functional type as parent, was %1")
                                  .arg(topElement.type, 0, 16);
                        return false;
                    }

                    QString index = attributes["index"];
                    if (index == "return")
                        index = "0";
                    else if (index == "this")
                        index = "-1";

                    bool ok = false;
                    int idx = index.toInt(&ok);
                    if (!ok) {
                        m_error = QString("Cannot convert '%1' to integer").arg(index);
                        return false;
                    }

                    QString replace_value = attributes["replace-value"];

                    if (!replace_value.isEmpty() && idx != 0) {
                        m_error = QString("replace-value is only supported for return values (index=0).");
                        return false;
                    }

                    ArgumentModification argumentModification = ArgumentModification(idx);
                    argumentModification.replace_value = replace_value;
                    argumentModification.modified_name = attributes["rename"];
                    argumentModification.reset_after_use = convertBoolean(attributes["invalidate-after-use"], "invalidate-after-use", false);
                    m_function_mods.last().argument_mods.append(argumentModification);
                }
                break;
                case StackElement::InstantiateTemplate: {
                    TemplateInstantiation templateInstantiation = TemplateInstantiation();
                    QString argument = attributes["argument"];
                    if(!argument.isEmpty())
                        templateInstantiation.arguments << argument;
                    templateInstantiation.setRenamedTo(attributes["java-name"]);
                    m_function_mods.last().template_instantiations.append(templateInstantiation);
                }
                break;
                case StackElement::NoNullPointers: {
                    if (topElement.type != StackElement::ModifyArgument
                            && topElement.type != StackElement::ModifyField) {
                        m_error = "no-null-pointer requires argument or field modification as parent";
                        return false;
                    }

                    if(topElement.type == StackElement::ModifyField){
                        m_field_mods.last().no_null_pointers = true;
                    }else{
                        m_function_mods.last().argument_mods.last().no_null_pointers = true;
                        if (m_function_mods.last().argument_mods.last().index == 0) {
                            m_function_mods.last().argument_mods.last().null_pointer_default_value = attributes["default-value"];
                        } else if (!attributes["default-value"].isEmpty()) {
                            ReportHandler::warning("default values for null pointer guards are only effective for return values");
                        }
                    }
                }
                break;
                case StackElement::DefineOwnership: {
                    if (topElement.type != StackElement::ModifyArgument
                            && topElement.type != StackElement::ModifyField) {
                        m_error = "define-ownership requires argument or field modification as parent";
                        return false;
                    }

                    static const QHash<QString, TypeSystem::Language> languageNames{
                        {"java", TypeSystem::TargetLangCode},
                        {"native", TypeSystem::NativeCode},
                        {"shell", TypeSystem::ShellCode}
                    };

                    QString classAttribute = attributes["class"].toLower();
                    TypeSystem::Language lang = languageNames.value(classAttribute, TypeSystem::NoLanguage);
                    if (lang == TypeSystem::NoLanguage) {
                        m_error = QString("unsupported class attribute: '%1'").arg(classAttribute);
                        return false;
                    }

                    static const QHash<QString, TypeSystem::Ownership> ownershipNames{
                        {"java", TypeSystem::TargetLangOwnership},
                        {"cplusplus", TypeSystem::CppOwnership},
                        {"cpp", TypeSystem::CppOwnership},
                        {"default", TypeSystem::DefaultOwnership},
                        {"ignore", TypeSystem::IgnoreOwnership},
                        {"invalidate", TypeSystem::Invalidate}
                    };

                    QString ownershipAttribute = attributes["owner"].toLower();
                    TypeSystem::Ownership owner = ownershipNames.value(ownershipAttribute, TypeSystem::InvalidOwnership);
                    if (owner == TypeSystem::InvalidOwnership) {
                        m_error = QString("unsupported owner attribute: '%1'").arg(ownershipAttribute);
                        return false;
                    }

                    if(topElement.type == StackElement::ModifyField){
                        m_field_mods.last().ownerships[lang] = OwnershipRule{owner, attributes["condition"]};
                    }else{
                        m_function_mods.last().argument_mods.last().ownerships[lang] = OwnershipRule{owner, attributes["condition"]};
                    }
                }
                break;
                case StackElement::SuppressedWarning:
                    if (attributes["text"].isEmpty())
                        ReportHandler::warning("Suppressed warning with no text specified");
                    else
                        m_database->addSuppressedWarning(attributes["text"]);
                    break;
                case StackElement::ArgumentMap: {
                    if (!(topElement.type & StackElement::CodeSnipMask)) {
                        m_error = "Argument maps requires code injection as parent";
                        return false;
                    }

                    bool ok;
                    int pos = attributes["index"].toInt(&ok);
                    if (!ok) {
                        m_error = QString("Can't convert attribute index '%1' to integer")
                                  .arg(attributes["index"]);
                        return false;
                    }

                    if (pos < 0) {
                        m_error = QString("Argument index %1 must be a positive number").arg(pos);
                        return false;
                    }

                    QString meta_name = attributes["meta-name"];
                    if (meta_name.isEmpty()) {
                        ReportHandler::warning("Empty meta name in argument map");
                    }

                    if (topElement.type == StackElement::InjectCodeInFunction) {
                        m_function_mods.last().snips.last().argumentMap[pos] = meta_name;
                    } else {
                        ReportHandler::warning("Argument maps are only useful for injection of code "
                                               "into functions.");
                    }
                }
                break;
                case StackElement::Removal: {
                    if (topElement.type != StackElement::ModifyFunction) {
                        m_error = "Function modification parent required";
                        return false;
                    }

                    static QHash<QString, TypeSystem::Language> languageNames{
                        {"java", TypeSystem::TargetLangAndNativeCode},
                        {"all", TypeSystem::All}
                    };

                    QString languageAttribute = attributes["class"].toLower();
                    TypeSystem::Language lang = languageNames.value(languageAttribute, TypeSystem::NoLanguage);
                    if (lang == TypeSystem::NoLanguage) {
                        m_error = QString("unsupported class attribute: '%1'").arg(languageAttribute);
                        return false;
                    }

                    m_function_mods.last().removal = lang;
                }
                break;
                case StackElement::Rename:
                case StackElement::Access: {
                    if (topElement.type != StackElement::ModifyField
                            && topElement.type != StackElement::ModifyFunction) {
                        m_error = "Function or field modification parent required";
                        return false;
                    }

                    Modification *mod = nullptr;
                    if (topElement.type == StackElement::ModifyFunction)
                        mod = &m_function_mods.last();
                    else
                        mod = &m_field_mods.last();

                    QString modifier;
                    if (element->type == StackElement::Rename) {
                        modifier = "rename";
                        QString renamed_to = attributes["to"];
                        if (renamed_to.isEmpty()) {
                            m_error = "Rename modifier requires 'to' attribute";
                            return false;
                        }

                        if (topElement.type == StackElement::ModifyFunction)
                            mod->setRenamedTo(renamed_to);
                        else
                            mod->setRenamedTo(renamed_to);
                    } else {
                        modifier = attributes["modifier"].toLower();
                    }

                    if (modifier.isEmpty()) {
                        m_error = "No access modification specified";
                        return false;
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

                    if (!modifierNames.contains(modifier)) {
                        m_error = QString("Unknown access modifier: '%1'").arg(modifier);
                        return false;
                    }

                    mod->modifiers |= modifierNames[modifier];
                }
                break;
                case StackElement::RemoveArgument:
                    if (topElement.type != StackElement::ModifyArgument) {
                        m_error = "Removing argument requires argument modification as parent";
                        return false;
                    }

                    m_function_mods.last().argument_mods.last().removed = true;

                    break;

                case StackElement::AddArgument:
                    if (topElement.type != StackElement::ModifyFunction) {
                        m_error = "Adding argument requires modify-function as parent";
                        return false;
                    }

                    {
                        ArgumentModification argumentModification = ArgumentModification(ArgumentModification::ADDED_ARGUMENT);
                        argumentModification.modified_name = attributes["name"];
                        argumentModification.modified_type = attributes["type"];
                        argumentModification.modified_jni_type = attributes["jni-type"];
                        argumentModification.reset_after_use = convertBoolean(attributes["invalidate-after-use"], "invalidate-after-use", false);
                        m_function_mods.last().argument_mods.append(argumentModification);
                    }
                    break;

                case StackElement::ModifyField: {
                    QString name = attributes["name"];
                    if (name.isEmpty())
                        break;
                    FieldModification fm;
                    fm.name = name;
                    fm.modifiers = 0;


                    QString rename = attributes["rename"];

                    if (convertBoolean(attributes["read"], "read", false)) {
                        fm.modifiers |= FieldModification::Readable;
                    }
                    if (convertBoolean(attributes["write"], "write", false)) {
                        fm.modifiers |= FieldModification::Writable;
                    }
                    if (!rename.isEmpty()){
                        fm.setRenamedTo(rename);
                    }

                    QString access = attributes["access"].toLower();
                    if (!access.isEmpty()) {
                        if (access == QLatin1String("private"))
                            fm.modifiers |= Modification::Private;
                        else if (access == QLatin1String("protected"))
                            fm.modifiers |= Modification::Protected;
                        else if (access == QLatin1String("friendly"))
                            fm.modifiers |= Modification::Friendly;
                        else if (access == QLatin1String("public"))
                            fm.modifiers |= Modification::Public;
                        else if (access == QLatin1String("final"))
                            fm.modifiers |= Modification::Final;
                        else if (access == QLatin1String("declared-final"))
                            fm.modifiers |= Modification::NativeDeclFinal;
                        else if (access == QLatin1String("non-final"))
                            fm.modifiers |= Modification::NonFinal;
                        else {
                            m_error = QString::fromLatin1("Bad access type '%1'").arg(access);
                            return false;
                        }
                    }
                    m_field_mods << fm;
                }
                break;
                case StackElement::ModifyFunction: {
                    if (!(topElement.type & StackElement::ComplexTypeEntryMask)) {
                        m_error = QString::fromLatin1("Modify function requires complex type as parent"
                                                      ", was=%1").arg(topElement.type, 0, 16);
                        return false;
                    }
                    QString signature = attributes["signature"];

                    // Perform global substitutions for QREAL/float/double to support ARM
                    signature = signature.replace(QLatin1String("qreal"), QLatin1String("qtjambireal"));

                    signature = QMetaObject::normalizedSignature(signature.toLocal8Bit().constData());
                    if (signature.isEmpty()) {
                        m_error = "No signature for modified function";
                        return false;
                    }

                    FunctionModification mod;
                    mod.signature = signature;
                    mod.ppCondition = attributes["pp-condition"];
                    mod.throws = attributes["throws"].trimmed();

                    QString access = attributes["access"].toLower();
                    if (!access.isEmpty()) {
                        if (access == QLatin1String("private"))
                            mod.modifiers |= Modification::Private;
                        else if (access == QLatin1String("protected"))
                            mod.modifiers |= Modification::Protected;
                        else if (access == QLatin1String("public"))
                            mod.modifiers |= Modification::Public;
                        else if (access == QLatin1String("friendly"))
                            mod.modifiers |= Modification::Friendly;
                        else if (access == QLatin1String("final"))
                            mod.modifiers |= Modification::Final;
                        else if (access == QLatin1String("declared-final"))
                            mod.modifiers |= Modification::NativeDeclFinal;
                        else if (access == QLatin1String("non-final"))
                            mod.modifiers |= Modification::NonFinal;
                        else {
                            m_error = QString::fromLatin1("Bad access type '%1'").arg(access);
                            return false;
                        }
                    }

                    if (convertBoolean(attributes["deprecated"], "deprecated", false)) {
                        mod.modifiers |= Modification::Deprecated;
                    }
                    if (convertBoolean(attributes["no-except"], "no-except", false)) {
                        mod.modifiers |= Modification::NoExcept;
                    }

                    QString remove = attributes["remove"].toLower();
                    if (!remove.isEmpty()) {
                        if (remove == QLatin1String("all"))
                            mod.removal = TypeSystem::All;
                        else if (remove == QLatin1String("java"))
                            mod.removal = TypeSystem::TargetLangAndNativeCode;
                        else if (remove == QLatin1String("none"))
                            mod.removal = TypeSystem::NoLanguage;
                        else {
                            m_error = QString::fromLatin1("Bad removal type '%1'").arg(remove);
                            return false;
                        }
                    }

                    QString rename = attributes["rename"];
                    if (!rename.isEmpty()) {
                        mod.renamedToName = rename;
                        mod.modifiers |= Modification::Rename;
                    }

                    QString association = attributes["associated-to"];
                    if (!association.isEmpty()) {
                        association = association.replace(QLatin1String("qreal"), QLatin1String("qtjambireal"));
                        mod.association = association;
                    }

                    mod.modifiers |= (convertBoolean(attributes["virtual-slot"], "virtual-slot", false) ? Modification::VirtualSlot : 0);
                    mod.modifiers |= (convertBoolean(attributes["allow-as-slot"], "allow-as-slot", false) ? Modification::AllowAsSlot : 0);
                    mod.modifiers |= (convertBoolean(attributes["private-signal"], "private-signal", false) ? Modification::PrivateSignal : 0);
                    if(attributes["thread-affine"]=="ui"){
                        mod.modifiers |= Modification::UIThreadAffine;
                    }else{
                        mod.modifiers |= (convertBoolean(attributes["thread-affine"], "thread-affine", false) ? Modification::ThreadAffine : 0);
                    }
                    m_function_mods << mod;
                }
                break;
                case StackElement::ReplaceDefaultExpression:
                    if (topElement.type != StackElement::ModifyArgument) {
                        m_error = "Replace default expression only allowed as child of argument modification";
                        return false;
                    }

                    if (attributes["with"].isEmpty()) {
                        m_error = "Default expression replaced with empty string. Use remove-default-expression instead.";
                        return false;
                    }

                    m_function_mods.last().argument_mods.last().replaced_default_expression = attributes["with"];
                    break;
                case StackElement::RemoveDefaultExpression:
                    m_function_mods.last().argument_mods.last().removed_default_expression = true;
                    break;
                case StackElement::CustomMetaConstructor: {
                    CustomFunction *func = new CustomFunction("custom");
                    func->param_name = attributes["param-name"];
                    func->placement_name = attributes["placement-name"];
                    element->value.customFunction = func;
                }
                break;
                case StackElement::CustomMetaDestructor: {
                    CustomFunction *func = new CustomFunction("custom");
                    func->param_name = attributes["param-name"];
                    element->value.customFunction = func;
                }
                break;
                case StackElement::ReferenceCount: {
                    if (topElement.type != StackElement::ModifyArgument
                            && topElement.type != StackElement::ModifyField) {
                        m_error = "reference-count must be child of modify-argument or modify-field";
                        return false;
                    }

                    ReferenceCount rc;
                    rc.threadSafe = convertBoolean(attributes["thread-safe"], "thread-safe", false);

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
                    rc.action = actions.value(attributes["action"].toLower(), ReferenceCount::Invalid);

                    rc.variableName = attributes["variable-name"];
                    if (rc.action != ReferenceCount::Ignore && rc.variableName.isEmpty()) {
                        m_error = "variable-name attribute must be specified";
                        return false;
                    }

                    if (rc.action == ReferenceCount::Put && rc.keyArgument<=0) {
                        m_error = "key-argument attribute must be specified and greater than 0";
                    }

                    rc.declareVariable = attributes["declare-variable"];
                    rc.condition = attributes["condition"];
                    bool ok = false;
                    uint ka = attributes["key-argument"].toUInt(&ok);
                    rc.keyArgument = ok ? ka : 0;

                    static const QHash<QString, uint> accessRights{
                        {"private", ReferenceCount::Private},
                        {"public", ReferenceCount::Public},
                        {"protected", ReferenceCount::Protected},
                        {"friendly", ReferenceCount::Friendly}
                    };
                    rc.access = accessRights.value(attributes["access"].toLower(), 0);
                    if (rc.access == 0) {
                        m_error = "unrecognized access value: " + attributes["access"];
                        return false;
                    }

                    if (rc.action == ReferenceCount::Invalid) {
                        m_error = "unrecognized value for action attribute. supported actions:";
                        for(QString action : actions.keys())
                            m_error += " " + action;
                    }

                    if(topElement.type == StackElement::ModifyField){
                        m_field_mods.last().referenceCounts.append(rc);
                    }else{
                        m_function_mods.last().argument_mods.last().referenceCounts.append(rc);
                    }
                }
                break;
                case StackElement::InterfacePolymorphicId:{
                    if ((topElement.type != StackElement::ObjectTypeEntry)
                            && (topElement.type != StackElement::ValueTypeEntry)
                            && (topElement.type != StackElement::InterfaceTypeEntry)) {
                        m_error = "wrong parent type for interface polymorphic id";
                        return false;
                    }
                    QString interface = attributes["interface"];
                    QString condition = attributes["condition"];
                    if(!interface.isEmpty()){
                        ComplexTypeEntry *ctype = static_cast<ComplexTypeEntry *>(element->entry);
                        ctype->addInterfacePolymorphicIdValue(interface, condition);
                    }
                }
                break;
                case StackElement::InjectCode: {
                    if (((topElement.type & StackElement::ComplexTypeEntryMask) == 0)
                            && (topElement.type != StackElement::EnumTypeEntry)
                            && (topElement.type != StackElement::ModifyFunction)
                            && (topElement.type != StackElement::Root)) {
                        m_error = "wrong parent type for code injection";
                        return false;
                    }

                    static const QHash<QString, TypeSystem::Language> languageNames{
                        {"java", TypeSystem::TargetLangCode},
                        {"native", TypeSystem::NativeCode},
                        {"shell", TypeSystem::ShellCode},
                        {"shell-declaration", TypeSystem::ShellDeclaration},
                        {"library-initializer", TypeSystem::PackageInitializer},
                        {"destructor-function", TypeSystem::DestructorFunction},
                        {"metainfo", TypeSystem::MetaInfo},
                        {"constructors", TypeSystem::Constructors},
                        {"interface", TypeSystem::Interface},
                        {"signal", TypeSystem::Signal}
                    };

                    QString className = attributes["class"].toLower();
                    if (!languageNames.contains(className)) {
                        m_error = QString("Invalid class specifier: '%1'").arg(className);
                        return false;
                    }


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

                    QString position = attributes["position"].toLower();
                    if (!positionNames.contains(position)) {
                        m_error = QString("Invalid position: '%1'").arg(position);
                        return false;
                    }

                    CodeSnip snip;
                    snip.language = languageNames[className];
                    snip.position = positionNames[position];

                    if (snip.language == TypeSystem::Interface && topElement.type != StackElement::InterfaceTypeEntry) {
                        m_error = "Interface code injections must be direct child of an interface type entry";
                        return false;
                    }

                    if (topElement.type == StackElement::ModifyFunction) {
                        FunctionModification mod = m_function_mods.last();
                        if (snip.language == TypeSystem::ShellDeclaration) {
                            m_error = "no function implementation in shell declaration in which to inject code";
                            return false;
                        }

                        m_function_mods.last().snips << snip;
                        element->type = StackElement::InjectCodeInFunction;
                    } else if (topElement.type == StackElement::Root) {
                        static_cast<TypeSystemTypeEntry *>(element->entry)->snips << snip;

                    } else if (topElement.type != StackElement::Root) {
                        m_code_snips << snip;
                    }
                }
                break;
                case StackElement::Include: {
                    if (
                               ((topElement.type & StackElement::ComplexTypeEntryMask) == 0)
                            && ((topElement.type & StackElement::FunctionalTypeEntry) == 0)
                            && (topElement.type != StackElement::ExtraIncludes)) {
                        m_error = "wrong parent type for include";
                        return false;
                    }

                    QString location = attributes["location"].toLower();

                    static const QHash<QString, Include::IncludeType> locationNames{
                        {"global", Include::IncludePath},
                        {"local", Include::LocalPath},
                        {"java", Include::TargetLangImport}
                    };

                    if (!locationNames.contains(location)) {
                        m_error = QString("Location not recognized: '%1'").arg(location);
                        return false;
                    }

                    Include::IncludeType loc = locationNames[location];
                    QMap<QString,QString> requiredFeatures;
                    for(const QString& feature : attributes["required-features"].split(",")){
                        if(!feature.isEmpty())
                            requiredFeatures[feature] = "";
                    }
                    bool inherited = convertBoolean(attributes["inherited"], "inherited", false);
                    bool suppressed = convertBoolean(attributes["suppressed"], "suppressed", false);
                    Include inc(loc, attributes["file-name"], requiredFeatures, inherited, suppressed);

                    if(element->entry->isTypeSystem()){
                        TypeSystemTypeEntry *ttype = static_cast<TypeSystemTypeEntry *>(element->entry);
                        if (topElement.type & StackElement::Root) {
                            ttype->setInclude(inc);
                        } else if (topElement.type == StackElement::ExtraIncludes) {
                            ttype->addExtraInclude(inc);
                        } else {
                            m_error = "Only supported parents are functional types and extra-includes";
                            return false;
                        }
                    }else if(element->entry->isFunctional()){
                        FunctionalTypeEntry *ctype = static_cast<FunctionalTypeEntry *>(element->entry);
                        if (topElement.type & StackElement::FunctionalTypeEntry) {
                            ctype->setInclude(inc);
                        } else if (topElement.type == StackElement::ExtraIncludes) {
                            ctype->addExtraInclude(inc);
                        } else {
                            m_error = "Only supported parents are functional types and extra-includes";
                            return false;
                        }
                    }else{
                        ComplexTypeEntry *ctype = static_cast<ComplexTypeEntry *>(element->entry);
                        if (topElement.type & StackElement::ComplexTypeEntryMask) {
                            ctype->setInclude(inc);
                        } else if (topElement.type == StackElement::ExtraIncludes) {
                            ctype->addExtraInclude(inc);
                        } else {
                            m_error = "Only supported parents are complex types and extra-includes";
                            return false;
                        }

                        if(ctype->isInterface()){
                            const InterfaceTypeEntry* itype = static_cast<const InterfaceTypeEntry*>(ctype);
                            ctype = itype->origin();
                        }else{
                            ctype = ctype->designatedInterface();
                        }
                        if (ctype) {
                            if (topElement.type & StackElement::ComplexTypeEntryMask) {
                                ctype->setInclude(inc);
                            } else if (topElement.type == StackElement::ExtraIncludes) {
                                ctype->addExtraInclude(inc);
                            }
                        }
                    }
                }
                break;
                case StackElement::Rejection: {
                    QString cls = attributes["class"];
                    QString function = attributes["function-name"];
                    QString field = attributes["field-name"];
                    QString enum_ = attributes["enum-name"];
                    if (cls == "*" && function == "*" && field == "*" && enum_ == "*") {
                        m_error = "bad reject entry, neither 'class', 'function-name' nor "
                                  "'field' specified";
                        return false;
                    }
                    m_database->addRejection(cls, function, field, enum_);
                }
                break;
                case StackElement::ImportTemplate:{
                        if ((topElement.type & StackElement::ComplexTypeEntryMask) == 0) {
                            m_error = "wrong parent type for import";
                            return false;
                        }
                        element->entry = m_database->findTemplateType(attributes["template"]);
                        if(!element->entry){
                            m_error = QString("Unable to find template-type '%1'").arg(attributes["template"]);
                            return false;
                        }
                    }
                    break;
                case StackElement::Template:
                    element->value.templateEntry = new TemplateEntry(attributes["name"]);
                    break;
                case StackElement::TemplateInstanceEnum:
                    if (!(topElement.type & StackElement::CodeSnipMask) &&
                            (topElement.type != StackElement::Template) &&
                            (topElement.type != StackElement::CustomMetaConstructor) &&
                            (topElement.type != StackElement::CustomMetaDestructor) &&
                            (topElement.type != StackElement::ConversionRule)) {
                        m_error = "Can only insert templates into code snippets, templates, custom-creators, custom-constructors, custom-deleters, custom-destructors or conversion-rule.";
                        return false;
                    }
                    element->value.templateInstance = new TemplateInstance(attributes["name"]);
                    break;
                case StackElement::Replace:
                    if (topElement.type != StackElement::TemplateInstanceEnum) {
                        m_error = "Can only insert replace rules into insert-template.";
                        return false;
                    }
                    element->parent->value.templateInstance->addReplaceRule(attributes["from"], attributes["to"]);
                    break;
                case StackElement::ExtraIncludes: {
                    if ((topElement.type & StackElement::ComplexTypeEntryMask) == 0
                        && (topElement.type & StackElement::FunctionalTypeEntry) == 0
                            && (topElement.type & StackElement::Root) == 0) {
                        m_error = "wrong parent type for extra-includes";
                        return false;
                    }
                }
                break;
                default:
                    break; // nada
            }
        }else{
            element->type = StackElement::None;
        }
    }

    current = element;
    return true;
}


