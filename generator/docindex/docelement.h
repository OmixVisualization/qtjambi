#ifndef DOCELEMENT_H
#define DOCELEMENT_H

#include <QtCore>

class DocElement: public QObject
{
    Q_OBJECT
public:
    DocElement(QObject* parent);

    const QString& name() const;
    const QString& fullName() const;
    const QString& href() const;
    const QString& brief() const;
    const QDir& subdir() const;
    void setName(const QString& name);
    void setFullName(const QString& fullName);
    void setHref(const QString& href);
    void setBrief(const QString& brief);
    void setSubdir(const QDir& subdir);
private:
    QString m_name;
    QString m_fullName;
    QString m_href;
    QString m_brief;
    QDir m_subdir;
};

#endif // DOCELEMENT_H
