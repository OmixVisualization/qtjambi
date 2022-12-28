#ifndef SUPPRESSEDWARNING_H
#define SUPPRESSEDWARNING_H

#include "qml_abstractobject.h"

class SuppressedWarning : public AbstractObject
{
    Q_OBJECT
    QML_ELEMENT
public:
    explicit SuppressedWarning(QObject *parent = nullptr);
    const QString &getText() const;
    void setText(const QString &newText);

signals:
    void textChanged();

private:
    QString text;
    Q_PROPERTY(QString text READ getText WRITE setText NOTIFY textChanged)
};

QML_DECLARE_TYPE(SuppressedWarning)

#endif // SUPPRESSEDWARNING_H
