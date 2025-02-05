/****************************************************************************
**
** Copyright (C) 2009-2025 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
**
** This file is part of Qt Jambi.
**
** $BEGIN_LICENSE$
** GNU Lesser General Public License Usage
** This file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
** $END_LICENSE$
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

#if !defined(QTJAMBI_UTILS_P_H) && !defined(QTJAMBI_GENERATOR_RUNNING)
#define QTJAMBI_UTILS_P_H

#include <QtCore/QtGlobal>
#include <QtCore/QLoggingCategory>
#include <QtCore/QReadWriteLock>
#include <QtCore/QMutex>
#include "qtjambiapi.h"

struct VariantUtility{
#if QT_VERSION >= QT_VERSION_CHECK(6, 7, 0)
    static QVariant createVariant(QMetaType type, void *data);
#else
#endif
    static QVariant createVariant(const QMetaType& type, const void *data);
};

template<typename Super, auto& gLock>
struct SecureContainer : Super{
    SecureContainer(){
    }
    ~SecureContainer(){
        Super container;
        if constexpr(std::is_same<decltype(gLock()),QReadWriteLock>::value){
            QWriteLocker locker(gLock());
            container.swap(*this);
        }else if constexpr(std::is_same<decltype(gLock()),QRecursiveMutex>::value){
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
            QMutexLocker locker(gLock());
#else
            QMutexLocker<QRecursiveMutex> locker(gLock());
#endif
            container.swap(*this);
        }else if constexpr(std::is_same<decltype(gLock()),QMutex>::value){
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
            QMutexLocker locker(gLock());
#else
            QMutexLocker<QMutex> locker(gLock());
#endif
            container.swap(*this);
        }
    }
};

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
namespace QtPrivate{
    struct AbstractConverterFunction;
}

bool registerConverter(const QtPrivate::AbstractConverterFunction *f, int from, int to);
const QtPrivate::AbstractComparatorFunction * registeredComparator(int typeId);
const QtPrivate::AbstractDebugStreamFunction * registeredDebugStreamOperator(int typeId);
bool isLessThan(const QMetaType& keyMetaType, const void * ptr, const void* ptr2);
bool isEquals(const QMetaType& keyMetaType, const void * ptr, const void* ptr2);
#endif

#if defined(Q_OS_ANDROID) || defined(Q_OS_FREEBSD)
#define unique_id(id) qHash(QLatin1String((id).name()))
#define typeid_equals(t1, t2) unique_id(t1)==unique_id(t2)
#define typeid_not_equals(t1, t2) unique_id(t1)!=unique_id(t2)
#else
#define unique_id(id) (id).hash_code()
#define typeid_equals(t1, t2) t1==t2
#define typeid_not_equals(t1, t2) t1!=t2
#endif

void registerContainerAccess(int metaType, AbstractContainerAccess* access);
void registerContainerAccess(int metaType, const QSharedPointer<AbstractContainerAccess>& sharedAccess);
bool hasRegisteredContainerAccess(const QMetaType& metaType);
QSharedPointer<AbstractContainerAccess> findContainerAccess(const QMetaType& metaType);

QString getFunctionLibraryPath(QFunctionPointer function);

bool isValidArray(JNIEnv *env, jobject object, jclass contentType);

jobject resolveLongEnum(JNIEnv *env, jint hashCode, jclass enumClass, jlong value, jstring entryName);
jobject resolveByteEnum(JNIEnv *env, jint hashCode, jclass enumClass, jbyte value, jstring entryName);
jobject resolveShortEnum(JNIEnv *env, jint hashCode, jclass enumClass, jshort value, jstring entryName);
jobject resolveIntEnum(JNIEnv *env, jint hashCode, jclass enumClass, jint value, jstring entryName);
void registerSwitchTableFields(JNIEnv *env, jobject switchTableFields);

bool simpleEventNotify(void **data);
bool threadAffineEventNotify(void **data);
void enableThreadAffinity(bool enabled);
void enableSignalEmitThreadCheck(bool enabled);

bool enabledDanglingPointerCheck(JNIEnv * env = nullptr);
bool compareMetaTypes(const QMetaType& typeA, const QMetaType& typeB);
void checkThreadOnQObject(JNIEnv *env, jobject object);
void checkThreadOnQObject(JNIEnv *env, PtrOwnerFunction owner_function, const void *qt_object, jclass cls);

void containerDisposer(AbstractContainerAccess* _access);

const std::type_info* tryGetTypeInfo(JNIEnv *env, TypeInfoSupplier typeInfoSupplier, const void* ptr);
const std::type_info* checkedGetTypeInfo(TypeInfoSupplier typeInfoSupplier, const void* ptr);

enum class NativeToJavaConversionMode{
    None,
    MakeCopyOfValues,
    TransferOwnership,
    CppOwnership
};

jobject internal_convertNativeToJavaObject(JNIEnv *env, const void *qt_object, const std::type_info& typeId, const char *qtName, NativeToJavaConversionMode mode, bool *ok = nullptr);
jobject internal_convertSmartPointerToJavaObject(JNIEnv *env, const char *className,
                                                const QSharedPointer<char>& ptr_shared_pointer);
jobject internal_convertSmartPointerToJavaInterface(JNIEnv *env, const std::type_info& interfaceType,
                                                const QSharedPointer<char>& ptr_shared_pointer);
jobject internal_convertSmartPointerToJavaObject(JNIEnv *env, const char *className,
                                                 const std::shared_ptr<char>& ptr_shared_pointer);
jobject internal_convertSmartPointerToJavaInterface(JNIEnv *env, const std::type_info& interfaceType,
                                                    const std::shared_ptr<char>& ptr_shared_pointer);

class QtJambiLink;

namespace DebugAPI{

struct PrinterPrivate;

class Printer{
    typedef void(*Deleter)(void*);
    typedef void(*Invoker)(void*,QDebug&);
public:
    typedef void(*FunctionPointer)(QDebug&);

private:
    explicit Printer(void* data, Invoker invoker, Deleter deleter) noexcept;
public:
    Printer() noexcept;
    ~Printer() noexcept;
    Printer(const Printer& other) noexcept;
    Printer(Printer&& other) noexcept;
    Printer(FunctionPointer functor) noexcept;
    inline Printer(nullptr_t) noexcept : Printer(FunctionPointer(nullptr)) {}

    Printer& operator=(const Printer& other) noexcept;
    Printer& operator=(Printer&& other) noexcept;

    template<typename Functor, typename std::enable_if<!std::is_pointer<Functor>::value, bool>::type = true
             , typename std::enable_if<!std::is_same<typename std::remove_reference<typename std::remove_cv<Functor>::type>::type, Printer>::value, bool>::type = true
             , typename std::enable_if<!std::is_null_pointer<typename std::remove_reference<typename std::remove_cv<Functor>::type>::type>::value, bool>::type = true
             , typename std::enable_if<!std::is_same<typename std::remove_reference<typename std::remove_cv<Functor>::type>::type, FunctionPointer>::value, bool>::type = true
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
             , typename std::enable_if<std::is_invocable<Functor,QDebug&>::value, bool>::type = true
#endif
             >
    Printer(Functor&& functor) noexcept
        : Printer(
            new typename std::remove_reference<typename std::remove_cv<Functor>::type>::type(std::move(functor)),
            [](void* data, QDebug& d){
                typename std::remove_reference<typename std::remove_cv<Functor>::type>::type* fct = reinterpret_cast<typename std::remove_reference<typename std::remove_cv<Functor>::type>::type*>(data);
                (*fct)(d);
            },
            [](void* data){
                delete reinterpret_cast<typename std::remove_reference<typename std::remove_cv<Functor>::type>::type*>(data);
            }
            ){}
    bool operator==(const Printer& other) const noexcept;
    void operator()(QDebug&) const;
    operator bool() const noexcept;
    bool operator !() const noexcept;
private:
    friend PrinterPrivate;
    QExplicitlySharedDataPointer<PrinterPrivate> d;
};

Q_DECLARE_LOGGING_CATEGORY(internalCategory)
Q_DECLARE_LOGGING_CATEGORY(debugAPIJavaOverloadsCategory)
Q_DECLARE_LOGGING_CATEGORY(debugAPIEventsCategory)
Q_DECLARE_LOGGING_CATEGORY(debugAPICleanupCallsCategory)
Q_DECLARE_LOGGING_CATEGORY(debugAPIInternalMethodsCategory)

class MethodPrintFromLink : public MethodPrint
{
public:
    MethodPrintFromLink(const QSharedPointer<QtJambiLink>& link, const char* method, const char* file, int line, const char *function);
    MethodPrintFromLink(const QWeakPointer<QtJambiLink>& link, const char* method, const char* file, int line, const char *function);
};

class MethodPrintFromArgs : public MethodPrint
{
public:
    MethodPrintFromArgs(MethodPrint::Type callType, const char* file, int line, const char *function, const char* method, ...);
    MethodPrintFromArgs(const QLoggingCategory& category, const char* file, int line, const char *function, const char* method, ...);
};

class MethodPrintFromSupplier : public MethodPrint
{
public:
    MethodPrintFromSupplier(MethodPrint::Type callType, const char* file, int line, const char *function, Printer&& supplier);
    MethodPrintFromSupplier(const QLoggingCategory& category, const char* file, int line, const char *function, Printer&& supplier);
};

class EventPrint : public MethodPrint
{
public:
    EventPrint(const char* file, int line, const char *function, QObject *receiver, QEvent *event);
};

void printArgs(const char *file, int line, const char *function, const char *format,...);
void print(const char *file, int line, const char *function, Printer&& printer);

}

#ifdef QTJAMBI_NO_METHOD_TRACE
#define QTJAMBI_DEBUG_PRINT_ARGS(...)
#define QTJAMBI_DEBUG_INTERNAL_PRINT(printer)
#define QTJAMBI_INTERNAL_METHOD_CALL_PRINT(printer)
#else
#define QTJAMBI_DEBUG_PRINT_ARGS(...) DebugAPI::printArgs(__FILE__, __LINE__, Q_FUNC_INFO, __VA_ARGS__);
#define QTJAMBI_DEBUG_INTERNAL_PRINT(printer) DebugAPI::print(__FILE__, __LINE__, Q_FUNC_INFO, printer);
#define QTJAMBI_INTERNAL_METHOD_CALL_PRINT(printer) DebugAPI::MethodPrintFromSupplier __debug_method_print(DebugAPI::debugAPIInternalMethodsCategory(), __FILE__, __LINE__, Q_FUNC_INFO, printer);
#endif


class QtJambiExceptionUnraiser{
public:
    QtJambiExceptionUnraiser();
    ~QtJambiExceptionUnraiser();
private:
    quint8 data;
    Q_DISABLE_COPY_MOVE(QtJambiExceptionUnraiser)
};

void reinitializeResettableFlag(JNIEnv * env, const char* property);
bool avoidJNIGlobalRefs(JNIEnv * env);

struct ResettableBoolFlag{
    ResettableBoolFlag(const char* property);
    ResettableBoolFlag(JNIEnv * env, const char* property);
    ~ResettableBoolFlag();
    operator bool();
private:
    qint8 value : 1;
    void reinitialize(JNIEnv * env, const char* property);
#ifdef Q_OS_ANDROID
    friend void reinitializeResettableFlags(JNIEnv * env);
#endif
    friend void reinitializeResettableFlag(JNIEnv * env, const char* property);
    friend void clearResettableFlags();
};

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
#define QtJambiMetaType QMetaType
#else
class QtJambiMetaType{
public:
    inline QtJambiMetaType() : m_metaType(new QMetaType()) {}
    inline QtJambiMetaType(const QtJambiMetaType& other) : m_metaType(other.m_metaType) {}
    inline QtJambiMetaType(int metaType) : m_metaType(new QMetaType(metaType)) {}
    inline QtJambiMetaType(const QMetaType& metaType) : m_metaType(new QMetaType(metaType.id())) {}
    inline QtJambiMetaType& operator=(int metaType){
        if(m_metaType->id()!=metaType)
            m_metaType.reset(new QMetaType(metaType));
        return *this;
    }
    inline QtJambiMetaType& operator=(const QMetaType& metaType){
        if(m_metaType->id()!=metaType.id())
            m_metaType.reset(new QMetaType(metaType.id()));
        return *this;
    }
    inline bool isValid() const { return m_metaType->isValid(); }
    inline QByteArray name() const { return m_metaType->name(); }
    inline int id() const { return m_metaType->id(); }
    inline operator int() const { return m_metaType->id(); }
    inline QMetaType::TypeFlags flags() const { return m_metaType->flags(); }
    inline int sizeOf() const { return m_metaType->sizeOf(); }
    inline operator const QMetaType&() const { return *m_metaType; }
    inline const QMetaType& metaType() const { return *m_metaType; }
    inline void *create(const void *copy = nullptr) const{
        return QMetaType::create(id(), copy);
    }
    inline void destroy(void *data) const{
        QMetaType::destroy(id(), data);
    }
    inline void *construct(void *where, const void *copy = nullptr) const{
        return QMetaType::construct(id(), where, copy);
    }
    inline void destruct(void *data) const{
        QMetaType::destruct(id(), data);
    }
private:
    QSharedPointer<QMetaType> m_metaType;
};
#endif

#endif // QTJAMBI_UTILS_P_H
