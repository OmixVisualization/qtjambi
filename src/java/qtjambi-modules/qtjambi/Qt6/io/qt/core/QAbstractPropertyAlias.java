/****************************************************************************
**
** Copyright (C) 2009-2022 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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
package io.qt.core;

import java.util.Objects;

import io.qt.QtUninvokable;

abstract class QAbstractPropertyAlias extends QPropertyObserver {
	
	final QBindableInterface iface;
	
	QAbstractPropertyAlias(io.qt.core.QUntypedPropertyData aliasedPropertyPtr, QBindableInterface iface) {
		super((QPrivateConstructor)null);
		initialize(this, Objects.requireNonNull(aliasedPropertyPtr));
		this.iface = iface;
		setObserver(QtJambi_LibraryUtilities.internal.nativeId(this), QtJambi_LibraryUtilities.internal.nativeId(Objects.requireNonNull(this.aliasedProperty())), QtJambi_LibraryUtilities.internal.nativeId(this.iface));
	}
	
	QAbstractPropertyAlias(QAbstractPropertyAlias other) {
		super((QPrivateConstructor)null);
		initialize(this, Objects.requireNonNull(other.aliasedProperty()));
		this.iface = other.iface;
		setObserver(QtJambi_LibraryUtilities.internal.nativeId(this), QtJambi_LibraryUtilities.internal.nativeId(this.aliasedProperty()), QtJambi_LibraryUtilities.internal.nativeId(this.iface));
	}
	
	private native static void initialize(Object instance, io.qt.core.QUntypedPropertyData aliasedPropertyPtr);
	
	private native void setObserver(long thisId, long aliasedPropertyId, long ifaceId);
	
	/**
	 * @see QProperty#setBinding(QUntypedPropertyBinding)
	 */
	@QtUninvokable
	public final boolean setBinding(QUntypedPropertyBinding newBinding) {
		return new QUntypedBindable(aliasedProperty(), iface).setBinding(newBinding);
    }
	
	/**
	 * @see QProperty#hasBinding()
	 */
	@QtUninvokable
	public final boolean hasBinding() { return new QUntypedBindable(aliasedProperty(), iface).hasBinding(); }
	
	/**
	 * @see QProperty#onValueChanged(Runnable)
	 */
	@QtUninvokable
	public final QPropertyChangeHandler onValueChanged(Runnable f) {
        return new QUntypedBindable(aliasedProperty(), iface).onValueChanged(f);
    }
	
	/**
	 * @see QProperty#addNotifier(Runnable)
	 */    
    @io.qt.QtUninvokable
    public QPropertyNotifier addNotifier(Runnable f) {
    	return new QUntypedBindable(aliasedProperty(), iface).addNotifier(f);
    }

	/**
	 * @see QProperty#subscribe(Runnable)
	 */
	@QtUninvokable
	public final QPropertyChangeHandler subscribe(Runnable f) {
        f.run();
        return onValueChanged(f);
    }
	
	/**
	 * Checks if alias is valid.
	 * @return valid
	 */
	@QtUninvokable
	public final boolean isValid() {
        return aliasedProperty() != null;
    }
}
