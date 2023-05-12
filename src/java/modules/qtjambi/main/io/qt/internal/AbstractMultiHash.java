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

import java.util.AbstractMap;
import java.util.ArrayList;
import java.util.HashSet;
import java.util.List;
import java.util.Objects;
import java.util.Set;

import io.qt.QtUninvokable;
import io.qt.core.QPair;

public abstract class AbstractMultiHash<K, V> extends AbstractMultiAssociativeContainer<K, V> {

	protected AbstractMultiHash(QPrivateConstructor p) {
		super(p);
	}
	
	@Override
    public abstract AbstractMultiHash<K,V> clone();
	
	@Override
    @QtUninvokable
	public final Set<K> keySet() {
		return new HashSet<>(keys());
	}
	
	@Override
    @QtUninvokable
	public final Set<Entry<K, List<V>>> entrySet() {
		Set<Entry<K, List<V>>> entrySet = new HashSet<>();
		if(!this.isEmpty()) {
			K currentKey = null;
			List<V> currentValues = null;
			for(QPair<K,V> pair : this) {
				if(currentValues==null) {
					currentValues = new ArrayList<>();
					currentValues.add(pair.second);
					currentKey = pair.first;
				}else{
					if(Objects.equals(currentKey, pair.first)) {
						currentValues.add(pair.second);
					}else {
						entrySet.add(new AbstractMap.SimpleImmutableEntry<>(currentKey, currentValues));
						currentValues = new ArrayList<>();
						currentValues.add(pair.second);
						currentKey = pair.first;
					}
				}
			}
			entrySet.add(new AbstractMap.SimpleImmutableEntry<>(currentKey, currentValues));
		}
		return entrySet;
	}
}
