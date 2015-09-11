/**************************************************************************
 *
 * Copyright (C) 1992-2009 Nokia. All rights reserved.
 *
 * This file is part of Qt Jambi.
 *
 * ** $BEGIN_LICENSE$
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

 *
 * This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
 * WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 *
 ***************************************************************************/

package org.qtjambi.qt.designer;

import java.lang.reflect.Constructor;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.HashSet;
import java.util.List;
import java.util.Map;

import org.qtjambi.qt.core.QFile;
import org.qtjambi.qt.core.QIODevice;
import org.qtjambi.qt.core.QMargins;
import org.qtjambi.qt.core.QObject;
import org.qtjambi.qt.core.QRect;
import org.qtjambi.qt.core.QSize;
import org.qtjambi.qt.core.Qt;
import org.qtjambi.qt.core.Qt.FocusPolicy;
import org.qtjambi.qt.gui.QIcon;
import org.qtjambi.qt.widgets.QAction;
import org.qtjambi.qt.widgets.QApplication;
import org.qtjambi.qt.widgets.QBoxLayout;
import org.qtjambi.qt.widgets.QComboBox;
import org.qtjambi.qt.widgets.QDockWidget;
import org.qtjambi.qt.widgets.QGridLayout;
import org.qtjambi.qt.widgets.QLabel;
import org.qtjambi.qt.widgets.QLayout;
import org.qtjambi.qt.widgets.QLayoutItem;
import org.qtjambi.qt.widgets.QLayoutItemInterface;
import org.qtjambi.qt.widgets.QListWidget;
import org.qtjambi.qt.widgets.QListWidgetItem;
import org.qtjambi.qt.widgets.QMainWindow;
import org.qtjambi.qt.widgets.QMenu;
import org.qtjambi.qt.widgets.QMenuBar;
import org.qtjambi.qt.widgets.QSizePolicy;
import org.qtjambi.qt.widgets.QSpacerItem;
import org.qtjambi.qt.widgets.QStatusBar;
import org.qtjambi.qt.widgets.QTabWidget;
import org.qtjambi.qt.widgets.QToolBar;
import org.qtjambi.qt.widgets.QWidget;
import org.qtjambi.qt.xml.QDomDocument;
import org.qtjambi.qt.xml.QDomElement;
import org.qtjambi.qt.xml.QDomNode;
import org.qtjambi.qt.xml.QDomNodeList;

public class QUiLoader {

    private interface PropertyReceiver {
        void setProperty(String name, Object value);
    }

    private class QObjectPropertyReceiver implements PropertyReceiver {
        QObject object;
        QObjectPropertyReceiver(QObject o) { this.object = o; }
        public void setProperty(String name, Object value) {
             if (name.equals("text") && !value.toString().equals("")) {
                 value = (Object) QApplication.translate(xmlClassName, value.toString());
             }
             self().setProperty(object, name, value);
        }
    }

    private class SpacerPropertyReceiver implements PropertyReceiver {
        Qt.Orientation orientation;
        QSize size;
        public void setProperty(String name, Object value) {
            if (name.equals("orientation")) orientation = (Qt.Orientation) value;
            else if (name.equals("sizeHint")) size = (QSize) value;
        }

        QSpacerItem spacerItem() {
            return new QSpacerItem(
                    size.width(),
                    size.height(),
                    orientation == Qt.Orientation.Horizontal
                            ? QSizePolicy.Policy.Expanding
                            : QSizePolicy.Policy.Minimum,
                    orientation == Qt.Orientation.Horizontal
                            ? QSizePolicy.Policy.Minimum
                            : QSizePolicy.Policy.Expanding);
        }
    }

    private class ContentItemPropertyReceiver implements PropertyReceiver {
        String text;
        QIcon icon;
        public void setProperty(String name, Object value) {
            if (name.equals("text")) text = (String) value;
            else if (name.equals("icon")) icon = (QIcon) value;
        }

        QListWidgetItem listWidgetItem() {
            return new QListWidgetItem(icon, text);
        }
    }

    private class AttributeReceiver implements PropertyReceiver {

        private QMainWindow mainWindow() {
            QWidget toplevel = parent instanceof QWidget ? ((QWidget) parent).window() : null;
            if (toplevel instanceof QMainWindow)
                return (QMainWindow) toplevel;
            return null;
        }

        public void setProperty(String name, Object value) {
            if (name.equals("toolBarArea")) {
                QMainWindow mw = mainWindow();
                if (mw != null && parent instanceof QToolBar)
                    mw.addToolBar(Qt.ToolBarArea.resolve((Integer) value), (QToolBar) parent);
            } else if (name.equals("dockWidgetArea")) {
                QMainWindow mw = mainWindow();
                if (mw != null && parent instanceof QDockWidget)
                    mw.addDockWidget(Qt.DockWidgetArea.resolve((Integer) value), (QDockWidget) parent);
            }

        }
    }

    public static QWidget load(QIODevice device) throws QUiLoaderException {
        return load(device, null);
    }

    public static QWidget load(QIODevice device, QWidget parent) throws QUiLoaderException {
        QUiLoader loader = new QUiLoader(device, parent);
        loader.parse();
        return loader.widget();
    }

    private QUiLoader(QIODevice device, QWidget parent) {
        this.inputDevice = device;
        this.topParent = parent;
    }

    private void parse() throws QUiLoaderException {
        if (!inputDevice.isOpen()) {
            if (!inputDevice.open(QIODevice.OpenModeFlag.ReadOnly)) {
                throw new QUiLoaderException("Unable to open iodevice: " + inputDevice);
            }
        }

        QDomDocument doc = new QDomDocument();
        doc.setContent(inputDevice);

        object = topParent;

        parseChildren(doc, topParent);

        uiWidget = (QWidget) object;

        // Reassign ownership to toplevel parent...
        for (QAction a : actions.values()) {
            a.setParent(uiWidget);
        }

        // Setup buddies...
        for (Map.Entry<QLabel, String> e : buddies.entrySet()) {
            QWidget buddy = widgetPool.get(e.getValue());
            e.getKey().setBuddy(buddy);
        }
    }

    private void parseChildren(QDomNode node, Object parent) throws QUiLoaderException {
        QDomNodeList children = node.childNodes();
        for (int i=0; i<children.size(); ++i) {
            this.parent = parent;
            parse(children.at(i));
        }
    }


    private void parse(QDomNode domNode) throws QUiLoaderException {
        String name = domNode.nodeName();

//       ++parseDepth;
//       for (int i=0; i< parseDepth; ++i) {
//           System.out.print("  ");
//       }
//       System.out.print(name + ", parent=");
//       if (parent instanceof QObject)
//           System.out.println(((QObject) parent).objectName() + " [" + parent.getClass() + "]");
//       else
//           System.out.println(parent);

        if (name.equals("ui")) parseUiRoot(domNode);
        else if (name.equals("widget")) parseWidget(domNode);
        else if (name.equals("layout")) parseLayout(domNode);
        else if (name.equals("property")) parseProperty(domNode);
        else if (name.equals("item")) parseItem(domNode);
        else if (name.equals("spacer")) parseSpacer(domNode);
        else if (name.equals("connections")) parseConnections(domNode);
        else if (name.equals("taborder")) parseTabOrder(domNode);
        else if (name.equals("tabstops")) parseTabOrder(domNode);
        else if (name.equals("addaction")) parseAddAction(domNode);
        else if (name.equals("attribute")) parseAttribute(domNode);
        else if (name.equals("action")) parseAction(domNode);
        else if (name.equals("class")) parseClass(domNode);
        else if (!ignorableStrings.contains(name)) throw new QUiLoaderException("Unknown tag: " + name);

//       if (object != null) {
//           for (int i=0; i< parseDepth; ++i) {
//               System.out.print("  ");
//           }
//           if (object instanceof QObject)
//               System.out.println(" - object: " + ((QObject) object).objectName() + ", " + object.getClass());
//           else
//               System.out.println(" - object: " + object);
//       }
//       --parseDepth;
    }

    private void parseUiRoot(QDomNode node) throws QUiLoaderException {
        QDomElement ui = node.toElement();

        String version = ui.attribute("version");
        if (!version.equals("4.0"))
            throw new QUiLoaderException("Unsupported version: " + version + ", expected 4.0");

        String language = ui.attribute("language");
        if (!language.equals("jambi"))
            throw new QUiLoaderException("Unsupported language: '" + language + "', expected 'jambi'");

        parseChildren(ui, topParent);
    }

    private void parseWidget(QDomNode node) throws QUiLoaderException {
        QDomElement widgetNode = node.toElement();

        // Should be a safe cast as parent is either layout or qwidget...
        QObject parent = (QObject) this.parent;

        // Create the uiWidget
        String cls = widgetNode.attribute("class");
        String name = widgetNode.attribute("name");

        Class<? extends QObject> cl = loadClass(cls);
        QWidget widget = (QWidget) createInstance(cl, parent);

        // Menus are referred to via the addaction tag so we need to add its action.
        if (widget instanceof QMenu) {
            actions.put(name, ((QMenu) widget).menuAction());
        }

        // To avoid that the widgets get deleted because there are no refs to them, we pool
        // them in a set. This is not a problem in practice as one does not loose the refs to
        // objects before they are assigned to a layout that _is_ assigned to a widget. Here
        // we need it because the layouts are not assigned to a widget before their child widgets
        // get collected, and they thus, don't have a parent...
        widgetPool.put(name, widget);

        widget.setObjectName(name);

        if (parent instanceof QMainWindow) {
            QMainWindow mainWindow = (QMainWindow) parent;
            if (name.equals("centralwidget"))
                mainWindow.setCentralWidget(widget);
            else if (name.equals("menubar"))
                mainWindow.setMenuBar((QMenuBar) widget);
            else if (name.equals("statusbar"))
                mainWindow.setStatusBar((QStatusBar) widget);

            // These are added when positioned via their attribute, see AttributeReceiver above
            else if (widget instanceof QToolBar || widget instanceof QDockWidget);

            else
                System.err.println("unhandled child of main window..." + widget + ", " + widget.objectName());
        } else if (parent instanceof QTabWidget) {
            String title = widgetNode.namedItem("attribute").firstChild().firstChild().nodeValue();
            ((QTabWidget ) parent).addTab(widget, title);

        // QDockWidgets don't have a special layout, so add it properly
        } else if (parent instanceof QDockWidget) {
            ((QDockWidget) parent).setWidget(widget);
        }

        PropertyReceiver oldReceiver = swapPropertyReceiver(new QObjectPropertyReceiver(widget));
        parseChildren(node, widget);
        swapPropertyReceiver(oldReceiver);

        this.object = widget;
    }

    private static int parseInt(String s, int def) {
        try {
            return Integer.parseInt(s);
        } catch (Exception e) {
            return def;
        }
    }

    private void parseLayout(QDomNode node) throws QUiLoaderException {
        QDomElement layoutNode = node.toElement();

        Class<? extends QObject> cls = loadClass(layoutNode.attribute("class"));
        QLayout layout = null;
        try {
            layout = (QLayout) cls.newInstance();
        } catch (Exception e) {
            throw new QUiLoaderException("Failed to create layout: " + cls.getName(), e);
        }

        Object parent = this.parent;

        if (parent instanceof QWidget)
            ((QWidget) parent).setLayout(layout);

        PropertyReceiver oldReceiver = swapPropertyReceiver(new QObjectPropertyReceiver(layout));
        parseChildren(node, layout);
        swapPropertyReceiver(oldReceiver);

        this.object = layout;
    }

    private void parseItem(QDomNode node) throws QUiLoaderException {
        if (parent instanceof QLayout) {
            parseLayoutItem(node);
        } else if (parent instanceof QListWidget) {
            parseListWidgetItem(node);
        } else if (parent instanceof QComboBox) {
            parseComboBoxItem(node);
        } else {
            System.err.println("Unhandled 'item' for '" + parent + "'");
        }
    }

    private void parseComboBoxItem(QDomNode node) throws QUiLoaderException {
        ContentItemPropertyReceiver item = new ContentItemPropertyReceiver();
        PropertyReceiver oldReceiver = swapPropertyReceiver(item);
        parseChildren(node, parent);
        swapPropertyReceiver(oldReceiver);

        QComboBox box = (QComboBox) parent;
        box.addItem(item.icon, item.text);
    }

    private void parseLayoutItem(QDomNode node) throws QUiLoaderException {

        QDomElement itemNode = node.toElement();

        assert this.parent instanceof QLayout;

        QLayout layout = (QLayout) this.parent;

        parseChildren(node, layout);

        Object child = this.object;

        if (layout instanceof QGridLayout) {
            int row = parseInt(itemNode.attribute("row"), 0);
            int col = parseInt(itemNode.attribute("column"), 0);
            int colspan = parseInt(itemNode.attribute("colspan"), 1);
            int rowspan = parseInt(itemNode.attribute("rowspan"), 1);

            QGridLayout grid = ((QGridLayout) layout);
            if (child instanceof QWidget) {
                grid.addWidget((QWidget) child, row, col, rowspan, colspan);
            } else if (child instanceof QLayout) {
                grid.addLayout((QLayout) child, row, col, rowspan, colspan);
            } else if (child instanceof QLayoutItemInterface) {
                grid.addItem((QLayoutItemInterface) child, row, col, rowspan, colspan);
            }

        } else {
            if (child instanceof QWidget) {
                layout.addWidget((QWidget) child);
            } else if (child instanceof QLayout) {
                ((QBoxLayout) layout).addLayout((QLayout) child);
            } else if (child instanceof QLayoutItem) {
                layout.addItem((QLayoutItem) child);
            }
        }


    }

    private void parseListWidgetItem(QDomNode node) throws QUiLoaderException {
        ContentItemPropertyReceiver item = new ContentItemPropertyReceiver();
        PropertyReceiver oldReceiver = swapPropertyReceiver(item);
        parseChildren(node, parent);
        QListWidget listWidget = (QListWidget) parent;
        listWidget.addItem(item.listWidgetItem());
        swapPropertyReceiver(oldReceiver);
    }

    private void parseSpacer(QDomNode node) throws QUiLoaderException {
        PropertyReceiver old = swapPropertyReceiver(new SpacerPropertyReceiver());
        parseChildren(node, this.parent);
        this.object = ((SpacerPropertyReceiver) propertyReceiver).spacerItem();
        swapPropertyReceiver(old);
    }

    private void parseProperty(QDomNode node) throws QUiLoaderException {
        QDomElement e = node.toElement();

        String name = e.attribute("name");
        assert !e.isNull();
        QDomElement val = e.firstChildElement();

        AbstractPropertyHandler handler = propertyHandlers.get(name);
        if (handler == null) {
            handler = typeHandlers.get(val.nodeName());
        }

        if (handler == null) {
            throw new QUiLoaderException("No handler for property " + name + ", of type: " + val.nodeName());
        }

        Object value = handler.create(val);
        propertyReceiver.setProperty(name, value);
    }

    private QObject createInstance(Class<? extends QObject> cl, QObject parent) throws QUiLoaderException {
        try {
            if (QWidget.class.isAssignableFrom(cl)) {
                if (parent instanceof QWidget) {
                    return cl.getConstructor(QWidget.class).newInstance((QWidget) parent);
                } else {
                    return cl.getConstructor().newInstance();
                }
            } else if (QLayout.class.isAssignableFrom(cl)) {
                if (parent instanceof QWidget) {
                    Constructor<? extends QObject> ctor = (Constructor<? extends QObject>) cl.getConstructor(QWidget.class);
                    return (QObject) ctor.newInstance(parent);
                } else {
                    return (QLayout) cl.getConstructor().newInstance();
                }
            } else {
                return cl.getConstructor(QObject.class).newInstance();
            }
        } catch (Exception e) {
            throw new QUiLoaderException("Failed to create instance", e);
        }
    }

    private Class<? extends QObject> loadClass(String cls) throws QUiLoaderException {
        Class<? extends QObject> cl;

        if (cls.equals("Line")) return Line.class;

        try {
            cl = Class.forName(cls).asSubclass(QObject.class);
        } catch (ClassNotFoundException e) {
            try {
                cl = Class.forName("org.qtjambi.qt.gui." + cls).asSubclass(QObject.class);
            } catch (Exception ex) {
                throw new QUiLoaderException("Failed to load class", ex);
            }
        } catch (Exception e) {
            throw new QUiLoaderException("Failed to load class", e);
        }
        return cl;
    }

    @SuppressWarnings("deprecation")
    private void setProperty(QObject o, String property, Object value) {
        try {
            if (property.equals("geometry") && o.isWidgetType() && ((QWidget) o).isWindow()) {
                QWidget window = (QWidget) o;
                QRect r = (QRect) value;
                if (r.x() == 0 && r.y() == 0)
                    window.resize(r.width(), r.height());
                else
                    window.setGeometry(r);
                return;
            } else if (property.equals("shortcut") && o instanceof QAction
                    && value instanceof String) {
                ((QAction) o).setShortcut((String) value);
                return;
            } else if (property.endsWith("focusPolicy") && o instanceof QWidget && value instanceof FocusPolicy) {
                ((QWidget)o).setFocusPolicy((FocusPolicy) value);
                return;
            } else if (property.equals("buddy") && o instanceof QLabel) {
                buddies.put((QLabel) o, (String) value);
                return;
            } else if (property.equals("icon") && o instanceof QWidget && ((QWidget) o).isWindow()) {
                ((QWidget) o).setWindowIcon((QIcon) value);
                return;
            } else if (property.endsWith("Margin") && o instanceof QLayout) {
                writeLayoutMargin((QLayout) o, property, value);
                return;
            } else if (property.endsWith("Spacing") && o instanceof QGridLayout) {
                writeLayoutSpacing((QGridLayout) o, property, value);
                return;
            }

            if (value == null)
                System.out.println("Null value for: " + property + ", " + value);

            if (value != null) {
                o.setProperty(property, value);
            }
        } catch (Exception e) {
            System.err.println("setProperty failed: value=" + value + ", name=" + property + ", on=" + o);
            e.printStackTrace();
        }

    }

    private void writeLayoutMargin(QLayout l, String property, Object value) {
        int x = (Integer) value;

        QMargins margins = l.contentsMargins();

        if (property.equals("rightMargin")) margins.setRight(x);
        else if (property.equals("leftMargin")) margins.setLeft(x);
        else if (property.equals("topMargin")) margins.setTop(x);
        else if (property.equals("bottomMargin")) margins.setBottom(x);

        l.setContentsMargins(margins);
    }

    private void writeLayoutSpacing(QGridLayout o, String property, Object value) {
        if (property.equals("verticalSpacing")) o.setVerticalSpacing((Integer) value);
        else if (property.equals("horizontalSpacing")) o.setHorizontalSpacing((Integer) value);
    }

    private void parseConnections(QDomNode node) throws QUiLoaderException {
        QDomElement e = node.toElement();
        assert !e.isNull();
        try {
        for (QDomElement el = e.firstChildElement(); !el.isNull(); el = el.nextSiblingElement()) {
            if (el.nodeName().equals("connection")) {
                String senderName = el.namedItem("sender").firstChild().nodeValue();
                String signalName = el.namedItem("signal").firstChild().nodeValue();
                String receiverName = el.namedItem("receiver").firstChild().nodeValue();
                String slotSignature = el.namedItem("slot").firstChild().nodeValue();

                QObject sender = widgetPool.get(senderName);
                if (sender == null)
                    sender = actions.get(senderName);
                if (sender == null)
                    throw new QUiLoaderException("Unknown sender: '" + senderName + "'");

                QObject receiver = widgetPool.get(receiverName);
                if (receiver == null)
                    throw new QUiLoaderException("Unknown sender: '" + receiverName + "'");

                int lt = signalName.indexOf('<');
                if (lt > 0)
                    signalName = signalName.substring(0, lt);

                int paren = signalName.indexOf('(');
                if (paren > 0)
                    signalName = signalName.substring(0, paren);

                // Get the signal object...
                try {
                    QObject.AbstractSignal signal = (QObject.AbstractSignal) sender.getClass().getField(signalName).get(sender);
                    signal.connect(receiver, slotSignature);
                } catch (Exception ex) {
                    throw new QUiLoaderException("Connection failed: "
                                                 + senderName + "." + signalName
                                                 + " to " + receiverName + "." + slotSignature, ex);
                }
            }
        }
        } catch (Exception ex) { ex.printStackTrace(); }
    }

    private void parseTabOrder(QDomNode node) {
        QDomElement e = node.toElement();
        if (e.isNull())
            return;
        List<String> list = new ArrayList<String>();
        for (QDomElement el = e.firstChildElement(); !el.isNull(); el = el.nextSiblingElement()) {
            assert !el.isNull();
            if (el.nodeName().equals("tabstop"))
                list.add(el.firstChild().nodeValue());
        }

        for (int i=0; i<list.size() - 1; ++i) {
            QWidget w1 = widgetPool.get(list.get(i));
            QWidget w2 = widgetPool.get(list.get(i+1));
            QWidget.setTabOrder(w1, w2);
        }
    }

    private void parseAddAction(QDomNode node) {
        QDomElement e = node.toElement();

        if (e.isNull())
            return;

        String name = e.attribute("name");

        // Special case separators based on the parent...
        if (name.equals("separator")) {
            if (parent instanceof QMenu) {
                ((QMenu) parent).addSeparator();
            } else if (parent instanceof QToolBar) {
                ((QToolBar) parent).addSeparator();
            }
            return;
        }

        QAction action = existingAction(name);

        if (parent instanceof QWidget) {
            ((QWidget) parent).addAction(action);
        }
    }

    private QAction existingAction(String name) {
        QAction action = actions.get(name);
        if (action == null) {
            action = new QAction(null);
            action.setObjectName(name);
            actions.put(name, action);
        }
        return action;
    }

    private void parseAttribute(QDomNode node) throws QUiLoaderException {
        AttributeReceiver attr = new AttributeReceiver();
        PropertyReceiver defaultReceiver = swapPropertyReceiver(attr);

        parseProperty(node);

        swapPropertyReceiver(defaultReceiver);
    }

    private void parseAction(QDomNode node) throws QUiLoaderException {
        QDomElement e = node.toElement();
        if (e.isNull())
            return;

        String name = e.attribute("name");
        QAction action = existingAction(name);

        QObjectPropertyReceiver rcv = new QObjectPropertyReceiver(action);
        PropertyReceiver old = swapPropertyReceiver(rcv);

        parseChildren(node,  action);

        swapPropertyReceiver(old);
    }

    private void parseClass(QDomNode node) throws QUiLoaderException {
        QDomElement e = node.toElement();
        if (e.isNull())
            return;

        QDomNode n = e.firstChild();
        if (!n.isNull())
            xmlClassName = n.nodeValue();
    }

    private QWidget widget() {
        return uiWidget;
    }

    private Object swapObject(Object o) {
        Object x = object;
        object = o;
        return x;
    }

    private PropertyReceiver swapPropertyReceiver(PropertyReceiver recv) {
        PropertyReceiver r = propertyReceiver;
        propertyReceiver = recv;
        return r;
    }

    private QUiLoader self() { return this; }

    private QIODevice inputDevice;
    private QWidget topParent;
    private QWidget uiWidget;

    private String xmlClassName = "";

    private int parseDepth;
    private Object parent;
    private Object object;
    private PropertyReceiver propertyReceiver;

    private HashMap<String, QWidget> widgetPool = new HashMap<String, QWidget>();
    private HashMap<String, QAction> actions = new HashMap<String, QAction>();
    private HashMap<QLabel, String> buddies = new HashMap<QLabel, String>();

    private static HashSet<String> ignorableStrings;
    private static HashMap<String, AbstractPropertyHandler> typeHandlers;
    private static HashMap<String, AbstractPropertyHandler> propertyHandlers;

    static {
        typeHandlers = new HashMap<String, AbstractPropertyHandler>();
        typeHandlers.put("cstring", new StringPropertyHandler());
        typeHandlers.put("rect", new RectPropertyHandler());
        typeHandlers.put("string", new StringPropertyHandler());
        typeHandlers.put("number", new NumberPropertyHandler());
        typeHandlers.put("sizepolicy", new SizePolicyPropertyHandler());
        typeHandlers.put("size", new SizePropertyHandler());
        typeHandlers.put("point", new PointPropertyHandler());
        typeHandlers.put("bool", new BoolPropertyHandler());
        typeHandlers.put("enum", new EnumPropertyHandler());
        typeHandlers.put("font", new FontPropertyHandler());
        typeHandlers.put("palette", new PalettePropertyHandler());
        typeHandlers.put("color", new ColorPropertyHandler());
        typeHandlers.put("pixmap", new PixmapPropertyHandler());
        typeHandlers.put("iconset", new IconsetPropertyHandler());
        typeHandlers.put("set", new SetPropertyHandler());

        propertyHandlers = new HashMap<String, AbstractPropertyHandler>();
        propertyHandlers.put("orientation", new OrientationPropertyHandler());

        ignorableStrings = new HashSet<String>();
        ignorableStrings.add("author");
        ignorableStrings.add("comment");
        ignorableStrings.add("customwidgets");
        ignorableStrings.add("exportmacro");
        ignorableStrings.add("pixmapfunction");
        ignorableStrings.add("resources");
    }

    public static void main(String args[]) throws QUiLoaderException {
        QApplication.initialize(args);

        QFile file = new QFile(args[0]);
        long t1 = System.currentTimeMillis();
        QWidget w = load(file);
        long t2 = System.currentTimeMillis();
        System.out.println("loading took: " + (t2 - t1));
        file.dispose();
        w.show();

        QApplication.execStatic();
        QApplication.shutdown();
    }
}
