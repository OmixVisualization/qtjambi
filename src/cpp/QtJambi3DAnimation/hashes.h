/****************************************************************************
**
** Copyright (C) 2009-2023 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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


#ifndef QTJAMBI3DANIMATION_HASHES_H
#define QTJAMBI3DANIMATION_HASHES_H

#include <QtCore/QtGlobal>
#include <QtJambi/QtJambiAPI>
#include <QtJambiGui/hashes.h>
#include <qanimationclipdata.h>
#include <qchannel.h>

namespace Qt3DAnimation {

inline bool operator==(const QChannelComponent &lhs, const QChannelComponent &rhs) Q_DECL_NOTHROW
{
    if(lhs.name() == rhs.name()
            && lhs.keyFrameCount() == rhs.keyFrameCount()){
        QChannelComponent::const_iterator liter = lhs.begin();
        QChannelComponent::const_iterator riter = rhs.begin();
        QChannelComponent::const_iterator lend = lhs.end();
        QChannelComponent::const_iterator rend = rhs.end();
        for(; liter!=lend && riter!=rend; ++liter, ++riter){
            if(!(*liter==*riter))
                return false;
        }
        return true;
    }
    return false;
}

inline bool operator==(const QChannel &lhs, const QChannel &rhs) Q_DECL_NOTHROW
{
    if(lhs.name() == rhs.name()
            && lhs.channelComponentCount() == rhs.channelComponentCount()){
        QChannel::const_iterator liter = lhs.begin();
        QChannel::const_iterator riter = rhs.begin();
        QChannel::const_iterator lend = lhs.end();
        QChannel::const_iterator rend = rhs.end();
        for(; liter!=lend && riter!=rend; ++liter, ++riter){
            if(!(*liter==*riter))
                return false;
        }
        return true;
    }
    return false;
}

#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
inline bool operator==(const QAnimationClipData &lhs, const QAnimationClipData &rhs) Q_DECL_NOTHROW
{
    if(lhs.name() == rhs.name()
            && lhs.channelCount() == rhs.channelCount()){
        QAnimationClipData::const_iterator liter = lhs.begin();
        QAnimationClipData::const_iterator riter = rhs.begin();
        QAnimationClipData::const_iterator lend = lhs.end();
        QAnimationClipData::const_iterator rend = rhs.end();
        for(; liter!=lend && riter!=rend; ++liter, ++riter){
            if(!(*liter==*riter))
                return false;
        }
        return true;
    }
    return false;
}
#endif

#ifndef QTJAMBI_GENERATOR_RUNNING
inline hash_type qHash(const QKeyFrame &value, hash_type seed = 0)
{
    QtPrivate::QHashCombine hash;
    seed = hash(seed, value.coordinates());
    seed = hash(seed, value.interpolationType());
    seed = hash(seed, value.leftControlPoint());
    seed = hash(seed, value.rightControlPoint());
    return seed;
}

inline hash_type qHash(const QChannelComponent &value, hash_type seed = 0)
{
    QtPrivate::QHashCombineCommutative hash;
    seed = hash(seed, value.name());
    seed = hash(seed, value.keyFrameCount());
    for(const QKeyFrame & f : value){
        seed = hash(seed, f);
    }
    return seed;
}

inline hash_type qHash(const QChannel &value, hash_type seed = 0)
{
    QtPrivate::QHashCombine hash;
    hash_type hashCode = seed;
    seed = hash(seed, value.name());
    hashCode = hashCode * 31 + ::qHash(value.channelComponentCount());
    for(const QChannelComponent & c : value){
        seed = hash(seed, c);
    }
    return hashCode;
}

inline hash_type qHash(const QAnimationClipData &value, hash_type seed = 0)
{
    QtPrivate::QHashCombineCommutative hash;
    seed = hash(seed, value.name());
    seed = hash(seed, value.channelCount());
    for(const QChannel & c : value){
        seed = hash(seed, c);
    }
    return seed;
}

#endif

}

#ifdef QTJAMBI_GENERATOR_RUNNING
hash_type qHash(const Qt3DAnimation::QKeyFrame &value, hash_type seed = 0);
hash_type qHash(const Qt3DAnimation::QChannelComponent &value, hash_type seed = 0);
hash_type qHash(const Qt3DAnimation::QChannel &value, hash_type seed = 0);
hash_type qHash(const Qt3DAnimation::QAnimationClipData &value, hash_type seed = 0);
#endif

#endif // HASHES_H
