#ifndef QTJAMBI_CORE_REPOSITORY_H
#define QTJAMBI_CORE_REPOSITORY_H

#include <qtjambi/qtjambi_core.h>
#include <qtjambi/qtjambi_repository.h>

#if QT_VERSION >= QT_VERSION_CHECK(6,2,0)
#include <QtCore/qpropertyprivate.h>
#endif

namespace Java{
namespace QtCore
{
    QTJAMBI_REPOSITORY_DECLARE_CLASS(QException,
                                     QTJAMBI_REPOSITORY_DECLARE_THROWABLE_CONSTRUCTOR()
                                     )
    QTJAMBI_REPOSITORY_DECLARE_CLASS(QUnhandledException,
                                     QTJAMBI_REPOSITORY_DECLARE_THROWABLE_CONSTRUCTOR()
                                     )
    QTJAMBI_REPOSITORY_DECLARE_CLASS(QDebug,
                                     QTJAMBI_REPOSITORY_DECLARE_OBJECT_FIELD(__rcDevice))
    QTJAMBI_REPOSITORY_DECLARE_CLASS(QCborStreamReader$StringResult,
                                     QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR())
    QTJAMBI_REPOSITORY_DECLARE_CLASS(QCborValue$FromCborResult,
                                     QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR())
    QTJAMBI_REPOSITORY_DECLARE_CLASS(QDate$Week,
                                     QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR())
    QTJAMBI_REPOSITORY_DECLARE_CLASS(QJsonDocument$FromJsonResult,
                                     QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR())
    QTJAMBI_REPOSITORY_DECLARE_CLASS(QSharedMemory,
                                     QTJAMBI_REPOSITORY_DECLARE_INT_FIELD(__qt_accessMode))
    QTJAMBI_REPOSITORY_DECLARE_CLASS(QLockFile$LockInfo,
                                     QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR())
    QTJAMBI_REPOSITORY_DECLARE_CLASS(QRunnable,)
#if QT_VERSION >= QT_VERSION_CHECK(5,15,0)
    QTJAMBI_REPOSITORY_DECLARE_CLASS(QFile$TrashResult,
                                     QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR())
    QTJAMBI_REPOSITORY_DECLARE_CLASS(QByteArray$FromBase64Result,
                                     QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR())
#endif
#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
    QTJAMBI_REPOSITORY_DECLARE_CLASS(QProperty,
                                     QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(value)
                                     QTJAMBI_REPOSITORY_DECLARE_VOID_METHOD(setValue)
                                     QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(binding)
                                     QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(setBinding)
                                     QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(makeBinding)
                                     QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(bindingData)
                                     )
    QTJAMBI_REPOSITORY_DECLARE_CLASS(QBooleanProperty,
                                     QTJAMBI_REPOSITORY_DECLARE_BOOLEAN_METHOD(value)
                                     QTJAMBI_REPOSITORY_DECLARE_VOID_METHOD(setValue)
                                     QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(binding)
                                     QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(setBinding)
                                     QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(makeBinding)
                                     QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(bindingData)
                                     )
    QTJAMBI_REPOSITORY_DECLARE_CLASS(QCharProperty,
                                     QTJAMBI_REPOSITORY_DECLARE_CHAR_METHOD(value)
                                     QTJAMBI_REPOSITORY_DECLARE_VOID_METHOD(setValue)
                                     QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(binding)
                                     QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(setBinding)
                                     QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(makeBinding)
                                     QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(bindingData)
                                     )
    QTJAMBI_REPOSITORY_DECLARE_CLASS(QByteProperty,
                                     QTJAMBI_REPOSITORY_DECLARE_BYTE_METHOD(value)
                                     QTJAMBI_REPOSITORY_DECLARE_VOID_METHOD(setValue)
                                     QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(binding)
                                     QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(setBinding)
                                     QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(makeBinding)
                                     QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(bindingData)
                                     )
    QTJAMBI_REPOSITORY_DECLARE_CLASS(QIntProperty,
                                     QTJAMBI_REPOSITORY_DECLARE_INT_METHOD(value)
                                     QTJAMBI_REPOSITORY_DECLARE_VOID_METHOD(setValue)
                                     QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(binding)
                                     QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(setBinding)
                                     QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(makeBinding)
                                     QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(bindingData)
                                     )
    QTJAMBI_REPOSITORY_DECLARE_CLASS(QShortProperty,
                                     QTJAMBI_REPOSITORY_DECLARE_SHORT_METHOD(value)
                                     QTJAMBI_REPOSITORY_DECLARE_VOID_METHOD(setValue)
                                     QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(binding)
                                     QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(setBinding)
                                     QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(makeBinding)
                                     QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(bindingData)
                                     )
    QTJAMBI_REPOSITORY_DECLARE_CLASS(QLongProperty,
                                     QTJAMBI_REPOSITORY_DECLARE_LONG_METHOD(value)
                                     QTJAMBI_REPOSITORY_DECLARE_VOID_METHOD(setValue)
                                     QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(binding)
                                     QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(setBinding)
                                     QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(makeBinding)
                                     QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(bindingData)
                                     )
    QTJAMBI_REPOSITORY_DECLARE_CLASS(QDoubleProperty,
                                     QTJAMBI_REPOSITORY_DECLARE_DOUBLE_METHOD(value)
                                     QTJAMBI_REPOSITORY_DECLARE_VOID_METHOD(setValue)
                                     QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(binding)
                                     QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(setBinding)
                                     QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(makeBinding)
                                     QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(bindingData)
                                     )
    QTJAMBI_REPOSITORY_DECLARE_CLASS(QFloatProperty,
                                     QTJAMBI_REPOSITORY_DECLARE_FLOAT_METHOD(value)
                                     QTJAMBI_REPOSITORY_DECLARE_VOID_METHOD(setValue)
                                     QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(binding)
                                     QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(setBinding)
                                     QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(makeBinding)
                                     QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(bindingData)
                                     )
    QTJAMBI_REPOSITORY_DECLARE_CLASS(QPropertyBinding,
                                     QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR()
                                     QTJAMBI_REPOSITORY_DECLARE_STATIC_OBJECT_METHOD(analyzeMetaType)
                                     )
    QTJAMBI_REPOSITORY_DECLARE_CLASS(QIntPropertyBinding,
                                     QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR()
                                     )
    QTJAMBI_REPOSITORY_DECLARE_CLASS(QBytePropertyBinding,
                                     QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR()
                                     )
    QTJAMBI_REPOSITORY_DECLARE_CLASS(QShortPropertyBinding,
                                     QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR()
                                     )
    QTJAMBI_REPOSITORY_DECLARE_CLASS(QLongPropertyBinding,
                                     QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR()
                                     )
    QTJAMBI_REPOSITORY_DECLARE_CLASS(QDoublePropertyBinding,
                                     QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR()
                                     )
    QTJAMBI_REPOSITORY_DECLARE_CLASS(QFloatPropertyBinding,
                                     QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR()
                                     )
    QTJAMBI_REPOSITORY_DECLARE_CLASS(QCharPropertyBinding,
                                     QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR()
                                     )
    QTJAMBI_REPOSITORY_DECLARE_CLASS(QBooleanPropertyBinding,
                                     QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR()
                                     )
    QTJAMBI_REPOSITORY_DECLARE_CLASS(QPropertyAlias,
                                     )
    QTJAMBI_REPOSITORY_DECLARE_CLASS(QIntPropertyAlias,
                                     )
    QTJAMBI_REPOSITORY_DECLARE_CLASS(QBytePropertyAlias,
                                     )
    QTJAMBI_REPOSITORY_DECLARE_CLASS(QShortPropertyAlias,
                                     )
    QTJAMBI_REPOSITORY_DECLARE_CLASS(QLongPropertyAlias,
                                     )
    QTJAMBI_REPOSITORY_DECLARE_CLASS(QDoublePropertyAlias,
                                     )
    QTJAMBI_REPOSITORY_DECLARE_CLASS(QFloatPropertyAlias,
                                     )
    QTJAMBI_REPOSITORY_DECLARE_CLASS(QCharPropertyAlias,
                                     )
    QTJAMBI_REPOSITORY_DECLARE_CLASS(QBooleanPropertyAlias,
                                     )
    QTJAMBI_REPOSITORY_DECLARE_CLASS(QIntBindable,
                                     )
    QTJAMBI_REPOSITORY_DECLARE_CLASS(QByteBindable,
                                     )
    QTJAMBI_REPOSITORY_DECLARE_CLASS(QShortBindable,
                                     )
    QTJAMBI_REPOSITORY_DECLARE_CLASS(QLongBindable,
                                     )
    QTJAMBI_REPOSITORY_DECLARE_CLASS(QDoubleBindable,
                                     )
    QTJAMBI_REPOSITORY_DECLARE_CLASS(QFloatBindable,
                                     )
    QTJAMBI_REPOSITORY_DECLARE_CLASS(QCharBindable,
                                     )
    QTJAMBI_REPOSITORY_DECLARE_CLASS(QBooleanBindable,
                                     )
    QTJAMBI_REPOSITORY_DECLARE_CLASS(QPropertyData,
                                     QTJAMBI_REPOSITORY_DECLARE_BOOLEAN_METHOD(setValueBypassingBindings)
                                     QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(getValueBypassingBindings)
                                     QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(valueMetaType)
                                     )
    QTJAMBI_REPOSITORY_DECLARE_CLASS(QIntPropertyData,
                                     QTJAMBI_REPOSITORY_DECLARE_BOOLEAN_METHOD(setValueBypassingBindings))
    QTJAMBI_REPOSITORY_DECLARE_CLASS(QShortPropertyData,
                                     QTJAMBI_REPOSITORY_DECLARE_BOOLEAN_METHOD(setValueBypassingBindings))
    QTJAMBI_REPOSITORY_DECLARE_CLASS(QBytePropertyData,
                                     QTJAMBI_REPOSITORY_DECLARE_BOOLEAN_METHOD(setValueBypassingBindings))
    QTJAMBI_REPOSITORY_DECLARE_CLASS(QLongPropertyData,
                                     QTJAMBI_REPOSITORY_DECLARE_BOOLEAN_METHOD(setValueBypassingBindings))
    QTJAMBI_REPOSITORY_DECLARE_CLASS(QDoublePropertyData,
                                     QTJAMBI_REPOSITORY_DECLARE_BOOLEAN_METHOD(setValueBypassingBindings))
    QTJAMBI_REPOSITORY_DECLARE_CLASS(QFloatPropertyData,
                                     QTJAMBI_REPOSITORY_DECLARE_BOOLEAN_METHOD(setValueBypassingBindings))
    QTJAMBI_REPOSITORY_DECLARE_CLASS(QBooleanPropertyData,
                                     QTJAMBI_REPOSITORY_DECLARE_BOOLEAN_METHOD(setValueBypassingBindings))
    QTJAMBI_REPOSITORY_DECLARE_CLASS(QCharPropertyData,
                                     QTJAMBI_REPOSITORY_DECLARE_BOOLEAN_METHOD(setValueBypassingBindings))
    QTJAMBI_REPOSITORY_DECLARE_CLASS(QPropertyChangeHandler,
                                     QTJAMBI_REPOSITORY_DECLARE_VOID_METHOD(invoke))
    QTJAMBI_REPOSITORY_DECLARE_CLASS(QPropertyNotifier,
                                     QTJAMBI_REPOSITORY_DECLARE_VOID_METHOD(invoke))
#endif
}

#if QT_VERSION >= QT_VERSION_CHECK(6,2,0)
#define QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD_QT6(M) QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(M)
#else
#define QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD_QT6(M)
#endif


namespace QtGui
{
    QTJAMBI_REPOSITORY_DECLARE_CLASS(QGuiApplication,
                                    QTJAMBI_REPOSITORY_DECLARE_STATIC_INT_METHOD(exec)
                                    QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD_QT6(resolveInterface)
                                    )
}

namespace QtWidgets
{
    QTJAMBI_REPOSITORY_DECLARE_CLASS(QWidget,
                                 QTJAMBI_REPOSITORY_DECLARE_VOID_METHOD(setParent))
}

namespace QtJambi
{
QTJAMBI_REPOSITORY_DECLARE_CLASS(QtJambiInternal,
                                 QTJAMBI_REPOSITORY_DECLARE_STATIC_OBJECT_METHOD(invocationInfoProvider))
QTJAMBI_REPOSITORY_DECLARE_CLASS(InternalAccess$CallerContext,
                                  QTJAMBI_REPOSITORY_DECLARE_CLASS_FIELD(declaringClass)
                                  QTJAMBI_REPOSITORY_DECLARE_STRING_FIELD(methodName)
                                  QTJAMBI_REPOSITORY_DECLARE_INT_FIELD(lineNumber))
QTJAMBI_REPOSITORY_DECLARE_CLASS(QtUtilities$ShortSupplier,
                                 QTJAMBI_REPOSITORY_DECLARE_SHORT_METHOD(getAsShort))
QTJAMBI_REPOSITORY_DECLARE_CLASS(QtUtilities$ByteSupplier,
                                 QTJAMBI_REPOSITORY_DECLARE_BYTE_METHOD(getAsByte))
QTJAMBI_REPOSITORY_DECLARE_CLASS(QtUtilities$FloatSupplier,
                                 QTJAMBI_REPOSITORY_DECLARE_FLOAT_METHOD(getAsFloat))
QTJAMBI_REPOSITORY_DECLARE_CLASS(QtUtilities$CharSupplier,
                                 QTJAMBI_REPOSITORY_DECLARE_CHAR_METHOD(getAsChar))
QTJAMBI_REPOSITORY_DECLARE_CLASS(QUnsuccessfulInvocationException,
              QTJAMBI_REPOSITORY_DECLARE_THROWABLE_CONSTRUCTOR())
QTJAMBI_REPOSITORY_DECLARE_CLASS(QtObjectInterface,)
QTJAMBI_REPOSITORY_DECLARE_CLASS(QFlags,)
}

namespace Runtime{
    QTJAMBI_REPOSITORY_DECLARE_CLASS(ArithmeticException,
                  QTJAMBI_REPOSITORY_DECLARE_THROWABLE_CONSTRUCTOR())

    QTJAMBI_REPOSITORY_DECLARE_CLASS(BigInteger,
                  QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR()
                  QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(abs)
                  QTJAMBI_REPOSITORY_DECLARE_INT_METHOD(signum)
                  QTJAMBI_REPOSITORY_DECLARE_INT_METHOD(bitLength)
                  QTJAMBI_REPOSITORY_DECLARE_LONG_METHOD(longValue)
                  QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(toByteArray))
    QTJAMBI_REPOSITORY_DECLARE_CLASS(Class,
                  QTJAMBI_REPOSITORY_DECLARE_STRING_METHOD(getName))
}
}

#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
QtPrivate::QPropertyObserverCallback qtjambi_get_signal_callback(JNIEnv *, QUntypedPropertyData *);
#endif

#endif // QTJAMBI_CORE_REPOSITORY_H
