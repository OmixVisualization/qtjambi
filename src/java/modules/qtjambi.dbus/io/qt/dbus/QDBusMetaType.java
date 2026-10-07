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

package io.qt.dbus;

import java.io.Serializable;
import java.util.Objects;

import io.qt.NonNull;
import io.qt.Nullable;
import io.qt.StrictNonNull;
import io.qt.core.QMetaType;

/**
 * The QDBusMetaType class allows you to register class types for marshalling and demarshalling over D-Bus.
 * <p>Java wrapper for Qt's class <code>QDBusMetaType</code></p>
 */
public final class QDBusMetaType {
	static {
		QtJambi_LibraryUtilities.initialize();
	}
	
	/**
	 * Registers type with the Qt D-Bus Type System and the Qt meta-type system, if it's not already registered.
	 * @param clazz
	 * @param instantiations
	 * @return
	 */
	public static @NonNull QMetaType registerDBusMetaType(@Nullable Class<?> clazz, @NonNull QMetaType @NonNull... instantiations) {
		QMetaType metaType = QMetaType.fromType(clazz, instantiations);
		if(metaType!=null && metaType.isValid()) {
			registerDBusMetaType(metaType.id(), null, null, null);
		}else {
			throw new RuntimeException("Unable to find meta type for class "+(clazz==null ? "null" : clazz.getName()));
		}
		return metaType;
	}
	
	/**
	 * Registers type with the Qt D-Bus Type System and the Qt meta-type system, if it's not already registered.
	 * @param metaType
	 * @return
	 */
	public static @NonNull QMetaType registerDBusMetaType(@NonNull QMetaType metaType) {
		if(metaType!=null && metaType.isValid()) {
			registerDBusMetaType(metaType.id(), null, null, null);
		}else if(metaType==null) {
			metaType = new QMetaType();
		}
		return metaType;
	}
	
	/**
	 * Marshalling function for type U
	 * @param <U>
     * @serial exclude
	 */
	public interface MarshallFunction<U> extends java.util.function.BiConsumer<QDBusArgument, U>, Serializable{
	}
	
	/**
	 * Demarshalling function for type U
	 * @param <U>
     * @serial exclude
	 */
	public interface DemarshallFunction<U> extends java.util.function.Function<QDBusArgument, U>, Serializable{
	}
	
	/**
	 * Registers type T with the Qt D-Bus Type System and the Qt meta-type system, if it's not already registered.
	 * Registers the marshalling and demarshalling functions for meta type metaType.
	 * @param <T>
	 * @param marshallFunction
	 * @param demarshallFunction
	 * @return
	 */
	public static <T> @NonNull QMetaType registerDBusMetaType(@StrictNonNull MarshallFunction<T> marshallFunction, @StrictNonNull DemarshallFunction<T> demarshallFunction) {
		int[] marshallFunctionTypes = QtJambi_LibraryUtilities.internal.lambdaMetaTypes(MarshallFunction.class, Objects.requireNonNull(marshallFunction, "Argument 'marshallFunction': null not expected."));
		int[] demarshallFunctionTypes = QtJambi_LibraryUtilities.internal.lambdaMetaTypes(DemarshallFunction.class, Objects.requireNonNull(demarshallFunction, "Argument 'demarshallFunction': null not expected."));
		Class<?>[] marshallFunctionClassTypes = QtJambi_LibraryUtilities.internal.lambdaClassTypes(MarshallFunction.class, Objects.requireNonNull(marshallFunction, "Argument 'marshallFunction': null not expected."));
		Class<?>[] demarshallFunctionClassTypes = QtJambi_LibraryUtilities.internal.lambdaClassTypes(DemarshallFunction.class, Objects.requireNonNull(demarshallFunction, "Argument 'demarshallFunction': null not expected."));
		if(marshallFunctionTypes==null || demarshallFunctionTypes==null 
				|| marshallFunctionTypes.length!=3 || demarshallFunctionTypes.length!=2)
			throw new IllegalArgumentException("Marshall and/or demarshall function not a lambda expression.");
		if(demarshallFunctionTypes[0]==0 || marshallFunctionTypes[2]==0)
			throw new IllegalArgumentException("Unable to recognize meta type.");
		if(demarshallFunctionTypes[0]!=marshallFunctionTypes[2] || demarshallFunctionClassTypes[0]!=marshallFunctionClassTypes[2]) {
			throw new IllegalArgumentException(String.format("Marshalled type %1$s (%2$s) is different from demarshalled type %3$s (%4$s).", marshallFunctionClassTypes[2].getTypeName(), new QMetaType(marshallFunctionTypes[2]).name(), demarshallFunctionClassTypes[0].getTypeName(), new QMetaType(demarshallFunctionTypes[0]).name()));
		}
		registerDBusMetaType(demarshallFunctionTypes[0], demarshallFunctionClassTypes[0], marshallFunction, demarshallFunction);
		return new QMetaType(demarshallFunctionTypes[0]);
	}
	
	private static native <T> void registerDBusMetaType(int metaType, Class<?> classType, MarshallFunction<T> marshallFunction, DemarshallFunction<T> demarshallFunction);
	
	/**
	 * Executes the demarshalling of type metaType (whose data will be placed in data) from the D-Bus marshalling argument arg. Returns true if the demarshalling succeeded, or false if an error occurred.
	 * @param arg
	 * @param id
	 * @return
	 * @throws UnsupportedOperationException
	 */
	public static native Object demarshall(@NonNull QDBusArgument arg, @NonNull QMetaType id) throws UnsupportedOperationException;
	
	/**
	 * Executes the marshalling of type metaType (whose data is contained in data) to the D-Bus marshalling argument arg. Returns true if the marshalling succeeded, or false if an error occurred.
	 * @param arg
	 * @param value
	 * @throws UnsupportedOperationException
	 */
	public static void marshall(@NonNull QDBusArgument arg, Object value) throws UnsupportedOperationException {
		marshall(arg, null, value);
	}
	
	/**
	 * Executes the marshalling of type metaType (whose data is contained in data) to the D-Bus marshalling argument arg. Returns true if the marshalling succeeded, or false if an error occurred.
	 * @param arg
	 * @param id
	 * @param value
	 * @throws UnsupportedOperationException
	 */
	public static native void marshall(@NonNull QDBusArgument arg, @NonNull QMetaType id, Object value) throws UnsupportedOperationException;
	
	/**
	 * Returns the D-Bus signature equivalent to the supplied meta type id type.
	 * @param metaType
	 * @return
	 */
	public static native @NonNull String typeToSignature(@NonNull QMetaType metaType);
	
	/**
	 * Returns the Qt meta type id for the given D-Bus signature for exactly one full type, given by signature. Note: this function only handles the basic D-Bus types.
	 * @param signature
	 * @return
	 */
	public static native @NonNull QMetaType signatureToMetaType(@NonNull String signature);
}
