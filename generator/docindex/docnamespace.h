#ifndef DOCNAMESPACE_H
#define DOCNAMESPACE_H

#include "docmodel.h"
#include "docclass.h"
#include "docenum.h"
#include "docfunction.h"

class DocNamespace: public DocElement
{
    Q_OBJECT
public:
    DocNamespace(QObject* parent);
    void addFunction(const DocFunction*);
    QList<const DocFunction*> getFunctions(const QString& name) const;
private:
    QMultiHash<QString,const DocFunction*> m_functions;
};

#endif // DOCNAMESPACE_H
