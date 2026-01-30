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
** 
** $END_LICENSE$
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

#ifndef PCH_P_H
#define PCH_P_H

#include <QtCore/QtCore>
#include "qtjambiapi.h"
#ifdef Q_OS_ANDROID
#include "androidapi.h"
#endif
#include "containerapi.h"
#include "containeraccess.h"
#include "containeraccess_p.h"
#include "coreapi.h"
#include "debugapi.h"
#include "exception.h"
#include "functionalbase.h"
#include "functionpointer.h"
#include "global.h"
#include "guiapi.h"
#include "java_p.h"
#include "javaapi.h"
#include "javaarrays.h"
#include "javabuffers.h"
#include "javainvalidate.h"
#include "javastrings.h"
#include "javautils.h"
#include "jnienvironment.h"
#include "jobjectwrapper.h"
#include "metainfo.h"
#include "modelapi.h"
#include "objectdata.h"
#include "paint.h"
#include "qmlapi.h"
#include "qtjambimetaobject_p.h"
#include "qtjambilink_p.h"
#include "qtjambishell.h"
#include "qtjambishell_p.h"
#include "registryapi.h"
#include "registryutil_p.h"
#include "scope.h"
#include "supertypeinfo_p.h"
#include "testapi.h"
#include "threadapi.h"
#include "threadutils_p.h"
#include "typeentry_p.h"
#include "typemanager_p.h"
#include "typetests.h"
#include "typeutils.h"
#include "utils.h"
#include "utils_p.h"
#include <stdlib.h>
#include <unordered_set>
#include <unordered_map>
#include <set>
#include <map>
#include <cstring>
#include <stdio.h>
#include <string.h>
#include <thread>
#include <sstream>
#include <memory>
#include <initializer_list>
#include <typeindex>
#include <typeinfo>
#include "qtjambi_cast.h"

#endif // PCH_P_H
