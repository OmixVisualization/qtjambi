#ifndef DOCVARIABLE_H
#define DOCVARIABLE_H

#include "docelement.h"

class DocVariable : public DocElement
{
    Q_OBJECT
public:
    DocVariable(QObject* parent);

    const QString& name() const;
    const QString& type() const;
    bool isStatic() const;

    void setName(const QString& name);
    void setType(const QString& type);
    void setStatic(bool _static);
private:
    QString m_name;
    QString m_type;
    bool m_static;
};

#endif // DOCVARIABLE_H
