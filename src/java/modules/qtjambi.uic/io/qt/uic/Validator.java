/****************************************************************************
**
** Copyright (C) 2009-2026 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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
import io.qt.uic.ui4.DomLayout;
import io.qt.uic.ui4.DomLayoutItem;
import io.qt.uic.ui4.DomUI;
import io.qt.uic.ui4.DomWidget;

/**
 * @hidden
 */
class Validator extends TreeWalker{
	public Validator(Uic ui) {
		this.m_driver = ui.driver();
	}
	
	@Override public void acceptUI(DomUI node)
	{
	    super.acceptUI(node);
	}

	@Override public void acceptWidget(DomWidget node)
	{
	    m_driver.findOrInsertWidget(node);

	    super.acceptWidget(node);
	}

	@Override public void acceptLayoutItem(DomLayoutItem node)
	{
	    m_driver.findOrInsertLayoutItem(node);

	    super.acceptLayoutItem(node);
	}

	@Override public void acceptLayout(DomLayout node)
	{
	    m_driver.findOrInsertLayout(node);

	    super.acceptLayout(node);
	}

	@Override public void acceptActionGroup(DomActionGroup node)
	{
	    m_driver.findOrInsertActionGroup(node);

	    super.acceptActionGroup(node);
	}

	@Override public void acceptAction(DomAction node)
	{
	    m_driver.findOrInsertAction(node);

	    super.acceptAction(node);
	}

	private final Driver m_driver;
}
