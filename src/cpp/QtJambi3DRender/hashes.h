/****************************************************************************
**
** Copyright (C) 2009-2024 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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

#ifndef QTJAMBI3DRENDER_HASHES_H
#define QTJAMBI3DRENDER_HASHES_H

#include <Qt3DRender/Qt3DRender>
#include <QtJambiGui/hashes.h>

#ifdef QTJAMBI_GENERATOR_RUNNING
namespace Qt3DRender{
    typedef Qt3DCore::QNode QNode;
}
hash_type qHash(const Qt3DRender::QLevelOfDetailBoundingSphere& p, hash_type seed = 0);

#if QT_VERSION >= QT_VERSION_CHECK(5,14,0)
hash_type qHash(const Qt3DRender::QTextureImageData& p, hash_type seed = 0);

hash_type qHash(const Qt3DRender::QTextureDataUpdate& p, hash_type seed = 0);
#endif

#else
namespace Qt3DRender{
	typedef Qt3DCore::QNode QNode;
    inline hash_type qHash(const QLevelOfDetailBoundingSphere& p, hash_type seed = 0){
        QtPrivate::QHashCombine hash;
        seed = hash(seed, p.center());
        seed = hash(seed, p.radius());
        return seed;
    }
#if QT_VERSION >= QT_VERSION_CHECK(5,14,0)
    inline hash_type qHash(const QTextureImageData& p, hash_type seed = 0){
        QtPrivate::QHashCombineCommutative hash;
        seed = hash(seed, p.width());
        seed = hash(seed, p.height());
        seed = hash(seed, p.depth());
        seed = hash(seed, p.layers());
        seed = hash(seed, p.mipLevels());
        seed = hash(seed, p.faces());
        seed = hash(seed, p.target());
        seed = hash(seed, p.format());
        seed = hash(seed, p.pixelType());
        seed = hash(seed, p.pixelFormat());
        for(int i=0; i<p.layers(); ++i){
            for(int j=0; j<p.faces(); ++j){
                for(int k=0; k<p.mipLevels(); ++k){
                    seed = hash(seed, p.data(i,j,k));
                }
            }
        }
        return seed;
    }

    inline hash_type qHash(const QTextureDataUpdate& p, hash_type seed = 0){
        QtPrivate::QHashCombine hash;
        seed = hash(seed, p.x());
        seed = hash(seed, p.y());
        seed = hash(seed, p.z());
        seed = hash(seed, p.layer());
        seed = hash(seed, p.mipLevel());
        seed = hash(seed, p.face());
        if(p.data())
            seed = hash(seed, *p.data().get());
        return seed;
    }
#endif
};
#endif

#endif // QTJAMBI3DRENDER_HASHES_H

