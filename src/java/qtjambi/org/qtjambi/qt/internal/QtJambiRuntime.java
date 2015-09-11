/****************************************************************************
**
** Copyright (C) 2011 Darryl L. Miles.  All rights reserved.
** Copyright (C) 2011 D L Miles Consulting Ltd.  All rights reserved.
**
** This file is part of Qt Jambi.
**
**
** $BEGIN_LICENSE$
** GNU Lesser General Public License Usage
** This file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html
** 
** In addition, as a special exception, the copyright holders grant you
** certain additional rights. These rights are described in the Nokia Qt
** LGPL Exception version 1.0, included in the file LGPL_EXCEPTION.txt in
** this package.
** 
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 2.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL2 included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 2.0 requirements will be
** met: http://www.gnu.org/licenses/gpl-2.0.html
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL3 included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html
** $END_LICENSE$
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

package org.qtjambi.qt.internal;

/*
 * @exclude
 *
 * This class exists to encapulate those aspects of the QtJambi bridge that might be
 * tweaked or configured for various reasons.
 */
public class QtJambiRuntime {
    /*
     * Don't mess with this unless you know what you are doing.
     * 0 = Disable always
     * 1 = Enable only things required.
     * 2 = Enable as much as possible (default).
     * FIXME: Make these method more private
     */
    public static native int getObjectCacheMode();
    public static native void setObjectCacheMode(int objectCacheMode);

    public static final int OBJECT_CACHE_MODE_DISABLE = 0;
    public static final int OBJECT_CACHE_MODE_LIMITED = 1;
    public static final int OBJECT_CACHE_MODE_DEFAULT = 2;

    /*
     * 1 = Flush (clear all)
     * 2 = Count
     */
    public static native int objectCacheOperation(int operation);

    public static final int OBJECT_CACHE_OPERATION_FLUSH = 0;
    public static final int OBJECT_CACHE_OPERATION_COUNT = 1;

    public static native boolean pushDebugMarker(long id);
    public static native long popDebugMarker();
    public static native long getDebugMarker();
}
