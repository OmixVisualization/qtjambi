# What's new in QtJambi 6.3.4 / 6.2.7 / 5.15.9

## Bugfixes

Solved issues:
* [Issue 73](../../../issues/73): Add support for QQuickWebEngineView
* [Issue 72](../../../issues/72): Web page runJavascript always crashes jvm when you try to use the returned object
* [Issue 71](../../../issues/71): Java DeferredDelete throws thread affinity exception, one of threads is undefined
* Solved bug in converting QVariantMap

## New Features

* Added QQmlEngine template method: `singletonInstance`
* Added QJSEngine template methods: `fromScriptValue`, `toScriptValue`, `fromManagedValue`, `toManagedValue`
* Added functions to QtQml: `qmlTypeId`, `qjsEngine`
* Added functions to QtWebEngineQuick: `toWebEngineSettings`, `toWebEngineScriptCollection`
* Qt 6.4 ready.

## Compatibility

QtJambi 6.3.4, 6.2.7 and 5.15.9 are source- and binary-compatible to its predecessor releases. 
