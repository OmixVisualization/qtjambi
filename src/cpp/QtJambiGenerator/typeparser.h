/****************************************************************************
**
** Copyright (C) 1992-2009 Nokia. All rights reserved.
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

#ifndef TYPEPARSER_H
#define TYPEPARSER_H

#include <QList>
#include <QString>
#include <QStringList>
#include <QStack>
#include "parser/codemodel.h"

class Scanner;
class TypeParser {

    public:
        struct Info {
            enum ReferenceType{
                NoReference,
                Reference,
                RReference
            };

            Info() : reference_type(NoReference), is_constant(false), is_volatile(false), is_variadics(false), is_functionPointer(false), is_busted(false), indirections() { }

            QStringList qualified_name;
            /**
             * List of different parts of “array”, or [foo]
             */
            QStringList arrays;
            QList<Info> arguments;
            ReferenceType reference_type;
            uint is_constant : 1;
            uint is_volatile : 1;
            uint is_variadics : 1;
            uint is_functionPointer : 1;
            /**
             * If the type is not supported / valid
             */
            uint is_busted : 1;
            /**
             * the size of this array represents the number of indirections of the type.
             * the individual entry 'false' it represents a *.
             * the individual entry 'true' it represents a * const.
             */
            QList<bool> indirections;

            QString originalType;
            QList<Info> functionalReturnType;
            QList<Info> functionalArgumentTypes;

            QString toString() const;
            QString instantiationName() const; //private?

            bool operator!()const{return qualified_name.isEmpty() && functionalReturnType.isEmpty();}
            operator bool()const{return !qualified_name.isEmpty() || !functionalReturnType.isEmpty();}
        };

        /**
         * Loops through the text token by token and returns the data
         */
        static Info parse(const QString &str);

    private:
        static void parseIdentifier(Scanner &scanner, QStack<Info *> &stack, QString &array, bool in_array, bool &colon_prefix);
};

#endif // TYPEPARSER_H
