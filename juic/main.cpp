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

#define _CRT_SECURE_NO_DEPRECATE

#include "uic.h"
#include "option.h"
#include "driver.h"

#include <QDateTime>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QTextStream>
#include <QTextCodec>

#include <QDebug>

static const char *error = 0;

#ifdef Q_WS_WIN
static char ENV_SPLITTER = ';';
#else
static char ENV_SPLITTER = ':';
#endif

enum JuicError {
    NoError                         = 0x00,
    HelpInvoked                     = 0x01,
    NoClassName                     = 0x02,
    CannotMakeOutputDirectory       = 0x03,
    CannotMakeDirectoryForPackage   = 0x04,
    NotAGeneratedFile               = 0x05,
    FailedToOpenFile                = 0x06,
    DriverFailed                    = 0x07
};

struct CmdOptions
{
    CmdOptions()
        : process_all(false),
          process_directory(false)
    {
        prefix = QLatin1String("Ui_");
    }

    QString application_name;
    QString file_name;
    QString package;
    QString out_dir;
    QString prefix;
    QStringList included;
    QStringList excluded;
    bool process_all;
    bool process_directory;
};

bool generate_java_main_function;

void showHelp(const char *appName);
bool ensurePath(const QString &path);
JuicError traverseAll(const CmdOptions &options);
JuicError traverseClassPath(const QString &rootPath, const QDir &dir, const CmdOptions &options);
QString findClassName(const QFileInfo &file);

bool shouldProcess(const QFileInfo &info, const CmdOptions &options);

JuicError runJuic(const QFileInfo &uiFile, const CmdOptions &options);
QStringList resolveFileInfoList(char *argv0, const QStringList &filenames);

static int num_processed_files;
static int num_updated_files;

int main(int argc, char *argv[])
{
    CmdOptions options;
    options.application_name = QString::fromLocal8Bit(argv[0]);

    int arg = 1;
    while (arg < argc) {
        QString opt = QString::fromLocal8Bit(argv[arg]);
        if (opt == QLatin1String("-h")
            || opt == QLatin1String("-help")
            || opt == QLatin1String("--help")) {
            showHelp(argv[0]);
            return 0;
        } else if (opt == QLatin1String("-a")) {
            options.process_all = true;

        } else if (opt == QLatin1String("-i")) {
            ++arg;
            if (!argv[arg]) {
                showHelp(argv[0]);
                return 1;
            }
            options.included = resolveFileInfoList(argv[0], QString(QString::fromLocal8Bit(argv[arg])).split(ENV_SPLITTER));

        } else if (opt == QLatin1String("-e")) {
            ++arg;
            if (!argv[arg]) {
                showHelp(argv[0]);
                return 1;
            }
            options.excluded = resolveFileInfoList(argv[0], QString(QString::fromLocal8Bit(argv[arg])).split(ENV_SPLITTER));

        } else if (opt == QLatin1String("-v") || opt == QLatin1String("-version")) {
            fprintf(stderr, "Qt Jambi user interface compiler %s.\n", QT_VERSION_STR);
            return 0;
        } else if (opt == QLatin1String("-p")) {
            ++arg;
            if (!argv[arg]) {
                showHelp(argv[0]);
                return 1;
            }
            options.package = QString::fromLocal8Bit(argv[arg]);

        } else if (opt == QLatin1String("-d")) {
            ++arg;
            if (!argv[arg]) {
                showHelp(argv[0]);
                return 1;
            }
            options.out_dir = QString::fromLocal8Bit(argv[arg]);

        } else if (opt == QLatin1String("-cp")) {
            options.process_directory = true;

        } else if (opt.startsWith(QLatin1String("-pf"))) {
            options.prefix = opt.right(opt.size() - 3);

        } else if (opt.startsWith(QLatin1String("--generate-main-method"))) {
            generate_java_main_function = true;

        } else if (options.file_name.isEmpty()) {
            options.file_name = QString::fromLocal8Bit(argv[arg]);

        } else {
            showHelp(argv[0]);
            return 1;
        }
        ++arg;
    }

    options.file_name = QDir::cleanPath(options.file_name);

    // Force update single files
    {
        QFileInfo info(options.file_name);
        options.process_all |= (info.exists() && info.isFile());
    }

    if (options.process_directory) {
        int error_code;

        if (options.file_name.isEmpty()) {
            error_code = traverseAll(options);
        } else {
            error_code = traverseClassPath(QFileInfo(options.file_name).absoluteFilePath(), QDir(options.file_name), options);
        }

        if (num_processed_files == 0) {
            fprintf(stdout, "juic: no .jui files found in CLASSPATH\n");
        } else if (num_updated_files == 0) {
            fprintf(stdout, "juic: all files up to date\n");
        } else {
            fprintf(stdout, "juic: updated %d files\n", num_updated_files);
        }

        return error_code;
    }

    if (options.file_name.isEmpty()) {
        showHelp(argv[0]);
        return 1;
    }

    return runJuic(QFileInfo(options.file_name), options);
}

QStringList resolveFileInfoList(char *, const QStringList &list)
{
    QStringList file_infos;
    foreach (QString item, list) {
        QFileInfo info(item);
        if (info.exists())
            file_infos.append(info.absoluteFilePath());
        else
            file_infos.append(item.replace('\\', '/'));
    }

    return file_infos;
}

bool ensurePath(const QString &path)
{
    if (path.isEmpty())
        return true;
    if (!QFileInfo(path).exists()) {
        QDir dir;
        if (!dir.mkpath(path)) {
            fprintf(stderr, "Failed to create output directory: %s\n", qPrintable(path));
            return false;
        }
    }
    return true;
}


QString findClassName(const QFileInfo &file)
{
    QFile f(file.absoluteFilePath());
    if (!f.open(QIODevice::ReadOnly)) {
        fprintf(stderr, "juic: failed to read file: %s\n",
                qPrintable(file.absoluteFilePath()));
        return QString();
    }
    QByteArray content = f.readAll();

    int start = content.indexOf("<class>") + 7;
    int end = content.indexOf("</class>");

    if (start == -1 || end == -1 || end < start) {
        fprintf(stdout, "%s", content.constData());
        fprintf(stderr, "Invalid input: %s\n",
                qPrintable(file.absoluteFilePath()));
        return QString();
    }

    return content.mid(start, end - start).trimmed();
}


void showHelp(const char *appName)
{
    fprintf(stderr, "Qt Jambi user interface compiler %s.\n", QT_VERSION_STR);
    if (error)
        fprintf(stderr, "%s: %s\n", appName, error);
    fprintf(stderr, "Usage: %s [OPTION]... <UIFILE>\n\n"
            "  -h, -help                display this help and exit\n"
            "  -v, -version             display version\n"
            "  -d <dir>                 output directory\n"
            "  -p <package>             package of generated class file, relative to output\n"
            "                           dir\n"
            "  -cp <optional path>      updates all .jui files based on the input path.\n"
            "                           $CLASSPATH is used if no argument is specified.\n"
            "  -pf<optional prefix>     set the prefix of the names of generated classes.\n"
            "                           The default is 'Ui_'. Omit the optional argument\n"
            "                           for no prefix.\n"
            "  -i <paths>               when used with 'cp' argument, only includes the\n"
            "                           files and traverses the directories specified. The\n"
            "                           paths should be separated by '%c'.\n"
            "  -e <paths>               when used with 'cp' argument, excludes the files\n"
            "                           and does not traverse the directories specified. The\n"
            "                           paths should be separated by '%c'.\n"
            "  -a                       update files regardless of modification date.\n"
            "\n", appName, ENV_SPLITTER, ENV_SPLITTER);
}

static bool isGeneratedFile(const QFileInfo &generated_file)
{
    QFile file(generated_file.absoluteFilePath());
    if (file.open(QIODevice::ReadOnly)) {
        QByteArray ba = file.read(1024).trimmed();
        return ba.contains("** WARNING! All changes made in this file will be lost when recompiling ui file!");
    }

    return false;
}

JuicError runJuic(const QFileInfo &uiFile, const CmdOptions &options)
{
    Driver driver;
    driver.option().generator = Option::JavaGenerator;
    driver.option().javaOutputDirectory = options.out_dir;
    driver.option().javaPackage = options.package;
    driver.option().prefix = options.prefix;
    driver.option().copyrightHeader = false;

    QString className = findClassName(uiFile);
    if (className.isEmpty())
        return NoClassName;

    QString javaFileName = driver.option().prefix + className + ".java";

    // Verify that we have the output directory.
    if (!ensurePath(driver.option().javaOutputDirectory)) {
        return CannotMakeOutputDirectory;
    }

    // Verify that the package subdirectory is ok
    if (!driver.option().javaPackage.isEmpty()) {
        QDir outDir(driver.option().javaOutputDirectory);
        QString subDir = QString(driver.option().javaPackage).replace(".", "/");
        if (!ensurePath(outDir.filePath(subDir)))
            return CannotMakeDirectoryForPackage;
    }

    ++num_processed_files;

    QString outFileName;
    if (!options.out_dir.isEmpty())
        outFileName += options.out_dir + "/";
    if (!options.package.isEmpty())
        outFileName += QString(options.package).replace(".", "/") + "/";
    outFileName += javaFileName;

    QFileInfo outFileInfo(outFileName);

    // File already generated
    if (!options.process_all
        && outFileInfo.exists()
        && uiFile.lastModified() < QFileInfo(outFileName).lastModified()) {
        return NoError;
    }

    if (outFileInfo.exists()
        && !isGeneratedFile(outFileInfo)) {
        fprintf(stderr, "%s: Skipping '%s': Not a generated file\n", qPrintable(options.application_name), qPrintable(outFileName));
        return NotAGeneratedFile;
    }

    // Remove the existing file. This is needed for Windows so the case of the letters in
    // the file name are the same as in the class name just in case it has changed
    if (outFileInfo.exists()) {
        QFile::remove(outFileInfo.filePath());
    }

    // Open the output file.
    QFile f(outFileName);
    if (!f.open(QIODevice::WriteOnly | QIODevice::Text)) {
        fprintf(stderr, "%s: Failed to open output file: %s\n", qPrintable(options.application_name), qPrintable(f.fileName()));
        return FailedToOpenFile;
    }

    // Run UIC
    QTextStream stream(&f);

    // The header... we don't use Qt's because it tends to produce
    // some bad encoding issues which conflicts with javac.
    QDateTime time = QDateTime::currentDateTime();
    stream <<
        "/********************************************************************************\n"
        " ** Form generated from reading ui file '"<< uiFile.fileName() << "'\n"
        " **\n"
        " ** Created by: Qt User Interface Compiler version " QT_VERSION_STR "\n"
        " **\n"
        " ** WARNING! All changes made in this file will be lost when recompiling ui file!\n"
        " ********************************************************************************/\n";

    if (!driver.uic(uiFile.absoluteFilePath(), &stream)) {
        fprintf(stderr, "%s: Failed on input file: '%s'\n", qPrintable(options.application_name), qPrintable(uiFile.absoluteFilePath()));
        return DriverFailed;
    }

    ++num_updated_files;

    fprintf(stdout, "updated: ");
    if (!options.package.isEmpty())
        fprintf(stdout, "%s.", qPrintable(options.package));
    fprintf(stdout, "%s%s\n", qPrintable(driver.option().prefix), qPrintable(className));

    return NoError;
}


JuicError process(const QString &rootPath, const QFileInfo &file, const CmdOptions &options)
{
    QString absFilePath = QDir::toNativeSeparators(file.absoluteFilePath());
    Q_ASSERT(absFilePath.length() > rootPath.length());
    QString relFilePath = absFilePath.mid(rootPath.length() + 1);

    QString package = QDir::toNativeSeparators(QFileInfo(relFilePath).path())
                      .replace(QDir::separator(), ".");
    if (package == QLatin1String("."))
        package = QString();

    CmdOptions new_options = options;
    new_options.package = package;

    return runJuic(file, new_options);
}

static bool has_match(const QFileInfo &file, const QString &pattern)
{
    QString potential = file.absoluteFilePath();
    QStringList segments = pattern.split(QLatin1Char('*'));
    int pos = -1;
    foreach (QString segment, segments) {
        if (!segment.isEmpty()) {
            int new_pos = potential.indexOf(segment, pos < 0 ? 0 : pos);
            if ((new_pos < 0) // no match
                || (new_pos > 0 && pos < 0)) { // match too far into string for first segment
                return false;
            }
            pos = new_pos + segment.length();

        } else if (pos == -1) {
            pos = 0;
        }

    }

    return (segments.last().isEmpty() || pos >= potential.length());
}

bool shouldProcess(const QFileInfo &file, const CmdOptions &options)
{
    QStringList included = options.included, excluded = options.excluded;

    if (!included.isEmpty() && file.isDir()) {
        return true;
    } else if (!included.isEmpty()) {
        bool found = false;
        foreach (QString info, included) {
            if (has_match(file, info)) {
                found = true;
                break;
            }
        }

        if (!found)
            return false;
    } else {
        foreach (QString info, excluded) {
            if (has_match(file, info))
                return false;
        }
    }

    return true;
}


JuicError traverseClassPath(const QString &rootPath, const QDir &dir, const CmdOptions &options)
{
    QFileInfoList uiFiles = dir.entryInfoList(QStringList() << "*.jui", QDir::Files);

    JuicError error = NoError;
    for (int i=0; i<uiFiles.size(); ++i) {
        if (shouldProcess(uiFiles.at(i), options)) {
            CmdOptions new_options = options;
            if (new_options.out_dir.isEmpty())
                new_options.out_dir = rootPath;

            JuicError err = process(rootPath, uiFiles.at(i), new_options);
            if (err != NoError)
                error = err;
        }
    }

    QFileInfoList subDirs = dir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot);
    for (int i=0; i<subDirs.size(); ++i) {
        if (shouldProcess(subDirs.at(i), options)) {
            JuicError err = traverseClassPath(rootPath, QDir(subDirs.at(i).filePath()), options);
            if (err != NoError)
                error = err;
        }
    }

    return error;
}


JuicError traverseAll(const CmdOptions &options)
{

    QString classPath = QString(getenv("CLASSPATH"));
    QStringList paths = classPath.split(ENV_SPLITTER);

    JuicError error = NoError;
    for (int i=0; i<paths.size(); ++i) {
        JuicError err = traverseClassPath(QFileInfo(paths.at(i)).absoluteFilePath(), QDir(paths.at(i)), options);
        if (err != NoError)
            error = err;
    }

    return error;
}
