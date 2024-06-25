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
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collections;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.concurrent.atomic.AtomicReference;
import java.util.function.BiFunction;
import java.util.function.Function;
import java.util.function.Supplier;
import java.util.logging.Level;

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
	private static final java.util.logging.Logger CLEANUP_LOGGER = java.util.logging.Logger.getLogger("io.qt.cleanup");
	private static Function<java.lang.Object, QMetaObject.DisposedSignal> disposedSignalFactory;
	private static final Map<Integer, NativeLink> interfaceLinks;
	private static final Map<NativeLink, QMetaObject.DisposedSignal> disposedSignals;
	private static final Thread cleanupRegistrationThread;
	private static final Function<QtObjectInterface, NativeLink> new_NativeLink;
	private static final Function<QtObjectInterface, PureInterfaceNativeLink> new_PureInterfaceNativeLink;
	private static final Function<QtObjectInterface, ReferenceCountingNativeLink> new_ReferenceCountingNativeLink;
	private static final BiFunction<QtObjectInterface, Map<Class<? extends QtObjectInterface>, io.qt.MemberAccess<?>>, MemberAccessReferenceCountingNativeLink> new_MemberAccessReferenceCountingNativeLink;
	private static final BiFunction<QtObjectInterface, Map<Class<? extends QtObjectInterface>, io.qt.MemberAccess<?>>, MemberAccessPureInterfaceNativeLink> new_MemberAccessPureInterfaceNativeLink;
	static {
		interfaceLinks = Collections.synchronizedMap(new HashMap<>());
		disposedSignals = Collections.synchronizedMap(new HashMap<>());
		cleanupRegistrationThread = new Thread(QueuedCleaner::cleanup);
		cleanupRegistrationThread.setName("QtJambiCleanupThread");
		cleanupRegistrationThread.setDaemon(true);
		if(Boolean.getBoolean("io.qt.enable-cleanup-logs")) {
			new_NativeLink = LoggingNativeLink::new;
			new_ReferenceCountingNativeLink = ReferenceCountingLoggingNativeLink::new;
			new_MemberAccessPureInterfaceNativeLink = MemberAccessPureInterfaceLoggingNativeLink::new;
			new_PureInterfaceNativeLink = PureInterfaceLoggingNativeLink::new;
			new_MemberAccessReferenceCountingNativeLink = MemberAccessReferenceCountingLoggingNativeLink::new;
		}else {
			new_NativeLink = NativeLink::new;
			new_ReferenceCountingNativeLink = ReferenceCountingNativeLink::new;
			new_MemberAccessPureInterfaceNativeLink = MemberAccessPureInterfaceNativeLink::new;
			new_PureInterfaceNativeLink = PureInterfaceNativeLink::new;
			new_MemberAccessReferenceCountingNativeLink = MemberAccessReferenceCountingNativeLink::new;
		}
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
	
	static abstract class QueuedCleaner<Ref> extends WeakReference<Ref> implements Cleanable {
		private static final ReferenceQueue<java.lang.Object> referenceQueue = new ReferenceQueue<>();
		
		QueuedCleaner(Ref object) {
			super(object, referenceQueue);
		}
		
		static void cleanup() {
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
		}
	}

	static class NativeLink extends QueuedCleaner<QtObjectInterface> {

		private NativeLink(QtObjectInterface object) {
			super(object);
		}

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
		
		@Override
		public void dismiss() {
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

		synchronized void dispose() {
			if (native__id != 0) {
				dispose(native__id);
			}
		}

		final synchronized boolean isDisposed() {
			return native__id == 0;
		}

		private static native void dispose(long native__id);

		private static native String qtTypeName(long native__id);

		io.qt.MemberAccess<?> getMemberAccess(Class<? extends QtObjectInterface> interfaceClass) {
			throw new RuntimeException("Requesting member access of non-interface object is not permitted.");
		}

		void initialize(NativeUtility.Object obj) {
		}

		@Override
		public final String toString() {
			QtObjectInterface o = get();
			if (o != null) {
				return AccessUtility.instance.getClass(o).getName() + "@" + Integer.toHexString(System.identityHashCode(o));
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
	
	private static class LoggingNativeLink extends NativeLink {
		
		private final Class<?> cls;
		private final int hashCode;
		private LoggingNativeLink(QtObjectInterface object) {
			super(object);
			cls = AccessUtility.instance.getClass(object);
			hashCode = System.identityHashCode(object);
		}
		
		@Override
		public synchronized void clean() {
			if (nativeId() != 0) {
				NativeUtility.CLEANUP_LOGGER.log(Level.FINE, ()->String.format("Begin cleanup of 0x%1$s@%2$s...", Integer.toHexString(hashCode), cls.getName()));
				super.clean();
				NativeUtility.CLEANUP_LOGGER.log(Level.FINE, ()->String.format("Cleanup of 0x%1$s@%2$s finished.", Integer.toHexString(hashCode), cls.getName()));
			}
		}
	}
	
	static class ReferenceCountingNativeLink extends NativeLink {
		private ReferenceCountingNativeLink(QtObjectInterface object) {
			super(object);
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
	
	private static class ReferenceCountingLoggingNativeLink extends ReferenceCountingNativeLink {
		private final Class<?> cls;
		private final int hashCode;
		private ReferenceCountingLoggingNativeLink(QtObjectInterface object) {
			super(object);
			cls = AccessUtility.instance.getClass(object);
			hashCode = System.identityHashCode(object);
		}
		
		@Override
		public synchronized void clean() {
			if (nativeId() != 0) {
				NativeUtility.CLEANUP_LOGGER.log(Level.FINE, ()->String.format("Begin cleanup of 0x%1$s@%2$s...", Integer.toHexString(hashCode), cls.getName()));
				super.clean();
				NativeUtility.CLEANUP_LOGGER.log(Level.FINE, ()->String.format("Cleanup of 0x%1$s@%2$s finished.", Integer.toHexString(hashCode), cls.getName()));
			}
		}
	}

	private static class MemberAccessReferenceCountingNativeLink extends ReferenceCountingNativeLink {

		private final Map<Class<? extends QtObjectInterface>, io.qt.MemberAccess<?>> memberAccesses;

		private MemberAccessReferenceCountingNativeLink(QtObjectInterface object, Map<Class<? extends QtObjectInterface>, io.qt.MemberAccess<?>> memberAccesses) {
			super(object);
			this.memberAccesses = memberAccesses;
		}

		io.qt.MemberAccess<?> getMemberAccess(Class<? extends QtObjectInterface> interfaceClass) {
			return memberAccesses.get(interfaceClass);
		}
	}
	
	private static final class MemberAccessReferenceCountingLoggingNativeLink extends MemberAccessReferenceCountingNativeLink {
		private final Class<?> cls;
		private final int hashCode;
		private MemberAccessReferenceCountingLoggingNativeLink(QtObjectInterface object, Map<Class<? extends QtObjectInterface>, io.qt.MemberAccess<?>> memberAccesses) {
			super(object, memberAccesses);
			cls = AccessUtility.instance.getClass(object);
			hashCode = System.identityHashCode(object);
		}
		
		@Override
		public synchronized void clean() {
			if (nativeId() != 0) {
				NativeUtility.CLEANUP_LOGGER.log(Level.FINE, ()->String.format("Begin cleanup of 0x%1$s@%2$s...", Integer.toHexString(hashCode), cls.getName()));
				super.clean();
				NativeUtility.CLEANUP_LOGGER.log(Level.FINE, ()->String.format("Cleanup of 0x%1$s@%2$s finished.", Integer.toHexString(hashCode), cls.getName()));
			}
		}
	}

	private static class MemberAccessPureInterfaceNativeLink extends MemberAccessReferenceCountingNativeLink {
		final int ownerHashCode;

		private MemberAccessPureInterfaceNativeLink(QtObjectInterface object, Map<Class<? extends QtObjectInterface>, io.qt.MemberAccess<?>> memberAccesses) {
			super(object, memberAccesses);
			ownerHashCode = System.identityHashCode(object);
			interfaceLinks.put(ownerHashCode, this);
		}

		@Override
		public synchronized void clean() {
			super.clean();
			interfaceLinks.remove(ownerHashCode);
		}
		
		@Override
		synchronized void dispose() {
			super.dispose();
			interfaceLinks.remove(ownerHashCode);
		}
	}
	
	private static class MemberAccessPureInterfaceLoggingNativeLink extends MemberAccessPureInterfaceNativeLink {
		private final Class<?> cls;
		private MemberAccessPureInterfaceLoggingNativeLink(QtObjectInterface object, Map<Class<? extends QtObjectInterface>, io.qt.MemberAccess<?>> memberAccesses) {
			super(object, memberAccesses);
			cls = AccessUtility.instance.getClass(object);
		}
		
		@Override
		public synchronized void clean() {
			if (nativeId() != 0) {
				NativeUtility.CLEANUP_LOGGER.log(Level.FINE, ()->String.format("Begin cleanup of 0x%1$s@%2$s...", Integer.toHexString(ownerHashCode), cls.getName()));
				super.clean();
				NativeUtility.CLEANUP_LOGGER.log(Level.FINE, ()->String.format("Cleanup of 0x%1$s@%2$s finished.", Integer.toHexString(ownerHashCode), cls.getName()));
			}
		}
	}
	
	private static class PureInterfaceNativeLink extends ReferenceCountingNativeLink {
		final int ownerHashCode;

		private PureInterfaceNativeLink(QtObjectInterface object) {
			super(object);
			ownerHashCode = System.identityHashCode(object);
			interfaceLinks.put(ownerHashCode, this);
		}

		@Override
		public synchronized void clean() {
			super.clean();
			interfaceLinks.remove(ownerHashCode);
		}
		
		@Override
		synchronized void dispose() {
			super.dispose();
			interfaceLinks.remove(ownerHashCode);
		}
	}
	
	private static class PureInterfaceLoggingNativeLink extends PureInterfaceNativeLink {
		private final Class<?> cls;
		private PureInterfaceLoggingNativeLink(QtObjectInterface object) {
			super(object);
			cls = AccessUtility.instance.getClass(object);
		}
		
		@Override
		public synchronized void clean() {
			if (nativeId() != 0) {
				NativeUtility.CLEANUP_LOGGER.log(Level.FINE, ()->String.format("Begin cleanup of 0x%1$s@%2$s...", Integer.toHexString(ownerHashCode), cls.getName()));
				super.clean();
				NativeUtility.CLEANUP_LOGGER.log(Level.FINE, ()->String.format("Cleanup of 0x%1$s@%2$s finished.", Integer.toHexString(ownerHashCode), cls.getName()));
			}
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
					link = new_PureInterfaceNativeLink.apply(iface);
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
							return disposedSignalFactory.apply(AccessUtility.instance.getClass(object));
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

	private static Map<Class<? extends QtObjectInterface>, io.qt.MemberAccess<?>> findMemberAccesses(QtObjectInterface object){
		Map<Class<? extends QtObjectInterface>, Function<QtObjectInterface,io.qt.MemberAccess<?>>> interfaceInfos = getInterfaceInfos(object);
		Map<Class<? extends QtObjectInterface>, io.qt.MemberAccess<?>> memberAccesses;
		if (interfaceInfos != null) {
			memberAccesses = new HashMap<>();
			for (Map.Entry<Class<? extends QtObjectInterface>, Function<QtObjectInterface,io.qt.MemberAccess<?>>> entry : interfaceInfos.entrySet()) {
				Function<QtObjectInterface,io.qt.MemberAccess<?>> factory = entry.getValue();
				if (factory != null) {
					try {
						io.qt.MemberAccess<?> memberAccess = factory.apply(object);
						if(memberAccess!=null)
							memberAccesses.put(entry.getKey(), memberAccess);
					} catch (Throwable e) {
						e.printStackTrace();
					}
				}
			}
		}else {
			memberAccesses = null;
		}
		return memberAccesses;
	}

	private static NativeLink createNativeLink(NativeUtility.Object object) {
		Map<Class<? extends QtObjectInterface>, io.qt.MemberAccess<?>> memberAccesses = findMemberAccesses(object);
		if (memberAccesses != null) {
			if(memberAccesses.isEmpty())
				return new_ReferenceCountingNativeLink.apply(object);
			else
				return new_MemberAccessReferenceCountingNativeLink.apply(object, Collections.unmodifiableMap(memberAccesses));
		} else {
			return new_NativeLink.apply(object);
		}
	}

	private static NativeLink createNativeLink(QtObjectInterface iface) {
		Map<Class<? extends QtObjectInterface>, io.qt.MemberAccess<?>> memberAccesses = findMemberAccesses(iface);
		if (memberAccesses != null) {
			if(memberAccesses.isEmpty())
				return new_PureInterfaceNativeLink.apply(iface);
			else
				return new_MemberAccessPureInterfaceNativeLink.apply(iface, Collections.unmodifiableMap(memberAccesses));
		} else {
			return null;
		}
	}

	protected static void initializeNativeObject(Class<?> declaringClass, QtObjectInterface object, Map<Class<?>, List<?>> arguments) throws IllegalArgumentException {
		initializeNativeObject(declaringClass, object, NativeUtility.findInterfaceLink(object, true, false), arguments);
	}

	private native static void initializeNativeObject(Class<?> callingClass, QtObjectInterface object, NativeLink link, Map<Class<?>, List<?>> arguments) throws IllegalArgumentException;

	static void initializeNativeObject(QtObjectInterface object, NativeLink link) throws IllegalArgumentException {
		Class<?> cls = AccessUtility.instance.getClass(object);
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

	private static class AssociativeReference extends QueuedCleaner<java.lang.Object> {
		
		private static final List<AssociativeReference> associativeReferences = new ArrayList<>();
		
		private java.lang.Object association;

		AssociativeReference(java.lang.Object reference, java.lang.Object association) {
			super(reference);
			this.association = association;
			if(association!=null) {
				synchronized(associativeReferences) {
					associativeReferences.add(this);
				}
				if(reference instanceof QtObjectInterface) {
					QMetaObject.DisposedSignal disposed = getSignalOnDispose((QtObjectInterface)reference, true);
					if (disposed != null)
						disposed.connect(this::clean);
				}
			}
		}

		@Override
		public void clean() {
			if(this.association!=null) {
				this.association = null;
				synchronized(associativeReferences) {
					associativeReferences.remove(this);
				}
			}
		}
		
		@Override
		public boolean enqueue() {
			clean();
			return super.enqueue();
		}
		
		@Override
		public void dismiss() {
			enqueue();
		}

		java.lang.Object association() {
			return association;
		}
		
		static AssociativeReference find(java.lang.Object reference) {
			AssociativeReference result = null;
			synchronized(associativeReferences) {
				for (AssociativeReference ref : associativeReferences) {
					if(ref.get() == reference) {
						result = ref;
						break;
					}
				}
			}
			return result;
		}
		
		static AssociativeReference findByHashCode(int hashCode) {
			AssociativeReference result = null;
			synchronized(associativeReferences) {
				for (AssociativeReference ref : associativeReferences) {
					if(System.identityHashCode(ref) == hashCode) {
						result = ref;
						break;
					}
				}
			}
			return result;
		}
	}

	@NativeAccess
	private static int createAssociation(java.lang.Object o1, java.lang.Object o2) {
		return System.identityHashCode(new AssociativeReference(o1, o2));
	}

	@NativeAccess
	private static boolean deleteAssociation(java.lang.Object o1) {
		AssociativeReference matchingReference = AssociativeReference.find(o1);
		if (matchingReference != null) {
			matchingReference.enqueue();
			return true;
		} else
			return false;
	}
	
	@NativeAccess
	private static java.lang.Object deleteAssociationByHashCode(int hashCode) {
		AssociativeReference matchingReference = AssociativeReference.findByHashCode(hashCode);
		if (matchingReference != null) {
			matchingReference.enqueue();
			return matchingReference.get();
		} else
			return null;
	}
	
	@NativeAccess
	private static java.lang.Object findAssociation(java.lang.Object o1) {
		AssociativeReference matchingReference = AssociativeReference.find(o1);
		return matchingReference == null ? null : matchingReference.association();
	}

	static class FinalizerReference<Ref> extends QueuedCleaner<Ref> {
		private final AtomicReference<Runnable> runnableRef = new AtomicReference<Runnable>();
	
		FinalizerReference(Ref referent) {
			super(referent);
		}
		
		FinalizerReference(Ref referent, Runnable runnable) {
			super(referent);
			runnableRef.set(runnable);
		}
	
		@Override
		public void clean() {
			Runnable runnable = runnableRef.getAndSet(null);
			if(runnable!=null)
				runnable.run();
		}
		
		void setFinally(Runnable runnable) {
			runnableRef.set(runnable);
		}
		
		@Override
		public boolean enqueue() {
			runnableRef.set(null);
			return super.enqueue();
		}
		
		@Override
		public void dismiss() {
			enqueue();
		}
	}
	
	private static class CleaningReference extends FinalizerReference<java.lang.Object> {
		private static final List<CleaningReference> cleaningReferences = new ArrayList<>();
		
		CleaningReference(java.lang.Object referent, Runnable runnable) {
			super(referent, runnable);
			synchronized (cleaningReferences) {
				cleaningReferences.add(this);
			}
		}
		
		@Override
		public void clean() {
			super.clean();
			synchronized (cleaningReferences) {
				cleaningReferences.remove(this);
			}
		}
		
		@Override
		public boolean enqueue() {
			synchronized (cleaningReferences) {
				cleaningReferences.remove(this);
			}
			return super.enqueue();
		}
	}

	static Cleanable registerCleaner(java.lang.Object object, Runnable action) {
		return new CleaningReference(object, action);
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
						"Function call on incomplete object of type: " + AccessUtility.instance.getClass(object).getName());
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
					"Function call on incomplete object of type: " + AccessUtility.instance.getClass(object).getName());
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
	
	private native static Map<Class<? extends QtObjectInterface>, Function<QtObjectInterface,io.qt.MemberAccess<?>>> getInterfaceInfos(QtObjectInterface object);
}
