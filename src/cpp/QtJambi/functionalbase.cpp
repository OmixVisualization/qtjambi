/****************************************************************************
**
** Copyright (C) 2009-2022 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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

#include "functionalbase.h"
#include "qtjambishell_p.h"
#include "java_p.h"

FunctionalBase::FunctionalBase()
    :m_ref(0)
{}

FunctionalBase::~FunctionalBase(){}

class FunctorBasePrivate : public QSharedData{
public:
    FunctorBasePrivate(const std::type_info& typeId, FunctionalBase& functional)
        : QSharedData(),
          m_method(functional.__shell()->javaMethod(typeId, 0)),
          m_link(static_cast<const QtJambiShellImpl*>(functional.__shell())->link()),
          m_typeId(typeId)
    {
        if(0==functional.m_ref++){
            if(QSharedPointer<QtJambiLink> link = m_link.toStrongRef()){
                if(JniEnvironment env{200}){
                    link->setCppOwnership(env);
                }
            }
        }
    }
    ~FunctorBasePrivate()
    {
        if(QSharedPointer<QtJambiLink> link = m_link.toStrongRef()){
            if(void* ptr = link->typedPointer(m_typeId)){
                FunctionalBase* functional = reinterpret_cast<FunctionalBase*>(ptr);
                if(--functional->m_ref==0){
                    if(DefaultJniEnvironment env{200}){
                        link->setJavaOwnership(env);
                    }
                }
            }
        }
    }

private:
    jmethodID m_method;
    QWeakPointer<QtJambiLink> m_link;
    const std::type_info& m_typeId;
    friend FunctorBase;
};

FunctorBase::FunctorBase(const FunctorBase& functor)
    : d(functor.d)
{
}

FunctorBase::FunctorBase(const std::type_info& typeId, FunctionalBase& functional)
    : d(QSharedDataPointer<FunctorBasePrivate>( new FunctorBasePrivate(typeId, functional) )) {}

FunctorBase::~FunctorBase()
{
}

jobject FunctorBase::getJavaObjectLocalRef(JNIEnv *env) const
{
    if(QSharedPointer<QtJambiLink> link = d->m_link.toStrongRef()){
        return link->getJavaObjectLocalRef(env);
    }
    return nullptr;
}

QtJambiShell* FunctorBase::shell() const
{
    if(QSharedPointer<QtJambiLink> link = d->m_link.toStrongRef()){
        void* ptr = link->pointer();
        return reinterpret_cast<FunctionalBase*>(ptr)->__shell();
    }
    return nullptr;
}

jmethodID FunctorBase::javaMethod() const
{
    return d->m_method;
}
