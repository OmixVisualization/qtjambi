/****************************************************************************
**
** Copyright (C) 2009-2022 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
**
** This file is part of Qt Jambi.
**
** ** $BEGIN_LICENSE$
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

#ifndef QT3DRENDER_H
#define QT3DRENDER_H

#include <Qt3DRender/Qt3DRender>
#include <qtjambi_gui/qtjambi_gui_qhashes.h>

namespace Qt3DRender{
	typedef Qt3DCore::QNode QNode;
};

inline hash_type qHash(const Qt3DRender::QLevelOfDetailBoundingSphere& p){
    hash_type hashCode = qHash(p.center());
    hashCode = hashCode * 31 + qHash(p.radius());
    return hashCode;
}

#if QT_VERSION >= QT_VERSION_CHECK(5,14,0)
inline hash_type qHash(const Qt3DRender::QTextureImageData& p){
    hash_type hashCode = qHash(p.width());
    hashCode = hashCode * 31 + qHash(p.height());
    hashCode = hashCode * 31 + qHash(p.depth());
    hashCode = hashCode * 31 + qHash(p.layers());
    hashCode = hashCode * 31 + qHash(p.mipLevels());
    hashCode = hashCode * 31 + qHash(p.faces());
    hashCode = hashCode * 31 + qHash(p.target());
    hashCode = hashCode * 31 + qHash(p.format());
    hashCode = hashCode * 31 + qHash(p.pixelType());
    hashCode = hashCode * 31 + qHash(p.pixelFormat());
    for(int i=0; i<p.layers(); ++i){
        for(int j=0; j<p.faces(); ++j){
            for(int k=0; k<p.mipLevels(); ++k){
                hashCode = hashCode * 31 + qHash(p.data(i,j,k));
            }
        }
    }
    return hashCode;
}

inline hash_type qHash(const Qt3DRender::QTextureDataUpdate& p){
    hash_type hashCode = qHash(p.x());
    hashCode = hashCode * 31 + qHash(p.y());
    hashCode = hashCode * 31 + qHash(p.z());
    hashCode = hashCode * 31 + qHash(p.layer());
    hashCode = hashCode * 31 + qHash(p.mipLevel());
    hashCode = hashCode * 31 + qHash(p.face());
    if(p.data())
        hashCode = hashCode * 31 + qHash(*p.data().get());
    return hashCode;
}
#endif

#endif // QT3DRENDER_H

