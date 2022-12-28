#ifndef QDOCMODEL_H
#define QDOCMODEL_H

#include <QObject>
#include "docclass.h"
#include "docnamespace.h"

class DocNamespace;
class DocEnum;
class DocClass;

class DocModel: public QObject
{
public:
    DocModel();
    void addNamespace(DocNamespace*);
    DocNamespace* getNamespace(const QString& name);
    const DocNamespace* getNamespace(const QString& name) const;
    QList<QString> namespaces() const;
    bool isEmpty() const;
    void addClass(const DocClass*);
    void addEnum(const DocEnum*);
    const DocClass* getClass(const QString& name) const;
    const DocEnum* getEnum(const QString& name) const;
    QList<QString> classes() const;
    const QString& url() const;
    void setUrl(const QString& url);
private:
    QString m_url;
    QMap<QString,DocNamespace*> m_namespaces;
    QMap<QString,const DocEnum*> m_enums;
    QMap<QString,const DocClass*> m_classes;
};

#endif // QDOCMODEL_H
