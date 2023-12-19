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

#ifndef JAVAGENERATOR_H
#define JAVAGENERATOR_H

#include "abstractgenerator.h"
#include "metalang.h"
#include "docindex/docmodel.h"
#include "typedatabase.h"

#include <QTextStream>
#include <QtNetwork>

class DocParser;

using namespace TS;

class JavaGenerator : public AbstractGenerator {
public:
    JavaGenerator(TS::TypeDatabase* database, bool nullness, bool noKotlinDelegates);

    QString translateType(const MetaType *java_type, const MetaClass *context, Option option = NoOption);

    virtual void generateFake(const MetaClass *java_class) override;

    void writeInjectedCode(QTextStream &s,
                           const MetaFunction *java_function,
                           CodeSnip::Position position);
    bool hasCodeInjections(const MetaFunction *java_function,
                           const QSet<CodeSnip::Position>& positions);
    void writeFunctionArgument(QTextStream &s,
                              const MetaFunction *java_function,
                              const MetaArgument *java_argument,
                              bool &commaRequired,
                              const QMap<int,const QString*>* alternativeTypes = nullptr,
                              Option options = Option::NoOption);
    void writeFunctional(QTextStream &s, const MetaFunctional *java_functional);
    void writeEnum(QTextStream &s, const MetaEnum *java_enum);
    void writeIntegerEnum(QTextStream &s, const uint size, const MetaEnum *java_enum);
    void writeSignal(QTextStream &s, const MetaFunction *java_function);
    void writeMultiSignal(QTextStream &s, const MetaFunctionList& signalList);
    void writeFunction(QTextStream &s, const MetaFunction *java_function,
                       uint included_attributes = 0, uint excluded_attributes = 0, Option option = NoOption);
    void writeFieldAccessors(QTextStream &s, const MetaField *field, Option functionOptions = NoOption, QList<const MetaFunction *> *overloadedFunctions = nullptr);
    void write(QTextStream &s, const MetaClass *java_class, int nesting_level = 0) override;
    void write(QTextStream &s, const MetaFunctional *java_class, int nesting_level = 0) override;

    void writeFunctionOverloads(QTextStream &s, const MetaFunction *java_function,
                                uint included_attributes = 0, uint excluded_attributes = 0, Option option = NoOption, const QString& alternativeFunctionName = QString());
    void writeExtraFunctions(QTextStream &s, const MetaClass *java_class);
    void writeExtraFunctions(QTextStream &s, const MetaFunctional *java_class, bool inInterface);
    void writeToStringFunction(QTextStream &s, const MetaClass *java_class);
    void writeCloneFunction(QTextStream &s, const MetaClass *java_class);
    void writeFunctionAttributes(QTextStream &s, const MetaFunction *java_function, int arg_count = -1,
                                 uint included_attributes = 0, uint excluded_attributes = 0,
                                 Option options = NoOption);
    void writeConstructorContents(QTextStream &s, const MetaFunction *java_function);
    void writeFunctionArguments(QTextStream &s, const MetaFunction *java_function,
                                const QMap<int,const QString*>& replacedArguments = {}, int count = -1, Option options = NoOption);
    void writeJavaCallThroughContents(QTextStream &s, const MetaFunction *java_function, uint attributes = 0);
    void writeOwnershipForContainer(QTextStream &s, const MetaFunction *java_function, TS::Ownership ownership, MetaType *type, const QString &arg_name);
    void writeFunctionCallForOwnership(QTextStream &s, const MetaFunction *java_function, TS::Ownership owner, const QString& variable);
    void writePrivateNativeFunction(QTextStream &s, const MetaFunction *java_function);
    void writeJavaLangObjectOverrideFunctions(QTextStream &s, const MetaClass *cls);
    void writeReferenceCount(QTextStream &s, const ReferenceCount &refCount, int argumentIndex, const MetaFunction *java_function, const QString &thisName = QLatin1String("this"));
    void retrieveModifications(const MetaFunction *f, const MetaClass *java_class,
                               uint *exclude_attributes, uint *include_attributes, Option option = NoOption) const;
    QString functionSignature(const MetaFunction *java_function,
                              uint included_attributes,
                              uint excluded_attributes,
                              Option option = NoOption,
                              const QMap<int,const QString*>& replacedArguments = {},
                              int arg_count = -1,
                              const QString& alternativeFunctionName = QString());
    void setupForFunction(const MetaFunction *java_function,
                          uint *included_attributes, uint *excluded_attributes, Option option) const;

    QString subDirectoryForClass(const MetaClass *java_class) const override;

    QString subDirectoryForFunctional(const MetaFunctional * java_class) const override;

    virtual QString fileNameForClass(const MetaClass *java_class) const override;

    bool isComparable(const MetaClass *cls) const;

    const MetaType * getIterableType(const MetaClass *cls, bool& isConst) const;

#if 0
    void write1_dot_5_enum(QTextStream &s, const AbstractMetaEnum *java_enum);
#endif

    virtual bool shouldGenerate(const MetaClass *java_class) const override {
        return java_class->enclosingClass()==nullptr // do not generate if enclosed class (in this case create static embedded class)
                && !java_class->typeEntry()->isIterator()
                && !java_class->isFake() && !java_class->typeEntry()->isQVariant()
                && (java_class->typeEntry()->codeGeneration() & TypeEntry::GenerateTargetLang);
    }

    bool shouldGenerate(const MetaFunctional *) const override { return false; }
    void generate() override;

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

    void setTypeSystemByPackage(const QMap<QString,TypeSystemTypeEntry *>& typeSystemByPackage){
        m_typeSystemByPackage = typeSystemByPackage;
    }

private:
    void printExtraCode(QStringList& lines, QTextStream &s, bool addFreeLine = false);
    void write_equals_parts(QTextStream &s, const MetaFunctionList &lst, char prefix, bool& first, bool& suppressUnchecked, bool& suppressRaw, QHash<const MetaFunction*,QString>& javaTypesByFunction);
    void write_compareto_parts(QTextStream &s, const MetaFunctionList &lst, int value, bool *first, bool& suppressUnchecked, bool& suppressRaw, QList<QPair<const MetaFunction*,QString>>& javaTypesByFunction);
    QString subDirectoryForPackage(const QString &package) const { return QString(package).replace(".", "/"); }
    void writeInstantiatedType(QTextStream &s, const MetaType *abstractMetaType, bool forceBoxed) const;
    void write(QTextStream &s, const MetaEnum *global_enum);
    void writeDeprecatedComment(QTextStream& commentStream, const MetaFunction *java_function);
    void registerPackage(QString className, bool inclExported = true);
protected:
    QString m_package_name;
    QString m_java_out_dir;
    QString m_log_out_dir;		// needed for *.log generation
    QString docsUrl;
    QList<const MetaFunction *> m_nativepointer_functions;
    QList<const MetaFunction *> m_resettable_object_functions;
    QList<const MetaFunction *> m_reference_count_candidate_functions;
    QList<const MetaFunction *> m_factory_functions;
    QList<const MetaFunction *> m_inconsistent_functions;
    QList<const MetaFunction *> m_implicit_constructors;
    QList<const MetaFunction *> m_functions_with_implicit_calls;
    TS::TypeDatabase* m_database;

private:
    QList<QString> m_currentPackages;
    QSet<QString> m_exportedPackages;
    QHash<QString,const MetaFunction *> m_currentMethodSignatures;
    QHash<TypeSystemTypeEntry *,QList<QString>> m_packagesByTypeSystem;
    QMap<QString,TypeSystemTypeEntry *> m_typeSystemByPackage;
    QNetworkAccessManager m_networkManager;
    bool m_noKotlinDelegates;
protected:
    bool m_nullness;
};

#endif // JAVAGENERATOR_H
