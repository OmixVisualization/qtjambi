
#ifndef WRAPPER_H_
#define WRAPPER_H_

#include <iostream>
#include <QMap>

#include "generatorset.h"
#include "debuglog.h"

class DefineUndefine {

    public:
        DefineUndefine(const QString &name, const QString &value, bool set) :
            m_name(name), m_value(value), m_set(set) {
        }

        DefineUndefine &operator=(const DefineUndefine &rhs) {
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
        static bool isTargetPlatformArmCpu;
        void setDefineUndefineStage(int stage) { defineUndefineStageCurrent = stage; }

        int runJambiGenerator();


    private:
        void displayHelp(GeneratorSet* arg1);
        void modifyCppDefine(const QString &arg, bool f_set);
        QMap<QString, QString> parseArguments(int argc, char *argv[]);
        void assignVariables();
        void handleArguments();

        QString default_file;
        QString default_system;

        QString fileName;
        QString typesystemFileName;
        QString pp_file;
        QStringList rebuild_classes;
        GeneratorSet *gs;
        QMap< QString, QString > args;
        QStringList includePathsList;
        QStringList inputDirectoryList;
        int defineUndefineStageCurrent;
        QList< DefineUndefine > defineUndefineStageOneList;
        QList< DefineUndefine > defineUndefineStageTwoList;
        int debugCppMode;
};

#endif
