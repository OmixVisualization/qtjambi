#include "docelement.h"

DocElement::DocElement(QObject* parent)
    : QObject(parent),
      m_name(),
      m_fullName(),
      m_href(),
      m_brief(),
      m_subdir()
{

}

const QString& DocElement::name() const{
    return m_name;
}

const QString& DocElement::fullName() const{
    return m_fullName.isEmpty() ? m_name : m_fullName;
}

const QString& DocElement::href() const{
    return m_href;
}

const QString& DocElement::brief() const{
    return m_brief;
}

const QDir& DocElement::subdir() const{
    return m_subdir;
}

void DocElement::setName(const QString& name){
    m_name = name;
}

void DocElement::setFullName(const QString& fullName){
    m_fullName = fullName;
}

void DocElement::setHref(const QString& href){
    m_href = href;
}

void DocElement::setBrief(const QString& brief){
    m_brief = brief;
}

void DocElement::setSubdir(const QDir& subdir){
    m_subdir = subdir;
}
