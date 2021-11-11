#ifndef DOCCLASS_H
#define DOCCLASS_H

#include "docelement.h"
#include "docenum.h"
#include "docfunction.h"
#include "docvariable.h"
#include "docproperty.h"
#include "doctypedef.h"


class DocClass: public DocElement
{
public:
    DocClass(QObject* parent);

    void addFunction(const DocFunction*);
    void addVariable(const DocVariable*);
    void addProperty(const DocProperty*);
    void addTypeDef(const DocTypeDef*);
    QList<const DocFunction*> getFunctions(const QString& name) const;
    const DocVariable* getVariable(const QString& name) const;
    const DocProperty* getProperty(const QString& name) const;
    const DocTypeDef* getTypeDef(const QString& name) const;
private:
    QHash<QString,const DocVariable*> m_variables;
    QHash<QString,const DocProperty*> m_properties;
    QHash<QString,const DocTypeDef*> m_typeDefs;
    QMultiHash<QString,const DocFunction*> m_functions;
};

#endif // DOCCLASS_H
