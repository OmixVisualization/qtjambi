/****************************************************************************
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

#include <QFileInfo>
#include <QStringList>
#include <QDir>
#include <QDebug>

#include <cstdio>

#include "preprocesshandler.h"

PreprocessHandler::PreprocessHandler(QString sourceFile, QIODevice& target,
                                     const std::function<void(std::string,std::string,const QFileInfo&,bool)> &featureRegistry,
                                     const QStringList &includePathList, int verbose) :
        env(featureRegistry),
        preprocess(env),
        verbose(verbose),
        ppconfig(":/io/qtjambi/generator/pp-qt-configuration"),
        sourceFile(sourceFile),
        target(target),
        includePathList(includePathList)
{
    //empty space for useless comments
    preprocess.verbose = verbose;
}

PreprocessHandler::~PreprocessHandler(){
    for(std::string* s : stdStrings){
        delete s;
    }
    stdStrings.clear();
    for(rpp::pp_fast_string* s : fastStrings){
        delete s;
    }
    fastStrings.clear();
}

bool PreprocessHandler::checkDefineUndefine(const QString &, int) const {
    return false;
}

bool PreprocessHandler::dumpCheck(int kind) const {
    if((verbose & kind) == kind)
        return true;
    return false;
}

void PreprocessHandler::dump(int kind) const {
    std::string stage;
    if(kind == DEBUGLOG_DUMP_BEFORE)
        stage = "STAGE1";
    else if(kind == DEBUGLOG_DUMP_MIDDLE)
        stage = "STAGE2";
    else if(kind == DEBUGLOG_DUMP_AFTER)
        stage = "AFTER";
    else
        return;

    std::cout << "DUMP " << stage << " (begin)" << std::endl;

    std::cout << "DUMP " << stage << " (end)" << std::endl;
}
        

bool PreprocessHandler::handler() {
    QFile file(ppconfig);
    if (!file.open(QFile::ReadOnly)) {
        std::fprintf(stderr, "Preprocessor configuration file not found '%s'\n", ppconfig);
        return false;
    }

    QByteArray ba = file.readAll();
    file.close();

// FIXME: Dump empty at start
// FIXME: If any debug mode, enable showing DEFINE/UNDEF/INCLUDE(summary/verbose)
// FIXME: If nothing set on cmdline enable #define Q_OS_OS2
// PROCESS string of "#define name value"
// FIXME: Replace "null_out" with stdout
    preprocess.process(ba.constData(), ba.constData() + ba.size(), null_out);
    preprocess.include_paths = includePathList;
    QString currentDir = QDir::current().absolutePath();

    writeTargetFile(currentDir);

    return true;
}

void PreprocessHandler::writeTargetFile(QString currentDir) {

    QFileInfo sourceInfo(sourceFile);
    QDir::setCurrent(sourceInfo.absolutePath());

    std::string result;
    result.reserve(20 * 1024);  // 20K

    result += "# 1 \"builtins\"\n";
    result += "# 1 \"";
    result += toStdString(sourceInfo.absoluteFilePath());
    result += "\"\n";

    qDebug() << "Processing source" << sourceInfo.absolutePath() << sourceInfo.fileName();
    preprocess.file(sourceInfo, rpp::pp_output_iterator<std::string> (result));
    QDir::setCurrent(currentDir);
    target.write(result.c_str(), qint64(result.length()));
}
