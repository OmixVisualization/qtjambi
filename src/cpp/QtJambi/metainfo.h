/****************************************************************************
**
** Copyright (C) 2009-2023 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
**
** This file is part of Qt Jambi.
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

#ifndef QTJAMBI_METAINFO_H
#define QTJAMBI_METAINFO_H

#include "qtjambiapi.h"
#if defined(JNI_VERSION_1_8)
#define QTJAMBI_ONLOAD_RETURN JNI_VERSION_1_8
#elif defined(JNI_VERSION_1_6)
#define QTJAMBI_ONLOAD_RETURN JNI_VERSION_1_6
#else
#define QTJAMBI_ONLOAD_RETURN JNI_VERSION_1_5
#endif

#ifdef Q_OS_IOS
#ifndef LIBRARY_NAME
#error "LIBRARY_NAME not defined"
#endif
#define JNI_ONLOAD JNI_OnLoad_##LIBRARY_NAME
#else
#define JNI_ONLOAD QTJAMBI_FUNCTION_PREFIX(JNI_OnLoad)
#endif

namespace MetaInfoAPI{

QTJAMBI_EXPORT jobject internalAccess(JNIEnv *env, jclass cls);

}

#endif // QTJAMBI_METAINFO_H
