/****************************************************************************
**
** Copyright (C) 2009-2015 Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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

package org.qtjambi.unittests.support;

// This filter is untested as with ANT I was not able to work out how to enable it
public class FilterDeclarative extends AbstractFilter {

	// There is always a danger when filtering to exclude that no one notices
	//  the test is missing, so ideally we'd want to put a caveat in the main
	//  summary of the test unit report explaining the CategorySQL exclusion.
	private static final Class<?> categoryMarkerClass = CategoryDeclarative.class;
	private static final String packageName = "org.qtjambi.qt.declarative";
	private static final String className = "QDeclarativeComponent";

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

	public FilterDeclarative() {
		super();
	}
	public FilterDeclarative(boolean includeFlag) {
		super(includeFlag);
	}

	@Override
	public String getDescriptionName() {
		return "Declarative";
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
