#ifndef DOCVARIABLE_H
#define DOCVARIABLE_H

#include "docelement.h"

class DocVariable : public DocElement
{
    Q_OBJECT
public:
    DocVariable(QObject* parent);

    const QString& type() const;
    bool isStatic() const;

    void setType(const QString& type);
    void setStatic(bool _static);
private:
    QString m_type;
    bool m_static;
};

#endif // DOCVARIABLE_H
