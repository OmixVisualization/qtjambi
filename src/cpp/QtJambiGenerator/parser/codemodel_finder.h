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


#ifndef CODEMODEL_FINDER_H
#define CODEMODEL_FINDER_H

#include "default_visitor.h"
#include "codemodel_fwd.h"
#include "name_compiler.h"

class TokenStream;
class Binder;

class CodeModelFinder: protected DefaultVisitor {
        enum ResolvePolicy {
            ResolveScope,
            ResolveItem
        };

    public:
        CodeModelFinder(CodeModelPtr model, Binder *binder);
        virtual ~CodeModelFinder();

        ScopeModelItem resolveScope(NameAST *name, ScopeModelItem scope);

        inline CodeModelPtr model() const { return _M_model; }

    protected:
        virtual void visitName(NameAST *node);
        virtual void visitUnqualifiedName(UnqualifiedNameAST *node);

        ScopeModelItem changeCurrentScope(ScopeModelItem scope);

    private:
        CodeModelPtr _M_model;
        Binder *_M_binder;
        TokenStream *_M_token_stream;
        NameCompiler name_cc;

        ScopeModelItem _M_current_scope;
        ResolvePolicy _M_resolve_policy;
};

#endif // CODEMODEL_FINDER_H

// kate: space-indent on; indent-width 2; replace-tabs on;
