/****************************************************************************
**
** Copyright (C) 1992-2009 Nokia. All rights reserved.
**
** This file is part of Qt Jambi.
**
** ** $BEGIN_LICENSE$
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

#ifndef GENERATOR_H
#define GENERATOR_H

#include "metajava.h"
#include "typesystem/typesystem.h"

#include "codemodel.h"

#include <QObject>
#include <QFile>

class Generator : public QObject {
        Q_OBJECT

        Q_PROPERTY(QString outputDirectory READ outputDirectory WRITE setOutputDirectory);

    public:
        enum Option {
            NoOption                        = 0x00000000,
            BoxedPrimitive                  = 0x00000001,
            ExcludeConst                    = 0x00000002,
            ExcludeReference                = 0x00000004,
            UseNativeIds                    = 0x00000008,

            EnumAsInts                      = 0x00000010,
            SkipName                        = 0x00000020,
            NoCasts                         = 0x00000040,
            SkipReturnType                  = 0x00000080,
            OriginalName                    = 0x00000100,
            ShowStatic                      = 0x00000200,
            UnderscoreSpaces                = 0x00000400,
            ForceEnumCast                   = 0x00000800,
            ArrayAsPointer                  = 0x00001000,
            VirtualCall                     = 0x00002000,
            SkipTemplateParameters          = 0x00004000,
            SkipAttributes                  = 0x00008000,
            OriginalTypeDescription         = 0x00010000,
            SkipRemovedArguments            = 0x00020000,
            IncludeDefaultExpression        = 0x00040000,
            NoReturnStatement               = 0x00080000,
            NoBlockedSlot                   = 0x00100000,
            NormalizeAndFixTypeSignature    = 0x00200000,
            SuperCall                       = 0x00400000,
            GlobalRefJObject                = 0x00800000,

            ForceValueType                  = ExcludeReference | ExcludeConst
        };

        Generator();

        void setClasses(const AbstractMetaClassList &classes) { m_classes = classes; }
        AbstractMetaClassList classes() const { return m_classes; }

        virtual QString resolveOutputDirectory() const { return outputDirectory(); }

        QString outputDirectory() const { return m_out_dir; }
        void setOutputDirectory(const QString &outDir) { m_out_dir = outDir; }
        virtual void generate();
        void printClasses();

        int numGenerated() { return m_num_generated; }
        int numGeneratedAndWritten() { return m_num_generated_written; }

        virtual bool shouldGenerate(const AbstractMetaClass *) const { return true; }
        virtual QString subDirectoryForClass(const AbstractMetaClass *java_class) const;
        virtual QString fileNameForClass(const AbstractMetaClass *java_class) const;
        virtual void write(QTextStream &s, const AbstractMetaClass *java_class, int nesting_level = 0);

        bool hasDefaultConstructor(const AbstractMetaType *type);


    protected:
        void verifyDirectoryFor(const QFile &file);

        AbstractMetaClassList m_classes;
        int m_num_generated;
        int m_num_generated_written;
        QString m_out_dir;
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
    private:
        QString m_string;
};

class Indentation {
    public:
        Indentation(Indentor &indentor):
                indentor(indentor) {
            indentor.indent++;
        }
        ~Indentation() {
            indentor.indent--;
        }
        const QString &string() const {
            return indentor.string();
        }

    private:
        Indentor &indentor;
};

inline QTextStream &operator <<(QTextStream &s, const Indentor &indentor) {
    for (int i = 0; i < indentor.indent; ++i)
        s << indentor.string();
    return s;
}


#endif // GENERATOR_H
