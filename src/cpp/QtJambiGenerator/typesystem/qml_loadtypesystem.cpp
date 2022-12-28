#include "qml_loadtypesystem.h"

LoadTypeSystem::LoadTypeSystem(QObject *parent)
    : AbstractObject{parent}
{

}

const QString &LoadTypeSystem::getName() const
{
    return name;
}

void LoadTypeSystem::setName(const QString &newName)
{
    if (name == newName)
        return;
    name = newName;
    emit nameChanged();
}

bool LoadTypeSystem::getGenerate() const
{
    return generate;
}

void LoadTypeSystem::setGenerate(bool newGenerate)
{
    if (generate == newGenerate)
        return;
    generate = newGenerate;
    emit generateChanged();
}

bool LoadTypeSystem::getOptional() const
{
    return optional;
}

void LoadTypeSystem::setOptional(bool newOptional)
{
    if (optional == newOptional)
        return;
    optional = newOptional;
    emit optionalChanged();
}
