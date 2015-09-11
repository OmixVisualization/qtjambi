/****************************************************************************
**
** Copyright (C) 1992-2009 Nokia. All rights reserved.
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

#include "main.h"
#include "wrapper.h"
#include "preprocesshandler.h"

bool Preprocess::preprocess(const QString& sourceFile, const QString& targetFile, const QString& phononinclude,
 const QStringList& includePathList, const QStringList& inputDirectoryList, int verbose) {
    PreprocessHandler handler(sourceFile, targetFile, phononinclude, includePathList, inputDirectoryList, verbose);
    return handler.handler();
}

int main(int argc, char *argv[]) {
    Wrapper wrapper(argc, argv);
    return wrapper.runJambiGenerator();
}

QString resolveFilePath(const QString &fileName, int opts, const QStringList &list) {
    QFileInfo fileinfoAbs(fileName);
    if(fileinfoAbs.isAbsolute()) {
qDebug() << "isAbsolute path: " << fileName;;
        return QString(fileName);
    }

    foreach(const QString &s, list) {
        QDir dir(s);
        if(!dir.exists()) {
            if(opts)
                qDebug() << "Absolute path: " << fileName << " in " << dir << ": No such directory";
            continue;
        }
        QFileInfo fileinfo(dir, fileName);
        if(fileinfo.isFile())
            return QString(fileinfo.absoluteFilePath());
        if(opts)
            qDebug() << "Absolute path: " << fileName << " in " << dir << ": No such file";
    }
    return QString();
}
