/****************************************************************************
**
** Copyright (C) 2009-2026 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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

package io.qt.tasktree;

import io.qt.*;

/**
 * <p>Java wrapper for Qt class <code>QtTaskTree::Timeout</code></p>
 */
public final class Timeout{
    static {
    	QtJambi_LibraryUtilities.initialize();
    }
	private final long __qt_directLink;
	private final boolean __qt_isMutable;
	private Timeout(long directLink, boolean isMutable) { 
		this.__qt_directLink = directLink;
		this.__qt_isMutable = isMutable;
	}
	public java.time.@NonNull Duration getTimeout() {
		if(__qt_directLink==0)
			throw new QNoNativeResourcesException("Function call on incomplete object of type: MilliSeconds");
		return  getTimeout(__qt_directLink);
	}
	
	public void setTimeout(java.time.temporal.@NonNull TemporalAmount milliseconds) {
		if(__qt_isMutable) {
    		if(__qt_directLink==0)
    			throw new QNoNativeResourcesException("Function call on incomplete object of type: MilliSeconds");
			setTimeout(__qt_directLink, milliseconds);
		}else {
			throw new QNoImplementationException("Constant time cannot be changed.");
		}
	}
	private static native java.time.@NonNull Duration getTimeout(long directLink);
	
	private static native void setTimeout(long directLink, java.time.temporal.@NonNull TemporalAmount milliseconds);
}
