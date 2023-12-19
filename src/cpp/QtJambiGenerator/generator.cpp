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

#include "generator.h"
#include "reporthandler.h"
#include "bufferedoutputstream.h"
#include "typesystem/typedatabase.h"
#include "typesystem/qmltypesystemreader.h"
#include "asttoxml.h"
#include "parser/binder.h"
#include "util.h"
#include "rpp/pp-engine-bits.h"
#include <QtConcurrent>
#include "docindex/docindexreader.h"
#include "preprocesshandler.h"
#include "reporthandler.h"

#include "ast.h"
#include "binder.h"
#include "control.h"
#include "default_visitor.h"
#include "dumptree.h"
#include "lexer.h"
#include "parser.h"
#include "tokens.h"

#include "javagenerator.h"
#include "cppheadergenerator.h"
#include "cppimplgenerator.h"
#include "metainfogenerator.h"
#include <jni.h>
#include <QtCore/QCommandLineOption>

Q_DECL_EXPORT int execute_generator(int argc, char *argv[]) {
    GeneratorApplication app(argc, argv);
    app.parseArguments();
    printf("Running QtJambi Generator. Please wait while files are being generated...\n");
    return app.generate();
}

/* Win64 ABI does not use underscore prefixes symbols we could also use !defined(__MINGW64__) */
#if defined(Q_CC_MINGW) && !defined(_WIN64)
#  define QTJAMBI_FUNCTION_PREFIX(name) _##name
#else
#  define QTJAMBI_FUNCTION_PREFIX(name) name
#endif

extern "C" Q_DECL_EXPORT int JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_qtjambi_generator_Main_invoke)
(JNIEnv * env,
 jclass,
 jobjectArray args)
{
    if(args){
        QVector<QByteArray> data;
        jsize length = env->GetArrayLength(args)+1;
        data.resize(length);
        char** argv = new char*[length];
        for(jsize i=0; i<length; ++i){
            if(i==0){
                data[i] = QByteArray("QtJambiGenerator");
            }else{
                jstring strg = jstring(env->GetObjectArrayElement(args, i-1));
                const char* chars = env->GetStringUTFChars(strg, nullptr);
                data[i] = chars;
                env->ReleaseStringUTFChars(strg, chars);
            }
            argv[i] = data[i].data();
        }
        return execute_generator(int(length), argv);
    }else{
        QByteArray args("QtJambiGenerator");
        char* argv = args.data();
        return execute_generator(1, &argv);
    }
}

extern "C" Q_DECL_EXPORT jlong JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_qtjambi_generator_GeneratorApplication_initialize)
(JNIEnv *,jclass)
{
    static int argc = 1;
    static char *argv = const_cast<char*>("QtJambiGenerator");
    void* ptr = new GeneratorApplication(argc, &argv);
    return jlong(ptr);
}

using namespace TS;

void ReportHandler_message_handler(const std::string &str) {
    ReportHandler::warning(fromStdString(str));
}

void messageHandler(QtMsgType type, const QMessageLogContext & /*ctxt*/, const QString & msg){
//    std::cout << qPrintable(msg) << std::endl;
    switch(type){
    case QtMsgType::QtWarningMsg:
    case QtMsgType::QtCriticalMsg:
    case QtMsgType::QtFatalMsg:
        if(!msg.startsWith(QStringLiteral("Trying to construct an instance of an invalid type, type id:")))
            std::cerr << qPrintable(msg) << std::endl;
        break;
    default: break;
    }
}

#ifndef QTJAMBI_PATCH_VERSION
#define QTJAMBI_PATCH_VERSION 0
#endif

GeneratorApplication::GeneratorApplication(int& argc, char *argv[]) :
        QCoreApplication(argc, argv),
        m_preProcessorFileName(),
        m_debugCppMode(DEBUGLOG_DEFAULTS),
        m_inputDirectory("."),
        m_outputDirectory("generatorout"),
        m_printStdout(false),
        m_docsUrl("@docRoot/"),
        m_qtVersionMajor(QT_VERSION_MAJOR),
        m_qtVersionMinor(QT_VERSION_MINOR),
        m_qtVersionPatch(QT_VERSION_PATCH),
        m_qtjambiVersionPatch(QTJAMBI_PATCH_VERSION),
        m_staticLibraries(),
        m_noJava(false),
        m_noCppHeaders(false),
        m_noCppImpl(false),
        m_noKotlinDelegates(false),
        m_noMetainfo(false),
        m_nullness(true),
        m_astToXml(false),
        m_dumpObjectTree(false),
        m_database(*TypeDatabase::instance()),
        m_metaBuilder(&m_database)
        {
    QCoreApplication::setOrganizationDomain(QStringLiteral("http://www.qtjambi.io"));
    QCoreApplication::setApplicationName(QStringLiteral("QtJambi Generator"));
    QCoreApplication::setApplicationVersion(QVersionNumber(m_qtVersionMajor, m_qtVersionMinor, m_qtjambiVersionPatch).toString());
    qInstallMessageHandler(messageHandler);
}

void GeneratorApplication::parseArguments(){
    QCommandLineParser parser;
    QCommandLineOption defineOption(QStringLiteral("D"), QStringLiteral("preprocessor defines"), QStringLiteral("define"));
    defineOption.setFlags(QCommandLineOption::ShortOptionStyle);
    parser.addOption(defineOption);
    QCommandLineOption undefineOption("U", "preprocessor blocked defines", "define");
    undefineOption.setFlags(QCommandLineOption::ShortOptionStyle);
    parser.addOption(undefineOption);
    QCommandLineOption astOption("ast-to-xml", "write abstract syntax tree to XML");
    astOption.setFlags(QCommandLineOption::HiddenFromHelp);
    parser.addOption(astOption);
    QCommandLineOption dumpOption("dump-object-tree", "dump");
    dumpOption.setFlags(QCommandLineOption::HiddenFromHelp);
    parser.addOption(dumpOption);
    QCommandLineOption noSuppressWarningsOption("no-suppress-warnings", "don't suppress warnings");
    parser.addOption(noSuppressWarningsOption);
    QCommandLineOption noNativeIDsOption("no-native-ids", "don't generate code using native ids infavor to object references");
    noNativeIDsOption.setFlags(QCommandLineOption::HiddenFromHelp);
    parser.addOption(noNativeIDsOption);
    QCommandLineOption noNullnessOption("no-nullness", "don't generate nullness annotation");
    parser.addOption(noNullnessOption);
    //QCommandLineOption noEclipseWarningsOption("no-java-suppress-warnings", "don't generate java code with eclipse warnings");
    //parser.addOption(noEclipseWarningsOption);
    QCommandLineOption debugLevelOption("debug-level", "level of debugging output [sparse|medium|full|types]", "level");
    parser.addOption(debugLevelOption);
    QCommandLineOption rebuildOnlyOption("rebuild-only", "list of classes", "classes");
    parser.addOption(rebuildOnlyOption);
    QCommandLineOption includeDirectoryOption({"I", "include-directories"}, "list of directories to find included files", "directories");
    includeDirectoryOption.setFlags(QCommandLineOption::ShortOptionStyle);
    parser.addOption(includeDirectoryOption);
    QCommandLineOption typesystemDirectoryOption({"T", "typesystem-directories"}, "list of directories to find typesystems", "directories");
    typesystemDirectoryOption.setFlags(QCommandLineOption::ShortOptionStyle);
    parser.addOption(typesystemDirectoryOption);
    QCommandLineOption qtDocDirectoryOption("qt-doc-directory", "directory of Qt documentation", "directory");
    parser.addOption(qtDocDirectoryOption);
    QCommandLineOption qtDocUrlOption("qt-doc-url", "", "url");
    qtDocUrlOption.setFlags(QCommandLineOption::HiddenFromHelp);
    parser.addOption(qtDocUrlOption);
    QCommandLineOption importDirectoryOption("import-directories", "list of directories to find typesystem imports", "directories");
    parser.addOption(importDirectoryOption);
    QCommandLineOption outputDirectoryOption("output-directory", "output directory", "directory");
    parser.addOption(outputDirectoryOption);
    QCommandLineOption ppfileNameOption("output-preprocess-file", "name of preprocess file", "name");
    parser.addOption(ppfileNameOption);
    QCommandLineOption javaOutputDirectoryOption("java-output-directory", "java output directory", "directory");
    parser.addOption(javaOutputDirectoryOption);
    QCommandLineOption cppOutputDirectoryOption("cpp-output-directory", "c++ output directory", "directory");
    parser.addOption(cppOutputDirectoryOption);
    QCommandLineOption noJavaOption("no-java", "don't generate java code");
    parser.addOption(noJavaOption);
    QCommandLineOption noCHOption("no-cpp-h", "don't generate c++ headers");
    parser.addOption(noCHOption);
    QCommandLineOption noCPOption("no-cpp-impl", "don't generate c++ implementation code");
    parser.addOption(noCPOption);
    QCommandLineOption noKotlinDelegatesOption("no-kotlin-property-delegates", "don't generate Kotlin-compatible property delegates");
    parser.addOption(noKotlinDelegatesOption);
    QCommandLineOption noMetaInfoOption("no-metainfo", "don't generate c++ metainfo code");
    parser.addOption(noMetaInfoOption);
    QCommandLineOption staticlibsOption("static", "comma-separated list of statically linked libraries", "libraries");
    parser.addOption(staticlibsOption);
    QCommandLineOption debugCppOption("debug-cpp", "debug flags", "flags");
    parser.addOption(debugCppOption);
    QCommandLineOption qtjambiVersionOption("qtjambi-version", "QtJambi patch version number", "version");
    parser.addOption(qtjambiVersionOption);
    QCommandLineOption printoutOption("print-stdout", "print to standard out");
    parser.addOption(printoutOption);
    QCommandLineOption typesystemOption({"t","typesystem"}, "typesystem specified in QML", "typesystemfile");
    typesystemOption.setFlags(QCommandLineOption::ShortOptionStyle);
    parser.addOption(typesystemOption);
    QCommandLineOption qmlOption(QStringLiteral("generate-typesystem"), QStringLiteral("generate new typesystem specification based upon all found types not covered by given typesystem specifications."), QStringLiteral("filename"));
    //parser.addOption(qmlOption);
    parser.addPositionalArgument(QStringLiteral("headerfile"), QStringLiteral("header file containing definitions for all native types to be generated"));
    parser.addHelpOption();
    parser.addVersionOption();
    parser.setApplicationDescription(QStringLiteral("QtJambi Generator %1 - Generates Java bindings for Qt's C++ API.").arg(QVersionNumber(m_qtVersionMajor, m_qtVersionMinor, m_qtjambiVersionPatch).toString()));
    qDebug("args: %s", qPrintable(QCoreApplication::arguments().join(", ")));

    QFunctionPointer unexit = nullptr;
    {
        QJsonValue iid("io.qtjambi.PluginImporter");
        for(QStaticPlugin sp : QPluginLoader::staticPlugins()){
            if(sp.metaData()["IID"]==iid && sp.instance){
                if(QObject* jarimportPlugin = sp.instance()){
                    QFunctionPointer preExit = QFunctionPointer(jarimportPlugin->qt_metacast("CoreAPI::preExit"));
                    unexit = QFunctionPointer(jarimportPlugin->qt_metacast("CoreAPI::unexit"));
                    if(preExit && unexit)
                        preExit();
                }
            }
        }
    }
    if(unexit){
        auto _qtjambi_unexit = qScopeGuard(unexit);
        parser.process(*this);
    }else{
        parser.process(*this);
    }
    //parser.addPositionalArgument("", "");
    if(!parser.unknownOptionNames().isEmpty()){
        parser.showHelp();
        return;
    }

    for(const QString& lib : parser.values(staticlibsOption)){
        m_staticLibraries << lib.split(',');
    }
    for(QString s : parser.values(debugCppOption)){
        if (s.isEmpty())
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
            m_debugCppMode |= mod;
        else if(space_set_mark < 0)
            m_debugCppMode &= ~mod;
        else
            m_debugCppMode = mod;
    }
    for(const QString& def : parser.values(defineOption)){
        modifyCppDefine(def, true);
    }
    for(const QString& def : parser.values(undefineOption)){
        modifyCppDefine(def, false);
    }
    m_astToXml = parser.isSet(astOption);
    m_dumpObjectTree = parser.isSet(dumpOption);
    if (parser.isSet(noSuppressWarningsOption))
        m_database.setSuppressWarnings(false);
    if(parser.isSet(noNativeIDsOption))
        setUseNativeIds(false);
    //if(parser.isSet(noEclipseWarningsOption))
    //    m_database.setIncludeEclipseWarnings(false);

    {
        QString level = parser.value(debugLevelOption);
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

    {
        QStringList classes;
        for(const QString& v : parser.values(rebuildOnlyOption)){
            classes << v.split(",", Qt::SkipEmptyParts);
        }
        m_database.setRebuildClasses(classes);
    }

#ifdef Q_OS_WINDOWS
#define PATHSEP ";"
#else
#define PATHSEP ":"
#endif
    for(const QString& v : parser.values(includeDirectoryOption)){
        m_includeDirectories << v.split(PATHSEP, Qt::SkipEmptyParts);
    }
    if(m_includeDirectories.isEmpty()){
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
        m_includeDirectories << QLibraryInfo::location(QLibraryInfo::HeadersPath);
#else
        m_includeDirectories << QLibraryInfo::path(QLibraryInfo::HeadersPath);
#endif
        printf("No include paths specified. Taking Qt's headers path instead: %s\n", qPrintable(m_includeDirectories[0]));
    }
    for(const QString& v : parser.values(typesystemDirectoryOption)){
        m_typesystemDirectories << v.split(PATHSEP, Qt::SkipEmptyParts);
    }
    m_docsDirectory = parser.value(qtDocDirectoryOption);

    QString v = parser.value(qtDocUrlOption);
    if (!v.isEmpty()){
        m_docsUrl = v;
        if(!m_docsUrl.endsWith("/"))
            m_docsUrl += "/";
    }
    for(const QString& v : parser.values(importDirectoryOption)){
        m_importDirectories << v.split(PATHSEP, Qt::SkipEmptyParts);
    }

    v = parser.value(outputDirectoryOption);
    if(!v.isEmpty())
        m_outputDirectory = v;
    QDir _outDir = m_outputDirectory.isEmpty() ? QDir::current() : QDir(m_outputDirectory);
    _outDir.mkpath(".");
    v = parser.value(ppfileNameOption);
    if (!v.isEmpty()){
        m_preProcessorFileName = v;
        if(!m_preProcessorFileName.contains('/') && !m_preProcessorFileName.contains('\\')){
            m_preProcessorFileName = _outDir.absoluteFilePath(m_preProcessorFileName);
        }else if(!QFileInfo(m_preProcessorFileName).isAbsolute()){
            m_preProcessorFileName = QFileInfo(m_preProcessorFileName).absoluteFilePath();
        }
    }
    /*
    if(parser.isSet(qmlOption)){
        m_generateTypeSystemQML = parser.value(qmlOption);
        if(!QFileInfo(m_generateTypeSystemQML).isAbsolute()){
            m_generateTypeSystemQML = _outDir.absoluteFilePath(m_generateTypeSystemQML);
        }
    }
    */
    v = parser.value(javaOutputDirectoryOption);
    if(v.isEmpty())
        m_javaOutputDirectory = _outDir.absoluteFilePath("java");
    else{
        m_javaOutputDirectory = v;
        if(!QFileInfo::exists(m_javaOutputDirectory) && !QFileInfo(m_javaOutputDirectory).isAbsolute()){
            m_javaOutputDirectory = _outDir.absoluteFilePath(m_javaOutputDirectory);
        }
    }
    v = parser.value(cppOutputDirectoryOption);
    if(v.isEmpty())
        m_cppOutputDirectory = _outDir.absoluteFilePath("cpp");
    else{
        m_cppOutputDirectory = v;
        if(!QFileInfo::exists(m_cppOutputDirectory) && !QFileInfo(m_cppOutputDirectory).isAbsolute()){
            m_cppOutputDirectory = _outDir.absoluteFilePath(m_cppOutputDirectory);
        }
    }

    m_noJava = parser.isSet(noJavaOption);
    m_noCppHeaders = parser.isSet(noCHOption);
    m_noCppImpl = parser.isSet(noCPOption);
    m_noKotlinDelegates = parser.isSet(noKotlinDelegatesOption);
    m_noMetainfo = parser.isSet(noMetaInfoOption);
    m_nullness = !parser.isSet(noNullnessOption);

    if(parser.isSet(qtjambiVersionOption)){
        v = parser.value(qtjambiVersionOption);
        bool ok = false;
        m_qtjambiVersionPatch = v.toUInt(&ok);
        if(!ok)
            qFatal("Unable to parse number: %s", qPrintable(v));
    }
    const QString default_system = QStringLiteral(":/io/qtjambi/generator/targets/all.qml");
    if(parser.isSet(typesystemOption)){
        m_typesystemFileName = parser.value(typesystemOption);
        QFileInfo _typesystemFileName(m_typesystemFileName);
        if(_typesystemFileName.exists()){
            m_typesystemFileName = _typesystemFileName.absoluteFilePath();
        }
    }else{
        printf("No typesystem file specified. Taking default QtJambi typesystem.\n");
        m_typesystemFileName = default_system;
    }

    const QString default_file = QStringLiteral(":/io/qtjambi/generator/targets/all.h");
    QStringList positionalArguments = parser.positionalArguments();
    if(positionalArguments.size()>0){
        m_headerFileName = positionalArguments.takeFirst();
        QFileInfo _fileName(m_headerFileName);
        if(_fileName.exists()){
            m_headerFileName = _fileName.absoluteFilePath();
        }
    }else{
        printf("No header file specified. Taking default header file to generate QtJambi sources.\n");
        m_headerFileName = default_file;
    }
    m_printStdout = parser.isSet(printoutOption);
}

void dumpMetaJavaClass(const MetaClass *cls);

int GeneratorApplication::generate() {
    try{
        //parse the type system file
        QSemaphore versionAvailable;
        QSemaphore docDirectoryAvailable;
        QFuture<void> typeSystemFuture;
        QFuture<const DocModel*> docModelFuture;
        if (!m_astToXml) {
            typeSystemFuture = QtConcurrent::run([this,&versionAvailable,&docDirectoryAvailable](){
                versionAvailable.acquire(3);
                if(m_docsDirectory.isEmpty()){
                    if(m_qtVersionMajor==QT_VERSION_MAJOR && m_qtVersionMinor==QT_VERSION_MINOR){
        #if QT_VERSION < QT_VERSION_CHECK(6,0,0)
                        m_docsDirectory = QLibraryInfo::location(QLibraryInfo::DocumentationPath);
        #else
                        m_docsDirectory = QLibraryInfo::path(QLibraryInfo::DocumentationPath);
        #endif
                        printf("No docs directory specified. Taking Qt's documentation path instead: %s\n", qPrintable(m_docsDirectory));
                    }
                    docDirectoryAvailable.release();
                }
                ReportHandler::setContext("Typesystem");
                m_database.setDefined([this](const QString& name)->bool{
                    for(const DefineUndefine& ddf : m_defineUndefineList){
                        if(ddf.isSet() && ddf.name()==name){
                            return true;
                        }
                    }
                    return false;
                });
                m_database.initialize(m_typesystemFileName, m_importDirectories, m_typesystemDirectories, QVersionNumber(m_qtVersionMajor, m_qtVersionMinor, m_qtVersionPatch), m_generateTypeSystemQML.isEmpty());
                analyzeDependencies();
            });
            if(!m_dumpObjectTree && m_generateTypeSystemQML.isEmpty()){
                QThread* targetThread = QThread::currentThread();
                if(m_docsDirectory.isEmpty()){
                    docModelFuture = QtConcurrent::run([&docDirectoryAvailable,targetThread,this]() -> const DocModel* {
                        docDirectoryAvailable.acquire();
                        QDir docsDirectory(m_docsDirectory);
                        if(docsDirectory.exists()){
                            DocIndexReader reader;
                            return reader.readDocIndexes(docsDirectory, targetThread);
                        }else return nullptr;
                    });
                }else{
                    docModelFuture = QtConcurrent::run([this, targetThread]() -> const DocModel* {
                        QDir docsDirectory(m_docsDirectory);
                        if(docsDirectory.exists()){
                            DocIndexReader reader;
                            return reader.readDocIndexes(docsDirectory, targetThread);
                        }else return nullptr;
                    });
                }
            }
        }

        ReportHandler::setContext("Preprocessor");
        rpp::MessageUtil::installMessageHandler(ReportHandler_message_handler);
        Binder::installMessageHandler(ReportHandler_message_handler);

        //removing file here for theoretical case of wanting to parse two master include files here
        if(QFileInfo::exists(m_preProcessorFileName))
            QFile::remove(m_preProcessorFileName);
        QMap<QString, QString> features;
        QSet<QString> undefFeatures;

        std::function<void(std::string,std::string,const QFileInfo&,bool)> featureRegistry = [&features, &undefFeatures, &versionAvailable, this](std::string macro, std::string definition, const QFileInfo& file, bool isDefine){
            QString _macro = QString::fromStdString(macro);
            if(isDefine){
                if(_macro.startsWith("QT_FEATURE_")){
                    QString _file = file.absoluteFilePath();
                    _macro = _macro.mid(11);
                    if(features.contains(_macro) && features[_macro]!=_file){
                        if(!undefFeatures.remove(_macro)){
                            QFileInfo file1(features[_macro]);
                            QFileInfo file2(_file);
                            QString filename = file1.fileName();
                            if(!file2.absoluteFilePath().endsWith("/private/"+filename.replace(".h", "_p.h")))
                                qWarning("Feature %s defined twice:\n%s\n%s", qPrintable(_macro), qPrintable(_file), qPrintable(features[_macro]));
                        }
                    }else{
                        features[_macro] = _file;
                    }
                }else if(_macro=="QT_VERSION_MAJOR"){
                    QString _definition = QString::fromStdString(definition);
                    bool ok = false;
                    uint v = _definition.toUInt(&ok);
                    if(ok){
                        m_qtVersionMajor = v;
                        versionAvailable.release();
                    }
                }else if(_macro=="QT_VERSION_MINOR"){
                    QString _definition = QString::fromStdString(definition);
                    bool ok = false;
                    uint v = _definition.toUInt(&ok);
                    if(ok){
                        m_qtVersionMinor = v;
                        versionAvailable.release();
                    }
                }else if(_macro=="QT_VERSION_PATCH"){
                    QString _definition = QString::fromStdString(definition);
                    bool ok = false;
                    uint v = _definition.toUInt(&ok);
                    if(ok){
                        m_qtVersionPatch = v;
                        versionAvailable.release();
                    }
                }
            }else{
                if(_macro.startsWith("QT_FEATURE_")){
                    //QString _file = file.absoluteFilePath();
                    _macro = _macro.mid(11);
                    undefFeatures.insert(_macro);
                }
            }
        };

        //preprocess using master include, preprocessed file and command line given include paths, if any
        QByteArray preprocessorOutput;
        {
            QBuffer buffer(&preprocessorOutput);
            buffer.open(QIODevice::Append | QIODevice::Text);
            bool result = preprocess(buffer, featureRegistry);
            buffer.close();
            if(!m_preProcessorFileName.isEmpty()){
                QFile f(m_preProcessorFileName);
                if (!f.open(QIODevice::WriteOnly)) {
                    std::fprintf(stderr, "Failed to write preprocessed file: %s\n", qPrintable(m_preProcessorFileName));
                }
                f.write(preprocessorOutput);
            }
            if (!result) {
                fprintf(stderr, "Preprocessor failed on file: '%s'\n", qPrintable(m_headerFileName));
                return 1;
            }
        }
        versionAvailable.release(3);

        try{
            typeSystemFuture.waitForFinished();
        }catch(const std::exception& exn){
            fprintf(stderr, "%s\n", exn.what());
            return -1;
        }catch(...){
            fprintf(stderr, "An error has occurred\n");
            return -1;
        }
        typeSystemFuture = {};

        QMap<QString,QStringList> requiredFeatures;
        FileModelItem dom;
        {
            ReportHandler::setContext("Parser");
            Control control;
            Parser p(&control, requiredFeatures);
            pool __pool;
            TranslationUnitAST *ast = p.parse(preprocessorOutput, std::size_t(preprocessorOutput.size()), &__pool);
            CodeModelPtr model{new CodeModel};
            Binder binder(m_qtVersionMajor, m_qtVersionMinor, m_qtVersionPatch, m_database, model, p.location());
            binder.run(ast);
            preprocessorOutput.clear();
            dom = model->globalNamespace();
        }

        //convert temp preprocessed file to xml
        if (m_astToXml) {
            astToXML(QDir(m_outputDirectory).filePath("ast.xml"), dom);
            dom.reset();
            return 0;
        }else{
            m_metaBuilder.setRequiredFeatures(requiredFeatures);
            m_metaBuilder.setIncludePathsList(m_includeDirectories);
            m_metaBuilder.setQtVersion(m_qtVersionMajor, m_qtVersionMinor, m_qtVersionPatch, m_qtjambiVersionPatch);
            if (!m_outputDirectory.isNull())
                m_metaBuilder.setOutputDirectory(m_outputDirectory);
            m_metaBuilder.setFeatures(features);
            m_metaBuilder.setGenerateTypeSystemQML(m_generateTypeSystemQML);
            if(!m_generateTypeSystemQML.isEmpty()){
                TypeDatabase::instance()->addType(new TypeSystemTypeEntry("generic"));
            }

            m_metaBuilder.build(std::move(dom));

            if (m_dumpObjectTree) {
                for(MetaClass *cls : m_metaBuilder.classes()) {
                    dumpMetaJavaClass(cls);
                }
                return 0;
            }

            const DocModel* docModel = docModelFuture.result();
            docModelFuture = {};
            if(docModel){
                m_metaBuilder.applyDocs(docModel);
                if(!docModel->url().isEmpty()){
                    this->m_docsUrl = docModel->url();
                    if(!this->m_docsUrl.endsWith("/"))
                        this->m_docsUrl += "/";
                }
                delete docModel;
            }


            // Code generation
            QList<AbstractGenerator *> generators;
            PriGenerator *priGenerator = new PriGenerator(&m_database);
            JavaGenerator *java_generator = nullptr;
            CppHeaderGenerator *cpp_header_generator = nullptr;
            CppImplGenerator *cpp_impl_generator = nullptr;
            MetaInfoGenerator *metainfo = nullptr;

            QStringList contexts;
            if (!m_noJava) {
                java_generator = new JavaGenerator(&m_database, m_nullness, m_noKotlinDelegates);
                java_generator->setTypeSystemByPackage(m_metaBuilder.typeSystemByPackage());
                if (!m_javaOutputDirectory.isNull())
                    java_generator->setJavaOutputDirectory(m_javaOutputDirectory);
                if (!m_outputDirectory.isNull())
                    java_generator->setLogOutputDirectory(m_outputDirectory);
                java_generator->setDocsUrl(m_docsUrl);
                generators << java_generator;

                contexts << "JavaGenerator";
            }

            if (!m_noCppHeaders) {
                cpp_header_generator = new CppHeaderGenerator(priGenerator);
                if (!m_cppOutputDirectory.isNull())
                    cpp_header_generator->setCppOutputDirectory(m_cppOutputDirectory);
                generators << cpp_header_generator;
                contexts << "CppHeaderGenerator";
            }

            if (!m_noCppImpl) {
                cpp_impl_generator = new CppImplGenerator(priGenerator);
                if (!m_cppOutputDirectory.isNull())
                    cpp_impl_generator->setCppOutputDirectory(m_cppOutputDirectory);
                generators << cpp_impl_generator;
                contexts << "CppImplGenerator";
            }

            if (!m_noMetainfo) {
                metainfo = new MetaInfoGenerator(priGenerator);
                metainfo->setStaticLibraries(m_staticLibraries);
                if (!m_cppOutputDirectory.isNull())
                    metainfo->setCppOutputDirectory(m_cppOutputDirectory);
                if (!m_javaOutputDirectory.isNull())
                    metainfo->setJavaOutputDirectory(m_javaOutputDirectory);
                metainfo->setIncludeDirectories(m_includeDirectories);
                generators << metainfo;
                contexts << "MetaInfoGenerator";
            }

            if (!m_cppOutputDirectory.isNull())
                priGenerator->setCppOutputDirectory(m_cppOutputDirectory);

            QList<QFuture<void>> generated;
            for (int i = 0; i < generators.size(); ++i) {
                AbstractGenerator *generator = generators.at(i);
                generator->setQtVersion(m_qtVersionMajor, m_qtVersionMinor, m_qtVersionPatch, m_qtjambiVersionPatch);

                if (generator->outputDirectory().isNull())
                    generator->setOutputDirectory(m_outputDirectory);
                generator->setClasses(m_metaBuilder.classes());
                if (m_printStdout){
                    ReportHandler::setContext(contexts.at(i));
                    generator->printClasses();
                }else{
                    generated << QtConcurrent::run([](AbstractGenerator *generator, const QString& context){
                                 ReportHandler::setContext(context);
                                 generator->generate();
                    }, generator, contexts.at(i));
                }
            }
            while(!generated.isEmpty()){
                generated.takeFirst().waitForFinished();
            }

            if (metainfo && java_generator) {
                ReportHandler::setContext("MetaInfoGenerator");
                metainfo->writeLibraryInitializers();
            }

            ReportHandler::setContext("PriGenerator");
            priGenerator->setQtVersion(m_qtVersionMajor, m_qtVersionMinor, m_qtVersionPatch, m_qtjambiVersionPatch);
            if (priGenerator->outputDirectory().isNull())
                priGenerator->setOutputDirectory(m_outputDirectory);
            priGenerator->setClasses(m_metaBuilder.classes());
            if (m_printStdout){
                priGenerator->printClasses();
            }else{
                priGenerator->generate();
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
                  .arg(m_metaBuilder.classes().size())
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
            printf("%s\n", qPrintable(res));
            printf("Done, %d warnings (%d known issues)\n", int(ReportHandler::reportedWarnings().size()),
                   ReportHandler::suppressedCount());

            QString fileName("reported_warnings.log");
            QFile file(fileName);
            if (!m_outputDirectory.isNull())
                file.setFileName(QDir(m_outputDirectory).absoluteFilePath(fileName));
            file.remove();
            if(ReportHandler::reportedWarnings().size()){
                if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
                    {
                        QTextStream s(&file);
                        for(const QString& w : ReportHandler::reportedWarnings()){
                            if(!w.contains("Suppressed warning with no text specified"))
                                s << w << Qt::endl;
                        }
                    }
                    file.close();
                }
            }
        }

        return 0;
    }catch(const std::exception& exn){
        fprintf(stderr, "%s\n", exn.what());
        return -1;
    }
}

QStringList readDependencies(const QString& file){
    QFile f(file);
    f.open(QIODevice::ReadOnly);
    QTextStream s(&f);
    QStringList result;
    while(!s.atEnd()){
        QString line = s.readLine().trimmed();
        if(line.startsWith('#')){
            line = line.mid(1).trimmed();
            if(line.startsWith("include")){
                line = line.mid(7).trimmed();
                if(line.startsWith("<") && line.endsWith(">")){
                    line.chop(1);
                    line = line.mid(1);
                    QStringList include = line.split("/");
                    if((include.size()==2 && include[0]==include[1]) || include.size()==1){
                        result << include[0];
                    }
                }
            }
        }
    }
    f.close();
    return result;
}

void GeneratorApplication::analyzeDependencies()
{
    QMap<QString,QStringList> dependenciesByLib;
    for(QDir dir : qAsConst(m_includeDirectories)){
        if(dir.exists()){
            for(const QString& entry : dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot)){
                if(entry.endsWith(".framework")){
                    QString dependsFile(entry+"/Headers/"+entry.chopped(10)+"Depends");
                    if(dir.exists(dependsFile)){
                        dependenciesByLib[entry.chopped(10)] = readDependencies(dir.absoluteFilePath(dependsFile));
                    }
                }else{
                    QString dependsFile(entry+"/"+entry+"Depends");
                    if(dir.exists(dependsFile)){
                        dependenciesByLib[entry] = readDependencies(dir.absoluteFilePath(dependsFile));
                    }
                }
            }
#ifdef Q_OS_MAC
            if(dir.dirName()=="include" && dir.cdUp() && dir.cd("lib")){
                for(const QString& entry : dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot)){
                    if(entry.endsWith(".framework")){
                        QString dependsFile(entry+"/Headers/"+entry.chopped(10)+"Depends");
                        if(dir.exists(dependsFile)){
                            dependenciesByLib[entry.chopped(10)] = readDependencies(dir.absoluteFilePath(dependsFile));
                        }
                    }
                }
            }
#endif
        }
    }
    QMap<QString,TypeSystemTypeEntry*> typeSystemsByQtLibrary = m_database.typeSystemsByQtLibrary();
    for(const QString& libName : dependenciesByLib.keys()){
        if(!typeSystemsByQtLibrary.contains(libName)){
            TypeSystemTypeEntry* entry = new TypeSystemTypeEntry(libName, libName, {});
            entry->setCodeGeneration(TypeEntry::GenerateNothing);
            m_database.addType(entry);
            typeSystemsByQtLibrary[entry->qtLibrary()] = entry;
        }
    }

    for(TypeSystemTypeEntry* entry : typeSystemsByQtLibrary.values()){
        for(const QString& dependency : dependenciesByLib[entry->qtLibrary()]){
            bool skip = false;
            for(const Dependency& requiredQtLibrary : entry->requiredQtLibraries()){
                if(requiredQtLibrary.entry==dependency && requiredQtLibrary.mode==TS::Dependency::Mandatory){
                    skip = true;
                    break;
                }
            }
            if(!skip)
                entry->addRequiredQtLibrary(QString(dependency));
        }
    }
}

bool GeneratorApplication::nullness() const
{
    return m_nullness;
}

void GeneratorApplication::setNullability(bool newNullability)
{
    if (m_nullness == newNullability)
        return;
    m_nullness = newNullability;
    emit nullnessChanged();
}

void GeneratorApplication::modifyCppDefine(const QString &arg, bool f_set) {
//qDebug() << "modifyCppDefine():" << arg << ((f_set) ? " define" : " undef");
    QStringList list;
    if (arg.length() > 0) {
        const QChar ch = arg.at(0);
        if (ch.isSpace()) {
            list = arg.split(QRegularExpression("[\\s]+"), Qt::SkipEmptyParts);
        } else if (ch == QChar(',')) {
            list = arg.split(ch, Qt::SkipEmptyParts);
        } else {
            list = QStringList(arg);	// 1 item
        }
    }

    for (int i = 0; i < list.size(); i++) {
        const QString& s = list.at(i);
        auto split = s.indexOf("=");
        const QString name(s.left(split));
        QString value("1");
        if (split > 0)
            value = s.mid(split + 1);
//qDebug() << "modifyCppDefine():" << name << " = " << value << ((f_set) ? " define" : " undef");
        if(name.compare("*") == 0) {	// "-U*" has the effect of clearing the list
            if(!f_set) {	// its invalid name so ignore on f_set==true
                m_defineUndefineList.clear();
            }
        } else {
            m_defineUndefineList.append(DefineUndefine(name, value, f_set));
        }
    }
}


void dumpMetaJavaTree(const MetaClassList &classes);

QString GeneratorApplication::generateTypeSystemQML() const
{
    return m_generateTypeSystemQML;
}

void GeneratorApplication::setGenerateTypeSystemQML(const QString& newGenerateTypeSystemQML)
{
    if (m_generateTypeSystemQML == newGenerateTypeSystemQML)
        return;
    m_generateTypeSystemQML = newGenerateTypeSystemQML;
    emit generateTypeSystemQMLChanged();
}

bool GeneratorApplication::useNativeIds() const
{
    return TypeDatabase::instance()->useNativeIds();
}

void GeneratorApplication::setUseNativeIds(bool newUseNativeIds)
{
    if (TypeDatabase::instance()->useNativeIds() == newUseNativeIds)
        return;
    TypeDatabase::instance()->setUseNativeIds(newUseNativeIds);
    emit useNativeIdsChanged();
}

void GeneratorApplication::setDumpObjectTree(bool newDumpObjectTree)
{
    if (m_dumpObjectTree == newDumpObjectTree)
        return;
    m_dumpObjectTree = newDumpObjectTree;
    emit dumpObjectTreeChanged();
}

bool GeneratorApplication::astToXml() const
{
    return m_astToXml;
}

void GeneratorApplication::setAstToXml(bool newAstToXml)
{
    if (m_astToXml == newAstToXml)
        return;
    m_astToXml = newAstToXml;
    emit astToXmlChanged();
}

bool GeneratorApplication::noMetainfo() const
{
    return m_noMetainfo;
}

void GeneratorApplication::setNoMetainfo(bool newNoMetainfo)
{
    if (m_noMetainfo == newNoMetainfo)
        return;
    m_noMetainfo = newNoMetainfo;
    emit noMetainfoChanged();
}

bool GeneratorApplication::noCppHeaders() const
{
    return m_noCppHeaders;
}

void GeneratorApplication::setNoCppHeaders(bool newNoCppHeaders)
{
    if (m_noCppHeaders == newNoCppHeaders)
        return;
    m_noCppHeaders = newNoCppHeaders;
    emit noCppHeadersChanged();
}

bool GeneratorApplication::noJava() const
{
    return m_noJava;
}

void GeneratorApplication::setNoJava(bool newNoJava)
{
    if (m_noJava == newNoJava)
        return;
    m_noJava = newNoJava;
    emit noJavaChanged();
}

const QStringList &GeneratorApplication::staticLibraries() const
{
    return m_staticLibraries;
}

void GeneratorApplication::setStaticLibraries(const QStringList &newStaticLibraries)
{
    if (m_staticLibraries == newStaticLibraries)
        return;
    m_staticLibraries = newStaticLibraries;
    emit staticLibrariesChanged();
}

uint GeneratorApplication::qtjambiVersionPatch() const
{
    return m_qtjambiVersionPatch;
}

void GeneratorApplication::setQtjambiVersionPatch(uint newQtjambiVersionPatch)
{
    if (m_qtjambiVersionPatch == newQtjambiVersionPatch)
        return;
    m_qtjambiVersionPatch = newQtjambiVersionPatch;
    emit qtjambiVersionPatchChanged();
}

bool GeneratorApplication::printStdout() const
{
    return m_printStdout;
}

void GeneratorApplication::setPrintStdout(bool newPrintStdout)
{
    if (m_printStdout == newPrintStdout)
        return;
    m_printStdout = newPrintStdout;
    emit printStdoutChanged();
}

const QString &GeneratorApplication::outputDirectory() const
{
    return m_outputDirectory;
}

void GeneratorApplication::setOutputDirectory(const QString &newOutputDirectory)
{
    if (m_outputDirectory == newOutputDirectory)
        return;
    m_outputDirectory = newOutputDirectory;
    emit outputDirectoryChanged();
}

const QString &GeneratorApplication::javaOutputDirectory() const
{
    return m_javaOutputDirectory;
}

void GeneratorApplication::setJavaOutputDirectory(const QString &newJavaOutputDirectory)
{
    if (m_javaOutputDirectory == newJavaOutputDirectory)
        return;
    m_javaOutputDirectory = newJavaOutputDirectory;
    emit javaOutputDirectoryChanged();
}

const QString &GeneratorApplication::cppOutputDirectory() const
{
    return m_cppOutputDirectory;
}

void GeneratorApplication::setCppOutputDirectory(const QString &newCppOutputDirectory)
{
    if (m_cppOutputDirectory == newCppOutputDirectory)
        return;
    m_cppOutputDirectory = newCppOutputDirectory;
    emit cppOutputDirectoryChanged();
}

const QStringList &GeneratorApplication::typesystemDirectories() const
{
    return m_typesystemDirectories;
}

void GeneratorApplication::setTypesystemDirectories(const QStringList &newTypesystemDirectories)
{
    if (m_typesystemDirectories == newTypesystemDirectories)
        return;
    m_typesystemDirectories = newTypesystemDirectories;
    emit typesystemDirectoriesChanged();
}

int GeneratorApplication::debugCppMode() const
{
    return m_debugCppMode;
}

void GeneratorApplication::setDebugCppMode(int newDebugCppMode)
{
    if (m_debugCppMode == newDebugCppMode)
        return;
    m_debugCppMode = newDebugCppMode;
    emit debugCppModeChanged();
}

const QStringList &GeneratorApplication::importDirectories() const
{
    return m_importDirectories;
}

void GeneratorApplication::setImportDirectories(const QStringList &newImportDirectories)
{
    if (m_importDirectories == newImportDirectories)
        return;
    m_importDirectories = newImportDirectories;
    emit importDirectoriesChanged();
}

const QStringList &GeneratorApplication::includeDirectories() const
{
    return m_includeDirectories;
}

void GeneratorApplication::setIncludeDirectories(const QStringList &newIncludeDirectories)
{
    if (m_includeDirectories == newIncludeDirectories)
        return;
    m_includeDirectories = newIncludeDirectories;
    emit includeDirectoriesChanged();
}

const QString &GeneratorApplication::docsDirectory() const
{
    return m_docsDirectory;
}

void GeneratorApplication::setDocsDirectory(const QString &newDocsDirectory)
{
    if (m_docsDirectory == newDocsDirectory)
        return;
    m_docsDirectory = newDocsDirectory;
    emit docsDirectoryChanged();
}

const QStringList &GeneratorApplication::rebuildClasses() const
{
    return TypeDatabase::instance()->rebuildClasses();
}

void GeneratorApplication::setRebuildClasses(const QStringList &newRebuildClasses)
{
    if (TypeDatabase::instance()->rebuildClasses() == newRebuildClasses)
        return;
    TypeDatabase::instance()->setRebuildClasses(newRebuildClasses);
    emit rebuildClassesChanged();
}

const QString &GeneratorApplication::preProcessorFileName() const
{
    return m_preProcessorFileName;
}

void GeneratorApplication::setPreProcessorFileName(const QString &newPreProcessorFileName)
{
    if (m_preProcessorFileName == newPreProcessorFileName)
        return;
    m_preProcessorFileName = newPreProcessorFileName;
    emit preProcessorFileNameChanged();
}

const QString &GeneratorApplication::typesystemFileName() const
{
    return m_typesystemFileName;
}

void GeneratorApplication::setTypesystemFileName(const QString &newTypesystemFileName)
{
    if (m_typesystemFileName == newTypesystemFileName)
        return;
    m_typesystemFileName = newTypesystemFileName;
    emit typesystemFileNameChanged();
}

const QString &GeneratorApplication::headerFileName() const
{
    return m_headerFileName;
}

void GeneratorApplication::setHeaderFileName(const QString &newHeaderFileName)
{
    if (m_headerFileName == newHeaderFileName)
        return;
    m_headerFileName = newHeaderFileName;
    emit headerFileNameChanged();
}

bool GeneratorApplication::preprocess(QIODevice& target, const std::function<void(std::string,std::string,const QFileInfo&,bool)>& featureRegistry){
    PreprocessHandler handler(m_headerFileName, target, featureRegistry, m_includeDirectories, m_debugCppMode);
    handler.define("__WORDSIZE", QString::number(8*sizeof(void*)));
    for(const DefineUndefine& ddf : m_defineUndefineList){
        if(ddf.isSet()){
            handler.define(ddf.name(), ddf.value());
        }else{
            handler.undefine(ddf.name());
        }
    }
    return handler.handler();
}

void dumpMetaJavaAttributes(const MetaAttributes *attr) {
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

void dumpMetaJavaType(const MetaType *type) {
    if (!type) {
        printf("[void]");
    } else {
        printf("[type: %s", qPrintable(type->typeEntry()->qualifiedCppName()));
        if (type->getReferenceType()==MetaType::Reference) printf(" &");
        if (type->getReferenceType()==MetaType::RReference) printf(" &&");
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
        if (type->isQString()) printf(" java_string");
        if (type->isQStringRef()) printf(" java_string");
        if (type->isConstant()) printf(" const");
        if (type->isVolatile()) printf(" volatile");
        printf("]");
    }
}

void dumpMetaJavaArgument(const MetaArgument *arg) {
    printf("        ");
    dumpMetaJavaType(arg->type());
    printf(" %s", qPrintable(arg->argumentName()));
    if (!arg->defaultValueExpression().isEmpty())
        printf(" = %s", qPrintable(arg->defaultValueExpression()));
    printf("\n");
}

void dumpMetaJavaFunction(const MetaFunction *func) {
    printf("    %s() - ", qPrintable(func->name()));
    dumpMetaJavaType(func->type());
    dumpMetaJavaAttributes(func);

    // Extra attributes...
    if (func->isSignal()) printf(" signal");
    if (func->isSlot()) printf(" slot");
    if (func->isConstant()) printf(" const");

    printf("\n      arguments:\n");
    for(MetaArgument *arg : func->arguments())
        dumpMetaJavaArgument(arg);
}

void dumpMetaJavaClass(const MetaClass *cls) {
    printf("\nclass: %s, package: %s\n", qPrintable(cls->name().replace("$", ".")), qPrintable(cls->package()));
    if (cls->hasVirtualFunctions())
        printf("    shell based\n");
    printf("  baseclass: %s %s\n", qPrintable(cls->baseClassName()), cls->isQObject() ? "'QObject-type'" : "'not a QObject-type'");
    printf("  interfaces:");
    for(MetaClass *iface : cls->interfaces())
        printf(" %s", qPrintable(iface->name().replace("$", ".")));
    printf("\n");
    printf("  attributes:");
    dumpMetaJavaAttributes(cls);

    printf("\n  functions:\n");
    for(const MetaFunction *func : cls->functions())
        dumpMetaJavaFunction(func);
}
