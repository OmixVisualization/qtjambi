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

package io.qt.internal;

import java.lang.reflect.Field;
import java.util.ArrayList;
import java.util.Collection;
import java.util.Collections;
import java.util.HashSet;
import java.util.IdentityHashMap;
import java.util.List;
import java.util.Map;
import java.util.Set;

import io.qt.NativeAccess;
import io.qt.QtObjectInterface;
import io.qt.core.QMetaObject;
import io.qt.internal.NativeUtility.NativeLink;

abstract class ReferenceUtility {
	private ReferenceUtility() {
		throw new RuntimeException();
	}

	static {
		QtJambi_LibraryUtilities.initialize();
	}

    private static native boolean needsReferenceCounting(long object);

	private static class RCList extends ArrayList<Object> {
		@NativeAccess
		public RCList() {
			super();
		}

		private static final long serialVersionUID = -4010060446825990721L;
		private final QMetaObject.Slot0 checkSlot = this::check;

		private synchronized void check() {
			List<Object> disposedElements = null;
			for (Object o : this) {
				if (o instanceof QtObjectInterface && ((QtObjectInterface) o).isDisposed()) {
					if (disposedElements == null) {
						disposedElements = Collections.singletonList(o);
					} else {
						if (disposedElements.size() == 1) {
							disposedElements = new ArrayList<>(disposedElements);
						}
						disposedElements.add(o);
					}
				}
			}
			if (disposedElements != null) {
				for (Object o : disposedElements) {
					super.remove(o);
				}
			}
		}

		@Override
		@NativeAccess
		public boolean add(Object e) {
			if(e instanceof QtObjectInterface) {
				if(!needsReferenceCounting(NativeUtility.nativeId((QtObjectInterface)e)))
					return false;
				boolean result;
				synchronized (this) {
					result = super.add(e);
				}
				if (result) {
					QMetaObject.DisposedSignal disposed = NativeUtility.getSignalOnDispose((QtObjectInterface) e, true);
					if (disposed != null)
						disposed.connect(checkSlot);
				}
				return result;
			}else {
				synchronized (this) {
					return super.add(e);
				}
			}
		}

		@Override
		@NativeAccess
		public boolean remove(Object o) {
			boolean result;
			synchronized (this) {
				result = super.remove(o);
			}
			if (result && o instanceof QtObjectInterface) {
				QMetaObject.DisposedSignal disposed = NativeUtility.getSignalOnDispose((QtObjectInterface) o, true);
				if (disposed != null)
					disposed.disconnect(checkSlot);
			}
			return result;
		}

		@Override
		@NativeAccess
		public void clear() {
			List<Object> disposedElements;
			synchronized(this) {
				disposedElements = new ArrayList<>(this);
				super.clear();
			}
			for (Object o : disposedElements) {
				if (o instanceof QtObjectInterface) {
					QMetaObject.DisposedSignal disposed = NativeUtility.getSignalOnDispose((QtObjectInterface) o, true);
					if (disposed != null)
						disposed.disconnect(checkSlot);
				}
			}
		}

		@Override
		@NativeAccess
		public boolean addAll(Collection<? extends Object> c) {
			boolean result = false;
			for (Object o : c) {
				result |= add(o);
			}
			return result;
		}
	}
	
	@NativeAccess
	private static class RCSet extends HashSet<Object> {
		@NativeAccess
		public RCSet() {
			super();
		}

		private static final long serialVersionUID = -4010060446825990721L;
		private final QMetaObject.Slot0 checkSlot = this::check;

		@SuppressWarnings("unused")
		private synchronized void check() {
			List<Object> disposedElements = null;
			for (Object o : this) {
				if (o instanceof QtObjectInterface && ((QtObjectInterface) o).isDisposed()) {
					if (disposedElements == null) {
						disposedElements = Collections.singletonList(o);
					} else {
						if (disposedElements.size() == 1) {
							disposedElements = new ArrayList<>(disposedElements);
						}
						disposedElements.add(o);
					}
				}
			}
			if (disposedElements != null) {
				for (Object o : disposedElements) {
					super.remove(o);
				}
			}
		}

		@Override
		@NativeAccess
		public boolean add(Object e) {
			if(e instanceof QtObjectInterface) {
				if(!needsReferenceCounting(NativeUtility.nativeId((QtObjectInterface)e)))
					return false;
				boolean result;
				synchronized (this) {
					result = super.add(e);
				}
				if (result) {
					QMetaObject.DisposedSignal disposed = NativeUtility.getSignalOnDispose((QtObjectInterface) e, true);
					if (disposed != null)
						disposed.connect(checkSlot);
				}
				return result;
			}else {
				synchronized (this) {
					return super.add(e);
				}
			}
		}

		@Override
		@NativeAccess
		public boolean remove(Object o) {
			boolean result;
			synchronized (this) {
				result = super.remove(o);
			}
			if (result && o instanceof QtObjectInterface) {
				QMetaObject.DisposedSignal disposed = NativeUtility.getSignalOnDispose((QtObjectInterface) o, true);
				if (disposed != null)
					disposed.disconnect(checkSlot);
			}
			return result;
		}

		@Override
		@NativeAccess
		public void clear() {
			Set<Object> disposedElements;
			synchronized(this) {
				disposedElements = new HashSet<>(this);
				super.clear();
			}
			for (Object o : disposedElements) {
				if (o instanceof QtObjectInterface) {
					QMetaObject.DisposedSignal disposed = NativeUtility.getSignalOnDispose((QtObjectInterface) o, true);
					if (disposed != null)
						disposed.disconnect(checkSlot);
				}
			}
		}

		@Override
		@NativeAccess
		public boolean addAll(Collection<? extends Object> c) {
			boolean result = false;
			for (Object o : c) {
				result |= add(o);
			}
			return result;
		}
	}
	
	private static class RCMap extends IdentityHashMap<Object, Object> {
		private static final long serialVersionUID = 3076251074218500284L;
		private final QMetaObject.Slot0 checkSlot = this::check;

		@NativeAccess
		public RCMap() {
			super();
		}
		
		@SuppressWarnings("unused")
		private synchronized void check() {
			List<Object> nulledKeys = null;
			List<Object> disposedKeys = null;
			for (Entry<Object, Object> entry : this.entrySet()) {
				if (entry.getValue() instanceof QtObjectInterface
						&& ((QtObjectInterface) entry.getValue()).isDisposed()) {
					if (entry.getKey() instanceof QtObjectInterface
							&& !((QtObjectInterface) entry.getKey()).isDisposed()) {
						if (nulledKeys == null) {
							nulledKeys = Collections.singletonList(entry.getKey());
						} else {
							if (nulledKeys.size() == 1) {
								nulledKeys = new ArrayList<>(nulledKeys);
							}
							nulledKeys.add(entry.getKey());
						}
					} else {
						if (disposedKeys == null) {
							disposedKeys = Collections.singletonList(entry.getKey());
						} else {
							if (disposedKeys.size() == 1) {
								disposedKeys = new ArrayList<>(disposedKeys);
							}
							disposedKeys.add(entry.getKey());
						}
					}
				}
			}
			if (disposedKeys != null) {
				for (Object key : disposedKeys) {
					super.remove(key);
				}
			}
			if (nulledKeys != null) {
				for (Object key : nulledKeys) {
					super.put(key, null);
				}
			}
		}

		@Override
		public Object put(Object key, Object value) {
			if (key instanceof QtObjectInterface || value instanceof QtObjectInterface) {
				if (key instanceof QtObjectInterface && value instanceof QtObjectInterface) {
					if(!needsReferenceCounting(NativeUtility.nativeId((QtObjectInterface)key)) && !needsReferenceCounting(NativeUtility.nativeId((QtObjectInterface)value)))
						return false;
				}else if (key instanceof QtObjectInterface) {
					if(!needsReferenceCounting(NativeUtility.nativeId((QtObjectInterface)key)))
						return false;
				}else {
					if(!needsReferenceCounting(NativeUtility.nativeId((QtObjectInterface)value)))
						return false;
				}
			}

			Object result;
			synchronized (this) {
				result = super.put(key, value);
			}
			if (key instanceof QtObjectInterface) {
				QMetaObject.DisposedSignal disposed = NativeUtility.getSignalOnDispose((QtObjectInterface) key, true);
				if (disposed != null)
					disposed.connect(checkSlot);
			}
			if (value instanceof QtObjectInterface) {
				QMetaObject.DisposedSignal disposed = NativeUtility.getSignalOnDispose((QtObjectInterface) value, true);
				if (disposed != null)
					disposed.connect(checkSlot);
			}
			if (result instanceof QtObjectInterface) {
				QMetaObject.DisposedSignal disposed = NativeUtility.getSignalOnDispose((QtObjectInterface) result, true);
				if (disposed != null)
					disposed.disconnect(checkSlot);
			}
			return result;
		}

		@Override
		public Object remove(Object key) {
			Object result;
			synchronized (this) {
				result = super.remove(key);
			}
			if (key instanceof QtObjectInterface) {
				QMetaObject.DisposedSignal disposed = NativeUtility.getSignalOnDispose((QtObjectInterface) key, true);
				if (disposed != null)
					disposed.disconnect(checkSlot);
			}
			if (result instanceof QtObjectInterface) {
				QMetaObject.DisposedSignal disposed = NativeUtility.getSignalOnDispose((QtObjectInterface) result, true);
				if (disposed != null)
					disposed.disconnect(checkSlot);
			}
			return result;
		}

		@Override
		public void clear() {
			IdentityHashMap<Object, Object> _this;
			synchronized (this) {
				_this = new IdentityHashMap<>(this);
				super.clear();
			}
			for (Entry<Object, Object> entry : _this.entrySet()) {
				if (entry.getKey() instanceof QtObjectInterface) {
					QMetaObject.DisposedSignal disposed = NativeUtility.getSignalOnDispose((QtObjectInterface) entry.getKey(), true);
					if (disposed != null)
						disposed.disconnect(checkSlot);
				}
				if (entry.getValue() instanceof QtObjectInterface) {
					QMetaObject.DisposedSignal disposed = NativeUtility.getSignalOnDispose((QtObjectInterface) entry.getValue(), true);
					if (disposed != null)
						disposed.disconnect(checkSlot);
				}
			}
		}

		@Override
		public boolean remove(Object key, Object value) {
			boolean result;
			synchronized (this) {
				result = super.remove(key, value);
			}
			if (result) {
				if (key instanceof QtObjectInterface) {
					QMetaObject.DisposedSignal disposed = NativeUtility.getSignalOnDispose((QtObjectInterface) key, true);
					if (disposed != null)
						disposed.disconnect(checkSlot);
				}
				if (value instanceof QtObjectInterface) {
					QMetaObject.DisposedSignal disposed = NativeUtility.getSignalOnDispose((QtObjectInterface) value, true);
					if (disposed != null)
						disposed.disconnect(checkSlot);
				}
			}
			return result;
		}
	}

	static void removeFromMapReferenceCount(QtObjectInterface owner, Class<? extends QtObjectInterface> declaringClass, String fieldName, boolean isStatic, Object value) {
		Object collection = null;
		boolean got = false;
		if (declaringClass.isInterface() && !isStatic) {
			NativeLink link = NativeUtility.findInterfaceLink(owner, false);
			if (link instanceof NativeUtility.InterfaceNativeLink) {
				collection = ((NativeUtility.InterfaceNativeLink) link).getReferenceCountCollection(declaringClass, fieldName, null);
				got = true;
			}
		}
		if (!got) {
			Field field = null;
			try {
				field = declaringClass.getDeclaredField(fieldName);
			} catch (NoSuchFieldException | SecurityException e2) {
			}
			if (field == null && owner != null) {
				Class<?> objectClass = ClassAnalyzerUtility.getClass(owner);
				do {
					try {
						field = objectClass.getDeclaredField(fieldName);
					} catch (NoSuchFieldException | SecurityException e2) {
					}
					objectClass = objectClass.getSuperclass();
				} while (objectClass != null && field == null);
			}
			if (field != null) {
				collection = ReflectionUtility.readField(owner, field);
			}
		}
		if (collection instanceof Map) {
			((Map<?, ?>) collection).remove(value);
		}
	}

	static void removeFromCollectionReferenceCount(QtObjectInterface owner, Class<? extends QtObjectInterface> declaringClass, String fieldName, boolean isStatic, Object value) {
		Object collection = null;
		boolean got = false;
		if (declaringClass.isInterface() && !isStatic) {
			NativeLink link = NativeUtility.findInterfaceLink(owner, false);
			if (link instanceof NativeUtility.InterfaceNativeLink) {
				collection = ((NativeUtility.InterfaceNativeLink) link).getReferenceCountCollection(declaringClass, fieldName, null);
				got = true;
			}
		}
		if (!got) {
			Field field = null;
			try {
				field = declaringClass.getDeclaredField(fieldName);
			} catch (NoSuchFieldException | SecurityException e2) {
			}
			if (field == null && owner != null) {
				Class<?> objectClass = ClassAnalyzerUtility.getClass(owner);
				do {
					try {
						field = objectClass.getDeclaredField(fieldName);
					} catch (NoSuchFieldException | SecurityException e2) {
					}
					objectClass = objectClass.getSuperclass();
				} while (objectClass != null && field == null);
			}
			if (field != null) {
				collection = ReflectionUtility.readField(owner, field);
			}
		}
		if (collection instanceof Collection) {
			((Collection<?>) collection).remove(value);
		}
	}

	@SuppressWarnings("unchecked")
	static void addAllReferenceCount(QtObjectInterface owner, Class<? extends QtObjectInterface> declaringClass, String fieldName, boolean isThreadSafe, boolean isStatic, Collection<?> values) {
		Object collection = null;
		boolean got = false;
		if (declaringClass.isInterface() && !isStatic) {
			NativeLink link = NativeUtility.findInterfaceLink(owner, false);
			if (link instanceof NativeUtility.InterfaceNativeLink) {
				collection = ((NativeUtility.InterfaceNativeLink) link).getReferenceCountCollection(declaringClass, fieldName, RCList::new);
				got = true;
			}
		}
		if (!got) {
			Field field = null;
			try {
				field = declaringClass.getDeclaredField(fieldName);
			} catch (NoSuchFieldException | SecurityException e2) {
			}
			if (field == null && owner != null) {
				Class<?> objectClass = ClassAnalyzerUtility.getClass(owner);
				do {
					try {
						field = objectClass.getDeclaredField(fieldName);
					} catch (NoSuchFieldException | SecurityException e2) {
					}
					objectClass = objectClass.getSuperclass();
				} while (objectClass != null && field == null);
			}
			if (field != null) {
				collection = ReflectionUtility.readField(owner, field);
				if (collection == null) {
					if (isThreadSafe) {
						collection = java.util.Collections.synchronizedList(new java.util.ArrayList<>());
					} else {
						collection = new java.util.ArrayList<>();
					}
					ReflectionUtility.writeField(owner, field, collection);
				}
			}
		}
		if (collection instanceof Collection) {
			((Collection<Object>) collection).addAll(values);
		}
	}

	@SuppressWarnings("unchecked")
	static void putReferenceCount(QtObjectInterface owner, Class<? extends QtObjectInterface> declaringClass, String fieldName, boolean isThreadSafe, boolean isStatic, Object key, Object value) {
		Object map = null;
		boolean got = false;
		if (declaringClass.isInterface() && !isStatic) {
			NativeLink link = NativeUtility.findInterfaceLink(owner, false);
			if (link instanceof NativeUtility.InterfaceNativeLink) {
				map = ((NativeUtility.InterfaceNativeLink) link).getReferenceCountCollection(declaringClass, fieldName, () -> {
					if (isThreadSafe) {
						return java.util.Collections.synchronizedMap(new RCMap());
					} else {
						return new RCMap();
					}
				});
				got = true;
			}
		}
		if (!got) {
			Field field = null;
			try {
				field = declaringClass.getDeclaredField(fieldName);
			} catch (NoSuchFieldException | SecurityException e2) {
			}
			if (field == null && owner != null) {
				Class<?> objectClass = ClassAnalyzerUtility.getClass(owner);
				do {
					try {
						field = objectClass.getDeclaredField(fieldName);
					} catch (NoSuchFieldException | SecurityException e2) {
					}
					objectClass = objectClass.getSuperclass();
				} while (objectClass != null && field == null);
			}
			if (field != null) {
				map = ReflectionUtility.readField(owner, field);
				if (map == null) {
					if (isThreadSafe) {
						map = java.util.Collections.synchronizedMap(new java.util.HashMap<>());
					} else {
						map = new java.util.HashMap<>();
					}
					ReflectionUtility.writeField(owner, field, map);
				}
			}
		}
		if (map instanceof Map) {
			((Map<Object, Object>) map).put(key, value);
		}
	}

	static void clearReferenceCount(QtObjectInterface owner, Class<? extends QtObjectInterface> declaringClass, String fieldName, boolean isStatic) {
		Object collection = null;
		boolean got = false;
		if (declaringClass.isInterface() && !isStatic) {
			NativeLink link = NativeUtility.findInterfaceLink(owner, false);
			if (link instanceof NativeUtility.InterfaceNativeLink) {
				collection = ((NativeUtility.InterfaceNativeLink) link).getReferenceCountCollection(declaringClass, fieldName, null);
				got = true;
			}
		}
		if (!got) {
			Field field = null;
			try {
				field = declaringClass.getDeclaredField(fieldName);
			} catch (NoSuchFieldException | SecurityException e2) {
			}
			if (field == null && owner != null) {
				Class<?> objectClass = ClassAnalyzerUtility.getClass(owner);
				do {
					try {
						field = objectClass.getDeclaredField(fieldName);
					} catch (NoSuchFieldException | SecurityException e2) {
					}
					objectClass = objectClass.getSuperclass();
				} while (objectClass != null && field == null);
			}
			if (field != null) {
				collection = ReflectionUtility.readField(owner, field);
			}
		}
		if (collection instanceof Map) {
			((Map<?, ?>) collection).clear();
		} else if (collection instanceof Collection) {
			((Collection<?>) collection).clear();
		}
	}

	@SuppressWarnings("unchecked")
	static void addReferenceCount(QtObjectInterface owner, Class<? extends QtObjectInterface> declaringClass, String fieldName, boolean isThreadSafe, boolean isStatic, Object value) {
		Object collection = null;
		boolean got = false;
		if (declaringClass.isInterface() && !isStatic) {
			NativeLink link = NativeUtility.findInterfaceLink(owner, false);
			if (link instanceof NativeUtility.InterfaceNativeLink) {
				collection = ((NativeUtility.InterfaceNativeLink) link).getReferenceCountCollection(declaringClass, fieldName, RCList::new);
				got = true;
			}
		}
		if (!got) {
			Field field = null;
			try {
				field = declaringClass.getDeclaredField(fieldName);
			} catch (NoSuchFieldException | SecurityException e2) {
			}
			if (field == null && owner != null) {
				Class<?> objectClass = ClassAnalyzerUtility.getClass(owner);
				do {
					try {
						field = objectClass.getDeclaredField(fieldName);
					} catch (NoSuchFieldException | SecurityException e2) {
					}
					objectClass = objectClass.getSuperclass();
				} while (objectClass != null && field == null);
			}
			if (field != null) {
				collection = ReflectionUtility.readField(owner, field);
				if (collection == null) {
					if (isThreadSafe) {
						collection = java.util.Collections.synchronizedList(new java.util.ArrayList<>());
					} else {
						collection = new java.util.ArrayList<>();
					}
					ReflectionUtility.writeField(owner, field, collection);
				}
			}
		}
		if (collection instanceof Collection) {
			((Collection<Object>) collection).add(value);
		}
	}
	
	static void setReferenceCount(QtObjectInterface owner, Class<? extends QtObjectInterface> declaringClass, String fieldName, boolean threadSafe, boolean isStatic, Object newValue) {
		if (threadSafe) {
			synchronized (isStatic ? declaringClass : owner) {
				setReferenceCount(owner, declaringClass, fieldName, isStatic, newValue);
			}
		} else {
			setReferenceCount(owner, declaringClass, fieldName, isStatic, newValue);
		}
	}

	static void setReferenceCount(QtObjectInterface owner, Class<? extends QtObjectInterface> declaringClass,
			String fieldName, boolean isStatic, Object newValue) {
		if (declaringClass.isInterface() && !isStatic) {
			NativeLink link = NativeUtility.findInterfaceLink(owner, true);
			if (link instanceof NativeUtility.InterfaceNativeLink) {
				((NativeUtility.InterfaceNativeLink) link).setReferenceCount(declaringClass, fieldName, newValue);
				return;
			}
		}
		Field field = null;
		try {
			field = declaringClass.getDeclaredField(fieldName);
		} catch (NoSuchFieldException | SecurityException e2) {
		}
		if (field == null && owner != null) {
			Class<?> objectClass = ClassAnalyzerUtility.getClass(owner);
			do {
				try {
					field = objectClass.getDeclaredField(fieldName);
				} catch (NoSuchFieldException | SecurityException e2) {
				}
				objectClass = objectClass.getSuperclass();
			} while (objectClass != null && field == null);
		}
		if (field != null) {
			ReflectionUtility.writeField(owner, field, newValue);
		}
	}

	static Map<Object, Object> newRCMap() {
		return new RCMap();
	}

	static List<Object> newRCList() {
		return new RCList();
	}
}
