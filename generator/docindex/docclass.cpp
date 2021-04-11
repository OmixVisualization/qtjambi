#include "docclass.h"

DocClass::DocClass(QObject* parent):DocElement(parent),
    m_variables(),
    m_properties(),
    m_functions()
{

}

void DocClass::addFunction(const DocFunction* fct){
    Q_ASSERT(fct);
    m_functions.insert(fct->name(), fct);
}

void DocClass::addVariable(const DocVariable* var){
    Q_ASSERT(var);
    m_variables[var->name()] = var;
}

void DocClass::addProperty(const DocProperty* prop){
    Q_ASSERT(prop);
    m_properties[prop->name()] = prop;
}

void DocClass::addTypeDef(const DocTypeDef* def){
    Q_ASSERT(def);
    m_typeDefs[def->name()] = def;
}

QList<const DocFunction*> DocClass::getFunctions(const QString& name) const{
    return m_functions.values(name);
}

const DocVariable* DocClass::getVariable(const QString& name) const{
    return m_variables[name];
}

const DocProperty* DocClass::getProperty(const QString& name) const{
    return m_properties[name];
}

const DocTypeDef* DocClass::getTypeDef(const QString& name) const{
    return m_typeDefs[name];
}

