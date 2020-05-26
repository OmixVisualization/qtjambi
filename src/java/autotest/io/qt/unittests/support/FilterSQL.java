/****************************************************************************
**
** Copyright (C) 1992-2009 Nokia. All rights reserved.
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
package io.qt.unittests.support;

// This filter is untested as with ANT I was not able to work out how to enable it
public class FilterSQL extends AbstractFilter {

	// There is always a danger when filtering to exclude that no one notices
	//  the test is missing, so ideally we'd want to put a caveat in the main
	//  summary of the test unit report explaining the CategorySQL exclusion.
	private static final Class<?> categoryMarkerClass = CategorySQL.class;
	private static final String packageName = "io.qt.sql";
	private static final String className = "QSql";

	public static boolean detectStatic() {
		try {
			// We are runtime checking the class exists on the classpath.
			Class<?> cls = Class.forName(packageName + "." + className);
			if (cls != null)
				return true;
		} catch (ClassNotFoundException e) {
		}
		return false;
	}

	public FilterSQL() {
		super();
	}
	public FilterSQL(boolean includeFlag) {
		super(includeFlag);
	}

	@Override
	public String getDescriptionName() {
		return "SQL";
	}

	@Override
	public boolean detect() {
		return detectStatic();
	}

	@Override
	public Class<?> getCategoryMarkerClass() {
		return categoryMarkerClass;
	}
}
