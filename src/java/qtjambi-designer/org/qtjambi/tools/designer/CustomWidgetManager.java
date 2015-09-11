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

package org.qtjambi.tools.designer;

import java.io.File;
import java.net.URL;
import java.net.URLClassLoader;
import java.util.ArrayList;
import java.util.Collections;
import java.util.Iterator;
import java.util.List;

import org.qtjambi.qt.core.QDir;
import org.qtjambi.qt.core.QFile;
import org.qtjambi.qt.core.QFileInfo;
import org.qtjambi.qt.widgets.*;
import org.qtjambi.qt.gui.*;
import org.qtjambi.qt.qtjambi.util.RetroTranslatorHelper;
import org.qtjambi.qt.xml.QDomDocument;
import org.qtjambi.qt.xml.QDomElement;
import org.qtjambi.qt.xml.QDomNode;
import org.qtjambi.qt.xml.QDomNodeList;

/**
 * The CustomWidgetManager class is used by the designer custom widget plugin to
 * load Java Widgets and expose them to designer.
 *
 * @author gunnar
 */
public class CustomWidgetManager {

    private CustomWidgetManager() {
        loadPlugins();
    }

    public static CustomWidgetManager instance() {
        if (instance == null)
            instance = new CustomWidgetManager();
        return instance;
    }

    public List<CustomWidget> customWidgets() {
        return customWidgets;
    }

    private void splitIntoList(String s, List<String> lst) {
        if (s != null)
            Collections.addAll(lst, RetroTranslatorHelper.split(s, File.pathSeparator));
    }

    private void loadPlugins() {
        warnings.clear();
        List<String> paths = new ArrayList<String>();

        splitIntoList(System.getenv("QT_PLUGIN_PATH"), paths);
        splitIntoList(System.getProperty("org.qtjambi.qt.plugin-path"), paths);

        for (String path : paths) {
            loadPluginsFromPath(path + "/qtjambi");
        }

        if (warnings.size() > 0) {
            StringBuilder sb = new StringBuilder();
            for (String s : warnings)
                sb.append(s).append("\n");
            QMessageBox.warning(null, "Custom widgets loading!", sb.toString());
        }
    }

    private void loadPlugins(String path) {
        customWidgets.clear();
        String paths[] = RetroTranslatorHelper.split(path, System.getProperty("path.separator"));
        for (int i=0; i<paths.length; ++i) {
            loadPluginsFromPath(paths[i]);
        }
    }

    private void loadPluginsFromPath(String path) {
        QDir dir = new QDir(path);
        if (!new QFileInfo(dir.absolutePath()).exists()) {
            // This warning is annoying, we should reduce it to a single warning when not qtjambi/*.xml was found on startup
            warn("CustomWidgetManager: plugin path doesn't exist: " + path);
            return;
        }

        List<String> nameFilters = new ArrayList<String>();
        nameFilters.add("*.xml");
        List<QFileInfo> plugins = dir.entryInfoList(nameFilters);

        for (QFileInfo fi : plugins) {
            loadPlugin(fi.absoluteFilePath());
        }
    }

    private void loadPlugin(String fileName) {
        QDomDocument doc = new QDomDocument();
        doc.setContent(new QFile(fileName));

        QDomElement root = doc.firstChild().toElement();

        QDomNodeList entries = root.childNodes();

        for (int i=0; i<entries.size(); ++i) {
            String errorPrefix = fileName + " : entry " + (i+1);

            QDomElement e = entries.at(i).toElement();

            String className = e.attribute("class");
            if (className.length() == 0) {
                warn(errorPrefix + "; missing 'class' attribute");
                continue;
            }

            try {
                Class<? extends QWidget> type = null;
                boolean failure = false;
                try {
                    Class<?> tmpClazz = Class.forName(e.attribute("class"));
                    type = tmpClazz.asSubclass(QWidget.class);
                } catch (ClassCastException f) {
                    failure = true;
                } catch (ClassNotFoundException f) {
                    failure = true;
                } catch (ExceptionInInitializerError error) {
                    failure = true;
                }

                if (failure) {
                    try {
                        String classPathsProperty = System.getProperty("org.qtjambi.qtjambi.internal.current.classpath");
                        if (classPathsProperty != null) {
                            String classpaths[] = RetroTranslatorHelper.split(classPathsProperty, ";");  // CHECKME confirm this is valid for unix

                            URL urls[] = new URL[classpaths.length];
                            for (int j=0; j<classpaths.length; ++j)
                                urls[j] = new URL(classpaths[j]);

                            URLClassLoader loader = new URLClassLoader(urls, getClass().getClassLoader());
                            Class<?> tmpClazz = loader.loadClass(e.attribute("class"));
                            type = tmpClazz.asSubclass(QWidget.class);
                        }
                    } catch (ExceptionInInitializerError error) {
                    } catch (ClassCastException g) {
                    } catch (ClassNotFoundException g) {
                        // Never mind. Some classes, like QWebView are not present in
                        // all configurations, so we have to silently ignore these exceptions
                    }
                }

                if (type != null) {
                    CustomWidget customWidget = new CustomWidget(type);
                    customWidgets.add(customWidget);

                    // The simple properties...
                    String group = e.attribute("group");
                    if (group.length() == 0)
                        group = "Qt Jambi Custom Widgets";
                    customWidget.setGroup(group);
                    customWidget.setTooltip(e.attribute("tool-tip"));
                    customWidget.setWhatsThis(e.attribute("whats-this"));
                    customWidget.setName(e.attribute("name"));
                    customWidget.setIncludeFile(e.attribute("import"));

                    // The icon
                    String iconPath = e.attribute("icon");
                    QIcon icon = null;
                    if (iconPath.length() != 0) {
                        icon = new QIcon(iconPath);
                        if (icon.isNull()) {
                            warn(errorPrefix + "; icon '" + iconPath + "' not loaded");
                            icon = null;
                        }
                    }
                    customWidget.setIcon(icon);

                    // is it a container?
                    boolean isContainer = false;
                    String container = e.attribute("container");
                    if (container != null) {
                        container = container.toLowerCase();
                        isContainer = container.equals("yes") || container.equals("true");
                    }
                    customWidget.setContainer(isContainer);
                }

            } catch (Exception ex) {
                warn("class=" + className
                     + ", file=" + fileName
                     + ", error=" + ex.getMessage());
                ex.printStackTrace();
            }
        }
    }

    private void warn(String s) {
        warnings.add(s);
    }

    private List<CustomWidget> customWidgets = new ArrayList<CustomWidget>();
    private List<String> warnings = new ArrayList<String>();

    private static CustomWidgetManager instance;



    public static void main(String[] args) {
        QApplication.initialize(args);

        List<CustomWidget> list = instance().customWidgets();
        for (CustomWidget w : list)
            w.createWidget(null).show();

        QApplication.execStatic();
        QApplication.shutdown();
    }

}
