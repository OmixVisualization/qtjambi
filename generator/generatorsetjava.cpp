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

#include "generatorsetjava.h"
#include "reporthandler.h"

#include "javagenerator.h"
#include "cppheadergenerator.h"
#include "cppimplgenerator.h"
#include "metainfogenerator.h"
#include "classlistgenerator.h"
#include "qdocgenerator.h"
#include "uiconverter.h"
#include "jumptable.h"

#include <QFileInfo>

GeneratorSet *GeneratorSet::getInstance() {
    return new GeneratorSetJava();
}

void dumpMetaJavaTree(const AbstractMetaClassList &classes);

GeneratorSetJava::GeneratorSetJava() :
        no_java(false),
        no_cpp_h(false),
        no_cpp_impl(false),
        no_metainfo(false),
        build_class_list(false),
        build_qdoc_japi(false),
        docs_enabled(false),
        do_ui_convert(false),
        native_jump_table(false),
        doc_dir("../../main/doc/jdoc") {}

QString GeneratorSetJava::usage() {
    QString usage =
        "  --no-java                                 \n"
        "  --no-metainfo                             \n"
        "  --no-cpp-h                                \n"
        "  --no-cpp-impl                             \n"
        "  --convert-to-jui=[.ui-file name]          \n"
        "  --custom-widgets=[file names]             \n";

    return usage;
}

bool GeneratorSetJava::readParameters(const QMap<QString, QString> args) {
    no_java = args.contains("no-java");
    no_cpp_h = args.contains("no-cpp-h");
    no_cpp_impl = args.contains("no-cpp-impl");
    no_metainfo = args.contains("no-metainfo");
    build_class_list = args.contains("build-class-list");
    native_jump_table = args.contains("native-jump-table");
    qtjambi_debug_tools = args.contains("qtjambi-debug-tools");

    if (args.contains("build-qdoc-japi")) {
        no_java = true;
        no_cpp_h = true;
        no_cpp_impl = true;
        no_metainfo = true;
        build_qdoc_japi = true;
    }

    if (args.contains("jdoc-dir")) {
        doc_dir =  args.value("jdoc-dir");
    }

    docs_enabled = args.contains("jdoc-enabled");

    if (args.contains("custom-widgets"))
        custom_widgets = args.value("custom-widgets");

    if (args.contains("convert-to-jui")) {
        ui_file_name = args.value("convert-to-jui");
        do_ui_convert = true;

        if (!QFileInfo(ui_file_name).exists()) {
            printf(".ui file '%s' does not exist\n", qPrintable(ui_file_name));
            return false;
        }
    }
    return GeneratorSet::readParameters(args);
}

void GeneratorSetJava::buildModel(const QString pp_file) {
    builder.setFileName(pp_file);
    if (!outDir.isNull())
        builder.setOutputDirectory(outDir);
    builder.build();
}

void GeneratorSetJava::dumpObjectTree() {
    dumpMetaJavaTree(builder.classes());
}

QString GeneratorSetJava::generate() {

    // Ui conversion...
    if (do_ui_convert) {
        UiConverter converter;
        converter.setClasses(builder.classes());
        converter.convertToJui(ui_file_name, custom_widgets);
        return 0;
    } else if (!custom_widgets.isEmpty()) {
        fprintf(stderr, "NOTE: The --custom-widgets option only has an effect when used with --convert-to-jui");
    }

    // Code generation
    QList<Generator *> generators;
    PriGenerator *priGenerator = new PriGenerator;
    JavaGenerator *java_generator = 0;
    CppHeaderGenerator *cpp_header_generator = 0;
    CppImplGenerator *cpp_impl_generator = 0;
    MetaInfoGenerator *metainfo = 0;
    JumpTablePreprocessor *jumpTablePreprocessor = 0;
    JumpTableGenerator *jumpTableGenerator = 0;

    QStringList contexts;
    if (build_qdoc_japi) {
        generators << new QDocGenerator;
        contexts << "QDocGenerator";
    }

    if (native_jump_table) {
        jumpTablePreprocessor = new JumpTablePreprocessor();
        generators << jumpTablePreprocessor;
        contexts << "JumpTablePreprocessor";
    }

    if (!no_java) {
        java_generator = new JavaGenerator;
        java_generator->setDocumentationDirectory(doc_dir);
        java_generator->setDocumentationEnabled(docs_enabled);
        java_generator->setNativeJumpTable(native_jump_table);
        if (!javaOutDir.isNull())
            java_generator->setJavaOutputDirectory(javaOutDir);
        if (!outDir.isNull())
            java_generator->setLogOutputDirectory(outDir);
        generators << java_generator;

        contexts << "JavaGenerator";
    }

    if (!no_cpp_h) {
        cpp_header_generator = new CppHeaderGenerator(priGenerator);
        if (!cppOutDir.isNull())
            cpp_header_generator->setCppOutputDirectory(cppOutDir);
        generators << cpp_header_generator;
        contexts << "CppHeaderGenerator";
    }

    if (!no_cpp_impl) {
        cpp_impl_generator = new CppImplGenerator(priGenerator);
        cpp_impl_generator->setNativeJumpTable(native_jump_table);
        cpp_impl_generator->setQtJambiDebugTools(qtjambi_debug_tools);
        if (!cppOutDir.isNull())
            cpp_impl_generator->setCppOutputDirectory(cppOutDir);
        generators << cpp_impl_generator;
        contexts << "CppImplGenerator";
    }

    if (native_jump_table) {
        jumpTableGenerator = new JumpTableGenerator(jumpTablePreprocessor, priGenerator);
        generators << jumpTableGenerator;
        contexts << "JumpTableGenerator";
    }

    if (!no_metainfo) {
        metainfo = new MetaInfoGenerator(priGenerator);
        metainfo->setQtJambiDebugTools(qtjambi_debug_tools);
        if (!cppOutDir.isNull())
            metainfo->setCppOutputDirectory(cppOutDir);
        if (!javaOutDir.isNull())
            metainfo->setJavaOutputDirectory(javaOutDir);
        if (!outDir.isNull())
            metainfo->setOutputDirectory(outDir);
        generators << metainfo;
        contexts << "MetaInfoGenerator";
    }

    if (build_class_list) {
        generators << new ClassListGenerator;
        contexts << "ClassListGenerator";
    }

    if (!cppOutDir.isNull())
        priGenerator->setCppOutputDirectory(cppOutDir);
    generators << priGenerator;
    contexts << "PriGenerator";

    for (int i = 0; i < generators.size(); ++i) {
        Generator *generator = generators.at(i);
        ReportHandler::setContext(contexts.at(i));

        if (generator->outputDirectory().isNull())
            generator->setOutputDirectory(outDir);
        generator->setClasses(builder.classes());
        if (printStdout)
            generator->printClasses();
        else
            generator->generate();
    }

    QString res;
    res = QString("Classes in typesystem: %1\n"
                  "Generated:\n"
                  "  - java......: %2 (%3)\n"
                  "  - cpp-impl..: %4 (%5)\n"
                  "  - cpp-h.....: %6 (%7)\n"
                  "  - meta-info.: %8 (%9)\n"
                  "  - pri.......: %10 (%11)\n"
                 )
          .arg(builder.classes().size())
          .arg(java_generator ? java_generator->numGenerated() : 0)
          .arg(java_generator ? java_generator->numGeneratedAndWritten() : 0)
          .arg(cpp_impl_generator ? cpp_impl_generator->numGenerated() : 0)
          .arg(cpp_impl_generator ? cpp_impl_generator->numGeneratedAndWritten() : 0)
          .arg(cpp_header_generator ? cpp_header_generator->numGenerated() : 0)
          .arg(cpp_header_generator ? cpp_header_generator->numGeneratedAndWritten() : 0)
          .arg(metainfo ? metainfo->numGenerated() : 0)
          .arg(metainfo ? metainfo->numGeneratedAndWritten() : 0)
          .arg(priGenerator->numGenerated())
          .arg(priGenerator->numGeneratedAndWritten());

    return res;
}

void dumpMetaJavaAttributes(const AbstractMetaAttributes *attr) {
    if (attr->isNative()) printf(" native");
    if (attr->isAbstract()) printf(" abstract");
    if (attr->isFinalInTargetLang()) printf(" final(java)");
    if (attr->isFinalInCpp()) printf(" final(cpp)");
    if (attr->isStatic()) printf(" static");
    if (attr->isPrivate()) printf(" private");
    if (attr->isProtected()) printf(" protected");
    if (attr->isPublic()) printf(" public");
    if (attr->isFriendly()) printf(" friendly");
}

void dumpMetaJavaType(const AbstractMetaType *type) {
    if (!type) {
        printf("[void]");
    } else {
        printf("[type: %s", qPrintable(type->typeEntry()->qualifiedCppName()));
        if (type->isReference()) printf(" &");
        for(int i=0; i<type->indirections().size(); i++){
            if(type->indirections()[i]){
                printf("*const ");
            }else{
                printf(" *");
            }
        }

        printf(", %s", qPrintable(type->typeEntry()->qualifiedTargetLangName()));

        if (type->isPrimitive()) printf(" primitive");
        if (type->isEnum()) printf(" enum");
        if (type->isQObject()) printf(" q_obj");
        if (type->isNativePointer()) printf(" n_ptr");
        if (type->isTargetLangString()) printf(" java_string");
        if (type->isTargetLangStringRef()) printf(" java_string");
        if (type->isConstant()) printf(" const");
        printf("]");
    }
}

void dumpMetaJavaArgument(const AbstractMetaArgument *arg) {
    printf("        ");
    dumpMetaJavaType(arg->type());
    printf(" %s", qPrintable(arg->argumentName()));
    if (!arg->defaultValueExpression().isEmpty())
        printf(" = %s", qPrintable(arg->defaultValueExpression()));
    printf("\n");
}

void dumpMetaJavaFunction(const AbstractMetaFunction *func) {
    printf("    %s() - ", qPrintable(func->name()));
    dumpMetaJavaType(func->type());
    dumpMetaJavaAttributes(func);

    // Extra attributes...
    if (func->isSignal()) printf(" signal");
    if (func->isSlot()) printf(" slot");
    if (func->isConstant()) printf(" const");

    printf("\n      arguments:\n");
    foreach(AbstractMetaArgument *arg, func->arguments())
        dumpMetaJavaArgument(arg);
}

void dumpMetaJavaClass(const AbstractMetaClass *cls) {
    printf("\nclass: %s, package: %s\n", qPrintable(cls->name().replace("$", ".")), qPrintable(cls->package()));
    if (cls->hasVirtualFunctions())
        printf("    shell based\n");
    printf("  baseclass: %s %s\n", qPrintable(cls->baseClassName()), cls->isQObject() ? "'QObject-type'" : "'not a QObject-type'");
    printf("  interfaces:");
    foreach(AbstractMetaClass *iface, cls->interfaces())
        printf(" %s", qPrintable(iface->name().replace("$", ".")));
    printf("\n");
    printf("  attributes:");
    dumpMetaJavaAttributes(cls);

    printf("\n  functions:\n");
    foreach(const AbstractMetaFunction *func, cls->functions())
        dumpMetaJavaFunction(func);

    //     printf("\n  fields:\n");
    //     foreach (const AbstractMetaField *field, cls->fields())
    //         dumpMetaJavaField(field);

    //     printf("\n  enums:\n");
    //     foreach (const AbstractMetaEnum *e, cls->enums())
    //         dumpMetaJavaEnum(e);
}

void dumpMetaJavaTree(const AbstractMetaClassList &classes) {
    foreach(AbstractMetaClass *cls, classes) {
        dumpMetaJavaClass(cls);
    }
}

