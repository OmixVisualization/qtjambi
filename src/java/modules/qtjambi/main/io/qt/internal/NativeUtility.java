/****************************************************************************
**
** Copyright (C) 2009-2025 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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
	private static final Map<Long, java.lang.Object> globalReferences;
	private static final Map<Long, WeakReference<java.lang.Object>> weakGlobalReferences;
	private static final Map<Long, QMetaObject.DisposedSignal> disposedSignals;
	private static final Thread cleanupThread;
	static {
		interfaceLinks = Collections.synchronizedMap(new HashMap<>());
		disposedSignals = Collections.synchronizedMap(new HashMap<>());
		globalReferences = Collections.synchronizedMap(new HashMap<>());
		weakGlobalReferences = Collections.synchronizedMap(new HashMap<>());
		cleanupThread = new Thread(QueuedCleaner::cleanup);
		cleanupThread.setName("QtJambiCleanupThread");
		cleanupThread.setDaemon(true);
		QtJambi_LibraryUtilities.initialize();
		cleanupThread.start();
	}
	
	protected NativeUtility() {
		throw new RuntimeException();
	}
	
	@NativeAccess
	private static void pushGlobalReference(long id, java.lang.Object object) {
		globalReferences.put(id, object);
	}
	
	@NativeAccess
	private static java.lang.Object getGlobalReference(long id) {
		return globalReferences.get(id);
	}
	
	@NativeAccess
	private static void releaseGlobalReference(long id) {
		globalReferences.remove(id);
	}
	
	@NativeAccess
	private static void pushWeakGlobalReference(long id, java.lang.Object object) {
		weakGlobalReferences.put(id, new WeakReference<>(object));
	}
	
	@NativeAccess
	private static java.lang.Object getWeakGlobalReference(long id) {
		WeakReference<java.lang.Object> weak = weakGlobalReferences.get(id);
		if(weak!=null)
			return weak.get();
		return null;
	}
	
	@NativeAccess
	private static void releaseWeakGlobalReference(long id) {
		weakGlobalReferences.remove(id);
	}

	@NativeAccess
	@QtUninvokable
	private static void terminateCleanupThread() throws Throwable {
		switch (cleanupThread.getState()) {
		case TERMINATED:
			break;
		default:
			cleanupThread.interrupt();
			cleanupThread.join();
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
	
	static final class Factories{
		static final Function<QtObjectInterface, NativeLink> new_NativeLink;
		static final Function<QtObjectInterface, PureInterfaceNativeLink> new_PureInterfaceNativeLink;
		static final Function<QtObjectInterface, ReferenceCountingNativeLink> new_ReferenceCountingNativeLink;
		static final BiFunction<QtObjectInterface, Map<Class<? extends QtObjectInterface>, io.qt.MemberAccess<?>>, MemberAccessReferenceCountingNativeLink> new_MemberAccessReferenceCountingNativeLink;
		static final BiFunction<QtObjectInterface, Map<Class<? extends QtObjectInterface>, io.qt.MemberAccess<?>>, MemberAccessPureInterfaceNativeLink> new_MemberAccessPureInterfaceNativeLink;
		static {
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
		}		
	}

	static class NativeLink extends QueuedCleaner<QtObjectInterface> {

		private NativeLink(QtObjectInterface object) {
			super(object);
		}

		private long native__id = 0;
		private QtObjectInterface referent__strong;

		@NativeAccess
		private final void detach(long native__id, boolean hasDisposedSignal) {
			synchronized (this) {
				if (this.native__id == native__id) {
					this.native__id = 0;
					this.referent__strong = null;
				}else {
					return;
				}
			}
			if (hasDisposedSignal) {
				QMetaObject.DisposedSignal disposed = takeSignalOnDispose(native__id);
				if (disposed != null) {
					try {
						disposed.emitSignal();
					} catch (Throwable e) {
						CLEANUP_LOGGER.log(Level.WARNING, "Exception occurred during DisposedSignal emit.", e);
					}
					disposed.disconnect();
				}
			}
			enqueue();
		}
		@NativeAccess
		private static void detachForID(long native__id, boolean hasDisposedSignal) {
			java.lang.Object object = getGlobalReference(native__id);
			if(object instanceof NativeLink)
				((NativeLink)object).detach(native__id, hasDisposedSignal);
		}

		@Override
		public synchronized void clean() {
			if (native__id != 0) {
				clean(native__id);
			}
		}
		
		@Override
		public void dismiss() {
		}

		@NativeAccess
		private final void reset(long native__id, boolean hasDisposedSignal) {
			synchronized (this) {
				if (this.native__id == native__id) {
					this.native__id = 0;
					this.referent__strong = null;
				}else {
					return;
				}
			}
			if (hasDisposedSignal) {
				QMetaObject.DisposedSignal disposed = takeSignalOnDispose(native__id);
				if (disposed != null)
					disposed.disconnect();
			}
		}
		@NativeAccess
		private static void resetForID(long native__id, boolean hasDisposedSignal) {
			java.lang.Object object = getGlobalReference(native__id);
			if(object instanceof NativeLink)
				((NativeLink)object).reset(native__id, hasDisposedSignal);
		}

		private static native void clean(long native__id);
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
			QtObjectInterface o = super.get();
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

		@NativeAccess
		public final synchronized long nativeId() {
			return native__id;
		}
		
		private final synchronized void assignNativeId(long ptr) {
			assignNativeId(native__id, ptr);
		}
		
		private static native void assignNativeId(long native__id, long ptr);

		@Override
		@NativeAccess
		public final synchronized QtObjectInterface get() {
			return referent__strong==null ? super.get() : referent__strong;
		}
		@NativeAccess
		private static QtObjectInterface getForID(long native__id) {
			java.lang.Object object = getGlobalReference(native__id);
			if(object instanceof NativeLink) {
				return ((NativeLink)object).get();
			}
			return null;
		}
		
		final synchronized QtObjectInterface weak() {
			return super.get();
		}

		@NativeAccess
		private final synchronized void releaseOwnership() {
			this.referent__strong = null;
		}
		@NativeAccess
		private static boolean releaseOwnershipForID(long native__id) {
			java.lang.Object object = getGlobalReference(native__id);
			if(object instanceof NativeLink) {
				((NativeLink)object).releaseOwnership();
				return true;
			}
			return false;
		}
		
		@NativeAccess
		private final synchronized void takeOwnership() {
			this.referent__strong = super.get();
		}
		@NativeAccess
		private static boolean takeOwnershipForID(long native__id) {
			java.lang.Object object = getGlobalReference(native__id);
			if(object instanceof NativeLink) {
				((NativeLink)object).takeOwnership();
				return true;
			}
			return false;
		}
		
		@NativeAccess
		private final void initialize(long nativeId){
			if(native__id==0)
				this.native__id = nativeId;
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
		
		@Override
		synchronized void dispose() {
			if (nativeId() != 0) {
				NativeUtility.CLEANUP_LOGGER.log(Level.FINE, ()->String.format("Begin dispose of 0x%1$s@%2$s...", Integer.toHexString(hashCode), cls.getName()));
				super.dispose();
				NativeUtility.CLEANUP_LOGGER.log(Level.FINE, ()->String.format("Dispose of 0x%1$s@%2$s finished.", Integer.toHexString(hashCode), cls.getName()));
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
		
		@Override
		synchronized void dispose() {
			if (nativeId() != 0) {
				NativeUtility.CLEANUP_LOGGER.log(Level.FINE, ()->String.format("Begin dispose of 0x%1$s@%2$s...", Integer.toHexString(hashCode), cls.getName()));
				super.dispose();
				NativeUtility.CLEANUP_LOGGER.log(Level.FINE, ()->String.format("Dispose of 0x%1$s@%2$s finished.", Integer.toHexString(hashCode), cls.getName()));
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
		
		@Override
		synchronized void dispose() {
			if (nativeId() != 0) {
				NativeUtility.CLEANUP_LOGGER.log(Level.FINE, ()->String.format("Begin dispose of 0x%1$s@%2$s...", Integer.toHexString(hashCode), cls.getName()));
				super.dispose();
				NativeUtility.CLEANUP_LOGGER.log(Level.FINE, ()->String.format("Dispose of 0x%1$s@%2$s finished.", Integer.toHexString(hashCode), cls.getName()));
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
		
		@Override
		synchronized void dispose() {
			if (nativeId() != 0) {
				NativeUtility.CLEANUP_LOGGER.log(Level.FINE, ()->String.format("Begin dispose of 0x%1$s@%2$s...", Integer.toHexString(ownerHashCode), cls.getName()));
				super.dispose();
				NativeUtility.CLEANUP_LOGGER.log(Level.FINE, ()->String.format("Dispose of 0x%1$s@%2$s finished.", Integer.toHexString(ownerHashCode), cls.getName()));
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
		
		@Override
		synchronized void dispose() {
			if (nativeId() != 0) {
				NativeUtility.CLEANUP_LOGGER.log(Level.FINE, ()->String.format("Begin dispose of 0x%1$s@%2$s...", Integer.toHexString(ownerHashCode), cls.getName()));
				super.dispose();
				NativeUtility.CLEANUP_LOGGER.log(Level.FINE, ()->String.format("Dispose of 0x%1$s@%2$s finished.", Integer.toHexString(ownerHashCode), cls.getName()));
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
					link = Factories.new_PureInterfaceNativeLink.apply(iface);
				}else if (initialize) {
					initializeNativeObject(iface, link);
				}
			}
			return link;
		}else return null;
	}
	
	@NativeAccess
	private static void findAndAssignInterfaceLink(QtObjectInterface iface, boolean forceCreation, boolean initialize, long ptr) {
		NativeLink nl = findInterfaceLink(iface, forceCreation, initialize);
		if(nl!=null)
			nl.assignNativeId(ptr);
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
			long native__id = nativeLink.nativeId();
			if(native__id!=0) {
				if (forceCreation) {
					NativeLink.setHasDisposedSignal(native__id);
					try {
						return disposedSignals.computeIfAbsent(native__id, id -> {
							Class<?> declaringClass;
							if(nativeLink instanceof LoggingNativeLink) {
								declaringClass = ((LoggingNativeLink) nativeLink).cls;
							}else {
								QtObjectInterface object = nativeLink.weak();
								declaringClass = AccessUtility.instance.getClass(object);
								object = null;
							}
							if(disposedSignalFactory==null)
								disposedSignalFactory = SignalUtility.<QMetaObject.DisposedSignal>getSignalFactory(QMetaObject.DisposedSignal.class);
							return disposedSignalFactory.apply(declaringClass);
						});
					} catch (NullPointerException e) {
						e.printStackTrace();
					}
				} else
					return disposedSignals.get(native__id);
			}
		}
		return null;
	}

	private static QMetaObject.DisposedSignal takeSignalOnDispose(long native__id) {
		return disposedSignals.remove(native__id);
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
				return Factories.new_ReferenceCountingNativeLink.apply(object);
			else
				return Factories.new_MemberAccessReferenceCountingNativeLink.apply(object, Collections.unmodifiableMap(memberAccesses));
		} else {
			return Factories.new_NativeLink.apply(object);
		}
	}

	private static NativeLink createNativeLink(QtObjectInterface iface) {
		Map<Class<? extends QtObjectInterface>, io.qt.MemberAccess<?>> memberAccesses = findMemberAccesses(iface);
		if (memberAccesses != null) {
			if(memberAccesses.isEmpty())
				return Factories.new_PureInterfaceNativeLink.apply(iface);
			else
				return Factories.new_MemberAccessPureInterfaceNativeLink.apply(iface, Collections.unmodifiableMap(memberAccesses));
		} else {
			return null;
		}
	}
    
	protected static <K,V> Function<K, ArrayList<V>> arrayListFactory(){
		return key->new ArrayList<>();
	}

	protected static void initializeNativeObject(Class<?> declaringClass, QtObjectInterface object, Map<Class<?>, List<Map.Entry<Class<?>,java.lang.Object>>> arguments) throws IllegalArgumentException {
		initializeNativeObject(declaringClass, object, NativeUtility.findInterfaceLink(object, true, false), arguments);
	}

	private native static void initializeNativeObject(Class<?> callingClass, QtObjectInterface object, NativeLink link, Map<Class<?>, List<Map.Entry<Class<?>,java.lang.Object>>> arguments) throws IllegalArgumentException;

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
	    
	    @NativeAccess
		private void assignNativeLink(long ptr) {
	    	nativeLink.assignNativeId(ptr);
	    }
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
					if (disposed != null) {
						if(LibraryUtility.operatingSystem==LibraryUtility.OperatingSystem.Android) {
							WeakReference<AssociativeReference> weakThis = new WeakReference<>(this);
							disposed.connect(()->{
								AssociativeReference _this = weakThis.get();
								if(_this!=null)
									_this.enqueue();
							});
						}else {
							disposed.connect(this::enqueue);
						}
					}
				}
				if(association instanceof QtObjectInterface) {
					QMetaObject.DisposedSignal disposed = getSignalOnDispose((QtObjectInterface)association, true);
					if (disposed != null) {
						if(LibraryUtility.operatingSystem==LibraryUtility.OperatingSystem.Android) {
							WeakReference<AssociativeReference> weakThis = new WeakReference<>(this);
							disposed.connect(()->{
								AssociativeReference _this = weakThis.get();
								if(_this!=null)
									_this.enqueue();
							});
						}else {
							disposed.connect(this::enqueue);
						}
					}
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
		
		static List<AssociativeReference> findAll(java.lang.Object reference) {
			List<AssociativeReference> result = new ArrayList<>();
			synchronized(associativeReferences) {
				for (AssociativeReference ref : associativeReferences) {
					if(ref.get() == reference) {
						result.add(ref);
					}
				}
			}
			return result;
		}
		
		static AssociativeReference findAllByHashCode(int hashCode) {
			synchronized(associativeReferences) {
				for (AssociativeReference ref : associativeReferences) {
					if(System.identityHashCode(ref) == hashCode) {
						return ref;
					}
				}
			}
			return null;
		}
	}

	@NativeAccess
	private static int createAssociation(java.lang.Object o1, java.lang.Object o2) {
		return System.identityHashCode(new AssociativeReference(o1, o2));
	}

	@NativeAccess
	private static boolean deleteAssociation(java.lang.Object o1) {
		boolean result = false;
		for(AssociativeReference matchingReference : AssociativeReference.findAll(o1)) {
			if (matchingReference != null) {
				matchingReference.enqueue();
				result = true;
			}
		}
		return result;
	}
	
	@NativeAccess
	private static java.lang.Object deleteAssociationByHashCode(int hashCode) {
		java.lang.Object result = null;
		AssociativeReference matchingReference = AssociativeReference.findAllByHashCode(hashCode);
		if (matchingReference != null) {
			result = matchingReference.association;
			matchingReference.enqueue();
		}
		return result;
	}
	
	@NativeAccess
	static java.lang.Object findAssociation(java.lang.Object o1) {
		List<AssociativeReference> matchingReferences = AssociativeReference.findAll(o1);
		return matchingReferences.isEmpty() ? null : matchingReferences.get(0).association();
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
	
	private native static java.nio.ByteBuffer mutableByteArrayData(long nid);
	
	private native static java.nio.CharBuffer mutableStringData(long nid);
	
	private native static java.nio.ByteBuffer mutableDataB(io.qt.QtObject iter, long nid);
	
	private native static java.nio.ShortBuffer mutableDataS(io.qt.QtObject iter, long nid);
	
	private native static java.nio.IntBuffer mutableDataI(io.qt.QtObject iter, long nid);
	
	private native static java.nio.LongBuffer mutableDataJ(io.qt.QtObject iter, long nid);
	
	private native static java.nio.CharBuffer mutableDataC(io.qt.QtObject iter, long nid);
	
	private native static java.nio.FloatBuffer mutableDataF(io.qt.QtObject iter, long nid);
	
	private native static java.nio.DoubleBuffer mutableDataD(io.qt.QtObject iter, long nid);
	
	private native static void truncateBuffer(long nid, java.nio.Buffer buffer);
	
	static void truncateBuffer(io.qt.QtObjectInterface owner, java.nio.Buffer buffer){
		truncateBuffer(checkedNativeId(owner), buffer);
	}
	
	static void truncateBuffer(io.qt.QtObject owner, java.nio.Buffer buffer){
		truncateBuffer(checkedNativeId(owner), buffer);
	}
	
	static java.nio.ByteBuffer mutableData(io.qt.core.QByteArray byteArray){
		return mutableByteArrayData(checkedNativeId(byteArray));
	}
	
	static java.nio.CharBuffer mutableData(io.qt.core.QString string){
		return mutableStringData(checkedNativeId(string));
	}
	
	static <C extends QtObject & Iterable<Character>> java.nio.CharBuffer mutableDataC(C list){
		return mutableDataC(list, checkedNativeId(list));
	}
    
	static <C extends QtObject & Iterable<Byte>> java.nio.ByteBuffer mutableDataB(C list){
		return mutableDataB(list, checkedNativeId(list));
	}
    
	static <C extends QtObject & Iterable<Short>> java.nio.ShortBuffer mutableDataS(C list){
		return mutableDataS(list, checkedNativeId(list));
	}
    
	static <C extends QtObject & Iterable<Integer>> java.nio.IntBuffer mutableDataI(C list){
		return mutableDataI(list, checkedNativeId(list));
	}
    
	static <C extends QtObject & Iterable<Long>> java.nio.LongBuffer mutableDataJ(C list){
		return mutableDataJ(list, checkedNativeId(list));
	}
    
	static <C extends QtObject & Iterable<Float>> java.nio.FloatBuffer mutableDataF(C list){
		return mutableDataF(list, checkedNativeId(list));
	}
    
	static <C extends QtObject & Iterable<Double>> java.nio.DoubleBuffer mutableDataD(C list){
		return mutableDataD(list, checkedNativeId(list));
	}

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
	
	static void registerConDestroyedObject(QtObjectInterface dependentObject, QtObjectInterface owner) {
		registerConDestroyedObject(nativeId(dependentObject), nativeId(owner));
	}
	
	static void unregisterConDestroyedObject(QtObjectInterface dependentObject, QtObjectInterface owner) {
		unregisterConDestroyedObject(nativeId(dependentObject), nativeId(owner));
	}
	
	static void registerConDestroyedObject(QtObjectInterface dependentObject, QtObject owner) {
		registerConDestroyedObject(nativeId(dependentObject), nativeId(owner));
	}
	
	static void unregisterConDestroyedObject(QtObjectInterface dependentObject, QtObject owner) {
		unregisterConDestroyedObject(nativeId(dependentObject), nativeId(owner));
	}
	
	static void registerConDestroyedObject(QtObject dependentObject, QtObjectInterface owner) {
		registerConDestroyedObject(nativeId(dependentObject), nativeId(owner));
	}
	
	static void unregisterConDestroyedObject(QtObject dependentObject, QtObjectInterface owner) {
		unregisterConDestroyedObject(nativeId(dependentObject), nativeId(owner));
	}
	
	static void registerConDestroyedObject(QtObject dependentObject, QtObject owner) {
		registerConDestroyedObject(nativeId(dependentObject), nativeId(owner));
	}
	
	static void unregisterConDestroyedObject(QtObject dependentObject, QtObject owner) {
		unregisterConDestroyedObject(nativeId(dependentObject), nativeId(owner));
	}

	private static native void setCppOwnership(long native__id);

	private static native void setDefaultOwnership(long native__id);

	private static native void setJavaOwnership(long native__id);
	
	private static native boolean isCppOwnership(long native__id);

	private static native boolean isSplitOwnership(long native__id);

	static native boolean isJavaOwnership(long native__id);

	private static native void invalidateObject(long native__id);
	
	private static native QObject owner(long native__id);

	private static native boolean hasOwnerFunction(long native__id);

	private native static void registerDependentObject(long dependentObject, long owner);

	private native static void unregisterDependentObject(long dependentObject, long owner);
	
	private native static void registerConDestroyedObject(long dependentObject, long owner);

	private native static void unregisterConDestroyedObject(long dependentObject, long owner);
	
	private native static Map<Class<? extends QtObjectInterface>, Function<QtObjectInterface,io.qt.MemberAccess<?>>> getInterfaceInfos(QtObjectInterface object);
}
