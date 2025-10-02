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

#include "coreapi.h"
#include "typemanager_p.h"
#include "exception.h"
#include "java_p.h"
#include "registryutil_p.h"
#include "supertypeinfo_p.h"
#include "qtjambilink_p.h"
#include "qtjambishell_p.h"
#include "containeraccess_p.h"
#include "qtjambimetaobject_p.h"
#include <QtCore/QModelIndex>

#include "qtjambi_cast.h"

jobject CoreAPI::convertQFlagsToJavaObject(JNIEnv *env, jint qt_flags, jclass cl)
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
            method = JavaAPI::resolveMethod(env, "<init>", "(J)V", cl);
            if(method){
                obj = env->NewObject(cl, method, jlong(qt_flags));
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
                Java::QtJambi::QFlags::setIntValue(env, obj, qt_flags);
            }
        }
    }
    return obj;
}

jobject CoreAPI::convertQFlagsToJavaObject(JNIEnv *env, jlong qt_flags, jclass cl)
{
    jobject obj = nullptr;
    Q_ASSERT(cl);
    if(Java::QtJambi::QtFlagEnumerator::isAssignableFrom(env, cl)){
        cl = Java::QtCore::QMetaEnum::flagsType(env, cl);
    }
    if(Java::QtJambi::QFlags::isAssignableFrom(env, cl)){
        if(Java::QtJambi::QFlags::isSameClass(env, cl) || Java::QtJambi::QLongFlags::isSameClass(env, cl)){
            cl = Java::QtJambi::QLongFlags$ConcreteWrapper::getClass(env);
        }
        jmethodID method = JavaAPI::resolveMethod(env, "<init>", "(J)V", cl);
        if(method){
            obj = env->NewObject(cl, method, qt_flags);
        }else{
            method = JavaAPI::resolveMethod(env, "<init>", "(I)V", cl);
            if(method){
                obj = env->NewObject(cl, method, jint(qt_flags));
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
                if(Java::QtJambi::QLongFlags::isAssignableFrom(env, cl))
                    Java::QtJambi::QLongFlags::setLongValue(env, obj, qt_flags);
                else
                    Java::QtJambi::QFlags::setIntValue(env, obj, qt_flags);
            }
        }
    }
    return obj;
}

void *QtJambiAPI::convertQNativePointerToNative(JNIEnv *env, jobject java_object, int* size, int* indirections)
{
    if (!java_object)
        return nullptr;
    Q_ASSERT(Java::QtJambi::QNativePointer::isInstanceOf(env, java_object));  // check the java object is right type
    if(size)
        *size = Java::QtJambi::QNativePointer::knownSize(env, java_object);
    if(indirections)
        *indirections = Java::QtJambi::QNativePointer::indirections(env, java_object);
    return reinterpret_cast<void *>(Java::QtJambi::QNativePointer::pointer(env,java_object));
}

jobject QtJambiAPI::convertNativeToQNativePointer(JNIEnv *env, const void *qt_object, QNativePointer::Type type, quint64 size, uint indirections)
{
    return Java::QtJambi::QNativePointer::fromNative(env, reinterpret_cast<jlong>(qt_object), int(type), jlong(size), jint(indirections), false);
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
                false, [](void * ptr,bool){delete reinterpret_cast<QChar*>(ptr);}, QtJambiLink::Ownership::Cpp)) {
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
                false, [](void * ptr,bool){delete reinterpret_cast<QVariant*>(ptr);}, QtJambiLink::Ownership::Cpp)) {
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
                false, [](void * ptr,bool){delete reinterpret_cast<QString*>(ptr);}, QtJambiLink::Ownership::Cpp)) {
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
    }else if (metaType == QMetaType::fromType<jobject>()) {
        return qt_variant.value<jobject>();
    }else if (metaType == QMetaType::fromType<JObjectWrapper>()) {
        JObjectWrapper wrapper = qt_variant.value<JObjectWrapper>();
        return env->NewLocalRef(wrapper.object(env));
    }else if (metaType == QMetaType::fromType<JIntArrayWrapper>()) {
        JObjectWrapper wrapper = qt_variant.value<JIntArrayWrapper>();
        return env->NewLocalRef(wrapper.object(env));
    }else if (metaType == QMetaType::fromType<JByteArrayWrapper>()) {
        JObjectWrapper wrapper = qt_variant.value<JByteArrayWrapper>();
        return env->NewLocalRef(wrapper.object(env));
    }else if (metaType == QMetaType::fromType<JShortArrayWrapper>()) {
        JObjectWrapper wrapper = qt_variant.value<JShortArrayWrapper>();
        return env->NewLocalRef(wrapper.object(env));
    }else if (metaType == QMetaType::fromType<JLongArrayWrapper>()) {
        JObjectWrapper wrapper = qt_variant.value<JLongArrayWrapper>();
        return env->NewLocalRef(wrapper.object(env));
    }else if (metaType == QMetaType::fromType<JBooleanArrayWrapper>()) {
        JObjectWrapper wrapper = qt_variant.value<JBooleanArrayWrapper>();
        return env->NewLocalRef(wrapper.object(env));
    }else if (metaType == QMetaType::fromType<JCharArrayWrapper>()) {
        JObjectWrapper wrapper = qt_variant.value<JCharArrayWrapper>();
        return env->NewLocalRef(wrapper.object(env));
    }else if (metaType == QMetaType::fromType<JDoubleArrayWrapper>()) {
        JObjectWrapper wrapper = qt_variant.value<JDoubleArrayWrapper>();
        return env->NewLocalRef(wrapper.object(env));
    }else if (metaType == QMetaType::fromType<JFloatArrayWrapper>()) {
        JObjectWrapper wrapper = qt_variant.value<JFloatArrayWrapper>();
        return env->NewLocalRef(wrapper.object(env));
    }else if (metaType == QMetaType::fromType<JObjectArrayWrapper>()) {
        JObjectWrapper wrapper = qt_variant.value<JObjectArrayWrapper>();
        return env->NewLocalRef(wrapper.object(env));
    } else if (metaType == QMetaType::fromType<JEnumWrapper>()) {
        JEnumWrapper wrapper = qt_variant.value<JEnumWrapper>();
        return env->NewLocalRef(wrapper.object(env));
    } else if (metaType == QMetaType::fromType<JCollectionWrapper>()
               || metaType == QMetaType::fromType<JMapWrapper>()
               || metaType == QMetaType::fromType<JIteratorWrapper>()) {
        JCollectionWrapper wrapper = qt_variant.value<JCollectionWrapper>();
        return env->NewLocalRef(wrapper.object(env));
    } else if (metaType == QMetaType::fromType<JMapWrapper>()) {
         JMapWrapper wrapper = qt_variant.value<JMapWrapper>();
         return env->NewLocalRef(wrapper.object(env));
    } else if (metaType == QMetaType::fromType<JIteratorWrapper>()) {
         JIteratorWrapper wrapper = qt_variant.value<JIteratorWrapper>();
         return env->NewLocalRef(wrapper.object(env));
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    } else if (isNativeWrapperMetaType(metaType)) {
         JObjectWrapper wrapper = qt_variant.value<JObjectWrapper>();
         return env->NewLocalRef(wrapper.object(env));
#endif
    } else {
        QString qtType = QLatin1String(metaType.name());
        QString javaType = QtJambiTypeManager::getExternalTypeName(env, qtType, metaType);
        // Find usage pattern
        QtJambiUtils::InternalToExternalConverter converter = QtJambiTypeManager::getInternalToExternalConverter(
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

QMetaType qtjambiRegisterPointerMetaType(JNIEnv *env, jclass clazz, const QByteArray& _type, const QMetaObject *metaObject){
    bool isQObject = Java::QtCore::QObject::isAssignableFrom(env, clazz);
    QMetaType metaType(isQObject ? QMetaType::QObjectStar : QMetaType::VoidStar);
    const QtPrivate::QMetaTypeInterface* iface = metaType.iface();
    QMetaType::TypeFlags flags(int(iface->flags));
    if(!isQObject && metaObject)
        flags |= QMetaType::PointerToGadget;
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
                                         flags,
                                         metaObject, nullptr);
    registerJavaClassForCustomMetaType(env, _metaType, clazz);
    _metaType.id();
    return _metaType;
}

bool trivialConverter(const void *src, void *target) {
    *reinterpret_cast<void**>(target) = *reinterpret_cast<void* const*>(src);
    return true;
};

void registerConverterVariant(JNIEnv *env, QMetaType metaType, QString qtName, const QString& fullJavaName, jclass clazz, QMetaType nativeWrapperType){
    QMetaType jObjectWrapperType = QMetaType::fromType<JObjectWrapper>();
    if(metaType.isValid() && metaType!=jObjectWrapperType){
        if(metaType.flags() & QMetaType::IsPointer || isJObjectWrappedMetaType(metaType)){
            if(!QMetaType::hasRegisteredConverterFunction(QMetaType(QMetaType::Nullptr), metaType)){
                QMetaType::registerConverterFunction([metaType](const void *, void *target) -> bool {
                    metaType.construct(target);
                    return true;
                }, QMetaType(QMetaType::Nullptr), metaType);
            }
        }
        if(Java::QtCore::QObject::isAssignableFrom(env, clazz)){
            if(!QMetaType::hasRegisteredConverterFunction(jObjectWrapperType, metaType))
                QMetaType::registerConverterFunction([](const void *src, void *target) -> bool {
                                                        if(JniEnvironment env{200}){
                                                            if(src){
                                                                const JObjectWrapper* wrapper = reinterpret_cast<const JObjectWrapper*>(src);
                                                                if(Java::QtCore::QObject::isInstanceOf(env, wrapper->object(env))){
                                                                    QObject* object = QtJambiAPI::convertJavaObjectToQObject(env, wrapper->object(env));
                                                                    if(target)
                                                                        *reinterpret_cast<void**>(target) = object;
                                                                    return target && object;
                                                                }else if(env->IsSameObject(nullptr, wrapper->object(env))){
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
                                                             *reinterpret_cast<JObjectWrapper*>(target) = JObjectWrapper(env, object);
                                                             return true;
                                                         }
                                                         return false;
                                                     }, metaType, jObjectWrapperType);
        }else{
            if(isJObjectWrappedMetaType(metaType)){
                if(!QMetaType::hasRegisteredConverterFunction(jObjectWrapperType, metaType)){
                    JObjectWrapper classWrapper(env, clazz);
                    QMetaType::registerConverterFunction([classWrapper](const void *src, void *target) -> bool {
                        if(src){
                            if(JniEnvironment env{200}){
                                jobject value = reinterpret_cast<const JObjectWrapper*>(src)->object(env);
                                if(value!=nullptr){
                                    if(!env->IsInstanceOf(value, jclass(classWrapper.object(env))))
                                        return false;
                                }
                            }
                            new (target)JObjectWrapper(*reinterpret_cast<const JObjectWrapper*>(src));
                        }else{
                            new (target)JObjectWrapper();
                        }
                        return true;
                    }, jObjectWrapperType, metaType);
                }
                if(!QMetaType::hasRegisteredConverterFunction(metaType, jObjectWrapperType)){
                    QMetaType::registerConverterFunction([](const void *src, void *target) -> bool {
                        if(src){
                            new (target)JObjectWrapper(*reinterpret_cast<const JObjectWrapper*>(src));
                        }else{
                            new (target)JObjectWrapper();
                        }
                        return true;
                    }, metaType, jObjectWrapperType);
                }
                if(!QMetaType::hasRegisteredConverterFunction(metaType, QMetaType::fromType<QString>()))
                    QMetaType::registerConverterFunction([](const void *src, void *target) -> bool {
                        const JObjectWrapper* wrapper = reinterpret_cast<const JObjectWrapper*>(src);
                        *reinterpret_cast<QString*>(target) = wrapper->toString();
                        return true;
                    }, metaType, QMetaType::fromType<QString>());
            }else{
                if(!QMetaType::hasRegisteredConverterFunction(jObjectWrapperType, metaType)){
                    QtJambiUtils::ExternalToInternalConverter converter = QtJambiTypeManager::tryGetExternalToInternalConverter(env, clazz, qtName, metaType);
                    clazz = getGlobalClassRef(env, clazz);
                    if(converter){
                        QMetaType::registerConverterFunction([clazz, metaType, converter](const void *src, void *target) -> bool {
                            if(src){
                                if(JniEnvironment env{500}){
                                    const JObjectWrapper * wrapper = reinterpret_cast<const JObjectWrapper *>(src);
                                    if(!wrapper->object(env) || env->IsInstanceOf(wrapper->object(env), clazz)){
                                        if(!target)
                                            metaType.construct(target, nullptr);
                                        if(!wrapper->object(env)){
                                            return true;
                                        }else{
                                            jvalue val;
                                            val.l = wrapper->object(env);
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
                                    if(!wrapper->object(env) || env->IsInstanceOf(wrapper->object(env), clazz)){
                                        if(!target)
                                            metaType.construct(target, nullptr);
                                        if(!wrapper->object(env)){
                                            return true;
                                        }else{
                                            jvalue val;
                                            val.l = wrapper->object(env);
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
                    QtJambiUtils::InternalToExternalConverter converter = QtJambiTypeManager::tryGetInternalToExternalConverter(env, qtName, metaType, clazz);
                    if(converter){
                        QMetaType::registerConverterFunction([converter](const void *src, void *target) -> bool {
                            if(!src){
                                *reinterpret_cast<JObjectWrapper*>(target) = JObjectWrapper();
                                return true;
                            }else if(JniEnvironment env{500}){
                                jvalue val;
                                val.l = nullptr;
                                if(converter(env, nullptr, src, val, true)){
                                    *reinterpret_cast<JObjectWrapper*>(target) = JObjectWrapper(env, val.l);
                                    return true;
                                }
                            }
                            return false;
                        }, metaType, jObjectWrapperType);
                    }else{
                        clazz = getGlobalClassRef(env, clazz);
                        QMetaType::registerConverterFunction([converter, qtName, metaType, clazz](const void *src, void *target) mutable -> bool {
                            if(!src){
                                *reinterpret_cast<JObjectWrapper*>(target) = JObjectWrapper();
                                return true;
                            }else if(JniEnvironment env{500}){
                                if(!converter){
                                    converter = QtJambiTypeManager::getInternalToExternalConverter(env, qtName, metaType, clazz);
                                    Q_ASSERT(converter);
                                }
                                jvalue val;
                                val.l = nullptr;
                                if(converter(env, nullptr, src, val, true)){
                                    *reinterpret_cast<JObjectWrapper*>(target) = JObjectWrapper(env, val.l);
                                    return true;
                                }
                            }
                            return false;
                        }, metaType, jObjectWrapperType);
                    }
                }
            }
        }
        if(nativeWrapperType.isValid()){
            if(nativeWrapperType.flags().testFlag(QMetaType::PointerToQObject)){
                if(!QMetaType::hasRegisteredConverterFunction(nativeWrapperType, jObjectWrapperType))
                    QMetaType::registerConverterFunction([](const void *src, void *target) -> bool {
                        const JQObjectWrapper* wrapper = reinterpret_cast<const JQObjectWrapper*>(src);
                        if(JniEnvironment env{200}){
                            *reinterpret_cast<JObjectWrapper*>(target) = wrapper->javaObject(env);
                            return true;
                        }
                        return false;
                    }, nativeWrapperType, jObjectWrapperType);
                if(!QMetaType::hasRegisteredConverterFunction(nativeWrapperType, QMetaType::fromType<QString>()))
                    QMetaType::registerConverterFunction([](const void *src, void *target) -> bool {
                        const JQObjectWrapper* wrapper = reinterpret_cast<const JQObjectWrapper*>(src);
                        *reinterpret_cast<QString*>(target) = QVariant::fromValue(wrapper->qobject()).toString();
                        return true;
                    }, nativeWrapperType, QMetaType::fromType<QString>());
                if(!QMetaType::hasRegisteredConverterFunction(nativeWrapperType, metaType)){
                    if(Java::Runtime::Class::isInterface(env, clazz)){
                        if(const std::type_info* interfaceTypeId = getTypeByJavaName(fullJavaName)){
                            QMetaType::registerConverterFunction([interfaceTypeId](const void *src, void *target) -> bool {
                                const JQObjectWrapper* wrapper = reinterpret_cast<const JQObjectWrapper*>(src);
                                if(QSharedPointer<QtJambiLink> link = wrapper->link()){
                                    *reinterpret_cast<void**>(target) = link->typedPointer(*interfaceTypeId);
                                    return true;
                                }
                                return false;
                            }, nativeWrapperType, metaType);
                        }
                    }else{
                        QMetaType::registerConverterFunction([](const void *src, void *target) -> bool {
                            const JQObjectWrapper* wrapper = reinterpret_cast<const JQObjectWrapper*>(src);
                            *reinterpret_cast<void**>(target) = wrapper->qobject();
                            return true;
                        }, nativeWrapperType, metaType);
                    }
                }
            }else{
                if(!QMetaType::hasRegisteredConverterFunction(nativeWrapperType, jObjectWrapperType))
                    QMetaType::registerConverterFunction([](const void *src, void *target) -> bool {
                        const JObjectWrapper* wrapper = reinterpret_cast<const JObjectWrapper*>(src);
                        *reinterpret_cast<JObjectWrapper*>(target) = *wrapper;
                        return true;
                    }, nativeWrapperType, jObjectWrapperType);
                if(!QMetaType::hasRegisteredConverterFunction(nativeWrapperType, QMetaType::fromType<QString>()))
                    QMetaType::registerConverterFunction([](const void *src, void *target) -> bool {
                        const JObjectWrapper* wrapper = reinterpret_cast<const JObjectWrapper*>(src);
                        bool ok = false;
                        *reinterpret_cast<QString*>(target) = wrapper->toString(&ok);
                        return ok;
                    }, nativeWrapperType, QMetaType::fromType<QString>());
                if(!QMetaType::hasRegisteredConverterFunction(nativeWrapperType, metaType) && metaType.flags().testFlag(QMetaType::IsPointer)){
                    if(Java::Runtime::Class::isInterface(env, clazz)){
                        if(const std::type_info* interfaceTypeId = getTypeByJavaName(fullJavaName)){
                            QMetaType::registerConverterFunction([interfaceTypeId](const void *src, void *target) -> bool {
                                const JObjectWrapper* wrapper = reinterpret_cast<const JObjectWrapper*>(src);
                                if(JniEnvironment env{200}){
                                    if (QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForJavaInterface(env, wrapper->object(env))){
                                        *reinterpret_cast<void**>(target) = link->typedPointer(*interfaceTypeId);
                                        return true;
                                    }
                                }
                                return false;
                            }, nativeWrapperType, metaType);
                        }
                    }else{
                        QSharedPointer<QtJambiLink> (*findLinkForJavaObject)(JNIEnv*,jobject)
                            = Java::QtJambi::NativeUtility$Object::isAssignableFrom(env, clazz) ? QtJambiLink::findLinkForJavaObject : QtJambiLink::findLinkForJavaInterface;
                        QMetaType::registerConverterFunction([findLinkForJavaObject](const void *src, void *target) -> bool {
                            const JObjectWrapper* wrapper = reinterpret_cast<const JObjectWrapper*>(src);
                            if(JniEnvironment env{200}){
                                if (QSharedPointer<QtJambiLink> link = findLinkForJavaObject(env, wrapper->object(env))){
                                    *reinterpret_cast<void**>(target) = link->pointer();
                                    return true;
                                }
                            }
                            return false;
                        }, nativeWrapperType, metaType);
                    }
                }
            }
        }
    }
    const std::type_info* typeId = getTypeByJavaName(fullJavaName);
    jclass closestQtSuperclass{nullptr};
    if(!typeId){
        closestQtSuperclass = JavaAPI::resolveClosestQtSuperclass(env, clazz);
        if(!env->IsSameObject(closestQtSuperclass, nullptr) && !env->IsSameObject(clazz, closestQtSuperclass)){
            typeId = getTypeByJavaName(QtJambiAPI::getClassName(env, closestQtSuperclass).replace(".", "/"));
        }else{
            closestQtSuperclass = nullptr;
        }
    }
    const QList<jclass> hirarchy = getFlatClassHirarchy(env, clazz);
    for(jclass superClass : qAsConst(hirarchy)){
        if(!env->IsSameObject(clazz, superClass)
            && !Java::QtJambi::QtObjectInterface::isSameClass(env, superClass)
            && !Java::QtJambi::QtObject::isSameClass(env, superClass)
            && !Java::Runtime::Object::isSameClass(env, superClass)
            && env->GetArrayLength(Java::Runtime::Class::getTypeParameters(env, superClass))==0){
            QString _fullJavaName = QtJambiAPI::getClassName(env, superClass).replace(".", "/");
            QString _qtName = QtJambiTypeManager::getInternalTypeName(env, superClass, false);
            QMetaType _metaType = QMetaType::fromName(_qtName.toLatin1());
            const std::type_info* superTypeId = getTypeByJavaName(_fullJavaName);
            if(!_metaType.isValid() && _qtName.endsWith("*") && superTypeId){
                _metaType = qtjambiRegisterPointerMetaType(env, superClass, _qtName.toUtf8(), registeredOriginalMetaObject(*superTypeId));
            }
            if(_metaType.isValid()){
                if(!QMetaType::hasRegisteredConverterFunction(metaType, _metaType)){
                    if(metaType==jObjectWrapperType
                        || JObjectValueWrapper::isValueType(metaType)
                        || isJObjectWrappedMetaType(metaType)){
                        superClass = getGlobalClassRef(env, superClass);
                        QSharedPointer<QtJambiLink> (*findLinkForJavaObject)(JNIEnv*,jobject)
                            = Java::QtJambi::NativeUtility$Object::isAssignableFrom(env, superClass) ? QtJambiLink::findLinkForJavaObject : QtJambiLink::findLinkForJavaInterface;
                        if(_qtName.endsWith("*")){
                            QMetaType::registerConverterFunction([superClass, superTypeId, findLinkForJavaObject](const void *src, void *target) -> bool {
                                if(JniEnvironment env{200}){
                                    if(src){
                                        const JObjectWrapper* wrapper = reinterpret_cast<const JObjectWrapper*>(src);
                                        if(!wrapper->object(env) || env->IsInstanceOf(wrapper->object(env), superClass)){
                                            void* object = nullptr;
                                            if(QSharedPointer<QtJambiLink> link = findLinkForJavaObject(env, wrapper->object(env)))
                                                object = superTypeId ? link->typedPointer(*superTypeId) : link->pointer();
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
                            QMetaType::registerConverterFunction([superClass, superTypeId, findLinkForJavaObject, _metaType](const void *src, void *target) -> bool {
                                if(JniEnvironment env{200}){
                                    if(src){
                                        const JObjectWrapper* wrapper = reinterpret_cast<const JObjectWrapper*>(src);
                                        if(env->IsInstanceOf(wrapper->object(env), superClass)){
                                            void* object = nullptr;
                                            if(QSharedPointer<QtJambiLink> link = findLinkForJavaObject(env, wrapper->object(env)))
                                                object = superTypeId ? link->typedPointer(*superTypeId) : link->pointer();
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
                    }else if(typeId && superTypeId){
                        int offset = registeredInterfaceOffset(*typeId, *superTypeId);
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
                registerConverterVariant(env, _metaType, _qtName, _fullJavaName, superClass, nativeWrapperType);
            }
        }
    }
}
#endif

bool hasReferenceCounts(JNIEnv * env, jobject container);
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
QList<int> registeredCustomMetaTypesForJavaClass(const QByteArray& javaClass);
#else
QList<QMetaType> registeredCustomMetaTypesForJavaClass(const QByteArray& javaClass);
#endif

QVariant internal_convertJavaObjectToQVariant(JNIEnv *env, jobject java_object, bool convert){
    static ResettableBoolFlag nullAsNull(env, "io.qt.experimental.javanull-as-nullvariant");
    if (!java_object)
        return nullAsNull ? QVariant(META_TYPE(QMetaType::Type::Nullptr), nullptr) : QVariant();

    jclass object_class = env->GetObjectClass(java_object);
    if (!object_class)
        return QVariant();

    // Test some quick ones first...
    if (Java::QtCore::QVariant::isInstanceOf(env, java_object)) {
        QVariant* variant = QtJambiAPI::convertJavaObjectToNative<QVariant>(env, java_object);
        if(variant)
            return *variant;
        else return QVariant();
    } else if (Java::QtCore::QVariant$Null::isInstanceOf(env, java_object)) {
        jint metaTypeID = Java::QtCore::QVariant$Null::metaTypeID(env, java_object);
        return QVariant(META_TYPE(metaTypeID), nullptr);
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
        int value = Java::QtJambi::QFlags::intValue(env, java_object);
        return QVariant(FROM_META_TYPE(internalMetaType), &value);
#if QT_VERSION >= QT_VERSION_CHECK(6, 9, 0)
    } else if (Java::QtCore::QMetaType$GenericLongFlags::isSameClass(env, object_class)) {
        const QMetaType& internalMetaType = qtjambi_cast<const QMetaType&>(env, Java::QtCore::QMetaType$GenericTypeInterface::metaType(env, java_object));
        if(!convert)
            return QVariant(FROM_META_TYPE(internalMetaType), nullptr);
        jlong value = Java::QtJambi::QLongFlags::longValue(env, java_object);
        return QVariant(FROM_META_TYPE(internalMetaType), &value);
#endif
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
#if QT_VERSION >= QT_VERSION_CHECK(6,7,0)
    } else if (Java::QtCore::AbstractSpan::isInstanceOf(env, java_object)) {
        if(QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForJavaObject(env, java_object)){
            if(AbstractSpanAccess* access = dynamic_cast<AbstractSpanAccess*>(link->containerAccess())){
                QMetaType containerMetaType(access->registerContainer(QStringLiteral(u"QSpan<%1%2>").arg(access->isConst() ? "const " : "", access->elementMetaType().name()).toLatin1()));
                registerContainerConverter(access->isConst() ? SequentialContainerType::QConstSpan : SequentialContainerType::QSpan, containerMetaType, access->elementMetaType());
                return QVariant::fromValue(JObjectWrapper(env, java_object));
                //QtJambiSpan* span = reinterpret_cast<QtJambiSpan*>(link->pointer());
                //return QVariant(META_TYPE(id), span);
            }else{
                return QVariant();
            }
        }else{
            Java::QtJambi::QNoNativeResourcesException::throwNew(env, QStringLiteral("Incomplete object of type: %1").arg(QtJambiAPI::getClassName(env, object_class).replace("$", ".")) QTJAMBI_STACKTRACEINFO );
        }
#endif
    } else if (Java::QtCore::QPair::isInstanceOf(env, java_object)) {
        jobject first = Java::QtCore::QPair::first(env, java_object);
        jobject second = Java::QtCore::QPair::second(env, java_object);
        jclass firstClass = first ? env->GetObjectClass(first) : nullptr;
        jclass secondClass = first ? env->GetObjectClass(second) : nullptr;
        QMetaType firstMetaType(first ? getMetaTypeIdForObject(env, first) : QMetaType::Nullptr);
        QMetaType secondMetaType(second ? getMetaTypeIdForObject(env, second) : QMetaType::Nullptr);
        if(firstMetaType.isValid() && secondMetaType.isValid()){
            AbstractContainerAccess* containerAccess = ContainerAccessAPI::createContainerAccess(AssociativeContainerType::QPair, firstMetaType, secondMetaType);
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
                QtJambiUtils::QHashFunction hashFunction1 = QtJambiTypeManager::findHashFunction(isPointer1, firstMetaType.id());
                QtJambiUtils::QHashFunction hashFunction2 = QtJambiTypeManager::findHashFunction(isPointer2, secondMetaType.id());
                QtJambiUtils::InternalToExternalConverter memberConverter1 = QtJambiTypeManager::getInternalToExternalConverter(
                                                                env,
                                                                firstType,
                                                                firstMetaType,
                                                                firstClass);
                QtJambiUtils::InternalToExternalConverter memberConverter2 = QtJambiTypeManager::getInternalToExternalConverter(
                                                                env,
                                                                secondType,
                                                                secondMetaType,
                                                                secondClass);
                QtJambiUtils::ExternalToInternalConverter memberReConverter1 = QtJambiTypeManager::getExternalToInternalConverter(
                                                                env, firstClass, firstType, firstMetaType);
                QtJambiUtils::ExternalToInternalConverter memberReConverter2 = QtJambiTypeManager::getExternalToInternalConverter(
                                                                env, secondClass, secondType, secondMetaType);
                QSharedPointer<AbstractContainerAccess> memberNestedContainerAccess1 = findContainerAccess(firstMetaType);
                QSharedPointer<AbstractContainerAccess> memberNestedContainerAccess2 = findContainerAccess(secondMetaType);
                const std::type_info* typeId = getTypeByQtName(firstMetaType.name());
                if(!typeId){
                    typeId = getTypeByMetaType(firstMetaType);
                }
                PtrOwnerFunction memberOwnerFunction1 = nullptr;
                if(typeId)
                    memberOwnerFunction1 = ContainerAPI::registeredOwnerFunction(*typeId);
                typeId = getTypeByQtName(secondMetaType.name());
                if(!typeId){
                    typeId = getTypeByMetaType(secondMetaType);
                }
                PtrOwnerFunction memberOwnerFunction2 = nullptr;
                if(typeId)
                    memberOwnerFunction2 = ContainerAPI::registeredOwnerFunction(*typeId);
                containerAccess = ContainerAccessAPI::createContainerAccess(
                                               env, AssociativeContainerType::QPair,
                                               firstMetaType,
                                               align1, size1,
                                               isPointer1,
                                               hashFunction1,
                                               memberConverter1,
                                               memberReConverter1,
                                               memberNestedContainerAccess1,
                                               memberOwnerFunction1,
                                               secondMetaType,
                                               align2, size2,
                                               isPointer2,
                                               hashFunction2,
                                               memberConverter2,
                                               memberReConverter2,
                                               memberNestedContainerAccess2,
                                               memberOwnerFunction2);
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
                    QString qtName;
                    if (Java::QtCore::QList::isSameClass(env, object_class)
                        || Java::QtCore::QQueue::isSameClass(env, object_class)
                        || Java::QtCore::QStack::isSameClass(env, object_class)) {
                        qtName = QLatin1String("QList<%1>");
                        qtName = qtName.arg(QLatin1String(containerAccess->elementMetaType().name()));
                    }else{
                        QString javaClassName = QtJambiAPI::getClassName(env, object_class).replace('.', '/');
                        if(const std::type_info* typeId = getTypeByJavaName(javaClassName)){
                            int id = registeredMetaTypeID(*typeId);
                            if(id==QMetaType::UnknownType){
                                QByteArray qtType = getQtName(*typeId);
                                EntryTypes entryType = getEntryType(*typeId);
                                switch(entryType){
                                case EntryTypes::ObjectTypeInfo:
                                case EntryTypes::QObjectTypeInfo:
                                case EntryTypes::InterfaceTypeInfo:
                                {
                                    qtType += "*";
                                }
                                break;
                                default: break;
                                }
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
                                id = QMetaType::type(qtType);
#else
                                id = QMetaType::fromName(qtType).id();
#endif
                            }
                            if(id!=QMetaType::UnknownType){
                                if(hasReferenceCounts(env, link->getJavaObjectLocalRef(env))){
                                    return QVariant::fromValue(JCollectionWrapper(env, java_object));
                                }else{
                                    return QVariant(META_TYPE(id), link->pointer());
                                }
                            }
                        }else{
                            const auto types = registeredCustomMetaTypesForJavaClass(javaClassName.toUtf8());
                            if(!types.isEmpty()){
                                if(hasReferenceCounts(env, link->getJavaObjectLocalRef(env))){
                                    return QVariant::fromValue(JCollectionWrapper(env, java_object));
                                }else{
                                    return QVariant(types[0], link->pointer());
                                }
                            }
                        }
                    }
                    QMetaType containerMetaType(containerAccess->registerContainer(qtName.toLatin1()));
                    if(hasReferenceCounts(env, link->getJavaObjectLocalRef(env))){
                        registerContainerConverter(SequentialContainerType::QList, containerMetaType, containerAccess->elementMetaType());
                        return QVariant::fromValue(JCollectionWrapper(env, java_object));
                    }else{
                        return QVariant(FROM_META_TYPE(containerMetaType), link->pointer());
                    }
                }
            }else{
                Java::QtJambi::QNoNativeResourcesException::throwNew(env, QStringLiteral("Incomplete object of type: %1").arg(QtJambiAPI::getClassName(env, object_class).replace("$", ".")) QTJAMBI_STACKTRACEINFO );
            }
        }else if (Java::QtCore::QSet::isInstanceOf(env, java_object)) {
            if (QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForJavaObject(env, java_object)){
                if(AbstractSetAccess* containerAccess = dynamic_cast<AbstractSetAccess*>(link->containerAccess())){
                    QString qtName;
                    if (Java::QtCore::QSet::isSameClass(env, object_class)) {
                        qtName = QLatin1String("QSet<%1>");
                        qtName = qtName.arg(QLatin1String(containerAccess->elementMetaType().name()));
                    }else{
                        QString javaClassName = QtJambiAPI::getClassName(env, object_class).replace('.', '/');
                        if(const std::type_info* typeId = getTypeByJavaName(javaClassName)){
                            int id = registeredMetaTypeID(*typeId);
                            if(id==QMetaType::UnknownType){
                                QByteArray qtType = getQtName(*typeId);
                                EntryTypes entryType = getEntryType(*typeId);
                                switch(entryType){
                                case EntryTypes::ObjectTypeInfo:
                                case EntryTypes::QObjectTypeInfo:
                                case EntryTypes::InterfaceTypeInfo:
                                {
                                    qtType += "*";
                                }
                                break;
                                default: break;
                                }
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
                                id = QMetaType::type(qtType);
#else
                                id = QMetaType::fromName(qtType).id();
#endif
                            }
                            if(id!=QMetaType::UnknownType){
                                if(hasReferenceCounts(env, link->getJavaObjectLocalRef(env))){
                                    return QVariant::fromValue(JCollectionWrapper(env, java_object));
                                }else{
                                    return QVariant(META_TYPE(id), link->pointer());
                                }
                            }
                        }else{
                            const auto types = registeredCustomMetaTypesForJavaClass(javaClassName.toUtf8());
                            if(!types.isEmpty()){
                                if(hasReferenceCounts(env, link->getJavaObjectLocalRef(env))){
                                    return QVariant::fromValue(JCollectionWrapper(env, java_object));
                                }else{
                                    return QVariant(types[0], link->pointer());
                                }
                            }
                        }
                    }
                    QMetaType containerMetaType(containerAccess->registerContainer(qtName.toLatin1()));
                    if(hasReferenceCounts(env, link->getJavaObjectLocalRef(env))){
                        registerContainerConverter(SequentialContainerType::QSet, containerMetaType, containerAccess->elementMetaType());
                        return QVariant::fromValue(JCollectionWrapper(env, java_object));
                    }else{
                        return QVariant(FROM_META_TYPE(containerMetaType), link->pointer());
                    }
                }
            }else{
                Java::QtJambi::QNoNativeResourcesException::throwNew(env, QStringLiteral("Incomplete object of type: %1").arg(QtJambiAPI::getClassName(env, object_class).replace("$", ".")) QTJAMBI_STACKTRACEINFO );
            }
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
        }else if (Java::QtCore::QLinkedList::isInstanceOf(env, java_object)) {
            if (QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForJavaObject(env, java_object)){
                if(AbstractLinkedListAccess* containerAccess = dynamic_cast<AbstractLinkedListAccess*>(link->containerAccess())){
                    QString qtName;
                    if (Java::QtCore::QLinkedList::isSameClass(env, object_class)) {
                        qtName = QLatin1String("QLinkedList<%1>");
                        qtName = qtName.arg(QLatin1String(containerAccess->elementMetaType().name()));
                    }else{
                        QString javaClassName = QtJambiAPI::getClassName(env, object_class).replace('.', '/');
                        if(const std::type_info* typeId = getTypeByJavaName(javaClassName)){
                            int id = registeredMetaTypeID(*typeId);
                            if(id==QMetaType::UnknownType){
                                QByteArray qtType = getQtName(*typeId);
                                EntryTypes entryType = getEntryType(*typeId);
                                switch(entryType){
                                case EntryTypes::ObjectTypeInfo:
                                case EntryTypes::QObjectTypeInfo:
                                case EntryTypes::InterfaceTypeInfo:
                                {
                                    qtType += "*";
                                }
                                break;
                                default: break;
                                }
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
                                id = QMetaType::type(qtType);
#else
                                id = QMetaType::fromName(qtType).id();
#endif
                            }
                            if(id!=QMetaType::UnknownType){
                                if(hasReferenceCounts(env, link->getJavaObjectLocalRef(env))){
                                    return QVariant::fromValue(JCollectionWrapper(env, java_object));
                                }else{
                                    return QVariant(META_TYPE(id), link->pointer());
                                }
                            }
                        }else{
                            const auto types = registeredCustomMetaTypesForJavaClass(javaClassName.toUtf8());
                            if(!types.isEmpty()){
                                if(hasReferenceCounts(env, link->getJavaObjectLocalRef(env))){
                                    return QVariant::fromValue(JCollectionWrapper(env, java_object));
                                }else{
                                    return QVariant(types[0], link->pointer());
                                }
                            }
                        }
                    }
                    QMetaType containerMetaType(containerAccess->registerContainer(qtName.toLatin1()));
                    registerContainerConverter(SequentialContainerType::QLinkedList, containerMetaType, containerAccess->elementMetaType());
                    if(hasReferenceCounts(env, link->getJavaObjectLocalRef(env))){
                        return QVariant::fromValue(JCollectionWrapper(env, java_object));
                    }else{
                        return QVariant(FROM_META_TYPE(containerMetaType), link->pointer());
                    }
                }
            }else{
                Java::QtJambi::QNoNativeResourcesException::throwNew(env, QStringLiteral("Incomplete object of type: %1").arg(QtJambiAPI::getClassName(env, object_class).replace("$", ".")) QTJAMBI_STACKTRACEINFO );
            }
        }else if (Java::QtCore::QVector::isInstanceOf(env, java_object)) {
            if (QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForJavaObject(env, java_object)){
                if(AbstractVectorAccess* containerAccess = dynamic_cast<AbstractVectorAccess*>(link->containerAccess())){
                    QString qtName;
                    if (Java::QtCore::QVector::isSameClass(env, object_class)
                        || Java::QtCore::QStack::isSameClass(env, object_class)) {
                        qtName = QLatin1String("QVector<%1>");
                        qtName = qtName.arg(QLatin1String(containerAccess->elementMetaType().name()));
                    }else{
                        QString javaClassName = QtJambiAPI::getClassName(env, object_class).replace('.', '/');
                        if(const std::type_info* typeId = getTypeByJavaName(javaClassName)){
                            int id = registeredMetaTypeID(*typeId);
                            if(id==QMetaType::UnknownType){
                                QByteArray qtType = getQtName(*typeId);
                                EntryTypes entryType = getEntryType(*typeId);
                                switch(entryType){
                                case EntryTypes::ObjectTypeInfo:
                                case EntryTypes::QObjectTypeInfo:
                                case EntryTypes::InterfaceTypeInfo:
                                {
                                    qtType += "*";
                                }
                                break;
                                default: break;
                                }
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
                                id = QMetaType::type(qtType);
#else
                                id = QMetaType::fromName(qtType).id();
#endif
                            }
                            if(id!=QMetaType::UnknownType){
                                if(hasReferenceCounts(env, link->getJavaObjectLocalRef(env))){
                                    return QVariant::fromValue(JCollectionWrapper(env, java_object));
                                }else{
                                    return QVariant(META_TYPE(id), link->pointer());
                                }
                            }
                        }else{
                            const auto types = registeredCustomMetaTypesForJavaClass(javaClassName.toUtf8());
                            if(!types.isEmpty()){
                                if(hasReferenceCounts(env, link->getJavaObjectLocalRef(env))){
                                    return QVariant::fromValue(JCollectionWrapper(env, java_object));
                                }else{
                                    return QVariant(types[0], link->pointer());
                                }
                            }
                        }
                    }
                    QMetaType containerMetaType(containerAccess->registerContainer(qtName.toLatin1()));
                    registerContainerConverter(SequentialContainerType::QVector, containerMetaType, containerAccess->elementMetaType());
                    if(hasReferenceCounts(env, link->getJavaObjectLocalRef(env))){
                        return QVariant::fromValue(JCollectionWrapper(env, java_object));
                    }else{
                        return QVariant(FROM_META_TYPE(containerMetaType), link->pointer());
                    }
                }
            }else{
                Java::QtJambi::QNoNativeResourcesException::throwNew(env, QStringLiteral("Incomplete object of type: %1").arg(QtJambiAPI::getClassName(env, object_class).replace("$", ".")) QTJAMBI_STACKTRACEINFO );
            }
#endif
        }
        if(Java::Runtime::Collection::size(env, java_object)>0){
            jobject iter = QtJambiAPI::iteratorOfJavaIterable(env, java_object);
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
                iter = QtJambiAPI::iteratorOfJavaIterable(env, java_object);
                while(QtJambiAPI::hasJavaIteratorNext(env, iter)){
                    element = QtJambiAPI::nextOfJavaIterator(env, iter);
                    list << qtjambi_cast<QString>(env, element);
                }
                return QVariant::fromValue(list);
            }else if(elementMetaTypeId == QMetaType::QVariant){
                QVariantList list;
                iter = QtJambiAPI::iteratorOfJavaIterable(env, java_object);
                while(QtJambiAPI::hasJavaIteratorNext(env, iter)){
                    element = QtJambiAPI::nextOfJavaIterator(env, iter);
                    list << QtJambiAPI::convertJavaObjectToQVariant(env, element);
                }
                return QVariant::fromValue(list);
            }else if(elementMetaTypeId != QMetaType::UnknownType && elementMetaTypeId != QMetaType::Nullptr){
                if (Java::Runtime::Set::isInstanceOf(env, java_object)) {
                    return internal_convertJavaObjectToQVariant(env, Java::QtCore::QSet::newInstance2(env, qtjambi_cast<jobject>(env, QMetaType(elementMetaTypeId)), java_object), convert);
                }else if (Java::Runtime::Deque::isInstanceOf(env, java_object)) {
                    return internal_convertJavaObjectToQVariant(env, Java::QtCore::QStack::newInstance2(env, qtjambi_cast<jobject>(env, QMetaType(elementMetaTypeId)), java_object), convert);
                }else if (Java::Runtime::Queue::isInstanceOf(env, java_object)) {
                    return internal_convertJavaObjectToQVariant(env, Java::QtCore::QQueue::newInstance2(env, qtjambi_cast<jobject>(env, QMetaType(elementMetaTypeId)), java_object), convert);
                }else{
                    return internal_convertJavaObjectToQVariant(env, Java::QtCore::QList::newInstance2(env, qtjambi_cast<jobject>(env, QMetaType(elementMetaTypeId)), java_object), convert);
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
                    QString qtName;
                    if (Java::QtCore::QMap::isSameClass(env, object_class)) {
                        qtName = QLatin1String("QMap<%1,%2>");
                        qtName = qtName.arg(QLatin1String(containerAccess->keyMetaType().name()))
                                       .arg(QLatin1String(containerAccess->valueMetaType().name()));
                    }else{
                        QString javaClassName = QtJambiAPI::getClassName(env, object_class).replace('.', '/');
                        if(const std::type_info* typeId = getTypeByJavaName(javaClassName)){
                            int id = registeredMetaTypeID(*typeId);
                            if(id==QMetaType::UnknownType){
                                QByteArray qtType = getQtName(*typeId);
                                EntryTypes entryType = getEntryType(*typeId);
                                switch(entryType){
                                case EntryTypes::ObjectTypeInfo:
                                case EntryTypes::QObjectTypeInfo:
                                case EntryTypes::InterfaceTypeInfo:
                                {
                                    qtType += "*";
                                }
                                break;
                                default: break;
                                }
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
                                id = QMetaType::type(qtType);
#else
                                id = QMetaType::fromName(qtType).id();
#endif
                            }
                            if(id!=QMetaType::UnknownType){
                                if(hasReferenceCounts(env, link->getJavaObjectLocalRef(env))){
                                    return QVariant::fromValue(JCollectionWrapper(env, java_object));
                                }else{
                                    return QVariant(META_TYPE(id), link->pointer());
                                }
                            }
                        }else{
                            const auto types = registeredCustomMetaTypesForJavaClass(javaClassName.toUtf8());
                            if(!types.isEmpty()){
                                if(hasReferenceCounts(env, link->getJavaObjectLocalRef(env))){
                                    return QVariant::fromValue(JCollectionWrapper(env, java_object));
                                }else{
                                    return QVariant(types[0], link->pointer());
                                }
                            }
                        }
                    }
                    QMetaType containerMetaType(containerAccess->registerContainer(qtName.toLatin1()));
                    if(hasReferenceCounts(env, link->getJavaObjectLocalRef(env))){
                        registerContainerConverter(AssociativeContainerType::QMap, containerMetaType, containerAccess->keyMetaType(), containerAccess->valueMetaType());
                        return QVariant::fromValue(JMapWrapper(env, java_object));
                    }else{
                        return QVariant(FROM_META_TYPE(containerMetaType), link->pointer());
                    }
                }
            }else{
                Java::QtJambi::QNoNativeResourcesException::throwNew(env, QStringLiteral("Incomplete object of type: %1").arg(QtJambiAPI::getClassName(env, object_class).replace("$", ".")) QTJAMBI_STACKTRACEINFO );
            }
        }else if (Java::QtCore::QHash::isInstanceOf(env, java_object)) {
            if (QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForJavaObject(env, java_object)){
                if(AbstractHashAccess* containerAccess = dynamic_cast<AbstractHashAccess*>(link->containerAccess())){
                    QString qtName;
                    if (Java::QtCore::QHash::isSameClass(env, object_class)) {
                        qtName = QLatin1String("QHash<%1,%2>");
                        qtName = qtName.arg(QLatin1String(containerAccess->keyMetaType().name()))
                                       .arg(QLatin1String(containerAccess->valueMetaType().name()));
                    }else{
                        QString javaClassName = QtJambiAPI::getClassName(env, object_class).replace('.', '/');
                        if(const std::type_info* typeId = getTypeByJavaName(javaClassName)){
                            int id = registeredMetaTypeID(*typeId);
                            if(id==QMetaType::UnknownType){
                                QByteArray qtType = getQtName(*typeId);
                                EntryTypes entryType = getEntryType(*typeId);
                                switch(entryType){
                                case EntryTypes::ObjectTypeInfo:
                                case EntryTypes::QObjectTypeInfo:
                                case EntryTypes::InterfaceTypeInfo:
                                {
                                    qtType += "*";
                                }
                                break;
                                default: break;
                                }
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
                                id = QMetaType::type(qtType);
#else
                                id = QMetaType::fromName(qtType).id();
#endif
                            }
                            if(id!=QMetaType::UnknownType){
                                if(hasReferenceCounts(env, link->getJavaObjectLocalRef(env))){
                                    return QVariant::fromValue(JCollectionWrapper(env, java_object));
                                }else{
                                    return QVariant(META_TYPE(id), link->pointer());
                                }
                            }
                        }else{
                            const auto types = registeredCustomMetaTypesForJavaClass(javaClassName.toUtf8());
                            if(!types.isEmpty()){
                                if(hasReferenceCounts(env, link->getJavaObjectLocalRef(env))){
                                    return QVariant::fromValue(JCollectionWrapper(env, java_object));
                                }else{
                                    return QVariant(types[0], link->pointer());
                                }
                            }
                        }
                    }
                    QMetaType containerMetaType(containerAccess->registerContainer(qtName.toLatin1()));
                    if(hasReferenceCounts(env, link->getJavaObjectLocalRef(env))){
                        registerContainerConverter(AssociativeContainerType::QHash, containerMetaType, containerAccess->keyMetaType(), containerAccess->valueMetaType());
                        return QVariant::fromValue(JMapWrapper(env, java_object));
                    }else{
                        return QVariant(FROM_META_TYPE(containerMetaType), link->pointer());
                    }
                }
            }else{
                Java::QtJambi::QNoNativeResourcesException::throwNew(env, QStringLiteral("Incomplete object of type: %1").arg(QtJambiAPI::getClassName(env, object_class).replace("$", ".")) QTJAMBI_STACKTRACEINFO );
            }
        }else if (Java::QtCore::QMultiMap::isInstanceOf(env, java_object)) {
            if (QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForJavaObject(env, java_object)){
                if(AbstractMultiMapAccess* containerAccess = dynamic_cast<AbstractMultiMapAccess*>(link->containerAccess())){
                    QString qtName;
                    if (Java::QtCore::QMultiMap::isSameClass(env, object_class)) {
                        qtName = QLatin1String("QMultiMap<%1,%2>");
                        qtName = qtName.arg(QLatin1String(containerAccess->keyMetaType().name()))
                                       .arg(QLatin1String(containerAccess->valueMetaType().name()));
                    }else{
                        QString javaClassName = QtJambiAPI::getClassName(env, object_class).replace('.', '/');
                        if(const std::type_info* typeId = getTypeByJavaName(javaClassName)){
                            int id = registeredMetaTypeID(*typeId);
                            if(id==QMetaType::UnknownType){
                                QByteArray qtType = getQtName(*typeId);
                                EntryTypes entryType = getEntryType(*typeId);
                                switch(entryType){
                                case EntryTypes::ObjectTypeInfo:
                                case EntryTypes::QObjectTypeInfo:
                                case EntryTypes::InterfaceTypeInfo:
                                {
                                    qtType += "*";
                                }
                                break;
                                default: break;
                                }
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
                                id = QMetaType::type(qtType);
#else
                                id = QMetaType::fromName(qtType).id();
#endif
                            }
                            if(id!=QMetaType::UnknownType){
                                if(hasReferenceCounts(env, link->getJavaObjectLocalRef(env))){
                                    return QVariant::fromValue(JCollectionWrapper(env, java_object));
                                }else{
                                    return QVariant(META_TYPE(id), link->pointer());
                                }
                            }
                        }else{
                            const auto types = registeredCustomMetaTypesForJavaClass(javaClassName.toUtf8());
                            if(!types.isEmpty()){
                                if(hasReferenceCounts(env, link->getJavaObjectLocalRef(env))){
                                    return QVariant::fromValue(JCollectionWrapper(env, java_object));
                                }else{
                                    return QVariant(types[0], link->pointer());
                                }
                            }
                        }
                    }
                    QMetaType containerMetaType(containerAccess->registerContainer(qtName.toLatin1()));
                    if(hasReferenceCounts(env, link->getJavaObjectLocalRef(env))){
                        registerContainerConverter(AssociativeContainerType::QMultiMap, containerMetaType, containerAccess->keyMetaType(), containerAccess->valueMetaType());
                        return QVariant::fromValue(JMapWrapper(env, java_object));
                    }else{
                        return QVariant(FROM_META_TYPE(containerMetaType), link->pointer());
                    }
                }
            }else{
                Java::QtJambi::QNoNativeResourcesException::throwNew(env, QStringLiteral("Incomplete object of type: %1").arg(QtJambiAPI::getClassName(env, object_class).replace("$", ".")) QTJAMBI_STACKTRACEINFO );
            }
        }else if (Java::QtCore::QMultiHash::isInstanceOf(env, java_object)) {
            if (QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForJavaObject(env, java_object)){
                if(AbstractMultiHashAccess* containerAccess = dynamic_cast<AbstractMultiHashAccess*>(link->containerAccess())){
                    QString qtName;
                    if (Java::QtCore::QMultiHash::isSameClass(env, object_class)) {
                        qtName = QLatin1String("QMultiHash<%1,%2>");
                        qtName = qtName.arg(QLatin1String(containerAccess->keyMetaType().name()))
                                       .arg(QLatin1String(containerAccess->valueMetaType().name()));
                    }else{
                        QString javaClassName = QtJambiAPI::getClassName(env, object_class).replace('.', '/');
                        if(const std::type_info* typeId = getTypeByJavaName(javaClassName)){
                            int id = registeredMetaTypeID(*typeId);
                            if(id==QMetaType::UnknownType){
                                QByteArray qtType = getQtName(*typeId);
                                EntryTypes entryType = getEntryType(*typeId);
                                switch(entryType){
                                case EntryTypes::ObjectTypeInfo:
                                case EntryTypes::QObjectTypeInfo:
                                case EntryTypes::InterfaceTypeInfo:
                                {
                                    qtType += "*";
                                }
                                break;
                                default: break;
                                }
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
                                id = QMetaType::type(qtType);
#else
                                id = QMetaType::fromName(qtType).id();
#endif
                            }
                            if(id!=QMetaType::UnknownType){
                                if(hasReferenceCounts(env, link->getJavaObjectLocalRef(env))){
                                    return QVariant::fromValue(JCollectionWrapper(env, java_object));
                                }else{
                                    return QVariant(META_TYPE(id), link->pointer());
                                }
                            }
                        }else{
                            const auto types = registeredCustomMetaTypesForJavaClass(javaClassName.toUtf8());
                            if(!types.isEmpty()){
                                if(hasReferenceCounts(env, link->getJavaObjectLocalRef(env))){
                                    return QVariant::fromValue(JCollectionWrapper(env, java_object));
                                }else{
                                    return QVariant(types[0], link->pointer());
                                }
                            }
                        }
                        }
                    QMetaType containerMetaType(containerAccess->registerContainer(qtName.toLatin1()));
                    if(hasReferenceCounts(env, link->getJavaObjectLocalRef(env))){
                        registerContainerConverter(AssociativeContainerType::QMultiHash, containerMetaType, containerAccess->keyMetaType(), containerAccess->valueMetaType());
                        return QVariant::fromValue(JMapWrapper(env, java_object));
                    }else{
                        return QVariant(FROM_META_TYPE(containerMetaType), link->pointer());
                    }
                }
            }else{
                Java::QtJambi::QNoNativeResourcesException::throwNew(env, QStringLiteral("Incomplete object of type: %1").arg(QtJambiAPI::getClassName(env, object_class).replace("$", ".")) QTJAMBI_STACKTRACEINFO );
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
                    iter = QtJambiAPI::iteratorOfJavaIterable(env, java_object);
                    while(QtJambiAPI::hasJavaIteratorNext(env, iter)){
                        jobject entry = QtJambiAPI::nextOfJavaIterator(env, iter);
                        jobject keyObject = QtJambiAPI::keyOfJavaMapEntry(env, entry);
                        jobject valueObject = QtJambiAPI::valueOfJavaMapEntry(env, entry);
                        map[qtjambi_cast<QString>(env, keyObject)] = QtJambiAPI::convertJavaObjectToQVariant(env, valueObject);
                    }
                    return QVariant::fromValue(map);
                }else{
                    QVariantHash map;
                    iter = QtJambiAPI::iteratorOfJavaIterable(env, java_object);
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
                    return internal_convertJavaObjectToQVariant(env, Java::QtCore::QMap::newInstance2(env, qtjambi_cast<jobject>(env, QMetaType(keyMetaTypeId)), qtjambi_cast<jobject>(env, QMetaType(valueMetaTypeId)), java_object), convert);
                }else{
                    return internal_convertJavaObjectToQVariant(env, Java::QtCore::QHash::newInstance2(env, qtjambi_cast<jobject>(env, QMetaType(keyMetaTypeId)), qtjambi_cast<jobject>(env, QMetaType(valueMetaTypeId)), java_object), convert);
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
    if(Java::Runtime::Class::isSynthetic(env, object_class)){
        object_class = JavaAPI::resolveClass(env, qPrintable(fullJavaName));
    }else{
        object_class = getGlobalClassRef(env, object_class, fullJavaName.toLatin1());
    }

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
    if(!type.isValid()){
        if(qtName.endsWith("*")){
            const std::type_info* typeId = getTypeByJavaName(fullJavaName);
            type = qtjambiRegisterPointerMetaType(env, object_class, qtName.toUtf8(),
                                           typeId ? registeredOriginalMetaObject(*typeId) : nullptr);
        }else if(qtName.isEmpty()){
            //const SuperTypeInfos infos = SuperTypeInfos::fromClass(env, object_class);
            //if(!infos.isEmpty()){
                type = QMetaType(CoreAPI::registerMetaType(env, object_class, nullptr));
                qtName = type.name();
            //}
        }
    }
    if(type.isValid() && isJObjectWrappedMetaType(type)){
        JObjectWrapper wrapper(env, java_object);
        return VariantUtility::createVariant(type, &wrapper);
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
                switch(link->ownership()){
                case QtJambiLink::Ownership::Java:
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
                    if(convert){
                        QMetaType ntype = getNativeWrapperType(type);
                        if(ntype.isValid()){
                            registerConverterVariant(env, type, qtName, fullJavaName, object_class, ntype);
                            if(type.flags().testFlag(QMetaType::PointerToQObject)){
                                JQObjectWrapper wrapper(env, std::move(link));
                                return VariantUtility::createVariant(ntype, &wrapper);
                            }else{
                                JObjectWrapper wrapper(env, java_object);
                                return VariantUtility::createVariant(ntype, &wrapper);
                            }
                        }
                    }
#endif
                    break;
                default:
                    findConverter = true;
                    break;
                }
            }else if(Java::QtJambi::QtObjectInterface::isInstanceOf(env, java_object)){
                Java::QtJambi::QNoNativeResourcesException::throwNew(env, QStringLiteral("Incomplete object of type: %1").arg(QtJambiAPI::getObjectClassName(env, java_object).replace("$", ".")) QTJAMBI_STACKTRACEINFO );
            }
        }
        if(findConverter){
            if(QtJambiUtils::ExternalToInternalConverter converter = QtJambiTypeManager::getExternalToInternalConverter(env, object_class, qtName, type)){
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
                    return VariantUtility::createVariant(type, ptr);
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
    QtJambiTypeEntryPtr typeEntry;
    const std::type_info* typeIdPtr = &typeId;
    if(typeIdPtr)
        typeEntry = QtJambiTypeEntry::getTypeEntry(env, typeId);
    if(typeEntry){
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
    if(QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForJavaInterface(env, java_object))
        return link->pointer();
    else if(Java::QtJambi::QtObjectInterface::isInstanceOf(env, java_object))
        Java::QtJambi::QNoNativeResourcesException::throwNew(env, QStringLiteral("Incomplete object of type: %1").arg(QtJambiAPI::getObjectClassName(env, java_object).replace("$", ".")) QTJAMBI_STACKTRACEINFO );
    return nullptr;
}

QObject *QtJambiAPI::convertJavaObjectToQObject(JNIEnv *env, jobject java_object)
{
    if(QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForJavaInterface(env, java_object))
        return link->isQObject() ? link->qobject() : nullptr;
    else if(Java::QtJambi::QtObjectInterface::isInstanceOf(env, java_object))
        Java::QtJambi::QNoNativeResourcesException::throwNew(env, QStringLiteral("Incomplete object of type: %1").arg(QtJambiAPI::getObjectClassName(env, java_object).replace("$", ".")) QTJAMBI_STACKTRACEINFO );
    return nullptr;
}

namespace QtJambiPrivate{

template<template<typename> class SmartPointer>
bool convertToNativeSmartPointer(const QSharedPointer<QtJambiLink>& link, SmartPointer<QObject>& output){
    if(link->isSmartPointer()){
        switch(link->smartPointerType()){
        case SmartPointerUtility<SmartPointer>::SharedPointerFlag:
            if(link->isQObject()){
                auto sp = SmartPointerUtility<SmartPointer>::template SharedPointer<QObject>(static_cast<typename SmartPointerUtility<SmartPointer>::SharedQObjectLink*>(link.data())->getSmartPointer());
                output = sp;
            }else{
                auto sp = SmartPointerUtility<SmartPointer>::template SharedPointer<char>(static_cast<typename SmartPointerUtility<SmartPointer>::SharedLink*>(link.data())->getSmartPointer());
                output = SmartPointerUtility<SmartPointer>::template SharedPointer<QObject>(sp, reinterpret_cast<QObject*>(link->pointer()));
            }
            return true;
        case SmartPointerUtility<SmartPointer>::WeakPointerFlag:
            if(link->isQObject()){
                auto sp = SmartPointerUtility<SmartPointer>::template SharedPointer<QObject>(static_cast<typename SmartPointerUtility<SmartPointer>::WeakQObjectLink*>(link.data())->getSmartPointer());
                output = sp;
            }else{
                auto sp = SmartPointerUtility<SmartPointer>::template SharedPointer<char>(static_cast<typename SmartPointerUtility<SmartPointer>::WeakLink*>(link.data())->getSmartPointer());
                output = SmartPointerUtility<SmartPointer>::template SharedPointer<QObject>(sp, reinterpret_cast<QObject*>(link->pointer()));
            }
            return true;
        default:
            break;
        }
    }
    return false;
}

template<template<typename> class SmartPointer>
bool convertToNativeSmartPointer(const QSharedPointer<QtJambiLink>& link, SmartPointer<char>& output, const std::type_info* typeId = nullptr){
    if(link->isSmartPointer()){
        if(typeId){
            void *ptr{nullptr};
            switch(link->smartPointerType()){
            case SmartPointerUtility<SmartPointer>::SharedPointerFlag:
                if(link->isQObject()){
                    auto sp = SmartPointerUtility<SmartPointer>::template SharedPointer<QObject>(static_cast<typename SmartPointerUtility<SmartPointer>::SharedQObjectLink*>(link.data())->getSmartPointer());
                    ptr = link->typedPointer(*typeId);
                    if(ptr){
                        output = SmartPointerUtility<SmartPointer>::template SharedPointer<char>(sp, reinterpret_cast<char*>(ptr));
                    }else{
                        output = SmartPointerUtility<SmartPointer>::template SharedPointer<char>(sp, reinterpret_cast<char*>(link->pointer()));
                    }
                }else{
                    auto sp = SmartPointerUtility<SmartPointer>::template SharedPointer<char>(static_cast<typename SmartPointerUtility<SmartPointer>::SharedLink*>(link.data())->getSmartPointer());
                    ptr = link->typedPointer(*typeId);
                    if(ptr){
                        output = SmartPointerUtility<SmartPointer>::template SharedPointer<char>(sp, reinterpret_cast<char*>(ptr));
                    }else{
                        output = sp;
                    }
                }
                return true;
            case SmartPointerUtility<SmartPointer>::WeakPointerFlag:
                if(link->isQObject()){
                    auto sp = SmartPointerUtility<SmartPointer>::template SharedPointer<QObject>(static_cast<typename SmartPointerUtility<SmartPointer>::WeakQObjectLink*>(link.data())->getSmartPointer());
                    ptr = link->typedPointer(*typeId);
                    if(ptr){
                        output = SmartPointerUtility<SmartPointer>::template SharedPointer<char>(sp, reinterpret_cast<char*>(ptr));
                    }else{
                        output = SmartPointerUtility<SmartPointer>::template SharedPointer<char>(sp, reinterpret_cast<char*>(link->pointer()));
                    }
                }else{
                    auto sp = SmartPointerUtility<SmartPointer>::template SharedPointer<char>(static_cast<typename SmartPointerUtility<SmartPointer>::WeakLink*>(link.data())->getSmartPointer());
                    ptr = link->typedPointer(*typeId);
                    if(ptr){
                        output = SmartPointerUtility<SmartPointer>::template SharedPointer<char>(sp, reinterpret_cast<char*>(ptr));
                    }else{
                        output = sp;
                    }
                }
                return true;
            default:
                break;
            }
        }else{
            switch(link->smartPointerType()){
            case SmartPointerUtility<SmartPointer>::SharedPointerFlag:
                if(link->isQObject()){
                    auto sp = SmartPointerUtility<SmartPointer>::template SharedPointer<QObject>(static_cast<typename SmartPointerUtility<SmartPointer>::SharedQObjectLink*>(link.data())->getSmartPointer());
                    output = SmartPointerUtility<SmartPointer>::template SharedPointer<char>(sp, reinterpret_cast<char*>(link->pointer()));
                }else{
                    auto sp = SmartPointerUtility<SmartPointer>::template SharedPointer<char>(static_cast<typename SmartPointerUtility<SmartPointer>::SharedLink*>(link.data())->getSmartPointer());
                    output = sp;
                }
                return true;
            case SmartPointerUtility<SmartPointer>::WeakPointerFlag:
                if(link->isQObject()){
                    auto sp = SmartPointerUtility<SmartPointer>::template SharedPointer<QObject>(static_cast<typename SmartPointerUtility<SmartPointer>::WeakQObjectLink*>(link.data())->getSmartPointer());
                    output = SmartPointerUtility<SmartPointer>::template SharedPointer<char>(sp, reinterpret_cast<char*>(link->pointer()));
                }else{
                    auto sp = SmartPointerUtility<SmartPointer>::template SharedPointer<char>(static_cast<typename SmartPointerUtility<SmartPointer>::WeakLink*>(link.data())->getSmartPointer());
                    output = sp;
                }
                return true;
            default:
                break;
            }
        }
    }
    return false;
}

template<template<typename> class SmartPointer>
SmartPointer<QObject> convertToNativeSmartPointer(JNIEnv *env, jobject java_object){
    SmartPointer<QObject> result;
    if(java_object){
        if(QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForJavaObject(env, java_object)){
            Q_ASSERT(link->isQObject());
            if(convertToNativeSmartPointer(link, result)){
                // ok!!
            }else if(!link->isSmartPointer()){
                void* ptr = link->pointer();
                bool createdByJava = link->createdByJava();
                bool is_shell = link->isShell();
                bool isQThread = link->isQThread();
                QtJambiLink::Ownership ownership = link->ownership();
                const InterfaceOffsetInfo* interfaceOffsets = link->interfaceOffsetInfo();
                QtJambiShellImpl* shell = nullptr;
                if(is_shell){
                    const SuperTypeInfos infos = SuperTypeInfos::fromClass(env, env->GetObjectClass(java_object));
                    Q_ASSERT(infos.size());
                    shell = *reinterpret_cast<QtJambiShellImpl**>( quintptr(ptr) + infos.at(0).size() );
                }

                link->reset(env);
                link->invalidate(env);
                link.clear();
                if(interfaceOffsets && !interfaceOffsets->offsets.isEmpty()){
                    link = QtJambiLink::createLinkForNewSmartPointerToQObject(env, java_object, createdByJava, ownership, is_shell, reinterpret_cast<QObject*>(ptr), isQThread, result, *interfaceOffsets);
                }else{
                    link = QtJambiLink::createLinkForNewSmartPointerToQObject(env, java_object, createdByJava, ownership, is_shell, reinterpret_cast<QObject*>(ptr), isQThread, result);
                }
                Q_ASSERT(link && link->isSmartPointer());
                if(shell){
                    shell->overrideLink(link);
                }
            }
        }
        else if(Java::QtJambi::QtObjectInterface::isInstanceOf(env, java_object))
            Java::QtJambi::QNoNativeResourcesException::throwNew(env, QStringLiteral("Incomplete object of type: %1").arg(QtJambiAPI::getObjectClassName(env, java_object).replace("$", ".")) QTJAMBI_STACKTRACEINFO );
    }
    return result;
}

template<template<typename> class SmartPointer>
SmartPointer<char> convertToNativeSmartPointer(JNIEnv *env, const std::type_info* typeId, jobject java_object){
    SmartPointer<char> result;
    if(java_object){
        if(typeId && typeid_equals(*typeId, typeid(QVariant)) && !Java::QtCore::QVariant::isInstanceOf(env, java_object)){
            auto sp = SmartPointerUtility<SmartPointer>::template SharedPointer<QVariant>(new QVariant(qtjambi_cast<QVariant>(env, java_object)));
            result = SmartPointer<char>(SmartPointerUtility<SmartPointer>::template SharedPointer<char>(sp, reinterpret_cast<char*>(sp.get())));
        }else if(QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForJavaInterface(env, java_object)){
            if(convertToNativeSmartPointer(link, result, typeId)){
                // ok!!
            }else if(!link->isSmartPointer()){
                void* ptr = link->pointer();
                if(typeId){
                    void* tptr = link->typedPointer(*typeId);
                    if(tptr)
                        ptr = tptr;
                }
                bool createdByJava = link->createdByJava();
                bool is_shell = link->isShell();
                QtJambiLink::Ownership ownership = link->ownership();
                QtJambiShellImpl* shell = nullptr;
                if(is_shell){
                    const SuperTypeInfos infos = SuperTypeInfos::fromClass(env, env->GetObjectClass(java_object));
                    Q_ASSERT(infos.size());
                    shell = *reinterpret_cast<QtJambiShellImpl**>( quintptr(ptr) + infos.at(0).size() );
                }
#if defined(QTJAMBI_DEBUG_TOOLS) || defined(QTJAMBI_LINK_NAME) || !defined(QT_NO_DEBUG)
                const char* className = link->qtTypeName();
#endif
                PtrDeleterFunction deleterFunction = link->deleterFunction();
                const QMetaType* elementMetaType = link->metaType();
                AbstractContainerAccess* containerAccess = link->containerAccess();
                if(containerAccess || deleterFunction || elementMetaType){
                    const InterfaceOffsetInfo* interfaceOffsets = link->interfaceOffsetInfo();
                    PtrOwnerFunction registeredOwnerFunction = link->ownerFunction();
                    if(containerAccess)
                        containerAccess = containerAccess->clone();
                    link->reset(env);
                    link->invalidate(env);
                    link.clear();
                    if(containerAccess){
                        if(interfaceOffsets && !interfaceOffsets->offsets.isEmpty()){
                            link = QtJambiLink::createLinkForNewSmartPointerToObject(env,
                                                                                     java_object,
                                                                                     LINK_NAME_ARG(className)
                                                                                     ownership,
                                                                                     ptr,
                                                                                     result,
                                                                                     containerAccess,
                                                                                     *interfaceOffsets);
                        }else{
                            link = QtJambiLink::createLinkForNewSmartPointerToObject(env,
                                                                                     java_object,
                                                                                     LINK_NAME_ARG(className)
                                                                                     ownership,
                                                                                     ptr,
                                                                                     result,
                                                                                     containerAccess);
                        }
                    }else if(deleterFunction){
                        if(interfaceOffsets && !interfaceOffsets->offsets.isEmpty()){
                            link = QtJambiLink::createLinkForNewSmartPointerToObject(env,
                                                                                  java_object,
                                                                                  LINK_NAME_ARG(className)
                                                                                  createdByJava,
                                                                                  ownership,
                                                                                  is_shell,
                                                                                  registeredOwnerFunction,
                                                                                  ptr,
                                                                                  result,
                                                                                  deleterFunction,
                                                                                  *interfaceOffsets);
                        }else{
                            link = QtJambiLink::createLinkForNewSmartPointerToObject(env,
                                                                                  java_object,
                                                                                  LINK_NAME_ARG(className)
                                                                                  createdByJava,
                                                                                  ownership,
                                                                                  is_shell,
                                                                                  registeredOwnerFunction,
                                                                                  ptr,
                                                                                  result,
                                                                                  deleterFunction);
                        }
                    }else{
                        if(interfaceOffsets && !interfaceOffsets->offsets.isEmpty()){
                            link = QtJambiLink::createLinkForNewSmartPointerToObject(env,
                                                                                  java_object,
                                                                                  createdByJava,
                                                                                  ownership,
                                                                                  is_shell,
                                                                                  registeredOwnerFunction,
                                                                                  ptr,
                                                                                  result,
                                                                                  *elementMetaType,
                                                                                  *interfaceOffsets);
                        }else{
                            link = QtJambiLink::createLinkForNewSmartPointerToObject(env,
                                                                                  java_object,
                                                                                  createdByJava,
                                                                                  ownership,
                                                                                  is_shell,
                                                                                  registeredOwnerFunction,
                                                                                  ptr,
                                                                                  result,
                                                                                  *elementMetaType);
                        }
                    }
                    Q_ASSERT(link && link->isSmartPointer());
                    if(shell){
                        shell->overrideLink(link);
                    }
                }
            }
        }else if(Java::QtJambi::QtObjectInterface::isInstanceOf(env, java_object)){
            Java::QtJambi::QNoNativeResourcesException::throwNew(env, QStringLiteral("Incomplete object of type: %1").arg(QtJambiAPI::getObjectClassName(env, java_object).replace("$", ".")) QTJAMBI_STACKTRACEINFO );
        }else if((!typeId || typeid_equals(*typeId, typeid(QString))) && Java::Runtime::String::isInstanceOf(env, java_object)){
            auto sp = SmartPointerUtility<SmartPointer>::template SharedPointer<QString>(new QString(qtjambi_cast<QString>(env, static_cast<jstring>(java_object))));
            result = SmartPointer<char>(SmartPointerUtility<SmartPointer>::template SharedPointer<char>(sp, reinterpret_cast<char*>(sp.get())));
        }else if(!typeId && Java::Runtime::Integer::isInstanceOf(env, java_object)){
            auto sp = SmartPointerUtility<SmartPointer>::template SharedPointer<qint32>(new qint32(Java::Runtime::Number::intValue(env, java_object)));
            result = SmartPointer<char>(SmartPointerUtility<SmartPointer>::template SharedPointer<char>(sp, reinterpret_cast<char*>(sp.get())));
        }else if(!typeId && Java::Runtime::Byte::isInstanceOf(env, java_object)){
            auto sp = SmartPointerUtility<SmartPointer>::template SharedPointer<qint8>(new qint8(Java::Runtime::Number::byteValue(env, java_object)));
            result = SmartPointer<char>(SmartPointerUtility<SmartPointer>::template SharedPointer<char>(sp, reinterpret_cast<char*>(sp.get())));
        }else if(!typeId && Java::Runtime::Short::isInstanceOf(env, java_object)){
            auto sp = SmartPointerUtility<SmartPointer>::template SharedPointer<qint16>(new qint16(Java::Runtime::Number::shortValue(env, java_object)));
            result = SmartPointer<char>(SmartPointerUtility<SmartPointer>::template SharedPointer<char>(sp, reinterpret_cast<char*>(sp.get())));
        }else if(!typeId && Java::Runtime::Long::isInstanceOf(env, java_object)){
            auto sp = SmartPointerUtility<SmartPointer>::template SharedPointer<qint64>(new qint64(Java::Runtime::Number::longValue(env, java_object)));
            result = SmartPointer<char>(SmartPointerUtility<SmartPointer>::template SharedPointer<char>(sp, reinterpret_cast<char*>(sp.get())));
        }else if(!typeId && Java::Runtime::Float::isInstanceOf(env, java_object)){
            auto sp = SmartPointerUtility<SmartPointer>::template SharedPointer<float>(new float(Java::Runtime::Number::floatValue(env, java_object)));
            result = SmartPointer<char>(SmartPointerUtility<SmartPointer>::template SharedPointer<char>(sp, reinterpret_cast<char*>(sp.get())));
        }else if(!typeId && Java::Runtime::Double::isInstanceOf(env, java_object)){
            auto sp = SmartPointerUtility<SmartPointer>::template SharedPointer<double>(new double(Java::Runtime::Number::doubleValue(env, java_object)));
            result = SmartPointer<char>(SmartPointerUtility<SmartPointer>::template SharedPointer<char>(sp, reinterpret_cast<char*>(sp.get())));
        }else if(!typeId && Java::Runtime::Boolean::isInstanceOf(env, java_object)){
            auto sp = SmartPointerUtility<SmartPointer>::template SharedPointer<bool>(new bool(Java::Runtime::Boolean::booleanValue(env, java_object)));
            result = SmartPointer<char>(SmartPointerUtility<SmartPointer>::template SharedPointer<char>(sp, reinterpret_cast<char*>(sp.get())));
        }else if(!typeId && Java::Runtime::Character::isInstanceOf(env, java_object)){
            auto sp = SmartPointerUtility<SmartPointer>::template SharedPointer<jchar>(new jchar(Java::Runtime::Character::charValue(env, java_object)));
            result = SmartPointer<char>(SmartPointerUtility<SmartPointer>::template SharedPointer<char>(sp, reinterpret_cast<char*>(sp.get())));
        }else if(typeId && (typeid_equals(*typeId, typeid(qint32))
                    || typeid_equals(*typeId, typeid(quint32))
                    || typeid_equals(*typeId, typeid(signed))
                    || typeid_equals(*typeId, typeid(signed int))
                    || typeid_equals(*typeId, typeid(unsigned))
                    || typeid_equals(*typeId, typeid(unsigned int))
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
                    || typeid_equals(*typeId, typeid(char32_t))
#endif
                    || typeid_equals(*typeId, typeid(jint))) && Java::Runtime::Number::isInstanceOf(env, java_object)){
            auto sp = SmartPointerUtility<SmartPointer>::template SharedPointer<qint32>(new qint32(Java::Runtime::Number::intValue(env, java_object)));
            result = SmartPointer<char>(SmartPointerUtility<SmartPointer>::template SharedPointer<char>(sp, reinterpret_cast<char*>(sp.get())));
        }else if(typeId && (typeid_equals(*typeId, typeid(qint8))
                    || typeid_equals(*typeId, typeid(quint8))
                    || typeid_equals(*typeId, typeid(char))
                    || typeid_equals(*typeId, typeid(uchar))
                    || typeid_equals(*typeId, typeid(signed char))
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
                    || typeid_equals(*typeId, typeid(std::byte))
#endif
                    || typeid_equals(*typeId, typeid(jbyte))) && Java::Runtime::Number::isInstanceOf(env, java_object)){
            result = SmartPointerUtility<SmartPointer>::template SharedPointer<char>(new char(Java::Runtime::Number::byteValue(env, java_object)));
        }else if(typeId && (typeid_equals(*typeId, typeid(qint16))
                    || typeid_equals(*typeId, typeid(quint16))
                    || typeid_equals(*typeId, typeid(signed short))
                    || typeid_equals(*typeId, typeid(unsigned short))
                    || typeid_equals(*typeId, typeid(jshort))) && Java::Runtime::Number::isInstanceOf(env, java_object)){
            auto sp = SmartPointerUtility<SmartPointer>::template SharedPointer<qint16>(new qint16(Java::Runtime::Number::shortValue(env, java_object)));
            result = SmartPointer<char>(SmartPointerUtility<SmartPointer>::template SharedPointer<char>(sp, reinterpret_cast<char*>(sp.get())));
        }else if(typeId && (typeid_equals(*typeId, typeid(qint64))
                    || typeid_equals(*typeId, typeid(quint64))
                    || typeid_equals(*typeId, typeid(signed long long))
                    || typeid_equals(*typeId, typeid(unsigned long long))
                    || typeid_equals(*typeId, typeid(jlong))) && Java::Runtime::Number::isInstanceOf(env, java_object)){
            auto sp = SmartPointerUtility<SmartPointer>::template SharedPointer<qint64>(new qint64(Java::Runtime::Number::longValue(env, java_object)));
            result = SmartPointer<char>(SmartPointerUtility<SmartPointer>::template SharedPointer<char>(sp, reinterpret_cast<char*>(sp.get())));
        }else if(typeId && typeid_equals(*typeId, typeid(float)) && Java::Runtime::Number::isInstanceOf(env, java_object)){
            auto sp = SmartPointerUtility<SmartPointer>::template SharedPointer<float>(new float(Java::Runtime::Number::floatValue(env, java_object)));
            result = SmartPointer<char>(SmartPointerUtility<SmartPointer>::template SharedPointer<char>(sp, reinterpret_cast<char*>(sp.get())));
        }else if(typeId && typeid_equals(*typeId, typeid(double)) && Java::Runtime::Number::isInstanceOf(env, java_object)){
            auto sp = SmartPointerUtility<SmartPointer>::template SharedPointer<double>(new double(Java::Runtime::Number::doubleValue(env, java_object)));
            result = SmartPointer<char>(SmartPointerUtility<SmartPointer>::template SharedPointer<char>(sp, reinterpret_cast<char*>(sp.get())));
        }else if(typeId && (typeid_equals(*typeId, typeid(QChar))
                    || typeid_equals(*typeId, typeid(jchar))
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
                    || typeid_equals(*typeId, typeid(char16_t))
#endif
                    || typeid_equals(*typeId, typeid(wchar_t))) && (Java::Runtime::Number::isInstanceOf(env, java_object) || Java::Runtime::Character::isInstanceOf(env, java_object))){
            auto sp = SmartPointerUtility<SmartPointer>::template SharedPointer<jchar>(new jchar(Java::Runtime::Character::isInstanceOf(env, java_object)
                                                                                                     ? Java::Runtime::Character::charValue(env, java_object)
                                                                                                     : Java::Runtime::Number::shortValue(env, java_object)));
            result = SmartPointer<char>(SmartPointerUtility<SmartPointer>::template SharedPointer<char>(sp, reinterpret_cast<char*>(sp.get())));
        }else if(typeId && (typeid_equals(*typeId, typeid(bool))
                    || typeid_equals(*typeId, typeid(jboolean))) && (Java::Runtime::Number::isInstanceOf(env, java_object)
                                                                        || Java::Runtime::Character::isInstanceOf(env, java_object)
                                                                        || Java::Runtime::Boolean::isInstanceOf(env, java_object))){
            auto sp = SmartPointerUtility<SmartPointer>::template SharedPointer<bool>(new bool(Java::Runtime::Boolean::isInstanceOf(env, java_object)
                                                                                                 ? Java::Runtime::Boolean::booleanValue(env, java_object)
                                                                                                   : (Java::Runtime::Character::isInstanceOf(env, java_object)
                                                                                                          ? Java::Runtime::Character::charValue(env, java_object)!=0
                                                                                                          : Java::Runtime::Number::longValue(env, java_object)!=0)));
            result = SmartPointer<char>(SmartPointerUtility<SmartPointer>::template SharedPointer<char>(sp, reinterpret_cast<char*>(sp.get())));
        }
    }
    return result;
}

} // namespace QtJambiPrivate

QSharedPointer<QObject> QtJambiAPI::convertJavaObjectToQSharedPointer(JNIEnv *env, jobject java_object)
{
    return QtJambiPrivate::convertToNativeSmartPointer<QSharedPointer>(env, java_object);
}

QWeakPointer<QObject> QtJambiAPI::convertJavaObjectToQWeakPointer(JNIEnv *env, jobject java_object)
{
    return QtJambiPrivate::convertToNativeSmartPointer<QWeakPointer>(env, java_object);
}

std::shared_ptr<QObject> QtJambiAPI::convertJavaObjectToSharedPtr(JNIEnv *env, jobject java_object)
{
    return QtJambiPrivate::convertToNativeSmartPointer<std::shared_ptr>(env, java_object);
}

std::weak_ptr<QObject> QtJambiAPI::convertJavaObjectToWeakPtr(JNIEnv *env, jobject java_object)
{
    return QtJambiPrivate::convertToNativeSmartPointer<std::weak_ptr>(env, java_object);
}

QSharedPointer<char> QtJambiAPI::convertJavaObjectToQSharedPointer(JNIEnv *env,
                                                                          const std::type_info* typeId,
                                                                          jobject java_object)
{
    return QtJambiPrivate::convertToNativeSmartPointer<QSharedPointer>(env, typeId, java_object);
}

QWeakPointer<char> QtJambiAPI::convertJavaObjectToQWeakPointer(JNIEnv *env,
                                                               const std::type_info* typeId,
                                                               jobject java_object)
{
    return QtJambiPrivate::convertToNativeSmartPointer<QWeakPointer>(env, typeId, java_object);
}

std::shared_ptr<char> QtJambiAPI::convertJavaObjectToSharedPtr(JNIEnv *env,
                                                                       const std::type_info* typeId,
                                                                       jobject java_object)
{
    return QtJambiPrivate::convertToNativeSmartPointer<std::shared_ptr>(env, typeId, java_object);
}

std::weak_ptr<char> QtJambiAPI::convertJavaObjectToWeakPtr(JNIEnv *env,
                                                               const std::type_info* typeId,
                                                               jobject java_object)
{
    return QtJambiPrivate::convertToNativeSmartPointer<std::weak_ptr>(env, typeId, java_object);
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
                        *interfaceOffsets);
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
                        *interfaceOffsets);
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
    if(QtJambiTypeEntry::convertModelIndexNativeToJava(env, &index, NativeToJavaConversionMode::MakeCopyOfValues, result)){
        return result;
    }else{
        return nullptr;
    }
}

jobject QtJambiAPI::convertModelIndexToEphemeralJavaObject(JNIEnv *env, QtJambiScope& scope, const QModelIndex* index){
    jobject result{nullptr};
    if(QtJambiTypeEntry::convertModelIndexNativeToJava(env, index, NativeToJavaConversionMode::MakeCopyOfValues, result, &scope)){
        return result;
    }else{
        return nullptr;
    }
}

bool QtJambiAPI::convertJavaToModelIndex(JNIEnv *env, jobject java_object, QModelIndex*& output, QtJambiScope* scope){
    if(!java_object)
        return true;
#if defined(QTJAMBI_LIGHTWEIGHT_MODELINDEX)
    if(!scope)
        JavaException::raiseError(env, "Cannot cast to QModelIndex reference without scope." QTJAMBI_STACKTRACEINFO );
#endif
    return QtJambiTypeEntry::convertModelIndexJavaToNative(env, java_object, &output, scope);
}

bool QtJambiAPI::convertJavaToModelIndex(JNIEnv *env, jobject java_object, QModelIndex& output){
    QModelIndex* indexPtr = nullptr;
    bool result = QtJambiTypeEntry::convertModelIndexJavaToNative(env, java_object, &indexPtr, nullptr);
    if(result && indexPtr)
        output = *indexPtr;
    return result;
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

template<template<typename> class SmartPointer>
jobject internal_convertSmartPointerToJavaObject_impl(JNIEnv *env,
                            const std::type_info& typeId, const SmartPointer<char>& smartPointer, bool* ok)
{
    if(ok) *ok = true;
    QtJambiTypeEntryPtr typeEntry;
    const std::type_info* typeIdPtr = &typeId;
    if(typeIdPtr)
        typeEntry = QtJambiTypeEntry::getTypeEntry(env, typeId);
    if(typeEntry){
        jvalue result;
        qintptr offset = 0;
        typeEntry = typeEntry->getFittingTypeEntry(env, smartPointer.get(), offset);
        if(typeEntry->convertSmartPointerToJava(env, smartPointer, offset, result, jValueType::l)){
            return result.l;
        }else{
            return nullptr;
        }
    }
    if(ok) *ok = false;
    return nullptr;
}

QSharedPointer<char> convertSmartPointer(const QSharedPointer<QObject>& smartPointer){
    char* _ptr = reinterpret_cast<char*>(smartPointer.get());
    return QtSharedPointer::copyAndSetPointer(_ptr, smartPointer);
}

std::shared_ptr<char> convertSmartPointer(const std::shared_ptr<QObject>& smartPointer){
    char* _ptr = reinterpret_cast<char*>(smartPointer.get());
    return std::shared_ptr<char>(smartPointer, _ptr);
}

template<template<typename> class SmartPointer>
jobject internal_convertQObjectSmartPointerToJavaObject_impl(JNIEnv *env, const std::type_info& typeId,
                                                             const SmartPointer<QObject>& smartPointer, bool* ok)
{
    if(ok) *ok = true;
    QtJambiTypeEntryPtr typeEntry;
    const std::type_info* typeIdPtr = &typeId;
    if(!typeEntry && typeIdPtr)
        typeEntry = QtJambiTypeEntry::getTypeEntry(env, typeId);
    if(!typeEntry)
        typeEntry = QtJambiTypeEntry::getTypeEntry(env, typeid(QObject));
    if(typeEntry){
        jvalue result;
        qintptr offset = 0;
        typeEntry = typeEntry->getFittingTypeEntry(env, smartPointer.get(), offset);
        if(typeEntry->convertSmartPointerToJava(env, convertSmartPointer(smartPointer), offset, result, jValueType::l)){
            return result.l;
        }else{
            return nullptr;
        }
    }
    if(ok) *ok = false;
    return nullptr;
}

template<template<typename> class SmartPointer>
jobject internal_convertSmartPointerToJavaObject_notype(JNIEnv *env, const char *className,
                                                        const SmartPointer<char>& smartPointer)
{
    void* qt_object = smartPointer.get();
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
    QSharedPointer<QtJambiLink> link;
    if(interfaceOffsets && !interfaceOffsets->offsets.isEmpty()){
        link = QtJambiLink::createLinkForSmartPointerToObject(
                    env,
                    returned,
                    LINK_NAME_ARG(nullptr)
                    false,
                    false,
                    nullptr,
                    smartPointer,
                    *interfaceOffsets);
    }else{
        link = QtJambiLink::createLinkForSmartPointerToObject(
                    env,
                    returned,
                    LINK_NAME_ARG(nullptr)
                    false,
                    false,
                    nullptr,
                    smartPointer
                    );
    }
    if (!link) {
        returned = nullptr;
    }

    return returned;
}

template<template<typename> class SmartPointer>
jobject internal_convertQObjectSmartPointerToJavaObject_notype(JNIEnv *env, const char *className,
                             const SmartPointer<QObject>& smartPointer)
{
    QObject* qt_object = smartPointer.get();
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
                    link->reset(env);
                    link.clear();
                    locker.relock();
                }
            }
            if(link && !link->isSmartPointer()){
                if(object){
                    bool createdByJava = link->createdByJava();
                    bool is_shell = link->isShell();
                    bool isQThread = link->isQThread();
                    QtJambiShellImpl* shell = nullptr;
                    if(is_shell){
                        const SuperTypeInfos infos = SuperTypeInfos::fromClass(env, env->GetObjectClass(object));
                        Q_ASSERT(infos.size());
                        shell = *reinterpret_cast<QtJambiShellImpl**>( quintptr(qt_object) + infos.at(0).size() );
                    }
                    link->reset(env);
                    link->invalidate(env);
                    link.clear();
                    const InterfaceOffsetInfo* interfaceOffsets = getInterfaceOffsets(env, env->GetObjectClass(object));
                    if(interfaceOffsets && !interfaceOffsets->offsets.isEmpty()){
                        link = QtJambiLink::createLinkForSmartPointerToQObject(env, object, createdByJava, is_shell, smartPointer, isQThread, *interfaceOffsets);
                    }else{
                        link = QtJambiLink::createLinkForSmartPointerToQObject(env, object, createdByJava, is_shell, smartPointer, isQThread);
                    }
                    if(shell){
                        shell->overrideLink(link);
                    }
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
                        link->reset(env);
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
            link = QtJambiLink::createLinkForSmartPointerToQObject(env, object, false, false, smartPointer, Java::QtCore::QThread::isAssignableFrom(env, clazz), *interfaceOffsets);
        }else{
            link = QtJambiLink::createLinkForSmartPointerToQObject(env, object, false, false, smartPointer, Java::QtCore::QThread::isAssignableFrom(env, clazz));
        }
        if (!link) {
            qCWarning(DebugAPI::internalCategory, "Qt Jambi: Couldn't created wrapper for class %s", className);
            return nullptr;
        }
    }
    return object;
}

template<template<typename> class SmartPointer>
jobject internal_convertSmartPointerToJavaInterface(JNIEnv *env, const std::type_info& interfaceType,
                                                    const SmartPointer<char>& smartPointer)
{
    void* qt_object = smartPointer.get();
    if (!qt_object)
        return nullptr;
    bool ok;
    jobject result = internal_convertSmartPointerToJavaObject_impl<SmartPointer>(env, interfaceType, smartPointer, &ok);
    if(ok)
        return result;
    QByteArray className = getJavaName(interfaceType);
    if(!className.isEmpty()){
        return internal_convertSmartPointerToJavaObject_notype<SmartPointer>(env, className, smartPointer);
    }else{
        return nullptr;
    }
}

template<template<typename> class SmartPointer>
jobject internal_convertSmartPointerToJavaObject(JNIEnv *env, const char *className,
                            const SmartPointer<char>& smartPointer)
{
    const std::type_info* typeId = getTypeByJavaName(className);
    if(typeId){
        bool ok;
        jobject result = internal_convertSmartPointerToJavaObject_impl<SmartPointer>(env, *typeId, smartPointer, &ok);
        if(ok)
            return result;
    }
    return internal_convertSmartPointerToJavaObject_notype<SmartPointer>(env, className, smartPointer);
}

jobject internal_convertSmartPointerToJavaObject(JNIEnv *env, const char *className,
                                                 const QSharedPointer<char>& smartPointer){
    return internal_convertSmartPointerToJavaObject<QSharedPointer>(env, className, smartPointer);
}
jobject internal_convertSmartPointerToJavaInterface(JNIEnv *env, const std::type_info& interfaceType,
                                                    const QSharedPointer<char>& smartPointer){
    return internal_convertSmartPointerToJavaInterface<QSharedPointer>(env, interfaceType, smartPointer);
}

jobject internal_convertSmartPointerToJavaObject(JNIEnv *env, const char *className,
                                                 const std::shared_ptr<char>& smartPointer){
    return internal_convertSmartPointerToJavaObject<std::shared_ptr>(env, className, smartPointer);
}
jobject internal_convertSmartPointerToJavaInterface(JNIEnv *env, const std::type_info& interfaceType,
                                                    const std::shared_ptr<char>& smartPointer){
    return internal_convertSmartPointerToJavaInterface<std::shared_ptr>(env, interfaceType, smartPointer);
}


jobject internal_convertQObjectToJavaObject_type(JNIEnv *env, const QObject *qt_object, const std::type_info& typeId, bool* ok, NativeToJavaConversionMode mode)
{
    if(ok) *ok = true;
    if (!qt_object)
        return nullptr;
    QtJambiTypeEntryPtr typeEntry;
    const std::type_info* typeIdPtr = &typeId;
    if(!typeEntry && typeIdPtr)
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
    const std::type_info* _typeId = tryGetTypeInfo(RegistryAPI::Private::PolymorphicTypeInfoSupplier<QObject>::value, qt_object);
    if(!_typeId){
        QString java_type;
        if(className.isNull() && clazz){
            java_type = QtJambiAPI::getClassName(env, clazz).replace(QLatin1Char('$'), QLatin1Char('.'));
        }else{
            java_type = className.replace('/', '.').replace('$', '.');
        }
        if(enabledDanglingPointerCheck()){
            Java::QtJambi::QDanglingPointerException::throwNew(env, QString::asprintf("Cannot convert dangling pointer %p to object of type %s", qt_object, qPrintable(java_type)) QTJAMBI_STACKTRACEINFO );
        }else{
            qCWarning(DebugAPI::internalCategory, "Trying to convert possible dangling pointer %p to object of type %s", qt_object, qPrintable(java_type));
        }
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
                link->reset(env);
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
                link->reset(env);
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

        link = QtJambiLink::createLinkForNativeQObject(env, obj, clazz, qt_object);
        if (!link) {
            qCWarning(DebugAPI::internalCategory, "Qt Jambi: Couldn't created wrapper for class %s", qPrintable(java_name));
            return nullptr;
        }
        if(mode==NativeToJavaConversionMode::CppOwnership)
            link->setCppOwnership(env);
    }
    return obj;
}

template<template<typename> class SmartPointer>
jobject internal_convertQObjectSmartPointerToJavaObject(JNIEnv *env, const std::type_info& typeId,
                             const SmartPointer<QObject>& smartPointer)
{
    bool ok;
    jobject result = internal_convertQObjectSmartPointerToJavaObject_impl<SmartPointer>(env, typeId, smartPointer, &ok);
    if(ok)
        return result;
    QByteArray className = getJavaName(typeId);
    return internal_convertQObjectSmartPointerToJavaObject_notype<SmartPointer>(env, className, smartPointer);
}

template<template<typename> class SmartPointer>
jobject internal_convertQObjectSmartPointerToJavaObject(JNIEnv *env, const char *className,
                             const SmartPointer<QObject>& smartPointer)
{
    const std::type_info* typeId = getTypeByJavaName(className);
    if(typeId){
        bool ok;
        jobject result = internal_convertQObjectSmartPointerToJavaObject_impl<SmartPointer>(env, *typeId, smartPointer, &ok);
        if(ok)
            return result;
    }
    return internal_convertQObjectSmartPointerToJavaObject_notype<SmartPointer>(env, className, smartPointer);
}

jobject QtJambiAPI::convertSmartPointerToJavaObject(JNIEnv *env, const std::type_info& typeId,
                                                    const QSharedPointer<QObject>& smartPointer)
{
    return internal_convertQObjectSmartPointerToJavaObject<QSharedPointer>(env, typeId, smartPointer);
}

jobject QtJambiAPI::convertSmartPointerToJavaObject(JNIEnv *env, const std::type_info& typeId,
                                                    const std::shared_ptr<QObject>& smartPointer)
{
    return internal_convertQObjectSmartPointerToJavaObject<std::shared_ptr>(env, typeId, smartPointer);
}

jobject internal_convertQObjectSmartPointerToJavaObject(JNIEnv *env, const char *className,
                                                    const QSharedPointer<QObject>& smartPointer)
{
    return internal_convertQObjectSmartPointerToJavaObject<QSharedPointer>(env, className, smartPointer);
}

jobject internal_convertQObjectSmartPointerToJavaObject(JNIEnv *env, const char *className,
                                                    const std::shared_ptr<QObject>& smartPointer)
{
    return internal_convertQObjectSmartPointerToJavaObject<std::shared_ptr>(env, className, smartPointer);
}

jobject QtJambiAPI::convertSmartPointerToJavaObject(JNIEnv *env, const std::type_info& typeId,
                                                    const QSharedPointer<char>& smartPointer)
{
    bool ok;
    jobject result = internal_convertSmartPointerToJavaObject_impl<QSharedPointer>(env, typeId, smartPointer, &ok);
    if(ok)
        return result;
    const char* java_type_name = getJavaName(typeId);
    if(java_type_name){
        return internal_convertSmartPointerToJavaObject_notype<QSharedPointer>(env, java_type_name, smartPointer);
    }
    return nullptr;
}

jobject QtJambiAPI::convertSmartPointerToJavaObject(JNIEnv *env, const std::type_info& typeId,
                                                    const std::shared_ptr<char>& smartPointer)
{
    bool ok;
    jobject result = internal_convertSmartPointerToJavaObject_impl<std::shared_ptr>(env, typeId, smartPointer, &ok);
    if(ok)
        return result;
    const char* java_type_name = getJavaName(typeId);
    if(java_type_name){
        return internal_convertSmartPointerToJavaObject_notype<std::shared_ptr>(env, java_type_name, smartPointer);
    }
    return nullptr;
}

jobject QtJambiAPI::convertQObjectToJavaObject(JNIEnv *env, const QObject *qt_object, const char *className)
{
    if(qt_object){
        try{
            QtJambiAPI::checkDanglingPointer(env, qt_object);
            // check rtti availability:
            if(QtJambiPrivate::CheckPointer<QObject>::trySupplyType(qt_object)!=nullptr)
                if(const QtJambiShellInterface* shell = dynamic_cast<const QtJambiShellInterface*>(qt_object))
                    return QtJambiShellInterface::getJavaObjectLocalRef(env, shell);
        }catch(...){}
    }
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
    if(qt_object){
        try{
            QtJambiAPI::checkDanglingPointer(env, qt_object);
            // check rtti availability:
            if(QtJambiPrivate::CheckPointer<QObject>::trySupplyType(qt_object)!=nullptr)
                if(const QtJambiShellInterface* shell = dynamic_cast<const QtJambiShellInterface*>(qt_object))
                    return QtJambiShellInterface::getJavaObjectLocalRef(env, shell);
        }catch(...){}
    }
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
    if(qt_object){
        try{
            QtJambiAPI::checkDanglingPointer(env, qt_object);
            // check rtti availability:
            if(QtJambiPrivate::CheckPointer<QObject>::trySupplyType(qt_object)!=nullptr)
                if(const QtJambiShellInterface* shell = dynamic_cast<const QtJambiShellInterface*>(qt_object))
                    return QtJambiShellInterface::getJavaObjectLocalRef(env, shell);
        }catch(...){}
    }
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
