#ifndef QML_ABSTRACTOBJECT_H
#define QML_ABSTRACTOBJECT_H

#include <QtQml>

struct Ownership : public QObject{
    Q_OBJECT
    QML_ELEMENT
    QML_UNCREATABLE("enum carrier")
public:
    enum Entries {
        Invalid,
        Default,
        Java,
        Cpp,
        Ignore,
        Dependent,
        Invalidate
    };
    Q_ENUM(Entries)
    Ownership() = delete;
    Q_DISABLE_COPY_MOVE(Ownership)
};

struct Affinity : public QObject{
    Q_OBJECT
    QML_ELEMENT
    QML_UNCREATABLE("enum carrier")
public:
    enum Entries {
        UI = 0x00800000,
        Pixmap = 0x08000000
    };
    Q_ENUM(Entries)
    Affinity() = delete;
    Q_DISABLE_COPY_MOVE(Affinity)
};

struct CodeClass : public QObject{
    Q_OBJECT
    QML_ELEMENT
    QML_UNCREATABLE("enum carrier")
public:
    enum Entries {
        NoLanguage          = 0x0000,
        Java                = 0x0001,
        Native              = 0x0002,
        Shell               = 0x0004,
        ShellDeclaration    = 0x0008,
        PackageInitializer  = 0x0010,
        DestructorFunction  = 0x0020,
        Constructors        = 0x0040,
        JavaInterface       = 0x0080,
        Signal              = 0x0100,
        MetaInfo            = 0x0200,
        ModuleInfo          = 0x0400,
        Getter              = 0x1000,
        Setter              = 0x2000,
        NativeGetter        = Native | Getter,
        NativeSetter        = Native | Setter,
        DeleterFunction     = 0x4000
    };
    Q_ENUM(Entries)
    CodeClass() = delete;
    Q_DISABLE_COPY_MOVE(CodeClass)
};

struct RemoveFlag : public QObject{
    Q_OBJECT
    QML_ELEMENT
    QML_UNCREATABLE("enum carrier")
public:
    enum Entries {
        None = CodeClass::NoLanguage,
        JavaAndNative = CodeClass::Java | CodeClass::Native,
        JavaOnly = CodeClass::Java,
        All
    };
    Q_ENUM(Entries)
    RemoveFlag() = delete;
    Q_DISABLE_COPY_MOVE(RemoveFlag)
};

struct Position : public QObject{
    Q_OBJECT
    QML_ELEMENT
    QML_UNCREATABLE("enum carrier")
public:
    enum Entries {
        Beginning = 0,
        Position1,
        Position2,
        Position3,
        Position4,
        Position5,
        Equals,
        Compare,
        HashCode,
        ToString,
        Clone,
        Comment,
        End = 1024
    };
    Q_ENUM(Entries)
    Position() = delete;
    Q_DISABLE_COPY_MOVE(Position)
};

struct Modification : public QObject{
    Q_OBJECT
    QML_ELEMENT
    QML_UNCREATABLE("enum carrier")
public:
    enum Entries {
        Private     = 0x01000,
        Protected   = 0x02000,
        Friendly    = 0x03000,
        Public      = 0x04000,

        AccessMask  = 0x0f000,
        Final       = 0x10000,
        DeclFinal   = 0x20000,
        NonFinal    = 0x40000
    };
    Q_ENUM(Entries)
    Modification() = delete;
    Q_DISABLE_COPY_MOVE(Modification)
};
Q_DECLARE_FLAGS(AccessModifications,Modification::Entries)

class AbstractObject : public ::QObject
{
    Q_OBJECT
    QML_ELEMENT
public:

    explicit AbstractObject(QObject *parent = nullptr);

    QVariant getUntil() const;
    void setUntil(const QVariant &newUntil);

    QVariant getSince() const;
    void setSince(const QVariant &newSince);

    const QVersionNumber &since() const{return m_since;}
    const QVersionNumber &until() const{return m_until;}

    const QString &getUnless() const;
    void setUnless(const QString &newUnless);

    QQmlListProperty<AbstractObject> getChildren();
    const QList<AbstractObject*>& childrenList() const {return children;}
    bool hasChildren() const {return !children.isEmpty();}

signals:

    void untilChanged();

    void sinceChanged();

    void unlessChanged();

    void childrenChanged();

private:
    QVersionNumber m_until{std::numeric_limits<int>::max(),std::numeric_limits<int>::max(),std::numeric_limits<int>::max()};
    QVersionNumber m_since{0,0,0};
    QString unless;
    QList<AbstractObject*> children;
    Q_PROPERTY(QVariant until READ getUntil WRITE setUntil NOTIFY untilChanged)
    Q_PROPERTY(QVariant since READ getSince WRITE setSince NOTIFY sinceChanged)
    Q_PROPERTY(QString unless READ getUnless WRITE setUnless NOTIFY unlessChanged)
    Q_PROPERTY(QQmlListProperty<AbstractObject> children READ getChildren NOTIFY childrenChanged)
    Q_CLASSINFO("DefaultProperty", "children")
    Q_DISABLE_COPY(AbstractObject)
};

QML_DECLARE_TYPE(AbstractObject)

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
Q_DECLARE_METATYPE(Ownership::Entries)
Q_DECLARE_METATYPE(Affinity::Entries)
Q_DECLARE_METATYPE(CodeClass::Entries)
Q_DECLARE_METATYPE(RemoveFlag::Entries)
Q_DECLARE_METATYPE(Position::Entries)
Q_DECLARE_METATYPE(Modification::Entries)
Q_DECLARE_METATYPE(AccessModifications)
#endif

#endif // QML_ABSTRACTOBJECT_H
