/****************************************************************************
**
** Copyright (C) 1992-2009 Nokia. All rights reserved.
** Copyright (C) 2009-2015 Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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

#include "qtdynamicmetaobject.h"
#include "qtjambi_core.h"
#include "qtjambitypemanager_p.h"

#include <QtCore>
#include <QtCore/QHash>
#include <QtCore/QVarLengthArray>
#include <QtCore/QMetaEnum>

/**
 * this is of course the worst way to
 * make dynamic meta objects available inside
 * the static_meta_call method, but currently, there
 * is no other way possible.
 * @brief staticMetaContexts
 * @author Peter Droste
 */
static QList<QtDynamicMetaObject*> qtjambi_static_meta_contexts;

class QtDynamicMetaObjectPrivate
{
    /**
     * this list stores all texts describing the class meta object.
     */
    QList<char*> stringdataList;
    QtDynamicMetaObject *q_ptr;
    Q_DECLARE_PUBLIC(QtDynamicMetaObject);

public:
    QtDynamicMetaObjectPrivate(QtDynamicMetaObject *q, JNIEnv *env, jclass java_class, const QMetaObject *original_meta_object);
    ~QtDynamicMetaObjectPrivate();

    void initialize(JNIEnv *jni_env, jclass java_class, const QMetaObject *original_meta_object);
    void invokeMethod(JNIEnv *env, jobject object, jobject method_object, void **_a, const QString &signature = QString(), const QString &native_signature = QString()) const;
    void invokeConstructor(JNIEnv *env, jobject constuctor_object, void **_a, const QString &_signature = QString(), const QString &qtSignature = QString()) const;

    typedef void(* StaticMetaCallFunction)(QObject *, QMetaObject::Call, int, void **);
private:
    StaticMetaCallFunction getStaticMetaCallFunctionPointer();

    int m_method_count;
    int m_signal_count;
    int m_constructor_count;
    int m_property_count;

    QString m_clazz_name;
    jobjectArray m_methods;
    jobjectArray m_signals;
    jobjectArray m_constructors; //FIX: using java constructors from QMetaObject

    jobjectArray m_property_readers;
    jobjectArray m_property_writers;
    jobjectArray m_property_resetters;
    jobjectArray m_property_notifies;
    jobjectArray m_property_designable_resolvers;
    jobjectArray m_property_scriptable_resolvers;
    jobjectArray m_property_editable_resolvers;
    jobjectArray m_property_stored_resolvers;
    jobjectArray m_property_user_resolvers;

    QString *m_original_signatures;
};

QtDynamicMetaObjectPrivate::QtDynamicMetaObjectPrivate(QtDynamicMetaObject *q, JNIEnv *env, jclass java_class, const QMetaObject *original_meta_object)
    : q_ptr(q), m_method_count(-1), m_signal_count(0),  m_constructor_count(0), m_property_count(0),
      m_clazz_name(qtjambi_class_name(env, java_class).replace('.', '/')),
      m_methods(0), m_signals(0), m_constructors(0),
      m_property_readers(0), m_property_writers(0), m_property_resetters(0), m_property_notifies(0), m_property_designable_resolvers(0),
      m_property_scriptable_resolvers(0), m_property_editable_resolvers(0), m_property_stored_resolvers(0),
      m_property_user_resolvers(0), m_original_signatures(0)
{
    Q_ASSERT(env != 0);
    Q_ASSERT(java_class != 0);

    initialize(env, java_class, original_meta_object);
}

QtDynamicMetaObjectPrivate::~QtDynamicMetaObjectPrivate()
{
    JNIEnv *env = qtjambi_current_environment();
    if (env != 0) {
        if (m_methods != 0) env->DeleteGlobalRef(m_methods);
        if (m_signals != 0) env->DeleteGlobalRef(m_signals);
        if (m_constructors != 0) env->DeleteGlobalRef(m_constructors);
        if (m_property_readers != 0) env->DeleteGlobalRef(m_property_readers);
        if (m_property_writers != 0) env->DeleteGlobalRef(m_property_writers);
        if (m_property_resetters != 0) env->DeleteGlobalRef(m_property_resetters);
        if (m_property_notifies != 0) env->DeleteGlobalRef(m_property_notifies);
        if (m_property_designable_resolvers != 0) env->DeleteGlobalRef(m_property_designable_resolvers);
        if (m_property_scriptable_resolvers != 0) env->DeleteGlobalRef(m_property_scriptable_resolvers);
        if (m_property_editable_resolvers != 0) env->DeleteGlobalRef(m_property_editable_resolvers);
        if (m_property_stored_resolvers != 0) env->DeleteGlobalRef(m_property_stored_resolvers);
        if (m_property_user_resolvers != 0) env->DeleteGlobalRef(m_property_user_resolvers);
    }

    foreach(char* strg , stringdataList){
        delete[] strg;
    }
    stringdataList.clear();

    delete[] m_original_signatures;
}

static void generic_static_meta_call(int dynamicMetaObjectIndex, QObject * o, QMetaObject::Call cl, int idx, void ** argv) {
    if(o){
        o->metaObject()->metacall(o, cl, idx+o->metaObject()->methodOffset(), argv);
    }else{
        QtDynamicMetaObject* dynamicMetaObject = qtjambi_static_meta_contexts.at(dynamicMetaObjectIndex);
        if(dynamicMetaObject){
            dynamicMetaObject->invokeConstructor(qtjambi_current_environment(), idx, argv);
        }
    }
}

/**
 * If the java class provides a scriptable constructor, the meta object must provide meta information in a static context.
 * this is realized here as a substitute for the corresponding method generated by moc.
 */
template<int index>
static void static_meta_call(QObject * o, QMetaObject::Call cl, int idx, void ** argv) {
    //TODO solve how to access static members better than with template!
    generic_static_meta_call(index, o, cl, idx, argv);
}

QtDynamicMetaObjectPrivate::StaticMetaCallFunction QtDynamicMetaObjectPrivate::getStaticMetaCallFunctionPointer(){
    int index = qtjambi_static_meta_contexts.indexOf(this->q_func());
    if(index<0){
        index = qtjambi_static_meta_contexts.size();
        qtjambi_static_meta_contexts.append(this->q_func());
    }
	switch(index){
        case 0: return &static_meta_call<0>;
        case 1: return &static_meta_call<1>;
        case 2: return &static_meta_call<2>;
        case 3: return &static_meta_call<3>;
        case 4: return &static_meta_call<4>;
        case 5: return &static_meta_call<5>;
        case 6: return &static_meta_call<6>;
        case 7: return &static_meta_call<7>;
        case 8: return &static_meta_call<8>;
        case 9: return &static_meta_call<9>;
        case 10: return &static_meta_call<10>;
        case 11: return &static_meta_call<11>;
        case 12: return &static_meta_call<12>;
        case 13: return &static_meta_call<13>;
        case 14: return &static_meta_call<14>;
        case 15: return &static_meta_call<15>;
        case 16: return &static_meta_call<16>;
        case 17: return &static_meta_call<17>;
        case 18: return &static_meta_call<18>;
        case 19: return &static_meta_call<19>;
        case 20: return &static_meta_call<20>;
        case 21: return &static_meta_call<21>;
        case 22: return &static_meta_call<22>;
        case 23: return &static_meta_call<23>;
        case 24: return &static_meta_call<24>;
        case 25: return &static_meta_call<25>;
        case 26: return &static_meta_call<26>;
        case 27: return &static_meta_call<27>;
        case 28: return &static_meta_call<28>;
        case 29: return &static_meta_call<29>;
        case 30: return &static_meta_call<30>;
        case 31: return &static_meta_call<31>;
        case 32: return &static_meta_call<32>;
        case 33: return &static_meta_call<33>;
        case 34: return &static_meta_call<34>;
        case 35: return &static_meta_call<35>;
        case 36: return &static_meta_call<36>;
        case 37: return &static_meta_call<37>;
        case 38: return &static_meta_call<38>;
        case 39: return &static_meta_call<39>;
        case 40: return &static_meta_call<40>;
        case 41: return &static_meta_call<41>;
        case 42: return &static_meta_call<42>;
        case 43: return &static_meta_call<43>;
        case 44: return &static_meta_call<44>;
        case 45: return &static_meta_call<45>;
        case 46: return &static_meta_call<46>;
        case 47: return &static_meta_call<47>;
        case 48: return &static_meta_call<48>;
        case 49: return &static_meta_call<49>;
        case 50: return &static_meta_call<50>;
        case 51: return &static_meta_call<51>;
        case 52: return &static_meta_call<52>;
        case 53: return &static_meta_call<53>;
        case 54: return &static_meta_call<54>;
        case 55: return &static_meta_call<55>;
        case 56: return &static_meta_call<56>;
        case 57: return &static_meta_call<57>;
        case 58: return &static_meta_call<58>;
        case 59: return &static_meta_call<59>;
        case 60: return &static_meta_call<60>;
        case 61: return &static_meta_call<61>;
        case 62: return &static_meta_call<62>;
        case 63: return &static_meta_call<63>;
        case 64: return &static_meta_call<64>;
        case 65: return &static_meta_call<65>;
        case 66: return &static_meta_call<66>;
        case 67: return &static_meta_call<67>;
        case 68: return &static_meta_call<68>;
        case 69: return &static_meta_call<69>;
        case 70: return &static_meta_call<70>;
        case 71: return &static_meta_call<71>;
        case 72: return &static_meta_call<72>;
        case 73: return &static_meta_call<73>;
        case 74: return &static_meta_call<74>;
        case 75: return &static_meta_call<75>;
        case 76: return &static_meta_call<76>;
        case 77: return &static_meta_call<77>;
        case 78: return &static_meta_call<78>;
        case 79: return &static_meta_call<79>;
        case 80: return &static_meta_call<80>;
        case 81: return &static_meta_call<81>;
        case 82: return &static_meta_call<82>;
        case 83: return &static_meta_call<83>;
        case 84: return &static_meta_call<84>;
        case 85: return &static_meta_call<85>;
        case 86: return &static_meta_call<86>;
        case 87: return &static_meta_call<87>;
        case 88: return &static_meta_call<88>;
        case 89: return &static_meta_call<89>;
        case 90: return &static_meta_call<90>;
        case 91: return &static_meta_call<91>;
        case 92: return &static_meta_call<92>;
        case 93: return &static_meta_call<93>;
        case 94: return &static_meta_call<94>;
        case 95: return &static_meta_call<95>;
        case 96: return &static_meta_call<96>;
        case 97: return &static_meta_call<97>;
        case 98: return &static_meta_call<98>;
        case 99: return &static_meta_call<99>;
	}
	return 0;
}

void QtDynamicMetaObjectPrivate::initialize(JNIEnv *env, jclass java_class, const QMetaObject *original_meta_object)
{
    Q_Q(QtDynamicMetaObject);

    StaticCache *sc = StaticCache::instance();
    sc->resolveMetaObjectTools();

    env->PushLocalFrame(100);

    jobject meta_data_struct = env->CallStaticObjectMethod(sc->MetaObjectTools.class_ref, sc->MetaObjectTools.buildMetaData, java_class);
    qtjambi_exception_check(env);
    if (meta_data_struct == 0)
        return;

    sc->resolveMetaData();
    jintArray meta_data = (jintArray) env->GetObjectField(meta_data_struct, sc->MetaData.metaData);
    Q_ASSERT(meta_data);

    jobjectArray string_data = (jobjectArray) env->GetObjectField(meta_data_struct, sc->MetaData.stringData);
    Q_ASSERT(string_data);
    int number_of_strings = env->GetArrayLength(string_data);
    QByteArrayData* __stringdata = new QByteArrayData[number_of_strings];
    for(int i=0; i<number_of_strings; i++){
        jbyteArray string_data_entry = (jbyteArray)env->GetObjectArrayElement(string_data, i);
        int string_data_len = env->GetArrayLength(string_data_entry);
        char* stringdata = new char[string_data_len+1];
        stringdata[string_data_len] = '\0';
        env->GetByteArrayRegion(string_data_entry, 0, string_data_len, (jbyte *) stringdata);
        stringdataList.append(stringdata);
        __stringdata[i].ref.atomic._q_value = -1L;
        __stringdata[i].alloc = 0;
        __stringdata[i].capacityReserved = 0;
        __stringdata[i].size = string_data_len;
        __stringdata[i].offset = (qptrdiff)(void*)stringdata - (qptrdiff)(void*)&(__stringdata[i]);
    }
    q->d.stringdata = __stringdata;

    q->d.superdata = qtjambi_metaobject_for_class(env, env->GetSuperclass(java_class), original_meta_object);

    int meta_data_len = env->GetArrayLength(meta_data);
    q->d.data = new uint[meta_data_len];
    q->d.extradata = 0;
    q->d.relatedMetaObjects = 0;

    bool hasStaticMembers = env->GetBooleanField(meta_data_struct, sc->MetaData.hasStaticMembers);

    if(hasStaticMembers){
        q->d.static_metacall = getStaticMetaCallFunctionPointer();
    }else{
        q->d.static_metacall = 0;
    }

    env->GetIntArrayRegion(meta_data, 0, meta_data_len, (jint *) q->d.data);

    m_methods = (jobjectArray) env->GetObjectField(meta_data_struct, sc->MetaData.slotsArray);
    m_signals = (jobjectArray) env->GetObjectField(meta_data_struct, sc->MetaData.signalsArray);
    m_constructors = (jobjectArray) env->GetObjectField(meta_data_struct, sc->MetaData.constructorsArray);
    m_property_readers = (jobjectArray) env->GetObjectField(meta_data_struct, sc->MetaData.propertyReadersArray);
    m_property_writers = (jobjectArray) env->GetObjectField(meta_data_struct, sc->MetaData.propertyWritersArray);
    m_property_resetters = (jobjectArray) env->GetObjectField(meta_data_struct, sc->MetaData.propertyResettersArray);
    m_property_notifies = (jobjectArray) env->GetObjectField(meta_data_struct, sc->MetaData.propertyNotifiesArray);
    m_property_designable_resolvers = (jobjectArray) env->GetObjectField(meta_data_struct, sc->MetaData.propertyDesignableResolverArray);
    m_property_scriptable_resolvers = (jobjectArray) env->GetObjectField(meta_data_struct, sc->MetaData.propertyScriptableResolverArray);
    m_property_editable_resolvers = (jobjectArray) env->GetObjectField(meta_data_struct, sc->MetaData.propertyEditableResolverArray);
    m_property_stored_resolvers = (jobjectArray) env->GetObjectField(meta_data_struct, sc->MetaData.propertyStoredResolverArray);
    m_property_user_resolvers = (jobjectArray) env->GetObjectField(meta_data_struct, sc->MetaData.propertyUserResolverArray);
    jobjectArray extra_data = (jobjectArray) env->GetObjectField(meta_data_struct, sc->MetaData.extraDataArray);

    if (m_methods != 0) {
        m_methods = (jobjectArray) env->NewGlobalRef(m_methods);
        m_method_count = env->GetArrayLength(m_methods);
    }

    if (m_signals != 0) {
        m_signals = (jobjectArray) env->NewGlobalRef(m_signals);
        m_signal_count = env->GetArrayLength(m_signals);
    }

    if (m_constructors != 0) {
        m_constructors = (jobjectArray) env->NewGlobalRef(m_constructors);
        m_constructor_count = env->GetArrayLength(m_constructors);
    }

    if (m_method_count + m_signal_count + m_constructor_count > 0) {
        m_original_signatures = new QString[m_method_count + m_signal_count + m_constructor_count];
        jobjectArray original_signatures = (jobjectArray) env->GetObjectField(meta_data_struct, sc->MetaData.originalSignatures);
        for (int i=0; i<m_method_count + m_signal_count + m_constructor_count; ++i)
            m_original_signatures[i] = qtjambi_to_qstring(env, (jstring) env->GetObjectArrayElement(original_signatures, i));
    }


    if (m_property_readers != 0) {
        m_property_readers = (jobjectArray) env->NewGlobalRef(m_property_readers);
        m_property_count = env->GetArrayLength(m_property_readers);
    }

    if (m_property_writers != 0) {
        m_property_writers = (jobjectArray) env->NewGlobalRef(m_property_writers);
        Q_ASSERT(m_property_count == env->GetArrayLength(m_property_writers));
    }

    if (m_property_resetters != 0) {
        m_property_resetters = (jobjectArray) env->NewGlobalRef(m_property_resetters);
        Q_ASSERT(m_property_count == env->GetArrayLength(m_property_resetters));
    }

    if (m_property_notifies != 0) {
        m_property_notifies = (jobjectArray) env->NewGlobalRef(m_property_notifies);
        Q_ASSERT(m_property_count == env->GetArrayLength(m_property_notifies));
    }

    if (m_property_designable_resolvers != 0) {
        m_property_designable_resolvers = (jobjectArray) env->NewGlobalRef(m_property_designable_resolvers);
        Q_ASSERT(m_property_count == env->GetArrayLength(m_property_designable_resolvers));
    }

    if (m_property_scriptable_resolvers != 0) {
        m_property_scriptable_resolvers = (jobjectArray) env->NewGlobalRef(m_property_scriptable_resolvers);
        Q_ASSERT(m_property_count == env->GetArrayLength(m_property_scriptable_resolvers));
    }

    if (m_property_editable_resolvers != 0) {
        m_property_editable_resolvers = (jobjectArray) env->NewGlobalRef(m_property_editable_resolvers);
        Q_ASSERT(m_property_count == env->GetArrayLength(m_property_editable_resolvers));
    }

    if (m_property_stored_resolvers != 0) {
        m_property_stored_resolvers = (jobjectArray) env->NewGlobalRef(m_property_stored_resolvers);
        Q_ASSERT(m_property_count == env->GetArrayLength(m_property_stored_resolvers));
    }

    if (m_property_user_resolvers != 0) {
        m_property_user_resolvers = (jobjectArray) env->NewGlobalRef(m_property_user_resolvers);
        Q_ASSERT(m_property_count == env->GetArrayLength(m_property_user_resolvers));
    }

    int extra_data_count = extra_data != 0 ? env->GetArrayLength(extra_data) : 0;
    if (extra_data_count > 0) {
        // ensure to not have a pointer to a static_metacall method
        QMetaObject  const* * _relatedMetaObjects = new QMetaObject const*[extra_data_count];
        for (int i=0; i<extra_data_count; ++i) {
            _relatedMetaObjects[i] = qtjambi_metaobject_for_class(env, reinterpret_cast<jclass>(env->GetObjectArrayElement(extra_data, i)), 0);
        }
        q->d.relatedMetaObjects = _relatedMetaObjects;
    }

    env->PopLocalFrame(0);
}

void QtDynamicMetaObjectPrivate::invokeMethod(JNIEnv *env, jobject object, jobject method_object, void **_a, const QString &_signature, const QString &qtSignature) const
{
    StaticCache *sc = StaticCache::instance();
    sc->resolveMetaObjectTools();

    jobject method_signature = env->CallStaticObjectMethod(sc->MetaObjectTools.class_ref, sc->MetaObjectTools.methodSignature2, method_object, true);
    QTJAMBI_EXCEPTION_CHECK(env);
    Q_ASSERT(method_signature != 0);

    // If no signature is specified, we look it up
    QString signature(_signature);
    if (signature.isEmpty())
        signature = qtjambi_to_qstring(env, reinterpret_cast<jstring>(method_signature));
    QTJAMBI_EXCEPTION_CHECK(env);
    Q_ASSERT(!signature.isEmpty());

    QtJambiTypeManager manager(env, true, QtJambiTypeManager::DynamicMetaObjectMode);

    QVector<QString> externalTypeNames = manager.parseSignature(signature);
    QVector<void *> input_arguments(externalTypeNames.size() - 1, 0);
    for (int i=1; i<externalTypeNames.size(); ++i)
        input_arguments[i - 1] = _a[i];
    QVector<QString> internalTypeNames;
    if(qtSignature.isEmpty()){
        for (int i=0; i<externalTypeNames.size(); ++i){
            internalTypeNames << manager.getInternalTypeName(externalTypeNames[i], i == 0 ? QtJambiTypeManager::ReturnType : QtJambiTypeManager::ArgumentType);
        }
    }else{
        internalTypeNames = manager.parseSignature(qtSignature);
    }


    if(internalTypeNames.size()==externalTypeNames.size()){
        QVector<void *> converted_arguments = manager.initInternalToExternal(input_arguments, internalTypeNames, externalTypeNames);
        if (converted_arguments.size() > 0) {
            QVector<jvalue> jvArgs(converted_arguments.size() - 1);
            jvalue **data = reinterpret_cast<jvalue **>(converted_arguments.data());
            for (int i=1; i<converted_arguments.count(); ++i) {
                memcpy(&jvArgs[i - 1], data[i], sizeof(jvalue));
            }

            jvalue *args = jvArgs.data();
            jvalue *returned = reinterpret_cast<jvalue *>(converted_arguments[0]);

            jvalue dummy;
            if (returned == 0) {
                dummy.j = 0;
                returned = &dummy;
            }

            jmethodID id = env->FromReflectedMethod(method_object);
            QTJAMBI_EXCEPTION_CHECK(env);
            Q_ASSERT(id != 0);

            QString jni_type = QtJambiTypeManager::mangle(externalTypeNames.at(0));
            if (!jni_type.isEmpty()) {
                switch (jni_type.at(0).toLatin1()) {
                case 'V': returned->j = 0; env->CallVoidMethodA(object, id, args); break;
                case 'I': returned->i = env->CallIntMethodA(object, id, args); break;
                case 'J': returned->j = env->CallLongMethodA(object, id, args); break;
                case 'Z': returned->z = env->CallBooleanMethodA(object, id, args); break;
                case 'S': returned->s = env->CallShortMethodA(object, id, args); break;
                case 'B': returned->b = env->CallByteMethodA(object, id, args); break;
                case 'F': returned->f = env->CallFloatMethodA(object, id, args); break;
                case 'D': returned->d = env->CallDoubleMethodA(object, id, args); break;
                case 'C': returned->c = env->CallCharMethodA(object, id, args); break;
                case 'L': returned->l = env->CallObjectMethodA(object, id, args); break;
                default:
                    qWarning("QtDynamicMetaObject::invokeMethod: Unrecognized JNI type '%c'", jni_type.at(0).toLatin1());
                    break;
                };
            }
            QTJAMBI_EXCEPTION_CHECK(env);
            manager.convertExternalToInternal(converted_arguments.at(0), _a, externalTypeNames.at(0),
                manager.getInternalTypeName(externalTypeNames.at(0), QtJambiTypeManager::ReturnType), QtJambiTypeManager::ReturnType);
            QTJAMBI_EXCEPTION_CHECK(env);
            manager.destroyConstructedExternal(converted_arguments);
            QTJAMBI_EXCEPTION_CHECK(env);
        } else {
            qWarning("QtDynamicMetaObject::invokeMethod: Failed to convert arguments");
        }
    }else{
        qWarning()<< "QtDynamicMetaObject::invokeMethod: Failed to convert method types:";
        qWarning()<< "java signature" << signature;
        qWarning()<< "qt signature" << qtSignature;
        qWarning()<< "java types" << externalTypeNames;
        qWarning()<< "qt types" << internalTypeNames;
    }
}

/**
 * This method calls the constructor of a Java class caused by a meta object constructor call.
 */
void QtDynamicMetaObjectPrivate::invokeConstructor(JNIEnv *env, jobject constructor_object, void **_a, const QString &_signature, const QString &qtSignature) const
{
    StaticCache *sc = StaticCache::instance();
    sc->resolveMetaObjectTools();

    jobject constructor_signature = env->CallStaticObjectMethod(sc->MetaObjectTools.class_ref, sc->MetaObjectTools.methodSignature3, constructor_object);
    QTJAMBI_EXCEPTION_CHECK(env);
    Q_ASSERT(constructor_signature != 0);

    // If no signature is specified, we look it up
    QString signature(_signature);
    if (signature.isEmpty())
        signature = qtjambi_to_qstring(env, reinterpret_cast<jstring>(constructor_signature));
    QTJAMBI_EXCEPTION_CHECK(env);
    Q_ASSERT(!signature.isEmpty());

    QtJambiTypeManager manager(env, true, QtJambiTypeManager::DynamicMetaObjectMode);

    QVector<QString> externalTypeNames = manager.parseSignature(signature);
    jclass clazz = env->FindClass(m_clazz_name.toLatin1().constData());
    //QString className = qtjambi_class_name(env, m_clazz).replace('.', '/');
    externalTypeNames.replace(0, m_clazz_name);
    QVector<void *> input_arguments(externalTypeNames.size() - 1, 0);
    for (int i=1; i<externalTypeNames.size(); ++i)
        input_arguments[i - 1] = _a[i];
    QVector<QString> internalTypeNames;
    if(qtSignature.isEmpty()){
        for (int i=0; i<externalTypeNames.size(); ++i){
            internalTypeNames << manager.getInternalTypeName(externalTypeNames[i], i == 0 ? QtJambiTypeManager::ReturnType : QtJambiTypeManager::ArgumentType);
        }
    }else{
        internalTypeNames = manager.parseSignature(qtSignature);
    }


    if(internalTypeNames.size()==externalTypeNames.size()){
        QVector<void *> converted_arguments = manager.initInternalToExternal(input_arguments, internalTypeNames, externalTypeNames);
        if (converted_arguments.size() > 0) {
            QVector<jvalue> jvArgs(converted_arguments.size() - 1);
            jvalue **data = reinterpret_cast<jvalue **>(converted_arguments.data());
            for (int i=1; i<converted_arguments.count(); ++i) {
                memcpy(&jvArgs[i - 1], data[i], sizeof(jvalue));
            }

            jvalue *args = jvArgs.data();
            jvalue *returned = reinterpret_cast<jvalue *>(converted_arguments[0]);

            jvalue dummy;
            if (returned == 0) {
                dummy.j = 0;
                returned = &dummy;
            }

            jmethodID id = env->FromReflectedMethod(constructor_object);
            QTJAMBI_EXCEPTION_CHECK(env);
            Q_ASSERT(id != 0);

            returned->l = env->NewObjectA(clazz, id, args);
            QTJAMBI_EXCEPTION_CHECK(env);

            if(returned->l){
                env->NewGlobalRef(returned->l);
            }

            manager.convertExternalToInternal(converted_arguments.at(0), _a, externalTypeNames.at(0),
                manager.getInternalTypeName(externalTypeNames.at(0), QtJambiTypeManager::ReturnType), QtJambiTypeManager::ReturnType);
            QTJAMBI_EXCEPTION_CHECK(env);
            manager.destroyConstructedExternal(converted_arguments);
            QTJAMBI_EXCEPTION_CHECK(env);
        } else {
            qWarning("QtDynamicMetaObject::invokeConstructor: Failed to convert arguments");
        }
    }else{
        qWarning()<< "QtDynamicMetaObject::invokeConstructor: Failed to convert method types:";
        qWarning()<< "java signature" << signature;
        qWarning()<< "qt signature" << qtSignature;
        qWarning()<< "java types" << externalTypeNames;
        qWarning()<< "qt types" << internalTypeNames;
    }
}


QtDynamicMetaObject::QtDynamicMetaObject(JNIEnv *jni_env, jclass java_class, const QMetaObject *original_meta_object)
    : d_ptr(new QtDynamicMetaObjectPrivate(this, jni_env, java_class, original_meta_object)) {
    qRegisterMetaType<JEnumWrapper>("JEnumWrapper");
    qRegisterMetaType<JObjectWrapper>("JObjectWrapper");
}

QtDynamicMetaObject::~QtDynamicMetaObject()
{
    delete d_ptr;
}

QString QtDynamicMetaObject::javaClassName() const {
    Q_D(const QtDynamicMetaObject);
    return d->m_clazz_name;
}

int QtDynamicMetaObject::originalSignalOrSlotSignature(JNIEnv *env, int _id, QString *signature) const
{
    Q_D(const QtDynamicMetaObject);

    const QMetaObject *super_class = superClass();

    bool is_dynamic = qtjambi_metaobject_is_dynamic(super_class);
    if (is_dynamic) {
        _id = static_cast<const QtDynamicMetaObject *>(super_class)->originalSignalOrSlotSignature(env, _id, signature);
    } else {
        if (_id < super_class->methodCount()) {
            QString qt_signature = QLatin1String(super_class->className()) + QLatin1String("::") + QString::fromLatin1(super_class->method(_id).methodSignature());
            *signature = getJavaName(qt_signature.toLatin1());
        }
        _id -= super_class->methodCount();
    }
    if (_id < 0) return _id;

    if (_id < d->m_signal_count + d->m_method_count)
        *signature = d->m_original_signatures[_id];

    return _id - d->m_method_count - d->m_signal_count;
}

int QtDynamicMetaObject::invokeSignalOrSlot(JNIEnv *env, jobject object, int _id, void **_a) const
{
    Q_D(const QtDynamicMetaObject);

    const QMetaObject *super_class = superClass();
    if (qtjambi_metaobject_is_dynamic(super_class))
        _id = static_cast<const QtDynamicMetaObject *>(super_class)->invokeSignalOrSlot(env, object, _id, _a);
    if (_id < 0) return _id;

    // Emit the correct signal
    if (_id < d->m_signal_count) {
        jobject signal_field = env->GetObjectArrayElement(d->m_signals, _id);
        QTJAMBI_EXCEPTION_CHECK(env);
        Q_ASSERT(signal_field);

        jfieldID field_id = env->FromReflectedField(signal_field);
        QTJAMBI_EXCEPTION_CHECK(env);
        Q_ASSERT(field_id);

        jobject signal_object = env->GetObjectField(object, field_id);
        QTJAMBI_EXCEPTION_CHECK(env);
        Q_ASSERT(signal_object);

        StaticCache *sc = StaticCache::instance();
        sc->resolveQtJambiInternal();

        jobject signal_emit_method = env->CallStaticObjectMethod(sc->QtJambiInternal.class_ref, sc->QtJambiInternal.findEmitMethod, signal_object);
        qtjambi_exception_check(env);
        Q_ASSERT(signal_emit_method);

        jstring j_signal_parameters = static_cast<jstring>(env->CallStaticObjectMethod(sc->QtJambiInternal.class_ref,
                                                                                       sc->QtJambiInternal.signalParameters,
                                                                                       signal_object));
        qtjambi_exception_check(env);
        Q_ASSERT(j_signal_parameters);

        // Because of type erasure, we need to find the compile time signature of the emit method
        QString signal_parameters = "void emit(" + qtjambi_to_qstring(env, j_signal_parameters) + ")";
        QTJAMBI_EXCEPTION_CHECK(env);
        d->invokeMethod(env, signal_object, signal_emit_method, _a, signal_parameters);
    } else if (_id < d->m_signal_count + d->m_method_count) { // Call the correct method
        jobject method_object = env->GetObjectArrayElement(d->m_methods, _id - d->m_signal_count);
        QTJAMBI_EXCEPTION_CHECK(env);
        Q_ASSERT(method_object != 0);

        //here, the native method signature must be sumbitted
        QString native_signature;
        if(_id+this->methodOffset() < this->methodCount()){
            QMetaMethod _m = this->method(_id+this->methodOffset());
            native_signature += _m.typeName();
            native_signature += " ";
            native_signature += QLatin1String(_m.methodSignature());
        }
        d->invokeMethod(env, object, method_object, _a, QString(), native_signature);
    } else if (_id < d->m_signal_count + d->m_method_count + d->m_constructor_count) { // Call the correct constructor
        jobject method_object = env->GetObjectArrayElement(d->m_constructors, _id - d->m_signal_count - d->m_method_count);
        QTJAMBI_EXCEPTION_CHECK(env);
        Q_ASSERT(method_object != 0);

        //here, the native method signature must be sumbitted
        QString native_signature;
        if(_id+this->methodOffset() < this->methodCount()){
            QMetaMethod _m = this->method(_id+this->methodOffset());
            native_signature += _m.typeName();
            native_signature += " ";
            native_signature += QLatin1String(_m.methodSignature());
        }
        d->invokeConstructor(env, method_object, _a, QString(), native_signature);
    }
    QTJAMBI_EXCEPTION_CHECK(env);

    return _id - d->m_method_count - d->m_signal_count - d->m_constructor_count;
}

int QtDynamicMetaObject::invokeConstructor(JNIEnv *env, int _id, void **_a) const
{
    Q_D(const QtDynamicMetaObject);

    const QMetaObject *super_class = superClass();
    if (qtjambi_metaobject_is_dynamic(super_class))
        _id = static_cast<const QtDynamicMetaObject *>(super_class)->invokeConstructor(env, _id, _a);
    if (_id < 0) return _id;

    if (_id < d->m_signal_count + d->m_method_count + d->m_constructor_count) { // Call the correct constructor
        jobject constructor_object = env->GetObjectArrayElement(d->m_constructors, _id);
        QTJAMBI_EXCEPTION_CHECK(env);
        Q_ASSERT(constructor_object != 0);

        //here, the native method signature must be sumbitted
        QString native_signature;
        if(_id < this->constructorCount()){
            QMetaMethod _m = this->constructor(_id);
            native_signature += QLatin1String(_m.methodSignature());
        }
        d->invokeConstructor(env, constructor_object, _a, QString(), native_signature);
    }
    QTJAMBI_EXCEPTION_CHECK(env);
    return _id - d->m_constructor_count;
}

int QtDynamicMetaObject::readProperty(JNIEnv *env, jobject object, int _id, void **_a) const
{
    Q_D(const QtDynamicMetaObject);

    const QMetaObject *super_class = superClass();
    if (qtjambi_metaobject_is_dynamic(super_class))
        _id = static_cast<const QtDynamicMetaObject *>(super_class)->readProperty(env, object, _id, _a);
    if (_id < 0) return _id;

    if (_id < d->m_property_count) {
        jobject method_object = env->GetObjectArrayElement(d->m_property_readers, _id);
        qtjambi_exception_check(env);
        Q_ASSERT(method_object != 0);

        d->invokeMethod(env, object, method_object, _a);
    }

    return _id - d->m_property_count;
}

int QtDynamicMetaObject::writeProperty(JNIEnv *env, jobject object, int _id, void **_a) const
{
    Q_D(const QtDynamicMetaObject);

    const QMetaObject *super_class = superClass();
    if (qtjambi_metaobject_is_dynamic(super_class))
        _id = static_cast<const QtDynamicMetaObject *>(super_class)->writeProperty(env, object, _id, _a);
    if (_id < 0) return _id;

    if (_id < d->m_property_count) {
        jobject method_object = env->GetObjectArrayElement(d->m_property_writers, _id);
        qtjambi_exception_check(env);
        if (method_object != 0) {
            // invokeMethod expects a place holder for return value, but write property meta calls
            // do not since all property writers return void by convention.
            void *a[2] = { 0, _a[0] };
            d->invokeMethod(env, object, method_object, a);
        }
    }

    return _id - d->m_property_count;
}

int QtDynamicMetaObject::resetProperty(JNIEnv *env, jobject object, int _id, void **_a) const
{
    Q_D(const QtDynamicMetaObject);

    const QMetaObject *super_class = superClass();
    if (qtjambi_metaobject_is_dynamic(super_class))
        _id = static_cast<const QtDynamicMetaObject *>(super_class)->resetProperty(env, object, _id, _a);
    if (_id < 0) return _id;

    if (_id < d->m_property_count) {
        jobject method_object = env->GetObjectArrayElement(d->m_property_resetters, _id);
        qtjambi_exception_check(env);
        if (method_object != 0)
            d->invokeMethod(env, object, method_object, _a);
    }

    return _id - d->m_property_count;
}

int QtDynamicMetaObject::notifyProperty(JNIEnv *env, jobject object, int _id, void **_a) const
{
    Q_D(const QtDynamicMetaObject);

    const QMetaObject *super_class = superClass();
    if (qtjambi_metaobject_is_dynamic(super_class))
        _id = static_cast<const QtDynamicMetaObject *>(super_class)->notifyProperty(env, object, _id, _a);
    if (_id < 0) return _id;

    if (_id < d->m_property_count) {
        jobject signal_field = env->GetObjectArrayElement(d->m_property_notifies, _id);
        if (signal_field != 0){
            jfieldID field_id = env->FromReflectedField(signal_field);
            qtjambi_exception_check(env);
            Q_ASSERT(field_id);

            jobject signal_object = env->GetObjectField(object, field_id);
            qtjambi_exception_check(env);
            Q_ASSERT(signal_object);

            StaticCache *sc = StaticCache::instance();
            sc->resolveQtJambiInternal();

            jobject signal_emit_method = env->CallStaticObjectMethod(sc->QtJambiInternal.class_ref, sc->QtJambiInternal.findEmitMethod, signal_object);
            qtjambi_exception_check(env);
            Q_ASSERT(signal_emit_method);

            jstring j_signal_parameters = static_cast<jstring>(env->CallStaticObjectMethod(sc->QtJambiInternal.class_ref,
                                                                                           sc->QtJambiInternal.signalParameters,
                                                                                           signal_object));
            qtjambi_exception_check(env);
            Q_ASSERT(j_signal_parameters);

            // Because of type erasure, we need to find the compile time signature of the emit method
            QString signal_parameters = "void emit(" + qtjambi_to_qstring(env, j_signal_parameters) + ")";
            d->invokeMethod(env, signal_object, signal_emit_method, _a, signal_parameters);
        }
    }

    return _id - d->m_property_count;
}

int QtDynamicMetaObject::queryPropertyDesignable(JNIEnv *env, jobject object, int _id, void **_a) const
{
    Q_D(const QtDynamicMetaObject);

    const QMetaObject *super_class = superClass();
    if (qtjambi_metaobject_is_dynamic(super_class))
        _id = static_cast<const QtDynamicMetaObject *>(super_class)->queryPropertyDesignable(env, object, _id, _a);
    if (_id < 0) return _id;

    if (_id < d->m_property_count) {
        jobject method_object = env->GetObjectArrayElement(d->m_property_designable_resolvers, _id);
        if (method_object != 0)
            d->invokeMethod(env, object, method_object, _a);
    }

    return _id - d->m_property_count;
}

int QtDynamicMetaObject::queryPropertyScriptable(JNIEnv *env, jobject object, int _id, void **_a) const
{
    Q_D(const QtDynamicMetaObject);

    const QMetaObject *super_class = superClass();
    if (qtjambi_metaobject_is_dynamic(super_class))
        _id = static_cast<const QtDynamicMetaObject *>(super_class)->queryPropertyScriptable(env, object, _id, _a);
    if (_id < 0) return _id;

    if (_id < d->m_property_count) {
        jobject method_object = env->GetObjectArrayElement(d->m_property_scriptable_resolvers, _id);
        if (method_object != 0)
            d->invokeMethod(env, object, method_object, _a);
    }

    return _id - d->m_property_count;
}

int QtDynamicMetaObject::queryPropertyStored(JNIEnv *env, jobject object, int _id, void **_a) const
{
    Q_D(const QtDynamicMetaObject);

    const QMetaObject *super_class = superClass();
    if (qtjambi_metaobject_is_dynamic(super_class))
        _id = static_cast<const QtDynamicMetaObject *>(super_class)->queryPropertyStored(env, object, _id, _a);
    if (_id < 0) return _id;

    if (_id < d->m_property_count) {
        jobject method_object = env->GetObjectArrayElement(d->m_property_stored_resolvers, _id);
        if (method_object != 0)
            d->invokeMethod(env, object, method_object, _a);
    }

    return _id - d->m_property_count;
}

int QtDynamicMetaObject::queryPropertyUser(JNIEnv *env, jobject object, int _id, void **_a) const
{
    Q_D(const QtDynamicMetaObject);

    const QMetaObject *super_class = superClass();
    if (qtjambi_metaobject_is_dynamic(super_class))
        _id = static_cast<const QtDynamicMetaObject *>(super_class)->queryPropertyUser(env, object, _id, _a);
    if (_id < 0) return _id;

    if (_id < d->m_property_count) {
        jobject method_object = env->GetObjectArrayElement(d->m_property_user_resolvers, _id);
        if (method_object != 0)
            d->invokeMethod(env, object, method_object, _a);
    }

    return _id - d->m_property_count;
}

int QtDynamicMetaObject::queryPropertyEditable(JNIEnv *env, jobject object, int _id, void **_a) const
{
    Q_D(const QtDynamicMetaObject);

    const QMetaObject *super_class = superClass();
    if (qtjambi_metaobject_is_dynamic(super_class))
        _id = static_cast<const QtDynamicMetaObject *>(super_class)->queryPropertyEditable(env, object, _id, _a);
    if (_id < 0) return _id;

    if (_id < d->m_property_count) {
        jobject method_object = env->GetObjectArrayElement(d->m_property_editable_resolvers, _id);
        if (method_object != 0)
            d->invokeMethod(env, object, method_object, _a);
    }

    return _id - d->m_property_count;
}
