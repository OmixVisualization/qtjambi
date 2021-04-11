#ifndef QTJAMBI_3DANIMATION_HASHES_H
#define QTJAMBI_3DANIMATION_HASHES_H

#include <QtCore/QtGlobal>
#include <qtjambi/qtjambi_core.h>
#include <qtjambi_gui/qtjambi_gui_qhashes.h>
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
}

inline hash_type qHash(const Qt3DAnimation::QKeyFrame &value)
{
    hash_type hashCode = qHash(value.coordinates());
    hashCode = hashCode * 31 + qHash(value.interpolationType());
    hashCode = hashCode * 31 + qHash(value.leftControlPoint());
    hashCode = hashCode * 31 + qHash(value.rightControlPoint());
    return hashCode;
}

inline hash_type qHash(const Qt3DAnimation::QChannelComponent &value)
{
    hash_type hashCode = qHash(value.name());
    hashCode = hashCode * 31 + qHash(value.keyFrameCount());
    for(const Qt3DAnimation::QKeyFrame & f : value){
        hashCode = hashCode * 31 + qHash(f);
    }
    return hashCode;
}

inline hash_type qHash(const Qt3DAnimation::QChannel &value)
{
    hash_type hashCode = qHash(value.name());
    hashCode = hashCode * 31 + qHash(value.channelComponentCount());
    for(const Qt3DAnimation::QChannelComponent & c : value){
        hashCode = hashCode * 31 + qHash(c);
    }
    return hashCode;
}

inline hash_type qHash(const Qt3DAnimation::QAnimationClipData &value)
{
    hash_type hashCode = qHash(value.name());
    hashCode = hashCode * 31 + qHash(value.channelCount());
    for(const Qt3DAnimation::QChannel & c : value){
        hashCode = hashCode * 31 + qHash(c);
    }
    return hashCode;
}

#endif // QTJAMBI_3DANIMATION_HASHES_H
