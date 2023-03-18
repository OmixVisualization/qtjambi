/****************************************************************************
**
** Copyright (C) 1992-2009 Nokia. All rights reserved.
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

#include "asttoxml.h"


#include <QXmlStreamWriter>
#include <QTextStream>
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
#include <QTextCodec>
#endif
#include <QFile>

void astToXML(const QString& name, FileModelItem dom) {
    QFile outputFile(name);
    if (!outputFile.open(QIODevice::WriteOnly)) {
        return;
    }

    QXmlStreamWriter s(&outputFile);
    s.setAutoFormatting(true);

    s.writeStartElement("code");

    for(const NamespaceModelItem& item : dom->namespaces()) {
        writeOutNamespace(s, item);
    }

    for(const ClassModelItem& item : dom->classes()) {
        writeOutClass(s, item);
    }
    s.writeEndElement();
}

void writeOutNamespace(QXmlStreamWriter &s, const NamespaceModelItem &item) {
    s.writeStartElement("namespace");
    s.writeAttribute("name", item->name());

    for(const NamespaceModelItem& item : item->namespaces()) {
        writeOutNamespace(s, item);
    }

    for(const ClassModelItem& item : item->classes()) {
        writeOutClass(s, item);
    }

    for(const EnumModelItem& item : item->enums()) {
        writeOutEnum(s, item);
    }

    s.writeEndElement();
}

void writeOutEnum(QXmlStreamWriter &s, const EnumModelItem &item) {
    QString qualified_name = item->qualifiedName().join("::");
    if (item->isScopedEnum()) {
        s.writeStartElement("scoped-enum");
    }else{
        s.writeStartElement("enum");
    }
    if (item->isAnonymous()) {
        s.writeEmptyElement("anonymous");
    } else {
        s.writeAttribute("name", qualified_name);
    }

    s.writeStartElement("accessPolicy");
    switch (item->accessPolicy()) {
        case CodeModel::Public:
            s.writeAttribute("value", "public");
            break;
        case CodeModel::Private:
            s.writeAttribute("value", "private");
            break;
        case CodeModel::Protected:
            s.writeAttribute("value", "protected");
            break;
    }
    s.writeEndElement();

    const EnumeratorList& enumList = item->enumerators();
    for (int i = 0; i < enumList.size() ; i++) {
        s.writeStartElement("enumerator");
        if (!enumList[i]->value().isEmpty()) {
            s.writeAttribute("value", enumList[i]->value());
        }
        s.writeCharacters(enumList[i]->name());
        s.writeEndElement();
    }
    s.writeEndElement();
}

void writeOutFunction(QXmlStreamWriter &s, const QString &classname, const FunctionModelItem &item) {
    QString name = item.constData()->name();
    bool noPrintReturn = false;

    if (item->functionType() == CodeModel::Signal || item->functionType() == CodeModel::PrivateSignal) {
        s.writeStartElement("signal");
    } else if (item->functionType() == CodeModel::Slot) {
        s.writeStartElement("slot");
    } else if (name == QString("~%1").arg(classname) ) {
        s.writeStartElement("destructor");
        noPrintReturn = true;
    } else if (name == classname) {
        s.writeStartElement("constructor");
        noPrintReturn = true;
    } else {
        s.writeStartElement("function");
    }

    s.writeAttribute("name", name);

    s.writeStartElement("accessPolicy");
    switch (item->accessPolicy()) {
        case CodeModel::Public:
            s.writeAttribute("value","public");
            break;
        case CodeModel::Private:
            s.writeAttribute("value","private");
            break;
        case CodeModel::Protected:
            s.writeAttribute("value","protected");
            break;
    }
    s.writeEndElement();

    s.writeStartElement("modifiers");
    if (item->isAbstract()) {
        s.writeEmptyElement("abstract");
    }

    if (item->isVirtual()) {
        s.writeEmptyElement("virtual");
    }

    if (item->isDeclFinal()) {
        s.writeEmptyElement("isDeclaredFinal");
    }

    if (item->isDeprecated()) {
        s.writeEmptyElement("isDeprecated");
    }

    if (item->isStatic()) {
        s.writeEmptyElement("static");
    }

    s.writeEndElement();

    const ArgumentList& arguments = item->arguments();
    s.writeStartElement("arguments");
    for(ArgumentModelItem arg : arguments) {
        s.writeStartElement("argument");

        if (arg->defaultValue())
            s.writeAttribute("default", arg->defaultValueExpression());
        writeType(s, arg->type());
        s.writeEndElement();
    }

    s.writeEndElement();

    if (!noPrintReturn) {
        s.writeStartElement("return");
        writeType(s, item->type());
        s.writeEndElement();
    }

    s.writeEndElement();
}

void writeOutVariable(QXmlStreamWriter &s, const VariableModelItem &item) {
    const QString& name = item->name();

    s.writeStartElement("variable");

    s.writeAttribute("name", name);

    s.writeStartElement("accessPolicy");
    switch (item->accessPolicy()) {
        case CodeModel::Public:
            s.writeAttribute("value","public");
            break;
        case CodeModel::Private:
            s.writeAttribute("value","private");
            break;
        case CodeModel::Protected:
            s.writeAttribute("value","protected");
            break;
    }
    s.writeEndElement();

    s.writeStartElement("modifiers");
    if (item->isConstant()) {
        s.writeEmptyElement("const");
    }

    if (item->isExtern()) {
        s.writeEmptyElement("extern");
    }

    if (item->isFriend()) {
        s.writeEmptyElement("friend");
    }

    if (item->isMutable()) {
        s.writeEmptyElement("mutable");
    }

    if (item->isRegister()) {
        s.writeEmptyElement("register");
    }

    if (item->isVolatile()) {
        s.writeEmptyElement("volatile");
    }

    if (item->isStatic()) {
        s.writeEmptyElement("static");
    }

    s.writeEndElement();

    s.writeStartElement("type");
    writeType(s, item->type());
    s.writeEndElement();

    s.writeEndElement();
}

void writeType(QXmlStreamWriter &s, const TypeInfo &t) {
    s.writeAttribute("type",  t.qualifiedName().join("::"));
    s.writeAttribute("indirections", QString("%1").arg(t.indirections().size()) );
    s.writeAttribute("referenceType", t.getReferenceType()==TypeInfo::Reference ? "Reference" : (t.getReferenceType()==TypeInfo::RReference ? "RValueReference" : "None"));
    s.writeAttribute("isConstant", t.isConstant() ? "true" : "false");
    s.writeAttribute("isFunctionPointer", t.isFunctionPointer() ? "true" : "false");

    if(!t.arguments().isEmpty()){
        s.writeStartElement("arguments");
        for(const TypeInfo& tt : t.arguments()) {
            writeType(s, tt);
        }
        s.writeEndElement();
    }
}

void writeOutClass(QXmlStreamWriter &s, const ClassModelItem &item) {
    QString qualified_name = item->name();
    s.writeStartElement("class");
    s.writeAttribute("name", qualified_name);

    QList<QPair<TypeInfo,bool>> bases = item.constData()->baseClasses();
    s.writeStartElement("inherits");
    for(const QPair<TypeInfo,bool> &c : bases) {
        s.writeStartElement("class");
        s.writeAttribute("name", c.first.toString());
        s.writeAttribute("access", c.second ? "public" : "protected");
        s.writeEndElement();
    }
    s.writeEndElement();

    for(const EnumModelItem& item : item->enums()) {
        writeOutEnum(s, item);
    }

    for(const FunctionModelItem& item : item->functions()) {
        writeOutFunction(s, qualified_name, item);
    }

    for(const VariableModelItem& item : item->variables()) {
        writeOutVariable(s, item);
    }

    for(const ClassModelItem& item : item->classes()) {
        writeOutClass(s, item);
    }

    QStringList propDecls = item.constData()->propertyDeclarations();
    for(const QString &p : propDecls) {
        writeOutProperty(s, p);
    }

    s.writeEndElement();
}

void writeOutProperty(QXmlStreamWriter &s, const QString &p) {
    QStringList l = p.split(QLatin1String(" "));

    static QLatin1String propDecls[] = {
        QLatin1String("READ"), QLatin1String("WRITE"), QLatin1String("RESET"), QLatin1String("DESIGNABLE")
    };

    s.writeStartElement("property");
    s.writeAttribute("name", l.at(1));
    int propCount = sizeof(propDecls) / sizeof(QLatin1String);
    for (int pos = 2; pos + 1 < l.size(); pos += 2) {
        for (int i=0; i<propCount; ++i) {
            if (l.at(pos) == propDecls[i]) {
                writeHelper(s, propDecls[i], l.at(pos + 1) );
                break;
            }
        }
    }
    s.writeEndElement();
}

void writeHelper(QXmlStreamWriter &s, const QLatin1String &k, const QString &v) {
    s.writeStartElement(QString(k).toLower());
    s.writeAttribute("value", v);
    s.writeEndElement();
}
