#ifndef TS_LOADTYPESYSTEM_H
#define TS_LOADTYPESYSTEM_H

#include "qml_abstractobject.h"

class LoadTypeSystem : public AbstractObject
{
    Q_OBJECT
    QML_ELEMENT
public:
    explicit LoadTypeSystem(QObject *parent = nullptr);
    const QString &getName() const;
    void setName(const QString &newName);

    bool getGenerate() const;
    void setGenerate(bool newGenerate);

    bool getOptional() const;
    void setOptional(bool newOptional);

signals:
    void nameChanged();

    void generateChanged();

    void optionalChanged();

private:
    QString name;
    bool generate = false;
    bool optional = false;
    Q_PROPERTY(QString name READ getName WRITE setName NOTIFY nameChanged REQUIRED)
    Q_PROPERTY(bool generate READ getGenerate WRITE setGenerate NOTIFY generateChanged)
    Q_PROPERTY(bool optional READ getOptional WRITE setOptional NOTIFY optionalChanged)
};

QML_DECLARE_TYPE(LoadTypeSystem)

#endif // TS_LOADTYPESYSTEM_H
