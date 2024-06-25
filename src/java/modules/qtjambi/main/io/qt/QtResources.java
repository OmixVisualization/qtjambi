/****************************************************************************
**
** Copyright (C) 2009-2024 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
**
** This file is part of Qt Jambi.
**
** $BEGIN_LICENSE$
** 
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
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
package io.qt;

import io.qt.core.QResource;

/**
 * Access utility class to the QtJambi &quot;classpath:&quot; resource system.
 * <br>Deprecated: Use <code>QResource</code> instead.
 * @see QResource
 */
@Deprecated(forRemoval=true)
public final class QtResources {

	private QtResources() {}

	/**
     * Adds <code>path</code> to the set of paths in which Qt Jambi should search for resources. 
     * Resources can be accessed using the &quot;classpath:&quot; scheme.
     * @see QResource#addClassPath(String)
     */
    @Deprecated(forRemoval=true)
	public static void addSearchPath(String path) {
        io.qt.core.QResource.addClassPath(path);
    }
    
    /**
     * Removes <code>path</code> from the set of paths in which Qt Jambi searches
     * for resources.
     * @see QResource#removeClassPath(String)
     */
    @Deprecated(forRemoval=true)
	public static void removeSearchPath(String path) {
        io.qt.core.QResource.removeClassPath(path);
    }
}
