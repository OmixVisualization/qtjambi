/****************************************************************************
**
** Copyright (C) 2009-2022 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
**
** This file is part of Qt Jambi.
**
** ** $BEGIN_LICENSE$
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

import java.util.ArrayList;
import java.util.Collections;
import java.util.List;
import java.util.Map;
import java.util.TreeMap;

import io.qt.uic.ui4.DomProperty;
import io.qt.uic.ui4.DomUI;
import io.qt.uic.ui4.DomWidget;

public class DatabaseInfo extends TreeWalker{
	
	@Override
	public void acceptUI(DomUI node) {
	    m_connections.clear();
	    m_cursors.clear();
	    m_fields.clear();
	    super.acceptUI(node);
	}
	
	public List<String> connections()
    { return Collections.unmodifiableList(m_connections); }

	@Override
	public void acceptWidget(DomWidget node) {
		Map<String, DomProperty> properties = Utils.propertyMap(node.elementProperty());

	    DomProperty frameworkCode = properties.get("frameworkCode");
	    if (frameworkCode!=null && !Boolean.parseBoolean(frameworkCode.elementBool()))
	        return;

	    DomProperty db = properties.get("database");
	    if (db!=null && db.elementStringList()!=null) {
	        List<String> info = db.elementStringList().elementString();
	        if (info.isEmpty() || info.get(0).isEmpty())
	            return;
	        String connection = info.get(0);
	        if(!m_connections.contains(connection))
	        	m_connections.add(connection);

	        String table = info.size() > 1 ? info.get(1) : "";
	        if (table.isEmpty())
	            return;
	        List<String> val = m_cursors.get(connection);
	        val.add(table);
	        m_cursors.put(connection, val);

	        String field = info.size() > 2 ? info.get(2) : "";
	        if (field.isEmpty())
	            return;
	        val = m_fields.get(connection);
	        val.add(field);
	        m_fields.put(connection, val);
	    }

	    super.acceptWidget(node);
	}


    private final List<String> m_connections = new ArrayList<>();
    private final Map<String, List<String>> m_cursors = new TreeMap<>();
    private final Map<String, List<String>> m_fields = new TreeMap<>();
}
