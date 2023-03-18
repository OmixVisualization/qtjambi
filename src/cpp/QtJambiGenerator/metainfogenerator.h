/****************************************************************************
**
** Copyright (C) 1992-2009 Nokia. All rights reserved.
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

#ifndef METAINFOGENERATOR_H
#define METAINFOGENERATOR_H

#include "abstractgenerator.h"
#include "javagenerator.h"
#include "cppgenerator.h"
#include "typedatabase.h"

class MetaInfoGenerator : public AbstractGenerator {
    public:
        MetaInfoGenerator(PriGenerator *pri);

        enum GenerationFlags : quint8 {
            None = 0,
            GeneratedJavaClasses = 0x01,
            GeneratedMetaInfo = 0x02
        };

        virtual void generate() override;
        virtual QString fileNameForClass(const MetaClass *java_class) const override;
        virtual void write(QTextStream &s, const MetaClass *java_class, int nesting_level = 0) override;

        void setFilenameStub(const QString &stub) { m_filenameStub = stub; }
        QString filenameStub() const { return m_filenameStub; }

        QString cppFilename() const { return filenameStub() + ".cpp";
                                    }
        virtual bool shouldGenerate(const MetaClass *) const override;

        QString cppOutputDirectory() const {
            if (!m_cpp_out_dir.isNull())
                return m_cpp_out_dir;
            return outputDirectory() + QLatin1String("/cpp");
        }
        void setCppOutputDirectory(const QString &cppOutDir) { m_cpp_out_dir = cppOutDir; }

        bool shouldGenerate(const TypeEntry *entry) const;
        void writeLibraryInitializers();

        void setStaticLibraries(const QStringList& staticLibraries){
            m_staticLibraries = staticLibraries;
        }
        void setIncludeDirectories(const QStringList &newIncludeDirectories);

        QString javaOutputDirectory() const {
            if (!m_java_out_dir.isNull())
                return m_java_out_dir;
            return outputDirectory() + QLatin1String("/java");
        }
        void setJavaOutputDirectory(const QString &javaOutDir) { m_java_out_dir = javaOutDir; }

private:
        void writeCppFile();
        void buildSkipList();
        void writeContainerAccess();

        QMap<QString, quint8> m_packageGenerationPolicies;
        QMap<QString, QString> m_typeSystemByPackage;
        QString m_filenameStub;
        QString m_java_out_dir;

        PriGenerator *priGenerator;

        QString m_cpp_out_dir;
        QStringList m_includeDirectories;

        QStringList m_staticLibraries;
        TS::TypeDatabase* m_database;
};

#endif // METAINFOGENERATOR_H
