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
package io.qt.uic;

import java.util.Arrays;
import java.util.Set;
import java.util.TreeSet;

import io.qt.uic.ui4.DomAction;
import io.qt.uic.ui4.DomActionGroup;
import io.qt.uic.ui4.DomActionRef;
import io.qt.uic.ui4.DomCustomWidget;
import io.qt.uic.ui4.DomLayout;
import io.qt.uic.ui4.DomProperty;
import io.qt.uic.ui4.DomSpacer;
import io.qt.uic.ui4.DomUI;
import io.qt.uic.ui4.DomWidget;

/**
 * @hidden
 */
public abstract class WriteIncludesBase extends TreeWalker{
	public WriteIncludesBase(Uic uic) {
		this.m_uic = uic;
	}
	
	public void acceptUI(DomUI node)
	{
	    m_knownClasses.clear();
	    m_laidOut = false;

	    if (node.elementIncludes()!=null)
	        acceptIncludes(node.elementIncludes());

	    // Populate known custom widgets first
	    if (node.elementCustomWidgets()!=null)
	        super.acceptCustomWidgets(node.elementCustomWidgets());

	    if (node.elementButtonGroups()!=null)
	        add("QButtonGroup");

	    super.acceptUI(node);
	}

	public void acceptWidget(DomWidget node)
	{
	    add(node.attributeClass());
	    super.acceptWidget(node);
	}

	public void acceptLayout(DomLayout node)
	{
	    add(node.attributeClass());
	    m_laidOut = true;
	    super.acceptLayout(node);
	}

	public void acceptSpacer(DomSpacer node)
	{
	    add("QSpacerItem");
	    super.acceptSpacer(node);
	}

	public void acceptProperty(DomProperty node)
	{
	    switch(node.kind()) {
		    case Brush:
		        add("QBrush");
		        break;
		    case Color:
		        add("QColor");
	        break;
		    case Cursor:
		    case CursorShape:
		        add("QCursor");
		        add("Qt");
	        break;
		    case Enum:
		        add("Qt");
	        break;
		    case Font:
		        add("QFont");
	        break;
		    case Palette:
		        add("QPalette");
		        add("QBrush");
		        add("QColor");
		        add("Qt");
		        break;
		    case Pixmap:
		        add("QPixmap");
	        break;
		    case Point:
		        add("QPoint");
	        break;
		    case PointF:
		        add("QPointF");
	        break;
		    case Date:
		        add("QDate");
	        break;
		    case DateTime:
		        add("QDateTime");
	        break;
		    case Locale:
		        add("QLocale");
	        break;
		    case IconSet:
		        add("QIcon");
	        break;
		    case Url:
		        add("QUrl");
	        break;
		    case Time:
		        add("QTime");
	        break;
		    case StringList:
		        add("QStringList");
	        break;
		    case SizePolicy:
		        add("QSizePolicy");
	        break;
		    case Size:
		        add("QSize");
	        break;
		    case SizeF:
		        add("QSizeF");
	        break;
		    case Rect:
		        add("QRect");
	        break;
		    case RectF:
		        add("QRectF");
	        break;
		default:
			break;
		}
	    super.acceptProperty(node);
	}

	protected void add(String className, DomCustomWidget dcw)
	{
	    if (className==null || className.isEmpty() || m_knownClasses.contains(className))
	        return;

	    m_knownClasses.add(className);

	    CustomWidgetsInfo cwi = m_uic.customWidgetsInfo();
	    if (cwi.extendsOneOf(className, Arrays.asList(
		        "QTreeView", "QTreeWidget",
		        "QTableView", "QTableWidget"
		    )))
	        add("QHeaderView");

	    if (!m_laidOut && cwi._extends(className, "QToolBox"))
	        add("QLayout"); // spacing property of QToolBox)

	    if (className.equals("Line")) { // ### hmm, deprecate me!
	        add("QFrame");
	        return;
	    }

	    if (cwi._extends(className, "QDialogButtonBox"))
	        add("QAbstractButton"); // for signal "clicked(QAbstractButton)"

	    doAdd(className, dcw);
	}

	public void acceptCustomWidget(DomCustomWidget node)
	{
	    String className = node.elementClass();
	    if (className!=null && !className.isEmpty())
	        add(className, node);
	}

	public void acceptActionGroup(DomActionGroup node)
	{
	    add("QActionGroup");
	    super.acceptActionGroup(node);
	}

	public void acceptAction(DomAction node)
	{
	    add("QAction");
	    super.acceptAction(node);
	}

	public void acceptActionRef(DomActionRef node)
	{
	    add("QAction");
	    super.acceptActionRef(node);
	}
	
	protected void add(String className) {
		add(className, null);
	}
	
	protected void doAdd(String className) {
		doAdd(className, null);
	}
	
	protected abstract void doAdd(String className, DomCustomWidget dcw);
	
    private final Set<String> m_knownClasses = new TreeSet<>();
    private final Uic m_uic;

	private boolean m_laidOut = false;

	protected Uic uic() {
		return m_uic;
	}
}
