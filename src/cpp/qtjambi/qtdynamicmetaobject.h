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

#ifndef QDYNAMICMETAOBJECT_H
#define QDYNAMICMETAOBJECT_H

#include "qtjambi_global.h"

#include <QtCore/QString>
#include <QtCore/QByteArray>
#include <QtCore/QMetaObject>

class QtDynamicMetaObjectPrivate;

class QTJAMBI_EXPORT QtDynamicMetaObject: public QMetaObject
{
public:
    QtDynamicMetaObject(JNIEnv *jni_env, jclass java_class, const QMetaObject *original_meta_object);
    ~QtDynamicMetaObject();

    int invokeSignalOrSlot(JNIEnv *env, jobject object, int _id, void **_a) const;
    int readProperty(JNIEnv *env, jobject object, int _id, void **_a) const;
    int writeProperty(JNIEnv *env, jobject object, int _id, void **_a) const;
    int resetProperty(JNIEnv *env, jobject object, int _id, void **_a) const;
    int notifyProperty(JNIEnv *env, jobject object, int _id, void **_a) const;
    int queryPropertyDesignable(JNIEnv *env, jobject object, int _id, void **_a) const;
    int queryPropertyScriptable(JNIEnv *env, jobject object, int _id, void **_a) const;

#if QT_VERSION >= 0x050000
    int invokeConstructor(JNIEnv *env, int _id, void **_a) const;
    int queryPropertyUser(JNIEnv *env, jobject object, int _id, void **_a) const;
    int queryPropertyStored(JNIEnv *env, jobject object, int _id, void **_a) const;
    int queryPropertyEditable(JNIEnv *env, jobject object, int _id, void **_a) const;

    QString javaClassName() const;
#endif

    int originalSignalOrSlotSignature(JNIEnv *env, int _id, QString *signature) const;

private:
    QtDynamicMetaObjectPrivate *d_ptr;
    Q_DECLARE_PRIVATE(QtDynamicMetaObject);
};

#endif // QDYNAMICMETAOBJECT_H
