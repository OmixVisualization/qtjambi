/****************************************************************************
**
** Copyright (C) 2009-2021 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
**
** This file is part of Qt Jambi.
**
** ** $BEGIN_LICENSE$
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

#ifdef QT_QTJAMBI_PORT
#include <QtCore>
#include <QtWidgets>
#include <qtjambi/qtjambi_repository.h>

class QtJambiLink;

void initialize_meta_info_QSignalMapper_connection(){
    registerSignalMapperConnector([](QSignalMapper* object, QWeakPointer<QtJambiLink> link, QSignalMapperMapped signalMapperMapped)->QMetaObject::Connection {
        return QObject::connect(object, QOverload<QWidget*>::of(&QSignalMapper::mapped), object, [link, signalMapperMapped](QWidget*  arg__1){signalMapperMapped(arg__1, link);});
    });
};
#endif // QT_QTJAMBI_PORT
