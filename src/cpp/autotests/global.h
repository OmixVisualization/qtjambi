/****************************************************************************
**
** Copyright (C) 1992-2009 Nokia. All rights reserved.
**
** This file is part of Qt Jambi.
**
** ** $BEGIN_LICENSE$
** Commercial Usage
** Licensees holding valid Qt Commercial licenses may use this file in
** accordance with the Qt Commercial License Agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Nokia.
** 
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
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
** If you are unsure which license is appropriate for your use, please
** contact the sales department at qt-sales@nokia.com.
** $END_LICENSE$

**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

#ifndef GLOBAL_H
#define GLOBAL_H

// QT_NO_STL removes extra generator parser noise (that it doesn't understand anyway)
#define QT_NO_STL
#include <QtCore/QtGlobal>
#ifndef QTJAMBI_GENERATOR_RUNNING
#include <QtJambi/global.h>
#endif
#include <QtCore/QtCore>
#ifndef QTJAMBI_NO_WIDGETS
#include <QtWidgets/QtWidgets>
#endif
#include <QtGui/QtGui>

#include "abstractclass.h"
#include "smartpointertest.h"
#include "destruction.h"
#include "enums.h"
#include "injectedcode.h"
#include "interfaces.h"
#include "namespace.h"
#include "metaobjectqtmetacast.h"
#include "messagehandler.h"
#include "nativepointertester.h"
#include "paintengine.h"
#include "polymorphictype.h"
#include "signalsandslots.h"
#include "testdialog.h"
#include "threads.h"
#include "tulip.h"
#include "containers.h"
#include "containers_qt5.h"
#include "bindableowner.h"
#include "variants.h"
#include "general.h"
#include "memorymanagement.h"
#include "flagsandenumtest.h"
#include "propertyandmethodcalltest.h"
#include "settingstest.h"
#include "multisignaltest.h"
#include "exceptiontest.h"
#include "performancetests.h"
#include "threadfactory.h"

#ifndef QTJAMBI_GENERATOR_RUNNING
#include <QtJambi/QtJambiAPI>
#endif

#endif
