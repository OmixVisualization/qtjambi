
#include <qanimationclipdata.h>
#include <qchannel.h>

namespace Qt3DAnimation {

bool operator==(const QChannelComponent &lhs, const QChannelComponent &rhs) Q_DECL_NOTHROW
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

bool operator==(const QChannel &lhs, const QChannel &rhs) Q_DECL_NOTHROW
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

bool operator==(const QAnimationClipData &lhs, const QAnimationClipData &rhs) Q_DECL_NOTHROW
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
}
