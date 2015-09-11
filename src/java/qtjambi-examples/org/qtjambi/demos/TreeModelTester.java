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

package org.qtjambi.demos;

import java.util.ArrayList;
import java.util.List;

import org.qtjambi.qt.core.QMimeData;
import org.qtjambi.qt.core.QModelIndex;
import org.qtjambi.qt.core.QObject;
import org.qtjambi.qt.core.Qt;
import org.qtjambi.qt.gui.QTreeModel;
import org.qtjambi.qt.widgets.QAbstractItemView;
import org.qtjambi.qt.widgets.QAction;
import org.qtjambi.qt.widgets.QApplication;
import org.qtjambi.qt.widgets.QTreeView;
import org.qtjambi.qt.widgets.QWidget;

/**
 * A pretty basic node implementation...
 */
class Node implements Cloneable {

    public Node(String s, Model model, Node parent) {
        this.text = s;
        this.model = model;
        this.parent = parent;
    }

    @Override
    public String toString() { return text + ":" + counter; }

    public Object clone() {
        Node newNode = new Node(text, model, parent);
        newNode.counter = counter;

        for (Node n : children)
            newNode.children.add((Node) n.clone());

        return newNode;
    }

    public boolean isChildOf(Node parent) {
        Node node = this;
        while (node != null) {
            if (node == parent)
                return true;
            else
                node = node.parent;
        }
        return false;
    }

    List<Node> children = new ArrayList<Node>();
    String text;
    int counter;
    Model model;
    Node parent;
}

class NodeRefMimeData extends QMimeData
{
    public NodeRefMimeData(QObject parent) { }
    public Node node;

    public String toString() {
        return "NodeRefMimeData(" + node.toString() + ")";
    }
}

/**
 * An example model implementation. It reimplements child(), childCount() and text() to
 * represent the data in a tree of Node's
 */
class Model extends QTreeModel {

    public Model() {
        Node child1 = new Node("Shortcuts", this, root);
        Node grandChild11 = new Node("Ctrl+A - Add a subnode", this, child1);
        Node grandChild12 = new Node("Ctrl+R - Remove the node", this, child1);
        Node grandChild13 = new Node("Ctrl+I - Increment the numerical value of the node", this, child1);
        Node grandChild14 = new Node("Ctrl+S - Swap the node with its next sibling", this, child1);
        Node child2 = new Node("Other Features", this, root);
        Node grandChild21 = new Node("Internal Drag & Drop enabled", this, child2);
        Node grandChild22 = new Node("Ctrl on Mac OS X is really Command...", this, child2);

        root.children.add(child1);
          child1.children.add(grandChild11);
          child1.children.add(grandChild12);
          child1.children.add(grandChild13);
          child1.children.add(grandChild14);
        root.children.add(child2);
          child2.children.add(grandChild21);
          child2.children.add(grandChild22);

    }

    /**
     * Called to query the child of parent at index. If parent is null we have only one child,
     * the root.
     */
    @Override
    public Object child(Object parent, int index) {
        if (parent == null)
            return root;
        return ((Node) parent).children.get(index);
    }

    /**
     * Called to query the number of children of the given object or the number of root objects if
     * parent is null.
     */
    @Override
    public int childCount(Object parent) {
        int count = parent == null ? 1 : ((Node) parent).children.size();
        return count;
    }

    /**
     * Convenience virtual function to get the textual value of an object. I could also
     * implement icon() for pixmap data or the data() function for other types of roles.
     */
    @Override
    public String text(Object value) {
        return "" + value;
    }

    public Qt.ItemFlags flags(QModelIndex index) {
        return defaultFlags;
    }

    /**
     * We implement this to indicate which mimetypes we support...
     */
    public List<String> mimeTypes() {
        List<String> types = new ArrayList<String>();
        types.add("text/plain");
        return types;
    }

    public QMimeData mimeData(List<QModelIndex> list) {
        if (list.size() > 0) {
            Node node = (Node) indexToValue(list.get(0));
            NodeRefMimeData data = new NodeRefMimeData(this);
            data.node = node;
            data.setText(node.toString());
            return data;
        }
        return null;
    }

    public boolean dropMimeData(QMimeData data, Qt.DropAction action,
                                int row, int col, QModelIndex parentIndex) {
        if (data instanceof NodeRefMimeData) {

            NodeRefMimeData nodeData = (NodeRefMimeData) data;
            Node parent = (Node) indexToValue(parentIndex);
            if (parent == null)
                return false;
            Node child = nodeData.node;

            // Copy...
//             Node cloned = (Node) child.clone();
//             cloned.parent = parent;
//             int pos = parent.children.size();
//             parent.children.add(cloned);
//             childrenInserted(valueToIndex(parent), pos, pos);
//             return true;


            // Move

            if (parent.isChildOf(child)) {
                System.out.println("Cannot move parent into child...\n");
                return false;
            }

            Node oldParent = child.parent;
            int oldPos = oldParent.children.indexOf(child);
            oldParent.children.remove(child);
            childrenRemoved(valueToIndex(oldParent), oldPos, oldPos);
            int newPos = parent.children.size();
            parent.children.add(child);
            childrenInserted(valueToIndex(parent), newPos, newPos);
            return true;

        }

        return false;
    }

    public Node root() { return root; }

    private Node root = new Node("Root", this, null);

    private static final Qt.ItemFlags defaultFlags
        = new Qt.ItemFlags(Qt.ItemFlag.ItemIsDragEnabled,
                           Qt.ItemFlag.ItemIsDropEnabled,
                           Qt.ItemFlag.ItemIsSelectable,
                           Qt.ItemFlag.ItemIsEnabled);
}

/**
 * A simple test application. It adds four actions, "add", "remove" and
 * "increment" and "swap" to test that we can add, remove and change the
 * value of nodes in the tree.
 */
public class TreeModelTester extends QTreeView {

    public TreeModelTester() {
        this(null);
    }

    public TreeModelTester(QWidget parent) {
        super(parent);

        setModel(model);

        QAction add = new QAction(this);
        add.setShortcut("Ctrl+A");
        add.triggered.connect(this, "add()");

        QAction remove = new QAction(this);
        remove.setShortcut("Ctrl+R");
        remove.triggered.connect(this, "remove()");

        QAction increment = new QAction(this);
        increment.setShortcut("Ctrl+I");
        increment.triggered.connect(this, "increment()");

        QAction swap = new QAction(this);
        swap.setShortcut("Ctrl+S");
        swap.triggered.connect(this, "swap()");

        addAction(add);
        addAction(remove);
        addAction(increment);
        addAction(swap);

        setDragEnabled(true);
        setAcceptDrops(true);
        setDragDropMode(QAbstractItemView.DragDropMode.DragDrop);
    }

    public void setModel(Model model) {
        this.model = model;
        super.setModel(model);
    }


    private void swap() {
        List<QModelIndex> pos = selectedIndexes();
        for (QModelIndex i : pos) {
            Node me = (Node) model.indexToValue(i);

            Node parent = me.parent;

            if (me.parent != null) {
                int mepos = parent.children.indexOf(me);
                if (mepos < parent.children.size() - 1) {
                    Node other = parent.children.get(mepos);

                    QModelIndex parentIndex = model.valueToIndex(parent);

                    parent.children.remove(me);
                    model.childrenRemoved(parentIndex, mepos, mepos);

                    parent.children.add(mepos + 1, me);
                    model.childrenInserted(parentIndex, mepos + 1, mepos + 1);

                } else {
                    System.out.println("cannot swap last element...");
                }

            } else {
                System.out.println("cannot swap the root node..");
            }
        }
    }


    private void add() {
        List<QModelIndex> pos = selectedIndexes();
        for (QModelIndex i : pos) {
            Node n = (Node) model.indexToValue(i);
            assert n != null;

            int size = n.children.size();

            Node child = new Node(names[(int) (Math.random() * names.length)], model, n);
            n.children.add(child);

            model.childrenInserted(i, size, size);
        }
    }

    @SuppressWarnings("unused")
    private void remove() {
        List<QModelIndex> pos = selectedIndexes();
        for (QModelIndex i : pos) {
            Node n = (Node) model.indexToValue(i);
            assert n != null;

            if (n == model.root())
                continue;

            QModelIndex parentIndex = model.parent(i);
            Node parent = (Node) model.indexToValue(parentIndex);

            parent.children.remove(n);
            model.childrenRemoved(parentIndex, i.row(), i.row());
        }
    }

    @SuppressWarnings("unused")
    private void increment() {
        List<QModelIndex> pos = selectedIndexes();
        for (QModelIndex i : pos) {
            Node n = (Node) model.indexToValue(i);
            n.counter++;

            model.dataChanged.emit(i, i);
        }
    }

    private Model model = new Model();
    private String names[] = { "Alpha", "Beta", "Gamma", "Delta" };

    public static void main(String args[]) {
        QApplication.initialize(args);
        TreeModelTester w = new TreeModelTester();
        w.setWindowTitle("Tree Model Tester");
        w.show();

        if (args.length > 0 && args[0].equals("--extra-view")) {
            for (int i=0; i<3; ++i) {
                TreeModelTester t = new TreeModelTester();
                t.setWindowTitle("Extra view: " + (i+1));
                t.disableGarbageCollection();
                t.show();
                t.setModel(w.model);
            }
        }

        QApplication.execStatic();
        QApplication.shutdown();
    }
}
