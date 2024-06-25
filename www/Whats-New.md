# What's new in QtJambi 6.7.2

## Bugfixes

Solved issue:
* Exception when emitting `QAbstractItemModel.dataChanged` with default arguments
* Auto-detected invokable method in subclass shadowing superclass property
* Signal connections to lambda expression fail on queued emit
* Type registration of `QMetaType(QAbstractItemModel\*)` incomplete when implicitly done during subclass registration
* Running on Android again

## New Feature

* New nullness support in `QVariant`:
	* `QVariant.fromValue()` with argument `null` now converts to `QVariant(Nullptr)` (`QVariant.setValue()` similar)
	* `QVariant.NULL` is an alias object represents `QVariant(Nullptr)`
	* `QVariant.nullable(Object)` returns the given object if not `null` and `QVariant.NULL` otherwise
	* `QVariant.typedNullable<T>(Object,Class,QMetaType...)` returns the given object if not `null` and a type-specific null alias object otherwise
* New experimental features to be enabled by JVM argument `-D<prop>=true` or via `System.setProperty("<prop>", "true")`:
	* `io.qt.experimental.fast-jni` enables improved gathering of JNI environment during native calls
	* `io.qt.experimental.fast-jni-for-overrides` enables improved gathering of JNI environment during native virtual calls
	* Following experimental features deal with the improvement of item model performance:
		* `io.qt.experimental.item-model-cached-virtuals` item model's `rowCount()` and `columnCount()` return values are cached to improve performance of item views.
		* `io.qt.experimental.enable-invalid-modelindex-singleton` invalid `QModelIndex` is represented by one single Java object.
		* `io.qt.experimental.enable-ephemeral-modelindexes` instead of calling virtual Java methods with a copy of a QModelIndex argument it submits an alias which is invalidated after use.

## Further Changes

* QPropertyAlias is deprecated

QtJambi 6.7.2 is not binary compatible to Qt 6.7.0. Please use Qt 6.7.1 or higher.