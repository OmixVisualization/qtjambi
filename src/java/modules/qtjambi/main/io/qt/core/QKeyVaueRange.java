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
package io.qt.core;

import java.util.Iterator;
import java.util.function.Supplier;

/**
 * A range object that allows iteration over this hash as key/value pairs.
 */
public final class QKeyVaueRange<Key,T> implements Iterable<QPair<Key,T>>{
    static {
    	QtJambi_LibraryUtilities.initialize();
    }
	
	private Supplier<QSequentialConstPairIterator<Key,T,?>> beginSupplier;

	QKeyVaueRange(Supplier<QSequentialConstPairIterator<Key, T, ?>> beginSupplier) {
		super();
		this.beginSupplier = beginSupplier;
	}

	@Override
	public Iterator<QPair<Key, T>> iterator() {
		QSequentialConstPairIterator<Key,T,?> begin = beginSupplier.get();
		return AbstractIterator.iterator(begin);
	}
}
