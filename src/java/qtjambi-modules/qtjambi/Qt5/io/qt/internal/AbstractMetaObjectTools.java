package io.qt.internal;

import java.lang.reflect.*;
import io.qt.*;
import io.qt.core.*;

class AbstractMetaObjectTools {
    static final PropertyFlags ResolveDesignable = PropertyFlags.ResolveDesignable;
    static final PropertyFlags ResolveScriptable = PropertyFlags.ResolveScriptable;
    static final PropertyFlags ResolveEditable = PropertyFlags.ResolveEditable;
    static final PropertyFlags Editable = PropertyFlags.Editable;
    static final PropertyFlags Notify = PropertyFlags.Notify;
    static final PropertyFlags ResolveStored = PropertyFlags.ResolveStored;
    static final PropertyFlags ResolveUser = PropertyFlags.ResolveUser;
    static final PropertyFlags Required;
    
    static{
        PropertyFlags _Required = null;
        try{
            _Required = PropertyFlags.valueOf("Required");
        }catch(Throwable t){}
        Required = _Required;
    }
    
    static final PropertyFlags Bindable = null;
    static final PropertyFlags Alias = null;
    static final MetaObjectFlags PropertyAccessInStaticMetaCall = MetaObjectFlags.PropertyAccessInStaticMetaCall;
    
    static MetaObjectTools.AnnotationInfo analyzeBindableAnnotation(Method method) {
        return null;
    }
    
    static boolean isValidBindable(Method method) {
        return false;
    }
    
    static boolean isValidQProperty(Field field) {
        return false;
    }
    
    static MetaObjectTools.QPropertyTypeInfo getQPropertyTypeInfo(Field field) {
        return null;
    }
    
    static QtJambiPropertyInfo analyzeProperty(QObject object, QtObject property) {
        return null;
    }
    
    static void registerPropertyField(QMetaProperty metaProperty, Field field){}
    
    static boolean isListType(Class<?> cls) {
        return cls==QList.class
                || cls==QQueue.class
                || cls==QStack.class
                || cls==QVector.class
                || cls==QLinkedList.class
                || cls==QSet.class;
    }
}
