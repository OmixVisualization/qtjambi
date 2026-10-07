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

#ifndef QTJAMBI_EVENT_H
#define QTJAMBI_EVENT_H

#include <QtCore/QByteArrayView>
#include "qtjambiapi.h"
#include "javaapi.h"

class QTJAMBI_EXPORT QNativeEvent{
private:
#if defined(QTJAMBI_GENERATOR_RUNNING)
    QNativeEvent();
    ~QNativeEvent();
    QNativeEvent(const QNativeEvent&);
public:
#else
    JNIEnv* m_env;
    jobject m_javaObject;
public:
    const QByteArray & m_eventType;
    void *m_message;
    qintptr *m_result;
    QNativeEvent(
        JNIEnv* env,
        const QByteArray & eventType,
        void *message,
        qintptr *result);
    ~QNativeEvent();
    operator jobject() const;
    static const QNativeEvent& fromJavaObject(JNIEnv* env, jobject object);
#endif
    QByteArrayView eventType() const;
    void setResult(qintptr result);
    bool acceptsResult() const;
};

#endif // QTJAMBI_EVENT_H
