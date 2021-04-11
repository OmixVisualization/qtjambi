#include <QtCore/QMetaType>
#include <QtCore/QHash>
#include "qtjambi_registry.h"
#include "qtjambi_application.h"

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
namespace QtPrivate{
class DummyType{};
template<>
struct MetaTypeSmartPointerHelper<DummyType,DummyType>{
    static bool registerComparatorFunction(const QtPrivate::AbstractComparatorFunction *f, int typeId){
#if !defined(Q_NO_TEMPLATE_FRIENDS) && !defined(Q_CC_MSVC)
        if(!QMetaType::hasRegisteredComparators(typeId))
            return QMetaType::registerComparatorFunction(f, typeId);
#else
        Q_UNUSED(typeId)
        Q_UNUSED(f)
#endif
        return false;
    }
    static bool registerConverterFunction(const AbstractConverterFunction *f, int from, int to)
    {
        if(QMetaType::hasRegisteredConverterFunction(from, to))
            return false;
        return QMetaType::registerConverterFunction(f, from, to);
    }
    static void unregisterConverterFunction(int from, int to)
    {
        QMetaType::unregisterConverterFunction(from, to);
    }
    static bool registerDebugStreamOperatorFunction(const AbstractDebugStreamFunction *f, int typeId)
    {
#if !defined(Q_NO_TEMPLATE_FRIENDS) && !defined(Q_CC_MSVC)
        if(!QMetaType::hasRegisteredDebugStreamOperator(typeId))
            return QMetaType::registerDebugStreamOperatorFunction(f, typeId);
#else
        Q_UNUSED(typeId)
        Q_UNUSED(f)
#endif
        return false;
    }
};
}

typedef QHash<int,const QtPrivate::AbstractComparatorFunction *> ComparatorFunctions;
Q_GLOBAL_STATIC(ComparatorFunctions, gComparatorFunctions)
typedef QHash<int,const QtPrivate::AbstractDebugStreamFunction *> DebugStreamFunctions;
Q_GLOBAL_STATIC(DebugStreamFunctions, gDebugStreamFunctions)

bool qtjambi_register_comparator(const QtPrivate::AbstractComparatorFunction *f, int typeId){
    if(QMetaType::hasRegisteredComparators(typeId))
        return true;
#if !defined(Q_NO_TEMPLATE_FRIENDS) && !defined(Q_CC_MSVC)
    return QtPrivate::MetaTypeSmartPointerHelper<QtPrivate::DummyType,QtPrivate::DummyType>::registerComparatorFunction(f, typeId);
#else
    gComparatorFunctions->insert(typeId, f);
    return true;
#endif
}

const QtPrivate::AbstractComparatorFunction * qtjambi_registered_comparator(int typeId){
    return gComparatorFunctions->value(typeId);
}

const QtPrivate::AbstractDebugStreamFunction * qtjambi_registered_debugstream_operator(int typeId){
    return gDebugStreamFunctions->value(typeId);
}

bool qtjambi_register_debugstream_operator(const QtPrivate::AbstractDebugStreamFunction *f, int typeId){
    if(QMetaType::hasRegisteredDebugStreamOperator(typeId))
        return false;
#if !defined(Q_NO_TEMPLATE_FRIENDS) && !defined(Q_CC_MSVC)
    return QtPrivate::MetaTypeSmartPointerHelper<QtPrivate::DummyType,QtPrivate::DummyType>::registerDebugStreamOperatorFunction(f, typeId);
#else
    gDebugStreamFunctions->insert(typeId, f);
    return true;
#endif
}

bool qtjambi_register_converter(const QtPrivate::AbstractConverterFunction *f, int from, int to){
#if !defined(Q_NO_TEMPLATE_FRIENDS) && !defined(Q_CC_MSVC)
    return QtPrivate::MetaTypeSmartPointerHelper<QtPrivate::DummyType,QtPrivate::DummyType>::registerConverterFunction(f, from, to);
#else
    if(QMetaType::hasRegisteredConverterFunction(from, to))
        return false;
    return QMetaType::registerConverterFunction(f, from, to);
#endif
}

void qtjambi_unregister_converter(int from, int to){
#if !defined(Q_NO_TEMPLATE_FRIENDS) && !defined(Q_CC_MSVC)
    QtPrivate::MetaTypeSmartPointerHelper<QtPrivate::DummyType,QtPrivate::DummyType>::unregisterConverterFunction(from, to);
#else
    QMetaType::unregisterConverterFunction(from, to);
#endif
}
#endif
