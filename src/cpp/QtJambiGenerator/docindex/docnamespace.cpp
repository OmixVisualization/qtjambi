#include "docnamespace.h"

DocNamespace::DocNamespace(QObject* parent):
    DocElement(parent)
{

}

void DocNamespace::addFunction(const DocFunction* fct){
    Q_ASSERT(fct);
    for(const DocFunction* f : m_functions.values(fct->name())){
        if(f->isConst()==fct->isConst()
                && f->isStatic()==fct->isStatic()
                && f->type()==fct->type()
                && f->parameters()==fct->parameters()){
            delete fct;
            return;
        }
    }
    m_functions.insert(fct->name(), fct);
}

QList<const DocFunction*> DocNamespace::getFunctions(const QString& name) const{
    return m_functions.values(name);
}
