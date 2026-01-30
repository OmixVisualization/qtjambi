/****************************************************************************
**
** Copyright (C) 2009-2026 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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

#ifndef QTJAMBI_JOBJECTWRAPPER_H
#define QTJAMBI_JOBJECTWRAPPER_H

#include <QtCore/QVariant>
#include <QtCore/QSharedPointer>
#include <QtCore/QList>
#include <QtCore/QStringList>
#include <QtCore/QDebug>
#include <typeinfo>
#include "jnienvironment.h"
#include "qtjambi_cast_impl_util.h"

class JObjectWrapperData;

template<typename JType>
class JArrayWrapper;
template<typename JType>
class JArrayAccessRef;

class QTJAMBI_EXPORT JObjectWrapper
{
public:
    JObjectWrapper();

    JObjectWrapper(const JObjectWrapper &wrapper);

    JObjectWrapper(JObjectWrapper &&wrapper);

    JObjectWrapper(jobject obj);

    JObjectWrapper(JNIEnv *env, jobject obj, bool globalRefs = true);

    ~JObjectWrapper();

    JObjectWrapper& operator=(const JObjectWrapper &wrapper);

    JObjectWrapper& operator=(JObjectWrapper &&wrapper);

    JObjectWrapper& operator=(jobject obj);

    bool operator==(const JObjectWrapper &other) const;

    bool operator==(jobject obj) const;

    inline bool operator==(std::nullptr_t) const { return isNull(); }

    inline bool operator!=(const JObjectWrapper &other) const { return !operator==(other); }

    inline bool operator!=(jobject obj) const { return !operator==(obj); }

    inline bool operator!=(std::nullptr_t) const { return !isNull(); }

    inline bool operator!() const { return isNull(); }

    inline operator bool () const { return !isNull(); }

    bool operator<(const JObjectWrapper &other) const;

    bool operator<(jobject obj) const;

    QString toString(bool * ok = nullptr) const;

    void assign(const JObjectWrapper &wrapper);

    void assign(JObjectWrapper &&wrapper);

    void assign(JNIEnv *env, jobject obj, bool globalRefs = true);

    bool compareEqual(JNIEnv *env, const JObjectWrapper &other) const;

    bool compareEqual(JNIEnv *env, jobject obj) const;

    bool compareLess(JNIEnv *env, const JObjectWrapper &other) const;

    bool compareLess(JNIEnv *env, jobject obj) const;

    QString toString(JNIEnv *env, bool * ok = nullptr) const;

    jobject object(JNIEnv *env) const;

    template<typename T>
    T typedObject(JNIEnv *env) const {
        return static_cast<T>(object(env));
    }

    operator jobject() const;

    bool isNull() const;

    template<typename T>
    operator T() const{
        if constexpr(std::is_same<T,bool>::value)
            return !isNull();
        else
            return static_cast<T>(operator jobject());
    }

    void clear(JNIEnv *env);

    bool isInstanceOf(JNIEnv *env, jclass cls) const;
protected:
    JObjectWrapper(JObjectWrapperData* data);
private:
    static bool isEquals(const std::type_info& typeId1, const std::type_info& typeId2);
    const void* array() const;
    void* array();
    void commitArray();
    jsize arrayLength() const;
    void swap(JObjectWrapper& other);
    QExplicitlySharedDataPointer<JObjectWrapperData> m_data;
    friend JObjectWrapperData;
    friend class JMapWrapper;
    friend class JCollectionWrapper;
    friend class JObjectArrayWrapper;
    friend JArrayWrapper<jintArray>;
    friend JArrayWrapper<jlongArray>;
    friend JArrayWrapper<jbyteArray>;
    friend JArrayWrapper<jshortArray>;
    friend JArrayWrapper<jcharArray>;
    friend JArrayWrapper<jbooleanArray>;
    friend JArrayWrapper<jdoubleArray>;
    friend JArrayWrapper<jfloatArray>;
    friend JArrayAccessRef<jintArray>;
    friend JArrayAccessRef<jlongArray>;
    friend JArrayAccessRef<jbyteArray>;
    friend JArrayAccessRef<jshortArray>;
    friend JArrayAccessRef<jcharArray>;
    friend JArrayAccessRef<jbooleanArray>;
    friend JArrayAccessRef<jdoubleArray>;
    friend JArrayAccessRef<jfloatArray>;
    friend class RCSet;
    friend class RCMap;
    friend class RCMultiMap;
    friend class JObjectWrapperRef;
};
Q_DECLARE_METATYPE(JObjectWrapper)

inline bool operator&&(bool a, const JObjectWrapper& b){
    return a && !b.isNull();
}

inline bool operator||(bool a, const JObjectWrapper& b){
    return a || !b.isNull();
}

inline bool operator&&(const JObjectWrapper& a, const JObjectWrapper& b){
    return !a.isNull() && !b.isNull();
}

inline bool operator||(const JObjectWrapper& a, const JObjectWrapper& b){
    return !a.isNull() || !b.isNull();
}

inline bool operator&&(const JObjectWrapper& a, bool b){
    return !a.isNull() && b;
}

inline bool operator||(const JObjectWrapper& a, bool b){
    return !a.isNull() || b;
}

class QTJAMBI_EXPORT JObjectWrapperRef{
public:
    void assign(JNIEnv *env, const JObjectWrapper &newValue);
    void assign(JNIEnv *env, jobject newValue);
    JObjectWrapperRef& operator=(const JObjectWrapper &newValue);
    JObjectWrapperRef& operator=(jobject newValue);
    operator JObjectWrapper() const;
    operator jobject() const;
private:
    JObjectWrapperRef(const JObjectWrapper& containerWrapper, jsize index);
    JObjectWrapper m_containerWrapper;
    jsize m_index;
    friend class JObjectArrayWrapper;
    friend class JCollectionWrapper;
};

class QTJAMBI_EXPORT JCollectionWrapper: public JObjectWrapper
{
public:
    JCollectionWrapper();

    JCollectionWrapper(JNIEnv *env, jobject obj, bool globalRefs = true);

    JCollectionWrapper(jobject obj);

    inline JCollectionWrapper(const JCollectionWrapper& list): JObjectWrapper(list) {}

    inline JCollectionWrapper(const JObjectWrapper& wrapper): JObjectWrapper() {
        JCollectionWrapper::operator=(wrapper);
    }

    inline JCollectionWrapper(JObjectWrapper&& wrapper): JObjectWrapper() {
        JCollectionWrapper::operator=(std::move(wrapper));
    }

    inline JCollectionWrapper(JCollectionWrapper&& wrapper): JObjectWrapper(std::move(wrapper)) {}

    inline ~JCollectionWrapper() {}

    int size() const;

    jobject at(JNIEnv *env, jsize index) const;

    JObjectWrapper operator[](jsize index) const;

    JObjectWrapperRef operator[](jsize index);

    inline bool isEmpty() const { return size()==0; }

    QList<QVariant> toList() const;

    QList<QVariant> toList(JNIEnv *env) const;

    QStringList toStringList(bool* ok = Q_NULLPTR) const;

    QStringList toStringList(JNIEnv *env, bool* ok = Q_NULLPTR) const;

    JCollectionWrapper& operator=(const JCollectionWrapper &wrapper);

    JCollectionWrapper& operator=(JCollectionWrapper &&wrapper);

    JCollectionWrapper& operator=(const JObjectWrapper &wrapper);

    JCollectionWrapper& operator=(JObjectWrapper &&wrapper);

    void assign(const JObjectWrapper &wrapper);

    void assign(JObjectWrapper &&wrapper);

    void assign(JNIEnv *env, jobject obj, bool globalRefs = true);

    inline operator QList<QVariant>() const {return toList();}

    inline operator QStringList() const {return toStringList();}

    using JObjectWrapper::operator==;
    using JObjectWrapper::operator<;
    using JObjectWrapper::operator=;
    using JObjectWrapper::toString;
    using JObjectWrapper::compareEqual;
    using JObjectWrapper::compareLess;
    using JObjectWrapper::operator jobject;
};
Q_DECLARE_METATYPE(JCollectionWrapper)

class QTJAMBI_EXPORT JMapWrapper: public JObjectWrapper
{
public:
    JMapWrapper();

    JMapWrapper(JNIEnv *env, jobject obj, bool globalRefs = true);

    JMapWrapper(jobject obj);

    inline JMapWrapper(const JMapWrapper& map): JObjectWrapper(map) {}

    inline JMapWrapper(const JObjectWrapper& wrapper): JObjectWrapper() {
        JMapWrapper::operator=(wrapper);
    }

    inline JMapWrapper(JObjectWrapper&& wrapper): JObjectWrapper() {
        JMapWrapper::operator=(std::move(wrapper));
    }

    inline JMapWrapper(JMapWrapper&& wrapper): JObjectWrapper(std::move(wrapper)) {
    }

    inline ~JMapWrapper() {}

    JCollectionWrapper entrySet() const;

    QMap<QVariant,QVariant> toMap() const;

    QVariantMap toStringMap(bool* ok = Q_NULLPTR) const;

    QVariantHash toStringHash(bool* ok = Q_NULLPTR) const;

    inline operator QMap<QVariant,QVariant>() const {return toMap();}

    inline operator QVariantMap() const {return toStringMap();}

    inline operator QVariantHash() const {return toStringHash();}

    JMapWrapper& operator=(const JMapWrapper &wrapper);

    JMapWrapper& operator=(JMapWrapper &&wrapper);

    JMapWrapper& operator=(const JObjectWrapper &wrapper);

    JMapWrapper& operator=(JObjectWrapper &&wrapper);

    void assign(const JObjectWrapper &wrapper);

    void assign(JObjectWrapper &&wrapper);

    void assign(JNIEnv *env, jobject obj, bool globalRefs = true);

    using JObjectWrapper::operator==;
    using JObjectWrapper::operator<;
    using JObjectWrapper::operator=;
    using JObjectWrapper::toString;
    using JObjectWrapper::compareEqual;
    using JObjectWrapper::compareLess;
    using JObjectWrapper::operator jobject;
private:
    jobject _entrySet(JNIEnv* env) const;
};
Q_DECLARE_METATYPE(JMapWrapper)

class QTJAMBI_EXPORT JObjectArrayWrapper: public JObjectWrapper
{
public:
    JObjectArrayWrapper();

    JObjectArrayWrapper(JNIEnv *env, jobjectArray obj, bool globalRefs = true);

    JObjectArrayWrapper(jobjectArray obj);

    inline JObjectArrayWrapper(const JObjectArrayWrapper& other): JObjectWrapper(other) {}

    inline JObjectArrayWrapper(JObjectArrayWrapper&& other): JObjectWrapper(std::move(other)) {}

    inline JObjectArrayWrapper(const JObjectWrapper& wrapper): JObjectWrapper() {
        JObjectArrayWrapper::operator=(wrapper);
    }

    inline JObjectArrayWrapper(JObjectWrapper&& wrapper): JObjectWrapper() {
        JObjectArrayWrapper::operator=(std::move(wrapper));
    }

    inline ~JObjectArrayWrapper() {}

    jsize length() const;

    jobject at(JNIEnv *env, jsize index) const;

    JObjectWrapper operator[](jsize index) const;

    JObjectWrapperRef operator[](jsize index);

    inline jobjectArray object(JNIEnv *env) const { return jobjectArray(JObjectWrapper::object(env)); }

    inline operator jobjectArray() const{ return jobjectArray(operator jobject()); }

    JObjectArrayWrapper& operator=(const JObjectArrayWrapper &wrapper);

    JObjectArrayWrapper& operator=(JObjectArrayWrapper &&wrapper);

    JObjectArrayWrapper& operator=(const JObjectWrapper &wrapper);

    JObjectArrayWrapper& operator=(JObjectWrapper &&wrapper);

    JObjectArrayWrapper& operator=(jobject obj);

    JObjectArrayWrapper& operator=(jobjectArray obj);

    void assign(const JObjectArrayWrapper &wrapper);

    void assign(JObjectArrayWrapper &&wrapper);

    void assign(const JObjectWrapper &wrapper);

    void assign(JObjectWrapper &&wrapper);

    void assign(JNIEnv *env, jobject obj, bool globalRefs = true);

    void assign(JNIEnv *env, jobjectArray obj, bool globalRefs = true);

    using JObjectWrapper::operator==;
    using JObjectWrapper::operator<;
    using JObjectWrapper::operator=;
    using JObjectWrapper::toString;
    using JObjectWrapper::compareEqual;
    using JObjectWrapper::compareLess;
private:
};
Q_DECLARE_METATYPE(JObjectArrayWrapper)

template<typename ArrayType> class JArrayWrapper;

template<typename ArrayType>
class JArrayAccessRef{
    typedef typename QtJambiPrivate::jni_type<ArrayType>::ElementType ElementType;
public:
    JArrayAccessRef& operator=(ElementType newValue){
        if(m_array){
            m_array[m_index] = newValue;
            m_arrayWrapper.commitArray();
            m_array = reinterpret_cast<ElementType*>(m_arrayWrapper.array());
        }
        return *this;
    }

    operator ElementType() const
    {
        if(m_array){
            return m_array[m_index];
        }
        return 0;
    }

    ~JArrayAccessRef(){
    }
private:
    JArrayAccessRef(const JObjectWrapper& arrayWrapper, jsize index)
        : m_arrayWrapper(arrayWrapper),
          m_index(index),
          m_array(reinterpret_cast<ElementType*>(m_arrayWrapper.array()))
    { }

    JObjectWrapper m_arrayWrapper;
    jsize m_index;
    ElementType* m_array;
    friend JArrayWrapper<ArrayType>;
};

template<typename ArrayType>
class QTJAMBI_EXPORT JArrayWrapper: public JObjectWrapper
{
    typedef typename QtJambiPrivate::jni_type<ArrayType>::ElementType ElementType;
public:
    JArrayWrapper();
    JArrayWrapper(JNIEnv *env, ArrayType obj, bool globalRefs = true);
    JArrayWrapper(ArrayType obj);
    JArrayWrapper(const JArrayWrapper& other);
    JArrayWrapper(JArrayWrapper&& other);
    JArrayWrapper(const JObjectWrapper& wrapper);
    JArrayWrapper(JObjectWrapper&& wrapper);
    ~JArrayWrapper();
    jsize length() const;
    const ElementType* array() const;
    ElementType* array();
    operator const ElementType*() const;
    operator ElementType*();
    ElementType operator[](jsize index) const;
    JArrayAccessRef<ArrayType> operator[](jsize index);
    ArrayType object(JNIEnv *env) const;
    operator ArrayType() const;
    JArrayWrapper& operator=(const JArrayWrapper &wrapper);
    JArrayWrapper& operator=(JArrayWrapper &&wrapper);
    JArrayWrapper& operator=(const JObjectWrapper &wrapper);
    JArrayWrapper& operator=(JObjectWrapper &&wrapper);
    JArrayWrapper& operator=(jobject object);
    JArrayWrapper& operator=(ArrayType object);
    void assign(const JArrayWrapper &wrapper);
    void assign(JArrayWrapper &&wrapper);
    void assign(const JObjectWrapper &wrapper);
    void assign(JObjectWrapper &&wrapper);
    void assign(JNIEnv *env, jobject object, bool globalRefs = true);
    void assign(JNIEnv *env, ArrayType object, bool globalRefs = true);
    using JObjectWrapper::operator==;
    using JObjectWrapper::operator<;
    using JObjectWrapper::toString;
    using JObjectWrapper::compareEqual;
    using JObjectWrapper::compareLess;
};

typedef JArrayWrapper<jintArray> JIntArrayWrapper;
Q_DECLARE_METATYPE(JIntArrayWrapper)

typedef JArrayWrapper<jlongArray> JLongArrayWrapper;
Q_DECLARE_METATYPE(JLongArrayWrapper)

typedef JArrayWrapper<jshortArray> JShortArrayWrapper;
Q_DECLARE_METATYPE(JShortArrayWrapper)

typedef JArrayWrapper<jbyteArray> JByteArrayWrapper;
Q_DECLARE_METATYPE(JByteArrayWrapper)

typedef JArrayWrapper<jbooleanArray> JBooleanArrayWrapper;
Q_DECLARE_METATYPE(JBooleanArrayWrapper)

typedef JArrayWrapper<jcharArray> JCharArrayWrapper;
Q_DECLARE_METATYPE(JCharArrayWrapper)

typedef JArrayWrapper<jfloatArray> JFloatArrayWrapper;
Q_DECLARE_METATYPE(JFloatArrayWrapper)

typedef JArrayWrapper<jdoubleArray> JDoubleArrayWrapper;
Q_DECLARE_METATYPE(JDoubleArrayWrapper)

QTJAMBI_EXPORT size_t qHash(const JObjectWrapper &value, size_t seed = 0);

inline size_t qHash(const JObjectArrayWrapper &value, size_t seed = 0){
    return qHash(static_cast<const JObjectWrapper &>(value), seed);
}

inline size_t qHash(const JMapWrapper &value, size_t seed = 0){
    return qHash(static_cast<const JObjectWrapper &>(value), seed);
}

inline size_t qHash(const JCollectionWrapper &value, size_t seed = 0){
    return qHash(static_cast<const JObjectWrapper &>(value), seed);
}

template<typename JType>
inline size_t qHash(const JArrayWrapper<JType> &value, size_t seed = 0){
    return qHash(static_cast<const JObjectWrapper &>(value), seed);
}

QTJAMBI_EXPORT QDataStream &operator<<(QDataStream &out, const JObjectWrapper &myObj);
QTJAMBI_EXPORT QDataStream &operator>>(QDataStream &in, JObjectWrapper &myObj);
QTJAMBI_EXPORT QDebug operator<<(QDebug out, const JObjectWrapper &myObj);

inline QDataStream &operator<<(QDataStream &out, const JMapWrapper &myObj){
    return out << static_cast<const JObjectWrapper &>(myObj);
}

inline QDataStream &operator>>(QDataStream &in, JMapWrapper &myObj){
    return in >> static_cast<JObjectWrapper &>(myObj);
}

inline QDebug operator<<(QDebug out, const JMapWrapper &myObj){
    return out << static_cast<const JObjectWrapper &>(myObj);
}

inline QDataStream &operator<<(QDataStream &out, const JCollectionWrapper &myObj){
    return out << static_cast<const JObjectWrapper &>(myObj);
}

inline QDataStream &operator>>(QDataStream &in, JCollectionWrapper &myObj){
    return in >> static_cast<JObjectWrapper &>(myObj);
}

inline QDebug operator<<(QDebug out, const JCollectionWrapper &myObj){
    return out << static_cast<const JObjectWrapper &>(myObj);
}

template<typename JType>
inline QDataStream &operator<<(QDataStream &out, const JArrayWrapper<JType> &myObj){
    return out << static_cast<const JObjectWrapper &>(myObj);
}

template<typename JType>
inline QDataStream &operator>>(QDataStream &in, JArrayWrapper<JType> &myObj){
    return in >> static_cast<JObjectWrapper &>(myObj);
}

template<typename JType>
inline QDebug operator<<(QDebug out, const JArrayWrapper<JType> &myObj){
    return out << static_cast<const JObjectWrapper &>(myObj);
}

#endif // QTJAMBI_JOBJECTWRAPPER_H
