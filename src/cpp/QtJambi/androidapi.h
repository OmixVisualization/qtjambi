/****************************************************************************
**
** Copyright (C) 2009-2024 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
**
** This file is part of Qt Jambi.
**
** $BEGIN_LICENSE$
** GNU Lesser General Public License Usage
** This file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review Wthe following information to
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

#if !defined(QTJAMBI_ANDROIDAPI_H) && !defined(QTJAMBI_GENERATOR_RUNNING)
#define QTJAMBI_ANDROIDAPI_H

#include "global.h"

#ifdef Q_OS_ANDROID
#include "javautils.h"

QT_WARNING_DISABLE_CLANG("-Wdollar-in-identifier-extension")

namespace Java{
namespace Android{
QTJAMBI_REPOSITORY_DECLARE_CLASS(QtNative,
                                 QTJAMBI_REPOSITORY_DECLARE_STATIC_OBJECT_METHOD(activity)
                                 QTJAMBI_REPOSITORY_DECLARE_STATIC_OBJECT_METHOD(service)
                                 QTJAMBI_REPOSITORY_DECLARE_STATIC_OBJECT_METHOD(getContext))
}
}

#endif //def Q_OS_ANDROID

#endif // QTJAMBI_ANDROIDAPI_H
