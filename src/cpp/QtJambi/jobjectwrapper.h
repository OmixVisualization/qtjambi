/****************************************************************************
**
** Copyright (C) 2009-2024 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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

    virtual ~JObjectWrapper();

    virtual JObjectWrapper& operator=(const JObjectWrapper &wrapper);

    virtual JObjectWrapper& operator=(JObjectWrapper &&wrapper);

    JObjectWrapper& operator=(jobject obj);

    bool operator==(const JObjectWrapper &other) const;

    bool operator==(jobject obj) const;

    bool operator<(const JObjectWrapper &other) const;

    bool operator<(jobject obj) const;

    QString toString(bool * ok = nullptr) const;

    virtual void assign(const JObjectWrapper &wrapper);

    virtual void assign(JNIEnv *env, JObjectWrapper &&wrapper);

    virtual void assign(JNIEnv *env, jobject obj);

    bool compareEqual(JNIEnv *env, const JObjectWrapper &other) const;

    bool compareEqual(JNIEnv *env, jobject obj) const;

    bool compareLess(JNIEnv *env, const JObjectWrapper &other) const;

    bool compareLess(JNIEnv *env, jobject obj) const;

    virtual QString toString(JNIEnv *env, bool * ok = nullptr) const;

    jobject object(JNIEnv *env) const;

    operator jobject() const;

    void clear(JNIEnv *env);
private:
    JObjectWrapper(JNIEnv *env, jobject obj, bool globalRefs, const std::type_info& typeId);
    void assign(JNIEnv* env, const JObjectWrapper& wrapper, const std::type_info& typeId);
    void assign(JNIEnv* env, JObjectWrapper&& wrapper, const std::type_info& typeId);
    jobject filterPrimitiveArray(JNIEnv *env, jobject object, const std::type_info& typeId);
    static bool isEquals(const std::type_info& typeId1, const std::type_info& typeId2);
    const void* array() const;
    void* array();
    void commitArray();
    jsize arrayLength() const;
    void swap(JObjectWrapper& other);
    QExplicitlySharedDataPointer<JObjectWrapperData> m_data;
    friend class JEnumWrapper;
    friend class JMapWrapper;
    friend class JCollectionWrapper;
    friend class JIteratorWrapper;
    friend class JObjectArrayWrapper;
    friend JArrayWrapper<jint>;
    friend JArrayWrapper<jlong>;
    friend JArrayWrapper<jbyte>;
    friend JArrayWrapper<jshort>;
    friend JArrayWrapper<jchar>;
    friend JArrayWrapper<jboolean>;
    friend JArrayWrapper<jdouble>;
    friend JArrayWrapper<jfloat>;
    friend JArrayAccessRef<jint>;
    friend JArrayAccessRef<jlong>;
    friend JArrayAccessRef<jbyte>;
    friend JArrayAccessRef<jshort>;
    friend JArrayAccessRef<jchar>;
    friend JArrayAccessRef<jboolean>;
    friend JArrayAccessRef<jdouble>;
    friend JArrayAccessRef<jfloat>;
    friend class RCSet;
    friend class RCMap;
    friend class RCMultiMap;
};
Q_DECLARE_METATYPE(JObjectWrapper)

class QTJAMBI_EXPORT JEnumWrapper: public JObjectWrapper
{
public:
    JEnumWrapper(): JObjectWrapper() {}

    JEnumWrapper(JNIEnv *env, jobject obj, bool globalRefs = true);

    JEnumWrapper(jobject obj);

    inline JEnumWrapper(const JEnumWrapper& enm): JObjectWrapper(enm) {}

    inline JEnumWrapper(const JObjectWrapper& wrapper): JObjectWrapper() {
        JEnumWrapper::operator=(wrapper);
    }

    inline JEnumWrapper(JObjectWrapper&& wrapper): JObjectWrapper() {
        JEnumWrapper::operator=(std::move(wrapper));
    }

    inline JEnumWrapper(JEnumWrapper&& wrapper): JObjectWrapper(std::move(wrapper)) {
    }

    inline ~JEnumWrapper()  override {}

    qint32 ordinal() const;

    qint32 ordinal(JNIEnv *env) const;

    inline operator qint32() const {return ordinal();}

    JEnumWrapper& operator=(const JEnumWrapper &wrapper);

    JEnumWrapper& operator=(JEnumWrapper &&wrapper);

    JEnumWrapper& operator=(const JObjectWrapper &wrapper) override;

    JEnumWrapper& operator=(JObjectWrapper &&wrapper) override;

    void assign(const JObjectWrapper &wrapper) override;

    void assign(JNIEnv *env, JObjectWrapper &&wrapper) override;

    void assign(JNIEnv *env, jobject obj) override;

    using JObjectWrapper::operator<;
    using JObjectWrapper::operator==;
    using JObjectWrapper::operator=;
    using JObjectWrapper::toString;
    using JObjectWrapper::compareEqual;
    using JObjectWrapper::compareLess;
    using JObjectWrapper::operator jobject;
};
Q_DECLARE_METATYPE(JEnumWrapper)

class QTJAMBI_EXPORT JIteratorWrapper: public JObjectWrapper
{
public:
    JIteratorWrapper(): JObjectWrapper() {}

    JIteratorWrapper(JNIEnv *env, jobject obj, bool globalRefs = true);

    JIteratorWrapper(jobject obj);

    inline JIteratorWrapper(const JIteratorWrapper& list): JObjectWrapper(list) {}

    inline JIteratorWrapper(const JObjectWrapper& wrapper): JObjectWrapper() {
        JIteratorWrapper::operator=(wrapper);
    }

    inline JIteratorWrapper(JObjectWrapper&& wrapper): JObjectWrapper() {
        JIteratorWrapper::operator=(std::move(wrapper));
    }

    inline JIteratorWrapper(JIteratorWrapper&& wrapper): JObjectWrapper(std::move(wrapper)) {
    }

    inline ~JIteratorWrapper() override {}

    JIteratorWrapper& operator=(const JIteratorWrapper &wrapper);

    JIteratorWrapper& operator=(JIteratorWrapper &&wrapper);

    JIteratorWrapper& operator=(const JObjectWrapper &wrapper) override;

    JIteratorWrapper& operator=(JObjectWrapper &&wrapper) override;

    void assign(const JObjectWrapper &wrapper) override;

    void assign(JNIEnv *env, JObjectWrapper &&wrapper) override;

    void assign(JNIEnv *env, jobject obj) override;

    bool hasNext() const;

    bool hasNext(JNIEnv *env) const;

    JObjectWrapper next() const;

    JObjectWrapper next(JNIEnv *env) const;

    using JObjectWrapper::operator==;
    using JObjectWrapper::operator<;
    using JObjectWrapper::operator=;
    using JObjectWrapper::toString;
    using JObjectWrapper::compareEqual;
    using JObjectWrapper::compareLess;
    using JObjectWrapper::operator jobject;
private:
    jobject _next(JNIEnv *env) const;

    friend class JCollectionWrapper;
    friend class JMapWrapper;
};
Q_DECLARE_METATYPE(JIteratorWrapper)

class QTJAMBI_EXPORT JCollectionWrapper: public JObjectWrapper
{
public:
    JCollectionWrapper(): JObjectWrapper() {}

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

    inline ~JCollectionWrapper() override {}

    int size() const;

    inline bool isEmpty() const { return size()==0; }

    JIteratorWrapper iterator() const;

    JIteratorWrapper iterator(JNIEnv *env) const;

    QList<QVariant> toList() const;

    QList<QVariant> toList(JNIEnv *env) const;

    QStringList toStringList(bool* ok = Q_NULLPTR) const;

    QStringList toStringList(JNIEnv *env, bool* ok = Q_NULLPTR) const;

    JCollectionWrapper& operator=(const JCollectionWrapper &wrapper);

    JCollectionWrapper& operator=(JCollectionWrapper &&wrapper);

    JCollectionWrapper& operator=(const JObjectWrapper &wrapper) override;

    JCollectionWrapper& operator=(JObjectWrapper &&wrapper) override;

    void assign(const JObjectWrapper &wrapper) override;

    void assign(JNIEnv *env, JObjectWrapper &&wrapper) override;

    void assign(JNIEnv *env, jobject obj) override;

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
    JMapWrapper(): JObjectWrapper() {}

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

    inline ~JMapWrapper() override {}

    JCollectionWrapper entrySet() const;

    QMap<QVariant,QVariant> toMap() const;

    QVariantMap toStringMap(bool* ok = Q_NULLPTR) const;

    QVariantHash toStringHash(bool* ok = Q_NULLPTR) const;

    inline operator QMap<QVariant,QVariant>() const {return toMap();}

    inline operator QVariantMap() const {return toStringMap();}

    inline operator QVariantHash() const {return toStringHash();}

    JMapWrapper& operator=(const JMapWrapper &wrapper);

    JMapWrapper& operator=(JMapWrapper &&wrapper);

    JMapWrapper& operator=(const JObjectWrapper &wrapper) override;

    JMapWrapper& operator=(JObjectWrapper &&wrapper) override;

    void assign(const JObjectWrapper &wrapper) override;

    void assign(JNIEnv *env, JObjectWrapper &&wrapper) override;

    void assign(JNIEnv *env, jobject obj) override;

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

class QTJAMBI_EXPORT JObjectWrapperRef{
public:
    JObjectWrapperRef& operator=(const JObjectWrapper &newValue);
    JObjectWrapperRef& operator=(jobject newValue);
    operator JObjectWrapper() const;
    operator jobject() const;
private:
    JObjectWrapperRef(const JObjectWrapper& arrayWrapper, jsize index);
    JObjectWrapper m_arrayWrapper;
    jsize m_index;
    friend class JObjectArrayWrapper;
};

class QTJAMBI_EXPORT JObjectArrayWrapper: public JObjectWrapper
{
public:
    inline JObjectArrayWrapper(): JObjectWrapper() {}

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

    inline ~JObjectArrayWrapper() override {}

    jsize length() const;

    jobject at(JNIEnv *env, jsize index) const;

    JObjectWrapper operator[](jsize index) const;

    JObjectWrapperRef operator[](jsize index);

    QString toString(JNIEnv *env, bool * ok = nullptr) const override;

    inline jobjectArray object(JNIEnv *env) const { return jobjectArray(JObjectWrapper::object(env)); }

    inline operator jobjectArray() const{ return jobjectArray(operator jobject()); }

    JObjectArrayWrapper& operator=(const JObjectArrayWrapper &wrapper);

    JObjectArrayWrapper& operator=(JObjectArrayWrapper &&wrapper);

    JObjectArrayWrapper& operator=(const JObjectWrapper &wrapper) override;

    JObjectArrayWrapper& operator=(JObjectWrapper &&wrapper) override;

    void assign(const JObjectWrapper &wrapper) override;

    void assign(JNIEnv *env, JObjectWrapper &&wrapper) override;

    void assign(JNIEnv *env, jobject obj) override;

    using JObjectWrapper::operator==;
    using JObjectWrapper::operator<;
    using JObjectWrapper::operator=;
    using JObjectWrapper::toString;
    using JObjectWrapper::compareEqual;
    using JObjectWrapper::compareLess;
};
Q_DECLARE_METATYPE(JObjectArrayWrapper)

template<typename JType>
struct JArray{
};

template<>
struct JArray<jint>{
    typedef jintArray Type;
    constexpr static auto GetArrayElements = &JNIEnv::GetIntArrayElements;
    constexpr static auto ReleaseArrayElements = &JNIEnv::ReleaseIntArrayElements;
};

template<>
struct JArray<jbyte>{
    typedef jbyteArray Type;
    constexpr static auto GetArrayElements = &JNIEnv::GetByteArrayElements;
    constexpr static auto ReleaseArrayElements = &JNIEnv::ReleaseByteArrayElements;
};

template<>
struct JArray<jlong>{
    typedef jlongArray Type;
    constexpr static auto GetArrayElements = &JNIEnv::GetLongArrayElements;
    constexpr static auto ReleaseArrayElements = &JNIEnv::ReleaseLongArrayElements;
};

template<>
struct JArray<jshort>{
    typedef jshortArray Type;
    constexpr static auto GetArrayElements = &JNIEnv::GetShortArrayElements;
    constexpr static auto ReleaseArrayElements = &JNIEnv::ReleaseShortArrayElements;
};

template<>
struct JArray<jchar>{
    typedef jcharArray Type;
    constexpr static auto GetArrayElements = &JNIEnv::GetCharArrayElements;
    constexpr static auto ReleaseArrayElements = &JNIEnv::ReleaseCharArrayElements;
};

template<>
struct JArray<jboolean>{
    typedef jbooleanArray Type;
    constexpr static auto GetArrayElements = &JNIEnv::GetBooleanArrayElements;
    constexpr static auto ReleaseArrayElements = &JNIEnv::ReleaseBooleanArrayElements;
};

template<>
struct JArray<jfloat>{
    typedef jfloatArray Type;
    constexpr static auto GetArrayElements = &JNIEnv::GetFloatArrayElements;
    constexpr static auto ReleaseArrayElements = &JNIEnv::ReleaseFloatArrayElements;
};

template<>
struct JArray<jdouble>{
    typedef jdoubleArray Type;
    constexpr static auto GetArrayElements = &JNIEnv::GetDoubleArrayElements;
    constexpr static auto ReleaseArrayElements = &JNIEnv::ReleaseDoubleArrayElements;
};

template<typename JType> class JArrayWrapper;

template<typename JType>
class JArrayAccessRef{
    typedef typename JArray<JType>::Type ArrayType;
public:
    JArrayAccessRef& operator=(JType newValue){
        if(m_array){
            m_array[m_index] = newValue;
            m_arrayWrapper.commitArray();
            m_array = reinterpret_cast<JType*>(m_arrayWrapper.array());
        }
        return *this;
    }

    operator JType() const
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
          m_array(reinterpret_cast<JType*>(m_arrayWrapper.array()))
    { }

    JObjectWrapper m_arrayWrapper;
    jsize m_index;
    JType* m_array;
    friend JArrayWrapper<JType>;
};

template<typename JType>
class QTJAMBI_EXPORT JArrayWrapper: public JObjectWrapper
{
    typedef typename JArray<JType>::Type ArrayType;
public:
    JArrayWrapper();
    JArrayWrapper(JNIEnv *env, ArrayType obj, bool globalRefs = true);
    JArrayWrapper(ArrayType obj);
    JArrayWrapper(const JArrayWrapper& other);
    JArrayWrapper(JArrayWrapper&& other);
    JArrayWrapper(const JObjectWrapper& wrapper);
    JArrayWrapper(JObjectWrapper&& wrapper);
    ~JArrayWrapper() override;
    jsize length() const;
    const JType* array() const;
    JType* array();
    operator const JType*() const;
    operator JType*();
    JType operator[](jsize index) const;
    JArrayAccessRef<JType> operator[](jsize index);
    QString toString(JNIEnv *env, bool * ok = nullptr) const override;
    ArrayType object(JNIEnv *env) const;
    operator ArrayType() const;
    JArrayWrapper& operator=(const JArrayWrapper &wrapper);
    JArrayWrapper& operator=(JArrayWrapper &&wrapper);
    JArrayWrapper& operator=(const JObjectWrapper &wrapper) override;
    JArrayWrapper& operator=(JObjectWrapper &&wrapper) override;
    void assign(const JObjectWrapper &wrapper) override;
    void assign(JNIEnv *env, JObjectWrapper &&wrapper) override;
    void assign(JNIEnv *env, jobject object) override;
    using JObjectWrapper::operator=;
    using JObjectWrapper::operator==;
    using JObjectWrapper::operator<;
    using JObjectWrapper::toString;
    using JObjectWrapper::compareEqual;
    using JObjectWrapper::compareLess;
};

typedef JArrayWrapper<jint> JIntArrayWrapper;
Q_DECLARE_METATYPE(JIntArrayWrapper)

typedef JArrayWrapper<jlong> JLongArrayWrapper;
Q_DECLARE_METATYPE(JLongArrayWrapper)

typedef JArrayWrapper<jshort> JShortArrayWrapper;
Q_DECLARE_METATYPE(JShortArrayWrapper)

typedef JArrayWrapper<jbyte> JByteArrayWrapper;
Q_DECLARE_METATYPE(JByteArrayWrapper)

typedef JArrayWrapper<jboolean> JBooleanArrayWrapper;
Q_DECLARE_METATYPE(JBooleanArrayWrapper)

typedef JArrayWrapper<jchar> JCharArrayWrapper;
Q_DECLARE_METATYPE(JCharArrayWrapper)

typedef JArrayWrapper<jfloat> JFloatArrayWrapper;
Q_DECLARE_METATYPE(JFloatArrayWrapper)

typedef JArrayWrapper<jdouble> JDoubleArrayWrapper;
Q_DECLARE_METATYPE(JDoubleArrayWrapper)

QTJAMBI_EXPORT hash_type qHash(const JObjectWrapper &value, hash_type seed = 0);

inline hash_type qHash(const JObjectArrayWrapper &value, hash_type seed = 0){
    return qHash(static_cast<const JObjectWrapper &>(value), seed);
}

inline hash_type qHash(const JIteratorWrapper &value, hash_type seed = 0){
    return qHash(static_cast<const JObjectWrapper &>(value), seed);
}

inline hash_type qHash(const JMapWrapper &value, hash_type seed = 0){
    return qHash(static_cast<const JObjectWrapper &>(value), seed);
}

inline hash_type qHash(const JCollectionWrapper &value, hash_type seed = 0){
    return qHash(static_cast<const JObjectWrapper &>(value), seed);
}

inline hash_type qHash(const JEnumWrapper &value, hash_type seed = 0){
    return qHash(static_cast<const JObjectWrapper &>(value), seed);
}

template<typename JType>
inline hash_type qHash(const JArrayWrapper<JType> &value, hash_type seed = 0){
    return qHash(static_cast<const JObjectWrapper &>(value), seed);
}

QTJAMBI_EXPORT QDataStream &operator<<(QDataStream &out, const JObjectWrapper &myObj);
QTJAMBI_EXPORT QDataStream &operator>>(QDataStream &in, JObjectWrapper &myObj);
QTJAMBI_EXPORT QDebug operator<<(QDebug out, const JObjectWrapper &myObj);

inline QDataStream &operator<<(QDataStream &out, const JEnumWrapper &myObj){
    return out << static_cast<const JObjectWrapper &>(myObj);
}

inline QDataStream &operator>>(QDataStream &in, JEnumWrapper &myObj){
    return in >> static_cast<JObjectWrapper &>(myObj);
}

inline QDebug operator<<(QDebug out, const JEnumWrapper &myObj){
    return out << static_cast<const JObjectWrapper &>(myObj);
}

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

inline QDataStream &operator<<(QDataStream &out, const JIteratorWrapper &myObj){
    return out << static_cast<const JObjectWrapper &>(myObj);
}

inline QDataStream &operator>>(QDataStream &in, JIteratorWrapper &myObj){
    return in >> static_cast<JObjectWrapper &>(myObj);
}

inline QDebug operator<<(QDebug out, const JIteratorWrapper &myObj){
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
