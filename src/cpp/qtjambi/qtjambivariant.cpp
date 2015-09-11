/****************************************************************************
**
** Copyright (C) 1992-2009 Nokia. All rights reserved.
** Copyright (C) 2009-2015 Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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

#include "qtjambivariant_p.h"

static const uint JOBJECTWRAPPER_TYPE = qMetaTypeId<JObjectWrapper>();
const QVariant::Handler *QtJambiVariant::lastHandler = 0;

Q_CORE_EXPORT const QVariant::Handler *qcoreVariantHandler();

inline static const JObjectWrapper *cast_to_object_wrapper(const QVariant::Private *d)
{
    if (d->type == JOBJECTWRAPPER_TYPE) {
        if (d->is_shared) return reinterpret_cast<const JObjectWrapper *>(d->data.shared->ptr);
        else return reinterpret_cast<const JObjectWrapper *>(&d->data.c);
    }
    return 0;
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
    if(d->type == JOBJECTWRAPPER_TYPE){
        const JObjectWrapper *wrapper = cast_to_object_wrapper(d);
        return wrapper->object!=0; // JObjectWrapper can also carry "null" reference
    }else if (QtJambiVariant::getLastHandler())
        return QtJambiVariant::getLastHandler()->isNull(d);
    else if (qcoreVariantHandler())
        return qcoreVariantHandler()->isNull(d);

    return false;
}

static bool convert(const QVariant::Private *d, int t,
                 void *result, bool *ok)
{
    const JObjectWrapper *wrapper = cast_to_object_wrapper(d);

    if(wrapper != 0) {
        JNIEnv *env = qtjambi_current_environment();
        StaticCache *sc = StaticCache::instance();
        jobject java_object =  wrapper->object;

        switch (t) {
        case QVariant::Int :
            sc->resolveQtEnumerator();
            if(java_object==0){  // avoid NullPointerException
                return true;
            }else if (env->IsInstanceOf(java_object, sc->QtEnumerator.class_ref)) {
                *((int*)result) = env->CallIntMethod(java_object, sc->QtEnumerator.value);
                qtjambi_exception_check(env);
                return true;
            }
            break;

        case QVariant::String :
            sc->resolveObject();
            if(java_object!=0){
                *((QString*)result) = qtjambi_to_qstring(env, static_cast<jstring>(env->CallObjectMethod(java_object, sc->Object.toString)));
                qtjambi_exception_check(env);
                return true;
            }else{  // avoid NullPointerException
                return true;
            }
            break;

        default :
            return false;
        }
    }

    if (QtJambiVariant::getLastHandler())
        return QtJambiVariant::getLastHandler()->convert(d, t, result, ok);
    else if (qcoreVariantHandler())
        return qcoreVariantHandler()->convert(d, t, result, ok);

    return false;
}

static bool compare(const QVariant::Private *a, const QVariant::Private *b)
{
    Q_ASSERT(a->type == b->type);
    const JObjectWrapper *wrapper_a = cast_to_object_wrapper(a);
    if (wrapper_a) {
        const JObjectWrapper *wrapper_b = cast_to_object_wrapper(b);
        JNIEnv *env = qtjambi_current_environment();
        StaticCache *sc = StaticCache::instance();
        if(wrapper_b->object==0 || wrapper_a->object==0){  // avoid NullPointerException
            return wrapper_b->object==0 && wrapper_a->object==0; // null == null
        }
        return env->CallBooleanMethod(wrapper_a->object, sc->Object.equals, wrapper_b->object);
    }
    if (QtJambiVariant::getLastHandler())
        return QtJambiVariant::getLastHandler()->compare(a, b);
    else if (qcoreVariantHandler())
        return qcoreVariantHandler()->compare(a, b);

    return false;
}


#if !defined(QT_NO_DEBUG_STREAM) && !defined(Q_BROKEN_DEBUG_STREAM)
static void streamDebug(QDebug dbg, const QVariant &v)
{

    if((uint)v.userType() == JOBJECTWRAPPER_TYPE) {
        const JObjectWrapper wrapper = v.value<JObjectWrapper>();
        JNIEnv *env = qtjambi_current_environment();
        StaticCache *sc = StaticCache::instance();
        sc->resolveObject();
        jobject java_object =  wrapper.object;
        if(java_object==0){ // avoid NullPointerException
            dbg << "null";
        }else{
            dbg << qtjambi_to_qstring(env, static_cast<jstring>(env->CallObjectMethod(java_object, sc->Object.toString)));
        }
        return;
    }
    else if (QtJambiVariant::getLastHandler()) {
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
    0,
    0,
#endif
    ::compare,
    ::convert,
    0,
#if !defined(QT_NO_DEBUG_STREAM) && !defined(Q_BROKEN_DEBUG_STREAM)
    ::streamDebug
#else
    0
#endif
};

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
    const uint size = type.sizeOf();
    if (!size) {
        qWarning("Trying to construct an instance of an invalid type, type id: %i", d->type);
        d->type = QVariant::Invalid;
        return;
    }

    // this logic should match with QVariantIntegrator::CanUseInternalSpace
    if (size <= sizeof(QVariant::Private::Data)
            && (type.flags() & QMetaType::MovableType)) {
        type.construct(&d->data.ptr, copy);
        d->is_shared = false;
    } else {
        void *ptr = operator new(size);
        type.construct(ptr, copy);
        d->is_shared = true;
        d->data.shared = new QVariant::PrivateShared(ptr);
    }
}

static void customClear(QVariant::Private *d)
{
    if (!d->is_shared) {
        QMetaType::destruct(d->type, &d->data.ptr);
    } else {
        QMetaType::destroy(d->type, d->data.shared->ptr);
        delete d->data.shared;
    }
}

static bool customIsNull(const QVariant::Private *d)
{
    return d->is_null;
}

static bool customCompare(const QVariant::Private *a, const QVariant::Private *b)
{
    const char *const typeName = QMetaType::typeName(a->type);
    if (Q_UNLIKELY(!typeName) && Q_LIKELY(!QMetaType::isRegistered(a->type)))
        qFatal("QVariant::compare: type %d unknown to QVariant.", a->type);

    const void *a_ptr = a->is_shared ? a->data.shared->ptr : &(a->data.ptr);
    const void *b_ptr = b->is_shared ? b->data.shared->ptr : &(b->data.ptr);

    uint typeNameLen = qstrlen(typeName);
    if (typeNameLen > 0 && typeName[typeNameLen - 1] == '*')
        return *static_cast<void *const *>(a_ptr) == *static_cast<void *const *>(b_ptr);

    if (a->is_null && b->is_null)
        return true;

    return !memcmp(a_ptr, b_ptr, QMetaType::sizeOf(a->type));
}

static bool customConvert(const QVariant::Private *, int, void *, bool *ok)
{
    if (ok)
        *ok = false;
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
    0,
    0,
#endif
    customCompare,
    customConvert,
    0,
#if !defined(QT_NO_DEBUG_STREAM)
    customStreamDebug
#else
    0
#endif
};

