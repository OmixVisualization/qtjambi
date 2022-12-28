/****************************************************************************
**
** Copyright (C) 1992-2009 Nokia. All rights reserved.
**
** This file is part of Qt Jambi.
**
** $BEGIN_LICENSE$
** 
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
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
** 
** $END_LICENSE$
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

package io.qt.autotests;

import io.qt.QtUtilities;

/**
 * Special debugging methods used by autoests which are only available
 * if Qt Jambi is compiled with QTJAMBI_DEBUG_TOOLS defined.
 *
 */
public final class DebugTools {

	static {
		QtUtilities.initializePackage(io.qt.autotests.generated.Tulip.class);
	}
	
	private DebugTools() { throw new RuntimeException();}

    public static native void enableCounters();
    
    public static native void disableCounters();

    /**
     * Same as calling resetFinalizedCount(null)
     */
    public static void reset_cleanCallerCount() { reset_cleanCallerCount(null); }

    /**
     * Same as calling finalizedCount(null)
     */
    public static int cleanCallerCount() { return cleanCallerCount(null); }

    /**
     * Resets the counter for calls to QtJambiObject.finalize()
     *
     * @param className The class name for which to reset, or null for all.
     */
    public static native void reset_cleanCallerCount(String className);

    /**
     * Retrieve number of calls to QtJambiObject.finalize()
     *
     * @param className The class name for which to retrieve, or null for all.
     * @return The number of QtJambiObjects that have been finalized
     */
    public static native int cleanCallerCount(String className);




    /**
     * Reset number of calls to the destructor of QtJambiUserData
     *
     * @param className The class name for which to reset, or null for all.
     */
    public static native void reset_userDataDestroyedCount(String className);

    /**
     * Retrieve number of calls to the destructor of QtJambiUserData
     *
     * @param className The class name for which to retrieve, or null for all.
     * @return The number of QObjects that have been destroyed
     */
    public static native int userDataDestroyedCount(String className);

    /**
     * Same as calling resetUserDataDestroyedCount(null)
     */
    public static void reset_userDataDestroyedCount() { reset_userDataDestroyedCount(null); }

    /**
     * Same as calling userDataDestroyedCount(null)
     */
    public static int userDataDestroyedCount() { return userDataDestroyedCount(null); }




    /**
     * Same as calling resetDestructorFunctionCalledCount(null)
     */
    public static void reset_destructorFunctionCalledCount() { reset_destructorFunctionCalledCount(null); }

    /**
     * Same as calling destructorFunctionCalledCount(null)
     */
    public static int destructorFunctionCalledCount() { return destructorFunctionCalledCount(null); }

    /**
     * Resets the counter for calls to destructor functions (used by QtJambiLink to delete native data)
     *
     * @param className The name of the class for which to reset or null for all
     */
    public static native void reset_destructorFunctionCalledCount(String className);

    /**
     * Retrieves the counter for calls to destructor functions (used by QtJambiLink to delete native data)
     *
     * @param className The name of the class for which to retrieve or null for all
     * @return The number of times destructor functions have been called
     */
    public static native int destructorFunctionCalledCount(String name);




    /**
     * Resets the counter for calls to shell class destructors in polymorphic classes.
     *
     * @param className The name of the class for which to reset or null for all
     */
    public static native void reset_shellDestructorCalledCount(String className);

    /**
     * Retrieves the counter for calls to shell class destructors in polymorphic classes.
     *
     * @param className The name of the class for which to retrieve or null for all
     * @return The number of times a shell class destructor has been called
     */
    public static native int shellDestructorCalledCount(String className);

    /**
     * Same as calling resetShellDestructorCalledCount(null)
     */
    public static void reset_shellDestructorCalledCount() {
        reset_shellDestructorCalledCount(null);
    }

    /**
     * Same as calling shellDestructorCalledCount(null)
     */
    public static int shellDestructorCalledCount() {
        return shellDestructorCalledCount(null);
    }




    /**
     * Resets the counter for calls to disposed() (Qt Jambi objects that have been invalidated)
     *
     * @param className The name of the class for which to reset or null for all
     */
    public static native void reset_objectInvalidatedCount(String className);

    /**
     * Retrieves the counter for calls to disposed() (Qt Jambi objects that have been invalidated)
     *
     * @param className The name of the class for which to reset or null for all
     */
    public static native int objectInvalidatedCount(String className);

    /**
     * Same as calling resetDisposedCalledCount(null)
     */
    public static void reset_objectInvalidatedCount() { reset_objectInvalidatedCount(null); }

    /**
     * Same as calling disposedCalledCount(null)
     */
    public static int objectInvalidatedCount() { return objectInvalidatedCount(null); }




    /**
     * Resets the counter for calls to dispose() (Qt Jambi objects that have been explicitly deleted by
     * Java code)
     *
     * @param className The name of class for which to reset or null for all
     */
    public static native void reset_disposeCalledCount(String className);

    /**
     * Retrieves the counter for calls to dispose() (Qt Jambi objects that have been explicitly deleted by
     * Java code)
     *
     * @param className The name of class for which to reset or null for all
     */
    public static native int disposeCalledCount(String className);

    /**
     * Same as calling resetDisposeCalledCount(null)
     */
    public static void reset_disposeCalledCount() { reset_disposeCalledCount(null); }

    /**
     * Same as calling disposeCalledCount(null)
     */
    public static int disposeCalledCount() { return disposeCalledCount(null); }




    /**
     * Resets the counter for calls to the QtJambiLink destructor
     *
     * @param className The name of class for which to reset or null for all
     */
    public static native void reset_linkDestroyedCount(String className);

    /**
     * Retrieves the counter for calls to the QtJambiLink destructor
     *
     * @param className The name of class for which to reset or null for all
     */
    public static native int linkDestroyedCount(String className);

    /**
     * Same as calling resetLinkDestroyedCount(null)
     */
    public static void reset_linkDestroyedCount() { reset_linkDestroyedCount(null); }

    /**
     * Same as calling linkDestroyedCount(null)
     */
    public static int linkDestroyedCount() { return linkDestroyedCount(null); }



    /**
     * Resets the counter for calls to the QtJambiLink destructor
     *
     * @param className The name of class for which to reset or null for all
     */
    public static native void reset_shellDestroyedCount(String className);

    /**
     * Retrieves the counter for calls to the QtJambiLink destructor
     *
     * @param className The name of class for which to reset or null for all
     */
    public static native int shellDestroyedCount(String className);

    /**
     * Same as calling resetLinkDestroyedCount(null)
     */
    public static void reset_shellDestroyedCount() { reset_shellDestroyedCount(null); }

    /**
     * Same as calling linkDestroyedCount(null)
     */
    public static int shellDestroyedCount() { return shellDestroyedCount(null); }



    /**
     * Resets the counter for calls to the QtJambiLink destructor
     *
     * @param className The name of class for which to reset or null for all
     */
    public static native void reset_linkConstructedCount(String className);

    /**
     * Retrieves the counter for calls to the QtJambiLink destructor
     *
     * @param className The name of class for which to reset or null for all
     */
    public static native int linkConstructedCount(String className);

    /**
     * Same as calling resetLinkDestroyedCount(null)
     */
    public static void reset_linkConstructedCount() { reset_linkConstructedCount(null); }

    /**
     * Same as calling linkDestroyedCount(null)
     */
    public static int linkConstructedCount() { return linkConstructedCount(null); }



    /**
     * Resets the counter for calls to the PointerContainer destructor
     *
     * @param className The name of class for which to reset or null for all
     */
    public static native void reset_pointerContainerDestroyedCount(String className);

    /**
     * Retrieves the counter for calls to the PointerContainer destructor
     *
     * @param className The name of class for which to reset or null for all
     */
    public static native int pointerContainerDestroyedCount(String className);

    /**
     * Same as calling reset_pointerContainerDestroyedCount(null)
     */
    public static void reset_pointerContainerDestroyedCount() { reset_pointerContainerDestroyedCount(null); }

    /**
     * Same as calling pointerContainerDestroyedCount(null)
     */
    public static int pointerContainerDestroyedCount() { return pointerContainerDestroyedCount(null); }
}
