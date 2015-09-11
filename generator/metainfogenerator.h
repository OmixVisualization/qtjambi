/****************************************************************************
**
** Copyright (C) 1992-2009 Nokia. All rights reserved.
** Copyright (C) 2009-2015 Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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

#ifndef METAINFOGENERATOR_H
#define METAINFOGENERATOR_H

#include "generator.h"
#include "javagenerator.h"
#include "cppgenerator.h"

class MetaInfoGenerator : public JavaGenerator {
    public:
        MetaInfoGenerator(PriGenerator *pri);

        enum GenerationFlags {
            GeneratedJavaClasses = 0x1,
            GeneratedMetaInfo = 0x2
        };

        enum OutputDirectoryType {
            CppDirectory,
            JavaDirectory
        };

        MetaInfoGenerator();

        virtual void generate();
        virtual QString fileNameForClass(const AbstractMetaClass *java_class) const;
        virtual void write(QTextStream &s, const AbstractMetaClass *java_class, int nesting_level = 0);

        void setFilenameStub(const QString &stub) { m_filenameStub = stub; }
        QString filenameStub() const { return m_filenameStub; }

        QString headerFilename() const { return filenameStub() + ".h"; }
        QString cppFilename() const { return filenameStub() + ".cpp"; }

        virtual QString subDirectoryForClass(const AbstractMetaClass *, OutputDirectoryType type) const;
        virtual QString subDirectoryForPackage(const QString &package, OutputDirectoryType type) const;
        virtual bool shouldGenerate(const AbstractMetaClass *) const;

        bool generated(const AbstractMetaClass *cls) const;
        bool generatedJavaClasses(const QString &package) const;
        bool generatedMetaInfo(const QString &package) const;

        QString cppOutputDirectory() const {
            if (!m_cpp_out_dir.isNull())
                return m_cpp_out_dir;
            return outputDirectory() + QLatin1String("/cpp");
        }
        void setCppOutputDirectory(const QString &cppOutDir) { m_cpp_out_dir = cppOutDir; }
        QString javaOutputDirectory() const {
            if (!m_java_out_dir.isNull())
                return m_java_out_dir;
            return outputDirectory() + QLatin1String("/java");
        }
        void setJavaOutputDirectory(const QString &javaOutDir) { m_java_out_dir = javaOutDir; }

        bool qtJambiDebugTools() const { return m_qtjambi_debug_tools; }
        void setQtJambiDebugTools(bool bf) { m_qtjambi_debug_tools = bf; }

    private:
        void writeCppFile();
        void writeHeaderFile();
        void writeLibraryInitializers();
        void writeInclude(QTextStream &s, const Include &inc, QSet<QString> &dedupe);
        void writeIncludeStatements(QTextStream &s, const AbstractMetaClassList &classList, const QString &package);
        void writeInitializationFunctionName(QTextStream &s, const QString &package, bool fullSignature);
        void writeInitialization(QTextStream &s, const TypeEntry *entry, const AbstractMetaClass *cls, bool registerMetaType = true);
        void writeCustomStructors(QTextStream &s, const TypeEntry *entry);
        void writeDestructors(QTextStream &s, const AbstractMetaClass *cls);
        void writeCodeBlock(QTextStream &s, const QString &code);
        void writeSignalsAndSlots(QTextStream &s, const QString &package);
        void writeEnums(QTextStream &s, const QString &package);
        void writeRegisterSignalsAndSlots(QTextStream &s);
        void writeRegisterEnums(QTextStream &s);
        QStringList writePolymorphicHandler(QTextStream &s, const QString &package, const AbstractMetaClassList &clss);
        bool shouldGenerate(const TypeEntry *entry) const;
        void buildSkipList();

        // This is only needed when qtJambiDebugTools() is set
        void writeNameLiteral(QTextStream &, const TypeEntry *, const QString &fileName);

        QHash<QString, int> m_skip_list;
        QString m_filenameStub;

        QHash<OutputDirectoryType, QString> m_out_dir;

        const AbstractMetaClass* lookupClassWithPublicDestructor(const AbstractMetaClass *cls);

        PriGenerator *priGenerator;

        QString m_cpp_out_dir;
        QString m_java_out_dir;

        bool m_qtjambi_debug_tools;
};

#endif // METAINFOGENERATOR_H
