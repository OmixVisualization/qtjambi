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

#include "jumptable.h"
#include "cppimplgenerator.h"
#include "reporthandler.h"
#include "fileout.h"


static QHash<QString, QString> shortNames;
static QHash<char, QString> expandNamesJNI;
static QHash<char, QString> expandNamesJava;

bool JumpTableGenerator::active = false;

static QString simplifyName(const QString &name, const QString &context, const QString &funcName) {
    if (shortNames.size() == 0) {
        shortNames.insert("jboolean", "Z");
        shortNames.insert("jbyte", "B");
        shortNames.insert("jchar", "C");
        shortNames.insert("jshort", "S");
        shortNames.insert("jint", "I");
        shortNames.insert("jlong", "J");
        shortNames.insert("jfloat", "F");
        shortNames.insert("jdouble", "D");
        shortNames.insert("jobject", "L");
        shortNames.insert("void", "V");

        // Because QBool is specialcased in the typesystem to do
        // automatic conversions from between bool and jboolean, we
        // need to handle bool specially here.
        shortNames.insert("bool", "Z");
    }

    QString sn = ((const QHash<QString, QString> &) shortNames).value(name);
    if (sn.isEmpty()) {
        printf("Failed to translate to shortname: %s in %s :: %s\n",
               qPrintable(name),
               qPrintable(context),
               qPrintable(funcName));
    }

    return shortNames.value(name);
}

static QString expandNameJNI(const QChar &c) {
    if (expandNamesJNI.size() == 0) {
        expandNamesJNI.insert('Z', "jboolean");
        expandNamesJNI.insert('B', "jbyte");
        expandNamesJNI.insert('C', "jchar");
        expandNamesJNI.insert('S', "jshort");
        expandNamesJNI.insert('I', "jint");
        expandNamesJNI.insert('J', "jlong");
        expandNamesJNI.insert('F', "jfloat");
        expandNamesJNI.insert('D', "jdouble");
        expandNamesJNI.insert('L', "jobject");
        expandNamesJNI.insert('V', "void");
    }

    QString n = ((const QHash<char, QString> &) expandNamesJNI).value(c.toLatin1());
    if (n.isEmpty())
        printf("Failed to translate to expanded names: %c\n", c.toLatin1());

    return n;
}

static QString expandNameJava(const QChar &c) {
    if (expandNamesJava.size() == 0) {
        expandNamesJava.insert('Z', "boolean");
        expandNamesJava.insert('B', "byte");
        expandNamesJava.insert('C', "char");
        expandNamesJava.insert('S', "short");
        expandNamesJava.insert('I', "int");
        expandNamesJava.insert('J', "long");
        expandNamesJava.insert('F', "float");
        expandNamesJava.insert('D', "double");
        expandNamesJava.insert('L', "Object");
        expandNamesJava.insert('V', "void");
    }

    QString n = ((const QHash<char, QString> &) expandNamesJava).value(c.toLatin1());
    if (n.isEmpty())
        printf("Failed to translate to expanded names: %c\n", c.toLatin1());

    return n;
}


void JumpTablePreprocessor::generate() {
    ReportHandler::setContext("JumpTablePreprocessor");
    foreach(AbstractMetaClass *cl, m_classes) {
        process(cl);
    }
}

void JumpTablePreprocessor::process(AbstractMetaClass *cls) {
    // Skip generate=no classes, such as QFutureIterator
    if (cls->typeEntry()->codeGeneration() != TypeEntry::GenerateAll) {
//         printf("skipping class: %s, generation is : %x vs %x\n",
//                qPrintable(cls->name()),
//                cls->typeEntry()->codeGeneration(),
//                TypeEntry::GenerateAll);
        return;
    }

    QString package = cls->package();

    if (!m_table.contains(package))
        m_table[package] = SignatureTable();


    SignatureTable &signatureList = m_table[package];

    // Native callbacks (all java functions require native callbacks)
    AbstractMetaFunctionList class_funcs = cls->functionsInTargetLang();
    foreach(AbstractMetaFunction *function, class_funcs) {
        if (!function->isEmptyFunction())
            process(function, &signatureList);
    }


    class_funcs = cls->queryFunctions(AbstractMetaClass::NormalFunctions
                                      | AbstractMetaClass::AbstractFunctions
                                      | AbstractMetaClass::NotRemovedFromTargetLang);
    foreach(AbstractMetaFunction *function, class_funcs) {
        if (function->implementingClass() != cls) {
            process(function, &signatureList);
        }
    }
}


QString JumpTablePreprocessor::signature(const AbstractMetaFunction *func) {
    QString signature;
    QString context = func->implementingClass()->name();
    QString functionSignature = func->signature();

    if (func->argumentRemoved(0))
        signature = "V";
    else
        signature = simplifyName(CppImplGenerator::jniReturnName(func), context, functionSignature);

    AbstractMetaArgumentList args = func->arguments();
    foreach(const AbstractMetaArgument *a, args) {
        if (!func->argumentRemoved(a->argumentIndex() + 1)) {
            if (!a->type()->hasNativeId())
                signature += simplifyName(CppImplGenerator::translateType(a->type(), EnumAsInts),
                                          context, functionSignature);
            else
                signature += "J";
        }
    }

    return signature;
}


void JumpTablePreprocessor::process(AbstractMetaFunction *func, SignatureTable *table) {
    if (!func->needsCallThrough())
        return;


    if (func->jumpTableId() >= 0) {
//         printf("%s::%s already has an ID=%d, for declaring=%s, owner=%s\n",
//                qPrintable(func->implementingClass()->name()),
//                qPrintable(func->signature()),
//                func->jumpTableId(),
//                qPrintable(func->declaringClass()->name()),
//                qPrintable(func->ownerClass()->name()));
        return;
    }

    QString sig = signature(func);

    AbstractMetaFunctionList &list = (*table)[sig];
    list.append(func);
    func->setJumpTableId(list.size());
}


JumpTableGenerator::JumpTableGenerator(JumpTablePreprocessor *pp, PriGenerator *pri)
        : m_preprocessor(pp),
        m_prigenerator(pri) {
    active = true;
}


void JumpTableGenerator::generate() {
    for (PackageJumpTable::const_iterator it = m_preprocessor->table()->constBegin();
            it != m_preprocessor->table()->constEnd(); ++it) {
        QString package = it.key();
        generatePackage(package, it.value());
    }
}


void JumpTableGenerator::generatePackage(const QString &packageName, const SignatureTable &table) {
    generateNativeTable(packageName, table);
    generateJavaTable(packageName, table);
}


void JumpTableGenerator::generateJavaTable(const QString &packageName,
        const SignatureTable &table) {
    QString tableFile = QString("%1/%2/JTbl.java")
                        .arg(outputDirectory())
                        .arg(QString(packageName).replace(".", "/"));

    printf("Generating jump table (.java): %s\n", qPrintable(tableFile));

    FileOut file(tableFile);

    QTextStream &s = file.stream;

    s << "package " << packageName << ";" << endl << endl;
    s << "class JTbl {" << endl;


    for (SignatureTable::const_iterator sit = table.constBegin(); sit != table.constEnd(); ++sit) {
        QString signature = sit.key();

        QString ret = expandNameJava(signature.at(0));

        s << "    static native " << ret << " " << signature << "(int id, long nid";

        for (int i = 1; i < signature.size(); ++i) {
            s << ", " << expandNameJava(signature.at(i)) << " a" << i;
        }

        s << ", Object _this);" << endl;
    }

    s << "}" << endl;
}


void JumpTableGenerator::generateNativeTable(const QString &packageName,
        const SignatureTable &table) {
    QString tableFile = QString("%1/%2/nativejumptable.cpp")
                        .arg(outputDirectory())
                        .arg(CppGenerator::subDirectoryForPackage(packageName));

    FileOut file(tableFile);

    QString pkgSubDir = QString(packageName).replace(".", "_");
    m_prigenerator->addSource(pkgSubDir + "/" + pkgSubDir + ".pri", "nativejumptable.cpp");

    printf("Generating jump table (.cpp): %s\n", qPrintable(tableFile));

    QTextStream &s = file.stream;

    s << "#include <qtjambi/qtjambi_global.h>" << endl;

    for (SignatureTable::const_iterator sit = table.constBegin(); sit != table.constEnd(); ++sit) {
        QString signature = sit.key();

        QString ret = expandNameJNI(signature.at(0));

        s << endl << endl
        << "extern \"C\" Q_DECL_EXPORT " << ret << " JNICALL QTJAMBI_FUNCTION_PREFIX(Java_"
        << QString(packageName).replace("_", "_1").replace(".", "_") << "_JTbl_" << signature << ")" << endl
        << "(JNIEnv *e, jclass, jint id, jlong nid";

        for (int i = 1; i < signature.size(); ++i) {
            s << ", " << expandNameJNI(signature.at(i)) << " a" << i;
        }

        s << ", jobject __this)" << endl
        << "{" << endl
        << "Q_UNUSED(__this);" << endl
        << "Q_UNUSED(nid);" << endl
        << "switch (id) { " << endl;

        AbstractMetaFunctionList functions = sit.value();
        bool hasReturn = signature.at(0) != 'V';

        foreach(AbstractMetaFunction *f, functions) {
            const AbstractMetaClass *cls = f->ownerClass();
            s << endl
            << "// " << cls->name() << "::" << f->signature() << ", declaring=" << f->declaringClass()->name() << ", implementing=" << f->implementingClass()->name() << endl
            << "case " << f->jumpTableId() << ":" << endl
            << "extern ";
            CppImplGenerator::writeFunctionName(s, f, cls, CppImplGenerator::ReturnType);
            s << endl;
            CppImplGenerator::writeFinalFunctionArguments(s, f, "object");
            s << ";" << endl;

            if (hasReturn && !f->isConstructor())
                s << "return ";

            CppImplGenerator::writeFunctionName(s, f, cls, 0);

            s << "(e";

            if (f->isStatic())
                s << ", 0";
            else if (f->isConstructor())
                s << ", __this";
            else
                s << ", __this, nid";

            for (int i = 1; i < signature.size(); ++i) {
                s << ", a" << i;
            }

            s << ");" << endl
            << "break;" << endl;
        }

        s << "} // switch..." << endl;

        if (hasReturn)
            s << "return 0;" << endl;

        s << "} // " << signature << endl;
    }
}

bool JumpTableGenerator::isJumpTableActive() {
    return active;
}
