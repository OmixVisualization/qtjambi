/****************************************************************************
**
** Copyright (C) 2009-2023 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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

#if !defined(QTJAMBI_SCOPE_H) && !defined(QTJAMBI_GENERATOR_RUNNING)
#define QTJAMBI_SCOPE_H

#include "global.h"
#include <functional>

class QtJambiShell;
class QtJambiScopePrivate;

enum class QtJambiNativeID : jlong;

class QTJAMBI_EXPORT QtJambiScope{
public:
    QtJambiScope();
    QtJambiScope(QtJambiShell* shell);
    QtJambiScope(QtJambiNativeID nativeId);
    QtJambiScope(JNIEnv *env, jobject object);
    ~QtJambiScope();
    void addFinalAction(std::function<void()>&& action);
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    void addDeletion(int metaTypeId, void* pointer);
#else
    void addDeletion(QMetaType metaType, void* pointer);
#endif
    template<typename T>
    void addDeletion(T* pointer){
        addFinalAction([pointer](){delete pointer;});
    }
    template<typename T>
    void addArrayDeletion(T* pointer){
        addFinalAction([pointer](){delete[] pointer;});
    }
    void addObjectInvalidation(JNIEnv *env, jobject object, bool checkJavaOwnership, bool persistent = true);
    QtJambiNativeID relatedNativeID() const;
protected:
    QtJambiScope(QtJambiScopePrivate&);
private:
    QtJambiScopePrivate* d;
    Q_DISABLE_COPY_MOVE(QtJambiScope)
};

class QTJAMBI_EXPORT DoFinally{
public:
    DoFinally(std::function<void()> action);
    ~DoFinally();
private:
    std::function<void()> action;
    Q_DISABLE_COPY_MOVE(DoFinally)
};

#define DO_FINALLY(expr)\
    DoFinally __qt_do_finally([=](){expr});\
    Q_UNUSED(__qt_do_finally)

#endif // QTJAMBI_SCOPE_H
