/****************************************************************************
**
** Copyright (C) 1992-2009 Nokia. All rights reserved.
** Copyright 2005 Harald Fernengel <harry@kdevelop.org>
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
** If you are unsure which license is appropriate for your use, please
** contact the sales department at qt-sales@nokia.com.
** $END_LICENSE$

**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

#include <QtCore/QtCore>

#include <string>

#include "preprocessor.h"
#include "pp-engine-bits.h"

// register callback for include hooks
//static void includeFileHook ( const std::string &, const std::string &, FILE * );

#define PP_HOOK_ON_FILE_INCLUDED(A, B, C) includeFileHook(A, B, C)

using namespace rpp;

QHash<QString, QStringList> includedFiles;

/*void includeFileHook ( const std::string &fileName, const std::string &filePath, FILE * )
{
    includedFiles[QString::fromStdString ( fileName ) ].append ( QString::fromStdString ( filePath ) );
}*/

Preprocessor::Preprocessor() {
    d = new PreprocessorPrivate;
    includedFiles.clear();
}

Preprocessor::~Preprocessor() {
    delete d;
}

void Preprocessor::processFile(const QString &fileName) {
    rpp::pp proc(d->env);
    d->initPP(proc);

    d->result.reserve(d->result.size() + 20 * 1024);

    d->result += "# 1 \"" + fileName.toLatin1() + "\"\n"; // ### REMOVE ME
    proc.file(fileName.toLocal8Bit().constData(), std::back_inserter(d->result));
}

void Preprocessor::processString(const QByteArray &str) {
    pp proc(d->env);
    d->initPP(proc);

    proc(str.begin(), str.end(), std::back_inserter(d->result));
}

QByteArray Preprocessor::result() const {
    return d->result;
}

void Preprocessor::addIncludePaths(const QStringList &includePaths) {
    d->includePaths += includePaths;
}

QStringList Preprocessor::macroNames() const {
    QStringList macros;

    pp_environment::const_iterator it = d->env.first_macro();
    while (it != d->env.last_macro()) {
        const pp_macro *m = *it;
        macros += QString::fromLatin1(m->name->begin(), (int) m->name->size());
        ++it;
    }

    return macros;
}

QList<Preprocessor::MacroItem> Preprocessor::macros() const {
    QList<MacroItem> items;

    pp_environment::const_iterator it = d->env.first_macro();
    while (it != d->env.last_macro()) {
        const pp_macro *m = *it;
        MacroItem item;
        item.name = QString::fromLatin1(m->name->begin(), (int)m->name->size());
        item.definition = QString::fromLatin1(m->definition->begin(),
                                              (int) m->definition->size());
        for (size_t i = 0; i < m->formals.size(); ++i) {
            item.parameters += QString::fromLatin1(m->formals[i]->begin(),
                                                   (int) m->formals[i]->size());
        }
        item.isFunctionLike = m->function_like;

#ifdef PP_WITH_MACRO_POSITION
        item.fileName = QString::fromLatin1(m->file->begin(), m->file->size());
#endif
        items += item;

        ++it;
    }

    return items;
}

/*
int main()
{
    Preprocessor pp;

    QStringList paths;
    paths << "/usr/include";
    pp.addIncludePaths(paths);

    pp.processFile("pp-configuration");
    pp.processFile("/usr/include/stdio.h");

    qDebug() << pp.result();

    return 0;
}
*/

