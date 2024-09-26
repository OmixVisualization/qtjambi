/****************************************************************************
**
** Copyright (C) 2009-2024 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
**
** This file is part of Qt Jambi.
**
** $BEGIN_LICENSE$
** GNU Lesser General Public License Usage
** This file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
** $END_LICENSE$
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/


#include <QtCore/QHash>
#include <QtCore/QVector>
#include <QtCore/QString>
#include <QtCore/QCoreApplication>
#include <QtCore/QReadWriteLock>
#include "qtjambiapi.h"
#include "java_p.h"
#include "supertypeinfo_p.h"
#include "registryutil_p.h"

typedef QHash<jint,SuperTypeInfos> SuperTypeInfosMap;
Q_GLOBAL_STATIC_WITH_ARGS(QReadWriteLock, gSuperTypeInfosLock, (QReadWriteLock::Recursive))
Q_GLOBAL_STATIC(SuperTypeInfosMap, gSuperTypeInfosMap)

void resolveClasses(JNIEnv *env, QString signature, QList<jclass> &argumentTypes){
    int arrayDepth = 0;
    while(!signature.isEmpty()){
        QChar c = signature.front();
        if(c==QLatin1Char('[')){
            ++arrayDepth;
            signature = signature.mid(1);
        }else{
            jclass cls;
            if(c==QLatin1Char('L')){
                signature = signature.mid(1);
                auto idx = signature.indexOf(QLatin1Char(';'));
                Q_ASSERT(idx>0);
                QString className = signature.mid(0, idx);
                cls = JavaAPI::resolveClass(env, qPrintable(className));
                signature = signature.mid(idx+1);
            }else{
                signature = signature.mid(1);
                if(c==QLatin1Char('I')){
                    cls = JavaAPI::resolveClass(env, "int");
                }else if(c==QLatin1Char('J')){
                    cls = JavaAPI::resolveClass(env, "long");
                }else if(c==QLatin1Char('S')){
                    cls = JavaAPI::resolveClass(env, "short");
                }else if(c==QLatin1Char('B')){
                    cls = JavaAPI::resolveClass(env, "byte");
                }else if(c==QLatin1Char('D')){
                    cls = JavaAPI::resolveClass(env, "double");
                }else if(c==QLatin1Char('F')){
                    cls = JavaAPI::resolveClass(env, "float");
                }else if(c==QLatin1Char('Z')){
                    cls = JavaAPI::resolveClass(env, "boolean");
                }else if(c==QLatin1Char('C')){
                    cls = JavaAPI::resolveClass(env, "char");
                }else{
                    cls = nullptr;
                    Q_ASSERT(false);
                }
            }
            if(arrayDepth>0){
                cls = getArrayClass(env, cls, arrayDepth);
                arrayDepth = 0;
            }
            argumentTypes << cls;
        }
    }
}

SuperTypeInfo::SuperTypeInfo(  const char* _qtName,
                const QString& _className,
                jclass _javaClass,
                size_t _size,
                bool _hasShell,
                size_t _offset,
                const QVector<ResolvedConstructorInfo>& _constructorInfos,
                Destructor _destructor,
                PtrOwnerFunction _ownerFunction,
                const std::type_info& typeId,
                char const* _iid)
    : m_qtName(_qtName),
      m_className(_className),
      m_javaClass(_javaClass),
      m_size(_size),
      m_hasShell(_hasShell),
      m_offset(_offset),
      m_constructorInfos(_constructorInfos),
      m_destructor(_destructor),
      m_ownerFunction(_ownerFunction),
      m_typeId(&typeId),
      m_iid(_iid)
{}

SuperTypeInfo::SuperTypeInfo()
    : m_qtName(nullptr),
      m_className(),
      m_javaClass(nullptr),
      m_size(0),
      m_hasShell(false),
      m_offset(0),
      m_constructorInfos(),
      m_destructor(nullptr),
      m_ownerFunction(nullptr),
      m_typeId(nullptr),
      m_iid(nullptr)
{}

SuperTypeInfo::SuperTypeInfo(  const SuperTypeInfo& other )
    : m_qtName(other.m_qtName),
      m_className(other.m_className),
      m_javaClass(other.m_javaClass),
      m_size(other.m_size),
      m_hasShell(other.m_hasShell),
      m_offset(other.m_offset),
      m_constructorInfos(other.m_constructorInfos),
      m_destructor(other.m_destructor),
      m_ownerFunction(other.m_ownerFunction),
      m_typeId(other.m_typeId),
      m_iid(other.m_iid)
{}

SuperTypeInfo& SuperTypeInfo::operator=(  const SuperTypeInfo& other )
{
      m_qtName = other.m_qtName;
      m_className = other.m_className;
      m_javaClass = other.m_javaClass;
      m_size = other.m_size;
      m_hasShell = other.m_hasShell;
      m_offset = other.m_offset;
      m_constructorInfos = other.m_constructorInfos;
      m_destructor = other.m_destructor;
      m_ownerFunction = other.m_ownerFunction;
      m_typeId = other.m_typeId;
      m_iid = other.m_iid;
      return *this;
}

SuperTypeInfo& SuperTypeInfo::operator=(  SuperTypeInfo&& other )
{
      m_qtName = std::move(other.m_qtName);
      m_className = std::move(other.m_className);
      m_javaClass = std::move(other.m_javaClass);
      m_size = std::move(other.m_size);
      m_hasShell = std::move(other.m_hasShell);
      m_offset = std::move(other.m_offset);
      m_constructorInfos = std::move(other.m_constructorInfos);
      m_destructor = std::move(other.m_destructor);
      m_ownerFunction = std::move(other.m_ownerFunction);
      m_typeId = std::move(other.m_typeId);
      m_iid = std::move(other.m_iid);
      return *this;
}

SuperTypeInfo::SuperTypeInfo(  SuperTypeInfo&& other )
    : m_qtName(std::move(other.m_qtName)),
      m_className(std::move(other.m_className)),
      m_javaClass(std::move(other.m_javaClass)),
      m_size(std::move(other.m_size)),
      m_hasShell(std::move(other.m_hasShell)),
      m_offset(std::move(other.m_offset)),
      m_constructorInfos(std::move(other.m_constructorInfos)),
      m_destructor(std::move(other.m_destructor)),
      m_ownerFunction(std::move(other.m_ownerFunction)),
      m_typeId(std::move(other.m_typeId)),
      m_iid(std::move(other.m_iid))
{}

void SuperTypeInfo::swap(SuperTypeInfo& other){
    qSwap(m_qtName, other.m_qtName);
    m_className.swap(other.m_className);
    qSwap(m_javaClass, other.m_javaClass);
    qSwap(m_size, other.m_size);
    qSwap(m_hasShell, other.m_hasShell);
    qSwap(m_offset, other.m_offset);
    m_constructorInfos.swap(other.m_constructorInfos);
    qSwap(m_destructor, other.m_destructor);
    qSwap(m_ownerFunction, other.m_ownerFunction);
    qSwap(m_typeId, other.m_typeId);
}

void swap(SuperTypeInfo& a, SuperTypeInfo& b) noexcept {
    a.swap(b);
}

const std::type_info& SuperTypeInfo::typeId() const {
    Q_ASSERT(m_typeId);
    return *m_typeId;
}

const char* SuperTypeInfo::qtName() const {
    return m_qtName;
}

const char* SuperTypeInfo::interfaceID() const {
    return m_iid;
}

const QString& SuperTypeInfo::className() const {
    return m_className;
}

jclass SuperTypeInfo::javaClass() const {
    return m_javaClass;
}

size_t SuperTypeInfo::size() const {
    return m_size;
}

bool SuperTypeInfo::hasShell() const {
    return m_hasShell;
}

size_t SuperTypeInfo::offset() const {
    return m_offset;
}

const QVector<ResolvedConstructorInfo>& SuperTypeInfo::constructorInfos() const {
    return m_constructorInfos;
}

Destructor SuperTypeInfo::destructor() const {
    return m_destructor;
}

PtrOwnerFunction SuperTypeInfo::ownerFunction() const {
    return m_ownerFunction;
}


SuperTypeInfos::SuperTypeInfos(JNIEnv *env, jobject interfaceInfos) : QVector<SuperTypeInfo>(), m_interfaceInfos(env, interfaceInfos){}

SuperTypeInfos::SuperTypeInfos(const SuperTypeInfos& other) : QVector<SuperTypeInfo>(other), m_interfaceInfos(other.m_interfaceInfos){
}

SuperTypeInfos::SuperTypeInfos(SuperTypeInfos&& other) : QVector<SuperTypeInfo>(std::move(other)), m_interfaceInfos(std::move(other.m_interfaceInfos)){
}

SuperTypeInfos& SuperTypeInfos::operator=(const SuperTypeInfos& other){
    if(this!=&other){
        QVector<SuperTypeInfo>& _this = *this;
        _this = other;
        m_interfaceInfos = other.m_interfaceInfos;
    }
    return *this;
}

SuperTypeInfos& SuperTypeInfos::operator=(SuperTypeInfos&& other){
    if(this!=&other){
        QVector<SuperTypeInfo>& _this = *this;
        _this = std::move(other);
        m_interfaceInfos = std::move(other.m_interfaceInfos);
    }
    return *this;
}

SuperTypeInfos::~SuperTypeInfos(){
}

jobject SuperTypeInfos::interfaceInfos() const{
    return m_interfaceInfos;
}

void clearSuperTypesAtShutdown(JNIEnv *env){
    SuperTypeInfosMap superTypeInfosMap;
    {
        QWriteLocker locker(gSuperTypeInfosLock());
        Q_UNUSED(locker)
        if(!gSuperTypeInfosMap.isDestroyed())
            superTypeInfosMap.swap(*gSuperTypeInfosMap);
    }
    if(env){
        for(SuperTypeInfos& info : superTypeInfosMap){
            info.m_interfaceInfos.clear(env);
        }
    }
}

SuperTypeInfos SuperTypeInfos::fromClass(JNIEnv *env, jclass clazz)
{
    if(clazz && !Java::Runtime::Class::isPrimitive(env, clazz)){
        //QString classNameStrg = qtjambi_cast<QString>(env, Java::Runtime::Class::getName(env, clazz));
        //key = qHash(qPrintable(classNameStrg));
        jint hashCode = Java::Runtime::Object::hashCode(env, Java::Runtime::Class::getName(env, clazz));
        {
            QReadLocker locker(gSuperTypeInfosLock());
            Q_UNUSED(locker)
            if(gSuperTypeInfosMap->contains(hashCode)){
                return (*gSuperTypeInfosMap)[hashCode];
            }
        }
        {
            QTJAMBI_JNI_LOCAL_FRAME(env, 512);
            jobject interfaceInfos = Java::QtJambi::ClassAnalyzerUtility::getImplementedInterfaceInfo(env, clazz);
            SuperTypeInfos superTypeInfos(env, interfaceInfos);
            size_t offset = 0;
            if(jclass closestQtClass = JavaAPI::resolveClosestQtSuperclass(env, clazz, nullptr)){
                Q_ASSERT(env->GetObjectRefType(closestQtClass)==JNIGlobalRefType);
                QString className = QtJambiAPI::getClassName(env, closestQtClass).replace('.', '/');
                if(const std::type_info* typeId = getTypeByJavaName(className)){
                    const char* qtName = getQtName(*typeId);
                    const char* iid = registeredInterfaceIDForClassName(className);
                    size_t size = getShellSize(*typeId);
                    bool hasShell = true;
                    if(size==0){
                        hasShell = false;
                        size = getValueSize(*typeId);
                    }
                    const QVector<const ConstructorInfo>* constructorInfos = registeredConstructorInfos(*typeId);
                    Destructor destructor = registeredDestructor(*typeId);
                    QVector<ResolvedConstructorInfo> resolvedConstructorInfos;
                    if(constructorInfos && destructor){
                        resolvedConstructorInfos.resize(constructorInfos->size());
                        for (int i=0; i<constructorInfos->size(); ++i) {
                            const ConstructorInfo& info = constructorInfos->at(i);
                            resolvedConstructorInfos[i].constructorFunction = info.constructorFunction;
                            if(info.signature){
                                resolveClasses(env, QLatin1String(info.signature), resolvedConstructorInfos[i].argumentTypes);
                            }
                        }
                    }
                    Java::QtJambi::ClassAnalyzerUtility::checkImplementation(env, closestQtClass, clazz);
                    if(Java::QtCore::QObject::isAssignableFrom(env, closestQtClass)){
                        superTypeInfos << SuperTypeInfo(qtName, className, closestQtClass, size, hasShell, offset, resolvedConstructorInfos, destructor, [](const void* ptr) -> const QObject* { return reinterpret_cast<const QObject*>(ptr); }, *typeId, iid);
                    }else{
                        superTypeInfos << SuperTypeInfo(qtName, className, closestQtClass, size, hasShell, offset, resolvedConstructorInfos, destructor, registeredOwnerFunction(*typeId), *typeId, iid);
                    }
                    offset += size + sizeof(void*);
                }
            }

            if(interfaceInfos && Java::Runtime::Map::size(env, interfaceInfos)!=0){
                jobject keySet = Java::Runtime::Map::keySet(env, interfaceInfos);
                jobject iterator = Java::Runtime::Collection::iterator(env, keySet);
                while(Java::Runtime::Iterator::hasNext(env, iterator)) {
                    jclass interfaceClass = jclass(Java::Runtime::Iterator::next(env, iterator));
                    QString className = QtJambiAPI::getClassName(env, interfaceClass).replace('.', '/');
                    if(const std::type_info* typeId = getTypeByJavaName(className)){
                        const char* qtName = getQtName(*typeId);
                        const char* iid = registeredInterfaceIDForClassName(className);
                        size_t size = getShellSize(*typeId);
                        bool hasShell = true;
                        if(size==0){
                            hasShell = false;
                            size = getValueSize(*typeId);
                        }
                        const QVector<const ConstructorInfo>* constructorInfos = registeredConstructorInfos(*typeId);
                        Destructor destructor = registeredDestructor(*typeId);
                        if(qtName && size>0 && constructorInfos && !constructorInfos->isEmpty() && destructor){
                            Java::QtJambi::ClassAnalyzerUtility::checkImplementation(env, interfaceClass, clazz);
                            QVector<ResolvedConstructorInfo> resolvedConstructorInfos(constructorInfos->size());
                            for (int i=0; i<constructorInfos->size(); ++i) {
                                const ConstructorInfo& info = constructorInfos->at(i);
                                resolvedConstructorInfos[i].constructorFunction = info.constructorFunction;
                                if(info.signature){
                                    resolveClasses(env, QLatin1String(info.signature), resolvedConstructorInfos[i].argumentTypes);
                                }
                            }
                            superTypeInfos << SuperTypeInfo(qtName, className, getGlobalClassRef(env, interfaceClass, qPrintable(className)), size, hasShell, offset, resolvedConstructorInfos, destructor, registeredOwnerFunction(*typeId), *typeId, iid);
                            offset += size + sizeof(void*);
                        }else{
                            jthrowable t = Java::QtJambi::QInterfaceCannotBeSubclassedException::newInstance(env, interfaceClass);
                            JavaException(env, t).raise( QTJAMBI_STACKTRACEINFO_ENV(env) );
                        }
                    }else{
                        jthrowable t = Java::QtJambi::QInterfaceCannotBeSubclassedException::newInstance(env, interfaceClass);
                        JavaException(env, t).raise( QTJAMBI_STACKTRACEINFO_ENV(env) );
                    }
                }
            }
            QWriteLocker locker(gSuperTypeInfosLock());
            Q_UNUSED(locker)
            if(gSuperTypeInfosMap->contains(hashCode)) // if it has been inserted meanwhile by other thread
                return (*gSuperTypeInfosMap)[hashCode];
            (*gSuperTypeInfosMap)[hashCode] = std::move(superTypeInfos);
            return (*gSuperTypeInfosMap)[hashCode];
        }
    }
    return QtJambiPrivate::getDefaultValue<SuperTypeInfos>();
}
