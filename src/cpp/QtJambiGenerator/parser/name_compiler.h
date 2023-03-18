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


#ifndef NAME_COMPILER_H
#define NAME_COMPILER_H

#include "default_visitor.h"
#include <QtCore/QStringList>
#include <codemodel.h>

class TokenStream;
class Binder;

class NameCompiler: protected DefaultVisitor {
    public:
        NameCompiler(Binder *binder);

        void run(NameAST *node) { internal_run(node); }
        void run(UnqualifiedNameAST *node) { internal_run(node); }

        QString name() const { return _M_name.join("::"); }
        const QStringList& templateArgs() const { return _M_templateArgs; }
        const QStringList& qualifiedName() const { return _M_name; }
        const TypeInfo& functionalReturnType() const { return _M_functionalReturnType; }
        const TypeInfo& operatorCastType() const { return _M_operatorCastType; }
        OperatorType operatorType() const { return _M_operatorType; }
        const QList<TypeInfo>& templateArgumentTypes() const { return _M_templateArgumentTypes; }
        const QList<TypeInfo>& functionalArgumentTypes() const { return _M_functionalArgumentTypes; }
        const QList<QString>& functionalArgumentNames() const { return _M_functionalArgumentNames; }

    protected:
        virtual void visitUnqualifiedName(UnqualifiedNameAST *node);
        virtual void visitTemplateArgument(TemplateArgumentAST *node);

        void internal_run(AST *node);
        QString decode_operator(std::size_t index, unsigned opLength) const;

    private:
        Binder *_M_binder;
        TokenStream *_M_token_stream;
        QStringList _M_name;
        QStringList _M_templateArgs;
        QList<TypeInfo> _M_templateArgumentTypes;
        TypeInfo _M_functionalReturnType;
        OperatorType _M_operatorType;
        TypeInfo _M_operatorCastType;
        QList<TypeInfo> _M_functionalArgumentTypes;
        QList<QString> _M_functionalArgumentNames;
};

#endif // NAME_COMPILER_H

// kate: space-indent on; indent-width 2; replace-tabs on;
