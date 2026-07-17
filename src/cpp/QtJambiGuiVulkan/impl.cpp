/****************************************************************************
**
** Copyright (C) 2009-2026 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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

#include "pch_p.h"

#if QT_CONFIG(vulkan) && __has_include(<vulkan/vulkan.h>)
void __qt_destruct_QVulkanLayers(void* ptr)
{
    QTJAMBI_NATIVE_METHOD_CALL("destruct QVulkanInfoVector<QVulkanLayer>")
    reinterpret_cast<QVulkanInfoVector<QVulkanLayer>*>(ptr)->~QVulkanInfoVector();
}

void __qt_delete_QVulkanLayers(void *ptr, bool isShell)
{
    QTJAMBI_NATIVE_METHOD_CALL("qtjambi_deleter for QVulkanInfoVector<QVulkanLayer>")
    QVulkanInfoVector<QVulkanLayer> *_ptr = reinterpret_cast<QVulkanInfoVector<QVulkanLayer> *>(ptr);
    if(!isShell){
        QtJambiAPI::registerNonShellDeletion(ptr);
    }
    delete _ptr;
}

void __qt_destruct_QVulkanExtensions(void* ptr)
{
    QTJAMBI_NATIVE_METHOD_CALL("destruct QVulkanInfoVector<QVulkanExtension>")
    reinterpret_cast<QVulkanInfoVector<QVulkanExtension>*>(ptr)->~QVulkanInfoVector();
}

void __qt_delete_QVulkanExtensions(void *ptr, bool isShell)
{
    QTJAMBI_NATIVE_METHOD_CALL("qtjambi_deleter for QVulkanInfoVector<QVulkanExtension>")
    QVulkanInfoVector<QVulkanExtension> *_ptr = reinterpret_cast<QVulkanInfoVector<QVulkanExtension> *>(ptr);
    if(!isShell){
        QtJambiAPI::registerNonShellDeletion(ptr);
    }
    delete _ptr;
}
#endif

void initialize_meta_info_vulkan(){
#if QT_CONFIG(vulkan) && __has_include(<vulkan/vulkan.h>)
    using namespace RegistryAPI;
    {
        const std::type_info& typeId = registerObjectTypeInfo<QVulkanInfoVector<QVulkanLayer>>("QVulkanInfoVector<QVulkanLayer>", "io/qt/gui/vulkan/QVulkanInfoVector");
        Q_UNUSED(typeId)
        registerConstructorInfos(typeId, 0, &__qt_destruct_QVulkanLayers, {});
        registerDeleter(typeId, &__qt_delete_QVulkanLayers);
        registerContainerAccessFactory(typeId, NewContainerAccessFunction(&QListAccess<QVulkanLayer>::newInstance));
    }
    {
        const std::type_info& typeId = registerObjectTypeInfo<QVulkanInfoVector<QVulkanExtension>>("QVulkanInfoVector<QVulkanExtension>", "io/qt/gui/vulkan/QVulkanInfoVector");
        Q_UNUSED(typeId)
        registerConstructorInfos(typeId, 0, &__qt_destruct_QVulkanExtensions, {});
        registerDeleter(typeId, &__qt_delete_QVulkanExtensions);
        registerContainerAccessFactory(typeId, NewContainerAccessFunction(&QListAccess<QVulkanExtension>::newInstance));
    }
#endif
}