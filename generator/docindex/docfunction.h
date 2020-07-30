#ifndef DOCFUNCTION_H
#define DOCFUNCTION_H

#include "docelement.h"

class DocFunction: public DocElement
{
    Q_OBJECT
public:
    DocFunction(QObject* parent);

    const QString& name() const;
    const QString& type() const;
    const QStringList& parameters() const;
    bool isConst() const;
    bool isStatic() const;

    void setName(const QString& name);
    void setType(const QString& type);
    void setParameters(const QStringList& parameters);
    void setConst(bool _const);
    void setStatic(bool _static);
private:
    QString m_name;
    QString m_type;
    QStringList m_parameters;
    bool m_const;
    bool m_static;
};

#endif // DOCFUNCTION_H
