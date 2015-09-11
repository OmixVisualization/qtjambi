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

#ifndef JAVAGENERATOR_H
#define JAVAGENERATOR_H

#include "generator.h"
#include "metajava.h"

#include <QTextStream>

class DocParser;

class JavaGenerator : public Generator {
        Q_OBJECT

    public:
        JavaGenerator();

        static QString translateType(const AbstractMetaType *java_type, const AbstractMetaClass *context, Option option = NoOption);

        void writeInjectedCode(QTextStream &s,
                               const AbstractMetaFunction *java_function,
                               CodeSnip::Position position);
        static void writeArgument(QTextStream &s,
                                  const AbstractMetaFunction *java_function,
                                  const AbstractMetaArgument *java_argument,
                                  uint options = 0);
        static QString argumentString(const AbstractMetaFunction *java_function,
                                      const AbstractMetaArgument *java_argument,
                                      uint options = 0);
        void writeEnum(QTextStream &s, const AbstractMetaEnum *java_enum);
        void writeIntegerEnum(QTextStream &s, const AbstractMetaEnum *java_enum);
        void writeSignal(QTextStream &s, const AbstractMetaFunction *java_function);
        void writeFunction(QTextStream &s, const AbstractMetaFunction *java_function,
                           uint included_attributes = 0, uint excluded_attributes = 0);
        void writeFieldAccessors(QTextStream &s, const AbstractMetaField *field);
        void write(QTextStream &s, const AbstractMetaClass *java_class, int nesting_level = 0);

        void writeFunctionOverloads(QTextStream &s, const AbstractMetaFunction *java_function,
                                    uint included_attributes, uint excluded_attributes);
        void writeEnumOverload(QTextStream &s, const AbstractMetaFunction *java_function,
                               uint include_attributes, uint exclude_attributes);
        void writeExtraFunctions(QTextStream &s, const AbstractMetaClass *java_class);
        void writeToStringFunction(QTextStream &s, const AbstractMetaClass *java_class);
        void writeCloneFunction(QTextStream &s, const AbstractMetaClass *java_class);
        void writeFunctionAttributes(QTextStream &s, const AbstractMetaFunction *java_function,
                                     uint included_attributes = 0, uint excluded_attributes = 0,
                                     uint options = 0);
        void writeConstructorContents(QTextStream &s, const AbstractMetaFunction *java_function);
        void writeFunctionArguments(QTextStream &s, const AbstractMetaFunction *java_function,
                                    int count = -1, uint options = 0);
        void writeJavaCallThroughContents(QTextStream &s, const AbstractMetaFunction *java_function, uint attributes = 0);
        void writeOwnershipForContainer(QTextStream &s, TypeSystem::Ownership ownership, AbstractMetaArgument *arg);
        void writeOwnershipForContainer(QTextStream &s, TypeSystem::Ownership ownership, AbstractMetaType *type,
                                        const QString &arg_name);
        void writePrivateNativeFunction(QTextStream &s, const AbstractMetaFunction *java_function);
        void writeJavaLangObjectOverrideFunctions(QTextStream &s, const AbstractMetaClass *cls);
        void writeReferenceCount(QTextStream &s, const ReferenceCount &refCount, const QString &argumentName, const AbstractMetaArgumentList& arguments);
        void retrieveModifications(const AbstractMetaFunction *f, const AbstractMetaClass *java_class,
                                   uint *exclude_attributes, uint *include_attributes) const;
        QString functionSignature(const AbstractMetaFunction *java_function,
                                  uint included_attributes,
                                  uint excluded_attributes,
                                  Option option = NoOption,
                                  int arg_count = -1);
        void setupForFunction(const AbstractMetaFunction *java_function,
                              uint *included_attributes, uint *excluded_attributes) const;

        virtual QString subDirectoryForClass(const AbstractMetaClass *java_class) const
        { return subDirectoryForPackage(java_class->package()); }

        virtual QString fileNameForClass(const AbstractMetaClass *java_class) const;

        bool isComparable(const AbstractMetaClass *cls) const;

#if 0
        void write1_dot_5_enum(QTextStream &s, const AbstractMetaEnum *java_enum);
#endif

        bool shouldGenerate(const AbstractMetaClass *java_class) const {
            return java_class->enclosingClass()==0 // do not generate if enclosed class (in this case create static embedded class)
                   && !java_class->typeEntry()->isContainer() && !java_class->typeEntry()->isVariant()
                   && (java_class->typeEntry()->codeGeneration() & TypeEntry::GenerateTargetLang);
        }

        QString documentationDirectory() const { return m_doc_directory; }
        void setDocumentationDirectory(const QString &docDir) { m_doc_directory = docDir; }

        bool documentationEnabled() const { return m_docs_enabled; }
        void setDocumentationEnabled(bool e) { m_docs_enabled = e; }
        void generate();

        inline bool nativeJumpTable() const { return m_native_jump_table; }
        inline void setNativeJumpTable(bool n) { m_native_jump_table = n; }

        /*virtual*/ QString resolveOutputDirectory() const { return javaOutputDirectory(); }

        QString javaOutputDirectory() const {
            if (!m_java_out_dir.isNull())
                return m_java_out_dir;
            return outputDirectory() + QLatin1String("/java");
        }
        void setJavaOutputDirectory(const QString &javaOutDir) { m_java_out_dir = javaOutDir; }

        QString logOutputDirectory() const {
            if (!m_log_out_dir.isNull())
                return m_log_out_dir;
            return outputDirectory();
        }
        void setLogOutputDirectory(const QString &logOutDir) { m_log_out_dir = logOutDir; }

    private:
        QString subDirectoryForPackage(const QString &package) const { return QString(package).replace(".", "/"); }
        void writeInstantiatedType(QTextStream &s, const AbstractMetaType *abstractMetaType, bool forceBoxed) const;

    protected:
        QString m_package_name;
        QString m_doc_directory;
        QString m_java_out_dir;
        QString m_log_out_dir;		// needed for *.log generation
        DocParser *m_doc_parser;
        bool m_docs_enabled;
        bool m_native_jump_table;
        QList<const AbstractMetaFunction *> m_nativepointer_functions;
        QList<const AbstractMetaFunction *> m_resettable_object_functions;
        QList<const AbstractMetaFunction *> m_reference_count_candidate_functions;
};

#endif // JAVAGENERATOR_H
