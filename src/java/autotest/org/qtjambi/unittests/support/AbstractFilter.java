package org.qtjambi.unittests.support;

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
