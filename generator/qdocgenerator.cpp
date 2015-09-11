/****************************************************************************
**
** Copyright (C) 1992-2009 Nokia. All rights reserved.
** Copyright (C) 2009-2015 Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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

#include "qdocgenerator.h"

#include "reporthandler.h"
#include "typesystem/typesystem.h"

#include <QtCore/QDir>
#include <QDebug>

QDocGenerator::QDocGenerator() { }

QString QDocGenerator::subDirectoryForClass(const AbstractMetaClass *) const {
    return "doc/japi";
}

QString QDocGenerator::fileNameForClass(const AbstractMetaClass *) const {
    return "qdoc.japi";
}

void QDocGenerator::generate() {
    QDir dir(outputDirectory() + "/" + subDirectoryForClass(0));
    dir.mkpath(dir.absolutePath());

    QFile f(dir.absoluteFilePath(fileNameForClass(0)));
    if (!f.open(QIODevice::WriteOnly)) {
        ReportHandler::warning(QString("failed to open file '%1' for writing")
                               .arg(f.fileName()));
        return;
    }

    QTextStream s(&f);
    s << "<japi>" << endl;

    AbstractMetaClassList clazzes = classes();
    foreach(AbstractMetaClass *cls, clazzes) {
        if (shouldGenerate(cls)) {
            write(s, cls);
        }
    }

    s << "</japi>" << endl;
}

// copy-paste from linguist/shared/metatranslator.cpp
static QString numericEntity(int ch) {
    return QString(ch <= 0x20 ? "<byte value=\"x%1\"/>" : "&#x%1;")
           .arg(ch, 0, 16);
}

static QString protect(const QByteArray& str) {
    QString result;
    int len = (int) str.length();
    for (int k = 0; k < len; k++) {
        switch (str[k]) {
            case '\"':
                result += QString("&quot;");
                break;
            case '&':
                result += QString("&amp;");
                break;
            case '>':
                result += QString("&gt;");
                break;
            case '<':
                result += QString("&lt;");
                break;
            case '\'':
                result += QString("&apos;");
                break;
            default:
                if ((uchar) str[k] < 0x20 && str[k] != '\n')
                    result += numericEntity((uchar) str[k]);
                else
                    result += str[k];
        }
    }
    return result;
}


void QDocGenerator::write(QTextStream &s, const AbstractMetaFunction *java_function) {
    AbstractMetaArgumentList arguments = java_function->arguments();
    int argument_count = arguments.size();

    int overload_count = 0;

    for (int i = 0; i < argument_count; ++i) {
        if (!arguments.at(i)->defaultValueExpression().isEmpty())
            ++overload_count;
    }

    for (int i = 0; i <= overload_count; ++i) {
        writeOverload(s, java_function, argument_count - i);
    }
}

void QDocGenerator::writeOverload(QTextStream &s,
                                  const AbstractMetaFunction *java_function,
                                  int arg_count) {
    if (java_function->isModifiedRemoved(TypeSystem::TargetLangCode))
        return;

    uint included_attributes = NoBlockedSlot;
    uint excluded_attributes = 0;
    setupForFunction(java_function, &included_attributes, &excluded_attributes);

    if (arg_count < java_function->arguments().size()) {
        // see JavaGenerator::writeFunctionOverloads()
        if (!java_function->isConstructor())
            included_attributes |= AbstractMetaAttributes::Final;
        excluded_attributes |= AbstractMetaAttributes::Abstract | AbstractMetaAttributes::Native;
    }

    QString signature = functionSignature(java_function,
                                          included_attributes,
                                          excluded_attributes,
                                          NoOption,
                                          arg_count);

    s << "<method java=\"" << protect(signature.toUtf8()) << "\"" << endl
    << "        cpp=\"" << protect(java_function->signature().toUtf8()) << "\"" << endl;

    FunctionModificationList mods = java_function->modifications(java_function->implementingClass());
    QList<ArgumentModification> argumentMods;
    foreach(const FunctionModification &m, mods) {
        if (!m.association.isEmpty())
            s << "        association=\"" << m.association << "\"" << endl;
        if (m.modifiers & Modification::AccessModifierMask)
            s << "        access-change=\"" << m.accessModifierString() << "\"" << endl;
        if (m.modifiers & Modification::Rename)
            s << "        renamed=\"" << m.renamedToName << "\"" << endl;
        if (m.modifiers & (Modification::FinalMask)) {
            s << "        final=\"" << ((m.modifiers & Modification::Final) ? "final" : ( (m.modifiers & Modification::NativeDeclFinal) ? "declared-final" : "non-final"))
            << "\"" << endl;
        }
        if (m.modifiers & Modification::Deprecated)
            s << "        deprecated=\"yes\"" << endl;
        if (m.removal)
            s << "        removal=\"" << m.removal << "\"" << endl;

        argumentMods << m.argument_mods;
    }

    AbstractMetaArgumentList arguments = java_function->arguments();
    bool wroteOwnershipStolen = false;
    foreach(AbstractMetaArgument *argument, arguments) {
        if (java_function->disabledGarbageCollection(java_function->implementingClass(),
                argument->argumentIndex() + 1)) {
            if (!wroteOwnershipStolen) {
                s << endl << "    steals-ownership-of=\"";
                wroteOwnershipStolen = true;
            } else {
                s << ",";
            }
            s << protect(argument->argumentName().toUtf8());
        }
    }
    if (wroteOwnershipStolen)
        s << "\"";

    s << ">" << endl;

    if (argumentMods.size()) {

        foreach(const ArgumentModification &m, argumentMods) {
            s << "    <argument index=\"" << m.index << "\"" << endl;
            if (m.removed_default_expression)
                s << "              remove-default-expression=\"yes\"" << endl;
            if (m.removed)
                s << "              removed=\"yes\"" << endl;
            if (m.no_null_pointers)
                s << "              no-null=\"yes\"" << endl;
            if (!m.modified_type.isEmpty())
                s << "              modified-type=\"" << protect(m.modified_type.toLatin1()) << "\"" << endl;
            if (!m.replaced_default_expression.isEmpty()) {
                s << "              default-expression=\"" << protect(m.replaced_default_expression.toUtf8())
                << "\"" << endl;
            }
            if (!m.referenceCounts.isEmpty())
                s << "              reference-counted=\"...\"" << endl;
            s << "/>" << endl;
        }
    }

    s << "</method>" << endl;
}

void QDocGenerator::write(QTextStream &s, const AbstractMetaEnumValue *java_enum_value) {
    s << "<enum-value java=\"" << protect(java_enum_value->name().toUtf8()) << "\"" << endl
    << "            cpp=\"" << protect(java_enum_value->name().toUtf8()) << "\"" << endl
    << "            value=\"" << java_enum_value->value() << "\"/>" << endl;
}

void QDocGenerator::write(QTextStream &s, const AbstractMetaEnum *java_enum) {
    s << "<enum java=\"" << protect(java_enum->name().toUtf8()) << "\"" << endl
    << "      cpp=\"" << protect(java_enum->name().toUtf8()) << "\"" << endl;

    if (java_enum->typeEntry()->flags()) {
        s << "      flags=\"" << protect(java_enum->typeEntry()->flags()->targetLangName().toUtf8())
        << "\"" << endl;
    }

    s << "      >" << endl;
    AbstractMetaEnumValueList values = java_enum->values();
    foreach(AbstractMetaEnumValue *value, values) {
        write(s, value);
    }
    s << "</enum>" << endl;
}

void QDocGenerator::writeSignal(QTextStream &s, const AbstractMetaFunction *java_function) {
    s << "<signal java=\""
    << protect(java_function->targetLangSignature().toUtf8()) << "\" cpp=\""
    << protect(java_function->signature().toUtf8()) << "\"/>" << endl;
}

void QDocGenerator::write(QTextStream &s, const AbstractMetaField *java_field) {
    uint included_attributes = NoBlockedSlot;
    uint excluded_attributes = 0;
    setupForFunction(java_field->getter(), &included_attributes, &excluded_attributes);
    s << "<variablegetter java=\"" << protect(functionSignature(java_field->getter(), included_attributes, excluded_attributes).toUtf8())
    << "\"" << endl
    << "    cpp=\"" << protect(java_field->name().toUtf8()) << "\" />" << endl;

    included_attributes = NoBlockedSlot;
    excluded_attributes = 0;
    setupForFunction(java_field->setter(), &included_attributes, &excluded_attributes);
    s << "<variablesetter java=\"" << protect(functionSignature(java_field->setter(), included_attributes, excluded_attributes).toUtf8())
    << "\"" << endl
    << "    cpp=\"" << protect(java_field->name().toUtf8()) << "\" />" << endl;
}

void QDocGenerator::write(QTextStream &s, const AbstractMetaClass *java_class) {
    s << "<class" << endl
    << "   java=\"" << protect(java_class->name().toUtf8()) << "\"" << endl
    << "   cpp=\"" << protect(java_class->typeEntry()->qualifiedCppName().toUtf8()) << "\"" << endl
    << "   java-extends=\"" << protect(java_class->baseClass() ? java_class->baseClass()->name().toUtf8() : "") << "\"" << endl
    << "   package=\"" << protect(java_class->package().toUtf8()) << "\"" << endl;

    AbstractMetaClassList interfaces = java_class->interfaces();
    if (interfaces.count() > 0) {
        s << "   javaimplements=\"";
        for (int i = 0; i < interfaces.count(); ++i) {
            const AbstractMetaClass *interfaze = interfaces.at(i);
            if (i > 0)
                s << ",";
            s << protect(interfaze->name().toUtf8());
        }
        s << "\"" << endl;
    }

    s << "   type=\"";
    if (java_class->isInterface())
        s << "interface";
    else if (java_class->typeEntry()->isValue())
        s << "value";
    else
        s << "object";
    s << "\">" << endl;

    // Write signals
    AbstractMetaFunctionList sigs = java_class->queryFunctions(AbstractMetaClass::Signals);
    foreach(AbstractMetaFunction *f, sigs) {
        writeSignal(s, f);
    }

    // Write functions
    AbstractMetaFunctionList functions = java_class->functionsInTargetLang();
    foreach(AbstractMetaFunction *f, functions) {
        write(s, f);
    }

    // Write enums
    AbstractMetaEnumList enums = java_class->enums();
    foreach(AbstractMetaEnum *e, enums) {
        write(s, e);
    }

    // Write setters and getters
    AbstractMetaFieldList fields = java_class->fields();
    foreach(AbstractMetaField *f, fields) {
        write(s, f);
    }

    s << "</class>" << endl;
}
