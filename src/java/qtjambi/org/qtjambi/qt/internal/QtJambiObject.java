/****************************************************************************
**
** Copyright (C) 1992-2009 Nokia. All rights reserved.
**
** This file is part of Qt Jambi.
**
** ** $BEGIN_LICENSE$
** Commercial Usage
** Licensees holding valid Qt Commercial licenses may use this file in
** accordance with the Qt Commercial License Agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Nokia.
** 
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
** 
** In addition, as a special exception, Nokia gives you certain
** additional rights. These rights are described in the Nokia Qt LGPL
** Exception version 1.0, included in the file LGPL_EXCEPTION.txt in this
** package.
** 
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
** 
** If you are unsure which license is appropriate for your use, please
** contact the sales department at qt-sales@nokia.com.
** $END_LICENSE$

**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

package org.qtjambi.qt.internal;

import org.qtjambi.qt.QNativePointer;
import org.qtjambi.qt.QNoNativeResourcesException;
import org.qtjambi.qt.QSignalEmitter;
import org.qtjambi.qt.QtJambiInterface;
import org.qtjambi.qt.QtJambi_LibraryInitializer;
import org.qtjambi.qt.QtProperty;
import org.qtjambi.qt.core.QCoreApplication;

import java.lang.reflect.Constructor;
import java.util.List;

/**
 * The super class of all class types in Qt Jambi. Loading this class or any
 * of its subclasses will imply a dependency on both the Qt Jambi
 * library and the Qt libraries.
 */
public abstract class QtJambiObject extends QSignalEmitter implements QtJambiInterface
{
    static {
        QtJambi_LibraryInitializer.init();
    }

    /**
     * @exclude
     */
    protected static class QPrivateConstructor { }

    /**
     * Creates a new QtJambiObject.
     */
    public QtJambiObject()
    {
        /* intentionally empty */
    }

    /**
     * @exclude
     */
    public QtJambiObject(QPrivateConstructor p)
    {
        /* intentionally empty */
    }

    /**
     * Called either as the native resources that belong to the object are being
     * cleaned up or directly before the object is finalized. Reimplement this
     * function to do clean up when the object is destroyed. The function
     * will never be called more than once per object, and the object is
     * guaranteed to be unusable after this function has returned. The default
     * implementation does nothing.
     *
     * Calling functions on the object itself from within this method could yield unpredictable
     * results, as there is no guarantee that the object is valid when disposed() is called. 
     */
    protected void disposed()
    {
        /* intentionally empty */
    }

    /**
     * Returns a translated version of source, or source itself if there is no appropriate translated version.
     * Note that if you need to translate in a static context, you can use QCoreApplication::translate(),
     * which is a static method.
     *
     * @param source the source text to translate.
     * @return translated version of the source text.
     */
    public String tr(String source) {
        String scope = getClass().getName();
        return QCoreApplication.translate(scope, source);
    }

    /**
     * Returns a translated version of source, or source itself if there is no appropriate translated version.
     * The comment is used to help translators translate the source text.
     * Note that if you need to translate in a static context, you can use QCoreApplication::translate(),
     * which is a static method.
     *
     * @param source the source text to translate.
     * @param comment helps the translator translate the source text.
     * @return translated version of the source text.
     */
    public String tr(String source, String comment) {
        String scope = getClass().getName();
        return QCoreApplication.translate(scope, source, comment);
    }

    /**
     * Returns a translated version of source, or source itself if there is no appropriate translated version.
     * If %n occurs in the source text, it will be substituted with count.
     * The comment is used to help translators translate the source text.
     * Note that if you need to translate in a static context, you can use QCoreApplication::translate(),
     * which is a static method.
     *
     * @param source the source text to translate.
     * @param comment helps the translator translate the source text.
     * @param count in source %n will be substituted by count.
     * @return translated version of the source text.
     */
    public String tr(String source, String comment, int count) {
        String scope = getClass().getName();
        return QCoreApplication.translate(scope, source, comment, count);
    }

    /**
     * Called before the java object is removed by the garbage collector. As the
     * native resources belonging to an object may be cleaned up prior to the
     * call of this function, it has been set as final. Reimplement disposed() instead,
     * which will be called either as the native resources are being removed
     * or just before the object is finalized, whichever happens first.
     *
     * If you decide to override this method in your subclass then please do it
     * with the following Java idiom:
     * <code>
     *     protected void finalize() {
     *         try {
     *             // put your code here
     *             doMyStuff();
     *         } finally {
     *             super.finalize();
     *         }
     *     }
     * </code>
     *
     * NOTES: In QtJambi this method used to be marked 'final' before around 4.7.x.
     */
    @Override
    protected native void finalize();

    /**
     * Explicitly removes the native resources held by the
     * object. Note that though this method does not guarantee that
     * the object will be garbage collected, it is not safe to
     * reference the object after it has been disposed.
     */
    public final native void dispose();


    /**
     * Used to check if this object shares native resources with the other object
     * if the other object is a subtype of QtJambiInterface. Otherwise it will
     * return the result of calling the super class implementation.
     *
     * @param other The object with which to compare
     * @return true if the native resources of the two objects are one and the same.
     */
    @Override
    public boolean equals(Object other) {
        if (other instanceof QtJambiInterface)
            return ((QtJambiInterface)other).nativeId() == nativeId();
        else
            return super.equals(other);
    }



    /**
     * This is an internal function. Calling it can have unexpected results.
     *
     * Disables garbage collection for this object. This should be
     * used when objects created in java are passed to C++ functions
     * that take ownership of the objects. Both the Java and C++ part
     * of the object will then be cleaned up by C++.
     * @exclude
     */
    public final native void disableGarbageCollection();

    /**
     * This is an internal function. Calling it can have unexpected results.
     *
     * Reenables garbage collection for this object. Should be used
     * on objects for which disableGarbageCollection() has previously
     * been called. After calling this function, the object ownership will be
     * reset to default.
     * @exclude
     */
    public final native void reenableGarbageCollection();

    /**
     * This is an internal function. Calling it can have unexpected results.
     *
     * Forces Java ownership of both the Java object and its C++ resources.
     * The C++ resources will be cleaned up when the Java object is finalized.
     * @exclude
     */
    public final native void setJavaOwnership();


    /**
     * Internal function which fetches a wrapper around the pointer to
     * the native resources held by this object.
     * @return A QNativePointer object for the current object.
     * @exclude
     */
    public final native QNativePointer nativePointer();

    /**
     * Internal function which fetches the native id of the current
     * object.
     * @return A long value which uniquely define the native resources
     * held by this object during their life time.
     * @exclude
     */
    public final long nativeId() { return native__id; }

    /**
     * In certain, uncommon cases, the native resources of a QtJambiObject object may
     * be out of sync with its class. In such cases this method can be
     * used to reassign the native resources to an object of another
     * class. Take special care when using this method, as it has
     * limited type safety and may cause crashes when used in the wrong way. Note
     * also that as the returned object "steals" the native resources
     * held by the original object, the original object will not be
     * usable after a call to this function. Invoking a method on the
     * original object may result in an exception being raised. If an
     * exception is raised, it is safe to assume that the original
     * object is still valid. If the object is already of the type
     * specified by clazz, then a reference to the object itself is
     * returned.
     *
     * @param object The original object which holds the native
     * resources. This object will be considered unusable after the
     * call.
     * @param clazz The class of the new object. The class must be a
     * subclass of QtJambiObject and you must not rely on any constructors
     * being invoked upon construction of the object.
     * @return An object of the specified type which owns the
     * resources previously held by object.
     * @throws ClassCastException If the class of object is unrelated
     * to clazz, or if clazz is an unsupported class type.
     * @throws InstantiationException If clazz cannot be instantiated
     */
    public static QtJambiObject reassignNativeResources(QtJambiObject object, Class<? extends QtJambiObject> clazz)
        throws InstantiationException
    {
        if (!object.getClass().isAssignableFrom(clazz)) {
            throw new ClassCastException("The object '" + object.toString() + "' (class: '" + object.getClass().getName() + "') "
                                         + "must be of class '" + clazz.getName() + "' or one of its superclasses.");
        }

        if (object.getClass().equals(clazz))
            return object;

        if (object.native__id == 0)
            throw new QNoNativeResourcesException("The object '" + object.toString() + "' does not have native resources.");

        Constructor<? extends QtJambiObject> c;
        try {
            c = clazz.getDeclaredConstructor(QPrivateConstructor.class);
        } catch (NoSuchMethodException e) {
            ClassCastException new_e = new ClassCastException("The class '" + clazz.getName() + "' must be of a generated type.");
            new_e.initCause(e);
            throw new_e;
        }

        long nativeId = object.native__id;
        object.native__id = 0;
        return __qt_reassignLink(nativeId, clazz, c);
    }

    /**
     * Accesses package visibility method with raw nativeId memory pointer.
     * @return
     */
    @org.qtjambi.qt.QtBlockedSlot
    protected QtProperty qtJambiUserProperty() {
        return QtJambiInternal.userProperty(nativeId());
    }

    /**
     * Accesses package visibility method with raw nativeId memory pointer.
     * @return
     */
    @org.qtjambi.qt.QtBlockedSlot
    protected List<QtProperty> qtJambiProperties() {
        return QtJambiInternal.properties(nativeId());
    }

    /**
     * Accesses package visibility method with raw nativeId memory pointer.
     * @param name
     * @return
     */
    @org.qtjambi.qt.QtBlockedSlot
    protected int qtJambiIndexOfProperty(String name) {
        return QtJambiInternal.indexOfProperty(nativeId(), name);
    }

    // The constructor must take a single object reference as its
    // parameter and accept null. Basically, it should be the
    // QPrivateConstructor-constructor.
    private static native QtJambiObject __qt_reassignLink(long newNativeId, Class<?> cls, Constructor<?> constructor);

    private long native__id = 0;
}
