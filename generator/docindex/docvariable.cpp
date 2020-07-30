#include "docvariable.h"

DocVariable::DocVariable(QObject* parent) : DocElement(parent), m_static(false)
{

}

const QString& DocVariable::name() const {
    return m_name;
}

const QString& DocVariable::type() const {
    return m_type;
}

bool DocVariable::isStatic() const {
    return m_static;
}

void DocVariable::setName(const QString& name) {
    m_name = name;
}

void DocVariable::setType(const QString& type) {
    m_type = type;
}

void DocVariable::setStatic(bool _static) {
    m_static = _static;
}

