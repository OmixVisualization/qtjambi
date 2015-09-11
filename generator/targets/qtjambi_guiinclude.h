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
** If you are unsure which license is appropriate for your use, please
** contact the sales department at qt-sales@nokia.com.
** $END_LICENSE$

**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

#include "generator_predefinitions.h"

#if QT_VERSION>=0x050000
	#include <QtCore/private/qabstractfileengine_p.h>
	#include <QtCore/private/qfsfileengine_p.h>
#endif
	
#include <QtCore/QtCore>
#include "qnamespace_enums.h"

#include <qtjambi_core/qtjambiqfuture.h>
#include <qtjambi_core/qtjambi_core_qhashes.h>

#include <QtNetwork/QtNetwork>
#include <qtjambi_network/qschemanetworkaccessmanager.h>
#include <qtjambi_network/qclasspathaccessmanager.h>
/*
#include <QtGui/QtGui>
#include <qtjambi_gui/qtreemodel.h>
#include <qtjambi_gui/qguisignalmapper.h>
#include <qtjambi_gui/qtjambitextobjectinterface.h>
#include <qtjambi_gui/qtjambi_gui_qhashes.h>
#include <qtjambi_gui/qtmatrixes.h>
*/
//#include <QtQml/QtQml>
//#include <QtQuick/QtQuick>
//#include <qtjambi_qml/qqmllistproperty.h>

//#define Q_COMPILER_EXPLICIT_OVERRIDES

/*
// the generator is not able to find enum QSceneChange::Priority
#include <Qt3DCore/QSceneChange>
#define Priority Qt3D::QSceneChange::Priority
#include <Qt3DCore/QBackendScenePropertyChange>
#undef Priority
*/
/*
#include <Qt3DCore/Qt3DCore>
#include <Qt3DCore/private/qobservableinterface_p.h>
#include <Qt3DCore/private/qlockableobserverinterface_p.h>
#include <Qt3DCore/private/qobserverinterface_p.h>
#include <Qt3DCore/private/qabstractaspectjobmanager_p.h>
*/
//#include <Qt3DRenderer/Qt3DRenderer>
//#include <Qt3DInput/Qt3DInput>
//#include <Qt3DQuick/Qt3DQuick>
//#include <Qt3DQuickRenderer/Qt3DQuickRenderer>


*/
# if QT_VERSION >= 0x050000
/*
#ifndef QT_NO_WINEXTRAS
//#include <QtWinExtras/QtWinExtras>
#endif

#ifndef QT_NO_MACEXTRAS
//#include <QtMacExtras/QtMacExtras>
#endif

#ifndef QT_NO_X11EXTRAS
//#include <QtX11Extras/QtX11Extras>
#endif

#define Q_NULLPTR 0
//#define NULL 0
//#define null 0
//#define nullptr 0
#ifndef QT_NO_SERIALPORT
//#include <QtSerialPort/QtSerialPort>
#endif
*/
/*
#ifndef QT_NO_QT3DCORE
#include <Qt3DCore/Qt3DCore>
#endif

#ifndef QT_NO_QT3DINPUT
#include <Qt3DInput/Qt3DInput>
#endif

#ifndef QT_NO_QT3DRENDERER
#include <Qt3DRenderer/Qt3DRenderer>
#endif

#ifndef QT_NO_QT3DQUICK
#include <Qt3DQuick/Qt3DQuick>
#endif
*/
#endif
