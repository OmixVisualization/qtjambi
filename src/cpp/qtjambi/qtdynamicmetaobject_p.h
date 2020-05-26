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

#ifndef QTDYNAMICMETAOBJECT_P_H
#define QTDYNAMICMETAOBJECT_P_H

#include "qtjambi_core.h"
#include "qtjambitypemanager_p.h"
#include <QtCore>

typedef void (*StaticMetaCallFunction)(QObject *, QMetaObject::Call, int, void **);

class QtDynamicMetaObjectPrivate;

class QtDynamicMetaObject: public QMetaObject
{
public:
    QtDynamicMetaObject(JNIEnv *jni_env, jclass java_class, const QMetaObject *original_meta_object);

    int invokeSignalOrSlot(JNIEnv *env, jobject object, int _id, void **_a, bool direct = false) const;
    int readProperty(JNIEnv *env, jobject object, int _id, void **_a, bool direct = false) const;
    int writeProperty(JNIEnv *env, jobject object, int _id, void **_a, bool direct = false) const;
    int resetProperty(JNIEnv *env, jobject object, int _id, void **_a, bool direct = false) const;
    int notifyProperty(JNIEnv *env, jobject object, int _id, void **_a, bool direct = false) const;
    int queryPropertyDesignable(JNIEnv *env, jobject object, int _id, void **_a, bool direct = false) const;
    int queryPropertyScriptable(JNIEnv *env, jobject object, int _id, void **_a, bool direct = false) const;

#if QT_VERSION >= 0x050000
    int invokeConstructor(JNIEnv *env, int _id, void **_a) const;
    int queryPropertyUser(JNIEnv *env, jobject object, int _id, void **_a, bool direct = false) const;
    int queryPropertyStored(JNIEnv *env, jobject object, int _id, void **_a, bool direct = false) const;
    int queryPropertyEditable(JNIEnv *env, jobject object, int _id, void **_a, bool direct = false) const;
#endif
    int originalSignalOrSlotSignature(JNIEnv *env, int _id, QString *signature) const;

    jclass typeOfProperty(int index) const;
    jclass javaClass() const;
    static jweak javaInstance(const QtDynamicMetaObject* metaObject);
    static void setJavaInstance(const QtDynamicMetaObject* metaObject, jweak weak);
    static jclass javaClass(JNIEnv * env, const QMetaObject* metaObject, bool exactOrNull = false);
    static bool isDynamic(const QMetaObject* metaObject);
    static int methodFromJMethod(const QMetaObject* metaObject, jmethodID methodId);
    static const QVector<ParameterTypeInfo>* methodParameterInfo(JNIEnv * env, const QMetaMethod& method);
private:
    ~QtDynamicMetaObject();
    const QSharedPointer<const QtDynamicMetaObject>& thisPointer() const;
    QSharedPointer<const QtDynamicMetaObject> dispose() const;
    QtDynamicMetaObjectPrivate *d_ptr;
    Q_DECLARE_PRIVATE(QtDynamicMetaObject)
    Q_DISABLE_COPY_MOVE(QtDynamicMetaObject)
    friend void clear_metaobjects_at_shutdown();
    friend const QMetaObject *qtjambi_metaobject_for_class(JNIEnv *env, jclass object_class, const std::function<const QMetaObject *()>& original_meta_object_provider);
    friend class DynamicMetaObjectFunctionTable;
    friend class DynamicMetaObjectInterfaceFunctionTable;
    friend QtSharedPointer::CustomDeleter<QtDynamicMetaObject,QtSharedPointer::NormalDeleter>;
};

#endif // QTDYNAMICMETAOBJECT_P_H
