/****************************************************************************
**
** Copyright (C) 2009-2024 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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

import java.io.File;
import java.lang.ref.Reference;
import java.lang.ref.ReferenceQueue;
import java.lang.ref.WeakReference;
import java.util.Arrays;
import java.util.Collections;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.function.Function;
import java.util.function.Supplier;

import io.qt.InternalAccess;
import io.qt.InternalAccess.Cleanable;
import io.qt.NativeAccess;
import io.qt.QNoNativeResourcesException;
import io.qt.QtObject;
import io.qt.QtObjectInterface;
import io.qt.QtUninvokable;
import io.qt.QtUtilities;
import io.qt.QtUtilities.LibraryRequirementMode;
import io.qt.core.QMetaObject;
import io.qt.core.QObject;

/**
 * @hidden
 */
public abstract class NativeUtility {
	private static Function<java.lang.Object, QMetaObject.DisposedSignal> disposedSignalFactory;
	private static final Map<Integer, InterfaceNativeLink> interfaceLinks;
	private static final Map<NativeLink, QMetaObject.DisposedSignal> disposedSignals;
	private static final Map<AssociativeReference, java.lang.Object> object2ObjectAssociations = new HashMap<>();
	static final ReferenceQueue<java.lang.Object> referenceQueue = new ReferenceQueue<>();
	private static final Thread cleanupRegistrationThread;
	static {
		interfaceLinks = Collections.synchronizedMap(new HashMap<>());
		disposedSignals = Collections.synchronizedMap(new HashMap<>());
		cleanupRegistrationThread = new Thread(() -> {
			while (true) {
				if(Thread.interrupted())
					break;
				try {
					Reference<?> ref = referenceQueue.remove();
					try {
						if(Thread.interrupted())
							break;
						if (ref instanceof Cleanable) {
							((Cleanable) ref).clean();
							if(Thread.interrupted())
								break;
						}
					} finally {
						ref = null;
					}
				} catch (InterruptedException e) {
					break;
				} catch (Throwable e) {
					e.printStackTrace();
				}
				if(Thread.interrupted())
					break;
			}
		});
		cleanupRegistrationThread.setName("QtJambiCleanupThread");
		cleanupRegistrationThread.setDaemon(true);
		QtJambi_LibraryUtilities.initialize();
		cleanupRegistrationThread.start();
	}

	@NativeAccess
	@QtUninvokable
	private static void terminateCleanupThread() throws Throwable {
		switch (cleanupRegistrationThread.getState()) {
		case TERMINATED:
			break;
		default:
			cleanupRegistrationThread.interrupt();
			cleanupRegistrationThread.join();
			break;
		}
	}

	static class NativeLink extends WeakReference<QtObjectInterface> implements Cleanable {

		private NativeLink(QtObjectInterface object) {
			super(object, NativeUtility.referenceQueue);
//			cls = object == null ? null : object.getClass();
		}

//		Class<?> cls;
		private @NativeAccess long native__id = 0;

		@NativeAccess
		private final void detach(long native__id, boolean hasDisposedSignal) {
			QMetaObject.DisposedSignal disposed = hasDisposedSignal ? takeSignalOnDispose(this) : null;
			boolean detached = false;
			synchronized (this) {
				if (this.native__id == native__id) {
					this.native__id = 0;
					detached = true;
				}
			}
			if (disposed != null) {
				try {
					if (detached)
						disposed.emitSignal();
					disposed.disconnect();
				} catch (Throwable e) {
					e.printStackTrace();
				}
			}
			if (detached)
				enqueue();
		}

		@Override
		public synchronized void clean() {
			if (native__id != 0) {
				clean(native__id);
			} else {
				QMetaObject.DisposedSignal disposed = takeSignalOnDispose(this);
				if (disposed != null)
					disposed.disconnect();
			}
		}

		@NativeAccess
		private final synchronized void reset() {
			if (native__id != 0 && hasDisposedSignal(native__id)) {
				QMetaObject.DisposedSignal disposed = takeSignalOnDispose(this);
				if (disposed != null)
					disposed.disconnect();
			}
			this.native__id = 0;
		}

		private static native void clean(long native__id);
		private static native boolean hasDisposedSignal(long native__id);
		private static native void setHasDisposedSignal(long native__id);

		final synchronized void dispose() {
			if (native__id != 0) {
				dispose(native__id);
			}
		}

		final synchronized boolean isDisposed() {
			return native__id == 0;
		}

		private static native void dispose(long native__id);

		private static native String qtTypeName(long native__id);

		java.lang.Object getMemberAccess(Class<?> interfaceClass) {
			throw new RuntimeException("Requesting member access of non-interface object is not permitted.");
		}

		void initialize(NativeUtility.Object obj) {
		}

		@Override
		public final String toString() {
			QtObjectInterface o = get();
			if (o != null) {
				return ClassAnalyzerUtility.getClass(o).getName() + "@" + Integer.toHexString(System.identityHashCode(o));
			} else {
				String qtTypeName = null;
				synchronized (this) {
					if (native__id != 0) {
						qtTypeName = qtTypeName(native__id);
					}
				}
				if (qtTypeName != null)
					return qtTypeName + "[disposed]";
				return super.toString();
			}
		}

		@Override
		public synchronized int hashCode() {
			return Long.hashCode(native__id);
		}

		@Override
		public boolean equals(java.lang.Object obj) {
			if (this == obj)
				return true;
			if (obj instanceof NativeLink)
				return equals((NativeLink) obj);
			return false;
		}
		
		synchronized boolean equals(NativeLink other) {
			return native__id == other.native__id;
		}

		public synchronized long nativeId() {
			return native__id;
		}
	}

	static class InterfaceNativeLink extends NativeLink {

		private static final Map<Class<?>, Function<java.lang.Object,java.lang.Object>> memberAccessConstructorHandles = new HashMap<>();

		private final Map<Class<?>, java.lang.Object> memberAccesses;

		private InterfaceNativeLink(QtObjectInterface object, List<Class<? extends QtObjectInterface>> interfaces) {
			super(object);
			Map<Class<?>, java.lang.Object> memberAccesses = new HashMap<>();
			synchronized (memberAccessConstructorHandles) {
				for (Class<? extends QtObjectInterface> _iface : interfaces) {
					@SuppressWarnings("unchecked")
					Function<java.lang.Object,java.lang.Object> constructorHandle = memberAccessConstructorHandles.computeIfAbsent(_iface, iface -> {
						for (Class<?> innerClass : iface.getClasses()) {
							if (io.qt.MemberAccess.class.isAssignableFrom(innerClass)) {
								try {
									return (Function<java.lang.Object,java.lang.Object>)ReflectionUtility.methodInvocationHandler.getFactory1(innerClass.getDeclaredConstructor(iface));
								} catch (Exception e) {
									e.printStackTrace();
								}
							}
						}
						return null;
					});
					if (constructorHandle != null) {
						try {
							java.lang.Object memberAccess = constructorHandle.apply(object);
							memberAccesses.put(_iface, memberAccess);
						} catch (Throwable e) {
							e.printStackTrace();
						}
					}
				}
			}
			this.memberAccesses = Collections.unmodifiableMap(memberAccesses);
		}

		java.lang.Object getMemberAccess(Class<?> interfaceClass) {
			return memberAccesses.get(interfaceClass);
		}

		private Map<Class<?>, Map<String, java.lang.Object>> referenceCounts;

		public void setReferenceCount(Class<? extends QtObjectInterface> declaringClass, String fieldName,
				java.lang.Object newValue) {
			if (referenceCounts == null) {
				referenceCounts = Collections.synchronizedMap(new HashMap<>());
			}
			Map<String, java.lang.Object> referenceCountsVariables = referenceCounts.computeIfAbsent(declaringClass,
					c -> Collections.synchronizedMap(new HashMap<>()));
			referenceCountsVariables.put(fieldName, newValue);
		}
		
		public java.lang.Object getReferenceCount(Class<? extends QtObjectInterface> declaringClass, String fieldName) {
			if (referenceCounts != null) {
				Map<String, java.lang.Object> referenceCountsVariables = referenceCounts.get(declaringClass);
				if(referenceCountsVariables!=null)
					return referenceCountsVariables.get(fieldName);
			}return null;
		}

		public java.lang.Object getReferenceCountCollection(Class<? extends QtObjectInterface> declaringClass, String fieldName,
				Supplier<java.lang.Object> collectionSupplier) {
			if (referenceCounts == null) {
				if (collectionSupplier != null) {
					referenceCounts = Collections.synchronizedMap(new HashMap<>());
					Map<String, java.lang.Object> referenceCountsVariables = referenceCounts.computeIfAbsent(declaringClass,
							c -> Collections.synchronizedMap(new HashMap<>()));
					java.lang.Object result = collectionSupplier.get();
					referenceCountsVariables.put(fieldName, result);
					return result;
				} else {
					return null;
				}
			} else {
				if (collectionSupplier != null) {
					return referenceCounts
							.computeIfAbsent(declaringClass, c -> Collections.synchronizedMap(new HashMap<>()))
							.computeIfAbsent(fieldName, _fieldName -> collectionSupplier.get());
				} else {
					return referenceCounts
							.computeIfAbsent(declaringClass, c -> Collections.synchronizedMap(new HashMap<>()))
							.get(fieldName);
				}
			}
		}

		@Override
		public synchronized void clean() {
			referenceCounts = null;
			super.clean();
		}

		void initialize(NativeUtility.Object obj) {
			initializeNativeObject(obj, this);
		}
	}

	private static final class InterfaceBaseNativeLink extends InterfaceNativeLink {
		final int ownerHashCode;

		private InterfaceBaseNativeLink(QtObjectInterface object, List<Class<? extends QtObjectInterface>> interfaces) {
			super(object, interfaces);
			ownerHashCode = System.identityHashCode(object);
			interfaceLinks.put(ownerHashCode, this);
		}

		@Override
		public synchronized void clean() {
			super.clean();
			interfaceLinks.remove(ownerHashCode);
		}
	}

	static NativeLink findInterfaceLink(QtObjectInterface iface, boolean forceCreation) {
		return findInterfaceLink(iface, forceCreation, forceCreation);
	}

	@NativeAccess
	private static NativeLink findInterfaceLink(QtObjectInterface iface, boolean forceCreation, boolean initialize) {
		if (iface instanceof NativeUtility.Object) {
			return ((NativeUtility.Object) iface).nativeLink;
		} else if (iface!=null){
			NativeLink link = interfaceLinks.get(System.identityHashCode(iface));
			if (link == null && forceCreation) {
				link = createNativeLink(iface);
				if (link == null) {
					link = new InterfaceBaseNativeLink(iface, Collections.emptyList());
				}else if (initialize) {
					initializeNativeObject(iface, link);
				}
			}
			return link;
		}else return null;
	}

	/**
	 * Emitted either as the native resources that belong to the object are being
	 * cleaned up or directly before the object is finalized. Connect to this signal
	 * to do clean up when the object is destroyed. The signal will never be emitted
	 * more than once per object, and the object is guaranteed to be unusable after
	 * this signal has returned.
	 */
	protected static QMetaObject.DisposedSignal getSignalOnDispose(QtObjectInterface object, boolean forceCreation) {
		return getSignalOnDispose(findInterfaceLink(object, forceCreation), forceCreation);
	}

	private static QMetaObject.DisposedSignal getSignalOnDispose(NativeLink nativeLink, boolean forceCreation) {
		if (nativeLink != null) {
			if (forceCreation) {
				long native__id = nativeLink.nativeId();
				if(native__id!=0) {
					NativeLink.setHasDisposedSignal(native__id);
					try {
						return disposedSignals.computeIfAbsent(nativeLink, lnk -> {
							QtObjectInterface object = lnk.get();
							if(disposedSignalFactory==null)
								disposedSignalFactory = SignalUtility.<QMetaObject.DisposedSignal>getSignalFactory(QMetaObject.DisposedSignal.class);
							return disposedSignalFactory.apply(ClassAnalyzerUtility.getClass(object));
						});
					} catch (NullPointerException e) {
					}
				}
			} else
				return disposedSignals.get(nativeLink);
		}
		return null;
	}

	private static QMetaObject.DisposedSignal takeSignalOnDispose(NativeLink nativeLink) {
		return disposedSignals.remove(nativeLink);
	}

	@NativeAccess
	private static NativeLink createNativeLink(NativeUtility.Object object) {
		List<Class<? extends QtObjectInterface>> interfaces = getInterfaces(object);
		if (interfaces != null) {
			return new InterfaceNativeLink(object, interfaces);
		} else {
			return new NativeLink(object);
		}
	}

	@NativeAccess
	private static NativeLink createNativeLink(QtObjectInterface iface) {
		List<Class<? extends QtObjectInterface>> interfaces = getInterfaces(iface);
		if (interfaces != null) {
			return new InterfaceBaseNativeLink(iface, interfaces);
		} else {
			return null;
		}
	}

	protected static void initializeNativeObject(Class<?> declaringClass, QtObjectInterface object, Map<Class<?>, List<?>> arguments) throws IllegalArgumentException {
		initializeNativeObject(declaringClass, object, NativeUtility.findInterfaceLink(object, true, false), arguments);
	}

	private native static void initializeNativeObject(Class<?> callingClass, QtObjectInterface object, NativeLink link, Map<Class<?>, List<?>> arguments) throws IllegalArgumentException;

	static void initializeNativeObject(QtObjectInterface object, NativeLink link) throws IllegalArgumentException {
		Class<?> cls = ClassAnalyzerUtility.getClass(object);
		QtUtilities.initializePackage(cls);
		initializeNativeObject(cls, object, link, Collections.emptyMap());
	}
	
	/**
	 * @hidden
	 */
	protected static abstract class Object implements QtObjectInterface
	{
	    static {
	    	QtJambi_LibraryUtilities.initialize();
	    }

	    protected Object(){
	    	nativeLink = createNativeLink(this);
			nativeLink.initialize(this);
	    }
	    
	    protected Object(java.lang.Object privateConstructor){
	    	nativeLink = createNativeLink(this);
	    }
	    
		@Override
	    public void dispose() {
	    	nativeLink.dispose();
	    }
	    
	    @QtUninvokable
		@Override
	    public boolean isDisposed() {
	    	return nativeLink.isDisposed();
	    }
	    
	    @Override
	    public boolean equals(java.lang.Object other) {
	    	Boolean result = NativeUtility.areObjectsEquals(this, other);
	    	return result!=null ? result : super.equals(other);
	    }

	    final @NativeAccess NativeLink nativeLink;
	}

	protected static void disposeObject(NativeUtility.Object object) {
		object.nativeLink.dispose();
	}

	protected static boolean isObjectDisposed(NativeUtility.Object object) {
		return object.nativeLink.isDisposed();
	}
	
	protected static void disposeObject(QtObjectInterface object) {
		NativeLink lnk = findInterfaceLink(object, false);
		if (lnk != null) {
			lnk.dispose();
		}
	}

	protected static boolean isObjectDisposed(QtObjectInterface object) {
		NativeLink lnk = findInterfaceLink(object, true);
		return lnk == null || lnk.isDisposed();
	}

	protected static Boolean areObjectsEquals(NativeUtility.Object object, java.lang.Object other) {
		if (other instanceof NativeUtility.Object)
			return object.nativeLink.equals(((NativeUtility.Object) other).nativeLink);
		else
			return null;
	}

	private static class AssociativeReference extends WeakReference<java.lang.Object> implements Cleanable {

		public AssociativeReference(java.lang.Object r) {
			super(r, referenceQueue);
		}

		@Override
		public void clean() {
			synchronized (object2ObjectAssociations) {
				object2ObjectAssociations.remove(this);
			}
		}
	}

	@NativeAccess
	private static void createAssociation(java.lang.Object o1, java.lang.Object o2) {
		synchronized (object2ObjectAssociations) {
			AssociativeReference reference = new AssociativeReference(o1);
			object2ObjectAssociations.put(reference, o2);
			if(o2 instanceof QtObjectInterface) {
				QMetaObject.DisposedSignal disposed = getSignalOnDispose((QtObjectInterface)o2, true);
				if (disposed != null)
					disposed.connect(()->object2ObjectAssociations.remove(reference));
			}
		}
	}

	@NativeAccess
	private static boolean deleteAssociation(java.lang.Object o1) {
		AssociativeReference matchingReference = null;
		synchronized (object2ObjectAssociations) {
			for (AssociativeReference ref : object2ObjectAssociations.keySet()) {
				if (ref.get() == o1) {
					matchingReference = ref;
					break;
				}
			}
			if (matchingReference != null)
				object2ObjectAssociations.remove(matchingReference);
		}
		if (matchingReference != null) {
			matchingReference.enqueue();
			return true;
		} else
			return false;
	}

	@NativeAccess
	private static java.lang.Object findAssociation(java.lang.Object o1) {
		synchronized (object2ObjectAssociations) {
			AssociativeReference matchingReference = null;
			for (AssociativeReference ref : object2ObjectAssociations.keySet()) {
				if (ref.get() == o1) {
					matchingReference = ref;
					break;
				}
			}
			return matchingReference == null ? null : object2ObjectAssociations.get(matchingReference);
		}
	}
	
	static class CleanableReference extends WeakReference<java.lang.Object> implements InternalAccess.Cleanable{
		private InternalAccess.Cleanable cleanable;

		public CleanableReference(java.lang.Object referent) {
			super(referent, referenceQueue);
		}

		public void clean() {
			if(cleanable!=null)
				cleanable.clean();
		}
		
		public void setCleanable(InternalAccess.Cleanable cleanable) {
			this.cleanable = cleanable;
		}
	}

	private static final Map<Cleaner, Runnable> cleaners = new HashMap<>();

	private static class Cleaner extends WeakReference<java.lang.Object> implements Cleanable {

		public Cleaner(java.lang.Object r) {
			super(r, referenceQueue);
		}

		@Override
		public void clean() {
			synchronized (cleaners) {
				Runnable runnable = cleaners.remove(this);
				if (runnable != null) {
					try {
						runnable.run();
					} catch (Throwable e) {
						e.printStackTrace();
					}
				}
			}
		}
	}
	
	static InternalAccess.Cleanable registerCleaner(java.lang.Object object, Runnable action) {
		synchronized (cleaners) {
			Cleaner cleanable = new Cleaner(object);
			cleaners.put(cleanable, action);
			return cleanable;
		}
	}

	protected static void loadQtJambiLibrary(Class<?> callerClass, String library) {
		LibraryUtility.loadQtJambiLibrary(callerClass, library);
	}
	
	protected static void loadJambiLibrary(Class<?> callerClass, String library) {
		LibraryUtility.loadJambiLibrary(callerClass, library);
	}

	protected static boolean isAvailableQtLibrary(Class<?> callerClass, String library) {
		return LibraryUtility.isAvailableQtLibrary(callerClass, library);
	}

	protected static boolean isAvailableLibrary(String library, String version) {
		return LibraryUtility.isAvailableLibrary(library, version);
	}

	protected static void loadQtLibrary(Class<?> callerClass, String library, LibraryRequirementMode libraryRequirementMode, String...platforms) {
		LibraryUtility.loadQtLibrary(callerClass, library, libraryRequirementMode, platforms);
	}

	protected static void loadUtilityLibrary(String library, String version, LibraryRequirementMode libraryRequirementMode, String...platforms) {
		LibraryUtility.loadUtilityLibrary(library, version, libraryRequirementMode, platforms);
	}

	protected static void loadLibrary(String lib) {
		LibraryUtility.loadLibrary(lib);
	}
	
	protected static void useAsGadget(Class<?> clazz) {
		MetaObjectUtility.useAsGadget(clazz);
    }
    
	protected static void usePackageContentAsGadgets(String _package) {
    	MetaObjectUtility.usePackageContentAsGadgets(_package);
    }
	
	protected static File jambiDeploymentDir() {
		return LibraryUtility.jambiDeploymentDir();
	}

	protected static Supplier<Class<?>> callerClassProvider() {
		return RetroHelper.callerClassProvider();
	}

	protected static int majorVersion() {
		return QtJambi_LibraryUtilities.qtMajorVersion;
	}
	
	protected static int minorVersion() {
		return QtJambi_LibraryUtilities.qtMinorVersion;
	}
	
	protected static int qtjambiPatchVersion() {
		return QtJambi_LibraryUtilities.qtJambiPatch;
	}

	static boolean isSplitOwnership(Object object) {
		return isSplitOwnership(nativeId(object));
	}
	
	static boolean isCppOwnership(Object object) {
		return isCppOwnership(nativeId(object));
	}
	
	static boolean isJavaOwnership(Object object) {
		return isJavaOwnership(nativeId(object));
	}
	
	static boolean isSplitOwnership(QtObjectInterface object) {
		return isSplitOwnership(nativeId(object));
	}
	
	static boolean isCppOwnership(QtObjectInterface object) {
		return isCppOwnership(nativeId(object));
	}
	
	static boolean isJavaOwnership(QtObjectInterface object) {
		return isJavaOwnership(nativeId(object));
	}
	
	static void setCppOwnership(QtObjectInterface object) {
		setCppOwnership(nativeId(object));
	}
	
	static void setCppOwnership(Object object) {
		setCppOwnership(nativeId(object));
	}
	
	static void setJavaOwnership(QtObjectInterface object) {
		setJavaOwnership(nativeId(object));
	}
	
	static void setJavaOwnership(Object object) {
		setJavaOwnership(nativeId(object));
	}
	
	static void setDefaultOwnership(QtObjectInterface object) {
		setDefaultOwnership(nativeId(object));
	}
	
	static void setDefaultOwnership(Object object) {
		setDefaultOwnership(nativeId(object));
	}
	
	static void invalidateObject(QtObjectInterface object) {
		invalidateObject(nativeId(object));
	}
	
	static void invalidateObject(Object object) {
		invalidateObject(nativeId(object));
	}
	
	static boolean hasOwnerFunction(Object object) {
		return hasOwnerFunction(nativeId(object));
	}
	
	static boolean hasOwnerFunction(QtObjectInterface object) {
		return hasOwnerFunction(nativeId(object));
	}
	
	static QObject owner(Object object) {
		return owner(nativeId(object));
	}
	
	static QObject owner(QtObjectInterface object) {
		return owner(nativeId(object));
	}
	
	static long nativeId(QtObjectInterface object) {
		NativeLink nativeLink = NativeUtility.findInterfaceLink(object, true);
		if (nativeLink != null) {
			return nativeLink.nativeId();
		}
		return 0;
	}
	
	static long nativeId(Object obj) {
		if (obj != null && obj.nativeLink != null) {
			return obj.nativeLink.nativeId();
		}
		return 0;
	}

	static long checkedNativeId(QtObject object) {
		if(object==null)
			return 0;
		try {
			long nid = nativeId(object);
			if (nid == 0) {
				QNoNativeResourcesException e = new QNoNativeResourcesException(
						"Function call on incomplete object of type: " + ClassAnalyzerUtility.getClass(object).getName());
				StackTraceElement[] st = e.getStackTrace();
				st = Arrays.copyOfRange(st, 1, st.length);
				e.setStackTrace(st);
				throw e;
			}
			return nid;
		} catch (NullPointerException e) {
			StackTraceElement[] st = e.getStackTrace();
			st = Arrays.copyOfRange(st, 1, st.length);
			e.setStackTrace(st);
			throw e;
		}
	}

	static long checkedNativeId(QtObjectInterface object) {
		if(object==null)
			return 0;
		long nid = nativeId(object);
		if (nid == 0) {
			QNoNativeResourcesException e = new QNoNativeResourcesException(
					"Function call on incomplete object of type: " + ClassAnalyzerUtility.getClass(object).getName());
			StackTraceElement[] st = e.getStackTrace();
			st = Arrays.copyOfRange(st, 1, st.length);
			e.setStackTrace(st);
			throw e;
		}
		return nid;
	}
	
	static java.nio.ByteBuffer mutableData(io.qt.core.QByteArray byteArray){
		return mutableData(checkedNativeId(byteArray));
	}
	
	private native static java.nio.ByteBuffer mutableData(long nid);

	static void registerDependentObject(QtObjectInterface dependentObject, QtObjectInterface owner) {
		registerDependentObject(nativeId(dependentObject), nativeId(owner));
	}

	static void registerDependentObject(Object dependentObject, QtObjectInterface owner) {
		registerDependentObject(nativeId(dependentObject), nativeId(owner));
	}

	static void registerDependentObject(QtObjectInterface dependentObject, Object owner) {
		registerDependentObject(nativeId(dependentObject), nativeId(owner));
	}

	static void registerDependentObject(QtObject dependentObject, Object owner) {
		registerDependentObject(nativeId(dependentObject), nativeId(owner));
	}

	static void unregisterDependentObject(QtObjectInterface dependentObject, QtObjectInterface owner) {
		unregisterDependentObject(nativeId(dependentObject), nativeId(owner));
	}

	static void unregisterDependentObject(Object dependentObject, QtObjectInterface owner) {
		unregisterDependentObject(nativeId(dependentObject), nativeId(owner));
	}

	static void unregisterDependentObject(QtObjectInterface dependentObject, Object owner) {
		unregisterDependentObject(nativeId(dependentObject), nativeId(owner));
	}

	static void unregisterDependentObject(QtObject dependentObject, Object owner) {
		unregisterDependentObject(nativeId(dependentObject), nativeId(owner));
	}

	private static native void setCppOwnership(long native__id);

	private static native void setDefaultOwnership(long native__id);

	private static native void setJavaOwnership(long native__id);
	
	private static native boolean isCppOwnership(long native__id);

	private static native boolean isSplitOwnership(long native__id);

	private static native boolean isJavaOwnership(long native__id);

	private static native void invalidateObject(long native__id);
	
	private static native QObject owner(long native__id);

	private static native boolean hasOwnerFunction(long native__id);

	private native static void registerDependentObject(long dependentObject, long owner);

	private native static void unregisterDependentObject(long dependentObject, long owner);
	
	private native static List<Class<? extends QtObjectInterface>> getInterfaces(QtObjectInterface object);
}
