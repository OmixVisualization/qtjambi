/****************************************************************************
**
** Copyright (C) 2009-2022 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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

#include <QtCore/QMutex>
#include "utils_p.h"

namespace Java{
namespace QtLocation{
QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/location,QGeoServiceProviderFactory$Result,
    QTJAMBI_REPOSITORY_DEFINE_CONSTRUCTOR(Lio/qt/core/QObject;Lio/qt/location/QGeoServiceProvider$Error;Ljava/lang/String;)
    QTJAMBI_REPOSITORY_DEFINE_METHOD(engine,()Lio/qt/core/QObject;)
    QTJAMBI_REPOSITORY_DEFINE_METHOD(error,()Lio/qt/location/QGeoServiceProvider$Error;)
    QTJAMBI_REPOSITORY_DEFINE_METHOD(errorString,()Ljava/lang/String;)
)
}
}
