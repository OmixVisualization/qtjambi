/****************************************************************************
**
** Copyright (C) 2007-2009 Nokia. All rights reserved.
** Copyright (C) 2009-2015 Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
**
** This file is part of the $MODULE$ of the Qt Toolkit.
**
** ** $BEGIN_LICENSE$
** 
** GNU Lesser General Public License Usage
** This file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
** 
** In addition, as a special exception, Nokia gives you certain
** additional rights. These rights are described in the Nokia Qt LGPL
** Exception version 1.0, included in the file LGPL_EXCEPTION.txt in this
** package.
** 
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
** 
** $END_LICENSE$
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

#include "qtjambiintrospection_p.h"
#include <qtjambi/qtdynamicmetaobject.h>
#include <qtjambi/qtjambi_core.h>
#include <qtjambi/qtjambi_cache.h>

#include <QtCore/QObject>
#include <QtCore/QMetaEnum>
#include <QtCore/QMetaProperty>
#include <QtCore/QMetaMethod>
#include <QtCore/QMetaObject>
#include <QtCore/QHash>
#include <QtCore/QStringList>

#if defined(signal)
#  undef signal
#endif

class QtJambiMetaObject;

class QtJambiMetaEnumerator: public QDesignerMetaEnumInterface
{
public:
    QtJambiMetaEnumerator(const QMetaEnum &regularEnum, const QtJambiMetaObject *jambiMetaObject);

    virtual bool isFlag() const;
    virtual QString key(int index) const;
    virtual int keyCount() const;
    virtual int keyToValue(const QString &key) const;
    virtual int keysToValue(const QString &keys) const;
    virtual QString name() const;
    virtual QString scope() const;
    virtual QString separator() const;
    virtual int value(int index) const;
    virtual QString valueToKey(int value) const;
    virtual QString valueToKeys(int value) const ;

private:
    QString scopeInJvmSyntax() const;
    void initialize();

    QHash<QString, int> m_values_for_keys;
    QMetaEnum m_regular_enum;

    const QtJambiMetaObject *m_jambi_meta_object;
};

class QtJambiMetaProperty: public QDesignerMetaPropertyInterface
{
public:
    QtJambiMetaProperty(const QMetaProperty &regularProperty, const QtJambiMetaObject *jambiMetaObject);
    virtual ~QtJambiMetaProperty();

    virtual const QDesignerMetaEnumInterface *enumerator() const;

    virtual Kind kind() const;
    virtual AccessFlags accessFlags() const;
    virtual Attributes attributes(const QObject *object = 0) const;

    virtual QVariant::Type type() const;
    virtual QString name() const;
    virtual QString typeName() const;
    virtual int userType() const;
    virtual bool hasSetter() const;

    virtual QVariant read(const QObject *object) const;
    virtual bool reset(QObject *object) const;
    virtual bool write(QObject *object, const QVariant &value) const;

private:
    QMetaProperty m_regular_property;
    const QtJambiMetaObject *m_jambi_meta_object;
    QtJambiMetaEnumerator *m_enumerator;
};

class QtJambiMetaMethod: public QDesignerMetaMethodInterface
{
public:
    QtJambiMetaMethod(const QMetaMethod &regularMethod, const QtJambiMetaObject *jambiMetaObject, int index);

    virtual Access access() const ;
    virtual MethodType methodType() const;
    virtual QStringList parameterNames() const;
    virtual QStringList parameterTypes() const;
    virtual QString signature() const;
    virtual QString normalizedSignature() const;
    virtual QString tag() const;
    virtual QString typeName() const ;

private:
    static QString boxed(const QString &unboxed);

    QStringList byteArraysToStrings(const QList<QByteArray> &) const;

    QString m_java_signature;
    QMetaMethod m_regular_method;
    const QtJambiMetaObject *m_jambi_meta_object;
};

class QtJambiMetaObject: public QDesignerMetaObjectInterface
{
public:
    QtJambiMetaObject(const QMetaObject *regularMetaObject);
    virtual ~QtJambiMetaObject();

    virtual QString className() const;
    virtual const QDesignerMetaEnumInterface *enumerator(int index) const;
    virtual int enumeratorCount() const;
    virtual int enumeratorOffset() const;

    virtual int indexOfEnumerator(const QString &name) const;
    virtual int indexOfMethod(const QString &method) const;
    virtual int indexOfProperty(const QString &name) const;
    virtual int indexOfSignal(const QString &signal) const;
    virtual int indexOfSlot(const QString &slot) const;

    virtual const QDesignerMetaMethodInterface *method(int index) const;
    virtual int methodCount() const;
    virtual int methodOffset() const;

    virtual const  QDesignerMetaPropertyInterface *property(int index) const;
    virtual int propertyCount() const;
    virtual int propertyOffset() const;

    virtual const QDesignerMetaObjectInterface *superClass() const;
    virtual const QDesignerMetaPropertyInterface *userProperty() const;

    const QMetaObject *metaObject() const;
    bool metaObjectIsDynamic() const;
    QString fullClassName() const;
    void resolve();

private:
    const QMetaObject *m_regular_meta_object;

    mutable int m_total_method_count;

    int m_method_count;
    int m_property_count;
    int m_enumerator_count;

    QtJambiMetaEnumerator      **m_enumerators;
    QtJambiMetaProperty        **m_properties;
    QVector<QtJambiMetaMethod *> m_methods;

    uint m_meta_object_is_dynamic : 1;
    uint m_reserved               : 31;
};


/**
 * QtJambiMetaEnumerator
 */

QtJambiMetaEnumerator::QtJambiMetaEnumerator(const QMetaEnum &regularEnum, const QtJambiMetaObject *jambiMetaObject)
    : m_regular_enum(regularEnum), m_jambi_meta_object(jambiMetaObject)
{
    initialize();
}

void QtJambiMetaEnumerator::initialize()
{
    JNIEnv *env = qtjambi_current_environment();
    Q_ASSERT(env != 0);

    QString className = scopeInJvmSyntax();
    jclass clazz = qtjambi_find_class(env, className.toLatin1().constData());
    if (clazz == 0) {
        qtjambi_exception_check(env);
        qWarning("Couldn't find enum class '%s' for '%s' %s", qPrintable(className),
           qPrintable(m_jambi_meta_object->fullClassName() + QLatin1String("$") + name()),
          (m_jambi_meta_object->metaObjectIsDynamic() ? "dynamic" : "not-dynamic"));
        return ;
    }

    StaticCache *sc = StaticCache::instance();
    sc->resolveClass();
    sc->resolveEnum();

    jobjectArray enumConstants = static_cast<jobjectArray>(env->CallObjectMethod(clazz, sc->Class.getEnumConstants));
    if (enumConstants == 0) {
        qWarning("No enum constants in class '%s'", qPrintable(className));
        return ;
    }

    int numEnumConstants = env->GetArrayLength(enumConstants);
    for (int i=0; i<numEnumConstants; ++i) {
        jobject enumConstant = env->GetObjectArrayElement(enumConstants, i);
        if (enumConstant != 0) {
            jstring javaName = static_cast<jstring>(env->CallObjectMethod(enumConstant, sc->Enum.name));
            if (javaName != 0) {
                QString name = qtjambi_to_qstring(env, javaName);

                // Get the value of the enum if it has a value() method
                // otherwise get the ordinal.
                int value = qtjambi_to_enum(env, enumConstant);

                m_values_for_keys[name] = value;
            } else {
                qWarning("Name of enum constant #%d in class '%s' is null", i, qPrintable(className));
            }
        } else {
            qWarning("Enum constant #%d in '%s' is null", i, qPrintable(className));
        }
    }
}

bool QtJambiMetaEnumerator::isFlag() const
{
    return m_regular_enum.isFlag();
}

QString QtJambiMetaEnumerator::key(int index) const
{
    if (index < 0 || index >= keyCount())
        return 0;
    else
        return m_values_for_keys.keys().at(index);
}

int QtJambiMetaEnumerator::keyCount() const
{
    return m_values_for_keys.size();
}

int QtJambiMetaEnumerator::keyToValue(const QString &key) const
{
    return m_values_for_keys.value(key, -1);
}

int QtJambiMetaEnumerator::keysToValue(const QString &keys) const
{
    QStringList keyList = keys.split('|');

    int val = -1;
    foreach (QString key, keyList) {
        if (val < 0)
            val = keyToValue(key);
        else
            val |= keyToValue(key);
    }

    return val;
}

QString QtJambiMetaEnumerator::name() const
{
    QString full_name = m_regular_enum.name();
    int pos = full_name.lastIndexOf(QLatin1String("::"));
    if (pos >= 0)
        return full_name.right(full_name.length() - pos - 2);
    else
        return full_name;
}

QString QtJambiMetaEnumerator::scopeInJvmSyntax() const
{
    JNIEnv *env = qtjambi_current_environment();
    Q_ASSERT(env != 0);

    QString flags_name = m_jambi_meta_object->fullClassName() + QLatin1String("$") + name();
    if (m_jambi_meta_object->metaObjectIsDynamic()) {
        QString scope = flags_name;

        // The scope for the enum values needs to be the enum type, not the flag type
        // so we need to look this up in Java.
        if (isFlag())
            scope = qtjambi_enum_name_for_flags_name(env, scope.replace(QLatin1String("."), QLatin1String("/")));

        return scope.replace(QLatin1String("."), QLatin1String("/"));
    } else {
        QString full_name = QLatin1String(m_regular_enum.scope()) + QLatin1String("::") + QLatin1String(m_regular_enum.name());
        full_name = getJavaName(full_name.toLatin1());

        return full_name;
    }
}

QString QtJambiMetaEnumerator::scope() const
{
    return scopeInJvmSyntax().replace(QLatin1String("/"), QLatin1String(".")).replace(QLatin1String("$"), QLatin1String("."));
}

QString QtJambiMetaEnumerator::separator() const
{
    return QLatin1String(".");
}

int QtJambiMetaEnumerator::value(int index) const
{
    return m_values_for_keys.values().at(index);
}

QString QtJambiMetaEnumerator::valueToKey(int value) const
{
    return m_values_for_keys.key(value);
}

QString QtJambiMetaEnumerator::valueToKeys(int value) const
{
    QList<QString> keys = m_values_for_keys.keys(value);

    QString ret;
    for (int i=0; i<keys.size(); ++i) {
        if (i > 0)
            ret += '|';

        ret += keys.at(i);
    }

    return ret;
}

static const QtJambiMetaObject *qtjambi_meta_object_stash(const QMetaObject *metaObject)
{
    if (metaObject == 0) // it could happen to anyone
        return 0;

    static QHash<QString, const QtJambiMetaObject *> meta_object_stash;
    QLatin1String className(metaObject->className());

    const QtJambiMetaObject *returned = meta_object_stash.value(className, 0);
    if (returned == 0) {
        QtJambiMetaObject *temp = new QtJambiMetaObject(metaObject);
        meta_object_stash.insert(className, temp);
        temp->resolve();

        returned = temp;
    }

    return returned;
}

/**
 * QtJambiMetaProperty
 */
QtJambiMetaProperty::QtJambiMetaProperty(const QMetaProperty &regularProperty, const QtJambiMetaObject *jambiMetaObject)
    : m_regular_property(regularProperty),
      m_jambi_meta_object(jambiMetaObject),
      m_enumerator(0)
{
    Q_ASSERT(m_jambi_meta_object != 0);

    if (regularProperty.isEnumType() || regularProperty.isFlagType()) {
        QMetaEnum property_enumerator = regularProperty.enumerator();
        const QMetaObject *enumerator_owner = property_enumerator.enclosingMetaObject();
        m_enumerator = new QtJambiMetaEnumerator(regularProperty.enumerator(), qtjambi_meta_object_stash(enumerator_owner));
    }
}

QtJambiMetaProperty::~QtJambiMetaProperty()
{
    delete m_enumerator;
}

const QDesignerMetaEnumInterface *QtJambiMetaProperty::enumerator() const
{
    return m_enumerator;
}

QDesignerMetaPropertyInterface::Kind QtJambiMetaProperty::kind() const
{
    if (m_regular_property.isFlagType()) return FlagKind;
    if (m_regular_property.isEnumType()) return EnumKind;
    return OtherKind;
}

QDesignerMetaPropertyInterface::AccessFlags QtJambiMetaProperty::accessFlags() const
{
    return AccessFlags( (m_regular_property.isReadable()   ? ReadAccess  : 0)
                     |  (m_regular_property.isWritable()   ? WriteAccess : 0)
                     |  (m_regular_property.isResettable() ? ResetAccess : 0));
}

QDesignerMetaPropertyInterface::Attributes QtJambiMetaProperty::attributes(const QObject *object) const
{
    return Attributes( (m_regular_property.isDesignable(object) ? DesignableAttribute : 0)
                    |  (m_regular_property.isScriptable(object) ? ScriptableAttribute : 0)
                    |  (m_regular_property.isStored(object)     ? StoredAttribute     : 0)
                    |  (m_regular_property.isUser(object)       ? UserAttribute       : 0));
}

QVariant::Type QtJambiMetaProperty::type() const
{
    return m_regular_property.type();
}

QString QtJambiMetaProperty::name() const
{
    return QLatin1String(m_regular_property.name());
}

QString QtJambiMetaProperty::typeName() const
{
    return QLatin1String(m_regular_property.typeName());
}

int QtJambiMetaProperty::userType() const
{
    return m_regular_property.userType();
}

bool QtJambiMetaProperty::hasSetter() const
{
    return m_regular_property.hasStdCppSet();
}

QVariant QtJambiMetaProperty::read(const QObject *object) const
{
    return m_regular_property.read(object);
}

bool QtJambiMetaProperty::reset(QObject *object) const
{
    return m_regular_property.reset(object);
}

bool QtJambiMetaProperty::write(QObject *object, const QVariant &value) const
{
    return m_regular_property.write(object, value);
}

/**
 * QtJambiMetaMethod
 */
QtJambiMetaMethod::QtJambiMetaMethod(const QMetaMethod &regularMethod, const QtJambiMetaObject *jambiMetaObject, int index)
    : m_regular_method(regularMethod), m_jambi_meta_object(jambiMetaObject)
{
    Q_ASSERT(jambiMetaObject != 0);

    if (m_jambi_meta_object->metaObjectIsDynamic()) {
        // If the meta object is dynamic, then we can query it for the original
        // signature of the method
        const QtDynamicMetaObject *dynamic_meta_object = static_cast<const QtDynamicMetaObject *>(jambiMetaObject->metaObject());
        dynamic_meta_object->originalSignalOrSlotSignature(qtjambi_current_environment(), index, &m_java_signature);
    } else {
        // If it's not dynamic, then we can query the meta info
#if QT_VERSION < 0x050000
        QString qt_signature = QLatin1String(m_regular_method.enclosingMetaObject()->className()) + QLatin1String("::") + m_regular_method.signature();
#else
        QString qt_signature = QLatin1String(m_regular_method.enclosingMetaObject()->className()) + QLatin1String("::") + QLatin1String(m_regular_method.methodSignature());
#endif
        m_java_signature = getJavaName(qt_signature.toLatin1());
    }

    if (methodType() == Signal) {
        m_java_signature.replace(QLatin1String("()"), QLatin1String(""))
                        .replace(QLatin1String("("), QLatin1String("<"))
                        .replace(QLatin1String(")"), QLatin1String(">"));
    }

    int pos = methodType() == Signal ? m_java_signature.lastIndexOf(QLatin1String("<")) : m_java_signature.lastIndexOf(QLatin1String("("));

    // Signals use object types, but the meta info system contains the original primitive types
    // because it contains the names of the method equivalents of signals. Thus, we convert
    // each primitive type name to its complex equivalent.
    if (methodType() == Signal) {
        int pos2 = m_java_signature.lastIndexOf(">");
        if (pos2 > pos) {
            QString paramString = m_java_signature.mid(pos + 1, pos2 - pos - 1);
            QStringList params = paramString.split(",");
            for (int i=0; i<params.size(); ++i)
                params[i] = boxed(params.at(i).trimmed());

            m_java_signature = m_java_signature.left(pos) + QLatin1String("<") + params.join(", ")
                            + QLatin1String(">") + m_java_signature.right(m_java_signature.length() - pos2 -1);
        }
    }

    pos = m_java_signature.lastIndexOf(QLatin1String("."), pos);

    if (pos >= 0)
        m_java_signature = m_java_signature.right(m_java_signature.length() - pos - 1);

    m_java_signature = m_java_signature.trimmed();
}

QString QtJambiMetaMethod::boxed(const QString &unboxed)
{
    static QHash<QString, const char *> boxes;
    if (boxes.isEmpty()) {
        boxes[QLatin1String("boolean")] = "java.lang.Boolean";
        boxes[QLatin1String("int")] = "java.lang.Integer";
        boxes[QLatin1String("char")] = "java.lang.Character";
        boxes[QLatin1String("long")] = "java.lang.Long";
        boxes[QLatin1String("float")] = "java.lang.Float";
        boxes[QLatin1String("double")] = "java.lang.Double";
        boxes[QLatin1String("short")] = "java.lang.Short";
        boxes[QLatin1String("byte")] = "java.lang.Byte";
    }

    return boxes.contains(unboxed) ? QString::fromLatin1(boxes.value(unboxed)) : unboxed;
}

QDesignerMetaMethodInterface::Access QtJambiMetaMethod::access() const
{
    QMetaMethod::Access a = m_regular_method.access();
    if (a == QMetaMethod::Private) return Private;
    else if (a == QMetaMethod::Public) return Public;
    else return Protected;
}

QDesignerMetaMethodInterface::MethodType QtJambiMetaMethod::methodType() const
{
    QMetaMethod::MethodType m = m_regular_method.methodType();
    if (m == QMetaMethod::Signal) return Signal;
    else if (m == QMetaMethod::Slot) return Slot;
    else return Method;
}

QStringList QtJambiMetaMethod::byteArraysToStrings(const QList<QByteArray> &byteArrays) const
{
    QStringList strings;
    foreach (QByteArray byteArray, byteArrays)
        strings.append(QLatin1String(byteArray));

    return strings;
}

QStringList QtJambiMetaMethod::parameterNames() const
{
    return byteArraysToStrings(m_regular_method.parameterNames());
}

QStringList QtJambiMetaMethod::parameterTypes() const
{
    return byteArraysToStrings(m_regular_method.parameterTypes());
}

QString QtJambiMetaMethod::signature() const
{
    return m_java_signature;
}

QString QtJambiMetaMethod::normalizedSignature() const
{
    return m_java_signature;
}

QString QtJambiMetaMethod::tag() const
{
    return QLatin1String(m_regular_method.tag());
}

QString QtJambiMetaMethod::typeName() const
{
    return QLatin1String(m_regular_method.typeName());
}

/**
 * QtJambiMetaObject
 */

QtJambiMetaObject::QtJambiMetaObject(const QMetaObject *regularMetaObject)
    :
    m_regular_meta_object(regularMetaObject),
    m_total_method_count(-1),
    m_method_count(0),
    m_property_count(0),
    m_enumerator_count(0),
    m_enumerators(0),
    m_properties(0),
    m_meta_object_is_dynamic(qtjambi_metaobject_is_dynamic(regularMetaObject))
{
    Q_ASSERT(m_regular_meta_object != 0);
}


QtJambiMetaObject::~QtJambiMetaObject()
{
    {
        int count = m_enumerator_count;
        for (int i=0; i<count; ++i)
            delete m_enumerators[i];
        delete[] m_enumerators;
    }

    {
        int count = m_property_count;
        for (int i=0; i<count; ++i)
            delete m_properties[i];
        delete[] m_properties;
    }

    {
        int count = m_method_count;
        for (int i=0; i<count; ++i)
            delete m_methods[i];
    }
}

void QtJambiMetaObject::resolve()
{
    int count = m_regular_meta_object->enumeratorCount();
    int offset = m_regular_meta_object->enumeratorOffset();
    if (count > 0) {
        m_enumerator_count = count - offset;
        m_enumerators = new QtJambiMetaEnumerator *[m_enumerator_count];

        for (int i=offset; i<count; ++i)
            m_enumerators[i - offset] = new QtJambiMetaEnumerator(m_regular_meta_object->enumerator(i), this);
    } else {
        m_enumerators = 0;
    }

    count = m_regular_meta_object->propertyCount();
    offset = m_regular_meta_object->propertyOffset();
    if (count > 0) {
        m_property_count = count - offset;
        m_properties = new QtJambiMetaProperty *[m_property_count];

        for (int i=offset; i<count; ++i)
            m_properties[i - offset] = new QtJambiMetaProperty(m_regular_meta_object->property(i), this);
    } else {
        m_properties = 0;
    }


    count = m_regular_meta_object->methodCount();
    offset = m_regular_meta_object->methodOffset();
    if (count > 0) {
        m_methods.resize(count - offset);

        m_method_count = 0;
        for (int i=offset; i<count; ++i) {
            QMetaMethod regular_method = m_regular_meta_object->method(i);

            // We don't allow signal overloads
            bool valid_method = (regular_method.methodType() != QMetaMethod::Signal || (regular_method.attributes() & QMetaMethod::Cloned) == 0);

            // Not all C++ signals and slots should appear in Designer
            // since not all are mapped.
            if (valid_method && !metaObjectIsDynamic()) {
                QString qt_signature = QLatin1String(m_regular_meta_object->className())
                                     + QLatin1String("::")
#if QT_VERSION < 0x050000
                                     + QLatin1String(regular_method.signature()
#else
									 + QLatin1String(regular_method.methodSignature()
#endif
						);
                QString java_signature = getJavaName(qt_signature.toLatin1());
                valid_method = !java_signature.isEmpty();
            }

            if (valid_method)
                m_methods[m_method_count++] = new QtJambiMetaMethod(regular_method, this, i);
        }

    }
}

QString QtJambiMetaObject::fullClassName() const
{
    if (m_meta_object_is_dynamic)
        return className().replace(QLatin1String("::"), QLatin1String("."));
    else
        return getJavaName(className()).replace(QLatin1String("/"), QLatin1String("."));
}

QString QtJambiMetaObject::className() const
{
    return QString::fromLatin1(m_regular_meta_object->className()).replace(QLatin1String("::"), QLatin1String("."));
}

const QDesignerMetaObjectInterface *QtJambiMetaObject::superClass() const
{
    return qtjambi_meta_object_stash(m_regular_meta_object->superClass());
}

const QDesignerMetaPropertyInterface *QtJambiMetaObject::userProperty() const
{
    return 0;
}

const QDesignerMetaEnumInterface *QtJambiMetaObject::enumerator(int index) const
{
    // First check for it in the super class
    const QtJambiMetaObject *super_class = static_cast<const QtJambiMetaObject *>(superClass());
    if (super_class != 0 && index < enumeratorOffset())
        return super_class->enumerator(index);

    index -= enumeratorOffset();

    Q_ASSERT(index < m_enumerator_count);
    return m_enumerators[index];
}

int QtJambiMetaObject::enumeratorCount() const
{
    return m_regular_meta_object->enumeratorCount();
}

int QtJambiMetaObject::enumeratorOffset() const
{
    return m_regular_meta_object->enumeratorOffset();
}

int QtJambiMetaObject::indexOfEnumerator(const QString &enumerator) const
{
    return m_regular_meta_object->indexOfEnumerator(enumerator.toLatin1());
}

int QtJambiMetaObject::indexOfMethod(const QString &method) const
{
    const QtJambiMetaObject *super_class = static_cast<const QtJambiMetaObject *>(superClass());

    int idx = super_class != 0 ? super_class->indexOfMethod(method) : -1;
    if (idx < 0) {
        for (int i=0; i<m_method_count; ++i) {
            if (m_methods[i]->signature() == method) {
                idx = i + methodOffset();
                break;
            }
        }
    }

    return idx;
}

int QtJambiMetaObject::indexOfProperty(const QString &property) const
{
    return m_regular_meta_object->indexOfProperty(property.toLatin1());
}

int QtJambiMetaObject::indexOfSignal(const QString &) const
{
    Q_ASSERT_X(false, "indexOfSignal", "function not implemented");
    return -1;
}

int QtJambiMetaObject::indexOfSlot(const QString &) const
{
    Q_ASSERT_X(false, "indexOfSlot", "function not implemented");
    return -1;
}

const QDesignerMetaMethodInterface *QtJambiMetaObject::method(int index) const
{
    // First check for it in the super class
    const QtJambiMetaObject *super_class = static_cast<const QtJambiMetaObject *>(superClass());
    if (super_class != 0 && index < methodOffset())
        return super_class->method(index);

    index -= methodOffset();

    Q_ASSERT(index < m_method_count);
    return m_methods[index];
}

int QtJambiMetaObject::methodCount() const
{
    if (m_total_method_count < 0) {
        const QtJambiMetaObject *super_class = static_cast<const QtJambiMetaObject *>(superClass());
        m_total_method_count = m_method_count + (super_class ? super_class->methodCount() : 0);
    }

    return m_total_method_count;
}

int QtJambiMetaObject::methodOffset() const
{
    return methodCount() - m_method_count;
}

const QDesignerMetaPropertyInterface *QtJambiMetaObject::property(int index) const
{
    // First check for it in the super class
    const QtJambiMetaObject *super_class = static_cast<const QtJambiMetaObject *>(superClass());
    if (super_class != 0 && index < propertyOffset())
        return super_class->property(index);

    index -= propertyOffset();
    Q_ASSERT(index < m_property_count);
    return m_properties[index];
}

int QtJambiMetaObject::propertyCount() const
{
    return m_regular_meta_object->propertyCount();
}

int QtJambiMetaObject::propertyOffset() const
{
    return m_regular_meta_object->propertyOffset();
}

const QMetaObject *QtJambiMetaObject::metaObject() const
{
    return m_regular_meta_object;
}

bool QtJambiMetaObject::metaObjectIsDynamic() const
{
    return m_meta_object_is_dynamic;
}


/**
 * QtJambiIntrospection
 */
QtJambiIntrospection::QtJambiIntrospection() { }

QtJambiIntrospection::~QtJambiIntrospection() { }

const QDesignerMetaObjectInterface* QtJambiIntrospection::metaObject(const QObject *object) const
{
    return qtjambi_meta_object_stash(object->metaObject());
}
