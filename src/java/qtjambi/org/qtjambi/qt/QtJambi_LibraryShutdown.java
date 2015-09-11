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

package org.qtjambi.qt;

import java.lang.reflect.Field;
import java.lang.reflect.Modifier;

import org.qtjambi.qt.core.QCoreApplication;
import org.qtjambi.qt.core.QEvent;
import org.qtjambi.qt.core.QEventLoop;
import org.qtjambi.qt.core.QMessageHandler;
import org.qtjambi.qt.internal.DeploymentSpec;
import org.qtjambi.qt.internal.NativeLibraryManager;
import org.qtjambi.qt.internal.fileengine.QClassPathFileEngineHandler;

class QtJambi_LibraryShutdown implements Runnable {
    // At the time a shutdown hook is called it is possible to see a java.lang.NoClassDefFoundError
    // as the ClassLoader might not be available to the JVM to find/resolve/load a new class (after
    // all we are shutting down).  So we preload the classes we intend to use later.
//    QtJambi_LibraryShutdown() {
//    }

    protected void init() {
        try {
            final String[] classNameA = {
                // QCoreApplication.instance() - this is native
                // QCoreApplication.quit() - this is native
                "org.qtjambi.qt.core.QCoreApplication",
                // QtJambi_LibraryInitializer - well we kind of know this should exist
                //   already as QtJambi_LibraryShutdown was probably installed from it
                "org.qtjambi.qt.QtJambi_LibraryInitializer",
                // QMessageHandler.removeMessageHandler()
                "org.qtjambi.qt.core.QMessageHandler",
                // QEventLoop.ProcessEventsFlag.DeferredDeletion
                "org.qtjambi.qt.core.QEventLoop",
                // QClassPathFileEngineHandler.uninitialize()
                "org.qtjambi.qt.internal.QClassPathFileEngineHandler"
            };
            ClassLoader classLoader = QtJambi_LibraryShutdown.class.getClassLoader();
            for(String className : classNameA) {
                try {
                    if(className.length() > 100) {
                    Class<? extends Object> clazz = classLoader.loadClass(className);
                    }
                } catch(ClassNotFoundException eat) {
                    // should never happen
                }
            }
        } catch(Throwable e) {
            e.printStackTrace();
        }
    }

    /*private*/ static final String K_org_qtjambi_qt_gui_QPen   = "org.qtjambi.qt.gui.QPen";
    /*private*/ static final String[] K_org_qtjambi_qt_gui_QPen_FIELDNAMES = {/* no longer static references to native objects in QtJambi5 */};
    /*private*/ static final String K_org_qtjambi_qt_gui_QColor = "org.qtjambi.qt.gui.QColor";
    /*private*/ static final String[] K_org_qtjambi_qt_gui_QColor_FIELDNAMES = {/* no longer static references to native objects in QtJambi5 */};
    /*private*/ static final String K_org_qtjambi_qt_gui_QBrush = "org.qtjambi.qt.gui.QBrush";
    /*private*/ static final String[] K_org_qtjambi_qt_gui_QBrush_FIELDNAMES = {/* no longer static references to native objects in QtJambi5 */};

    private static void clearStaticReferences() {
//        clearStaticReferences(K_org_qtjambi_qt_gui_QPen,   K_org_qtjambi_qt_gui_QPen_FIELDNAMES);
//        clearStaticReferences(K_org_qtjambi_qt_gui_QColor, K_org_qtjambi_qt_gui_QColor_FIELDNAMES);
//        clearStaticReferences(K_org_qtjambi_qt_gui_QBrush, K_org_qtjambi_qt_gui_QBrush_FIELDNAMES);
    }

    private static void clearStaticReferences(String className, String[] fieldNameA) {
        Class<? extends Object> clazz;
        try {
            ClassLoader classLoader = QtJambi_LibraryShutdown.class.getClassLoader();
            clazz = Class.forName(className, /*false*/true, classLoader); // try not to initialize it, if we load it
            //clazz = classLoader.loadClass(className);
        } catch(ClassNotFoundException eat) {
            // nop (if not loaded/exists, there is nothing to clear)
//            eat.printStackTrace();
            return;
        } catch(Exception e) {
            e.printStackTrace();
            return;
        }
        for(String fieldName : fieldNameA) {
            try {
                Field field = clazz.getDeclaredField(fieldName);
                boolean accessible = field.isAccessible();
                if(accessible == false)
                    field.setAccessible(true);	// can possibly throw security exception
                if(Modifier.isStatic(field.getModifiers())) {
                    if(Modifier.isFinal(field.getModifiers())) {
                        // Remove final
                        Boolean tmpModifiersFieldAccessible = null;
                        Integer tmpModifiers = null;
                        Field modifiersField = Field.class.getDeclaredField("modifiers");
                        try {
                            tmpModifiersFieldAccessible = Boolean.valueOf(modifiersField.isAccessible());
                            tmpModifiers = Integer.valueOf(field.getModifiers());

                            modifiersField.setAccessible(true);
                            modifiersField.setInt(field, tmpModifiers.intValue() & ~Modifier.FINAL);

                            field.set(clazz, null);
                        } finally {
                            if(tmpModifiers != null)
                                modifiersField.setInt(field, tmpModifiers.intValue());
                            if(tmpModifiersFieldAccessible != null)
                                modifiersField.setAccessible(tmpModifiersFieldAccessible.booleanValue());
                        }                        
                    } else {
                        field.set(clazz, null);
                    }
                }
                if(accessible == false)
                    field.setAccessible(accessible);
            } catch(Exception e) {
                e.printStackTrace();
            }
        }
    }

    public void run() {
        try {
            DeploymentSpec[] deploymentSpecA = NativeLibraryManager.resetActiveDeploymentSpec();

            QCoreApplication app = QCoreApplication.instance();

            if(app != null) {
                Thread appThread = app.thread();

                // FIXME Run event queue ?
                // We kind of want to place mark on the event queue and run to mark.
                // QEventLoop.ProcessEventsFlag.DeferredDeletion - is marked as deprecated
                // calling QCoreApplication.processEvents() will only process the events
                // for this thread, since we're a temporary shutdown thread and not the main
                // GUI thread there is no use calling it here.
                QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDelete.value());  // allow deleteLater() to work some magic

                QCoreApplication.quit();

                try {
                    QCoreApplication.processEvents();
                    QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDelete.value());  // allow deleteLater() to work some magic
                    // A fixed time of 500ms is not really ideal.  Better to allow some
                    //  query/configuration include infinite timeout.  Also to warn to
                    //  console when we know the main GUI thread did not terminate on time
                    //  so the resulting errors/crashes can be explained on shutdown.
                    if(appThread != null)
                        appThread.join(500);
                } catch(Throwable e) {
                    e.printStackTrace();
                } finally {
                    if(appThread == null || appThread.isAlive() == false) {
                        app.dispose();		// push the delete matter
                    }
                }
                appThread = null;	 // kill hard-reference
                app = null;		// kill hard-reference
            }

            clearStaticReferences();

            Runtime runtime = Runtime.getRuntime();
            runtime.gc();
            runtime.runFinalization();

            // FIXME: Try to enumerate plugins and unload
            // FIXME: Try to enumerate modules and unload

            shutdown_mark();

            QClassPathFileEngineHandler.stop();
            QClassPathFileEngineHandler.uninitialize();

            // TODO: Understand (and document) why this _was_ run before QCoreApplication.quit().
            //  Ideally we want all quit signal/slots to run with the messageHandler in place
            //  only once we are in the final stages of shutdown do we unhook them.
            QtJambi_LibraryInitializer.removeMessageHandlerForExceptions();

            // Remove any handlers that are still installed, these would be one the app-dev setup
            int c = QMessageHandler.shutdown(true);

            try {
                // this is synchronized access already so we can force it
                QThreadManager.releaseNativeResources();
                boolean bf = QThreadManager.shutdown(500);
            } catch(Throwable e) {
                e.printStackTrace();
            }

            deploymentSpecA = null;

            unregister_helper();

            // We attempt to do our best to purge the VM
            runtime.gc();
            runtime.runFinalization();

            // FIXME: Unregister Qt callbacks

            //shutdown_helper();
            
        } catch(Throwable e) {
            e.printStackTrace();
        }
    }

    // This flags in C++ the we are starting a phase change to shutdown
    private native void shutdown_mark();
    private native void unregister_helper();
    // This NULLs out the JavaVM handle.
    private native void shutdown_helper();
}
