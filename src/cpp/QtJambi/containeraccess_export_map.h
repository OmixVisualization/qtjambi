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

#ifndef CONTAINERACCESS_EXPORT_MAP_H
#define CONTAINERACCESS_EXPORT_MAP_H

#include "containeraccess_associative.h"
#include "qtjambi_cast_template2.h"
#include "qtjambi_cast_arithmetic.h"
#include "qtjambi_cast_template1.h"
#include "containeraccess_export_list.h"
#include "containeraccess_export_stringlist.h"
#include "containeraccess_export_bytearraylist.h"
#include "containeraccess_export_variantlist.h"
#include "containeraccess_export_pair.h"

namespace QtJambiPrivate{
template<typename T>
struct QMapExport{};
} // namespace QtJambiPrivate

#if defined(Q_CC_MSVC) || defined(_LIBCPP_VERSION) || !defined(Q_OS_WIN)
extern template class QTJAMBI_TEMPLATE_EXPORT QMapAccess<qint32,qint32>;

#ifdef QVARIANT_H
extern template class QTJAMBI_TEMPLATE_EXPORT QMapAccess<qint32,QVariant>;
#ifdef QSTRING_H
extern template class QTJAMBI_TEMPLATE_EXPORT QMapAccess<QString,QVariant>;
#endif
#ifdef QBYTEARRAY_H
extern template class QTJAMBI_TEMPLATE_EXPORT QMapAccess<QByteArray,QVariant>;
#endif
#endif

#ifdef QSTRING_H
extern template class QTJAMBI_TEMPLATE_EXPORT QMapAccess<QString,QString>;
#endif
#ifdef QBYTEARRAY_H
extern template class QTJAMBI_TEMPLATE_EXPORT QMapAccess<QByteArray,QByteArray>;
#endif
#endif

#endif // CONTAINERACCESS_EXPORT_MAP_H
