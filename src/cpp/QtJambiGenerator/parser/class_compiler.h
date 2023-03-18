/****************************************************************************
**
** Copyright (C) 1992-2009 Nokia. All rights reserved.
** Copyright (C) 2002-2005 Roberto Raggi <roberto@kdevelop.org>
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


#ifndef CLASS_COMPILER_H
#define CLASS_COMPILER_H

#include <QtCore/qglobal.h>
#include <QtCore/QStringList>

#include "default_visitor.h"
#include "name_compiler.h"
#include "type_compiler.h"
#include "lexer.h"

class TokenStream;
class Binder;

class ClassCompiler: protected DefaultVisitor {
    public:
        ClassCompiler(Binder *binder);
        ~ClassCompiler() override;
        const QString& name() const;
        const QList<TypeInfo>& templateArgumentTypes() const;
        const QList<QPair<TypeInfo,bool>>& baseClasses() const;
        void run(ClassSpecifierAST *node);
    protected:
        void visitBaseSpecifier(BaseSpecifierAST *node) override;
    private:
        bool m_isClass;
        Binder *_M_binder;
        TokenStream *_M_token_stream;
        QString _M_name;
        QList<TypeInfo> _M_templateArgumentTypes;
        QList<QPair<TypeInfo,bool>> _M_base_classes;
        NameCompiler name_cc;
        TypeCompiler type_cc;
};

class EnumCompiler: protected DefaultVisitor {
    public:
        EnumCompiler(Binder *binder);
        ~EnumCompiler() override;
        const QString& name() const;
        const TypeInfo& baseType() const;
        void run(EnumSpecifierAST *node);
    private:
        Binder *_M_binder;
        TokenStream *_M_token_stream;
        QString _M_name;
        TypeInfo _M_base_type;
        NameCompiler name_cc;
        TypeCompiler type_cc;
};

#endif // CLASS_COMPILER_H

// kate: space-indent on; indent-width 2; replace-tabs on;
