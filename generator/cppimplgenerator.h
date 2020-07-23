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

#ifndef CPPIMPLGENERATOR_H
#define CPPIMPLGENERATOR_H

#include "cppgenerator.h"
#include "metajava.h"

enum JNISignatureFormat {
    Underscores = 0x0001,        //!< Used in the jni exported function names
    SlashesAndStuff = 0x0010,     //!< Used for looking up functions through jni
    ReturnType = 0x0100,
    NoModification = 0x1000
};

QString jni_signature(const AbstractMetaFunctional *function, JNISignatureFormat format);
QString jni_signature(const AbstractMetaFunction *function, JNISignatureFormat format);
QString jni_signature(const QString &full_name, JNISignatureFormat format);
QString jni_signature(const AbstractMetaType *java_type, JNISignatureFormat format = Underscores);

class CppImplGenerator : public CppGenerator {
        Q_OBJECT

    public:
        enum JNISignatureMode {
            JNIExport       = 0x0001,
            ReturnType      = 0x0002,
            ExternC         = 0x0004,
            Deprecated      = 0x0008,

            StandardJNISignature = JNIExport | ReturnType | ExternC
        };

        CppImplGenerator(PriGenerator *pri)
                : m_native_jump_table(false) {
            priGenerator = pri;
        }

        void generateFake(const AbstractMetaClass *);

        virtual QString fileNameForClass(const AbstractMetaClass *cls) const;

        virtual QString fileNameForFunctional(const AbstractMetaFunctional *cls) const;

        void write(QTextStream &s, const AbstractMetaClass *java_class, int nesting_level = 0);

        void write(QTextStream &s, const AbstractMetaFunctional *java_class, int nesting_level = 0);

        void writeExtraIncludes(QTextStream &s, const AbstractMetaClass *java_class, QSet<QString>& dedupe);

        void writeExtraIncludes(QTextStream &s, const AbstractMetaFunctional *java_class, QSet<QString>& dedupe);

        void writeAssignment(QTextStream &s, const QString &destName, const QString &srcName,
                             const AbstractMetaType *java_type);
        static void writeCustomStructors(QTextStream &s, const TypeEntry *entry);
        void writeCodeInjections(QTextStream &s,
                                 const ComplexTypeEntry *typeEntry,
                                 CodeSnip::Position position,
                                 TypeSystem::Language language);
        void writeCodeInjections(QTextStream &s,
                                 const AbstractMetaFunction *java_function,
                                 const AbstractMetaClass *implementor,
                                 CodeSnip::Position position,
                                 TypeSystem::Language language,
                                 const QString& __jni_env = "__jni_env",
                                 const QString& qtjambi_scope = "",
                                 Option option = NoOption);
        bool hasCodeInjections(const AbstractMetaFunction *java_function,
                                 const AbstractMetaClass *implementor,
                                 const QSet<CodeSnip::Position>& positions,
                                 TypeSystem::Language language);
        bool hasCodeInjections(const AbstractMetaFunctional *java_function,
                                 const QSet<CodeSnip::Position>& positions,
                                 TypeSystem::Language language);
        void writeExtraFunctions(QTextStream &s, const AbstractMetaClass *java_class);
        void writeToStringFunction(QTextStream &s, const AbstractMetaClass *java_class);
        void writeCloneFunction(QTextStream &s, const AbstractMetaClass *java_class);
        void writeShellConstructor(QTextStream &s, bool isInterface, const AbstractMetaFunction *java_function, int options = 0);
        void writeShellDestructor(QTextStream &s, bool isInterface, const AbstractMetaClass *java_class);
        void writeShellConstructor(QTextStream &s, const AbstractMetaFunctional *java_class);
        void writeShellDestructor(QTextStream &s, const AbstractMetaFunctional *java_class);
        void writeArgumentConversion(QTextStream &s, const AbstractMetaFunction *signal);
        void writeTypeConversion(QTextStream &s, const AbstractMetaFunction *function, AbstractMetaType *type, int index, const QString& metaTypeId, const QString& parameterType);
        void writeMetaInfo(QTextStream &s, const AbstractMetaClass *java_class,
                           const QMultiMap<int,AbstractMetaFunction *>& availabeConstructors,
                           const QList<const AbstractMetaFunction *>& signalsInTargetLang,
                           const QList<QString>& signalNames,
                           const QMap<QString,QList<const AbstractMetaFunction*>>& sortedSignals,
                           bool isInterface);
        void writeMetaInfo(QTextStream &s, const AbstractMetaClass *owner, const AbstractMetaEnum *java_class, bool ownerIsPublic);
        void writeMetaInfo(QTextStream &s, const AbstractMetaFunctional *java_class);
        void writeSignalFunction(QTextStream &s, const AbstractMetaFunction *java_function,
                                 const AbstractMetaClass *implementor, int pos);
        void writeShellFunction(QTextStream &s, const AbstractMetaFunction *java_function,
                                const AbstractMetaClass *implementor, int pos);
        void writePublicFunctionOverride(QTextStream &s,
                                         const AbstractMetaFunction *java_function,
                                         const AbstractMetaClass *java_class);
        void writeVirtualFunctionOverride(QTextStream &s,
                                          const AbstractMetaFunction *java_function,
                                          const AbstractMetaClass *java_class);
        bool writeBaseClassFunctionCall(QTextStream &s,
                                        const AbstractMetaFunction *java_function,
                                        const AbstractMetaClass *java_class,
                                        Option options = NoOption);
        void writeDeleteAndOwnerFunction(QTextStream &s, const AbstractMetaClass *cls);
        void writeQObjectFunctions(QTextStream &s, const AbstractMetaClass *java_class);
        void writeFunctionCall(QTextStream &s,
                               const QString &variable_name,
                               const AbstractMetaFunction *java_function,
                               QString prefix = QString(),
                               Option option = NoOption,
                               const QStringList &extraParameters = QStringList());
        void writeFunctionCallArguments(QTextStream &s, const AbstractMetaFunction *java_function,
                                        const QString &prefix = QString(), Option option = NoOption);
        void writeFunctionCallArguments(QTextStream &s, const AbstractMetaFunction *java_function,
                                        const QList<const AbstractMetaArgument*>& arguments,
                                        const QString &prefix = QString(), Option option = NoOption);
        void writeFunctionCallArguments(QTextStream &s, const AbstractMetaFunctional *java_function,
                                        const QString &prefix = QString(), Option option = NoOption);
        static void writeFunctionName(QTextStream &s,
                                      const AbstractMetaFunction *java_function,
                                      const AbstractMetaClass *java_class = nullptr,
                                      uint options = StandardJNISignature);

        void writeJavaToQt(QTextStream &s,
                           const AbstractMetaType *java_type,
                           const QString &qt_name,
                           const QString &java_name,
                           const AbstractMetaFunction *java_function,
                           const AbstractMetaFunctional *java_functional,
                           int argument_index,
                           Option option = OriginalName,
                           const QString& __jni_env = "__jni_env",
                           const QString& qtjambi_scope = "");

        void writeConstructor(QTextStream &s,
                                const AbstractMetaFunction *java_function,
                                const QList<const AbstractMetaArgument *>& arguments,
                                const AbstractMetaClass *java_class, int counter);
        void writeFinalFunction(QTextStream &s,
                                const AbstractMetaFunction *java_function,
                                const AbstractMetaClass *java_class);
        void writeFinalConstructor(QTextStream &s,
                                const AbstractMetaFunction *java_function,
                                const AbstractMetaClass *java_class, const QHash<const AbstractMetaFunction *,uint>& constructorIndexes = QHash<const AbstractMetaFunction *,uint>());
        static void writeFinalFunctionArguments(QTextStream &s,
                                                const AbstractMetaFunction *java_function,
                                                const QString &java_object_name);
        void writeFinalFunctionSetup(QTextStream &s,
                                     const AbstractMetaFunction *java_function,
                                     const QString &qt_object_name,
                                     const AbstractMetaClass *java_class);
        void writeOwnership(QTextStream &s,
                            const AbstractMetaFunction *java_function,
                            const QString &java_var_name,
                            const QString &qt_var_name,
                            int var_index,
                            const AbstractMetaClass *implementor,
                            TypeSystem::Language,
                            const QString& __jni_env = "__jni_env",
                            const QString& qtjambi_scope = "");
        bool hasOwnership(const AbstractMetaFunction *java_function,
                            int var_index,
                            const AbstractMetaClass *implementor,
                            TypeSystem::Language);
        void writeQtToJava(QTextStream &s,
                           const AbstractMetaType *java_type,
                           const QString &qt_name,
                           const QString &java_name,
                           const AbstractMetaFunction *java_function,
                           const AbstractMetaFunctional *java_functional,
                           int argument_index,
                           Option option = NoOption,
                           const QString& __jni_env = "__jni_env",
                           const QString& qtjambi_scope = "");

        void writeQIteratorFunction(QTextStream &s,
                                      const AbstractMetaType *java_type,
                                      const AbstractMetaType *elementType,
                                      const QString& variable,
                                      Option option = NoOption,
                                      const QString& iteratorName = "const_iterator",
                                      const QString& __jni_env = "__jni_env",
                                      const QString& qtjambi_scope = "");

        void writeQMapIteratorFunction(QTextStream &s,
                                      const AbstractMetaType *java_type,
                                      const AbstractMetaType *keyType,
                                      const AbstractMetaType *valueType,
                                      const QString& variable,
                                      Option option = NoOption,
                                      const QString& iteratorName = "const_iterator",
                                      const QString& __jni_env = "__jni_env",
                                      const QString& qtjambi_scope = "");

        bool writeConversionRule(QTextStream &s,
                                 TypeSystem::Language target_language,
                                 const AbstractMetaFunction *java_function,
                                 const AbstractMetaFunctional *java_functional,
                                 int argument_index,
                                 const QString &qt_name,
                                 const QString &java_name,
                                 Option option = NoOption,
                                 const QString& __jni_env = "__jni_env",
                                 const QString& qtjambi_scope = "");

        void writeFieldAccessors(QTextStream &s, const AbstractMetaField *java_field);

        void writeJavaLangObjectOverrideFunctions(QTextStream &s, const AbstractMetaClass *cls);

        void writeJavaToQtContainer(QTextStream &s,
                                    const AbstractMetaType *java_type,
                                    const QString &qt_name,
                                    const QString &java_name,
                                    const AbstractMetaFunction *java_function,
                                    const AbstractMetaFunctional *java_functional,
                                    int argument_index,
                                    const QString& __jni_env = "__jni_env",
                                    const QString& qtjambi_scope = "",
                                    Option option = NoOption);

        void writeJavaToQtInitializerList(QTextStream &s,
                                    const AbstractMetaType *java_type,
                                    const QString &qt_name,
                                    const QString &java_name,
                                    const AbstractMetaFunction *java_function,
                                    const AbstractMetaFunctional *java_functional,
                                    int argument_index,
                                    const QString& __jni_env = "__jni_env",
                                    const QString& qtjambi_scope = "",
                                    Option option = NoOption);

        bool hasCustomDestructor(const AbstractMetaClass *java_class) const;

        static QString translateType(const AbstractMetaType *java_type, Option option = NoOption);

        inline bool nativeJumpTable() const { return m_native_jump_table; }
        inline void setNativeJumpTable(bool n) { m_native_jump_table = n; }

        static QString jniReturnName(const AbstractMetaFunction *java_function);

        bool qtJambiDebugTools() const { return m_qtjambi_debug_tools; }
        void setQtJambiDebugTools(bool bf) { m_qtjambi_debug_tools = bf; }
        bool shouldGenerate(const AbstractMetaClass *java_class) const {
            return /*(!java_class->isNamespace() || java_class->functionsInTargetLang().size() > 0)
                   &&*/ !java_class->isInterface()
                   && !java_class->typeEntry()->isVariant()
                   && !java_class->typeEntry()->isIterator()
                   && (java_class->typeEntry()->codeGeneration() & TypeEntry::GenerateCpp)
                   && !java_class->isFake();
        }
    private:
        QString fromObject(const AbstractMetaType *java_type, const QString &var_name, bool invalidateAfterUse = false,
                           const QString& __jni_env = "__jni_env");
        bool m_native_jump_table;
        bool m_qtjambi_debug_tools;
};

#endif // CPPIMPLGENERATOR_H
