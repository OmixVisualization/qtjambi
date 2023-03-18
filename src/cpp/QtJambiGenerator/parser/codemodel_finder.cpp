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


#include "codemodel_finder.h"
#include "codemodel.h"
#include "binder.h"

CodeModelFinder::CodeModelFinder(CodeModelPtr model, Binder *binder)
        : _M_model(model),
        _M_binder(binder),
        _M_token_stream(binder->tokenStream()),
        name_cc(_M_binder),
        _M_resolve_policy(ResolveItem) {
}

CodeModelFinder::~CodeModelFinder() {
}

ScopeModelItem CodeModelFinder::resolveScope(NameAST *name, ScopeModelItem scope) {
    Q_ASSERT(scope != nullptr);

    ResolvePolicy saved_resolve_policy = _M_resolve_policy;
    _M_resolve_policy = ResolveScope;

    ScopeModelItem old = changeCurrentScope(scope);

    visit(name);
    ScopeModelItem result = _M_current_scope;

    changeCurrentScope(old); // restore

    _M_resolve_policy = saved_resolve_policy;

    return result;
}

ScopeModelItem CodeModelFinder::changeCurrentScope(ScopeModelItem scope) {
    ScopeModelItem old = _M_current_scope;
    _M_current_scope = scope;
    return old;
}

void CodeModelFinder::visitName(NameAST *node) {
    visitNodes(this, node->qualified_names);

    if (_M_resolve_policy == ResolveItem)
        visit(node->unqualified_name);
}

void CodeModelFinder::visitUnqualifiedName(UnqualifiedNameAST *node) {
    if (!_M_current_scope) {
        // nothing to do
        return;
    }

    name_cc.run(node);
    QString id = name_cc.name();

    if (ClassModelItem klass = _M_current_scope->findClass(id)) {
        _M_current_scope = klass;
    } else if (NamespaceModelItem parentNamespace = model_safe_cast<NamespaceModelItem>(_M_current_scope)) {
        NamespaceModelItem ns = parentNamespace->findNamespace(id);
        _M_current_scope = model_static_cast<ScopeModelItem>(ns);
    } else if (FileModelItem file = model_safe_cast<FileModelItem>(_M_current_scope)) {
        NamespaceModelItem ns = file->findNamespace(id);
        _M_current_scope = model_static_cast<ScopeModelItem>(ns);
    }
}

// kate: space-indent on; indent-width 2; replace-tabs on;

