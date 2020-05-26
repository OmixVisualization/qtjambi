/****************************************************************************
**
** Copyright (C) 1992-2009 Nokia. All rights reserved.
** Copyright (C) 2009-2020 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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

import org.hamcrest.BaseMatcher;
import org.hamcrest.Description;
import org.hamcrest.Matcher;

/**
 * This class tests the features that are marked as arriving
 *  since a certain Qt version as per classname.
 */
public class MatcherQtVersion {

	public static Matcher<QtVersion> isQtGt(final QtVersion expected) {
		return new BaseMatcher<QtVersion>() {
			//@Override // Java6 only
			public boolean matches(final Object actualObject) {
				QtVersion actual = (QtVersion) actualObject;
				return expected.compareTo(actual) > 0; 
			}
			//@Override // Java6 only
			public void describeTo(final Description description) {
				description.appendText("isQtGt not matching QT version ").appendText(expected.getVersion());
			}
			//@Override // Java6 only
			@SuppressWarnings("unused")
			public void describeMismatch(final Object actualObject, final Description description) {
				description.appendText("was ").appendValue(((QtVersion)actualObject).getVersion());
			}
		};
	}

	public static Matcher<QtVersion> isQtGe(final QtVersion expected) {
		return new BaseMatcher<QtVersion>() {
			//@Override // Java6 only
			public boolean matches(final Object actualObject) {
				QtVersion actual = (QtVersion) actualObject;
				return expected.compareTo(actual) >= 0; 
			}
			//@Override // Java6 only
			public void describeTo(final Description description) {
				description.appendText("isQtGt not matching QT version ").appendText(expected.getVersion());
			}
			//@Override // Java6 only
			@SuppressWarnings("unused")
			public void describeMismatch(final Object actualObject, final Description description) {
				description.appendText("was ").appendValue(((QtVersion)actualObject).getVersion());
			}
		};
	}

	public static Matcher<QtVersion> isQtEq(final QtVersion expected) {
		return new BaseMatcher<QtVersion>() {
			//@Override // Java6 only
			public boolean matches(final Object actualObject) {
				QtVersion actual = (QtVersion) actualObject;
				return expected.compareTo(actual) == 0; 
			}
			//@Override // Java6 only
			public void describeTo(final Description description) {
				description.appendText("isQtGt not matching QT version ").appendText(expected.getVersion());
			}
			//@Override // Java6 only
			@SuppressWarnings("unused")
			public void describeMismatch(final Object actualObject, final Description description) {
				description.appendText("was ").appendValue(((QtVersion)actualObject).getVersion());
			}
		};
	}

	public static Matcher<QtVersion> isQtLe(final QtVersion expected) {
		return new BaseMatcher<QtVersion>() {
			//@Override // Java6 only
			public boolean matches(final Object actualObject) {
				QtVersion actual = (QtVersion) actualObject;
				return expected.compareTo(actual) <= 0; 
			}
			//@Override // Java6 only
			public void describeTo(final Description description) {
				description.appendText("isQtGt not matching QT version ").appendText(expected.getVersion());
			}
			//@Override // Java6 only
			@SuppressWarnings("unused")
			public void describeMismatch(final Object actualObject, final Description description) {
				description.appendText("was ").appendValue(((QtVersion)actualObject).getVersion());
			}
		};
	}

	public static Matcher<QtVersion> isQtLt(final QtVersion expected) {
		return new BaseMatcher<QtVersion>() {
			//@Override // Java6 only
			public boolean matches(final Object actualObject) {
				QtVersion actual = (QtVersion) actualObject;
				return expected.compareTo(actual) < 0; 
			}
			//@Override // Java6 only
			public void describeTo(final Description description) {
				description.appendText("isQtGt not matching QT version ").appendText(expected.getVersion());
			}
			//@Override // Java6 only
			@SuppressWarnings("unused")
			public void describeMismatch(final Object actualObject, final Description description) {
				description.appendText("was ").appendValue(((QtVersion)actualObject).getVersion());
			}
		};
	}

}
