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

InternalToExternalConverter ContainerConverter::getInternalToExternalConverter_QMultiHash(const QString& internalTypeName, const InternalToExternalConverter& memberConverter1, bool isPointer1, bool isStaticType1, size_t align1, size_t size1, int memberMetaType1, const InternalToExternalConverter& memberConverter2, bool isPointer2, bool isStaticType2, size_t align2, size_t size2, int memberMetaType2, MapType mapType)
{
    Q_UNUSED(isStaticType1)
    Q_UNUSED(isStaticType2)
    if(isPointer1)
        size1 = 0;
    if(isPointer2)
        size2 = 0;
#define ELEMENT_ALIGNSIZE2_CASEACTION(AL1,SZ1,AL2,SZ2)\
    return ContainerConverter::getInternalToExternalConverter<QMultiHash,AL1,SZ1,true,AL2,SZ2,true>(internalTypeName, memberConverter1, memberMetaType1, memberConverter2, memberMetaType2, mapType);
    ELEMENT_ALIGNSIZE2_SWITCH(align1,size1,align2,size2)
#undef ELEMENT_ALIGNSIZE2_CASEACTION
    return nullptr;
}

InternalToExternalConverter ContainerConverter::getInternalToExternalConverter_QHash(const QString& internalTypeName, const InternalToExternalConverter& memberConverter1, bool isPointer1, bool isStaticType1, size_t align1, size_t size1, int memberMetaType1, const InternalToExternalConverter& memberConverter2, bool isPointer2, bool isStaticType2, size_t align2, size_t size2, int memberMetaType2, MapType mapType)
{
    Q_UNUSED(isStaticType1)
    Q_UNUSED(isStaticType2)
    if(isPointer1)
        size1 = 0;
    if(isPointer2)
        size2 = 0;
#define ELEMENT_ALIGNSIZE2_CASEACTION(AL1,SZ1,AL2,SZ2)\
    return ContainerConverter::getInternalToExternalConverter<QHash,AL1,SZ1,true,AL2,SZ2,true>(internalTypeName, memberConverter1, memberMetaType1, memberConverter2, memberMetaType2, mapType);
    ELEMENT_ALIGNSIZE2_SWITCH(align1,size1,align2,size2)
#undef ELEMENT_ALIGNSIZE2_CASEACTION
    return nullptr;
}
