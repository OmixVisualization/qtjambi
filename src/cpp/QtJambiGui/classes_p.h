/****************************************************************************
**
** Copyright (C) 2009-2026 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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

#ifndef QTJAMBIGUI_CLASSES_P_H
#define QTJAMBIGUI_CLASSES_P_H

#include <QtCore/QObject>

#if defined(Q_OS_MACOS)
class NSWindowUtils final : public QObject{
    Q_OBJECT
public:
    Q_INVOKABLE bool isOnActiveSpace(QWindow* window) const;
    Q_INVOKABLE bool isMainWindow(QWindow* window) const;
    Q_INVOKABLE bool canBecomeMain(QWindow* window) const;
    Q_INVOKABLE void makeMain(QWindow* window) const;
    Q_INVOKABLE int windowNumber(QWindow* window) const;
    Q_INVOKABLE bool canBecomeVisibleWithoutLogin(QWindow* window) const;
    Q_INVOKABLE void setCanBecomeVisibleWithoutLogin(QWindow* window, bool b) const;
    Q_INVOKABLE bool allowsToolTipsWhenApplicationIsInactive(QWindow* window) const;
    Q_INVOKABLE void setAllowsToolTipsWhenApplicationIsInactive(QWindow* window, bool b) const;
};
#endif

#endif // QTJAMBIGUI_CLASSES_P_H
