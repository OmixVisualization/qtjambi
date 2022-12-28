#include "qml_template.h"

Template::Template(QObject *parent)
    : AbstractObject{parent}
{

}

const QString &Template::getName() const
{
    return name;
}

void Template::setName(const QString &newName)
{
    if (name == newName)
        return;
    name = newName;
    emit nameChanged();
}

TemplateArguments::TemplateArguments(QObject *parent)
    : AbstractObject{parent}
{

}

const QStringList &TemplateArguments::getArguments() const
{
    return arguments;
}

void TemplateArguments::setArguments(const QStringList &newArguments)
{
    if (arguments == newArguments)
        return;
    arguments = newArguments;
    emit argumentsChanged();
}

const QString &InsertTemplate::getName() const
{
    return name;
}

void InsertTemplate::setName(const QString &newName)
{
    if (name == newName)
        return;
    name = newName;
    emit nameChanged();
}

const QString &Replace::getFrom() const
{
    return from;
}

void Replace::setFrom(const QString &newFrom)
{
    if (from == newFrom)
        return;
    from = newFrom;
    emit fromChanged();
}

const QString &Replace::getTo() const
{
    return to;
}

void Replace::setTo(const QString &newTo)
{
    if (to == newTo)
        return;
    to = newTo;
    emit toChanged();
}
