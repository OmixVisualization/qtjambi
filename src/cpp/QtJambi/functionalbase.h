/****************************************************************************
**
** Copyright (C) 2009-2024 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
**
** This file is part of Qt Jambi.
**
** $BEGIN_LICENSE$
**
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
**
** $END_LICENSE$
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

#ifndef QTJAMBI_FUNCTIONALBASE_H
#define QTJAMBI_FUNCTIONALBASE_H

#include <QtCore/QAtomicInt>
#include <QtCore/QSharedDataPointer>
#include "qtjambishell.h"

class FunctorBasePrivate;

class QTJAMBI_EXPORT FunctionalBase : public QtJambiShellInterface {
public:
    FunctionalBase();
    virtual ~FunctionalBase() override;
    virtual void getFunctional(JNIEnv *,void*) = 0;
private:
    Q_DISABLE_COPY_MOVE(FunctionalBase)
    QAtomicInt m_ref;
    friend class FunctorBase;
    friend class FunctorBasePrivate;
};

class QTJAMBI_EXPORT FunctorBase{
public:
    FunctorBase(const FunctorBase& functor);
    virtual ~FunctorBase();
    FunctorBase& operator=(const FunctorBase& base) = delete;
protected:
    FunctorBase(const std::type_info& typeId, FunctionalBase& functional);
    jmethodID javaMethod() const;
    jobject getJavaObjectLocalRef(JNIEnv *env) const;
    QtJambiShell* shell() const;
private:
    QSharedDataPointer<FunctorBasePrivate> d;
};

#endif // QTJAMBI_FUNCTIONALBASE_H
