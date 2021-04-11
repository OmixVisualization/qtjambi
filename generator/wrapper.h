
#ifndef WRAPPER_H_
#define WRAPPER_H_

#include <iostream>
#include <QMap>
#include <QDir>

#include "generatorset.h"
#include "debuglog.h"

class DefineUndefine {

    public:
        inline DefineUndefine(const QString &name, const QString &value, bool set) :
            m_name(name), m_value(value), m_set(set) {
        }

        inline DefineUndefine(const DefineUndefine &rhs) {
            m_name  = rhs.m_name;
            m_value = rhs.m_value;
            m_set   = rhs.m_set;
        }

        inline DefineUndefine &operator=(const DefineUndefine &rhs) {
            if(this != &rhs) {
                m_name  = rhs.m_name;
                m_value = rhs.m_value;
                m_set   = rhs.m_set;
            }
            return *this;
        }

        const QString &name() const { return m_name; }
        const QString &value() const { return m_value; }
        bool isSet() const { return m_set; }

    private:
        QString m_name;
        QString m_value;
        bool m_set;
};

class TypeDatabase;

/**
 * Class that parses and initializes actual generator.
 * Name of this class ought to be Generator, but until someone
 * has done some refactoring to generator classes
 */
class Wrapper {

    public:
        Wrapper(int argc, char *argv[]);

        static QString include_directory;
        static bool qtjambi_debug_tools;
        void setDefineUndefineStage(int stage) { defineUndefineStageCurrent = stage; }

        int runJambiGenerator();

        static const QList< DefineUndefine >& getDefineUndefineStageOneList();
    private:
        Q_NORETURN void displayHelp(GeneratorSet* arg1);
        void modifyCppDefine(const QString &arg, bool f_set);
        QMap<QString, QString> parseArguments(int argc, char *argv[]);
        void assignVariables();
        void handleArguments();

        void analyzeDependencies(TypeDatabase* typeDatabase);

        static bool defined(const QString& name);

        QString default_file;
        QString default_system;

        QString fileName;
        QString typesystemFileName;
        QString pp_file;
        QStringList rebuild_classes;
        GeneratorSet *gs;
        QDir docsDirectory;
        QMap< QString, QString > args;
        QStringList includePathsList;
        QStringList inputDirectoryList;
        static int defineUndefineStageCurrent;
        static QList< DefineUndefine > defineUndefineStageOneList;
        static QList< DefineUndefine > defineUndefineStageTwoList;
        int debugCppMode;
};

#endif
