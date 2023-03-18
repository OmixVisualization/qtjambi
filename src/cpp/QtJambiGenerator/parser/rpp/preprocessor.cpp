/****************************************************************************
**
** Copyright (C) 1992-2009 Nokia. All rights reserved.
** Copyright 2005 Harald Fernengel <harry@kdevelop.org>
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

class PreprocessorPrivate {
    public:
        PreprocessorPrivate(const std::function<void(std::string,std::string,const QFileInfo&,bool)>& featureRegistry);
        QByteArray result;
        rpp::pp_environment env;
        QStringList includePaths;

        void initPP(rpp::pp &proc) {
            proc.include_paths = includePaths;
        }
};

/*void includeFileHook ( const std::string &fileName, const std::string &filePath, FILE * )
{
    includedFiles[QString::fromStdString ( fileName ) ].append ( QString::fromStdString ( filePath ) );
}*/

Preprocessor::Preprocessor(const std::function<void(std::string,std::string,const QFileInfo&,bool)>& featureRegistry)
    : d(new PreprocessorPrivate(featureRegistry)) {
}

PreprocessorPrivate::PreprocessorPrivate(const std::function<void(std::string,std::string,const QFileInfo&,bool)>& featureRegistry)
    : result(),
      env(featureRegistry),
      includePaths()
{}

Preprocessor::~Preprocessor() {
    delete d;
}

void Preprocessor::processFile(const QFileInfo &file) {
    rpp::pp proc(d->env);
    d->initPP(proc);

    d->result.reserve(d->result.size() + 20 * 1024);

    d->result += "# 1 \"" + file.absoluteFilePath().toLocal8Bit() + "\"\n"; // ### REMOVE ME
    proc.file(file, std::back_inserter(d->result));
}

void Preprocessor::processString(const QByteArray &str) {
    pp proc(d->env);
    d->initPP(proc);

    proc.process(str.begin(), str.end(), std::back_inserter(d->result));
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
        macros += QString::fromLatin1(m->name->begin(), int(m->name->size()));
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
        item.name = QString::fromLatin1(m->name->begin(), int(m->name->size()));
        item.definition = QString::fromLatin1(m->definition->begin(),
                                              int(m->definition->size()));
        for (size_t i = 0; i < m->formals.size(); ++i) {
            item.parameters += QString::fromLatin1(m->formals[i]->begin(),
                                                   int(m->formals[i]->size()));
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

