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


#ifndef CODEMODEL_FWD_H
#define CODEMODEL_FWD_H

#include <QtCore/QList>
#include <QtCore/QSharedDataPointer>

// forward declarations
class CodeModel;
class _ArgumentModelItem;
class _ClassModelItem;
class _CodeModelItem;
class _EnumModelItem;
class _EnumeratorModelItem;
class _FileModelItem;
class _FunctionDefinitionModelItem;
class _FunctionModelItem;
class _NamespaceModelItem;
class _ScopeModelItem;
class _TemplateParameterModelItem;
class _TypeAliasModelItem;
class _VariableModelItem;
class _MemberModelItem;
class TypeInfo;

typedef QExplicitlySharedDataPointer<CodeModel> CodeModelPtr;
typedef QExplicitlySharedDataPointer<_ArgumentModelItem> ArgumentModelItem;
typedef QExplicitlySharedDataPointer<_ClassModelItem> ClassModelItem;
typedef QExplicitlySharedDataPointer<_CodeModelItem> CodeModelItem;
typedef QExplicitlySharedDataPointer<_EnumModelItem> EnumModelItem;
typedef QExplicitlySharedDataPointer<_EnumeratorModelItem> EnumeratorModelItem;
typedef QExplicitlySharedDataPointer<_FileModelItem> FileModelItem;
typedef QExplicitlySharedDataPointer<_FunctionDefinitionModelItem> FunctionDefinitionModelItem;
typedef QExplicitlySharedDataPointer<_FunctionModelItem> FunctionModelItem;
typedef QExplicitlySharedDataPointer<_NamespaceModelItem> NamespaceModelItem;
typedef QExplicitlySharedDataPointer<_ScopeModelItem> ScopeModelItem;
typedef QExplicitlySharedDataPointer<_TemplateParameterModelItem> TemplateParameterModelItem;
typedef QExplicitlySharedDataPointer<_TypeAliasModelItem> TypeAliasModelItem;
typedef QExplicitlySharedDataPointer<_VariableModelItem> VariableModelItem;
typedef QExplicitlySharedDataPointer<_MemberModelItem> MemberModelItem;

typedef QList<ArgumentModelItem> ArgumentList;
typedef QList<ClassModelItem> ClassList;
typedef QList<CodeModelItem> ItemList;
typedef QList<EnumModelItem> EnumList;
typedef QList<EnumeratorModelItem> EnumeratorList;
typedef QList<FileModelItem> FileList;
typedef QList<FunctionDefinitionModelItem> FunctionDefinitionList;
typedef QList<FunctionModelItem> FunctionList;
typedef QList<NamespaceModelItem> NamespaceList;
typedef QList<ScopeModelItem> ScopeList;
typedef QList<TemplateParameterModelItem> TemplateParameterList;
typedef QList<TypeAliasModelItem> TypeAliasList;
typedef QList<VariableModelItem> VariableList;
typedef QList<MemberModelItem> MemberList;

#endif // CODEMODEL_FWD_H
