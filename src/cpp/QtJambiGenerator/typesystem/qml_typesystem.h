#ifndef TS_TYPESYSTEM_H
#define TS_TYPESYSTEM_H

#include "qml_abstractobject.h"

class TypeSystem : public AbstractObject
{
    Q_OBJECT
    QML_ELEMENT
public:
    explicit TypeSystem(QObject *parent = nullptr);
    const QString &getPackageName() const;
    void setPackageName(const QString &newPackageName);

    const QString &getDefaultSuperClass() const;
    void setDefaultSuperClass(const QString &newDefaultSuperClass);

    const QString &getQtLibrary() const;
    void setQtLibrary(const QString &newQtLibrary);

    bool getNoExports() const;
    void setNoExports(bool newNoExports);

    const QString &getModule() const;
    void setModule(const QString &newModule);

    const QString &getDescription() const;
    void setDescription(const QString &newDescription);

    const QString &getTargetName() const;
    void setTargetName(const QString &newTargetName);

    QString getDefaultPPCondition() const;
    void setDefaultPPCondition(const QString &newDefaultPPCondition);

signals:
    void packageNameChanged();

    void defaultSuperClassChanged();

    void qtLibraryChanged();

    void noExportsChanged();

    void moduleChanged();

    void descriptionChanged();

    void targetNameChanged();

    void defaultPPConditionChanged();

private:
    QString packageName;
    QString targetName;
    QString defaultSuperClass;
    QString qtLibrary;
    bool noExports = false;
    QString module;
    QString description;
    QString defaultPPCondition;
    Q_PROPERTY(QString packageName READ getPackageName WRITE setPackageName NOTIFY packageNameChanged)
    Q_PROPERTY(QString defaultSuperClass READ getDefaultSuperClass WRITE setDefaultSuperClass NOTIFY defaultSuperClassChanged)
    Q_PROPERTY(QString qtLibrary READ getQtLibrary WRITE setQtLibrary NOTIFY qtLibraryChanged)
    Q_PROPERTY(bool noExports READ getNoExports WRITE setNoExports NOTIFY noExportsChanged)
    Q_PROPERTY(QString module READ getModule WRITE setModule NOTIFY moduleChanged)
    Q_PROPERTY(QString description READ getDescription WRITE setDescription NOTIFY descriptionChanged)
    Q_PROPERTY(QString targetName READ getTargetName WRITE setTargetName NOTIFY targetNameChanged)
    Q_PROPERTY(QString defaultPPCondition READ getDefaultPPCondition WRITE setDefaultPPCondition NOTIFY defaultPPConditionChanged FINAL)
};

class Module : public AbstractObject{
    Q_OBJECT
    QML_ELEMENT
public:
    explicit Module(QObject *parent = nullptr);
    const QString &getName() const;
    void setName(const QString &newName);

signals:
    void nameChanged();

private:
    QString name;
    Q_PROPERTY(QString name READ getName WRITE setName NOTIFY nameChanged)
};

class Description : public AbstractObject{
    Q_OBJECT
    QML_ELEMENT
public:
    explicit Description(QObject *parent = nullptr);

    const QString &getText() const;
    void setText(const QString &newText);

signals:
    void textChanged();

private:
    QString text;
    Q_PROPERTY(QString text READ getText WRITE setText NOTIFY textChanged)
};

class RequiredLibrary : public AbstractObject{
    Q_OBJECT
    QML_ELEMENT
public:
    enum Mode{
        Mandatory,
        Optional,
        ProvideOnly
    };
    Q_ENUM(Mode)
    explicit RequiredLibrary(QObject *parent = nullptr):AbstractObject(parent){}
    const QString &getName() const;
    void setName(const QString &newText);

    Mode getMode() const;
    void setMode(Mode newMode);

    const QStringList &getPlatforms() const;
    void setPlatforms(const QStringList &newPlatforms);

signals:
    void nameChanged();

    void modeChanged();

    void platformsChanged();

private:
    QString name;
    Mode mode = Mandatory;
    QStringList platforms;
    Q_PROPERTY(QString name READ getName WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(RequiredLibrary::Mode mode READ getMode WRITE setMode NOTIFY modeChanged)
    Q_PROPERTY(QStringList platforms READ getPlatforms WRITE setPlatforms NOTIFY platformsChanged)
};

class RequiredPackage : public AbstractObject{
    Q_OBJECT
    QML_ELEMENT
public:
    explicit RequiredPackage(QObject* parent = nullptr):AbstractObject(parent){}
    const QString &getName() const;
    void setName(const QString &newName);

signals:
    void nameChanged();

private:
    QString name;
    Q_PROPERTY(QString name READ getName WRITE setName NOTIFY nameChanged)
};

class ForwardDeclaration : public AbstractObject{
    Q_OBJECT
    QML_ELEMENT
public:
    explicit ForwardDeclaration(QObject *parent = nullptr):AbstractObject(parent){}

    const QString &getClassName() const;
    void setClassName(const QString &newCodeClass);

signals:
    void codeClassChanged();

private:
    QString className;
    Q_PROPERTY(QString className READ getClassName WRITE setClassName NOTIFY codeClassChanged)
};

class NamespacePrefix : public AbstractObject{
    Q_OBJECT
    QML_ELEMENT
public:
    enum NamingPolicy{
        Cut,
        Preserve
    };
    Q_ENUM(NamingPolicy)
    explicit NamespacePrefix(QObject *parent = nullptr):AbstractObject(parent){}

    const QString &getPrefix() const;
    void setPrefix(const QString &newPrefix);

    const QString &getNamespace() const;
    void setNamespace(const QString &newNamespace);

    NamingPolicy getNamingPolicy() const;
    void setNamingPolicy(NamingPolicy newNamingPolicy);

signals:
    void prefixChanged();

    void namespaceChanged();

    void namingPolicyChanged();

private:
    QString prefix;
    QString m_namespace;
    NamingPolicy namingPolicy = NamingPolicy::Preserve;
    Q_PROPERTY(QString prefix READ getPrefix WRITE setPrefix NOTIFY prefixChanged)
    Q_PROPERTY(QString namespace READ getNamespace WRITE setNamespace NOTIFY namespaceChanged)
    Q_PROPERTY(NamingPolicy namingPolicy READ getNamingPolicy WRITE setNamingPolicy NOTIFY namingPolicyChanged)
};

class Rejection : public AbstractObject{
    Q_OBJECT
    QML_ELEMENT
public:
    explicit Rejection(QObject *parent = nullptr):AbstractObject(parent){}
    const QString &getClassName() const;
    void setClassName(const QString &newClassName);

    const QString &getFunctionName() const;
    void setFunctionName(const QString &newFunctionName);

    const QString &getFieldName() const;
    void setFieldName(const QString &newFieldName);

    const QString &getEnumName() const;
    void setEnumName(const QString &newEnumName);

signals:
    void classNameChanged();

    void functionNameChanged();

    void fieldNameChanged();

    void enumNameChanged();

private:
    QString className = "*";
    QString functionName = "";
    QString fieldName = "";
    QString enumName = "";
    Q_PROPERTY(QString className READ getClassName WRITE setClassName NOTIFY classNameChanged)
    Q_PROPERTY(QString functionName READ getFunctionName WRITE setFunctionName NOTIFY functionNameChanged)
    Q_PROPERTY(QString fieldName READ getFieldName WRITE setFieldName NOTIFY fieldNameChanged)
    Q_PROPERTY(QString enumName READ getEnumName WRITE setEnumName NOTIFY enumNameChanged)
};

QML_DECLARE_TYPE(TypeSystem)
QML_DECLARE_TYPE(Module)
QML_DECLARE_TYPE(NamespacePrefix)
QML_DECLARE_TYPE(Description)
QML_DECLARE_TYPE(ForwardDeclaration)
QML_DECLARE_TYPE(Rejection)
QML_DECLARE_TYPE(RequiredPackage)
QML_DECLARE_TYPE(RequiredLibrary)

Q_DECLARE_METATYPE(RequiredLibrary::Mode)

#endif // TS_TYPESYSTEM_H
