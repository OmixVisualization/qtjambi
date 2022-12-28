#include "docmodel.h"

DocModel::DocModel(): QObject(),
    m_namespaces() {}

bool DocModel::isEmpty() const{
    return m_namespaces.isEmpty();
}

void DocModel::addNamespace(DocNamespace* ns){
    Q_ASSERT(ns);
    m_namespaces.insert(ns->fullName(), ns);
}

DocNamespace* DocModel::getNamespace(const QString& name){
    return m_namespaces[name];
}

const DocNamespace* DocModel::getNamespace(const QString& name) const{
    return m_namespaces[name];
}

QList<QString> DocModel::namespaces() const {
    return m_namespaces.keys();
}

void DocModel::addClass(const DocClass* cls){
    Q_ASSERT(cls);
    m_classes.insert(cls->fullName(), cls);
}

void DocModel::addEnum(const DocEnum* enm){
    Q_ASSERT(enm);
    m_enums[enm->fullName()] = enm;
}

const DocClass* DocModel::getClass(const QString& name) const{
    return m_classes[name];
}

const DocEnum* DocModel::getEnum(const QString& name) const{
    return m_enums[name];
}

QList<QString> DocModel::classes() const{
    return m_classes.keys();
}

const QString& DocModel::url() const{
    return m_url;
}

void DocModel::setUrl(const QString& url){
    m_url = url;
}
