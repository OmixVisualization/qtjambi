
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
