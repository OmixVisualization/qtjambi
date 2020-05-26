/****************************************************************************
**
** Copyright (C) 2009-2020 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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

#include "qtjambi_repository_p.h"
#include "qtjambi_jobjectwrapper.h"
#include "qtjambitypemanager_p.h"
#include "qtjambilink_p.h"
#include "qtjambi_core.h"
#include "qtjambi_cast.h"
#include "qtjambi_registry_p.h"
#include "qtjambi_interfaces.h"
#include "qtjambi_functionpointer.h"

ExternalToInternalConverter ContainerConverter::getExternalToInternalConverter_QVector(const QString& internalTypeName, const ExternalToInternalConverter& memberConverter, bool isPointer, bool isStaticType, size_t align, size_t size, int memberMetaType)
{
    Q_UNUSED(align)
    Q_UNUSED(isStaticType)
    if(isPointer)
        size = 0;
#define ELEMENT_SIZE_CASEACTION(SZ)\
    return ContainerConverter::getExternalToInternalConverter<QVector,0,SZ,true>(internalTypeName, memberConverter, memberMetaType);
    ELEMENT_SIZE_SWITCH(size)
#undef ELEMENT_SIZE_CASEACTION
    return nullptr;
}

ExternalToInternalConverter ContainerConverter::getExternalToInternalConverter_QStack(const QString& internalTypeName, const ExternalToInternalConverter& memberConverter, bool isPointer, bool isStaticType, size_t align, size_t size, int memberMetaType)
{
    Q_UNUSED(align)
    Q_UNUSED(isStaticType)
    if(isPointer)
        size = 0;
#define ELEMENT_SIZE_CASEACTION(SZ)\
    return ContainerConverter::getExternalToInternalConverter<QStack,0,SZ,true>(internalTypeName, memberConverter, memberMetaType);
    ELEMENT_SIZE_SWITCH(size)
#undef ELEMENT_SIZE_CASEACTION
    return nullptr;
}

ExternalToInternalConverter ContainerConverter::getExternalToInternalConverter_QLinkedList(const QString& internalTypeName, const ExternalToInternalConverter& memberConverter, bool isPointer, bool isStaticType, size_t align, size_t size, int memberMetaType)
{
    Q_UNUSED(align)
    Q_UNUSED(isStaticType)
    if(isPointer)
        size = 0;
#define ELEMENT_SIZE_CASEACTION(SZ)\
    return ContainerConverter::getExternalToInternalConverter<QLinkedList,0,SZ,true>(internalTypeName, memberConverter, memberMetaType);
    ELEMENT_SIZE_SWITCH(size)
#undef ELEMENT_SIZE_CASEACTION
    return nullptr;
}

ExternalToInternalConverter ContainerConverter::getExternalToInternalConverter_QSet(const QString& internalTypeName, const ExternalToInternalConverter& memberConverter, bool isPointer, bool isStaticType, size_t align, size_t size, int memberMetaType)
{
    Q_UNUSED(align)
    Q_UNUSED(isStaticType)
    if(isPointer)
        size = 0;
#define ELEMENT_SIZE_CASEACTION(SZ)\
    return ContainerConverter::getExternalToInternalConverter<QSet,0,SZ,true>(internalTypeName, memberConverter, memberMetaType);
    ELEMENT_SIZE_SWITCH(size)
#undef ELEMENT_SIZE_CASEACTION
    return nullptr;
}
