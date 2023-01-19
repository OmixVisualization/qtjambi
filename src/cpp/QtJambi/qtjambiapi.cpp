/****************************************************************************
**
** Copyright (C) 2009-2023 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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

#include "typemanager_p.h"
#include "exception.h"
#include "java_p.h"
#include "qtjambilink_p.h"

#include "qtjambi_cast.h"

// type name helpers...

QString QtJambiAPI::getClassName(JNIEnv *env, jclass java_class)
{
    Q_ASSERT(java_class);
    //Q_ASSERT(Java::Runtime::Class::isInstanceOf(env, java_class));  // check the java object is right type
    jstring name = Java::Runtime::Class::getName(env,java_class);
    QString fullJavaName = qtjambi_cast<QString>(env, name);
    if(Java::Runtime::Class::isSynthetic(env,java_class)){
        jobjectArray interfaces = Java::Runtime::Class::getInterfaces(env, java_class);
        QString strClassName;
        QString strPackage;
        QtJambiTypeManager::splitClassName(strClassName, strPackage, fullJavaName, QLatin1Char('.'));
        if(interfaces && env->GetArrayLength(interfaces)>0){
            registerLambdaClass(env, java_class, qPrintable(fullJavaName.replace(".", "/")));
            java_class = jclass( env->GetObjectArrayElement(interfaces, 0) );
            fullJavaName = QtJambiAPI::getClassName(env, java_class);
            QtJambiTypeManager::splitClassName(strClassName, strPackage, fullJavaName, QLatin1Char('.'));
        }
    }
    return fullJavaName;
}

QString QtJambiAPI::getObjectClassName(JNIEnv *env, jobject java_object)
{
    return java_object ? QtJambiAPI::getClassName(env, env->GetObjectClass(java_object)) : QString{};
}

#ifndef Q_CC_MSVC
QByteArray analyze_type_name(QByteArray& typeName);
QByteArray analyze_functionpointer(QByteArray& typeName);
QByteArray analyze_functiondecl(QByteArray& typeName);
QByteArrayList analyze_namespace(QByteArray& typeName);
QByteArrayList analyze_instantiations(QByteArray& typeName);

QByteArray analyze_type_name(QByteArray& typeName){
    if(!typeName.isEmpty()){
        bool isPointer = false;
        bool isReference = false;
        if(typeName.startsWith("PF")){
            return analyze_functionpointer(typeName);
        }else if(typeName.startsWith("F")){
            return analyze_functiondecl(typeName);
        }else if(typeName.startsWith("PK")){
            isPointer = true;
            typeName = typeName.mid(2);
        }else if(typeName.startsWith("P")){
            isPointer = true;
            typeName = typeName.mid(1);
        }else if(typeName.startsWith("R")){
            isReference = true;
            typeName = typeName.mid(1);
        }
        if(!typeName.isEmpty()){
            QByteArray result;
            if(typeName.startsWith("St3__1")){
                typeName = typeName.mid(6);
                result = "std::";
            }else if(typeName.startsWith("St")){
                typeName = typeName.mid(2);
                result = "std::";
            }
            switch(typeName[0]){
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
                {
                    QString number;
                    number += typeName[0];
                    typeName = typeName.mid(1);
                    while(!typeName.isEmpty() && typeName[0]>='0' && typeName[0]<='9'){
                        number += typeName[0];
                        typeName = typeName.mid(1);
                    }
                    int count = number.toInt();
                    result += typeName.mid(0, count);
                    typeName = typeName.mid(count);
                }
                break;
            case 'b':
                result = "bool";
                typeName = typeName.mid(1);
                break;
            case 'a':
                result = "signed char";
                typeName = typeName.mid(1);
                break;
            case 'c':
                result = "char";
                typeName = typeName.mid(1);
                break;
            case 'h':
                result = "unsigned char";
                typeName = typeName.mid(1);
                break;
            case 'f':
                result = "float";
                typeName = typeName.mid(1);
                break;
            case 'd':
                result = "double";
                typeName = typeName.mid(1);
                break;
            case 'i':
                result = "int";
                typeName = typeName.mid(1);
                break;
            case 'j':
                result = "unsigned int";
                typeName = typeName.mid(1);
                break;
            case 'x':
                result = "long long";
                typeName = typeName.mid(1);
                break;
            case 'y':
                result = "unsigned long long";
                typeName = typeName.mid(1);
                break;
            case 's':
                result = "short";
                typeName = typeName.mid(1);
                break;
            case 't':
                result = "unsigned short";
                typeName = typeName.mid(1);
                break;
            case 'v':
                result = "void";
                typeName = typeName.mid(1);
                break;
            case 'N':
                result = analyze_namespace(typeName).join("::");
                break;
            default:break;
            }
            if(!result.isEmpty()){
                if(!typeName.isEmpty() && typeName[0]=='I'){
                    QByteArrayList instantiations = analyze_instantiations(typeName);
                    if(!instantiations.isEmpty()){
                        result += "<" + instantiations.join(",") + ">";
                    }
                }
                if(isPointer){
                    result += "*";
                }else if(isReference){
                    result += "&";
                }
                return result;
            }
        }
    }
    return typeName;
}

QByteArray analyze_functionpointer(QByteArray& typeName){
    QByteArrayList types;
    typeName = typeName.mid(2);//PF
    while(!typeName.isEmpty()){
        if(typeName[0]=='E'){
            typeName = typeName.mid(1);
            break;
        }else{
            auto size1 = typeName.size();
            types << analyze_type_name(typeName);
            auto size2 = typeName.size();
            if(size1==size2){
                auto idx = typeName.indexOf('E');
                if(idx<0)
                    typeName.clear();
                else
                    typeName = typeName.mid(idx+1);
            }
        }
    }
    QByteArray result;
    if(!types.isEmpty()){
        result += types.takeFirst() + "(*)(";
        if(types.size()>1 || types[0]!="void"){
            result += types.join(",");
        }
        result += ")";
    }
    return result;
}

QByteArray analyze_functiondecl(QByteArray& typeName){
    QByteArrayList types;
    typeName = typeName.mid(1);//F
    while(!typeName.isEmpty()){
        if(typeName[0]=='E'){
            typeName = typeName.mid(1);
            break;
        }else{
            auto size1 = typeName.size();
            types << analyze_type_name(typeName);
            auto size2 = typeName.size();
            if(size1==size2){
                auto idx = typeName.indexOf('E');
                if(idx<0)
                    typeName.clear();
                else
                    typeName = typeName.mid(idx+1);
            }
        }
    }
    QByteArray result;
    if(!types.isEmpty()){
        result += types.takeFirst() + "(";
        if(types.size()>1 || types[0]!="void"){
            result += types.join(",");
        }
        result += ")";
    }
    return result;
}

QByteArrayList analyze_namespace(QByteArray& typeName){
    QByteArrayList result;
    typeName = typeName.mid(1);//N
    while(!typeName.isEmpty()){
        if(typeName=="St3__1"){
        }else if(typeName[0]=='E'){
            typeName = typeName.mid(1);
            break;
        }else{
            auto size1 = typeName.size();
            result << analyze_type_name(typeName);
            auto size2 = typeName.size();
            if(size1==size2){
                auto idx = typeName.indexOf('E');
                if(idx<0)
                    typeName.clear();
                else
                    typeName = typeName.mid(idx+1);
            }
        }
    }
    return result;
}

QByteArrayList analyze_instantiations(QByteArray& typeName){
    QByteArrayList result;
    typeName = typeName.mid(1);//I
    while(!typeName.isEmpty()){
        if(typeName[0]=='I'){
            typeName = typeName.mid(1);
            result << "jlong";
        }else if(typeName[0]=='E'){
            typeName = typeName.mid(1);
            break;
        }else{
            auto size1 = typeName.size();
            result << analyze_type_name(typeName);
            auto size2 = typeName.size();
            if(size1==size2){
                auto idx = typeName.indexOf('E');
                if(idx<0)
                    typeName.clear();
                else
                    typeName = typeName.mid(idx+1);
            }
        }
    }
    return result;
}
#endif

QByteArray QtJambiAPI::typeName(const std::type_info& typeId){
    QByteArray typeName = typeId.name();
#ifdef Q_CC_MSVC
    typeName = typeName.replace(" (__cdecl*)", "(*)");
    typeName = typeName.replace(" __cdecl(", "(");
    typeName = typeName.replace("class ", "");
    typeName = typeName.replace("struct ", "");
    typeName = typeName.replace("union ", "");
    typeName = typeName.replace("enum ", "");
    typeName = typeName.replace(" & __ptr64", "&");
    typeName = typeName.replace(" const * __ptr64", "*");
    typeName = typeName.replace(" * __ptr64", "*");
    typeName = typeName.replace("(void)", "()");
    typeName = typeName.replace("__int64", "long long");
#else
    typeName = analyze_type_name(typeName);
#endif
    return typeName;
}

void QtJambiAPI::setJavaOwnershipForTopLevelObject(JNIEnv *env, QObject* qobject)
{
    if(qobject && !qobject->parent()){
        if(QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForQObject(qobject)){
            link->setJavaOwnership(env);
        }
    }
}

void QtJambiAPI::setDefaultOwnershipForTopLevelObject(JNIEnv *env, QObject* qobject)
{
    if(qobject && !qobject->parent()){
        if(QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForQObject(qobject)){
            link->setDefaultOwnership(env);
        }
    }
}

void QtJambiAPI::setCppOwnershipForTopLevelObject(JNIEnv *env, QObject* qobject)
{
    if(qobject && !qobject->parent()){
        if(QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForQObject(qobject)){
            link->setCppOwnership(env);
        }
    }
}

void QtJambiAPI::setJavaOwnership(JNIEnv *env, jobject object)
{
    if(QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForJavaObject(env, object)){
        link->setJavaOwnership(env);
    }
}

void QtJambiAPI::setDefaultOwnership(JNIEnv *env, jobject object)
{
    if(QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForJavaObject(env, object)){
        link->setDefaultOwnership(env);
    }
}

void QtJambiAPI::setCppOwnership(JNIEnv *env, jobject object)
{
    if(QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForJavaObject(env, object)){
        link->setCppOwnership(env);
    }
}

void QtJambiAPI::setJavaOwnership(JNIEnv *env, QtJambiNativeID objectId)
{
    if(QSharedPointer<QtJambiLink> link = QtJambiLink::fromNativeId(objectId)){
        link->setJavaOwnership(env);
    }
}

void QtJambiAPI::setDefaultOwnership(JNIEnv *env, QtJambiNativeID objectId)
{
    if(QSharedPointer<QtJambiLink> link = QtJambiLink::fromNativeId(objectId)){
        link->setDefaultOwnership(env);
    }
}

void QtJambiAPI::setCppOwnership(JNIEnv *env, QtJambiNativeID objectId)
{
    if(QSharedPointer<QtJambiLink> link = QtJambiLink::fromNativeId(objectId)){
        link->setCppOwnership(env);
    }
}

void QtJambiAPI::registerNonShellDeletion(void* ptr)
{
    for(const QSharedPointer<QtJambiLink>& link : QtJambiLink::findLinksForPointer(ptr)){
        if(link && !link->isQObject() && !link->isShell()){
            if(JniEnvironment env = link->noThreadInitializationOnPurge() ? JniEnvironment{300} : DefaultJniEnvironment{300}){
                link->invalidate(env);
            }else{
                link->invalidate(nullptr);
            }
        }
    }
}

bool QtJambiAPI::isShell(QtJambiNativeID nativeId)
{
    return !!nativeId ? reinterpret_cast<QtJambiLink *>(nativeId)->isShell() : false;
}

bool QtJambiAPI::javaObjectHasShell(JNIEnv *env, jobject object)
{
    if(QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForJavaObject(env, object))
        return link->isShell();
    return false;
}

bool QtJambiAPI::javaInterfaceHasShell(JNIEnv *env, jobject object)
{
    if(QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForJavaInterface(env, object))
        return link->isShell();
    return false;
}

QtJambiNativeID QtJambiAPI::javaObjectToNativeId(JNIEnv *env, jobject object){
    return QtJambiNativeID(Java::QtJambi::NativeUtility::nativeId(env, object));
}

QtJambiNativeID QtJambiAPI::javaInterfaceToNativeId(JNIEnv *env, jobject object){
    return QtJambiNativeID(Java::QtJambi::NativeUtility::nativeIdInterface(env, object));
}

void *QtJambiAPI::fromNativeId(QtJambiNativeID nativeId)
{
    return !!nativeId ? reinterpret_cast<QtJambiLink *>(nativeId)->pointer() : nullptr;
}

void *QtJambiAPI::fromNativeId(QtJambiNativeID nativeId, const std::type_info& typeId)
{
    return !!nativeId ? reinterpret_cast<QtJambiLink *>(nativeId)->typedPointer(typeId) : nullptr;
}

uint QtJambiAPI::getJavaObjectIdentity(JNIEnv *env, jobject object){
    return uint(Java::Runtime::System::identityHashCode(env, object));
}

uint QtJambiAPI::getJavaObjectHashCode(JNIEnv *env, jobject object){
    return uint(Java::Runtime::Object::hashCode(env, object));
}

void QtJambiAPI::checkPointer(JNIEnv *env, const void* ptr, const std::type_info& typeId){
    if(!ptr){
        QLatin1String java_type(getJavaName(typeId));
        QString msg = QLatin1String("Function call on incomplete object of type: %1");
        if(!java_type.isEmpty()){
            msg = msg.arg(QString(QLatin1String(java_type)).replace(QLatin1Char('/'), QLatin1Char('.')).replace(QLatin1Char('$'), QLatin1Char('.')));
        }else{
            QLatin1String qt_type(getQtName(typeId));
            if(!qt_type.isEmpty()){
                msg = msg.arg(qt_type);
            }else{
                msg = msg.arg(QLatin1String(QtJambiAPI::typeName(typeId)));
            }
        }
        Java::QtJambi::QNoNativeResourcesException::throwNew(env, msg QTJAMBI_STACKTRACEINFO );
    }
}

// array helpers...

jobjectArray QtJambiAPI::createObjectArray(JNIEnv *env, const std::type_info& componentType, jsize size){
    jclass arrayElementType = JavaAPI::resolveClass(env, getJavaInterfaceName(componentType));
    JavaException::check(env QTJAMBI_STACKTRACEINFO );
    Q_ASSERT(arrayElementType);
    jobjectArray array = env->NewObjectArray(jsize(size), arrayElementType, nullptr);
    JavaException::check(env QTJAMBI_STACKTRACEINFO );
    return array;
}

jintArray QtJambiAPI::toJIntArray(JNIEnv *__jni_env, const jint* in, jsize length) {
    jintArray out = __jni_env->NewIntArray(length);
    JavaException::check(__jni_env QTJAMBI_STACKTRACEINFO );
    __jni_env->SetIntArrayRegion(out, 0, length, in);
    JavaException::check(__jni_env QTJAMBI_STACKTRACEINFO );
    return out;
}

jshortArray QtJambiAPI::toJShortArray(JNIEnv *__jni_env, const jshort* in, jsize length) {
    jshortArray out = __jni_env->NewShortArray(length);
    JavaException::check(__jni_env QTJAMBI_STACKTRACEINFO );
    __jni_env->SetShortArrayRegion(out, 0, length, in);
    JavaException::check(__jni_env QTJAMBI_STACKTRACEINFO );
    return out;
}

jbyteArray QtJambiAPI::toJByteArray(JNIEnv *__jni_env, const jbyte* in, jsize length) {
    jbyteArray out = __jni_env->NewByteArray(length);
    JavaException::check(__jni_env QTJAMBI_STACKTRACEINFO );
    __jni_env->SetByteArrayRegion(out, 0, length, in);
    JavaException::check(__jni_env QTJAMBI_STACKTRACEINFO );
    return out;
}

jlongArray QtJambiAPI::toJLongArray(JNIEnv *__jni_env, const jlong* in, jsize length) {
    jlongArray out = __jni_env->NewLongArray(length);
    JavaException::check(__jni_env QTJAMBI_STACKTRACEINFO );
    __jni_env->SetLongArrayRegion(out, 0, length, in);
    JavaException::check(__jni_env QTJAMBI_STACKTRACEINFO );
    return out;
}

jfloatArray QtJambiAPI::toJFloatArray(JNIEnv *__jni_env, const jfloat* in, jsize length) {
    jfloatArray out = __jni_env->NewFloatArray(length);
    JavaException::check(__jni_env QTJAMBI_STACKTRACEINFO );
    __jni_env->SetFloatArrayRegion(out, 0, length, in);
    JavaException::check(__jni_env QTJAMBI_STACKTRACEINFO );
    return out;
}

jdoubleArray QtJambiAPI::toJDoubleArray(JNIEnv *__jni_env, const jdouble* in, jsize length) {
    jdoubleArray out = __jni_env->NewDoubleArray(length);
    JavaException::check(__jni_env QTJAMBI_STACKTRACEINFO );
    __jni_env->SetDoubleArrayRegion(out, 0, length, in);
    JavaException::check(__jni_env QTJAMBI_STACKTRACEINFO );
    return out;
}

jcharArray QtJambiAPI::toJCharArray(JNIEnv *__jni_env, const jchar* in, jsize length) {
    jcharArray out = __jni_env->NewCharArray(length);
    JavaException::check(__jni_env QTJAMBI_STACKTRACEINFO );
    __jni_env->SetCharArrayRegion(out, 0, length, in);
    JavaException::check(__jni_env QTJAMBI_STACKTRACEINFO );
    return out;
}

jbooleanArray QtJambiAPI::toJBooleanArray(JNIEnv *__jni_env, const jboolean* in, jsize length) {
    jbooleanArray out = __jni_env->NewBooleanArray(length);
    JavaException::check(__jni_env QTJAMBI_STACKTRACEINFO );
    __jni_env->SetBooleanArrayRegion(out, 0, length, in);
    JavaException::check(__jni_env QTJAMBI_STACKTRACEINFO );
    return out;
}

bool QtJambiAPI::isValidArray(JNIEnv *env, jobject object, const std::type_info& typeId){
    if(jclass contentType = JavaAPI::resolveClass(env, getJavaInterfaceName(typeId))){
        return isValidArray(env, object, contentType);
    }
    return false;
}

jobjectArray QtJambiAPI::toJObjectArray(JNIEnv *env, const std::type_info& typeInfo, const void* iterable, jsize length, std::function<jobject(JNIEnv *,const void*,jsize)> convertFunction) {
    jclass elementClass = JavaAPI::resolveClass(env, getJavaInterfaceName(typeInfo));
    JavaException::check(env QTJAMBI_STACKTRACEINFO );
    if(!elementClass){
        elementClass = Java::Runtime::Object::getClass(env);
    }

    Q_ASSERT(elementClass);
    jobjectArray out = nullptr;
    if(length>=0){
        out = env->NewObjectArray(length, elementClass, nullptr);
        JavaException::check(env QTJAMBI_STACKTRACEINFO );
        for (jsize i = 0; i < length; ++i) {
            env->SetObjectArrayElement(out, i, convertFunction(env, iterable, i));
            JavaException::check(env QTJAMBI_STACKTRACEINFO );
        }
    }
    return out;
}

jobjectArray QtJambiAPI::createObjectArray(JNIEnv *env, const char* componentClass, jsize size){
    jclass arrayElementType = JavaAPI::resolveClass(env, componentClass);
    JavaException::check(env QTJAMBI_STACKTRACEINFO );
    Q_ASSERT(arrayElementType);
    jobjectArray array = env->NewObjectArray(jsize(size), arrayElementType, nullptr);
    JavaException::check(env QTJAMBI_STACKTRACEINFO );
    return array;
}

// Java container helpers...

jobject QtJambiAPI::newQPair(JNIEnv *env, jobject first, jobject second) {
    return Java::QtCore::QPair::newInstance(env, first, second);
}

jobject QtJambiAPI::getQPairFirst(JNIEnv *env, jobject pair) {
    return Java::QtCore::QPair::first(env, pair);
}

jobject QtJambiAPI::getQPairSecond(JNIEnv *env, jobject pair) {
    return Java::QtCore::QPair::second(env, pair);
}

jobject QtJambiAPI::newJavaHashSet(JNIEnv *env) {
    return Java::Runtime::HashSet::newInstance(env);
}

jobject QtJambiAPI::newJavaHashMap(JNIEnv *env, int size) {
    return Java::Runtime::HashMap::newInstance(env, size);
}

jobject QtJambiAPI::newJavaTreeMap(JNIEnv *env) {
    return Java::Runtime::TreeMap::newInstance(env);
}

void QtJambiAPI::putJavaMap(JNIEnv *env, jobject map, jobject key, jobject val)
{
    (void)Java::Runtime::Map::put(env, map, key, val);
}

void QtJambiAPI::putJavaMultiMap(JNIEnv *env, jobject map, jobject key, jobject val)
{
    Java::QtJambi::AbstractMultiAssociativeContainer::putMultiMap(env, map, key, val);
}

void QtJambiAPI::clearJavaMap(JNIEnv *env, jobject map)
{
    Java::Runtime::Map::clear(env, map);
}

jobject QtJambiAPI::entrySetIteratorOfJavaMap(JNIEnv *env, jobject map)
{
    jobject set = Java::Runtime::Map::entrySet(env,map);
    return Java::Runtime::Collection::iterator(env,set);
}

jobject QtJambiAPI::keyOfJavaMapEntry(JNIEnv *env, jobject entry)
{
    return Java::Runtime::Map$Entry::getKey(env, entry);
}

jobject QtJambiAPI::valueOfJavaMapEntry(JNIEnv *env, jobject entry)
{
    return Java::Runtime::Map$Entry::getValue(env,entry);
}

jobject QtJambiAPI::newJavaArrayList(JNIEnv *env, jint size) {
    return Java::Runtime::ArrayList::newInstance(env, size);
}

void QtJambiAPI::addToJavaCollection(JNIEnv *env, jobject list, jobject obj) {
    Java::Runtime::Collection::add(env, list, obj);
}

void QtJambiAPI::clearJavaCollection(JNIEnv *env, jobject collection)
{
    Java::Runtime::Collection::clear(env,collection);
}

int QtJambiAPI::sizeOfJavaCollection(JNIEnv *env, jobject col)
{
    return int(Java::Runtime::Collection::size(env, col));
}

jobject QtJambiAPI::iteratorOfJavaCollection(JNIEnv *env, jobject col)
{
    return Java::Runtime::Collection::iterator(env, col);
}

jobject QtJambiAPI::nextOfJavaIterator(JNIEnv *env, jobject col)
{
    return Java::Runtime::Iterator::next(env, col);
}

bool QtJambiAPI::hasJavaIteratorNext(JNIEnv *env, jobject col)
{
    return Java::Runtime::Iterator::hasNext(env, col);
}

jobject QtJambiAPI::newJavaOptional(JNIEnv *env, bool hasValue, jobject object){
    if(hasValue){
        return Java::Runtime::Optional::ofNullable(env, object);
    }else{
        return Java::Runtime::Optional::empty(env);
    }
}

jobject QtJambiAPI::newJavaOptionalInt(JNIEnv *env, bool hasValue, jint value){
    if(hasValue){
        return Java::Runtime::OptionalInt::of(env, value);
    }else{
        return Java::Runtime::OptionalInt::empty(env);
    }
}

jobject QtJambiAPI::newJavaOptionalLong(JNIEnv *env, bool hasValue, jlong value){
    if(hasValue){
        return Java::Runtime::OptionalLong::of(env, value);
    }else{
        return Java::Runtime::OptionalLong::empty(env);
    }
}

jobject QtJambiAPI::newJavaOptionalDouble(JNIEnv *env, bool hasValue, jdouble value){
    if(hasValue){
        return Java::Runtime::OptionalDouble::of(env, value);
    }else{
        return Java::Runtime::OptionalDouble::empty(env);
    }
}

jobject QtJambiAPI::readJavaOptional(JNIEnv *env, jobject object, bool& isPresent){
    if((isPresent = object && Java::Runtime::Optional::isPresent(env, object))){
        Java::Runtime::Optional::get(env, object);
    }
    return nullptr;
}

jint QtJambiAPI::readJavaOptionalInt(JNIEnv *env, jobject object, bool& isPresent){
    if((isPresent = object && Java::Runtime::OptionalInt::isPresent(env, object))){
        Java::Runtime::OptionalInt::getAsInt(env, object);
    }
    return 0;
}

jlong QtJambiAPI::readJavaOptionalLong(JNIEnv *env, jobject object, bool& isPresent){
    if((isPresent = object && Java::Runtime::OptionalLong::isPresent(env, object))){
        Java::Runtime::OptionalLong::getAsLong(env, object);
    }
    return 0;
}

jdouble QtJambiAPI::readJavaOptionalDouble(JNIEnv *env, jobject object, bool& isPresent){
    if((isPresent = object && Java::Runtime::OptionalDouble::isPresent(env, object))){
        Java::Runtime::OptionalDouble::getAsDouble(env, object);
    }
    return 0;
}

// Boxing functions
jobject QtJambiAPI::toJavaIntegerObject(JNIEnv *env, jint int_value) {
    return Java::Runtime::Integer::valueOf(env, int_value);
}


jint QtJambiAPI::fromJavaIntegerObject(JNIEnv *env, jobject int_object)
{
    return int_object ? Java::Runtime::Number::intValue(env, int_object) : 0;
}


jobject QtJambiAPI::toJavaDoubleObject(JNIEnv *env, jdouble double_value) {
    return Java::Runtime::Double::valueOf(env, double_value);
}


jdouble QtJambiAPI::fromJavaDoubleObject(JNIEnv *env, jobject double_object) {
    return double_object ? Java::Runtime::Number::doubleValue(env, double_object) : 0;
}

jobject QtJambiAPI::toJavaBooleanObject(JNIEnv *env, jboolean bool_value)
{
    return Java::Runtime::Boolean::valueOf(env, bool_value);
}

bool QtJambiAPI::fromJavaBooleanObject(JNIEnv *env, jobject bool_object)
{
    return bool_object && Java::Runtime::Boolean::booleanValue(env, bool_object);
}

jlong QtJambiAPI::fromJavaLongObject(JNIEnv *env, jobject long_object)
{
    return long_object ? Java::Runtime::Number::longValue(env, long_object) : 0;
}

jobject QtJambiAPI::toJavaLongObject(JNIEnv *env, jlong long_value)
{
    return Java::Runtime::Long::valueOf(env,long_value);
}

jobject QtJambiAPI::toJavaShortObject(JNIEnv *env, jshort short_value)
{
    return Java::Runtime::Short::valueOf(env, short_value);
}

jobject QtJambiAPI::toJavaFloatObject(JNIEnv *env, jfloat float_value)
{
    return Java::Runtime::Float::valueOf(env, double(float_value));
}

jobject QtJambiAPI::toJavaByteObject(JNIEnv *env, jbyte byte_value)
{
    return Java::Runtime::Byte::valueOf(env, byte_value);
}


jchar QtJambiAPI::fromJavaCharacterObject(JNIEnv *env, jobject char_object)
{
    return char_object ? Java::Runtime::Character::charValue(env, char_object) : 0;
}

jobject QtJambiAPI::toJavaCharacterObject(JNIEnv *env, jchar char_value)
{
    return Java::Runtime::Character::valueOf(env, char_value);
}

jfloat QtJambiAPI::fromJavaFloatObject(JNIEnv *env, jobject float_object)
{
    return float_object ? Java::Runtime::Number::floatValue(env, float_object) : 0;
}

jshort QtJambiAPI::fromJavaShortObject(JNIEnv *env, jobject short_object)
{
    return short_object ? Java::Runtime::Number::shortValue(env, short_object) : 0;
}

jbyte QtJambiAPI::fromJavaByteObject(JNIEnv *env, jobject byte_object)
{
    return byte_object ? Java::Runtime::Number::byteValue(env, byte_object) : 0;
}

bool QtJambiAPI::isJavaString(JNIEnv *env, jobject obj){
    return Java::Runtime::String::isInstanceOf(env, obj);
}

bool QtJambiAPI::isQStringObject(JNIEnv *env, jobject obj){
    if(env->IsSameObject(obj, nullptr) || !Java::QtJambi::QtObjectInterface::isInstanceOf(env, obj)){
        return false;
    }
    return Java::QtCore::QString::isInstanceOf(env, obj);
}

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
bool QtJambiAPI::isQByteArrayViewObject(JNIEnv *env, jobject obj){
    return Java::QtCore::QByteArrayView::isInstanceOf(env, obj);
}
#endif

bool QtJambiAPI::isQByteArrayObject(JNIEnv *env, jobject obj){
    return Java::QtCore::QByteArray::isInstanceOf(env, obj);
}

bool QtJambiAPI::isQVariantObject(JNIEnv *env, jobject obj){
    return Java::QtCore::QVariant::isInstanceOf(env, obj);
}

bool QtJambiAPI::isQCharObject(JNIEnv *env, jobject obj){
    return Java::QtCore::QChar::isInstanceOf(env, obj);
}

jstring QtJambiAPI::toJavaString(JNIEnv *env, jobject object)
{
    jstring java_string = nullptr;
    if (object){
        if(Java::Runtime::String::isInstanceOf(env, object)){
            java_string = jstring(object);
        }else if(Java::Runtime::CharSequence::isInstanceOf(env, object)){
            java_string = Java::Runtime::Object::toString(env, object);
            JavaException::check(env QTJAMBI_STACKTRACEINFO );
        }else{
            JavaException::raiseIllegalArgumentException(env, QStringLiteral("Cannot cast object of type %1 to String").arg(getObjectClassName(env, object).replace('$', '.')) QTJAMBI_STACKTRACEINFO );
        }
    }
    return java_string;
}

bool QtJambiAPI::enumValue(JNIEnv *env, jobject java_object, void* ptr, size_t size)
{
    switch(size){
    case sizeof(qint8):
        if(Java::Runtime::Number::isInstanceOf(env, java_object)){
            *reinterpret_cast<qint8*>(ptr) = QtJambiAPI::fromJavaByteObject(env, java_object);
        }else if(Java::QtJambi::QFlags::isInstanceOf(env, java_object)){
            *reinterpret_cast<qint8*>(ptr) = qint8(Java::QtJambi::QFlags::value(env, java_object));
        }else if(Java::QtJambi::QtEnumerator::isInstanceOf(env, java_object)){
            *reinterpret_cast<qint8*>(ptr) = qint8(Java::QtJambi::QtEnumerator::value(env,java_object));
        }else if(Java::QtJambi::QtByteEnumerator::isInstanceOf(env, java_object)){
            *reinterpret_cast<qint8*>(ptr) = qint8(Java::QtJambi::QtByteEnumerator::value(env, java_object));
        }else if(Java::QtJambi::QtShortEnumerator::isInstanceOf(env, java_object)){
            *reinterpret_cast<qint8*>(ptr) = qint8(Java::QtJambi::QtShortEnumerator::value(env, java_object));
        }else if(Java::QtJambi::QtLongEnumerator::isInstanceOf(env, java_object)){
            *reinterpret_cast<qint8*>(ptr) = qint8(Java::QtJambi::QtLongEnumerator::value(env, java_object));
        }else if(Java::Runtime::Enum::isInstanceOf(env, java_object)){
            *reinterpret_cast<qint8*>(ptr) = qint8(Java::Runtime::Enum::ordinal(env, java_object));
        }else{
            *reinterpret_cast<qint8*>(ptr) = 0;
            return false;
        }
        return true;
    case sizeof(qint16):
        if(Java::Runtime::Number::isInstanceOf(env, java_object)){
            *reinterpret_cast<qint16*>(ptr) = QtJambiAPI::fromJavaByteObject(env, java_object);
        }else if(Java::QtJambi::QFlags::isInstanceOf(env, java_object)){
            *reinterpret_cast<qint16*>(ptr) = qint16(Java::QtJambi::QFlags::value(env, java_object));
        }else if(Java::QtJambi::QtEnumerator::isInstanceOf(env, java_object)){
            *reinterpret_cast<qint16*>(ptr) = qint16(Java::QtJambi::QtEnumerator::value(env,java_object));
        }else if(Java::QtJambi::QtByteEnumerator::isInstanceOf(env, java_object)){
            *reinterpret_cast<qint16*>(ptr) = qint16(Java::QtJambi::QtByteEnumerator::value(env, java_object));
        }else if(Java::QtJambi::QtShortEnumerator::isInstanceOf(env, java_object)){
            *reinterpret_cast<qint16*>(ptr) = qint16(Java::QtJambi::QtShortEnumerator::value(env, java_object));
        }else if(Java::QtJambi::QtLongEnumerator::isInstanceOf(env, java_object)){
            *reinterpret_cast<qint16*>(ptr) = qint16(Java::QtJambi::QtLongEnumerator::value(env, java_object));
        }else if(Java::Runtime::Enum::isInstanceOf(env, java_object)){
            *reinterpret_cast<qint16*>(ptr) = qint16(Java::Runtime::Enum::ordinal(env, java_object));
        }else{
            *reinterpret_cast<qint16*>(ptr) = 0;
            return false;
        }
        return true;
    case sizeof(qint32):
        if(Java::Runtime::Number::isInstanceOf(env, java_object)){
            *reinterpret_cast<qint32*>(ptr) = QtJambiAPI::fromJavaByteObject(env, java_object);
        }else if(Java::QtJambi::QFlags::isInstanceOf(env, java_object)){
            *reinterpret_cast<qint32*>(ptr) = qint32(Java::QtJambi::QFlags::value(env, java_object));
        }else if(Java::QtJambi::QtEnumerator::isInstanceOf(env, java_object)){
            *reinterpret_cast<qint32*>(ptr) = qint32(Java::QtJambi::QtEnumerator::value(env,java_object));
        }else if(Java::QtJambi::QtByteEnumerator::isInstanceOf(env, java_object)){
            *reinterpret_cast<qint32*>(ptr) = qint32(Java::QtJambi::QtByteEnumerator::value(env, java_object));
        }else if(Java::QtJambi::QtShortEnumerator::isInstanceOf(env, java_object)){
            *reinterpret_cast<qint32*>(ptr) = qint32(Java::QtJambi::QtShortEnumerator::value(env, java_object));
        }else if(Java::QtJambi::QtLongEnumerator::isInstanceOf(env, java_object)){
            *reinterpret_cast<qint32*>(ptr) = qint32(Java::QtJambi::QtLongEnumerator::value(env, java_object));
        }else if(Java::Runtime::Enum::isInstanceOf(env, java_object)){
            *reinterpret_cast<qint32*>(ptr) = qint32(Java::Runtime::Enum::ordinal(env, java_object));
        }else{
            *reinterpret_cast<qint32*>(ptr) = 0;
            return false;
        }
        return true;
    case sizeof(qint64):
        if(Java::Runtime::Number::isInstanceOf(env, java_object)){
            *reinterpret_cast<qint64*>(ptr) = QtJambiAPI::fromJavaByteObject(env, java_object);
        }else if(Java::QtJambi::QFlags::isInstanceOf(env, java_object)){
            *reinterpret_cast<qint64*>(ptr) = qint64(Java::QtJambi::QFlags::value(env, java_object));
        }else if(Java::QtJambi::QtEnumerator::isInstanceOf(env, java_object)){
            *reinterpret_cast<qint64*>(ptr) = qint64(Java::QtJambi::QtEnumerator::value(env,java_object));
        }else if(Java::QtJambi::QtByteEnumerator::isInstanceOf(env, java_object)){
            *reinterpret_cast<qint64*>(ptr) = qint64(Java::QtJambi::QtByteEnumerator::value(env, java_object));
        }else if(Java::QtJambi::QtShortEnumerator::isInstanceOf(env, java_object)){
            *reinterpret_cast<qint64*>(ptr) = qint64(Java::QtJambi::QtShortEnumerator::value(env, java_object));
        }else if(Java::QtJambi::QtLongEnumerator::isInstanceOf(env, java_object)){
            *reinterpret_cast<qint64*>(ptr) = qint64(Java::QtJambi::QtLongEnumerator::value(env, java_object));
        }else if(Java::Runtime::Enum::isInstanceOf(env, java_object)){
            *reinterpret_cast<qint64*>(ptr) = qint64(Java::Runtime::Enum::ordinal(env, java_object));
        }else{
            *reinterpret_cast<qint64*>(ptr) = 0;
            return false;
        }
        return true;
    default:
        return false;
    }
}

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
jobject QtJambiAPI::createQPromise(JNIEnv* env, jobject futureInterface, const void* promise, QtJambiScope* scope){
    for(const QSharedPointer<QtJambiLink>& link : QtJambiLink::findLinksForPointer(promise)){
        if(link){
            jobject ni = link->getJavaObjectLocalRef(env);
            if(Java::QtCore::QPromise$NativeInstance::isInstanceOf(env, ni)){
                jobject result = Java::QtCore::QPromise$NativeInstance::promise(env, ni);
                if(result)
                    return result;
            }
        }
    }
    jobject result = Java::QtCore::QPromise::newInstance(env, futureInterface, true);
    jobject ni = QtJambiAPI::convertNativeToJavaObject(env, promise, Java::QtCore::QPromise$NativeInstance::getClass(env), false, false);
    if(scope){
        JObjectWrapper _result(env, result);
        scope->addFinalAction([env, _result](){
            jobject ni = Java::QtCore::QPromise::nativeInstance(env, _result.object());
            Java::QtCore::QPromise::set_nativeInstance(env, _result.object(), nullptr);
            InvalidateAfterUse::invalidate(env, ni);
        });
    }
    Java::QtCore::QPromise$NativeInstance::set_promise(env, ni, result);
    Java::QtCore::QPromise::set_nativeInstance(env, result, ni);
    return result;
}

void* QtJambiAPI::getNativeQPromise(JNIEnv* env, jobject promise){
    jobject ni = Java::QtCore::QPromise::nativeInstance(env, promise);
    return ni ? QtJambiAPI::convertJavaObjectToNative(env, ni) : nullptr;
}
#endif

void QtJambiAPI::setQQmlListPropertyElementType(JNIEnv *env, jobject list, jobject elementType){
    Java::QtQml::QQmlListProperty::set_elementType(env, list, elementType);
}

