#ifndef DOCCLASS_H
#define DOCCLASS_H

#include "docelement.h"
#include "docenum.h"
#include "docfunction.h"
#include "docvariable.h"
#include "docproperty.h"


class DocClass: public DocElement
{
    Q_OBJECT
public:
    DocClass(QObject* parent);

    void addFunction(const DocFunction*);
    void addVariable(const DocVariable*);
    void addProperty(const DocProperty*);
    QList<const DocFunction*> getFunctions(const QString& name) const;
    const DocVariable* getVariable(const QString& name) const;
    const DocProperty* getProperty(const QString& name) const;
private:
    QHash<QString,const DocVariable*> m_variables;
    QHash<QString,const DocProperty*> m_properties;
    QMultiHash<QString,const DocFunction*> m_functions;
};

#endif // DOCCLASS_H
