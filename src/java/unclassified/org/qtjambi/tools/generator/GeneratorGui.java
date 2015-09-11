/****************************************************************************
**
** Copyright (C) 1992-2009 Nokia. All rights reserved.
**
** This file is part of Qt Jambi.
**
** ** $BEGIN_LICENSE$
** Commercial Usage
** Licensees holding valid Qt Commercial licenses may use this file in
** accordance with the Qt Commercial License Agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Nokia.
** 
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
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
** If you are unsure which license is appropriate for your use, please
** contact the sales department at qt-sales@nokia.com.
** $END_LICENSE$

**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

package org.qtjambi.tools.generator;

import java.util.Arrays;
import java.util.HashMap;
import java.util.LinkedList;
import java.util.List;

import org.qtjambi.qt.core.QCoreApplication;
import org.qtjambi.qt.core.QFile;
import org.qtjambi.qt.core.QIODevice;
import org.qtjambi.qt.core.QTextStream;

public class GeneratorGui
{
    public static void main(String args[])
    {
        new QCoreApplication(args);

        String default_file = "tests/dummy.h";
        String default_system = "build_all.txt";

        String fileName = "";
        String typesystemFileName = "";
        String out_dir = "..";
        String pp_file = ".preprocessed.tmp";

        String juic_file = "../juic/juic.xml";
        boolean print_stdout = false;

        boolean no_java = false;
        boolean no_cpp_h = false;
        boolean no_cpp_impl = false;
        boolean no_metainfo = false;
        boolean display_help = false;
        boolean dump_object_tree = false;

        for (int i=1; i<args.length; ++i) {
            String arg = args[i];
            if (arg.startsWith("--no-suppress-warnings")) {
                TypeDatabase db = TypeDatabase.instance();
                db.setSuppressWarnings(false);
            } else if (arg.startsWith("--output-directory=")) {
                out_dir = arg.substring(19);
            } else if (arg.startsWith("--print-stdout")) {
                print_stdout = true;
            } else if (arg.startsWith("--debug-level=")) {
                String level = arg.substring(14);
                if (level == "sparse")
                    ReportHandler.setDebugLevel(ReportHandler.DebugLevel.SparseDebug);
                else if (level == "medium")
                    ReportHandler.setDebugLevel(ReportHandler.DebugLevel.MediumDebug);
                else if (level == "full")
                    ReportHandler.setDebugLevel(ReportHandler.DebugLevel.FullDebug);
            } else if (arg.startsWith("--juic-file=")) {
                juic_file = arg.substring(12);
            } else if (arg.startsWith("--no-java")) {
                no_java = true;
            } else if (arg.startsWith("--no-cpp-h")) {
                no_cpp_h = true;
            } else if (arg.startsWith("--no-cpp-impl")) {
                no_cpp_impl = true;
            } else if (arg.startsWith("--no-metainfo")) {
                no_metainfo = true;
            } else if (arg.startsWith("--help") || arg.startsWith("-h") || arg.startsWith("-?")) {
                display_help = true;
            } else if (arg.startsWith("--dump-object-tree")) {
                dump_object_tree = true;
            } else if (arg.startsWith("--rebuild-only")) {
                String classes[] = args[i+1].split(",");

                List<String> classList = Arrays.asList(classes);
                TypeDatabase.instance().setRebuildClasses(classList);
                ++i;
            } else if (arg.startsWith("--test-typeparser")) {
                String s = args[i+1];
                System.out.printf("%s   --->   %s\n",
                       s,
                       TypeParser.parse(s).toString());
                return ;
            } else {
                if (fileName.length() == 0)
                    fileName = args[i];
                else
                    typesystemFileName = args[i];
            }
        }

        if (fileName.length() == 0)
            fileName = default_file;

        if (typesystemFileName.length() == 0)
            typesystemFileName = default_system;

        display_help = display_help || fileName.length() == 0 || typesystemFileName.length() == 0;

        if (display_help) {
            System.out.printf("Usage:\n  %s [options] header-file typesystem-file\n", args[0]);
            System.out.printf("Available options:\n");
            System.out.printf("  --debug-level=[sparse|medium|full]\n" +
                   "  --dump-object-tree                \n" +
                   "  --help, -h or -?                  \n" +
                   "  --juic-file=[name]                \n" +
                   "  --no-cpp-h                        \n" +
                   "  --no-cpp-impl                     \n" +
                   "  --no-java                         \n" +
                   "  --no-metainfo                     \n" +
                   "  --no-suppress-warnings            \n" +
                   "  --output-directory=[dir]          \n" +
                   "  --print-stdout                    \n"
                   );
            return ;
        }

        if (!TypeDatabase.instance().parseFile(typesystemFileName))
            System.err.printf("Cannot parse file: '%s'", typesystemFileName);

        /*if (!Preprocess.preprocess(fileName, pp_file)) {
            System.err.printf("Preprocessor failed on file: '%s'\n", fileName);
            return ;
        }*/

        // Building the code inforamation...
        ReportHandler.setContext("MetaJavaBuilder");
        MetaJavaBuilder builder = new MetaJavaBuilder();
        builder.setFileName(pp_file);
        builder.build();

        if (dump_object_tree) {
            //dumpMetaJavaTree(builder.classes());
            System.out.println("Not implemented: dump_object_tree");
            return ;
        }

        // Code generation
        List<Generator> generators = new LinkedList<Generator>();

        //MetaJavaClassList keep = builder.classes();

        List<String> contexts = new LinkedList<String>();
        if (!no_java) {
            JavaGenerator java_generator = new JavaGenerator();
            generators.add(java_generator);
            contexts.add("JavaGenerator");
        }

        if (!no_cpp_h) {
            generators.add(new CppHeaderGenerator());
            contexts.add("CppHeaderGenerator");
        }
        if (!no_cpp_impl) {
            generators.add(new CppImplGenerator());
            contexts.add("CppImplGenerator");
        }

        MetaInfoGenerator metainfo = null;
        if (!no_metainfo) {
            metainfo = new MetaInfoGenerator();
            generators.add(metainfo);
            contexts.add("MetaInfoGenerator");
        }

        for (int i=0; i<generators.size(); ++i) {
            Generator generator = generators.get(i);
            ReportHandler.setContext(contexts.get(i));

            generator.setOutputDirectory(out_dir);
            generator.setClasses(builder.classes());
            if (print_stdout)
                generator.printClasses();
            else
                generator.generate();
        }

        if (!no_cpp_impl || !no_cpp_h || !no_metainfo) {
            generatePriFile(out_dir, "cpp", builder.classes(),
                            no_metainfo ? "" : metainfo.filenameStub());
        }

        JuicDataGenerator juicg = new JuicDataGenerator();
        juicg.setClasses(builder.classes());
        juicg.setFileName(juic_file);
        juicg.generate();

        System.out.printf("Built %d Java classes\n", builder.classes().size());

        System.out.printf("Done, %d warnings (%d known issues)\n", ReportHandler.warningCount(),
               ReportHandler.suppressedCount());

    }

    static QFile openPriFile(String base_dir, String sub_dir, MetaJavaClass cls)
    {
        String pro_file_name = cls.packageName().replace(".", "_") + "/" + cls.packageName().replace(".", "_") + ".pri";

        if (sub_dir.length() != 0)
            pro_file_name = sub_dir + "/" + pro_file_name;

        if (base_dir.length() != 0)
            pro_file_name = base_dir + "/" + pro_file_name;

        QFile pro_file = new QFile(pro_file_name);
        if (!pro_file.open(QIODevice.OpenModeFlag.WriteOnly)) {
            ReportHandler.warning("failed to open " + pro_file_name + " for writing...");
            return new QFile();
        }

        return pro_file;
    }

    static void generatePriFile(String base_dir, String sub_dir,
            MetaJavaClassList classes,
            String metaInfoStub)
    {
        HashMap<String, QFile> fileHash = new HashMap<String, QFile>();
        String endl = System.getProperty("line.separator");

        for (int i=0; i<classes.size(); ++i) {
            MetaJavaClass cls = classes.at(i);

            if ((cls.typeEntry().codeGeneration() & TypeEntry.CodeGeneration.GenerateCpp.value()) == 0)
                continue;

            QTextStream s = new QTextStream();

            QFile f = fileHash.containsKey(cls.packageName())
                ? fileHash.get(cls.packageName())
                : null;


            if (f == null) {
                f = openPriFile(base_dir, sub_dir, cls);
                fileHash.put(cls.packageName(), f);

                s.setDevice(f);
                if (metaInfoStub.length() != 0) {
                    s.operator_shift_left("HEADERS += $$QTJAMBI_CPP/" + cls.packageName().replace(".", "_") +
                                          "/" + metaInfoStub + ".h" + endl);
                }

                s.operator_shift_left("SOURCES += \\" + endl);
                if (metaInfoStub.length() != 0) {
                    s.operator_shift_left("        " + "$$QTJAMBI_CPP/" + cls.packageName().replace(".",  "_") +
                                          "/"  + metaInfoStub  + ".cpp \\"  + endl);
                }
                s.operator_shift_left("     $$QTJAMBI_CPP/" + cls.packageName().replace(".", "_") +
                                      "/qtjambi_libraryinitializer.cpp \\" + endl);
            } else {
               s.setDevice(f);
            }


            if (!cls.isNamespace() && !cls.isInterface() && !cls.typeEntry().isVariant()) {
                s.operator_shift_left("        " + "$$QTJAMBI_CPP/" + /*cls.packageName().replace(".", "_") +*/
                                      "/qtjambishell_" + cls.name() + ".cpp \\" + endl);
            }
        }

        for (QFile f : fileHash.values()) {
            QTextStream s = new QTextStream(f);
            s.operator_shift_left(endl + "HEADERS += \\" + endl);
        }

        for (int i=0; i<classes.size(); ++i) {
            MetaJavaClass cls = classes.at(i);

            if ((cls.typeEntry().codeGeneration() & TypeEntry.CodeGeneration.GenerateCpp.value()) == 0)
                continue;

            QFile f = fileHash.containsKey(cls.packageName())
                ? fileHash.get(cls.packageName())
                : null;

            QTextStream s = new QTextStream(f);
            boolean shellfile = (cls.generateShellClass()
                || cls.queryFunctions(MetaJavaClass.FunctionQueryOption.Signals.value()).size() > 0)
                && !cls.isNamespace() && !cls.isInterface() && !cls.typeEntry().isVariant();
            if (shellfile) {
                s.operator_shift_left("        $$QTJAMBI_CPP/" + cls.packageName().replace(".", "_") + "/qtjambishell_" +
                                      cls.name() + ".h \\" + endl);
            }
        }

        for (QFile f : fileHash.values()) {
            f.close();
            f.dispose();
        }
    }


}

