package org.qtjambi.unittests.support;

// This filter is untested as with ANT I was not able to work out how to enable it
public class FilterSQL extends AbstractFilter {

	// There is always a danger when filtering to exclude that no one notices
	//  the test is missing, so ideally we'd want to put a caveat in the main
	//  summary of the test unit report explaining the CategorySQL exclusion.
	private static final Class<?> categoryMarkerClass = CategorySQL.class;
	private static final String packageName = "org.qtjambi.qt.sql";
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
