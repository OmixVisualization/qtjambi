/****************************************************************************
**
** Copyright (C) 2009-2026 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
** Copyright (C) 1992-2009 Nokia. All rights reserved.
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
        enum Option : qint64{
            NoOption                        = 0x000000000LL,
            BoxedPrimitive                  = 0x000000001LL,
            ExcludeConst                    = 0x000000002LL,
            ExcludeReference                = 0x000000004LL,
            UseNativeIds                    = 0x000000008LL,

            EnumAsInts                      = 0x000000010LL,
            SkipName                        = 0x000000020LL,
            NoCasts                         = 0x000000040LL,
            SkipReturnType                  = 0x000000080LL,
            OriginalName                    = 0x000000100LL,
            ShowStatic                      = 0x000000200LL,
            UnderscoreSpaces                = 0x000000400LL,
            ForceEnumCast                   = 0x000000800LL,
            ArrayAsPointer                  = 0x000001000LL,
            VirtualCall                     = 0x000002000LL,
            SkipTemplateParameters          = 0x000004000LL,
            SkipAttributes                  = 0x000008000LL,
            OriginalTypeDescription         = 0x000010000LL,
            SkipRemovedArguments            = 0x000020000LL,
            IncludeDefaultExpression        = 0x000040000LL,
            NoReturnStatement               = 0x000080000LL,
            NoBlockedSlot                   = 0x000100000LL,
            NormalizeAndFixTypeSignature    = 0x000200000LL,
            SuperCall                       = 0x000400000LL,
            GlobalRefJObject                = 0x000800000LL,
            DefaultFunction                 = 0x001000000LL,
            VarArgsAsArray                  = 0x002000000LL,
            CollectionAsCollection          = 0x004000000LL,
            NoQCollectionContainers         = 0x008000000LL,

            NoTmpVariable                   = 0x0010000000LL,
            DirectReturn                    = 0x0020000000LL,
            ForceConstReference             = 0x0040000000LL,
            ShowOverride                    = 0x0080000000LL,
            OptionalScope                   = 0x0100000000LL,
            SkipArray                       = 0x0200000000LL,
            EnumFromInt                     = 0x0400000000LL,
            InFunctionComment               = 0x0800000000LL,
            FunctionOverride                = 0x1000000000LL,
            NoEnumAsInts                    = 0x2000000000LL,
            NoSuppressExports               = 0x4000000000LL,
            JNIProxyFunction                = 0x8000000000LL,
            NoNullness                      = 0x10000000000LL,
            StrictNonNull                   = 0x20000000000LL,
            PlainShell                      = 0x40000000000LL,
            MShell                          = 0x80000000000LL,
            IsReturnType                    = 0x100000000000LL,
            IsNativeCall                    = 0x200000000000LL,
            NoSpace                         = 0x400000000000LL,

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

#define INDENTATION(INDENT) Indentation indent(INDENT)
#define INDENTATIONRESET(INDENT) IndentationReset indentReset(INDENT)

inline QTextStream &operator <<(QTextStream &s, const Indentor &indentor) {
    s << indentor.end();
    for (int i = 0; i < indentor.indent; ++i)
        s << indentor.string();
    return s;
}


#endif // ABSTRACTGENERATOR_H
