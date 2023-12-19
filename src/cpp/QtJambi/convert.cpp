/****************************************************************************
**
** Copyright (C) 2009-2023 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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

#include "typemanager_p.h"
#include "exception.h"
#include "java_p.h"
#include "registryutil_p.h"
#include "supertypeinfo_p.h"
#include "qtjambilink_p.h"
#include "qtjambishell_p.h"
#include "containeraccess_p.h"
#include "qtjambimetaobject_p.h"

#include "qtjambi_cast.h"

// enum helpers...

template<typename INT>
jobject convertEnumToJavaObject(JNIEnv *env, INT qt_enum, jclass cl, const QString& className, jobject (*resolveEnum)(JNIEnv *, jint, jclass, INT, jstring))
{
    Q_ASSERT(cl);

    jobject obj;
    QString sig;
    switch ( sizeof(INT) ) {
    case 1:  sig = QLatin1String("(B)L%1;"); break;
    case 2:  sig = QLatin1String("(S)L%1;"); break;
    case 8:  sig = QLatin1String("(J)L%1;"); break;
    default: sig = QLatin1String("(I)L%1;"); break;
    }
    jmethodID method = JavaAPI::resolveMethod(env, "resolve", qPrintable(sig.arg(className)), cl, true);
    if(method){
        obj = env->CallStaticObjectMethod(cl, method, qt_enum);
        JavaException::check(env QTJAMBI_STACKTRACEINFO );
    }else{
        jint hashCode = Java::Runtime::Object::hashCode(env,cl);
        obj = resolveEnum(env, hashCode, cl, qt_enum, nullptr);
    }
    return obj;
}

jobject QtJambiAPI::convertEnumToJavaObject(JNIEnv *env, qint32 qt_enum, jclass cls)
{
    return convertEnumToJavaObject<jint>(env, jint(qt_enum), cls, QtJambiAPI::getClassName(env, cls).replace(QLatin1Char('.'), QLatin1Char('/')), &resolveIntEnum);
}

jobject QtJambiAPI::convertEnumToJavaObject(JNIEnv *env, qint16 qt_enum, jclass cls)
{
    return convertEnumToJavaObject<jshort>(env, jshort(qt_enum), cls, QtJambiAPI::getClassName(env, cls).replace(QLatin1Char('.'), QLatin1Char('/')), &resolveShortEnum);
}

jobject QtJambiAPI::convertEnumToJavaObject(JNIEnv *env, qint8 qt_enum, jclass cls)
{
    return convertEnumToJavaObject<jbyte>(env, jbyte(qt_enum), cls, QtJambiAPI::getClassName(env, cls).replace(QLatin1Char('.'), QLatin1Char('/')), &resolveByteEnum);
}

jobject QtJambiAPI::convertEnumToJavaObject(JNIEnv *env, qint64 qt_enum, jclass cls)
{
    return convertEnumToJavaObject<jlong>(env, jlong(qt_enum), cls, QtJambiAPI::getClassName(env, cls).replace(QLatin1Char('.'), QLatin1Char('/')), &resolveLongEnum);
}

jobject QtJambiAPI::convertEnumToJavaObject(JNIEnv *env, quint32 qt_enum, jclass cls)
{
    return convertEnumToJavaObject<jint>(env, jint(qt_enum), cls, QtJambiAPI::getClassName(env, cls).replace(QLatin1Char('.'), QLatin1Char('/')), &resolveIntEnum);
}

jobject QtJambiAPI::convertEnumToJavaObject(JNIEnv *env, quint16 qt_enum, jclass cls)
{
    return convertEnumToJavaObject<jshort>(env, jshort(qt_enum), cls, QtJambiAPI::getClassName(env, cls).replace(QLatin1Char('.'), QLatin1Char('/')), &resolveShortEnum);
}

jobject QtJambiAPI::convertEnumToJavaObject(JNIEnv *env, quint8 qt_enum, jclass cls)
{
    return convertEnumToJavaObject<jbyte>(env, jbyte(qt_enum), cls, QtJambiAPI::getClassName(env, cls).replace(QLatin1Char('.'), QLatin1Char('/')), &resolveByteEnum);
}

jobject QtJambiAPI::convertEnumToJavaObject(JNIEnv *env, quint64 qt_enum, jclass cls)
{
    return convertEnumToJavaObject<jlong>(env, jlong(qt_enum), cls, QtJambiAPI::getClassName(env, cls).replace(QLatin1Char('.'), QLatin1Char('/')), &resolveLongEnum);
}

jobject QtJambiAPI::convertQFlagsToJavaObject(JNIEnv *env, int qt_flags, jclass cl)
{
    jobject obj = nullptr;
    Q_ASSERT(cl);
    if(Java::QtJambi::QtFlagEnumerator::isAssignableFrom(env, cl)){
        cl = Java::QtCore::QMetaEnum::flagsType(env, cl);
    }
    if(Java::QtJambi::QFlags::isAssignableFrom(env, cl)){
        if(Java::QtJambi::QFlags::isSameClass(env, cl)){
            cl = Java::QtJambi::QFlags$ConcreteWrapper::getClass(env);
        }
        jmethodID method = JavaAPI::resolveMethod(env, "<init>", "(I)V", cl);
        if(method){
            obj = env->NewObject(cl, method, qt_flags);
        }else{
            method = JavaAPI::resolveMethod(env, "<init>", "()V", cl);
            if(method){
                obj = env->NewObject(cl, method);
                JavaException::check(env QTJAMBI_STACKTRACEINFO );
            }else{
                jclass enumClass = Java::QtJambi::EnumUtility::getEnumForQFlags(env, cl);
                QString args = QLatin1String("(L%1;)V");
                args = args.arg(QtJambiAPI::getClassName(env, enumClass).replace(".", "/"));
                method = JavaAPI::resolveMethod(env, "<init>", qPrintable(args), cl);
                if(method){
                    obj = env->NewObject(cl, method, env->NewObjectArray(0, enumClass, nullptr));
                    JavaException::check(env QTJAMBI_STACKTRACEINFO );
                }else{
                    obj = env->AllocObject(cl);
                    JavaException::check(env QTJAMBI_STACKTRACEINFO );
                }
            }
            Java::QtJambi::QFlags::setValue(env, obj, qt_flags);
        }
    }
    return obj;
}

void *QtJambiAPI::convertQNativePointerToNative(JNIEnv *env, jobject java_object, int indirections)
{
    if (!java_object)
        return nullptr;
    Q_ASSERT(Java::QtJambi::QNativePointer::isInstanceOf(env, java_object));  // check the java object is right type
    int object_indirections = Java::QtJambi::QNativePointer::indirections(env, java_object);
    // What is this != test doing ?
    if (object_indirections != indirections) {
        JavaException::raiseIllegalArgumentException(env, "Illegal number of indirections" QTJAMBI_STACKTRACEINFO );
        return nullptr;
    }
    return reinterpret_cast<void *>(Java::QtJambi::QNativePointer::pointer(env,java_object));
}

jobject QtJambiAPI::convertNativeToQNativePointer(JNIEnv *env, const void *qt_object, QNativePointer::Type type, int indirections)
{
    return Java::QtJambi::QNativePointer::fromNative(env, reinterpret_cast<jlong>(qt_object), int(type), jlong(-1), indirections, false);
}

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
namespace QtAnyStringView{
jstring convert(JNIEnv *env, QUtf8StringView s){
    jstring str = env->NewStringUTF(s.data());
    JavaException::check(env QTJAMBI_STACKTRACEINFO );
    return str;
}

jstring convert(JNIEnv *env, QStringView s){
    Q_ASSERT(s.length()>=0);
    jstring str = env->NewString(reinterpret_cast<const jchar *>(s.data()), jsize(s.length()));
    JavaException::check(env QTJAMBI_STACKTRACEINFO );
    return str;
}

jstring convert(JNIEnv *env, QLatin1String s){
    jstring str = env->NewStringUTF(s.data());
    JavaException::check(env QTJAMBI_STACKTRACEINFO );
    return str;
}
}

jstring QtJambiAPI::convertNativeToJavaObject(JNIEnv *env, QAnyStringView s)
{
    return s.visit([env](auto text){return QtAnyStringView::convert(env, text);});
}
jstring QtJambiPrivate::convertString(JNIEnv *env, QAnyStringView s)
{
    return QtJambiAPI::convertNativeToJavaObject(env, s);
}
#endif

jobject QtJambiAPI::convertQCharToJavaObject(JNIEnv *env, const QChar &strg)
{
    jobject returned = Java::QtCore::QChar::newInstance(env, nullptr);
    QChar* sptr = new QChar(strg);
    if (!QtJambiLink::createLinkForNativeObject(
                env,
                returned,
                sptr,
                LINK_NAME_ARG("QChar")
                false,
                false, [](void * ptr,bool){delete reinterpret_cast<QChar*>(ptr);}, QtJambiLink::Ownership::Java)) {
        delete sptr;
        returned = nullptr;
    }
    return returned;
}

jobject QtJambiAPI::convertQCharToJavaObject(JNIEnv *env, QChar *strg)
{
    jobject returned = Java::QtCore::QChar::newInstance(env, nullptr);
    if (!QtJambiLink::createLinkForNativeObject(
                env,
                returned,
                strg,
                LINK_NAME_ARG("QChar")
                false,
                false, [](void * ptr,bool){delete reinterpret_cast<QChar*>(ptr);}, QtJambiLink::Ownership::None)) {
        returned = nullptr;
    }
    return returned;
}

jobject QtJambiAPI::convertQVariantToJavaVariant(JNIEnv *env, const QVariant &variant)
{
    jobject returned = Java::QtCore::QVariant::newInstance(env, nullptr);
    QVariant* vptr = new QVariant(variant);
    if (!QtJambiLink::createLinkForNativeObject(
                env,
                returned,
                vptr,
                LINK_NAME_ARG("QVariant")
                false,
                false, [](void * ptr,bool){delete reinterpret_cast<QVariant*>(ptr);}, QtJambiLink::Ownership::Java)) {
        delete vptr;
        returned = nullptr;
    }
    return returned;
}

jobject QtJambiAPI::convertQVariantToJavaVariant(JNIEnv *env, QVariant *variant)
{
    jobject returned = Java::QtCore::QVariant::newInstance(env, nullptr);
    if (!QtJambiLink::createLinkForNativeObject(
                env,
                returned,
                variant,
                LINK_NAME_ARG("QVariant")
                false,
                false, [](void * ptr,bool){delete reinterpret_cast<QVariant*>(ptr);}, QtJambiLink::Ownership::None)) {
        returned = nullptr;
    }
    return returned;
}

jobject QtJambiAPI::convertQStringToJavaObject(JNIEnv *env, const QString &strg)
{
    jobject returned = Java::QtCore::QString::newInstance(env, nullptr);
    QString* sptr = new QString(strg);
    if (!QtJambiLink::createLinkForNativeObject(
                env,
                returned,
                sptr,
                LINK_NAME_ARG("QString")
                false,
                false, [](void * ptr,bool){delete reinterpret_cast<QString*>(ptr);}, QtJambiLink::Ownership::Java)) {
        delete sptr;
        returned = nullptr;
    }
    return returned;
}

jobject QtJambiAPI::convertQStringToJavaObject(JNIEnv *env, QString *strg)
{
    jobject returned = Java::QtCore::QString::newInstance(env, nullptr);
    if (!QtJambiLink::createLinkForNativeObject(
                env,
                returned,
                strg,
                LINK_NAME_ARG("QString")
                false,
                false, [](void * ptr,bool){delete reinterpret_cast<QString*>(ptr);}, QtJambiLink::Ownership::None)) {
        returned = nullptr;
    }
    return returned;
}

// converters...

jobject QtJambiAPI::convertQVariantToJavaObject(JNIEnv *env, const QVariant &qt_variant)
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QMetaType metaType(qt_variant.type() == QVariant::UserType ? qt_variant.userType() : int(qt_variant.type()));
#else
    QMetaType metaType = qt_variant.metaType();
#endif
    switch (metaType.id()) {
    case QMetaType::UnknownType:
        return nullptr;
    case QMetaType::Nullptr:
    case QMetaType::Void:
        return QtJambiAPI::convertQVariantToJavaVariant(env, qt_variant);
    case QMetaType::SChar:
    case QMetaType::Char:
    case QMetaType::UChar:
        return Java::Runtime::Byte::valueOf(env, qt_variant.value<qint8>());
    case QMetaType::UShort:
    case QMetaType::Short:
        return Java::Runtime::Short::valueOf(env, qt_variant.value<qint16>());
    case QMetaType::Int:
    case QMetaType::UInt:
        return Java::Runtime::Integer::valueOf(env, qt_variant.toInt());
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    case QMetaType::Char16:
        return Java::Runtime::Character::valueOf(env, jchar(qt_variant.value<char16_t>()));
    case QMetaType::Char32:
        return Java::Runtime::Integer::valueOf(env, jchar(qt_variant.value<char32_t>()));
#endif
    case QMetaType::QChar:
        return Java::Runtime::Character::valueOf(env, jchar(qt_variant.toChar().unicode()));
    case QMetaType::Float:
        return Java::Runtime::Float::valueOf(env, qt_variant.toFloat());
    case QMetaType::Double:
        return Java::Runtime::Double::valueOf(env, qt_variant.toDouble());
    case QMetaType::QString:
        return qtjambi_cast<jstring>(env, qt_variant.toString());
    case QMetaType::Long:
    case QMetaType::ULong:
        if(metaType.sizeOf()==4)
            return Java::Runtime::Integer::valueOf(env, qt_variant.toInt());
        else
            return Java::Runtime::Long::valueOf(env, qt_variant.toLongLong());
    case QMetaType::LongLong:
    case QMetaType::ULongLong:
        return Java::Runtime::Long::valueOf(env, qt_variant.toLongLong());
    case QMetaType::Bool:
        return Java::Runtime::Boolean::valueOf(env, qt_variant.toBool());
    case QMetaType::QStringList:
        return qtjambi_cast<jobject>(env, qt_variant.value<QStringList>());
    case QMetaType::QByteArrayList:
        return qtjambi_cast<jobject>(env, qt_variant.value<QByteArrayList>());
    case QMetaType::QVariantMap:
        return qtjambi_cast<jobject>(env, qt_variant.value<QVariantMap>());
    case QMetaType::QVariantHash:
        return qtjambi_cast<jobject>(env, qt_variant.value<QVariantHash>());
    case QMetaType::QVariantList:
        return qtjambi_cast<jobject>(env, qt_variant.value<QVariantList>());
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    case QMetaType::QVariantPair:
        return qtjambi_cast<jobject>(env, qt_variant.value<QVariantPair>());
#endif
    case QMetaType::QObjectStar:
        return qtjambi_cast<jobject>(env, qt_variant.value<QObject*>());
    default:
        break;
    }

    // generic java object
    if (metaType == QMetaType::fromType<qint8>()
            || metaType == QMetaType::fromType<quint8>()) {
        return Java::Runtime::Byte::valueOf(env, jbyte(qt_variant.value<qint8>()));
    }else if (metaType == QMetaType::fromType<qint16>()
            || metaType == QMetaType::fromType<quint16>()) {
        return Java::Runtime::Short::valueOf(env, jshort(qt_variant.value<qint16>()));
    }else if (metaType == QMetaType::fromType<float>()) {
        return Java::Runtime::Float::valueOf(env, qt_variant.value<float>());
    }else if (metaType == QMetaType::fromType<JObjectWrapper>()) {
        JObjectWrapper wrapper = qt_variant.value<JObjectWrapper>();
        return env->NewLocalRef(wrapper.object());
    }else if (metaType == QMetaType::fromType<JIntArrayWrapper>()) {
        JObjectWrapper wrapper = qt_variant.value<JIntArrayWrapper>();
        return env->NewLocalRef(wrapper.object());
    }else if (metaType == QMetaType::fromType<JByteArrayWrapper>()) {
        JObjectWrapper wrapper = qt_variant.value<JByteArrayWrapper>();
        return env->NewLocalRef(wrapper.object());
    }else if (metaType == QMetaType::fromType<JShortArrayWrapper>()) {
        JObjectWrapper wrapper = qt_variant.value<JShortArrayWrapper>();
        return env->NewLocalRef(wrapper.object());
    }else if (metaType == QMetaType::fromType<JLongArrayWrapper>()) {
        JObjectWrapper wrapper = qt_variant.value<JLongArrayWrapper>();
        return env->NewLocalRef(wrapper.object());
    }else if (metaType == QMetaType::fromType<JBooleanArrayWrapper>()) {
        JObjectWrapper wrapper = qt_variant.value<JBooleanArrayWrapper>();
        return env->NewLocalRef(wrapper.object());
    }else if (metaType == QMetaType::fromType<JCharArrayWrapper>()) {
        JObjectWrapper wrapper = qt_variant.value<JCharArrayWrapper>();
        return env->NewLocalRef(wrapper.object());
    }else if (metaType == QMetaType::fromType<JDoubleArrayWrapper>()) {
        JObjectWrapper wrapper = qt_variant.value<JDoubleArrayWrapper>();
        return env->NewLocalRef(wrapper.object());
    }else if (metaType == QMetaType::fromType<JFloatArrayWrapper>()) {
        JObjectWrapper wrapper = qt_variant.value<JFloatArrayWrapper>();
        return env->NewLocalRef(wrapper.object());
    }else if (metaType == QMetaType::fromType<JObjectArrayWrapper>()) {
        JObjectWrapper wrapper = qt_variant.value<JObjectArrayWrapper>();
        return env->NewLocalRef(wrapper.object());
    } else if (metaType == QMetaType::fromType<JEnumWrapper>()) {
        JEnumWrapper wrapper = qt_variant.value<JEnumWrapper>();
        return env->NewLocalRef(wrapper.object());
    } else if (metaType == QMetaType::fromType<JCollectionWrapper>()
               || metaType == QMetaType::fromType<JMapWrapper>()
               || metaType == QMetaType::fromType<JIteratorWrapper>()) {
        JCollectionWrapper wrapper = qt_variant.value<JCollectionWrapper>();
        return env->NewLocalRef(wrapper.object());
    } else if (metaType == QMetaType::fromType<JMapWrapper>()) {
         JMapWrapper wrapper = qt_variant.value<JMapWrapper>();
         return env->NewLocalRef(wrapper.object());
    } else if (metaType == QMetaType::fromType<JIteratorWrapper>()) {
         JIteratorWrapper wrapper = qt_variant.value<JIteratorWrapper>();
         return env->NewLocalRef(wrapper.object());
     } else {
        QString qtType = QLatin1String(metaType.name());
        QString javaType = QtJambiTypeManager::getExternalTypeName(env, qtType, metaType);
        // Find usage pattern
        InternalToExternalConverter converter = QtJambiTypeManager::getInternalToExternalConverter(
                                                env,
                                                qtType,
                                                metaType,
                                                JavaAPI::resolveClass(env, qPrintable(javaType)),
                                                true);
        jvalue val;
        val.l = nullptr;
        if(!converter || !converter(env, nullptr, qt_variant.constData(), val, true)){
            qCWarning(DebugAPI::internalCategory, "Unable to convert qVariant to jobject");
        }
        return val.l;
    }
}

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
void QtJambiAPI::convertJavaObjectToQModelRoleData(JNIEnv *env, QtJambiScope& scope, jobject java_object, void * &data, qsizetype &length){
    length = Java::Runtime::Map::size(env, java_object);
    if(length>0){
        struct ModelRoleData
        {
            int m_role = 0;
            QVariant m_data;
        };
        ModelRoleData* _data = new ModelRoleData[size_t(length)];
        scope.addArrayDeletion(_data);
        data = _data;
        jobject iter = QtJambiAPI::entrySetIteratorOfJavaMap(env, java_object);
        size_t i=0;
        while(QtJambiAPI::hasJavaIteratorNext(env, iter)){
            jobject entry = QtJambiAPI::nextOfJavaIterator(env, iter);
            _data[i].m_role = QtJambiAPI::fromJavaIntegerObject(env, QtJambiAPI::keyOfJavaMapEntry(env, entry));
            _data[i].m_data = QtJambiAPI::convertJavaObjectToQVariant(env, QtJambiAPI::valueOfJavaMapEntry(env, entry));
            ++i;
        }
    }
}
#endif

int getMetaTypeIdForObject(JNIEnv *env, jobject java_object){
    if (!java_object)
        return QMetaType::Nullptr;
    jclass object_class = env->GetObjectClass(java_object);
    if (Java::QtCore::QMetaType$GenericTypeInterface::isSameClass(env, object_class)) {
        return qtjambi_cast<const QMetaType&>(env, Java::QtCore::QMetaType$GenericTypeInterface::metaType(env, java_object)).id();
    } else if (Java::Runtime::String::isSameClass(env, object_class)) {
        return QMetaType::QString;
    } else if (Java::Runtime::Integer::isSameClass(env, object_class)) {
        return QMetaType::Int;
    } else if (Java::Runtime::Character::isSameClass(env, object_class)) {
        return QMetaType::QChar;
    } else if (Java::Runtime::Double::isSameClass(env, object_class)) {
        return QMetaType::Double;
    } else if (Java::Runtime::Float::isSameClass(env, object_class)) {
        return QMetaType::Float;
    } else if (Java::Runtime::Short::isSameClass(env, object_class)) {
        return QMetaType::Short;
    } else if (Java::Runtime::Byte::isSameClass(env, object_class)) {
        return QMetaType::SChar;
    } else if (Java::Runtime::Long::isSameClass(env, object_class)) {
        return QMetaType::LongLong;
    } else if (Java::Runtime::Boolean::isSameClass(env, object_class)) {
        return QMetaType::Bool;
    }else if (Java::QtCore::QList::isInstanceOf(env, java_object)) {
        if (!Java::QtCore::QList::isSameClass(env, object_class)) {
            if(const std::type_info* typeId = getTypeByJavaName(QtJambiAPI::getClassName(env, object_class))){
                int metaType = registeredMetaTypeID(*typeId);
                if(metaType!=QMetaType::UnknownType){
                    return metaType;
                }
            }
        }
        if(QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForJavaObject(env, java_object)){
            if(AbstractListAccess* containerAccess = dynamic_cast<AbstractListAccess*>(link->containerAccess())){
                if(QByteArray(containerAccess->elementMetaType().name()).contains("*"))
                    return qMetaTypeId<JCollectionWrapper>();
                QByteArray qtName;
                if(Java::QtCore::QQueue::isInstanceOf(env, java_object)){
                    qtName = "QQueue<";
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
                }else if(Java::QtCore::QStack::isInstanceOf(env, java_object)){
                    qtName = "QStack<";
#endif
                }else{
                    qtName = "QList<";
                }
                qtName += containerAccess->elementMetaType().name();
                qtName += ">";
                return containerAccess->registerContainer(qtName);
            }
        }
        return qMetaTypeId<JCollectionWrapper>();
    }else if (Java::QtCore::QSet::isInstanceOf(env, java_object)) {
        if (!Java::QtCore::QSet::isSameClass(env, object_class)) {
            if(const std::type_info* typeId = getTypeByJavaName(QtJambiAPI::getClassName(env, object_class))){
                int metaType = registeredMetaTypeID(*typeId);
                if(metaType!=QMetaType::UnknownType){
                    return metaType;
                }
            }
        }
        if(QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForJavaObject(env, java_object)){
            if(AbstractSetAccess* containerAccess = dynamic_cast<AbstractSetAccess*>(link->containerAccess())){
                if(QByteArray(containerAccess->elementMetaType().name()).contains("*"))
                    return qMetaTypeId<JCollectionWrapper>();
                QByteArray qtName = "QSet<";
                qtName += containerAccess->elementMetaType().name();
                qtName += ">";
                return containerAccess->registerContainer(qtName);
            }
        }
        return qMetaTypeId<JCollectionWrapper>();
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    }else if (Java::QtCore::QLinkedList::isInstanceOf(env, java_object)) {
        if (!Java::QtCore::QLinkedList::isSameClass(env, object_class)) {
            if(const std::type_info* typeId = getTypeByJavaName(QtJambiAPI::getClassName(env, object_class))){
                int metaType = registeredMetaTypeID(*typeId);
                if(metaType!=QMetaType::UnknownType){
                    return metaType;
                }
            }
        }
        if(QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForJavaObject(env, java_object)){
            if(AbstractLinkedListAccess* containerAccess = dynamic_cast<AbstractLinkedListAccess*>(link->containerAccess())){
                if(QByteArray(containerAccess->elementMetaType().name()).contains("*"))
                    return qMetaTypeId<JCollectionWrapper>();
                QByteArray qtName = "QLinkedList<";
                qtName += containerAccess->elementMetaType().name();
                qtName += ">";
                return containerAccess->registerContainer(qtName);
            }
        }
        return qMetaTypeId<JCollectionWrapper>();
    }else if (Java::QtCore::QVector::isInstanceOf(env, java_object)) {
        if (!Java::QtCore::QVector::isSameClass(env, object_class)) {
            if(const std::type_info* typeId = getTypeByJavaName(QtJambiAPI::getClassName(env, object_class))){
                int metaType = registeredMetaTypeID(*typeId);
                if(metaType!=QMetaType::UnknownType){
                    return metaType;
                }
            }
        }
        if(QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForJavaObject(env, java_object)){
            if(AbstractVectorAccess* containerAccess = dynamic_cast<AbstractVectorAccess*>(link->containerAccess())){
                if(QByteArray(containerAccess->elementMetaType().name()).contains("*"))
                    return qMetaTypeId<JCollectionWrapper>();
                QByteArray qtName;
                if(Java::QtCore::QStack::isInstanceOf(env, java_object)){
                    qtName = "QStack<";
                }else{
                    qtName = "QVector<";
                }
                qtName += containerAccess->elementMetaType().name();
                qtName += ">";
                return containerAccess->registerContainer(qtName);
            }
        }
        return qMetaTypeId<JCollectionWrapper>();
#endif
    }else if (Java::QtCore::QMap::isInstanceOf(env, java_object)) {
        if (!Java::QtCore::QMap::isSameClass(env, object_class)) {
            if(const std::type_info* typeId = getTypeByJavaName(QtJambiAPI::getClassName(env, object_class))){
                int metaType = registeredMetaTypeID(*typeId);
                if(metaType!=QMetaType::UnknownType){
                    return metaType;
                }
            }
        }
        if(QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForJavaObject(env, java_object)){
            if(AbstractMapAccess* containerAccess = dynamic_cast<AbstractMapAccess*>(link->containerAccess())){
                if(QByteArray(containerAccess->valueMetaType().name()).contains("*") || QByteArray(containerAccess->keyMetaType().name()).contains("*"))
                    return qMetaTypeId<JMapWrapper>();
                QByteArray qtName = "QMap<";
                qtName += containerAccess->keyMetaType().name();
                qtName += ",";
                qtName += containerAccess->valueMetaType().name();
                qtName += ">";
                return containerAccess->registerContainer(qtName);
            }
        }
        return qMetaTypeId<JCollectionWrapper>();
    }else if (Java::QtCore::QHash::isInstanceOf(env, java_object)) {
        if (!Java::QtCore::QHash::isSameClass(env, object_class)) {
            if(const std::type_info* typeId = getTypeByJavaName(QtJambiAPI::getClassName(env, object_class))){
                int metaType = registeredMetaTypeID(*typeId);
                if(metaType!=QMetaType::UnknownType){
                    return metaType;
                }
            }
        }
        if(QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForJavaObject(env, java_object)){
            if(AbstractHashAccess* containerAccess = dynamic_cast<AbstractHashAccess*>(link->containerAccess())){
                if(QByteArray(containerAccess->valueMetaType().name()).contains("*") || QByteArray(containerAccess->keyMetaType().name()).contains("*"))
                    return qMetaTypeId<JMapWrapper>();
                QByteArray qtName = "QHash<";
                qtName += containerAccess->keyMetaType().name();
                qtName += ",";
                qtName += containerAccess->valueMetaType().name();
                qtName += ">";
                return containerAccess->registerContainer(qtName);
            }
        }
        return qMetaTypeId<JCollectionWrapper>();
    }else if (Java::QtCore::QMultiMap::isInstanceOf(env, java_object)) {
        if (!Java::QtCore::QMultiMap::isSameClass(env, object_class)) {
            if(const std::type_info* typeId = getTypeByJavaName(QtJambiAPI::getClassName(env, object_class))){
                int metaType = registeredMetaTypeID(*typeId);
                if(metaType!=QMetaType::UnknownType){
                    return metaType;
                }
            }
        }
        if(QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForJavaObject(env, java_object)){
            if(AbstractMultiMapAccess* containerAccess = dynamic_cast<AbstractMultiMapAccess*>(link->containerAccess())){
                if(QByteArray(containerAccess->valueMetaType().name()).contains("*") || QByteArray(containerAccess->keyMetaType().name()).contains("*"))
                    return qMetaTypeId<JMapWrapper>();
                QByteArray qtName = "QMultiMap<";
                qtName += containerAccess->keyMetaType().name();
                qtName += ",";
                qtName += containerAccess->valueMetaType().name();
                qtName += ">";
                return containerAccess->registerContainer(qtName);
            }
        }
        return qMetaTypeId<JCollectionWrapper>();
    }else if (Java::QtCore::QMultiHash::isInstanceOf(env, java_object)) {
        if (!Java::QtCore::QMultiHash::isSameClass(env, object_class)) {
            if(const std::type_info* typeId = getTypeByJavaName(QtJambiAPI::getClassName(env, object_class))){
                int metaType = registeredMetaTypeID(*typeId);
                if(metaType!=QMetaType::UnknownType){
                    return metaType;
                }
            }
        }
        if(QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForJavaObject(env, java_object)){
            if(AbstractMultiHashAccess* containerAccess = dynamic_cast<AbstractMultiHashAccess*>(link->containerAccess())){
                if(QByteArray(containerAccess->valueMetaType().name()).contains("*") || QByteArray(containerAccess->keyMetaType().name()).contains("*"))
                    return qMetaTypeId<JMapWrapper>();
                QByteArray qtName = "QMultiHash<";
                qtName += containerAccess->keyMetaType().name();
                qtName += ",";
                qtName += containerAccess->valueMetaType().name();
                qtName += ">";
                return containerAccess->registerContainer(qtName);
            }
        }
        return qMetaTypeId<JCollectionWrapper>();
    }else{
        return CoreAPI::metaTypeId(env, object_class);
    }
}

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
#define META_TYPE(t) t
#define FROM_META_TYPE(t) t.id()
#define META_TYPE_FROM(t) qMetaTypeId<t>()
#else
#define META_TYPE(t) QMetaType(t)
#define FROM_META_TYPE(t) t
#define META_TYPE_FROM(t) QMetaType::fromType<t>()

QMetaType qtjambiRegisterMetaType(JNIEnv *env, jclass clazz, const QByteArray& _type, const QMetaObject *metaObject, bool isQObject){
    QMetaType metaType(isQObject ? QMetaType::QObjectStar : QMetaType::VoidStar);
    const QtPrivate::QMetaTypeInterface* iface = metaType.iface();
    QMetaType _metaType = createMetaType(QMetaObject::normalizedType(_type),
                                         true,
                                         iface->defaultCtr,
                                         iface->copyCtr,
                                         iface->moveCtr,
                                         iface->dtor,
                                         iface->equals,
                                         iface->lessThan,
                                         iface->debugStream,
                                         iface->dataStreamOut,
                                         iface->dataStreamIn,
                                         iface->legacyRegisterOp,
                                         uint(iface->size),
                                         ushort(iface->alignment),
                                         QMetaType::UnknownType,
                                         QMetaType::TypeFlags(int(iface->flags)),
                                         metaObject, nullptr);
    registerJavaClassForCustomMetaType(env, _metaType, clazz);
    _metaType.id();
    return _metaType;
}

bool trivialConverter(const void *src, void *target) {
    *reinterpret_cast<void**>(target) = *reinterpret_cast<void* const*>(src);
    return true;
};

void registerConverterVariant(JNIEnv *env, QMetaType metaType, QString qtName, const QString& fullJavaName, jclass clazz){
    QMetaType jObjectWrapperType = QMetaType::fromType<JObjectWrapper>();
    if(metaType.isValid() && metaType!=jObjectWrapperType){
        if(Java::QtCore::QObject::isAssignableFrom(env, clazz)){
            if(!QMetaType::hasRegisteredConverterFunction(jObjectWrapperType, metaType))
                QMetaType::registerConverterFunction([](const void *src, void *target) -> bool {
                                                        if(JniEnvironment env{200}){
                                                            if(src){
                                                                const JObjectWrapper* wrapper = reinterpret_cast<const JObjectWrapper*>(src);
                                                                if(Java::QtCore::QObject::isInstanceOf(env, wrapper->object())){
                                                                    QObject* object = QtJambiAPI::convertJavaObjectToQObject(env, wrapper->object());
                                                                    if(target)
                                                                        *reinterpret_cast<void**>(target) = object;
                                                                    return target && object;
                                                                }else if(env->IsSameObject(nullptr, wrapper->object())){
                                                                    if(target)
                                                                        *reinterpret_cast<void**>(target) = nullptr;
                                                                    return target;
                                                                }else{
                                                                    return false;
                                                                }
                                                            }else{
                                                                if(target)
                                                                    *reinterpret_cast<void**>(target) = nullptr;
                                                                return target;
                                                            }
                                                        }
                                                        return false;
                                                    }, jObjectWrapperType, metaType);
            if(!QMetaType::hasRegisteredConverterFunction(metaType, jObjectWrapperType))
                QMetaType::registerConverterFunction([](const void *src, void *target) -> bool {
                                                         if(JniEnvironment env{200}){
                                                             jobject object = QtJambiAPI::convertQObjectToJavaObject(env, *reinterpret_cast<QObject* const*>(src));
                                                             new (target)JObjectWrapper(env, object);
                                                             return true;
                                                         }
                                                         return false;
                                                     }, metaType, jObjectWrapperType);
        }else{
            if(!QMetaType::hasRegisteredConverterFunction(jObjectWrapperType, metaType)){
                ExternalToInternalConverter converter = QtJambiTypeManager::tryGetExternalToInternalConverter(env, clazz, qtName, metaType);
                clazz = getGlobalClassRef(env, clazz);
                if(converter){
                    QMetaType::registerConverterFunction([clazz, metaType, converter](const void *src, void *target) -> bool {
                        if(src){
                            if(JniEnvironment env{500}){
                                const JObjectWrapper * wrapper = reinterpret_cast<const JObjectWrapper *>(src);
                                if(!wrapper->object() || env->IsInstanceOf(wrapper->object(), clazz)){
                                    if(!target)
                                        metaType.construct(target, nullptr);
                                    if(!wrapper->object()){
                                        return true;
                                    }else{
                                        JniLocalFrame __jniLocalFrame(env, 200);
                                        jvalue val;
                                        val.l = wrapper->object();
                                        return converter(env, nullptr, val, target, jValueType::l);
                                    }
                                }
                            }
                        }
                        return false;
                    }, jObjectWrapperType, metaType);
                }else{
                    QMetaType::registerConverterFunction([clazz, metaType, qtName, converter](const void *src, void *target) mutable -> bool {
                        if(src){
                            if(JniEnvironment env{500}){
                                if(!converter){
                                    converter = QtJambiTypeManager::getExternalToInternalConverter(env, clazz, qtName, metaType);
                                    Q_ASSERT(converter);
                                }
                                const JObjectWrapper * wrapper = reinterpret_cast<const JObjectWrapper *>(src);
                                if(!wrapper->object() || env->IsInstanceOf(wrapper->object(), clazz)){
                                    if(!target)
                                        metaType.construct(target, nullptr);
                                    if(!wrapper->object()){
                                        return true;
                                    }else{
                                        JniLocalFrame __jniLocalFrame(env, 200);
                                        jvalue val;
                                        val.l = wrapper->object();
                                        return converter(env, nullptr, val, target, jValueType::l);
                                    }
                                }
                            }
                        }
                        return false;
                    }, jObjectWrapperType, metaType);
                }
            }
            if(!QMetaType::hasRegisteredConverterFunction(metaType, jObjectWrapperType)){
                InternalToExternalConverter converter = QtJambiTypeManager::tryGetInternalToExternalConverter(env, qtName, metaType, clazz);
                if(converter){
                    QMetaType::registerConverterFunction([converter](const void *src, void *target) -> bool {
                        if(!src){
                            new (target)JObjectWrapper();
                            return true;
                        }else if(JniEnvironment env{500}){
                            jvalue val;
                            val.l = nullptr;
                            if(converter(env, nullptr, src, val, true)){
                                new (target)JObjectWrapper(env, val.l);
                                return true;
                            }
                        }
                        return false;
                    }, metaType, jObjectWrapperType);
                }else{
                    clazz = getGlobalClassRef(env, clazz);
                    QMetaType::registerConverterFunction([converter, qtName, metaType, clazz](const void *src, void *target) mutable -> bool {
                        if(!src){
                            new (target)JObjectWrapper();
                            return true;
                        }else if(JniEnvironment env{500}){
                            if(!converter){
                                converter = QtJambiTypeManager::getInternalToExternalConverter(env, qtName, metaType, clazz);
                                Q_ASSERT(converter);
                            }
                            jvalue val;
                            val.l = nullptr;
                            if(converter(env, nullptr, src, val, true)){
                                new (target)JObjectWrapper(env, val.l);
                                return true;
                            }
                        }
                        return false;
                    }, metaType, jObjectWrapperType);
                }
            }
        }
    }
    JObjectArrayWrapper interfaces(env, Java::Runtime::Class::getInterfaces(env, clazz));
    for(int i=0; i<interfaces.length(); ++i){
        if(jclass interfaceClass = jclass(interfaces.at(env, i))){
            if(!Java::QtJambi::QtObjectInterface::isSameClass(env, interfaceClass)
                    && !Java::QtJambi::QtObject::isSameClass(env, interfaceClass)
                    && !Java::Runtime::Object::isSameClass(env, interfaceClass)){
                QString _fullJavaName = QtJambiAPI::getClassName(env, interfaceClass).replace(".", "/");
                QString _qtName = QtJambiTypeManager::getInternalTypeName(env, interfaceClass, false);
                QMetaType _metaType = QMetaType::fromName(_qtName.toLatin1());
                const std::type_info* interfaceTypeId = getTypeByJavaName(_fullJavaName);
                if(!interfaceTypeId){
                    if(jclass _object_class = JavaAPI::resolveClosestQtSuperclass(env, interfaceClass)){
                        interfaceClass = _object_class;
                        _fullJavaName = QtJambiAPI::getClassName(env, _object_class).replace(".", "/");
                        interfaceTypeId = getTypeByJavaName(_fullJavaName);
                    }
                }
                if(!_metaType.isValid() && _qtName.endsWith("*") && interfaceTypeId){
                    _metaType = qtjambiRegisterMetaType(env, clazz, _qtName.toUtf8(), registeredOriginalMetaObject(*interfaceTypeId), false);
                }
                if(_metaType.isValid()){
                    if(!QMetaType::hasRegisteredConverterFunction(metaType, _metaType)){
                        if(metaType==jObjectWrapperType
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
                                || JObjectValueWrapper::isValueType(metaType)
#endif
                                || isJObjectWrappedMetaType(metaType)){
                            interfaceClass = getGlobalClassRef(env, interfaceClass);
                            const QSharedPointer<QtJambiLink>& (*findLinkForJavaObject)(JNIEnv*,jobject)
                                    = Java::QtJambi::NativeUtility$Object::isAssignableFrom(env, interfaceClass) ? QtJambiLink::findLinkForJavaObject : QtJambiLink::findLinkForJavaInterface;
                            if(_qtName.endsWith("*")){
                                QMetaType::registerConverterFunction([interfaceClass, interfaceTypeId, findLinkForJavaObject](const void *src, void *target) -> bool {
                                    if(JniEnvironment env{200}){
                                        if(src){
                                            const JObjectWrapper* wrapper = reinterpret_cast<const JObjectWrapper*>(src);
                                            if(!wrapper->object() || env->IsInstanceOf(wrapper->object(), interfaceClass)){
                                                void* object = nullptr;
                                                if(QSharedPointer<QtJambiLink> link = findLinkForJavaObject(env, wrapper->object()))
                                                    object = interfaceTypeId ? link->typedPointer(*interfaceTypeId) : link->pointer();
                                                *reinterpret_cast<void**>(target) = object;
                                                return true;
                                            }else{
                                                return false;
                                            }
                                        }
                                    }
                                    return false;
                                }, metaType, _metaType);
                            }else{
                                QMetaType::registerConverterFunction([interfaceClass, interfaceTypeId, findLinkForJavaObject, _metaType](const void *src, void *target) -> bool {
                                    if(JniEnvironment env{200}){
                                        if(src){
                                            const JObjectWrapper* wrapper = reinterpret_cast<const JObjectWrapper*>(src);
                                            if(env->IsInstanceOf(wrapper->object(), interfaceClass)){
                                                void* object = nullptr;
                                                if(QSharedPointer<QtJambiLink> link = findLinkForJavaObject(env, wrapper->object()))
                                                    object = interfaceTypeId ? link->typedPointer(*interfaceTypeId) : link->pointer();
                                                _metaType.destruct(target);
                                                _metaType.construct(target, object);
                                                return true;
                                            }else{
                                                return false;
                                            }
                                        }
                                    }
                                    return false;
                                }, metaType, _metaType);
                            }
                        }else if(!getTypeByJavaName(fullJavaName) && interfaceTypeId){
                            if(jclass _object_class = JavaAPI::resolveClosestQtSuperclass(env, clazz)){
                                if(const std::type_info* typeId = getTypeByJavaName(QtJambiAPI::getClassName(env, _object_class).replace(".", "/"))){
                                    int offset = registeredInterfaceOffset(*typeId, *interfaceTypeId);
                                    if(offset==0){
                                        QMetaType::registerConverterFunction(&trivialConverter, metaType, _metaType);
                                    }else if(offset>0){
                                        QMetaType::registerConverterFunction([offset](const void *src, void *target) ->bool {
                                                                                 char* ptr = *reinterpret_cast<char*const*>(src);
                                                                                 ptr += offset;
                                                                                 *reinterpret_cast<const void**>(target) = ptr;
                                                                                 return true;
                                                                             }, metaType, _metaType);
                                    }
                                }
                            }
                        }
                    }
                    registerConverterVariant(env, _metaType, _qtName, _fullJavaName, interfaceClass);
                }
            }
        }
    }
    if(!Java::Runtime::Class::isInterface(env, clazz)){
        if(jclass _clazz = env->GetSuperclass(clazz)){
            if(!Java::QtJambi::QtObject::isSameClass(env, _clazz)
                    && !Java::Runtime::Object::isSameClass(env, _clazz)
                    && env->GetArrayLength(Java::Runtime::Class::getTypeParameters(env, _clazz))==0){
                QString _fullJavaName = QtJambiAPI::getClassName(env, _clazz).replace(".", "/");
                QString _qtName = QtJambiTypeManager::getInternalTypeName(env, _clazz, false);
                QMetaType _metaType = QMetaType::fromName(_qtName.toLatin1());
                if(!_metaType.isValid()){
                    if(jclass _object_class = JavaAPI::resolveClosestQtSuperclass(env, _clazz)){
                        _fullJavaName = QtJambiAPI::getClassName(env, _object_class).replace(".", "/");
                        QString __fullJavaName = QtJambiAPI::getClassName(env, _object_class).replace(".", "/");
                        QString __qtName = QtJambiTypeManager::getInternalTypeName(env, _object_class, false);
                        _metaType = QMetaType::fromName(__qtName.toLatin1());
                        if(_metaType.isValid()){
                            _fullJavaName = __fullJavaName;
                            _qtName = __qtName;
                        }
                    }
                }
                const std::type_info* superTypeId = getTypeByJavaName(_fullJavaName);
                if(!superTypeId){
                    if(jclass _object_class = JavaAPI::resolveClosestQtSuperclass(env, _clazz)){
                        _clazz = _object_class;
                        _fullJavaName = QtJambiAPI::getClassName(env, _object_class).replace(".", "/");
                        superTypeId = getTypeByJavaName(_fullJavaName);
                    }
                }
                if(!_metaType.isValid() && _qtName.endsWith("*") && !superTypeId){
                    _metaType = qtjambiRegisterMetaType(env, clazz, _qtName.toUtf8(), registeredOriginalMetaObject(*superTypeId), Java::QtCore::QObject::isAssignableFrom(env, _clazz));
                }
                if(_metaType.isValid()){
                    if(!QMetaType::hasRegisteredConverterFunction(metaType, _metaType)){
                        if(metaType==jObjectWrapperType
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
                                || JObjectValueWrapper::isValueType(metaType)
#endif
                                || isJObjectWrappedMetaType(metaType)){
                            _clazz = getGlobalClassRef(env, _clazz);
                            const QSharedPointer<QtJambiLink>& (*findLinkForJavaObject)(JNIEnv*,jobject)
                                    = Java::QtJambi::NativeUtility$Object::isAssignableFrom(env, _clazz) ? QtJambiLink::findLinkForJavaObject : QtJambiLink::findLinkForJavaInterface;
                            if(_qtName.endsWith("*")){
                                QMetaType::registerConverterFunction([_clazz, findLinkForJavaObject](const void *src, void *target) -> bool {
                                    if(JniEnvironment env{200}){
                                        if(src){
                                            const JObjectWrapper* wrapper = reinterpret_cast<const JObjectWrapper*>(src);
                                            if(!wrapper->object() || env->IsInstanceOf(wrapper->object(), _clazz)){
                                                void* object = nullptr;
                                                if(QSharedPointer<QtJambiLink> link = findLinkForJavaObject(env, wrapper->object()))
                                                    object = link->pointer();
                                                *reinterpret_cast<void**>(target) = object;
                                                return true;
                                            }else{
                                                return false;
                                            }
                                        }
                                    }
                                    return false;
                                }, metaType, _metaType);
                            }else{
                                QMetaType::registerConverterFunction([_clazz, findLinkForJavaObject, _metaType](const void *src, void *target) -> bool {
                                    if(JniEnvironment env{200}){
                                        if(src){
                                            const JObjectWrapper* wrapper = reinterpret_cast<const JObjectWrapper*>(src);
                                            if(env->IsInstanceOf(wrapper->object(), _clazz)){
                                                void* object = nullptr;
                                                if(QSharedPointer<QtJambiLink> link = findLinkForJavaObject(env, wrapper->object()))
                                                    object = link->pointer();
                                                _metaType.destruct(target);
                                                _metaType.construct(target, object);
                                                return true;
                                            }else{
                                                return false;
                                            }
                                        }
                                    }
                                    return false;
                                }, metaType, _metaType);
                            }
                        }else{
                            QMetaType::registerConverterFunction(&trivialConverter, metaType, _metaType);
                        }
                    }
                    registerConverterVariant(env, _metaType, _qtName, _fullJavaName, _clazz);
                }
            }
        }
    }

    if(const std::type_info* typeId = getTypeByJavaName(fullJavaName)){
        jobject interfaceList = Java::QtJambi::ClassAnalyzerUtility::getAllImplementedInterfaces(env, clazz);
        int count = interfaceList ? QtJambiAPI::sizeOfJavaCollection(env, interfaceList) : 0;
        if(count>0){
            jobject iterator = QtJambiAPI::iteratorOfJavaCollection(env, interfaceList);
            while(QtJambiAPI::hasJavaIteratorNext(env, iterator)) {
                jclass interfaceClass = jclass(QtJambiAPI::nextOfJavaIterator(env, iterator));
                if(env->GetArrayLength(Java::Runtime::Class::getTypeParameters(env, interfaceClass))==0){
                    QString _fullJavaName = QtJambiAPI::getClassName(env, interfaceClass).replace(".", "/");
                    if(const std::type_info* interfaceTypeId = getTypeByJavaName(_fullJavaName)){
                        QString _qtName = QtJambiTypeManager::getInternalTypeName(env, interfaceClass, false);
                        QMetaType _metaType = QMetaType::fromName(_qtName.toUtf8());
                        if(_metaType.isValid()){
                            if(!QMetaType::hasRegisteredConverterFunction(metaType, _metaType)){
                                if(metaType==jObjectWrapperType
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
                                        || JObjectValueWrapper::isValueType(metaType)
#endif
                                        || isJObjectWrappedMetaType(metaType)){
                                    if(_qtName.endsWith("*")){
                                        interfaceClass = getGlobalClassRef(env, interfaceClass);
                                        QMetaType::registerConverterFunction([interfaceClass,interfaceTypeId](const void *src, void *target) -> bool {
                                            if(JniEnvironment env{200}){
                                                if(src){
                                                    const JObjectWrapper* wrapper = reinterpret_cast<const JObjectWrapper*>(src);
                                                    if(!wrapper->object() || env->IsInstanceOf(wrapper->object(), interfaceClass)){
                                                        void* object = QtJambiAPI::convertJavaInterfaceToNative(env, wrapper->object(), *interfaceTypeId);
                                                        Q_ASSERT(target);
                                                        *reinterpret_cast<void**>(target) = object;
                                                        return true;
                                                    }else{
                                                        return false;
                                                    }
                                                }
                                            }
                                            return false;
                                        }, metaType, _metaType);
                                    }else{
                                        interfaceClass = getGlobalClassRef(env, interfaceClass);
                                        QMetaType::registerConverterFunction([interfaceClass,interfaceTypeId, _metaType](const void *src, void *target) -> bool {
                                            if(JniEnvironment env{200}){
                                                if(src){
                                                    const JObjectWrapper* wrapper = reinterpret_cast<const JObjectWrapper*>(src);
                                                    if(env->IsInstanceOf(wrapper->object(), interfaceClass)){
                                                        void* object = QtJambiAPI::convertJavaInterfaceToNative(env, wrapper->object(), *interfaceTypeId);
                                                        Q_ASSERT(target);
                                                        _metaType.destruct(target);
                                                        _metaType.construct(target, object);
                                                        return true;
                                                    }else{
                                                        return false;
                                                    }
                                                }
                                            }
                                            return false;
                                        }, metaType, _metaType);
                                    }
                                }else{
                                    int offset = registeredInterfaceOffset(*typeId, *interfaceTypeId);
                                    if(offset==0){
                                        QMetaType::registerConverterFunction(&trivialConverter, metaType, _metaType);
                                    }else if(offset>0){
                                        QMetaType::registerConverterFunction([offset](const void *src, void *target) ->bool {
                                                                                 char* ptr = *reinterpret_cast<char*const*>(src);
                                                                                 ptr += offset;
                                                                                 *reinterpret_cast<const void**>(target) = ptr;
                                                                                 return true;
                                                                             }, metaType, _metaType);
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}
#endif

QVariant internal_convertJavaObjectToQVariant(JNIEnv *env, jobject java_object, bool convert){
    if (!java_object)
        return QVariant();

    jclass object_class = env->GetObjectClass(java_object);
    if (!object_class)
        return QVariant();

    // Test some quick ones first...
    if (Java::QtCore::QVariant::isInstanceOf(env, java_object)) {
        QVariant* variant = QtJambiAPI::convertJavaObjectToNative<QVariant>(env, java_object);
        if(variant)
            return *variant;
        else return QVariant();
    } else if (Java::Runtime::Object::isSameClass(env, object_class)) {
        if(!convert)
            return QVariant(META_TYPE(qMetaTypeId<JObjectWrapper>()), nullptr);
        return QVariant::fromValue(JObjectWrapper(env, java_object));
    } else if (Java::QtCore::QMetaType$GenericEnumerator::isSameClass(env, object_class)) {
        const QMetaType& internalMetaType = qtjambi_cast<const QMetaType&>(env, Java::QtCore::QMetaType$GenericTypeInterface::metaType(env, java_object));
        if(!convert)
            return QVariant(FROM_META_TYPE(internalMetaType), nullptr);
        int value = Java::QtJambi::QtEnumerator::value(env, java_object);
        return QVariant(FROM_META_TYPE(internalMetaType), &value);
    } else if (Java::QtCore::QMetaType$GenericByteEnumerator::isSameClass(env, object_class)) {
        const QMetaType& internalMetaType = qtjambi_cast<const QMetaType&>(env, Java::QtCore::QMetaType$GenericTypeInterface::metaType(env, java_object));
        if(!convert)
            return QVariant(FROM_META_TYPE(internalMetaType), nullptr);
        qint8 value = Java::QtJambi::QtByteEnumerator::value(env, java_object);
        return QVariant(FROM_META_TYPE(internalMetaType), &value);
    } else if (Java::QtCore::QMetaType$GenericShortEnumerator::isSameClass(env, object_class)) {
        const QMetaType& internalMetaType = qtjambi_cast<const QMetaType&>(env, Java::QtCore::QMetaType$GenericTypeInterface::metaType(env, java_object));
        if(!convert)
            return QVariant(FROM_META_TYPE(internalMetaType), nullptr);
        qint16 value = Java::QtJambi::QtShortEnumerator::value(env, java_object);
        return QVariant(FROM_META_TYPE(internalMetaType), &value);
    } else if (Java::QtCore::QMetaType$GenericLongEnumerator::isSameClass(env, object_class)) {
        const QMetaType& internalMetaType = qtjambi_cast<const QMetaType&>(env, Java::QtCore::QMetaType$GenericTypeInterface::metaType(env, java_object));
        if(!convert)
            return QVariant(FROM_META_TYPE(internalMetaType), nullptr);
        qint64 value = Java::QtJambi::QtLongEnumerator::value(env, java_object);
        return QVariant(FROM_META_TYPE(internalMetaType), &value);
    } else if (Java::QtCore::QMetaType$GenericFlags::isSameClass(env, object_class)) {
        const QMetaType& internalMetaType = qtjambi_cast<const QMetaType&>(env, Java::QtCore::QMetaType$GenericTypeInterface::metaType(env, java_object));
        if(!convert)
            return QVariant(FROM_META_TYPE(internalMetaType), nullptr);
        int value = Java::QtJambi::QFlags::value(env, java_object);
        return QVariant(FROM_META_TYPE(internalMetaType), &value);
    } else if (Java::QtCore::QMetaType$GenericValue::isSameClass(env, object_class)) {
        const QMetaType& internalMetaType = qtjambi_cast<const QMetaType&>(env, Java::QtCore::QMetaType$GenericTypeInterface::metaType(env, java_object));
        if(!convert)
            return QVariant(FROM_META_TYPE(internalMetaType), nullptr);
        return QVariant(FROM_META_TYPE(internalMetaType), QtJambiAPI::convertJavaObjectToNative(env, java_object));
    } else if (Java::QtCore::QMetaType$GenericObject::isSameClass(env, object_class)) {
        const QMetaType& internalMetaType = qtjambi_cast<const QMetaType&>(env, Java::QtCore::QMetaType$GenericTypeInterface::metaType(env, java_object));
        if(!convert)
            return QVariant(FROM_META_TYPE(internalMetaType), nullptr);
        void* ptr = QtJambiAPI::convertJavaObjectToNative(env, java_object);
        return QVariant(FROM_META_TYPE(internalMetaType), &ptr);
    } else if (Java::QtCore::QMetaType$GenericGadget::isSameClass(env, object_class)) {
        const QMetaType& internalMetaType = qtjambi_cast<const QMetaType&>(env, Java::QtCore::QMetaType$GenericTypeInterface::metaType(env, java_object));
        if(!convert)
            return QVariant(FROM_META_TYPE(internalMetaType), nullptr);
        void* ptr = QtJambiAPI::convertJavaObjectToNative(env, java_object);
        if(internalMetaType.flags() & QMetaType::PointerToGadget)
            return QVariant(FROM_META_TYPE(internalMetaType), &ptr);
        else
            return QVariant(FROM_META_TYPE(internalMetaType), ptr);
    } else if (Java::Runtime::String::isSameClass(env, object_class)) {
        if(!convert)
            return QVariant(META_TYPE(QMetaType::QString), nullptr);
        return qtjambi_cast<QString>(env, static_cast<jstring>(java_object));
    } else if (Java::QtCore::QString::isSameClass(env, object_class)) {
        if(!convert)
            return QVariant(META_TYPE(QMetaType::QString), nullptr);
        QString* strg = QtJambiAPI::convertJavaObjectToNative<QString>(env, java_object);
        return QVariant(META_TYPE(QMetaType::QString), strg);
    } else if (Java::Runtime::Integer::isSameClass(env, object_class)) {
        if(!convert)
            return QVariant(META_TYPE(QMetaType::Int), nullptr);
        qint32 i = Java::Runtime::Number::intValue(env,java_object);
        return QVariant::fromValue<qint32>(i);
    } else if (Java::Runtime::Character::isSameClass(env, object_class)) {
        if(!convert)
            return QVariant(META_TYPE(QMetaType::QChar), nullptr);
        ushort s = ushort(Java::Runtime::Character::charValue(env, java_object));
        return QVariant::fromValue<QChar>(QChar(s));
    } else if (Java::Runtime::Double::isSameClass(env, object_class)) {
        if(!convert)
            return QVariant(META_TYPE(QMetaType::Double), nullptr);
        double d = Java::Runtime::Number::doubleValue(env, java_object);
        return QVariant::fromValue<double>(d);
    } else if (Java::Runtime::Float::isSameClass(env, object_class)) {
        if(!convert)
            return QVariant(META_TYPE(QMetaType::Float), nullptr);
        float f = Java::Runtime::Number::floatValue(env, java_object);
        return QVariant::fromValue<float>(f);
    } else if (Java::Runtime::Short::isSameClass(env, object_class)) {
        if(!convert)
            return QVariant(META_TYPE(QMetaType::Short), nullptr);
        qint16 i = Java::Runtime::Number::shortValue(env, java_object);
        return QVariant::fromValue<qint16>(i);
    } else if (Java::Runtime::Byte::isSameClass(env, object_class)) {
        if(!convert)
            return QVariant(META_TYPE(QMetaType::SChar), nullptr);
        qint8 i = Java::Runtime::Number::byteValue(env, java_object);
        return QVariant::fromValue<qint8>(i);
    } else if (Java::Runtime::Long::isSameClass(env, object_class)) {
        if(!convert)
            return QVariant(META_TYPE(QMetaType::LongLong), nullptr);
        qint64 i = Java::Runtime::Number::longValue(env, java_object);
        return QVariant::fromValue<qint64>(i);
    } else if (Java::Runtime::Boolean::isSameClass(env, object_class)) {
        if(!convert)
            return QVariant(META_TYPE(QMetaType::Bool), nullptr);
        bool b = Java::Runtime::Boolean::booleanValue(env, java_object);
        return QVariant::fromValue<bool>(b);
    } else if (Java::QtCore::QPair::isInstanceOf(env, java_object)) {
        jobject first = Java::QtCore::QPair::first(env, java_object);
        jobject second = Java::QtCore::QPair::second(env, java_object);
        jclass firstClass = first ? env->GetObjectClass(first) : nullptr;
        jclass secondClass = first ? env->GetObjectClass(second) : nullptr;
        QMetaType firstMetaType(first ? getMetaTypeIdForObject(env, first) : QMetaType::Nullptr);
        QMetaType secondMetaType(second ? getMetaTypeIdForObject(env, second) : QMetaType::Nullptr);
        if(firstMetaType.isValid() && secondMetaType.isValid()){
            AbstractContainerAccess* containerAccess = AbstractContainerAccess::create(env, AssociativeContainerType::QPair, firstMetaType, secondMetaType);
            if(!containerAccess){
                QString firstType = QLatin1String(firstMetaType.name());
                QString secondType = QLatin1String(secondMetaType.name());
                size_t size1 = 0;
                size_t align1 = 0;
                size_t size2 = 0;
                size_t align2 = 0;
#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
                bool isPointer1 = firstMetaType.flags() & QMetaType::IsPointer;
                if(!isPointer1){
                    size1 = size_t(firstMetaType.sizeOf());
                    align1 = size_t(firstMetaType.alignOf());
                }
#else
                bool isPointer1 = firstMetaType.name().endsWith("*")
                        || firstMetaType.name().contains("(*)")
                        || firstMetaType.name().contains("(__cdecl*)");
                if(!isPointer1){
                    size1 = size_t(firstMetaType.sizeOf());
                    align1 = QtJambiTypeManager::getInternalAlignment(QLatin1String(firstMetaType.name()));
                }
#endif
#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
                bool isPointer2 = secondMetaType.flags() & QMetaType::IsPointer;
                if(!isPointer2){
                    size2 = size_t(secondMetaType.sizeOf());
                    align2 = size_t(secondMetaType.alignOf());
                }
#else
                bool isPointer2 = secondMetaType.name().endsWith("*")
                        || secondMetaType.name().contains("(*)")
                        || secondMetaType.name().contains("(__cdecl*)");
                if(!isPointer2){
                    size2 = size_t(secondMetaType.sizeOf());
                    align2 = QtJambiTypeManager::getInternalAlignment(QLatin1String(secondMetaType.name()));
                }
#endif
                QHashFunction hashFunction1 = QtJambiTypeManager::findHashFunction(isPointer1, firstMetaType.id());
                QHashFunction hashFunction2 = QtJambiTypeManager::findHashFunction(isPointer2, secondMetaType.id());
                InternalToExternalConverter memberConverter1 = QtJambiTypeManager::getInternalToExternalConverter(
                                                                env,
                                                                firstType,
                                                                firstMetaType,
                                                                firstClass);
                InternalToExternalConverter memberConverter2 = QtJambiTypeManager::getInternalToExternalConverter(
                                                                env,
                                                                secondType,
                                                                secondMetaType,
                                                                secondClass);
                ExternalToInternalConverter memberReConverter1 = QtJambiTypeManager::getExternalToInternalConverter(
                                                                env, firstClass, firstType, firstMetaType);
                ExternalToInternalConverter memberReConverter2 = QtJambiTypeManager::getExternalToInternalConverter(
                                                                env, secondClass, secondType, secondMetaType);
                containerAccess = AbstractContainerAccess::create(
                                               env, AssociativeContainerType::QPair,
                                               firstMetaType,
                                               align1, size1,
                                               isPointer1,
                                               hashFunction1,
                                               memberConverter1,
                                               memberReConverter1,
                                               secondMetaType,
                                               align2, size2,
                                               isPointer2,
                                               hashFunction2,
                                               memberConverter2,
                                               memberReConverter2);
                if(QSharedPointer<AbstractPairAccess> pairAccess = QSharedPointer<AbstractContainerAccess>(containerAccess, &containerDisposer).dynamicCast<AbstractPairAccess>()){
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
                    QString qtName = QLatin1String("QPair<%1,%2>");
#else
                    QString qtName = QLatin1String("std::pair<%1,%2>");
#endif
                    qtName = qtName.arg(firstType, secondType);
                    QMetaType containerMetaType(containerAccess->registerContainer(qtName.toLatin1()));
                    if(firstType.contains("*") || secondType.contains("*")){
                        registerContainerConverter(pairAccess, containerMetaType);
                        return QVariant::fromValue(JObjectWrapper(env, java_object));
                    }else{
                        void* pair = pairAccess->createContainer();
                        pairAccess->setFirst(env, pair, first);
                        pairAccess->setSecond(env, pair, second);
                        QVariant variant(FROM_META_TYPE(containerMetaType), pair);
                        pairAccess->deleteContainer(pair);
                        return variant;
                    }
                }
            }
        }
        return QVariant::fromValue(JObjectWrapper(env, java_object));
    } else if (Java::Runtime::Collection::isInstanceOf(env, java_object)) {
        if (Java::QtCore::QList::isInstanceOf(env, java_object)) {
            if (QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForJavaObject(env, java_object)){
                if(AbstractListAccess* containerAccess = dynamic_cast<AbstractListAccess*>(link->containerAccess())){
                    QString qtName = QLatin1String("QList<%1>");
                    qtName = qtName.arg(QLatin1String(containerAccess->elementMetaType().name()));
                    QMetaType containerMetaType(containerAccess->registerContainer(qtName.toLatin1()));
                    if(dynamic_cast<WrapperListAccess*>(containerAccess)){
                        registerContainerConverter(SequentialContainerType::QList, containerMetaType, containerAccess->elementMetaType());
                        return QVariant::fromValue(JCollectionWrapper(env, java_object));
                    }else{
                        return QVariant(FROM_META_TYPE(containerMetaType), link->pointer());
                    }
                }
            }else{
                Java::QtJambi::QNoNativeResourcesException::throwNew(env, QStringLiteral("Incomplete object of type: %1").arg(QtJambiAPI::getClassName(env, object_class)) QTJAMBI_STACKTRACEINFO );
            }
        }else if (Java::QtCore::QSet::isInstanceOf(env, java_object)) {
            if (QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForJavaObject(env, java_object)){
                if(AbstractSetAccess* containerAccess = dynamic_cast<AbstractSetAccess*>(link->containerAccess())){
                    QString qtName = QLatin1String("QSet<%1>");
                    qtName = qtName.arg(QLatin1String(containerAccess->elementMetaType().name()));
                    QMetaType containerMetaType(containerAccess->registerContainer(qtName.toLatin1()));
                    if(dynamic_cast<WrapperSetAccess*>(containerAccess)){
                        registerContainerConverter(SequentialContainerType::QSet, containerMetaType, containerAccess->elementMetaType());
                        return QVariant::fromValue(JCollectionWrapper(env, java_object));
                    }else{
                        return QVariant(FROM_META_TYPE(containerMetaType), link->pointer());
                    }
                }
            }else{
                Java::QtJambi::QNoNativeResourcesException::throwNew(env, QStringLiteral("Incomplete object of type: %1").arg(QtJambiAPI::getClassName(env, object_class)) QTJAMBI_STACKTRACEINFO );
            }
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
        }else if (Java::QtCore::QLinkedList::isInstanceOf(env, java_object)) {
            if (QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForJavaObject(env, java_object)){
                if(AbstractLinkedListAccess* containerAccess = dynamic_cast<AbstractLinkedListAccess*>(link->containerAccess())){
                    QString qtName = QLatin1String("QLinkedList<%1>");
                    qtName = qtName.arg(QLatin1String(containerAccess->elementMetaType().name()));
                    QMetaType containerMetaType(containerAccess->registerContainer(qtName.toLatin1()));
                    registerContainerConverter(SequentialContainerType::QLinkedList, containerMetaType, containerAccess->elementMetaType());
                    if(dynamic_cast<WrapperLinkedListAccess*>(containerAccess)){
                        return QVariant::fromValue(JCollectionWrapper(env, java_object));
                    }else{
                        return QVariant(FROM_META_TYPE(containerMetaType), link->pointer());
                    }
                }
            }else{
                Java::QtJambi::QNoNativeResourcesException::throwNew(env, QStringLiteral("Incomplete object of type: %1").arg(QtJambiAPI::getClassName(env, object_class)) QTJAMBI_STACKTRACEINFO );
            }
        }else if (Java::QtCore::QVector::isInstanceOf(env, java_object)) {
            if (QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForJavaObject(env, java_object)){
                if(AbstractVectorAccess* containerAccess = dynamic_cast<AbstractVectorAccess*>(link->containerAccess())){
                    QString qtName = QLatin1String("QVector<%1>");
                    qtName = qtName.arg(QLatin1String(containerAccess->elementMetaType().name()));
                    QMetaType containerMetaType(containerAccess->registerContainer(qtName.toLatin1()));
                    registerContainerConverter(SequentialContainerType::QVector, containerMetaType, containerAccess->elementMetaType());
                    if(dynamic_cast<WrapperVectorAccess*>(containerAccess)){
                        return QVariant::fromValue(JCollectionWrapper(env, java_object));
                    }else{
                        return QVariant(FROM_META_TYPE(containerMetaType), link->pointer());
                    }
                }
            }else{
                Java::QtJambi::QNoNativeResourcesException::throwNew(env, QStringLiteral("Incomplete object of type: %1").arg(QtJambiAPI::getClassName(env, object_class)) QTJAMBI_STACKTRACEINFO );
            }
#endif
        }
        if(Java::Runtime::Collection::size(env, java_object)>0){
            jobject iter = QtJambiAPI::iteratorOfJavaCollection(env, java_object);
            jobject element = QtJambiAPI::nextOfJavaIterator(env, iter);
            int elementMetaTypeId = element ? getMetaTypeIdForObject(env, element) : QMetaType::Nullptr;
            while(QtJambiAPI::hasJavaIteratorNext(env, iter)){
                element = QtJambiAPI::nextOfJavaIterator(env, iter);
                if(element){
                    int _elementMetaTypeId = getMetaTypeIdForObject(env, element);
                    if(elementMetaTypeId==QMetaType::Nullptr){
                        elementMetaTypeId = _elementMetaTypeId;
                    }else if(_elementMetaTypeId!=elementMetaTypeId){
                        elementMetaTypeId = QMetaType::UnknownType;
                        break;
                    }
                }
            }
            if(elementMetaTypeId == QMetaType::QString){
                QStringList list;
                iter = QtJambiAPI::iteratorOfJavaCollection(env, java_object);
                while(QtJambiAPI::hasJavaIteratorNext(env, iter)){
                    element = QtJambiAPI::nextOfJavaIterator(env, iter);
                    list << qtjambi_cast<QString>(env, element);
                }
                return QVariant::fromValue(list);
            }else if(elementMetaTypeId == QMetaType::QVariant){
                QVariantList list;
                iter = QtJambiAPI::iteratorOfJavaCollection(env, java_object);
                while(QtJambiAPI::hasJavaIteratorNext(env, iter)){
                    element = QtJambiAPI::nextOfJavaIterator(env, iter);
                    list << QtJambiAPI::convertJavaObjectToQVariant(env, element);
                }
                return QVariant::fromValue(list);
            }else if(elementMetaTypeId != QMetaType::UnknownType && elementMetaTypeId != QMetaType::Nullptr){
                if (Java::Runtime::Set::isInstanceOf(env, java_object)) {
                    return internal_convertJavaObjectToQVariant(env, Java::QtCore::QSet::newInstance2(env, java_object), convert);
                }else if (Java::Runtime::Deque::isInstanceOf(env, java_object)) {
                    return internal_convertJavaObjectToQVariant(env, Java::QtCore::QStack::newInstance2(env, java_object), convert);
                }else if (Java::Runtime::Queue::isInstanceOf(env, java_object)) {
                    return internal_convertJavaObjectToQVariant(env, Java::QtCore::QQueue::newInstance2(env, java_object), convert);
                }else{
                    return internal_convertJavaObjectToQVariant(env, Java::QtCore::QList::newInstance2(env, java_object), convert);
                }
            }
        }
        if(!convert)
            return QVariant(META_TYPE_FROM(JCollectionWrapper), nullptr);
        return QVariant::fromValue<JCollectionWrapper>(JCollectionWrapper(env, java_object));
    } else if (Java::Runtime::Map::isInstanceOf(env, java_object)) {
        if (Java::QtCore::QMap::isInstanceOf(env, java_object)) {
            if (QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForJavaObject(env, java_object)){
                if(AbstractMapAccess* containerAccess = dynamic_cast<AbstractMapAccess*>(link->containerAccess())){
                    QString qtName = QLatin1String("QMap<%1,%2>");
                    qtName = qtName.arg(QLatin1String(containerAccess->keyMetaType().name()))
                                   .arg(QLatin1String(containerAccess->valueMetaType().name()));
                    QMetaType containerMetaType(containerAccess->registerContainer(qtName.toLatin1()));
                    if(dynamic_cast<WrapperMapAccess*>(link->containerAccess())){
                        registerContainerConverter(AssociativeContainerType::QMap, containerMetaType, containerAccess->keyMetaType(), containerAccess->valueMetaType());
                        return QVariant::fromValue(JMapWrapper(env, java_object));
                    }else{
                        return QVariant(FROM_META_TYPE(containerMetaType), link->pointer());
                    }
                }
            }else{
                Java::QtJambi::QNoNativeResourcesException::throwNew(env, QStringLiteral("Incomplete object of type: %1").arg(QtJambiAPI::getClassName(env, object_class)) QTJAMBI_STACKTRACEINFO );
            }
        }else if (Java::QtCore::QHash::isInstanceOf(env, java_object)) {
            if (QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForJavaObject(env, java_object)){
                if(AbstractHashAccess* containerAccess = dynamic_cast<AbstractHashAccess*>(link->containerAccess())){
                    QString qtName = QLatin1String("QHash<%1,%2>");
                    qtName = qtName.arg(QLatin1String(containerAccess->keyMetaType().name()))
                                   .arg(QLatin1String(containerAccess->valueMetaType().name()));
                    QMetaType containerMetaType(containerAccess->registerContainer(qtName.toLatin1()));
                    if(dynamic_cast<WrapperHashAccess*>(link->containerAccess())){
                        registerContainerConverter(AssociativeContainerType::QHash, containerMetaType, containerAccess->keyMetaType(), containerAccess->valueMetaType());
                        return QVariant::fromValue(JMapWrapper(env, java_object));
                    }else{
                        return QVariant(FROM_META_TYPE(containerMetaType), link->pointer());
                    }
                }
            }else{
                Java::QtJambi::QNoNativeResourcesException::throwNew(env, QStringLiteral("Incomplete object of type: %1").arg(QtJambiAPI::getClassName(env, object_class)) QTJAMBI_STACKTRACEINFO );
            }
        }else if (Java::QtCore::QMultiMap::isInstanceOf(env, java_object)) {
            if (QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForJavaObject(env, java_object)){
                if(AbstractMultiMapAccess* containerAccess = dynamic_cast<AbstractMultiMapAccess*>(link->containerAccess())){
                    QString qtName = QLatin1String("QMultiMap<%1,%2>");
                    qtName = qtName.arg(QLatin1String(containerAccess->keyMetaType().name()))
                                   .arg(QLatin1String(containerAccess->valueMetaType().name()));
                    QMetaType containerMetaType(containerAccess->registerContainer(qtName.toLatin1()));
                    if(dynamic_cast<WrapperMultiMapAccess*>(link->containerAccess())){
                        registerContainerConverter(AssociativeContainerType::QMultiMap, containerMetaType, containerAccess->keyMetaType(), containerAccess->valueMetaType());
                        return QVariant::fromValue(JMapWrapper(env, java_object));
                    }else{
                        return QVariant(FROM_META_TYPE(containerMetaType), link->pointer());
                    }
                }
            }else{
                Java::QtJambi::QNoNativeResourcesException::throwNew(env, QStringLiteral("Incomplete object of type: %1").arg(QtJambiAPI::getClassName(env, object_class)) QTJAMBI_STACKTRACEINFO );
            }
        }else if (Java::QtCore::QMultiHash::isInstanceOf(env, java_object)) {
            if (QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForJavaObject(env, java_object)){
                if(AbstractMultiHashAccess* containerAccess = dynamic_cast<AbstractMultiHashAccess*>(link->containerAccess())){
                    QString qtName = QLatin1String("QMultiHash<%1,%2>");
                    qtName = qtName.arg(QLatin1String(containerAccess->keyMetaType().name()))
                                   .arg(QLatin1String(containerAccess->valueMetaType().name()));
                    QMetaType containerMetaType(containerAccess->registerContainer(qtName.toLatin1()));
                    if(dynamic_cast<WrapperMultiHashAccess*>(link->containerAccess())){
                        registerContainerConverter(AssociativeContainerType::QMultiHash, containerMetaType, containerAccess->keyMetaType(), containerAccess->valueMetaType());
                        return QVariant::fromValue(JMapWrapper(env, java_object));
                    }else{
                        return QVariant(FROM_META_TYPE(containerMetaType), link->pointer());
                    }
                }
            }else{
                Java::QtJambi::QNoNativeResourcesException::throwNew(env, QStringLiteral("Incomplete object of type: %1").arg(QtJambiAPI::getClassName(env, object_class)) QTJAMBI_STACKTRACEINFO );
            }
        }
        if(Java::Runtime::Map::size(env, java_object)>0){
            jobject iter = QtJambiAPI::entrySetIteratorOfJavaMap(env, java_object);
            jobject entry = QtJambiAPI::nextOfJavaIterator(env, iter);
            jobject keyObject = QtJambiAPI::keyOfJavaMapEntry(env, entry);
            jobject valueObject = QtJambiAPI::valueOfJavaMapEntry(env, entry);
            int keyMetaTypeId = getMetaTypeIdForObject(env, keyObject);
            int valueMetaTypeId = getMetaTypeIdForObject(env, valueObject);
            while(QtJambiAPI::hasJavaIteratorNext(env, iter)){
                entry = QtJambiAPI::nextOfJavaIterator(env, iter);
                if(entry){
                    keyObject = QtJambiAPI::keyOfJavaMapEntry(env, entry);
                    if(keyObject){
                        int _keyMetaTypeId = getMetaTypeIdForObject(env, keyObject);
                        if(keyMetaTypeId==QMetaType::Nullptr){
                            keyMetaTypeId = _keyMetaTypeId;
                        }else if(_keyMetaTypeId!=keyMetaTypeId){
                            keyMetaTypeId = QMetaType::UnknownType;
                            break;
                        }
                    }
                    valueObject = QtJambiAPI::valueOfJavaMapEntry(env, entry);
                    if(valueObject){
                        int _valueMetaTypeId = getMetaTypeIdForObject(env, valueObject);
                        if(valueMetaTypeId==QMetaType::Nullptr){
                            valueMetaTypeId = _valueMetaTypeId;
                        }else if(_valueMetaTypeId!=valueMetaTypeId){
                            valueMetaTypeId = QMetaType::UnknownType;
                            break;
                        }
                    }
                }
            }
            if(keyMetaTypeId == QMetaType::QString && valueMetaTypeId == QMetaType::QVariant){
                if(Java::Runtime::NavigableMap::isInstanceOf(env, java_object)){
                    QVariantMap map;
                    iter = QtJambiAPI::iteratorOfJavaCollection(env, java_object);
                    while(QtJambiAPI::hasJavaIteratorNext(env, iter)){
                        jobject entry = QtJambiAPI::nextOfJavaIterator(env, iter);
                        jobject keyObject = QtJambiAPI::keyOfJavaMapEntry(env, entry);
                        jobject valueObject = QtJambiAPI::valueOfJavaMapEntry(env, entry);
                        map[qtjambi_cast<QString>(env, keyObject)] = QtJambiAPI::convertJavaObjectToQVariant(env, valueObject);
                    }
                    return QVariant::fromValue(map);
                }else{
                    QVariantHash map;
                    iter = QtJambiAPI::iteratorOfJavaCollection(env, java_object);
                    while(QtJambiAPI::hasJavaIteratorNext(env, iter)){
                        jobject entry = QtJambiAPI::nextOfJavaIterator(env, iter);
                        jobject keyObject = QtJambiAPI::keyOfJavaMapEntry(env, entry);
                        jobject valueObject = QtJambiAPI::valueOfJavaMapEntry(env, entry);
                        map[qtjambi_cast<QString>(env, keyObject)] = QtJambiAPI::convertJavaObjectToQVariant(env, valueObject);
                    }
                    return QVariant::fromValue(map);
                }
            }else if(keyMetaTypeId != QMetaType::UnknownType
                     && keyMetaTypeId != QMetaType::Nullptr
                     && valueMetaTypeId != QMetaType::UnknownType
                     && valueMetaTypeId != QMetaType::Nullptr){
                if (Java::Runtime::NavigableMap::isInstanceOf(env, java_object)) {
                    return internal_convertJavaObjectToQVariant(env, Java::QtCore::QMap::newInstance2(env, java_object), convert);
                }else{
                    return internal_convertJavaObjectToQVariant(env, Java::QtCore::QHash::newInstance2(env, java_object), convert);
                }
            }
        }
        if(!convert)
            return QVariant(META_TYPE_FROM(JMapWrapper), nullptr);
        return QVariant::fromValue(JMapWrapper(env, java_object));
    } else if (Java::Runtime::Iterator::isInstanceOf(env, java_object)) {
        if(!convert)
            return QVariant(META_TYPE_FROM(JIteratorWrapper), nullptr);
        return QVariant::fromValue(JIteratorWrapper(env, java_object));
    }

    if(Java::Runtime::Class::isArray(env, object_class)){
        jclass componentType = Java::Runtime::Class::getComponentType(env, object_class);
        if(!Java::Runtime::Class::isPrimitive(env, componentType)){
            if(!convert)
                return QVariant(META_TYPE_FROM(JObjectArrayWrapper), nullptr);
            return QVariant::fromValue(JObjectArrayWrapper(env, jobjectArray(java_object)));
        }else if(Java::Runtime::Integer::isPrimitiveType(env, componentType)){
            if(!convert)
                return QVariant(META_TYPE_FROM(JIntArrayWrapper), nullptr);
            return QVariant::fromValue(JIntArrayWrapper(env, jintArray(java_object)));
        }else if(Java::Runtime::Long::isPrimitiveType(env, componentType)){
            if(!convert)
                return QVariant(META_TYPE_FROM(JLongArrayWrapper), nullptr);
            return QVariant::fromValue(JLongArrayWrapper(env, jlongArray(java_object)));
        }else if(Java::Runtime::Short::isPrimitiveType(env, componentType)){
            if(!convert)
                return QVariant(META_TYPE_FROM(JShortArrayWrapper), nullptr);
            return QVariant::fromValue(JShortArrayWrapper(env, jshortArray(java_object)));
        }else if(Java::Runtime::Byte::isPrimitiveType(env, componentType)){
            if(!convert)
                return QVariant(META_TYPE_FROM(JByteArrayWrapper), nullptr);
            return QVariant::fromValue(JByteArrayWrapper(env, jbyteArray(java_object)));
        }else if(Java::Runtime::Boolean::isPrimitiveType(env, componentType)){
            if(!convert)
                return QVariant(META_TYPE_FROM(JBooleanArrayWrapper), nullptr);
            return QVariant::fromValue(JBooleanArrayWrapper(env, jbooleanArray(java_object)));
        }else if(Java::Runtime::Character::isPrimitiveType(env, componentType)){
            if(!convert)
                return QVariant(META_TYPE_FROM(JCharArrayWrapper), nullptr);
            return QVariant::fromValue(JCharArrayWrapper(env, jcharArray(java_object)));
        }else if(Java::Runtime::Double::isPrimitiveType(env, componentType)){
            if(!convert)
                return QVariant(META_TYPE_FROM(JDoubleArrayWrapper), nullptr);
            return QVariant::fromValue(JDoubleArrayWrapper(env, jdoubleArray(java_object)));
        }else if(Java::Runtime::Float::isPrimitiveType(env, componentType)){
            if(!convert)
                return QVariant(META_TYPE_FROM(JFloatArrayWrapper), nullptr);
            return QVariant::fromValue(JFloatArrayWrapper(env, jfloatArray(java_object)));
        }
    }

    // Do the slightly slower fallback...
    QString fullJavaName = QtJambiAPI::getClassName(env, object_class).replace(".", "/");

    // may it allow enum and flags?
    QString qtName = QtJambiTypeManager::getInternalTypeName(env, object_class, false);
    if(qtName=="JEnumWrapper"){
        if(!convert)
            return QVariant(META_TYPE_FROM(JEnumWrapper), nullptr);
        return QVariant::fromValue(JEnumWrapper(env, java_object));
    }else if(qtName=="JMapWrapper"){
        if(!convert)
            return QVariant(META_TYPE_FROM(JMapWrapper), nullptr);
        return QVariant::fromValue(JMapWrapper(env, java_object));
    }else if(qtName=="JCollectionWrapper"){
        if(!convert)
            return QVariant(META_TYPE_FROM(JCollectionWrapper), nullptr);
        return QVariant::fromValue(JCollectionWrapper(env, java_object));
    }else if(qtName=="JIteratorWrapper"){
        if(!convert)
            return QVariant(META_TYPE_FROM(JIteratorWrapper), nullptr);
        return QVariant::fromValue(JIteratorWrapper(env, java_object));
    }else if(qtName=="JIntArrayWrapper" || qtName=="JArrayWrapper<jint>"){
        if(!convert)
            return QVariant(META_TYPE_FROM(JIntArrayWrapper), nullptr);
        return QVariant::fromValue(JIntArrayWrapper(env, jintArray(java_object)));
    }else if(qtName=="JLongArrayWrapper" || qtName=="JArrayWrapper<jlong>"){
        if(!convert)
            return QVariant(META_TYPE_FROM(JLongArrayWrapper), nullptr);
        return QVariant::fromValue(JLongArrayWrapper(env, jlongArray(java_object)));
    }else if(qtName=="JShortArrayWrapper" || qtName=="JArrayWrapper<jshort>"){
        if(!convert)
            return QVariant(META_TYPE_FROM(JShortArrayWrapper), nullptr);
        return QVariant::fromValue(JShortArrayWrapper(env, jshortArray(java_object)));
    }else if(qtName=="JByteArrayWrapper" || qtName=="JArrayWrapper<jbyte>"){
        if(!convert)
            return QVariant(META_TYPE_FROM(JByteArrayWrapper), nullptr);
        return QVariant::fromValue(JByteArrayWrapper(env, jbyteArray(java_object)));
    }else if(qtName=="JBooleanArrayWrapper" || qtName=="JArrayWrapper<jboolean>"){
        if(!convert)
            return QVariant(META_TYPE_FROM(JBooleanArrayWrapper), nullptr);
        return QVariant::fromValue(JBooleanArrayWrapper(env, jbooleanArray(java_object)));
    }else if(qtName=="JCharArrayWrapper" || qtName=="JArrayWrapper<jchar>"){
        if(!convert)
            return QVariant(META_TYPE_FROM(JCharArrayWrapper), nullptr);
        return QVariant::fromValue(JCharArrayWrapper(env, jcharArray(java_object)));
    }else if(qtName=="JDoubleArrayWrapper"
             || qtName=="JArrayWrapper<double>"
             || qtName=="JArrayWrapper<jdouble>"){
        if(!convert)
            return QVariant(META_TYPE_FROM(JDoubleArrayWrapper), nullptr);
        return QVariant::fromValue(JDoubleArrayWrapper(env, jdoubleArray(java_object)));
    }else if(qtName=="JFloatArrayWrapper"
             || qtName=="JArrayWrapper<float>"
             || qtName=="JArrayWrapper<jfloat>"){
        if(!convert)
            return QVariant(META_TYPE_FROM(JFloatArrayWrapper), nullptr);
        return QVariant::fromValue(JFloatArrayWrapper(env, jfloatArray(java_object)));
    }
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    QMetaType type = QMetaType::fromName(qPrintable(qtName));
    if(!type.isValid() && qtName.endsWith("*")){
        const std::type_info* typeId = getTypeByJavaName(fullJavaName);
        type = qtjambiRegisterMetaType(env, object_class, qtName.toUtf8(),
                                       typeId ? registeredOriginalMetaObject(*typeId) : nullptr,
                                       Java::QtCore::QObject::isAssignableFrom(env, object_class));
    }else if(type.isValid() && isJObjectWrappedMetaType(type)){
        JObjectWrapper wrapper(env, java_object);
        return QVariant(type, &wrapper);
    }
#else
    int metaTypeId = !qtName.isEmpty()
        ? QMetaType::type(qPrintable(qtName))
        : int(QMetaType::UnknownType);
    if(metaTypeId == QMetaType::UnknownType && qtName.endsWith("*")){
        if(Java::QtCore::QObject::isAssignableFrom(env, object_class)){
            metaTypeId = qRegisterMetaType<QObject*>(qPrintable(qtName));
        }else{
            metaTypeId = qRegisterMetaType<void*>(qPrintable(qtName));
        }
    }
    QMetaType type(metaTypeId);
#endif

    if(type.isValid() && type != QMetaType::fromType<JObjectWrapper>()){
        bool findConverter = !qtName.endsWith("*") || !convert;
        if(!findConverter){
            if (QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForJavaInterface(env, java_object)){
                if(link->ownership()!=QtJambiLink::Ownership::Java)
                    findConverter = true;
            }else if(Java::QtJambi::QtObjectInterface::isInstanceOf(env, java_object)){
                Java::QtJambi::QNoNativeResourcesException::throwNew(env, QStringLiteral("Incomplete object of type: %1").arg(QtJambiAPI::getObjectClassName(env, java_object).replace("$", ".")) QTJAMBI_STACKTRACEINFO );
            }
        }
        if(findConverter){
            if(ExternalToInternalConverter converter = QtJambiTypeManager::getExternalToInternalConverter(env, object_class, qtName, type)){
                if(convert){
                    void* ptr = nullptr;
                    QtJambiScope scope;
                    jvalue val;
                    val.l = java_object;
                    bool result = converter(env, &scope, val, ptr, jValueType::l);
                    if(!result){
                        qCWarning(DebugAPI::internalCategory, "Cannot convert external type '%s' to '%s'",
                                 qPrintable(fullJavaName.replace("/", ".").replace("$", ".")),
                                 qPrintable(qtName));
                        return QVariant(META_TYPE(QMetaType::UnknownType), nullptr);
                    }
                    return QVariant(FROM_META_TYPE(type), ptr);
                }else{
                    return QVariant(FROM_META_TYPE(type), nullptr);
                }
            }
        }
    }
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    registerConverterVariant(env, type, qtName, fullJavaName, object_class);
#endif
    if(!convert)
        return QVariant(META_TYPE_FROM(JObjectWrapper), nullptr);
    else
        return QVariant::fromValue(JObjectWrapper(env, java_object));
}
#undef META_TYPE

QVariant QtJambiAPI::convertJavaObjectToQVariant(JNIEnv *env, jobject java_object){
    return internal_convertJavaObjectToQVariant(env, java_object, true);
}

bool QtJambiAPI::convertJavaToNative(JNIEnv *env, const std::type_info& typeId, jobject java_object, void * output, QtJambiScope* scope)
{
    if(QtJambiTypeEntryPtr typeEntry = QtJambiTypeEntry::getTypeEntry(env, typeId)){
        jvalue java_value;
        java_value.l = java_object;
        return typeEntry->convertToNative(env, java_value, jValueType::l, output, scope);
    }else return false;
}

bool QtJambiAPI::convertJavaToNative(JNIEnv *env, const std::type_info& typeId, const char* qtName, const char* javaName, jobject java_object, void * output, QtJambiScope* scope)
{
    QtJambiTypeEntryPtr typeEntry = QtJambiTypeEntry::getTypeEntry(env, typeId, qtName);
    if(!qtName && javaName && typeEntry->isFunctional()){
        if(const std::type_info* typeId = getTypeByJavaName(javaName))
            qtName = getQtName(*typeId);
        if(qtName)
            typeEntry = QtJambiTypeEntry::getTypeEntry(env, typeId, qtName);
    }
    if(typeEntry){
        jvalue java_value;
        java_value.l = java_object;
        return typeEntry->convertToNative(env, java_value, jValueType::l, output, scope);
    }else return false;
}

bool QtJambiAPI::convertJavaToNative(JNIEnv *env, const std::type_info& typeId, const char* typeName, jobject java_object, void * output, QtJambiScope* scope)
{
    QtJambiTypeEntryPtr typeEntry = QtJambiTypeEntry::getTypeEntry(env, typeId, typeName);
    if(!typeEntry){
        if(typeName){
            if(const std::type_info* typeId = getTypeByJavaName(typeName)){
                typeName = getQtName(*typeId);
            }
            if(typeName)
                typeEntry = QtJambiTypeEntry::getTypeEntry(env, typeId, typeName);
        }
    }
    if(typeEntry){
        jvalue java_value;
        java_value.l = java_object;
        return typeEntry->convertToNative(env, java_value, jValueType::l, output, scope);
    }else{
        return false;
    }
}

void *QtJambiAPI::convertJavaObjectToNative(JNIEnv *env, jobject java_object)
{
    if(const QSharedPointer<QtJambiLink>& link = QtJambiLink::findLinkForJavaObject(env, java_object))
        return link->pointer();
    else if(Java::QtJambi::QtObjectInterface::isInstanceOf(env, java_object))
        Java::QtJambi::QNoNativeResourcesException::throwNew(env, QStringLiteral("Incomplete object of type: %1").arg(QtJambiAPI::getObjectClassName(env, java_object).replace("$", ".")) QTJAMBI_STACKTRACEINFO );
    return nullptr;
}

QObject *QtJambiAPI::convertJavaObjectToQObject(JNIEnv *env, jobject java_object)
{
    if(const QSharedPointer<QtJambiLink>& link = QtJambiLink::findLinkForJavaObject(env, java_object))
        return link->isQObject() ? link->qobject() : nullptr;
    else if(Java::QtJambi::QtObjectInterface::isInstanceOf(env, java_object))
        Java::QtJambi::QNoNativeResourcesException::throwNew(env, QStringLiteral("Incomplete object of type: %1").arg(QtJambiAPI::getObjectClassName(env, java_object).replace("$", ".")) QTJAMBI_STACKTRACEINFO );
    return nullptr;
}

const void *QtJambiAPI::convertJavaObjectToNativeAsSmartPointer(JNIEnv *env, jobject java_object, SmartPointerCreator pointerCreator, SmartPointerDeleter pointerDeleter, SmartPointerQObjectGetter pointerGetter)
{
    if(java_object){
        if(QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForJavaObject(env, java_object)){
            Q_ASSERT(link->isQObject());
            if(link->isSmartPointer()){
                SmartPointerLink* pointerLink = static_cast<SmartPointerLink*>(link.data());
                return pointerLink->getSmartPointer();
            }else{
                void* ptr = link->pointer();
                bool createdByJava = link->createdByJava();
                bool is_shell = link->isShell();
                QtJambiShellImpl* shell = nullptr;
                if(is_shell){
                    const SuperTypeInfos& infos = SuperTypeInfos::fromClass(env, env->GetObjectClass(java_object));
                    Q_ASSERT(infos.size());
                    shell = *reinterpret_cast<QtJambiShellImpl**>( quintptr(ptr) + infos.at(0).size() );
                }

                jobject nativeLink = link->nativeLink(env);
                if(nativeLink){
                    Java::QtJambi::NativeUtility$NativeLink::reset(env, nativeLink);
                }

                link->invalidate(env);
                link.clear();
                const InterfaceOffsetInfo* interfaceOffsets = getInterfaceOffsets(env, env->GetObjectClass(java_object));
                if(interfaceOffsets && !interfaceOffsets->offsets.isEmpty()){
                    link = QtJambiLink::createLinkForSmartPointerToQObject(env, java_object, createdByJava, is_shell, pointerCreator(ptr), pointerDeleter, pointerGetter, interfaceOffsets->offsets, interfaceOffsets->interfaces, interfaceOffsets->inheritedInterfaces);
                }else{
                    link = QtJambiLink::createLinkForSmartPointerToQObject(env, java_object, createdByJava, is_shell, pointerCreator(ptr), pointerDeleter, pointerGetter);
                }
                Q_ASSERT(link && link->isSmartPointer());
                if(shell){
                    shell->overrideLink(link);
                }
                return static_cast<SmartPointerLink*>(link.data())->getSmartPointer();
            }
        }
        else if(Java::QtJambi::QtObjectInterface::isInstanceOf(env, java_object))
            Java::QtJambi::QNoNativeResourcesException::throwNew(env, QStringLiteral("Incomplete object of type: %1").arg(QtJambiAPI::getObjectClassName(env, java_object).replace("$", ".")) QTJAMBI_STACKTRACEINFO );
    }
    // this is a memory leak because this is not linked to java object!!!
    return nullptr;
}

const void *QtJambiAPI::convertJavaObjectToNativeAsSmartPointer(JNIEnv *env, jobject java_object, SmartPointerCreator pointerCreator, SmartPointerDeleter pointerDeleter, SmartPointerGetter pointerGetter)
{
    if(java_object){
        if(QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForJavaObject(env, java_object)){
            Q_ASSERT(!link->isQObject());
            if(link->isSmartPointer()){
                return static_cast<SmartPointerLink*>(link.data())->getSmartPointer();
            }else if(!link->isQObject()){
                void* ptr = link->pointer();
                bool createdByJava = link->createdByJava();
                bool is_shell = link->isShell();
                QtJambiShellImpl* shell = nullptr;
                if(is_shell){
                    const SuperTypeInfos& infos = SuperTypeInfos::fromClass(env, env->GetObjectClass(java_object));
                    Q_ASSERT(infos.size());
                    shell = *reinterpret_cast<QtJambiShellImpl**>( quintptr(ptr) + infos.at(0).size() );
                }
#if defined(QTJAMBI_DEBUG_TOOLS) || defined(QTJAMBI_LINK_NAME) || !defined(QT_NO_DEBUG)
                const char* className = link->qtTypeName();
#endif
                PtrOwnerFunction registeredOwnerFunction = nullptr;
                if(!link->isQObject()){
                    PointerToObjectLink* polink = static_cast<PointerToObjectLink*>(link.data());
                    registeredOwnerFunction = polink->ownerFunction();
                }

                jobject nativeLink = link->nativeLink(env);
                if(nativeLink){
                    Java::QtJambi::NativeUtility$NativeLink::reset(env, nativeLink);
                }

                link->invalidate(env);
                link.clear();
                const InterfaceOffsetInfo* interfaceOffsets = getInterfaceOffsets(env, env->GetObjectClass(java_object));
                if(interfaceOffsets && !interfaceOffsets->offsets.isEmpty()){
                    link = QtJambiLink::createLinkForSmartPointerToObject(env,
                                                                               java_object,
                                                                               LINK_NAME_ARG(className)
                                                                               createdByJava,
                                                                               is_shell,
                                                                               registeredOwnerFunction,
                                                                               pointerCreator(ptr),
                                                                               pointerDeleter,
                                                                               pointerGetter,
                                                                               interfaceOffsets->offsets,
                                                                               interfaceOffsets->interfaces,
                                                                               interfaceOffsets->inheritedInterfaces);
                }else{
                    link = QtJambiLink::createLinkForSmartPointerToObject(env,
                                                                          java_object,
                                                                          LINK_NAME_ARG(className)
                                                                          createdByJava,
                                                                          is_shell,
                                                                          registeredOwnerFunction,
                                                                          pointerCreator(ptr),
                                                                          pointerDeleter,
                                                                          pointerGetter);
                }
                if(shell){
                    shell->overrideLink(link);
                }
                Q_ASSERT(link && link->isSmartPointer());
                return static_cast<SmartPointerLink*>(link.data())->getSmartPointer();
            }
        }
        else if(Java::QtJambi::QtObjectInterface::isInstanceOf(env, java_object))
            Java::QtJambi::QNoNativeResourcesException::throwNew(env, QStringLiteral("Incomplete object of type: %1").arg(QtJambiAPI::getObjectClassName(env, java_object).replace("$", ".")) QTJAMBI_STACKTRACEINFO );
    }
    // this is a memory leak because this is not linked to java object!!!
    return nullptr;
}

void *QtJambiAPI::convertJavaInterfaceToNative(JNIEnv *env, jobject object, const char *, const std::type_info& typeId){
    return QtJambiLink::findPointerForJavaInterface(env, object, typeId);
}

void *QtJambiAPI::convertJavaInterfaceToNative(JNIEnv *env, jobject object, const std::type_info& typeId){
    return QtJambiLink::findPointerForJavaInterface(env, object, typeId);
}

jobject internal_convertNativeToJavaObject(JNIEnv *env, const void *qt_object, const std::type_info& typeId, const char *qtName, NativeToJavaConversionMode mode, bool *ok)
{
    if(ok) *ok = true;
    if (!qt_object)
        return nullptr;
    if(QtJambiTypeEntryPtr typeEntry = QtJambiTypeEntry::getTypeEntry(env, typeId, qtName)){
        jvalue jv;
        jv.l = nullptr;
        qintptr offset = 0;
        typeEntry = typeEntry->getFittingTypeEntry(env, qt_object, offset);
        if(offset!=0){
            qt_object = reinterpret_cast<const char*>(qt_object)-offset;
        }
        if(typeEntry->convertToJava(env, qt_object, mode, jv, jValueType::l)){
            return jv.l;
        }else{
            return nullptr;
        }
    }
    if(ok) *ok = false;
    return nullptr;
}

jobject internal_convertNativeToJavaObject(JNIEnv *env, const void *qt_object, QByteArray className, jclass clazz, NativeToJavaConversionMode mode, bool *ok)
{
    if(ok)
        *ok = true;
    if (!qt_object || !clazz)
        return nullptr;
    int metaType = QMetaType::UnknownType;
    if(const std::type_info* typeId = getTypeByJavaName(className)){
        metaType = registeredMetaTypeID(*typeId);
        if(metaType==QMetaType::UnknownType){
            metaType = QMetaType::type(getQtName(*typeId));
        }
    }

    // If it's not a value type, we just link to the pointer directly.
    void *copy = nullptr;
    if (mode!=NativeToJavaConversionMode::MakeCopyOfValues || metaType == QMetaType::UnknownType) {
        // If the object is constructed in Java, then we can look it up
        for(const QSharedPointer<QtJambiLink>& link : QtJambiLink::findLinksForPointer(qt_object)){
            if(link){
                jobject obj = link->getJavaObjectLocalRef(env);
                if(!obj && link->ownership()==QtJambiLink::Ownership::Split){
                    link->invalidate(env);
                }else{
                    jclass targetType = nullptr;
                    try{
                        targetType = JavaAPI::resolveClass(env, className);
                    }catch(...){}
                    if(targetType && env->IsInstanceOf(obj, targetType)){
                        return obj;
                    }
                }
            }
        }

        // Otherwise we have to create it
        copy = const_cast<void *>(qt_object);
    } else {
        copy = QMetaType::create(metaType, qt_object);
        if (!copy){
            if(ok)
                *ok = false;
            return nullptr;
        }
    }

    jobject returned = nullptr;
    if (clazz) {
        try{
            if(Java::QtJambi::QtObject::isAssignableFrom(env, clazz)){
                if(jmethodID constructorId = findInternalPrivateConstructor(env, clazz)){
                    returned = env->NewObject(clazz, constructorId, 0);
                    JavaException::check(env QTJAMBI_STACKTRACEINFO );
                }else{
                    if (metaType != QMetaType::UnknownType && copy)
                        QMetaType::destroy(metaType, copy);
                    if(ok)
                        *ok = false;
                    return nullptr;
                }
            }
        }catch(const JavaException& exn){
            if (metaType != QMetaType::UnknownType && copy)
                QMetaType::destroy(metaType, copy);
            throw exn;
        }
    }

    if (!returned){
        if(ok)
            *ok = false;
        return nullptr;
    }
    QtJambiLink::Ownership ownership = QtJambiLink::Ownership::None;
    // If the type is copied in, we own the pointer
    if ((metaType != QMetaType::UnknownType && mode==NativeToJavaConversionMode::MakeCopyOfValues) || mode==NativeToJavaConversionMode::TransferOwnership){
        ownership = QtJambiLink::Ownership::Java;
    }
    const InterfaceOffsetInfo* interfaceOffsets = getInterfaceOffsets(env, clazz);
    QSharedPointer<QtJambiLink> link;
    if(metaType == QMetaType::UnknownType){
        if(interfaceOffsets && !interfaceOffsets->offsets.isEmpty()){
            link = QtJambiLink::createLinkForNativeObject(
                        env,
                        returned,
                        copy,
                        LINK_NAME_ARG(nullptr)
                        false,
                        false,
                        ownership,
                        interfaceOffsets->offsets,
                        interfaceOffsets->interfaces,
                        interfaceOffsets->inheritedInterfaces);
        }else{
            link = QtJambiLink::createLinkForNativeObject(
                        env,
                        returned,
                        copy,
                        LINK_NAME_ARG(nullptr)
                        false,
                        false,
                        ownership);
        }
        if(!link){
            if(ok)
                *ok = false;
            returned = nullptr;
        }
    }else{
        if(interfaceOffsets && !interfaceOffsets->offsets.isEmpty()){
            link = QtJambiLink::createLinkForNativeObject(
                        env,
                        returned,
                        copy,
                        QMetaType(metaType),
                        false,
                        false,
                        ownership,
                        interfaceOffsets->offsets,
                        interfaceOffsets->interfaces,
                        interfaceOffsets->inheritedInterfaces);
        }else{
            link = QtJambiLink::createLinkForNativeObject(
                        env,
                        returned,
                        copy,
                        QMetaType(metaType),
                        false,
                        false,
                        ownership);
        }
        if(!link){
            if (mode==NativeToJavaConversionMode::MakeCopyOfValues && copy)
                QMetaType::destroy(metaType, copy);
            returned = nullptr;
            if(ok)
                *ok = false;
        }
    }
    return returned;
}

jobject internal_convertNativeToJavaObject(NativeToJavaConversionMode mode, JNIEnv *env, const void *qt_object, const std::type_info& typeId, const char *qtName){
    bool ok{false};
    jobject result = internal_convertNativeToJavaObject(env, qt_object, typeId, qtName, mode, &ok);
    if(!ok){
        QByteArray className = getJavaName(typeId);
        if(!className.isEmpty()
                && (!className.startsWith("io/qt/core/Q")
                    || (   className!="io/qt/core/QList"
                        && className!="io/qt/core/QVector"
                        && className!="io/qt/core/QLinkedList"
                        && className!="io/qt/core/QMultiMap"
                        && className!="io/qt/core/QMultiHash"
                        && className!="io/qt/core/QMap"
                        && className!="io/qt/core/QHash"
                        && className!="io/qt/core/QSet"
                        && className!="io/qt/core/QStack"
                        && className!="io/qt/core/QQueue"
                        && className!="io/qt/core/QSequentialIterator"
                        && className!="io/qt/core/QAssociativeIterator"
                        && className!="io/qt/core/QSequentialConstIterator"
                        && className!="io/qt/core/QAssociativeConstIterator"
                        && className!="io/qt/core/QPair"))){
            return internal_convertNativeToJavaObject(env, qt_object, className, JavaAPI::resolveClass(env, className), mode, &ok);
        }
    }
    if(ok)
        return result;
    JavaException::raiseIllegalArgumentException(env, QStringLiteral("Cannot cast %1 to java object").arg(QLatin1String(QtJambiAPI::typeName(typeId))) QTJAMBI_STACKTRACEINFO );
    return nullptr;
}

jobject QtJambiAPI::convertNativeToJavaObjectAsCopy(JNIEnv *env, const void *qt_object, const std::type_info& typeId, const char *qtName){
    return internal_convertNativeToJavaObject(NativeToJavaConversionMode::MakeCopyOfValues, env, qt_object, typeId, qtName);
}

jobject QtJambiAPI::convertNativeToJavaObjectAsWrapper(JNIEnv *env, const void *qt_object, const std::type_info& typeId, const char *qtName){
    return internal_convertNativeToJavaObject(NativeToJavaConversionMode::None, env, qt_object, typeId, qtName);
}

jobject QtJambiAPI::convertNativeToJavaOwnedObjectAsWrapper(JNIEnv *env, const void *qt_object, const std::type_info& typeId, const char *qtName){
    return internal_convertNativeToJavaObject(NativeToJavaConversionMode::TransferOwnership, env, qt_object, typeId, qtName);
}

jobject internal_convertNativeToJavaObject(NativeToJavaConversionMode mode, JNIEnv *env, const void *qt_object, jclass clazz){
    QString java_name = QtJambiAPI::getClassName(env, clazz).replace(QLatin1Char('.'), QLatin1Char('/'));
    const std::type_info* typeId = getTypeByJavaName(java_name);
    bool ok{false};
    jobject result{nullptr};
    if(typeId)
        result = internal_convertNativeToJavaObject(env, qt_object, *typeId, getQtName(*typeId), mode, &ok);
    if(!ok)
        result = internal_convertNativeToJavaObject(env, qt_object, qPrintable(java_name), clazz, mode, &ok);
    if(ok)
        return result;
    JavaException::raiseIllegalArgumentException(env, QStringLiteral("Cannot cast %1 to java object").arg(QtJambiAPI::getClassName(env, clazz).replace(QLatin1Char('$'), QLatin1Char('.'))) QTJAMBI_STACKTRACEINFO );
    return nullptr;
}

jobject QtJambiAPI::convertModelIndexToJavaObject(JNIEnv *env, const QModelIndex& index){
    jobject result{nullptr};
    if(QModelIndexTypeEntry::convertNativeToJava(env, &index, NativeToJavaConversionMode::MakeCopyOfValues, result)){
        return result;
    }else{
        return nullptr;
    }
}

bool QtJambiAPI::convertJavaToModelIndex(JNIEnv *env, jobject java_object, QModelIndex** output){
    if(Java::QtCore::QModelIndex::isInstanceOf(env, java_object)){
        *output = reinterpret_cast<QModelIndex*>(QtJambiLink::findPointerForJavaObject(env, java_object));
        return true;
    }else if(env->IsSameObject(java_object, nullptr)){
        return true;
    }
    return false;
}

jobject QtJambiAPI::convertNativeToJavaObjectAsCopy(JNIEnv *env, const void *qt_object, jclass clazz){
    return internal_convertNativeToJavaObject(NativeToJavaConversionMode::MakeCopyOfValues, env, qt_object, clazz);
}

jobject QtJambiAPI::convertNativeToJavaOwnedObjectAsWrapper(JNIEnv *env, const void *qt_object, jclass clazz){
    return internal_convertNativeToJavaObject(NativeToJavaConversionMode::TransferOwnership, env, qt_object, clazz);
}

jobject QtJambiAPI::convertNativeToJavaObjectAsWrapper(JNIEnv *env, const void *qt_object, jclass clazz){
    return internal_convertNativeToJavaObject(NativeToJavaConversionMode::None, env, qt_object, clazz);
}

jobject internal_convertSmartPointerToJavaObject_impl(JNIEnv *env,
                            const std::type_info& typeId, void* ptr_shared_pointer, SmartPointerDeleter sharedPointerDeleter, SmartPointerGetterFunction sharedPointerGetter, bool* ok)
{
    if(ok) *ok = true;
    if(QtJambiTypeEntryPtr typeEntry = QtJambiTypeEntry::getTypeEntry(env, typeId)){
        jvalue result;
        qintptr offset = 0;
        typeEntry = typeEntry->getFittingTypeEntry(env, ptr_shared_pointer, sharedPointerGetter, offset);
        if(offset!=0){
            SmartPointerGetterFunction _sharedPointerGetter = std::move(sharedPointerGetter);
            sharedPointerGetter = [_sharedPointerGetter, offset](const void *_shared_pointer)->void*{
                void* result = _sharedPointerGetter(_shared_pointer);
                return reinterpret_cast<char*>(result)-offset;
            };
        }
        if(typeEntry->convertSharedPointerToJava(env, ptr_shared_pointer, sharedPointerDeleter, sharedPointerGetter, result, jValueType::l)){
            return result.l;
        }else{
            return nullptr;
        }
    }
    if(ok) *ok = false;
    return nullptr;
}

jobject internal_convertSmartPointerToJavaObject_notype(JNIEnv *env, const char *className,
                            void* ptr_shared_pointer, SmartPointerDeleter sharedPointerDeleter, SmartPointerGetterFunction sharedPointerGetter)
{
    Q_ASSERT(sharedPointerGetter);
    Q_ASSERT(sharedPointerDeleter);
    if (!ptr_shared_pointer || !sharedPointerDeleter || !sharedPointerGetter)
        return nullptr;
    void* qt_object = sharedPointerGetter(ptr_shared_pointer);
    if (!qt_object)
        return nullptr;

    for(const QSharedPointer<QtJambiLink>& link : QtJambiLink::findLinksForPointer(qt_object)){
        if(link){
            jobject obj = link->getJavaObjectLocalRef(env);
            if(!obj && link->ownership()==QtJambiLink::Ownership::Split){
                link->invalidate(env);
            }else{
                jclass targetType = nullptr;
                try{
                    targetType = JavaAPI::resolveClass(env, className);
                }catch(...){}
                if(targetType && env->IsInstanceOf(obj, targetType)){
                    return obj;
                }
            }
        }
    }

    jobject returned = nullptr;
    jclass clazz = JavaAPI::resolveClass(env, className);
    if (clazz && Java::QtJambi::QtObject::isAssignableFrom(env, clazz)) {
        if(jmethodID constructorId = findInternalPrivateConstructor(env, clazz)){
            returned = env->NewObject(clazz, constructorId, 0);
            JavaException::check(env QTJAMBI_STACKTRACEINFO );
        }else{
            return nullptr;
        }
    }
    if (!returned)
        return nullptr;

    const InterfaceOffsetInfo* interfaceOffsets = getInterfaceOffsets(env, clazz);
    const QSharedPointer<QtJambiLink>& link =
        interfaceOffsets && !interfaceOffsets->offsets.isEmpty() ?
        QtJambiLink::createLinkForSmartPointerToObject(
                env,
                returned,
                LINK_NAME_ARG(nullptr)
                false,
                false,
                nullptr,
                ptr_shared_pointer,
                sharedPointerDeleter,
                sharedPointerGetter,
                interfaceOffsets->offsets,
                interfaceOffsets->interfaces,
                interfaceOffsets->inheritedInterfaces
            )
        : QtJambiLink::createLinkForSmartPointerToObject(
               env,
               returned,
               LINK_NAME_ARG(nullptr)
               false,
               false,
               nullptr,
               ptr_shared_pointer,
               sharedPointerDeleter,
               sharedPointerGetter
               );
    if (!link) {
        returned = nullptr;
    }

    return returned;
}

jobject internal_convertQObjectSmartPointerToJavaObject_notype(JNIEnv *env, const char *className,
                             void* ptr_shared_pointer, SmartPointerDeleter shared_pointer_deleter, SmartPointerGetterFunction pointerGetter)
{
    Q_ASSERT(pointerGetter);
    Q_ASSERT(shared_pointer_deleter);
    if (!ptr_shared_pointer || !pointerGetter || !shared_pointer_deleter)
        return nullptr;
    QObject* qt_object = reinterpret_cast<QObject*>(pointerGetter(ptr_shared_pointer));
    if(!qt_object)
        return nullptr;

    jobject object = nullptr;
    QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForQObject(qt_object);
    {
        // Since QObjects are created in a class based on virtual function calls,
        // if they at some point during their constructor are converted to Java,
        // the Java object will get the wrong class. In order to fix this as well
        // as possible, we replace the java object if it turns out it has previously
        // been created using a different metaObject than the current. This should
        // at least make the brokeness identical to that of C++, and we can't do this
        // better than C++ since we depend on C++ to do it.
        if(link){
            object = link->getJavaObjectLocalRef(env);
            if(!object && !link->createdByJava()){
                QWriteLocker locker(QtJambiLinkUserData::lock());
                QtJambiLinkUserData *p = QTJAMBI_GET_OBJECTUSERDATA(QtJambiLinkUserData, qt_object);
                if (p && p->metaObject() && p->metaObject() != qt_object->metaObject()) {
                    QTJAMBI_SET_OBJECTUSERDATA(QtJambiLinkUserData, qt_object, nullptr);
                    locker.unlock();
                    delete p;
                    // It should already be split ownership, but in case it has been changed, we need to make sure the c++
                    // object isn't deleted.
                    jobject nativeLink = link->nativeLink(env);
                    Java::QtJambi::NativeUtility$NativeLink::reset(env, nativeLink);
                    link->setSplitOwnership(env);
                    link.clear();
                    locker.relock();
                }
            }
            if(link && !link->isSmartPointer()){
                if(object){
                    bool createdByJava = link->createdByJava();
                    bool is_shell = link->isShell();
                    QtJambiShellImpl* shell = nullptr;
                    if(is_shell){
                        const SuperTypeInfos& infos = SuperTypeInfos::fromClass(env, env->GetObjectClass(object));
                        Q_ASSERT(infos.size());
                        shell = *reinterpret_cast<QtJambiShellImpl**>( quintptr(qt_object) + infos.at(0).size() );
                    }
                    jobject nativeLink = link->nativeLink(env);
                    if(nativeLink){
                        Java::QtJambi::NativeUtility$NativeLink::reset(env, nativeLink);
                    }
                    link->invalidate(env);
                    link.clear();
                    const InterfaceOffsetInfo* interfaceOffsets = getInterfaceOffsets(env, env->GetObjectClass(object));
                    if(interfaceOffsets && !interfaceOffsets->offsets.isEmpty()){
                        link = QtJambiLink::createLinkForSmartPointerToQObject(env, object, createdByJava, is_shell, ptr_shared_pointer, shared_pointer_deleter, pointerGetter, interfaceOffsets->offsets, interfaceOffsets->interfaces, interfaceOffsets->inheritedInterfaces);
                    }else{
                        link = QtJambiLink::createLinkForSmartPointerToQObject(env, object, createdByJava, is_shell, ptr_shared_pointer, shared_pointer_deleter, pointerGetter);
                    }
                    if(shell){
                        shell->overrideLink(link);
                    }
                    env->DeleteLocalRef(nativeLink);
                    if (!link) {
                        qCWarning(DebugAPI::internalCategory, "Qt Jambi: Couldn't created wrapper for class %s", className);
                        return nullptr;
                    }
                }else{
                    QtJambiLinkUserData *p;
                    {
                        QWriteLocker locker(QtJambiLinkUserData::lock());
                        Q_UNUSED(locker)
                        p = QTJAMBI_GET_OBJECTUSERDATA(QtJambiLinkUserData, qt_object);
                        QTJAMBI_SET_OBJECTUSERDATA(QtJambiLinkUserData, qt_object, nullptr);
                    }
                    if(p){
                        delete p;
                        jobject nativeLink = link->nativeLink(env);
                        if(nativeLink){
                            Java::QtJambi::NativeUtility$NativeLink::reset(env, nativeLink);
                        }
                        link->invalidate(env);
                        link.clear();
                    }
                }
            }
        }
    }
    if (!link || !object) {
        const QMetaObject *mo = QtJambiMetaObject::findFirstStaticMetaObject(qt_object->metaObject());

        QByteArray javaClassName(className);

        jclass clazz = nullptr;
        try{
            clazz = JavaAPI::resolveClass(env, javaClassName);
        }catch(...){}

        // Search the hierarchy of classes for a class that has been mapped to Java.
        // Prefer the requested class if no other can be found.
        // Only return objects of subclasses of the requested class
        // If the requested class is not in the object's hierarchy, then we prefer
        // the requested class (this means it's basically a proper subclass of the
        // requested class, and thus probably the concrete wrapper, but atleast a
        // more specific version than anything we can find)
        while (mo) {
            QString javaName;
            if(const std::type_info* typeId = getTypeByMetaObject(mo)){
                javaName = QLatin1String(getJavaName(*typeId));
            }
            if (javaName==javaClassName){
                // Never go further down the hierarchy than the requested class
                break;
            }else if(!javaName.isEmpty()){
                // use the found java class instead
                jclass cls = nullptr;
                try{
                    cls = JavaAPI::resolveClass(env, qPrintable(javaName));
                }catch(...){}
                if(cls){
                    javaClassName = javaName.toLatin1();
                    clazz = cls;
                    break;
                }
                javaClassName = javaName.toLatin1();
                break;
            }
            mo = mo->superClass();
        }

        if(!clazz){
            clazz = JavaAPI::resolveClass(env, javaClassName);
        }
        if (clazz) {
            if(Java::Runtime::Class::isInterface(env, clazz)){
                javaClassName += "$Impl";
                clazz = JavaAPI::resolveClass(env, javaClassName);
            }
        }
        if (clazz) {
            int modifiers = Java::Runtime::Class::getModifiers(env, clazz);
            if(Java::Runtime::Modifier::isAbstract(env, modifiers)){
                javaClassName += "$ConcreteWrapper";
                clazz = JavaAPI::resolveClass(env, javaClassName);
            }
        }
        if (clazz) {
            if(jmethodID constructorId = findInternalPrivateConstructor(env, clazz)){
                object = env->NewObject(clazz, constructorId, 0);
                JavaException::check(env QTJAMBI_STACKTRACEINFO );
            }
        }

        if (!object)
            return nullptr;

        const InterfaceOffsetInfo* interfaceOffsets = getInterfaceOffsets(env, clazz);
        if(interfaceOffsets && !interfaceOffsets->offsets.isEmpty()){
            link = QtJambiLink::createLinkForSmartPointerToQObject(env, object, false, false, const_cast<void*>(ptr_shared_pointer), shared_pointer_deleter, pointerGetter, interfaceOffsets->offsets, interfaceOffsets->interfaces, interfaceOffsets->inheritedInterfaces);
        }else{
            link = QtJambiLink::createLinkForSmartPointerToQObject(env, object, false, false, const_cast<void*>(ptr_shared_pointer), shared_pointer_deleter, pointerGetter);
        }
        if (!link) {
            qCWarning(DebugAPI::internalCategory, "Qt Jambi: Couldn't created wrapper for class %s", className);
            return nullptr;
        }
    }
    return object;
}

jobject internal_convertSmartPointerToJavaInterface(JNIEnv *env, const std::type_info& interfaceType,
                            void* ptr_shared_pointer, SmartPointerDeleter sharedPointerDeleter, SmartPointerGetterFunction sharedPointerGetter)
{
    Q_ASSERT(sharedPointerGetter);
    Q_ASSERT(sharedPointerDeleter);
    if (!ptr_shared_pointer || !sharedPointerDeleter || !sharedPointerGetter)
        return nullptr;
    void* qt_object = sharedPointerGetter(ptr_shared_pointer);
    if (!qt_object)
        return nullptr;
    bool ok;
    jobject result = internal_convertSmartPointerToJavaObject_impl(env, interfaceType, ptr_shared_pointer, sharedPointerDeleter, sharedPointerGetter, &ok);
    if(ok)
        return result;
    QByteArray className = getJavaName(interfaceType);
    if(!className.isEmpty()){
        return internal_convertSmartPointerToJavaObject_notype(env, className, ptr_shared_pointer, sharedPointerDeleter, sharedPointerGetter);
    }else{
        return nullptr;
    }
}

jobject internal_convertSmartPointerToJavaObject(JNIEnv *env, const char *className,
                            void* ptr_shared_pointer, SmartPointerDeleter sharedPointerDeleter, SmartPointerGetterFunction sharedPointerGetter)
{
    const std::type_info* typeId = getTypeByJavaName(className);
    if(typeId){
        bool ok;
        jobject result = internal_convertSmartPointerToJavaObject_impl(env, *typeId, ptr_shared_pointer, sharedPointerDeleter, sharedPointerGetter, &ok);
        if(ok)
            return result;
    }
    return internal_convertSmartPointerToJavaObject_notype(env, className, ptr_shared_pointer, sharedPointerDeleter, sharedPointerGetter);
}

jobject internal_convertQObjectToJavaObject_type(JNIEnv *env, const QObject *qt_object, const std::type_info& typeId, bool* ok, NativeToJavaConversionMode mode)
{
    if(ok) *ok = true;
    if (!qt_object)
        return nullptr;
    QtJambiTypeEntryPtr typeEntry;
    if(!typeEntry)
        typeEntry = QtJambiTypeEntry::getTypeEntry(env, typeId);
    if(!typeEntry)
        typeEntry = QtJambiTypeEntry::getTypeEntry(env, typeid(QObject));
    if(typeEntry){
        jvalue jv;
        jv.l = nullptr;
        const void* ptr = qt_object;
        qintptr offset = 0;
        typeEntry = typeEntry->getFittingTypeEntry(env, ptr, offset);
        if(offset!=0){
            ptr = reinterpret_cast<const char*>(ptr)-offset;
        }
        if(typeEntry->convertToJava(env, ptr, mode, jv, jValueType::l)){
            return jv.l;
        }else{
            return nullptr;
        }
    }
    if(ok) *ok = false;
    return nullptr;
}

jobject internal_convertQObjectToJavaObject_notype(JNIEnv *env, const QObject *const_qt_object, QByteArray className, jclass clazz, NativeToJavaConversionMode mode)
{
    if (!const_qt_object)
        return nullptr;
    QObject * qt_object = const_cast<QObject *>(const_qt_object);
    const std::type_info* _typeId = tryGetTypeInfo(env, QtJambiPrivate::CheckPointer<QObject>::supplyType, qt_object);
    if(!_typeId){
        Java::QtJambi::QDanglingPointerException::throwNew(env, QString::asprintf("Dangling pointer to QObject: %p", qt_object) QTJAMBI_STACKTRACEINFO );
    }
    QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForQObject(qt_object);
    {
        // Since QObjects are created in a class based on virtual function calls,
        // if they at some point during their constructor are converted to Java,
        // the Java object will get the wrong class. In order to fix this as well
        // as possible, we replace the java object if it turns out it has previously
        // been created using a different metaObject than the current. This should
        // at least make the brokeness identical to that of C++, and we can't do this
        // better than C++ since we depend on C++ to do it.
        if(link && !link->createdByJava()){
            QWriteLocker locker(QtJambiLinkUserData::lock());
            QtJambiLinkUserData *p = QTJAMBI_GET_OBJECTUSERDATA(QtJambiLinkUserData, qt_object);
            if (p && p->metaObject() && p->metaObject() != qt_object->metaObject()) {
                QTJAMBI_SET_OBJECTUSERDATA(QtJambiLinkUserData, qt_object, nullptr);
                locker.unlock();
                delete p;
                // It should already be split ownership, but in case it has been changed, we need to make sure the c++
                // object isn't deleted.
                jobject nativeLink = link->nativeLink(env);
                Java::QtJambi::NativeUtility$NativeLink::reset(env, nativeLink);
                link->setSplitOwnership(env);
                link.clear();
                locker.relock();
            }
        }
    }
    jobject obj = nullptr;
    if(link){
        obj = link->getJavaObjectLocalRef(env);
        if(!obj && link->ownership()==QtJambiLink::Ownership::Split){
            QWriteLocker locker(QtJambiLinkUserData::lock());
            if(QtJambiLinkUserData *p = QTJAMBI_GET_OBJECTUSERDATA(QtJambiLinkUserData, qt_object)){
                QTJAMBI_SET_OBJECTUSERDATA(QtJambiLinkUserData, qt_object, nullptr);
                locker.unlock();
                delete p;
                jobject nativeLink = link->nativeLink(env);
                if(nativeLink){
                    Java::QtJambi::NativeUtility$NativeLink::reset(env, nativeLink);
                }
                link->invalidate(env);
                link.clear();
                locker.relock();
            }
        }
    }
    if (!link || !obj) {
        if(_typeId){
            if(QtJambiTypeEntryPtr typeEntry = QtJambiTypeEntry::getTypeEntry(env, *_typeId)){
                jvalue jv;
                jv.l = nullptr;
                if(typeEntry->convertToJava(env, qt_object, mode, jv, jValueType::l))
                    return jv.l;
            }
        }
        const QMetaObject *mo = QtJambiMetaObject::findFirstStaticMetaObject(qt_object->metaObject());

        QString java_name;
        if(className.isNull() && clazz){
            java_name = QtJambiAPI::getClassName(env, clazz).replace(QLatin1Char('.'), QLatin1Char('/'));
        }else{
            java_name = className;
        }

        // Search the hierarchy of classes for a class that has been mapped to Java.
        // Prefer the requested class if no other can be found.
        // Only return objects of subclasses of the requested class
        // If the requested class is not in the object's hierarchy, then we prefer
        // the requested class (this means it's basically a proper subclass of the
        // requested class, and thus probably the concrete wrapper, but atleast a
        // more specific version than anything we can find)
        while (mo) {
            QString javaName;
            if(const std::type_info* typeId = getTypeByMetaObject(mo)){
                javaName = QLatin1String(getJavaName(*typeId));
            }
            if (javaName==java_name){
                // Never go further down the hierarchy than the requested class
                break;
            }else if(!javaName.isEmpty()){
                // use the found java class instead
                jclass cls = nullptr;
                try{
                    cls = JavaAPI::resolveClass(env, qPrintable(javaName));
                    JavaException::check(env QTJAMBI_STACKTRACEINFO );
                }catch(...){}
                if(cls){
                    if(!clazz || env->IsAssignableFrom(cls, clazz)){
                        java_name = javaName;
                        clazz = cls;
                        break;
                    }
                }
            }
            mo = mo->superClass();
        }
        if(!java_name.isEmpty()){
            if (!clazz) {
                clazz = JavaAPI::resolveClass(env, qPrintable(java_name));
            }
            if (clazz) {
                if(Java::Runtime::Class::isInterface(env, clazz)){
                    java_name += "$Impl";
                    clazz = JavaAPI::resolveClass(env, qPrintable(java_name));
                }
            }
            if (clazz) {
                int modifiers = Java::Runtime::Class::getModifiers(env, clazz);
                if(Java::Runtime::Modifier::isAbstract(env, modifiers)){
                    java_name += "$ConcreteWrapper";
                    clazz = JavaAPI::resolveClass(env, qPrintable(java_name));
                }
            }
        }
        if (clazz) {
            if(jmethodID constructorId = findInternalPrivateConstructor(env, clazz)){
                obj = env->NewObject(clazz, constructorId, 0);
                JavaException::check(env QTJAMBI_STACKTRACEINFO );
                if (!obj){
                    qCWarning(DebugAPI::internalCategory, "Qt Jambi: Couldn't created object of class %s", qPrintable(java_name));
                    return nullptr;
                }
            }
        }else{
            qCWarning(DebugAPI::internalCategory, "Qt Jambi: Couldn't find java class %s", qPrintable(java_name));
            return nullptr;
        }

        link = QtJambiLink::createLinkForQObject(env, obj, qt_object, false, false);
        if (!link) {
            qCWarning(DebugAPI::internalCategory, "Qt Jambi: Couldn't created wrapper for class %s", qPrintable(java_name));
            return nullptr;
        }
        if(mode==NativeToJavaConversionMode::CppOwnership)
            link->setCppOwnership(env);
    }
    return obj;
}

jobject internal_convertQObjectSmartPointerToJavaObject_impl(JNIEnv *env, const std::type_info& typeId,
                             void* ptr_shared_pointer, SmartPointerDeleter shared_pointer_deleter, SmartPointerGetter pointerGetter, bool* ok)
{
    if(ok) *ok = true;
    if(QtJambiTypeEntryPtr typeEntry = QtJambiTypeEntry::getTypeEntry(env, typeId)){
        jvalue result;
        qintptr offset = 0;
        typeEntry = typeEntry->getFittingTypeEntry(env, ptr_shared_pointer, pointerGetter, offset);
        SmartPointerGetterFunction sharedPointerGetter = pointerGetter;
        if(offset!=0){
            sharedPointerGetter = [pointerGetter, offset](const void *_shared_pointer)->void*{
                void* result = pointerGetter(_shared_pointer);
                return reinterpret_cast<char*>(result)-offset;
            };
        }
        if(typeEntry->convertSharedPointerToJava(env, ptr_shared_pointer, shared_pointer_deleter, sharedPointerGetter, result, jValueType::l)){
            return result.l;
        }else{
            return nullptr;
        }
    }
    if(ok) *ok = false;
    return nullptr;
}

jobject internal_convertQObjectSmartPointerToJavaObject(JNIEnv *env, const std::type_info& typeId,
                             void* ptr_shared_pointer, SmartPointerDeleter shared_pointer_deleter, SmartPointerGetter pointerGetter)
{
    bool ok;
    jobject result = internal_convertQObjectSmartPointerToJavaObject_impl(env, typeId, ptr_shared_pointer, shared_pointer_deleter, pointerGetter, &ok);
    if(ok)
        return result;
    QByteArray className = getJavaName(typeId);
    return internal_convertQObjectSmartPointerToJavaObject_notype(env, className, ptr_shared_pointer, shared_pointer_deleter, pointerGetter);
}

jobject internal_convertQObjectSmartPointerToJavaObject(JNIEnv *env, const char *className,
                             void* ptr_shared_pointer, SmartPointerDeleter shared_pointer_deleter, SmartPointerGetter pointerGetter)
{
    const std::type_info* typeId = getTypeByJavaName(className);
    if(typeId){
        bool ok;
        jobject result = internal_convertQObjectSmartPointerToJavaObject_impl(env, *typeId, ptr_shared_pointer, shared_pointer_deleter, pointerGetter, &ok);
        if(ok)
            return result;
    }
    return internal_convertQObjectSmartPointerToJavaObject_notype(env, className, ptr_shared_pointer, shared_pointer_deleter, pointerGetter);
}

jobject QtJambiAPI::convertSmartPointerToJavaObject(JNIEnv *env, const std::type_info& typeId,
                                           void* ptr_shared_pointer, SmartPointerDeleter sharedPointerDeleter, SmartPointerGetter sharedPointerGetter)
{
    bool ok;
    jobject result = internal_convertSmartPointerToJavaObject_impl(env, typeId, ptr_shared_pointer, sharedPointerDeleter, sharedPointerGetter, &ok);
    if(ok)
        return result;
    const char* java_type_name = getJavaName(typeId);
    if(java_type_name){
        return internal_convertSmartPointerToJavaObject_notype(env, java_type_name, ptr_shared_pointer, sharedPointerDeleter, sharedPointerGetter);
    }
    return nullptr;
}

jobject QtJambiAPI::convertQObjectToJavaObject(JNIEnv *env, const QObject *qt_object, const char *className)
{
    const std::type_info* typeId = className ? getTypeByJavaName(className) : &typeid(QObject);
    if(typeId){
        bool ok;
        jobject result = internal_convertQObjectToJavaObject_type(env, qt_object, *typeId, &ok, NativeToJavaConversionMode::None);
        if(ok)
            return result;
    }
    return internal_convertQObjectToJavaObject_notype(env, qt_object, className, nullptr, NativeToJavaConversionMode::None);
}

jobject QtJambiAPI::convertQObjectToJavaObject(JNIEnv *env, const QObject *qt_object, jclass clazz)
{
    QString className;
    const std::type_info* typeId;
    if(clazz){
        className = QtJambiAPI::getClassName(env, clazz).replace(".", "/");
        typeId = getTypeByJavaName(className);
    }else{
        typeId = &typeid(QObject);
    }
    if(typeId){
        bool ok;
        jobject result = internal_convertQObjectToJavaObject_type(env, qt_object, *typeId, &ok, NativeToJavaConversionMode::None);
        if(ok)
            return result;
    }
    return internal_convertQObjectToJavaObject_notype(env, qt_object, className.toUtf8(), clazz, NativeToJavaConversionMode::None);
}

jobject QtJambiAPI::convertQObjectToJavaObject(JNIEnv *env, const QObject *qt_object, const std::type_info& typeId)
{
    bool ok;
    jobject result = internal_convertQObjectToJavaObject_type(env, qt_object, typeId, &ok, NativeToJavaConversionMode::None);
    if(ok)
        return result;
    QByteArray className = getJavaName(typeId);
    if(!className.isEmpty())
        return internal_convertQObjectToJavaObject_notype(env, qt_object, className, JavaAPI::resolveClass(env, className), NativeToJavaConversionMode::None);
    else
        return internal_convertQObjectToJavaObject_notype(env, qt_object, className, nullptr, NativeToJavaConversionMode::None);
}

jobject CoreAPI::convertQObjectToJavaObjectCppOwnership(JNIEnv *env, const QObject *qt_object, const std::type_info& typeId)
{
    bool ok;
    jobject result = internal_convertQObjectToJavaObject_type(env, qt_object, typeId, &ok, NativeToJavaConversionMode::CppOwnership);
    if(ok)
        return result;
    QByteArray className = getJavaName(typeId);
    if(!className.isEmpty())
        return internal_convertQObjectToJavaObject_notype(env, qt_object, className, JavaAPI::resolveClass(env, className), NativeToJavaConversionMode::CppOwnership);
    else
        return internal_convertQObjectToJavaObject_notype(env, qt_object, className, nullptr, NativeToJavaConversionMode::CppOwnership);
}
