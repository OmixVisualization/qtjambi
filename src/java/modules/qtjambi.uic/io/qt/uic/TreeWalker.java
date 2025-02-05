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
package io.qt.uic;

import io.qt.uic.ui4.DomAction;
import io.qt.uic.ui4.DomActionGroup;
import io.qt.uic.ui4.DomActionRef;
import io.qt.uic.ui4.DomButtonGroup;
import io.qt.uic.ui4.DomButtonGroups;
import io.qt.uic.ui4.DomColor;
import io.qt.uic.ui4.DomColorGroup;
import io.qt.uic.ui4.DomConnection;
import io.qt.uic.ui4.DomConnectionHint;
import io.qt.uic.ui4.DomConnectionHints;
import io.qt.uic.ui4.DomConnections;
import io.qt.uic.ui4.DomCustomWidget;
import io.qt.uic.ui4.DomCustomWidgets;
import io.qt.uic.ui4.DomDate;
import io.qt.uic.ui4.DomDateTime;
import io.qt.uic.ui4.DomFont;
import io.qt.uic.ui4.DomInclude;
import io.qt.uic.ui4.DomIncludes;
import io.qt.uic.ui4.DomLayout;
import io.qt.uic.ui4.DomLayoutDefault;
import io.qt.uic.ui4.DomLayoutFunction;
import io.qt.uic.ui4.DomLayoutItem;
import io.qt.uic.ui4.DomPalette;
import io.qt.uic.ui4.DomPoint;
import io.qt.uic.ui4.DomProperty;
import io.qt.uic.ui4.DomRect;
import io.qt.uic.ui4.DomSize;
import io.qt.uic.ui4.DomSizePolicy;
import io.qt.uic.ui4.DomSpacer;
import io.qt.uic.ui4.DomTabStops;
import io.qt.uic.ui4.DomTime;
import io.qt.uic.ui4.DomUI;
import io.qt.uic.ui4.DomWidget;

/**
 * @hidden
 */
public class TreeWalker {
	public void acceptUI(DomUI ui)
	{
	    acceptWidget(ui.elementWidget());
	    DomButtonGroups domButtonGroups = ui.elementButtonGroups();
	    if (domButtonGroups!=null)
	        acceptButtonGroups(domButtonGroups);

	    acceptTabStops(ui.elementTabStops());
	}

	public void acceptLayoutDefault(DomLayoutDefault layoutDefault)
	{
	}

	public void acceptLayoutFunction(DomLayoutFunction layoutFunction)
	{
	}

	public void acceptTabStops(DomTabStops tabStops)
	{
	}

	public void acceptLayout(DomLayout layout)
	{
	    for (DomProperty p : layout.elementProperty())
	        acceptProperty(p);

	    for (DomLayoutItem p : layout.elementItem())
	        acceptLayoutItem(p);
	}

	public void acceptLayoutItem(DomLayoutItem layoutItem)
	{
	    switch (layoutItem.kind()) {
	        case Widget:
	            acceptWidget(layoutItem.elementWidget());
	            return;
	        case Layout:
	            acceptLayout(layoutItem.elementLayout());
	            return;
	        case Spacer:
	            acceptSpacer(layoutItem.elementSpacer());
	            return;
	        case Unknown:
	            break;
	    }
	}

	public void acceptWidget(DomWidget widget)
	{
	    for (DomAction d : widget.elementAction())
	        acceptAction(d);

	    for (DomActionGroup d : widget.elementActionGroup())
	        acceptActionGroup(d);

	    for (DomActionRef d : widget.elementAddAction())
	        acceptActionRef(d);

	    for (DomProperty d : widget.elementProperty())
	        acceptProperty(d);



	    // recurse down
	    for (DomWidget child : widget.elementWidget()) {
	        acceptWidget(child);
	    }

	    if (!widget.elementLayout().isEmpty())
	        acceptLayout(widget.elementLayout().at(0));
	}

	public void acceptSpacer(DomSpacer spacer)
	{
	    for (DomProperty d : spacer.elementProperty())
	        acceptProperty(d);
	}

	public void acceptColor(DomColor color)
	{
	}

	public void acceptColorGroup(DomColorGroup colorGroup)
	{
	}

	public void acceptPalette(DomPalette palette)
	{
	    acceptColorGroup(palette.elementActive());
	    acceptColorGroup(palette.elementInactive());
	    acceptColorGroup(palette.elementDisabled());
	}

	public void acceptFont(DomFont font)
	{
	}

	public void acceptPoint(DomPoint point)
	{
	}

	public void acceptRect(DomRect rect)
	{
	}

	public void acceptSizePolicy(DomSizePolicy sizePolicy)
	{
	}

	public void acceptSize(DomSize size)
	{
	}

	public void acceptDate(DomDate date)
	{
	}

	public void acceptTime(DomTime time)
	{
	}

	public void acceptDateTime(DomDateTime dateTime)
	{
	}

	public void acceptProperty(DomProperty property)
	{
	    switch (property.kind()) {
	        case Bool:
	        case Color:
	        case Cstring:
	        case Cursor:
	        case CursorShape:
	        case Enum:
	        case Font:
	        case Pixmap:
	        case IconSet:
	        case Palette:
	        case Point:
	        case PointF:
	        case Rect:
	        case RectF:
	        case Set:
	        case Locale:
	        case SizePolicy:
	        case Size:
	        case SizeF:
	        case String:
	        case Number:
	        case LongLong:
	        case Char:
	        case Date:
	        case Time:
	        case DateTime:
	        case Url:
	        case Unknown:
	        case StringList:
	        case Float:
	        case Double:
	        case UInt:
	        case ULongLong:
	        case Brush:
	            break;
	    }
	}

	public void acceptCustomWidgets(DomCustomWidgets customWidgets)
	{
	    for (DomCustomWidget d : customWidgets.elementCustomWidget())
	        acceptCustomWidget(d);
	}

	public void acceptCustomWidget(DomCustomWidget customWidget)
	{
	}

	public void acceptAction(DomAction action)
	{
	}

	public void acceptActionGroup(DomActionGroup actionGroup)
	{
	    for (DomAction d : actionGroup.elementAction())
	        acceptAction(d);

	    for (int i=0; i<actionGroup.elementActionGroup().size(); ++i)
	        acceptActionGroup(actionGroup.elementActionGroup().at(i));
	}

	public void acceptActionRef(DomActionRef actionRef)
	{
	}

	public void acceptIncludes(DomIncludes includes)
	{
	    for (DomInclude d : includes.elementInclude())
	        acceptInclude(d);
	}

	public void acceptInclude(DomInclude incl)
	{
	}

	public void acceptConnections(DomConnections connections)
	{
	    for (DomConnection d : connections.elementConnection())
	        acceptConnection(d);
	}

	public void acceptConnection(DomConnection connection)
	{
	    acceptConnectionHints(connection.elementHints());
	}

	public void acceptConnectionHints(DomConnectionHints connectionHints)
	{
	    for (DomConnectionHint d : connectionHints.elementHint())
	        acceptConnectionHint(d);
	}

	public void acceptConnectionHint(DomConnectionHint connectionHint)
	{
	}

	public void acceptButtonGroups(DomButtonGroups domButtonGroups)
	{
	    for (DomButtonGroup g : domButtonGroups.elementButtonGroup())
	        acceptButtonGroup(g);
	}

	public void acceptButtonGroup(DomButtonGroup g)
	{
	}
}
