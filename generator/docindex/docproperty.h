#ifndef DOCPROPERTY_H
#define DOCPROPERTY_H

#include "docelement.h"

class DocProperty : public DocElement
{
public:
    DocProperty(QObject* parent);

    const QString& name() const;
    const QString& type() const;
    const QString& setter() const;
    const QString& getter() const;

    void setName(const QString& name);
    void setType(const QString& type);
    void setSetter(const QString& setter);
    void setGetter(const QString& getter);
private:
    QString m_name;
    QString m_type;
    QString m_setter;
    QString m_getter;
};

#endif // DOCPROPERTY_H
