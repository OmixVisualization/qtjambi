/****************************************************************************
**
** Copyright (C) 1992-2009 Nokia. All rights reserved.
**
** This file is part of QtJambi.
**
** $BEGIN_LICENSE$
** Commercial Usage
** Licensees holding valid Qt Commercial licenses may use this file in
** accordance with the Qt Commercial License Agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Nokia.
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
** If you are unsure which license is appropriate for your use, please
** contact the sales department at qt-sales@nokia.com.
** $END_LICENSE$

**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

#ifndef ABSTRACTGENERATOR_H
#define ABSTRACTGENERATOR_H

#include "metalang.h"
#include "typesystem/utils.h"

#include "codemodel.h"

#include <QObject>
#include <QFile>
#include <QFlags>

class AbstractGenerator : public QObject {
    public:
        enum Option : quint64{
            NoOption                        = 0x000000000,
            BoxedPrimitive                  = 0x000000001,
            ExcludeConst                    = 0x000000002,
            ExcludeReference                = 0x000000004,
            UseNativeIds                    = 0x000000008,

            EnumAsInts                      = 0x000000010,
            SkipName                        = 0x000000020,
            NoCasts                         = 0x000000040,
            SkipReturnType                  = 0x000000080,
            OriginalName                    = 0x000000100,
            ShowStatic                      = 0x000000200,
            UnderscoreSpaces                = 0x000000400,
            ForceEnumCast                   = 0x000000800,
            ArrayAsPointer                  = 0x000001000,
            VirtualCall                     = 0x000002000,
            SkipTemplateParameters          = 0x000004000,
            SkipAttributes                  = 0x000008000,
            OriginalTypeDescription         = 0x000010000,
            SkipRemovedArguments            = 0x000020000,
            IncludeDefaultExpression        = 0x000040000,
            NoReturnStatement               = 0x000080000,
            NoBlockedSlot                   = 0x000100000,
            NormalizeAndFixTypeSignature    = 0x000200000,
            SuperCall                       = 0x000400000,
            GlobalRefJObject                = 0x000800000,
            DefaultFunction                 = 0x001000000,
            VarArgsAsArray                  = 0x002000000,
            CollectionAsCollection          = 0x004000000,
            NoQCollectionContainers         = 0x008000000,

            NoTmpVariable                   = 0x0010000000,
            DirectReturn                    = 0x0020000000,
            ForceConstReference             = 0x0040000000,
            ShowOverride                    = 0x0080000000,
            OptionalScope                   = 0x0100000000,
            SkipArray                       = 0x0200000000,
            EnumFromInt                     = 0x0400000000,
            InFunctionComment               = 0x0800000000,
            FunctionOverride                = 0x1000000000,
            NoEnumAsInts                    = 0x2000000000,
            NoSuppressExports               = 0x4000000000,
            JNIProxyFunction                = 0x8000000000,
            NoNullness                      = 0x10000000000,
            StrictNonNull                   = 0x20000000000,

            ForceValueType                  = (ExcludeReference | ExcludeConst) & ~ForceConstReference
        };

        AbstractGenerator();

        void setClasses(const MetaClassList &classes) { m_classes = classes; }
        const MetaClassList& classes() const { return m_classes; }

        virtual QString resolveOutputDirectory() const { return outputDirectory(); }

        const QString& outputDirectory() const { return m_out_dir; }
        void setOutputDirectory(const QString &outDir) { m_out_dir = outDir; }
        virtual void generate();
        void printClasses();

        int numGenerated() { return m_num_generated; }
        int numGeneratedAndWritten() { return m_num_generated_written; }

        virtual bool shouldGenerate(const MetaClass *) const;
        virtual bool shouldGenerate(const MetaFunctional *) const;
        virtual QString subDirectoryForClass(const MetaClass *java_class) const;
        virtual QString subDirectoryForFunctional(const MetaFunctional *java_class) const;
        virtual QString fileNameForClass(const MetaClass *java_class) const;
        virtual QString fileNameForFunctional(const MetaFunctional *java_class) const;
        virtual void write(QTextStream &s, const MetaClass *java_class, int nesting_level = 0);
        virtual void write(QTextStream &s, const MetaFunctional *java_class, int nesting_level = 0);
        virtual void generateFake(const MetaClass *java_class);

        bool hasDefaultConstructor(const MetaType *type);
        bool hasPublicDefaultConstructor(const MetaType *type);
        bool hasPublicAssignmentOperator(const MetaType *type);
        void setQtVersion(uint qtVersionMajor, uint qtVersionMinor, uint qtVersionPatch, uint qtjambiVersionPatch) {
            m_qtVersionMajor = qtVersionMajor;
            m_qtVersionMinor = qtVersionMinor;
            m_qtVersionPatch = qtVersionPatch;
            m_qtjambiVersionPatch = qtjambiVersionPatch;
        }

    protected:
        void writeInclude(QTextStream &s, const TS::Include &inc, QSet<QString> &dedupe);
        void verifyDirectoryFor(const QFile &file);
        static QString annotationFreeTypeName(QString typeName);
        static bool isCharSequenceSubstitute(const MetaType* type);

        MetaClassList m_classes;
        int m_num_generated;
        int m_num_generated_written;
        QString m_out_dir;
        uint m_qtVersionMajor;
        uint m_qtVersionMinor;
        uint m_qtVersionPatch;
        uint m_qtjambiVersionPatch;
};

class Indentor {
    public:
        Indentor():
                indent(0),
                m_string("    ")
        {
        }
        int indent;
        const QString &string() const {
            return m_string;
        }
        const QString &end() const {
            return m_end;
        }
        void setEnd(const QString & end){
            m_end = end;
        }
    private:
        QString m_string;
        QString m_end;
};

class Indentation {
    public:
        Indentation(Indentor &indentor, uint offset = 1):
                indentor(indentor), m_offset(offset) {
            indentor.indent+=offset;
        }
        ~Indentation() {
            indentor.indent-=m_offset;
        }
        const QString &string() const {
            return indentor.string();
        }

    private:
        Indentor &indentor;
        uint m_offset;
};

class IndentationReset {
    public:
        IndentationReset(Indentor &indentor):
                indentor(indentor) {
            indent = indentor.indent;
            indentor.indent = 0;
        }
        ~IndentationReset() {
            indentor.indent = indent;
        }
        const QString &string() const {
            return indentor.string();
        }

    private:
        Indentor &indentor;
        int indent;
};

#define INDENTATION(INDENT)\
    Indentation indent(INDENT);\
    Q_UNUSED(indent)
#define INDENTATIONRESET(INDENT)\
    IndentationReset indentReset(INDENT);\
    Q_UNUSED(indentReset)

inline QTextStream &operator <<(QTextStream &s, const Indentor &indentor) {
    s << indentor.end();
    for (int i = 0; i < indentor.indent; ++i)
        s << indentor.string();
    return s;
}


#endif // ABSTRACTGENERATOR_H
