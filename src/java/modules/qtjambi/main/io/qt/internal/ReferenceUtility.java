/****************************************************************************
**
** Copyright (C) 2009-2026 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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

import java.lang.ref.WeakReference;
import java.lang.reflect.Field;
import java.util.AbstractMap;
import java.util.ArrayList;
import java.util.Collection;
import java.util.Collections;
import java.util.HashSet;
import java.util.IdentityHashMap;
import java.util.Iterator;
import java.util.List;
import java.util.Map;
import java.util.Map.Entry;
import java.util.Set;
import java.util.function.Function;

import io.qt.NativeAccess;
import io.qt.QtObjectInterface;
import io.qt.core.QMetaObject;
import io.qt.internal.NativeUtility.NativeLink;

/**
 * @hidden
 */
abstract class ReferenceUtility {
	private ReferenceUtility() {
		throw new RuntimeException();
	}

	static {
		QtJambi_LibraryUtilities.initialize();
	}

	private static class RCList extends ArrayList<Object> {
		@NativeAccess
		public RCList() {
			super();
			if(LibraryUtility.operatingSystem==LibraryUtility.OperatingSystem.Android) {
				WeakReference<RCList> _this = new WeakReference<>(this);
				checkSlot = ()->{
					RCList ref = _this.get();
					if(ref!=null)
						ref.check();
				};
			}else {
				checkSlot = this::check;
			}
		}

		private static final long serialVersionUID = -4010060446825990721L;
		private final QMetaObject.Slot0 checkSlot;

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
		
		boolean needsReferenceCounting(QtObjectInterface i) {
			return ReferenceUtility.needsReferenceCounting(NativeUtility.nativeId(i));
		}

		@Override
		@NativeAccess
		public boolean add(Object e) {
			if(e instanceof QtObjectInterface) {
				if(!needsReferenceCounting((QtObjectInterface)e))
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
	
	private static class UncheckedRCList extends RCList {
		private static final long serialVersionUID = -1665613298886703452L;
		@Override
		boolean needsReferenceCounting(QtObjectInterface i) {
			return true;
		}
	}
	
	private final static class Ref implements Cloneable{
		static final Function<Object,Ref> FACTORY = Ref::new;
		Ref(Object obj){}
		private Ref(Ref ref){this.ref = ref.ref;}
		public Ref clone() { return new Ref(this); }
		private int ref = 0;
		int ref() {
			return ++ref;
		}
		int deref() {
			return --ref;
		}
	}
	
	@NativeAccess
	private static class RCSet implements Collection<Object> {
		static final Function<Object,RCSet> FACTORY = RCSet::new;
		RCSet(Object obj){
			this();
		}
		
		private final Map<Object,Ref> map = new IdentityHashMap<>();
		
		@NativeAccess
		public RCSet() {
			super();
			if(LibraryUtility.operatingSystem==LibraryUtility.OperatingSystem.Android) {
				WeakReference<RCSet> _this = new WeakReference<>(this);
				checkSlot = ()->{
					RCSet ref = _this.get();
					if(ref!=null)
						ref.check();
				};
			}else {
				checkSlot = this::check;
			}
		}

		private final QMetaObject.Slot0 checkSlot;

		private synchronized void check() {
			List<Object> disposedElements = null;
			synchronized(map) {
				for (Object o : map.keySet()) {
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
						map.remove(o);
					}
				}
			}
		}

		boolean needsReferenceCounting(QtObjectInterface i) {
			return ReferenceUtility.needsReferenceCounting(NativeUtility.nativeId(i));
		}

		@Override
		@NativeAccess
		public boolean add(Object e) {
			if(e instanceof QtObjectInterface) {
				QtObjectInterface i = (QtObjectInterface)e;
				if(!needsReferenceCounting(i))
					return false;
				int ref;
				synchronized(map) {
					ref = map.computeIfAbsent(i, Ref.FACTORY).ref();
				}
				if(ref==1) {
					QMetaObject.DisposedSignal disposed = NativeUtility.getSignalOnDispose(i, true);
					if (disposed != null)
						disposed.connect(checkSlot);
				}
			}else {
				synchronized(map) {
					map.computeIfAbsent(e, Ref.FACTORY).ref();
				}
			}
			return true;
		}
		
		@Override
		@NativeAccess
		public boolean remove(Object o) {
			boolean result = false;
			synchronized(map) {
				Ref ref = map.get(o);
				if(ref!=null) {
					if(ref.deref()==0) {
						map.remove(o);
						if (o instanceof QtObjectInterface) {
							QMetaObject.DisposedSignal disposed = NativeUtility.getSignalOnDispose((QtObjectInterface) o, true);
							if (disposed != null)
								disposed.disconnect(checkSlot);
						}
					}
					result = true;
				}
			}
			return result;
		}

		@Override
		@NativeAccess
		public void clear() {
			Set<Object> disposedElements;
			synchronized(map) {
				disposedElements = new IdentityHashMap<>(map).keySet();
				map.clear();
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

		@Override
		public int size() {
			synchronized(map) {
				return map.size();
			}
		}

		@Override
		public boolean isEmpty() {
			return size()==0;
		}

		@Override
		public boolean contains(Object o) {
			synchronized(map) {
				return map.containsKey(o);
			}
		}

		@Override
		public Iterator<Object> iterator() {
			synchronized(map) {
				return new Iterator<Object>() {
					final Iterator<Entry<Object, Ref>> iter = map.entrySet().iterator();
					Object current;
					Ref ref = advance();
					
					private Ref advance(){
						if(iter.hasNext()) {
							Entry<Object, Ref> entry = iter.next();
							current = entry.getKey();
							return entry.getValue().clone();
						}else {
							return null;
						}
					}

					@Override
					public boolean hasNext() {
						return ref!=null;
					}

					@Override
					public Object next() {
						Object next = current;
						if(ref.deref()==0) {
							synchronized(map) {
								ref = advance();
							}
						}
						return next;
					}
				};
			}
		}

		@Override
		public Object[] toArray() {
			return null;
		}

		@Override
		public <T> T[] toArray(T[] a) {
			return null;
		}

		@Override
		public boolean containsAll(Collection<?> c) {
			synchronized(map) {
				for (Object o : c) {
					if(!map.containsKey(o))
						return false;
				}
			}
			return true;
		}

		@Override
		public boolean retainAll(Collection<?> c) {
			return false;
		}

		@Override
		public boolean removeAll(Collection<?> c) {
			boolean result = false;
			synchronized(map) {
				for (Object o : c) {
					Ref ref = map.get(o);
					if(ref!=null) {
						if(ref.deref()==0) {
							map.remove(o);
							if (o instanceof QtObjectInterface) {
								QMetaObject.DisposedSignal disposed = NativeUtility.getSignalOnDispose((QtObjectInterface) o, true);
								if (disposed != null)
									disposed.disconnect(checkSlot);
							}
						}
						result = true;
					}
				}
			}
			return result;
		}
	}
	
	private static class RCMap extends IdentityHashMap<Object, Object> {
		private static final long serialVersionUID = 3076251074218500284L;
		private final QMetaObject.Slot0 checkSlot;

		@NativeAccess
		public RCMap() {
			super();
			if(LibraryUtility.operatingSystem==LibraryUtility.OperatingSystem.Android) {
				WeakReference<RCMap> _this = new WeakReference<>(this);
				checkSlot = ()->{
					RCMap ref = _this.get();
					if(ref!=null)
						ref.check();
				};
			}else {
				checkSlot = this::check;
			}
		}
		
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
		
		boolean needsReferenceCounting(QtObjectInterface i) {
			return ReferenceUtility.needsReferenceCounting(NativeUtility.nativeId(i));
		}

		@Override
		public Object put(Object key, Object value) {
			if (key instanceof QtObjectInterface || value instanceof QtObjectInterface) {
				if (key instanceof QtObjectInterface && value instanceof QtObjectInterface) {
					if(!needsReferenceCounting((QtObjectInterface)key) && !needsReferenceCounting((QtObjectInterface)value))
						return false;
				}else if (key instanceof QtObjectInterface) {
					if(!needsReferenceCounting((QtObjectInterface)key))
						return false;
				}else {
					if(!needsReferenceCounting((QtObjectInterface)value))
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
	
	@NativeAccess
	private static class RCMultiMap implements Map<Object, Object> {
		Map<Object, RCSet> map = new IdentityHashMap<>();
		private final QMetaObject.Slot0 checkSlot;

		@NativeAccess
		public RCMultiMap() {
			super();
			if(LibraryUtility.operatingSystem==LibraryUtility.OperatingSystem.Android) {
				WeakReference<RCMultiMap> _this = new WeakReference<>(this);
				checkSlot = ()->{
					RCMultiMap ref = _this.get();
					if(ref!=null)
						ref.check();
				};
			}else {
				checkSlot = this::check;
			}
		}
		
		private synchronized void check() {
			synchronized(map) {
				List<Object> disposedKeys = null;
				for (Entry<Object, RCSet> entry : map.entrySet()) {
					if (entry.getKey() instanceof QtObjectInterface
							&& ((QtObjectInterface) entry.getKey()).isDisposed()) {
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
				if (disposedKeys != null) {
					for (Object key : disposedKeys) {
						map.remove(key);
					}
				}
			}
		}
		
		boolean needsReferenceCounting(QtObjectInterface i) {
			return ReferenceUtility.needsReferenceCounting(NativeUtility.nativeId(i));
		}

		@Override
		public Object put(Object key, Object value) {
			if (key instanceof QtObjectInterface || value instanceof QtObjectInterface) {
				if (key instanceof QtObjectInterface && value instanceof QtObjectInterface) {
					if(!needsReferenceCounting((QtObjectInterface)key) && !needsReferenceCounting((QtObjectInterface)value))
						return false;
				}else if (key instanceof QtObjectInterface) {
					if(!needsReferenceCounting((QtObjectInterface)key))
						return false;
				}else {
					if(!needsReferenceCounting((QtObjectInterface)value))
						return false;
				}
			}

			synchronized (map) {
				map.computeIfAbsent(key, RCSet.FACTORY).add(value);
			}
			if (key instanceof QtObjectInterface) {
				QMetaObject.DisposedSignal disposed = NativeUtility.getSignalOnDispose((QtObjectInterface) key, true);
				if (disposed != null)
					disposed.connect(checkSlot);
			}
			return null;
		}

		@Override
		public Object remove(Object key) {
			RCSet result;
			synchronized (map) {
				result = map.remove(key);
			}
			if (key instanceof QtObjectInterface) {
				QMetaObject.DisposedSignal disposed = NativeUtility.getSignalOnDispose((QtObjectInterface) key, true);
				if (disposed != null)
					disposed.disconnect(checkSlot);
			}
			return result!=null && !result.isEmpty() ? result.iterator().next() : null;
		}

		@Override
		public void clear() {
			IdentityHashMap<Object, RCSet> _this;
			synchronized (map) {
				_this = new IdentityHashMap<>(map);
				map.clear();
			}
			for (Entry<Object, RCSet> entry : _this.entrySet()) {
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
			boolean result = false;
			RCSet set;
			synchronized (map) {
				set = map.get(key);
			}
			if (set!=null) {
				result = set.remove(value);
				if(set.isEmpty()) {
					if (key instanceof QtObjectInterface) {
						QMetaObject.DisposedSignal disposed = NativeUtility.getSignalOnDispose((QtObjectInterface) key, true);
						if (disposed != null)
							disposed.disconnect(checkSlot);
					}
					synchronized (map) {
						set = map.remove(key);
					}
				}
			}
			return result;
		}

		@Override
		public int size() {
			synchronized (map) {
				return map.size();
			}
		}

		@Override
		public boolean isEmpty() {
			return size()==0;
		}

		@Override
		public boolean containsKey(Object key) {
			synchronized (map) {
				return map.containsKey(key);
			}
		}

		@Override
		public boolean containsValue(Object value) {
			synchronized (map) {
				for (Entry<Object, RCSet> entry : map.entrySet()) {
					if(entry.getValue().contains(value))
						return true;
				}
			}
			return false;
		}

		@Override
		public Object get(Object key) {
			RCSet set;
			synchronized (map) {
				set = map.get(key);
			}
			return set!=null && !set.isEmpty() ? set.iterator().next() : null;
		}

		@Override
		public void putAll(Map<? extends Object, ? extends Object> m) {
			for(Entry<? extends Object, ? extends Object> entry : m.entrySet()) {
				put(entry.getKey(), entry.getValue());
			}
		}

		@Override
		public Set<Object> keySet() {
			synchronized (map) {
				return map.keySet();
			}
		}

		@Override
		public Collection<Object> values() {
			ArrayList<Object> result = new ArrayList<>();
			synchronized (map) {
				for (Entry<Object, RCSet> entry : map.entrySet()) {
					result.addAll(entry.getValue());
				}
			}
			return result;
		}

		@Override
		public Set<Entry<Object, Object>> entrySet() {
			HashSet<Entry<Object, Object>> result = new HashSet<>();
			synchronized (map) {
				for (Entry<Object, RCSet> entry : map.entrySet()) {
					for(Object value : entry.getValue())
						result.add(new AbstractMap.SimpleImmutableEntry<>(entry.getKey(), value));
				}
			}
			return result;
		}
	}
	
	private static class UncheckedRCMap extends RCMap {
		private static final long serialVersionUID = -1215145480483063080L;
		@Override
		boolean needsReferenceCounting(QtObjectInterface i) {
			return true;
		}
	}

	static void removeFromMapReferenceCount(QtObjectInterface owner, Class<? extends QtObjectInterface> declaringClass, String fieldName, boolean isStatic, Object value) {
		Object collection = null;
		boolean got = false;
		if (declaringClass!=null && declaringClass.isInterface() && !isStatic) {
			NativeLink link = NativeUtility.findInterfaceLink(owner, false);
			if (link instanceof NativeUtility.ReferenceCountingNativeLink) {
				collection = ((NativeUtility.ReferenceCountingNativeLink) link).getReferenceCountCollection(declaringClass, fieldName, null);
				got = true;
			}
		}
		if (!got) {
			Field field = null;
			if(declaringClass!=null) {
				try {
					field = declaringClass.getDeclaredField(fieldName);
				} catch (NoSuchFieldException | SecurityException e2) {
				}
			}
			if (field == null && owner != null) {
				Class<?> objectClass = AccessUtility.instance.getClass(owner);
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
		if (declaringClass!=null && declaringClass.isInterface() && !isStatic) {
			NativeLink link = NativeUtility.findInterfaceLink(owner, false);
			if (link instanceof NativeUtility.ReferenceCountingNativeLink) {
				collection = ((NativeUtility.ReferenceCountingNativeLink) link).getReferenceCountCollection(declaringClass, fieldName, null);
				got = true;
			}
		}
		if (!got) {
			Field field = null;
			if(declaringClass!=null) {
				try {
					field = declaringClass.getDeclaredField(fieldName);
				} catch (NoSuchFieldException | SecurityException e2) {
				}
			}
			if (field == null && owner != null) {
				Class<?> objectClass = AccessUtility.instance.getClass(owner);
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
		if (declaringClass!=null && declaringClass.isInterface() && !isStatic) {
			NativeLink link = NativeUtility.findInterfaceLink(owner, false);
			if (link instanceof NativeUtility.ReferenceCountingNativeLink) {
				collection = ((NativeUtility.ReferenceCountingNativeLink) link).getReferenceCountCollection(declaringClass, fieldName, RCList::new);
				got = true;
			}
		}
		if (!got) {
			Field field = null;
			if (declaringClass!=null){
				try {
					field = declaringClass.getDeclaredField(fieldName);
				} catch (NoSuchFieldException | SecurityException e2) {
				}
			}
			if (field == null && owner != null) {
				Class<?> objectClass = AccessUtility.instance.getClass(owner);
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
		if (declaringClass!=null && declaringClass.isInterface() && !isStatic) {
			NativeLink link = NativeUtility.findInterfaceLink(owner, false);
			if (link instanceof NativeUtility.ReferenceCountingNativeLink) {
				map = ((NativeUtility.ReferenceCountingNativeLink) link).getReferenceCountCollection(declaringClass, fieldName, () -> {
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
			if (declaringClass!=null){
				try {
					field = declaringClass.getDeclaredField(fieldName);
				} catch (NoSuchFieldException | SecurityException e2) {
				}
			}
			if (field == null && owner != null) {
				Class<?> objectClass = AccessUtility.instance.getClass(owner);
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
		if (declaringClass!=null && declaringClass.isInterface() && !isStatic) {
			NativeLink link = NativeUtility.findInterfaceLink(owner, false);
			if (link instanceof NativeUtility.ReferenceCountingNativeLink) {
				collection = ((NativeUtility.ReferenceCountingNativeLink) link).getReferenceCountCollection(declaringClass, fieldName, null);
				got = true;
			}
		}
		if (!got) {
			Field field = null;
			if(declaringClass!=null) {
				try {
					field = declaringClass.getDeclaredField(fieldName);
				} catch (NoSuchFieldException | SecurityException e2) {
				}
			}
			if (field == null && owner != null) {
				Class<?> objectClass = AccessUtility.instance.getClass(owner);
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
		if (declaringClass!=null && declaringClass.isInterface() && !isStatic) {
			NativeLink link = NativeUtility.findInterfaceLink(owner, false);
			if (link instanceof NativeUtility.ReferenceCountingNativeLink) {
				collection = ((NativeUtility.ReferenceCountingNativeLink) link).getReferenceCountCollection(declaringClass, fieldName, RCList::new);
				got = true;
			}
		}
		if (!got) {
			Field field = null;
			if(declaringClass!=null) {
				try {
					field = declaringClass.getDeclaredField(fieldName);
				} catch (NoSuchFieldException | SecurityException e2) {
				}
			}
			if (field == null && owner != null) {
				Class<?> objectClass = AccessUtility.instance.getClass(owner);
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
	
	static void copyReferenceCount(QtObjectInterface owner, Class<? extends QtObjectInterface> declaringClass, String fieldName, QtObjectInterface copy) {
		Field field = null;
		Object newValue = null;
		if(declaringClass!=null) {
			if (declaringClass.isInterface()) {
				NativeLink link = NativeUtility.findInterfaceLink(copy, true);
				if (link instanceof NativeUtility.ReferenceCountingNativeLink) {
					newValue = ((NativeUtility.ReferenceCountingNativeLink) link).getReferenceCount(declaringClass, fieldName);
					link = NativeUtility.findInterfaceLink(owner, true);
					if (link instanceof NativeUtility.ReferenceCountingNativeLink) {
						((NativeUtility.ReferenceCountingNativeLink) link).setReferenceCount(declaringClass, fieldName, newValue);
						return;
					}
				}
			}
			try {
				field = declaringClass.getDeclaredField(fieldName);
			} catch (NoSuchFieldException | SecurityException e2) {
			}
		}
		if (field == null && copy != null) {
			Class<?> objectClass = AccessUtility.instance.getClass(copy);
			do {
				try {
					field = objectClass.getDeclaredField(fieldName);
				} catch (NoSuchFieldException | SecurityException e2) {
				}
				objectClass = objectClass.getSuperclass();
			} while (objectClass != null && field == null);
		}
		if (field != null) {
			if(newValue==null) {
				newValue = ReflectionUtility.readField(copy, field);
			}
			if (declaringClass!=null && declaringClass.isInterface()) {
				NativeLink link = NativeUtility.findInterfaceLink(owner, true);
				if (link instanceof NativeUtility.ReferenceCountingNativeLink) {
					((NativeUtility.ReferenceCountingNativeLink) link).setReferenceCount(declaringClass, fieldName, newValue);
					return;
				}
			}
			ReflectionUtility.writeField(owner, field, newValue);
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

	private static void setReferenceCount(QtObjectInterface owner, Class<? extends QtObjectInterface> declaringClass,
			String fieldName, boolean isStatic, Object newValue) {
		Field field = null;
		if(declaringClass!=null) {
			if (declaringClass.isInterface() && !isStatic) {
				NativeLink link = NativeUtility.findInterfaceLink(owner, true);
				if (link instanceof NativeUtility.ReferenceCountingNativeLink) {
					((NativeUtility.ReferenceCountingNativeLink) link).setReferenceCount(declaringClass, fieldName, newValue);
					return;
				}
			}
			try {
				field = declaringClass.getDeclaredField(fieldName);
			} catch (NoSuchFieldException | SecurityException e2) {
			}
		}
		if (field == null && owner != null) {
			Class<?> objectClass = AccessUtility.instance.getClass(owner);
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
		return new UncheckedRCMap();
	}

	static List<Object> newRCList() {
		return new UncheckedRCList();
	}
	
	static native boolean needsReferenceCounting(long native__id);
}
