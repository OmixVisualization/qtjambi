package org.qtjambi.unittests.support;

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
