/****************************************************************************
**
** Copyright (C) 2009-2022 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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

#if !defined(QTJAMBI_JAVAENVIRONMENT_H) && !defined(QTJAMBI_GENERATOR_RUNNING)
#define QTJAMBI_JAVAENVIRONMENT_H

#include "global.h"

enum class jValueType {
    z = 1,
    b,
    c,
    s,
    i,
    j,
    f,
    d,
    l,
};

class QTJAMBI_EXPORT JniEnvironment{
public:
    JniEnvironment(int capacity = 0);
    JniEnvironment(JniEnvironment&& other);
    ~JniEnvironment();
    operator bool() const;
    operator JNIEnv *() const;
    JNIEnv * environment() const;
    JNIEnv *operator->() const;
protected:
    JniEnvironment(bool,int capacity);
private:
    JNIEnv *m_env;
    bool m_hasLocalFrames;
};

class QTJAMBI_EXPORT JniLocalFrame{
public:
    JniLocalFrame(JNIEnv *env, int capacity);
    ~JniLocalFrame();
private:
    JNIEnv *m_env;
    Q_DISABLE_COPY_MOVE(JniLocalFrame)
};

#define QTJAMBI_JNI_LOCAL_FRAME(env, capacity)\
    JniLocalFrame __jniLocalFrame(env, capacity)

#endif // QTJAMBI_JAVAENVIRONMENT_H
