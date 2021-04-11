/****************************************************************************
**
** Copyright (C) 1992-2009 Nokia. All rights reserved.
** Copyright (C) 2009-2021 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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
#include "docindex/docmodel.h"

#include <QTextStream>
#include <QtNetwork>

class DocParser;

class JavaGenerator : public Generator {
        Q_OBJECT

    public:
        JavaGenerator();

        QString translateType(const AbstractMetaType *java_type, const AbstractMetaClass *context, Option option = NoOption);

        virtual void generateFake(const AbstractMetaClass *java_class) override;

        void writeInjectedCode(QTextStream &s,
                               const AbstractMetaFunction *java_function,
                               CodeSnip::Position position);
        bool hasCodeInjections(const AbstractMetaFunction *java_function,
                               const QSet<CodeSnip::Position>& positions);
        void writeArgument(QTextStream &s,
                                  const AbstractMetaFunction *java_function,
                                  const AbstractMetaArgument *java_argument,
                                  uint options = 0);
        QString argumentString(const AbstractMetaFunction *java_function,
                                      const AbstractMetaArgument *java_argument,
                                      uint options = 0);
        void writeFunctional(QTextStream &s, const AbstractMetaFunctional *java_functional);
        void writeEnum(QTextStream &s, const AbstractMetaEnum *java_enum);
        void writeIntegerEnum(QTextStream &s, const uint size, const AbstractMetaEnum *java_enum);
        void writeSignal(QTextStream &s, const AbstractMetaFunction *java_function);
        void writeMultiSignal(QTextStream &s, const AbstractMetaFunctionList& signalList);
        void writeFunction(QTextStream &s, const AbstractMetaFunction *java_function,
                           uint included_attributes = 0, uint excluded_attributes = 0, Option option = NoOption);
        void writeFieldAccessors(QTextStream &s, const AbstractMetaField *field, Option functionOptions = NoOption);
        void write(QTextStream &s, const AbstractMetaClass *java_class, int nesting_level = 0) override;
        void write(QTextStream &s, const AbstractMetaFunctional *java_class, int nesting_level = 0) override;

        void writeFunctionOverloads(QTextStream &s, const AbstractMetaFunction *java_function,
                                    uint included_attributes, uint excluded_attributes, Option option, const QString& alternativeFunctionName = QString());
        void writeEnumOverload(QTextStream &s, const AbstractMetaFunction *java_function,
                               uint include_attributes, uint exclude_attributes, Option option);
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
        void writeOwnershipForContainer(QTextStream &s, TypeSystem::Ownership ownership, AbstractMetaArgument *arg, const AbstractMetaFunction *java_function);
        void writeOwnershipForContainer(QTextStream &s, TypeSystem::Ownership ownership, AbstractMetaType *type,
                                        const QString &arg_name, const AbstractMetaFunction *java_function);
        void writePrivateNativeFunction(QTextStream &s, const AbstractMetaFunction *java_function);
        void writeJavaLangObjectOverrideFunctions(QTextStream &s, const AbstractMetaClass *cls);
        void writeReferenceCount(QTextStream &s, const ReferenceCount &refCount, int argumentIndex, const AbstractMetaFunction *java_function, const QString &thisName = QLatin1String("this"));
        void retrieveModifications(const AbstractMetaFunction *f, const AbstractMetaClass *java_class,
                                   uint *exclude_attributes, uint *include_attributes, Option option = NoOption) const;
        QString functionSignature(const AbstractMetaFunction *java_function,
                                  uint included_attributes,
                                  uint excluded_attributes,
                                  Option option = NoOption,
                                  int arg_count = -1,
                                  const QString& alternativeFunctionName = QString());
        void setupForFunction(const AbstractMetaFunction *java_function,
                              uint *included_attributes, uint *excluded_attributes, Option option) const;

        QString subDirectoryForClass(const AbstractMetaClass *java_class) const override;

        QString subDirectoryForFunctional(const AbstractMetaFunctional * java_class) const override;

        virtual QString fileNameForClass(const AbstractMetaClass *java_class) const override;

        bool isComparable(const AbstractMetaClass *cls) const;

        const AbstractMetaType * getIterableType(const AbstractMetaClass *cls) const;

#if 0
        void write1_dot_5_enum(QTextStream &s, const AbstractMetaEnum *java_enum);
#endif

        virtual bool shouldGenerate(const AbstractMetaClass *java_class) const override {
            return java_class->enclosingClass()==nullptr // do not generate if enclosed class (in this case create static embedded class)
                    && !java_class->typeEntry()->isIterator()
                    && !java_class->isFake() && !java_class->typeEntry()->isVariant()
                    && (java_class->typeEntry()->codeGeneration() & TypeEntry::GenerateTargetLang);
        }

        bool shouldGenerate(const AbstractMetaFunctional *) const override { return false; }

        QString documentationDirectory() const { return m_doc_directory; }
        void setDocumentationDirectory(const QString &docDir) { m_doc_directory = docDir; }

        bool documentationEnabled() const { return m_docs_enabled; }
        void setDocumentationEnabled(bool e) { m_docs_enabled = e; }
        void generate() override;

        void writeIteratorFunctions(QTextStream &s, const AbstractMetaClass *java_class);
        void writeAbstractMapFunctions(QTextStream &s, const AbstractMetaClass *java_class);
        void writeAbstractMultiMapFunctions(QTextStream &s, const AbstractMetaClass *java_class);
        void writeAbstractListFunctions(QTextStream &s, const AbstractMetaClass *java_class);
        void writeMapFunctions(QTextStream &s, const AbstractMetaClass *java_class);
        void writeMultiMapFunctions(QTextStream &s, const AbstractMetaClass *java_class);
        void writeMultiHashFunctions(QTextStream &s, const AbstractMetaClass *java_class);
        void writeHashFunctions(QTextStream &s, const AbstractMetaClass *java_class);
        void writeCollectionFunctions(QTextStream &s, const AbstractMetaClass *java_class);
        void writeListFunctions(QTextStream &s, const AbstractMetaClass *java_class);
        void writeLinkedListFunctions(QTextStream &s, const AbstractMetaClass *java_class);
        void writeSetFunctions(QTextStream &s, const AbstractMetaClass *java_class);
        void writeQueueFunctions(QTextStream &s, const AbstractMetaClass *java_class);
        void writeStackFunctions(QTextStream &s, const AbstractMetaClass *java_class);
        void writeVectorFunctions(QTextStream &s, const AbstractMetaClass *java_class);

        inline bool nativeJumpTable() const { return m_native_jump_table; }
        inline void setNativeJumpTable(bool n) { m_native_jump_table = n; }

        inline uint targetJDKVersion() const { return target_JDK_version; }
        inline void setTargetJDKVersion(uint v) { target_JDK_version = v; }

        /*virtual*/ QString resolveOutputDirectory() const override { return javaOutputDirectory(); }

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

        void setDocsUrl(const QString& _docsUrl){
            docsUrl = _docsUrl;
        }

    private:
        void printExtraCode(QStringList& lines, QTextStream &s, bool addFreeLine = false);
        void write_equals_parts(QTextStream &s, const AbstractMetaFunctionList &lst, char prefix, bool& first, bool& suppressUnchecked, bool& suppressRaw);
        QString subDirectoryForPackage(const QString &package) const { return QString(package).replace(".", "/"); }
        void writeInstantiatedType(QTextStream &s, const AbstractMetaType *abstractMetaType, bool forceBoxed) const;
        void write(QTextStream &s, const AbstractMetaEnum *global_enum);
        void writeDeprecatedComment(QTextStream& commentStream, const AbstractMetaFunction *java_function);
    protected:
        QString m_package_name;
        QString m_doc_directory;
        QString m_java_out_dir;
        QString m_log_out_dir;		// needed for *.log generation
        QString docsUrl;
        DocParser *m_doc_parser;
        bool m_docs_enabled;
        bool m_native_jump_table;
        uint target_JDK_version;
        QList<const AbstractMetaFunction *> m_nativepointer_functions;
        QList<const AbstractMetaFunction *> m_resettable_object_functions;
        QList<const AbstractMetaFunction *> m_reference_count_candidate_functions;
        QList<const AbstractMetaFunction *> m_factory_functions;
        QList<const AbstractMetaFunction *> m_inconsistent_functions;
private:
        QNetworkAccessManager m_networkManager;
        bool m_current_class_needs_internal_import;
};

#endif // JAVAGENERATOR_H
