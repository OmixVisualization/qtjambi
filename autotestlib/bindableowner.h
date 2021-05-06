#ifndef BINDABLEOWNER_H
#define BINDABLEOWNER_H

#ifndef QT_JAMBI_RUN
#include <qtjambi/qtjambi_global.h>
#endif
#include <QtCore/QMetaProperty>
#include <QtCore/QObject>
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
#include <QtCore/QProperty>

#ifdef QT_JAMBI_RUN
#undef Q_OBJECT_BINDABLE_PROPERTY
#define Q_OBJECT_BINDABLE_PROPERTY(...)
#endif

class BindableOwner : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int x READ x WRITE setX NOTIFY xChanged BINDABLE bindableX)
    Q_PROPERTY(QString text READ text WRITE setText NOTIFY textChanged BINDABLE bindableText)
public:
    explicit BindableOwner(QObject *parent = nullptr);

    int x() const { return xProp; }
    void setX(int x) { xProp = x; }
    QBindable<int> bindableX() { return QBindable<int>(&xProp); }

    const QString& text() const { return textProp; }
    void setText(const QString& text) { textProp = text; }
    QBindable<QString> bindableText() { return QBindable<QString>(&textProp); }

    static bool bind(QObject* targetObject, QStringView targetPropertyName, QObject* sourceObject, QStringView sourcePropertyName);

signals:
    void xChanged();
    void textChanged();

private:
    Q_OBJECT_BINDABLE_PROPERTY(BindableOwner, int, xProp, &BindableOwner::xChanged)
    Q_OBJECT_BINDABLE_PROPERTY(BindableOwner, QString, textProp, &BindableOwner::textChanged)
};
#endif //QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)

#endif // BINDABLEOWNER_H
