/****************************************************************************
**
** Copyright (C) 2009-2025 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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

#ifndef QTJAMBIGUIRHI_HASHES_H
#define QTJAMBIGUIRHI_HASHES_H

#include <QtCore/QtCore>
#include <QtGui/rhi/qrhi.h>
#include <QtJambi/Global>

inline size_t qHash(const QShaderVersion& value, size_t seed = 0){
    return qHashMulti(seed, value.version(), value.flags());
}

inline size_t qHash(const QShaderDescription::BlockVariable& value, size_t seed = 0){
    return qHashMulti(seed, value.name, value.type, value.offset, value.size, value.arrayDims, value.arrayStride, value.matrixStride, value.matrixIsRowMajor, value.structMembers);
}

inline size_t qHash(const QShaderDescription::InOutVariable& value, size_t seed = 0){
    return qHashMulti(seed, value.name, value.type, value.location, value.binding, value.descriptorSet, value.imageFormat, value.imageFlags, value.arrayDims, value.perPatch, value.structMembers);
}

inline size_t qHash(const QShaderDescription::UniformBlock& value, size_t seed = 0){
    return qHashMulti(seed, value.blockName, value.structName, value.size, value.binding, value.descriptorSet, value.members);
}

inline size_t qHash(const QShaderDescription::PushConstantBlock& value, size_t seed = 0){
    return qHashMulti(seed, value.name, value.size, value.members);
}

inline size_t qHash(const QShaderDescription::StorageBlock& value, size_t seed = 0){
    return qHashMulti(seed, value.blockName, value.instanceName, value.knownSize, value.binding, value.descriptorSet, value.members, value.runtimeArrayStride, value.qualifierFlags);
}

inline size_t qHash(const QShaderDescription::BuiltinVariable& value, size_t seed = 0){
    return qHashMulti(seed, value.type, value.varType, value.arrayDims);
}

inline size_t qHash(const QShaderDescription& value, size_t seed = 0){
    return qHashMulti(seed,
                      value.inputVariables(),
                      value.outputVariables(),
                      value.uniformBlocks(),
                      value.pushConstantBlocks(),
                      value.storageBlocks(),
                      value.combinedImageSamplers(),
                      value.separateImages(),
                      value.separateSamplers(),
                      value.storageImages() ,
                      value.inputBuiltinVariables(),
                      value.outputBuiltinVariables(),
                      qHashRange(value.computeShaderLocalSize().begin(), value.computeShaderLocalSize().end()),
                      value.tessellationOutputVertexCount(),
                      value.tessellationMode(),
                      value.tessellationWindingOrder(),
                      value.tessellationPartitioning());
}

#endif // QTJAMBIGUIRHI_HASHES_H
