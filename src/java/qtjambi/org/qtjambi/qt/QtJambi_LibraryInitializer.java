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

import java.lang.ref.WeakReference;

import java.lang.reflect.Field;
import java.lang.reflect.Modifier;

import org.qtjambi.qt.core.QMessageHandler;
import org.qtjambi.qt.internal.fileengine.QClassPathFileEngineHandler;

public abstract class QtJambi_LibraryInitializer
{
    private static QMessageHandler messageHandler;
    // It is upto the JVM to keep a hard reference to
    static WeakReference<Thread> shutdownHookThread;

    static {
        try {
            Utilities.loadSystemLibraries();
            if(org.qtjambi.qt.Utilities.isAvailableUtilityLibrary("icudata", "51")){
        		org.qtjambi.qt.Utilities.loadUtilityLibrary("icudata", "51");
        	}
        	if(org.qtjambi.qt.Utilities.isAvailableUtilityLibrary("icuuc", "51")){
        		org.qtjambi.qt.Utilities.loadUtilityLibrary("icuuc", "51");
        	}
        	if(org.qtjambi.qt.Utilities.isAvailableUtilityLibrary("icui18n", "51")){
        		org.qtjambi.qt.Utilities.loadUtilityLibrary("icui18n", "51");
        	}
            Utilities.loadQtLibrary("QtCore");
            Utilities.loadJambiJniLibrary("qtjambi");

            String s = System.getProperty("org.qtjambi.qt.exceptions-for-messages");
            installMessageHandlerForExceptions(s);

            QClassPathFileEngineHandler.initialize();
            QClassPathFileEngineHandler.start();

            initialize();
            QThreadManager.initialize();

            Thread thread = new Thread(new QtJambi_LibraryShutdown());	// hard reference
            thread.setName("qtjambi-" + QtJambi_LibraryShutdown.class.getName());
            Runtime.getRuntime().addShutdownHook(thread);
            shutdownHookThread = new WeakReference<Thread>(thread);	// weak reference

            // pre-load the known static data (this appears to be needed since there isn't any way
            //  of asking the ClassLoader if a particular class is currently loaded so we don't
            //  load it in an attempt to null static final references on shutdown).
            // We only do this to help the debugging mode ALIVE stats (leak checking)
//            if(Utilities.configuration == Utilities.Configuration.Debug) {
//                setupStaticReferences(QtJambi_LibraryShutdown.K_org_qtjambi_qt_gui_QPen,   QtJambi_LibraryShutdown.K_org_qtjambi_qt_gui_QPen_FIELDNAMES);
//                setupStaticReferences(QtJambi_LibraryShutdown.K_org_qtjambi_qt_gui_QColor, QtJambi_LibraryShutdown.K_org_qtjambi_qt_gui_QColor_FIELDNAMES);
//                setupStaticReferences(QtJambi_LibraryShutdown.K_org_qtjambi_qt_gui_QBrush, QtJambi_LibraryShutdown.K_org_qtjambi_qt_gui_QBrush_FIELDNAMES);
//            }
        } catch(Throwable e) {
            e.printStackTrace();
        }
    }

    private static void setupStaticReferences(String className, String[] fieldNameA) {
        Class<? extends Object> clazz;
        try {
            ClassLoader classLoader = QtJambi_LibraryInitializer.class.getClassLoader();
            clazz = Class.forName(className, /*false*/true, classLoader);
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
                if(Modifier.isStatic(field.getModifiers())) {
                    if(Modifier.isFinal(field.getModifiers())) {
                        // Get value (and discard)
                        field.get(clazz);
                    }
                }
            } catch(Exception e) {
                e.printStackTrace();
            }
        }
    }

    /**
     *  This is called from QtJambi_LibraryShutdown.java
     */
    static void removeMessageHandlerForExceptions() {
        if (messageHandler != null) {
            QMessageHandler.removeMessageHandler(messageHandler);
            messageHandler = null;
        }
    }

    private static void installMessageHandlerForExceptions(String config) {
        if (config != null) {
            config = config.trim().toUpperCase();
            // which things throw RuntimeException ?
            final boolean all = config.equals("") || config.equals("ALL") || config.equals("TRUE");
            final boolean critical = config.contains("CRITICAL");
            final boolean debug = config.contains("DEBUG");
            final boolean fatal = config.contains("FATAL");
            final boolean warning = config.contains("WARNING");
            // this option is for System.err for all
            final boolean none = config.contains("NONE");

            if (all || critical || debug || fatal || warning || none) {
                QMessageHandler newMessageHandler = new QMessageHandler() {

                    @Override
                    public void critical(String message) {
                        if (critical || all)
                            throw new RuntimeException("Critical: " + message);
                        else
                            System.err.println("Critical: " + message);
                    }

                    @Override
                    public void debug(String message) {
                        if (debug || all)
                            throw new RuntimeException("Debug: " + message);
                        else
                            System.err.println("Debug: " + message);
                    }

                    @Override
                    public void fatal(String message) {
                        if (fatal || all)
                            throw new RuntimeException("Fatal: " + message);
                        else
                            System.err.println("Fatal: " + message);
                    }

                    @Override
                    public void warning(String message) {
                        if (warning || all)
                            throw new RuntimeException("Warning: " + message);
                        else
                            System.err.println("Warning: " + message);
                    }
                };
                QMessageHandler.replaceMessageHandler(messageHandler, newMessageHandler);
                messageHandler = newMessageHandler;
            }
        }
    }

    public static void init() {}

    private static native void initialize();
}
