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

#ifndef CPPIMPLGENERATOR_H
#define CPPIMPLGENERATOR_H

#include "cppgenerator.h"
#include "metajava.h"

enum JNISignatureFormat {
    Underscores,        //!< Used in the jni exported function names
    SlashesAndStuff     //!< Used for looking up functions through jni
};

QString jni_signature(const QString &full_name, JNISignatureFormat format);
QString jni_signature(const AbstractMetaType *java_type, JNISignatureFormat format = Underscores);

class CppImplGenerator : public CppGenerator {
        Q_OBJECT

    public:
        enum JNISignatureMode {
            JNIExport       = 0x0001,
            ReturnType      = 0x0002,
            ExternC         = 0x0004,

            StandardJNISignature = JNIExport | ReturnType | ExternC
        };

        CppImplGenerator(PriGenerator *pri)
                : m_native_jump_table(false) {
            priGenerator = pri;
        }

        virtual QString fileNameForClass(const AbstractMetaClass *cls) const;

        void write(QTextStream &s, const AbstractMetaClass *java_class, int nesting_level = 0);

        void writeExtraIncludes(QTextStream &s, const AbstractMetaClass *java_class);

        void writeAssignment(QTextStream &s, const QString &destName, const QString &srcName,
                             const AbstractMetaType *java_type);
        void writeSignalInitialization(QTextStream &s, const AbstractMetaClass *java_class);
        void writeCodeInjections(QTextStream &s,
                                 const AbstractMetaFunction *java_function,
                                 const AbstractMetaClass *implementor,
                                 CodeSnip::Position position,
                                 TypeSystem::Language language);
        void writeExtraFunctions(QTextStream &s, const AbstractMetaClass *java_class);
        void writeToStringFunction(QTextStream &s, const AbstractMetaClass *java_class);
        void writeCloneFunction(QTextStream &s, const AbstractMetaClass *java_class);
        void writeShellSignatures(QTextStream &s, const AbstractMetaClass *java_class);
        void writeShellConstructor(QTextStream &s, const AbstractMetaFunction *java_function, int options = 0);
        void writeShellDestructor(QTextStream &s, const AbstractMetaClass *java_class);
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
        void writeFinalDestructor(QTextStream &s, const AbstractMetaClass *cls);
        void writeFinalConstructor(QTextStream &s,
                                   const AbstractMetaFunction *java_function,
                                   const QString &qt_object_name,
                                   const QString &java_object_name);
        void writeQObjectFunctions(QTextStream &s, const AbstractMetaClass *java_class);
        void writeFunctionCall(QTextStream &s,
                               const QString &variable_name,
                               const AbstractMetaFunction *java_function,
                               const QString &prefix = QString(),
                               Option option = NoOption,
                               const QStringList &extraParameters = QStringList());
        void writeFunctionCallArguments(QTextStream &s, const AbstractMetaFunction *java_function,
                                        const QString &prefix = QString(), Option option = NoOption);
        static void writeFunctionName(QTextStream &s,
                                      const AbstractMetaFunction *java_function,
                                      const AbstractMetaClass *java_class = 0,
                                      uint options = StandardJNISignature);

        void writeJavaToQt(QTextStream &s,
                           const AbstractMetaClass *java_class,
                           const AbstractMetaType *function_return_type,
                           const QString &qt_name,
                           const QString &java_name,
                           const AbstractMetaFunction *java_function,
                           int argument_index);
        void writeJavaToQt(QTextStream &s,
                           const AbstractMetaType *java_type,
                           const QString &qt_name,
                           const QString &java_name,
                           const AbstractMetaFunction *java_function,
                           int argument_index,
                           Option option = OriginalName);

        void writeFinalFunction(QTextStream &s,
                                const AbstractMetaFunction *java_function,
                                const AbstractMetaClass *java_class);
        static void writeFinalFunctionArguments(QTextStream &s,
                                                const AbstractMetaFunction *java_function,
                                                const QString &java_object_name);
        void writeFinalFunctionSetup(QTextStream &s,
                                     const AbstractMetaFunction *java_function,
                                     const QString &qt_object_name,
                                     const AbstractMetaClass *java_class);
        void writeOwnership(QTextStream &s,
                            const AbstractMetaFunction *java_function,
                            const QString &var_name,
                            int var_index,
                            const AbstractMetaClass *implementor);
        void writeQtToJava(QTextStream &s,
                           const AbstractMetaType *java_type,
                           const QString &qt_name,
                           const QString &java_name,
                           const AbstractMetaFunction *java_function,
                           int argument_index,
                           Option option = NoOption);

        bool writeConversionRule(QTextStream &s,
                                 TypeSystem::Language target_language,
                                 const AbstractMetaFunction *java_function,
                                 int argument_index,
                                 const QString &qt_name,
                                 const QString &java_name);

        void writeFieldAccessors(QTextStream &s, const AbstractMetaField *java_field);

        void writeOriginalMetaObjectFunction(QTextStream &s, const AbstractMetaClass *java_class);
        void writeFromNativeFunction(QTextStream &s,
                                     const AbstractMetaClass *java_class);
        void writeFromArrayFunction(QTextStream &s, const AbstractMetaClass *java_class);
        void writeJavaLangObjectOverrideFunctions(QTextStream &s, const AbstractMetaClass *cls);

        void writeInterfaceCastFunction(QTextStream &s,
                                        const AbstractMetaClass *java_class,
                                        const AbstractMetaClass *interface);

        void writeQtToJavaContainer(QTextStream &s,
                                    const AbstractMetaType *java_type,
                                    const QString &qt_name,
                                    const QString &java_name,
                                    const AbstractMetaFunction *java_function,
                                    int argument_index);
        void writeJavaToQtContainer(QTextStream &s,
                                    const AbstractMetaType *java_type,
                                    const QString &qt_name,
                                    const QString &java_name,
                                    const AbstractMetaFunction *java_function,
                                    int argument_index);

        void writeDefaultConstructedValues(QTextStream &s, const AbstractMetaClass *java_class);

        bool hasCustomDestructor(const AbstractMetaClass *java_class) const;

        static QString translateType(const AbstractMetaType *java_type, Option option = NoOption);

        inline bool nativeJumpTable() const { return m_native_jump_table; }
        inline void setNativeJumpTable(bool n) { m_native_jump_table = n; }

        static QString jniReturnName(const AbstractMetaFunction *java_function);

        bool qtJambiDebugTools() const { return m_qtjambi_debug_tools; }
        void setQtJambiDebugTools(bool bf) { m_qtjambi_debug_tools = bf; }

    private:
        void writeDefaultConstructedValues_helper(QSet<QString> &values,
                const AbstractMetaFunction *function);
        QString fromObject(const TypeEntry *centry, const QString &var_name);
        QString fromPointerContainer(const AbstractMetaType *type, const QString &var_name);

        bool m_native_jump_table;
        bool m_qtjambi_debug_tools;
};

#endif // CPPIMPLGENERATOR_H
