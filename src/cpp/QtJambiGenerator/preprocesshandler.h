/****************************************************************************
**
** Copyright (C) 2009-2025 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
**
** This file is part of Qt Jambi.
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
** $END_LICENSE$
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/


#ifndef PREPROCESSHANDLER_H_
#define PREPROCESSHANDLER_H_

#include <QString>

#include "parser/rpp/pp-iterator.h"
#include "parser/rpp/pp-engine-bits.h"
#include "parser/rpp/pp-environment.h"
#include "util.h"
#include "debuglog.h"

class PreprocessHandler {

    public:
        PreprocessHandler(QString sourceFile, QIODevice& target,
                          const std::function<void(std::string,std::string,const QFileInfo&,bool)> &featureRegistry,
                          const QStringList &includePathList, int verbose);

        bool handler();
        ~PreprocessHandler();
    private:
        rpp::pp_environment env;
        rpp::pp preprocess;
        rpp::pp_null_output_iterator null_out;

        int verbose;
        const char *ppconfig;
        QString sourceFile;
        QIODevice& target;
        QStringList includePathList;
        QList<std::string*> stdStrings;
        QList<rpp::pp_fast_string*> fastStrings;

        /**
         * Reads through master include file and writes preprocessed file for actual
         * processing of headers.
         *
         * TODO: more indepth description of this system somewhere
         */
        void writeTargetFile(QString currentDir);

        bool dumpCheck(int kind) const;
        void dump(int kind) const;

    public:
        void undefine(const QString &name) {
            rpp::pp_fast_string fs_name(::toStdString(name));
            env.unbind(&fs_name);
        }
        void define(const QString &name, const QString &value) {
            std::string* s_name = new std::string(name.toStdString());
            std::string* s_value = new std::string(value.toStdString());
            stdStrings << s_name << s_value;
            rpp::pp_fast_string* fs_name = new rpp::pp_fast_string(*s_name);
            rpp::pp_fast_string* fs_value = new rpp::pp_fast_string(*s_value);
            fastStrings << fs_name << fs_value;
            rpp::pp_macro macro;
            macro.name = fs_name;
            macro.definition = fs_value;
            env.bind(macro.name, macro);
        }

    private:
        void setDebugMode(int or_value, int and_value) {
            verbose |= or_value;
            verbose &= and_value;
        }
        bool checkDefineUndefine(const QString &name, int check) const;
        QList<PreprocessHandler> checkDefineUndefineList;
};

#endif
