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

#ifndef CPPIMPLGENERATOR_H
#define CPPIMPLGENERATOR_H

#include "cppgenerator.h"
#include "metalang.h"

class CppImplGenerator : public CppGenerator {
    public:
        enum JNISignatureMode {
            JNIExport       = 0x0001,
            ReturnType      = 0x0002,
            ExternC         = 0x0004,
            Deprecated      = 0x0008,

            StandardJNISignature = JNIExport | ReturnType | ExternC
        };

        CppImplGenerator(PriGenerator *pri);

        void generateFake(const MetaClass *) override;

        virtual QString fileNameForClass(const MetaClass *cls) const override;

        virtual QString fileNameForFunctional(const MetaFunctional *cls) const override;

        void write(QTextStream &s, const MetaClass *java_class, int nesting_level = 0) override;

        void write(QTextStream &s, const MetaFunctional *java_class, int nesting_level = 0) override;

        void writeExtraIncludes(QTextStream &s, const MetaClass *java_class, QSet<QString>& dedupe, bool skipQtJambi = false);

        void writeExtraIncludes(QTextStream &s, const MetaFunctional *java_class, QSet<QString>& dedupe, bool skipQtJambi = false);

        void writeAssignment(QTextStream &s, const QString &destName, const QString &srcName,
                             const MetaType *java_type);
        void writeCustomStructors(QTextStream &s, const ComplexTypeEntry *entry);
        void writeCodeInjections(QTextStream &s,
                                 const TypeEntry *typeEntry,
                                 CodeSnip::Position position,
                                 TS::Language language);
        void writeCodeInjections(QTextStream &s,
                                 const MetaFunction *java_function,
                                 const MetaClass *implementor,
                                 CodeSnip::Position position,
                                 TS::Language language,
                                 const QString& __jni_env = "__jni_env",
                                 const QString& qtjambi_scope = "",
                                 Option option = NoOption);
        bool hasCodeInjections(const MetaFunction *java_function,
                                 const MetaClass *implementor,
                                 const QSet<CodeSnip::Position>& positions,
                                 TS::Language language);
        bool hasCodeInjections(const MetaFunctional *java_function,
                                 const QSet<CodeSnip::Position>& positions,
                                 TS::Language language);
        void writeClassCodeInjections(QTextStream &s, const MetaClass *java_class, CodeSnip::Position position);
        void writeToStringFunction(QTextStream &s, const MetaClass *java_class);
        void writeCloneFunction(QTextStream &s, const MetaClass *java_class);
        void writeShellConstructor(QTextStream &s, bool isInterface, const MetaFunction *java_function, Option options = NoOption);
        void writeShellDestructor(QTextStream &s, bool isInterface, const MetaClass *java_class);
        void writeShellConstructor(QTextStream &s, const MetaFunctional *java_class);
        void writeShellDestructor(QTextStream &s, const MetaFunctional *java_class);
        void writeArgumentConversion(QTextStream &s, const MetaFunction *signal, QStringList& converterFunctions, QSet<QString> &forwardDeclarations);
        void writeTypeConversion(QTextStream &s, const MetaFunction *function, MetaType *type, int index, const QString& metaTypeId, QStringList& converterFunctions, QSet<QString> &forwardDeclarations);
        void writeMetaInfo(QTextStream &s, const MetaClass *java_class,
                           const QMultiMap<int,MetaFunction *>& availabeConstructors,
                           const QList<const MetaFunction *>& signalsInTargetLang,
                           const QList<QString>& signalNames,
                           const QMap<QString,QList<const MetaFunction*>>& sortedSignals,
                           bool isInterface);
        void writeMetaInfo(QTextStream &s, const MetaClass *owner, const MetaEnum *java_class, bool ownerIsPublic);
        void writeMetaInfo(QTextStream &s, const MetaFunctional *java_class);
        void writeShellFunction(QTextStream &s, const MetaFunction *java_function,
                                const MetaClass *implementor, int pos);
        void writePublicFunctionOverride(QTextStream &s,
                                         const MetaFunction *java_function,
                                         const MetaClass *java_class);
        void writeVirtualFunctionOverride(QTextStream &s,
                                          const MetaFunction *java_function,
                                          const MetaClass *java_class);
        bool writeBaseClassFunctionCall(QTextStream &s,
                                        const MetaFunction *java_function,
                                        const MetaClass *java_class,
                                        Option options = NoOption);
        void writeDeleteAndOwnerFunction(QTextStream &s, const MetaClass *cls);
        void writeQObjectFunctions(QTextStream &s, const MetaClass *java_class);
        void writeFunctionCall(QTextStream &s,
                               const QString &variable_name,
                               const MetaFunction *java_function,
                               QString prefix = QString(),
                               Option option = NoOption,
                               const QStringList &extraParameters = QStringList());
        void writeFunctionCallArguments(QTextStream &s, const MetaFunction *java_function,
                                        const QString &prefix = QString(), Option option = NoOption);
        void writeFunctionCallArguments(QTextStream &s, const MetaFunction *java_function,
                                        const QList<const MetaArgument*>& arguments,
                                        const QString &prefix = QString(), Option option = NoOption);
        void writeFunctionCallArguments(QTextStream &s, const MetaFunctional *java_function,
                                        const QString &prefix = QString(), Option option = NoOption);
        void writeFunctionName(QTextStream &s,
                                      const MetaFunction *java_function,
                                      const MetaClass *java_class = nullptr,
                                      uint options = StandardJNISignature) const;

        bool writeJavaToQt(QTextStream &s,
                           const MetaType *java_type,
                           const QString &qt_name,
                           const QString &java_name,
                           const MetaFunction *java_function,
                           const MetaFunctional *java_functional,
                           int argument_index,
                           Option option = OriginalName,
                           const QString& __jni_env = "__jni_env",
                           const QString& qtjambi_scope = "");

        void writeConstructor(QTextStream &s,
                                const MetaFunction *java_function,
                                const QList<const MetaArgument *>& arguments,
                                const MetaClass *java_class, int counter);
        void writeFinalFunction(QTextStream &s,
                                const MetaFunction *java_function,
                                const MetaClass *java_class);
        void writeFinalConstructor(QTextStream &s,
                                const MetaFunction *java_function,
                                const MetaClass *java_class, const QHash<const MetaFunction *,uint>& constructorIndexes = QHash<const MetaFunction *,uint>());
        void writeFinalFunctionArguments(QTextStream &s,
                                                const MetaFunction *java_function,
                                                const QString &java_object_name) const;
        void writeFinalFunctionSetup(QTextStream &s,
                                     const MetaFunction *java_function,
                                     const QString &qt_object_name,
                                     const MetaClass *java_class, Option option = NoOption);
        void writeOwnership(QTextStream &s,
                            const MetaFunction *java_function,
                            const QString &java_var_name,
                            const QString &qt_var_name,
                            int var_index,
                            const MetaClass *implementor,
                            TS::Language,
                            const QString& __jni_env = "__jni_env",
                            const QString& qtjambi_scope = "");
        bool hasOwnership(const MetaFunction *java_function,
                            int var_index,
                            const MetaClass *implementor,
                            TS::Language);
        bool writeQtToJava(QTextStream &s,
                           const MetaType *java_type,
                           const QString &qt_name,
                           const QString &java_name,
                           const MetaFunction *java_function,
                           const MetaFunctional *java_functional,
                           int argument_index,
                           Option option = NoOption,
                           const QString& __jni_env = "__jni_env",
                           const QString& qtjambi_scope = "");

        void writeQSequentialIteratorFunction(QTextStream &s,
                                      const MetaType *java_type,
                                      const MetaType *elementType,
                                      const QString& variable,
                                      Option option = NoOption,
                                      const QString& iteratorName = "const_iterator",
                                      const QString& __jni_env = "__jni_env",
                                      const QString& qtjambi_scope = "");

        void writeQAssociativeIteratorFunction(QTextStream &s,
                                      const MetaType *java_type,
                                      const MetaType *keyType,
                                      const MetaType *valueType,
                                      const QString& variable,
                                      Option option = NoOption,
                                      const QString& iteratorName = "const_iterator",
                                      const QString& __jni_env = "__jni_env",
                                      const QString& qtjambi_scope = "");

        bool writeConversionRule(QTextStream &s,
                                 TS::Language target_language,
                                 const MetaFunction *java_function,
                                 const MetaFunctional *java_functional,
                                 int argument_index,
                                 const QString &qt_name,
                                 const QString &java_name,
                                 Option option = NoOption,
                                 const QString& __jni_env = "__jni_env",
                                 const QString& qtjambi_scope = "");

        void writeFieldAccessors(QTextStream &s, const MetaField *java_field);

        void writeJavaLangObjectOverrideFunctions(QTextStream &s, const MetaClass *cls);

        void writeJavaToQtContainer(QTextStream &s,
                                    const MetaType *java_type,
                                    const QString &qt_name,
                                    const QString &java_name,
                                    const MetaFunction *java_function,
                                    const MetaFunctional *java_functional,
                                    int argument_index,
                                    const QString& __jni_env = "__jni_env",
                                    const QString& qtjambi_scope = "",
                                    Option option = NoOption);

        void writeJavaToQtInitializerList(QTextStream &s,
                                    const MetaType *java_type,
                                    const QString &qt_name,
                                    const QString &java_name,
                                    const MetaFunction *java_function,
                                    const MetaFunctional *java_functional,
                                    int argument_index,
                                    const QString& __jni_env = "__jni_env",
                                    const QString& qtjambi_scope = "",
                                    Option option = NoOption);

        QString jniReturnName(const MetaFunction *java_function) const;
        bool shouldGenerate(const MetaClass *java_class) const override;
        QString default_return_statement_qt(const MetaType *java_type, AbstractGenerator::Option options = AbstractGenerator::NoOption);
        void setContainerBaseClasses(const QMap<TypeSystemTypeEntry *,QList<QPair<TypeInfo,bool>>>& containerBaseClasses);
    private:
        QByteArray callXxxMethod(const QString &name) const;
        QByteArray callXxxMethod(const MetaType *java_type) const;
        QMap<QString,QMap<QString,const MetaClass*>> nativeToJavaConverterInfos;
        QMap<QString,QMap<QString,const MetaClass*>> javaToNativeConverterInfos;
};

#endif // CPPIMPLGENERATOR_H
