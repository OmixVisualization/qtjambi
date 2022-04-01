/****************************************************************************
**
** Copyright (C) 1992-2009 Nokia. All rights reserved.
**
** This file is part of Qt Jambi.
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

#include "qtjambi_core.h"
#include "qtjambi_application.h"
#include "qtjambi_repository_p.h"
#include "qtjambifunctiontable_p.h"
#include "qtjambi_registry_p.h"
#include "qtjambi_interfaces_p.h"
#include "qtjambi_typeinfo_p.h"
#include "qtjambimetaobject_p.h"

#include <memory>
#include <typeindex>

JNIEnv *qtjambi_current_environment(bool initializeJavaThread);

class AbstractFunctionTable : public QtJambiFunctionTable
{
public:
    AbstractFunctionTable(JNIEnv* env, jclass clazz);
    ~AbstractFunctionTable() override;
    jclass javaClass() const override;
    const QMetaObject* metaObject() const override;
    jmethodID javaMethod(const std::type_info& typeId, int pos) const override;
private:
    jclass m_class;
};

typedef QHash<int, QSharedPointer<const QtJambiFunctionTable>> FunctionTableHash;
Q_GLOBAL_STATIC(FunctionTableHash, functionTableCache)
Q_GLOBAL_STATIC(QReadWriteLock, gTableLock)


class FunctionTable : public AbstractFunctionTable
{
public:
    FunctionTable(JNIEnv* env, jclass clazz, const QList<jmethodID> & methods);
    jmethodID javaMethod(const std::type_info& typeId, int pos) const;
private:
    QList<jmethodID> m_method_ids;
};

class EmptyFunctionTable : public AbstractFunctionTable
{
public:
    EmptyFunctionTable(JNIEnv* env, jclass clazz);
};

class InterfaceFunctionTable : public AbstractFunctionTable
{
public:
    InterfaceFunctionTable(JNIEnv* env, jclass clazz, const QMap<std::type_index, QList<jmethodID>> & methods);
    jmethodID javaMethod(const std::type_info& typeId, int pos) const;
private:
    QMap<std::type_index, QList<jmethodID>> m_method_ids;
};

class MetaObjectFunctionTable : public FunctionTable
{
public:
    MetaObjectFunctionTable(JNIEnv* env, jclass clazz, const QList<jmethodID> & methods, const QMetaObject* metaObject);
    const QMetaObject* metaObject() const;
private:
    const QMetaObject* m_metaObject;
};

class MetaObjectEmptyFunctionTable : public EmptyFunctionTable
{
public:
    MetaObjectEmptyFunctionTable(JNIEnv* env, jclass clazz, const QMetaObject* metaObject);
    const QMetaObject* metaObject() const;
private:
    const QMetaObject* m_metaObject;
};

class MetaObjectInterfaceFunctionTable : public InterfaceFunctionTable
{
public:
    MetaObjectInterfaceFunctionTable(JNIEnv* env, jclass clazz, const QMap<std::type_index, QList<jmethodID>> & methods, const QMetaObject* metaObject);
    const QMetaObject* metaObject() const;
private:
    const QMetaObject* m_metaObject;
};

class DynamicMetaObjectFunctionTable : public FunctionTable
{
public:
    DynamicMetaObjectFunctionTable(JNIEnv* env, jclass clazz, const QList<jmethodID> & methods, const QtJambiMetaObject* metaObject);
    const QMetaObject* metaObject() const;
private:
    QSharedPointer<const QtJambiMetaObject> m_metaObject;
};

class DynamicMetaObjectInterfaceFunctionTable : public InterfaceFunctionTable
{
public:
    DynamicMetaObjectInterfaceFunctionTable(JNIEnv* env, jclass clazz, const QMap<std::type_index, QList<jmethodID>> & methods, const QtJambiMetaObject* metaObject);
    const QMetaObject* metaObject() const;
private:
    QSharedPointer<const QtJambiMetaObject> m_metaObject;
};

QtJambiFunctionTable::~QtJambiFunctionTable()
{}

AbstractFunctionTable::AbstractFunctionTable(JNIEnv* env, jclass clazz)
    : m_class(getGlobalClassRef(env, clazz))
{
}

AbstractFunctionTable::~AbstractFunctionTable(){
}

jclass AbstractFunctionTable::javaClass() const
{
    return m_class;
}

const QMetaObject* AbstractFunctionTable::metaObject() const
{
    if(JNIEnv* env = qtjambi_current_environment()){
        QTJAMBI_JNI_LOCAL_FRAME(env, 200)
        return qtjambi_metaobject_for_class(env, m_class, nullptr);
    }else{
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
        return qt_getQtMetaObject();
#else
        return &Qt::staticMetaObject;
#endif
    }
}

jmethodID AbstractFunctionTable::javaMethod(const std::type_info&, int) const
{
    return nullptr;
}

FunctionTable::FunctionTable(JNIEnv* env, jclass clazz,
                                         const QList<jmethodID> & methods)
    : AbstractFunctionTable(env, clazz),
      m_method_ids(methods)
{
}

jmethodID FunctionTable::javaMethod(const std::type_info&, int pos) const
{
    Q_ASSERT(pos >= 0);
    Q_ASSERT(pos < m_method_ids.size());
    return pos >= 0 && pos < m_method_ids.size() ? m_method_ids.at(pos) : nullptr;
}

InterfaceFunctionTable::InterfaceFunctionTable(JNIEnv* env, jclass clazz,
                                         const QMap<std::type_index, QList<jmethodID>> & methods)
    : AbstractFunctionTable(env, clazz),
      m_method_ids(methods)
{
}

jmethodID InterfaceFunctionTable::javaMethod(const std::type_info& typeId, int pos) const
{
    Q_ASSERT(m_method_ids.contains(typeId));
    Q_ASSERT(pos >= 0);
    Q_ASSERT(pos < m_method_ids[typeId].size());
    return pos >= 0 && pos < m_method_ids[typeId].size() ? m_method_ids[typeId].at(pos) : nullptr;
}

EmptyFunctionTable::EmptyFunctionTable(JNIEnv* env, jclass clazz)
    : AbstractFunctionTable(env, clazz)
{

}

MetaObjectEmptyFunctionTable::MetaObjectEmptyFunctionTable(JNIEnv* env, jclass clazz, const QMetaObject* metaObject)
: EmptyFunctionTable(env, clazz), m_metaObject(metaObject) {}

const QMetaObject* MetaObjectEmptyFunctionTable::metaObject() const { return m_metaObject; }

MetaObjectFunctionTable::MetaObjectFunctionTable(JNIEnv* env, jclass clazz, const QList<jmethodID> & methods, const QMetaObject* metaObject)
    : FunctionTable(env, clazz, methods), m_metaObject(metaObject) {}

const QMetaObject* MetaObjectFunctionTable::metaObject() const { return m_metaObject; }

MetaObjectInterfaceFunctionTable::MetaObjectInterfaceFunctionTable(JNIEnv* env, jclass clazz, const QMap<std::type_index, QList<jmethodID>> & methods, const QMetaObject* metaObject)
    : InterfaceFunctionTable(env, clazz, methods), m_metaObject(metaObject) {}

const QMetaObject* MetaObjectInterfaceFunctionTable::metaObject() const { return m_metaObject; }

DynamicMetaObjectFunctionTable::DynamicMetaObjectFunctionTable(JNIEnv* env, jclass clazz, const QList<jmethodID> & methods, const QtJambiMetaObject* metaObject)
    : FunctionTable(env, clazz, methods), m_metaObject(metaObject->thisPointer()) {}

const QMetaObject* DynamicMetaObjectFunctionTable::metaObject() const { return m_metaObject.get(); }

DynamicMetaObjectInterfaceFunctionTable::DynamicMetaObjectInterfaceFunctionTable(JNIEnv* env, jclass clazz, const QMap<std::type_index, QList<jmethodID>> & methods, const QtJambiMetaObject* metaObject)
    : InterfaceFunctionTable(env, clazz, methods), m_metaObject(metaObject->thisPointer()) {}

const QMetaObject* DynamicMetaObjectInterfaceFunctionTable::metaObject() const { return m_metaObject.get(); }

QString jniSignatureToJava(QString jniSignature, const QString& methodName){
    QStringList signature;
    QString array;
    QString returnType;
    if(jniSignature.startsWith('(')){
        jniSignature = jniSignature.mid(1);
        while(!jniSignature.isEmpty() && !jniSignature.startsWith(')')){
            if(jniSignature.startsWith('L')){
                jniSignature = jniSignature.mid(1);
                auto idx = jniSignature.indexOf(';');
                if(idx>=0){
                    signature << jniSignature.left(idx).replace('/', '.').replace('$', '.') + array;
                    array.clear();
                    jniSignature = jniSignature.mid(idx+1);
                }else{
                    jniSignature.clear();
                }
            }else{
                if(jniSignature.startsWith('V')){
                }else if(jniSignature.startsWith('I')){
                    signature << "int" + array;
                    array.clear();
                }else if(jniSignature.startsWith('J')){
                    signature << "long" + array;
                    array.clear();
                }else if(jniSignature.startsWith('S')){
                    signature << "short" + array;
                    array.clear();
                }else if(jniSignature.startsWith('B')){
                    signature << "byte" + array;
                    array.clear();
                }else if(jniSignature.startsWith('C')){
                    signature << "char" + array;
                    array.clear();
                }else if(jniSignature.startsWith('Z')){
                    signature << "boolean" + array;
                    array.clear();
                }else if(jniSignature.startsWith('D')){
                    signature << "double" + array;
                    array.clear();
                }else if(jniSignature.startsWith('F')){
                    signature << "float" + array;
                    array.clear();
                }else if(jniSignature.startsWith('[')){
                    array += "[]";
                }
                jniSignature = jniSignature.mid(1);
            }
        }
        if(jniSignature.startsWith(')')){
            jniSignature = jniSignature.mid(1);
            if(jniSignature.startsWith('L')){
                jniSignature = jniSignature.mid(1);
                auto idx = jniSignature.indexOf(';');
                if(idx>=0){
                    returnType = jniSignature.left(idx).replace('/', '.').replace('$', '.') + array;
                    jniSignature = jniSignature.mid(idx+1);
                }else{
                    jniSignature.clear();
                }
            }else{
                if(jniSignature.startsWith('V')){
                    returnType = "void";
                }else if(jniSignature.startsWith('I')){
                    returnType = "int" + array;
                }else if(jniSignature.startsWith('J')){
                    returnType = "long" + array;
                }else if(jniSignature.startsWith('S')){
                    returnType = "short" + array;
                }else if(jniSignature.startsWith('B')){
                    returnType = "byte" + array;
                }else if(jniSignature.startsWith('C')){
                    returnType = "char" + array;
                }else if(jniSignature.startsWith('Z')){
                    returnType = "boolean" + array;
                }else if(jniSignature.startsWith('D')){
                    returnType = "double" + array;
                }else if(jniSignature.startsWith('F')){
                    returnType = "float" + array;
                }else if(jniSignature.startsWith('[')){
                    array += "[]";
                }
                jniSignature = jniSignature.mid(1);
            }
        }
    }
    return QString("%1 %2(%3)").arg(returnType).arg(methodName).arg(signature.join(", "));
}

QString qtjambi_class_display_name(JNIEnv *env, jclass object_class)
{
    QString name = qtjambi_class_name(env, object_class);//.replace('$', '.')
    auto idx = name.indexOf('$');
    QString _name;
    while(idx>=0 && idx<name.length()){
        _name += name.left(idx);
        name = name.mid(idx+1);
        if(name[0]=='$'
                || (name[0]>='0' && name[0]<='9')){
            _name += '$';
        }else{
            _name += '.';
        }
        idx = name.indexOf('$');
    }
    _name += name;
    return _name;
}

QList<jmethodID> get_methods(JNIEnv *env, jclass object_class, const std::type_info& typeId)
{
    bool is_interface = isInterface(typeId) || isFunctional(typeId);
    QList<jmethodID> methods;
    const QtJambiTypeEntry* typeEntry = QtJambiTypeEntry::getTypeEntry(env, typeId);
    Q_ASSERT(typeEntry);
    const QVector<const FunctionInfo>* virtualFcts(nullptr);
    if(typeEntry->isObject()){
        virtualFcts = static_cast<const ObjectTypeEntry*>(typeEntry)->virtualFunctions();
    }else if(typeEntry->isQObject()){
        virtualFcts = static_cast<const QObjectTypeEntry*>(typeEntry)->virtualFunctions();
    }else if(typeEntry->isInterface()){
        virtualFcts = static_cast<const InterfaceTypeEntry*>(typeEntry)->virtualFunctions();
    }else if(typeEntry->isFunctional()){
        virtualFcts = static_cast<const FunctionalTypeEntry*>(typeEntry)->virtualFunctions();
    }
    if(virtualFcts){
        for (int i=0; i<virtualFcts->size(); ++i) {
            const FunctionInfo& info = (*virtualFcts)[i];
            if (jmethodID method_id = env->GetMethodID(object_class, info.name, info.signature)) {
                if (jobject method_object = env->ToReflectedMethod(object_class, method_id, false)) {
                    if (Java::QtJambi::QtJambiInternal::isImplementedInJava(env,
                                                     jboolean(info.flags & FunctionInfo::Abstract),
                                                     method_object, typeEntry->javaClass())) {
                        if(info.flags & FunctionInfo::Private){
                            bool isPrivateOverride = Java::Runtime::AccessibleObject::isAnnotationPresent(env, method_object, Java::QtJambi::QtPrivateOverride::getClass(env));
                            if(!isPrivateOverride){
                                QString message(QLatin1String("Method '%1' in class %2 misses @QtPrivateOverride annotation."));
                                message = message.arg(jniSignatureToJava(QLatin1String(info.signature), QLatin1String(info.name)));
                                message = message.arg(qtjambi_class_display_name(env, object_class));
                                Java::QtJambi::QMissingVirtualOverridingException::throwNew(env, message QTJAMBI_STACKTRACEINFO );
                            }
                        }
                        methods << method_id;
                    }else if(info.flags & FunctionInfo::Abstract){
                        QString message(QLatin1String("Class %1 does not implement pure virtual function '%2' from Qt declared in %3."));
                        message = message.arg(qtjambi_class_display_name(env, object_class));
                        message = message.arg(jniSignatureToJava(QLatin1String(info.signature), QLatin1String(info.name)));
                        message = message.arg(QLatin1String(typeEntry->qtName()));
                        Java::QtJambi::QMissingVirtualOverridingException::throwNew(env, message QTJAMBI_STACKTRACEINFO );
                    }else{
                        methods << nullptr;
                    }
                    env->DeleteLocalRef(method_object);
                }else{
                    jthrowable thr = nullptr;
                    if(env->ExceptionCheck()){
                        thr = env->ExceptionOccurred();
                        env->ExceptionClear();
                    }
                    fprintf(stderr,
                            "vtable setup conversion to reflected method failed: %s::%s %s\n",
                            qPrintable(qtjambi_class_display_name(env, object_class)), info.name, info.signature);
                    if(thr)
                        JavaException(env, thr).raise( QTJAMBI_STACKTRACEINFO_ENV(env) );
                    methods << nullptr;
                }
            }else{
                if(is_interface){
                    if(env->ExceptionCheck()){
                        env->ExceptionClear();
                    }
                    if(info.flags & FunctionInfo::Abstract){
                        QString message;
                        if(isFunctional(typeId)){
                            message = QLatin1String("Class %1 does not implement functional interface method '%2' from Qt declared in %3.");
                        }else{
                            message = QLatin1String("Class %1 does not implement pure virtual function '%2' from Qt declared in %3.");
                        }
                        message = message.arg(qtjambi_class_display_name(env, object_class));
                        message = message.arg(jniSignatureToJava(QLatin1String(info.signature), QLatin1String(info.name)));
                        message = message.arg(QLatin1String(typeEntry->qtName()));
                        Java::QtJambi::QMissingVirtualOverridingException::throwNew(env, message QTJAMBI_STACKTRACEINFO );
                    }
                }else if(info.flags & FunctionInfo::Private){
                    if(env->ExceptionCheck()){
                        env->ExceptionClear();
                    }
                    QString message(QLatin1String("Class %1 does not implement private pure virtual function '%2' from Qt declared in %3."));
                    message = message.arg(qtjambi_class_display_name(env, object_class));
                    message = message.arg(jniSignatureToJava(QLatin1String(info.signature), QLatin1String(info.name)));
                    message = message.arg(QLatin1String(typeEntry->qtName()));
                    Java::QtJambi::QMissingVirtualOverridingException::throwNew(env, message QTJAMBI_STACKTRACEINFO );
                }else{
                    jthrowable thr = nullptr;
                    if(env->ExceptionCheck()){
                        thr = env->ExceptionOccurred();
                        env->ExceptionClear();
                    }
                    fprintf(stderr, "vtable setup failed: %s::%s %s\n",
                            qPrintable(qtjambi_class_display_name(env, object_class)), info.name, info.signature);
                    if(thr)
                        JavaException(env, thr).raise( QTJAMBI_STACKTRACEINFO_ENV(env) );
                }
                methods << nullptr;
            }
        }
    }
    return methods;
}

const QSharedPointer<const QtJambiFunctionTable> &qtjambi_setup_vtable(JNIEnv *env, jclass object_class, jobject object, const std::type_info& typeId, const SuperTypeInfos* superTypeInfos, const QMetaObject* originalMetaObject, bool hasCustomMetaObject, JavaException& ocurredException)
{
    if(!ocurredException.object()){
        try {
            Q_UNUSED(typeId)

            Q_ASSERT(object);
            Q_ASSERT(object_class);

            int classHashCode = Java::Runtime::Object::hashCode(env,object_class);
            {
                QReadLocker locker(gTableLock());
                Q_UNUSED(locker)
                Q_ASSERT(functionTableCache());
                if(functionTableCache->contains(classHashCode))
                    return (*functionTableCache)[classHashCode];
            }

            AbstractFunctionTable * table;
            if(superTypeInfos && !superTypeInfos->isEmpty()){
                QMap<std::type_index, QList<jmethodID>> methodsByType;
                for(const SuperTypeInfo& info : *superTypeInfos){
                    if(!methodsByType.contains(info.typeId())){
                        methodsByType.insert(info.typeId(), get_methods(env, object_class, info.typeId()));
                    }
                }
                if(methodsByType.size()==1){
                    if(originalMetaObject){
                        const QMetaObject* metaObject = originalMetaObject;
                        if(!env->IsSameObject(superTypeInfos->first().javaClass(), object_class))
                            metaObject = qtjambi_metaobject_for_class(env, object_class, metaObject, hasCustomMetaObject);
                        if(!metaObject){
                            table = new FunctionTable(env, object_class, methodsByType.first());
                        }else if(const QtJambiMetaObject* mo = QtJambiMetaObject::cast(metaObject)){
                            table = new DynamicMetaObjectFunctionTable(env, object_class, methodsByType.first(), mo);
                        }else{
                            table = new MetaObjectFunctionTable(env, object_class, methodsByType.first(), metaObject);
                        }
                    }else{
                        table = new FunctionTable(env, object_class, methodsByType.first());
                    }
                }else{
                    if(originalMetaObject){
                        const QMetaObject* metaObject = originalMetaObject;
                        if(!env->IsSameObject(superTypeInfos->first().javaClass(), object_class))
                            metaObject = qtjambi_metaobject_for_class(env, object_class, metaObject, hasCustomMetaObject);
                        if(!metaObject){
                            table = new InterfaceFunctionTable(env, object_class, methodsByType);
                        }else if(const QtJambiMetaObject* mo = QtJambiMetaObject::cast(metaObject)){
                            table = new DynamicMetaObjectInterfaceFunctionTable(env, object_class, methodsByType, mo);
                        }else{
                            table = new MetaObjectInterfaceFunctionTable(env, object_class, methodsByType, metaObject);
                        }
                    }else{
                        table = new InterfaceFunctionTable(env, object_class, methodsByType);
                    }
                }
            }else{
                // this should be exactly the qt type, so no virtual overrides.
                if(originalMetaObject){
                    Q_ASSERT(!QtJambiMetaObject::isInstance(originalMetaObject));
                    table = new MetaObjectEmptyFunctionTable(env, object_class, originalMetaObject);
                }else{
                    table = new EmptyFunctionTable(env, object_class);
                }
            }
            QWriteLocker locker(gTableLock());
            Q_UNUSED(locker)
            Q_ASSERT(functionTableCache());
            if(!functionTableCache->contains(classHashCode))
                functionTableCache->insert(classHashCode, QSharedPointer<const QtJambiFunctionTable>(table));
            else
                delete table;
            return (*functionTableCache)[classHashCode];
        }catch(const JavaException& exn){
            ocurredException.addSuppressed(env, exn);
        }
    }
    static QSharedPointer<const QtJambiFunctionTable> noPointer;
    return noPointer;
}
