TEMPLATE = subdirs
SUBDIRS += GenericFunction
SUBDIRS += Generic1Function
SUBDIRS += Generic2Function
SUBDIRS += Generic3Function
SUBDIRS += Generic4Function
SUBDIRS += Generic5Function
SUBDIRS += Generic6Function
SUBDIRS += DoubleToDoubleFunction
SUBDIRS += FloatToFloatFunction
greaterThan(QT_MAJOR_VERSION, 5): {
    SUBDIRS += PluginMetaDataFunction MetaTypeFunction QmlTypeCreatorFunction
	greaterThan(QT_MAJOR_VERSION, 6) | greaterThan(QT_MINOR_VERSION, 4): {
		SUBDIRS += PropertyBindingWrapperFunction BindingSetterFunction MakeBindingFunction
	}
}
else{
    SUBDIRS += VariantDataFunction
}
