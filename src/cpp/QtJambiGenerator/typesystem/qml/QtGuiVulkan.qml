/****************************************************************************
**
** Copyright (C) 2009-2026 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
**
** This file is part of QtJambi.
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

import QtJambiGenerator 1.0

TypeSystem{
    packageName: "io.qt.gui.vulkan"
    noPackageInfo: true
    defaultSuperClass: "QtObject"
    targetName: "QtJambiGuiVulkan"
    module: "qtjambi"
    LoadTypeSystem{name: "QtGui"}
    RequiredLibrary{
        name: "QtGui"
    }

    InjectCode{
        target: CodeClass.MetaInfo
        Text{content: "initialize_meta_info_vulkan();"}
    }

    NativePointerType{
        name: "VkInstance"
    }

    NativePointerType{
        name: "VkPhysicalDevice"
    }

    NativePointerType{
        name: "VkDevice"
    }

    NativePointerType{
        name: "VkQueue"
    }

    NativePointerType{
        name: "VkCommandBuffer"
    }

    PrimitiveType{
        name: "VkBool32"
        javaName: "int"
        jniName: "jint"
        preferredConversion: false
    }

    PrimitiveType{
        name: "VkDeviceAddress"
        javaName: "long"
        jniName: "jlong"
        preferredConversion: false
    }

    PrimitiveType{
        name: "VkDeviceSize"
        javaName: "long"
        jniName: "jlong"
        preferredConversion: false
    }

    PrimitiveType{
        name: "VkFlags"
        javaName: "int"
        jniName: "jint"
        preferredConversion: false
    }

    PrimitiveType{
        name: "VkSampleMask"
        javaName: "int"
        jniName: "jint"
        preferredConversion: false
    }

    PrimitiveType{
        name: "VkSemaphore"
        javaName: "long"
        jniName: "jlong"
        preferredConversion: false
    }

    PrimitiveType{
        name: "VkFence"
        javaName: "long"
        jniName: "jlong"
        preferredConversion: false
    }

    PrimitiveType{
        name: "VkDeviceMemory"
        javaName: "long"
        jniName: "jlong"
        preferredConversion: false
    }

    PrimitiveType{
        name: "VkBuffer"
        javaName: "long"
        jniName: "jlong"
        preferredConversion: false
    }

    PrimitiveType{
        name: "VkQueryPool"
        javaName: "long"
        jniName: "jlong"
        preferredConversion: false
    }

    PrimitiveType{
        name: "VkImageView"
        javaName: "long"
        jniName: "jlong"
        preferredConversion: false
    }

    PrimitiveType{
        name: "VkCommandPool"
        javaName: "long"
        jniName: "jlong"
        preferredConversion: false
    }

    PrimitiveType{
        name: "VkRenderPass"
        javaName: "long"
        jniName: "jlong"
        preferredConversion: false
    }

    PrimitiveType{
        name: "VkFramebuffer"
        javaName: "long"
        jniName: "jlong"
        preferredConversion: false
    }

    PrimitiveType{
        name: "VkEvent"
        javaName: "long"
        jniName: "jlong"
        preferredConversion: false
    }

    PrimitiveType{
        name: "VkBufferView"
        javaName: "long"
        jniName: "jlong"
        preferredConversion: false
    }

    PrimitiveType{
        name: "VkShaderModule"
        javaName: "long"
        jniName: "jlong"
        preferredConversion: false
    }

    PrimitiveType{
        name: "VkPipelineCache"
        javaName: "long"
        jniName: "jlong"
        preferredConversion: false
    }

    PrimitiveType{
        name: "VkPipeline"
        javaName: "long"
        jniName: "jlong"
        preferredConversion: false
    }

    PrimitiveType{
        name: "VkPipelineLayout"
        javaName: "long"
        jniName: "jlong"
        preferredConversion: false
    }

    PrimitiveType{
        name: "VkDescriptorSetLayout"
        javaName: "long"
        jniName: "jlong"
        preferredConversion: false
    }

    PrimitiveType{
        name: "VkSampler"
        javaName: "long"
        jniName: "jlong"
        preferredConversion: false
    }

    PrimitiveType{
        name: "VkDescriptorSet"
        javaName: "long"
        jniName: "jlong"
        preferredConversion: false
    }

    PrimitiveType{
        name: "VkDescriptorPool"
        javaName: "long"
        jniName: "jlong"
        preferredConversion: false
    }

    NativePointerType{
        name: "VkExternalComputeQueueNV"
    }

    PrimitiveType{
        name: "VkDeviceQueueCreateInfo"
        javaName: "int"
        jniName: "jint"
        preferredConversion: false
    }

    PrimitiveType{
        name: "VkQueueFamilyProperties"
        javaName: "int"
        jniName: "jint"
        preferredConversion: false
    }

    PrimitiveType{
        name: "VkPhysicalDeviceProperties"
        javaName: "int"
        jniName: "jint"
        preferredConversion: false
    }

    PrimitiveType{
        name: "VkResult"
        javaName: "int"
        jniName: "jint"
        preferredConversion: false
    }

    PrimitiveType{
        name: "VkObjectType"
        javaName: "int"
        jniName: "jint"
        preferredConversion: false
    }

    PrimitiveType{
        name: "VkVendorId"
        javaName: "int"
        jniName: "jint"
        preferredConversion: false
    }

    PrimitiveType{
        name: "VkSystemAllocationScope"
        javaName: "int"
        jniName: "jint"
        preferredConversion: false
    }

    PrimitiveType{
        name: "VkImageTiling"
        javaName: "int"
        jniName: "jint"
        preferredConversion: false
    }

    PrimitiveType{
        name: "VkImageType"
        javaName: "int"
        jniName: "jint"
        preferredConversion: false
    }

    PrimitiveType{
        name: "VkPhysicalDeviceType"
        javaName: "int"
        jniName: "jint"
        preferredConversion: false
    }

    PrimitiveType{
        name: "VkQueryType"
        javaName: "int"
        jniName: "jint"
        preferredConversion: false
    }

    PrimitiveType{
        name: "VkSharingMode"
        javaName: "int"
        jniName: "jint"
        preferredConversion: false
    }

    PrimitiveType{
        name: "VkComponentSwizzle"
        javaName: "int"
        jniName: "jint"
        preferredConversion: false
    }

    PrimitiveType{
        name: "VkImageViewType"
        javaName: "int"
        jniName: "jint"
        preferredConversion: false
    }

    PrimitiveType{
        name: "VkCommandBufferLevel"
        javaName: "int"
        jniName: "jint"
        preferredConversion: false
    }

    PrimitiveType{
        name: "VkIndexType"
        javaName: "int"
        jniName: "jint"
        preferredConversion: false
    }

    PrimitiveType{
        name: "VkPipelineCacheHeaderVersion"
        javaName: "int"
        jniName: "jint"
        preferredConversion: false
    }

    PrimitiveType{
        name: "VkBorderColor"
        javaName: "int"
        jniName: "jint"
        preferredConversion: false
    }

    PrimitiveType{
        name: "VkFilter"
        javaName: "int"
        jniName: "jint"
        preferredConversion: false
    }

    PrimitiveType{
        name: "VkSamplerAddressMode"
        javaName: "int"
        jniName: "jint"
        preferredConversion: false
    }

    PrimitiveType{
        name: "VkCompareOp"
        javaName: "int"
        jniName: "jint"
        preferredConversion: false
    }

    PrimitiveType{
        name: "VkSamplerMipmapMode"
        javaName: "int"
        jniName: "jint"
        preferredConversion: false
    }

    PrimitiveType{
        name: "VkDescriptorType"
        javaName: "int"
        jniName: "jint"
        preferredConversion: false
    }

    PrimitiveType{
        name: "VkPipelineBindPoint"
        javaName: "int"
        jniName: "jint"
        preferredConversion: false
    }

    PrimitiveType{
        name: "VkBlendFactor"
        javaName: "int"
        jniName: "jint"
        preferredConversion: false
    }

    PrimitiveType{
        name: "VkBlendOp"
        javaName: "int"
        jniName: "jint"
        preferredConversion: false
    }

    PrimitiveType{
        name: "VkDynamicState"
        javaName: "int"
        jniName: "jint"
        preferredConversion: false
    }

    PrimitiveType{
        name: "VkFrontFace"
        javaName: "int"
        jniName: "jint"
        preferredConversion: false
    }

    PrimitiveType{
        name: "VkLogicOp"
        javaName: "int"
        jniName: "jint"
        preferredConversion: false
    }

    PrimitiveType{
        name: "VkStencilOp"
        javaName: "int"
        jniName: "jint"
        preferredConversion: false
    }

    PrimitiveType{
        name: "VkVertexInputRate"
        javaName: "int"
        jniName: "jint"
        preferredConversion: false
    }

    PrimitiveType{
        name: "VkPrimitiveTopology"
        javaName: "int"
        jniName: "jint"
        preferredConversion: false
    }

    PrimitiveType{
        name: "VkPolygonMode"
        javaName: "int"
        jniName: "jint"
        preferredConversion: false
    }

    PrimitiveType{
        name: "VkAttachmentLoadOp"
        javaName: "int"
        jniName: "jint"
        preferredConversion: false
    }

    PrimitiveType{
        name: "VkAttachmentStoreOp"
        javaName: "int"
        jniName: "jint"
        preferredConversion: false
    }

    PrimitiveType{
        name: "VkSubpassContents"
        javaName: "int"
        jniName: "jint"
        preferredConversion: false
    }

    PrimitiveType{
        name: "VkFormatFeatureFlagBits"
        javaName: "int"
        jniName: "jint"
        preferredConversion: false
    }

    PrimitiveType{
        name: "VkImageCreateFlagBits"
        javaName: "int"
        jniName: "jint"
        preferredConversion: false
    }

    PrimitiveType{
        name: "VkSampleCountFlagBits"
        javaName: "int"
        jniName: "jint"
        preferredConversion: false
    }

    PrimitiveType{
        name: "VkImageUsageFlagBits"
        javaName: "int"
        jniName: "jint"
        preferredConversion: false
    }

    PrimitiveType{
        name: "VkInstanceCreateFlagBits"
        javaName: "int"
        jniName: "jint"
        preferredConversion: false
    }

    PrimitiveType{
        name: "VkMemoryHeapFlagBits"
        javaName: "int"
        jniName: "jint"
        preferredConversion: false
    }

    PrimitiveType{
        name: "VkMemoryPropertyFlagBits"
        javaName: "int"
        jniName: "jint"
        preferredConversion: false
    }

    PrimitiveType{
        name: "VkQueueFlagBits"
        javaName: "int"
        jniName: "jint"
        preferredConversion: false
    }

    PrimitiveType{
        name: "VkShaderStageFlagBits"
        javaName: "int"
        jniName: "jint"
        preferredConversion: false
    }

    PrimitiveType{
        name: "VkDeviceQueueCreateFlagBits"
        javaName: "int"
        jniName: "jint"
        preferredConversion: false
    }

    PrimitiveType{
        name: "VkPipelineStageFlagBits"
        javaName: "int"
        jniName: "jint"
        preferredConversion: false
    }

    PrimitiveType{
        name: "VkMemoryMapFlagBits"
        javaName: "int"
        jniName: "jint"
        preferredConversion: false
    }

    PrimitiveType{
        name: "VkImageAspectFlagBits"
        javaName: "int"
        jniName: "jint"
        preferredConversion: false
    }

    PrimitiveType{
        name: "VkSparseImageFormatFlagBits"
        javaName: "int"
        jniName: "jint"
        preferredConversion: false
    }

    PrimitiveType{
        name: "VkSparseMemoryBindFlagBits"
        javaName: "int"
        jniName: "jint"
        preferredConversion: false
    }

    PrimitiveType{
        name: "VkFenceCreateFlagBits"
        javaName: "int"
        jniName: "jint"
        preferredConversion: false
    }

    PrimitiveType{
        name: "VkFenceCreateFlags"
        javaName: "int"
        jniName: "jint"
        preferredConversion: false
    }

    PrimitiveType{
        name: "VkSemaphoreCreateFlags"
        javaName: "int"
        jniName: "jint"
        preferredConversion: false
    }

    PrimitiveType{
        name: "VkQueryPoolCreateFlagBits"
        javaName: "int"
        jniName: "jint"
        preferredConversion: false
    }

    PrimitiveType{
        name: "VkFormatFeatureFlags"
        javaName: "int"
        jniName: "jint"
        preferredConversion: false
    }

    PrimitiveType{
        name: "VkImageCreateFlags"
        javaName: "int"
        jniName: "jint"
        preferredConversion: false
    }

    PrimitiveType{
        name: "VkSampleCountFlags"
        javaName: "int"
        jniName: "jint"
        preferredConversion: false
    }

    PrimitiveType{
        name: "VkImageUsageFlags"
        javaName: "int"
        jniName: "jint"
        preferredConversion: false
    }

    PrimitiveType{
        name: "VkInstanceCreateFlags"
        javaName: "int"
        jniName: "jint"
        preferredConversion: false
    }

    PrimitiveType{
        name: "VkMemoryHeapFlags"
        javaName: "int"
        jniName: "jint"
        preferredConversion: false
    }

    PrimitiveType{
        name: "VkMemoryPropertyFlags"
        javaName: "int"
        jniName: "jint"
        preferredConversion: false
    }

    PrimitiveType{
        name: "VkQueueFlags"
        javaName: "int"
        jniName: "jint"
        preferredConversion: false
    }

    PrimitiveType{
        name: "VkShaderStageFlags"
        javaName: "int"
        jniName: "jint"
        preferredConversion: false
    }

    PrimitiveType{
        name: "VkDeviceCreateFlags"
        javaName: "int"
        jniName: "jint"
        preferredConversion: false
    }

    PrimitiveType{
        name: "VkDeviceQueueCreateFlags"
        javaName: "int"
        jniName: "jint"
        preferredConversion: false
    }

    PrimitiveType{
        name: "VkPipelineStageFlags"
        javaName: "int"
        jniName: "jint"
        preferredConversion: false
    }

    PrimitiveType{
        name: "VkMemoryMapFlags"
        javaName: "int"
        jniName: "jint"
        preferredConversion: false
    }

    PrimitiveType{
        name: "VkImageAspectFlags"
        javaName: "int"
        jniName: "jint"
        preferredConversion: false
    }

    PrimitiveType{
        name: "VkSparseImageFormatFlags"
        javaName: "int"
        jniName: "jint"
        preferredConversion: false
    }

    PrimitiveType{
        name: "VkSparseMemoryBindFlags"
        javaName: "int"
        jniName: "jint"
        preferredConversion: false
    }

    PrimitiveType{
        name: "VkQueryPoolCreateFlags"
        javaName: "int"
        jniName: "jint"
        preferredConversion: false
    }

    PrimitiveType{
        name: "VkQueryPipelineStatisticFlagBits"
        javaName: "int"
        jniName: "jint"
        preferredConversion: false
    }

    PrimitiveType{
        name: "VkQueryPipelineStatisticFlags"
        javaName: "int"
        jniName: "jint"
        preferredConversion: false
    }

    PrimitiveType{
        name: "VkQueryResultFlagBits"
        javaName: "int"
        jniName: "jint"
        preferredConversion: false
    }

    PrimitiveType{
        name: "VkQueryResultFlags"
        javaName: "int"
        jniName: "jint"
        preferredConversion: false
    }

    PrimitiveType{
        name: "VkBufferCreateFlagBits"
        javaName: "int"
        jniName: "jint"
        preferredConversion: false
    }

    PrimitiveType{
        name: "VkBufferCreateFlags"
        javaName: "int"
        jniName: "jint"
        preferredConversion: false
    }

    PrimitiveType{
        name: "VkBufferUsageFlagBits"
        javaName: "int"
        jniName: "jint"
        preferredConversion: false
    }

    PrimitiveType{
        name: "VkBufferUsageFlags"
        javaName: "int"
        jniName: "jint"
        preferredConversion: false
    }

    PrimitiveType{
        name: "VkImageViewCreateFlagBits"
        javaName: "int"
        jniName: "jint"
        preferredConversion: false
    }

    PrimitiveType{
        name: "VkImageViewCreateFlags"
        javaName: "int"
        jniName: "jint"
        preferredConversion: false
    }

    PrimitiveType{
        name: "VkAccessFlagBits"
        javaName: "int"
        jniName: "jint"
        preferredConversion: false
    }

    PrimitiveType{
        name: "VkAccessFlags"
        javaName: "int"
        jniName: "jint"
        preferredConversion: false
    }

    PrimitiveType{
        name: "VkDependencyFlagBits"
        javaName: "int"
        jniName: "jint"
        preferredConversion: false
    }

    PrimitiveType{
        name: "VkDependencyFlags"
        javaName: "int"
        jniName: "jint"
        preferredConversion: false
    }

    PrimitiveType{
        name: "VkCommandPoolCreateFlagBits"
        javaName: "int"
        jniName: "jint"
        preferredConversion: false
    }

    PrimitiveType{
        name: "VkCommandPoolCreateFlags"
        javaName: "int"
        jniName: "jint"
        preferredConversion: false
    }

    PrimitiveType{
        name: "VkCommandPoolResetFlagBits"
        javaName: "int"
        jniName: "jint"
        preferredConversion: false
    }

    PrimitiveType{
        name: "VkCommandPoolResetFlags"
        javaName: "int"
        jniName: "jint"
        preferredConversion: false
    }

    PrimitiveType{
        name: "VkQueryControlFlagBits"
        javaName: "int"
        jniName: "jint"
        preferredConversion: false
    }

    PrimitiveType{
        name: "VkQueryControlFlags"
        javaName: "int"
        jniName: "jint"
        preferredConversion: false
    }

    PrimitiveType{
        name: "VkCommandBufferUsageFlagBits"
        javaName: "int"
        jniName: "jint"
        preferredConversion: false
    }

    PrimitiveType{
        name: "VkCommandBufferUsageFlags"
        javaName: "int"
        jniName: "jint"
        preferredConversion: false
    }

    PrimitiveType{
        name: "VkCommandBufferResetFlagBits"
        javaName: "int"
        jniName: "jint"
        preferredConversion: false
    }

    PrimitiveType{
        name: "VkCommandBufferResetFlags"
        javaName: "int"
        jniName: "jint"
        preferredConversion: false
    }

    PrimitiveType{
        name: "VkEventCreateFlagBits"
        javaName: "int"
        jniName: "jint"
        preferredConversion: false
    }

    PrimitiveType{
        name: "VkEventCreateFlags"
        javaName: "int"
        jniName: "jint"
        preferredConversion: false
    }

    PrimitiveType{
        name: "VkBufferViewCreateFlags"
        javaName: "int"
        jniName: "jint"
        preferredConversion: false
    }

    PrimitiveType{
        name: "VkShaderModuleCreateFlags"
        javaName: "int"
        jniName: "jint"
        preferredConversion: false
    }

    PrimitiveType{
        name: "VkPipelineCacheCreateFlagBits"
        javaName: "int"
        jniName: "jint"
        preferredConversion: false
    }

    PrimitiveType{
        name: "VkPipelineCacheCreateFlags"
        javaName: "int"
        jniName: "jint"
        preferredConversion: false
    }

    PrimitiveType{
        name: "VkPipelineCreateFlagBits"
        javaName: "int"
        jniName: "jint"
        preferredConversion: false
    }

    PrimitiveType{
        name: "VkPipelineCreateFlags"
        javaName: "int"
        jniName: "jint"
        preferredConversion: false
    }

    PrimitiveType{
        name: "VkPipelineLayoutCreateFlagBits"
        javaName: "int"
        jniName: "jint"
        preferredConversion: false
    }

    PrimitiveType{
        name: "VkPipelineLayoutCreateFlags"
        javaName: "int"
        jniName: "jint"
        preferredConversion: false
    }

    PrimitiveType{
        name: "VkPipelineShaderStageCreateFlagBits"
        javaName: "int"
        jniName: "jint"
        preferredConversion: false
    }

    PrimitiveType{
        name: "VkPipelineShaderStageCreateFlags"
        javaName: "int"
        jniName: "jint"
        preferredConversion: false
    }

    PrimitiveType{
        name: "VkSamplerCreateFlagBits"
        javaName: "int"
        jniName: "jint"
        preferredConversion: false
    }

    PrimitiveType{
        name: "VkSamplerCreateFlags"
        javaName: "int"
        jniName: "jint"
        preferredConversion: false
    }

    PrimitiveType{
        name: "VkDescriptorPoolCreateFlagBits"
        javaName: "int"
        jniName: "jint"
        preferredConversion: false
    }

    PrimitiveType{
        name: "VkDescriptorPoolCreateFlags"
        javaName: "int"
        jniName: "jint"
        preferredConversion: false
    }

    PrimitiveType{
        name: "VkDescriptorPoolResetFlags"
        javaName: "int"
        jniName: "jint"
        preferredConversion: false
    }

    PrimitiveType{
        name: "VkDescriptorSetLayoutCreateFlagBits"
        javaName: "int"
        jniName: "jint"
        preferredConversion: false
    }

    PrimitiveType{
        name: "VkDescriptorSetLayoutCreateFlags"
        javaName: "int"
        jniName: "jint"
        preferredConversion: false
    }

    PrimitiveType{
        name: "VkColorComponentFlagBits"
        javaName: "int"
        jniName: "jint"
        preferredConversion: false
    }

    PrimitiveType{
        name: "VkColorComponentFlags"
        javaName: "int"
        jniName: "jint"
        preferredConversion: false
    }

    PrimitiveType{
        name: "VkCullModeFlagBits"
        javaName: "int"
        jniName: "jint"
        preferredConversion: false
    }

    PrimitiveType{
        name: "VkCullModeFlags"
        javaName: "int"
        jniName: "jint"
        preferredConversion: false
    }

    PrimitiveType{
        name: "VkPipelineColorBlendStateCreateFlagBits"
        javaName: "int"
        jniName: "jint"
        preferredConversion: false
    }

    PrimitiveType{
        name: "VkPipelineColorBlendStateCreateFlags"
        javaName: "int"
        jniName: "jint"
        preferredConversion: false
    }

    PrimitiveType{
        name: "VkPipelineDepthStencilStateCreateFlagBits"
        javaName: "int"
        jniName: "jint"
        preferredConversion: false
    }

    PrimitiveType{
        name: "VkPipelineDepthStencilStateCreateFlags"
        javaName: "int"
        jniName: "jint"
        preferredConversion: false
    }

    PrimitiveType{
        name: "VkPipelineDynamicStateCreateFlags"
        javaName: "int"
        jniName: "jint"
        preferredConversion: false
    }

    PrimitiveType{
        name: "VkPipelineInputAssemblyStateCreateFlags"
        javaName: "int"
        jniName: "jint"
        preferredConversion: false
    }

    PrimitiveType{
        name: "VkPipelineMultisampleStateCreateFlags"
        javaName: "int"
        jniName: "jint"
        preferredConversion: false
    }

    PrimitiveType{
        name: "VkPipelineRasterizationStateCreateFlags"
        javaName: "int"
        jniName: "jint"
        preferredConversion: false
    }

    PrimitiveType{
        name: "VkPipelineTessellationStateCreateFlags"
        javaName: "int"
        jniName: "jint"
        preferredConversion: false
    }

    PrimitiveType{
        name: "VkPipelineVertexInputStateCreateFlags"
        javaName: "int"
        jniName: "jint"
        preferredConversion: false
    }

    PrimitiveType{
        name: "VkPipelineViewportStateCreateFlags"
        javaName: "int"
        jniName: "jint"
        preferredConversion: false
    }

    PrimitiveType{
        name: "VkAttachmentDescriptionFlagBits"
        javaName: "int"
        jniName: "jint"
        preferredConversion: false
    }

    PrimitiveType{
        name: "VkAttachmentDescriptionFlags"
        javaName: "int"
        jniName: "jint"
        preferredConversion: false
    }

    PrimitiveType{
        name: "VkFramebufferCreateFlagBits"
        javaName: "int"
        jniName: "jint"
        preferredConversion: false
    }

    PrimitiveType{
        name: "VkFramebufferCreateFlags"
        javaName: "int"
        jniName: "jint"
        preferredConversion: false
    }

    PrimitiveType{
        name: "VkRenderPassCreateFlagBits"
        javaName: "int"
        jniName: "jint"
        preferredConversion: false
    }

    PrimitiveType{
        name: "VkRenderPassCreateFlags"
        javaName: "int"
        jniName: "jint"
        preferredConversion: false
    }

    PrimitiveType{
        name: "VkSubpassDescriptionFlagBits"
        javaName: "int"
        jniName: "jint"
        preferredConversion: false
    }

    PrimitiveType{
        name: "VkSubpassDescriptionFlags"
        javaName: "int"
        jniName: "jint"
        preferredConversion: false
    }

    PrimitiveType{
        name: "VkStencilFaceFlagBits"
        javaName: "int"
        jniName: "jint"
        preferredConversion: false
    }

    PrimitiveType{
        name: "VkStencilFaceFlags"
        javaName: "int"
        jniName: "jint"
        preferredConversion: false
    }

    PrimitiveType{
            name: "VkSubgroupFeatureFlags"
            javaName: "int"
            jniName: "jint"
            preferredConversion: false
    }

    PrimitiveType{
            name: "VkPeerMemoryFeatureFlags"
            javaName: "int"
            jniName: "jint"
            preferredConversion: false
    }

    PrimitiveType{
            name: "VkMemoryAllocateFlags"
            javaName: "int"
            jniName: "jint"
            preferredConversion: false
    }

    PrimitiveType{
            name: "VkCommandPoolTrimFlags"
            javaName: "int"
            jniName: "jint"
            preferredConversion: false
    }

    PrimitiveType{
            name: "VkExternalMemoryHandleTypeFlags"
            javaName: "int"
            jniName: "jint"
            preferredConversion: false
    }

    PrimitiveType{
            name: "VkExternalMemoryFeatureFlags"
            javaName: "int"
            jniName: "jint"
            preferredConversion: false
    }

    PrimitiveType{
            name: "VkExternalFenceHandleTypeFlags"
            javaName: "int"
            jniName: "jint"
            preferredConversion: false
    }

    PrimitiveType{
            name: "VkExternalFenceFeatureFlags"
            javaName: "int"
            jniName: "jint"
            preferredConversion: false
    }

    PrimitiveType{
            name: "VkFenceImportFlags"
            javaName: "int"
            jniName: "jint"
            preferredConversion: false
    }

    PrimitiveType{
            name: "VkSemaphoreImportFlags"
            javaName: "int"
            jniName: "jint"
            preferredConversion: false
    }

    PrimitiveType{
            name: "VkExternalSemaphoreHandleTypeFlags"
            javaName: "int"
            jniName: "jint"
            preferredConversion: false
    }

    PrimitiveType{
            name: "VkExternalSemaphoreFeatureFlags"
            javaName: "int"
            jniName: "jint"
            preferredConversion: false
    }

    PrimitiveType{
            name: "VkDescriptorUpdateTemplateCreateFlags"
            javaName: "int"
            jniName: "jint"
            preferredConversion: false
    }

    PrimitiveType{
            name: "VkResolveModeFlags"
            javaName: "int"
            jniName: "jint"
            preferredConversion: false
    }

    PrimitiveType{
            name: "VkSemaphoreWaitFlags"
            javaName: "int"
            jniName: "jint"
            preferredConversion: false
    }

    PrimitiveType{
            name: "VkDescriptorBindingFlags"
            javaName: "int"
            jniName: "jint"
            preferredConversion: false
    }

    PrimitiveType{
            name: "VkToolPurposeFlags"
            javaName: "int"
            jniName: "jint"
            preferredConversion: false
    }

    PrimitiveType{
            name: "VkPrivateDataSlotCreateFlags"
            javaName: "int"
            jniName: "jint"
            preferredConversion: false
    }

    PrimitiveType{
            name: "VkSubmitFlags"
            javaName: "int"
            jniName: "jint"
            preferredConversion: false
    }

    PrimitiveType{
            name: "VkPipelineCreationFeedbackFlags"
            javaName: "int"
            jniName: "jint"
            preferredConversion: false
    }

    PrimitiveType{
            name: "VkRenderingFlags"
            javaName: "int"
            jniName: "jint"
            preferredConversion: false
    }

    PrimitiveType{
            name: "VkMemoryUnmapFlags"
            javaName: "int"
            jniName: "jint"
            preferredConversion: false
    }

    PrimitiveType{
            name: "VkHostImageCopyFlags"
            javaName: "int"
            jniName: "jint"
            preferredConversion: false
    }

    PrimitiveType{
            name: "VkSurfaceTransformFlagsKHR"
            javaName: "int"
            jniName: "jint"
            preferredConversion: false
    }

    PrimitiveType{
            name: "VkCompositeAlphaFlagsKHR"
            javaName: "int"
            jniName: "jint"
            preferredConversion: false
    }

    PrimitiveType{
            name: "VkSwapchainCreateFlagsKHR"
            javaName: "int"
            jniName: "jint"
            preferredConversion: false
    }

    PrimitiveType{
            name: "VkDeviceGroupPresentModeFlagsKHR"
            javaName: "int"
            jniName: "jint"
            preferredConversion: false
    }

    PrimitiveType{
            name: "VkDisplayModeCreateFlagsKHR"
            javaName: "int"
            jniName: "jint"
            preferredConversion: false
    }

    PrimitiveType{
            name: "VkDisplayPlaneAlphaFlagsKHR"
            javaName: "int"
            jniName: "jint"
            preferredConversion: false
    }

    PrimitiveType{
            name: "VkDisplaySurfaceCreateFlagsKHR"
            javaName: "int"
            jniName: "jint"
            preferredConversion: false
    }

    PrimitiveType{
            name: "VkVideoCodecOperationFlagsKHR"
            javaName: "int"
            jniName: "jint"
            preferredConversion: false
    }

    PrimitiveType{
            name: "VkVideoChromaSubsamplingFlagsKHR"
            javaName: "int"
            jniName: "jint"
            preferredConversion: false
    }

    PrimitiveType{
            name: "VkVideoComponentBitDepthFlagsKHR"
            javaName: "int"
            jniName: "jint"
            preferredConversion: false
    }

    PrimitiveType{
            name: "VkVideoCapabilityFlagsKHR"
            javaName: "int"
            jniName: "jint"
            preferredConversion: false
    }

    PrimitiveType{
            name: "VkVideoSessionCreateFlagsKHR"
            javaName: "int"
            jniName: "jint"
            preferredConversion: false
    }

    PrimitiveType{
            name: "VkVideoSessionParametersCreateFlagsKHR"
            javaName: "int"
            jniName: "jint"
            preferredConversion: false
    }

    PrimitiveType{
            name: "VkVideoBeginCodingFlagsKHR"
            javaName: "int"
            jniName: "jint"
            preferredConversion: false
    }

    PrimitiveType{
            name: "VkVideoEndCodingFlagsKHR"
            javaName: "int"
            jniName: "jint"
            preferredConversion: false
    }

    PrimitiveType{
            name: "VkVideoCodingControlFlagsKHR"
            javaName: "int"
            jniName: "jint"
            preferredConversion: false
    }

    PrimitiveType{
            name: "VkVideoDecodeCapabilityFlagsKHR"
            javaName: "int"
            jniName: "jint"
            preferredConversion: false
    }

    PrimitiveType{
            name: "VkVideoDecodeUsageFlagsKHR"
            javaName: "int"
            jniName: "jint"
            preferredConversion: false
    }

    PrimitiveType{
            name: "VkVideoDecodeFlagsKHR"
            javaName: "int"
            jniName: "jint"
            preferredConversion: false
    }

    PrimitiveType{
            name: "VkVideoEncodeH264CapabilityFlagsKHR"
            javaName: "int"
            jniName: "jint"
            preferredConversion: false
    }

    PrimitiveType{
            name: "VkVideoEncodeH264StdFlagsKHR"
            javaName: "int"
            jniName: "jint"
            preferredConversion: false
    }

    PrimitiveType{
            name: "VkVideoEncodeH264RateControlFlagsKHR"
            javaName: "int"
            jniName: "jint"
            preferredConversion: false
    }

    PrimitiveType{
            name: "VkVideoEncodeH265CapabilityFlagsKHR"
            javaName: "int"
            jniName: "jint"
            preferredConversion: false
    }

    PrimitiveType{
            name: "VkVideoEncodeH265StdFlagsKHR"
            javaName: "int"
            jniName: "jint"
            preferredConversion: false
    }

    PrimitiveType{
            name: "VkVideoEncodeH265CtbSizeFlagsKHR"
            javaName: "int"
            jniName: "jint"
            preferredConversion: false
    }

    PrimitiveType{
            name: "VkVideoEncodeH265TransformBlockSizeFlagsKHR"
            javaName: "int"
            jniName: "jint"
            preferredConversion: false
    }

    PrimitiveType{
            name: "VkVideoEncodeH265RateControlFlagsKHR"
            javaName: "int"
            jniName: "jint"
            preferredConversion: false
    }

    PrimitiveType{
            name: "VkVideoDecodeH264PictureLayoutFlagsKHR"
            javaName: "int"
            jniName: "jint"
            preferredConversion: false
    }

    PrimitiveType{
            name: "VkCommandPoolTrimFlagsKHR"
            javaName: "int"
            jniName: "jint"
            preferredConversion: false
    }

    PrimitiveType{
            name: "VkPerformanceCounterDescriptionFlagsKHR"
            javaName: "int"
            jniName: "jint"
            preferredConversion: false
    }

    PrimitiveType{
            name: "VkAcquireProfilingLockFlagsKHR"
            javaName: "int"
            jniName: "jint"
            preferredConversion: false
    }

    PrimitiveType{
            name: "VkVideoEncodeFlagsKHR"
            javaName: "int"
            jniName: "jint"
            preferredConversion: false
    }

    PrimitiveType{
            name: "VkVideoEncodeCapabilityFlagsKHR"
            javaName: "int"
            jniName: "jint"
            preferredConversion: false
    }

    PrimitiveType{
            name: "VkVideoEncodeRateControlModeFlagsKHR"
            javaName: "int"
            jniName: "jint"
            preferredConversion: false
    }

    PrimitiveType{
            name: "VkVideoEncodeFeedbackFlagsKHR"
            javaName: "int"
            jniName: "jint"
            preferredConversion: false
    }

    PrimitiveType{
            name: "VkVideoEncodeUsageFlagsKHR"
            javaName: "int"
            jniName: "jint"
            preferredConversion: false
    }

    PrimitiveType{
            name: "VkVideoEncodeContentFlagsKHR"
            javaName: "int"
            jniName: "jint"
            preferredConversion: false
    }

    PrimitiveType{
            name: "VkVideoEncodeRateControlFlagsKHR"
            javaName: "int"
            jniName: "jint"
            preferredConversion: false
    }

    PrimitiveType{
            name: "VkAddressCommandFlagsKHR"
            javaName: "int"
            jniName: "jint"
            preferredConversion: false
    }

    PrimitiveType{
            name: "VkConditionalRenderingFlagsEXT"
            javaName: "int"
            jniName: "jint"
            preferredConversion: false
    }

    PrimitiveType{
            name: "VkAccelerationStructureCreateFlagsKHR"
            javaName: "int"
            jniName: "jint"
            preferredConversion: false
    }

    PrimitiveType{
            name: "VkPresentScalingFlagsKHR"
            javaName: "int"
            jniName: "jint"
            preferredConversion: false
    }

    PrimitiveType{
            name: "VkPresentGravityFlagsKHR"
            javaName: "int"
            jniName: "jint"
            preferredConversion: false
    }

    PrimitiveType{
            name: "VkVideoEncodeAV1CapabilityFlagsKHR"
            javaName: "int"
            jniName: "jint"
            preferredConversion: false
    }

    PrimitiveType{
            name: "VkVideoEncodeAV1StdFlagsKHR"
            javaName: "int"
            jniName: "jint"
            preferredConversion: false
    }

    PrimitiveType{
            name: "VkVideoEncodeAV1SuperblockSizeFlagsKHR"
            javaName: "int"
            jniName: "jint"
            preferredConversion: false
    }

    PrimitiveType{
            name: "VkVideoEncodeAV1RateControlFlagsKHR"
            javaName: "int"
            jniName: "jint"
            preferredConversion: false
    }

    PrimitiveType{
            name: "VkAddressCopyFlagsKHR"
            javaName: "int"
            jniName: "jint"
            preferredConversion: false
    }

    PrimitiveType{
            name: "VkVideoEncodeIntraRefreshModeFlagsKHR"
            javaName: "int"
            jniName: "jint"
            preferredConversion: false
    }

    PrimitiveType{
            name: "VkDeviceFaultFlagsKHR"
            javaName: "int"
            jniName: "jint"
            preferredConversion: false
    }

    PrimitiveType{
            name: "VkRenderingAttachmentFlagsKHR"
            javaName: "int"
            jniName: "jint"
            preferredConversion: false
    }

    PrimitiveType{
            name: "VkResolveImageFlagsKHR"
            javaName: "int"
            jniName: "jint"
            preferredConversion: false
    }

    PrimitiveType{
            name: "VkDebugReportFlagsEXT"
            javaName: "int"
            jniName: "jint"
            preferredConversion: false
    }

    PrimitiveType{
            name: "VkPipelineRasterizationStateStreamCreateFlagsEXT"
            javaName: "int"
            jniName: "jint"
            preferredConversion: false
    }

    PrimitiveType{
            name: "VkExternalMemoryHandleTypeFlagsNV"
            javaName: "int"
            jniName: "jint"
            preferredConversion: false
    }

    PrimitiveType{
            name: "VkExternalMemoryFeatureFlagsNV"
            javaName: "int"
            jniName: "jint"
            preferredConversion: false
    }

    PrimitiveType{
            name: "VkSurfaceCounterFlagsEXT"
            javaName: "int"
            jniName: "jint"
            preferredConversion: false
    }

    PrimitiveType{
            name: "VkPipelineViewportSwizzleStateCreateFlagsNV"
            javaName: "int"
            jniName: "jint"
            preferredConversion: false
    }

    PrimitiveType{
            name: "VkPipelineDiscardRectangleStateCreateFlagsEXT"
            javaName: "int"
            jniName: "jint"
            preferredConversion: false
    }

    PrimitiveType{
            name: "VkPipelineRasterizationConservativeStateCreateFlagsEXT"
            javaName: "int"
            jniName: "jint"
            preferredConversion: false
    }

    PrimitiveType{
            name: "VkPipelineRasterizationDepthClipStateCreateFlagsEXT"
            javaName: "int"
            jniName: "jint"
            preferredConversion: false
    }

    PrimitiveType{
            name: "VkDebugUtilsMessengerCallbackDataFlagsEXT"
            javaName: "int"
            jniName: "jint"
            preferredConversion: false
    }

    PrimitiveType{
            name: "VkDebugUtilsMessageTypeFlagsEXT"
            javaName: "int"
            jniName: "jint"
            preferredConversion: false
    }

    PrimitiveType{
            name: "VkDebugUtilsMessageSeverityFlagsEXT"
            javaName: "int"
            jniName: "jint"
            preferredConversion: false
    }

    PrimitiveType{
            name: "VkDebugUtilsMessengerCreateFlagsEXT"
            javaName: "int"
            jniName: "jint"
            preferredConversion: false
    }

    PrimitiveType{
            name: "VkSpirvResourceTypeFlagsEXT"
            javaName: "int"
            jniName: "jint"
            preferredConversion: false
    }

    PrimitiveType{
            name: "VkPipelineCoverageToColorStateCreateFlagsNV"
            javaName: "int"
            jniName: "jint"
            preferredConversion: false
    }

    PrimitiveType{
            name: "VkPipelineCoverageModulationStateCreateFlagsNV"
            javaName: "int"
            jniName: "jint"
            preferredConversion: false
    }

    PrimitiveType{
            name: "VkValidationCacheCreateFlagsEXT"
            javaName: "int"
            jniName: "jint"
            preferredConversion: false
    }

    PrimitiveType{
            name: "VkGeometryFlagsKHR"
            javaName: "int"
            jniName: "jint"
            preferredConversion: false
    }

    PrimitiveType{
            name: "VkGeometryInstanceFlagsKHR"
            javaName: "int"
            jniName: "jint"
            preferredConversion: false
    }

    PrimitiveType{
            name: "VkBuildAccelerationStructureFlagsKHR"
            javaName: "int"
            jniName: "jint"
            preferredConversion: false
    }

    PrimitiveType{
            name: "VkPipelineCompilerControlFlagsAMD"
            javaName: "int"
            jniName: "jint"
            preferredConversion: false
    }

    PrimitiveType{
            name: "VkPresentStageFlagsEXT"
            javaName: "int"
            jniName: "jint"
            preferredConversion: false
    }

    PrimitiveType{
            name: "VkPastPresentationTimingFlagsEXT"
            javaName: "int"
            jniName: "jint"
            preferredConversion: false
    }

    PrimitiveType{
            name: "VkPresentTimingInfoFlagsEXT"
            javaName: "int"
            jniName: "jint"
            preferredConversion: false
    }

    PrimitiveType{
            name: "VkShaderCorePropertiesFlagsAMD"
            javaName: "int"
            jniName: "jint"
            preferredConversion: false
    }

    PrimitiveType{
            name: "VkHeadlessSurfaceCreateFlagsEXT"
            javaName: "int"
            jniName: "jint"
            preferredConversion: false
    }

    PrimitiveType{
            name: "VkIndirectStateFlagsNV"
            javaName: "int"
            jniName: "jint"
            preferredConversion: false
    }

    PrimitiveType{
            name: "VkIndirectCommandsLayoutUsageFlagsNV"
            javaName: "int"
            jniName: "jint"
            preferredConversion: false
    }

    PrimitiveType{
            name: "VkDeviceMemoryReportFlagsEXT"
            javaName: "int"
            jniName: "jint"
            preferredConversion: false
    }

    PrimitiveType{
            name: "VkDeviceDiagnosticsConfigFlagsNV"
            javaName: "int"
            jniName: "jint"
            preferredConversion: false
    }

    PrimitiveType{
            name: "VkTileShadingRenderPassFlagsQCOM"
            javaName: "int"
            jniName: "jint"
            preferredConversion: false
    }

    PrimitiveType{
            name: "VkGraphicsPipelineLibraryFlagsEXT"
            javaName: "int"
            jniName: "jint"
            preferredConversion: false
    }

    PrimitiveType{
            name: "VkAccelerationStructureMotionInfoFlagsNV"
            javaName: "int"
            jniName: "jint"
            preferredConversion: false
    }

    PrimitiveType{
            name: "VkAccelerationStructureMotionInstanceFlagsNV"
            javaName: "int"
            jniName: "jint"
            preferredConversion: false
    }

    PrimitiveType{
            name: "VkImageCompressionFlagsEXT"
            javaName: "int"
            jniName: "jint"
            preferredConversion: false
    }

    PrimitiveType{
            name: "VkImageCompressionFixedRateFlagsEXT"
            javaName: "int"
            jniName: "jint"
            preferredConversion: false
    }

    PrimitiveType{
            name: "VkDeviceAddressBindingFlagsEXT"
            javaName: "int"
            jniName: "jint"
            preferredConversion: false
    }

    PrimitiveType{
            name: "VkFrameBoundaryFlagsEXT"
            javaName: "int"
            jniName: "jint"
            preferredConversion: false
    }

    PrimitiveType{
            name: "VkVideoEncodeRgbModelConversionFlagsVALVE"
            javaName: "int"
            jniName: "jint"
            preferredConversion: false
    }

    PrimitiveType{
            name: "VkVideoEncodeRgbRangeCompressionFlagsVALVE"
            javaName: "int"
            jniName: "jint"
            preferredConversion: false
    }

    PrimitiveType{
            name: "VkVideoEncodeRgbChromaOffsetFlagsVALVE"
            javaName: "int"
            jniName: "jint"
            preferredConversion: false
    }

    PrimitiveType{
            name: "VkBuildMicromapFlagsEXT"
            javaName: "int"
            jniName: "jint"
            preferredConversion: false
    }

    PrimitiveType{
            name: "VkMicromapCreateFlagsEXT"
            javaName: "int"
            jniName: "jint"
            preferredConversion: false
    }

    PrimitiveType{
            name: "VkDirectDriverLoadingFlagsLUNARG"
            javaName: "int"
            jniName: "jint"
            preferredConversion: false
    }

    PrimitiveType{
            name: "VkOpticalFlowGridSizeFlagsNV"
            javaName: "int"
            jniName: "jint"
            preferredConversion: false
    }

    PrimitiveType{
            name: "VkOpticalFlowUsageFlagsNV"
            javaName: "int"
            jniName: "jint"
            preferredConversion: false
    }

    PrimitiveType{
            name: "VkOpticalFlowSessionCreateFlagsNV"
            javaName: "int"
            jniName: "jint"
            preferredConversion: false
    }

    PrimitiveType{
            name: "VkOpticalFlowExecuteFlagsNV"
            javaName: "int"
            jniName: "jint"
            preferredConversion: false
    }

    PrimitiveType{
            name: "VkShaderCreateFlagsEXT"
            javaName: "int"
            jniName: "jint"
            preferredConversion: false
    }

    PrimitiveType{
            name: "VkDataGraphTOSAQualityFlagsARM"
            javaName: "int"
            jniName: "jint"
            preferredConversion: false
    }

    PrimitiveType{
            name: "VkClusterAccelerationStructureAddressResolutionFlagsNV"
            javaName: "int"
            jniName: "jint"
            preferredConversion: false
    }

    PrimitiveType{
            name: "VkClusterAccelerationStructureClusterFlagsNV"
            javaName: "int"
            jniName: "jint"
            preferredConversion: false
    }

    PrimitiveType{
            name: "VkClusterAccelerationStructureGeometryFlagsNV"
            javaName: "int"
            jniName: "jint"
            preferredConversion: false
    }

    PrimitiveType{
            name: "VkClusterAccelerationStructureIndexFormatFlagsNV"
            javaName: "int"
            jniName: "jint"
            preferredConversion: false
    }

    PrimitiveType{
            name: "VkPartitionedAccelerationStructureInstanceFlagsNV"
            javaName: "int"
            jniName: "jint"
            preferredConversion: false
    }

    PrimitiveType{
            name: "VkIndirectCommandsInputModeFlagsEXT"
            javaName: "int"
            jniName: "jint"
            preferredConversion: false
    }

    PrimitiveType{
            name: "VkIndirectCommandsLayoutUsageFlagsEXT"
            javaName: "int"
            jniName: "jint"
            preferredConversion: false
    }

    PrimitiveType{
            name: "VkDataGraphOpticalFlowGridSizeFlagsARM"
            javaName: "int"
            jniName: "jint"
            preferredConversion: false
    }

    PrimitiveType{
            name: "VkDataGraphOpticalFlowCreateFlagsARM"
            javaName: "int"
            jniName: "jint"
            preferredConversion: false
    }

    PrimitiveType{
            name: "VkDataGraphOpticalFlowImageUsageFlagsARM"
            javaName: "int"
            jniName: "jint"
            preferredConversion: false
    }

    PrimitiveType{
            name: "VkDataGraphOpticalFlowExecuteFlagsARM"
            javaName: "int"
            jniName: "jint"
            preferredConversion: false
    }

    PrimitiveType{
            name: "VkShaderInstrumentationValuesFlagsARM"
            javaName: "int"
            jniName: "jint"
            preferredConversion: false
    }

    ValueType{
        name: "QVulkanExtension"
        ppCondition: "QT_CONFIG(vulkan) && __has_include(<vulkan/vulkan.h>)"
    }
    ValueType{
        name: "QVulkanLayer"
        ppCondition: "QT_CONFIG(vulkan) && __has_include(<vulkan/vulkan.h>)"
    }

    ObjectType{
        name: "QVulkanInstance"
        ppCondition: "QT_CONFIG(vulkan) && __has_include(<vulkan/vulkan.h>)"
        ModifyFunction{
            signature: "QVulkanInstance()"
            InjectCode{
                target: CodeClass.Native
                position: Position.Beginning
                Text{content: String.raw`QtJambiAPI::checkThreadConstructingQWindow(__jni_env, typeid(QVulkanInstance), nullptr);`}
            }
        }
        ModifyFunction{
            signature: "handle()const"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "create()"
            InjectCode{
                target: CodeClass.Native
                position: Position.End
                Text{content: String.raw`
                    if(__java_return_value)
                        QtJambiAPI::setJavaOwnership(%env, __this_nativeId);`}
            }
        }
        ModifyFunction{
            signature: "destroy()"
            remove: RemoveFlag.All
            InjectCode{
                target: CodeClass.Native
                position: Position.End
                Text{content: String.raw`QtJambiAPI::setDefaultOwnership(%env, __this_nativeId);`}
            }
        }
        ModifyFunction{
            signature: "supportedExtensions()const"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "supportedLayers()const"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "deviceFunctions(VkDevice)"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "functions()const"
            remove: RemoveFlag.All
        }
    }
    Rejection{className: "QVulkanFunctions"}
    ObjectType{
        name: "QVulkanFunctions"
        ppCondition: "QT_CONFIG(vulkan) && __has_include(<vulkan/vulkan.h>)"
    }
    Rejection{className: "QVulkanDeviceFunctions"}
    ObjectType{
        name: "QVulkanDeviceFunctions"
        ppCondition: "QT_CONFIG(vulkan) && __has_include(<vulkan/vulkan.h>)"
        Rejection{functionName: "vkCmdSetCullMode"}
        Rejection{functionName: "vkCmdSetFrontFace"}
        Rejection{functionName: "vkCmdSetPrimitiveTopology"}
        Rejection{functionName: "vkCmdBindVertexBuffers2"}
        Rejection{functionName: "vkCmdSetDepthTestEnable"}
        Rejection{functionName: "vkCmdSetDepthWriteEnable"}
        Rejection{functionName: "vkCmdSetDepthCompareOp"}
        Rejection{functionName: "vkCmdSetDepthBoundsTestEnable"}
        Rejection{functionName: "vkCmdSetStencilTestEnable"}
        Rejection{functionName: "vkCmdSetStencilOp"}
        Rejection{functionName: "vkCmdSetRasterizerDiscardEnable"}
        Rejection{functionName: "vkCmdSetDepthBiasEnable"}
        Rejection{functionName: "vkCmdSetPrimitiveRestartEnable"}
        Rejection{functionName: "vkCmdEndRendering"}
        Rejection{functionName: "vkCmdSetLineStipple"}
        Rejection{functionName: "vkCmdBindIndexBuffer2"}
    }
    ObjectType{
        name: "QVulkanWindow"
        ExtraIncludes{
            Include{
                fileName: "QtGui/qvulkaninstance.h"
                location: Include.Global
                suppressed: true
            }
        }
        ppCondition: "QT_CONFIG(vulkan) && __has_include(<vulkan/vulkan.h>)"
        ModifyFunction{
            signature: "flags()const"
            rename: "vulkanFlags"
        }
        ModifyFunction{
            signature: "createRenderer()"
            ModifyArgument{
                index: 0
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Java
                }
            }
        }
        Rejection{functionName: "setEnabledFeaturesModifier"}
        Rejection{functionName: "setQueueCreateInfoModifier"}
    }
    InterfaceType{
        name: "QVulkanWindowRenderer"
        ExtraIncludes{
            Include{
                fileName: "QtGui/qvulkaninstance.h"
                location: Include.Global
                suppressed: true
            }
            Include{
                fileName: "QtGui/qvulkanwindow.h"
                location: Include.Global
                suppressed: true
            }
            Include{
                fileName: "QtGui/QVulkanWindow"
                location: Include.Global
                ckeckAvailability: true
            }
        }
        ppCondition: "QT_CONFIG(vulkan) && __has_include(<vulkan/vulkan.h>)"
    }
}
