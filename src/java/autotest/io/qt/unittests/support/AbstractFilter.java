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

import org.junit.experimental.categories.Category;
import org.junit.runner.Description;
import org.junit.runner.manipulation.Filter;

// This filter is untested as with ANT I was not able to work out how to enable it
public abstract class AbstractFilter extends Filter {

	protected boolean includeFlag;  // by default we exclude
	protected boolean defaultFlag;  // by default we exclude

	protected boolean packageExists;

	public abstract boolean detect();
	public abstract Class<?> getCategoryMarkerClass();
	public abstract String getDescriptionName();

	private void setUp() {
		defaultFlag = !includeFlag;
		packageExists = detect();
	}

	public AbstractFilter() {
		setUp();
	}
	public AbstractFilter(boolean includeFlag) {
		this.includeFlag = includeFlag;
		setUp();
	}

	@Override
	public String describe() {
		return "filter " + (includeFlag ? "include" : "exclude") + " " + getDescriptionName();
	}

	@Override
	public boolean shouldRun(Description description) {
		Boolean has = hasCorrectCategoryAnnotation(description);
		if(has != null && has.booleanValue()) {
			// at this point we have specially marked the test
			if(packageExists)
				return !includeFlag;
			return false;
		}

		for (Description each : description.getChildren()) {
			if (shouldRun(each))
				return true;
		}

		return defaultFlag;
	}

	private Boolean hasCorrectCategoryAnnotation(Description description) {
		Category category = description.getAnnotation(Category.class);
		if(category == null)
			return null;
		for (Class<?> each : category.value()) {
			if(getCategoryMarkerClass().isAssignableFrom(each))
				return true;
		}
		return false;
	}
}
