/****************************************************************************
**
** Copyright (C) 1992-2009 Nokia. All rights reserved.
** Copyright (C) 2009-2022 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
**
** This file is part of Qt Jambi.
**
** ** $BEGIN_LICENSE$
**
** GNU Lesser General Public License Usage
** This file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Nokia gives you certain
** additional rights. These rights are described in the Nokia Qt LGPL
** Exception version 1.0, included in the file LGPL_EXCEPTION.txt in this
** package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
**
** $END_LICENSE$
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

#include <QDebug>

#include "qtjambi_core.h"
#include "qtjambi_jobjectwrapper.h"
#include "qtjambivariant_p.h"
#include "qtjambitypemanager_p.h"
#include "qtjambi_repository_p.h"
#include "qtjambi_registry_p.h"
#include "qtjambilink_p.h"

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
const QVariant::Handler *QtJambiVariant::lastHandler = nullptr;

Q_CORE_EXPORT const QVariant::Handler *qcoreVariantHandler();

const QtPrivate::AbstractComparatorFunction * qtjambi_registered_comparator(int typeId);
const QtPrivate::AbstractDebugStreamFunction * qtjambi_registered_debugstream_operator(int typeId);

inline static const JObjectWrapper *cast_to_object_wrapper(const QVariant::Private *d)
{
    int d_type = d->type;
    if (d_type == qMetaTypeId<JObjectWrapper>()
            || d_type == qMetaTypeId<JEnumWrapper>()
            || d_type == qMetaTypeId<JMapWrapper>()
            || d_type == qMetaTypeId<JCollectionWrapper>()
            || d_type == qMetaTypeId<JIteratorWrapper>()) {
        if (d->is_shared) return reinterpret_cast<const JObjectWrapper *>(d->data.shared->ptr);
        else return reinterpret_cast<const JObjectWrapper *>(&d->data.ptr);
    }
    return nullptr;
}

static void construct(QVariant::Private *x, const void *copy)
{
    if (QtJambiVariant::getLastHandler())
        QtJambiVariant::getLastHandler()->construct(x, copy);
    else if (qcoreVariantHandler())
        qcoreVariantHandler()->construct(x, copy);
}

static void clear(QVariant::Private *d)
{
    if (QtJambiVariant::getLastHandler())
        QtJambiVariant::getLastHandler()->clear(d);
    else if (qcoreVariantHandler())
        qcoreVariantHandler()->clear(d);
}

static bool isNull(const QVariant::Private *d)
{
    int d_type = d->type;
    if(d_type == qMetaTypeId<JObjectWrapper>()
            || d_type == qMetaTypeId<JEnumWrapper>()
            || d_type == qMetaTypeId<JMapWrapper>()
            || d_type == qMetaTypeId<JCollectionWrapper>()
            || d_type == qMetaTypeId<JIteratorWrapper>()){
        const JObjectWrapper *wrapper = cast_to_object_wrapper(d);
        return wrapper->object()!=nullptr; // JObjectWrapper can also carry "null" reference
    }else if (QtJambiVariant::getLastHandler())
        return QtJambiVariant::getLastHandler()->isNull(d);
    else if (qcoreVariantHandler())
        return qcoreVariantHandler()->isNull(d);

    return false;
}

static bool convert(const QVariant::Private *d, int t,
                 void *result, bool *ok)
{
    if (qcoreVariantHandler()){
        if(qcoreVariantHandler()->convert(d, t, result, ok)){
            if(!ok || *ok)
                return true;
        }
    }
    switch (t) {
    case QVariant::Int :
    case QVariant::UInt :
    case QVariant::ULongLong :
    case QVariant::LongLong :
    case QVariant::Char :
    case QVariant::String :
        if(JNIEnv *env = qtjambi_current_environment()){
            QTJAMBI_JNI_LOCAL_FRAME(env, 200)
            const JObjectWrapper *wrapper = cast_to_object_wrapper(d);
            int d_type = d->type;

            jobject java_object =  nullptr;
            if(wrapper != nullptr) {
                java_object =  wrapper->object();
            }else{
                QMetaType typeInfo(d_type);
                //if (!(typeInfo.flags() & QMetaType::IsEnumeration))
                //    return false;
                jvalue jval;
                jval.l = nullptr;
                void * in = d->is_shared ? const_cast<void **>(&d->data.shared->ptr) : const_cast<void **>(&d->data.ptr);
                        //in = d->is_shared ? d->data.shared->ptr : d->data.ptr;
                InternalToExternalConverter variantToJavaConverter;
                {
                    QString qtName = QMetaType::typeName(d_type);
                    QString javaName = QtJambiTypeManager::getExternalTypeName(env, qtName, typeInfo);
                    jclass targetClass = resolveClass(env, qPrintable(javaName));
                    // Find usage pattern
                    variantToJavaConverter = QtJambiTypeManager::getInternalToExternalConverter(
                                                        env,
                                                        qtName,
                                                        typeInfo,
                                                        targetClass,
                                                        false);
                }
                if(variantToJavaConverter
                        && variantToJavaConverter(env, nullptr, in, &jval, true)){
                    java_object = jval.l;
                }else{
                    return false;
                }
            }
            switch (t) {
            case QVariant::Int :
            case QVariant::UInt :
            case QVariant::ULongLong :
            case QVariant::LongLong :
            case QVariant::Char :
                {
                    QVariant enumValue;
                    if(java_object==nullptr){  // avoid NullPointerException
                        if(ok) *ok=true;
                        return true;
                    }else if (Java::QtJambi::QtEnumerator::isInstanceOf(env, java_object)) {
                        enumValue.setValue<qint32>(Java::QtJambi::QtEnumerator::value(env, java_object));
                    }else if (Java::QtJambi::QtShortEnumerator::isInstanceOf(env, java_object)) {
                        enumValue.setValue<qint16>(Java::QtJambi::QtShortEnumerator::value(env, java_object));
                    }else if (Java::QtJambi::QtByteEnumerator::isInstanceOf(env, java_object)) {
                        enumValue.setValue<qint8>(Java::QtJambi::QtByteEnumerator::value(env, java_object));
                    }else if (Java::QtJambi::QtLongEnumerator::isInstanceOf(env, java_object)) {
                        enumValue.setValue<qint64>(Java::QtJambi::QtLongEnumerator::value(env, java_object));
                    }else if (Java::QtJambi::QFlags::isInstanceOf(env, java_object)) {
                        enumValue.setValue<qint32>(Java::QtJambi::QFlags::value(env, java_object));
                    }else if (Java::Runtime::Integer::isInstanceOf(env, java_object)) {
                        enumValue.setValue<qint32>(Java::Runtime::Number::intValue(env, java_object));
                    }else if (Java::Runtime::Short::isInstanceOf(env, java_object)) {
                        enumValue.setValue<qint16>(Java::Runtime::Number::shortValue(env, java_object));
                    }else if (Java::Runtime::Byte::isInstanceOf(env, java_object)) {
                        enumValue.setValue<qint8>(Java::Runtime::Number::byteValue(env, java_object));
                    }else if (Java::Runtime::Long::isInstanceOf(env, java_object)) {
                        enumValue.setValue<qint64>(Java::Runtime::Number::longValue(env, java_object));
                    }else if (Java::Runtime::Double::isInstanceOf(env, java_object)) {
                        enumValue.setValue<double>(Java::Runtime::Number::doubleValue(env, java_object));
                    }else if (Java::Runtime::Float::isInstanceOf(env, java_object)) {
                        enumValue.setValue<float>(Java::Runtime::Number::floatValue(env, java_object));
                    }else if (Java::Runtime::Boolean::isInstanceOf(env, java_object)) {
                        enumValue.setValue<bool>(Java::Runtime::Boolean::booleanValue(env, java_object));
                    }else if (Java::Runtime::Character::isInstanceOf(env, java_object)) {
                        enumValue.setValue<ushort>(ushort(Java::Runtime::Character::charValue(env, java_object)));
                    }
                    if(enumValue.isValid()){
                        switch (t) {
                        case QVariant::Int :
                            *reinterpret_cast<int*>(result) = enumValue.value<int>();
                            break;
                        case QVariant::UInt :
                            *reinterpret_cast<uint*>(result) = enumValue.value<uint>();
                            break;
                        case QVariant::ULongLong :
                            *reinterpret_cast<qulonglong*>(result) = enumValue.value<qulonglong>();
                            break;
                        case QVariant::LongLong :
                            *reinterpret_cast<qlonglong*>(result) = enumValue.value<qlonglong>();
                            break;
                        case QVariant::Char :
                            *reinterpret_cast<ushort*>(result) = enumValue.value<ushort>();
                            break;
                        default:
                            break;
                        }
                        if(ok) *ok=true;
                        return true;
                    }else{
                        if(ok) *ok=false;
                        return false;
                    }
                }
            case QVariant::String :
                if(java_object){
                    if(wrapper && Java::QtJambi::QtObjectInterface::isInstanceOf(env, java_object)){
                        jclass object_class = env->GetObjectClass(java_object);
                        QString fullJavaName = qtjambi_object_class_name(env, java_object).replace(".", "/");
                        QString qtName = QtJambiTypeManager::getInternalTypeName(env, object_class);
                        if(qtName.endsWith("*")){
                            int type = QMetaType::type(qPrintable(qtName));
                            if (type != QVariant::Invalid
                                    && type != qMetaTypeId<JObjectWrapper>()
                                    && type != qMetaTypeId<JEnumWrapper>()
                                    && type != qMetaTypeId<JMapWrapper>()
                                    && type != qMetaTypeId<JCollectionWrapper>()
                                    && type != qMetaTypeId<JIteratorWrapper>()
                                    ) {
                                if (QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForJavaObject(env, java_object)){
                                    if(link->ownership()==QtJambiLink::Ownership::Java){
                                        void* p = link->pointer();

#if !defined(Q_NO_TEMPLATE_FRIENDS) && !defined(Q_CC_MSVC)
                                        struct ConvertVariant : public QVariant{
                                        public:
                                            ConvertVariant(int typeId, const void *copy) : QVariant(typeId, copy){}
                                            bool convert(const int t, void *ptr) const{
                                                return QVariant::convert(t, ptr);
                                            }
                                        };
                                        ConvertVariant nativeVariant(type, &p);
#else
                                        QVariant nativeVariant(type, &p);
#endif
                                        bool b = nativeVariant.convert(t, result);
                                        if(ok) *ok=b;
                                        return b;
                                    }
                                }
                            }
                        }
                    }
                    jstring strg = Java::Runtime::Object::toString(env, java_object);
                    *reinterpret_cast<QString*>(result) = qtjambi_to_qstring(env, strg);
                }
                if(ok) *ok=true;
                return true;
            default :
                break;
            }
        }
        break;
    default :
        if(const JObjectWrapper *wrapper = cast_to_object_wrapper(d)){
            QByteArray targetType = QMetaType::typeName(t);
            if(targetType=="QRemoteObjectPendingCall")
                return false;
            const std::type_info* typeId = getTypeByMetaType(t);
            if(!typeId){
                QByteArray typeName = QMetaType::typeName(t);
                if(typeName.endsWith("*")){
                    typeName = typeName.chopped(1);
                }
                typeId = getTypeByQtName(typeName.data());
            }
            if(typeId){
                if(JNIEnv *env = qtjambi_current_environment()){
                    QTJAMBI_JNI_LOCAL_FRAME(env, 200)
                    if(jobject java_object = wrapper->object()){
                        if(qtjambi_convert_to_native(env, *typeId, java_object, result)){
                            if(ok) *ok=true;
                            return true;
                        }
                    }
                }
            }
            if((targetType.startsWith("QList<")
                || targetType.startsWith("QQueue<")
                || targetType.startsWith("QStack<")
                || targetType.startsWith("QVector<")
                || targetType.startsWith("QLinkedList<")
                || targetType.startsWith("QSet<")) && targetType.endsWith('>')){
                auto idx = targetType.indexOf("<");
                QByteArray instantiation = targetType.mid(idx+1).chopped(1).trimmed();
                int elementMetaType = QMetaType::type(instantiation);
                if(elementMetaType!=QMetaType::UnknownType){
                    if(JNIEnv *env = qtjambi_current_environment()){
                        QTJAMBI_JNI_LOCAL_FRAME(env, 200)
                        if(QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForJavaObject(env, wrapper->object())){
                            if(AbstractContainerAccess* _containerAccess = link->containerAccess()){
                                if(targetType.startsWith("QList<") || targetType.startsWith("QQueue<")){
                                    if(AbstractListAccess* containerAccess = dynamic_cast<AbstractListAccess*>(_containerAccess)){
                                        if(elementMetaType==containerAccess->elementMetaType().id()){
                                            containerAccess->assign(result, link->pointer());
                                            if(ok) *ok=true;
                                            return true;
                                        }
                                    }
                                }else if(targetType.startsWith("QVector<") || targetType.startsWith("QStack<")){
                                    if(AbstractVectorAccess* containerAccess = dynamic_cast<AbstractVectorAccess*>(_containerAccess)){
                                        if(elementMetaType==containerAccess->elementMetaType().id()){
                                            containerAccess->assign(result, link->pointer());
                                            if(ok) *ok=true;
                                            return true;
                                        }
                                    }
                                }else if(targetType.startsWith("QSet<")){
                                    if(AbstractSetAccess* containerAccess = dynamic_cast<AbstractSetAccess*>(_containerAccess)){
                                        if(elementMetaType==containerAccess->elementMetaType().id()){
                                            containerAccess->assign(result, link->pointer());
                                            if(ok) *ok=true;
                                            return true;
                                        }
                                    }
                                }else if(targetType.startsWith("QLinkedList<")){
                                    if(AbstractLinkedListAccess* containerAccess = dynamic_cast<AbstractLinkedListAccess*>(_containerAccess)){
                                        if(elementMetaType==containerAccess->elementMetaType().id()){
                                            containerAccess->assign(result, link->pointer());
                                            if(ok) *ok=true;
                                            return true;
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }else if((targetType.startsWith("QMap<")
                         || targetType.startsWith("QHash<")
                         || targetType.startsWith("QMultiMap<")
                         || targetType.startsWith("QMultiHash<")) && targetType.endsWith('>')){
                auto idx = targetType.indexOf("<");
                QByteArrayList instantiations = targetType.mid(idx+1).chopped(1).trimmed().split(',');
                if(instantiations.size()==2){
                    int keyMetaType = QMetaType::type(instantiations[0]);
                    int valueMetaType = QMetaType::type(instantiations[0]);
                    if(keyMetaType!=QMetaType::UnknownType && valueMetaType!=QMetaType::UnknownType){
                        if(JNIEnv *env = qtjambi_current_environment()){
                            QTJAMBI_JNI_LOCAL_FRAME(env, 200)
                            if(QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForJavaObject(env, wrapper->object())){
                                if(AbstractContainerAccess* _containerAccess = link->containerAccess()){
                                    if(targetType.startsWith("QMap<")){
                                        if(AbstractMapAccess* containerAccess = dynamic_cast<AbstractMapAccess*>(_containerAccess)){
                                            if(keyMetaType==containerAccess->keyMetaType().id()
                                                    && valueMetaType==containerAccess->valueMetaType().id()){
                                                containerAccess->assign(result, link->pointer());
                                                if(ok) *ok=true;
                                                return true;
                                            }
                                        }
                                    }else if(targetType.startsWith("QHash<")){
                                        if(AbstractHashAccess* containerAccess = dynamic_cast<AbstractHashAccess*>(_containerAccess)){
                                            if(keyMetaType==containerAccess->keyMetaType().id()
                                                    && valueMetaType==containerAccess->valueMetaType().id()){
                                                containerAccess->assign(result, link->pointer());
                                                if(ok) *ok=true;
                                                return true;
                                            }
                                        }
                                    }else if(targetType.startsWith("QMultiMap<")){
                                        if(AbstractMultiMapAccess* containerAccess = dynamic_cast<AbstractMultiMapAccess*>(_containerAccess)){
                                            if(keyMetaType==containerAccess->keyMetaType().id()
                                                    && valueMetaType==containerAccess->valueMetaType().id()){
                                                containerAccess->assign(result, link->pointer());
                                                if(ok) *ok=true;
                                                return true;
                                            }
                                        }
                                    }else if(targetType.startsWith("QMultiHash<")){
                                        if(AbstractMultiHashAccess* containerAccess = dynamic_cast<AbstractMultiHashAccess*>(_containerAccess)){
                                            if(keyMetaType==containerAccess->keyMetaType().id()
                                                    && valueMetaType==containerAccess->valueMetaType().id()){
                                                containerAccess->assign(result, link->pointer());
                                                if(ok) *ok=true;
                                                return true;
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }else{
            const std::type_info* typeId1 = getTypeByMetaType(d->type);
            if(!typeId1){
                QByteArray typeName = QMetaType::typeName(d->type);
                if(typeName.endsWith("*")){
                    typeName = typeName.chopped(1);
                    typeId1 = getTypeByQtName(typeName.data());
                    if(typeId1){
                        const std::type_info* typeId2 = getTypeByMetaType(t);
                        if(!typeId2){
                            QByteArray typeName = QMetaType::typeName(t);
                            if(typeName.endsWith("*")){
                                typeName = typeName.chopped(1);
                                typeId2 = getTypeByQtName(typeName.data());
                                if(typeId2){
                                    int offset = registeredInterfaceOffset(*typeId1, *typeId2);
                                    if(offset>=0){
                                        void * in = d->is_shared ? const_cast<void **>(&d->data.shared->ptr) : const_cast<void **>(&d->data.ptr);
                                        char* ptr = *reinterpret_cast<char*const*>(in);
                                        ptr += offset;
                                        *reinterpret_cast<const void**>(result) = ptr;
                                        return true;
                                    }else{
                                        offset = registeredInterfaceOffset(*typeId2, *typeId1);
                                        if(offset>=0){
                                            void * in = d->is_shared ? const_cast<void **>(&d->data.shared->ptr) : const_cast<void **>(&d->data.ptr);
                                            char* ptr = *reinterpret_cast<char*const*>(in);
                                            ptr += offset;
                                            *reinterpret_cast<const void**>(result) = ptr;
                                            return true;
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
        break;
    }
    if (QtJambiVariant::getLastHandler())
        return QtJambiVariant::getLastHandler()->convert(d, t, result, ok);
    return false;
}

static bool canConvert(const QVariant::Private *d, int t)
{
    if (qcoreVariantHandler()){
        if(qcoreVariantHandler()->canConvert(d, t)){
            return true;
        }
    }
    if(strcmp("QRemoteObjectPendingCall", QMetaType::typeName(t))==0)
        return false;
    switch (t) {
    case QVariant::Int :
    case QVariant::UInt :
    case QVariant::ULongLong :
    case QVariant::LongLong :
    case QVariant::Char :
    case QVariant::String :
        if(JNIEnv *env = qtjambi_current_environment()){
            QTJAMBI_JNI_LOCAL_FRAME(env, 200)
            const JObjectWrapper *wrapper = cast_to_object_wrapper(d);

            jobject java_object =  nullptr;
            if(wrapper != nullptr) {
                java_object =  wrapper->object();
            }else{
                int d_type = d->type;
                QMetaType typeInfo(d_type);
                if (typeInfo.flags() & QMetaType::IsEnumeration) {
                    jvalue jval;
                    jval.l = nullptr;
                    void * in = d->is_shared ? const_cast<void **>(&d->data.shared->ptr) : const_cast<void **>(&d->data.ptr);
                            //in = d->is_shared ? d->data.shared->ptr : d->data.ptr;
                    InternalToExternalConverter variantToJavaConverter;
                    {
                        QString qtName = QMetaType::typeName(d_type);
                        QString javaName = QtJambiTypeManager::getExternalTypeName(env, qtName, typeInfo);
                        jclass targetClass = resolveClass(env, qPrintable(javaName));
                        // Find usage pattern
                        variantToJavaConverter = QtJambiTypeManager::getInternalToExternalConverter(
                                                            env,
                                                            qtName,
                                                            typeInfo,
                                                            targetClass,
                                                            false);
                    }
                    if(variantToJavaConverter
                            && variantToJavaConverter(env, nullptr, in, &jval, true)){
                        java_object = jval.l;
                    }else{
                        return false;
                    }
                }else{
                    return false;
                }
            }
            switch (t) {
            case QVariant::Int :
            case QVariant::UInt :
            case QVariant::ULongLong :
            case QVariant::LongLong :
            case QVariant::Char :
            {
                return (java_object==nullptr
                        || Java::QtJambi::QtEnumerator::isInstanceOf(env, java_object)
                        || Java::QtJambi::QtShortEnumerator::isInstanceOf(env, java_object)
                        || Java::QtJambi::QtByteEnumerator::isInstanceOf(env, java_object)
                        || Java::QtJambi::QtLongEnumerator::isInstanceOf(env, java_object)
                        || Java::QtJambi::QFlags::isInstanceOf(env, java_object)
                        || Java::Runtime::Integer::isInstanceOf(env, java_object)
                        || Java::Runtime::Short::isInstanceOf(env, java_object)
                        || Java::Runtime::Byte::isInstanceOf(env, java_object)
                        || Java::Runtime::Long::isInstanceOf(env, java_object)
                        || Java::Runtime::Double::isInstanceOf(env, java_object)
                        || Java::Runtime::Float::isInstanceOf(env, java_object)
                        || Java::Runtime::Boolean::isInstanceOf(env, java_object)
                        || Java::Runtime::Character::isInstanceOf(env, java_object));
            }
            case QVariant::String :
                if(java_object){
                    if(wrapper && Java::QtJambi::QtObjectInterface::isInstanceOf(env, java_object)){
                        jclass object_class = env->GetObjectClass(java_object);
                        QString fullJavaName = qtjambi_object_class_name(env, java_object).replace(".", "/");
                        QString qtName = QtJambiTypeManager::getInternalTypeName(env, object_class);
                        if(qtName.endsWith("*")){
                            int type = QMetaType::type(qPrintable(qtName));
                            if (type != QVariant::Invalid
                                    && type != qMetaTypeId<JObjectWrapper>()
                                    && type != qMetaTypeId<JEnumWrapper>()
                                    && type != qMetaTypeId<JMapWrapper>()
                                    && type != qMetaTypeId<JCollectionWrapper>()
                                    && type != qMetaTypeId<JIteratorWrapper>()
                                    ) {
                                if (QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForJavaObject(env, java_object)){
                                    if(link->ownership()==QtJambiLink::Ownership::Java){
                                        void* p = link->pointer();
                                        QVariant nativeVariant(type, &p);
                                        return nativeVariant.canConvert(t);
                                    }
                                }
                            }
                        }

                    }
                }
                return true;
            default :
                break;
            }
        }
        break;
    default :
        if(const JObjectWrapper *wrapper = cast_to_object_wrapper(d)){
            const std::type_info* typeId = getTypeByMetaType(t);
            if(!typeId){
                QString typeName = QLatin1String(QMetaType::typeName(t));
                if(typeName.endsWith("*")){
                    typeName.chop(1);
                }
                typeId = getTypeByQtName(typeName);
            }
            if(typeId){
                if(JNIEnv *env = qtjambi_current_environment()){
                    QTJAMBI_JNI_LOCAL_FRAME(env, 200)
                    if(jobject java_object = wrapper->object()){
                        void* result = QMetaType::create(t);
                        bool can = qtjambi_convert_to_native(env, *typeId, java_object, result);
                        QMetaType::destroy(t, result);
                        if(can){
                            return true;
                        }else{
                            QObject* object = nullptr;
                            if(qtjambi_convert_to_native(env, typeid(QObject), java_object, &object)){
                                if(object){
                                    void* ptr = object->qt_metacast(QMetaType::typeName(t));
                                    if(ptr){
                                        return true;
                                    }else{
                                        QString typeName = QLatin1String(QMetaType::typeName(t));
                                        if(typeName.endsWith("*")){
                                            typeName.chop(1);
                                            ptr = object->qt_metacast(qPrintable(typeName));
                                            if(ptr){
                                                return true;
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
        break;
    }
    if (QtJambiVariant::getLastHandler())
        return QtJambiVariant::getLastHandler()->canConvert(d, t);
    return false;
}

static bool compare(const QVariant::Private *a, const QVariant::Private *b)
{
    Q_ASSERT(a->type == b->type);
    const JObjectWrapper *wrapper_a = cast_to_object_wrapper(a);
    if (wrapper_a) {
        const JObjectWrapper *wrapper_b = cast_to_object_wrapper(b);
        return wrapper_a==wrapper_b;
    }
    if(const QtPrivate::AbstractComparatorFunction * fun = qtjambi_registered_comparator(a->type)){
        if(fun->equals){
            const void* ptrA = a->is_shared ? a->data.shared->ptr : a->data.ptr;
            const void* ptrB = b->is_shared ? b->data.shared->ptr : b->data.ptr;
            return fun->equals(fun, ptrA, ptrB);
        }
    }
    if (QtJambiVariant::getLastHandler())
        return QtJambiVariant::getLastHandler()->compare(a, b);
    else if (qcoreVariantHandler())
        return qcoreVariantHandler()->compare(a, b);

    return false;
}

QString qtjambi_to_qstring(JNIEnv *env, jobject object);

#if !defined(QT_NO_DEBUG_STREAM) && !defined(Q_BROKEN_DEBUG_STREAM)
static void streamDebug(QDebug dbg, const QVariant &v)
{

    if(v.userType() == qMetaTypeId<JObjectWrapper>()
            || v.userType() == qMetaTypeId<JEnumWrapper>()
            || v.userType() == qMetaTypeId<JMapWrapper>()
            || v.userType() == qMetaTypeId<JCollectionWrapper>()
            || v.userType() == qMetaTypeId<JIteratorWrapper>()) {
        const JObjectWrapper wrapper = v.value<JObjectWrapper>();
        dbg << wrapper;
        return;
    }
    if(const QtPrivate::AbstractDebugStreamFunction * function = qtjambi_registered_debugstream_operator(v.userType())){
        if(function->stream){
            function->stream(function, dbg, v.constData());
            return;
        }
    }
    if (QtJambiVariant::getLastHandler()) {
        QtJambiVariant::getLastHandler()->debugStream(dbg, v);
        return;
    }
    else if (qcoreVariantHandler()) {
        qcoreVariantHandler()->debugStream(dbg, v);
        return;
    }
}
#endif

const QVariant::Handler QtJambiVariant::handler = {
    ::construct,
    ::clear,
    ::isNull,
#ifndef QT_NO_DATASTREAM
    nullptr,
    nullptr,
#endif
    ::compare,
    ::convert,
    ::canConvert,
#if !defined(QT_NO_DEBUG_STREAM) && !defined(Q_BROKEN_DEBUG_STREAM)
    ::streamDebug
#else
    nullptr
#endif
};

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
/**
 * This is a simple copy out of variant.cpp because the corresponding
 * originals are inaccessible through annonymous namespaces.
 * However, we need the original custom handler as fallback because we register a new
 * one.
 *
 * @brief customConstruct
 * @param d
 * @param copy
 */
static void customConstruct(QVariant::Private *d, const void *copy)
{
    const QMetaType type(d->type);
    const uint size = uint(type.sizeOf());
    if (!size) {
        qWarning("Trying to construct an instance of an invalid type, type id: %i", d->type);
        d->type = QVariant::Invalid;
        return;
    }

    // this logic should match with QVariantIntegrator::CanUseInternalSpace
    if (size <= sizeof(QVariant::Private::Data)
            && (type.flags() & (QMetaType::MovableType | QMetaType::IsEnumeration))) {
        type.construct(&d->data.ptr, copy);
        d->is_null = d->data.ptr == nullptr;
        d->is_shared = false;
    } else {
        // Private::Data contains long long, and long double is the biggest standard type.
        const size_t maxAlignment =
            qMax(Q_ALIGNOF(QVariant::Private::Data), Q_ALIGNOF(long double));
        const size_t s = sizeof(QVariant::PrivateShared);
        const size_t offset = s + ((s * maxAlignment - s) % maxAlignment);
        void *data = operator new(offset + size);
        void *ptr = static_cast<char *>(data) + offset;
        type.construct(ptr, copy);
        d->is_null = ptr == nullptr;
        d->is_shared = true;
        d->data.shared = new (data) QVariant::PrivateShared(ptr);
    }
}

static void customClear(QVariant::Private *d)
{
    if (!d->is_shared) {
        QMetaType::destruct(d->type, &d->data.ptr);
    } else {
        QMetaType::destruct(d->type, d->data.shared->ptr);
        d->data.shared->~PrivateShared();
        operator delete(d->data.shared);
    }
}
#endif //QT_VERSION < QT_VERSION_CHECK(6, 0, 0)

static bool customIsNull(const QVariant::Private *d)
{
    if (d->is_null)
        return true;
    const char *const typeName = QMetaType::typeName(d->type);
    if (Q_UNLIKELY(!typeName) && Q_LIKELY(!QMetaType::isRegistered(d->type)))
        qFatal("QVariant::isNull: type %d unknown to QVariant.", d->type);
    auto typeNameLen = qstrlen(typeName);
    if (typeNameLen > 0 && typeName[typeNameLen - 1] == '*') {
        const void *d_ptr = d->is_shared ? d->data.shared->ptr : &(d->data.ptr);
        return *static_cast<void *const *>(d_ptr) == nullptr;
    }
    return false;
}

static bool customCompare(const QVariant::Private *a, const QVariant::Private *b)
{
    const char *const typeName = QMetaType::typeName(a->type);
    if (Q_UNLIKELY(!typeName) && Q_LIKELY(!QMetaType::isRegistered(a->type)))
        qFatal("QVariant::compare: type %d unknown to QVariant.", a->type);

    const void *a_ptr = a->is_shared ? a->data.shared->ptr : &(a->data.ptr);
    const void *b_ptr = b->is_shared ? b->data.shared->ptr : &(b->data.ptr);

    auto typeNameLen = qstrlen(typeName);
    if (typeNameLen > 0 && typeName[typeNameLen - 1] == '*')
        return *static_cast<void *const *>(a_ptr) == *static_cast<void *const *>(b_ptr);

    if (a->is_null && b->is_null)
        return true;

    return !memcmp(a_ptr, b_ptr, size_t(QMetaType::sizeOf(a->type)));
}

static bool customConvert(const QVariant::Private *, int, void *, bool *ok)
{
    if (ok)
        *ok = false;
    return false;
}

static bool customCanConvert(const QVariant::Private *, int)
{
    return false;
}

#if !defined(QT_NO_DEBUG_STREAM)
static void customStreamDebug(QDebug dbg, const QVariant &variant) {
#ifndef QT_BOOTSTRAPPED
    QMetaType::TypeFlags flags = QMetaType::typeFlags(variant.userType());
    if (flags & QMetaType::PointerToQObject)
        dbg.nospace() << variant.value<QObject*>();
#else
    Q_UNUSED(dbg);
    Q_UNUSED(variant);
#endif
}
#endif

const QVariant::Handler QtJambiVariant::qt_custom_variant_handler = {
    customConstruct,
    customClear,
    customIsNull,
#ifndef QT_NO_DATASTREAM
    nullptr,
    nullptr,
    customCompare,
#endif
    customConvert,
    customCanConvert,
#if !defined(QT_NO_DEBUG_STREAM)
    customStreamDebug
#else
    nullptr
#endif
};
#endif //QT_VERSION < QT_VERSION_CHECK(6, 0, 0)

