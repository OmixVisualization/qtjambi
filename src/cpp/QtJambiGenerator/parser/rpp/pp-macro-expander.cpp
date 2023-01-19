/****************************************************************************
**
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

#include "rpp/pp-macro-expander.h"

std::string const *rpp::pp_macro_expander::resolve_formal(pp_fast_string const *name) {
    assert(name != nullptr);

    if (! frame)
        return nullptr;

    assert(frame->expanding_macro != nullptr);

    std::vector<pp_fast_string const *> const formals = frame->expanding_macro->formals;
    for (std::size_t index = 0; index < formals.size(); ++index) {
        pp_fast_string const *formal = formals[index];

        if (*formal != *name)
            continue;

        else if (frame->actuals && index < frame->actuals->size())
            return &(*frame->actuals)[index];

        else{
            env.log(QString("Macro %1 expects %2 arguments. Given: %3").arg(frame->expanding_macro->name->begin()).arg(formals.size()).arg(frame->actuals ? frame->actuals->size() : 0).toStdString());
            exit(-1);
            // internal error?
        }
    }

    return nullptr;
}
