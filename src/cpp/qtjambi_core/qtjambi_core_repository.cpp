#include "qtjambi_core_repository.h"

namespace Java{
namespace QtCore{
QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/core,QUnhandledException,)
#if QT_VERSION >= QT_VERSION_CHECK(5,15,0)
QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/core,QFile$TrashResult,
    QTJAMBI_REPOSITORY_DEFINE_CONSTRUCTOR(ZLjava/lang/String;)
)
QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/core,QByteArray$FromBase64Result,
    QTJAMBI_REPOSITORY_DEFINE_CONSTRUCTOR(Lio/qt/core/QByteArray;I)
)
#endif

#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/core,QPropertyData,
                                 QTJAMBI_REPOSITORY_DEFINE_METHOD(getValueBypassingBindings,()Ljava/lang/Object;)
                                 QTJAMBI_REPOSITORY_DEFINE_METHOD(setValueBypassingBindings,(Ljava/lang/Object;)Z)
                                 QTJAMBI_REPOSITORY_DEFINE_METHOD(valueMetaType,()Lio/qt/core/QMetaType;)
)
QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/core,QProperty,
                                 QTJAMBI_REPOSITORY_DEFINE_METHOD(value,()Ljava/lang/Object;)
                                 QTJAMBI_REPOSITORY_DEFINE_METHOD(setValue,(Ljava/lang/Object;)V)
                                 QTJAMBI_REPOSITORY_DEFINE_METHOD(binding,()Lio/qt/core/QPropertyBinding;)
                                 QTJAMBI_REPOSITORY_DEFINE_METHOD(setBinding,(Lio/qt/core/QPropertyBinding;)Lio/qt/core/QPropertyBinding;)
                                 QTJAMBI_REPOSITORY_DEFINE_METHOD(makeBinding,()Lio/qt/core/QPropertyBinding;)
                                 QTJAMBI_REPOSITORY_DEFINE_METHOD(bindingData,()Lio/qt/core/QPropertyBindingData;)
)
QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/core,QBooleanProperty,
                                 QTJAMBI_REPOSITORY_DEFINE_METHOD(value,()Z)
                                 QTJAMBI_REPOSITORY_DEFINE_METHOD(setValue,(Z)V)
                                 QTJAMBI_REPOSITORY_DEFINE_METHOD(binding,()Lio/qt/core/QBooleanPropertyBinding;)
                                 QTJAMBI_REPOSITORY_DEFINE_METHOD(setBinding,(Lio/qt/core/QBooleanPropertyBinding;)Lio/qt/core/QBooleanPropertyBinding;)
                                 QTJAMBI_REPOSITORY_DEFINE_METHOD(makeBinding,()Lio/qt/core/QBooleanPropertyBinding;)
                                 QTJAMBI_REPOSITORY_DEFINE_METHOD(bindingData,()Lio/qt/core/QPropertyBindingData;)
)
QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/core,QByteProperty,
                              QTJAMBI_REPOSITORY_DEFINE_METHOD(value,()B)
                              QTJAMBI_REPOSITORY_DEFINE_METHOD(setValue,(B)V)
                              QTJAMBI_REPOSITORY_DEFINE_METHOD(binding,()Lio/qt/core/QBytePropertyBinding;)
                              QTJAMBI_REPOSITORY_DEFINE_METHOD(setBinding,(Lio/qt/core/QBytePropertyBinding;)Lio/qt/core/QBytePropertyBinding;)
                              QTJAMBI_REPOSITORY_DEFINE_METHOD(makeBinding,()Lio/qt/core/QBytePropertyBinding;)
                              QTJAMBI_REPOSITORY_DEFINE_METHOD(bindingData,()Lio/qt/core/QPropertyBindingData;)
)
QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/core,QShortProperty,
                               QTJAMBI_REPOSITORY_DEFINE_METHOD(value,()S)
                               QTJAMBI_REPOSITORY_DEFINE_METHOD(setValue,(S)V)
                               QTJAMBI_REPOSITORY_DEFINE_METHOD(binding,()Lio/qt/core/QShortPropertyBinding;)
                               QTJAMBI_REPOSITORY_DEFINE_METHOD(setBinding,(Lio/qt/core/QShortPropertyBinding;)Lio/qt/core/QShortPropertyBinding;)
                               QTJAMBI_REPOSITORY_DEFINE_METHOD(makeBinding,()Lio/qt/core/QShortPropertyBinding;)
                               QTJAMBI_REPOSITORY_DEFINE_METHOD(bindingData,()Lio/qt/core/QPropertyBindingData;)
)
QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/core,QCharProperty,
                              QTJAMBI_REPOSITORY_DEFINE_METHOD(value,()C)
                              QTJAMBI_REPOSITORY_DEFINE_METHOD(setValue,(C)V)
                              QTJAMBI_REPOSITORY_DEFINE_METHOD(binding,()Lio/qt/core/QCharPropertyBinding;)
                              QTJAMBI_REPOSITORY_DEFINE_METHOD(setBinding,(Lio/qt/core/QCharPropertyBinding;)Lio/qt/core/QCharPropertyBinding;)
                              QTJAMBI_REPOSITORY_DEFINE_METHOD(makeBinding,()Lio/qt/core/QCharPropertyBinding;)
                              QTJAMBI_REPOSITORY_DEFINE_METHOD(bindingData,()Lio/qt/core/QPropertyBindingData;)
)
QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/core,QIntProperty,
                                QTJAMBI_REPOSITORY_DEFINE_METHOD(value,()I)
                                QTJAMBI_REPOSITORY_DEFINE_METHOD(setValue,(I)V)
                                QTJAMBI_REPOSITORY_DEFINE_METHOD(binding,()Lio/qt/core/QIntPropertyBinding;)
                                QTJAMBI_REPOSITORY_DEFINE_METHOD(setBinding,(Lio/qt/core/QIntPropertyBinding;)Lio/qt/core/QIntPropertyBinding;)
                                QTJAMBI_REPOSITORY_DEFINE_METHOD(makeBinding,()Lio/qt/core/QIntPropertyBinding;)
                                QTJAMBI_REPOSITORY_DEFINE_METHOD(bindingData,()Lio/qt/core/QPropertyBindingData;)
)
QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/core,QLongProperty,
                                 QTJAMBI_REPOSITORY_DEFINE_METHOD(value,()J)
                                 QTJAMBI_REPOSITORY_DEFINE_METHOD(setValue,(J)V)
                                 QTJAMBI_REPOSITORY_DEFINE_METHOD(binding,()Lio/qt/core/QLongPropertyBinding;)
                                 QTJAMBI_REPOSITORY_DEFINE_METHOD(setBinding,(Lio/qt/core/QLongPropertyBinding;)Lio/qt/core/QLongPropertyBinding;)
                                 QTJAMBI_REPOSITORY_DEFINE_METHOD(makeBinding,()Lio/qt/core/QLongPropertyBinding;)
                                 QTJAMBI_REPOSITORY_DEFINE_METHOD(bindingData,()Lio/qt/core/QPropertyBindingData;)
)
QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/core,QDoubleProperty,
                              QTJAMBI_REPOSITORY_DEFINE_METHOD(value,()D)
                              QTJAMBI_REPOSITORY_DEFINE_METHOD(setValue,(D)V)
                              QTJAMBI_REPOSITORY_DEFINE_METHOD(binding,()Lio/qt/core/QDoublePropertyBinding;)
                              QTJAMBI_REPOSITORY_DEFINE_METHOD(setBinding,(Lio/qt/core/QDoublePropertyBinding;)Lio/qt/core/QDoublePropertyBinding;)
                              QTJAMBI_REPOSITORY_DEFINE_METHOD(makeBinding,()Lio/qt/core/QDoublePropertyBinding;)
                              QTJAMBI_REPOSITORY_DEFINE_METHOD(bindingData,()Lio/qt/core/QPropertyBindingData;)
)
QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/core,QFloatProperty,
                               QTJAMBI_REPOSITORY_DEFINE_METHOD(value,()F)
                               QTJAMBI_REPOSITORY_DEFINE_METHOD(setValue,(F)V)
                               QTJAMBI_REPOSITORY_DEFINE_METHOD(binding,()Lio/qt/core/QFloatPropertyBinding;)
                               QTJAMBI_REPOSITORY_DEFINE_METHOD(setBinding,(Lio/qt/core/QFloatPropertyBinding;)Lio/qt/core/QFloatPropertyBinding;)
                               QTJAMBI_REPOSITORY_DEFINE_METHOD(makeBinding,()Lio/qt/core/QFloatPropertyBinding;)
                               QTJAMBI_REPOSITORY_DEFINE_METHOD(bindingData,()Lio/qt/core/QPropertyBindingData;)
)
QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/core,QPropertyBinding,
                                 QTJAMBI_REPOSITORY_DEFINE_CONSTRUCTOR(Lio/qt/core/QUntypedPropertyBinding;)
                                 QTJAMBI_REPOSITORY_DEFINE_STATIC_METHOD(analyzeMetaType,(Ljava/io/Serializable;)Lio/qt/core/QMetaType;)
)
QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/core,QIntPropertyBinding,
                                 QTJAMBI_REPOSITORY_DEFINE_CONSTRUCTOR(Lio/qt/core/QUntypedPropertyBinding;))
QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/core,QBytePropertyBinding,
                                 QTJAMBI_REPOSITORY_DEFINE_CONSTRUCTOR(Lio/qt/core/QUntypedPropertyBinding;))
QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/core,QShortPropertyBinding,
                                 QTJAMBI_REPOSITORY_DEFINE_CONSTRUCTOR(Lio/qt/core/QUntypedPropertyBinding;))
QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/core,QLongPropertyBinding,
                                 QTJAMBI_REPOSITORY_DEFINE_CONSTRUCTOR(Lio/qt/core/QUntypedPropertyBinding;))
QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/core,QDoublePropertyBinding,
                                 QTJAMBI_REPOSITORY_DEFINE_CONSTRUCTOR(Lio/qt/core/QUntypedPropertyBinding;))
QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/core,QFloatPropertyBinding,
                                 QTJAMBI_REPOSITORY_DEFINE_CONSTRUCTOR(Lio/qt/core/QUntypedPropertyBinding;))
QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/core,QBooleanPropertyBinding,
                                 QTJAMBI_REPOSITORY_DEFINE_CONSTRUCTOR(Lio/qt/core/QUntypedPropertyBinding;))
QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/core,QCharPropertyBinding,
                                 QTJAMBI_REPOSITORY_DEFINE_CONSTRUCTOR(Lio/qt/core/QUntypedPropertyBinding;))
QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/core,QIntBindable,)
QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/core,QByteBindable,)
QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/core,QShortBindable,)
QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/core,QLongBindable,)
QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/core,QDoubleBindable,)
QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/core,QFloatBindable,)
QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/core,QBooleanBindable,)
QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/core,QCharBindable,)
QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/core,QPropertyAlias,)
QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/core,QIntPropertyAlias,)
QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/core,QBytePropertyAlias,)
QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/core,QShortPropertyAlias,)
QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/core,QLongPropertyAlias,)
QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/core,QDoublePropertyAlias,)
QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/core,QFloatPropertyAlias,)
QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/core,QBooleanPropertyAlias,)
QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/core,QCharPropertyAlias,)
QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/core,QIntPropertyData,
                                 QTJAMBI_REPOSITORY_DEFINE_METHOD(setValueBypassingBindings,(I)Z))
QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/core,QShortPropertyData,
                                 QTJAMBI_REPOSITORY_DEFINE_METHOD(setValueBypassingBindings,(S)Z))
QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/core,QBytePropertyData,
                                 QTJAMBI_REPOSITORY_DEFINE_METHOD(setValueBypassingBindings,(B)Z))
QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/core,QLongPropertyData,
                                 QTJAMBI_REPOSITORY_DEFINE_METHOD(setValueBypassingBindings,(J)Z))
QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/core,QDoublePropertyData,
                                 QTJAMBI_REPOSITORY_DEFINE_METHOD(setValueBypassingBindings,(D)Z))
QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/core,QFloatPropertyData,
                                 QTJAMBI_REPOSITORY_DEFINE_METHOD(setValueBypassingBindings,(F)Z))
QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/core,QBooleanPropertyData,
                                 QTJAMBI_REPOSITORY_DEFINE_METHOD(setValueBypassingBindings,(Z)Z))
QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/core,QCharPropertyData,
                                 QTJAMBI_REPOSITORY_DEFINE_METHOD(setValueBypassingBindings,(C)Z))
QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/core,QPropertyChangeHandler,
                                 QTJAMBI_REPOSITORY_DEFINE_METHOD(invoke,()V))
QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/core,QPropertyNotifier,
                                 QTJAMBI_REPOSITORY_DEFINE_METHOD(invoke,()V))
#endif

QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/core,QCborStreamReader$StringResult,
    QTJAMBI_REPOSITORY_DEFINE_CONSTRUCTOR(Ljava/lang/Object;Lio/qt/core/QCborStreamReader$StringResultCode;)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/core,QCborValue$FromCborResult,
    QTJAMBI_REPOSITORY_DEFINE_CONSTRUCTOR(Lio/qt/core/QCborValue;Lio/qt/core/QCborParserError;)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/core,QDate$Week,
    QTJAMBI_REPOSITORY_DEFINE_CONSTRUCTOR(II)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/core,QLockFile$LockInfo,
    QTJAMBI_REPOSITORY_DEFINE_CONSTRUCTOR(JLjava/lang/String;Ljava/lang/String;)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/core,QJsonDocument$FromJsonResult,
    QTJAMBI_REPOSITORY_DEFINE_CONSTRUCTOR(Lio/qt/core/QJsonDocument;Lio/qt/core/QJsonParseError;)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/core,QSharedMemory,
    QTJAMBI_REPOSITORY_DEFINE_FIELD(__qt_accessMode,I)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/core,QDebug,
    QTJAMBI_REPOSITORY_DEFINE_FIELD(__rcDevice,Ljava/lang/Object;)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/core,QRunnable,)
}

namespace QtWidgets
{
QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/widgets,QWidget,
                                 QTJAMBI_REPOSITORY_DEFINE_METHOD(setParent,(Lio/qt/widgets/QWidget;)V))
}

namespace QtGui
{
#if QT_VERSION >= QT_VERSION_CHECK(6,2,0)
#define QTJAMBI_REPOSITORY_DEFINE_METHOD_QT6(M,P) QTJAMBI_REPOSITORY_DEFINE_METHOD(M,P)
#else
#define QTJAMBI_REPOSITORY_DEFINE_METHOD_QT6(M,P)
#endif


QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/gui,QGuiApplication,
                                 QTJAMBI_REPOSITORY_DEFINE_STATIC_METHOD(exec,()I)
                                 QTJAMBI_REPOSITORY_DEFINE_METHOD_QT6(resolveInterface,(Ljava/lang/Class;)Lio/qt/QtObjectInterface;)
                                )
}

namespace QtJambi
{
QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt,QtObjectInterface,)
QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt,QUnsuccessfulInvocationException,
    QTJAMBI_REPOSITORY_DEFINE_CONSTRUCTOR(Ljava/lang/String;)
)QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/internal,QtJambiInternal,
    QTJAMBI_REPOSITORY_DEFINE_STATIC_METHOD(invocationInfoProvider,()Ljava/util/function/IntFunction;)
)
QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt,InternalAccess$CallerContext,
    QTJAMBI_REPOSITORY_DEFINE_FIELD(declaringClass, Ljava/lang/Class;)
    QTJAMBI_REPOSITORY_DEFINE_FIELD(methodName, Ljava/lang/String;)
    QTJAMBI_REPOSITORY_DEFINE_FIELD(lineNumber, I)
)
QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt,QtUtilities$ShortSupplier,
                                 QTJAMBI_REPOSITORY_DEFINE_METHOD(getAsShort,()S))
QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt,QtUtilities$ByteSupplier,
                                 QTJAMBI_REPOSITORY_DEFINE_METHOD(getAsByte,()B))
QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt,QtUtilities$FloatSupplier,
                                 QTJAMBI_REPOSITORY_DEFINE_METHOD(getAsFloat,()F))
QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt,QtUtilities$CharSupplier,
                                 QTJAMBI_REPOSITORY_DEFINE_METHOD(getAsChar,()C))
QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt,QFlags,)
}

namespace Runtime{
QTJAMBI_REPOSITORY_DEFINE_CLASS(java/lang,ArithmeticException,
    QTJAMBI_REPOSITORY_DEFINE_CONSTRUCTOR(Ljava/lang/String;)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(java/math,BigInteger,
    QTJAMBI_REPOSITORY_DEFINE_CONSTRUCTOR(I[B)
    QTJAMBI_REPOSITORY_DEFINE_METHOD(abs,()Ljava/math/BigInteger;)
    QTJAMBI_REPOSITORY_DEFINE_METHOD(signum,()I)
    QTJAMBI_REPOSITORY_DEFINE_METHOD(bitLength,()I)
    QTJAMBI_REPOSITORY_DEFINE_METHOD(longValue,()J)
    QTJAMBI_REPOSITORY_DEFINE_METHOD(toByteArray,()[B)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(java/lang,Class,
    QTJAMBI_REPOSITORY_DEFINE_METHOD(getName,()Ljava/lang/String;))

namespace Private{
QTJAMBI_REPOSITORY_DEFINE_CLASS(java/lang,Long,
    QTJAMBI_REPOSITORY_DEFINE_STATIC_METHOD(hashCode,(J)I))
}

}

}

