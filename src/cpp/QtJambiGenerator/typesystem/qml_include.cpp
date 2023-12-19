#include "qml_include.h"

Include::Include(QObject *parent)
    : AbstractObject{parent}
{

}
const QString &Include::getFileName() const
{
    return fileName;
}

void Include::setFileName(const QString &newFileName)
{
    if (fileName == newFileName)
        return;
    fileName = newFileName;
    emit fileNameChanged();
}

const QString &Include::getPpCondition() const
{
    return ppCondition;
}

void Include::setPpCondition(const QString &newPpCondition)
{
    if (ppCondition == newPpCondition)
        return;
    ppCondition = newPpCondition;
    emit ppConditionChanged();
}

Include::IncludeType Include::getLocation() const
{
    return location;
}

void Include::setLocation(IncludeType newLocation)
{
    if (location == newLocation)
        return;
    location = newLocation;
    emit locationChanged();
}

bool Include::getInherited() const
{
    return inherited;
}

void Include::setInherited(bool newInherited)
{
    if (inherited == newInherited)
        return;
    inherited = newInherited;
    emit inheritedChanged();
}

bool Include::getSuppressed() const
{
    return suppressed;
}

void Include::setSuppressed(bool newSuppressed)
{
    if (suppressed == newSuppressed)
        return;
    suppressed = newSuppressed;
    emit suppressedChanged();
}

const QStringList &Include::getRequiredFeatures() const
{
    return requiredFeatures;
}

void Include::setRequiredFeatures(const QStringList &newRequiredFeatures)
{
    if (requiredFeatures == newRequiredFeatures)
        return;
    requiredFeatures = newRequiredFeatures;
    emit requiredFeaturesChanged();
}

bool Include::getCkeckAvailability() const
{
    return ckeckAvailability;
}

void Include::setCkeckAvailability(bool newCkeckAvailability)
{
    if (ckeckAvailability == newCkeckAvailability)
        return;
    ckeckAvailability = newCkeckAvailability;
    emit ckeckAvailabilityChanged();
}

ExtraIncludes::ExtraIncludes(QObject *parent)
    : AbstractObject{parent}
{

}
