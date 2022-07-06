#include "qtjambi_core.h"
#include <QtCore/QReadWriteLock>
#include <QtCore/QMap>
#include <QtCore/QSharedPointer>
#include "qtjambi_containeraccess_p.h"
#include "qtjambi_functionpointer.h"
#include "qtjambi_registry_p.h"

AutoIteratorAccess::~AutoIteratorAccess() = default;
AutoIteratorAccess::AutoIteratorAccess(
        const InternalToExternalConverter& internalToExternalConverter,
        std::function<void(void*)>&& increment,
        std::function<void(void*)>&& decrement,
        std::function<const void*(void*)>&& value,
        std::function<bool(void*,void*)>&& lessThan,
        std::function<bool(void*,void*)>&& equals
    )
    : AbstractIteratorAccess(),
      m_internalToExternalConverter(internalToExternalConverter),
      m_increment(std::move(increment)),
      m_decrement(std::move(decrement)),
      m_value(std::move(value)),
      m_lessThan(std::move(lessThan)),
      m_equals(std::move(equals))
{
    Q_ASSERT(m_value);
}

void AutoIteratorAccess::dispose() {delete this;}

AbstractIteratorAccess* AutoIteratorAccess::clone()
{
    return new AutoIteratorAccess(
                m_internalToExternalConverter,
                std::function<void(void*)>{m_increment},
                std::function<void(void*)>{m_decrement},
                std::function<const void*(void*)>{m_value},
                std::function<bool(void*,void*)>{m_lessThan},
                std::function<bool(void*,void*)>{m_equals});
}

jobject AutoIteratorAccess::value(JNIEnv * env, void* iterator)
{
    const void* v = m_value(iterator);
    jvalue jval;
    jval.l = nullptr;
    if(m_internalToExternalConverter(env, nullptr, v, &jval, true))
        return jval.l;
    return nullptr;
}

void AutoIteratorAccess::increment(JNIEnv *, void* iterator)
{
    m_increment(iterator);
}

void AutoIteratorAccess::decrement(JNIEnv *, void* iterator)
{
    m_decrement(iterator);
}

jboolean AutoIteratorAccess::lessThan(JNIEnv *, void* iterator, void* other)
{
    return m_lessThan(iterator, other);
}

bool AutoIteratorAccess::canLess()
{
    if(m_lessThan)
        return true;
    else return false;
}

jboolean AutoIteratorAccess::equals(JNIEnv *, void* iterator, void* other)
{
    return m_equals(iterator, other);
}


AutoBiIteratorAccess::~AutoBiIteratorAccess(){}
AutoBiIteratorAccess::AutoBiIteratorAccess(
        const InternalToExternalConverter& internalToExternalConverter,
        std::function<void(void*)>&& increment,
        std::function<void(void*)>&& decrement,
        std::function<const void*(void*)>&& value,
        std::function<bool(void*,void*)>&& lessThan,
        std::function<bool(void*,void*)>&& equals,
        const InternalToExternalConverter& keyInternalToExternalConverter,
        std::function<const void*(void*)>&& key
        )
    : AbstractBiIteratorAccess(),
      AutoIteratorAccess(internalToExternalConverter,
                        std::move(increment),
                        std::move(decrement),
                        std::move(value),
                        std::move(lessThan),
                        std::move(equals)),
      m_keyInternalToExternalConverter(keyInternalToExternalConverter),
      m_key(std::move(key))
{
    Q_ASSERT(m_key);
}

AbstractBiIteratorAccess* AutoBiIteratorAccess::clone(){
    return new AutoBiIteratorAccess(
                m_internalToExternalConverter,
                std::function<void(void*)>{m_increment},
                std::function<void(void*)>{m_decrement},
                std::function<const void*(void*)>{m_value},
                std::function<bool(void*,void*)>{m_lessThan},
                std::function<bool(void*,void*)>{m_equals},
                m_keyInternalToExternalConverter,
                std::function<const void*(void*)>{m_key});
}

jobject AutoBiIteratorAccess::key(JNIEnv * env, void* iterator){
    const void* v = m_key(iterator);
    jvalue jval;
    jval.l = nullptr;
    if(m_keyInternalToExternalConverter(env, nullptr, v, &jval, true))
        return jval.l;
    return nullptr;
}

jobject AutoBiIteratorAccess::value(JNIEnv * env, void* iterator){return AutoIteratorAccess::value(env, iterator);}
void AutoBiIteratorAccess::increment(JNIEnv * env, void* iterator){AutoIteratorAccess::increment(env, iterator);}
void AutoBiIteratorAccess::decrement(JNIEnv * env, void* iterator){AutoIteratorAccess::decrement(env, iterator);}
jboolean AutoBiIteratorAccess::lessThan(JNIEnv * env, void* iterator, void* other){return AutoIteratorAccess::lessThan(env, iterator, other);}
bool AutoBiIteratorAccess::canLess(){return AutoIteratorAccess::canLess();}
jboolean AutoBiIteratorAccess::equals(JNIEnv * env, void* iterator, void* other){return AutoIteratorAccess::equals(env, iterator, other);}
