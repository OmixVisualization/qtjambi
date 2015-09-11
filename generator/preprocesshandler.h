
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
        PreprocessHandler(QString sourceFile, QString targetFile, const QString &phononInclude,
            const QStringList &includePathList, const QStringList &inputDirectoryList, int verbose);

        bool handler();

    private:
        rpp::pp_environment env;
        rpp::pp preprocess;
        rpp::pp_null_output_iterator null_out;

        int verbose;
        const char *ppconfig;
        QString sourceFile;
        QString targetFile;
        QString phononInclude;
        QStringList includePathList;
        QStringList inputDirectoryList;

        QStringList setIncludes();

        /**
         * Reads through master include file and writes preprocessed file for actual
         * processing of headers.
         *
         * TODO: more indepth description of this system somewhere
         */
        void writeTargetFile(QString sourceFile, QString targetFile, QString currentDir);

        bool dumpCheck(int kind) const;
        void dump(int kind) const;
        void undefine(const QString &name) {
            rpp::pp_fast_string fs_name(::toStdString(name));
            env.unbind(&fs_name);
        }
        void define(const QString &name, const QString &value) {
            rpp::pp_fast_string fs_name(::toStdString(name));
            rpp::pp_fast_string fs_value(::toStdString(value));
            rpp::pp_macro macro;
            macro.name = &fs_name;
            macro.definition = &fs_value;
            env.bind(macro.name, macro);
        }
        void setDebugMode(int or_value, int and_value) {
            verbose |= or_value;
            verbose &= and_value;
        }
        bool checkDefineUndefine(const QString &name, int check) const;
        QList<PreprocessHandler> checkDefineUndefineList;
};

#endif
