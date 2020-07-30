/****************************************************************************
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

#include "wrapper.h"
#include "reporthandler.h"
#include "fileout.h"
#include "typesystem/typedatabase.h"
#include "main.h"
#include "asttoxml.h"
#include "parser/binder.h"
#include "util.h"
#include "rpp/pp-engine-bits.h"

QString Wrapper::include_directory = QString();

void ReportHandler_message_handler(const std::string &str) {
    ReportHandler::warning(fromStdString(str));
}

int Wrapper::defineUndefineStageCurrent = 1;
QList< DefineUndefine > Wrapper::defineUndefineStageOneList;
QList< DefineUndefine > Wrapper::defineUndefineStageTwoList;

void messageHandler(QtMsgType /*type*/, const QMessageLogContext & /*ctxt*/, const QString & /*msg*/){
//    std::cout << qPrintable(msg) << std::endl;
}

Wrapper::Wrapper(int argc, char *argv[]) :
        default_file("targets/qtjambi_masterinclude.h"),
        default_system("targets/build_all.xml"),
        pp_file("preprocessed.tmp"),
        debugCppMode(DEBUGLOG_DEFAULTS) {

    gs = GeneratorSet::getInstance();
    args = parseArguments(argc, argv);
    handleArguments();
    assignVariables();

    //qInstallMessageHandler(&messageHandler);
}

void Wrapper::handleArguments() {
    if (args.contains("no-suppress-warnings")) {
        TypeDatabase *db = TypeDatabase::instance();
        db->setSuppressWarnings(false);
    }

    if (args.contains("include-eclipse-warnings")) {
        TypeDatabase *db = TypeDatabase::instance();
        db->setIncludeEclipseWarnings(true);
    }

    if (args.contains("debug-level")) {
        QString level = args.value("debug-level");
        if (level == "sparse") {
            ReportHandler::setDebugLevel(ReportHandler::SparseDebug);
        } else if (level == "medium") {
            ReportHandler::setDebugLevel(ReportHandler::MediumDebug);
        } else if (level == "full") {
            ReportHandler::setDebugLevel(ReportHandler::FullDebug);
        } else if (level == "types") {
            ReportHandler::setDebugLevel(ReportHandler::TypeDebug);
        }
    }

    if (args.contains("dummy")) {
        FileOut::dummy = true;
    }

    if (args.contains("diff")) {
        FileOut::diff = true;
    }

    if (args.contains("rebuild-only")) {
        QStringList classes = args.value("rebuild-only").split(",", Qt::SkipEmptyParts);
        TypeDatabase::instance()->setRebuildClasses(classes);
    }

    if (args.contains("qt-include-directory"))
        include_directory = args.value("qt-include-directory");

    if (args.contains("qt-doc-directory"))
        gs->docsDirectory = args.value("qt-doc-directory");

    if (args.contains("qt-doc-url")){
        gs->docsUrl = args.value("qt-doc-url");
        if(!gs->docsUrl.endsWith("/"))
            gs->docsUrl += "/";
    }

    if (args.contains("include-paths")) {        // split on path
        QString arg = args.value("include-paths");
#if defined(Q_OS_WIN32)
        QChar pathSeparator(';');
#else
        QChar pathSeparator(':');
#endif
        if(arg.length() > 0) {
            const QChar firstChar = arg.at(0);
            if(firstChar == QChar(':') || firstChar == QChar(';'))
                pathSeparator = firstChar;	// this allows override default trick
        }
        includePathsList = arg.split(pathSeparator, Qt::SkipEmptyParts);
    }

    if (args.contains("input-directory")) {
        QString arg = args.value("input-directory");
#if defined(Q_OS_WIN32)
        QChar pathSeparator(';');
#else
        QChar pathSeparator(':');
#endif
        if(arg.length() > 0) {
            const QChar firstChar = arg.at(0);
            if(firstChar == QChar(':') || firstChar == QChar(';'))
                pathSeparator = firstChar;	// this allows override default trick
        }
        inputDirectoryList = arg.split(pathSeparator, Qt::SkipEmptyParts);
        QString first;
        if(!inputDirectoryList.isEmpty())
            first = inputDirectoryList.first();

        // Resolve process, resolveAbsoluteFilePath(filepath, flags, inputList, inputList2?);
        //  flags = try relative, keep/strip filepath dir, return orig on no match
//        resolve(default_file, 0, inputDirectoryList);
//        resolve(default_system, 0, inputDirectoryList);
        if(!first.isNull()) {
            default_file = QDir(first).absoluteFilePath(default_file);
            default_system = QDir(first).absoluteFilePath(default_system);
        }
    }

    if (args.contains("output-preprocess-file"))
        pp_file = args.value("output-preprocess-file");

    if (args.contains("output-directory"))
        pp_file = QDir(args.value("output-directory")).absoluteFilePath(pp_file);

    if (args.contains("cpp-output-directory"))
        gs->cppOutDir = args.value("cpp-output-directory");

    if (args.contains("java-output-directory"))
        gs->javaOutDir = args.value("java-output-directory");

    if (args.contains("debug-cpp")) {
        const QStringList list = args.value("debug-cpp").split(',');
        for (int i = 0; i < list.size(); i++) {
            QString s = list.at(i);
            if (s.length() == 0)
                continue;
            int space_set_mark = 1;
            int mod = 0;
            if (s.at(0) == QChar('-')) {
                s = s.mid(1);
                space_set_mark = -1;    // space
            } else if(s.at(0) == QChar('=')) {
                s = s.mid(1);
                space_set_mark = 0;     // set
            } else if(s.at(0) == QChar('+')) {
                s = s.mid(1);
                space_set_mark = 1;	// mark
            }
            if (s.compare("all") == 0) {
               mod = ~0;
            } else if (s.compare("off") == 0) {
               mod = 0;
               space_set_mark = 0;	// force set
            } else if (s.compare("include") == 0) {
               mod = DEBUGLOG_INCLUDE_ERRORS|DEBUGLOG_INCLUDE_FULL|DEBUGLOG_INCLUDE_DIRECTIVE;
            } else if (s.compare("include-errors") == 0) {
               mod = DEBUGLOG_INCLUDE_ERRORS;
            } else if (s.compare("include-full") == 0) {
               mod = DEBUGLOG_INCLUDE_FULL;
            } else if (s.compare("include-directive") == 0) {
               mod = DEBUGLOG_INCLUDE_DIRECTIVE;
            } else if (s.compare("def") == 0 || s.compare("define") == 0) {
               mod = DEBUGLOG_DEFINE;
            } else if (s.compare("undef") == 0 || s.compare("undefine") == 0) {
               mod = DEBUGLOG_UNDEF;
            } else if (s.compare("dump") == 0) {
               mod = DEBUGLOG_DUMP_BEFORE|DEBUGLOG_DUMP_MIDDLE|DEBUGLOG_DUMP_AFTER;
            } else if (s.compare("dump-before") == 0) {
               mod = DEBUGLOG_DUMP_BEFORE;
            } else if (s.compare("dump-middle") == 0) {
               mod = DEBUGLOG_DUMP_MIDDLE;
            } else if (s.compare("dump-after") == 0) {
               mod = DEBUGLOG_DUMP_AFTER;
            } else {
               std::cerr << "Invalid debug-cpp value: " << toStdString(s) << " (ignored)" << std::endl;
               continue;        // has the effect of not changing anything
            }
            if(space_set_mark > 0)
                debugCppMode |= mod;
            else if(space_set_mark < 0)
                debugCppMode &= ~mod;
            else
                debugCppMode = mod;
        }
    }
}

void Wrapper::assignVariables() {
    //set file name
    fileName = args.value("arg-1");

    //set typesystem filename
    typesystemFileName = args.value("arg-2");
    if (args.contains("arg-3"))
        displayHelp(gs);

    //set filename to default masterinclude if is empty
    if (fileName.isEmpty())
        fileName = default_file;

    //if type system filename is empty, set it to default system file...
    if (typesystemFileName.isEmpty())
        typesystemFileName = default_system;

    //if filename or typesystem filename is still empty, show help
    if (fileName.isEmpty() || typesystemFileName.isEmpty())
        displayHelp(gs);

    //if generatorset can't read arguments, show help
    if (!gs->readParameters(args))
        displayHelp(gs);
}

int Wrapper::runJambiGenerator() {
    ReportHandler::setContext("Preprocessor");
    rpp::MessageUtil::installMessageHandler(ReportHandler_message_handler);
    Binder::installMessageHandler(ReportHandler_message_handler);

    TypeDatabase* typeDatabase = TypeDatabase::instance();
    typeDatabase->setDefined(&defined);

    printf("Running the Qt Jambi Generator. Please wait while source files are being generated...\n");

    //parse the type system file
    if (!typeDatabase->parseFile(typesystemFileName, inputDirectoryList))
        qFatal("Cannot parse file: '%s'", qPrintable(typesystemFileName));

    //removing file here for theoretical case of wanting to parse two master include files here
    QFile::remove(pp_file);
    QMap<QString, QString> features;
    bool isQtJambiPort = false;
    std::function<void(std::string,std::string)> featureRegistry = [&features, &isQtJambiPort](std::string macro, std::string file){
        QString _macro = QString::fromStdString(macro);
        if(_macro.startsWith("QT_FEATURE_")){
            QString _file = QString::fromStdString(file);
            _macro = _macro.mid(11);
            if(features.contains(_macro) && features[_macro]!=_file){
                qWarning("Feature %s defined twice:\n%s\n%s", qPrintable(_macro), qPrintable(_file), qPrintable(features[_macro]));
            }
            features[_macro] = _file;
        }else if(_macro=="QT_QTJAMBI_PORT"){
            isQtJambiPort = true;
        }
    };
    //preprocess using master include, preprocessed file and command line given include paths, if any
    if (!Preprocess::preprocess(fileName, pp_file, featureRegistry, args.value("phonon-include"), includePathsList, inputDirectoryList, debugCppMode)) {
        fprintf(stderr, "Preprocessor failed on file: '%s'\n", qPrintable(fileName));
        return 1;
    }

    //convert temp preprocessed file to xml
    if (args.contains("ast-to-xml")) {
        astToXML(pp_file);
        return 0;
    }

    if(isQtJambiPort){
        typeDatabase->replaceQThreadType();
    }

    analyzeDependencies(typeDatabase);

    gs->buildModel(features, pp_file);

    if (args.contains("dump-object-tree")) {
        gs->dumpObjectTree();
        return 0;
    }

    printf("%s\n", qPrintable(gs->generate()));

    printf("Done, %d warnings (%d known issues)\n", ReportHandler::warningCount(),
           ReportHandler::suppressedCount());

    return 0;
}

void Wrapper::analyzeDependencies(TypeDatabase* typeDatabase)
{
    const QMap<QString,TypeSystemTypeEntry*>& typeSystemsByQtLibrary = typeDatabase->typeSystemsByQtLibrary();
    for(TypeSystemTypeEntry * entry : typeSystemsByQtLibrary.values()){
        QStringList pathsList;
        pathsList << include_directory << includePathsList;
        for(const QString& path : pathsList){
            QString file(path+"/"+entry->qtLibrary()+"/"+entry->qtLibrary()+"Depends");
            if(!QFileInfo(file).exists()){
                file = path+"/"+entry->qtLibrary()+"Depends";
            }
#ifdef Q_OS_MAC
            if(!QFileInfo(file).exists()){
                file = path+"/" + entry->qtLibrary() + ".framework/Headers/"+entry->qtLibrary()+"Depends";
            }
            if(!QFileInfo(file).exists()){
                file = path+"/../lib/" + entry->qtLibrary() + ".framework/Headers/"+entry->qtLibrary()+"Depends";
            }
#endif
            if(QFileInfo(file).exists()){
                QFile f(file);
                f.open(QIODevice::ReadOnly);
                QTextStream s(&f);
                while(!s.atEnd()){
                    QString line = s.readLine();
                    if(line.startsWith("#include <") && line.endsWith(">")){
                        line.chop(1);
                        line = line.mid(10);
                        QStringList include = line.split("/");
                        if((include.size()==2 && include[0]==include[1]) || include.size()==1){
                            TypeSystemTypeEntry * lib = typeSystemsByQtLibrary[include[0]];
                            if(lib){
                                entry->addRequiredTypeSystem(lib);
                            }else{
                                entry->addRequiredQtLibrary(include[0]);
                            }
                        }
                    }
                }
                break;
            }
        }
    }
}

bool Wrapper::defined(const QString& name){
    for(const DefineUndefine& ddf : defineUndefineStageOneList){
        if(ddf.isSet() && ddf.name()==name){
            return true;
        }
    }
    return false;
}

const QList< DefineUndefine >& Wrapper::getDefineUndefineStageOneList(){
    return defineUndefineStageOneList;
}

void Wrapper::displayHelp(GeneratorSet* generatorSet) {
#if defined(Q_OS_WIN32)
    char path_splitter = ';';
#else
    char path_splitter = ':';
#endif
    printf("Usage:\n  generator [options] header-file typesystem-file\n\n");
    printf("Available options:\n\n");
    printf("General:\n");
    printf("  --debug-level=[types|sparse|medium|full]  \n"
           "  --debug-cpp=[def|undef|include|dump|all]  \n"
           "  --dump-object-tree                        \n"
           "  --help, -h or -?                          \n"
           "  --no-suppress-warnings                    \n"
           "  --include-eclipse-warnings                \n"
           "  --input-directory=[dir]                   \n"
           "  --output-directory=[dir]                  \n"
           "  --cpp-output-directory=[dir]              \n"
           "  --java-output-directory=[dir]             \n"
           "  --output-preprocess-file=[file|path]      \n"
           "  --include-paths=<path>[%c<path>%c...]     \n"
           "  --print-stdout                            \n"
           "  --qt-include-directory=[dir]              \n"
           "  --qtjambi-debug-tools                     \n"
           "  --preproc-stage1                          \n"
           "  --preproc-stage2                          \n"
           "  --target-platform-arm-cpu                 \n"
           "  -Dname[=definition]                       \n"
           "  -Uname                                    \n",
           path_splitter, path_splitter);

    printf("%s", qPrintable(generatorSet->usage()));
    exit(0);
}

void Wrapper::modifyCppDefine(const QString &arg, bool f_set) {
//qDebug() << "modifyCppDefine():" << arg << ((f_set) ? " define" : " undef");
    QStringList list;
    if (arg.length() > 0) {
        const QChar ch = arg.at(0);
        if (ch.isSpace()) {
            list = arg.split(QRegExp("[\\s]+"), Qt::SkipEmptyParts);
        } else if (ch == QChar(',')) {
            list = arg.split(ch, Qt::SkipEmptyParts);
        } else {
            list = QStringList(arg);	// 1 item
        }
    }

    for (int i = 0; i < list.size(); i++) {
        const QString& s = list.at(i);
        int split = s.indexOf("=");
        const QString name(s.left(split));
        QString value("1");
        if (split > 0)
            value = s.mid(split + 1);
//qDebug() << "modifyCppDefine():" << name << " = " << value << ((f_set) ? " define" : " undef");
        if(name.compare("*") == 0) {	// "-U*" has the effect of clearing the list
            if(!f_set) {	// its invalid name so ignore on f_set==true
                if(defineUndefineStageCurrent == 2)
                    defineUndefineStageTwoList.clear();
                else
                    defineUndefineStageOneList.clear();
            }
        } else {
            if(defineUndefineStageCurrent == 2)
                defineUndefineStageTwoList.append(DefineUndefine(name, value, f_set));
            else
                defineUndefineStageOneList.append(DefineUndefine(name, value, f_set));
        }
    }
}

QMap<QString, QString> Wrapper::parseArguments(int argc, char *argv[]) {
    QMap<QString, QString> args;

    int argNum = 0;
    for (int i = 1; i < argc; ++i) {
        QString arg(argv[i]);
        arg = arg.trimmed();

        if (arg.startsWith("--")) {
            if(arg.compare("--preproc-stage1") == 0) {
                setDefineUndefineStage(1);
            } else if(arg.compare("--preproc-stage2") == 0) {
                setDefineUndefineStage(2);
            } else {
                int split = arg.indexOf("=");

                if (split > 0)
                    args[arg.mid(2).left(split-2)] = arg.mid(split + 1).trimmed();
                else
                    args[arg.mid(2)] = QString();
            }
        } else if (arg.startsWith("-D")) {
            modifyCppDefine(arg.mid(2), true);
        } else if (arg.startsWith("-U")) {
            modifyCppDefine(arg.mid(2), false);
        } else if (arg.startsWith("-")) {
            args[arg.mid(1)] = QString();
        } else {
            argNum++;
            args[QString("arg-%1").arg(argNum)] = arg;
        }
    }

    return args;
}
