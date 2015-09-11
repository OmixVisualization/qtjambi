/****************************************************************************
**
** Copyright (C) 1992-2009 Nokia. All rights reserved.
** Copyright (C) 2009-2015 Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
**
** This file is part of Qt Jambi.
**
** ** $BEGIN_LICENSE$
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
** $END_LICENSE$
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

#ifndef QTJAMBIVARIANT_H
#define QTJAMBIVARIANT_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Qt Jambi API.
// This header file may change from version to version without notice,
// or even be removed.
//
// We mean it.
//
//

#include <QtCore/QVariant>
#include <QtCore/private/qvariant_p.h>
#include "qtjambi_core.h"

class QtJambiVariant: private QVariant
{

 public:
    static const QVariant::Handler *getLastHandler()
        {
            return lastHandler;
        }

    static int registerHandler()
        {
            lastHandler = &qt_custom_variant_handler;

            setHandler(&handler);
            return 1;
        }

    static int unregisterHandler()
        {
            setHandler(lastHandler);
            lastHandler = 0;
            return 1;
        }

 private:
    static const QVariant::Handler handler;
    static const QVariant::Handler qt_custom_variant_handler;

    static void setHandler(const Handler *handler) {
        Q_UNUSED(handler)
        QVariantPrivate::registerHandler(3, handler);
    }

    static const QVariant::Handler *lastHandler;

};

#endif // QTJAMBIVARIANT_H
