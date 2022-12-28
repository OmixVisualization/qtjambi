#include "qml_injectcode.h"

InjectCode::InjectCode(QObject *parent)
    : AbstractObject{parent}
{

}

const QString &InjectCode::getPackageName() const
{
    return packageName;
}

void InjectCode::setPackageName(const QString &newPackageName)
{
    if (packageName == newPackageName)
        return;
    packageName = newPackageName;
    emit packageNameChanged();
}

CodeClass::Entries InjectCode::getTarget() const
{
    return target;
}

void InjectCode::setTarget(CodeClass::Entries newTarget)
{
    if (target == newTarget)
        return;
    target = newTarget;
    emit targetChanged();
}

Position::Entries InjectCode::getPosition() const
{
    return position;
}

void InjectCode::setPosition(Position::Entries newPosition)
{
    if (position == newPosition)
        return;
    position = newPosition;
    emit positionChanged();
}

ArgumentMap::ArgumentMap(QObject *parent)
    : AbstractObject{parent}
{

}

int ArgumentMap::getIndex() const
{
    return index;
}

void ArgumentMap::setIndex(int newIndex)
{
    if (index == newIndex)
        return;
    index = newIndex;
    emit indexChanged();
}

const QString &ArgumentMap::getMetaName() const
{
    return metaName;
}

void ArgumentMap::setMetaName(const QString &newMetaName)
{
    if (metaName == newMetaName)
        return;
    metaName = newMetaName;
    emit metaNameChanged();
}

Text::Text(QObject *parent)
    : AbstractObject{parent}
{

}

const QString &Text::getContent() const
{
    return content;
}

void Text::setContent(const QString &newContent)
{
    if (content == newContent)
        return;
    content = newContent;
    emit contentChanged();
}

const QString &ImportFile::getQuoteAfterLine() const
{
    return quoteAfterLine;
}

void ImportFile::setQuoteAfterLine(const QString &newQuoteAfterLine)
{
    if (quoteAfterLine == newQuoteAfterLine)
        return;
    quoteAfterLine = newQuoteAfterLine;
    emit quoteAfterLineChanged();
}

const QString &ImportFile::getQuoteBeforeLine() const
{
    return quoteBeforeLine;
}

void ImportFile::setQuoteBeforeLine(const QString &newQuoteBeforeLine)
{
    if (quoteBeforeLine == newQuoteBeforeLine)
        return;
    quoteBeforeLine = newQuoteBeforeLine;
    emit quoteBeforeLineChanged();
}

const QString &ImportFile::getName() const
{
    return name;
}

void ImportFile::setName(const QString &newName)
{
    if (name == newName)
        return;
    name = newName;
    emit nameChanged();
}
