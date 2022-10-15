#ifndef DOCNAMESPACE_H
#define DOCNAMESPACE_H

#include "docmodel.h"
#include "docclass.h"
#include "docenum.h"
#include "docfunction.h"

class DocNamespace: public DocElement
{
public:
    DocNamespace(QObject* parent);
    void addFunction(const DocFunction*);
    QList<const DocFunction*> getFunctions(const QString& name) const;
    const QMultiHash<QString,const DocFunction*>& functions() const{return m_functions;}
private:
    QMultiHash<QString,const DocFunction*> m_functions;
};

#endif // DOCNAMESPACE_H
