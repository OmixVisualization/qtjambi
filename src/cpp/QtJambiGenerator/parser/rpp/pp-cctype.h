/****************************************************************************
**
** Copyright (C) 1992-2009 Nokia. All rights reserved.
** Copyright 2005 Roberto Raggi <roberto@kdevelop.org>
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
** If you are unsure which license is appropriate for your use, please
** contact the sales department at qt-sales@nokia.com.
** $END_LICENSE$

**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

#ifndef PP_CCTYPE_H
#define PP_CCTYPE_H

#include <cctype>

namespace rpp {

    inline bool pp_isalpha(int __ch)
    { return std::isalpha(static_cast<unsigned char>(__ch)) != 0; }

    inline bool pp_isalnum(int __ch)
    { return std::isalnum(static_cast<unsigned char>(__ch)) != 0; }

    inline bool pp_isdigit(int __ch)
    { return std::isdigit(static_cast<unsigned char>(__ch)) != 0; }

    inline bool pp_isspace(int __ch)
    { return std::isspace(static_cast<unsigned char>(__ch)) != 0; }

} // namespace rpp

#endif // PP_CCTYPE_H
