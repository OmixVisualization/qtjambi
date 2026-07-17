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


#ifndef CONTAINERACCESS_H
#define CONTAINERACCESS_H

#include <QtCore/QList>
#include <QtCore/QTypeInfo>
#include <QtCore/QDebug>
#include "qtjambiapi.h"
#include "containerapi.h"
#include "containerutils.h"

class AbstractNestedSequentialAccess{
protected:
    AbstractNestedSequentialAccess() = default;
    virtual ~AbstractNestedSequentialAccess();
    Q_DISABLE_COPY_MOVE(AbstractNestedSequentialAccess)
public:
    virtual const QSharedPointer<AbstractContainerAccess>& sharedElementNestedContainerAccess() = 0;
};

class AbstractNestedAssociativeAccess{
protected:
    AbstractNestedAssociativeAccess() = default;
    virtual ~AbstractNestedAssociativeAccess();
    Q_DISABLE_COPY_MOVE(AbstractNestedAssociativeAccess)
public:
    virtual const QSharedPointer<AbstractContainerAccess>& sharedKeyNestedContainerAccess() = 0;
    virtual const QSharedPointer<AbstractContainerAccess>& sharedValueNestedContainerAccess() = 0;
};

class AbstractNestedPairAccess{
protected:
    AbstractNestedPairAccess() = default;
    virtual ~AbstractNestedPairAccess();
    Q_DISABLE_COPY_MOVE(AbstractNestedPairAccess)
public:
    virtual const QSharedPointer<AbstractContainerAccess>& sharedFirstNestedContainerAccess() = 0;
    virtual const QSharedPointer<AbstractContainerAccess>& sharedSecondNestedContainerAccess() = 0;
};

AbstractContainerAccess* createContainerAccess(JNIEnv* env, SequentialContainerType containerType,
                                                              const QMetaType& metaType,
                                                              size_t align, size_t size,
                                                              bool isPointer,
                                                              const QtJambiUtils::QHashFunction& hashFunction,
                                                              const QtJambiUtils::InternalToExternalConverter& memberConverter,
                                                              const QtJambiUtils::ExternalToInternalConverter& memberReConverter,
                                                              const QSharedPointer<AbstractContainerAccess>& memberNestedContainerAccess,
                                                              PtrOwnerFunction ownerFunction);
AbstractContainerAccess* createContainerAccess(JNIEnv* env, AssociativeContainerType mapType,
                                                              const QMetaType& memberMetaType1,
                                                              size_t align1, size_t size1,
                                                              bool isPointer1,
                                                              const QtJambiUtils::QHashFunction& hashFunction1,
                                                              const QtJambiUtils::InternalToExternalConverter& memberConverter1,
                                                              const QtJambiUtils::ExternalToInternalConverter& memberReConverter1,
                                                              const QSharedPointer<AbstractContainerAccess>& memberNestedContainerAccess1,
                                                              PtrOwnerFunction ownerFunction1,
                                                              const QMetaType& memberMetaType2,
                                                              size_t align2, size_t size2,
                                                              bool isPointer2,
                                                              const QtJambiUtils::QHashFunction& hashFunction2,
                                                              const QtJambiUtils::InternalToExternalConverter& memberConverter2,
                                                              const QtJambiUtils::ExternalToInternalConverter& memberReConverter2,
                                                              const QSharedPointer<AbstractContainerAccess>& memberNestedContainerAccess2,
                                                              PtrOwnerFunction ownerFunction2);
AbstractContainerAccess* createContainerAccess(SequentialContainerType containerType, const QMetaType& memberMetaType);
AbstractContainerAccess* createContainerAccess(AssociativeContainerType mapType,
                                                              const QMetaType& memberMetaType1,
                                                              const QMetaType& memberMetaType2);

size_t pointerHashFunction(const void* ptr, size_t seed);

#endif // CONTAINERACCESS_H
