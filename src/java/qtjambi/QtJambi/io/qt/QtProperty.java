/****************************************************************************
**
** Copyright (C) 1992-2009 Nokia. All rights reserved.
** Copyright (C) 2009-2020 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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
package io.qt;

import io.qt.internal.NativeAccess;

/**
 * Information about a property in a QObject subclass.
 */
public final class QtProperty {
    private final boolean writable;
    private final boolean designable;
    private final boolean resettable;
    private final boolean user;
    private final String name;

    @NativeAccess
    private QtProperty(boolean writable, boolean designable, boolean resettable, boolean user, String name) {
        this.writable = writable;
        this.designable = designable;
        this.resettable = resettable;
        this.user = user;
        this.name = name;
    }

    public final boolean isWritable() { return writable; }
    public final boolean isResettable() { return resettable; }
    public final boolean isDesignable() { return designable; }
    public final boolean isUser() { return user; }
    public final String name() { return name; }

    @Override
    public final String toString() {
        return name;
    }
}
