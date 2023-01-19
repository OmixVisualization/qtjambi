/****************************************************************************
**
** Copyright (C) 2009-2023 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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
package io.qt.internal;

import io.qt.QtUninvokable;
import io.qt.core.QPair;

public abstract class AbstractAssociativeConstIterator<K,V> extends AbstractSequentialConstIterator<V>{
	
	protected AbstractAssociativeConstIterator(Object owner) {
		super(owner);
	}

	@QtUninvokable
    protected final java.util.Iterator<QPair<K,V>> toJavaMapIterator(){
    	return new java.util.Iterator<QPair<K,V>>() {
    		private final AbstractSequentialConstIterator<?> end = end();
    		
            @Override
            public boolean hasNext() {
                return !AbstractAssociativeConstIterator.this.equals(end);
            }

            @Override
            public QPair<K,V> next() {
                if(!hasNext())
                    throw new java.util.NoSuchElementException();
            	if(!end.equals(end()))
            		throw new IllegalMonitorStateException();
                QPair<K,V> e = new QPair<>(checkedKey(), checkedValue());
                increment();
                return e;
            }
        };
    }
    
    @QtUninvokable
    protected abstract K checkedKey();
}
