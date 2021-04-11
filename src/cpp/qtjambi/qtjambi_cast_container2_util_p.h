#ifndef QTJAMBI_CAST_CONTAINER2_UTIL_P_H
#define QTJAMBI_CAST_CONTAINER2_UTIL_P_H

#include "qtjambi_cast_util_p.h"

namespace QtJambiPrivate {

template<template<typename K, typename T> class Container, typename K, typename T>
struct IntermediateBiContainer : Container<K,T>{
    IntermediateBiContainer(JNIEnv *env, jobject object, QtJambiScope& scope) : Container<K,T>(), m_scope(scope), m_object(env->NewWeakGlobalRef(object)){}
    ~IntermediateBiContainer(){
        try{
            if(JNIEnv *env = qtjambi_current_environment()){
                QTJAMBI_JNI_LOCAL_FRAME(env, 200)
                try{
                    jobject object = env->NewLocalRef(m_object);
                    if(!env->IsSameObject(object, nullptr)){
                        qtjambi_map_clear(env, object);
                        for(typename Container<K,T>::const_iterator i = Container<K,T>::constBegin(); i!=Container<K,T>::constEnd(); ++i){
                            jobject key = qtjambi_scoped_cast<true,jobject,decltype(i.key())>::cast(env, i.key(), nullptr, &m_scope);
                            jobject val = qtjambi_scoped_cast<true,jobject,decltype(i.value())>::cast(env, i.value(), nullptr, &m_scope);
                            qtjambi_map_put(env, object, key, val);
                        }
                    }
                }catch(const JavaException& exn){
                    exn.raiseInJava(env);
                }
            }
        }catch(const std::exception& exn){
            printf("An exception occurred: %s\n", exn.what());
        }catch(...){
            printf("An unknown exception occurred.\n");
        }
    }
    QtJambiScope& m_scope;
    jobject m_object;
};

}

#endif // QTJAMBI_CAST_CONTAINER2_UTIL_P_H
