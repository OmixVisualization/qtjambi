/****************************************************************************
**
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

#ifndef GENERATOR_H_
#define GENERATOR_H_

#include <iostream>
#include <QMap>
#include <QDir>
#include <QCoreApplication>

#include "metabuilder.h"
#include "codemodel.h"

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

namespace TS{
    class TypeDatabase;
}

/**
 * Class that parses and initializes actual generator.
 * Name of this class ought to be Generator, but until someone
 * has done some refactoring to generator classes
 */
class GeneratorApplication : public QCoreApplication{
    Q_OBJECT
public:
    GeneratorApplication(int& argc, char *argv[]);
    Q_INVOKABLE void parseArguments();
    Q_INVOKABLE int generate();
    Q_INVOKABLE void modifyCppDefine(const QString &arg, bool f_set);

    const QString &headerFileName() const;
    void setHeaderFileName(const QString &newHeaderFileName);

    const QString &typesystemFileName() const;
    void setTypesystemFileName(const QString &newTypesystemFileName);

    const QString &preProcessorFileName() const;
    void setPreProcessorFileName(const QString &newPreProcessorFileName);

    const QStringList &rebuildClasses() const;
    void setRebuildClasses(const QStringList &newRebuildClasses);

    const QString &docsDirectory() const;
    void setDocsDirectory(const QString &newDocsDirectory);

    const QStringList &includeDirectories() const;
    void setIncludeDirectories(const QStringList &newIncludeDirectories);

    const QStringList &importDirectories() const;
    void setImportDirectories(const QStringList &newImportDirectories);

    int debugCppMode() const;
    void setDebugCppMode(int newDebugCppMode);

    const QStringList &typesystemDirectories() const;
    void setTypesystemDirectories(const QStringList &newTypesystemDirectories);

    const QString &cppOutputDirectory() const;
    void setCppOutputDirectory(const QString &newCppOutputDirectory);

    const QString &javaOutputDirectory() const;
    void setJavaOutputDirectory(const QString &newJavaOutputDirectory);

    const QString &outputDirectory() const;
    void setOutputDirectory(const QString &newOutputDirectory);

    bool printStdout() const;
    void setPrintStdout(bool newPrintStdout);

    uint qtjambiVersionPatch() const;
    void setQtjambiVersionPatch(uint newQtjambiVersionPatch);

    const QStringList &staticLibraries() const;
    void setStaticLibraries(const QStringList &newStaticLibraries);

    bool noJava() const;
    void setNoJava(bool newNoJava);

    bool noCppHeaders() const;
    void setNoCppHeaders(bool newNoCppHeaders);

    bool noMetainfo() const;
    void setNoMetainfo(bool newNoMetainfo);

    bool astToXml() const;
    void setAstToXml(bool newAstToXml);

    void setDumpObjectTree(bool newDumpObjectTree);
    bool isDumpObjectTree() const {return m_dumpObjectTree;}

    bool useNativeIds() const;
    void setUseNativeIds(bool newUseNativeIds);

    QString generateTypeSystemQML() const;
    void setGenerateTypeSystemQML(const QString& newGenerateTypeSystemQML);

    bool nullness() const;
    void setNullability(bool newNullability);

signals:
    void headerFileNameChanged();

    void typesystemFileNameChanged();

    void preProcessorFileNameChanged();

    void rebuildClassesChanged();

    void docsDirectoryChanged();

    void includeDirectoriesChanged();

    void importDirectoriesChanged();

    void debugCppModeChanged();

    void typesystemDirectoriesChanged();

    void cppOutputDirectoryChanged();

    void javaOutputDirectoryChanged();

    void outputDirectoryChanged();

    void printStdoutChanged();

    void qtjambiVersionPatchChanged();

    void staticLibrariesChanged();

    void noJavaChanged();

    void noCppHeadersChanged();

    void noMetainfoChanged();

    void astToXmlChanged();

    void dumpObjectTreeChanged();

    void useNativeIdsChanged();

    void generateTypeSystemQMLChanged();

    void nullnessChanged();

private:
    bool preprocess(QIODevice& target, const std::function<void(std::string,std::string,const QFileInfo&,bool)>& featureRegistry);

    void analyzeDependencies();

    QString m_headerFileName;
    QString m_typesystemFileName;
    QString m_preProcessorFileName;
    QString m_docsDirectory;
    QStringList m_includeDirectories;
    QStringList m_importDirectories;
    QList< DefineUndefine > m_defineUndefineList;
    int m_debugCppMode;
    QStringList m_typesystemDirectories;
    QString m_inputDirectory;
    QString m_cppOutputDirectory;
    QString m_javaOutputDirectory;
    QString m_outputDirectory;
    bool m_printStdout;
    QString m_docsUrl;
    uint m_qtVersionMajor;
    uint m_qtVersionMinor;
    uint m_qtVersionPatch;
    uint m_qtjambiVersionPatch;
    QStringList m_staticLibraries;

    bool m_noJava;
    bool m_noCppHeaders;
    bool m_noCppImpl;
    bool m_noKotlinDelegates;
    bool m_noMetainfo;
    bool m_nullness;
    bool m_astToXml;
    bool m_dumpObjectTree;
    QString m_generateTypeSystemQML;

    TypeDatabase& m_database;
    MetaBuilder m_metaBuilder;
    Q_PROPERTY(QString headerFileName READ headerFileName WRITE setHeaderFileName NOTIFY headerFileNameChanged)
    Q_PROPERTY(QString typesystemFileName READ typesystemFileName WRITE setTypesystemFileName NOTIFY typesystemFileNameChanged)
    Q_PROPERTY(QString preProcessorFileName READ preProcessorFileName WRITE setPreProcessorFileName NOTIFY preProcessorFileNameChanged)
    Q_PROPERTY(QStringList rebuildClasses READ rebuildClasses WRITE setRebuildClasses NOTIFY rebuildClassesChanged)
    Q_PROPERTY(QString docsDirectory READ docsDirectory WRITE setDocsDirectory NOTIFY docsDirectoryChanged)
    Q_PROPERTY(QStringList includeDirectories READ includeDirectories WRITE setIncludeDirectories NOTIFY includeDirectoriesChanged)
    Q_PROPERTY(QStringList importDirectories READ importDirectories WRITE setImportDirectories NOTIFY importDirectoriesChanged)
    Q_PROPERTY(int debugCppMode READ debugCppMode WRITE setDebugCppMode NOTIFY debugCppModeChanged)
    Q_PROPERTY(QStringList typesystemDirectories READ typesystemDirectories WRITE setTypesystemDirectories NOTIFY typesystemDirectoriesChanged)
    Q_PROPERTY(QString cppOutputDirectory READ cppOutputDirectory WRITE setCppOutputDirectory NOTIFY cppOutputDirectoryChanged)
    Q_PROPERTY(QString javaOutputDirectory READ javaOutputDirectory WRITE setJavaOutputDirectory NOTIFY javaOutputDirectoryChanged)
    Q_PROPERTY(QString outputDirectory READ outputDirectory WRITE setOutputDirectory NOTIFY outputDirectoryChanged)
    Q_PROPERTY(bool printStdout READ printStdout WRITE setPrintStdout NOTIFY printStdoutChanged)
    Q_PROPERTY(uint qtjambiVersionPatch READ qtjambiVersionPatch WRITE setQtjambiVersionPatch NOTIFY qtjambiVersionPatchChanged)
    Q_PROPERTY(QStringList staticLibraries READ staticLibraries WRITE setStaticLibraries NOTIFY staticLibrariesChanged)
    Q_PROPERTY(bool noJava READ noJava WRITE setNoJava NOTIFY noJavaChanged)
    Q_PROPERTY(bool noCppHeaders READ noCppHeaders WRITE setNoCppHeaders NOTIFY noCppHeadersChanged)
    Q_PROPERTY(bool noMetainfo READ noMetainfo WRITE setNoMetainfo NOTIFY noMetainfoChanged)
    Q_PROPERTY(bool astToXml READ astToXml WRITE setAstToXml NOTIFY astToXmlChanged)
    Q_PROPERTY(bool dumpObjectTree READ isDumpObjectTree WRITE setDumpObjectTree NOTIFY dumpObjectTreeChanged)
    Q_PROPERTY(bool useNativeIds READ useNativeIds WRITE setUseNativeIds NOTIFY useNativeIdsChanged)
    Q_PROPERTY(QString generateTypeSystemQML READ generateTypeSystemQML WRITE setGenerateTypeSystemQML NOTIFY generateTypeSystemQMLChanged)
    Q_PROPERTY(bool nullness READ nullness WRITE setNullability NOTIFY nullnessChanged)
};

#endif
