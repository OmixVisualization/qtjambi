/****************************************************************************
**
** Copyright (C) 1992-2009 Nokia. All rights reserved.
** Copyright 2005 Roberto Raggi <roberto@kdevelop.org>
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

#ifndef PP_MACRO_H
#define PP_MACRO_H

#include "pp-fwd.h"

#include <vector>

namespace rpp {

    struct pp_macro {
#if defined (PP_WITH_MACRO_POSITION)
        pp_fast_string const *file;
#endif
        pp_fast_string const *name;
        pp_fast_string const *definition;
        std::vector<pp_fast_string const *> formals;

        union {
            int unsigned state;

            struct {
            int unsigned hidden: 1;
            int unsigned function_like: 1;
            int unsigned variadics: 1;
            };
        };

        int lines;
        pp_macro *next;
        std::size_t hash_code;

        inline pp_macro() :
#if defined (PP_WITH_MACRO_POSITION)
                file(0),
#endif
                name(nullptr),
                definition(nullptr),
                state(0),
                lines(0),
                next(nullptr),
                hash_code(0) {}
    };

} // namespace rpp

#endif // PP_MACRO_H
