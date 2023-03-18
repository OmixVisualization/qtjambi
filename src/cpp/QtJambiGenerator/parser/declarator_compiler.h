/****************************************************************************
**
** Copyright (C) 1992-2009 Nokia. All rights reserved.
** Copyright (C) 2002-2005 Roberto Raggi <roberto@kdevelop.org>
** Copyright (C) 2009-2023 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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


#ifndef DECLARATOR_COMPILER_H
#define DECLARATOR_COMPILER_H

#include "default_visitor.h"
#include "codemodel.h"

#include <QtCore/QString>
#include <QtCore/QList>

class TokenStream;
class Binder;

class DeclaratorCompiler: protected DefaultVisitor {
    public:

        enum ReferenceType{
            NoReference,
            Reference,
            RReference
        };

        struct Parameter {
            TypeInfo type;
            QString name;
            QString defaultValueExpression;
            bool defaultValue;

            Parameter(): defaultValue(false) {}
        };

    public:
        DeclaratorCompiler(Binder *binder);

        void run(DeclaratorAST *node);

        inline const QString& id() const { return _M_id; }
        inline const QStringList& arrayElements() const { return _M_array; }
        inline bool isFunction() const { return _M_function; }
        inline bool isVariadics() const { return _M_variadics; }
        inline ReferenceType getReferenceType() const { return _M_reference_type; }
        inline const QList<bool>& indirection() const { return _M_indirection; }
        inline const QList<Parameter>& parameters() const { return _M_parameters; }
        OperatorType operatorType() const { return _M_operatorType; }
        const TypeInfo& operatorCastType() const { return _M_operatorCastType; }

    protected:
        virtual void visitPtrOperator(PtrOperatorAST *node);
        virtual void visitParameterDeclaration(ParameterDeclarationAST *node);

    private:
        Binder *_M_binder;
        TokenStream *_M_token_stream;

        bool _M_function;
        ReferenceType _M_reference_type;
        bool _M_variadics;
        /**
         * the size of this array represents the number of indirections of the type.
         * the individual entry 'false' it represents a *.
         * the individual entry 'true' it represents a * const.
         */
        QList<bool> _M_indirection;
        QString _M_id;
        OperatorType _M_operatorType;
        TypeInfo _M_operatorCastType;
        QStringList _M_array;
        QList<Parameter> _M_parameters;
};

#endif // DECLARATOR_COMPILER_H

// kate: space-indent on; indent-width 2; replace-tabs on;
