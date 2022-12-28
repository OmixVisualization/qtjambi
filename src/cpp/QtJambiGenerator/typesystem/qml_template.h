#ifndef TEMPLATE_H
#define TEMPLATE_H

#include "qml_abstractobject.h"

class Template : public AbstractObject
{
    Q_OBJECT
    QML_ELEMENT
public:
    explicit Template(QObject *parent = nullptr);
    const QString &getName() const;
    void setName(const QString &newName);

signals:
    void nameChanged();

private:
    QString name;
    Q_PROPERTY(QString name READ getName WRITE setName NOTIFY nameChanged)
};

class TemplateArguments : public AbstractObject{
    Q_OBJECT
    QML_ELEMENT
public:
    explicit TemplateArguments(QObject *parent = nullptr);
    const QStringList &getArguments() const;
    void setArguments(const QStringList &newArguments);

signals:
    void argumentsChanged();

private:
    QStringList arguments;
    Q_PROPERTY(QStringList arguments READ getArguments WRITE setArguments NOTIFY argumentsChanged)
};

class InsertTemplate : public AbstractObject
{
    Q_OBJECT
    QML_ELEMENT
public:
    explicit InsertTemplate(QObject *parent = nullptr):AbstractObject(parent){}
    const QString &getName() const;
    void setName(const QString &newName);

signals:
    void nameChanged();

private:
    QString name;
    Q_PROPERTY(QString name READ getName WRITE setName NOTIFY nameChanged)
};

class Replace : public AbstractObject
{
    Q_OBJECT
    QML_ELEMENT
public:
    explicit Replace(QObject *parent = nullptr):AbstractObject(parent){}
    const QString &getFrom() const;
    void setFrom(const QString &newFrom);

    const QString &getTo() const;
    void setTo(const QString &newTo);

signals:
    void fromChanged();

    void toChanged();

private:
    QString from;
    QString to;
    Q_PROPERTY(QString from READ getFrom WRITE setFrom NOTIFY fromChanged)
    Q_PROPERTY(QString to READ getTo WRITE setTo NOTIFY toChanged)
};

QML_DECLARE_TYPE(Template)
QML_DECLARE_TYPE(InsertTemplate)
QML_DECLARE_TYPE(TemplateArguments)
QML_DECLARE_TYPE(Replace)

#endif // TEMPLATE_H
