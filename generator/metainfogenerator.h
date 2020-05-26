/****************************************************************************
**
** Copyright (C) 1992-2009 Nokia. All rights reserved.
** Copyright (C) 2009-2020 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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

        virtual void generate() override;
        virtual QString fileNameForClass(const AbstractMetaClass *java_class) const override;
        virtual void write(QTextStream &s, const AbstractMetaClass *java_class, int nesting_level = 0) override;

        void setFilenameStub(const QString &stub) { m_filenameStub = stub; }
        QString filenameStub() const { return m_filenameStub; }

        QString cppFilename() const { return filenameStub() + ".cpp"; }

        static QString subDirectoryForClass(const AbstractMetaClass *, OutputDirectoryType type);
        static QString subDirectoryForClass(const AbstractMetaFunctional *, OutputDirectoryType type);
        static QString subDirectoryForPackage(const QString &package, OutputDirectoryType type);
        virtual bool shouldGenerate(const AbstractMetaClass *) const override;

        bool generated(const AbstractMetaClass *cls) const;
        bool generatedJavaClasses(const QString &package) const;
        bool generatedMetaInfo(const QString &package) const;

        QString cppOutputDirectory() const {
            if (!m_cpp_out_dir.isNull())
                return m_cpp_out_dir;
            return outputDirectory() + QLatin1String("/cpp");
        }
        void setCppOutputDirectory(const QString &cppOutDir) { m_cpp_out_dir = cppOutDir; }

        bool qtJambiDebugTools() const { return m_qtjambi_debug_tools; }
        void setQtJambiDebugTools(bool bf) { m_qtjambi_debug_tools = bf; }
        bool shouldGenerate(const TypeEntry *entry) const;

    private:
        void writeCppFile();
        void writeLibraryInitializers();
        void buildSkipList();

        QHash<QString, int> m_skip_list;
        QString m_filenameStub;

        //QHash<OutputDirectoryType, QString> m_out_dir;

        PriGenerator *priGenerator;

        QString m_cpp_out_dir;

        bool m_qtjambi_debug_tools;
};

#endif // METAINFOGENERATOR_H
