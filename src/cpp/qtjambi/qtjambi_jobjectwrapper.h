/****************************************************************************
**
** Copyright (C) 1992-2009 Nokia. All rights reserved.
** Copyright (C) 2009-2020 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
**
** This file is part of Qt Jambi.
**
** ** $BEGIN_LICENSE$
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

#ifndef QOBJECTWRAPPER_H
#define QOBJECTWRAPPER_H

#include "qtjambi_global.h"
#include <QtCore/QtCore>

class JObjectWrapperData;

class QTJAMBI_EXPORT JObjectWrapper
{
public:
    JObjectWrapper();

    JObjectWrapper(const JObjectWrapper &wrapper);

    JObjectWrapper(JNIEnv *env, jobject obj, bool globalRefs = true);

    virtual ~JObjectWrapper();

    JObjectWrapper& operator=(const JObjectWrapper &wrapper);

    bool operator==(const JObjectWrapper &other) const;

    bool operator<(const JObjectWrapper &other) const;

    virtual QString toString() const;

    jobject object() const;
private:
    QSharedPointer<JObjectWrapperData> m_data;
};
Q_DECLARE_METATYPE(JObjectWrapper)

class QTJAMBI_EXPORT JEnumWrapper: public JObjectWrapper
{
public:
    JEnumWrapper(): JObjectWrapper() {}

    JEnumWrapper(JNIEnv *env, jobject obj): JObjectWrapper(env, obj) {}

    JEnumWrapper(const JEnumWrapper& enm): JObjectWrapper(enm) {}

    virtual ~JEnumWrapper() {}

    bool operator==(const JEnumWrapper &other) const;

    JEnumWrapper& operator=(const JObjectWrapper &wrapper);

    JEnumWrapper& operator=(const JEnumWrapper &wrapper);

    bool operator<(const JEnumWrapper &other) const;

    qint32 ordinal() const;

    operator qint32() const {return ordinal();}
};
Q_DECLARE_METATYPE(JEnumWrapper)

class QTJAMBI_EXPORT JIteratorWrapper: public JObjectWrapper
{
public:
    JIteratorWrapper(): JObjectWrapper() {}

    JIteratorWrapper(JNIEnv *env, jobject obj): JObjectWrapper(env, obj) {}

    JIteratorWrapper(const JIteratorWrapper& list): JObjectWrapper(list) {}

    virtual ~JIteratorWrapper() {}

    bool hasNext() const;

    QVariant next() const;

    bool operator<(const JIteratorWrapper &other) const;

    JIteratorWrapper& operator=(const JObjectWrapper &wrapper);

    JIteratorWrapper& operator=(const JIteratorWrapper &wrapper);

    bool operator==(const JIteratorWrapper &other) const;
private:
    jobject _next() const;

    friend class JCollectionWrapper;
    friend class JMapWrapper;
};
Q_DECLARE_METATYPE(JIteratorWrapper)

class QTJAMBI_EXPORT JCollectionWrapper: public JObjectWrapper
{
public:
    JCollectionWrapper(): JObjectWrapper() {}

    JCollectionWrapper(JNIEnv *env, jobject obj): JObjectWrapper(env, obj){}

    JCollectionWrapper(const JCollectionWrapper& list): JObjectWrapper(list) {}

    virtual ~JCollectionWrapper() {}

    int size() const;

    bool isEmpty() const { return size()==0; }

    JIteratorWrapper iterator() const;

    QList<QVariant> toList() const;

    QStringList toStringList(bool* ok = Q_NULLPTR) const;

    operator QList<QVariant>() const {return toList();}

    operator QStringList() const {return toStringList();}

    bool operator<(const JCollectionWrapper &other) const;

    bool operator==(const JCollectionWrapper &other) const;

    JCollectionWrapper& operator=(const JObjectWrapper &wrapper);

    JCollectionWrapper& operator=(const JCollectionWrapper &wrapper);
};
Q_DECLARE_METATYPE(JCollectionWrapper)

class QTJAMBI_EXPORT JMapWrapper: public JObjectWrapper
{
public:
    JMapWrapper(): JObjectWrapper() {}

    JMapWrapper(JNIEnv *env, jobject obj): JObjectWrapper(env, obj){}

    JMapWrapper(const JMapWrapper& map): JObjectWrapper(map) {}

    virtual ~JMapWrapper() {}

    JCollectionWrapper entrySet() const;

    QMap<QVariant,QVariant> toMap() const;

    QVariantMap toStringMap(bool* ok = Q_NULLPTR) const;

    QVariantHash toStringHash(bool* ok = Q_NULLPTR) const;

    operator QMap<QVariant,QVariant>() const {return toMap();}

    operator QVariantMap() const {return toStringMap();}

    operator QVariantHash() const {return toStringHash();}

    bool operator<(const JMapWrapper &other) const;

    bool operator==(const JMapWrapper &other) const;

    JMapWrapper& operator=(const JObjectWrapper &wrapper);

    JMapWrapper& operator=(const JMapWrapper &wrapper);

private:
    jobject _entrySet() const;
};
Q_DECLARE_METATYPE(JMapWrapper)

QTJAMBI_EXPORT JObjectWrapper qtjambi_to_jobjectwrapper(JNIEnv *env, jobject java_object);

QTJAMBI_EXPORT jobject qtjambi_from_jobjectwrapper(JNIEnv *env, const JObjectWrapper &wrapper);

QTJAMBI_EXPORT jobject qtjambi_from_destroyed_qobject(JNIEnv *env, QObject* object);

QTJAMBI_EXPORT uint qHash(const JIteratorWrapper &value);
QTJAMBI_EXPORT uint qHash(const JMapWrapper &value);
QTJAMBI_EXPORT uint qHash(const JCollectionWrapper &value);
QTJAMBI_EXPORT uint qHash(const JEnumWrapper &value);
QTJAMBI_EXPORT uint qHash(const JObjectWrapper &value);

#endif // QOBJECTWRAPPER_H
