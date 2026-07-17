/****************************************************************************
**
** Copyright (C) 2009-2026 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
**
** This file is part of Qt Jambi.
**
** $BEGIN_LICENSE$
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
** $END_LICENSE$
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

#ifndef CONTAINERACCESS_EXPORT_LIST_H
#define CONTAINERACCESS_EXPORT_LIST_H

#include "containeraccess_sequential.h"
#include "qtjambi_cast_arithmetic.h"
#include "qtjambi_cast_template1.h"

namespace QtJambiPrivate{
template<typename T>
struct QListExport{};
} // namespace QtJambiPrivate

#if defined(Q_CC_MSVC) || defined(_LIBCPP_VERSION) || !defined(Q_OS_WIN)
extern template class QTJAMBI_TEMPLATE_EXPORT QSpanAccess<bool>;
extern template class QTJAMBI_TEMPLATE_EXPORT QSpanAccess<const bool>;
extern template class QTJAMBI_TEMPLATE_EXPORT QListAccess<bool>;

extern template class QTJAMBI_TEMPLATE_EXPORT QSpanAccess<qint8>;
extern template class QTJAMBI_TEMPLATE_EXPORT QSpanAccess<const qint8>;
extern template class QTJAMBI_TEMPLATE_EXPORT QListAccess<qint8>;

extern template class QTJAMBI_TEMPLATE_EXPORT QSpanAccess<qint16>;
extern template class QTJAMBI_TEMPLATE_EXPORT QSpanAccess<const qint16>;
extern template class QTJAMBI_TEMPLATE_EXPORT QListAccess<qint16>;

extern template class QTJAMBI_TEMPLATE_EXPORT QSpanAccess<qint32>;
extern template class QTJAMBI_TEMPLATE_EXPORT QSpanAccess<const qint32>;
extern template class QTJAMBI_TEMPLATE_EXPORT QListAccess<qint32>;

extern template class QTJAMBI_TEMPLATE_EXPORT QSpanAccess<qint64>;
extern template class QTJAMBI_TEMPLATE_EXPORT QSpanAccess<const qint64>;
extern template class QTJAMBI_TEMPLATE_EXPORT QListAccess<qint64>;

extern template class QTJAMBI_TEMPLATE_EXPORT QSpanAccess<const double>;
extern template class QTJAMBI_TEMPLATE_EXPORT QListAccess<double>;
extern template class QTJAMBI_TEMPLATE_EXPORT QSpanAccess<double>;

extern template class QTJAMBI_TEMPLATE_EXPORT QSpanAccess<float>;
extern template class QTJAMBI_TEMPLATE_EXPORT QSpanAccess<const float>;
extern template class QTJAMBI_TEMPLATE_EXPORT QListAccess<float>;

extern template class QTJAMBI_TEMPLATE_EXPORT QSpanAccess<char16_t>;
extern template class QTJAMBI_TEMPLATE_EXPORT QSpanAccess<const char16_t>;
extern template class QTJAMBI_TEMPLATE_EXPORT QListAccess<char16_t>;

extern template class QTJAMBI_TEMPLATE_EXPORT QSpanAccess<char32_t>;
extern template class QTJAMBI_TEMPLATE_EXPORT QSpanAccess<const char32_t>;
extern template class QTJAMBI_TEMPLATE_EXPORT QListAccess<char32_t>;
#ifdef QCHAR_H
extern template class QTJAMBI_TEMPLATE_EXPORT QSpanAccess<QChar>;
extern template class QTJAMBI_TEMPLATE_EXPORT QSpanAccess<const QChar>;
extern template class QTJAMBI_TEMPLATE_EXPORT QListAccess<QChar>;
#endif
#endif

#endif // CONTAINERACCESS_EXPORT_LIST_H
