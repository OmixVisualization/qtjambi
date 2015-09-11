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

#include "cppheadergenerator.h"

#include <QtCore/QDir>

#include <qdebug.h>

QString CppHeaderGenerator::fileNameForClass(const AbstractMetaClass *java_class) const {
    return QString("qtjambishell_%1.h").arg(java_class->name().replace("$", "_"));
}

void CppHeaderGenerator::writeFieldAccessors(QTextStream &s, const AbstractMetaField *java_field) {
    Q_ASSERT(java_field->isProtected());

    const AbstractMetaFunction *setter = java_field->setter();
    const AbstractMetaFunction *getter = java_field->getter();

    if (!java_field->type()->isConstant())
        writeFunction(s, setter);

    writeFunction(s, getter);
}

void CppHeaderGenerator::writeSignalWrapper(QTextStream &s, const AbstractMetaFunction *signal) {
    s << "    ";
    writeFunctionSignature(s, signal, 0, signalWrapperPrefix(),
                           Option(NormalizeAndFixTypeSignature | OriginalName |
                                  OriginalTypeDescription | IncludeDefaultExpression));
    s << ";" << endl;
}

void CppHeaderGenerator::writeSignalWrappers(QTextStream &s, const AbstractMetaClass *java_class) {
    AbstractMetaFunctionList signal_funcs = signalFunctions(java_class);
    if (signal_funcs.size() > 0) {
        s << endl << "public slots:" << endl;
        foreach(const AbstractMetaFunction *signal, signal_funcs) {
            writeSignalWrapper(s, signal);
        }
    }
}

void CppHeaderGenerator::writeWrapperClass(QTextStream &s, const AbstractMetaClass *java_class) {
    AbstractMetaFunctionList signal_functions = signalFunctions(java_class);
    if (signal_functions.size() == 0)
        return ;

    s << "class QtJambi_SignalWrapper_" << java_class->name() << ": public QObject" << endl
    << "{" << endl
    << "  Q_OBJECT" << endl;
    writeSignalWrappers(s, java_class);
    s << endl << "public:" << endl
    << "    QtJambiSignalInfo m_signals[" << signal_functions.size() << "];" << endl
    << "    QtJambiLink *link;" << endl
    << "};" << endl << endl;
}

void CppHeaderGenerator::write(QTextStream &s, const AbstractMetaClass *java_class, int) {
    QString include_block = "QTJAMBISHELL_" + java_class->name().replace("$", "_").toUpper() + "_H";

    s << "#ifndef " << include_block << endl
    << "#define " << include_block << endl << endl
    << "#include <qtjambi/qtjambi_core.h>" << endl
    << "#include <QtCore/QHash>" << endl;

    Include inc = java_class->typeEntry()->include();
    s << "#include ";
    if (inc.type == Include::IncludePath)
        s << "<";
    else
        s << "\"";
    s << inc.name;
    if (inc.type == Include::IncludePath)
        s << ">";
    else
        s << "\"";
    s << endl << endl;

    IncludeList list = java_class->typeEntry()->extraIncludes();
    qSort(list.begin(), list.end());
    foreach(const Include &inc, list) {
        if (inc.type == Include::TargetLangImport)
            continue;

        s << "#include ";
        if (inc.type == Include::LocalPath)
            s << "\"";
        else
            s << "<";

        s << inc.name;

        if (inc.type == Include::LocalPath)
            s << "\"";
        else
            s << ">";

        s << endl;
    }

    writeForwardDeclareSection(s, java_class);

    writeWrapperClass(s, java_class);

    if (java_class->generateShellClass()) {
        s << "class " << shellClassName(java_class)
        << " : public " << java_class->qualifiedCppName() << endl
        << "{" << endl;

        if (java_class->isQObject()) {
            s << "public:" << endl
            << "  Q_OBJECT_CHECK" << endl
            << "  mutable const QMetaObject *m_meta_object;" << endl;

            if (java_class->hasVirtualSlots()) {
                s << "  mutable QHash<int,int> m_map;" << endl;
            }

            s << "  const QMetaObject *metaObject() const;" << endl
            << "  void *qt_metacast(const char *);" << endl
            << "  QT_TR_FUNCTIONS" << endl
            << "  virtual int qt_metacall(QMetaObject::Call, int, void **);" << endl
            << "private:" << endl;
        }


        s << "public:" << endl;
        foreach(const AbstractMetaFunction *function, java_class->functions()) {
            if (function->isConstructor() && !function->isPrivate())
                writeFunction(s, function, SkipRemovedArguments);
        }

        s << "    ~" << shellClassName(java_class) << "();" << endl;
        s << endl;

        // All functions in original class that should be reimplemented in shell class
        AbstractMetaFunctionList shell_functions = java_class->functionsInShellClass();
        foreach(const AbstractMetaFunction *function, shell_functions) {
            writeFunction(s, function);
        }

        // Public call throughs for protected functions
        AbstractMetaFunctionList public_overrides = java_class->publicOverrideFunctions();
        foreach(const AbstractMetaFunction *function, public_overrides) {
            writePublicFunctionOverride(s, function);
        }

        // Override all virtual functions to get the decision on static/virtual call
        AbstractMetaFunctionList virtual_functions = java_class->virtualOverrideFunctions();
        foreach(const AbstractMetaFunction *function, virtual_functions) {
            writeVirtualFunctionOverride(s, function);
        }

        // Field accessors
        foreach(const AbstractMetaField *field, java_class->fields()) {
            if (field->isProtected())
                writeFieldAccessors(s, field);
        }

        writeVariablesSection(s, java_class);
        writeInjectedCode(s, java_class);

        s  << "};" << endl << endl;
    }

    s  << "#endif // " << include_block << endl;

    QString pro_file_name = java_class->package().replace(".", "_") + "/" +
                            java_class->package().replace(".", "_") + ".pri";

    priGenerator->addHeader(pro_file_name, fileNameForClass(java_class));
}


/*!
    Writes out declarations of virtual C++ functions so that they
    can be reimplemented from the java side.
*/
void CppHeaderGenerator::writeFunction(QTextStream &s, const AbstractMetaFunction *java_function, int options) {
    if (java_function->isModifiedRemoved(TypeSystem::ShellCode))
        return;
    if (java_function->hasTemplateArgumentTypes())
        return;

    s << "    ";
    writeFunctionSignature(s, java_function, 0, QString(), Option(OriginalName | ShowStatic | options));
    s << ";" << endl;
}

void CppHeaderGenerator::writePublicFunctionOverride(QTextStream &s,
        const AbstractMetaFunction *java_function) {
    s << "    ";
    writeFunctionSignature(s, java_function, 0, "__public_", Option(EnumAsInts |
                           ShowStatic | UnderscoreSpaces));
    s << ";" << endl;
}


void CppHeaderGenerator::writeVirtualFunctionOverride(QTextStream &s,
        const AbstractMetaFunction *java_function) {
    if (java_function->isModifiedRemoved(TypeSystem::NativeCode))
        return;

    s << "    ";
    writeFunctionSignature(s, java_function, 0, "__override_", Option(EnumAsInts | ShowStatic |
                           UnderscoreSpaces), QString(), QStringList() << "bool static_call");
    s << ";" << endl;
}


void CppHeaderGenerator::writeForwardDeclareSection(QTextStream &s, const AbstractMetaClass *) {
    s << endl
    << "class QtJambiFunctionTable;" << endl
    << "class QtJambiLink;" << endl;
}


void CppHeaderGenerator::writeVariablesSection(QTextStream &s, const AbstractMetaClass *) {
    s << endl
    << "    QtJambiFunctionTable *m_vtable;" << endl
    << "    QtJambiLink *m_link;" << endl;
}

void CppHeaderGenerator::writeInjectedCode(QTextStream &s, const AbstractMetaClass *java_class) {
    CodeSnipList code_snips = java_class->typeEntry()->codeSnips();
    foreach(const CodeSnip &cs, code_snips) {
        if (cs.language == TypeSystem::ShellDeclaration) {
            s << cs.code() << endl;
        }
    }
}
