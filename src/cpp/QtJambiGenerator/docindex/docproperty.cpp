#include "docproperty.h"

DocProperty::DocProperty(QObject* parent) : DocElement(parent)
{

}

const QString& DocProperty::name() const {
    return m_name;
}

const QString& DocProperty::type() const {
    return m_type;
}

const QString& DocProperty::setter() const {
    return m_setter;
}

const QString& DocProperty::getter() const {
    return m_getter;
}

void DocProperty::setName(const QString& name) {
    m_name = name;
}

void DocProperty::setType(const QString& type) {
    m_type = type;
}

void DocProperty::setSetter(const QString& setter) {
    m_setter = setter;
}

void DocProperty::setGetter(const QString& getter) {
    m_getter = getter;
}
