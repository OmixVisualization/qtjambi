/****************************************************************************
**
** Copyright (C) 2009-2023 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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

#ifndef UTILS_P_H
#define UTILS_P_H

#include <QtJambi/QtJambiAPI>
#include <QtJambi/JavaAPI>

namespace Java{
namespace QtPositioning {
    QTJAMBI_REPOSITORY_DECLARE_CLASS(QNmeaPositionInfoSource$Result,
                                     QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR()
                                     QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(info)
                                     QTJAMBI_REPOSITORY_DECLARE_BOOLEAN_METHOD(hasFix))
}
namespace QtJambi {
    QTJAMBI_REPOSITORY_DECLARE_CLASS(SignalUtility$AbstractSignal,
                                     QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(containingObject)
                                     QTJAMBI_REPOSITORY_DECLARE_INT_METHOD(methodIndex))
}
namespace QtCore {
    QTJAMBI_REPOSITORY_DECLARE_CLASS(QMetaMethod,
                                     QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(toSignal))
}
}

#endif // UTILS_P_H
