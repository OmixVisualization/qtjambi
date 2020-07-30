#include "docnamespace.h"

DocNamespace::DocNamespace(QObject* parent):
    DocElement(parent)
{

}

void DocNamespace::addFunction(const DocFunction* fct){
    Q_ASSERT(fct);
    m_functions.insert(fct->name(), fct);
}

QList<const DocFunction*> DocNamespace::getFunctions(const QString& name) const{
    return m_functions.values(name);
}
