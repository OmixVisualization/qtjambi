#include "docfunction.h"

DocFunction::DocFunction(QObject* parent):DocElement(parent)
{

}

const QString& DocFunction::name() const {
    return m_name;
}

const QString& DocFunction::type() const {
    return m_type;
}

const QStringList& DocFunction::parameters() const {
    return m_parameters;
}

bool DocFunction::isConst() const {
    return m_const;
}

bool DocFunction::isStatic() const {
    return m_static;
}

void DocFunction::setName(const QString& name) {
    m_name = name;
}

void DocFunction::setType(const QString& type) {
    m_type = type;
}

void DocFunction::setParameters(const QStringList& parameters) {
    m_parameters = parameters;
}

void DocFunction::setConst(bool _const) {
    m_const = _const;
}

void DocFunction::setStatic(bool _static) {
    m_static = _static;
}

