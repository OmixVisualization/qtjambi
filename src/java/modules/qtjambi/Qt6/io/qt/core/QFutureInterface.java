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

package io.qt.core;

import io.qt.*;

/**
 * <p>Java wrapper for Qt class QFutureInterface</p>
 */
public final class QFutureInterface<T> extends io.qt.core.QFutureInterfaceBase
    implements java.lang.Cloneable
{
    /**
     * <p>Overloaded factory for {@link #createVoidFutureInterface(io.qt.core.QFutureInterfaceBase.State)}
     *  with <code>initialState = io.qt.core.QFutureInterfaceBase.State.NoState</code>.</p>
     */
    public static @NonNull QFutureInterface<Void> createVoidFutureInterface() {
    	return createVoidFutureInterface(State.NoState);
    }
    
    public static @NonNull QFutureInterface<Void> createVoidFutureInterface(@StrictNonNull State initialState) {
    	return new QFutureInterface<>(true, initialState);
    }
    
    public static @NonNull QFutureInterface<Void> createVoidFutureInterface(io.qt.core.@StrictNonNull QFutureInterfaceBase other) {
    	return new QFutureInterface<>(other);
    }
    
    public static <T> @NonNull QFutureInterface<T> canceledResult()
    { return new QFutureInterface<T>(false, State.resolve(State.Started.value() | State.Finished.value() | State.Canceled.value())); }
	
	
    /**
     * <p>Overloaded constructor for {@link #QFutureInterface(io.qt.core.QFutureInterfaceBase.State)}
     *  with <code>initialState = io.qt.core.QFutureInterfaceBase.State.NoState</code>.</p>
     */
    private QFutureInterface(boolean isVoid) {
        this(isVoid, io.qt.core.QFutureInterfaceBase.State.NoState);
    }
    
    private QFutureInterface(boolean isVoid, io.qt.core.QFutureInterfaceBase.State initialState){
        super((QPrivateConstructor)null);
        initialize_native(this, initialState, isVoid);
    }
    
    /**
     * <p>Overloaded constructor for {@link #QFutureInterface(io.qt.core.QFutureInterfaceBase.State)}
     *  with <code>initialState = io.qt.core.QFutureInterfaceBase.State.NoState</code>.</p>
     */
    public QFutureInterface() {
        this(false, io.qt.core.QFutureInterfaceBase.State.NoState);
    }
    
    public QFutureInterface(io.qt.core.QFutureInterfaceBase.State initialState){
    	this(false, initialState);
    }
    
    private native static <T> void initialize_native(QFutureInterface<T> instance, io.qt.core.QFutureInterfaceBase.State initialState, boolean isVoid);
    
    public QFutureInterface(io.qt.core.@StrictNonNull QFutureInterface<T> other){
        super((QPrivateConstructor)null);
        initialize_native(this, java.util.Objects.requireNonNull(other));
    }
    
    private QFutureInterface(io.qt.core.QFutureInterfaceBase other){
        super((QPrivateConstructor)null);
        initialize_native(this, java.util.Objects.requireNonNull(other));
    }
    
    private native static <T> void initialize_native(QFutureInterface<T> instance, io.qt.core.QFutureInterfaceBase other);
    
    @io.qt.QtUninvokable
    public final io.qt.core.@NonNull QFuture<T> future(){
        return new io.qt.core.QFuture<T>(this);
    }
    
    @io.qt.QtUninvokable
    public final boolean reportFinished(T result){
        return reportFinished(QtJambi_LibraryUtilities.internal.nativeId(this), result);
    }
    
    @io.qt.QtUninvokable
    private native boolean reportFinished(long __this__nativeId, T result);
    
    /**
     * <p>Overloaded function for {@link #reportResult(T, int)}
     *  with <code>index = -1</code>.</p>
     */
    @io.qt.QtUninvokable
    public final boolean reportResult(T result) {
        return reportResult(QtJambi_LibraryUtilities.internal.nativeId(this), result, (int)-1);
    }
    
    @io.qt.QtUninvokable
    public final boolean reportResult(T result, int index){
        return reportResult(QtJambi_LibraryUtilities.internal.nativeId(this), result, index);
    }
    
    @io.qt.QtUninvokable
    static native <T> boolean reportResult(long __this__nativeId, T result, int index);
    
    /**
     * <p>Overloaded function for {@link #reportResults(java.util.Collection, int, int)}
     *  with <code>count = -1</code>.</p>
     */
    @io.qt.QtUninvokable
    public final boolean reportResults(java.util.Collection<T> results, int beginIndex) {
        return reportResults(results, beginIndex, (int)-1);
    }
    
    /**
     * <p>Overloaded function for {@link #reportResults(java.util.Collection, int, int)}</p>
     * <p>with: </p><ul>
     * <li><code>beginIndex = -1</code></li>
     * <li><code>count = -1</code></li>
     * </ul>
     */
    @io.qt.QtUninvokable
    public final boolean reportResults(java.util.Collection<T> results) {
        return reportResults(results, (int)-1, (int)-1);
    }
    
    @io.qt.QtUninvokable
    public final boolean reportResults(java.util.Collection<T> results, int beginIndex, int count){
        return reportResults(QtJambi_LibraryUtilities.internal.nativeId(this), results, beginIndex, count);
    }
    
    @io.qt.QtUninvokable
    private native boolean reportResults(long __this__nativeId, java.util.Collection<T> results, int beginIndex, int count);
    
    @io.qt.QtUninvokable
    public final T result(int index){
        return result(QtJambi_LibraryUtilities.internal.nativeId(this), index);
    }
    
    @io.qt.QtUninvokable
    static native <T> T result(long __this__nativeId, int index);
    
    @io.qt.QtUninvokable
    public final io.qt.core.@NonNull QList<T> results(){
        return results(QtJambi_LibraryUtilities.internal.nativeId(this));
    }
    
    @io.qt.QtUninvokable
    static native <T> io.qt.core.QList<T> results(long __this__nativeId);
    
    /**
     * Constructor for internal use only.
     * @param p expected to be <code>null</code>.
     */
    @io.qt.NativeAccess
    private QFutureInterface(QPrivateConstructor p) { super(p); } 
    
    
    @Override
    public @NonNull QFutureInterface<T> clone() {
        return clone(QtJambi_LibraryUtilities.internal.nativeId(this));
    }
    private native QFutureInterface<T> clone(long __this_nativeId);
}
