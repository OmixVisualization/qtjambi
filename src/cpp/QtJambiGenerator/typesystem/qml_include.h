#ifndef INCLUDE_H
#define INCLUDE_H

#include "qml_abstractobject.h"

class Include : public AbstractObject
{
    Q_OBJECT
    QML_ELEMENT
public:
    enum IncludeType {
        Global,
        Local,
        Java
    };
    Q_ENUM(IncludeType)
    explicit Include(QObject *parent = nullptr);
    const QString &getFileName() const;
    void setFileName(const QString &newFileName);

    const QString &getPpCondition() const;
    void setPpCondition(const QString &newPpCondition);

    IncludeType getLocation() const;
    void setLocation(IncludeType newLocation);

    bool getInherited() const;
    void setInherited(bool newInherited);

    bool getSuppressed() const;
    void setSuppressed(bool newSuppressed);

    const QStringList &getRequiredFeatures() const;
    void setRequiredFeatures(const QStringList &newRequiredFeatures);

signals:
    void fileNameChanged();

    void ppConditionChanged();

    void locationChanged();

    void inheritedChanged();

    void suppressedChanged();

    void requiredFeaturesChanged();

private:
    QString fileName;
    QString ppCondition;
    IncludeType location;
    bool inherited = false;
    bool suppressed = false;
    QStringList requiredFeatures;
    Q_PROPERTY(QString fileName READ getFileName WRITE setFileName NOTIFY fileNameChanged)
    //Q_PROPERTY(QString ppCondition READ getPpCondition WRITE setPpCondition NOTIFY ppConditionChanged)
    Q_PROPERTY(IncludeType location READ getLocation WRITE setLocation NOTIFY locationChanged)
    Q_PROPERTY(bool inherited READ getInherited WRITE setInherited NOTIFY inheritedChanged)
    Q_PROPERTY(bool suppressed READ getSuppressed WRITE setSuppressed NOTIFY suppressedChanged)
    Q_PROPERTY(QStringList requiredFeatures READ getRequiredFeatures WRITE setRequiredFeatures NOTIFY requiredFeaturesChanged)
};

class ExtraIncludes : public AbstractObject{
    Q_OBJECT
    QML_ELEMENT
public:
    explicit ExtraIncludes(QObject *parent = nullptr);

};

QML_DECLARE_TYPE(Include)
QML_DECLARE_TYPE(ExtraIncludes)

Q_DECLARE_METATYPE(Include::IncludeType)

#endif // INCLUDE_H
