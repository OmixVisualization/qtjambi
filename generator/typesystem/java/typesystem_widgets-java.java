/****************************************************************************
**
** Copyright (C) 1992-2009 Nokia. All rights reserved.
** Copyright (C) 2009-2015 Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
**
** This file is part of Qt Jambi.
**
** ** $BEGIN_LICENSE$
**
** GNU Lesser General Public License Usage
** This file may be used under the terms of the GNU Lesser
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
** $END_LICENSE$
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

package generator;

import org.qtjambi.qt.QNativePointer;
import org.qtjambi.qt.QtBlockedSlot;
import org.qtjambi.qt.Utilities;
import org.qtjambi.qt.internal.QtJambiObject.QPrivateConstructor;
import org.qtjambi.qt.core.QCoreApplication;
import org.qtjambi.qt.core.QPoint;
import org.qtjambi.qt.core.QPointF;
import org.qtjambi.qt.gui.*;

class QTreeWidgetItemIterator___ extends QTreeWidgetItemIterator {
        @QtBlockedSlot
        public final void next(int i) {
            operator_add_assign(i);
        }

        @QtBlockedSlot
        public final void previous(int i) {
            operator_subtract_assign(i);
        }

        @QtBlockedSlot
        public final void next() {
            operator_increment();
        }

        @QtBlockedSlot
        public final void previous() {
            operator_decrement();
        }

        @QtBlockedSlot
        public final QTreeWidgetItem current() {
            return operator_multiply();
        }
}// class

class QLineEdit___ extends QLineEdit {

        /**
            The TextMargins class represents the margins of
            the text in a QLineEdit.<p>It consists of four
            public integers, giving the top, bottom,
            left, and right margins.
        */
        public static final class TextMargins {
                /** The top margin of the QLineEdit. */
                public final int top;
                /** The bottom margin of the QLineEdit */
                public final int bottom;
                /** The left margin of the QLineEdit. */
                public final int left;
                /** The right margin of the QLineEdit. */
                public final int right;

                private TextMargins(int left, int top, int right, int bottom) {
                    this.left = left;
                    this.top = top;
                    this.right = right;
                    this.bottom = bottom;
                }
        }

        public final TextMargins getTextMargins() {
            QNativePointer left = new QNativePointer(QNativePointer.Type.Int);
            QNativePointer top = new QNativePointer(QNativePointer.Type.Int);
            QNativePointer right = new QNativePointer(QNativePointer.Type.Int);
            QNativePointer bottom = new QNativePointer(QNativePointer.Type.Int);

            getTextMargins_private(left, top, right, bottom);

            return new TextMargins(left.intValue(), top.intValue(), right.intValue(),
                                   bottom.intValue());
        }
}// class

class QWizardPage___ extends QWizardPage {

        protected final void registerField(String name, QWidget widget) {
            registerField(name, widget, (org.qtjambi.qt.QNativePointer) null, (org.qtjambi.qt.QNativePointer) null);
        }

        protected final void registerField(String name, QWidget widget, String property) {
            registerField(name, widget, QNativePointer.createCharPointer(property), null);
        }

        /**
         * Creates a field called <code>name</code> associated with the given
         * property of the given widget. From then on, that property becomes
         * accessible using <code>field()</code> and <code>setField()</code>.
         * Fields are global to the entire wizard and make it easy for any single
         * page to access information stored by another page, without having to put
         * all the logic in <code>QWizard</code> or having the pages know
         * explicitly about each other. If name ends with an asterisk (*), the field
         * is a mandatory field. When a page has mandatory fields, the Next and/or
         * Finish buttons are enabled only when all mandatory fields are filled.
         * This requires a <code>changedSignal</code> to be specified, to tell
         * QWizard to recheck the value stored by the mandatory field.
         *
         * QWizard knows the most common Qt widgets. For these (or their
         * subclasses), you don't need to specify a <code>property</code> or a
         * <code>changedSignal</code>. The table below lists these widgets:
         *
         * <code>QAbstractButton</code> (for which the relevant property is the
         * <code>checked</code> property), <code>QAbstractSlider</code> (the
         * <code>value</code> property), <code>QComboBox</code> (<code>currentIndex</code>
         * property), <code>QDateTimeEdit</code>(<code>dateTime</code>
         * property), <code>QLineEdit</code>(<code>text</code> property),
         * <code>QListWidget</code>(<code>currentRow</code> property), or
         * <code>QSpinBox</code>(<code>value</code> property).
         *
         * @param name
         *            The name which will be used to access the field. Names ending
         *            with an asterisk signify mandatory fields.
         * @param widget
         *            The widget whose property will be accessed using this field.
         * @param property
         *            The name of the property associated with the field.
         * @param changedSignal
         *            The name of a signal which is emitted when the associated
         *            property's value changes.
         *
         * @see org.qtjambi.qt.gui.QWizardPage#field
         * @see org.qtjambi.qt.gui.QWizardPage#setField
         */
        protected final void registerField(String name, QWidget widget, String property, String changedSignal) {
            String signalSignature = org.qtjambi.qt.internal.MetaObjectTools.cppSignalSignature(widget, changedSignal);
            if (signalSignature.length() == 0)
                throw new QNoSuchSignalException("Signal '" + changedSignal
                                                 + "' does not exist or has argument types that cannot be converted to Qt Jambi or java.lang types.");
            registerField(name, widget, org.qtjambi.qt.QNativePointer.createCharPointer(property), org.qtjambi.qt.QNativePointer
                          .createCharPointer(org.qtjambi.qt.internal.QtJambiInternal.SignalPrefix + signalSignature));
        }

}// class

class QFontDialog___ extends QFontDialog {

        public static final class Result {
                public Result(QFont font, boolean ok) {
                    this.font = font;
                    this.ok = ok;
                }

                public QFont font;
                public boolean ok;
        }

        public static Result getFont(QFont initial, QWidget parent, String title, FontDialogOptions options) {
            QNativePointer np = new QNativePointer(QNativePointer.Type.Boolean);
            QFont returned = getFont(np, initial, parent, title, options);
            return new Result(returned, np.booleanValue());
        }

        public static Result getFont(QFont initial, QWidget parent, String caption) {
            QNativePointer np = new QNativePointer(QNativePointer.Type.Boolean);
            QFont returned = getFont(np, initial, parent, caption);
            return new Result(returned, np.booleanValue());
        }

        public static Result getFont(QWidget parent) {
            QNativePointer np = new QNativePointer(QNativePointer.Type.Boolean);
            QFont returned = getFont(np, parent);
            return new Result(returned, np.booleanValue());
        }

        public static Result getFont() {
            return getFont((QWidget) null);
        }

        public static Result getFont(QFont initial, QWidget parent) {
            QNativePointer np = new QNativePointer(QNativePointer.Type.Boolean);
            QFont returned = getFont(np, initial, parent);
            return new Result(returned, np.booleanValue());
        }

        public static Result getFont(QFont initial) {
            return getFont(initial , (QWidget) null);
        }

}// class

class QMenu___ extends QMenu {

        protected final void initStyleOption(org.qtjambi.qt.widgets.QStyleOptionMenuItem option, QAction action) {
            initStyleOption(option.nativePointer(), action);
        }

}// class

class QMenuBar___ extends QMenuBar {

        protected final void initStyleOption(org.qtjambi.qt.widgets.QStyleOptionMenuItem option, QAction action) {
            initStyleOption(option.nativePointer(), action);
        }

}// class

class QShortcut___ extends QShortcut {

        public QShortcut(QKeySequence key, QWidget parent) {
            this(key, parent, null, null, org.qtjambi.qt.core.Qt.ShortcutContext.WindowShortcut);
        }

        public QShortcut(QKeySequence key, QWidget parent, org.qtjambi.qt.core.Qt.ShortcutContext context) {
            this(key, parent, null, null, context);
        }

}// class

class QAbstractButton___ extends QAbstractButton {

        /**
         * Sets the shortcut to the key sequence for the given key string. For
         * example "Ctrl+O" gives CTRL+'O'. The strings "Ctrl", "Shift", "Alt" and
         * "Meta" are recognized, as well as their translated equivalents in the
         * "QShortcut" context (using QObject::tr()). Up to four key codes may be
         * entered by separating them with commas, e.g. "Alt+X,Ctrl+S,Q".
         *
         * @param key
         *            The description of the key sequence. Typically used with tr()
         *            so key sequences can be locale aware.
         */
        public final void setShortcut(String key) {
            setShortcut(new QKeySequence(key));
        }

        /**
         * Sets the shortcut to the key sequence for the given key. The result will
         * depend on the currently running platform. The key sequence will be based
         * on the first element in the list of key bindings for the key.
         *
         * @param key
         *            The key for which to select a key sequence
         */
        public final void setShortcut(QKeySequence.StandardKey key) {
            setShortcut(new QKeySequence(key));
        }

}// class

class QStyle___ extends QStyle {

        @QtBlockedSlot
        public final int combinedLayoutSpacing(QSizePolicy.ControlTypes controls1, QSizePolicy.ControlTypes controls2,
                                               org.qtjambi.qt.core.Qt.Orientation orientation, QStyleOption option, QWidget widget) {
            return combinedLayoutSpacing(controls1, controls2, orientation, option == null ? null : option.nativePointer(), widget);
        }

        @QtBlockedSlot
        public final int combinedLayoutSpacing(QSizePolicy.ControlTypes controls1, QSizePolicy.ControlTypes controls2,
                                               org.qtjambi.qt.core.Qt.Orientation orientation, QStyleOption option) {
            return combinedLayoutSpacing(controls1, controls2, orientation, option, null);
        }

        @QtBlockedSlot
        public final int combinedLayoutSpacing(QSizePolicy.ControlTypes controls1, QSizePolicy.ControlTypes controls2,
                                               org.qtjambi.qt.core.Qt.Orientation orientation) {
            return combinedLayoutSpacing(controls1, controls2, orientation, null);
        }

}// class

class QLayout___ extends QLayout {

}// class

class QGridLayout___ extends QGridLayout {

        public final org.qtjambi.qt.gui.QTableArea getItemPosition(int index) {
            QNativePointer row = new QNativePointer(QNativePointer.Type.Int);
            QNativePointer column = new QNativePointer(QNativePointer.Type.Int);
            QNativePointer rowSpan = new QNativePointer(QNativePointer.Type.Int);
            QNativePointer columnSpan = new QNativePointer(QNativePointer.Type.Int);

            getItemPosition(index, row, column, rowSpan, columnSpan);

            return new org.qtjambi.qt.gui.QTableArea(row.intValue(), column.intValue(), rowSpan.intValue(), columnSpan.intValue());
        }

}// class

class QWidget___ extends QWidget {

        private native static void __qt_QMessageBox_setWindowTitle(long native_id, String windowTitle);

        private native static void __qt_QMessageBox_setWindowModality(long native_id, int modality);

}// class

class QFileDialog___ extends QFileDialog {

        public static class Filter {
                public Filter(String filter) {
                    this.filter = filter;
                }

                public String filter;
                public String selectedFilter = "";
        };

}// class

class QTabBar___ extends QTabBar {

        public final void initStyleOption(QStyleOptionTab option, int tabIndex) {
            initStyleOption(option.nativePointer(), tabIndex);
        }

}// class

class QAbstractScrollArea___ extends QAbstractScrollArea {

        public QPaintEngine paintEngine() {
            throw new RuntimeException("Cannot open a painter directly on a QAbstractScrollArea, open QPainter on its viewport instead...");
        }

}// class

class QSplitter___ extends QSplitter {

        public static class Range {
                public Range(int min, int max) {
                    minimum = min;
                    maximum = max;
                }

                public int minimum;
                public int maximum;
        }

        public final Range getRange(int index) {
            QNativePointer min = new QNativePointer(QNativePointer.Type.Int);
            QNativePointer max = new QNativePointer(QNativePointer.Type.Int);

            getRange(index, min, max);

            return new Range(min.intValue(), max.intValue());
        }

}// class

class QAction___ extends QAction {

        private QActionGroup __rcActionGroup = null;

        /**
         * Sets the shortcut to the key sequence for the given key string. For
         * example "Ctrl+O" gives CTRL+'O'. The strings "Ctrl", "Shift", "Alt" and
         * "Meta" are recognized, as well as their translated equivalents in the
         * "QShortcut" context (using QObject::tr()). Up to four key codes may be
         * entered by separating them with commas, e.g. "Alt+X,Ctrl+S,Q".
         *
         * @param key
         *            The description of the key sequence. Typically used with tr()
         *            so key sequences can be locale aware.
         */
        public final void setShortcut(String key) {
            setShortcut(new QKeySequence(key));
        }

        /**
         * Sets the shortcut to the key sequence for the given key. The result will
         * depend on the currently running platform. The key sequence will be based
         * on the first element in the list of key bindings for the key.
         *
         * @param key
         *            The key for which to select a key sequence
         */
        public final void setShortcut(QKeySequence.StandardKey key) {
            setShortcut(new QKeySequence(key));
        }

        public final void setIcon(QPixmap pm) {
            setIcon(new QIcon(pm));
        }

}// class

class QApplication___ extends QApplication {

        public static void initialize(String args[]) {
            if (m_instance != null)
                throw new RuntimeException("QApplication can only be initialized once");

            org.qtjambi.qt.internal.HelperFunctions.setAsMainThread();

            List<String> paths = Utilities.unpackPlugins();
            if (paths != null) {
                Collections.reverse(paths);  // Qt prepends but our list is in highest priority first order
                for (String p : paths)
                    addLibraryPath(p);
            }
            org.qtjambi.qt.internal.QtJambiInternal.setupDefaultPluginPath();
            m_instance = new QApplication(args);
            m_instance.aboutToQuit.connect(m_instance, "disposeOfMyself()");
        }

        public static void initialize(String applicationName, String args[]) {
            if (m_instance != null)
                throw new RuntimeException("QApplication can only be initialized once");

            org.qtjambi.qt.internal.HelperFunctions.setAsMainThread();

            List<String> paths = Utilities.unpackPlugins();
            if (paths != null) {
                Collections.reverse(paths);  // Qt prepends but our list is in highest priority first order
                for (String p : paths)
                    addLibraryPath(p);
            }
            org.qtjambi.qt.internal.QtJambiInternal.setupDefaultPluginPath();
            m_instance = new QApplication(applicationName, args);
            m_instance.aboutToQuit.connect(m_instance, "disposeOfMyself()");
        }

        public static void shutdown() {
            org.qtjambi.qt.core.QCoreApplication.shutdown();
        }

        public static void aboutQtJambi() {
            org.qtjambi.qt.QtJambiGuiInternal.aboutQtJambi();
        }
        
        /**
         *
         * @see #execStatic()
         */
        public int exec() {
            return exec_internal();
        }

        /**
         *
         * @see #exec()
         */
        public static int execStatic() {
            if (m_instance == null)
                throw new RuntimeException("QApplication has not been initialized with QApplication.initialize()");
            return exec_internal();
        }

        public static QApplication instance() {
            if (m_instance instanceof QApplication)
                return (QApplication) m_instance;
            return null;
        }

        public QApplication(String args[]) {
            this(argc(args), argv(args));
        }

        public QApplication(String applicationName, String args[]) {
            this(argc(args), argv(applicationName, args));
        }

        public static void setFont(QFont font) {
            setFont(font, null);
        }

        public static void setPalette(QPalette palette) {
            setPalette(palette, null);
        }

}// class

class QFormLayout___ extends QFormLayout {
        /**
         * Retrieves the row and role (column) of the item at the specified index.
         * If index is out of bounds, -1 is returned in place of the row.
         *
         * @param index The index of the item for which to retrieve the position.
         * @return A pair of an int (row of item) and an ItemRole (role of the item.)
         **/
        @QtBlockedSlot
        public final org.qtjambi.qt.QPair<Integer, ItemRole> getItemPosition(int index) {
            org.qtjambi.qt.QNativePointer row = new org.qtjambi.qt.QNativePointer(org.qtjambi.qt.QNativePointer.Type.Int);
            org.qtjambi.qt.QNativePointer role = new org.qtjambi.qt.QNativePointer(org.qtjambi.qt.QNativePointer.Type.Int);

            getItemPosition(index, row, role);

            return new org.qtjambi.qt.QPair<Integer, ItemRole>(row.intValue(), ItemRole.resolve(role.intValue()));
        }

        /**
         * Retrieves the row and role (column) of the specified child layout.
         * If the layout is not in the form layout, -1 is returned in place of the row.
         *
         * @param layout The layout for which to retrieve the position.
         * @return A pair of an int (row of item) and an ItemRole (role of the item.)
         */
        @QtBlockedSlot
        public final org.qtjambi.qt.QPair<Integer, ItemRole> getLayoutPosition(QLayout layout) {
            org.qtjambi.qt.QNativePointer row = new org.qtjambi.qt.QNativePointer(org.qtjambi.qt.QNativePointer.Type.Int);
            org.qtjambi.qt.QNativePointer role = new org.qtjambi.qt.QNativePointer(org.qtjambi.qt.QNativePointer.Type.Int);

            getLayoutPosition(layout, row, role);

            return new org.qtjambi.qt.QPair<Integer, ItemRole>(row.intValue(), ItemRole.resolve(role.intValue()));
        }

        /**
         * Retrieves the row and role (column) of the specified widget in the layout.
         * If the widget is not in the form layout, -1 is returned in place of the row.
         *
         * @param widget The widget for which to retrieve the position.
         * @return A pair of an int (row of item) and an ItemRole (role of the item.)
         */
        @QtBlockedSlot
        public final org.qtjambi.qt.QPair<Integer, ItemRole> getWidgetPosition(QWidget widget) {
            org.qtjambi.qt.QNativePointer row = new org.qtjambi.qt.QNativePointer(org.qtjambi.qt.QNativePointer.Type.Int);
            org.qtjambi.qt.QNativePointer role = new org.qtjambi.qt.QNativePointer(org.qtjambi.qt.QNativePointer.Type.Int);

            getWidgetPosition(widget, row, role);

            return new org.qtjambi.qt.QPair<Integer, ItemRole>(row.intValue(), ItemRole.resolve(role.intValue()));
        }

        /**
         * Sets the item in the given row for the given role to item, extending the layout with empty rows if necessary.
         * If the cell is already occupied, the item is not inserted and a IllegalArgumentException is thrown.
         */
        @QtBlockedSlot
        public final void setItem(int row, ItemRole role, QLayoutItem item) {
            if (itemAt(row, role) == null)
                setItem_private(row, role, item);
            else
                throw new IllegalArgumentException("Cell in form layout is already occupied");
        }

}// class

class Subclass_of_QGraphicsLayoutItem___ extends QGraphicsLayoutItem {
        @QtBlockedSlot
        public final org.qtjambi.qt.core.QMarginsF contentsMargins() {
            QNativePointer left = new QNativePointer(QNativePointer.Type.Double);
            QNativePointer top = new QNativePointer(QNativePointer.Type.Double);
            QNativePointer right = new QNativePointer(QNativePointer.Type.Double);
            QNativePointer bottom = new QNativePointer(QNativePointer.Type.Double);

            getContentsMargins(left, top, right, bottom);
            return new org.qtjambi.qt.core.QMarginsF(left.doubleValue(), top.doubleValue(), right.doubleValue(), bottom.doubleValue());
        }
}// class

class QGraphicsWidget___ extends QGraphicsWidget {

        /**
         * Gets the widget's window frame margins.
         *
         * @returns An object containing the margins in left, right, top and bottom coordinates.
         */
        @QtBlockedSlot
        public final org.qtjambi.qt.core.QMarginsF windowFrameMargins() {
            QNativePointer left = new QNativePointer(QNativePointer.Type.Double);
            QNativePointer top = new QNativePointer(QNativePointer.Type.Double);
            QNativePointer right = new QNativePointer(QNativePointer.Type.Double);
            QNativePointer bottom = new QNativePointer(QNativePointer.Type.Double);

            getWindowFrameMargins(left, top, right, bottom);
            return new org.qtjambi.qt.core.QMarginsF(left.doubleValue(), top.doubleValue(), right.doubleValue(), bottom.doubleValue());
        }
}// class

class QListWidget___ extends QListWidget {

        /**
         * Inserts an item with the text label at the end of the list widget.
         *
         * @param label The text of the new item.
         */
        public final void addItem(String label) {
            addItem(new QListWidgetItem(label, this));
        }

        /**
         * Inserts items with the text labels at the end of the list widget.
         *
         * @param labels The collection of labels for the new items.
         */
        public final void addItems(java.util.List<String> labels) {
        for (String label : labels)
                addItem(label);
        }

        /**
         * This is an overloaded member function, provided for convenience.
         * Inserts an item with the text label in the list widget at the position given by row.
         *
         * @param row The position in which to insert the new item.
         * @param label The text of the new item.
         */
        public final void insertItem(int row, String label) {
            insertItem(row, new QListWidgetItem(label, this));
        }

        /**
         * Inserts items from the list of labels into the list, starting at the given row.
         *
         * @param row The position in which to insert the new items.
         * @param labels The collection of labels for the new items.
         */
        public final void insertItems(int row, java.util.List<String> labels) {
        for (String label : labels)
                insertItem(row++, label);
        }
}// class


class QGraphicsScene___ extends QGraphicsScene {
        /**
         * Equivalent to addEllipse(rect, null, null)
         */
        public final QGraphicsEllipseItem addEllipse(org.qtjambi.qt.core.QRectF rect) {
            return addEllipse(rect, null);
        }

        /**
         * Equivalent to addEllipse(rect, pen, null)
         */
        public final QGraphicsEllipseItem addEllipse(org.qtjambi.qt.core.QRectF rect, QPen pen) {
            return addEllipse(rect, pen, null);
        }


        /**
         * Creates and adds an ellipse item to the scene, and returns the item
         * reference. The geometry of the ellipse is defined by rect, and its pen
         * and brush are initialized to pen and brush.
         *
         * Note that the item's geometry is provided in item coordinates, and its
         * position is initialized to (0, 0).
         *
         * If the item is visible (i.e., QGraphicsItem::isVisible() returns true),
         * QGraphicsScene will emit changed() once control goes back to the event
         * loop.
         *
         * @param rect The bounding rectangle of the ellipse.
         * @param pen The pen for the resulting QGraphicsEllipseItem.
         * @param brush The brush for the resulting QGraphicsEllipseItem.
         * @return The resulting item.
         */
        public final QGraphicsEllipseItem addEllipse(org.qtjambi.qt.core.QRectF rect, QPen pen, QBrush brush) {
            return addEllipse(rect.x(), rect.y(), rect.width(), rect.height(), pen, brush);
        }

        /**
         * Equivalent to addEllipse(x, y, w, h, null, null)
         */
        public final QGraphicsEllipseItem addEllipse(double x, double y, double w, double h) {
            return addEllipse(x, y, w, h, null);
        }

        /**
         * Equivalent to addEllipse(x, y, w, h, pen, null)
         */
        public final QGraphicsEllipseItem addEllipse(double x, double y, double w, double h, QPen pen) {
            return addEllipse(x, y, w, h, pen, null);
        }

        /**
         * Creates and adds an ellipse item to the scene, and returns the item
         * reference. The geometry of the ellipse is defined by x, y, w, h, and its pen
         * and brush are initialized to pen and brush.
         *
         * Note that the item's geometry is provided in item coordinates, and its
         * position is initialized to (0, 0).
         *
         * If the item is visible (i.e., QGraphicsItem.isVisible() returns true),
         * QGraphicsScene will emit changed once control goes back to the event
         * loop.
         *
         * @param x The left horizontal coordinate of the ellipse's bounding rectangle.
         * @param y The top vertical coordinate of the ellipse's bounding rectangle.
         * @param w The width of the ellipse's bounding rectangle.
         * @param h The height of the ellipse's bounding rectangle.
         * @param pen The pen for the resulting QGraphicsEllipseItem.
         * @param brush The brush for the resulting QGraphicsEllipseItem.
         * @return The resulting item.
         */
        public final QGraphicsEllipseItem addEllipse(double x, double y, double w, double h, QPen pen, QBrush brush) {
            QGraphicsEllipseItem item = new QGraphicsEllipseItem(x, y, w, h);
            item.setPen(pen);
            item.setBrush(brush);

            addItem(item);

            return item;
        }

        /**
         * Equivalent to addLine(line, null)
         */
        public final QGraphicsLineItem addLine(QLineF line) {
            return addLine(line, null);
        }

        /**
         * Creates and adds a line item to the scene, and returns the item
         * reference. The geometry of the line is defined by line, and its pen
         * is initialized to pen.
         *
         * Note that the item's geometry is provided in item coordinates, and its
         * position is initialized to (0, 0).
         *
         * If the item is visible (i.e., QGraphicsItem.isVisible() returns true),
         * QGraphicsScene will emit changed once control goes back to the event
         * loop.
         *
         * @param line The definition of the line.
         * @param pen The pen with which to draw the line.
         * @return The resulting item.
         */
        public final QGraphicsLineItem addLine(QLineF line, QPen pen) {
            return addLine(line.x1(), line.y1(), line.x2(), line.y2(), pen);
        }

        /**
         * Equivalent to addLine(x1, y1, x2, y2, null)
         */
        public final QGraphicsLineItem addLine(double x1, double y1, double x2, double y2) {
            return addLine(x1, y1, x2, y2, null);
        }

        /**
         * Creates and adds a line item to the scene, and returns the item
         * reference. The geometry of the line is defined by x1, y1, x2, y2, and its pen
         * is initialized to pen.
         *
         * Note that the item's geometry is provided in item coordinates, and its
         * position is initialized to (0, 0).
         *
         * If the item is visible (i.e., QGraphicsItem.isVisible() returns true),
         * QGraphicsScene will emit changed once control goes back to the event
         * loop.
         *
         * @param x1 The first horizontal coordinate of the line.
         * @param y1 The first vertical coordinate of the line.
         * @param x2 The second horizontal coordinate of the line.
         * @param y2 The second vertical coordinate of the line.
         * @param pen The pen with which to draw the line.
         * @return The resulting item.
         */
        public final QGraphicsLineItem addLine(double x1, double y1, double x2, double y2, QPen pen) {
            QGraphicsLineItem item = new QGraphicsLineItem(x1, y1, x2, y2);
            item.setPen(pen);

            addItem(item);

            return item;
        }


        /**
         * Equivalent to addPath(path, null, null)
         */
        public final QGraphicsPathItem addPath(QPainterPath path) {
            return addPath(path, null);
        }

        /**
         * Equivalent to addPath(path, pen, null)
         */
        public final QGraphicsPathItem addPath(QPainterPath path, QPen pen) {
            return addPath(path, pen, null);
        }
        /**
         * Creates and adds a path item to the scene, and returns the item
         * reference. The geometry of the path is defined by path, and its pen and
         * brush are initialized to pen and brush.
         *
         * Note that the item's geometry is provided in item coordinates, and its
         * position is initialized to (0, 0).
         *
         * If the item is visible (i.e., QGraphicsItem.isVisible() returns true),
         * QGraphicsScene will emit changed once control goes back to the event
         * loop.
         *
         * @param path The definition of the path.
         * @param pen The pen for drawing the path.
         * @param brush The brush for drawing the path.
         * @return The resulting item.
         */
        public final QGraphicsPathItem addPath(QPainterPath path, QPen pen, QBrush brush) {
            QGraphicsPathItem item = new QGraphicsPathItem(path);
            item.setPen(pen);
            item.setBrush(brush);

            addItem(item);

            return item;
        }

        /**
         * Creates and adds a pixmap item to the scene, and returns the item
         * reference. The pixmap is defined by pixmap.
         *
         * Note that the item's geometry is provided in item coordinates, and its
         * position is initialized to (0, 0).
         *
         * If the item is visible (i.e., QGraphicsItem.isVisible() returns true),
         * QGraphicsScene will emit changed once control goes back to the event
         * loop.
         *
         * @param pixmap The pixmap for which to create a graphics item.
         * @return The resulting item.
         */
        public final QGraphicsPixmapItem addPixmap(QPixmap pixmap) {
            QGraphicsPixmapItem item = new QGraphicsPixmapItem(pixmap);

            addItem(item);

            return item;
        }

        /**
         * Equivalent to addPolygon(polygon, null, null)
         */
        public final QGraphicsPolygonItem addPolygon(QPolygonF polygon) {
            return addPolygon(polygon, null);
        }

        /**
         * Equivalent to addPolygon(polygon, pen, null)
         */
        public final QGraphicsPolygonItem addPolygon(QPolygonF polygon, QPen pen) {
            return addPolygon(polygon, pen, null);
        }

        /**
         * Creates and adds a polygon item to the scene, and returns the item
         * reference. The polygon is defined by polygon, and its pen and
         * brush are initialized to pen and brush.
         *
         * Note that the item's geometry is provided in item coordinates, and its
         * position is initialized to (0, 0).
         *
         * If the item is visible (i.e., QGraphicsItem.isVisible() returns true),
         * QGraphicsScene will emit changed once control goes back to the event
         * loop.
         *
         * @param polygon The definition of the polygon.
         * @param pen The pen with which to draw the polygon.
         * @param brush The brush with which to draw the polygon.
         * @return The resulting item.
         */
        public final QGraphicsPolygonItem addPolygon(QPolygonF polygon, QPen pen, QBrush brush) {
            QGraphicsPolygonItem item = new QGraphicsPolygonItem(polygon);
            item.setPen(pen);
            item.setBrush(brush);

            addItem(item);

            return item;
        }

        /**
         * Equivalent to addRect(rect, null, null)
         */
        public final QGraphicsRectItem addRect(org.qtjambi.qt.core.QRectF rect) {
            return addRect(rect, null);
        }

        /**
         * Equivalent to addRect(rect, pen, null)
         */
        public final QGraphicsRectItem addRect(org.qtjambi.qt.core.QRectF rect, QPen pen) {
            return addRect(rect, pen, null);
        }

        /**
         * Creates and adds a rectangle item to the scene, and returns the item
         * reference. The geometry of the rectangle is defined by rect, and its pen
         * and brush are initialized to pen and brush.
         *
         * Note that the item's geometry is provided in item coordinates, and its
         * position is initialized to (0, 0). For example, if a QRect(50, 50, 100,
         * 100) is added, its top-left corner will be at (50, 50) relative to the
         * origin in the item's coordinate system.
         *
         * If the item is visible (i.e., QGraphicsItem.isVisible() returns true),
         * QGraphicsScene will emit changed once control goes back to the event
         * loop.
         *
         * @param rect The definition of the rectangle.
         * @param pen The pen with which to draw the rectangle.
         * @param brush The brush with which to draw the rectangle.
         * @return The resulting item.
         */
        public final QGraphicsRectItem addRect(org.qtjambi.qt.core.QRectF rect, QPen pen, QBrush brush) {
            return addRect(rect.x(), rect.y(), rect.width(), rect.height(), pen, brush);
        }

        /**
         * Equivalent to addRect(x, y, w, h, null, null)
         */
        public final QGraphicsRectItem addRect(double x, double y, double w, double h) {
            return addRect(x, y, w, h, null);
        }

        /**
         * Equivalent to addRect(x, y, w, h, pen, null)
         */
        public final QGraphicsRectItem addRect(double x, double y, double w, double h, QPen pen) {
            return addRect(x, y, w, h, pen, null);
        }

        /**
         * Creates and adds a rectangle item to the scene, and returns the item
         * reference. The geometry of the rectangle is defined by x, y, w, h, and its pen
         * and brush are initialized to pen and brush.
         *
         * Note that the item's geometry is provided in item coordinates, and its
         * position is initialized to (0, 0).
         *
         * If the item is visible (i.e., QGraphicsItem.isVisible() returns true),
         * QGraphicsScene will emit changed once control goes back to the event
         * loop.
         *
         * @param x The left horizontal coordinate of the rectangle.
         * @param y The top vertical coordinate of the rectangle.
         * @param w The width of the rectangle.
         * @param h The height of the rectangle.
         * @param pen The pen with which to draw the rectangle.
         * @param brush The brush with which to draw the rectangle.
         * @return The resulting item.
         */
        public final QGraphicsRectItem addRect(double x, double y, double w, double h, QPen pen, QBrush brush) {
            QGraphicsRectItem item = new QGraphicsRectItem(x, y, w, h);
            item.setPen(pen);
            item.setBrush(brush);

            addItem(item);

            return item;
        }

        /**
         * Equivalent to addSimpleText(text, null)
         */
        public final QGraphicsSimpleTextItem addSimpleText(String text) {
            return addSimpleText(text, null);
        }

        /**
         * Creates and adds a QGraphicsSimpleTextItem to the scene, and returns the
         * item reference. The text string is initialized to text, and its font is
         * initialized to font.
         *
         * The item's position is initialized to (0, 0).
         *
         * If the item is visible (i.e., QGraphicsItem.isVisible() returns true),
         * QGraphicsScene will emit changed once control goes back to the event
         * loop.
         *
         * @param text The text to add to the scene.
         * @param font The font to use for displaying the text.
         * @return The resulting item.
         */
        public final QGraphicsSimpleTextItem addSimpleText(String text, QFont font) {
            QGraphicsSimpleTextItem item = new QGraphicsSimpleTextItem(text);
            item.setFont(font);

            addItem(item);

            return item;
        }

        /**
         * Equivalent to addText(text, null)
         */
        public final QGraphicsTextItem addText(String text) {
            return addText(text, null);
        }

        /**
         * Creates and adds a text item to the scene, and returns the item
         * reference. The text string is initialized to text, and its font
         * is initialized to font.
         *
         * The item's position is initialized to (0, 0).
         *
         * If the item is visible (i.e., QGraphicsItem.isVisible() returns true),
         * QGraphicsScene will emit changed once control goes back to the event
         * loop.
         *
         * @param text The text to add to the scene.
         * @param font The font to use for displaying the text.
         * @return The resulting item.
         */
        public final QGraphicsTextItem addText(String text, QFont font) {
            QGraphicsTextItem item = new QGraphicsTextItem(text);
            item.setFont(font);

            addItem(item);

            return item;
        }

}// class


class QInputDialog___{

    public static final Integer getInt(QWidget parent, String title, String label, int value, int min, int max, int step, org.qtjambi.qt.core.Qt.WindowFlags flags)
    {
        QNativePointer ok = new QNativePointer(QNativePointer.Type.Boolean);

        int result = getInt_private(parent, title, label, value, min, max, step, ok, flags);

        return ok.booleanValue() ? result : null;

    }

    public static final Integer getInt(QWidget parent, String title, String label, int value, int min, int max, int step, org.qtjambi.qt.core.Qt.WindowType[] f)
    {
        QNativePointer ok = new QNativePointer(QNativePointer.Type.Boolean);

        int result = getInt_private(parent, title, label, value, min, max, step, ok, f);

        return ok.booleanValue() ? result : null;
    }

    public static final Integer getInt(QWidget parent, String title, String label, int value, int min, int max, int step)
    {
        return getInt(parent, title, label, value, min, max, step, new org.qtjambi.qt.core.Qt.WindowFlags(0));
    }

    public static final Integer getInt(QWidget parent, String title, String label, int value, int min, int max)
    {
        return getInt(parent, title, label, value, min, max, 1);
    }

    public static final Integer getInt(QWidget parent, String title, String label, int value)
    {
        return getInt(parent, title, label, value, (int) -2147483647, (int) 2147483647);
    }

    public static final Integer getInt(QWidget parent, String title, String label)
    {
        return getInt(parent, title, label, 0);
    }

    public static Double getDouble(org.qtjambi.qt.widgets.QWidget parent, java.lang.String title, java.lang.String label, double value, double minValue, double maxValue) {
        return getDouble(parent, title, label, value, minValue, maxValue, 1);
    }

    public static Double getDouble(org.qtjambi.qt.widgets.QWidget parent, java.lang.String title, java.lang.String label, double value, double minValue) {
        return getDouble(parent, title, label, value, minValue, (double)2147483647);
    }


    public static Double getDouble(org.qtjambi.qt.widgets.QWidget parent, java.lang.String title, java.lang.String label, double value) {
        return getDouble(parent, title, label, value, (double)-2147483647);
    }

    public static Double getDouble(org.qtjambi.qt.widgets.QWidget parent, java.lang.String title, java.lang.String label) {
        return getDouble(parent, title, label, 0.0);
    }
    
    public static Double getDouble(org.qtjambi.qt.widgets.QWidget parent, java.lang.String title, java.lang.String label, double value, double minValue, double maxValue, int decimals, org.qtjambi.qt.core.Qt.WindowFlags f) {
        org.qtjambi.qt.QNativePointer ok = new org.qtjambi.qt.QNativePointer(org.qtjambi.qt.QNativePointer.Type.Boolean);
        Double result = getDouble_internal(parent, title, label, value, minValue, maxValue, decimals, ok, f);
        return ok.booleanValue() ? result : null;
    }

    public static Double getDouble(org.qtjambi.qt.widgets.QWidget parent, java.lang.String title, java.lang.String label, double value, double minValue, double maxValue, int decimals, org.qtjambi.qt.core.Qt.WindowType ... f) {
        org.qtjambi.qt.QNativePointer ok = new org.qtjambi.qt.QNativePointer(org.qtjambi.qt.QNativePointer.Type.Boolean);
        Double result = getDouble_internal(parent, title, label, value, minValue, maxValue, decimals, ok, f);
        return ok.booleanValue() ? result : null;
    }

    public static Double getDouble(org.qtjambi.qt.widgets.QWidget parent, java.lang.String title, java.lang.String label, double value, double minValue, double maxValue, int decimals) {
        return getDouble(parent, title, label, value, minValue, maxValue, decimals, org.qtjambi.qt.core.Qt.WindowType.Widget);
    }

    public static java.lang.String getItem(org.qtjambi.qt.widgets.QWidget parent, java.lang.String title, java.lang.String label, java.util.List<java.lang.String> list, int current) {
        return getItem(parent, title, label, list, current, true);
    }

    public static java.lang.String getItem(org.qtjambi.qt.widgets.QWidget parent, java.lang.String title, java.lang.String label, java.util.List<java.lang.String> list) {
        return getItem(parent, title, label, list, 0);
    }

    public static java.lang.String getText(org.qtjambi.qt.widgets.QWidget parent, java.lang.String title, java.lang.String label, org.qtjambi.qt.widgets.QLineEdit.EchoMode echo) {
        return getText(parent, title, label, echo, "");
    }

    public static java.lang.String getText(org.qtjambi.qt.widgets.QWidget parent, java.lang.String title, java.lang.String label) {
        return getText(parent, title, label, org.qtjambi.qt.widgets.QLineEdit.EchoMode.Normal);
    }

    public static java.lang.String getMultiLineText(org.qtjambi.qt.widgets.QWidget parent, java.lang.String title, java.lang.String label) {
        return getMultiLineText(parent, title, label, "");
    }
    
    public static String getItem(org.qtjambi.qt.widgets.QWidget parent, java.lang.String title, java.lang.String label, java.util.List<java.lang.String> list, int current, boolean editable, org.qtjambi.qt.core.Qt.WindowFlags f) {
        org.qtjambi.qt.QNativePointer ok = new org.qtjambi.qt.QNativePointer(org.qtjambi.qt.QNativePointer.Type.Boolean);
        String result = getItem_internal(parent, title, label, list, current, editable, ok, f);
        return ok.booleanValue() ? result : null;
    }
    
    public static String getItem(org.qtjambi.qt.widgets.QWidget parent, java.lang.String title, java.lang.String label, java.util.List<java.lang.String> list, int current, boolean editable, org.qtjambi.qt.core.Qt.WindowType ... f) {
        org.qtjambi.qt.QNativePointer ok = new org.qtjambi.qt.QNativePointer(org.qtjambi.qt.QNativePointer.Type.Boolean);
        String result = getItem_internal(parent, title, label, list, current, editable, ok, new org.qtjambi.qt.core.Qt.WindowFlags(f));
        return ok.booleanValue() ? result : null;
    }
    
    public static String getItem(org.qtjambi.qt.widgets.QWidget parent, java.lang.String title, java.lang.String label, java.util.List<java.lang.String> list, int current, boolean editable, org.qtjambi.qt.core.Qt.WindowFlags f, org.qtjambi.qt.core.Qt.InputMethodHint ... inputMethodHints) {
        org.qtjambi.qt.QNativePointer ok = new org.qtjambi.qt.QNativePointer(org.qtjambi.qt.QNativePointer.Type.Boolean);
        String result = getItem_internal(parent, title, label, list, current, editable, ok, f, inputMethodHints);
        return ok.booleanValue() ? result : null;
    }

    public static String getItem(org.qtjambi.qt.widgets.QWidget parent, java.lang.String title, java.lang.String label, java.util.List<java.lang.String> list, int current, boolean editable, org.qtjambi.qt.core.Qt.WindowFlags f, org.qtjambi.qt.core.Qt.InputMethodHints inputMethodHints) {
        org.qtjambi.qt.QNativePointer ok = new org.qtjambi.qt.QNativePointer(org.qtjambi.qt.QNativePointer.Type.Boolean);
        String result = getItem_internal(parent, title, label, list, current, editable, ok, f, inputMethodHints);
        return ok.booleanValue() ? result : null;
    }

    public static String getItem(org.qtjambi.qt.widgets.QWidget parent, java.lang.String title, java.lang.String label, java.util.List<java.lang.String> list, int current, boolean editable) {
        return getItem(parent, title, label, list, current, editable, org.qtjambi.qt.core.Qt.WindowType.Widget);
    }
    
    public static String getText(org.qtjambi.qt.widgets.QWidget parent, java.lang.String title, java.lang.String label, org.qtjambi.qt.widgets.QLineEdit.EchoMode echo, java.lang.String text, org.qtjambi.qt.core.Qt.WindowFlags f) {
        org.qtjambi.qt.QNativePointer ok = new org.qtjambi.qt.QNativePointer(org.qtjambi.qt.QNativePointer.Type.Boolean);
        String result = getText_internal(parent, title, label, echo, text, ok, f);
        return ok.booleanValue() ? result : null;
    }
    
    public static String getText(org.qtjambi.qt.widgets.QWidget parent, java.lang.String title, java.lang.String label, org.qtjambi.qt.widgets.QLineEdit.EchoMode echo, java.lang.String text, org.qtjambi.qt.core.Qt.WindowType ... f) {
        org.qtjambi.qt.QNativePointer ok = new org.qtjambi.qt.QNativePointer(org.qtjambi.qt.QNativePointer.Type.Boolean);
        String result = getText_internal(parent, title, label, echo, text, ok, new org.qtjambi.qt.core.Qt.WindowFlags(f));
        return ok.booleanValue() ? result : null;    	
    }

    public static String getText(org.qtjambi.qt.widgets.QWidget parent, java.lang.String title, java.lang.String label, org.qtjambi.qt.widgets.QLineEdit.EchoMode echo, java.lang.String text, org.qtjambi.qt.core.Qt.WindowFlags f, org.qtjambi.qt.core.Qt.InputMethodHints inputMethodHints) {
        org.qtjambi.qt.QNativePointer ok = new org.qtjambi.qt.QNativePointer(org.qtjambi.qt.QNativePointer.Type.Boolean);
        String result = getText_internal(parent, title, label, echo, text, ok, f, inputMethodHints);
        return ok.booleanValue() ? result : null;
    }

    public static String getText(org.qtjambi.qt.widgets.QWidget parent, java.lang.String title, java.lang.String label, org.qtjambi.qt.widgets.QLineEdit.EchoMode echo, java.lang.String text, org.qtjambi.qt.core.Qt.WindowFlags f, org.qtjambi.qt.core.Qt.InputMethodHint ... inputMethodHints) {
        org.qtjambi.qt.QNativePointer ok = new org.qtjambi.qt.QNativePointer(org.qtjambi.qt.QNativePointer.Type.Boolean);
        String result = getText_internal(parent, title, label, echo, text, ok, f, inputMethodHints);
        return ok.booleanValue() ? result : null;
    }

    public static String getText(org.qtjambi.qt.widgets.QWidget parent, java.lang.String title, java.lang.String label, org.qtjambi.qt.widgets.QLineEdit.EchoMode echo, java.lang.String text) {
        return getText(parent, title, label, echo, text, org.qtjambi.qt.core.Qt.WindowType.Widget);
    }
    
    public static String getMultiLineText(org.qtjambi.qt.widgets.QWidget parent, java.lang.String title, java.lang.String label, java.lang.String text, org.qtjambi.qt.core.Qt.WindowFlags f) {
        org.qtjambi.qt.QNativePointer ok = new org.qtjambi.qt.QNativePointer(org.qtjambi.qt.QNativePointer.Type.Boolean);
        String result = getMultiLineText_internal(parent, title, label, text, ok, f);
        return ok.booleanValue() ? result : null;
    }
    
    public static String getMultiLineText(org.qtjambi.qt.widgets.QWidget parent, java.lang.String title, java.lang.String label, java.lang.String text, org.qtjambi.qt.core.Qt.WindowType ... f) {
        org.qtjambi.qt.QNativePointer ok = new org.qtjambi.qt.QNativePointer(org.qtjambi.qt.QNativePointer.Type.Boolean);
        String result = getMultiLineText_internal(parent, title, label, text, ok, new org.qtjambi.qt.core.Qt.WindowFlags(f));
        return ok.booleanValue() ? result : null;    	
    }

    public static String getMultiLineText(org.qtjambi.qt.widgets.QWidget parent, java.lang.String title, java.lang.String label, java.lang.String text, org.qtjambi.qt.core.Qt.WindowFlags f, org.qtjambi.qt.core.Qt.InputMethodHints inputMethodHints) {
        org.qtjambi.qt.QNativePointer ok = new org.qtjambi.qt.QNativePointer(org.qtjambi.qt.QNativePointer.Type.Boolean);
        String result = getMultiLineText_internal(parent, title, label, text, ok, f, inputMethodHints);
        return ok.booleanValue() ? result : null;
    }

    public static String getMultiLineText(org.qtjambi.qt.widgets.QWidget parent, java.lang.String title, java.lang.String label, java.lang.String text, org.qtjambi.qt.core.Qt.WindowFlags f, org.qtjambi.qt.core.Qt.InputMethodHint ... inputMethodHints) {
        org.qtjambi.qt.QNativePointer ok = new org.qtjambi.qt.QNativePointer(org.qtjambi.qt.QNativePointer.Type.Boolean);
        String result = getMultiLineText_internal(parent, title, label, text, ok, f, inputMethodHints);
        return ok.booleanValue() ? result : null;
    }

    public static String getMultiLineText(org.qtjambi.qt.widgets.QWidget parent, java.lang.String title, java.lang.String label, java.lang.String text) {
        return getMultiLineText(parent, title, label, text, org.qtjambi.qt.core.Qt.WindowType.Widget);
    }

}// class



